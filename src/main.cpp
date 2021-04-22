#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <iostream>
#include <map>
#include <sys/time.h>
#include <vector>
#include "MediusMessageParser.h"
#include "RTPacket.h"
#include "World.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
std::map<int, std::map<int, World *> *> apps;

unsigned long long getTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    return (unsigned long long) (tv.tv_sec) * 1000 + (unsigned long long) (tv.tv_usec) / 1000;
}

void *socketThread(void *arg) {
    int newSocket = *((int *) arg);

    int appId;
    int worldId;
    unsigned short clientIndex;
    unsigned short scertId;
    char sessionKey[17];
    char clientIP[16];
    World *thisWorld; // Needs synchronized access
    Player *thisPlayer; // Needs synchronized access


    bool running = true;

    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(newSocket, (struct sockaddr *) &addr, &addr_size);
    strcpy(clientIP, inet_ntoa(addr.sin_addr));

    while (running) {
        unsigned char command_bytes[1];
        unsigned char length_bytes[2];
        unsigned char message[2048];
        memset(message, 0, 2048);
        int totalRead = 0;

        memset(command_bytes, 0, 1);
        memset(length_bytes, 0, 2);
        memset(message, 0, 2048);

        int read = recv(newSocket, command_bytes, 1, 0);

        // todo put this in a function
        if (read == 0) {
            running = false;
            pthread_mutex_lock(&lock);
            delete thisPlayer;
            thisWorld->_gameSockets[clientIndex] = nullptr;
            pthread_mutex_unlock(&lock);
            break;
        }

        do {
            read = recv(newSocket, length_bytes, 2 - totalRead, 0);

            totalRead += read;

            if (read == 0) {
                running = false;
                pthread_mutex_lock(&lock);
                delete thisPlayer;
                thisWorld->_gameSockets[clientIndex] = nullptr;
                pthread_mutex_unlock(&lock);
                goto end;
            }
        } while(totalRead < 2);

        auto length = *(unsigned short *) length_bytes;
        auto command = *(unsigned char *) (command_bytes);

        totalRead = 0;
        do {
            read = recv(newSocket, message, length - totalRead, 0);

            totalRead += read;

            if (read == 0) {
                running = false;
                pthread_mutex_lock(&lock);
                delete thisPlayer;
                thisWorld->_gameSockets[clientIndex] = nullptr;
                pthread_mutex_unlock(&lock);
                goto end;
            }
        } while (totalRead < length);

        std::cout << clientIP << " sent message ";
        printf("%02hhx\n", command);

        switch (command) {
            case 36: {
                char packet_buffer[7] = {0x25, 0x04, 0x00, 0x6e, 0x00, 0x00, 0x00};
                send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
            }
                break;
            case 0: {
                auto *rt_packet = (RT_MSG_CLIENT_CONNECT_TCP *) message;
                worldId = rt_packet->WorldId;
                appId = rt_packet->AppId;
                strcpy(sessionKey, rt_packet->SessionKey);

                //Create world
                pthread_mutex_lock(&lock);
                std::map<int, World *> *app;
                try {
                    app = apps.at(appId);
                } catch (const std::out_of_range &oor) {
                    app = new std::map<int, World *>();
                    apps.insert({appId, app});
                }

                try {
                    thisWorld = app->at(worldId);
                } catch (const std::out_of_range &oor) {
                    thisWorld = new World();
                    thisWorld->_worldId = worldId;
                    // TODO set start time
                }

                thisWorld->_curClients += 1;
                auto retTuple = thisWorld->RegisterClientIndex(newSocket);
                clientIndex = std::get<0>(retTuple);
                if (clientIndex == -1) {
                    pthread_mutex_unlock(&lock);
                    running = false; // todo Handle error later
                    break;
                }

                thisPlayer = std::get<1>(retTuple);

                scertId = thisWorld->GenerateNewScertId();

                app->insert({thisWorld->_worldId, thisWorld});
                pthread_mutex_unlock(&lock);

                char packet_buffer[6] = {0x22, 0x03, 0x00, 0x02, 0x48, 0x02};
                send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
            }
                break;
            case 35: {
                RT_MSG_SERVER_CONNECT_ACCEPT_TCP acceptTcp;
                acceptTcp.clientIndex = clientIndex;
                acceptTcp.clientId = scertId;
                strcpy(acceptTcp.IP, clientIP);

                pthread_mutex_lock(&lock);
                std::map<int, World *> *app;
                try {
                    app = apps.at(appId);
                } catch (const std::out_of_range &oor) {
                    std::cout << "Could not find app" << std::endl;
                    pthread_mutex_unlock(&lock);
                    running = false;
                    break;
                }

                try {
                    thisWorld = app->at(worldId);
                } catch (const std::out_of_range &oor) {
                    std::cout << "Could not find world" << std::endl;
                    pthread_mutex_unlock(&lock);
                    running = false;
                    break;
                }

                acceptTcp.currentClients = thisWorld->_curClients;
                pthread_mutex_unlock(&lock);

                char packet_buffer[3 + sizeof(acceptTcp)];
                packet_buffer[0] = 7;
                unsigned short len = sizeof(RT_MSG_SERVER_CONNECT_ACCEPT_TCP);
                memcpy(&packet_buffer[1], &len, sizeof(unsigned short));
                memcpy(&packet_buffer[3], &acceptTcp, sizeof(RT_MSG_SERVER_CONNECT_ACCEPT_TCP));

                send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
            }
                break;
            case 33: {
                auto *rt_packet = (RT_MSG_CLIENT_CONNECT_READY_TCP *) message;

                pthread_mutex_lock(&lock);
                thisPlayer->SetFlags(rt_packet->recvFlags);
                thisPlayer->authTime = getTimeMillis();
                thisWorld->NotifyPlayerJoined(clientIndex, scertId, clientIP);

                if (!thisWorld->_startTime) {
                    thisWorld->_startTime = getTimeMillis();
                }

                int time = (int) (getTimeMillis() - thisWorld->_startTime);
                char packet_buffer[8] = {0x26, 0x05, 0x00, 0x04};
                memcpy(&packet_buffer[4], &time, sizeof(time));

                send(newSocket, packet_buffer, sizeof(packet_buffer), 0);

                unsigned char packet2_part1[5] = {0x1a, 0x02};
                unsigned char packet2_part2[11] = {0x1f, 0x08, 0x00, 0xa0, 0x18, 0x00, 0x00, 0xe9, 0xfb, 0x0c,
                                                   0x00};

                memcpy(&packet2_part1[3], &thisWorld->_curClients, sizeof(thisWorld->_curClients));

                unsigned char packet_buffer2[sizeof(packet2_part1) + sizeof(packet2_part2)];
                memcpy(&packet_buffer2, &packet2_part1, sizeof(packet2_part1));
                memcpy(&packet_buffer2[sizeof(packet2_part1)], &packet2_part2, sizeof(packet2_part2));

                send(newSocket, packet_buffer2, sizeof(packet_buffer2), 0);

                unsigned char packet_buffer3[21] = {0x0a, 0x12, 0x00, 0x00, 0x00, 0x32, 0x2e, 0x31, 0x30, 0x2e,
                                                    0x30,
                                                    0x30, 0x30, 0x39};
                send(newSocket, packet_buffer3, sizeof(packet_buffer3), 0);

                pthread_mutex_unlock(&lock);

                /*
                 if (sock.aux === true) {
                packet = Utils.AppendBytes(packet, Utils.HexToBytes('181200'));
                packet = Utils.AppendBytes(packet, Utils.FixedStringToBytes('192.168.1.93', 16));
                let bug = Buffer.alloc(2);
                bug.writeUInt16LE(10081);
                packet = Utils.AppendBytes(packet, bug);
                }
                 */
            }
                break;
            case 5: {
                auto packet_buffer = new unsigned char[length + 3];
                packet_buffer[0] = command;
                memcpy(&packet_buffer[1], &length, sizeof(unsigned short));
                memcpy(&packet_buffer[3], &message, length);
                send(newSocket, packet_buffer, length + 3, 0);
                delete[] packet_buffer;
            }
                break;
            case 13: {
                pthread_mutex_lock(&lock);

                thisPlayer->SetFlags(message[0]);

                pthread_mutex_unlock(&lock);
            }
                break;
            case 45: {
                unsigned int clientTime = *(unsigned int *) (message);
                pthread_mutex_lock(&lock);
                unsigned int curTime = getTimeMillis() - thisWorld->_startTime;
                pthread_mutex_unlock(&lock);

                char packet_buffer[11] = {0x2e, 0x08, 0x00};
                memcpy(&packet_buffer[3], &clientTime, sizeof(clientTime));
                memcpy(&packet_buffer[7], &curTime, sizeof(curTime));

                send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
            }
                break;
            case 2: {
                pthread_mutex_lock(&lock);
                thisWorld->BroadcastMessage(&message[0], clientIndex, length);
                pthread_mutex_unlock(&lock);
            }
                break;
            case 3: {
                unsigned short target;
                memcpy(&target, &message, sizeof(target));

                auto *payload = new unsigned char[length - 2];
                memcpy(payload, &message[2], length - 2);

                pthread_mutex_lock(&lock);
                thisWorld->SendTcpAppSingle(target, clientIndex, payload, length - 2);
                pthread_mutex_unlock(&lock);

                delete[] payload;
            }
                break;
            case 4: {
                unsigned char listsize;
                memcpy(&listsize, &message, sizeof(listsize));
                auto *mask = new unsigned char[listsize];
                memcpy(mask, &message[1], sizeof(listsize));
                std::vector<int> targets;
                for (unsigned char byte = 0; byte < listsize; byte++) {
                    for (char i = 0; i < 8; i++) {
                        if ((mask[byte] & (1 << i)) != 0)
                            targets.push_back(i + (byte * 8));
                    }
                }

                pthread_mutex_lock(&lock);
                thisWorld->SendTcpAppList(&message[1 + listsize], clientIndex, targets, length - 1 - listsize);
                pthread_mutex_unlock(&lock);

                delete[] mask;
            }
                break;
            case 11: {
                unsigned char packet_buffer[13] = {0x0a, 0x0a, 0x00, 0x00, 0x01, 0x90, 0x19, 0x7a, 0x0e, 0x00, 0x00, 0xff, 0x09};

                send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
            }
            break;
            case 32: {
                running = false;
                pthread_mutex_lock(&lock);
                delete thisPlayer;
                thisWorld->_gameSockets[clientIndex] = nullptr;
30,                 pthread_mutex_unlock(&lock);
            }
                break;
            case 31:
                break; //Server echo, ignore
            default: {
                printf("%02hhx\n", command);
                std::cout << length << std::endl;
                std::cout << "medius message hashedChars: " << std::endl;
                for (int i = 0; i < read; i++) {
                    printf("\\%02hhx", (unsigned char) message[i]);
                }
                std::cout << std::endl;
            }
                break;
        }

    }
    end:
    printf("Exit socketThread \n");
    close(newSocket);
    pthread_exit(nullptr);
}

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    //Create the socket.
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    // Configure settings of the server address struct
    // Address family = Internet
    serverAddr.sin_family = AF_INET;

    //Set port number, using htons function to use proper byte order
    serverAddr.sin_port = htons(10080);

    //Set IP address to localhost
    serverAddr.sin_addr.s_addr = inet_addr("192.168.1.93");


    //Set all bits of the padding field to 0
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Bind the address struct to the socket
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    //Listen on the socket, with 40 max connection requests queued
    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");
    pthread_t tid[60];
    int i = 0;
    while (1) {
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

        //for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
        if (pthread_create(&tid[i++], NULL, socketThread, &newSocket) != 0)
            printf("Failed to create thread\n");

        if (i >= 1000) { //1000 max threads = 1000 max users
            i = 0;
            while (i < 50) {
                pthread_join(tid[i++], NULL);
            }
            i = 0;
        }
    }
    return 0;
}
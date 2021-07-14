#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <iostream>
#include <map>
#include <sys/time.h>
#include <vector>
#include <netinet/tcp.h>
#include <unordered_map>
#include "../common/RTPacket.h"
#include "World.h"

//todo move everything to std::
//todo thread per udp socket(use connect(2))
//todo notify player left
//todo handle graceful disconnect
//todo handle brutal disconnect
//todo consider giving player objects pointers to their world object instead of the world id

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
std::map<int, std::map<int, World *> *> apps;

unsigned long long getTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    return (unsigned long long) (tv.tv_sec) * 1000 + (unsigned long long) (tv.tv_usec) / 1000;
}

void *udpSocketThread(void *arg) {
    int sockfd;
    char buffer[512];
    struct sockaddr_in servaddr, cliaddr;

    std::unordered_map<std::string, Player*> players;
    std::unordered_map<int, World*> worlds;

    bool running = true;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(10081);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
              sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int n;

    while (running) {
        int cliLen;

        n = recvfrom(sockfd, &buffer, 512,
                     MSG_WAITALL, (struct sockaddr *) &cliaddr,
                     reinterpret_cast<socklen_t *>(&cliLen));

        char cli_id[32];

        strcpy(cli_id, inet_ntoa(cliaddr.sin_addr));
        sprintf(&cli_id[strlen(cli_id)], "%d", cliaddr.sin_port);

        int i = 0;
        do {
            if (strcmp(cli_id, "0.0.0.00") == 0)
                break;

            uint8_t command;
            uint16_t commandLength;
            char message[512];

            command = buffer[0];
            commandLength = buffer[1];
            memcpy(&message, &buffer[3], commandLength);

            switch (command) {
                case 0x16:
                {
                    pthread_mutex_lock(&lock);

                    auto packet = (RT_MSG_CLIENT_CONNECT_AUX_UDP *)message;

                    std::map<int, World *> *app;
                    World *thisWorld; // Needs synchronized access
                    Player *thisPlayer; // Needs synchronized access

                    try {
                        app = apps.at(packet->ApplicationId);
                    } catch (const std::out_of_range &oor) {
                        std::cout << "Error: Failed to get UDP world." << std::endl;
                        return nullptr;
                    }

                    try {
                        if (players[cli_id] == nullptr) {
                            if (worlds[packet->WorldId] == nullptr)
                                worlds[packet->WorldId] = app->at(packet->WorldId);

                            worlds[packet->WorldId]->SetPlayerUdpSocket(packet->UNK_26, cliaddr);

                            players[cli_id] = worlds[packet->WorldId]->GetPlayerByScertId(packet->UNK_26);

                            players[cli_id]->CurWorldId = packet->WorldId;
                        }

                        thisPlayer = players[cli_id];
                    } catch (const std::out_of_range &oor) {
                        std::cout << "Error: Failed to get UDP world." << std::endl;
                        return nullptr;
                    }

                    RT_MSG_SERVER_CONNECT_ACCEPT_AUX_UDP udpAccept;

                    udpAccept.PlayerCount = app->at(packet->WorldId)->_curClients;
                    udpAccept.ScertId = packet->UNK_26;
                    udpAccept.PlayerId = thisPlayer->clientIndex;
                    udpAccept.port = cliaddr.sin_port;
                    strcpy(udpAccept.IP, inet_ntoa(cliaddr.sin_addr));

                    pthread_mutex_unlock(&lock);

                    char packet_buffer[3 + sizeof(RT_MSG_SERVER_CONNECT_ACCEPT_AUX_UDP)];
                    packet_buffer[0] = 0x19;
                    unsigned short len = sizeof(RT_MSG_SERVER_CONNECT_ACCEPT_AUX_UDP);
                    memcpy(&packet_buffer[1], &len, sizeof(unsigned short));
                    memcpy(&packet_buffer[3], &udpAccept, sizeof(RT_MSG_SERVER_CONNECT_ACCEPT_AUX_UDP));

                    sendto(sockfd, packet_buffer, 3 + sizeof(RT_MSG_SERVER_CONNECT_ACCEPT_AUX_UDP), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, cliLen);

                }
                break;
                case 0x05: {
                    auto packet_buffer = new unsigned char[commandLength + 3];
                    packet_buffer[0] = command;
                    memcpy(&packet_buffer[1], &commandLength, sizeof(unsigned short));
                    memcpy(&packet_buffer[3], &message, commandLength);
                    sendto(sockfd, packet_buffer, 3 + commandLength, MSG_CONFIRM, (const struct sockaddr *)&cliaddr, cliLen);
                    delete[] packet_buffer;
                } break;
                case 2: {
                    pthread_mutex_lock(&lock);
                    worlds[players[cli_id]->CurWorldId]->BroadcastUdpMessage(&message[0], players[cli_id]->clientIndex, commandLength, sockfd);
                    pthread_mutex_unlock(&lock);
                } break;
                case 3: {
                    unsigned short target;
                    memcpy(&target, &message, sizeof(target));

                    auto *payload = new char[commandLength - 2];
                    memcpy(payload, &message[2], commandLength - 2);

                    pthread_mutex_lock(&lock);
                    worlds[players[cli_id]->CurWorldId]->SendUdpAppSingle(target, players[cli_id]->clientIndex, payload, commandLength - 2, sockfd);
                    pthread_mutex_unlock(&lock);

                    delete[] payload;
                } break;
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
                    worlds[players[cli_id]->CurWorldId]->SendUdpAppList(&message[1 + listsize], players[cli_id]->clientIndex, targets, commandLength - 1 - listsize, sockfd);
                    pthread_mutex_unlock(&lock);

                    delete[] mask;
                } break;
                case 0x20:
                {
                    running = false;
                }
                break;
                default:
                    break;
            }

            i += commandLength + 3;
        } while (i < n);
    }
    /*sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
           len);
    printf("Hello message sent.\n");*/
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

    unsigned char command_bytes[1];
    unsigned char length_bytes[2];
    char message[2048];

    int totalRead = 0;
    int readState = 0; // 0 = command; 1 = length, 2 = message; 3 = process

    bool running = true;

    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(newSocket, (struct sockaddr *) &addr, &addr_size);
    strcpy(clientIP, inet_ntoa(addr.sin_addr));

    //todo move to own func
    int flags = fcntl(newSocket, F_GETFL, 0);
    if (flags != -1) {
        flags = (flags | O_NONBLOCK);
        //todo error check this
        fcntl(newSocket, F_SETFL, flags);
    }

    int disable = 1;
    int result = setsockopt(newSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &disable, sizeof(int));
    //todo error check if (result < 0)

    while (running) {
        int read;

        switch(readState) {
            case 0:
            {
                read = recv(newSocket, command_bytes, 1, 0);
                if (read > 0) {
                    totalRead += read;
                    if (totalRead == 1) {
                        readState = 1;
                        totalRead = 0;
                    }
                }
                break;
            }
            case 1:
            {
                read = recv(newSocket, length_bytes, 2 - totalRead, 0);
                if (read > 0) {
                    totalRead += read;
                    if (totalRead == 2) {
                        if (*(unsigned short *)length_bytes > 0)
                            readState = 2;
                        else
                            readState = 3;
                        totalRead = 0;
                    }
                }
                break;
            }
            case 2:
            {
                auto length = *(unsigned short *) length_bytes;
                read = recv(newSocket, message, length - totalRead, 0);
                if (read > 0) {
                    totalRead += read;
                    if (totalRead == length) {
                        readState = 3;
                        totalRead = 0;
                    }
                }
                break;
            }
            default:
            {
                //todo error and exit
                break;
            }
        }

        if (read >= 0 && readState == 3) {

            auto length = *(unsigned short *) length_bytes;
            auto command = *(unsigned char *) (command_bytes);

            std::cout << clientIP << " sent message ";
            printf("%02hhx\n", command);

            switch (command) {
                case 36: {
                    char packet_buffer[7] = {0x25, 0x04, 0x00, 0x6e, 0x00, 0x01, 0x00};
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

                    scertId = thisPlayer->scertId = thisWorld->GenerateNewScertId();

                    thisPlayer->CurWorldId = thisWorld->_worldId;

                    app->insert({thisWorld->_worldId, thisWorld});
                    pthread_mutex_unlock(&lock);

                    char packet_buffer[6] = {0x22, 0x03, 0x00, 0x02, 0x48, 0x02};
                    send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
                }
                    break;
                case 0x15: {
                    auto *rt_packet = (RT_MSG_CLIENT_CONNECT_TCP *) message;
                    worldId = rt_packet->WorldId;
                    appId = rt_packet->AppId;
                    strcpy(sessionKey, rt_packet->SessionKey);

                    RT_MSG_SERVER_CONNECT_ACCEPT_TCP acceptTcp;
                    strcpy(acceptTcp.IP, clientIP);

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

                    scertId = thisPlayer->scertId = thisWorld->GenerateNewScertId();

                    acceptTcp.clientIndex = clientIndex;
                    acceptTcp.clientId = scertId;

                    acceptTcp.currentClients = thisWorld->_curClients;

                    app->insert({thisWorld->_worldId, thisWorld});
                    pthread_mutex_unlock(&lock);

                    char packet_buffer[3 + sizeof(acceptTcp)];
                    packet_buffer[0] = 7;
                    unsigned short len = sizeof(RT_MSG_SERVER_CONNECT_ACCEPT_TCP);
                    memcpy(&packet_buffer[1], &len, sizeof(unsigned short));
                    memcpy(&packet_buffer[3], &acceptTcp, sizeof(RT_MSG_SERVER_CONNECT_ACCEPT_TCP));

                    send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
                }
                    break;
                case 0x21: {
                    auto *rt_packet = (RT_MSG_CLIENT_CONNECT_READY_TCP *) message;

                    pthread_mutex_lock(&lock);
                    thisPlayer->SetFlags(rt_packet->recvFlags);
                    thisPlayer->authTime = getTimeMillis();

                    if (!thisWorld->_startTime) {
                        thisWorld->_startTime = getTimeMillis();
                    }

                    int time = (int) (getTimeMillis() - thisWorld->_startTime);
                    char packet_buffer[8] = {0x26, 0x05, 0x00, 0x04};
                    memcpy(&packet_buffer[4], &time, sizeof(time));

                    send(newSocket, packet_buffer, sizeof(packet_buffer), 0);

                    RT_MSG_SERVER_INFO_AUX_UDP auxUdp;

                    memset(&auxUdp, 0, sizeof(RT_MSG_SERVER_INFO_AUX_UDP));

                    strcpy(auxUdp.IP, "192.168.1.93");
                    auxUdp.port = 10081;

                    char packet_buffer2[3 + sizeof(RT_MSG_SERVER_INFO_AUX_UDP)];
                    packet_buffer2[0] = 0x18;
                    unsigned short len = sizeof(RT_MSG_SERVER_INFO_AUX_UDP);
                    memcpy(&packet_buffer2[1], &len, sizeof(unsigned short));
                    memcpy(&packet_buffer2[3], &auxUdp, sizeof(RT_MSG_SERVER_INFO_AUX_UDP));

                    send(newSocket, packet_buffer2, sizeof(packet_buffer2), 0);

                    /*unsigned char packet2_part1[5] = {0x1a, 0x02};
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
                    send(newSocket, packet_buffer3, sizeof(packet_buffer3), 0);*/

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
                    thisPlayer->SetFlags(message[0]);
                }
                    break;
                case 45: {
                    unsigned int clientTime = *(unsigned int *) (message);
                    unsigned int curTime = getTimeMillis() - thisWorld->_startTime;

                    char packet_buffer[11] = {0x2e, 0x08, 0x00};
                    memcpy(&packet_buffer[3], &clientTime, sizeof(clientTime));
                    memcpy(&packet_buffer[7], &curTime, sizeof(curTime));

                    send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
                }
                    break;
                case 2: {
                    //pthread_mutex_lock(&lock);
                    thisWorld->BroadcastMessage(&message[0], clientIndex, length);
                    //pthread_mutex_unlock(&lock);
                }
                    break;
                case 3: {
                    unsigned short target;
                    memcpy(&target, &message, sizeof(target));

                    auto *payload = new char[length - 2];
                    memcpy(payload, &message[2], length - 2);

                    //pthread_mutex_lock(&lock);
                    thisWorld->SendTcpAppSingle(target, clientIndex, payload, length - 2);
                    //pthread_mutex_unlock(&lock);

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

                    //pthread_mutex_lock(&lock);
                    thisWorld->SendTcpAppList(&message[1 + listsize], clientIndex, targets, length - 1 - listsize);
                    //pthread_mutex_unlock(&lock);

                    delete[] mask;
                }
                    break;
                case 0x17: {
                    pthread_mutex_lock(&lock);
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
                    //send(newSocket, packet_buffer3, sizeof(packet_buffer3), 0);

                    thisWorld->NotifyPlayerJoined(clientIndex, scertId, clientIP);

                    pthread_mutex_unlock(&lock);
                }
                break;
                case 11: {
                    unsigned char packet_buffer[13] = {0x0a, 0x0a, 0x00, 0x00, 0x01, 0x90, 0x19, 0x7a, 0x0e, 0x00, 0x00,
                                                       0xff, 0x09};

                    send(newSocket, packet_buffer, sizeof(packet_buffer), 0);
                }
                    break;
                case 32: {
                    running = false;
                    pthread_mutex_lock(&lock);
                    delete thisPlayer;
                    thisWorld->_gameSockets[clientIndex] = nullptr;
                    pthread_mutex_unlock(&lock);
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

            readState = 0;

        } else {
            // todo put this in a function
            if (read == 0) {
                running = false;
                pthread_mutex_lock(&lock);
                delete thisPlayer;
                thisWorld->_gameSockets[clientIndex] = nullptr;
                pthread_mutex_unlock(&lock);
                break;
            }
        }

        //if (thisPlayer != nullptr)
        //    thisPlayer->ProcessQueue();
    }
    end:
    printf("Exit socketThread \n");
    close(newSocket);
    pthread_exit(nullptr);
}

//todo refactor all of this
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
    int retbind = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (retbind != 0) {
        std::cout << "Failed to bind tcp socket: " << retbind << std::endl;
        return 1;
    }

    //Listen on the socket, with 40 max connection requests queued
    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");
    pthread_t tid[60];

    pthread_t udpThread;

    if (pthread_create(&udpThread, NULL, udpSocketThread, nullptr) != 0)
        printf("Failed to create udp thread\n");

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
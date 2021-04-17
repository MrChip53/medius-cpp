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
#include "MediusMessageParser.h"

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *socketThread(void *arg) {
    int newSocket = *((int *) arg);

    bool running = true;

    while (running) {
        int read = recv(newSocket, client_message, 2000, 0);

        std::cout << "hashedChars: " << std::endl;
        for (int i = 0; i < read; i++) {
            printf("\\%02hhx", (unsigned char) client_message[i]);
        }
        std::cout << std::endl;

        auto message = new MediusMessageParser(client_message);

        std::cout << message->command << std::endl;
        std::cout << message->length << std::endl;

        std::cout << "hashedChars: " << std::endl;
        for (int i = 0; i < read; i++) {
            printf("\\%02hhx", (unsigned char) message->mediusMessage[i]);
        }
        std::cout << std::endl;

        switch (message->command) {
            case 36: {
                char buffer[7] = {0x25, 0x04, 0x00, 0x6e, 0x00, 0x00, 0x00};
                send(newSocket,buffer,sizeof(buffer),0);
            }
                break;
            case 32:
                running = false;
                break;
            default: {

            }
                break;
        }

        /*//std::cout << message->mediusMessage << std::endl;

        // Send message to the client socket
        pthread_mutex_lock(&lock);

        pthread_mutex_unlock(&lock);
        sleep(1);
        //send(newSocket,buffer,13,0);*/
    }
    printf("Exit socketThread \n");
    close(newSocket);
    pthread_exit(NULL);
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

        if (i >= 50) {
            i = 0;
            while (i < 50) {
                pthread_join(tid[i++], NULL);
            }
            i = 0;
        }
    }
    return 0;
}
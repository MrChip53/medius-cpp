//
// Created by chris on 4/17/21.
//

#ifndef LIBUV_WORLD_H
#define LIBUV_WORLD_H


#include <map>
#include <cstring>
#include <sys/socket.h>
#include <vector>
#include <iostream>
#include "RTPacket.h"

typedef struct {
    char message[512];
    int length;
} Message;

class Player {
public:
    int socket;
    unsigned short clientIndex;
    unsigned char recvFlags;
    unsigned long long authTime;
    std::vector<Message*> messageQueue;

    pthread_mutex_t messageQLock;

    Player(int sock, unsigned short clientIndex) {
        this->socket = sock;
        this->clientIndex = clientIndex;
        pthread_mutex_init(&messageQLock, NULL);
    }

    void QueueMessage(Message *message) {
        pthread_mutex_lock(&messageQLock);

        messageQueue.push_back(message);

        pthread_mutex_unlock(&messageQLock);
    }

    void ProcessQueue() {
        pthread_mutex_lock(&messageQLock);

        for(auto & message : messageQueue) {
            send(socket, message->message, message->length, 0);
            free(message);
        }

        messageQueue.clear();

        pthread_mutex_unlock(&messageQLock);
    }

    void SetFlags(unsigned char flags) {
        this->recvFlags = flags;
        std::cout << "Recv flags updated: " << this->recvFlags << ", Notif: " << this->HasFlag(8) << ", Broadcast: "
            << this->HasFlag(1) << ", App single: " << this->HasFlag(4) << ", App list: " << this->HasFlag(2) << std::endl;
    }

    bool HasFlag(int flag) {
        if ((this->recvFlags & flag) == flag)
            return true;
        return false;
    }
};

class World {
public:
    int _worldId;
    unsigned long long _startTime;
    unsigned short _curClients = 0;
    unsigned short _totalClients = 0;
    std::array<Player*, 256> _gameSockets;
    std::array<pthread_mutex_t, 256> _playerMutex;

    std::tuple<unsigned short, Player*> RegisterClientIndex(int socket) {
        //todo change the hardcoded 16
        for (unsigned short i = 0; i < 16; i++) {
            if (!this->_gameSockets[i]) {
                this->_gameSockets[i] = new Player(socket, i);
                return std::make_tuple(i, this->_gameSockets[i]);
            }
        }
        return std::make_tuple(-1, nullptr);
    }

    unsigned short GenerateNewScertId() {
        this->_totalClients += 1;
        unsigned short sceId = this->_totalClients;
        return sceId;
    }

    void NotifyPlayerJoined(int clientIndex, int scertId, char* IP) {
        RT_MSG_SERVER_CONNECT_NOTIFY rt_packet;
        rt_packet.clientIndex = clientIndex;
        rt_packet.scertId = scertId;
        strcpy(rt_packet.IP, IP);

        rt_packet.UNK01 = 0;

        memset(rt_packet.RSAKey, 0, 64);

        char packet_buffer[512];
        packet_buffer[0] = 8;
        unsigned short len = sizeof(RT_MSG_SERVER_CONNECT_NOTIFY);
        memcpy(&packet_buffer[1], &len, sizeof(unsigned short));
        memcpy(&packet_buffer[3], &rt_packet, sizeof(rt_packet));

        auto message = (Message *)(malloc(sizeof(Message)));
        memcpy(message->message, packet_buffer, len + 3);
        message->length = len + 3;

        //todo change all these for loops to be < total online
        for(const auto& player : this->_gameSockets) {
            //todo remove hardcoded flag value
            if (player == nullptr)
                continue;
            if (player->clientIndex == clientIndex || !player->authTime)
                continue;
            if (!player->HasFlag(8))
                continue;
            player->QueueMessage(message);
        }
    }

    void BroadcastMessage(const char *message, unsigned short sourceIndex, unsigned short length) {
        int newLength = length + 2;

        char packet_buffer[512];

        packet_buffer[0] = 0x03;
        memcpy(&packet_buffer[1], &newLength, sizeof(unsigned short));
        memcpy(&packet_buffer[3], &sourceIndex, sizeof(unsigned short));
        memcpy(&packet_buffer[5], message, length);

        Message *rt_message = (Message *)malloc(sizeof(Message));
        memcpy(rt_message->message, packet_buffer, newLength + 3);
        rt_message->length = newLength + 3;

        for(const auto& player : this->_gameSockets) {
            //todo remove hardcoded flag value
            if (player == nullptr || player->clientIndex == sourceIndex || !player->authTime || (player->recvFlags & 1) == 0)
                continue;
            std::cout << "Sent broadcast message out to client " << player->clientIndex << std::endl;
            player->QueueMessage(rt_message);
        }
    }

    void SendTcpAppSingle(unsigned short targetIndex, unsigned short sourceIndex, const char *message, unsigned short length) {
        char packet_buffer[512];

        unsigned short newLength = length + 2;

        packet_buffer[0] = 0x03;
        memcpy(&packet_buffer[1], &newLength, sizeof(unsigned short));
        memcpy(&packet_buffer[3], &sourceIndex, sizeof(unsigned short));
        memcpy(&packet_buffer[5], message, length);

        Message *rt_message = (Message *)malloc(sizeof(Message));
        memcpy(rt_message->message, packet_buffer, newLength + 3);
        rt_message->length = newLength + 3;

        //todo remove hardcoded flag value, set player into player variable
        if (this->_gameSockets[targetIndex] != nullptr || !this->_gameSockets[targetIndex]->authTime || this->_gameSockets[targetIndex]->clientIndex == sourceIndex || (this->_gameSockets[targetIndex]->recvFlags & 4) == 0)
            this->_gameSockets[targetIndex]->QueueMessage(rt_message);
    }

    void SendTcpAppList(const char *message, unsigned short sourceIndex, std::vector<int> targets, unsigned short length) {
        int newLength = length + 2;

        char packet_buffer[512];

        packet_buffer[0] = 0x03;
        memcpy(&packet_buffer[1], &newLength, sizeof(unsigned short));
        memcpy(&packet_buffer[3], &sourceIndex, sizeof(unsigned short));
        memcpy(&packet_buffer[5], message, length);

        Message *rt_message = (Message *)malloc(sizeof(Message));
        memcpy(rt_message->message, packet_buffer, newLength + 3);
        rt_message->length = newLength + 3;

        for(const auto& player : this->_gameSockets) {
            //todo remove hardcoded flag value
            if (player == nullptr || player->clientIndex == sourceIndex || !player->authTime || std::find(targets.begin(), targets.end(), player->clientIndex) == targets.end() || (player->recvFlags & 2) == 0)
                continue;
            player->QueueMessage(rt_message);
        }
    }
};


#endif //LIBUV_WORLD_H

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

class Player {
public:
    int socket;
    unsigned short clientIndex;
    unsigned char recvFlags;
    unsigned long long authTime;

    Player(int sock, unsigned short clientIndex) {
        this->socket = sock;
        this->clientIndex = clientIndex;
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

        //todo change all these for loops to be < total online
        for(const auto& player : this->_gameSockets) {
            //todo remove hardcoded flag value
            if (player == nullptr)
                continue;
            if (player->clientIndex == clientIndex || !player->authTime)
                continue;
            if (!player->HasFlag(8))
                continue;
            send(player->socket, packet_buffer, len + 3, 0);
        }
    }

    void BroadcastMessage(const unsigned char *message, unsigned short sourceIndex, unsigned short length) {
        int newLength = length + 2;

        unsigned char packet_buffer[512];

        packet_buffer[0] = 0x03;
        memcpy(&packet_buffer[1], &newLength, sizeof(unsigned short));
        memcpy(&packet_buffer[3], &sourceIndex, sizeof(unsigned short));
        memcpy(&packet_buffer[5], message, length);

        for(const auto& player : this->_gameSockets) {
            //todo remove hardcoded flag value
            if (player == nullptr || player->clientIndex == sourceIndex || !player->authTime || (player->recvFlags & 1) == 0)
                continue;
            std::cout << "Sent broadcast message out to client " << player->clientIndex << std::endl;
            send(player->socket, packet_buffer, newLength + 3, 0);
        }
    }

    void SendTcpAppSingle(unsigned short targetIndex, unsigned short sourceIndex, const unsigned char *message, unsigned short length) {
        unsigned char packet_buffer[512];

        unsigned short newLength = length + 2;

        packet_buffer[0] = 0x03;
        memcpy(&packet_buffer[1], &newLength, sizeof(unsigned short));
        memcpy(&packet_buffer[3], &sourceIndex, sizeof(unsigned short));
        memcpy(&packet_buffer[5], message, length);

        printf("Target: %d\n", sourceIndex);
        std::cout << "Length: " << length << std::endl;
        std::cout << "o medius message hashedChars:    " << std::endl;
        for (int i = 0; i < length; i++) {
            printf("\\%02hhx", (unsigned char) message[i]);
        }
        std::cout << std::endl;

        std::cout << "new medius message hashedChars: " << std::endl;
        for (int i = 0; i < newLength + 3; i++) {
            printf("\\%02hhx", (unsigned char) packet_buffer[i]);
        }
        std::cout << std::endl;

        std::cout << "Length: " << newLength << std::endl;

        //todo remove hardcoded flag value
        if (this->_gameSockets[targetIndex] != nullptr || !this->_gameSockets[targetIndex]->authTime || this->_gameSockets[targetIndex]->clientIndex == sourceIndex || (this->_gameSockets[targetIndex]->recvFlags & 4) == 0)
            send(this->_gameSockets[targetIndex]->socket, packet_buffer, newLength + 3, 0);
    }

    void SendTcpAppList(const unsigned char *message, unsigned short sourceIndex, std::vector<int> targets, unsigned short length) {
        int newLength = length + 2;

        char packet_buffer[512];

        packet_buffer[0] = 0x03;
        memcpy(&packet_buffer[1], &newLength, sizeof(unsigned short));
        memcpy(&packet_buffer[3], &sourceIndex, sizeof(unsigned short));
        memcpy(&packet_buffer[5], message, length);

        for(const auto& player : this->_gameSockets) {
            //todo remove hardcoded flag value
            if (player == nullptr || player->clientIndex == sourceIndex || !player->authTime || std::find(targets.begin(), targets.end(), player->clientIndex) == targets.end() || (player->recvFlags & 2) == 0)
                continue;
            send(player->socket, packet_buffer, newLength + 3, 0);
        }
    }
};


#endif //LIBUV_WORLD_H

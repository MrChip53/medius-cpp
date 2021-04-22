//
// Created by chris on 4/17/21.
//

#ifndef LIBUV_RTPACKET_H
#define LIBUV_RTPACKET_H

typedef struct {
    unsigned int WorldId;
    int AppId;
    char RSAKey[64];
    char SessionKey[17];
    char AccessKey[17];
} RT_MSG_CLIENT_CONNECT_TCP;

typedef struct {
    unsigned short clientIndex;
    int clientId;
    unsigned short currentClients;
    char IP[16];
} __attribute__((packed)) RT_MSG_SERVER_CONNECT_ACCEPT_TCP;

typedef struct {
    char startOp;
    unsigned char recvFlags;
} RT_MSG_CLIENT_CONNECT_READY_TCP;

typedef struct {
    unsigned short clientIndex;
    unsigned short scertId;
    unsigned short UNK01;
    char IP[16];
    char RSAKey[64];
} RT_MSG_SERVER_CONNECT_NOTIFY;

#endif //LIBUV_RTPACKET_H

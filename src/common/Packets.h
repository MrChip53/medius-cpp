//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_PACKETS_H
#define MEDIUS_MUIS_SERVER_PACKETS_H


class Packets {
public:
    typedef struct {
        uint8_t serverVersion;
        uint8_t unk01;
        uint8_t encrypt;
        uint8_t unk02;
    } RT_MSG_SERVER_HELLO;

    typedef struct {
        uint32_t TargetWorldId;
        int32_t AppId;
        uint8_t RSAKey[64];
    } RT_MSG_CLIENT_CONNECT_TCP;

    typedef struct {
        uint32_t TargetWorldId;
        int32_t AppId;
        uint8_t RSAKey[64];
        char SessionKey[17];
        char AccessKey[17];
    } RT_MSG_CLIENT_CONNECT_TCP_EXTENDED;
};


#endif //MEDIUS_MUIS_SERVER_PACKETS_H

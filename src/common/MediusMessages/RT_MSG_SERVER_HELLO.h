//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

#include "../Packets.h"
#include "../../dme/MediusMessageParser.h"

class RT_MSG_SERVER_HELLO {

public:
    const static uint8_t Request = 0x24;
    const static uint8_t Response = 0x25;

    static struct iovec process(MediusHandler::MediusMessage data, UserData uData) {
        iovec iov;

        void* recvPacket;
        bool extendedPacket = false;

        if (data.length > sizeof(Packets::RT_MSG_CLIENT_CONNECT_TCP)) {
            extendedPacket = true;
            recvPacket = new Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED();
            memcpy(recvPacket, data.mediusMessage, sizeof(Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED));
        } else {
            recvPacket = new Packets::RT_MSG_CLIENT_CONNECT_TCP();
            memcpy(recvPacket, data.mediusMessage, sizeof(Packets::RT_MSG_CLIENT_CONNECT_TCP));
        }

        Packets::RT_MSG_SERVER_HELLO *packet = static_cast<Packets::RT_MSG_SERVER_HELLO *>(malloc(sizeof(Packets::RT_MSG_SERVER_HELLO)));
        memset(packet, 0, sizeof(Packets::RT_MSG_SERVER_HELLO));
        packet->encrypt = 0;
        packet->serverVersion = 0x6e;

        iov.iov_len = sizeof(Packets::RT_MSG_SERVER_HELLO) + 3;
        char *buffer = new char[sizeof(Packets::RT_MSG_SERVER_HELLO) + 3];
        buffer[0] = Response;
        buffer[1] = (uint16_t)sizeof(Packets::RT_MSG_SERVER_HELLO);
        memcpy(&buffer[3], packet, sizeof(Packets::RT_MSG_SERVER_HELLO));
        free(packet);
        delete recvPacket;
        iov.iov_base = buffer;
        return iov;
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_ACCEPT_TCP_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_ACCEPT_TCP_H

#include "../Packets.h"

class RT_MSG_SERVER_CONNECT_ACCEPT_TCP {
public:
    const static uint8_t Request = 0x00;
    const static uint8_t Response = 0x07;

    static struct iovec process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
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

        Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP *packet = static_cast<Packets::RT_MSG_SERVER_HELLO *>(malloc(sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP)));
        memset(packet, 0, sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP));
        packet->CurClients = 0x01;

        iov.iov_len = sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP) + 3;
        char *buffer = new char[sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP) + 3];
        buffer[0] = Response;
        buffer[1] = (uint16_t)sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP);
        memcpy(&buffer[3], packet, sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP));
        free(packet);
        delete recvPacket;
        iov.iov_base = buffer;
        return iov;
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_ACCEPT_TCP_H

//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

#include "../Packets.h"

class RT_MSG_SERVER_HELLO {


public:
    const static int Request = 0x24;

    static struct iovec process(char* data) {
        iovec iov;

        Packets::RT_MSG_SERVER_HELLO *packet = static_cast<Packets::RT_MSG_SERVER_HELLO *>(malloc(sizeof(Packets::RT_MSG_SERVER_HELLO)));
        memset(packet, 0, sizeof(Packets::RT_MSG_SERVER_HELLO));
        packet->encrypt = 0;
        packet->serverVersion = 0x6e;

        iov.iov_len = sizeof(Packets::RT_MSG_SERVER_HELLO) + 3;
        char *buffer = new char[sizeof(Packets::RT_MSG_SERVER_HELLO) + 3];
        buffer[0] = 0x25;
        buffer[1] = (uint16_t)sizeof(Packets::RT_MSG_SERVER_HELLO);
        memcpy(&buffer[3], packet, sizeof(Packets::RT_MSG_SERVER_HELLO));
        free(packet);
        iov.iov_base = buffer;
        return iov;
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

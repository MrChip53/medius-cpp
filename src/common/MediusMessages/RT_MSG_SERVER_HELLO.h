//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

class RT_MSG_SERVER_HELLO {
    typedef struct {
        uint8_t serverVersion;
        uint8_t unk01;
        uint8_t encrypt;
        uint8_t unk02;
    } RT_MSG_SERVER_HELLO_S;

public:
    const static int Request = 0x24;

    static struct iovec process(char* data) {
        iovec iov;

        RT_MSG_SERVER_HELLO_S *packet = static_cast<RT_MSG_SERVER_HELLO_S *>(malloc(sizeof(RT_MSG_SERVER_HELLO_S)));
        memset(packet, 0, sizeof(RT_MSG_SERVER_HELLO_S));
        packet->encrypt = 0;
        packet->serverVersion = 0x6e;

        iov.iov_len = sizeof(RT_MSG_SERVER_HELLO_S) + 3;
        char *buffer = new char[sizeof(RT_MSG_SERVER_HELLO_S) + 3];
        buffer[0] = 0x25;
        buffer[1] = (uint16_t)sizeof(RT_MSG_SERVER_HELLO_S);
        memcpy(&buffer[3], &packet, sizeof(RT_MSG_SERVER_HELLO_S));
        free(packet);
        iov.iov_base = buffer;
        return iov;
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

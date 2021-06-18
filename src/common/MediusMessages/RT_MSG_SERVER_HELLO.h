//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

class RT_MSG_SERVER_HELLO {
public:
    struct RT_MSG_SERVER_HELLO_S {
        uint8_t serverVersion = 0x6e;
        uint8_t unk01 = 0x00;
        uint8_t encrypt = 0x01;
        uint8_t unk02 = 0x00;
    };

    static struct iovec process(char* data) {
        iovec iov;
        RT_MSG_SERVER_HELLO_S* ret = static_cast<RT_MSG_SERVER_HELLO_S *>(malloc(sizeof(RT_MSG_SERVER_HELLO_S)));
        ret->serverVersion = 0x6e;
        ret->unk01 = 0x00;
        ret->unk02 = 0x00;
        ret->encrypt = 0x01;
        iov.iov_len = sizeof(RT_MSG_SERVER_HELLO_S) + 3;
        char *buffer = new char[sizeof(RT_MSG_SERVER_HELLO_S) + 3];
        buffer[0] = 0x25;
        buffer[1] = (uint16_t)sizeof(RT_MSG_SERVER_HELLO_S);
        memcpy(&buffer[3], ret, sizeof(RT_MSG_SERVER_HELLO_S));
        iov.iov_base = buffer;
        return iov;
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

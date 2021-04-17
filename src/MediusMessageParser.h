//
// Created by chris on 4/17/21.
//

#ifndef LIBUV_MEDIUSMESSAGEPARSER_H
#define LIBUV_MEDIUSMESSAGEPARSER_H


#include <cstring>

class MediusMessageParser {
public:
    int command;
    unsigned short length;
    char* mediusMessage;

    explicit MediusMessageParser(char* message) {
        this->command = (unsigned char)message[0];
        this->length = *(unsigned short*)(message+1);
        this->mediusMessage = new char[length];
        memcpy(this->mediusMessage, reinterpret_cast<const void *>(message + 3), this->length * sizeof(char));
    }

    ~MediusMessageParser() {
        delete[] this->mediusMessage;
    }
};


#endif //LIBUV_MEDIUSMESSAGEPARSER_H

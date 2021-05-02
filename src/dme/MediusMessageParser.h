//
// Created by chris on 4/17/21.
//

#ifndef LIBUV_MEDIUSMESSAGEPARSER_H
#define LIBUV_MEDIUSMESSAGEPARSER_H


#include <cstring>
#include <vector>

typedef struct {
    int command;
    unsigned short length;
    bool encrypted;
    unsigned char mediusMessage[2048];
} MediusMessage;

class MediusMessageParser {
public:
    std::vector<MediusMessage> rt_messages;

    explicit MediusMessageParser(char* message) {
        int pos = 0;
        int msglen = sizeof(message);
        do {
            MediusMessage thisMessage;

            thisMessage.command = *(unsigned char*)(message);
            message++;
            thisMessage.length = *(unsigned short*)(message);
            message += 2;
            if (thisMessage.length == 0) {
                break;
            }
            memcpy(thisMessage.mediusMessage, message, thisMessage.length);
            message += thisMessage.length;

            rt_messages.push_back(thisMessage);
        } while (pos < sizeof(message) - 1);

    }
};


#endif //LIBUV_MEDIUSMESSAGEPARSER_H

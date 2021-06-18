//
// Created by chris on 6/18/21.
//

#include <cstring>
#include <bits/types/struct_iovec.h>
#include "MediusHandler.h"

void MediusHandler::ParseMessages(char *message) {
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

void MediusHandler::RegisterMessageHandler(int msgType, struct iovec (*msg_func)(char *)) {
    msgCallbacks[msgType] = msg_func;
}

std::vector<struct iovec> MediusHandler::ProcessMessages() {
    std::vector<struct iovec> iovs;
    for (int i = 0; i < rt_messages.size(); i++) {
        struct iovec iov = msgCallbacks[rt_messages[i].command](reinterpret_cast<char *>(rt_messages[i].mediusMessage));
        iovs.push_back(iov);
    }
    rt_messages.clear();

    return iovs;
}

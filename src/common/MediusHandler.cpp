//
// Created by chris on 6/18/21.
//

#include <cstring>
#include <bits/types/struct_iovec.h>
#include <iostream>
#include "MediusHandler.h"

void MediusHandler::ParseMessages(char *message) {
    int pos = 0;

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

void MediusHandler::RegisterMessageHandler(int msgRequest, struct iovec (*msg_func)(MediusHandler::MediusMessage)) {
    msgCallbacks[msgRequest] = msg_func;
}

std::vector<struct iovec> MediusHandler::ProcessMessages() {
    std::vector<struct iovec> iovs;
    for (int i = 0; i < rt_messages.size(); i++) {
        if (msgCallbacks[rt_messages[i].command] != nullptr) {
            struct iovec iov = msgCallbacks[rt_messages[i].command](rt_messages[i]);
            iovs.push_back(iov);
        } else {
            std::cout << "Unhandled message: ";
            printf("0x%02x", rt_messages[i].command);
            std::cout << std::endl;
        }
    }
    rt_messages.clear();

    return iovs;
}

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

void MediusHandler::RegisterMessageHandler(int msgRequest, struct iovec (*msg_func)(MediusHandler::MediusMessage, const std::shared_ptr<UserData>&)) {
    msgCallbacks[msgRequest].push_back(msg_func);
}

std::vector<struct iovec> MediusHandler::ProcessMessages(const std::shared_ptr<UserData>& uData) {
    std::vector<struct iovec> iovs;
    for (auto & rt_message : rt_messages) {
        if (msgCallbacks[rt_message.command].empty()) {
            std::cout << "Unhandled message: ";
            printf("0x%02x", rt_message.command);
            std::cout << std::endl;
            continue;
        }

        for (auto & msgCallback : msgCallbacks[rt_message.command]) {
            if (msgCallback != nullptr) {
                struct iovec iov = msgCallback(rt_message, uData);
                iovs.push_back(iov);
            }
        }
    }
    rt_messages.clear();

    return iovs;
}

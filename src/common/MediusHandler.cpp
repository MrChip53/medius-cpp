//
// Created by chris on 6/18/21.
//

#include <cstring>
#include <bits/types/struct_iovec.h>
#include <iostream>
#include <iomanip>
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

void MediusHandler::RegisterRTMessageHandler(int msgRequest, std::vector<struct iovec> (*msg_func)(MediusHandler::MediusMessage, MediusHandler*, const std::shared_ptr<UserData>&)) {
    RTMessageCallbacks[msgRequest].push_back(msg_func);
}

void MediusHandler::RegisterMediusMessageHandler(int msgClass, int msgId, std::vector<struct iovec> (*msg_func)(MediusHandler::MediusMessage, MediusHandler*, const std::shared_ptr<UserData>&)) {
    MediusMessageCallbacks[msgClass][msgId].push_back(msg_func);
}

std::vector<struct iovec> MediusHandler::ProcessRTMessages(const std::shared_ptr<UserData>& uData) {
    std::vector<struct iovec> iovs;

    for (auto & rt_message : rt_messages) {
        if (RTMessageCallbacks[rt_message.command].empty() && rt_message.command != 0x0b) {
            std::cerr << "Unhandled message: 0x"
                      << std::hex
                      << std::setfill('0')
                      << std::setw(2)
                      << rt_message.command << std::endl;
            continue;
        } else {
            std::cout << "Handled message: ";
            printf("0x%02x", rt_message.command);
            std::cout << std::endl;
        }

        if (rt_message.command == 0x0b) {
            std::vector<struct iovec> medius_iovs = ProcessMediusMessage(rt_message, uData);
            iovs.insert(iovs.end(), medius_iovs.begin(), medius_iovs.end());
        } else {
            for (auto &msgCallback : RTMessageCallbacks[rt_message.command]) {
                if (msgCallback != nullptr) {
                    std::vector<struct iovec> rt_iovs = msgCallback(rt_message, this, uData);
                    iovs.insert(iovs.end(), rt_iovs.begin(), rt_iovs.end());
                }
            }
        }
    }

    rt_messages.clear();

    return iovs;
}

std::vector<struct iovec> MediusHandler::ProcessMediusMessage(MediusHandler::MediusMessage message, const std::shared_ptr<UserData> &uData) {
    std::vector<struct iovec> iovs;

    uint8_t type = message.mediusMessage[0];
    uint8_t id = message.mediusMessage[1];

    if (MediusMessageCallbacks[type][id].empty()) {
        std::cerr << "Unhandled message: Type: 0x"
                  << std::hex
                  << std::setfill('0')
                  << std::setw(2)
                  << (int)type
                  << " ID: 0x"
                  << std::hex
                  << std::setfill('0')
                  << std::setw(2)
                  << (int)id
                  << std::endl;
        return std::vector<struct iovec>();
    } else {
        std::cout << "Handled message: ";
        printf("Type: 0x%02x, ID: 0x%02x", type, id);
        std::cout << std::endl;
    }

    for (auto & msgCallback : MediusMessageCallbacks[type][id]) {
        if (msgCallback != nullptr) {
            std::vector<struct iovec> rt_iovs = msgCallback(message, this, uData);
            iovs.insert(iovs.end(), rt_iovs.begin(), rt_iovs.end());
        }
    }

    return iovs;
}

void MediusHandler::CreateApp(int AppID) {

}

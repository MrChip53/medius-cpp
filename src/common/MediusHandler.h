//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H
#define MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H

#include <vector>
#include <array>
#include <memory>
#include "UserData.h"

class MediusHandler {
public:
    typedef struct {
        int32_t command;
        uint16_t length;
        bool encrypted;
        uint8_t mediusMessage[2048];
    } MediusMessage;

    void ParseMessages(char * message);
    std::vector<struct iovec> ProcessRTMessages(const std::shared_ptr<UserData>& uData);

    void RegisterRTMessageHandler(int msgRequest, struct iovec(*msg_func)(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData));

private:
    std::vector<MediusHandler::MediusMessage> rt_messages;
    std::array<std::vector<struct iovec(*)(MediusHandler::MediusMessage, const std::shared_ptr<UserData>&)>, 55> msgCallbacks;
};


#endif //MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H

//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H
#define MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H

#include <vector>
#include <array>
#include <memory>
#include <unordered_map>
#include "UserData.h"
#include "Game.h"

class MediusHandler {
public:
    typedef struct {
        int32_t command;
        uint16_t length;
        bool encrypted;
        uint8_t mediusMessage[2048];
    } MediusMessage;

    void CreateApp(int AppID);

    void ParseMessages(char * message);

    std::vector<struct iovec> ProcessRTMessages(const std::shared_ptr<UserData>& uData);
    std::vector<struct iovec> ProcessMediusMessage(MediusHandler::MediusMessage message, const std::shared_ptr<UserData>& uData);

    void RegisterRTMessageHandler(int msgRequest, std::vector<struct iovec>(*msg_func)(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData));
    void RegisterMediusMessageHandler(int msgClass, int msgId, std::vector<struct iovec>(*msg_func)(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData));

    // TODO change this to be an App class that contains games with other info about the app like channels etc
    std::unordered_map<int, std::vector<std::shared_ptr<Game>>> AppGames;

private:
    std::vector<MediusHandler::MediusMessage> rt_messages;
    std::array<std::vector<std::vector<struct iovec>(*)(MediusHandler::MediusMessage, MediusHandler* handler, const std::shared_ptr<UserData>&)>, 55> RTMessageCallbacks;
    std::array<std::array<std::vector<std::vector<struct iovec>(*)(MediusHandler::MediusMessage, MediusHandler*, const std::shared_ptr<UserData>&)>, 300>, 6> MediusMessageCallbacks;
};


#endif //MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H

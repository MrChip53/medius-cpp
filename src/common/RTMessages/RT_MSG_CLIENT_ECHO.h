//
// Created by chris on 6/24/21.
//

#ifndef MEDIUS_MLS_SERVER_RT_MSG_CLIENT_ECHO_H
#define MEDIUS_MLS_SERVER_RT_MSG_CLIENT_ECHO_H

#include "../MediusHandler.h"

class RT_MSG_CLIENT_ECHO {
public:
    const static uint8_t Request = 0x05;
    const static uint8_t Response = 0x05;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        char* packet = static_cast<char *>(malloc(sizeof(char) * data.length));
        memcpy(packet, data.mediusMessage, data.length);

        return Util::CreateIovec(packet, Response, data.length);
    }
};


#endif //MEDIUS_MLS_SERVER_RT_MSG_CLIENT_ECHO_H

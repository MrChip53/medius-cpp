//
// Created by chris on 6/22/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_CLIENT_DISCONNECT_WITH_REASON_H
#define MEDIUS_MUIS_SERVER_RT_MSG_CLIENT_DISCONNECT_WITH_REASON_H

#include "../UserData.h"
#include <memory>

class RT_MSG_CLIENT_DISCONNECT_WITH_REASON {
public:
    const static uint8_t Request = 0x20;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        close(uData->SocketFd());
        uData->Closed() = true;

        return std::vector<struct iovec>();
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_CLIENT_DISCONNECT_WITH_REASON_H

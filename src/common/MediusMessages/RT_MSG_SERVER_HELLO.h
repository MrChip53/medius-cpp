//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

#include "../Packets.h"
#include "../../dme/MediusMessageParser.h"

class RT_MSG_SERVER_HELLO {

public:
    const static uint8_t Request = 0x24;
    const static uint8_t Response = 0x25;

    static struct iovec process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        iovec iov;

        Packets::RT_MSG_SERVER_HELLO *packet = static_cast<Packets::RT_MSG_SERVER_HELLO *>(malloc(sizeof(Packets::RT_MSG_SERVER_HELLO)));
        memset(packet, 0, sizeof(Packets::RT_MSG_SERVER_HELLO));
        packet->encrypt = 0;
        packet->serverVersion = 0x6e;

        return Util::CreateIovec(packet, Response, sizeof(Packets::RT_MSG_SERVER_HELLO));
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_HELLO_H

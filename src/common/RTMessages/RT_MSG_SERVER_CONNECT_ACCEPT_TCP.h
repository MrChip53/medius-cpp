//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_ACCEPT_TCP_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_ACCEPT_TCP_H

#include "../Packets.h"

class RT_MSG_SERVER_CONNECT_ACCEPT_TCP {
public:
    const static uint8_t Request = 0x00;
    const static uint8_t Response = 0x07;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        if (data.length > sizeof(Packets::RT_MSG_CLIENT_CONNECT_TCP)) {
            //Extended packet
            uData->AppId() = ((Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED*)data.mediusMessage)->AppId;
            uData->WorldId() = ((Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED*)data.mediusMessage)->TargetWorldId;
            //TODO verify and store session and access keys
            uData->Username() = "KiLLLaTHON";
        } else {
            //Normal packet
            uData->AppId() = ((Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED*)data.mediusMessage)->AppId;
            uData->WorldId() = ((Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED*)data.mediusMessage)->TargetWorldId;
        }

        Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP *packet = static_cast<Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP *>(malloc(sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP)));
        memset(packet, 0, sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP));
        packet->CurClients = 0x01;
        strcpy(packet->ClientIP, uData->IP().c_str());

        return Util::CreateIovec(packet, Response, sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP));
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_ACCEPT_TCP_H

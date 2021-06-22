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

    static struct iovec process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        iovec iov;

        if (data.length > sizeof(Packets::RT_MSG_CLIENT_CONNECT_TCP)) {
            //Extended packet
            uData->AppId() = ((Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED*)data.mediusMessage)->AppId;
            uData->WorldId() = ((Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED*)data.mediusMessage)->TargetWorldId;
            //TODO verify and store session and access keys
        } else {
            //Normal packet
            uData->AppId() = ((Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED*)data.mediusMessage)->AppId;
            uData->WorldId() = ((Packets::RT_MSG_CLIENT_CONNECT_TCP_EXTENDED*)data.mediusMessage)->TargetWorldId;
        }

        Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP *packet = static_cast<Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP *>(malloc(sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP)));
        memset(packet, 0, sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP));
        packet->CurClients = 0x01;
        strcpy(packet->IP, uData->IP().c_str());

        //TODO make this a Util:: function
        iov.iov_len = sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP) + 3;
        char *buffer = new char[sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP) + 3];
        memset(buffer, 0, sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP) + 3);
        buffer[0] = Response;
        buffer[1] = (uint16_t)sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP);
        memcpy(&buffer[3], packet, sizeof(Packets::RT_MSG_SERVER_CONNECT_ACCEPT_TCP));
        free(packet);

        iov.iov_base = buffer;
        return iov;
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_ACCEPT_TCP_H

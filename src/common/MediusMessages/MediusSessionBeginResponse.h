//
// Created by chris on 6/24/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSSESSIONBEGINRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSSESSIONBEGINRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusSessionBeginResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::SessionBeginResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        std::vector<struct iovec> iovs;

        Packets::MessageID msgid;

        if (data.length > sizeof(Packets::MediusSessionBeginRequest)) {
            memcpy(msgid, ((Packets::MediusExtendedSessionBeginRequest*)&data.mediusMessage[2])->SessionBeginRequest.MsgID, MESSAGEID_MAXLEN);
        } else {
            memcpy(msgid, ((Packets::MediusSessionBeginRequest*)&data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        }

        Packets::MediusSessionBeginResponse *packet = static_cast<Packets::MediusSessionBeginResponse *>(malloc(sizeof(Packets::MediusSessionBeginResponse)));
        memset(packet, 0, sizeof(Packets::MediusSessionBeginResponse));
        memcpy(packet->MsgID, msgid, MESSAGEID_MAXLEN);
        // TODO generate session key, verify its unique and store in DB
        strcpy(packet->SKey, "TestKey");
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusSessionBeginResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSSESSIONBEGINRESPONSE_H

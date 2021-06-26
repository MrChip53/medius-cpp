//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSACCOUNTGETIDRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSACCOUNTGETIDRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusAccountGetIDResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::AccountGetIDResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusAccountGetIDResponse *packet = static_cast<Packets::MediusAccountGetIDResponse *>(malloc(
                sizeof(Packets::MediusAccountGetIDResponse)));
        memset(packet, 0, sizeof(Packets::MediusAccountGetIDResponse));
        // TODO Pull from DB
        memcpy(packet->MsgID, ((Packets::MediusAccountGetIDRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;
        packet->AccountID = 1;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusAccountGetIDResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSACCOUNTGETIDRESPONSE_H

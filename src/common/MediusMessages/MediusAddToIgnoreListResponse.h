//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSADDTOIGNORELISTRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSADDTOIGNORELISTRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusAddToIgnoreListResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::AddToIgnoreListResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusAddToIgnoreListResponse *packet = static_cast<Packets::MediusAddToIgnoreListResponse *>(malloc(
                sizeof(Packets::MediusAddToIgnoreListResponse)));
        memset(packet, 0, sizeof(Packets::MediusAddToIgnoreListResponse));
        // TODO grab acccount id and store in DB
        memcpy(packet->MsgID, ((Packets::MediusAddToIgnoreListRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusAddToIgnoreListResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSADDTOIGNORELISTRESPONSE_H

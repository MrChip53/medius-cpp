//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSGETIGNORELISTRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSGETIGNORELISTRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusGetIgnoreListResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::GetIgnoreListResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        Packets::MediusGetIgnoreListResponse *packet = static_cast<Packets::MediusGetIgnoreListResponse *>(malloc(
                sizeof(Packets::MediusGetIgnoreListResponse)));
        memset(packet, 0, sizeof(Packets::MediusGetIgnoreListResponse));
        // TODO Pull from DB
        memcpy(packet->MsgID, ((Packets::MediusGetIgnoreListRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusNoResult;
        packet->EndOfList = true;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusGetIgnoreListResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSGETIGNORELISTRESPONSE_H

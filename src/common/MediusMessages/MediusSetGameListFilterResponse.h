//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSSETGAMELISTFILTERRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSSETGAMELISTFILTERRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusSetGameListFilterResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobbyExt;
    const static PacketInfo::LobbyExt::Packet RespId = PacketInfo::LobbyExt::SetGameListFilterResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusSetGameListFilterResponse *packet = static_cast<Packets::MediusSetGameListFilterResponse *>(malloc(
                sizeof(Packets::MediusSetGameListFilterResponse)));
        memset(packet, 0, sizeof(Packets::MediusSetGameListFilterResponse));
        memcpy(packet->MsgID, ((Packets::MediusSetGameListFilterRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;
        // TODO this should not be static
        packet->FilterID = 1;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusSetGameListFilterResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSSETGAMELISTFILTERRESPONSE_H

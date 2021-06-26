//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSGAMELIST_EXTRAINFORESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSGAMELIST_EXTRAINFORESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusGameList_ExtraInfoResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobbyExt;
    const static PacketInfo::LobbyExt::Packet RespId = PacketInfo::LobbyExt::GameList_ExtraInfoResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusGameList_ExtraInfoResponse *packet = static_cast<Packets::MediusGameList_ExtraInfoResponse *>(malloc(
                sizeof(Packets::MediusGameList_ExtraInfoResponse)));
        memset(packet, 0, sizeof(Packets::MediusGameList_ExtraInfoResponse));
        // TODO Check pageID and page size, retrieve info from DB
        memcpy(packet->MsgID, ((Packets::MediusGameList_ExtraInfoRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusNoResult;
        packet->EndOfList = true;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusGameList_ExtraInfoResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSGAMELIST_EXTRAINFORESPONSE_H

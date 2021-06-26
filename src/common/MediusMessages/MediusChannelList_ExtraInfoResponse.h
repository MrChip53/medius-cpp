//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSCHANNELLIST_EXTRAINFORESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSCHANNELLIST_EXTRAINFORESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusChannelList_ExtraInfoResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::ChannelList_ExtraInfoResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusChannelList_ExtraInfoResponse *packet = static_cast<Packets::MediusChannelList_ExtraInfoResponse *>(malloc(
                sizeof(Packets::MediusChannelList_ExtraInfoResponse)));
        memset(packet, 0, sizeof(Packets::MediusChannelList_ExtraInfoResponse));
        // TODO Check pageID and page size, retrieve info from DB
        memcpy(packet->MsgID, ((Packets::MediusChannelList_ExtraInfoRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;
        packet->MediusWorldID = 2;
        packet->EndOfList = true;
        packet->GameWorldCount = 0;
        packet->MaxPlayers = 256;
        strcpy(packet->LobbyName, "Socom Cafe");

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusChannelList_ExtraInfoResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSCHANNELLIST_EXTRAINFORESPONSE_H

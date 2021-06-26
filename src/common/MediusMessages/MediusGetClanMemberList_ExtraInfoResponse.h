//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSGETCLANMEMBERLIST_EXTRAINFORESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSGETCLANMEMBERLIST_EXTRAINFORESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusGetClanMemberList_ExtraInfoResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::GetClanMemberList_ExtraInfoResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusGetClanMemberList_ExtraInfoResponse *packet = static_cast<Packets::MediusGetClanMemberList_ExtraInfoResponse *>(malloc(
                sizeof(Packets::MediusGetClanMemberList_ExtraInfoResponse)));
        memset(packet, 0, sizeof(Packets::MediusGetClanMemberList_ExtraInfoResponse));
        // TODO Pull from DB
        memcpy(packet->MsgID, ((Packets::MediusGetClanMemberList_ExtraInfoRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusNoResult;
        packet->EndOfList = true;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusGetClanMemberList_ExtraInfoResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSGETCLANMEMBERLIST_EXTRAINFORESPONSE_H

//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSCHECKMYCLANINVITATIONSRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSCHECKMYCLANINVITATIONSRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusCheckMyClanInvitationsResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::CheckMyClanInvitationsResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        Packets::MediusCheckMyClanInvitationsResponse *packet = static_cast<Packets::MediusCheckMyClanInvitationsResponse *>(malloc(
                sizeof(Packets::MediusCheckMyClanInvitationsResponse)));
        memset(packet, 0, sizeof(Packets::MediusCheckMyClanInvitationsResponse));
        // TODO Pull from DB, check session key, start page and page size
        memcpy(packet->MsgID, ((Packets::MediusCheckMyClanInvitationsRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusNoResult;
        packet->EndOfList = true;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusCheckMyClanInvitationsResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSCHECKMYCLANINVITATIONSRESPONSE_H

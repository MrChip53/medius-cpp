//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSGETBUDDYINVITATIONSRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSGETBUDDYINVITATIONSRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusGetBuddyInvitationsResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::LobbyExt::Packet RespId = PacketInfo::LobbyExt::GetBuddyInvitationsResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        Packets::MediusGetBuddyInvitationsResponse *packet = static_cast<Packets::MediusGetBuddyInvitationsResponse *>(malloc(
                sizeof(Packets::MediusGetBuddyInvitationsResponse)));
        memset(packet, 0, sizeof(Packets::MediusGetBuddyInvitationsResponse));
        // TODO Pull from DB
        memcpy(packet->MsgID, ((Packets::MediusGetBuddyInvitationsRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusNoResult;
        packet->EndOfList = true;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusGetBuddyInvitationsResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSGETBUDDYINVITATIONSRESPONSE_H

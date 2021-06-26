//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSGETMYCLANSRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSGETMYCLANSRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusGetMyClansResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::GetMyClansResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusGetMyClansResponse *packet = static_cast<Packets::MediusGetMyClansResponse *>(malloc(
                sizeof(Packets::MediusGetMyClansResponse)));
        memset(packet, 0, sizeof(Packets::MediusGetMyClansResponse));
        // TODO Pull from DB, check session key and page and start in request packet
        memcpy(packet->MsgID, ((Packets::MediusGetMyClansRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusNoResult;
        packet->EndOfList = true;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusGetMyClansResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSGETMYCLANSRESPONSE_H

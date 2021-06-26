//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSGETSERVERTIMERESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSGETSERVERTIMERESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusGetServerTimeResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobbyExt;
    const static PacketInfo::LobbyExt::Packet RespId = PacketInfo::LobbyExt::GetServerTimeResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusGetServerTimeResponse *packet = static_cast<Packets::MediusGetServerTimeResponse *>(malloc(
                sizeof(Packets::MediusGetServerTimeResponse)));
        memset(packet, 0, sizeof(Packets::MediusGetServerTimeResponse));
        memcpy(packet->MsgID, ((Packets::MediusGetServerTimeRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->GMT_time = Packets::MediusTimeZone_CST;
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;
        // TODO this should not be static
        packet->GMT_time = 1614920978;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusGetServerTimeResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSGETSERVERTIMERESPONSE_H

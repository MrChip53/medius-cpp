//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSPLAYERINFORESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSPLAYERINFORESPONSE_H

#include <iostream>
#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusPlayerInfoResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::PlayerInfoResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusPlayerInfoResponse *packet = static_cast<Packets::MediusPlayerInfoResponse *>(malloc(
                sizeof(Packets::MediusPlayerInfoResponse)));
        memset(packet, 0, sizeof(Packets::MediusPlayerInfoResponse));
        // TODO verify Session key
        memcpy(packet->MsgID, ((Packets::MediusPlayerInfoRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;
        packet->ApplicationID = uData->AppId();
        packet->PlayerStatus = Packets::MediusPlayerInChatWorld;
        // TODO store this in UserData and Account Stats
        packet->ConnectionType = Packets::Ethernet;
        memcpy(packet->AccountName, uData->Username().c_str(), ACCOUNTNAME_MAXLEN);

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusPlayerInfoResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSPLAYERINFORESPONSE_H

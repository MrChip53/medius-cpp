//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSTEXTFILTERRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSTEXTFILTERRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusTextFilterResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::TextFilterResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusTextFilterResponse *packet = static_cast<Packets::MediusTextFilterResponse *>(malloc(
                sizeof(Packets::MediusTextFilterResponse)));
        memset(packet, 0, sizeof(Packets::MediusTextFilterResponse));
        // TODO check session key
        memcpy(packet->MsgID, ((Packets::MediusTextFilterRequest *)&data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusPass;
        memcpy(packet->Text, ((Packets::MediusTextFilterRequest *)&data.mediusMessage[2])->Text, CHATMESSAGE_MAXLEN);



        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusTextFilterResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSTEXTFILTERRESPONSE_H

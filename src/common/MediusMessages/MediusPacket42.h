//
// Created by chris on 6/24/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSPACKET42_H
#define MEDIUS_MLS_SERVER_MEDIUSPACKET42_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusPacket42 {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobbyExt;
    const static PacketInfo::LobbyExt::Packet RespId = PacketInfo::LobbyExt::UnknownPacket_41Response;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        std::vector<struct iovec> iovs;
        // TODO this should not be static
        auto packet_data = Util::HexToBytes("0a220004420000000000000000000000000000000000000000000000000000000012bd4160");

        auto len = std::get<0>(packet_data);
        auto packet = std::get<1>(packet_data);

        iovs.push_back(Util::CreateStaticIovec(packet, len));

        return iovs;
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSPACKET42_H

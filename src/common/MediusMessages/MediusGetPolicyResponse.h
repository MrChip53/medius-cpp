//
// Created by chris on 6/24/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSGETPOLICYRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSGETPOLICYRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"

class MediusGetPolicyResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::PolicyResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        std::vector<struct iovec> iovs;
        // TODO this should not be static
        auto packet_data = Util::HexToBytes("0a220101485573616765000000000000000000000000000000000000000000000054657374696e6700000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000");

        auto len = std::get<0>(packet_data);
        auto packet = std::get<1>(packet_data);

        iovs.push_back(Util::CreateStaticIovec(packet, len));

        return iovs;
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSGETPOLICYRESPONSE_H

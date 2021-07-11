//
// Created by chris on 6/26/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSJOINGAMERESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSJOINGAMERESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusJoinGameResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::JoinGameResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::NetConnectionInfo nci;
        Packets::NetAddressList nal;
        Packets::NetAddress na1;
        Packets::NetAddress na2;
        memset(&nci, 0, sizeof(Packets::NetConnectionInfo));
        memset(&nal, 0, sizeof(Packets::NetAddressList));
        memset(&na1, 0, sizeof(Packets::NetAddress));
        memset(&na2, 0, sizeof(Packets::NetAddress));

        Packets::MediusJoinGameResponse *packet = static_cast<Packets::MediusJoinGameResponse *>(malloc(
                sizeof(Packets::MediusJoinGameResponse)));
        memset(packet, 0, sizeof(Packets::MediusJoinGameResponse));
        // TODO check session key, grab info from a DB ofc
        memcpy(packet->MsgID, ((Packets::MediusJoinGameRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;

        // TODO abstract all this net connect info into functions
        nci.WorldID = ((Packets::MediusJoinGameRequest *) &data.mediusMessage[2])->MediusWorldID;
        nci.Type = Packets::NetConnectionTypeClientServerTCP;
        strcpy(nci.AKey, "TestAccess");
        memcpy(nci.SKey, ((Packets::MediusJoinGameRequest *)&data.mediusMessage[2])->SKey, sizeof(Packets::SessionKey));

        nal.aAddressList[0].AddressType = Packets::NetAddressTypeExternal;
        strcpy(nal.aAddressList[0].Address, "192.168.1.93");
        nal.aAddressList[0].Port = 10080;

        nal.aAddressList[1].AddressType = Packets::NetAddressTypeNATService;
        strcpy(nal.aAddressList[1].Address, "192.168.1.93");
        nal.aAddressList[1].Port = 2999;

        nci.AddressList = nal;

        memcpy(&packet->ConnectInfo, &nci, sizeof(Packets::NetConnectionInfo));

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusJoinGameResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSJOINGAMERESPONSE_H

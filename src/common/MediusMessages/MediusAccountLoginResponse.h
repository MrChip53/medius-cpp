//
// Created by chris on 6/24/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSACCOUNTLOGINRESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSACCOUNTLOGINRESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusAccountLoginResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::AccountLoginResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        std::vector<struct iovec> iovs;

        Packets::NetConnectionInfo nci;
        Packets::NetAddressList nal;
        Packets::NetAddress na1;
        Packets::NetAddress na2;
        memset(&nci, 0, sizeof(Packets::NetConnectionInfo));
        memset(&nal, 0, sizeof(Packets::NetAddressList));
        memset(&na1, 0, sizeof(Packets::NetAddress));
        memset(&na2, 0, sizeof(Packets::NetAddress));


        auto *packet = static_cast<Packets::MediusAccountLoginResponse *>(malloc(sizeof(Packets::MediusAccountLoginResponse)));
        memset(packet, 0, sizeof(Packets::MediusAccountLoginResponse));
        memcpy(packet->MsgID, ((Packets::MediusAccountLoginRequest *)&data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        // TODO verify account login
        uData->Username() = ((Packets::MediusAccountLoginRequest *)&data.mediusMessage[2])->AccountName;
        packet->AccountID = 1;
        packet->AccountType = Packets::MediusMasterAccount;
        packet->MediusWorldID = 1;

        nci.WorldID = 1;
        nci.Type = Packets::NetConnectionTypeClientServerTCP;
        strcpy(nci.AKey, "TestAccess");
        memcpy(nci.SKey, ((Packets::MediusAccountLoginRequest *)&data.mediusMessage[2])->SKey, sizeof(Packets::SessionKey));

        nal.aAddressList[0].AddressType = Packets::NetAddressTypeExternal;
        strcpy(nal.aAddressList[0].Address, "192.168.1.93");
        nal.aAddressList[0].Port = 10078;

        nal.aAddressList[1].AddressType = Packets::NetAddressTypeNATService;
        strcpy(nal.aAddressList[1].Address, "192.168.1.93");
        nal.aAddressList[1].Port = 2999;

        //memcpy(&nal.aAddressList[0], &na1, sizeof(Packets::NetAddress));
        //memcpy(&nal.aAddressList[1], &na2, sizeof(Packets::NetAddress));

        nci.AddressList = nal;

        memcpy(&packet->ConnectInfo, &nci, sizeof(Packets::NetConnectionInfo));
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusAccountLoginResponse), RespType, RespId);
    }

};


#endif //MEDIUS_MLS_SERVER_MEDIUSACCOUNTLOGINRESPONSE_H

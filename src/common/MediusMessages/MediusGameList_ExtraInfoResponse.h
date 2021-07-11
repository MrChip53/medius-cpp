//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSGAMELIST_EXTRAINFORESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSGAMELIST_EXTRAINFORESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusGameList_ExtraInfoResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobbyExt;
    const static PacketInfo::LobbyExt::Packet RespId = PacketInfo::LobbyExt::GameList_ExtraInfoResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        std::vector<struct iovec> iovs;

        if (handler->AppGames[uData->AppId()].size() > 0) {
            std::vector<struct iovec> gameiovs;
            Packets::MediusGameList_ExtraInfoResponse *last_packet;
            for (auto & game : handler->AppGames[uData->AppId()]) {
                Packets::MediusGameList_ExtraInfoResponse *packet = static_cast<Packets::MediusGameList_ExtraInfoResponse *>(malloc(
                        sizeof(Packets::MediusGameList_ExtraInfoResponse)));
                memset(packet, 0, sizeof(Packets::MediusGameList_ExtraInfoResponse));
                // TODO Check pageID and page size, retrieve info from DB
                memcpy(packet->MsgID, ((Packets::MediusGameList_ExtraInfoRequest *) &data.mediusMessage[2])->MsgID,
                       MESSAGEID_MAXLEN);
                packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;
                packet->MediusWorldID = game.second->MediusWorldID;
                memcpy(packet->GameStats, game.second->GameStats, GAMESTATS_MAXLEN);
                strncpy(packet->GameName, game.second->GameName, GAMENAME_MAXLEN);
                packet->GameLevel = game.second->GameLevel;
                packet->PlayerSkillLevel = game.second->PlayerSkillLevel;
                packet->RulesSet = game.second->RulesSet;
                packet->SecurityLevel = game.second->SecurityLevel;
                packet->PlayerCount = game.second->PlayerCount;
                packet->GameHostType = game.second->GameHostType;
                packet->MinPlayers = game.second->MinPlayers;
                packet->GenericField1 = game.second->GenericField1;
                packet->GenericField2 = game.second->GenericField2;
                packet->GenericField3 = game.second->GenericField3;
                packet->GenericField4 = game.second->GenericField4;
                packet->GenericField5 = game.second->GenericField5;
                packet->GenericField6 = game.second->GenericField6;
                packet->GenericField7 = game.second->GenericField7;
                packet->GenericField8 = game.second->GenericField8;
                packet->WorldStatus = game.second->WorldStatus;
                packet->MaxPlayers = game.second->MaxPlayers;
                // TODO Fix setting end of list
                packet->EndOfList = true;
                std::vector<struct iovec> iov = Util::CreateMediusIovec(packet,
                                                                        sizeof(Packets::MediusGameList_ExtraInfoResponse),
                                                                        RespType, RespId);
                for(auto & iov_game : iov) {
                    iovs.push_back(iov_game);
                }

                last_packet = packet;
            }
            if (last_packet != nullptr)
                last_packet->EndOfList = true;
        } else {
            Packets::MediusGameList_ExtraInfoResponse *packet = static_cast<Packets::MediusGameList_ExtraInfoResponse *>(malloc(
                    sizeof(Packets::MediusGameList_ExtraInfoResponse)));
            memset(packet, 0, sizeof(Packets::MediusGameList_ExtraInfoResponse));
            memcpy(packet->MsgID, ((Packets::MediusGameList_ExtraInfoRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
            packet->StatusCode = Packets::MediusCallbackStatus::MediusNoResult;
            packet->EndOfList = true;
            return Util::CreateMediusIovec(packet, sizeof(Packets::MediusGameList_ExtraInfoResponse), RespType, RespId);
        }

        return iovs;
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSGAMELIST_EXTRAINFORESPONSE_H

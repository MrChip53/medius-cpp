//
// Created by chris on 6/26/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSCREATEGAMERESPONSE_H
#define MEDIUS_MLS_SERVER_MEDIUSCREATEGAMERESPONSE_H

#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusCreateGameResponse {
public:
    const static PacketInfo::PacketType RespType = PacketInfo::MessageClassLobby;
    const static PacketInfo::Lobby::Packet RespId = PacketInfo::Lobby::CreateGameResponse;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusCreateGameResponse *packet = static_cast<Packets::MediusCreateGameResponse *>(malloc(
                sizeof(Packets::MediusCreateGameResponse)));
        memset(packet, 0, sizeof(Packets::MediusCreateGameResponse));
        // TODO Check session key
        memcpy(packet->MsgID, ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->MsgID, MESSAGEID_MAXLEN);
        packet->StatusCode = Packets::MediusCallbackStatus::MediusSuccess;
        packet->MediusWorldID = handler->curWorldIDCounter;

        std::shared_ptr<Game> newGame(new Game);

        memset(newGame.get(), 0, sizeof(Game));

        newGame->MaxPlayers = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->MaxPlayers;
        newGame->MinPlayers = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->MinPlayers;
        newGame->MediusWorldID = packet->MediusWorldID;

        newGame->GenericField1 = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GenericField1;
        newGame->GenericField2 = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GenericField2;
        newGame->GenericField3 = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GenericField3;
        newGame->GenericField4 = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GenericField4;
        newGame->GenericField5 = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GenericField5;
        newGame->GenericField6 = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GenericField6;
        newGame->GenericField7 = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GenericField7;
        newGame->GenericField8 = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GenericField8;

        // TODO change back when UDP AUX is done
        newGame->GameHostType = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GameHostType;
        strncpy(newGame->GameName, ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GameName, GAMENAME_MAXLEN);
        strncpy(newGame->GamePassword, ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GamePassword, GAMEPASSWORD_MAXLEN);
        strncpy(newGame->SpectatorPassword, ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->SpectatorPassword, GAMEPASSWORD_MAXLEN);
        newGame->GameLevel = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->GameLevel;
        newGame->PlayerSkillLevel = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->PlayerSkillLevel;
        newGame->RulesSet = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->RulesSet;
        newGame->Attributes = ((Packets::MediusCreateGameRequest *) &data.mediusMessage[2])->Attributes;

        handler->curWorldIDCounter++;

        handler->AppGames[uData->AppId()][packet->MediusWorldID] = std::move(newGame);

        return Util::CreateMediusIovec(packet, sizeof(Packets::MediusCreateGameResponse), RespType, RespId);
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSCREATEGAMERESPONSE_H

//
// Created by chris on 6/26/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSWORLDREPORT_H
#define MEDIUS_MLS_SERVER_MEDIUSWORLDREPORT_H

#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusWorldReport {
public:
    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        Packets::MediusWorldReport *report = (Packets::MediusWorldReport *)&data.mediusMessage[2];

        std::shared_ptr<Game> thisGame = handler->AppGames[uData->AppId()][report->MediusWorldID];

        strncpy(thisGame->GameName, report->GameName, GAMENAME_MAXLEN);
        memcpy(thisGame->GameStats, report->GameStats, GAMESTATS_MAXLEN);
        thisGame->MaxPlayers = report->MaxPlayers;
        thisGame->WorldStatus = report->WorldStatus;
        thisGame->MinPlayers = report->MinPlayers;
        thisGame->PlayerCount = report->PlayerCount;
        thisGame->RulesSet = report->RulesSet;
        thisGame->PlayerSkillLevel = report->PlayerSkillLevel;
        thisGame->GameLevel = report->GameLevel;
        thisGame->GenericField1 = report->GenericField1;
        thisGame->GenericField2 = report->GenericField2;
        thisGame->GenericField3 = report->GenericField3;
        thisGame->GenericField4 = report->GenericField4;
        thisGame->GenericField5 = report->GenericField5;
        thisGame->GenericField6 = report->GenericField6;
        thisGame->GenericField7 = report->GenericField7;
        thisGame->GenericField8 = report->GenericField8;

        return std::vector<struct iovec>();
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSWORLDREPORT_H

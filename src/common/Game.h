//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_GAME_H
#define MEDIUS_MLS_SERVER_GAME_H

#include "./Packets.h"

typedef struct {
    int MediusWorldID;
    unsigned short PlayerCount;
    unsigned short MinPlayers;
    unsigned short MaxPlayers;
    int GameLevel;
    int PlayerSkillLevel;
    int RulesSet;
    int GenericField1;
    int GenericField2;
    int GenericField3;
    int GenericField4;
    int GenericField5;
    int GenericField6;
    int GenericField7;
    int GenericField8;
    Packets::MediusWorldSecurityLevelType SecurityLevel;
    Packets::MediusWorldStatus WorldStatus;
    Packets::MediusGameHostType GameHostType;
    Packets::MediusWorldAttributesType Attributes;
    char GameName[GAMENAME_MAXLEN];
    char GamePassword[GAMEPASSWORD_MAXLEN];
    char SpectatorPassword[GAMEPASSWORD_MAXLEN];
    char GameStats[GAMESTATS_MAXLEN];
} Game;

#endif //MEDIUS_MLS_SERVER_GAME_H

//
// Created by chris on 6/20/21.
//

#ifndef MEDIUS_MUIS_SERVER_MESSAGES_H
#define MEDIUS_MUIS_SERVER_MESSAGES_H

#include "RTMessages/RT_MSG_SERVER_HELLO.h"
#include "RTMessages/RT_MSG_SERVER_CONNECT_ACCEPT_TCP.h"
#include "RTMessages/RT_MSG_SERVER_CONNECT_COMPLETE.h"
#include "RTMessages/RT_MSG_SERVER_ECHO.h"
#include "RTMessages/RT_MSG_CLIENT_DISCONNECT_WITH_REASON.h"
#include "RTMessages/RT_MSG_CLIENT_ECHO.h"

#include "MediusMessages/MediusUniverseVariableInformationResponse.h"
#include "MediusMessages/MediusSessionBeginResponse.h"
#include "MediusMessages/MediusSetLocalizationParamsResponse.h"
#include "MediusMessages/MediusGetPolicyResponse.h"
#include "MediusMessages/MediusAccountLoginResponse.h"
#include "MediusMessages/MediusPacket42.h"
#include "MediusMessages/MediusGetServerTimeResponse.h"
#include "MediusMessages/MediusUpdateUserState.h"
#include "MediusMessages/MediusSetGameListFilterResponse.h"
#include "MediusMessages/MediusPlayerInfoResponse.h"
#include "MediusMessages/MediusChannelList_ExtraInfoResponse.h"
#include "MediusMessages/MediusGetBuddyInvitationsResponse.h"
#include "MediusMessages/MediusAccountStatsRequest.h"
#include "MediusMessages/MediusGetMyClansResponse.h"
#include "MediusMessages/MediusUpdateLadderStatsWideRequest.h"
#include "MediusMessages/MediusGetIgnoreListResponse.h"
#include "MediusMessages/MediusCheckMyClanInvitationsResponse.h"
#include "MediusMessages/MediusGetClanMemberList_ExtraInfoResponse.h"
#include "MediusMessages/MediusAccountGetIDResponse.h"
#include "MediusMessages/MediusAddToIgnoreListResponse.h"
#include "MediusMessages/MediusJoinChannelResponse.h"
#include "MediusMessages/MediusJoinGameResponse.h"
#include "MediusMessages/MediusTextFilterResponse.h"
#include "MediusMessages/MediusGameList_ExtraInfoResponse.h"
#include "MediusMessages/MediusCreateGameResponse.h"

#endif //MEDIUS_MUIS_SERVER_MESSAGES_H

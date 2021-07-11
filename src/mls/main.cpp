#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h>
#include "../common/IOuring.h"
#include "../common/Util.h"

#include "../common/Messages.h"

#define DEFAULT_SERVER_PORT     10078

IOuring* ring;

void sigint_handler(int signo) {
    printf("^C pressed. Shutting down.\n");
    ring->Close();
    exit(0);
}

int main() {
    MediusHandler* mHandler;

    mHandler = new MediusHandler();

    ring = new IOuring(DEFAULT_SERVER_PORT);
    signal(SIGINT, sigint_handler);

    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::ExtendedSessionBeginRequest, MediusSessionBeginResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::UnknownPacket_41, MediusPacket42::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::GetServerTimeRequest, MediusGetServerTimeResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::SetGameListFilter, MediusSetGameListFilterResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::ChannelList_ExtraInfo, MediusChannelList_ExtraInfoResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::GetBuddyInvitations, MediusGetBuddyInvitationsResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::UpdateLadderStatsWide, MediusUpdateLadderStatsWideRequest::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::GameList_ExtraInfo, MediusGameList_ExtraInfoResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::CreateGame, MediusCreateGameResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobbyExt, PacketInfo::LobbyExt::WorldReport, MediusWorldReport::process);

    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::SetLocalizationParams, MediusSetLocalizationParamsResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::UpdateUserState, MediusUpdateUserState::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::PlayerInfo, MediusPlayerInfoResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::AccountUpdateStats, MediusAccountStatsRequest::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::GetMyClans, MediusGetMyClansResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::GetIgnoreList, MediusGetIgnoreListResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::CheckMyClanInvitations, MediusCheckMyClanInvitationsResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::GetClanMemberList_ExtraInfo, MediusGetClanMemberList_ExtraInfoResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::AccountGetID, MediusAccountGetIDResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::AddToIgnoreList, MediusAddToIgnoreListResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::JoinChannel, MediusJoinChannelResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::TextFilter, MediusTextFilterResponse::process);
    mHandler->RegisterMediusMessageHandler(PacketInfo::MessageClassLobby, PacketInfo::Lobby::JoinGame, MediusJoinGameResponse::process);

    mHandler->RegisterRTMessageHandler(RT_MSG_SERVER_HELLO::Request, RT_MSG_SERVER_HELLO::process);
    mHandler->RegisterRTMessageHandler(RT_MSG_SERVER_CONNECT_ACCEPT_TCP::Request, RT_MSG_SERVER_CONNECT_ACCEPT_TCP::process);
    mHandler->RegisterRTMessageHandler(RT_MSG_SERVER_CONNECT_COMPLETE::Request, RT_MSG_SERVER_CONNECT_COMPLETE::process);
    mHandler->RegisterRTMessageHandler(RT_MSG_SERVER_ECHO::Request, RT_MSG_SERVER_ECHO::process);
    mHandler->RegisterRTMessageHandler(RT_MSG_CLIENT_DISCONNECT_WITH_REASON::Request, RT_MSG_CLIENT_DISCONNECT_WITH_REASON::process);
    mHandler->RegisterRTMessageHandler(RT_MSG_CLIENT_ECHO::Request, RT_MSG_CLIENT_ECHO::process);

    ring->SetMediusHandler(mHandler);
    ring->ServerLoop();

    return 0;
}
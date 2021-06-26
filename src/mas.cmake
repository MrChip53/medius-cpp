cmake_minimum_required(VERSION 3.19)

include(ExternalProject)

project(medius_mas_server)

set(CMAKE_CXX_STANDARD 20)

set(MASDIR ${CMAKE_CURRENT_LIST_DIR}/mas)
set(CMNDIR ${CMAKE_CURRENT_LIST_DIR}/common)

add_executable(medius_mas_server ${MASDIR}/main.cpp
                                  ${CMNDIR}/RTPacket.h
                                  ${CMNDIR}/IOuring.cpp
                                  ${CMNDIR}/IOuring.h
                                  ${CMNDIR}/Util.cpp
                                  ${CMNDIR}/Util.h
                                  ${CMNDIR}/MediusHandler.cpp
                                  ${CMNDIR}/MediusHandler.h
                                  ${CMNDIR}/RTMessages/RT_MSG_SERVER_HELLO.h
                                  ${CMNDIR}/Packets.h
                                  ${CMNDIR}/RTMessages/RT_MSG_SERVER_CONNECT_ACCEPT_TCP.h
                                  ${CMNDIR}/UserData.cpp
                                  ${CMNDIR}/UserData.h
                                  ${CMNDIR}/Messages.h
                                  ${CMNDIR}/RTMessages/RT_MSG_SERVER_CONNECT_COMPLETE.h
                                  ${CMNDIR}/RTMessages/RT_MSG_SERVER_ECHO.h ${CMNDIR}/MediusMessages/MediusUniverseVariableInformationResponse.h ${CMNDIR}/RTMessages/RT_MSG_CLIENT_DISCONNECT_WITH_REASON.h ${CMNDIR}/MediusMessages/MediusSessionBeginResponse.h ${CMNDIR}/PacketIds.h ${CMNDIR}/RTMessages/RT_MSG_CLIENT_ECHO.h ${CMNDIR}/MediusMessages/MediusSetLocalizationParamsResponse.h ${CMNDIR}/MediusMessages/MediusGetPolicyResponse.h ${CMNDIR}/MediusMessages/MediusAccountLoginResponse.h ${CMNDIR}/MediusMessages/MediusPacket42.h ${CMNDIR}/MediusMessages/MediusGetServerTimeResponse.h ${CMNDIR}/MediusMessages/MediusUpdateUserState.h ${CMNDIR}/MediusMessages/MediusSetGameListFilterResponse.h ${CMNDIR}/MediusMessages/MediusPlayerInfoResponse.h ${CMNDIR}/MediusMessages/MediusChannelList_ExtraInfoResponse.h ${CMNDIR}/MediusMessages/MediusGetBuddyInvitationsResponse.h ${CMNDIR}/MediusMessages/MediusAccountStatsRequest.h ${CMNDIR}/MediusMessages/MediusGetBuddyList_ExtraInfoResponse.h ${CMNDIR}/MediusMessages/MediusGetMyClansResponse.h ${CMNDIR}/MediusMessages/MediusUpdateLadderStatsWideRequest.h ${CMNDIR}/MediusMessages/MediusGetIgnoreListResponse.h ${CMNDIR}/MediusMessages/MediusCheckMyClanInvitationsResponse.h ${CMNDIR}/MediusMessages/MediusGetClanMemberList_ExtraInfoResponse.h ${CMNDIR}/MediusMessages/MediusAccountGetIDResponse.h ${CMNDIR}/MediusMessages/MediusAddToIgnoreListResponse.h ${CMNDIR}/MediusMessages/MediusJoinChannelResponse.h ${CMNDIR}/MediusMessages/MediusTextFilterResponse.h ${CMNDIR}/MediusMessages/MediusGameList_ExtraInfoResponse.h ${CMNDIR}/Game.h ${CMNDIR}/MediusMessages/MediusCreateGameResponse.h)

include_directories(${install_dir}/src/project_liburing/src/include)

target_link_libraries(medius_mas_server liburing)
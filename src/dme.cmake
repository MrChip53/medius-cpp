cmake_minimum_required(VERSION 3.19)
project(medius_dme_server)

set(CMAKE_CXX_STANDARD 20)

set(DMEDIR ${CMAKE_CURRENT_LIST_DIR}/dme)
set(CMNDIR ${CMAKE_CURRENT_LIST_DIR}/common)

add_executable(medius_dme_server ${DMEDIR}/main.cpp ${DMEDIR}/MediusMessageParser.cpp ${DMEDIR}/MediusMessageParser.h ${CMNDIR}/RTPacket.h ${DMEDIR}/World.cpp ${DMEDIR}/World.h ${CMNDIR}/database/DatabaseRepository.cpp ${CMNDIR}/database/DatabaseRepository.h ${CMNDIR}/database/models/UniverseModel.cpp ${CMNDIR}/database/models/UniverseModel.h ${CMNDIR}/IOuring.cpp ${CMNDIR}/IOuring.h ${CMNDIR}/Util.cpp ${CMNDIR}/Util.h ${CMNDIR}/MediusHandler.cpp ${CMNDIR}/MediusHandler.h ${CMNDIR}/RTMessages/RT_MSG_SERVER_HELLO.h ${CMNDIR}/Packets.h ${CMNDIR}/RTMessages/RT_MSG_SERVER_CONNECT_ACCEPT_TCP.h ${CMNDIR}/UserData.cpp ${CMNDIR}/UserData.h ${CMNDIR}/Messages.h ${CMNDIR}/RTMessages/RT_MSG_SERVER_CONNECT_COMPLETE.h ${CMNDIR}/RTMessages/RT_MSG_SERVER_ECHO.h ${CMNDIR}/MediusMessages/MediusUniverseVariableInformationResponse.h ${CMNDIR}/RTMessages/RT_MSG_CLIENT_DISCONNECT_WITH_REASON.h ${CMNDIR}/MediusMessages/MediusSessionBeginResponse.h ${CMNDIR}/PacketIds.h ${CMNDIR}/RTMessages/RT_MSG_CLIENT_ECHO.h ${CMNDIR}/MediusMessages/MediusSetLocalizationParamsResponse.h ${CMNDIR}/MediusMessages/MediusGetPolicyResponse.h ${CMNDIR}/MediusMessages/MediusAccountLoginResponse.h ${CMNDIR}/MediusMessages/MediusPacket42.h ${CMNDIR}/MediusMessages/MediusGetServerTimeResponse.h ${CMNDIR}/MediusMessages/MediusUpdateUserState.h)

target_link_libraries(medius_dme_server ${LIBUV_LIBRARIES})
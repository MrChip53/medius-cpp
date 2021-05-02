cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

set(DMEDIR ${CMAKE_CURRENT_LIST_DIR}/dme)
set(CMNDIR ${CMAKE_CURRENT_LIST_DIR}/common)

add_executable(medius_dme_server ${DMEDIR}/main.cpp ${DMEDIR}/MediusMessageParser.cpp ${DMEDIR}/MediusMessageParser.h ${CMNDIR}/RTPacket.h ${DMEDIR}/World.cpp ${DMEDIR}/World.h)

target_link_libraries(medius_dme_server ${LIBUV_LIBRARIES})
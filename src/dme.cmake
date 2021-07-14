cmake_minimum_required(VERSION 3.19)
project(medius_dme_server)

set(CMAKE_CXX_STANDARD 20)

set(DMEDIR ${CMAKE_CURRENT_LIST_DIR}/dme)
set(CMNDIR ${CMAKE_CURRENT_LIST_DIR}/common)

find_package (Threads)

add_executable(medius_dme_server ${DMEDIR}/main.cpp ${DMEDIR}/MediusMessageParser.cpp ${DMEDIR}/MediusMessageParser.h ${CMNDIR}/RTPacket.h ${DMEDIR}/World.cpp ${DMEDIR}/World.h)

target_link_libraries(medius_dme_server ${CMAKE_THREAD_LIBS_INIT})
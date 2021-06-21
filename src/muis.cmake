cmake_minimum_required(VERSION 3.19)

include(ExternalProject)

project(medius_muis_server)

set(CMAKE_CXX_STANDARD 20)

set(MUISDIR ${CMAKE_CURRENT_LIST_DIR}/muis)
set(CMNDIR ${CMAKE_CURRENT_LIST_DIR}/common)

ExternalProject_Add(project_liburing
        URL https://github.com/axboe/liburing/archive/refs/tags/liburing-2.0.tar.gz
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/liburing-2.0
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND ""
        PREFIX=${CMAKE_CURRENT_BINARY_DIR}/liburing-2.0
        )

ExternalProject_Get_Property(project_liburing install_dir)

message(STATUS "${${install_dir}}")

add_library(liburing STATIC IMPORTED)

set_property(TARGET liburing PROPERTY IMPORTED_LOCATION ${install_dir}/src/project_liburing/src/liburing.a)

add_dependencies(liburing project_liburing)

add_executable(medius_muis_server ${MUISDIR}/main.cpp
                                  ${CMNDIR}/RTPacket.h
                                  ${CMNDIR}/database/DatabaseRepository.cpp
                                  ${CMNDIR}/database/DatabaseRepository.h
                                  ${CMNDIR}/database/models/UniverseModel.cpp
                                  ${CMNDIR}/database/models/UniverseModel.h
                                  ${CMNDIR}/IOuring.cpp
                                  ${CMNDIR}/IOuring.h
                                  ${CMNDIR}/Util.cpp
                                  ${CMNDIR}/Util.h
                                  ${CMNDIR}/MediusHandler.cpp
                                  ${CMNDIR}/MediusHandler.h
                                  ${CMNDIR}/MediusMessages/RT_MSG_SERVER_HELLO.h
                                  ${CMNDIR}/Packets.h
                                  ${CMNDIR}/MediusMessages/RT_MSG_SERVER_CONNECT_ACCEPT_TCP.h
                                  ${CMNDIR}/UserData.cpp
                                  ${CMNDIR}/UserData.h ${CMNDIR}/MediusMessages/Messages.h)

include_directories(${install_dir}/src/project_liburing/src/include)

target_link_libraries(medius_muis_server liburing)
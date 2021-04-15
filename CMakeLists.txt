cmake_minimum_required(VERSION 3.19)
project(medius_dme_server)

set(CMAKE_CXX_STANDARD 20)

include(deps/uv.cmake)
set(LIBUV_LIBRARIES uv)

add_executable(medius_dme_server src/main.cpp)

target_link_libraries(medius_dme_server ${LIBUV_LIBRARIES})
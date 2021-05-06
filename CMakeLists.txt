cmake_minimum_required(VERSION 3.19)
project(medius_server)

set(CMAKE_CXX_STANDARD 20)

set(SRCDIR ${CMAKE_CURRENT_LIST_DIR}/src)

include(deps/uv.cmake)
set(LIBUV_LIBRARIES uv)

include(${SRCDIR}/dme.cmake)
include(${SRCDIR}/muis.cmake)
cmake_minimum_required(VERSION 3.19)
project(medius_server)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 20)

set(SRCDIR ${CMAKE_CURRENT_LIST_DIR}/src)

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

include(${SRCDIR}/dme.cmake)
include(${SRCDIR}/muis.cmake)
include(${SRCDIR}/mas.cmake)
include(${SRCDIR}/mls.cmake)
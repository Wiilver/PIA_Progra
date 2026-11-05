cmake_minimum_required(VERSION 3.11)

project(PIA_Progra)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

add_executable(PIA_Progra main.c)

target_link_libraries(PIA_Progra raylib)
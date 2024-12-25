cmake_minimum_required(VERSION 3.19)

project(select)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-g -O2 -Wall -Werror -Wno-unused)

include_directories(./)

file(GLOB_RECURSE SOURCES "utility/*.cpp" "socket/*.cpp")

add_executable(server ${SOURCES} server.cpp)
add_executable(client ${SOURCES} client.cpp)
add_executable(select_server ${SOURCES} select_server.cpp)

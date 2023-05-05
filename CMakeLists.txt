cmake_minimum_required(VERSION 3.23)
project(ConsoleB C)

set(CMAKE_C_STANDARD 99)
include_directories(inc)

add_executable(ConsoleB platform/win/main.c src/consoleb.c src/consoleb_user.c src/consoleb_platform.c)

if(WIN32)
    target_link_libraries(ConsoleB pthread)
endif(WIN32)

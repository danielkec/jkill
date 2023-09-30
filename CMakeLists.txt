cmake_minimum_required(VERSION 3.25.1)
project(jkill)

set(CMAKE_CXX_STANDARD 23)

set(CURSES_LIBRARY "/usr/lib/x86_64-linux-gnu/libncurses.so")
set(CURSES_INCLUDE_PATH "/opt/include")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(jkill main.cpp
        Process.cpp
        Process.h
        StringUtils.cpp
        StringUtils.h
        Menu.cpp
        Menu.h)
target_link_libraries(jkill ${CURSES_LIBRARIES})

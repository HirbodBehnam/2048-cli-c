cmake_minimum_required(VERSION 2.8.10) # Version from https://stackoverflow.com/a/41338282/4213397
project(2048-cli C)

set(CURSES_NEED_WIDE TRUE) # This is needed because of boxing chars
set(CMAKE_C_STANDARD 99)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(2048-cli main.c)
target_link_libraries(2048-cli ${CURSES_LIBRARIES})
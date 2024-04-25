cmake_minimum_required(VERSION 3.22)
project(chatRoom_cli C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(chatRoom_cli chatRoom_cli.c
        service/connect.c
      )



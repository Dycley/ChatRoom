cmake_minimum_required(VERSION 3.22)
project(chatRoom_client C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(chatRoom_client chatRoom_client.c
        service/connect.c
        common/cJSON.c
        common/common.c
        view/hello_UI.c
        include/view/hello_UI.h
        service/accountSrv.c
        include/service/accountSrv.h
        view/account_UI.c
        include/view/account_UI.h
        service/service.c
        include/service/service.h
        view/chat_UI.c
        include/view/chat_UI.h
        service/chatSrv.c
        include/service/chatSrv.h
)

file(COPY config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


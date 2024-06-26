cmake_minimum_required(VERSION 3.22)
project(chatRoom_server C)

set(CMAKE_C_STANDARD 11)

# 查找 SQLite3 库
find_library(SQLITE3_LIBRARY NAMES sqlite3 PATHS /path/to/sqlite3/lib)
find_package(Threads REQUIRED)

# 设置头文件搜索路径
include_directories(/path/to/sqlite3/include)

include_directories(include)


add_executable(chatRoom_server chatRoom_server.c
        service/connect.c
        include/common/global.h
        common/common.c
        include/common/common.h
        dao/db_connect.c
        include/dao/db_connect.h
        service/accountSrv.c
        dao/accountDao.c
        common/cJSON.c
        service/service.c
        include/service/service.h
        common/sha256.c
        include/common/sha256.h
        dao/chatDao.c
        include/dao/chatDao.h
        service/chatSrv.c
        include/service/chatSrv.h
)

file(COPY database/chatRoom.db DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/database)
file(COPY logs/log.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/log)
file(COPY config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 链接 SQLite3 库
target_link_libraries(chatRoom_server ${SQLITE3_LIBRARY})
target_link_libraries(chatRoom_server PRIVATE ${THREADS_LIBRARY})
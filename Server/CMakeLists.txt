cmake_minimum_required(VERSION 3.22)
project(chatRoom_srv C)

set(CMAKE_C_STANDARD 11)

# 查找 SQLite3 库
find_library(SQLITE3_LIBRARY NAMES sqlite3 PATHS /path/to/sqlite3/lib)
find_package(Threads REQUIRED)

# 设置头文件搜索路径
include_directories(/path/to/sqlite3/include)

include_directories(include)

add_executable(chatRoom_srv chatRoom_srv.c
        service/connect.c
        include/commom/global.h
        common/common.c
        include/commom/common.h
        dao/db_connect.c
        include/dao/db_connect.h
)

install(FILES database/chatRoom.db DESTINATION database)

# 链接 SQLite3 库
target_link_libraries(chatRoom_srv ${SQLITE3_LIBRARY})
target_link_libraries(chatRoom_srv PRIVATE ${THREADS_LIBRARY})
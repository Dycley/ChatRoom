cmake_minimum_required(VERSION 3.22)
project(chatRoom_srv C)

set(CMAKE_C_STANDARD 11)

# 查找 SQLite3 库
find_library(SQLITE3_LIBRARY NAMES sqlite3 PATHS /path/to/sqlite3/lib)

# 设置头文件搜索路径
include_directories(/path/to/sqlite3/include)

include_directories(include)

add_executable(chatRoom_srv chatRoom_srv.c
        service/connect.c)

# 链接 SQLite3 库
target_link_libraries(chatRoom_srv ${SQLITE3_LIBRARY})
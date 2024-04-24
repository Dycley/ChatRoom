cmake_minimum_required(VERSION 3.22)

# 设置项目名称和版本
project(ChatRoom VERSION 1.0)

# 指定C++标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# 添加子目录
add_subdirectory(Server)
add_subdirectory(Client)

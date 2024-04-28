# chatRoom
## Introduction
A multi-chatroom application based on Linux.  
[View the Project](https://github.com/Dycley/ChatRoom)

## Versions used
- C11  
- sqlite 3.45 
- cmake 3.22

## Quick Start
### Check the Environment
1. install cmake  
     ```shell
     sudo apt-get install cmake
     ```
1. install sqlite3  
     ```shell
     sudo apt-get install libsqlite3-dev
     ```
### Build
1. build  
   From the top-level directory of the source (which contains this file), run:
    ```shell
    mkdir build
    cd build
    cmake ..
    make
    ```
### Run
1. Run the server  
    ```shell
    ./Server/chatRoom_server
    ```

2. Run the client  
    ```shell
    ./Client/chatRoom_client
    ```

   

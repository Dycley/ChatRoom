# chatRoom
## Introduction
A multi-chatroom program based on Linux.  
[View the Project](https://github.com/Dycley/ChatRoom)

## Versions used
- C11  
- sqlite 3.45 
- cmake 3.22

## Quick Start
### Check the Environment
1. Install cmake  
     ```shell
     sudo apt-get install cmake
     ```
1. Install sqlite3  
     ```shell
     sudo apt-get install libsqlite3-dev
     ```
### Build
1. Build  
   From the top-level directory of the source (which contains this file), run:
    ```shell
    mkdir build
    cd build
    cmake ..
    make
    ```
### Run
1. Run the server  
   From the top-level directory of the source (which contains this file), run:
    ```shell
   cd build/Server
    ./chatRoom_server
    ```

2. Run the client  
   From the top-level directory of the source (which contains this file), run:
    ```shell
   cd build/Clinet
    ./chatRoom_client
    ```

   

//
// Created by dycley on 4/24/24.
//

#ifndef CHATROOM_GLOBAL_H
#define CHATROOM_GLOBAL_H

typedef struct online{
    int uid;
    int sock_fd;
    struct online *next;
} online_t;

enum MsgType {
    MESSAGE
};

#endif //CHATROOM_GLOBAL_H

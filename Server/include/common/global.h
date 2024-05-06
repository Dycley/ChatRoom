//
// Created by dycley on 4/24/24.
//

#ifndef CHATROOM_GLOBAL_H
#define CHATROOM_GLOBAL_H

#define MSG_LEN 1024

typedef struct online{
    int uid;
    int sock_fd;
    struct online *next;
} online_t;

/**
 * the json's format of MsgType
 * warning {type: MsgType, content: string, timestamp: int64}
 * error   {type: MsgType, content: string, timestamp: int64}
 * info    {type: MsgType, content: string, timestamp: int64}
 * message {type: MsgType, content: string, timestamp: int64}
 * file    {type: MsgType, filename: string, timestamp: int64, content: string}
 * operation{type: MsgType,operation: string, args[...]}
 */
enum MsgType {
    WARNING, ERROR, INFO, REQUEST, RESPONSE
};

#endif //CHATROOM_GLOBAL_H

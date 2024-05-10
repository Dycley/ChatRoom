//
// Created by dycley on 4/24/24.
//

#ifndef CHATROOM_GLOBAL_H
#define CHATROOM_GLOBAL_H

#define MSG_LEN 1024

#define MAX_ACCOUNT 50
typedef struct AccountStatus{
    int uid;
    int sock_fd;
    int status;
}Account;

enum MsgType {
    WARNING, ERROR, INFO, REQUEST, RESPONSE, DATA
};

#endif //CHATROOM_GLOBAL_H

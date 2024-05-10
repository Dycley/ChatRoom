//
// Created by dycley on 4/24/24.
//

#ifndef CHATROOM_GLOBAL_H
#define CHATROOM_GLOBAL_H

#define MSG_LEN 1024

typedef struct AccountOnline{
    int uid;
    char name[50];
    int sex;
    char *introduction;
} Account;

enum MsgType {
    WARNING, ERROR, INFO, REQUEST, RESPONSE, DATA
};

#endif //CHATROOM_GLOBAL_H

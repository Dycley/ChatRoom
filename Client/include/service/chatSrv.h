//
// Created by dycley on 5/8/24.
//

#ifndef CHATROOM_CHATSRV_H
#define CHATROOM_CHATSRV_H

#include "common/cJSON.h"

int Chat_Srv_Send_Msg(char *msg);

int Chat_Srv_Recv_Msg(cJSON *root);

int Chat_Srv_Send_File_Msg(char *filepath);

int Chat_Srv_Recv_File_Msg(cJSON *root);
#endif //CHATROOM_CHATSRV_H

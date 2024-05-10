//
// Created by dycley on 5/8/24.
//

#ifndef CHATROOM_CHATSRV_H
#define CHATROOM_CHATSRV_H

#include "common/cJSON.h"

int Chat_Srv_Send_Msg(char *msg);

int Chat_Srv_Recv_Msg(cJSON *root);

int Chat_Srv_Send_File(char *filepath);

int Chat_Srv_Recv_File_Msg(cJSON *root);

void Chat_Srv_Req_Download(const char *filename);

void Chat_Srv_Download(cJSON *root);

void downloadFile(int sock_fd, off_t fileSize, const char* directory, const char *filename);
#endif //CHATROOM_CHATSRV_H

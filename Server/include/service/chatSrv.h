//
// Created by dycley on 5/9/24.
//

#ifndef CHATROOM_CHATSRV_H
#define CHATROOM_CHATSRV_H

#include "common/cJSON.h"

int Chat_Srv_Data_Transmit(int sock_fd, char *json);

int Chat_Srv_Send_File(int sock_fd,const char *directory, const char *filename);

void downloadFile(int sock_fd, off_t fileSize,const char* directory, const char *filename);

#endif //CHATROOM_CHATSRV_H

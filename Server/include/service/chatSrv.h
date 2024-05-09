//
// Created by dycley on 5/9/24.
//

#ifndef CHATROOM_CHATSRV_H
#define CHATROOM_CHATSRV_H

#include "common/cJSON.h"

int Chat_Srv_Data_Transmit(int sock_fd, char *json);

#endif //CHATROOM_CHATSRV_H

//
// Created by dycley on 4/28/24.
//

#ifndef CHATROOM_ACCOUNT_H
#define CHATROOM_ACCOUNT_H

int Account_Srv_Register(int sock_fd, const char *json);

int Account_Srv_Login(int sock_fd, const char *json);
#endif //CHATROOM_ACCOUNT_H

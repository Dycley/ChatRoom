//
// Created by dycley on 5/2/24.
//

#ifndef CHATROOM_ACCOUNTSRV_H
#define CHATROOM_ACCOUNTSRV_H

#include "common/cJSON.h"

void Account_Srv_Register(const char *name, const char *password);

int Account_Srv_Register_Parse(cJSON* root);

void Account_Srv_Login_By_Uid(int uid, const char *password);

void Account_Srv_Login_By_Name(const char *name, const char *password);

int Account_Srv_Login_Parse(cJSON *root);
#endif //CHATROOM_ACCOUNTSRV_H

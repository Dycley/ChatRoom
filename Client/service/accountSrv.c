//
// Created by dycley on 5/2/24.
//

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include "service/accountSrv.h"
#include "common/cJSON.h"
#include "common/global.h"
#include "common/common.h"

Account account;

extern int sock_fd;

void Account_Clear() {
    account.uid = -1;
    memset(account.name,'\0',sizeof account.name);
    account.sex = -1;
    account.introduction = NULL;
}

void Account_Srv_Register(const char *name, const char *password) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", REQUEST);
    cJSON_AddStringToObject(root,"request","register");
    cJSON_AddStringToObject(root,"name",name);
    cJSON_AddStringToObject(root,"password",password);
    char *buf = cJSON_Print(root);
    if(send(sock_fd,buf,MSG_LEN,0)<=0){
        perror("Error on sending");
    }
    cJSON_free(root);
}



int Account_Srv_Register_Response(cJSON *root) {
    cJSON *item;
    item = cJSON_GetObjectItem(root,"status-code");
    int state = item -> valueint;
    switch (state) {
        case 0:
            item = cJSON_GetObjectItem(root,"uid");
            printf("注册成功，您的UID是: %d\n", item -> valueint);
            break;
        case 1:
            print_colored("yellow", "用户名已存在\n");
            break;
        case 2:
            print_colored("yellow", "注册失败\n");
            break;
        default:
            perror("Undefined status-code");
    }
    cJSON_free(item);
    return 0;
}

void Account_Srv_Login_By_Uid(int uid, const char *password) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", REQUEST);
    cJSON_AddStringToObject(root,"request","login");
    cJSON_AddStringToObject(root,"method","uid");
    cJSON_AddNumberToObject(root,"uid",uid);
    cJSON_AddStringToObject(root,"password",password);
    char *buf = cJSON_Print(root);
    if(send(sock_fd,buf,MSG_LEN,0)<=0){
        perror("Error on sending");
    }
    cJSON_free(root);
}

void Account_Srv_Login_By_Name(const char *name, const char *password) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"type", REQUEST);
    cJSON_AddStringToObject(root,"request","login");
    cJSON_AddStringToObject(root,"method","name");
    cJSON_AddStringToObject(root,"name",name);
    cJSON_AddStringToObject(root,"password",password);
    char *buf = cJSON_Print(root);
    cJSON_free(root);
    if(send(sock_fd,buf,MSG_LEN,0)<=0){
        perror("Error on sending");
    }
}


int Account_Srv_Login_Response(cJSON *root) {
    cJSON *item;
    item = cJSON_GetObjectItem(root,"status-code");
    int state = item -> valueint;
    switch (state) {
        case 0:
            item = cJSON_GetObjectItem(root,"name");
            printf("用户 %s, 欢迎您!\n", item->valuestring);
            strcpy(account.name, item->valuestring);
            item = cJSON_GetObjectItem(root,"uid");
            account.uid = item->valueint;
            break;
        case 1:
            print_colored("yellow", "用户名或密码错误\n");
            break;
        case 2:
            print_colored("yellow", "登录失败\n");
            break;
        default:
            perror("Undefined status-code");
    }
    cJSON_free(item);
    return 0;
}

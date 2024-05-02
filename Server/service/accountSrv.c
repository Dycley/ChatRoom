//
// Created by dycley on 4/28/24.
//

#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <stdio.h>
#include "service/connect.h"
#include "commom/List.h"
#include "commom/global.h"
#include "service/accountSrv.h"
#include "commom/cJSON.h"
#include "dao/accountDao.h"
#include "commom/common.h"

extern online_t *OnlineList;


int Account_Init();



int Account_Srv_Register(int sock_fd, const char *json) {
    char name[50], password[50];
    cJSON *root = cJSON_Parse(json);
    cJSON *item = cJSON_GetObjectItem(root, "name");
    strcpy(name,item->valuestring);
    item = cJSON_GetObjectItem(root, "password");
    strcpy(password,item->valuestring);
    cJSON_Delete(root);

    root =cJSON_CreateObject();
    if(Account_Register_By_Name_Pwd(name,password)!=-1){
        int uid = Account_Get_Uid_By_Name(name);
        char content[100];
        sprintf(content,"用户创建成功,您的uid是：%d",uid);
        cJSON_AddNumberToObject(root, "type", INFO);
        cJSON_AddNumberToObject(root,"timestamp",time(NULL));
        cJSON_AddStringToObject(root,"content", content);
        char *msg_json = cJSON_Print(root);
        if(send(sock_fd, (void *)msg_json,MSG_LEN,0)< 0){
            // 错误处理
            cJSON_free(root);
            return -1;
        }
    }else{
        char *content=Account_Get_Uid_By_Name(name)>=0 ? "用户名已存在\n":"注册失败\n";
        cJSON_AddNumberToObject(root, "type", WARNING);
        cJSON_AddNumberToObject(root,"timestamp",time(NULL));
        cJSON_AddStringToObject(root,"content",content);
        char *msg_json = cJSON_Print(root);
        if(send(sock_fd, (void *)msg_json,MSG_LEN,0) < 0){
            // 错误处理
            cJSON_free(root);
            return -1;
        }
    }
    cJSON_free(root);
    return 0;
}


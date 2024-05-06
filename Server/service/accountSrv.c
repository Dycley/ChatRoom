//
// Created by dycley on 4/28/24.
//

#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <stdio.h>
#include "service/connect.h"
#include "common/List.h"
#include "common/global.h"
#include "service/accountSrv.h"
#include "common/cJSON.h"
#include "dao/accountDao.h"
#include "common/common.h"

extern online_t *OnlineList;


int Account_Init();


int Account_Srv_Register(int sock_fd, const char *json) {
    char name[50], password[50];
    cJSON *root = cJSON_Parse(json);
    cJSON *item = cJSON_GetObjectItem(root,"request");
    if(strcmp(item->valuestring,"register")!=0){
        logs("Wrong function call: Account_Srv_Register");
        return -1;
    }
    item = cJSON_GetObjectItem(root, "name");
    strcpy(name,item->valuestring);
    item = cJSON_GetObjectItem(root, "password");
    strcpy(password,item->valuestring);
    cJSON_Delete(root);

    root =cJSON_CreateObject();
    if(Account_Register_By_Name_Pwd(name,password)>=0){
        int uid = Account_Get_Uid_By_Name(name);
//        char content[100];
//        sprintf(content,"用户创建成功,您的uid是：%d",uid);
        cJSON_AddNumberToObject(root, "type", RESPONSE);
        cJSON_AddStringToObject(root,"response","register");
        cJSON_AddNumberToObject(root,"status-code",0);
        cJSON_AddNumberToObject(root,"timestamp",time(NULL));
        cJSON_AddNumberToObject(root,"uid",uid);
        cJSON_AddStringToObject(root,"name",name);
//        cJSON_AddStringToObject(root,"content", content);
    }else{
        int state_code = Account_Get_Uid_By_Name(name) >= 0 ? 1 : 2;
//        char *content= state_code == 1? "用户名已存在\n":"注册失败\n";
        cJSON_AddNumberToObject(root, "type", RESPONSE);
        cJSON_AddStringToObject(root,"response","register");
        cJSON_AddNumberToObject(root, "status-code", state_code);
        cJSON_AddNumberToObject(root,"timestamp",time(NULL));
//        cJSON_AddStringToObject(root,"content",content);
    }
    char *msg_json = cJSON_Print(root);
    if(send(sock_fd, (void *)msg_json,MSG_LEN,0)< 0){
        // 错误处理
        cJSON_free(root);
        return -1;
    }
    cJSON_free(root);
    return 0;
}


int Account_Srv_Login(int sock_fd, const char *json) {
    char name[50], password[50];
    int uid;
    int state=0;
    cJSON *root = cJSON_Parse(json);
    cJSON *item = cJSON_GetObjectItem(root,"request");
    if(strcmp(item->valuestring,"login")!=0){
        logs("Wrong function call: Account_Srv_Login");
        return -1;
    }
    item = cJSON_GetObjectItem(root,"method");
    if(strcmp(item->valuestring,"uid")==0){
        item = cJSON_GetObjectItem(root,"uid");
        uid = item->valueint;
        char *tmp = Account_Get_Name_By_Uid(uid);
        if(tmp!=NULL)
            strcpy(name,tmp);
        else
            state = 1;
    }else if(strcmp(item->valuestring,"name")==0){
        item = cJSON_GetObjectItem(root, "name");
        strcpy(name,item->valuestring);
        if((uid = Account_Get_Uid_By_Name(name))<0)
            state = 1;
    }else{
        logs("Login method error: Account_Srv_Login");
        return -1;
    }

    item = cJSON_GetObjectItem(root, "password");
    strcpy(password,item->valuestring);
    cJSON_Delete(root);

    root =cJSON_CreateObject();
    int st;
    if(state != 0 || (st = Account_Check_Uid_Pwd(uid,password)) != 0){
//        char *content= (state > 0 || st > 0)? "账号或密码错误\n":"登录失败\n";
        int state_code = (state > 0 || st > 0)? 1 : 2;
        cJSON_AddNumberToObject(root, "type", RESPONSE);
        cJSON_AddStringToObject(root,"response","login");
        cJSON_AddNumberToObject(root,"status-code",state_code);
        cJSON_AddNumberToObject(root,"timestamp",time(NULL));
//        cJSON_AddStringToObject(root,"content",content);
        char *msg_json = cJSON_Print(root);
        if(send(sock_fd, (void *)msg_json,MSG_LEN,0) < 0){
            // 错误处理
            cJSON_free(root);
            return -1;
        }
    }else{
//        char content[100];
//        sprintf(content,"用户 %s，欢迎你!\n", name);
        cJSON_AddNumberToObject(root, "type", RESPONSE);
        cJSON_AddStringToObject(root,"response","login");
        cJSON_AddNumberToObject(root,"status-code",0);
        cJSON_AddNumberToObject(root,"uid",uid);
        cJSON_AddStringToObject(root,"name",name);
        cJSON_AddNumberToObject(root,"timestamp",time(NULL));
//        cJSON_AddStringToObject(root,"content", content);
        char *msg_json = cJSON_Print(root);
        if(send(sock_fd, (void *)msg_json,MSG_LEN,0)< 0){
            // 错误处理
            cJSON_free(root);
            return -1;
        }
    }
    cJSON_free(root);
    return 0;
}

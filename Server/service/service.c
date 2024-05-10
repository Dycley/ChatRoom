//
// Created by dycley on 5/3/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "common/common.h"
#include "common/global.h"
#include "common/cJSON.h"
#include "service/accountSrv.h"
#include "service/service.h"
#include "service/chatSrv.h"

#define LISTEN_NUM 12 //连接请求队列长度
#define MSG_LEN 1024

Account accountArr[MAX_ACCOUNT];
int account_cnt;
extern char file_save_dir[256];


void set_account_offline(int sock_fd) {
    for(int i=0;i<account_cnt;i++){
        if(accountArr[i].sock_fd==sock_fd){
            accountArr[i].status = 0;
            accountArr[i].sock_fd = -1;
            logs("Account who's uid: %d signed out",accountArr[i].uid);
            break;
        }
    }
}

void add_account(int uid, int sock_fd) {
    int i;
    for(i=0;i<account_cnt;i++){
        if(accountArr[i].uid == uid){
            accountArr[i].status = 1;
            break;
        }
    }
    if(i==account_cnt){
        accountArr[i].uid = uid;
        accountArr[i].sock_fd = sock_fd;
        accountArr[i].status = 1;
        account_cnt++;
    }
}

void *thread(void *arg) {
    int sock_fd=(int)(long)arg;
    int recv_size, ret;

    char buf[MSG_LEN];
    while(1){
        memset(buf, 0, sizeof buf);
        recv_size = 0;
        while(recv_size < MSG_LEN){
            if((ret = recv(sock_fd , buf + recv_size , MSG_LEN - recv_size, 0)) <= 0){
                set_account_offline(sock_fd);
//                perror("Error on receiving");
                return NULL;
            }
            recv_size += ret;
        }
//        printf("%s\n", buf);
        parse(sock_fd, buf);
    }
    free(arg);
    return NULL;
}

void parse(int sock_fd, char *buf) {
    cJSON *root, *item;
    root = cJSON_Parse(buf);
    item = cJSON_GetObjectItem(root,"type");
    switch (item -> valueint) {
        case ERROR:
            item = cJSON_GetObjectItem(root,"content");
            fprintf(stderr,"%s\n", cJSON_GetStringValue(item));
            // 错误信息
            break;
        case WARNING:
            item = cJSON_GetObjectItem(root,"content");
            print_colored("yellow","%s\n",cJSON_GetStringValue(item));
            // 警告信息
            break;
        case INFO:
            item = cJSON_GetObjectItem(root,"content");
            printf("%s\n",cJSON_GetStringValue(item));
            // 通知信息
            break;
        case REQUEST:
            item = cJSON_GetObjectItem(root,"request");
            if(strcmp(item -> valuestring, "register")==0){
                Account_Srv_Register(sock_fd,buf);
            }else if(strcmp(item -> valuestring, "login")==0){
                if(Account_Srv_Login(sock_fd, buf)>=0){
                    add_account(item->valueint, sock_fd);
                }
            }else if(strcmp(item -> valuestring, "download")==0){
                item = cJSON_GetObjectItem(root,"filename");
                char filepath[256];
                snprintf(filepath, sizeof(filepath), "%s/%s", file_save_dir, item->valuestring);
                Chat_Srv_Send_File(sock_fd, filepath);
            }
            break;
        case DATA:
            item = cJSON_GetObjectItem(root,"data");
            Chat_Srv_Data_Transmit(sock_fd,buf);
            break;
        case RESPONSE:
        default:
            logs("Received undefined message: %s", buf);
            perror("Undefined message\n");
    }
    cJSON_free(item);
    cJSON_free(root);
}



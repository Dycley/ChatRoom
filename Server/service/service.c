//
// Created by dycley on 5/3/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "common/List.h"
#include "common/common.h"
#include "common/global.h"
#include "common/cJSON.h"
#include "service/accountSrv.h"
#include "service/service.h"
#include "service/chatSrv.h"

#define LISTEN_NUM 12 //连接请求队列长度
#define MSG_LEN 1024


void *thread(void *arg) {
    int sock_fd=(int)(long)arg;
    int recv_size, ret;

    char buf[MSG_LEN];
    while(1){
        memset(buf, 0, sizeof buf);
        recv_size = 0;
        while(recv_size < MSG_LEN){
            if((ret = recv(sock_fd , buf + recv_size , MSG_LEN - recv_size, 0)) <= 0){

//                int uid = Account_Srv_ChIsOnline(-1 , 0 ,client_fd);
//                if(uid != -1){
//                    Account_Srv_SendIsOnline(uid ,0);
//                    //向在线好友发送下线通知
//                }
                perror("Error on receiving");
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
                Account_Srv_Login(sock_fd, buf);
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



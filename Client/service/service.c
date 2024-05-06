//
// Created by dycley on 5/3/24.
//

#include "service/service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "common/common.h"
#include "common/cJSON.h"
#include "common/global.h"
#include "service/accountSrv.h"

#define MSG_LEN 1024
extern int sock_fd;
char message[MSG_LEN];
cJSON *root, *item;

void *thread(void *arg) {
    sock_fd=(int)(long)arg;
    int recv_size, ret;

    while(1){
        memset(message, 0, sizeof message);
        recv_size = 0;
        while(recv_size < MSG_LEN){
            if((ret = recv(sock_fd ,message + recv_size ,MSG_LEN - recv_size,0)) <= 0){
                fprintf(stderr, "\n服务器开小差了(；′⌒`)\n");
                exit(0);
            }
            recv_size += ret;
        }
//        printf("%s\n",message);
        parse(message);
    }
    free(arg);
    return NULL;
}



void parse(char *buf) {
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
        case RESPONSE:
            parse_response();
            break;
        case REQUEST:
        default:
            logs("Received undefined message: %s",buf);
            perror("Undefined message\n");
    }
}



void parse_response() {
    item = cJSON_GetObjectItem(root, "response");
    if(strcmp(item->valuestring, "register")==0){
        Account_Srv_Register_Parse(root);
    }else if(strcmp(item->valuestring, "login")==0){
        Account_Srv_Login_Parse(root);
    }
//    if(strcmp(item->valuestring, "message")!=0){
//        item = cJSON_GetObjectItem(root,"status-code");
//        int state = item -> valueint;
//        item = cJSON_GetObjectItem(root,"content");
//        if(!state){
//            printf("%s\n",cJSON_GetStringValue(item));
//        }else{
//            print_colored("yellow", cJSON_GetStringValue(item));
//        }
//    }
}

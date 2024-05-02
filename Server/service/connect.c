//
// Created by dycley on 4/24/24.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "service/connect.h"
#include "commom/List.h"
#include "commom/common.h"
#include "commom/global.h"
#include "commom/cJSON.h"
#include "service/accountSrv.h"

#define LISTEN_NUM 12 //连接请求队列长度
#define MSG_LEN 1024

online_t *OnlineList;

void * thread(void *arg){
    int sock_fd=(int)(long)arg;
    int recv_size, ret;
    cJSON *root, *item;
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
                perror("\nError on receiving\n");
                return NULL;
            }
            recv_size += ret;
        }
//        printf("%s\n", buf);
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
            case OPERATION:
                item = cJSON_GetObjectItem(root,"operation");
                if(strcmp(item -> valuestring, "register")==0){
                    Account_Srv_Register(sock_fd,buf);
                }
                break;
            default:
                logs("Received undefined message: %s", buf);
                perror("Undefined message\n");
        }
    }
    free(arg);
    return NULL;
}

void connect_cli(int port) {
    int sock_fd;
    int client_fd;
    int len;
    int optval;
    List_Init(OnlineList , online_t);
    struct sockaddr_in serv_addr , client_addr;
    len = sizeof(struct sockaddr_in);
    memset(&serv_addr , 0 ,len);
    memset(&client_addr , 0 , len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_fd = socket(AF_INET , SOCK_STREAM , 0);
    if(sock_fd < 0) {
        error("Error opening socket");
    }
    optval = 1;
    if(setsockopt(sock_fd , SOL_SOCKET , SO_REUSEADDR , (void *)&optval , sizeof(int)) < 0){
        error("Error on socksetopt");
    }
    if(bind(sock_fd , (struct sockaddr *)&serv_addr , len) < 0){
        error("Error on binding");
    }

    printf("listening on the port: %d\n", port);
    if(listen(sock_fd , LISTEN_NUM) < 0){
        error("Error on listening");
    }

    while(1){
        client_fd = accept(sock_fd , (struct sockaddr *)&client_addr , (socklen_t *)&len);
        if(client_fd < 0) {
            error("Error on accept\n");
        }
        void* addr=&(client_addr.sin_addr);
        char *client_ip = inet_ntoa(*(struct in_addr *)addr);
        printf("Client: %s is connecting\n", client_ip);
        logs("Client: %s connected", client_ip);
        pthread_t thid;
        pthread_create(&thid , NULL , thread ,(void *)(long)client_fd);
    }
}


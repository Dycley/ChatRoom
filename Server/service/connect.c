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
#include "../include/service/connect.h"
#include "../include/commom/List.h"
#include "../include/commom/common.h"
#include "../include/commom/global.h"

#define LISTEN_NUM 12 //连接请求队列长度
#define MSG_LEN 1024

online_t *OnlineList;

void * thread(void *arg){
    char buf[MSG_LEN];
    int ret ,recv_len;
    int client_fd = (int)(long)arg;
    while(1){
        recv_len = 0;
        while(recv_len < MSG_LEN){
            ret = 0;
            if((ret = recv(client_fd , buf + recv_len , MSG_LEN - recv_len , 0)) <= 0){
//                int uid = Account_Srv_ChIsOnline(-1 , 0 ,client_fd);
//                if(uid != -1){
//                    Account_Srv_SendIsOnline(uid ,0);
//                    //向在线好友发送下线通知
//                }
                perror("recv");
                return NULL;
            }
            recv_len += ret;
            printf("356%s\n", buf);
        }
        printf("123%s\n", buf);

    }
    return ((void *)0);
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
            error("Error on accept");
        }
        void* addr=&(client_addr.sin_addr);
        char *client_ip = inet_ntoa(*(struct in_addr *)addr);
        printf("Client: %s is connecting\n", client_ip);
        pthread_t thid;
        pthread_create(&thid , NULL , thread ,(void *)(long)client_fd);
    }
}

void Connect() {
    printf("connect_srv\n");
}

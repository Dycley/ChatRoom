//
// Created by dycley on 4/24/24.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "service/connect.h"
#include "common/common.h"
#include "common/cJSON.h"
#include "common/global.h"

#define MSG_LEN 1024
int sock_fd;
//pthread_mutex_t mutex;
//pthread_cond_t cond;
extern int my_mutex;
char message[MSG_LEN];

void * thread(void *arg){
    sock_fd=(int)(long)arg;
    int recv_size, ret;
    cJSON *root, *item;
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
        root = cJSON_Parse(message);
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
            default:
                logs("Received undefined message: %s",message);
                perror("Undefined message\n");
        }
    }
    free(arg);
    return NULL;
}


void connect_server(const char *host, int port) {
    struct sockaddr_in clie_addr;
    //char *out;
    pthread_t thid;
    //   pthread_mutex_init(&mutex ,NULL);
    // pthread_cond_init(&cond ,NULL);
    memset(&clie_addr , 0 ,sizeof(struct sockaddr_in));
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(port);
    clie_addr.sin_addr.s_addr = inet_addr(host);
    sock_fd = socket(AF_INET , SOCK_STREAM , 0);

    if(sock_fd < 0){
        error("Error on socket");
    }

    if(connect(sock_fd , (struct sockaddr *)&clie_addr,sizeof(struct sockaddr_in)) < 0){
        error("Error on connect");
    }

    if(pthread_create(&thid ,NULL ,thread ,(void *)(long)sock_fd)>0){
        error("Error on create thread");
    }
//    pthread_join(thid,NULL);
    //usleep(5000);
    //pthread_cond_signal(&cond);
}

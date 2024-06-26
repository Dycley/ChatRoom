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
#include "service/service.h"

//#define MSG_LEN 1024
int sock_fd;
//char message[MSG_LEN];


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

//
// Created by dycley on 5/2/24.
//

#include <stdio.h>
#include <sys/socket.h>
#include "service/accountSrv.h"
#include "common/cJSON.h"
#include "common/global.h"

extern int sock_fd;

void Account_Srv_Register(const char *name, const char *password) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"type",OPERATION);
    cJSON_AddStringToObject(root,"operation","register");
    cJSON_AddStringToObject(root,"name",name);
    cJSON_AddStringToObject(root,"password",password);
    char *buf = cJSON_Print(root);
    cJSON_free(root);
//    printf("%s",buf);
    if(send(sock_fd,buf,MSG_LEN,0)<=0){
        perror("Error on sending\n");
    }
}

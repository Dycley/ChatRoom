//
// Created by dycley on 5/9/24.
//

#include <string.h>
#include <sys/socket.h>
#include "service/chatSrv.h"
#include "common/global.h"
#include "common/cJSON.h"
#include "common/common.h"
#include "dao/chatDao.h"
#include "dao/accountDao.h"


int Chat_Srv_Data_Transmit(int sock_fd, char *json) {
    int msgtype, timestamp, uid;
    char message[500];
    cJSON *root = cJSON_Parse(json);
    cJSON *item = cJSON_GetObjectItem(root,"datatype");
    if(strcmp(item->valuestring,"message")==0){
        msgtype = 0;
        item = cJSON_GetObjectItem(root, "timestamp");
        timestamp = item -> valueint;
        item = cJSON_GetObjectItem(root, "uid");
        uid = item -> valueint;
        item = cJSON_GetObjectItem(root, "message");
        strcpy(message,item->valuestring);
        char *name = Account_Get_Name_By_Uid(uid);
        cJSON_AddStringToObject(root,"name",name);
    }else if(strcmp(item->valuestring,"file")==0){
        msgtype = 1;
        item = cJSON_GetObjectItem(root, "timestamp");
        timestamp = item -> valueint;
        item = cJSON_GetObjectItem(root, "uid");
        uid = item -> valueint;
        item = cJSON_GetObjectItem(root, "filename");
        strcpy(message,item->valuestring);
        char *name = Account_Get_Name_By_Uid(uid);
        cJSON_AddStringToObject(root,"name",name);
    }else{
        logs("Wrong function call: Chat_Srv_Data_Transmit");
        return -1;
    }

    Chat_Save_Msg(uid,msgtype,message,timestamp);

    json = cJSON_Print(root);

    if(send(sock_fd,(void *)json,MSG_LEN,0)<0){
        return -1;
    }

    cJSON_free(item);
    cJSON_free(root);
    return 0;
}

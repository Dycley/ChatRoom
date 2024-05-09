//
// Created by dycley on 5/8/24.
//

#include <sys/socket.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include "service/chatSrv.h"
#include "common/global.h"
#include "common/cJSON.h"
#include "common/common.h"
#include "sys/sendfile.h"

extern Account account;
extern int sock_fd;


int Chat_Srv_Send_Msg(char *msg) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", DATA);
    cJSON_AddStringToObject(root,"datatype","message");
    cJSON_AddNumberToObject(root,"timestamp",time(NULL));
    cJSON_AddNumberToObject(root,"uid",account.uid);
    cJSON_AddStringToObject(root,"message",msg);
    char *buf = cJSON_Print(root);
    if(send(sock_fd,buf,MSG_LEN,0)<=0){
        perror("Error on sending");
    }
    cJSON_free(root);
    return 0;
}

extern int isInRoom;
int Chat_Srv_Recv_Msg(cJSON *root) {
    if(!isInRoom) return -1;
    cJSON *item;
    int uid;
    char name[50];
    item = cJSON_GetObjectItem(root,"uid");
    uid = item -> valueint;
    item = cJSON_GetObjectItem(root,"name");
    strcpy(name,item -> valuestring);
    item = cJSON_GetObjectItem(root,"message");
    char *message = item -> valuestring;
    clean_last_line();
    print_colored("1;35","%s  ", curTime());
    print_colored("1;32","%s\t",name);
    printf("%s\n\n", message);
    print_colored("cyan","功能: /r 返回上一级 /u 上传文件 /d 下载文件 /e 退出\n");
    return  0;
}

int Chat_Srv_Send_File_Msg(char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if(file == NULL) {
        print_colored("red","无法打开文件！\n");
        return -1;
    }
    long filesize = get_file_size(file);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", DATA);
    cJSON_AddStringToObject(root,"datatype","file");
    cJSON_AddNumberToObject(root,"timestamp",time(NULL));
    cJSON_AddNumberToObject(root,"uid",account.uid);
    cJSON_AddStringToObject(root,"filename",basename(filepath));
    cJSON_AddNumberToObject(root,"filesize",filesize);
    char *buf = cJSON_Print(root);
    if(send(sock_fd,buf,MSG_LEN,0)<=0){
        perror("Error on sending");
    }
    cJSON_free(root);
    fclose(file);
    return 0;
}

int Chat_Srv_Recv_File_Msg(cJSON *root) {
    if(!isInRoom) return -1;
    cJSON *item;
    char name[50];
    int uid,filesize;
    item = cJSON_GetObjectItem(root,"uid");
    uid = item -> valueint;
    item = cJSON_GetObjectItem(root,"filesize");
    filesize = item -> valueint;
    item = cJSON_GetObjectItem(root,"name");
    strcpy(name,item -> valuestring);
    item = cJSON_GetObjectItem(root,"filename");
    char *filename = item -> valuestring;
    clean_last_line();
    print_colored("1;35","%s  ", curTime());
    print_colored("1;32","%s\t",name);
    print_colored("yellow","上传了文件: %s 大小: %d\n\n", filename ,filesize);
    print_colored("cyan","功能: /r 返回上一级 /u 上传文件 /d 下载文件 /e 退出\n");
    return  0;
}

//int upload_file(int sock, int size, char *filepath){
//    FILE *fp = fopen(filepath, "r");
//    if(fp == NULL){
//        perror("Error on opening file");
//        return -1;
//    }
//
//}
//
//void download_file(){
//
//}
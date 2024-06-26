//
// Created by dycley on 5/9/24.
//

#include <string.h>
#include <sys/socket.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <time.h>
#include <libgen.h>
#include "service/chatSrv.h"
#include "common/global.h"
#include "common/cJSON.h"
#include "common/common.h"
#include "dao/chatDao.h"
#include "dao/accountDao.h"
#include "service/service.h"

extern Account accountArr[MAX_ACCOUNT];
extern int account_cnt;
extern char file_save_dir[256];

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
        item = cJSON_GetObjectItem(root, "filesize");
        off_t filesize = item -> valueint;
        downloadFile(sock_fd,filesize,file_save_dir,message);
    }else{
        logs("Wrong function call: Chat_Srv_Data_Transmit");
        return -1;
    }

    Chat_Save_Msg(uid,msgtype,message, timestamp);
    json = cJSON_Print(root);
    for(int i=0;i<account_cnt;i++){
//        printf("uid %d st%d fd%d\n",accountArr[i].uid,accountArr[i].status,accountArr[i].sock_fd);
        if (accountArr[i].status)
            if(send(accountArr[i].sock_fd,(void *)json,MSG_LEN,0)<0){
    //            return -1;
            }
    }

    cJSON_free(item);
    cJSON_free(root);
    return 0;
}


int Chat_Srv_Send_File(int sock_fd, char *filepath) {
    // 打开待传输的文件
    int file_fd = open(filepath, O_RDONLY);
    if (file_fd == -1) {
        print_colored("red","无法打开文件！\n");
        char *warning_buf = "{\"type\": 0, \"content\":\"无此文件！\"}";
        if(send(sock_fd,warning_buf,MSG_LEN,0)<=0){
            perror("Send error");
        }
//        close(sock_fd);
//        exit(1);
        return -1;
    }

    // 获取文件大小
    struct stat fileInfo;
    if (fstat(file_fd, &fileInfo) == -1) {
        perror("Error getting the file size");
        close(file_fd);
//        close(sock_fd);
//        exit(1);
        return -1;
    }
    off_t filesize = fileInfo.st_size;

    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", DATA);
    cJSON_AddStringToObject(root,"datatype","file-download");
    cJSON_AddNumberToObject(root,"timestamp",time(NULL));
    cJSON_AddStringToObject(root,"filename",basename(filepath));
    cJSON_AddNumberToObject(root,"filesize",filesize);
    char *buf = cJSON_Print(root);
    if(send(sock_fd,buf,MSG_LEN,0)<=0){
        perror("Error on sending");
    }
    cJSON_free(root);

    // 发送文件内容
    off_t offset = 0; // 用于sendfile函数的偏移量
    ssize_t bytesSent = 0;
    while (offset < filesize) {
        bytesSent = sendfile(sock_fd, file_fd, &offset, filesize - offset);
        if (bytesSent == -1) {
            perror("send file content failed");
            close(file_fd);
            close(sock_fd);
//            exit(1);
            return -1;
        }
    }

    return 0;
}


void downloadFile(int sock_fd, off_t fileSize, const char* directory, const char *filename) {
    size_t iResult;

    // 接收文件内容
    char* recvBuffer = malloc(fileSize);
    if (recvBuffer == NULL) {
        perror("Memory allocation failed");
        close(sock_fd);
        exit(1);
    }
    int bytesReceived = 0;
    while (bytesReceived < fileSize) {
        iResult = recv(sock_fd, recvBuffer + bytesReceived, fileSize - bytesReceived, 0);
        if (iResult == -1) {
            logs("recv file content failed");
            free(recvBuffer);
//            close(sock_fd);
//            exit(1);
        }
        bytesReceived += iResult;
    }
    char filepath[256];
    // 检查目录是否存在，如果不存在则创建
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0700);
    }
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // 将接收到的文件保存在服务器本地
    int filefd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (filefd == -1) {
        perror("Failed to create file");
        free(recvBuffer);
//        close(sock_fd);
//        exit(1);
    }
    write(filefd, recvBuffer, fileSize);
    close(filefd);

    logs("File \"%s\" saved successfully.\n", filename);

    // 关闭连接
    free(recvBuffer);
//    close(sock_fd);
}

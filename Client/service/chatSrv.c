//
// Created by dycley on 5/8/24.
//

#include <sys/socket.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include "service/chatSrv.h"
#include "common/global.h"
#include "common/cJSON.h"
#include "common/common.h"
#include "sys/sendfile.h"

extern Account account;
extern int sock_fd;
extern char download_dir[256];

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
    print_colored("1;32",uid == account.uid? "%s (我)\t":"%s\t",name);
    printf("%s\n\n", message);
    print_colored("cyan","输入消息并按回车以发送 菜单选项: /r 返回上一级 /u 上传文件 /d 下载文件 /e 退出\n");
    return  0;
}

int Chat_Srv_Send_File(char *filepath) {
    // 打开待传输的文件
    int file_fd = open(filepath, O_RDONLY);
    if (file_fd == -1) {
        print_colored("red","无法打开文件！\n");
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
    print_colored("1;32",uid == account.uid? "%s (我)\t":"%s\t", name);
    print_colored("yellow","上传了文件: %s 大小: %d\n\n", filename ,filesize);
    print_colored("cyan","输入消息并按回车以发送 菜单选项: /r 返回上一级 /u 上传文件 /d 下载文件 /e 退出\n");
    return  0;
}

void Chat_Srv_Req_Download(const char *filename) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", REQUEST);
    cJSON_AddStringToObject(root,"request","download");
    cJSON_AddStringToObject(root,"filename",filename);
    char *buf = cJSON_Print(root);
    if(send(sock_fd,buf,MSG_LEN,0)<=0){
        perror("Error on sending");
    }
    cJSON_free(root);
}


void Chat_Srv_Download(cJSON *root) {
    cJSON *item;
    off_t filesize;
    item = cJSON_GetObjectItem(root,"filesize");
    filesize = item->valueint;
    item = cJSON_GetObjectItem(root,"filename");
    char *filename = item -> valuestring;
    print_colored("blue","正在下载文件...\n");
    downloadFile(sock_fd, filesize,download_dir, filename);
    unlock();
    cJSON_free(root);
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

    print_colored("yellow","文件 \"%s\" 下载完成!\n", filename);

    // 关闭连接
    free(recvBuffer);
//    close(sock_fd);
}


//void downloadFile(int sock_fd, const char *directory, const char *filename) {
//    // 接收文件大小信息
//    off_t fileSize;
//    ssize_t iResult = recv(sock_fd, &fileSize, sizeof(fileSize), 0);
//    if (iResult == -1) {
//        perror("recv file size failed");
//        close(sock_fd);
//        exit(1);
//    }
//
//    // 接收文件内容
//    char* recvBuffer = malloc(fileSize);
//    if (recvBuffer == NULL) {
//        perror("Memory allocation failed");
//        close(sock_fd);
//        exit(1);
//    }
//    int bytesReceived = 0;
//    while (bytesReceived < fileSize) {
//        iResult = recv(sock_fd, recvBuffer + bytesReceived, fileSize - bytesReceived, 0);
//        if (iResult == -1) {
//            logs("recv file content failed");
//            free(recvBuffer);
////            close(sock_fd);
////            exit(1);
//        }
//        bytesReceived += iResult;
//    }
//    char filepath[256];
//    // 检查目录是否存在，如果不存在则创建
//    struct stat st = {0};
//    if (stat(directory, &st) == -1) {
//        mkdir(directory, 0700);
//    }
//    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
//
//    // 将接收到的文件保存在服务器本地
//    int filefd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
//    if (filefd == -1) {
//        perror("Failed to create file");
//        free(recvBuffer);
////        close(sock_fd);
////        exit(1);
//    }
//    write(filefd, recvBuffer, fileSize);
//    close(filefd);
//
//    print_colored("yellow","文件 \"%s\" 下载完成!\n", filename);
//
//    // 关闭连接
//    free(recvBuffer);
////    close(sock_fd);
//}

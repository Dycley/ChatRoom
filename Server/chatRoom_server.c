//
// Created by dycley on 4/23/24.
//
#include<stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "include/service/connect.h"
#include "include/dao/db_connect.h"
#include "common/common.h"
#include "common/cJSON.h"

char file_save_dir[256];

int main(){
    printf("Hello Server\n");
    FILE *fp = fopen("config.json", "r");
    if(fp == NULL){
        error("Error on opening file \"config.json\"");
    }
    char buf[200] = {0};
    while (!feof(fp)) //没有到文件末尾
    {
        memset(buf, 0, sizeof(buf));
        size_t len = fread(buf, sizeof(char), sizeof(buf), fp);
    }
    fclose(fp);
    cJSON* root = cJSON_Parse(buf);
    cJSON* item;
    item = cJSON_GetObjectItem(root,"file_save_dir");
    strcpy(file_save_dir,item->valuestring);
    item = cJSON_GetObjectItem(root,"port");
    int port = item->valueint;
    cJSON_free(root);

    db_connect("database/chatRoom.db");
    connect_client(port);
    db_close();
    return 0;
}
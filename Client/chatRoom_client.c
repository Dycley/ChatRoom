//
// Created by dycley on 4/23/24.
//
#include <stdio.h>
#include <string.h>
#include "service/connect.h"
#include "common/cJSON.h"
#include "common/common.h"
#include "view/hello_UI.h"

int main(){
    printf("Hello Client\n");
    FILE *fp = fopen("config.json", "r");
    if(fp == NULL){
        error("Error on opening file \"config.json\"\n");
    }
    char buf[100] = {0};
    while (!feof(fp)) //没有到文件末尾
    {
        memset(buf, 0, sizeof(buf));
        size_t len = fread(buf, sizeof(char), sizeof(buf), fp);
    }
    fclose(fp);
    cJSON* root = cJSON_Parse(buf);
    cJSON* item;
    item = cJSON_GetObjectItem(root,"host");
    char host[50];
    strcpy(host,item->valuestring);
    item = cJSON_GetObjectItem(root,"port");
    int port = item->valueint;
    cJSON_free(root);

    connect_server(host, port);
    Hello_UI();
    return 0;
}
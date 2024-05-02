//
// Created by dycley on 4/23/24.
//
#include<stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include "include/service/connect.h"
#include "include/dao/db_connect.h"
#include "commom/common.h"

int main(){
    printf("Hello Server\n");
    db_connect("database/chatRoom.db");
//    db_close();
    connect_cli(8888);
    return 0;
}
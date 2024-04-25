//
// Created by dycley on 4/23/24.
//
#include<stdio.h>
#include "include/service/connect.h"

int main(){
    printf("Hello Client\n");
    connect_srv("127.0.0.1", 8888);
    return 0;
}
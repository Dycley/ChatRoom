//
// Created by dycley on 5/2/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "view/hello_UI.h"
#include "common/common.h"
#include "service/accountSrv.h"
#include "view/account_UI.h"

int isInRoom;

void Init_UI() {
    system("clear");
    isInRoom = 0;
    print_colored("green","+++++++++++++++++++++++++++\n");
    print_colored("blue","+ Welcome to the chatRoom +\n");
    print_colored("pink","+\t 欢迎使用小煤球聊天室 \t  +\n");
    print_colored("green","+++++++++++++++++++++++++++\n");
}

void Hello_UI() {
    Init_UI();
    Account_Clear();
    int choice;
    while (1) {
        print_colored("cyan","功能选项:\n");
        print_colored("green",
                      "\t1. 登录\n"
                      "\t2. 注册\n"
                      "\t3. 退出\n");
        print_colored("cyan","请输入您的操作: ");
        scanf("%d", &choice);
        clear_input_buffer();
        switch (choice) {
            case 1:
                Login_UI();
                return;
            case 2:
                Register_UI();
                return;
            case 3:
                return;
            default:
                print_colored("red","输入错误!\n");
        }
    }
}




//
// Created by dycley on 5/2/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "hello_UI.h"
#include "common/common.h"
#include "service/accountSrv.h"

void Init_UI(){
    system("clear");
    print_colored("green","+++++++++++++++++++++++++++\n");
    print_colored("blue","+ Welcome to the chatRoom +\n");
    print_colored("pink","+  \t 欢迎使用小煤球聊天室 \t  +\n");
    print_colored("green","+++++++++++++++++++++++++++\n");
}

void Hello_UI() {
    Init_UI();
    int choice;
    while (1) {
        printf("功能选项:\n");
        print_colored("green",
                       "\t1. 登录\n"
                             "\t2. 注册\n"
                             "\t3. 退出\n");
        print_colored("cyan","请输入您的操作: ");
        scanf("%d", &choice);
        clear_input_buffer();
        switch (choice) {
            case 1:
                Register_UI();
                return;
            case 2:
                Register_UI();
                return;
            case 3:
                return;
                break;
            default:
                print_colored("red","输入错误!\n");
        }
    }
}

void Register_UI() {
    Init_UI();
    char name[50], password[50], repeat_password[50], ch;
    printf("注册账号\n"
           "---------------------------\n");

    while(1) {
        print_colored("cyan","请输入昵称：");
        scanf("%49s",name);
        clear_input_buffer();
        set_echo(0);
        while (1) {
            print_colored("cyan", "请输入密码：");
            scanf("%49s", password);
            clear_input_buffer();
            print_colored("cyan", "\n请重新输入密码：");
            scanf("%49s", repeat_password);
            clear_input_buffer();

            if(strcmp(password,repeat_password)!=0){
                print_colored("red", "\n两次输入密码不同，请重试\n");
            }else{
                break;
            }
        }
        set_echo(1);

        do {
            print_colored("yellow", "\n确认注册(Y/N)?\n");
            scanf("%c", &ch);
            clear_input_buffer();
            switch (ch) {
                case 'Y':
                case 'y':
                    Account_Srv_Register(name,password);
                    sleep(1);
                case 'N':
                case 'n':
                    print_colored("2","按下回车键返回...");
                    getchar();
                    Hello_UI();
                    return;
                default:
                    print_colored("red", "请输入(Y/N)");
            }
        } while (ch!='Y'&&ch!='y'&&ch!='N'&&ch!='n');
    }
}

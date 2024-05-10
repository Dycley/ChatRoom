//
// Created by dycley on 5/3/24.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "view/account_UI.h"
#include "view/hello_UI.h"
#include "common/common.h"
#include "service/accountSrv.h"
#include "common/global.h"
#include "view/chat_UI.h"

extern Account account;

void Register_UI() {
    Init_UI();
    char name[50], password[50], repeat_password[50], ch;
    printf("注册账号\n"
           "---------------------------\n");

    while(1) {
        print_colored("cyan","请输入用户名：");
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
                    lock();
                    sleep(1);
                case 'N':
                case 'n':
                    print_colored("2","按下回车键返回...");
                    clear_input_buffer();
                    Hello_UI();
                    return;
                default:
                    print_colored("red", "请输入(Y/N)");
            }
        } while (ch!='Y'&&ch!='y'&&ch!='N'&&ch!='n');
    }
}


void Login_UI() {
    Init_UI();
    int choice;
    int uid;
    char name[50],password[50];
    printf("登录账号\n"
           "---------------------------------------\n");
    while (1){
        print_colored("cyan", "请选择登录方式\n");
        print_colored("green",
                "\t1. 账号登录\n"
                      "\t2. 用户名登录\n"
                      "\t3. 返回主界面\n");
        print_colored("cyan", "请输入你的操作:");
        scanf("%d", &choice);
        clear_input_buffer();
        switch (choice) {
            case 1:
                print_colored("cyan","输入账号:");
                scanf("%d",&uid);
                clear_input_buffer();
                print_colored("cyan","输入密码:");
                set_echo(0);
                scanf("%49s",password);
                set_echo(1);
                clear_input_buffer();
                printf("\n");
                Account_Srv_Login_By_Uid(uid,password);
                lock();
                sleep(1);
                print_colored("2","按下回车键继续...");
                clear_input_buffer();
                if(account.uid>0)
                    Account_Menu_UI();
                else
                    Hello_UI();
                return;
            case 2:
                print_colored("cyan","输入用户名:");
                scanf("%s", name);
                clear_input_buffer();
                print_colored("cyan","输入密码:");
                set_echo(0);
                scanf("%49s",password);
                set_echo(1);
                clear_input_buffer();
                printf("\n");
                Account_Srv_Login_By_Name(name,password);
                lock();
                sleep(1);
                print_colored("2","按下回车键继续...");
                clear_input_buffer();
                if(account.uid>0)
                    Account_Menu_UI();
                else
                    Hello_UI();
                return;
            case 3:
                Hello_UI();
                return;
            default:
                print_colored("red","输入错误!\n");
        }
    }
}



void Account_Menu_UI() {
    Init_UI();
    int choice;
    print_colored("yellow","欢迎您, %s!\n", account.name);
    printf("用户菜单\n"
           "---------------------------------------\n");
    while (1){
        print_colored("cyan", "选项:\n");
        print_colored("green",
                      "\t1. 进入聊天室"
                      "\t2. 查看个人主页\n"
//                      "\t3. 修改个人信息"
                      "\t3. 注销登录\n");
        print_colored("cyan", "请输入你的操作:");
        scanf("%d",&choice);
        clear_input_buffer();
        switch (choice) {
            case 1:
                Chat_Room_UI();
                return;
            case 2:
                Account_Homepage_UI();
                return;
//            case 3:
//                return;
            case 3:
                Hello_UI();
                return;
            default:
                print_colored("red","输入错误!\n");
        }
    }
}


void Account_Homepage_UI() {
    Init_UI();
    printf("个人主页\n"
           "---------------------------------------\n");
    print_colored("pink","\tUID:\t");
    print_colored("yellow","%d\n", account.uid);
    print_colored("pink","\t用户名:\t");
    print_colored("yellow","%s\n", account.name);
    print_colored("pink","\t性别:\t");
    print_colored("yellow","%s\n", account.sex>0?(account.sex==0? "男":"女"):"暂未设置性别");
    print_colored("pink","\t简介:\t");
    print_colored("yellow","%s\n", account.introduction!=NULL?account.introduction:"暂未设置简介");
    print_colored("2","按下回车键返回...");
    clear_input_buffer();
    Account_Menu_UI();
}

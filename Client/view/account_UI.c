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
    printf("登录账号\n"
           "---------------------------\n");
    while (1){
        int choice;
        int uid;
        char name[50],password[50];
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
                sleep(1);
                print_colored("2","按下回车键继续...");
                clear_input_buffer();
                return;
            case 2:
                print_colored("cyan","输入账号:");
                scanf("%s", name);
                clear_input_buffer();
                print_colored("cyan","输入密码:");
                set_echo(0);
                scanf("%49s",password);
                set_echo(1);
                clear_input_buffer();
                printf("\n");
                Account_Srv_Login_By_Name(name,password);
                sleep(1);
                print_colored("2","按下回车键继续...");
                clear_input_buffer();
                return;
            case 3:
                Hello_UI();
                return;
            default:
                print_colored("red","输入错误!\n");
        }
    }
}


void Account_Menu_UI(){

}
//
// Created by dycley on 5/8/24.
//

#include <stdio.h>
#include <string.h>
#include "view/chat_UI.h"
#include "common/common.h"
#include "common/global.h"
#include "view/hello_UI.h"
#include "view/account_UI.h"
#include "service/chatSrv.h"

extern Account account;
extern int isInRoom;

void Chat_Room_UI() {
    char msg[800],filepath[200],filename[200];
    Init_UI();
    isInRoom = 1;
    print_colored("cyan","输入消息并按回车以发送 菜单选项: /r 返回上一级 /u 上传文件 /d 下载文件 /e 退出\n");
    while (1){
        fgets(msg,sizeof(msg),stdin);
//        clear_input_buffer();
        if(strcmp(msg,"/r\n")==0){
            Account_Menu_UI();
            return;
        }else if(strcmp(msg,"/u\n")==0){
            print_colored("cyan","请输入上传的文件路径: ");
            scanf("%199s",filepath);
            clear_input_buffer();
            if(Chat_Srv_Send_File(filepath) < 0){
                print_colored("cyan","输入消息并按回车以发送 菜单选项: /r 返回上一级 /u 上传文件 /d 下载文件 /e 退出\n");
            }
        }else if(strcmp(msg,"/d\n")==0){
            print_colored("cyan","请输入要下载的文件名: ");
            scanf("%199s",filename);
            clear_input_buffer();
            Chat_Srv_Download(filename);
            print_colored("cyan","输入消息并按回车以发送 菜单选项: /r 返回上一级 /u 上传文件 /d 下载文件 /e 退出\n");
        }else if(strcmp(msg,"/e\n")==0){
            return;
        }else{
            clean_last_line();
            Chat_Srv_Send_Msg(msg);
        }
    }
}


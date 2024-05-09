//
// Created by dycley on 4/25/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include "common/common.h"
#include "time.h"

#define MSG_LEN 1024

pthread_mutex_t mutex;

void lock() {
    pthread_mutex_lock(&mutex);
}

void unlock() {
    pthread_mutex_unlock(&mutex);
}

// 初始化互斥锁
int init_mutex() {
    return pthread_mutex_init(&mutex, NULL);
}

// 销毁互斥锁
int destroy_mutex() {
    return pthread_mutex_destroy(&mutex);
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

char *curTime() {
    static char buffer[80]; // 静态缓冲区，用于存储格式化的时间
    time_t currentTime;
    struct tm *localTime;

    // 获取当前日历时间
    time(&currentTime);

    // 转换为本地时间
    localTime = localtime(&currentTime);

    // 使用 strftime 格式化时间
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);

    return buffer; // 返回静态缓冲区的地址
}

void logs(const char *format, ...) {
    FILE *out;
    va_list args; // 定义一个 va_list 类型的变量来存储参数列表

//    out = fopen("log/log.txt","a+");
    out = stderr;
    if(out != NULL){
        fprintf(out,"%s ",curTime());

        va_start(args, format); // 初始化 args 变量，使其指向第一个可选参数
        vfprintf(out, format, args); // 使用可变参数将格式化的字符串写入文件
        va_end(args); // 清理完成的 args

        fprintf(out, "\n");
//        fclose(out); // 关闭文件
    } else {
        // 如果文件无法打开，打印错误消息
        perror("无法打开文件\n");
    }
}


//通用格式控制：0: 重置所有属性 1: 高亮/加粗 2: 暗淡 4: 下划线 5: 闪烁 7: 反转 8: 隐藏
//前景色： 30: 黑色 31: 红色 32: 绿色 33: 黄色 34: 蓝色 35: 品红 36: 青色
//背景色：40: 黑色 41: 红色 42: 绿色 43: 黄色 44: 蓝色 45: 品红 46: 青色

void print_colored(const char *color_code, const char *format, ...) {
    if(!strcmp(color_code, "black")){
        color_code = "30";
    }else if(!strcmp(color_code, "red")){
        color_code = "31";
    }else if(!strcmp(color_code, "green")){
        color_code = "32";
    }else if(!strcmp(color_code, "yellow")){
        color_code = "33";
    }else if(!strcmp(color_code, "blue")){
        color_code = "34";
    }else if(!strcmp(color_code, "pink")){
        color_code = "35";
    }else if(!strcmp(color_code, "cyan")){
        color_code = "36";
    }
    va_list args;
    va_start(args, format);
    printf("\033[0m\033[%sm", color_code);
    vprintf(format, args);
    printf("\033[0m");
    va_end(args);
}


void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}

void set_echo(int enable) {
    struct termios settings;
    tcgetattr(STDIN_FILENO, &settings);
    if (!enable) {
        settings.c_lflag &= ~ECHO; // 关闭回显
    } else {
        settings.c_lflag |= ECHO;  // 打开回显
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}



void clean_last_line() {
    printf("\033[A\033[2K\r");
//    printf("\r");
//    for (int i = 0; i < n; i++) {
//        putchar(' ');
//    }
//    printf("\r");
}



long get_file_size(FILE *file) {
    long size;
    long current_position = ftell(file); // 记录当前位置
    if (current_position == -1) {
        // 错误处理
        perror("ftell error");
        return -1;
    }

    // 将文件指针移动到文件末尾
    if (fseek(file, 0, SEEK_END) != 0) {
        // 错误处理
        perror("fseek error");
        return -1;
    }

    // 获取当前位置（文件末尾），即为文件大小
    size = ftell(file);
    if (size == -1) {
        // 错误处理
        perror("ftell error");
        return -1;
    }

    // 恢复文件指针到原来的位置
    if (fseek(file, current_position, SEEK_SET) != 0) {
        // 错误处理
        perror("fseek error");
        return -1;
    }

    return size; // 返回文件大小
}

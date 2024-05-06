//
// Created by dycley on 4/25/24.
//

#ifndef CHATROOM_COMMON_H
#define CHATROOM_COMMON_H

void error(const char *msg);

void logs(const char *format, ...);

/**
 * 通用格式控制：0: 重置所有属性 1: 高亮/加粗 2: 暗淡 4: 下划线 5: 闪烁 7: 反转 8: 隐藏  \n
 * 前景色： 30: 黑色 31: 红色 32: 绿色 33: 黄色 34: 蓝色 35: 品红 36: 青色  \n
 * 背景色：40: 黑色 41: 红色 42: 绿色 43: 黄色 44: 蓝色 45: 品红 46: 青色  \n
 * @param color_code （
 * @param format
 * @param ...
 */
void print_colored(const char *color_code, const char *format, ...);
/**
 * 对密码加密
 * @param pwd
 * @return 加密后字符串
 */
char *hash_pwd(const char *pwd);
#endif //CHATROOM_COMMON_H

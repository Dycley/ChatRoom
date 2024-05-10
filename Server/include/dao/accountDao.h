//
// Created by dycley on 4/28/24.
//

#ifndef CHATROOM_ACCOUNTDAO_H
#define CHATROOM_ACCOUNTDAO_H

/**
 * 注册用户，成功则返回0， 错误返回-1
 * @param name
 * @param password
 * @return exec code
 */
int Account_Register_By_Name_Pwd(const char *name, const char *password);

int Account_Check_Name_Pwd(const char *name, const char *password);

/**
 * 检查用户名和密码是否匹配
 * @param uid
 * @param password
 * @return 匹配(0) 不匹配(1) 执行失败(-1)
 */
int Account_Check_Uid_Pwd(int uid, const char *password);

/**
 * 输入用户姓名，返回用户uid
 * @param name
 * @return uid of account
 */
int Account_Get_Uid_By_Name(const char *name);

char* Account_Get_Name_By_Uid(int uid);
#endif //CHATROOM_ACCOUNTDAO_H

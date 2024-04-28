//
// Created by dycley on 4/28/24.
//

#ifndef CHATROOM_ACCOUNTDAO_H
#define CHATROOM_ACCOUNTDAO_H

/**
 * 注册用户，成功则返回0， 错误返回-1
 * @param name
 * @param pwd
 * @return exec code
 */
int Account_Register_By_Name_Pwd(char *name, char *pwd);

/**
 * 输入用户姓名，返回用户uid
 * @param name
 * @return uid of account
 */
int Account_Get_Uid_By_Name(char *name);

char* Account_Get_Name_By_Uid(int uid);
#endif //CHATROOM_ACCOUNTDAO_H

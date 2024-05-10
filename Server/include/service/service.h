//
// Created by dycley on 5/3/24.
//

#ifndef CHATROOM_SERVICE_H
#define CHATROOM_SERVICE_H

void add_account(int uid, int sock_fd);

void set_account_offline(int sock_fd);

void * thread(void *arg);

void parse(int sock_fd, char *buf);

#endif //CHATROOM_SERVICE_H

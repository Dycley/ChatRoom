//
// Created by dycley on 5/3/24.
//

#ifndef CHATROOM_SERVICE_H
#define CHATROOM_SERVICE_H

void * thread(void *arg);

void parse(int sock_fd, char *buf);

#endif //CHATROOM_SERVICE_H

//
// Created by dycley on 4/25/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "commom/common.h"
#include "time.h"

#define MSG_LEN 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}



ssize_t my_send(int fd, const void *buf) {
    return send(fd, buf, MSG_LEN, 0);
}

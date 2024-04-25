//
// Created by dycley on 4/25/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "commom/common.h"



void error(const char *msg) {
    perror(msg);
    exit(1);
}

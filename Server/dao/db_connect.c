//
// Created by dycley on 4/25/24.
//

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include "dao/db_connect.h"

sqlite3 *db;


void db_connect(char *db_path) {
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(db_path, &db);

    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
}

void db_close() {
    sqlite3_close(db);
}


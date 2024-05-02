//
// Created by dycley on 4/25/24.
//

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include "dao/db_connect.h"
#include "dao/accountDao.h"
sqlite3 *db;

void db_connect(char *db_path) {
    int rc;
    rc = sqlite3_open(db_path, &db);

    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }

//    Account_Register_By_Name_Pwd("Tom", "123");
//    int tmp=Account_Get_Uid_By_Name("Tom");
//    printf("%d\n",tmp);
//    char *tmpname= Account_Get_Name_By_Uid(2);
//    printf("%s\n",tmpname);
}

void db_close() {
    sqlite3_close(db);
}


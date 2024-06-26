//
// Created by dycley on 4/28/24.
//

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <malloc.h>
#include "common/cJSON.h"
#include "dao/accountDao.h"
#include "common/common.h"

extern sqlite3 *db;

int Account_Register_By_Name_Pwd(const char *name, const char *password){
    char *hashed_pwd = hash_pwd(password);
    sqlite3_stmt *res;
    char *sql = "INSERT into ACCOUNT(name, password)"\
                "VALUES(@name, @password)";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(res, "@name");
        sqlite3_bind_text(res, idx, name, -1, SQLITE_STATIC);
        idx = sqlite3_bind_parameter_index(res, "@password");
        sqlite3_bind_text(res, idx, hashed_pwd, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    rc = sqlite3_step(res);
    // 检查是否成功执行
    if (rc != SQLITE_DONE) {
        fprintf(stderr,"Execute SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(res);
        return -1;
    } else {
        printf("Account %s created successfully\n", name);
    }
    sqlite3_finalize(res);
    free(hashed_pwd);
    return 0;
}


int Account_Check_Name_Pwd(const char *name, const char *password) {
    sqlite3_stmt *res;
    char *sql = "SELECT password FROM account WHERE name = @name";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(res, "@name");
        sqlite3_bind_text(res, idx, name, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    rc = sqlite3_step(res);
    // 检查是否成功执行

    if (rc == SQLITE_ROW) {
        char *hashed_pwd = hash_pwd(password);
        const unsigned char *selected_pwd = sqlite3_column_text(res,0);
        int eql = strcmp(hashed_pwd,(const char *)selected_pwd) != 0;
        sqlite3_finalize(res);
        return eql;
    } else {
        fprintf(stderr,"Execute SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(res);
        return -1;
    }
    sqlite3_finalize(res);
    return -1;
}


int Account_Check_Uid_Pwd(int uid, const char *password) {
    sqlite3_stmt *res;
    char *sql = "SELECT password FROM account WHERE uid = @uid";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(res, "@uid");
        sqlite3_bind_int(res, idx, uid);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    rc = sqlite3_step(res);
    // 检查是否成功执行

    if (rc == SQLITE_ROW) {
        char *hashed_pwd = hash_pwd(password);
        const unsigned char *selected_pwd = sqlite3_column_text(res,0);
        int eql = strcmp(hashed_pwd,(const char *)selected_pwd) != 0;
        sqlite3_finalize(res);
        return eql;
    } else {
        fprintf(stderr,"Execute SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(res);
        return -1;
    }
    sqlite3_finalize(res);
    return -1;
}

int Account_Get_Uid_By_Name(const char *name) {
    sqlite3_stmt *res;
    char *sql = "SELECT uid FROM account WHERE name = @name";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(res, "@name");
        sqlite3_bind_text(res, idx, name, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    rc = sqlite3_step(res);
    // 检查是否成功执行
    int uid = -1;
    if (rc == SQLITE_ROW) {
        uid = sqlite3_column_int(res,0);
    } else {
        fprintf(stderr,"Execute SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(res);
        return -1;
    }
    sqlite3_finalize(res);
    return uid;
}



char* Account_Get_Name_By_Uid(int uid) {
    sqlite3_stmt *res;
    char *sql = "SELECT name FROM account WHERE uid = @uid";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(res, "@uid");
        sqlite3_bind_int(res, idx, uid);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    rc = sqlite3_step(res);
    // 检查是否成功执行
    const unsigned char *temp_name;
    char *name;
    if (rc == SQLITE_ROW) {
        temp_name = sqlite3_column_text(res, 0);
        name = malloc(strlen((const char *)temp_name) + 1);
        strcpy(name, (const char *)temp_name);
    } else {
        fprintf(stderr,"Execute SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(res);
        return NULL;
    }
    sqlite3_finalize(res);
    return name;
}

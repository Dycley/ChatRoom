//
// Created by dycley on 5/6/24.
//

#include <sqlite3.h>
#include <stdio.h>
#include "dao/chatDao.h"

extern sqlite3 *db;
enum DATATYPE{
    MESSAGE_TYPE, FILE_TYPE
};

int Chat_Save_Msg(int uid, int msgtype, const char *msg, int timestamp) {
    sqlite3_stmt *res;
    char *sql = "INSERT into MESSAGE(uid, timestamp, msgtype, msg)"\
                "VALUES(@uid, @timestamp, @msgtype, @msg)";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(res,"@uid");
        sqlite3_bind_int(res,idx,uid);
        idx = sqlite3_bind_parameter_index(res,"@timestamp");
        sqlite3_bind_int(res,idx,timestamp);
        idx = sqlite3_bind_parameter_index(res,"@msgtype");
        sqlite3_bind_int(res, idx, msgtype);
        idx = sqlite3_bind_parameter_index(res, "@msg");
        sqlite3_bind_text(res, idx, msg, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    rc = sqlite3_step(res);
    // 检查是否成功执行
    if (rc != SQLITE_DONE) {
        fprintf(stderr,"Execute SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(res);
        return -1;
    }
    sqlite3_finalize(res);
    return 0;
}

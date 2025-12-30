#include "database.h"
#include <stdio.h>
#include <stdlib.h>

static sqlite3 *db = NULL;

int init_database(const char *db_path) {
    if (db != NULL) return 1;

    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_busy_timeout(db, 5000);

    const char *sql =
        "CREATE TABLE IF NOT EXISTS zako("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "tag INTEGER NOT NULL, "
        "status INTEGER NOT NULL, "
        "name TEXT NOT NULL, "
        "detail TEXT NOT NULL, "
        "discord TEXT NOT NULL)";

    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }

    return 1;
}

sqlite3* get_database() {
    return db;
}

void close_database() {
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}

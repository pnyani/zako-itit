#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

int init_database(const char *db_path);
sqlite3* get_database();
void close_database();

#endif // DATABASE_H

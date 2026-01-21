#ifndef DATABASE_H
#define DATABASE_H

#include <exec/types.h>
#include <sqlite3.h>
sqlite3 *get_db(void);

BOOL db_init(const char *db_path);
void db_close(void);

BOOL db_create_tables(void);
BOOL db_test_connection(void);

#endif
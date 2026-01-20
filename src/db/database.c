#include "../../include/common.h"
#include "../../include/db/database.h"
#include <sqlite3.h>
#include <string.h>

static sqlite3 *db = NULL;

BOOL db_init(const char *db_path)
{   
    int rc;

    printf("Initializing database at %s\n", db_path);
    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
    printf("Database opened successfully\n");
    if (!db_create_tables())
    {
        printf("Failed to create tables\n");
        sqlite3_close(db);
        return FALSE;
    }

    return TRUE;
}

void db_close(void)
{
    if(db)
    {
        sqlite3_close(db);
        db = NULL;
        printf("Database closed successfully\n");
    }
}

BOOL db_create_tables(void)
{
    int rc;
    char *err_msg = NULL;
    const char *sql_projects;
    const char *sql_time_entries;
    const char *sql_settings;
    
    sql_projects =
            "CREATE TABLE IF NOT EXISTS projects ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "customer TEXT,"
        "start_date TEXT,"
        "estimated_hours REAL,"
        "status TEXT DEFAULT 'Active',"
        "created_at TEXT DEFAULT CURRENT_TIMESTAMP"
        ");";

    sql_time_entries = 
        "CREATE TABLE IF NOT EXISTS time_entries ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "project_id INTEGER NOT NULL,"
        "start_time TEXT NOT NULL,"
        "end_time TEXT,"
        "duration_seconds INTEGER,"
        "note TEXT,"
        "created_at TEXT DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (project_id) REFERENCES projects(id)"
        ");";
    
    sql_settings = 
        "CREATE TABLE IF NOT EXISTS app_settings ("
        "key TEXT PRIMARY KEY,"
        "value TEXT"
        ");";   
    
    rc = sqlite3_exec(db, sql_projects, NULL, NULL, &err_msg);   
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to create projects table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return FALSE;
    }   
    
    rc = sqlite3_exec(db, sql_time_entries, NULL, NULL, &err_msg);   
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to create time entries table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return FALSE;
    }
    
    rc = sqlite3_exec(db, sql_settings, NULL, NULL, &err_msg);   
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to create settings table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return FALSE;
    }
    printf("Tables created successfully\n");
    return TRUE;
}
    
BOOL db_test_connection(void)
    {
        if (!db)
        {
            printf("Database is not initialized\n");
            return FALSE;
        }
        printf("Database connection test successful\n");
        return TRUE;
    }
   

#include "../../include/common.h"
#include "../../include/model/project.h"
#include "../../include/db/database.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

extern sqlite3 *get_db(void);

BOOL project_create(const char *name, const char *customer, float estimated_hours)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    const char *sql;
    
    db = get_db();
    if (!db)
    {
        fprintf(stderr, "Database is not initialized\n");
        return FALSE;
    }
    
    sql = "INSERT INTO projects (name, customer, estimated_hours, start_date) VALUES (?, ?, ?, CURRENT_TIMESTAMP);";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, customer, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, estimated_hours);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to insert project: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
    
    sqlite3_finalize(stmt);
    return TRUE;
}

Project *project_get_by_id(int id)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    const char *sql;
    Project *project;
    
    db = get_db();
    if (!db)
    {
        fprintf(stderr, "Database is not initialized\n");
        return NULL;
    }
    
    sql = "SELECT id, name, customer, start_date, estimated_hours, status FROM projects WHERE id = ?;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        fprintf(stderr, "Failed to get project: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    
    project->id = sqlite3_column_int(stmt, 0);
    strncpy(project->name, (const char *)sqlite3_column_text(stmt, 1), 255);
    project->name[255] = '\0';
    strncpy(project->customer, (const char *)sqlite3_column_text(stmt, 2), 255);
    project->customer[255] = '\0';
    strncpy(project->start_date, (const char *)sqlite3_column_text(stmt, 3), 31);
    project->start_date[31] = '\0';
    project->estimated_hours = sqlite3_column_double(stmt, 4);
    strncpy(project->status, (const char *)sqlite3_column_text(stmt, 5), 31);
    project->status[31] = '\0';
    
    sqlite3_finalize(stmt);
    return project;
}

Project **project_get_all(int *count)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    const char *sql;
    
    db = get_db();
    if (!db)
    {
        fprintf(stderr, "Database is not initialized\n");
        return NULL;
    }
    
    sql = "SELECT id, name, customer, start_date, estimated_hours, status FROM projects;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    
    *count = 0;
    Project **projects = NULL;
    
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Project *project = malloc(sizeof(Project));
        project->id = sqlite3_column_int(stmt, 0);
        strncpy(project->name, (const char *)sqlite3_column_text(stmt, 1), 255);
        project->name[255] = '\0';
        strncpy(project->customer, (const char *)sqlite3_column_text(stmt, 2), 255);
        project->customer[255] = '\0';
        strncpy(project->start_date, (const char *)sqlite3_column_text(stmt, 3), 31);
        project->start_date[31] = '\0';
        project->estimated_hours = sqlite3_column_double(stmt, 4);
        strncpy(project->status, (const char *)sqlite3_column_text(stmt, 5), 31);
        project->status[31] = '\0';
        
        projects = realloc(projects, (*count + 1) * sizeof(Project *));
        projects[*count] = project;
        (*count)++;
    }
    
    sqlite3_finalize(stmt);
    return projects;
}   

BOOL project_update(int id, const char *name, const char *customer, float estimated_hours, const char *status)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    const char *sql;
    
    db = get_db();
    if (!db)
    {
        fprintf(stderr, "Database is not initialized\n");
        return FALSE;
    }
    
    sql = "UPDATE projects SET name = ?, customer = ?, estimated_hours = ?, status = ? WHERE id = ?;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, customer, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, estimated_hours);
    sqlite3_bind_text(stmt, 4, status, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, id);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to update project: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
    
    sqlite3_finalize(stmt);
    return TRUE;
}   

BOOL project_delete(int id)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    const char *sql;
    
    db = get_db();
    if (!db)
    {
        fprintf(stderr, "Database is not initialized\n");
        return FALSE;
    }
    
    sql = "DELETE FROM projects WHERE id = ?;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE);
}

void project_free(Project *project)
{
    if(project)
    {
        free(project);
    }
}

void project_free_all(Project **projects, int count)
{
    if(projects)
    {
        for(int i = 0; i < count; i++)
        {
            project_free(projects[i]);
        }
        free(projects);
    }
}   



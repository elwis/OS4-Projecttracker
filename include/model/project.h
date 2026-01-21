#ifndef PROJECT_H
#define PROJECT_H

#include <exec/types.h>

typedef struct
{
    int id;
    char name[256];
    char customer[256];
    char start_date[32];
    float estimated_hours;
    char status[32];
} Project;

//CRUD
BOOL project_create(const char *name, const char *customer, float estimated_hours);
Project *project_get_by_id(int id);
Project **project_get_all(int *count);
BOOL project_update(int id, const char *name, const char *customer, float estimated_hours, const char *status);
BOOL project_delete(int id);

//Mem management
void project_free(Project *project);
void project_free_all(Project **projects, int count);   

#endif  
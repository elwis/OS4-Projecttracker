#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <exec/types.h>
#include <intuition/intuition.h>

// create/show main window
struct Window *create_main_window(void);

//close & clean
void close_main_window(void);

//main window events
BOOL handle_main_window_events(void);

//update projectlists
void refresh_project_list(void);

#endif /* MAIN_WINDOW_H */
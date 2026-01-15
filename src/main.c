#include "../include/common.h"
#include "../include/gui/main_window.h"

int main(void)
{
    struct Window *window;
    BOOL running = TRUE;
    
    printf("%s v%s starting...\n", APP_NAME, APP_VERSION);
    
    // Skapa huvudfönster
    window = create_main_window();
    if (!window) {
        printf("Failed to create main window\n");
        return 1;
    }
    
    printf("Main window created successfully\n");
    
    // Event loop
    while (running) {
        IExec->Wait(1L << window->UserPort->mp_SigBit);
        running = handle_main_window_events();
    }
    
    // Städa upp
    close_main_window();
    
    printf("Application closed\n");
    return 0;
}
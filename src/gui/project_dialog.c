#include "../../include/common.h"
#include "../../include/gui/project_dialog.h"
#include "../../include/model/project.h"
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/button.h>
#include <proto/string.h>
#include <proto/integer.h>
#include <classes/window.h>
#include <gadgets/layout.h>
#include <gadgets/button.h>
#include <gadgets/string.h>
#include <gadgets/integer.h>
#include <intuition/classusr.h>
#include <string.h>
#include <stdlib.h>

#define GAD_NAME      1
#define GAD_CUSTOMER  2
#define GAD_HOURS     3
#define GAD_OK        4
#define GAD_CANCEL    5

BOOL show_new_project_dialog(void)
{
    struct Window *dialog_window;
    Object *dialog_obj;
    Object *name_str, *customer_str, *hours_int;
    Object *btn_ok, *btn_cancel;
    Object *main_layout, *button_layout;
    BOOL result;
    BOOL running;
    ULONG win_result;
    UWORD code;
    char *name_value;
    char *customer_value;
    LONG hours_value;
    
    printf("Creating dialog gadgets...\n");
    
    dialog_window = NULL;
    result = FALSE;
    running = TRUE;
    name_value = NULL;
    customer_value = NULL;
    
    /* Skapa string gadgets */
    name_str = IIntuition->NewObject(NULL, "string.gadget",
        GA_ID, GAD_NAME,
        GA_TabCycle, TRUE,
        STRINGA_MaxChars, 255,
        TAG_DONE);
    
    if (!name_str) {
        printf("Failed to create name string gadget\n");
        return FALSE;
    }
    printf("Name gadget created\n");
    
    customer_str = IIntuition->NewObject(NULL, "string.gadget",
        GA_ID, GAD_CUSTOMER,
        GA_TabCycle, TRUE,
        STRINGA_MaxChars, 255,
        TAG_DONE);
    
    if (!customer_str) {
        printf("Failed to create customer string gadget\n");
        return FALSE;
    }
    printf("Customer gadget created\n");
    
    hours_int = IIntuition->NewObject(NULL, "integer.gadget",
        GA_ID, GAD_HOURS,
        GA_TabCycle, TRUE,
        INTEGER_Minimum, 0,
        INTEGER_Maximum, 10000,
        INTEGER_Number, 10,
        TAG_DONE);
    
    if (!hours_int) {
        printf("Failed to create hours integer gadget\n");
        return FALSE;
    }
    printf("Hours gadget created\n");
    
    /* Skapa knappar */
    btn_ok = IIntuition->NewObject(NULL, "button.gadget",
        GA_ID, GAD_OK,
        GA_Text, "Create",
        GA_RelVerify, TRUE,
        TAG_DONE);
    
    if (!btn_ok) {
        printf("Failed to create OK button\n");
        return FALSE;
    }
    printf("OK button created\n");
    
    btn_cancel = IIntuition->NewObject(NULL, "button.gadget",
        GA_ID, GAD_CANCEL,
        GA_Text, "Cancel",
        GA_RelVerify, TRUE,
        TAG_DONE);
    
    if (!btn_cancel) {
        printf("Failed to create Cancel button\n");
        return FALSE;
    }
    printf("Cancel button created\n");
    
    /* Knapp-layout */
    button_layout = IIntuition->NewObject(NULL, "layout.gadget",
        LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ,
        LAYOUT_AddChild, btn_ok,
        CHILD_WeightedWidth, 0,
        LAYOUT_AddChild, btn_cancel,
        CHILD_WeightedWidth, 0,
        TAG_DONE);
    
    if (!button_layout) {
        printf("Failed to create button layout\n");
        return FALSE;
    }
    printf("Button layout created\n");
    
    /* Huvud-layout */
    main_layout = IIntuition->NewObject(NULL, "layout.gadget",
        LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
        LAYOUT_AddChild, name_str,
        CHILD_WeightedHeight, 0,
        LAYOUT_AddChild, customer_str,
        CHILD_WeightedHeight, 0,
        LAYOUT_AddChild, hours_int,
        CHILD_WeightedHeight, 0,
        LAYOUT_AddChild, button_layout,
        CHILD_WeightedHeight, 0,
        TAG_DONE);
    
    if (!main_layout) {
        printf("Failed to create main layout\n");
        return FALSE;
    }
    printf("Main layout created\n");
    
    /* Skapa dialog */
    dialog_obj = IIntuition->NewObject(NULL, "window.class",
        WA_Title, "New Project",
        WA_Width, 400,
        WA_Height, 200,
        WA_CloseGadget, TRUE,
        WA_DragBar, TRUE,
        WA_DepthGadget, TRUE,
        WA_Activate, TRUE,
        WINDOW_Position, WPOS_CENTERSCREEN,
        WINDOW_Layout, main_layout,
        TAG_DONE);
    
    if (!dialog_obj) {
        printf("Failed to create dialog object\n");
        return FALSE;
    }
    printf("Dialog object created\n");
    
    /* Öppna dialog */
    printf("Opening dialog window...\n");
    dialog_window = (struct Window *)IIntuition->IDoMethod(dialog_obj, WM_OPEN, NULL);
    
    if (!dialog_window) {
        printf("Failed to open dialog window\n");
        IIntuition->DisposeObject(dialog_obj);
        return FALSE;
    }
    
    printf("Dialog window opened successfully!\n");
    printf("Entering event loop...\n");
    
    /* Event loop */
    while (running) {
        IExec->Wait(1L << dialog_window->UserPort->mp_SigBit);
        
        while ((win_result = IIntuition->IDoMethod(dialog_obj, WM_HANDLEINPUT, &code)) != WMHI_LASTMSG) {
            printf("Got event: %ld\n", win_result);
            
            switch (win_result & WMHI_CLASSMASK) {
                case WMHI_CLOSEWINDOW:
                    printf("Close window clicked\n");
                    running = FALSE;
                    break;
                    
                case WMHI_GADGETUP:
                    switch (win_result & WMHI_GADGETMASK) {
                        case GAD_OK:
                            printf("OK button clicked\n");
                            /* Hämta värden från gadgets */
                            IIntuition->GetAttr(STRINGA_TextVal, name_str, (ULONG *)&name_value);
                            IIntuition->GetAttr(STRINGA_TextVal, customer_str, (ULONG *)&customer_value);
                            IIntuition->GetAttr(INTEGER_Number, hours_int, (ULONG *)&hours_value);
                            
                            printf("Name: %s, Customer: %s, Hours: %ld\n", 
                                   name_value ? name_value : "(empty)", 
                                   customer_value ? customer_value : "(empty)", 
                                   hours_value);
                            
                            /* Validera */
                            if (name_value && strlen(name_value) > 0) {
                                /* Skapa projekt */
                                if (project_create(name_value, 
                                                  customer_value ? customer_value : "", 
                                                  (float)hours_value)) {
                                    printf("Project created successfully!\n");
                                    result = TRUE;
                                    running = FALSE;
                                } else {
                                    printf("Failed to create project\n");
                                }
                            } else {
                                printf("Project name is required\n");
                            }
                            break;
                            
                        case GAD_CANCEL:
                            printf("Cancel button clicked\n");
                            running = FALSE;
                            break;
                    }
                    break;
            }
        }
    }
    
    printf("Closing dialog...\n");
    
    /* Städa upp */
    IIntuition->DisposeObject(dialog_obj);
    
    printf("Dialog closed, returning result: %d\n", result);
    return result;
}
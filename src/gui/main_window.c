#include "../../include/common.h"
#include "../../include/gui/main_window.h"
#include "../../include/gui/project_dialog.h"
#include "../../include/model/project.h"
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/button.h>
#include <proto/listbrowser.h>
#include <proto/space.h>
#include <classes/window.h>
#include <gadgets/layout.h>
#include <gadgets/button.h>
#include <gadgets/listbrowser.h>
#include <gadgets/space.h>
#include <intuition/classusr.h>


// Privata variabler
static struct Window *main_window = NULL;
static Object *win_obj = NULL;
static Object *listbrowser = NULL;
static struct List *project_list = NULL;

struct Window *create_main_window(void)
{
    Object *btn_new, *btn_open, *btn_delete, *btn_export;
    Object *main_layout, *button_layout, *space_obj;
    
    struct ColumnInfo column_info[] = {
        { 200, "Name",     CIF_DRAGGABLE },
        { 150, "Customer", CIF_DRAGGABLE },
        { 80, "Hours",    CIF_DRAGGABLE },
        { 80, "Status",   CIF_DRAGGABLE },
        { -1, NULL,       0 }
    };
    
    // Skapa tom lista för projekt
    project_list = IExec->AllocSysObjectTags(ASOT_LIST, TAG_DONE);
    if (!project_list) {
        return NULL;
    }
    
    // Skapa listbrowser
    listbrowser = IIntuition->NewObject(NULL, "listbrowser.gadget",
        GA_ID, GAD_LISTBROWSER,
        GA_RelVerify, TRUE,
        LISTBROWSER_ColumnInfo, column_info,
        LISTBROWSER_ColumnTitles, TRUE,
        LISTBROWSER_Labels, project_list,
        LISTBROWSER_Striping, LBS_ROWS,
        LISTBROWSER_Editable, FALSE,
        LISTBROWSER_ShowSelected, TRUE,
        LISTBROWSER_Separators, TRUE,
        LISTBROWSER_TitleClickable, TRUE,
        TAG_DONE);
    
    // Skapa knappar
    btn_new = IIntuition->NewObject(NULL, "button.gadget",
        GA_ID, GAD_NEW,
        GA_Text, "New",
        GA_RelVerify, TRUE,
        TAG_DONE);
    
    btn_open = IIntuition->NewObject(NULL, "button.gadget",
        GA_ID, GAD_OPEN,
        GA_Text, "Open",
        GA_RelVerify, TRUE,
        TAG_DONE);
    
    btn_delete = IIntuition->NewObject(NULL, "button.gadget",
        GA_ID, GAD_DELETE,
        GA_Text, "Delete",
        GA_RelVerify, TRUE,
        TAG_DONE);
    
    btn_export = IIntuition->NewObject(NULL, "button.gadget",
        GA_ID, GAD_EXPORT,
        GA_Text, "Export",
        GA_RelVerify, TRUE,
        TAG_DONE);
    
    // Skapa space
    space_obj = IIntuition->NewObject(NULL, "space.gadget", TAG_DONE);
    
    // Knapp-layout (horisontell)
    button_layout = IIntuition->NewObject(NULL, "layout.gadget",
        LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ,
        LAYOUT_AddChild, btn_new,
        CHILD_WeightedWidth, 0,
        LAYOUT_AddChild, btn_open,
        CHILD_WeightedWidth, 0,
        LAYOUT_AddChild, btn_delete,
        CHILD_WeightedWidth, 0,
        LAYOUT_AddChild, space_obj,
        LAYOUT_AddChild, btn_export,
        CHILD_WeightedWidth, 0,
        TAG_DONE);
    
    // Huvud-layout
    main_layout = IIntuition->NewObject(NULL, "layout.gadget",
        LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
        LAYOUT_AddChild, listbrowser,
        CHILD_WeightedHeight, 100,
        LAYOUT_AddChild, button_layout,
        CHILD_WeightedHeight, 0,
        TAG_DONE);
    
    // Skapa fönster
    win_obj = IIntuition->NewObject(NULL, "window.class",
        WA_Title, APP_NAME,
        WA_ScreenTitle, APP_NAME " v" APP_VERSION,
        WA_Width, 600,
        WA_Height, 400,
        WA_CloseGadget, TRUE,
        WA_DepthGadget, TRUE,
        WA_DragBar, TRUE,
        WA_SizeGadget, TRUE,
        WA_Activate, TRUE,
        WINDOW_Position, WPOS_CENTERSCREEN,
        WINDOW_Layout, main_layout,
        TAG_DONE);
    
    if (!win_obj) {
        IExec->FreeSysObject(ASOT_LIST, project_list);
        return NULL;
    }
    
    // Öppna fönster
    main_window = (struct Window *)IIntuition->IDoMethod(win_obj, WM_OPEN, NULL);
    
    if (!main_window) {
        IIntuition->DisposeObject(win_obj);
        IExec->FreeSysObject(ASOT_LIST, project_list);
        return NULL;
    }
    
    printf("Main Window created successfully\n");
    refresh_project_list();
    return main_window;
}

void close_main_window(void)
{
    if (win_obj) {
        IIntuition->DisposeObject(win_obj);
        win_obj = NULL;
    }
    
    if (project_list) {
        IExec->FreeSysObject(ASOT_LIST, project_list);
        project_list = NULL;
    }
    
    main_window = NULL;
}

BOOL handle_main_window_events(void)
{
    ULONG result;
    UWORD code;
    
    if (!main_window) {
        return FALSE;
    }
    
    while ((result = IIntuition->IDoMethod(win_obj, WM_HANDLEINPUT, &code)) != WMHI_LASTMSG) {
        switch (result & WMHI_CLASSMASK) {
            case WMHI_CLOSEWINDOW:
                return FALSE;  // Signal att stänga
                
            case WMHI_GADGETUP:
                switch (result & WMHI_GADGETMASK) {
                    case GAD_NEW:
                        printf("New project clicked\n");
                        if(show_new_project_dialog())   
                        {
                            refresh_project_list();
                        }
                        break;
                        
                    case GAD_OPEN:
                        printf("Open project clicked\n");
                        // TODO: Öppna valt projekt
                        break;
                        
                    case GAD_DELETE:
                        printf("Delete project clicked\n");
                        // TODO: Ta bort valt projekt
                        break;
                        
                    case GAD_EXPORT:
                        printf("Export clicked\n");
                        // TODO: Exportera till CSV/JSON
                        break;
                        
                    case GAD_LISTBROWSER:
                        printf("Project selected\n");
                        break;
                }
                break;
        }
    }
    
    return TRUE;  // Fortsätt köra
}

void refresh_project_list(void)
{
    Project **projects;
    int count;
    int i;
    struct Node *node;
    char hours_str[32];

    printf("Refreshing project list...\n");

    if(!listbrowser || !project_list){
        printf("Listbrowser or project_list not initialized\n");
        return;
    }

    IIntuition->SetAttrs(listbrowser, LISTBROWSER_Labels, NULL, TAG_DONE);  
    while((node = IExec->RemHead(project_list))){
        IListBrowser->FreeListBrowserNode(node);    
    }
    projects = project_get_all(&count);

    if(!projects){
        printf("No projects found\n");
        IIntuition->RefreshSetGadgetAttrs((struct Gadget *)listbrowser, main_window, NULL, LISTBROWSER_Labels, project_list, TAG_DONE);
        return;
    }
    printf("Found %d projects\n", count);

    for(i = 0; i < count; i++){
        sprintf(hours_str, "%.1f", projects[i]->estimated_hours);
        
        node = IListBrowser->AllocListBrowserNode(4, 
            LBNA_Column, 0, LBNCA_CopyText, TRUE, LBNCA_Text, projects[i]->name, 
            LBNA_Column, 1, LBNCA_CopyText, TRUE, LBNCA_Text, projects[i]->customer, 
            LBNA_Column, 2, LBNCA_CopyText, TRUE, LBNCA_Text, hours_str, 
            LBNA_Column, 3, LBNCA_CopyText, TRUE, LBNCA_Text, projects[i]->status, 
            TAG_DONE);
        if(node){
            IExec->AddTail(project_list, node);
        }   
    }
    
    //attach list again
    IIntuition->RefreshSetGadgetAttrs((struct Gadget *)listbrowser, main_window, NULL, LISTBROWSER_Labels, project_list, TAG_DONE);

    project_free_all(projects, count);
    printf("Projectlist refreshed with %d projects\n", count);
    
    
}
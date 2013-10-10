#include "fpslider.h"
#define BUTTONBORDER 15
#define CHARWIDTH 12

typedef struct OPTIONDATA{
    int id,val,n;
    void (*proc)();
    char *title;
    char **strings;
    int *vals;
} Optiondata;

typedef struct optionset{
    char **labels;
    int *vals;
}Optionset;

typedef struct _menu_item {
    char        *label;         /* the label for the item */
    void       (*callback)();   /* routine to call; NULL if none */
    void *callback_data; /* client_data for callback() */
    struct _menu_item *subitems; /* pullright menu items, if not NULL */
} MenuItem;


#define Menu_item MenuItem*
#define Menu MenuItem*

struct NUMERIC{
	void (*proc)();
	int data;
	int val,min,max;
};

Menu_item make_pullright();
Menu_item menu_create_item();
Menu menu_create();

#define Xv_font int
#define Textsw_index int

#define Notify_value int
#define Notify_arg int
#define Notify_event_type int
#define Destroy_status int
#define Pixfont int
#define Panel_setting int

typedef struct NUMERIC Numeric;
void my_expose();
void my_hide();

char *my_filepath();

#ifndef RECTDEF
typedef struct {
	int rx,ry,lx,ly;
} RECTANGLE;
#define RECTDEF 1
#endif

#define PW_SRC 0
#define PW_XOR 1
#define PW_OR 2

char *my_handle_text();
Menu_item add_file_to_menu();


#define TEXTSW_INSERTION_POINT 1
#define TEXTSW_CONTENTS 1
#define TEXTSW_FILE 1
#define FRAME 1
#define FRAME_CLOSED 1
#define MESSAGE_STRINGS 1
#define MESSAGE_STRINGS_ARRAY_PTR 2
#define DESTROY_CHECKING 3
#define MENU_VALUE 5
#define MENU_ACTION_PROC 6
#define MENU_NOTIFY_PROC 6
#define MENU_PULLRIGHT 7
#define MENU_GEN_PIN_WINDOW 8
#define MENU_PARENT 9
#define MENU_STRING 10
#define MENU_DEFAULT 11
#define FRAME_LABEL XmNtitle
#define WIN_HEIGHT XmNheight
#define MENU_NCOLS 1
#define MENU_ITEM 0
#define MENU_NTH_ITEM 1
#define MENU_STRINGS 1
#define MENU_CLIENT_DATA 1
#define MENU_NITEMS 2
#define MENU_APPEND_ITEM 1
#define MENU_TITLE_ITEM 1
#define MENU_RELEASE_IMAGE 1
#define MENU_RELEASE 1
#define MENU_SELECTED 1

#define MS_LEFT 1
#define MS_RIGHT 2
#define MS_MIDDLE 3

#define NOTIFY_DONE 4
#define NOTIFY_SAFE 1
#define WIN_X XmNleftOffset
#define WIN_Y XmNtopOffset
#define WIN_WIDTH XmNwidth
#define PANEL_ALL 12
#define PANEL_CHOICE_STRINGS 12
#define PANEL_LABEL_STRING XmNtitle
#define PANEL_LABEL_X XmNleftOffset
#define PANEL_LABEL_Y XmNtopOffset
#define PANEL_LABEL 13

#define PANEL_CLIENT_DATA XmNuserData
#define PANEL_NEXT 14
#define PANEL_NONE 15
#define PANEL_VALUE XmNvalue
#define PANEL_NOTIFY_LEVEL 1
#define PANEL_NOTIFY_PROC 1
#define PANEL_MIN_VALUE 16
#define PANEL_MAX_VALUE 17
#define PANEL_READ_ONLY 18 /* not XmNeditable becuase used in my own switch */
#define NUMERIC_VALUE 19
#define PANEL_LAYOUT 1
#define PANEL_HORIZONTAL 1
#define PANEL_VALUE_DISPLAY_LENGTH XmNcolumns
#define PANEL_BACKGROUND_PROC 0
#define WIN_SHOW 0

#define alert_prompt my_prompt

#define ALERT_MESSAGE_STRINGS MESSAGE_STRINGS
#define ALERT_MESSAGE_STRINGS_ARRAY_PTR MESSAGE_STRINGS_ARRAY_PTR
#define ALERT_YES 1
#define ALERT_NO 0
#define ALERT_TRIGGERED 3
#define CMAX 256


#define POPUPBUTTON (MenuItem *) -1
#define pf_default() (Xv_Font)xv_find(NULL, FONT, NULL)

#define MY_MS_LEFT 1
#define MY_MS_RIGHT 2
#define MY_MS_MIDDLE 3
#define LOC_MOVE 4
#define LOC_DRAG 5
#define DELETE_KEY 6
#define cycle_set_value(x,y) setmenu(x,y,NULL)
Menu_item add_path_to_menu();

#define FRAME_NO_CONFIRM 1
#define WIN_EVENT_PROC 1
#define ie_locx xbutton.x
#define ie_locy xbutton.y
#define ie_time xbutton.time

#define event_is_up(ev) (ev->xany.type == ButtonRelease)
#define event_is_down(ev) (ev->xany.type == ButtonPress)
#define event_x(ev) (ev->xbutton.x)
#define event_y(ev) (ev->xbutton.y)
#define event_shift_is_down(ev) (ev->modifierKey & ShiftMask)
#define event_ctrl_is_down(ev) (ev->modifierKey & ControlMask)
#define event_meta_is_down(ev) (ev->modifierKey & Mod1Mask)
#define panel_set_value(x,y) XtVaSetValues(x, XmNvalue, y, 0)
#define panel_get_value(x) panel_get(x, XmNvalue)
#define set_fplabel(fp,s) my_set_label(fp->label,s)






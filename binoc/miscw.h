#import <X11/keysym.h>
#import <X11/keysymdef.h>
#import <Xm/MainW.h>
#import <Xm/ArrowBG.h>
#import <Xm/RowColumn.h>
#import <Xm/PushB.h>
#import <Xm/PushBG.h>
#import <Xm/CascadeB.h>
#import <Xm/CascadeBG.h>
#import <Xm/Frame.h>
#import <Xm/Scale.h>
#import <Xm/Label.h>
#import <Xm/LabelG.h>
#import <Xm/Form.h>
#import <Xm/TextF.h>
#import <Xm/ToggleB.h>
#import <Xm/ToggleBG.h>
#import <Xm/SelectioB.h>
#import <Xm/ToggleBG.h>
#import <Xm/MessageB.h>
#import <X11/Xaw/Cardinals.h>
#if defined(IRIX64) || defined(IRIX)
#import <X11/Xirisw/GlxMDraw.h>
#endif
#define BUTTONBORDER 15
#define CHARWIDTH 12

typedef struct _menu_item {
    char        *label;         /* the label for the item */
    WidgetClass *class;         /* pushbutton, label, separator... */
    char         mnemonic;      /* mnemonic; NULL if none */
    char        *accelerator;   /* accelerator; NULL if none */
    char        *accel_text;    /* to be converted to compound string */
    void       (*callback)();   /* routine to call; NULL if none */
    XtPointer    callback_data; /* client_data for callback() */
    struct _menu_item *subitems; /* pullright menu items, if not NULL */
} MenuItem;


#define Menu_item MenuItem
struct NUMERIC{
  Widget text_w;
	void (*proc)();
	int data;
	int val,min;
};

typedef struct NUMERIC Numeric;
Widget my_make_text(Widget parent, char *label,  char *val, 
 XtPointer proc, int x, int y, int w, XtPointer data);

Widget my_make_button(Widget parent, char *label,  
 int proc, int data, int x, int y);

Widget my_make_label(Widget parent, char *label, int x, int y, int w, XtPointer data);

Widget my_make_menu_button(Widget parent, char *label, int proc, MenuItem *items,  XtPointer data, int x, int y);

Widget my_make_numeric(Widget parent, char *label,  int val, 
 void (*proc)(), int x, int y, int w, int data);

Widget my_make_cycle(Widget parent, char *label, int proc, char **strings,
XtPointer data, int x, int y);

void DestroyShell(Widget w, Widget shell);

void CloseShell(Widget w, Widget shell);

#define Textsw Widget
#define Panel Widget
#define Canvas Widget
#define Frame Widget
#define Panel_item Widget
#define WIN_X XmNleftOffset
#define WIN_Y XmNtopOffset
#define Notify_value int
#define Notify_client Widget
#define Destroy_status int
#define Pixfont int
#define Panel_setting int

Panel message_in_window();
Canvas my_make_canvas();
Frame my_make_frame();
Panel my_make_panel();
Panel make_main_panel();
void my_expose();
void my_hide();
char *my_filepath();
Panel_item my_make_dir();
Panel_item my_make_filename();

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

#define MESSAGE_STRINGS 1
#define MESSAGE_STRINGS_ARRAY_PTR 2
#define DESTROY_CHECKING 3
#define NOTIFY_DONE 4
#define MENU_VALUE 5

#define Menu MenuItem*
#define window_create XtVaCreate
#define window_set XtVaSetValues
#define panel_set XtVaSetValues
#define panel_create_item XtVaCreate
#define alert_prompt my_prompt
#define window_main_loop XtRealizeApp
#define ALERT_BUTTON 1001
#define ALERT_BUTTON_YES 1
#define ALERT_BUTTON_NO 0
#define ALERT_MESSAGE_STRINGS MESSAGE_STRINGS
#define ALERT_MESSAGE_STRINGS_ARRAY_PTR MESSAGE_STRINGS_ARRAY_PTR
#define ALERT_YES 1
#define ALERT_NO 0
#define ALERT_TRIGGERED 3
#define CMAX 256
#define pf_default() (Xv_Font)xv_find(NULL, FONT, NULL)

#define MY_MS_LEFT ButtonPress
#define MY_MS_RIGHT ButtonPress
#define MY_MS_MIDDLE ButtonPress
#define cycle_set_value(x,y) setmenu(x,y,NULL)
Menu_item add_path_to_menu();

char *numeric_get_string(Widget rc);






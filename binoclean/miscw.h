#define BUTTONBORDER 15
#define CHARWIDTH 12

typedef struct _menu_item {
    char        *label;         /* the label for the item */
    char         mnemonic;      /* mnemonic; NULL if none */
    char        *accelerator;   /* accelerator; NULL if none */
    char        *accel_text;    /* to be converted to compound string */
} MenuItem;


#define Menu_item MenuItem
struct NUMERIC{
	void (*proc)();
	int data;
	int val,min;
};

typedef struct NUMERIC Numeric;

#define Notify_value int
#define Destroy_status int
#define Pixfont int
#define Panel_setting int

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

#define MESSAGE_STRINGS 1
#define MESSAGE_STRINGS_ARRAY_PTR 2
#define DESTROY_CHECKING 3
#define NOTIFY_DONE 4
#define MENU_VALUE 5

#define alert_prompt my_prompt
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





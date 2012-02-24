#ifndef WINPOSDEFS
#define WINPOSDEFS 1
#ifndef Position
#define Position int
#endif
typedef struct winloc{
    Position x,y;
    char label[20];
}Winloc;

#define NWINDOWS 10
#define STEPPER_WIN 0
#define PENLOG_WIN 1 
#define BW_WIN 2
#define AFC_WIN 3
#define MAXWINS 5
#define OPTIONS_WIN 4
#endif

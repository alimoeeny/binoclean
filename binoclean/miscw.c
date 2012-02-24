#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <math.h>
#include "myview.h"


int fplaces(float x, int n);
static char fplab[50];

struct MENUDATA{
    char *s;
    void* client_data;
    int menuval;
    int id;
};

typedef struct MENUDATA Menudata;

#define fround(x,y) ((int)(x/y) * y)

static int sliderid = 0;

#define POPUP_STRING 8
static char *sliderstrings[] = {
	"Shift Range Up", 
	"Shift Range Down", 
	"Double Range", 
	"Halve Range", 
	"Range x 10", 
	"Range/10", 
	"Center Range", 
	"+/-", 
    
	"Pop slider up", 
	NULL,
};

#define UPRANGE 0
#define DOWNRANGE 1
#define DOUBLERANGE 2
#define HALVERANGE 3
#define RANGEX10 4
#define RANGEDIV10 5
#define CENTRERANGE 6
#define SLIDER_PLUSMINUS 7
#define SLIDER_POPUP 8


int free_optiondata(Optiondata *od)
{
    int i;
    if(od->strings){
        for(i = 0; i < od->n; i++){
            if(od->strings[i])
                free(od->strings[i]);
        }
        free(od->strings);
    }
}


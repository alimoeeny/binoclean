#include <stdio.h>
#include <math.h>

#define SUBPIXLEVELS 16
typedef struct MAPENTRY{
    short red;
    short blue;
    short green;
}mapentry;


unsigned long *get_primitive(int size_x, int size_y, int rightindex, int numlevels, int dotintensity);
void rgb_draw_primitive(int size_x, int size_y, int rightindex, int levels, int intensity);
void readpixel(unsigned long *parray, int size_x, int size_y);
long rect_setgraphics();
mapentry *storemap(int levels);
void restoremap(mapentry *startmap,  int levels);
mapentry *setgreymap(int levels, int intensity);
unsigned long *rgb_calc_primitive(int size_x, int size_y, int rightindex, int levels, int intensity);

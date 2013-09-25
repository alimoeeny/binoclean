#import <stdio.h>
#import <math.h>
#import "mymath.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <sys/time.h>
#import "commdefs.h"
#import "stimuli.h"

void init_test(Stimulus *st)
{
}

void calc_test(Stimulus *st)
{
}



void test_proc(Stimulus *st)
{
    Locator *pos = &st->pos;
    int imw,imh,nreps,i,x,y;
    struct timeval start,stop;
    float timediff();
    float angle = 900;
    
    for(i = 0; i < 30; i++)
    {
        calc_stimulus(st,1);
        paint_stimulus(st,1);
        glRotatef(angle,0.0,0.0,1.0);
        paint_stimulus(st);
        glRotatef(-angle,0.0,0.0,1.0);
        change_frame();
        increment_stimulus(st, pos);
    }
}


void paint_test(Stimulus *st)
{
}

void box(int ox, int oy, int ww, int wh)
{
    float x[2],vertexcolor[3];
    
    
    vertexcolor[0] = 1.0;
    vertexcolor[1] = 1.0;
    vertexcolor[2] = 1.0;
	mycolor(vertexcolor);	
	glBegin(GL_POLYGON);
	x[0] = ox;
	x[1] = oy;
	myvx(x);
	x[0] = ox+ww-1;
    vertexcolor[0] = 0;
    vertexcolor[1] = 1;
    vertexcolor[2] = 0;
	mycolor(vertexcolor);	
	myvx(x);
	x[1] = oy+wh -1;
	myvx(x);
	x[0] = ox;
	myvx(x);
	glEnd();
    
}

void flatbox(int ox, int oy, int ww, int wh)
{
    short x[2],vertexcolor[3];
    
    
	glBegin(GL_POLYGON);
	x[0] = ox;
	x[1] = oy;
	myvx(x);
	x[0] = ox+ww-1;
	myvx(x);
	x[1] = oy+wh -1;
	myvx(x);
	x[0] = ox;
	myvx(x);
	glEnd();
    
}

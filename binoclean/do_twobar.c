#include <stdio.h>
#include <math.h>
#include "mymath.h"
#include <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#include <sys/time.h>
#include "commdefs.h"
#include "stimuli.h"
#include "sprotos.h"
extern double gammaval;
extern int verbose;

OneStim *Newtwobar(Stimulus *st,Substim *sst)
{
    OneStim *gb = (OneStim *)malloc(sizeof(OneStim));
    float fval;
    
    if(st->pos.size[0] <= 1)
        st->pos.size[0] = 250;
    if(st->pos.size[1] <= 1)
        st->pos.size[1] = 10;
    fval = 2 * st->pos.radius[1];
    st->pos.imsize[1] = rint(fval + deg2pix(1/(2*st->f)));
    FreeStimulus(sst);
    sst->ptr = gb;
    init_bar(st, sst);
    st->type = STIM_TWOBAR;
    sst->type = STIM_TWOBAR;
    return(gb);
}


void paint_annulus(Stimulus *st, Substim *sst, int mode){
    Locator *pos = &sst->pos;
    int i,ci,w,h;
    float x[2],z[2],vcolor[3],*xc,*yc,*cc,*rc,cval;
    float angle,val;
    double ra,rb;
    
    val = (st->background) * (1+ pos->contrast);
    cval = dogamma(val);
    vcolor[0] = vcolor[1] = vcolor[2] = cval;
    
    glPushMatrix();
    glTranslatef((pos->xy[0]),pos->xy[1],0);
    angle = get_polarangle(pos->xy[0], pos->xy[1]) + (float)pos->angle * 180.0/M_PI;
    glRotatef(angle,0,0,1);
    
    if(mode == LEFTMODE)
    {
        rc = cc = &vcolor[0];
    }
    else if(mode == RIGHTMODE)
    {
        cc = &vcolor[1];
        rc = &vcolor[2];
    }
    val = pos->phase;
    while(val > (2 * M_PI))
        val -= (M_PI * 2);
    
    /*
     * for annulus, pos->f determines the width of the bar, while
     * pos->radius[0,1] determines size of the hole
     */
    ra = pos->radius[0];
    rb = pos->radius[0];
    w = deg2pix(1/(2*st->f));
    
    mycolor(vcolor);
    glBegin(GL_QUAD_STRIP);
    for(angle = 0; angle < M_PI * 2.03; angle += M_PI/50){
        z[0] = ra * cos(angle);
        z[1] = ra * sin(angle);
        myvx(z);
        z[0] = (ra+w) * cos(angle);
        z[1] = (ra+w) * sin(angle);
        myvx(z);
    }
    glEnd();
    glPopMatrix();
    
}

void paint_twobar(Stimulus *st, Substim *sst, int mode)
{
    Locator *pos = &sst->pos;
    int i,ci,w,h;
    float x[2],z[2],vcolor[3],*xc,*yc,*cc,*rc,cval;
    float angle,val;
    
    val = (st->background) * (1+ pos->contrast);
    cval = dogamma(val);
    vcolor[0] = vcolor[1] = vcolor[2] = cval;
    
    glPushMatrix();
    glTranslatef((pos->xy[0]),pos->xy[1],0);
    angle = get_polarangle(pos->xy[0], pos->xy[1]) + (float)pos->angle * 180.0/M_PI;
    glRotatef(angle,0,0,1);
    
    if(mode == LEFTMODE)
    {
        rc = cc = &vcolor[0];
    }
    else if(mode == RIGHTMODE)
    {
        cc = &vcolor[1];
        rc = &vcolor[2];
    }
    val = pos->phase;
    while(val > (2 * M_PI))
        val -= (M_PI * 2);
    
    /*
     * for bars, pos->f determines the width of the bar, while
     * pos->radius[1] determines the field over which the bar moves
     */
    h = pos->radius[0];
    w = deg2pix(1/(2*st->f));
    z[0] = -pos->radius[0];
    z[1] = (((pos->radius[1]) * cos(val)) - w/2)  - (pos->barsep)/2;
    
    glBegin(GL_POLYGON);
    mycolor(vcolor);	
    x[0] = z[0];
    x[1] = z[1];
    myvx(x);
    x[1] = z[1] + w;
    myvx(x);
    x[0] = z[0] +2 *  pos->radius[0];
    myvx(x);
    x[1] = z[1];
    myvx(x);
    glEnd();
    
    z[1] = (((pos->radius[1]) * cos(val)) - w/2)  + (pos->barsep)/2;
    glBegin(GL_POLYGON);
    mycolor(vcolor);	
    x[0] = z[0];
    x[1] = z[1];
    myvx(x);
    x[1] = z[1] + w;
    
    myvx(x);
    x[0] = z[0] +2 *  pos->radius[0];
    myvx(x);
    x[1] = z[1];
    myvx(x);
    glEnd();
    glPopMatrix();
    
}


float get_polarangle(float x, float y)
{
    
    if(x > 0 && y >= 0)
        return (atan(y / x)) * (180 / M_PI);
    if(x < 0)
        return (M_PI + atan(y / x)) * (180 / M_PI);
    if(x > 0 && y < 0)
        return ((2 * M_PI) + atan(y / x)) * (180 / M_PI);
    if(x == 0 && x >= 0)
        return 90.0;
    if(x == 0 && x < 0)
        return 270.0;
}

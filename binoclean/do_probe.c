#include <stdio.h>
#include <math.h>
#include "mymath.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#include <sys/time.h>
#include "commdefs.h"
#include "stimuli.h"
#include "sprotos.h"
extern double gammaval;

OneStim *NewProbe(Stimulus *st,Substim *sst)
{
    OneStim *gb;
    
    st->pos.size[0] = 250;
    st->pos.size[1] = 10;
    st->pos.size[0] = 250;
    st->pos.size[1] = 10;
    FreeStimulus(sst);
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
    
    init_probe(st, sst);
    st->type = STIM_PROBE;
    sst->type = STIM_PROBE;
    return(sst->ptr);
}

void free_probe(Substim *st)
{
    OneStim *ptr;
    
    if((ptr = (OneStim *)st->ptr) != NULL)
    {
        //      free(ptr);
    }
}


void init_probe(Stimulus *st, Substim *sst)
{
}

void calc_probe(Stimulus *st)
{
}



void extent_probe(Stimulus *st, Rectangle *pr)
{
    int w,h;
    Locator *pos = &st->pos;
    
    h = pos->radius[0];
    w = deg2pix(1/st->f);
    pr->lx = (-pos->imsize[0]);
    pr->rx = (pos->imsize[0]);
    
    pr->ly = (pos->imsize[1]);
    pr->ry = -(pos->imsize[1]);
}

void paint_alt_probe(Stimulus *st, Substim *sst, int mode)
{
    Locator *pos = &sst->pos;
    int i,ci,w,h;
    vcoord *p,*q,*end,vx,vy,*vpx,*vpy,*vpoy,*vpox;
    vcoord x[2],z[2],vcolor[3],*xc,*yc,*cc,*rc,c,*lc;
    float angle;
    double val,cval;
    
    vcolor[0] = vcolor[1] = vcolor[2] = st->gammaback;
    
    glPushMatrix();
    glTranslatef((pos->xy[0]),pos->xy[1],0);
    angle = (float) (pos->angle * 180.0/M_PI);
    glRotatef(angle,0.0,0.0,1.0);
    
    if(mode == LEFTMODE)
    {
        lc = rc = cc = &vcolor[0];
    }
    else if(mode == RIGHTMODE)
    {
        lc = cc = &vcolor[1];
        rc = &vcolor[2];
    }
    else if(mode == BOTHMODE)
    {
        lc = &vcolor[0];
        cc = &vcolor[1];
        rc = &vcolor[2];
    }
    val = pos->phase;
    while(val > (2 * M_PI))
        val -= (M_PI * 2);
    
    /*
     * for probes, pos->f determines the width of the probe, while
     * pos->radius[1] determines the field over which the probe moves
     */
    h = pos->radius[0];
    w = deg2pix(1/(2*st->f));
    z[0] = (-pos->radius[0]);
    z[1] = ((-w/2));
    glBegin(GL_POLYGON);
    mycolor(vcolor);	
    x[0] = z[0];
    x[1] = z[1];
    myvx(x);
    x[1] = z[1] + w;
    myvx(x);
    x[0] = z[0] + (pos->radius[0]-pos->radius[1]);
    myvx(x);
    x[1] = z[1];
    myvx(x);
    glEnd();
    z[0] = (pos->radius[0]);
    z[1] = ((-w/2));
    glBegin(GL_POLYGON);
    mycolor(vcolor);	
    x[0] = z[0];
    x[1] = z[1];
    myvx(x);
    x[1] = z[1] + w;
    myvx(x);
    x[0] = z[0] - (pos->radius[0]-pos->radius[1]);
    myvx(x);
    x[1] = z[1];
    myvx(x);
    glEnd();
    glPopMatrix();
    
}

void paint_probe(Stimulus *st, Substim *sst, int mode)
{
    Locator *pos = &sst->pos;
    int i,ci,w,h;
    vcoord *p,*q,*end,vx,vy,*vpx,*vpy,*vpoy,*vpox;
    vcoord x[2],z[2],*xc,*yc,*cc,*rc,c;
    float vcolor[3];
    float angle;
    double val,cval;
    
    vcolor[0] = vcolor[1] = vcolor[2] = st->gammaback;
    
    glPushMatrix();
    glTranslatef((pos->xy[0]),pos->xy[1],0);
    angle = (float) (pos->angle * 180.0/M_PI);
    glRotatef(angle,0.0,0.0,1.0);
    
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
     * for probes, pos->f determines the width of the probe, while
     * pos->radius[1] determines the field over which the probe moves
     */
    h = pos->radius[0];
    w = deg2pix(1/(2*st->f));
    z[0] = (-pos->radius[0]);
    z[1] = ((-w/2));
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
    z[0] = (-w/2);
    z[1] = ((-pos->radius[1]));
    glBegin(GL_POLYGON);
    mycolor(vcolor);	
    x[0] = z[0];
    x[1] = z[1];
    myvx(x);
    x[1] = z[1] + 2 * pos->radius[1];
    myvx(x);
    x[0] = z[0] + w;
    myvx(x);
    x[1] = z[1];
    myvx(x);
    glEnd();
    glPopMatrix();
    
}


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

OneStim *NewSquare(Stimulus *st,Substim *sst)
{
    OneStim *gb;
    
    st->pos.size[0] = 250;
    st->pos.size[1] = 10;
    st->pos.size[0] = 250;
    st->pos.size[1] = 10;
    FreeStimulus(sst);
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
    gb = sst->ptr; 
    init_square(st, sst);
    st->type = STIM_SQUARE;
    sst->type = STIM_SQUARE;
    return(gb);
}

void free_square(Substim *st)
{
    OneStim *ptr;
    
    if((ptr = (OneStim *)st->ptr) != NULL)
    {
        //      free(ptr);
    }
}


void init_square(Stimulus *st, Substim *sst)
{
}

void calc_square(Stimulus *st)
{
}



void extent_square(Stimulus *st, Rectangle *pr)
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

void paint_square(Stimulus *st, Substim *sst, int mode)
{
    Locator *pos = &sst->pos;
    int i,ci,w,h,ncycles;
    short *p,*q,*end,vx,vy,*vpx,*vpy,*vpoy,*vpox;
    vcoord x[2],z[2],*xc,*yc,c;
    float offcolor[3],vcolor[3],*rc,*cc;
    float angle, oldplaid;
    double val,cval;
    
    if(sst->ptr->plaid_angle < -0.1)
        val = (double)(st->background) * (1+ pos->contrast_amp);
    else
        val = (double)(st->background) * (1+ pos->contrast);
    if(st->background < 0.01 && pos->contrast > 0)
        val = pos->contrast;
    
    vcolor[0] = vcolor[1] = vcolor[2] = dogamma(val);
    
    if(sst->ptr->plaid_angle < -0.1)
        val = (double)(st->background) * (1- pos->contrast_amp);
    else
        val = (double)(st->background) * (1- pos->contrast);
    if(st->background < 0.01 && pos->contrast < 0)
        val = -pos->contrast;
    
    offcolor[0] = offcolor[1] = offcolor[2] = dogamma(val);
    
    glPushMatrix();
    glTranslatef((pos->xy[0]),pos->xy[1],0);
    angle  = (pos->angle+sst->ptr->plaid_angle/2) * 180/M_PI;
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
     * for ss, pos->f determines the width of the square, while
     * pos->radius[1] determines the field over which the squaren moves
     */
    h = pos->radius[0];
    w = deg2pix(1/(2*st->f));
    ncycles = pos->radius[1]/w;
    z[1] = -w * val/M_PI - (pos->radius[1]);
    z[0] = (-pos->radius[0]);
    while(z[1] < pos->radius[1])
    {
        mycolor(vcolor);	
        glBegin(GL_POLYGON);
        x[0] = z[0];
        x[1] = z[1];
        if(x[1] < -pos->radius[1])
            x[1] = -pos->radius[1];
        else if(x[1] > pos->radius[1])
            x[1] = pos->radius[1];
        myvx(x);
        x[1] = z[1] + w;
        if(x[1] < -pos->radius[1])
            x[1] = -pos->radius[1];
        else if(x[1] > pos->radius[1])
            x[1] = pos->radius[1];
        myvx(x);
        x[0] = z[0] +2 *  pos->radius[0];
        myvx(x);
        x[1] = z[1];
        if(x[1] < -pos->radius[1])
            x[1] = -pos->radius[1];
        else if(x[1] > pos->radius[1])
            x[1] = pos->radius[1];
        myvx(x);
        glEnd();
        z[1] += w;
        
        mycolor(offcolor);	
        glBegin(GL_POLYGON);
        x[0] = z[0];
        x[1] = z[1];
        if(x[1] < -pos->radius[1])
            x[1] = -pos->radius[1];
        else if(x[1] > pos->radius[1])
            x[1] = pos->radius[1];
        myvx(x);
        x[1] = z[1] + w;
        if(x[1] < -pos->radius[1])
            x[1] = -pos->radius[1];
        else if(x[1] > pos->radius[1])
            x[1] = pos->radius[1];
        myvx(x);
        x[0] = z[0] +2 *  pos->radius[0];
        myvx(x);
        x[1] = z[1];
        if(x[1] < -pos->radius[1])
            x[1] = -pos->radius[1];
        else if(x[1] > pos->radius[1])
            x[1] = pos->radius[1];
        myvx(x);
        glEnd();
        
        z[1] += w;
    }
    
    glPopMatrix();
    if(sst->ptr->plaid_angle > 0.01)
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendColor(0.5, 0.5, 0.5,  0.5);
        glBlendFunc (GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_COLOR);
        oldplaid = sst->ptr->plaid_angle;
        sst->ptr->plaid_angle = -sst->ptr->plaid_angle;
        paint_square(st, sst, mode);
        glDisable(GL_BLEND);
        sst->ptr->plaid_angle = oldplaid;
    }
    
}


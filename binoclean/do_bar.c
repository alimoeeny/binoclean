#import <stdio.h>
#import <math.h>
#ifdef WINDOWS
#import "windows.h"
#import "winbinoc.h"
#else
#import "mymath.h"
#import <sys/time.h>
#endif
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import "commdefs.h"
#import "stimuli.h"
#import "sprotos.h"

extern double gammaval;
extern int optionflag;
extern int debug;

OneStim *NewBar(Stimulus *st,Substim *sst)
{
    OneStim *gb;
    float fval;
    
    if(st->pos.size[0] <= 1)
        st->pos.size[0] = 250;
    if(st->pos.size[1] <= 1)
        st->pos.size[1] = 10;
    fval = 2 * st->pos.radius[1];
    st->pos.imsize[1] = rint(fval + deg2pix(1/(2*st->f)));
    FreeStimulus(sst);
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
    gb = sst->ptr;
    sst->nbars = 0;
    init_bar(st, sst);
    st->type = STIM_BAR;
    sst->type = STIM_BAR;
    return(gb);
}

void free_bar(Substim *st)
{
    /*
     if(st->ptr != NULL)
     {
     free(st->ptr);
     }
     */
}


void init_bar(Stimulus *st, Substim *sst)
{
    st->pos.barsep = NOTSET;
    sst->pos.barsep = NOTSET;
    if(sst->nbars > sst->xpl)
    {
        sst->xpl = sst->nbars;
        if(sst->xpos != NULL)
            free(sst->xpos);
        sst->xpos = (vcoord *)malloc(sst->xpl * sizeof(vcoord));
    }
    if(sst->nbars > sst->ypl)
    {
        sst->ypl = sst->nbars;
        if(sst->ypos != NULL)
            free(sst->ypos);
        sst->ypos = (vcoord *)malloc(sst->ypl * sizeof(vcoord));
    }
    if(sst->nbars > sst->imblen || sst->imb == NULL) /* need new memory */
    {
        if(sst->imb != NULL)
            free(sst->imb);
        sst->imblen = sst->nbars;
        sst->imb = (float *)malloc(sst->imblen * sizeof(float));
    }
    sst->nbars = 0;
}

void calc_bar(Stimulus *st)
{
}



void extent_bar(Stimulus *st, Rectangle *pr)
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

void paint_bar(Stimulus *st, Substim *sst, int mode)
{
    Locator *pos = &sst->pos;
    int i,ci,w,h;
    float x[2],z[2],vcolor[3],*xc,*yc,*cc,*rc,cval;
    float angle,val;
    
    val = (st->background) * (1+ pos->contrast);
    if(pos->contrast > 2)
        val = 1.0;
    else if (st->background == 0)
        val = pos->contrast;
    cval = dogamma(val);
    vcolor[0] = vcolor[1] = vcolor[2] = cval;
    
    glPushMatrix();
    glTranslatef((pos->xy[0]),pos->xy[1],0);
    angle = (float) (pos->angle * 180.0/M_PI);
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
    if (st->type == STIM_RECT)
        val = M_PI;
    else{
    val = pos->phase;
    while(val > (2 * M_PI))
        val -= (M_PI * 2);
    }
    /*
     * for bars, pos->f determines the width of the bar, while
     * pos->radius[1] determines the field over which the bar moves
     */
    h = pos->radius[0];
    w = deg2pix(1/(2*st->f));
    z[0] = (-pos->radius[0]);
    /*
     * if SQUARE, have constant velocity sweeps in one direction. Sign reversal 
     * is so that direction matches sines/rds
     */
    
    if(st->type == STIM_RECT){
        z[1] = -(w/2);
        sst->nbars = 0;
    }
    else if(st->flag & STIMULUS_IS_SQUARE)
        z[1] = -w/2 - ((pos->radius[1]) * (val/M_PI -1));
    else
        z[1] = ((pos->radius[1]) * cos(val)) - w/2;
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
    
    if(optionflag & ANTIALIAS_BIT)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glLineWidth(1.0);
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
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
        glDisable(GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    glPopMatrix();
    for(i = 0; i < sst->nbars; i++){
        glPushMatrix();
        glTranslatef(sst->xpos[i],sst->ypos[i],0);
        angle = (float) (sst->imb[i] * 180.0/M_PI);
        glRotatef(angle,0,0,1);
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
        if(debug){
            glFlushRenderAPPLE();
            glSwapAPPLE();
        }
    }
    
}


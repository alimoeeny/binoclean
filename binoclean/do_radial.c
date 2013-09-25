#include <stdio.h>
#include "mymath.h"
#include <math.h>
#import <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include "commdefs.h"
#include "stimuli.h"
#include "sprotos.h"

int bullseye = 1;

void init_radial(Stimulus *st, Substim *sst)
{
    
	float *p;
	int i,j,w,h;
	float val,xval,yval,x,y,cval,cm,deg,sx,sy;
	Locator *pos = &sst->pos;
	OneStim *gb = sst->ptr;
    
	w = st->pos.imsize[0];
	h = st->pos.imsize[1];
	sst->calculated = 0;
	if(gb->sy < 0.01 || gb->sy > 20.0) /* error: left over memory*/
        gb->sy = 0.0;
    
    /*
     * if previous stimulus type was not a grating, we don't want to inherit
     * some arbitary values into the Grating Structure, so force plaid angle to 0
     */
    
	if(st->lasttype != STIM_GRATING && st->lasttype != STIM_GRATING2)
        gb->plaid_angle = 0;
	if(w * h > sst->iimlen || sst->iim == NULL) 
	{
        sst->iimlen = w * h + 2;
        if(sst->iim != NULL)
            free(sst->iim);
        sst->iim = (int *)malloc(sst->iimlen * sizeof(int));
	}
	if(w * h > sst->imblen || sst->imb == NULL || sst->imc == NULL) 
	{
        if(sst->imb != NULL)
            free(sst->imb);
        sst->imb = (float *)malloc(sst->imlen * sizeof(float));
        if(sst->imc != NULL)
            free(sst->imc);
        sst->imc = (float *)malloc(sst->imlen * sizeof(float));
	}
	memcpy(pos,&st->pos,sizeof(Locator));
}

OneStim *NewRadial(Stimulus *st, Substim *sst, Substim *copy)
{
    
    FreeStimulus(sst);  /* free up other stimulus-structure */
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
    /*
     * force Xsampling to be 2 inititally, since there is a bug with 1
     * currently
     */
    st->pos.size[1] = st->pos.imsize[1]/2;
    init_radial(st, sst);
    st->type = STIM_RADIAL;
    sst->type = STIM_RADIAL;
    return(sst->ptr);
}


void free_radial(Substim *st)
{
    OneStim *ptr;
    
    if((ptr = (OneStim *)st->ptr) != NULL)
    {
        //      free(ptr);
    }
}


void paint_radial(Substim *sst, int mode, int shift)
{
    int w = sst->pos.imsize[0];
    int h = sst->pos.imsize[1];
    glRasterPos2i(sst->pos.xy[0]-w/2,sst->pos.xy[1]-h/2);
    glDrawPixels(w, h, GL_LUMINANCE, GL_FLOAT, sst->imc);
}



float SetRadial(int ix, int iy, int w, int h, Substim *sst, int type)
{ 
    float r = sqrt(sqr((ix-w/2)) + sqr((iy-h/2)));
    float *ip,lum,env,theta,a,*ipc;
    float dr = r - sst->ptr->radius,ds;
    int *im;
    
    ip = &sst->imb[ix + iy*w];
    ipc = &sst->imc[ix + iy*w];
    if(bullseye && r < sst->ptr->radius - sst->ptr->rsigma *2)
    {
        lum = cos((r) * 2 * M_PI * sst->pos.pixsf + sst->pos.phase);
        env = (float)exp(-(sqr(r))/( 2  * sqr(sst->ptr->pixelsy)));
        *ip =  env * lum * 0.5;
        *ipc =  0.5 + (env * lum * 0.5);
        return(*ip);
    }
    
    im = &sst->iim[ix + iy*w];
    *im = 1;
    env = (float)exp(-(sqr(dr))/sqr(sst->ptr->rsigma));
    theta = atan2(iy-h/2,ix-w/2);
    while(theta -sst->ptr->ori2 >  M_PI)
        theta -= ( 2* M_PI);
    while(theta -sst->ptr->ori2 < -M_PI)
        theta += ( 2 * M_PI);
    
    /*
     * dr = radial distance from centre of the annulus.
     */
    
    if(type == CIRCULAR_GRATING){
        lum = cos((r-sst->ptr->radius) * 2 * M_PI * sst->pos.pixsf + sst->pos.phase);
        if(fabs(theta-sst->ptr->ori2) < sst->wedge_angle)
            *im = 2;
    }
    else if(type == SPIRAL_GRATING){
        /*
         * ds is a phase increment added as a function of radial distance
         */
        if(sst->pos.angle < 0){
            ds = -0.5 * sst->ptr->radius * (1 - cos(dr/sst->ptr->radius));
            lum = cos(sst->pos.phase + 2 * M_PI * (theta * sst->ptr->radius + dr+ds) * sst->pos.pixsf);
        }
        else{
            ds = 0.5 * sst->ptr->radius * (1 - cos(dr/sst->ptr->radius));
            lum = cos(sst->pos.phase + 2 * M_PI * (theta * sst->ptr->radius - dr+ds) * sst->pos.pixsf);
        }
        if(env > 0.05)
            env = 1;
        else
            env = 0;
    }
    else if(type == RADIAL_GRATING){
        lum = cos(2 * M_PI * theta * sst->ptr->radius * sst->pos.pixsf + sst->pos.phase);
        if((a = fabs(theta-sst->ptr->ori2)) < sst->wedge_angle){
            *im = 1 + 1000 * (exp(-(a * a)/(sst->wedge_angle * sst->wedge_angle*0.5)));
            //	  *im = 1000;
        }
    }
    else{
        lum = 0;
    }
    
    *ip =  env * lum * 0.5;
}


void precalc_radial(Stimulus *st, Substim *sst, float disp)
{
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    float lumscale,sd,x,y,rmax,rmin;
    int ix,iy,ymin,ymax;
    int w = st->pos.imsize[0];
    int h = st->pos.imsize[1];
    float *ip,*im;
    int *iim;
    int type = CIRCULAR_GRATING;
    
    type  =  sst->stimmode;
    
    lumscale = st->background * pos->contrast;
    sd = gb->rsigma;
    sst->pos.pixsf = pix2deg(sst->pos.sf);
    rmax = gb->radius + 2 * sd;
    rmin = gb->radius - 2 * sd;
    memset(sst->imb,0,w * h * sizeof(float));
    ip = sst->imb;
    im = sst->imc;
    iim = sst->iim;
    for(ix = 0; ix < w * h; ix++,ip++,im++,iim++){
        *ip = -10;
        *im = 0.5;
        *iim = 0;
    }
    if(sst->ptr->sy > 0)
        bullseye = 1;
    else
        bullseye = 0;
    for(ix = 0; ix < w; ix++){
        x = ix - (float)(w)/2;
        if(sqr(x) > sqr(rmax)){
            ip = &sst->imb[ix + iy*w];
        }
        else{
            y = sqrt(sqr(rmax) - sqr(x));
            ymax = (int)(ceil(y));
            if(ymax > h/2)
                ymax = h/2;
            if(sqr(x) > sqr(rmin)){
                for(iy = h/2-ymax; iy < h/2+ymax; iy++){
                    SetRadial(ix,iy,w,h,sst,type);
                }
            }
            else{
                ymin = (int)floor(sqrt(sqr(rmin)-sqr(x)));
                if(ymin > h/2)
                    ymin = h/2;
                if(bullseye){
                    for(iy = h/2-ymax; iy < h/2+ymax; iy++){
                        SetRadial(ix,iy,w,h,sst,type);
                    }
                }
                else{
                    for(iy = h/2+ymin; iy < h/2+ymax; iy++){
                        SetRadial(ix,iy,w,h,sst,type);
                    }
                    for(iy = h/2-ymin; iy > h/2-ymax; iy--){
                        SetRadial(ix,iy,w,h,sst,type);
                    }
                }
            }
        }
    }
    sst->calculated = 1;
}


void calc_radial(Stimulus *st, Substim *sst, float disp)
{
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    float lumscale,sd,x,y,rmax,rmin;
    int ix,iy,ymin,ymax;
    int w = st->pos.imsize[0];
    int h = st->pos.imsize[1];
    float *ip,*im,maxc;
    int i,*iim;
    
    int type = CIRCULAR_GRATING;
    
    if(!sst->calculated)
        precalc_radial(st, sst, disp);
    
    ip = sst->imb;
    im = sst->imc;
    iim = sst->iim;
    /*
     *  calculate the actual image by multiplying the precalculated image * contrast.
     * allows counterphase modulation without having to recalculated the whole thing
     * a sement may be shown at higher contrast than the rest, indicated with iim == 2.
     * 
     */
    if(sst->pos.contrast < 0)
        maxc = -sst->pos.contrast2 - sst->pos.contrast;
    else
        maxc = sst->pos.contrast2 - sst->pos.contrast;
    for(i = 0; i < w * h; i++,ip++,im++,iim++){
        if(*iim){
            if(*iim > 1 && sst->show_increment)
                *im = 0.5 + (*ip * (sst->pos.contrast + maxc * (*iim-1)/1000));
            else
                *im = 0.5 + (*ip * sst->pos.contrast);
        }
    }
    sst->calculated = 2;
}

/*
 void increment_radial(Stimulus *st){
 Substim *sst;
 float lumscale,sd,x,y,rmax,rmin,theta;
 int ix,iy,ymin,ymax;
 int w = st->pos.imsize[0];
 int h = st->pos.imsize[1];
 int type = RADIAL_GRATING;
 
 if(st->left->stimmode == RADIAL_GRATING)
 sst = st->left;
 else
 sst = st->right;
 
 sd = sst->ptr->rsigma;
 sst->pos.pixsf = pix2deg(sst->pos.sf);
 rmax = gb->radius + 2 * sd;
 rmin = gb->radius - 2 * sd;
 
 for(ix = 0; ix < w; ix++){
 x = ix - (float)(w)/2;
 if(sqr(x) < sqr(rmax)){
 y = sqrt(sqr(rmax) - sqr(x));
 ymax = (int)(ceil(y));
 if(ymax > h/2)
 ymax = h/2;
 if(sqr(x) > sqr(rmin))
 ymin = 0;
 else
 ymin = (int)floor(sqrt(sqr(rmin)-sqr(x)));
 if(ymin > h/2)
 ymin = h/2;
 for(iy = h/2+ymin; iy < h/2+ymax; iy++){
 theta = atan2(iy-h/2,ix-w/2);
 if(theta > -0.5 && theta < 0.5) 
 SetRadial(ix,iy,w,h,sst,type);
 }
 for(iy = h/2-ymin; iy > h/2-ymax; iy--){
 theta = atan2(iy-h/2,ix-w/2);
 if(theta > -0.5 && theta < 0.5) 
 SetRadial(ix,iy,w,h,sst,type);
 }
 }
 
 }
 }
 
 */

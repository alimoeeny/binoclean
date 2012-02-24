#include <stdio.h>
#include "mymath.h"
#include <math.h>
#include "commdefs.h"
#include "stimuli.h"
//#include "sprotos.h"
#include "myview.h"
#include "protos.h"

extern int option2flag;
extern int optionflags[];
extern double gammaval;
extern int optionflag;
int init_rds(Stimulus *st,  Substim *sst, float density);

/*
 * NewRds Allocate memory for a new RDS stimulus structure
 * and initializes it.
 */

OneStim *NewCorrug(Stimulus *st, Substim *sst, Substim  *copy)
{
    OneStim *gb;
    Locator *pos = &st->pos;/*j*/
    
    FreeStimulus(sst);
    gb = GetOneStimPtr(sst);
    if (pos->dotsize > 0.5) /*j fudge so that cyl and rds share common dotsize*/
        gb->dotsiz[0] = gb->dotsiz[1] = pos->dotsize;
    else
        gb->dotsiz[0] = gb->dotsiz[1] = 5;
    gb->ndots = 0;
    if(pos->density != 0)
        gb->density = pos->density;
    else
        gb->density = 20;
    sst->seed = sst->baseseed = 1001;
    sst->seedloop = 0;
    sst->ptr = gb;
    init_corrug(st,sst,gb->density);
    st->type = STIM_CORRUG;
    sst->type = STIM_CORRUG;
    return(gb);
}

/*
 * free corrug frees up the memory used by and CORRUG stimulus structure
 */
void free_corrug(Substim *st)
{
    OneStim *ptr;
    
    if((ptr = st->ptr) != NULL)
    {
        free(ptr->ypos);
        ptr->ypos = NULL;
        /*      free(ptr);
         st->ptr = 0;
         */
    }
}

/*
 * init_corrug initializes an CORRUG stimulus structure. This
 * involves allocating memory for a list of dot locations
 * long enough to produce the desired to-density, given
 * the stimulus size
 */
#define CIRC2SQU  (4/3.14)

int init_corrug(Stimulus *st,  Substim *sst, float density)
{
    
	int i,j,*p;
	double val,xval,yval,x,y,cval,cm,deg,sx,sy;
	Locator *pos = &st->pos;
	OneStim *gb = (OneStim *)sst->ptr;
	int ndots, cndots;
    
	if(density > 0)
        gb->density = density;
	else if(gb->density <= 0.0)
        gb->density = 20.0;
    /*
     * calculate actual number of dots from density
     * N.B. 25 * dotsiz * dotsiz = 100 * dotsiz/2 * dotsiz/2,
     * = area of dot
     */
    if(optionflag & SQUARE_RDS)
        ndots = gb->density * 4 * pos->radius[0] * pos->radius[1]/(100 * gb->dotsiz[0] * gb->dotsiz[1]);
    else
        ndots = gb->density * M_PI * pos->radius[0] * pos->radius[1]/(100 * gb->dotsiz[0] * gb->dotsiz[1]);
    
    cndots = 1 + (ndots * CIRC2SQU);
	if(cndots > sst->imlen || gb->im == NULL) /* need new memory */
	{
        if(gb->im != NULL)
            free(gb->im);
        gb->im = (int *)malloc(cndots * sizeof(int));
        sst->imlen = cndots;
	}
	if(cndots > sst->xpl || gb->xpos == NULL) /* need new memory */
	{
        if(gb->xpos != NULL)
            free(gb->xpos);
        gb->xpos = (vcoord *)malloc(cndots * sizeof(vcoord));
        sst->xpl = cndots;
	}
	if(ndots > gb->ndots || gb->ypos == NULL) /* need new memory */
	{
        if(gb->ypos != NULL)
            free(gb->ypos);
        gb->ypos = (vcoord *)malloc(cndots * sizeof(vcoord));
	}
	if(ndots > 0)
        gb->ndots = ndots;
	p = gb->im;
	for(i = 0; i < gb->ndots; i++)
        *p++ = random();
	for(i = 0; i < 2; i++)
	{
        if(pos->imsize[i] == 0)
            pos->imsize[i] = 256;
        if(st->pos.radius[i] ==0)
            st->pos.radius[i] = st->pos.imsize[i]/2;
	}
	gb->xshift = 0;
    memcpy(&sst->pos,&st->pos,sizeof(Locator));
	if(st->depth_mod > 0)
        SetStimulus(st, st->depth_mod, DEPTH_MOD, NULL);
	return(gb->ndots);
}
#define PIXM 10
void calc_corrug(Stimulus *st, Substim *sst)
{
    int i,j,partdisp,ndots;
    float cval,f,sy,cm,deg,iscale[2],val[2];
    float asq,bsq,csq,dsq,xsq,ysq,pixdisp[2],offset[2],sindisp[2];
    float cosa, sina;
    int *p,*q,*cend;
    vcoord *x,*y,w,h, y_tra_with_angle;
    int xshift[2],iw,ih;
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    float phase,contrast = pos->contrast, testval;
    float iphase;
    
    float angle;
    float depth_freq, fore_amplitude, back_amplitude; /* depth_freq in c/deg from StimulusProperty - need to convert*/
    float amplitude, dotdepth;
    
    iphase = deg_rad(st->vals[START_PHASE]);
    if (StimulusProperty((Stimulus *)st, SF)<0.01)
    {
        calc_plane(st,sst);
        return;
    }
    if(st->flag & ANTICORRELATE && sst->mode == RIGHTMODE)
        contrast = -pos->contrast;
    if(st->flag & CONTRAST_NEGATIVE)
        val[0] = (double)st->background * (1 - contrast);
    else
        val[0] = (double)st->background * (1 + contrast);
    if(optionflags[COUNTERPHASE]){
        phase = iphase;
        amplitude =  gb->depth_mod * cos(sst->pos.phase);
    }
    else{
        phase = sst->pos.phase;
        amplitude =  gb->depth_mod;
    }
    if(st->flag & CONTRAST_POSITIVE)
    {
        if(contrast >= 1.0)
            val[1]  = 1.0;
        else
            val[1] = (float)st->background * (1 + contrast);
    }
    else
        val[1] = val[0];
    for(i = 0; i < 2; i++)
        gb->lum[i] = dogamma(val[i]);
    
    offset[0] = offset[1] = 0;
    if(optionflag & SQUARE_RDS)
        ndots = gb->density * 4 * pos->radius[0] * pos->radius[1]/(100 * gb->dotsiz[0] * gb->dotsiz[1]);
    else
        ndots = gb->density * M_PI * pos->radius[0] * pos->radius[1]/(100 * gb->dotsiz[0] * gb->dotsiz[1]);
    
    if(ndots > gb->ndots)
        gb->ndots = init_corrug(st,sst,gb->density);
    else
        gb->ndots = ndots;
    /* 
     *if st->prev != NULL, this is a stimuls that forms a background 
     * that means a ``hole'', with an appropriate disparity, needs to
     * be made in the background
     */
    cosa = cos(pos->angle);
    sina = sin(pos->angle);
    
    if(st->prev != NULL)
    {
        if(optionflag & BACKGROUND_FIXED_BIT)
        {
            offset[0] = st->prev->pos.xy[0] + (st->pos.xy[0] * cosa)
            + (st->pos.xy[1] * sina);
            offset[1] = st->prev->pos.xy[0] + (st->pos.xy[0] * sina)
            - (st->pos.xy[1] * cosa);
        }
        pixdisp[0] = (st->prev->disp - st->disp) * cosa +
        (st->prev->vdisp - st->vdisp) * sina;
        pixdisp[1] = (st->disp - st->prev->disp) * sina +
        (st->prev->vdisp - st->vdisp) * cosa;
    }
    else
    {  
        pixdisp[0] =  pixdisp[1] = 0;
    }
    
    /*
     phase = 0;
     */
    
    if(gb->xshift != 0)
    {
        xshift[0] = (int)(rint(gb->xshift * cosa));
        xshift[1] = (int)(rint(gb->xshift * sina));
        if(sst->seed & 1)
        {
            xshift[0] = -xshift[0];
            xshift[1] = -xshift[1];
        }
        xshift[1] += phase;
    }
    else
    {
        xshift[1] = phase;
        xshift[0] = 0;
    }
    csq = dsq= 0;
    w = (vcoord)(pos->imsize[0] - gb->dotsiz[0]);
    h = (vcoord)(pos->imsize[1] - gb->dotsiz[1]);
    w = (vcoord)(2 * pos->radius[0] - gb->dotsiz[0]);
    h = (vcoord)(2 * pos->radius[1] - gb->dotsiz[1]);
    srandom(sst->seed);
    if(!(optionflag & SQUARE_RDS))
    {
        asq = pos->radius[0] * pos->radius[0];
        bsq = pos->radius[1] * pos->radius[1];
        /* calculate parameters for hole */
        if(st->prev != NULL && st->prev->type == STIM_CORRUG)
        {
            csq = st->prev->pos.radius[0] * st->prev->pos.radius[0];
            dsq = st->prev->pos.radius[1] * st->prev->pos.radius[1];
        }
    }
    else  if(st->prev != NULL && st->prev->type == STIM_CORRUG) /* square background */
    {
        csq = fabsf(st->prev->pos.radius[0]);
        dsq = fabsf(st->prev->pos.radius[1]);
    }
    p = gb->im;
    x = gb->xpos;
    y = gb->ypos;
    iw = w;
    ih = h;
    /* now set random x,y locations */
    depth_freq =  StimulusProperty((Stimulus *)st, SF);
    depth_freq = 1 / (   deg2pix(1/depth_freq)  );
    if(st->prev != NULL){
        depth_freq =  StimulusProperty((Stimulus *)st->prev, SF);
        depth_freq = 1 / (   deg2pix(1/depth_freq)  );
        if(optionflags[COUNTERPHASE]){
            fore_amplitude =  st->prev->left->ptr->depth_mod * cos(sst->pos.phase);
            phase = iphase;
        }
        else{
            fore_amplitude =  st->prev->left->ptr->depth_mod;
        }
    }
    for(i = 0; i < gb->ndots; )
    {
        do{
            *p = random();
            *x = (float)((*p + xshift[0]) % (iw*PIXM))/PIXM - w/2;
            *y = (float)(((*p>16) + xshift[1]) % (ih*PIXM))/PIXM - h/2;
            if(optionflag & SQUARE_RDS)
                testval = 0;
            else
            {
                xsq = (*x) * (*x);
                ysq = (*y) * (*y);
                testval = (xsq/asq + ysq/bsq); /* outside circle, try again */
            }
        }while(testval > 1.0);
        
        /**** To calculate position of hole in background it is necessary to subtract pixdisp[1] from 
         * the y value (i.e. to remove the overall translation of dots added to give a positional     
         * disparity. Otherwise the calculation checks whether the dots are present only by           
         * considering the shift due to the local disparity of the corrugation. Pixdisp[1] is added   
         * to 1 eye and subtracted from the other because the overall positional disparity shift is in
         * opposite directions in the 2 eyes. *******************/
        
        if(st->prev != NULL)
        {
            if(sst->mode == LEFTMODE)
                dotdepth = fore_amplitude * sinf(((*y - pixdisp[1]) * depth_freq * 2 * M_PI)+phase);
            else
                dotdepth = fore_amplitude * sinf(((*y + pixdisp[1]) * depth_freq * 2 * M_PI)+phase);
            sindisp[0] = (dotdepth * -cosa) + pixdisp[0];
            sindisp[1] = (dotdepth * sina) + pixdisp[1];
        }
        else
        {
            dotdepth = amplitude * sinf((*y * depth_freq * 2 * M_PI)+phase);
            sindisp[0] = dotdepth * -cosa;
            sindisp[1] = dotdepth  * sina;
            if(sst->mode == LEFTMODE)
            {
                *x += sindisp[0];
                *y += sindisp[1];
            }
            else
            {
                *x -= sindisp[0];
                *y -= sindisp[1];
            }
            
        }
        
        /* 
         * if csq != 0, that means that there is an CORRUG pattern
         * to be painted inside this one, so be sure to leave
         * an approriate hole in the middle
         */
        *p = 0;
        /* HOLLY need to set csq for background if there is any depth modulation in foreground */
        if(csq != 0) 
        {
            if(optionflag & SQUARE_RDS)
            {
                xsq = fabsf((float)(*x - sindisp[0]+offset[0]));
                ysq = fabsf((float)(*y - sindisp[1]+offset[1]));
                if(xsq > csq || ysq > dsq)
                    *p |= LEFTMODE;
                xsq = fabsf((float)(*x + sindisp[0]+offset[0]));
                ysq = fabsf((float)(*y + sindisp[1]+offset[1]));
                if(xsq > csq || ysq > dsq)
                    *p |= RIGHTMODE;
            }
            else
            {
                xsq = (*x - sindisp[0]) * (*x - sindisp[0]);
                ysq = (*y - sindisp[1]) * (*y - sindisp[1]);
                if((xsq/csq + ysq/dsq) > 1)
                    *p |= LEFTMODE;
                xsq = (*x + sindisp[0]) * (*x + sindisp[0]);
                ysq = (*y + sindisp[1]) * (*y + sindisp[1]);
                if((xsq/csq + ysq/dsq) > 1)
                    *p |= RIGHTMODE;
            }
        }
        else /*always paint the dot */
        {
            /* HOLLY. Need to calculate pixdisp[0] and [1] depending on local depth of
             * corrugation. Then add these to *x, *y;
             */
            
	        *p = (RIGHTMODE | LEFTMODE);
        }
        /*
         * only increment the counter if the dot is inside the
         * stimulus's ellipse
         */
        i++,x++,y++,p++;
    }
}



/*
 * calc_plane is called when sf = 0 to give a plane rather than a corrugation. It calculates a new set of random x-y 
 * locations for the
 * dots, and randomizes them to BLACK/WHITE if required
 */
#define PIXM 10
void calc_plane(Stimulus *st, Substim *sst)
{
    int i,j,partdisp,ndots;
    float cval,f,sy,cm,deg,iscale[2],val[2];
    float asq,bsq,csq,dsq,xsq,ysq,pixdisp[2],offset[2],sindisp[2];
    float cosa, sina;
    int *p,*q,*cend;
    vcoord *x,*y,w,h; 
    int xshift[2],iw,ih;
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    float phase,contrast = pos->contrast, testval;
    
    float angle;
    float depth_freq, fore_amplitude; /* depth_freq in c/deg from StimulusProperty - need to convert*/
    float amplitude, dotdepth;
    
    if(st->flag & ANTICORRELATE && sst->mode == RIGHTMODE)
        contrast = -pos->contrast;
    if(st->flag & CONTRAST_NEGATIVE)
        val[0] = (double)st->background * (1 - contrast);
    else
        val[0] = (double)st->background * (1 + contrast);
    phase = sst->pos.phase;
    if(st->flag & CONTRAST_POSITIVE)
    {
        if(contrast >= 1.0)
            val[1]  = 1.0;
        else
            val[1] = (float)st->background * (1 + contrast);
    }
    else
        val[1] = val[0];
    for(i = 0; i < 2; i++)
        gb->lum[i] = dogamma(val[i]);
    
    offset[0] = offset[1] = 0;
    if(optionflag & SQUARE_RDS)
        ndots = gb->density * 4 * pos->radius[0] * pos->radius[1]/(100 * gb->dotsiz[0] * gb->dotsiz[1]);
    else
        ndots = gb->density * M_PI * pos->radius[0] * pos->radius[1]/(100 * gb->dotsiz[0] * gb->dotsiz[1]);
    
    if(ndots > gb->ndots)
        gb->ndots = init_corrug(st,sst,gb->density);
    else
        gb->ndots = ndots;
    /* 
     *if st->prev != NULL, this is a stimuls that forms a background 
     * that means a ``hole'', with an appropriate disparity, needs to
     * be made in the background
     */
    cosa = cos(pos->angle);
    sina = sin(pos->angle);
    
    if(st->prev != NULL)
    {
        if(optionflag & BACKGROUND_FIXED_BIT)
        {
            offset[0] = st->prev->pos.xy[0] + (st->pos.xy[0] * cosa)
            + (st->pos.xy[1] * sina);
            offset[1] = st->prev->pos.xy[0] + (st->pos.xy[0] * sina)
            - (st->pos.xy[1] * cosa);
        }
        pixdisp[0] = (st->prev->disp - st->disp) * cosa +
        (st->prev->vdisp - st->vdisp) * sina;
        pixdisp[1] = (st->disp - st->prev->disp) * sina +
        (st->prev->vdisp - st->vdisp) * cosa;
    }
    else
    {  
        pixdisp[0] =  pixdisp[1] = 0;
    }
    
    /*
     phase = 0;
     */
    
    if(gb->xshift != 0)
    {
        xshift[0] = (int)(rint(gb->xshift * cosa));
        xshift[1] = (int)(rint(gb->xshift * sina));
        if(sst->seed & 1)
        {
            xshift[0] = -xshift[0];
            xshift[1] = -xshift[1];
        }
        xshift[1] += phase;
    }
    else
    {
        xshift[1] = phase;
        xshift[0] = 0;
    }
    csq = dsq= 0;
    w = (vcoord)(pos->imsize[0] - gb->dotsiz[0]);
    h = (vcoord)(pos->imsize[1] - gb->dotsiz[1]);
    w = (vcoord)(2 * pos->radius[0] - gb->dotsiz[0]);
    h = (vcoord)(2 * pos->radius[1] - gb->dotsiz[1]);
    srandom(sst->seed);
    if(!(optionflag & SQUARE_RDS))
    {
        asq = pos->radius[0] * pos->radius[0];
        bsq = pos->radius[1] * pos->radius[1];
        /* calculate parameters for hole */
        if(st->prev != NULL && st->prev->type == STIM_CORRUG)
        {
            csq = st->prev->pos.radius[0] * st->prev->pos.radius[0];
            dsq = st->prev->pos.radius[1] * st->prev->pos.radius[1];
        }
    }
    else  if(st->prev != NULL && st->prev->type == STIM_CORRUG) /* square background */
    {
        csq = fabsf(st->prev->pos.radius[0]);
        dsq = fabsf(st->prev->pos.radius[1]);
    }
    p = gb->im;
    x = gb->xpos;
    y = gb->ypos;
    iw = w;
    ih = h;
    /* now set random x,y locations */
    amplitude =  gb->disp_grad;
    if(st->prev != NULL){
        fore_amplitude =  st->prev->left->ptr->disp_grad;
    }
    for(i = 0; i < gb->ndots; )
    {
        do{
            *p = random();
            *x = (float)((*p + xshift[0]) % (iw*PIXM))/PIXM - w/2;
            *y = (float)(((*p>16) + xshift[1]) % (ih*PIXM))/PIXM - h/2;
            if(optionflag & SQUARE_RDS)
                testval = 0;
            else
            {
                xsq = (*x) * (*x);
                ysq = (*y) * (*y);
                testval = (xsq/asq + ysq/bsq); /* outside circle, try again */
            }
        }while(testval > 1.0);
        
        /**** To calculate position of hole in background it is necessary to subtract pixdisp[1] from 
         * the y value (i.e. to remove the overall translation of dots added to give a positional     
         * disparity. Otherwise the calculation checks whether the dots are present only by           
         * considering the shift due to the local disparity of the corrugation. Pixdisp[1] is added   
         * to 1 eye and subtracted from the other because the overall positional disparity shift is in
         * opposite directions in the 2 eyes. *******************/
        
        if(st->prev != NULL)
        {
            if(sst->mode == LEFTMODE)
                dotdepth = *y * ((2 * fore_amplitude)/iw);
            else
                dotdepth = *y * ((2 * fore_amplitude)/iw);
            
            /*  dotdepth = fore_amplitude * sinf(((*y - pixdisp[1]) * 0.002 * 2 * M_PI));
             else
             dotdepth = fore_amplitude * sinf(((*y + pixdisp[1]) * 0.002 * 2 * M_PI));
             */
            sindisp[0] = (dotdepth * -cosa) + pixdisp[0];
            sindisp[1] = (dotdepth * sina) + pixdisp[1];
        }
        
        else
        {
            dotdepth = *y * ((2 * amplitude)/iw);
            /* dotdepth = amplitude * sinf((*y * 0.002 * 2 * M_PI));*/
            sindisp[0] = dotdepth * -cosa;
            sindisp[1] = dotdepth  * sina;
            if(sst->mode == LEFTMODE)
            {
                *x += sindisp[0];
                *y += sindisp[1];
            }
            else
            {
                *x -= sindisp[0];
                *y -= sindisp[1];
            }
            
        }
        
        
        /* 
         * if csq != 0, that means that there is an CORRUG pattern
         * to be painted inside this one, so be sure to leave
         * an approriate hole in the middle
         */
        *p = 0;
        /* HOLLY need to set csq for background if there is any depth modulation in foreground */
        if(csq != 0) 
        {
            if(optionflag & SQUARE_RDS)
            {
                xsq = fabsf((float)(*x - sindisp[0]+offset[0]));
                ysq = fabsf((float)(*y - sindisp[1]+offset[1]));
                if(xsq > csq || ysq > dsq)
                    *p |= LEFTMODE;
                xsq = fabsf((float)(*x + sindisp[0]+offset[0]));
                ysq = fabsf((float)(*y + sindisp[1]+offset[1]));
                if(xsq > csq || ysq > dsq)
                    *p |= RIGHTMODE;
            }
            else
            {
                xsq = (*x - sindisp[0]) * (*x - sindisp[0]);
                ysq = (*y - sindisp[1]) * (*y - sindisp[1]);
                if((xsq/csq + ysq/dsq) > 1)
                    *p |= LEFTMODE;
                xsq = (*x + sindisp[0]) * (*x + sindisp[0]);
                ysq = (*y + sindisp[1]) * (*y + sindisp[1]);
                if((xsq/csq + ysq/dsq) > 1)
                    *p |= RIGHTMODE;
            }
        }
        else /*always paint the dot */
        {
            /* HOLLY. Need to calculate pixdisp[0] and [1] depending on local depth of
             * corrugation. Then add these to *x, *y;
             */
            
            *p = (RIGHTMODE | LEFTMODE);
        }
        /*
         * only increment the counter if the dot is inside the
         * stimulus's ellipse
         */
        i++,x++,y++,p++;
    }
}

/*
 * this simply draws all the dots on the list, with an appropriate
 * disparity
 */
void paint_corrug(Stimulus *st, int mode)
{
    int i, j;
    int *p,d,*end, *q;
    vcoord  w,h,*x,*y,fw,fh;
    short pt[2];
    float vcolor[4], bcolor[4];
    vcoord xmv;
    OneStim *gb = st->left->ptr;
    Locator *pos = &st->pos;
    float angle;
    float amplitude; /* depth_freq not needed (sf = 0)*/
    vcoord rect[8],crect[8];
    float sina,cosa;
    angle = rad_deg(pos->angle);
    /*
     * first glTranslatef the co-ordinate system to put the stimulus
     * in the right place/orientation. Use different colors for
     * L/R dots
     */
    glPushMatrix();
    vcolor[0] = vcolor[1] = vcolor[2] = 0;
    bcolor[0] = bcolor[1] = bcolor[2] = 0;
    
    if(mode == LEFTMODE)
    {
        gb = st->left->ptr;
        xmv = pos->xy[0]+st->disp;
        glTranslatef(xmv,pos->xy[1]+st->vdisp,0);
        vcolor[0] = gb->lum[0];
        bcolor[0] = gb->lum[1];
    }
    else if(mode == RIGHTMODE)
    {
        gb = st->right->ptr;
        xmv = pos->xy[0]-st->disp;
        glTranslatef(xmv,pos->xy[1]-st->vdisp,0);
        vcolor[1] = vcolor[2] = gb->lum[0];
        bcolor[1] = bcolor[2] = gb->lum[1];
    }
    if(optionflags[STIMULUS_IN_OVERLAY])
    {
        vcolor[1] = vcolor[2] = vcolor[0] = gb->lum[0];
        bcolor[1] = bcolor[2] = bcolor[0] = gb->lum[1];
    }
    bcolor[3] = vcolor[3] = 1.0;
    glRotatef(angle,0.0,0.0,1.0); 
    mycolor(vcolor);
    w = gb->dotsiz[0]/2;
    h = gb->dotsiz[1]/2;
    fw = gb->dotsiz[0];
    fh = gb->dotsiz[1];
    
    cosa = cos(pos->angle);
    sina = sin(pos->angle);
    rect[0] = -w * cosa - h * sina;
    rect[1] = -h * cosa + w * sina;
    rect[2] = -w * cosa + h * sina;
    rect[3] = h * cosa + w * sina;
    rect[4] = w * cosa + h * sina;
    rect[5] = h * cosa - w * sina;
    rect[6] = w * cosa - h * sina;
    rect[7] = -h * cosa - w * sina;
    h = h+0.5;
    crect[0] = -h * sina;
    crect[1] = -h * cosa;
    crect[2] = h * sina;
    crect[3] = h * cosa;
    
    
    p = gb->im;
    end = (gb->im+gb->ndots);
    x = gb->xpos;
    y = gb->ypos;
    i = 0;
    
    /* now paint the dots */
    if(gb->mode == STIM_CIRCLE)
    {
        for(;p < end; p++,x++,y++)
            mycirc(*x,*y,w);
    }
    else
    {
        if(optionflag & ANTIALIAS_BIT)
        {
            glLineWidth(1.0);
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_LINE_SMOOTH);
            glBegin(GL_LINES);
            i = 0;
            for(;p < end; p++,x++,y++)
            {
                if((i = !i))
                    mycolor(vcolor);      
                else
                    mycolor(bcolor);
                if(*p & mode)
                    aarotrect(rect, *x,*y);
            }
            glEnd();
            p = gb->im;
            x = gb->xpos;
            y = gb->ypos;
        }
        glDisable(GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(w*2);
        glBegin(GL_LINES);
        i = 0;
        for(;p < end; p++,x++,y++)
        {
            if((i = !i))
                mycolor(vcolor);      
            else
                mycolor(bcolor);
            if(*p & mode)
                rotrect(crect,*x,*y);
        }
        glEnd();
    }
    glPopMatrix();
}

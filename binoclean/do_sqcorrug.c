#include <stdio.h>
#include "mymath.h"
#include <math.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#include "commdefs.h"
#include "stimuli.h"
#include "sprotos.h"

extern int option2flag;
extern int optionflags[];
extern double gammaval;
extern int optionflag;
int init_rds(Stimulus *st,  Substim *sst, float density);
int square_wave(float sin_value);

extern Expt expt;

/*
 * NewRds Allocate memory for a new RDS stimulus structure
 * and initializes it.
 */

OneStim *NewSqcorrug(Stimulus *st, Substim *sst, Substim  *copy)
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
    init_sqcorrug(st,sst,gb->density);
    st->type = STIM_SQCORRUG;
    sst->type = STIM_SQCORRUG;
    return(gb);
}

/*
 * free sqcorrug frees up the memory used by and SQCORRUG stimulus structure
 */
void free_sqcorrug(Substim *st)
{
    OneStim *ptr;
    
    if((ptr = st->ptr) != NULL)
    {
        /*
         free(ptr->ypos);
         ptr->ypos = NULL;
         free(ptr);
         st->ptr = 0;
         */
    }
}

/*
 * init_sqcorrug initializes an SQCORRUG stimulus structure. This
 * involves allocating memory for a list of dot locations
 * long enough to produce the desired to-density, given
 * the stimulus size
 */
#define CIRC2SQU  (4/3.14)

int init_sqcorrug(Stimulus *st,  Substim *sst, float density)
{
    
	int i,j,*p, size = 0;
	double val,xval,yval,x,y,cval,cm,deg,sx,sy;
	Locator *pos = &st->pos;
	OneStim *gb = (OneStim *)sst->ptr;
	int ndots, npts;
    
	if(density > 0)
        gb->density = density;
	else if(gb->density <= 0.0)
        gb->density = 20.0;
    /*
     * calculate actual number of dots from density
     * N.B. 25 * dotsiz * dotsiz = 100 * dotsiz/2 * dotsiz/2,
     * = area of dot
     */
	if(gb->dotsiz[0] < 1)
        gb->dotsiz[0] = 1;
	if(gb->dotsiz[1] < 1)
        gb->dotsiz[1] = 1;
    if(optionflag & SQUARE_RDS)
        ndots = gb->density * 4 * pos->radius[0] * pos->radius[1]/(100 * gb->dotsiz[0] * gb->dotsiz[1]);
    else
        ndots = gb->density * M_PI * pos->radius[0] * pos->radius[1]/(100 * gb->dotsiz[0] * gb->dotsiz[1]);
    npts = ndots * CIRC2SQU;
    if(npts > sst->imlen || gb->im == NULL) /* need new memory */
	{
        if(gb->im != NULL)
            free(gb->im);
        size = npts * sizeof(int);
        sst->imlen = npts;
        if((gb->im = (int *)malloc(size)) == NULL){
            fprintf(stderr, "Memory Error\n");
            return(0);
        }
	}
	if(ndots > sst->xpl || gb->xpos == NULL) /* need new memory */
	{
        if(gb->xpos != NULL)
            free(gb->xpos);
        npts = ndots * CIRC2SQU;
        sst->xpl = npts+2;
        gb->xpos = (vcoord *)malloc((sst->xpl * sizeof(vcoord)));
	}
	if(npts > sst->ypl || sst->ypos == NULL) /* need new memory */
	{
        if(sst->ypos != NULL)
            free(sst->ypos);
        sst->ypl = npts+2;
        sst->ypos = (vcoord *)malloc(sst->ypl * sizeof(vcoord));
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

/*
 * cal_sqcorrug calculates a new set of random x-y locations for the
 * dots, and randomizes them to BLACK/WHITE if required
 */
#define PIXM 10

#define nofabs(x)  fabs(x)

void calc_sqcorrug(Stimulus *st, Substim *sst)
{
    int i,j,partdisp,ndots;
    float cval,f,sy,cm,deg,iscale[2],val[2];
    float asq=0,bsq=0,csq,dsq,xsq,ysq,pixdisp[2],offset[2],sindisp[2];
    float cosa, sina;
    int *p,*q,*cend;
    vcoord *x,*y,w,h, y_tra_with_angle;
    int xshift[2],iw,ih;
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    float phase,contrast = pos->contrast, testval;
    
    float angle;
    float depth_freq, fore_amplitude, back_amplitude; /* depth_freq in c/deg from StimulusProperty - need to convert*/
    double fore_vamplitude;
    float amplitude, dotdepth,vamplitude;
    float left_sin_depth, right_sin_depth, foreground_depth;
    float left_sin_vdepth, right_sin_vdepth, foreground_vdepth;
    float y_with_phase, foreground_period, distance_from_depth_jump;
    int kill_overlap_0, kill_overlap_1;
    int kill_overlap_v0, kill_overlap_v1,kill_overlap;
    int fill_hole_0, fill_hole_1;
    int fill_hole_v0, fill_hole_v1,fill_hole;
    float iphase;
    double dotvd, chksign;
    float ucw = 0;
    
    if(expt.vals[ALTERNATE_STIM_MODE] == 10){
        if(st->prev != NULL)
            ucw = st->prev->ucgap;
        else
            ucw = st->ucgap;
    }
    iphase = deg_rad(st->vals[START_PHASE]);
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
        gb->ndots = init_sqcorrug(st,sst,gb->density);
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
        if(st->prev != NULL && st->prev->type == STIM_SQCORRUG)
        {
            csq = st->prev->pos.radius[0] * st->prev->pos.radius[0];
            dsq = st->prev->pos.radius[1] * st->prev->pos.radius[1];
        }
    }
    else  if(st->prev != NULL && st->prev->type == STIM_SQCORRUG) /* square background */
    {
        csq = fabsf(st->prev->pos.radius[0] -gb->dotsiz[0]/2);
        dsq = fabsf(st->prev->pos.radius[1] -gb->dotsiz[1]/2);
    }
    p = gb->im;
    x = gb->xpos;
    y = sst->ypos;
    iw = w;
    ih = h;
    
    
    /* now set random x,y locations */
    
    depth_freq =  StimulusProperty((Stimulus *)st, SF);
    depth_freq = 1 / (   deg2pix(1/depth_freq)  );
    
    /*
     * chksign determines whether the gap that needs filling is in the left eye or
     * right eye. N.B. This is only implemented for horizontal disparity.
     *  Need an equivalent test for cosa and vdepth_mod before negative values of
     * vdepth_mod are allowed
     */
    
    if(gb->depth_mod < 0)
        chksign = sina * -1;
    else
        chksign = sina;
    
    
    if(optionflags[COUNTERPHASE]){
        phase = iphase;
        amplitude =  gb->depth_mod * cos(sst->pos.phase);
    }
    else{
        phase = sst->pos.phase;
        amplitude =  gb->depth_mod;
        vamplitude =  deg2pix(st->vdisp_mod)/2;
    }
    if(st->prev != NULL){
        depth_freq =  StimulusProperty((Stimulus *)st->prev, SF);
        depth_freq = 1 / (   deg2pix(1/depth_freq)  );
        if(optionflags[COUNTERPHASE]){
            fore_amplitude =  ((OneStim *)(st->prev->left->ptr))->depth_mod * cos(sst->pos.phase);
        }
        else{
            fore_amplitude =  ((OneStim *)(st->prev->left->ptr))->depth_mod;
            fore_vamplitude =  deg2pix(st->prev->vdisp_mod)/2;
        }
        if(fore_amplitude < 0)
            chksign = sina * -1;
        else
            chksign = sina;
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
         * considering the shift due to the local disparity of the sqcorrugation. Pixdisp[1] is added   
         * to 1 eye and subtracted from the other because the overall positional disparity shift is in
         * opposite directions in the 2 eyes. *******************/
        
        /* information for hole filling / overlap deletion */
        foreground_period = (1 / depth_freq);
        if(sst->mode == LEFTMODE)
            y_with_phase = *y + phase * ( foreground_period / ( 2 * M_PI)) - pixdisp[1];
        else
            y_with_phase = *y + phase * ( foreground_period / ( 2 * M_PI)) + pixdisp[1];
        distance_from_depth_jump = y_with_phase - foreground_period * floor(y_with_phase / foreground_period);
        
        if(st->prev != NULL)
        {
            if(sst->mode == LEFTMODE) {
                left_sin_depth = sinf(((*y - pixdisp[1]) * depth_freq * 2 * M_PI)+phase);
                left_sin_vdepth = sinf(((*y - pixdisp[1]) * depth_freq * 2 * M_PI)+phase);
                dotdepth = fore_amplitude * square_wave(left_sin_depth);
                dotvd = fore_vamplitude * square_wave(left_sin_vdepth);
                sindisp[0] = (dotdepth * -cosa) - dotvd * sina + pixdisp[0];
                sindisp[1] = (dotdepth * sina) -dotvd * cosa + pixdisp[1];
            } else {
                /*
                 * +/- pixdisp here seems to make NO difference, which I do not understand.
                 * ?Looks like dotdepth is no longer used in subsequent calculations!
                 */
                right_sin_depth = sinf(((*y + pixdisp[1]) * depth_freq * 2 * M_PI)+phase);
                right_sin_vdepth = sinf(((*y + pixdisp[1]) * depth_freq * 2 * M_PI)+phase);
                dotdepth = fore_amplitude * square_wave(right_sin_depth);
                dotvd = fore_vamplitude * square_wave(right_sin_vdepth);
                sindisp[0] = (dotdepth * -cosa) - dotvd * sina + pixdisp[0];
                sindisp[1] = (dotdepth * sina) -dotvd * cosa + pixdisp[1];
            }
        }
        else
        {
            foreground_depth = sinf((*y * depth_freq * 2 * M_PI)+phase);
            dotdepth = amplitude * square_wave(foreground_depth);
            /*  dotdepth = amplitude * sinf((*y * depth_freq * 2 * M_PI)+phase);*/
            dotvd = vamplitude * square_wave(foreground_depth);
            sindisp[0] = dotdepth * -cosa - dotvd * sina;
            sindisp[1] = dotdepth  * sina - dotvd * cosa;
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
         * if csq != 0, that means that there is an SQCORRUG pattern
         * to be painted inside this one, so be sure to leave
         * an approriate hole in the middle
         * sina, cosa are simple sin(or), cos(or) so if ori = 0, sina = 0;
         * fill_hole_0 = 1 checks for filling when sina > 0 Reye, or sina < 0 Leye
         * fill_hole_1 = 1 means sina < 0 Reye, or sina > 0 Leye
         * fill_hole_1 = 1 means 
         */
        *p = 0;
        /* HOLLY need to set csq for background if there is any depth modulation in foreground */
        if(csq != 0) 
        {
            /* tests for holes in sqcorrug */
            /* test 0 dots near enough to step edge at 180 phase*/
            if ((distance_from_depth_jump >= (foreground_period /2 - nofabs(dotdepth * sina)) ) && (distance_from_depth_jump <= (foreground_period /2 + nofabs(dotdepth * sina)) ) ) {
                fill_hole_0 = 1;
            } else {
                fill_hole_0 = 0;
            }
            /* test 1 dots near step edge at 0 phase*/
            if (  (distance_from_depth_jump <= nofabs(dotdepth * sina)) || (distance_from_depth_jump >= (foreground_period - nofabs(dotdepth * sina))) ) {
                fill_hole_1 = 1;
            } else {
                fill_hole_1 = 0;
            }
            
            if ((distance_from_depth_jump >= (foreground_period /2 - nofabs(dotvd * cosa)) ) && (distance_from_depth_jump <= (foreground_period /2 + nofabs(dotvd * cosa)) ) ) {
                fill_hole_v0 = 1;
            } else {
                fill_hole_v0 = 0;
            }
            /* test 1 */ 
            if (  (distance_from_depth_jump <= nofabs(dotvd * cosa) +ucw) || (distance_from_depth_jump >= (foreground_period - nofabs(dotvd * cosa)-ucw)) ) {
                fill_hole_v1 = 1;
            } else {
                fill_hole_v1 = 0;
            }
            
            if(distance_from_depth_jump < ucw || distance_from_depth_jump >= (foreground_period -ucw) )
                fill_hole = 1;
            else
                fill_hole = 0;
            /*fill in sqcorrug holes*/
            
            /*
             *  for dots near the central step edge, fill_hole_v1 is set. This means paint a dot in the left
             * eye if ori > 90 (cosa < 0)
             */
            
            if(sst->mode == RIGHTMODE) {
                if (  ((chksign >= 0) && fill_hole_0) || ((chksign < 0) && fill_hole_1) || (cosa <= 0 && fill_hole_v0) || (cosa > 0 && fill_hole_v1)  || fill_hole) {
                    *p |= RIGHTMODE;
                }
            }
            else if(sst->mode == LEFTMODE) {
                if (  ((chksign >= 0) && fill_hole_1) || ((chksign < 0) && fill_hole_0) || (cosa <= 0 && fill_hole_v1) || (cosa > 0 && fill_hole_v0)   || fill_hole) {
                    *p |= LEFTMODE;
                }
            }
            
            
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
        else /*always paint the dot, ... unless you are in an overlapping region */
        {
            /* Is dot in overlap?*/
            /* distance_from_depth_jump calculated above */
            
            /* test 0 */
            if(distance_from_depth_jump <= fabs(dotdepth * sina) || 
               (distance_from_depth_jump >= (foreground_period - fabs(dotdepth * sina))) ){
                kill_overlap_0 = 1;
            } else {
                kill_overlap_0 = 0;
            }
            if((distance_from_depth_jump <= foreground_period /2 + fabs(dotvd * cosa)) && 
               distance_from_depth_jump >= (foreground_period/2 - fabs(dotvd * cosa))){ 
                kill_overlap_v0 = 1;
            } else {
                kill_overlap_v0 = 0;
            }
            
            
            /* test 1 */
            
            if((distance_from_depth_jump <= foreground_period /2 + fabs(dotdepth * sina)) && 
			   (distance_from_depth_jump >= (foreground_period/2 - fabs(dotdepth * sina)))){
                kill_overlap_1 = 1;
            } else {
                kill_overlap_1 = 0;
            }
            if(distance_from_depth_jump < ucw  || distance_from_depth_jump >= foreground_period - ucw){
                kill_overlap = 1;
            }
            else
                kill_overlap = 0;
            
            
            /*
             * this removes dots in the region where disparity has slid two regions on
             * top of each other
             */
            if(distance_from_depth_jump <= fabs(dotvd * cosa)+ucw ||
               (distance_from_depth_jump >= (foreground_period - fabs(dotvd * cosa)-ucw))){
                kill_overlap_v1 = 1;
            } else {
                kill_overlap_v1 = 0;
            }
            
            if(sst->mode == RIGHTMODE) {
                if (((chksign >= 0) && kill_overlap_0) || ((chksign < 0) && kill_overlap_1) || (cosa >= 0 && kill_overlap_v0) || (cosa < 0 && kill_overlap_v1) || kill_overlap){
                    *p &= (~RIGHTMODE); 
                } else {
                    *p |= RIGHTMODE;
                }
            }
            else if(sst->mode == LEFTMODE) {
                if (((chksign >= 0) && kill_overlap_1) || ((chksign < 0) && kill_overlap_0) || (cosa >= 0 && kill_overlap_v1) || (cosa < 0 && kill_overlap_v0) || kill_overlap) {
                    *p &= (~LEFTMODE);
                } else {
                    *p |= LEFTMODE;
                }
                
            }
            
            
            
            /* *p = (RIGHTMODE | LEFTMODE); */
        }
        /*
         * only increment the counter if the dot is inside the
         * stimulus's ellipse
         */
        i++,x++,y++,p++;
    }
}

int square_wave(float sin_value)
{
    if (sin_value >= 0)
        return(1);
    else if (sin_value < 0)
        return(-1);
    else /*!!*/
        return(0);
}


/*
 * this simply draws all the dots on the list, with an appropriate
 * disparity
 */
void paint_sqcorrug(Stimulus *st, int mode)
{
    int i, j;
    int *p,d,*end, *q;
    vcoord  w,h,*x,*y,fw,fh, *right_x_tra, *left_x_tra, *y_tra, y_tra_with_angle;
    short pt[2];
    float vcolor[4], bcolor[4];
    vcoord xmv;
    OneStim *gb = (OneStim *)(st->left->ptr);
    Locator *pos = &st->pos;
    float angle;
    float depth_freq, amplitude; /* depth_freq in c/deg from StimulusProperty - need to convert*/
    vcoord rect[8],crect[8];
    float cosa,sina;
    Substim *sst = st->left;
    
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
        gb = (OneStim *)(st->left->ptr);
        if(optionflags[SPLITSCREEN])
            glTranslatef(-320,0,0);
        xmv = pos->xy[0]+st->disp;
        glTranslatef(xmv,pos->xy[1]+st->vdisp,0);
        vcolor[0] = gb->lum[0];
        bcolor[0] = gb->lum[1];
    }
    else if(mode == RIGHTMODE)
    {
        gb = (OneStim  *)(st->right->ptr);
        sst = st->right;
        if(optionflags[SPLITSCREEN])
            glTranslatef(320,0,0);
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
    y = sst->ypos;
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
            y = sst->ypos;
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




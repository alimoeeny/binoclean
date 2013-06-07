#include <stdio.h>
#include "mymath.h"
#include <math.h>
#include <stdlib.h>
#include <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#include "commdefs.h"
#include "stimuli.h"
#include "sprotos.h"

extern int option2flag;
extern double gammaval;
extern int optionflag,optionflags[],testflags[];
static FILE *rlsfd = NULL;
extern FILE *seroutfile;
extern Expt expt;

//Ali
char * VERSION_NUMBER;


int init_rls(Stimulus *st,  Substim *sst, float density);

#define NSEEDS 10
long *rndarray;
int rndarraylen = 0;
long seeds[NSEEDS] = {
    //  2, 4, 6, 8, 10, 12, 14, 16, 18, 20
    4, 8, 12, 16, 20, 24, 28, 32, 36, 10040
};

/*
 * NewRls Allocates memory for a new RLS stimulus structure
 * and initializes it.
 */

Substim *NewRls(Stimulus *st, Substim *sst, Substim  *copy)
{
    Locator *pos = &st->pos;/*j*/
    
    FreeStimulus(sst);
    if (pos->dotsize > 0.5) /*j fudge so that cyl and rls share common dotsize*/
        sst->dotsiz[0] = sst->dotsiz[1] = pos->dotsize;
    else
        sst->dotsiz[0] = sst->dotsiz[1] = 5;
    sst->xpos = sst->ypos = NULL;
    if(pos->density != 0)
        sst->density = pos->density;
    else
        sst->density = 20;
    if(sst->baseseed <= 0 || sst->baseseed > 1000000)
        sst->seed = sst->baseseed = 1001;
    st->correlation = 1.0;
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *) (malloc(sizeof(OneStim)));
    init_rls(st,sst,sst->density);
    st->flag |= (CONTRAST_POSITIVE | CONTRAST_NEGATIVE);
    st->type = STIM_RLS;
    sst->type = STIM_RLS;
    return(sst);
}

/*
 * free rls frees up the memory used by and RLS stimulus structure
 */
void free_rls(Substim *st)
{
    if(st->ypos != NULL){
        free(st->ypos);
        st->ypos = NULL;
    }
}

/*
 * init_rls initializes an RLS stimulus structure. This
 * involves allocating memory for a list of dot locations
 * long enough to produce the desired to-density, given
 * the stimulus size
 */
int init_rls(Stimulus *st,  Substim *sst, float density)
{
    
	int i,j,*p;
	double val,xval,yval,x,y,cval,cm,deg,sx,sy;
	Locator *pos = &st->pos;
	int ndots,nrect;
    
	if(density > 0)
        sst->density = sst->density = density;
	else if(sst->density <= 0.0)
        sst->density = sst->density = 20.0;
    /*
     * calculate actual number of dots from density
     * N.B. 25 * dotsiz * dotsiz = 100 * dotsiz/2 * dotsiz/2,
     * = area of dot
     */
	ndots = 1+(2 * pos->radius[1]/(sst->dotsiz[1]));
	if(pos->ss[0] > 1)
        nrect = 1+ (pos->radius[0] * 2)/pos->ss[0];
	else
        nrect = 20;
	
    
	if(ndots * 4 * nrect > sst->iimlen || sst->iim == NULL) /* need new memory */
	{
#ifdef DEBUG
        printf("%d dots %d %d\n",ndots,sst->iim,sst->xpos);
#endif
        sst->iimlen = ndots *4 * nrect;
        if(sst->iim != NULL)
            free(sst->iim);
        if(sst->iimb != NULL)
            free(sst->iimb);
        sst->iim = (int *)malloc(sst->iimlen * sizeof(int));
        sst->iimb = (int *)malloc(sst->iimlen * sizeof(int));
	}
	if(ndots *4*nrect > sst->imblen || sst->imb == NULL) /* need new memory */
    {
        if(sst->imb != NULL)
            free(sst->imb);
	    sst->imblen = ndots *4*nrect;
	    sst->imb = (float *)malloc(sst->imblen * sizeof(float));
    }
	if(ndots *4*nrect> sst->xpl || sst->xpos == NULL) /* need new memory */
	{
        sst->xpl = ndots *4*nrect;
        if(sst->xpos != NULL)
            free(sst->xpos);
        sst->xpos = (vcoord *)malloc(sst->xpl * sizeof(vcoord));
	}
	if(ndots > sst->ndots || sst->ypos == NULL) /* need new memory */
	{
        if(sst->ypos != NULL)
            free(sst->ypos);
        sst->ypos = (vcoord *)malloc(sst->xpl * sizeof(vcoord));
	}
	if(ndots *4*nrect> sst->xpla || sst->xpos == NULL) /* need new memory */
	{
        sst->xpla = ndots *4*nrect;
        if(sst->xposa != NULL)
            free(sst->xposa);
        sst->xposa = (vcoord *)malloc(sst->xpla * sizeof(vcoord));
	}
	if(ndots *4*nrect> sst->ypla || sst->ypos == NULL) /* need new memory */
	{
        sst->ypla = ndots *4*nrect;
        if(sst->yposa != NULL)
            free(sst->yposa);
        sst->yposa = (vcoord *)malloc(sst->ypla * sizeof(vcoord));
	}
	if(ndots > 0)
        sst->ndots = ndots;
	if(st->correlation < 1 && st->correlation > -1)
        sst->corrdots = sst->ndots * fabsf(st->correlation);
	else
        sst->corrdots = sst->ndots * 2;
	p = sst->iim;
	sst->seed = sst->baseseed;
	myrnd_init(sst->baseseed);
	for(i = 0; i < sst->ndots; i++)
        *p++ = myrnd_i();
	for(i = 0; i < 2; i++)
	{
        if(pos->imsize[i] == 0)
            pos->imsize[i] = 256;
        if(st->pos.radius[i] ==0)
            st->pos.radius[i] = st->pos.imsize[i]/2;
	}
	sst->xshift = 0;
    memcpy(&sst->pos,&st->pos,sizeof(Locator));
	glShadeModel(GL_FLAT);
    /*
     if(option2flag & RANDOM){
     if(rlsfd)
     fclose(rlsfd);
     rlsfd = fopen("./rlslog","a");
     }
     */
	return(0);
}

/*
 * cal_rls calculates a new set of random x-y locations for the
 * dots, and randomizes them to BLACK/WHITE if required
 */
#define PIXM 10
void calc_rls(Stimulus *st, Substim *sst)
{
    int i,j,partdisp,ndots,nx = 0;
    float cval,f,sy,cm,deg,iscale[2],val[2];
    float asq,bsq = 0,csq,dsq,xsq,ysq,pixdisp[2],offset[2];
    int *p,*q,*cend,yi,lastp,lastq;
    vcoord *x,*y,w,h,lastx,eh=0,lasty,*zy,*zx,lastzx,lastzy;
    int xshift[3],iw,ih;
    Locator *pos = &sst->pos;
    float phase,contrast = pos->contrast;
    int pixmul = 1,seedcall = 0;
    vcoord yp,diff;
    double drnd,aval;
    int bit, nbit;
    long *rp,rnd,*rq;
    int orthoguc = 0,orthogac = 0;
    int pblank = 0,*pi;

    if (sst->density < 100)
        pblank = rint((100-sst->density) * 2.55);
    
    if (expt.stimmode == ORTHOG_UC || (st->left->ptr->plaid_angle) > M_2_PI)
        orthoguc = 1;
    if (expt.stimmode == ORTHOG_AC || (st->left->ptr->plaid_angle) > 2 * M_2_PI)
        orthogac = 1;
    
    
    if(st->left->ptr->sx > 0.01 && optionflag & SQUARE_RDS)
    {
        calc_rls_polys(st,  sst);
        return;
    }
    if(st->flag & ANTICORRELATE && sst->mode == RIGHTMODE)
        contrast = -pos->contrast;
    if(st->flag & CONTRAST_NEGATIVE)
        val[0] = (double)st->background * (1 - contrast);
    else if(contrast >= 1.0)
        val[0]  = 1.0;
    else
        val[0] = (double)st->background * (1 + contrast);
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
        sst->lum[i] = dogamma(val[i]);
    
    offset[0] = offset[1] = 0;
    ndots = 1+(2 * pos->radius[1]/(sst->dotsiz[1]));
    if(ndots > sst->ndots)
        init_rls(st,sst,sst->density);
    sst->ndots = ndots;
    /* 
     *if st->prev != NULL, this is a stimuls that forms a background 
     * that means a ``hole'', with an appropriate disparity, needs to
     * be made in the background
     */
    if(st->prev != NULL)
    {
        if(optionflag & BACKGROUND_FIXED_BIT)
        {
            offset[0] = st->prev->pos.xy[0] + (st->pos.xy[0] * cos(pos->angle))
            + (st->pos.xy[1] * sin(pos->angle));
            offset[1] = st->prev->pos.xy[0] + (st->pos.xy[0] * sin(pos->angle))
            - (st->pos.xy[1] * cos(pos->angle));
        }
        pixdisp[0] = (st->prev->disp - st->disp) * cos(pos->angle) +
        (st->prev->vdisp - st->vdisp) * sin(pos->angle);
        pixdisp[1] = (st->disp - st->prev->disp) * sin(pos->angle) +
        (st->prev->vdisp - st->vdisp) * cos(pos->angle);
    }
    else
    {
        pixdisp[0] = pixdisp[1] = 0;
    }
    
    if(sst->seedloop == 1)
    {
        /*
         * this gives the right "TF, and gives the right
         * response to manual phase settings of +- PI, but
         * seems wrong!
         * N.B. minus sign at front gives same direction of motion for
         * sine/rls
         * Dec 2000 changed so that TF does not control speed, allows speed
         * to be matched to a sinewave
         */
        phase = -(pos->radius[1]*2 - sst->dotsiz[1]) * pos->phase/( 2 * M_PI);
        phase = -pos->locn[0];
    }
    else
        phase = 0;
 
    // for RLS, phase is only used for disp phase, so its in pixels already
    
    phase = -(pos->phase+pos->locn[0]);
    
    if(optionflag & ANTIALIAS_BIT)
        pixmul = 1; // RLS all done in floats anyway
    else
        pixmul = 1;
    
    w = (vcoord)(pos->imsize[0] - sst->dotsiz[0]);
    h = (vcoord)(pos->imsize[1] - sst->dotsiz[1]);
    w = (vcoord)(2 * pos->radius[0] - sst->dotsiz[0]);
    h = (vcoord)(2 * pos->radius[1] - sst->dotsiz[1]);
    w = (vcoord)(2 * pos->radius[0]);
    h = (vcoord)(2 * pos->radius[1]);
    
    if(sst->xshift != 0)
    {
        xshift[0] = (int)(rint(sst->xshift * cos(pos->angle)));
        xshift[1] = (int)(rint(sst->xshift * sin(pos->angle)));
        /*
         * don't want this now xshift is used for 
         * drifting the RLS
         if(sst->seed & 1)
         {
         xshift[0] = -xshift[0];
         xshift[1] = -xshift[1];
         }
         */
        xshift[1] += phase;
    }
    else
    {
//xshift translates the pattern (relative to the window).  Make sure its a positive number to simplify calculations
//below
        xshift[1] = (phase * pixmul);
        while(xshift[1] > h){
            xshift[1] -= h;
        }
        while(xshift[1] < 0){
            xshift[1] += h;
        }
        xshift[2] = pos->phase2 * pixmul;
        while(xshift[2] > h){
            xshift[2] -= h;
        }
        while(xshift[2] < 0){
            xshift[2] += h;
        }
        xshift[0] = 0;
    }
    csq = dsq= 0;
    if(!(optionflag & SQUARE_RDS))
    {
        asq = pos->radius[0] * pos->radius[0];
        bsq = pos->radius[1] * pos->radius[1];
        eh = pos->radius[0]/10;
        /* 
         * calculate parameters for hole 
         */
        if(st->prev != NULL && st->prev->type == STIM_RLS)
        {
            csq = st->prev->pos.radius[0] * st->prev->pos.radius[0];
            dsq = st->prev->pos.radius[1] * st->prev->pos.radius[1];
            // circular patches don't work yet. let it work as square.
            csq = fabsf(st->prev->pos.radius[0]);	
        }
    }
    else  if(st->prev != NULL && st->prev->type == STIM_RLS)
    {
        csq = fabsf(st->prev->pos.radius[0]-sst->dotsiz[0]/2);
        dsq = fabsf(st->prev->pos.radius[1]);
    }
    if(st->prev != NULL && optionflags[PAINT_BACKGROUND])
        csq = dsq = 0;
    p = sst->iim;
    q = sst->imb; //for second component. E.G. plaid
    pi = sst->iimb; //records 1D pattern
    x = sst->xpos;
    y = sst->ypos;
    zy = sst->yposa;
    zx = sst->xposa;
    iw = w;
    ih = h;
    seedcall = 0;
    
    if(st->correlation < 1 && st->correlation > -1)
        sst->corrdots = sst->ndots * fabsf(st->correlation);
    else
        sst->corrdots = sst->ndots*2;
    /*
     if(1){
     srand48(sst->seed);
     rnd = myrand();
     i = sst->seed % NSEEDS;
     rnd = seeds[i];
     srand48(rnd);
     printf("rnd %ld\n",rnd);
     }
     */
    if(sst->ndots > rndarraylen){
        if(rndarray != NULL)
            free(rndarray);
        rndarray = (long *)malloc(4 * sst->ndots * sizeof(long));
        rndarraylen = 4 * sst->ndots;
    }
    rp = rndarray;
    rq = &rndarray[sst->ndots];
    myrnd_init(sst->seed);
    for(i = 0; i < 10; i++){
        sst->bits[i] = 0;
    }
    
    sst->npaint = sst->npainta = sst->ndots;
    for(i = 0; i < sst->ndots; )
    {
        *x = 0;
        *y = -h/2 + i * sst->dotsiz[1] + xshift[1];
        if(*y > h/2)
            *y -= h;
        *zy = -h/2 + i * sst->dotsiz[1] + xshift[2];
        if(*zy > h/2)
            *zy -= h;
        
        /*     
         * Aproach 1. Don't reset the seed, just call lrand again.
         * must then keep track of values, can't just generate again. Alsp
         * means sl=1 must be done by not calling lrand here. Will need to 
         * keep track of number of calls to lrand() in order to be able to
         * regnerate stimuli later.....
         */
        
        //     if((i > sst->corrdots || sst->mode == LEFTMODE) && sst->newdots)
        /*
         * N.B. This does not work for correlation > 0 and < 1, because the first
         * corrdots lines are ALL the same, then the remainder are all uncorrelated
         * i.e. its because these are painted in order, unlike rds....
         */
        if(sst->corrdots == 0 && sst->mode == RIGHTMODE && !seedcall)
            myrnd_init(sst->seed+200),seedcall++;
        *rp = myrnd_i();
        if(i == sst->ndots -2)
            j = *rp & 1;
        /*
         * used to be << 16 and 17 below, but rnd_i has some funny correlations
         * for these bits after 16 calls (covariance matrix for the resulting RLS not flat
         */
        if(st->dotdist == WHITENOISE16)
            *p = *rp & 0xf;
        else if(*rp & (1<<1)){
            *p = WHITEMODE;
            *pi = 2;
        }
        else{
            *p = BLACKMODE;
            *pi = 0;
        }
        if (sst->mode == RIGHTMODE && orthoguc)
        {
            if(*rp & (1<<4))
                *q = WHITEMODE;
            else
                *q = BLACKMODE;
        }
        if (sst->mode == RIGHTMODE && orthogac)
        {
            if(*rp & (1<<3))
                *q = BLACKMODE;
            else
                *q = WHITEMODE;
        }
        else{
                if(*rp & (1<<3))
                    *q = WHITEMODE;
                else
                    *q = BLACKMODE;
            }
        if ((*rp & 0xff) < pblank){
            *p = 0;
            *pi = 1;
        }
        
        
        if(sst->corrdots > 0 && sst->corrdots < sst->ndots && sst->mode == RIGHTMODE){
            rnd = (*rp>>3) % sst->ndots;
            if(rnd > sst->corrdots){
                if(st->dotdist == WHITENOISE16)
                    *p = *rp & 0xf;
                else if(*rp & (1<<2))
                    *p = WHITEMODE;
                else
                    *p = BLACKMODE;
            }
        }
        bit = i % (sizeof(long) * 8);
        nbit = i/(sizeof(long) * 8);
        if(*p == WHITEMODE){
            sst->bits[nbit] |= (1<<bit);
        }
        
        /*     
         * Aproach 2. Do more manipulations to mask the problem. 
         * We only need one random bit. Choose the bit to test from the
         * random number (or the seed?).
         * use random() to determine the bit?
         * use some semi-random system measure, like the last bit of the clock-time?
         * (non-recoverable)
         
         *rp = lrand48();
         bit = *rp % 32;
         bit = 0;
         if(*rp & (1<< bit))
         *p = WHITEMODE;
         else
         *p = BLACKMODE;
         */
        
        /*
         * drand was no better.
         drnd = drand48();
         if(drnd > 0.5)
         *p = WHITEMODE;
         else
         *p = BLACKMODE;
         */
        
        if(csq != 0)  /* this is the background */
        {
            xsq = fabsf((float)(*x - pixdisp[0]+offset[0]));
            ysq = fabsf((float)(*y - pixdisp[1]+offset[1]));
            if(xsq > csq || ysq > dsq)
                *p |= LEFTDOT;
            xsq = fabsf((float)(*x + pixdisp[0]+offset[0]));
            ysq = fabsf((float)(*y + pixdisp[1]+offset[1]));
            if(xsq > csq || ysq > dsq)
                *p |= RIGHTDOT;
            if(bsq > 0){
                aval = 1 - (*y * *y)/bsq;
                if(aval > 0)
                    *x = pos->radius[0] * sqrt(aval);
                else
                    *x = pos->radius[0];
                *x = pos->radius[0];
            }
            else
                *x = pos->radius[0];
        }
        else /*always paint the dot */
        {
            if(bsq > 0){
                aval = 1 - (*y * *y)/bsq;
                if(aval > 0)
                    lastx = *x = pos->radius[0] * sqrt(aval);
                else if (i == 0 || lastx > 0){
                    *x = eh;
                    lastx = 0;
                }
                else
                    *x = 0;
                
                aval = 1 - (*zy * *zy)/bsq;
                if(aval > 0)
                    lastzx = *zx = pos->radius[0] * sqrt(aval);
                else if (i == 0 || lastzx > 0){
                    *zx = eh;
                    lastzx = 0;
                }
                else
                    *zx = 0;
            }
            else
                *x = pos->radius[0];
            *p |= (RIGHTDOT | LEFTDOT);
            if(*y < lasty && i > 0 && bsq > 0){
                lasty = *y;
                if(lasty+1 < h/2){ //gone back to Left side. Add final vertex to R first
                    *y++ = h/2;
                    if(optionflag & SQUARE_RDS){
                        *x++ = pos->radius[0];
                        *x = pos->radius[0];
                    }
                    else{
                        lastx = *x;
                        *x++ = 0;
                        *x = lastx;
                    }
                    nx++;
                    *y = lasty;
                    *p++ = lastp; // finish off last bar;
                    sst->npaint++;
                }
                if(*y > 1-h/2){
                    lasty = *y;
                    *y++ = -h/2;
                    *y = lasty;
                    /* now set lasty so we don't come here again */
                    lasty = -h/2;
                    if(optionflag & SQUARE_RDS){
                        *x++ = pos->radius[0];
                        *x = pos->radius[0];
                    }
                    else{
                        lastx = *x;
                        *x++ = 0;
                        *x = lastx;
                    }
                    nx++;
                    *(p+1) = *p;
                    *p++;
                    sst->npaint++;
                }
            }
            lasty = *y;
            lastp = *p;
            if(*zy < lastzy && i > 0 && bsq > 0){
                lastzy = *zy;
                if(lastzy+1 < h/2){
                    *zy++ = h/2;
                    if(optionflag & SQUARE_RDS){
                        *zx++ = pos->radius[0];
                        *zx = pos->radius[0];
                    }
                    else{
                        lastzx = *zx;
                        *zx++ = 0;
                        *zx = lastzx;
                    }
                    *zy = lastzy;
                    *q++ = lastq; // finish off last bar;
                    sst->npainta++;
                }
                if(*zy > 1-h/2){
                    lastzy = *zy;
                    *zy++ = -h/2;
                    *zy = lastzy;
                    /* now set lasty so we don't come here again */
                    lastzy = -h/2;
                    if(optionflag & SQUARE_RDS){
                        *zx++ = pos->radius[0];
                        *zx = pos->radius[0];
                    }
                    else{
                        lastzx = *zx;
                        *zx++ = 0;
                        *zx = lastzx;
                    }
                    *(q+1) = *q;
                    *q++;
                    sst->npainta++;
                }
            }
            lastq = *q;
            lastzy = *zy;
        }
        i++,x++,y++,p++,rp++,q++,zx++,zy++,pi++;
        nx++;
    }
    if(nx > sst->xpl)
        fprintf(stderr,"Made too many (%d) Quads in RLS (alloc %d)\n",nx,sst->xpl);
    if(seroutfile && option2flag & RANDOM){
        if(sst->mode == RIGHTMODE)
            fprintf(seroutfile,"R%d ",st->framectr);
        else
            fprintf(seroutfile,"L%d ",st->framectr);
        j = 0;
        while(sst->bits[j] != 0 && j < 10){
            fprintf(seroutfile,"%x ",sst->bits[j]);
            j++;
        }
        fprintf(seroutfile,"\n");
    }
}


void calc_rls_polys(Stimulus *st, Substim *sst)
{
    int i,j,partdisp,ndots,nx=0;
    float cval,f,sy,cm,deg,iscale[2],val[2];
    float asq,bsq = 0,csq,dsq,xsq,ysq,pixdisp[2],offset[2];
    int *p,*q,*cend,yi,lastp,lastq;
    vcoord *x,*y,w,h,lastx,eh=0,lasty;
    int xshift[2],iw,ih;
    Locator *pos = &sst->pos;
    float phase,contrast = pos->contrast;
    int pixmul = 1,seedcall = 0;
    vcoord yp,diff;
    double drnd,aval;
    int bit, nbit,k,nrect;
    long *rp,rnd;
    float dc,xstart,xp,dw,*pf,ey,ex,ysd,xsd,eyb,exb,xstep;
    
    
    if(st->flag & ANTICORRELATE && sst->mode == RIGHTMODE)
        contrast = -pos->contrast;
    if(st->flag & CONTRAST_NEGATIVE)
        val[0] = (double)st->background * (1 - contrast);
    else if(contrast >= 1.0)
        val[0]  = 1.0;
    else
        val[0] = (double)st->background * (1 + contrast);
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
        sst->lum[i] = dogamma(val[i]);
    
    offset[0] = offset[1] = 0;
    dw = sst->dotsiz[1];
    ndots = 1+(2 * pos->radius[1]/(sst->dotsiz[1]));
    if(ndots > sst->ndots)
        init_rls(st,sst,sst->density);
    sst->ndots = ndots;
    nrect = 1+ (pos->radius[0] * 2)/pos->ss[0];
    if(nrect > 20)
        xstep = pos->radius[0]/9;
    else
        xstep = pos->ss[0];
    
    /* 
     *if st->prev != NULL, this is a stimuls that forms a background 
     * that means a ``hole'', with an appropriate disparity, needs to
     * be made in the background
     */
    if(st->prev != NULL)
    {
        if(optionflag & BACKGROUND_FIXED_BIT)
        {
            offset[0] = st->prev->pos.xy[0] + (st->pos.xy[0] * cos(pos->angle))
            + (st->pos.xy[1] * sin(pos->angle));
            offset[1] = st->prev->pos.xy[0] + (st->pos.xy[0] * sin(pos->angle))
            - (st->pos.xy[1] * cos(pos->angle));
        }
        pixdisp[0] = (st->prev->disp - st->disp) * cos(pos->angle) +
        (st->prev->vdisp - st->vdisp) * sin(pos->angle);
        pixdisp[1] = (st->disp - st->prev->disp) * sin(pos->angle) +
        (st->prev->vdisp - st->vdisp) * cos(pos->angle);
    }
    else
    {
        pixdisp[0] = pixdisp[1] = 0;
    }
    
    if(sst->seedloop == 1)
    {
        /*
         * this gives the right "TF, and gives the right
         * response to manual phase settings of +- PI, but
         * seems wrong!
         * N.B. minus sign at front gives same direction of motion for
         * sine/rls
         * Dec 2000 changed so that TF does not control speed, allows speed
         * to be matched to a sinewave
         */
        phase = -(pos->radius[1]*2 - sst->dotsiz[1]) * pos->phase/( 2 * M_PI);
        phase = -pos->locn[0];
        phase = -(pos->radius[1]*2 - sst->dotsiz[1]) * pos->phase/( 2 * M_PI);
        phase = -(deg2pix(1/st->f) * pos->phase/(2 * M_PI)+pos->locn[0]);
    }
    else
        phase = 0;
    
    phase = -(pos->radius[1]*2 - sst->dotsiz[1]) * pos->phase/( 2 * M_PI);
    phase = -(deg2pix(1/st->f) * pos->phase/(2 * M_PI)+pos->locn[0]);
    // for RLS, phase is only used for disp phase, so its in pixels already
    
    phase = -(pos->phase+pos->locn[0]);
    
    if(optionflag & ANTIALIAS_BIT)
        pixmul = 1; // RLS all done in floats anyway
    else
        pixmul = 1;
    
    w = (vcoord)(pos->imsize[0] - sst->dotsiz[0]);
    h = (vcoord)(pos->imsize[1] - sst->dotsiz[1]);
    w = (vcoord)(2 * pos->radius[0] - sst->dotsiz[0]);
    h = (vcoord)(2 * pos->radius[1] - sst->dotsiz[1]);
    w = (vcoord)(2 * pos->radius[0]);
    h = (vcoord)(2 * pos->radius[1]);
    
    if(sst->xshift != 0)
    {
        xshift[0] = (int)(rint(sst->xshift * cos(pos->angle)));
        xshift[1] = (int)(rint(sst->xshift * sin(pos->angle)));
        xshift[1] += phase;
    }
    else
    {
        xshift[1] = (phase * pixmul);
        while(xshift[1] > h){
            xshift[1] -= h;
        }
        while(xshift[1] < 0){
            xshift[1] += h;
        }
        xshift[0] = 0;
    }
    csq = dsq= 0;
    if(!(optionflag & SQUARE_RDS))
    {
        asq = pos->radius[0] * pos->radius[0];
        bsq = pos->radius[1] * pos->radius[1];
        eh = pos->radius[0]/10;
        /* 
         * calculate parameters for hole 
         */
        if(st->prev != NULL && st->prev->type == STIM_RLS)
        {
            csq = st->prev->pos.radius[0] * st->prev->pos.radius[0];
            dsq = st->prev->pos.radius[1] * st->prev->pos.radius[1];
            // circular patches don't work yet. let it work as square.
            csq = fabsf(st->prev->pos.radius[0]);	
        }
    }
    else  if(st->prev != NULL && st->prev->type == STIM_RLS)
    {
        csq = fabsf(st->prev->pos.radius[0]-sst->dotsiz[0]/2);
        dsq = fabsf(st->prev->pos.radius[1]);
    }
    if(st->prev != NULL && optionflags[PAINT_BACKGROUND])
        csq = dsq = 0;
    p = sst->iim;
    pf = sst->imb;
    x = sst->xpos;
    y = sst->ypos;
    iw = w;
    ih = h;
    seedcall = 0;
    
    if(st->correlation < 1 && st->correlation > -1)
        sst->corrdots = sst->ndots * fabsf(st->correlation);
    else
        sst->corrdots = sst->ndots*2;
    if(sst->ndots > rndarraylen){
        if(rndarray != NULL)
            free(rndarray);
        rndarray = (long *)malloc(2 * sst->ndots * sizeof(long));
        rndarraylen = 2 * sst->ndots;
    }
    rp = rndarray;
    myrnd_init(sst->seed);
    for(i = 0; i < 10; i++){
        sst->bits[i] = 0;
    }
    
    sst->npaint = 0;
    ysd = deg2pix(sst->ptr->sx);
    ysd = 2 * ysd * ysd;
    xsd = deg2pix(sst->ptr->sy);
    xsd = 2 * xsd * xsd;
    for(i = 0; i < sst->ndots; )
    {
        *x = 0;
        *y = -h/2 + i * sst->dotsiz[1] + xshift[1];
        yp = -h/2 + i * sst->dotsiz[1] + xshift[1];
        if(yp > h/2)
            yp -= h;
        ysq = (yp+dw) * (yp+dw);
        eyb = exp(-(ysq/ysd));
        ysq = yp * yp;
        ey = exp(-(ysq/ysd));
        
        if(sst->corrdots == 0 && sst->mode == RIGHTMODE && !seedcall)
            myrnd_init(sst->seed+200),seedcall++;
        *rp = myrnd_i();
        if(i == sst->ndots -2)
            j = *rp & 1;
        /*
         * used to be << 16 and 17 below, but rnd_i has some funny correlations
         * for these bits after 16 calls (covariance matrix for the resulting RLS not flat
         */
        if(st->dotdist == WHITENOISE16)
            *p = *rp & 0xf;
        else if(*rp & (1<<1)){
            *p = WHITEMODE;
            dc = sst->lum[1];
        }
        else{
            *p = BLACKMODE;
            dc = sst->lum[0];
        }
        if(sst->corrdots > 0 && sst->corrdots < sst->ndots && sst->mode == RIGHTMODE){
            rnd = (*rp>>3) % sst->ndots;
            if(rnd > sst->corrdots){
                if(st->dotdist == WHITENOISE16)
                    *p = *rp & 0xf;
                else if(*rp & (1<<2))
                    *p = WHITEMODE;
                else
                    *p = BLACKMODE;
            }
        }
        bit = i % (sizeof(long) * 8);
        nbit = i/(sizeof(long) * 8);
        if(*p == WHITEMODE){
            sst->bits[nbit] |= (1<<bit);
        }
        
        
        if(csq != 0)  /* this is the background */
        {
            xsq = fabsf((float)(*x - pixdisp[0]+offset[0]));
            ysq = fabsf((float)(yp - pixdisp[1]+offset[1]));
            if(xsq > csq || ysq > dsq)
                *p |= LEFTDOT;
            xsq = fabsf((float)(*x + pixdisp[0]+offset[0]));
            ysq = fabsf((float)(yp + pixdisp[1]+offset[1]));
            if(xsq > csq || ysq > dsq)
                *p |= RIGHTDOT;
            if(bsq > 0){
                aval = 1 - (ysq)/bsq;
                if(aval > 0)
                    *x = pos->radius[0] * sqrt(aval);
                else
                    *x = pos->radius[0];
                *x = pos->radius[0];
            }
            else
                *x = pos->radius[0];
        }
        else /*always paint the dot */
        {
            if(bsq > 0){
                aval = 1 - (ysq)/bsq;
                if(aval > 0)
                    lastx = *x = pos->radius[0] * sqrt(aval);
                else if (i == 0 || lastx > 0){
                    *x = eh;
                    lastx = 0;
                }
                else
                    *x = 0;
            }
            else{
                *x = pos->radius[0];
                xstart = *x;
                *p |= (RIGHTDOT | LEFTDOT);
                
                ex = exp(-(xstart * xstart)/xsd);
                nrect = 0;
                for(xp = -xstart; xp < xstart; xp +=  xstep){
                    *x++ = xp; 
                    *y++ = yp;
                    nx++;
                    exb = ex;	
                    xsq = (xp +xstep)* (xp+xstep);
                    ex = exp(-(xsq/xsd));
                    *pf++ =  0.5 + (dc-0.5) * ey * exb;
                    
                    *x++ = xp+xstep;
                    *y++ = yp;
                    nx++;
                    *pf++ =  0.5 + (dc-0.5) * ey * ex;
                    
                    *x++ = xp+xstep; 
                    *y++ = yp+dw;
                    *pf++ =  0.5+(dc-0.5) *eyb * ex;
                    nx++;
                    
                    *x++ = xp; 
                    *y++ = yp+dw;
                    *pf++ =  0.5+(dc-0.5) *eyb *exb;
                    nx++;
                    sst->npaint++;  // # of polygons made
                    nrect++;
                }
            }
            /*
             * if its a circular patch, want a final triangle on the end
             */
            if(*(y-1) < lasty && i > 0 && bsq > 0){
                if(lasty+1 < h/2){
                    *y++ = h/2;
                    if(optionflag & SQUARE_RDS)
                        *x++ = pos->radius[0];
                    else
                        *x++ = 0;
                    nx++;
                    *p++ = lastp; // finish off last bar;
                    sst->npaint++;
                }
                if(lasty+1 < h/2){
                    *y++ = h/2;
                    if(optionflag & SQUARE_RDS)
                        *x++ = pos->radius[0];
                    else
                        *x++ = 0;
                    nx++;
                    *p++ = lastp; // finish off last bar;
                    sst->npaint++;
                }
                if(*y > 1-h/2){
                    lasty = *y;
                    *y++ = -h/2;
                    *y = lasty;
                    if(optionflag & SQUARE_RDS){
                        *x++ = pos->radius[0];
                        *x = pos->radius[0];
                    }
                    else{
                        lastx = *x;
                        *x++ = 0;
                        *x = lastx;
                    }
                    nx++;
                    *(p+1) = *p;
                    *p++;
                    sst->npaint++;
                }
            } // end last y
            lasty = *y;
            lastp = *p;

        }
        i++,rp++;
    }
    if(seroutfile && option2flag & RANDOM){
        if(sst->mode == RIGHTMODE)
            fprintf(seroutfile,"R%d ",st->framectr);
        else
            fprintf(seroutfile,"L%d ",st->framectr);
        j = 0;
        while(sst->bits[j] != 0 && j < 10){
            fprintf(seroutfile,"%x ",sst->bits[j]);
            j++;
        }
        fprintf(seroutfile,"\n");
    }
}



/*
 * this simply draws all the dots on the list, with an appropriate
 * disparity
 */
void paint_rls(Stimulus *st, int mode)
{
    int i;
    int *p,d,*end,n = 0;
    vcoord  w,h,*x,*y,fw,fh,lasty;
    vcoord z[2];
    short pt[2];
    float vcolor[4], bcolor[4],gcolor[4];
    vcoord xmv;
    int dotmode = 0;
    Substim *sst = st->left;
    Locator *pos = &st->pos;
    float angle,cosa,sina,val,valsum = 0;
    vcoord rect[8],crect[8];
    
    
    if (fabs(st->left->ptr->plaid_angle) > 0 && st->left->ptr->plaid_angle > -1000){
        paint_rls_plaid(st, mode);
        return;
    }
    if(st->left->ptr->sx > 0.01 && optionflag & SQUARE_RDS)
    {
        paint_rls_polygons(st, mode);
        return;
    }
    
    angle = rad_deg(pos->angle);
    /*
     * first glTranslatef the co-ordinate system to put the stimulus
     * in the right place/orientation. Use different colors for
     * L/R dots
     */
    
    glPushMatrix();
    vcolor[0] = vcolor[1] = vcolor[2] = 0;
    bcolor[0] = bcolor[1] = bcolor[2] = 0;
    bcolor[3] = vcolor[3] = 1.0;
    gcolor[0] = gcolor[1] = gcolor[2] = 0.5;
    if(mode == LEFTMODE)
    {
        dotmode = LEFTDOT;
        xmv = pos->xy[0]+st->disp;
        glTranslatef(xmv,pos->xy[1]+st->vdisp,0);
        vcolor[0] = sst->lum[0];
        bcolor[0] = sst->lum[1];
        vcolor[3] = sst->lum[0];
        bcolor[3] = sst->lum[1];
        gcolor[3] = 0.5;
    }
    else if(mode == RIGHTMODE)
    {
        dotmode = RIGHTDOT;
        sst = st->right;
        xmv = pos->xy[0]-st->disp;
        glTranslatef(xmv,pos->xy[1]-st->vdisp,0);
        vcolor[3] = vcolor[1] = vcolor[2] = sst->lum[0];
        bcolor[3] = bcolor[1] = bcolor[2] = sst->lum[1];
        gcolor[3] = gcolor[1] = gcolor[2] = 0.5;
    }
    if(optionflags[STIMULUS_IN_OVERLAY])
    {
        vcolor[1] = vcolor[2] = vcolor[0] = sst->lum[0];
        bcolor[1] = bcolor[2] = bcolor[0] = sst->lum[1];
    }
    glRotatef(angle,0.0,0.0,1.0);
    
    mycolor(vcolor);
    w = sst->dotsiz[0]/2;
    h = sst->dotsiz[1]/2;
    fw = sst->dotsiz[1];
    fh = sst->dotsiz[0];
    
    h = h - 0.5;
    
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
    
    p = sst->iim;
    end = (sst->iim+sst->npaint);
    x = sst->xpos;
    y = sst->ypos;
    i = 0;
    /* now paint the lines */
    
    p = sst->iim;
    x = sst->xpos;
    y = sst->ypos;
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0);
    
    if(optionflag & ANTIALIAS_BIT)
	{
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		if(expt.polygonsmooth)
//            glEnable(GL_POLYGON_SMOOTH);
		glLineWidth(1.0);
	}
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glBegin(GL_QUAD_STRIP);
    i = 0;
    lasty = *y;
    for(;p < end; p++,x++,y++)
    {
        if(*y < lasty){
            glEnd();
            glBegin(GL_QUAD_STRIP);
        }
        lasty = *y;
        
        if(st->dotdist == WHITENOISE16){
            val = (float)(*p & 0xf)/0xe;  //0 ->1, not 0 ->15/16
            valsum += val;
            n++;
            glColor3f(val,val,val);
        }
        else if(*p & BLACKMODE)
            mycolor(vcolor);      
        else if(*p & WHITEMODE)
            mycolor(bcolor);
        else
            mycolor(gcolor);
        if(*p & dotmode){
            z[0] = *x; 
            z[1] = *y; 
            myvx(z);
            z[0] = -*x; 
            z[1] = *y; 
            myvx(z);
        }
    }
    glEnd();
 
    if(optionflag & ANTIALIAS_BIT)
	{
        p = sst->iim;
        x = sst->xpos;
        y = sst->ypos;
                glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_LINE_SMOOTH);
                glLineWidth(1.0);
    glBegin(GL_LINES);
    i = 0;
    lasty = *y;
    for(;p < end; p++,x++,y++)
    {
        lasty = *y;
        
        if(st->dotdist == WHITENOISE16){
            val = (float)(*p & 0xf)/0xe;  //0 ->1, not 0 ->15/16
            valsum += val;
            n++;
            glColor3f(val,val,val);
        }
        else if(*p & BLACKMODE)
            mycolor(vcolor);      
        else if(*p & WHITEMODE)
            mycolor(bcolor);
        if(*p & dotmode){
            z[0] = *x; 
            z[1] = *y; 
            myvx(z);
            z[0] = -*x; 
            z[1] = *y; 
            myvx(z);
        }
    }
    glEnd();
    }
    glPopMatrix();
    val = valsum/n;
}

/*
 * Draws two RLS superimposed to make a noise plaid
 */
void paint_rls_plaid(Stimulus *st, int mode)
{
    int i;
    int *p,d,*end,n = 0;
    vcoord  w,h,*x,*y,fw,fh,lasty;
    vcoord z[2];
    short pt[2];
    float vcolor[4], bcolor[4];
    vcoord xmv;
    int dotmode = 0;
    Substim *sst = st->left;
    Locator *pos = &st->pos;
    float angle,cosa,sina,val,valsum = 0,alpha=1.0;
    vcoord rect[8],crect[8];
    
    if(st->left->ptr->sx > 0.01 && optionflag & SQUARE_RDS)
    {
        paint_rls_polygons(st, mode);
        return;
    }
    
    angle = rad_deg(pos->angle);
    /*
     * first glTranslatef the co-ordinate system to put the stimulus
     * in the right place/orientation. Use different colors for
     * L/R dots
     */
    
    glPushMatrix();
    vcolor[0] = vcolor[1] = vcolor[2] = 0;
    bcolor[0] = bcolor[1] = bcolor[2] = 0;
    bcolor[3] = vcolor[3] = 1.0;
    if(mode == LEFTMODE)
    {
        dotmode = LEFTDOT;
        xmv = pos->xy[0]+st->disp;
        glTranslatef(xmv,pos->xy[1]+st->vdisp,0);
        vcolor[0] = sst->lum[0];
        bcolor[0] = sst->lum[1];
        vcolor[3] = sst->lum[0];
        bcolor[3] = sst->lum[1];
    }
    else if(mode == RIGHTMODE)
    {
        dotmode = RIGHTDOT;
        sst = st->right;
        xmv = pos->xy[0]-st->disp;
        glTranslatef(xmv,pos->xy[1]-st->vdisp,0);
        vcolor[3] = vcolor[1] = vcolor[2] = sst->lum[0];
        bcolor[3] = bcolor[1] = bcolor[2] = sst->lum[1];
    }
    if(optionflags[STIMULUS_IN_OVERLAY])
    {
        vcolor[1] = vcolor[2] = vcolor[0] = sst->lum[0];
        bcolor[1] = bcolor[2] = bcolor[0] = sst->lum[1];
    }
    glRotatef(angle,0.0,0.0,1.0);
    
    mycolor(vcolor);
    w = sst->dotsiz[0]/2;
    h = sst->dotsiz[1]/2;
    fw = sst->dotsiz[1];
    fh = sst->dotsiz[0];
    
    h = h - 0.5;
    
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
    
    p = sst->iim;
    end = (sst->iim+sst->npaint);
    x = sst->xpos;
    y = sst->ypos;
    i = 0;
    /* now paint the lines */
    
    p = sst->iim;
    x = sst->xpos;
    y = sst->ypos;
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0);
    
    if(optionflag & ANTIALIAS_BIT)
	{
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		if(expt.polygonsmooth)
            //            glEnable(GL_POLYGON_SMOOTH);
            glLineWidth(1.0);
	}
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glBegin(GL_QUAD_STRIP);
    i = 0;
    lasty = *y;
    for(;p < end; p++,x++,y++)
    {
        if(*y < lasty){
            glEnd();
            glBegin(GL_QUAD_STRIP);
        }
        lasty = *y;
        
        if(st->dotdist == WHITENOISE16){
            val = (float)(*p & 0xf)/0xe;  //0 ->1, not 0 ->15/16
            valsum += val;
            n++;
            glColor4f(val,val,val,alpha);
        }
        else if(*p & BLACKMODE)
            glColor4f(vcolor[0], vcolor[1], vcolor[2], alpha);
        else if(*p & WHITEMODE)
            glColor4f(bcolor[0], bcolor[1], bcolor[2], alpha);
        if(*p & dotmode){
            z[0] = *x;
            z[1] = *y;
            myvx(z);
            z[0] = -*x;
            z[1] = *y;
            myvx(z);
        }
        else{ // for debugger
            z[0] = *x;
        }
    }
    glEnd();
    
    if(optionflag & ANTIALIAS_BIT)
	{
        p = sst->iim;
        x = sst->xpos;
        y = sst->ypos;
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1.0);
        glBegin(GL_LINES);
        i = 0;
        lasty = *y;
        for(;p < end; p++,x++,y++)
        {
            lasty = *y;
            
            if(st->dotdist == WHITENOISE16){
                val = (float)(*p & 0xf)/0xe;  //0 ->1, not 0 ->15/16
                valsum += val;
                n++;
                glColor3f(val,val,val);
            }
            else if(*p & BLACKMODE)
                mycolor(vcolor);
            else if(*p & WHITEMODE)
                mycolor(bcolor);
            if(*p & dotmode){
                z[0] = *x;
                z[1] = *y;
                myvx(z);
                z[0] = -*x;
                z[1] = *y;
                myvx(z);
            }
            else{ // for debugger
                z[0] = *x;
            }
        }
        glEnd();
    }
    
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRotatef(rad_deg(sst->ptr->plaid_angle),0.0,0.0,1.0);
    
    if(optionflag & ANTIALIAS_BIT)
	{
		glEnable(GL_LINE_SMOOTH);
        glLineWidth(1.0);
	}
    p = sst->imb;
        end = (sst->imb+sst->npainta);
    x = sst->xposa;
    y = sst->yposa;
    glBegin(GL_QUAD_STRIP);
    i = 0;
    lasty = *y;
    alpha = 0.5;
    for(;p < end; p++,x++,y++)
    {
        if(*y < lasty){
            glEnd();
            glBegin(GL_QUAD_STRIP);
        }
        lasty = *y;
        
        if(st->dotdist == WHITENOISE16){
            val = (float)(*p & 0xf)/0xe;  //0 ->1, not 0 ->15/16
            valsum += val;
            n++;
            glColor4f(val,val,val, alpha);
        }
        else if(*p & BLACKMODE)
            glColor4f(vcolor[0], vcolor[1], vcolor[2], alpha);
        else if(*p & WHITEMODE)
            glColor4f(bcolor[0], bcolor[1], bcolor[2], alpha);
            z[0] = *x;
            z[1] = *y;
            myvx(z);
            z[0] = -*x;
            z[1] = *y;
            myvx(z);
    }
    glEnd();
    
    
    if(optionflag & ANTIALIAS_BIT)
	{
        p = sst->imb;
        x = sst->xposa;
        y = sst->yposa;
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1.0);
        glBegin(GL_LINES);
        i = 0;
        lasty = *y;
        for(;p < end; p++,x++,y++)
        {
            lasty = *y;
            
            if(st->dotdist == WHITENOISE16){
                val = (float)(*p & 0xf)/0xe;  //0 ->1, not 0 ->15/16
                valsum += val;
                n++;
                glColor3f(val,val,val);
            }
            else if(*p & BLACKMODE)
                mycolor(vcolor);
            else if(*p & WHITEMODE)
                mycolor(bcolor);
            if(*p & dotmode){
                z[0] = *x;
                z[1] = *y;
                myvx(z);
                z[0] = -*x;
                z[1] = *y;
                myvx(z);
            }
            else{ // for debugger
                z[0] = *x;
            }
        }
        glEnd();
    }
    
    glDisable(GL_BLEND);
    
    glPopMatrix();
    val = valsum/n;
}


/*
 * this simply draws all the dots on the list, with an appropriate
 * disparity
 */
void paint_rls_polygons(Stimulus *st, int mode)
{
    int i,j,k;
    int *p,d,*end,n = 0;
    vcoord  w,h,*x,*y,fw,fh,lasty;
    vcoord z[2];
    short pt[2];
    float vcolor[4], bcolor[4],pcolor[4],*pf;
    vcoord xmv;
    int dotmode = 0;
    Substim *sst = st->left;
    Locator *pos = &sst->pos;
    float angle,cosa,sina,val,valsum = 0;
    vcoord rect[8],crect[8];
    
    
    angle = rad_deg(pos->angle);
    /*
     * first glTranslatef the co-ordinate system to put the stimulus
     * in the right place/orientation. Use different colors for
     * L/R dots
     */
    
    glPushMatrix();
    vcolor[0] = vcolor[1] = vcolor[2] = 0;
    bcolor[0] = bcolor[1] = bcolor[2] = 0;
    bcolor[3] = vcolor[3] = 1.0;
    if(mode == LEFTMODE)
    {
        dotmode = LEFTDOT;
        xmv = pos->xy[0]+st->disp;
        glTranslatef(xmv,pos->xy[1]+st->vdisp,0);
        vcolor[0] = sst->lum[0];
        bcolor[0] = sst->lum[1];
        vcolor[3] = sst->lum[0];
        bcolor[3] = sst->lum[1];
        angle = rad_deg(st->left->pos.angle);
    }
    else if(mode == RIGHTMODE)
    {
        dotmode = RIGHTDOT;
        sst = st->right;
        xmv = pos->xy[0]-st->disp;
        glTranslatef(xmv,pos->xy[1]-st->vdisp,0);
        vcolor[3] = vcolor[1] = vcolor[2] = sst->lum[0];
        bcolor[3] = bcolor[1] = bcolor[2] = sst->lum[1];
        angle = rad_deg(st->right->pos.angle);
    }
    if(optionflags[STIMULUS_IN_OVERLAY])
    {
        vcolor[1] = vcolor[2] = vcolor[0] = sst->lum[0];
        bcolor[1] = bcolor[2] = bcolor[0] = sst->lum[1];
    }
    glRotatef(angle,0.0,0.0,1.0);
    
    mycolor(vcolor);
    w = sst->dotsiz[0]/2;
    h = sst->dotsiz[1]/2;
    fw = sst->dotsiz[1];
    fh = sst->dotsiz[0];
    
    h = h - 0.5;
    
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
    
    p = sst->iim;
    pf = sst->imb;
    end = (sst->iim+sst->npaint);
    x = sst->xpos;
    y = sst->ypos;
    i = 0;
    /* now paint the lines */
    
    p = sst->iim;
    x = sst->xpos;
    y = sst->ypos;
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0);
    glShadeModel(GL_SMOOTH);
    
    if(optionflag & ANTIALIAS_BIT)
	{
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
	}
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    i = 0;
    for(i = 0; i < sst->npaint; i++)
    {
        glBegin(GL_POLYGON);
        for(k =0; k < 4; k++){
            z[0] = *x++; 
            z[1] = *y++; 
            pcolor[0] = pcolor[1] = pcolor[2] = pcolor[3] = *pf++;
            mycolor(pcolor);
            myvx(z);
        }
        glEnd();
    }
    glPopMatrix();
    val = valsum/n;
}

void paint_rls_lines(Stimulus *st, int mode)
{
    int i;
    int *p,d,*end;
    vcoord  w,h,*x,*y,fw,fh;
    vcoord z[2];
    short pt[2];
    float vcolor[4], bcolor[4];
    vcoord xmv;
    Substim *sst = st->left;
    Locator *pos = &st->pos;
    float angle,cosa,sina;
    vcoord rect[8],crect[8];
    
    
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
        xmv = pos->xy[0]+st->disp;
        glTranslatef(xmv,pos->xy[1]+st->vdisp,0);
        vcolor[0] = sst->lum[0];
        bcolor[0] = sst->lum[1];
    }
    else if(mode == RIGHTMODE)
    {
        sst = st->right;
        xmv = pos->xy[0]-st->disp;
        glTranslatef(xmv,pos->xy[1]-st->vdisp,0);
        vcolor[1] = vcolor[2] = sst->lum[0];
        bcolor[1] = bcolor[2] = sst->lum[1];
    }
    if(optionflags[STIMULUS_IN_OVERLAY])
    {
        vcolor[1] = vcolor[2] = vcolor[0] = sst->lum[0];
        bcolor[1] = bcolor[2] = bcolor[0] = sst->lum[1];
    }
    bcolor[3] = vcolor[3] = 1.0;
    glRotatef(angle,0.0,0.0,1.0);
    
    mycolor(vcolor);
    w = sst->dotsiz[0]/2;
    h = sst->dotsiz[1]/2;
    fw = sst->dotsiz[1];
    fh = sst->dotsiz[0];
    
    h = h - 0.5;
    
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
    
    p = sst->iim;
    end = (sst->iim+sst->ndots);
    x = sst->xpos;
    y = sst->ypos;
    i = 0;
    /* now paint the lines */
    
    p = sst->iim;
    x = sst->xpos;
    y = sst->ypos;
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(fw*1.5);
    
    if(optionflag & ANTIALIAS_BIT)
	{
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(fw*2);
	}
    
    glBegin(GL_LINES);
    i = 0;
    for(;p < end; p++,x++,y++)
    {
        if(*p & BLACKMODE)
            mycolor(vcolor);      
        else if(*p & WHITEMODE)
            mycolor(bcolor);
        if(*p & mode){
            z[0] = *x+pos->radius[0]; 
            z[1] = *y; 
            myvx(z);
            z[0] = *x-pos->radius[0]; 
            z[1] = *y; 
            myvx(z);
        }
    }
    glEnd();
    glPopMatrix();
}


int SaveRls(Stimulus *st, FILE *fd)
{
    Locator *pos = &st->pos;
    vcoord offset[2],xp[5000],yp[5000],disps[5000];
    int *p,d,*end,i;
    vcoord  w,h,*x,*y,fw,fh,*pdisp;
    Substim *sst = st->left;
    short colors[5000];
    int ndots[2];
    char name[BUFSIZ];
    static int firstcall = 1;
    
    
    ndots[0] = ndots[1] = 0;
    if(fd == NULL){
        fprintf(stderr,"Can't write rls\n");
        return(0);
    }
    
    if(testflags[SAVE_IMAGES] == 5)
        fprintf(fd,"BinocDisps RLS %s px %.4f\n",VERSION_NUMBER,deg2pix(1));
    else   if(testflags[SAVE_IMAGES] != 6)
        fprintf(fd,"BinocFrame RLS %s\n",VERSION_NUMBER);
    
    
    offset[0] = pos->xy[0] + st->disp;
    offset[1] = pos->xy[1] + st->vdisp;
    p = sst->iim;
    x = sst->xpos;
    y = sst->ypos;
    pdisp = sst->imb;
    end = (sst->iim+sst->ndots);
    i = 0;
    
    /*
     * First write out left eye dot locations
     */
    for(;p < end; p++,x++,y++,pdisp++)
    {
        if(*p & LEFTDOT){
            if(st->dotdist == WHITENOISE16)
                colors[i] = *p & 0xf;
            else if(*p & BLACKMODE)
                colors[i] = 0;
            else if(*p & WHITEMODE)
                colors[i] = 1;
        }
        else
            colors[i] = 10000;
        i++;
    }
    ndots[0] = i;
    if(testflags[SAVE_IMAGES] == 6){
        fprintf(fd,"%d", st->left->seed);
        for(i = 0; i < ndots[0]; i++)
            fprintf(fd," %d", colors[i]);
        fprintf(fd,"\n");
    }
    else{
        fprintf(fd,"Id %d frame %d dots %d se%d\n",
                expt.allstimid,expt.framesdone,ndots[0],st->left->seed);
        fwrite(colors,sizeof(short),ndots[0], fd);
    }
    
    /*
     * Then right eye dot locaitons
     */
    offset[0] = pos->xy[0] - st->disp;
    offset[1] = pos->xy[1] - st->vdisp;
    sst = st->right;
    p = sst->iim;
    x = sst->xpos;
    y = sst->ypos;
    end = (sst->iim+sst->ndots);
    i = 0;
    for(;p < end; p++,x++,y++)
    {
        if(*p & RIGHTDOT){
            if(st->dotdist == WHITENOISE16)
                colors[i] = *p & 0xf;
            else if(*p & BLACKMODE)
                colors[i] = 0;
            else if(*p & WHITEMODE)
                colors[i] = 1;
        }
        else
            colors[i] = 10000;
        i++;
    }
    ndots[1] = i;
    
    if(testflags[SAVE_IMAGES] == 6){
        fprintf(fd,"%d", expt.allstimid);
        for(i = 0; i < ndots[0]; i++)
            fprintf(fd," %d", colors[i]);
        fprintf(fd,"\n");
    }
    else{
        fwrite(colors,sizeof(short),ndots[1], fd);
    }
    return(ndots[0]);
}


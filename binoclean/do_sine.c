#import <stdio.h>
#import "mymath.h"
#import <math.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import "commdefs.h"
#import "stimuli.h"
#import "sprotos.h"

#import <stdlib.h>

extern double gammaval;
extern Expt expt;
extern int option2flag,optionflag,optionflags[],testflags[];
extern int onlineversion;

OneStim *NewGrating(Stimulus *st, Substim *sst, Substim *copy)
{
    OneStim *gb;
    
    FreeStimulus(sst);  /* free up other stimulus-structure */
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
    gb = sst->ptr;
    gb->plaid_angle = 0;
    /*
     * force Xsampling to be 2 inititally, since there is a bug with 1
     * currently
     */
    st->pos.size[1] = st->pos.imsize[1]/2;
    init_grating(st, sst);
    st->type = STIM_GRATING;
    sst->type = STIM_GRATING;
    return(gb);
}

OneStim *NewGrating2(Stimulus *st, Substim *sst, Substim *copy)
{
    float sf;
    OneStim *gb;
    
    FreeStimulus(sst);  /* free up other stimulus-structure */
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
    gb = sst->ptr; 
    
    /*
     * force Xsampling to be 2 inititally, since there is a bug with 1
     * currently
     */
    st->pos.size[1] = st->pos.imsize[1]/2;
    init_grating(st, sst);
    st->pos.contrast2 = st->pos.contrast;
    st->type = STIM_GRATING2;
    sst->type = STIM_GRATING2;
    return(gb);
}

OneStim *NewGratingN(Stimulus *st, Substim *sst, Substim *copy)
{
    float sf;
    OneStim *gb;
    
    FreeStimulus(sst);  /* free up other stimulus-structure */
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
    gb = sst->ptr; 
    
    /*
     * force Xsampling to be 2 inititally, since there is a bug with 1
     * currently
     */
    st->pos.size[1] = st->pos.imsize[1]/2;
    st->rolldisp = 0;
    init_grating(st, sst);
    st->pos.contrast2 = st->pos.contrast;
    st->type = STIM_GRATINGN;
    sst->type = STIM_GRATINGN;
    if(st->nfreqs <=0){
        st->nfreqs = 4;
        st->freqs[0] = st->f;
        st->freqs[1] = sst->ptr->sf2;
        st->freqs[2] = (st->f+sst->ptr->sf2)/2;
        st->freqs[3] = 2 * sst->ptr->sf2 - st->f;
    }
    sst->autoscale = 1;
    if(sst->nfreqs <=0){
        sst->nfreqs = 4;
    }
    return(gb);
}

void free_grating(Substim *st)
{
    OneStim *ptr;
    
    /*
     if((ptr = (OneStim *)st->ptr) != NULL)
     {
     free(ptr);
     st->ptr = NULL;
     }
     */
}

void init_grating(Stimulus *st, Substim *sst)
{
    
	float *p;
	int i,j,w,h;
	float val,xval,yval,x,y,cval,cm,deg,sx,sy;
	Locator *pos = &sst->pos;
	OneStim *gb = sst->ptr;
    
	w = st->pos.size[0];
	h = st->pos.size[1];
	if(gb->sy < 0.01 || gb->sy > 20.0) /* error: left over memory*/
        gb->sy = 0.0;
    
    /*
     * if previous stimulus type was not a grating, we don't want to inherit
     * some arbitary values into the Grating Structure, so force plaid angle to 0
     */
    
	if(st->lasttype != STIM_GRATING && st->lasttype != STIM_GRATING2 || st->type == STIM_TEST)
        gb->plaid_angle = 0;
	if(h *2 > sst->imlen || sst->im == NULL) 
	{
        sst->imlen = (h+3) *2;
        if(sst->im != NULL)
            free(sst->im);
        sst->im = (float *)malloc(sst->imlen * sizeof(float));
	}
	if(h *2 > sst->imblen || sst->imb == NULL)
    {
	    sst->imblen = (h+3) *2;
	    if(sst->imb != NULL)
            free(sst->imb);
	    sst->imb = (float *)malloc(sst->imblen * sizeof(float));
    }
	memcpy(pos,&st->pos,sizeof(Locator));
}

float CalcLum(float *p, float f, float y, float phase, float lumscale, float background)
{
    float cval, yval;
    
    if( y != y) /* NaN*/
        return(0.5);
    cval = cos(2 * M_PI * f * y + phase);
    yval = (cval * lumscale);
    return((yval + background));
}

float SetLum(float *p, float f, float y, float phase, float lumscale, float background)
{
    float cval, yval;
    
    if( y != y) /* NaN*/
        return(0.5);
    cval = cos(2 * M_PI * f * y + phase);
    yval = (cval * lumscale);
    *p = dogamma((yval + background));
    //  *p = (yval + background);
    return(*p);
    
}

void calc_grating(Stimulus *st, Substim *sst, float disp)
{
    int i,j,h,partdisp;
    float cval,y,f,sy,cm,deg,iscale[2],pixdisp[2];
    float lumscale,ytest,lumbscale;
    float *p,*q,*cend,c1,c2,f2;
    
    int xval,yval,val,*yv,*xv,*end;
    vcoord x[2],z[2];
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    
    
    cval = 1.0;
    
    
    if(st->background > 0.005)
        lumscale = st->background * pos->contrast;
    else
        lumscale = 1.0;
    lumbscale = st->background * pos->contrast2;
    
    sy = deg_rad(gb->sy);
    if(sy > pos->radius[1] * 2 || sy < 0.01)
        sy = 0;
    pos->ss[1] = pos->imsize[1]/pos->size[1];
    if(pos->ss[1] <= 0 || pos->radius[1] < 1.1)
        pos->ss[1] = 1.0;
    for(i = 0; i < 2; i++)
    {
        deg = deg_rad(pix2deg(pos->ss[i] * pos->size[i]));
        iscale[i] = deg/pos->size[i];
    }
    
    /* s.f. is in cpd, so need to convert to cpradian */
    f2 = f = rad_deg(pos->sf);
    if (gb->sf2 > 0)
        f2 = rad_deg(gb->sf2);
    
    if(disp == 0)
        pixdisp[0] = pixdisp[1] = 0;
    else
    {
        pixdisp[0] = disp * cos(pos->angle)/pos->ss[0];
        pixdisp[1] = -disp * sin(pos->angle)/pos->ss[1];
    }
    p = sst->im;
    q = sst->imb;
    y = (double)(pixdisp[1] * iscale[1]) - deg_rad(pix2deg(pos->radius[1]));
    /* y is angular distance from centre, in radians */
    if(sy > 0) /* 1-D gabor */
    {
        ytest = exp(-sqr(y/sy));
        if(sst->ptr->plaid_angle > 0.1)
            SetLum(q++, f2, y, pos->phase2, lumbscale, st->background);
        SetLum(p, f, y, pos->phase, lumscale, st->background);
        p++;
        for(i = 0; i < pos->size[1]; i++,p++,q++)
        {
            y = (double)((pixdisp[1] + i - pos->size[1]/2)* iscale[1]);
            ytest = exp(-sqr(y/sy));
            SetLum(p, f, y, pos->phase, lumscale* ytest, st->background);
            if(sst->ptr->plaid_angle > 0.1)
                SetLum(q, f, y, pos->phase, lumbscale, st->background);
        }
        y = (double)(pixdisp[1] * iscale[1]) + deg_rad(pix2deg(pos->radius[1]));
        ytest = exp(-sqr(y/sy));
        SetLum(p, f, y, pos->phase, lumscale*ytest, st->background);
        if(sst->ptr->plaid_angle > 0.1)
            SetLum(q, f2, y, pos->phase2, lumbscale, st->background);
    }
    else
    {
        SetLum(p, f, y, pos->phase, lumscale, st->background);
        if(sst->ptr->plaid_angle > 0.1)
            SetLum(q++, f2, y, pos->phase2, lumbscale, st->background);
        p++;
        for(i = 0; i < pos->size[1]; i++,p++,q++)
        {
            
            y = (double)((pixdisp[1] + i - pos->size[1]/2)* iscale[1]);
            SetLum(p, f, y, pos->phase, lumscale, st->background);
            if(sst->ptr->plaid_angle > 0.1)
                SetLum(q, f2, y, pos->phase2, lumbscale, st->background);
        }
        y = (double)(pixdisp[1] * iscale[1]) + deg_rad(pix2deg(pos->radius[1]));
        /* y is angular distance from centre, in radians */
        SetLum(p, f, y, pos->phase, lumscale, st->background);
        if(sst->ptr->plaid_angle > 0.1)
            SetLum(q, f2, y, pos->phase2, lumbscale, st->background);
    }
    p = (sst->im+i);
}

calc_grating2(Stimulus *st, Substim *sst, float disp)
{
    int i,j,h,partdisp;
    float cval,y,f,sy,cm,deg,iscale[2],pixdisp[2],ytest;
    float lumscale,lumscale2;
    float *p,*q,*cend,c1,c2;
    
    int xval,yval,val,*yv,*xv,*end;
    short x[2],z[2];
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    
    cval = 1.0;
    
    lumscale = st->background * pos->contrast;
    lumscale2 = st->background * pos->contrast2;
    sy = deg_rad(gb->sy);
    if(sy > pos->radius[1] * 2)
        sy = 0;
    pos->ss[1] = pos->imsize[1]/pos->size[1];
    if(pos->ss[1] <= 0)
        pos->ss[1] = 1.0;
    for(i = 0; i < 2; i++)
    {
        deg = deg_rad(pix2deg(pos->ss[i] * pos->size[i]));
        iscale[i] = deg/pos->size[i];
    }
    
    /* s.f. is in cpd, so need to convert to cpradian */
    f = rad_deg(st->f);
    if(disp == 0)
        pixdisp[0] = pixdisp[1] = 0;
    else
    {
        pixdisp[0] = disp * cos(pos->angle)/pos->ss[0];
        pixdisp[1] = -disp * sin(pos->angle)/pos->ss[1];
    }
    p = sst->im;
    y = (double)(pixdisp[1] * iscale[1]) - deg_rad(pix2deg(pos->radius[1]));
    /* y is angular distance from centre, in radians */
    if(sy > 0)
        ytest = exp(-sqr(y/sy));
    else
        ytest = 1.0;
    
    c1 = CalcLum(p, f, y, pos->phase, lumscale * ytest, st->background);
    c2 =  CalcLum(p, rad_deg(gb->sf2), y, pos->phase2, lumscale2 * ytest, st->background);
    *p = dogamma((c1 + c2)/2);
    p++;
    
    for(i = 0; i < pos->size[1]; i++,p++)
    {
        y = (double)((pixdisp[1] + i - pos->size[1]/2)* iscale[1]);
        if(sy > 0)
            ytest = exp(-sqr(y/sy));
        else
            ytest = 1.0;
        c1 = CalcLum(p, f, y, pos->phase, (lumscale * ytest), st->background);
        c2 = CalcLum(p, rad_deg(gb->sf2), y, pos->phase2, (lumscale2 * ytest), st->background);
        *p = dogamma((c1+c2)/2);
    }
    y = (double)(pixdisp[1] * iscale[1]) + deg_rad(pix2deg(pos->radius[1]));
    /* y is angular distance from centre, in radians */
    if(sy > 0)
        ytest = exp(-sqr(y/sy));
    else
        ytest = 1.0;
    c1 = CalcLum(p, f, y, pos->phase, lumscale*ytest, st->background);
    c2 = CalcLum(p, rad_deg(gb->sf2), y, pos->phase2, lumscale2*ytest, st->background);
    *p = dogamma((c1+c2)/2);
    
    p = (sst->im+i);
}


int SetGratingFrequencies(Stimulus *st)
{
    double x,minf,maxf,f2;
    int i;
    
    if(st->freqmode == MANUAL_FREQ)
        return(0);
    
    f2 = st->left->ptr->sf2;
    if(expt.stimmode == RANDOM_ONOFF_LOG || expt.stimmode == INDEP_ONOFF_LOG
       || expt.stimmode == BINOC_INDEP_ONOFF_LOG){
        minf = st->freqs[0] = st->f + (f2 - st->f) * -(st->nfreqs)/2;
        maxf = st->freqs[st->nfreqs-1] = st->f + (f2 - st->f) * ((st->nfreqs-1) - (st->nfreqs)/2);
        x = pow(maxf/minf,1.0/(st->nfreqs-1));
        for(i = 1; i < st->nfreqs-1; i++){
            st->freqs[i] = st->freqs[i-1] * x;
        }
    }
    else{
        for(i = 0; i < st->nfreqs; i++){
            st->freqs[i] = st->f + (f2 - st->f) * (i - (st->nfreqs)/2);
        }
    }
}

double SetFreqOnOff433(Substim *sst, int nc)
{
    int i,j,nset;
    double tpower = 0,drnd;
    
/* 
 * RANDOM_ONOFF sets 50% of the freqs on, 50% off
 * RANDOM_ONOFF_LOG sets 50% of the freqs on, 50% off, Log spaces fresq
 * INDEP_ONOFF sets each component ON/Off with independent p = 0.5
 * INDEP_ONOFF_LOG does this with log spacing of freqs
 * BINOC_INDEP_ONOFF_LOG is randomly on/off indep in each eye
 */
 
    if(expt.stimmode == RANDOM_ONOFF || expt.stimmode == RANDOM_ONOFF_LOG){
        nset = 0;
        for(j = 0; j < nc; j++){
            sst->powers[j] = 0;
        }
        while(nset < nc/2){
            do{
                j = floor(drand48() * nc);
            }while(sst->powers[j] > 0);
            sst->powers[j] = 1.0;
            nset++;
        }
        tpower = nset;
    }
    else if(expt.stimmode == INDEP_ONOFF || expt.stimmode == INDEP_ONOFF_LOG 
            || expt.stimmode == BINOC_INDEP_ONOFF_LOG){
        for(j = 0; j < nc; j++){
            if((drnd = drand48()) > 0.5)
                sst->powers[j] = 1.0;
            else
                sst->powers[j] = 0.0;
            if(j == 4){
                drnd = drnd;
            }
        }
        tpower = nc/2;
    }
    else{
        tpower = 0;
        for(j = 0; j < nc; j++){
            if(expt.stimmode == RANDOM_POWER)
                sst->powers[j] = drand48();
            else
                sst->powers[j] = 1;
            tpower += sst->powers[j];
        }
    }
    return(tpower);
}

double SetFreqOnOff(Substim *sst, int nc)
{
    int i,j,nset;
    double tpower = 0,drnd,dc=0;
    unsigned long rnd,iseed;
    int bit, rem;
    
    
    if(expt.stimmode == RANDOM_ONOFF || expt.stimmode == RANDOM_ONOFF_LOG){
        iseed = myrnd_u();
        srand48(iseed);
        nset = 0;
        for(j = 0; j < nc; j++){
            sst->powers[j] = 0;
        }
        while(nset < nc/2){
            do{
                j = floor(drand48() * nc);
            }while(sst->powers[j] > 0);
            sst->powers[j] = 1.0;
            nset++;
        }
        tpower = nset;
    }
    else if(expt.stimmode == INDEP_ONOFF || expt.stimmode == INDEP_ONOFF_LOG 
            || expt.stimmode == BINOC_INDEP_ONOFF_LOG || expt.stimmode == BINOC_INDEP_ONOFF){
        rnd = myrnd_u();
        bit = 0;
        for(j = 0; j < nc; j++){
            if(rnd & (1L<<bit))
                sst->powers[j] = 1.0;
            else
                sst->powers[j] = 0.0;
            if(++bit > 31){
                bit = 0;
                rnd = myrnd_u();
            }
        }
        tpower = (double)(nc)/2;
    }
    else if(expt.stimmode == THREE_CONTRAST_LEVEL){
        rnd = myrnd_u();
        bit = 0;
        for(j = 0; j < nc; j++){
            rem = (rnd >> bit) % 3;
            if(rem > 1)
                sst->powers[j] = 1.0;
            else if(rem > 0)
                sst->powers[j] = 0.5;
            else
                sst->powers[j] = 0.0;
            bit = bit+8;
            if(bit > 31){
                bit = 0;
                rnd = myrnd_u();
            }
        }
        tpower = (double)(nc)/2;
    }
    else{
        tpower = 0;
        for(j = 0; j < nc; j++){
            if(expt.stimmode == RANDOM_POWER)
                sst->powers[j] = drand48();
            else
                sst->powers[j] = 1;
            tpower += sst->powers[j];
        }
    }
    if(expt.stimmode == BINOC_INDEP_ONOFF) 
    {
        /*
         *  taking the inverse FFT of this stimulus generates a set of
         * independent "pixels" whose size is determined by the nyquist frequency.
         * for nc components, this generates 2nc+1 independent pixels. However,
         * for the pixels to be truly independent, it is necessary to include 
         * DC fluctuations of appropriate magnigute. 
         * Each pixel is the sum of m random samples drawn from a sinusoidal
         * distribtion, where m is a random number (binomal distribution p=0.5, n = nc)
         * The DC fluctuation is then the mean of 2nc pixels like this 
         * (NOT 2nc+1 - 1 df is lost because the mean is forced to be zero before this DC is added)
         * So this is the single pixel value/sqrt(2nc);
         */
        dc = 0;
        for(j = 0; j < nc; j++){
            rnd = myrnd_u();
            if(rnd & (1L<<12))
                dc = dc + sin((rnd & 0xff) * M_PI/128);
        }
        sst->powers[nc] = dc/sqrt(nc * 2);
    }
    else 
        sst->powers[nc] = 0;
    return(tpower);
}

calc_gratingN(Stimulus *st, Substim *sst, float disp)
{
    int i,j,h,partdisp,nset = 0;
    float cval,y,f,sy,cm,deg,iscale[2],pixdisp[2],ytest;
    float lumscale,lumscale2,rescale,maxc = 0;
    float *p,*q,*cend,c1,c2,lum;
    int nc = sst->nfreqs > 0 ? sst->nfreqs : 1;
    int xval,yval,val,*yv,*xv,*end;
    short x[2],z[2];
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    double powers[MAXFREQS],tpower,drnd,/*drand48(),  ALI */drands[MAXFREQS];
    
    if(nc > MAXFREQS)
        nc = MAXFREQS;
    
    if(expt.stimmode == UNNORMALIZED)
        sst->autoscale = 0;
    else if(issfrc(expt.stimmode))
        sst->autoscale = 0;
    else
        sst->autoscale = 1;
    cval = 1.0;
    
    lumscale = st->background * pos->contrast;
    lumscale2 = st->background * pos->contrast2;
    rescale = 1;
    
    if(onlineversion > 444)
        myrnd_init(sst->seed);
    srand48(sst->seed);
    sy = deg_rad(gb->sy);
    if(sy > pos->radius[1] * 2)
        sy = 0;
    pos->ss[1] = pos->imsize[1]/pos->size[1];
    if(pos->ss[1] <= 0)
        pos->ss[1] = 1.0;
    for(i = 0; i < 2; i++)
    {
        deg = deg_rad(pix2deg(pos->ss[i] * pos->size[i]));
        iscale[i] = deg/pos->size[i];
    }
    
    /* s.f. is in cpd, so need to convert to cpradian */
    f = rad_deg(st->f);
    if(disp == 0)
        pixdisp[0] = pixdisp[1] = 0;
    else
    {
        pixdisp[0] = disp * cos(pos->angle)/pos->ss[0];
        pixdisp[1] = -disp * sin(pos->angle)/pos->ss[1];
    }
    p = sst->im;
    y = (double)(pixdisp[1] * iscale[1]) - deg_rad(pix2deg(pos->radius[1]));
    /* y is angular distance from centre, in radians */
    if(sy > 0)
        ytest = exp(-sqr(y/sy));
    else
        ytest = 1.0;

    if(st->freqmode == MANUAL_FREQ){
        tpower = 0;
        for (i = 0; i < nc; i++){
            sst->powers[i] = st->contrasts[i];
            tpower += sst->powers[i];
        }
    }
    else if(onlineversion < 434)
        tpower = SetFreqOnOff433(sst, nc);
    else
        tpower = SetFreqOnOff(sst, nc);
    
    c1 = SetLum(p, f, y, pos->phase, lumscale * ytest, st->background);
    c2 =  SetLum(p, rad_deg(gb->sf2), y, pos->phase2, lumscale2 * ytest, st->background);
    *p = (c1 + c2)/2;
    maxc = 0;
    lum = 0;
    for(j = 0; j < nc; j++){
        c1 = CalcLum(p, rad_deg(sst->freqs[j]), y, sst->phases[j], (lumscale * ytest), 0);
        lum += (c1 * sst->powers[j]);
    }
    *p = st->background + (lum+(sst->powers[nc] * lumscale))/tpower;
    p++;
    
    
    for(i = 0; i < pos->size[1]; i++,p++)
    {
        y = (double)((pixdisp[1] + i - pos->size[1]/2)* iscale[1]);
        if(sy > 0)
            ytest = exp(-sqr(y/sy));
        else if(pos->contrast > 1)
            ytest = pos->contrast;
        else
            ytest = 1.0;
        lum = 0;
        for(j = 0; j < nc; j++){
            c1 = CalcLum(p, rad_deg(sst->freqs[j]), y, sst->phases[j], (lumscale * ytest), 0);
            lum += (c1 * sst->powers[j]);
        }
        *p = st->background + (lum+(sst->powers[nc] * lumscale))/tpower;
        if(fabs(*p-0.5) > maxc)
            maxc = fabs(*p-0.5);
    }
    y = (double)(pixdisp[1] * iscale[1]) + deg_rad(pix2deg(pos->radius[1]));
    /* y is angular distance from centre, in radians */
    if(sy > 0)
        ytest = exp(-sqr(y/sy));
    else
        ytest = 1.0;
    
    *p = 0;
    lum = 0;
    for(j = 0; j < nc; j++){
        c1 = SetLum(p, sst->freqs[j], y, sst->phases[j], (lumscale * ytest),0);
        lum += (c1 * sst->powers[j]);
    }
    *p = st->background + (lum+(sst->powers[nc] * lumscale))/tpower;
    /*
     * if max > 0.5, then we must resscale since this will exceed the monitor range
     */
    if((sst->autoscale && maxc > 0) || maxc > 0.5){
        if(maxc > 0.5)
            expt.noverflow++;
        if(pos->contrast > 1)
            rescale = 1.0/(maxc * 2.01);
        else
            rescale = pos->contrast/(maxc * 2.01);
    }
    expt.ncalc++;
    p = sst->im;
    for(i = 0; i < pos->size[1]; i++,p++)
        *p = dogamma(((*p-0.5) * rescale)+0.5);
    
    
}


void paint_grating(Substim *sst, int mode, int shift)
{
    int i,ci =0,j,oi =0;
    float *p,*q,*cend,last;
    static float alpha = 1.0;
    float vcolor[3];
    vcoord x[2],z[2];
    Locator *pos = &sst->pos;
    OneStim *gb = (OneStim *)sst->ptr;
    double angle  = pos->angle * 180/M_PI;
    double xtest,vy,oldplaid,oldphase;
    
    
    
    if(pos->radius[0] < 0.5 || pos->radius[1] < 0.5) /* less than half a pixel long = don't paint */
        return;
    glDisable(GL_DITHER);
    glPushMatrix();
    
    glTranslatef((pos->xy[0]),pos->xy[1],0);
    angle  = (pos->angle+gb->plaid_angle/2) * 180/M_PI;
    glRotatef(angle,0,0,1);
    
    vcolor[0] = vcolor[1] = vcolor[2] = 0.5;
    
    oi = 2;
    if(mode == LEFTMODE)
    {
        ci = 0;
        oi = 1;
    }
    else if(mode == RIGHTMODE)
    {
        ci = 1;
        oi = 1;
    }
    else if(mode == BOTHMODE)
    {
        ci = 0;
        oi = 0;
    }
    
    /*
     * for 2grating,ngrating, other contrasts may be non-zero
     */
    if(fabsf(pos->contrast) < 0.0001 && sst->type == STIM_GRATING){
        x[1] = x[0] = 0;
        vcolor[0] = vcolor[1] = vcolor[2] = sst->gammaback;
       	glColor4f(vcolor[0],vcolor[1],vcolor[2], alpha);
        mycircle(0,0, pos->radius[0],30);	
        glPopMatrix();
        return;
    }
    if(fabs(sst->pos.sf) < 0.0001 && sst->type == STIM_GRATING){
        x[1] = x[0] = 0;
        if(testflags[TEST_RC] && pos->angle == 0)
            sst->lum[0] = sst->lum[1] = 0;
        else
            vcolor[0] = vcolor[1] = vcolor[2] = *(sst->im);
       	glColor4f(vcolor[0],vcolor[1],vcolor[2], alpha);
        mycircle(0,0, pos->radius[0],30);	
        glPopMatrix();
        return;
    }
    
    if(gb->plaid_angle < -0.1)
        p = sst->imb;
    else
        p = sst->im;
    
    q = p;
    if(pos->ss[1] <= 1.1) /* line spacing same as pixels */
    {
        if(optionflag & ANTIALIAS_BIT || fabs(gb->plaid_angle) > 0.1){  //need blending for plaids
            glLineWidth(1.1);
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else{
            /*
             * Need to set a wider line width to avoid holes. Doesn't matter becuase
             * the next line along covers anything that is too wide
             */
            glLineWidth(1.5);
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
        }
        glBegin(GL_LINES);
    }
    else
    {
        glBegin(GL_QUAD_STRIP);
    }
    /*
     * the color values calculated in calc_grating, via SetLum, have already 
     * been gamma corrected, so here we can just use glColor  calls
     */
    
    
    x[0] = z[0] = 0;
    x[1] = z[1] = -pos->radius[1];
    vcolor[ci] = *q;
    p++;
    vcolor[2] = vcolor[1];
    glColor4f(vcolor[0],vcolor[1],vcolor[2], alpha);
    myvx(x);
    myvx(x);
    
    /*
     for(i = 0; i < pos->size[1]; i++)
     {
     q = p;
     x[1] = (i - pos->size[1]/2) * pos->ss[1]; 
     z[1] = x[1];
     x[0] = pos->radius[0];
     z[0] = -x[0];
     vcolor[ci] = *p;
     vcolor[2] = vcolor[1];
     glColor4f(vcolor[0],vcolor[1],vcolor[2], alpha);
     myvx(x);
     vcolor[ci] = *q;
     vcolor[2] = vcolor[1];
     glColor4f(vcolor[0],vcolor[1],vcolor[2], alpha);
     myvx(z);
     p++;
     }
     glEnd();
     glPopMatrix();
     return;
     */
    
    if(optionflag & SQUARE_RDS)
    {
        x[0] = pos->radius[0];
        z[0] = -x[0];
        for(i = 0; i < pos->size[1]; i++)
        {
            x[1] = (i - pos->size[1]/2) * pos->ss[1]; 
            z[1] = x[1];
            if(optionflags[SPLITSCREEN])
                vcolor[2] = vcolor[1] = vcolor[0] = *p;
            else
            {
                vcolor[ci] = *p;
                vcolor[2] = vcolor[1] = vcolor[oi];
            }
            glColor4f(vcolor[0],vcolor[1],vcolor[2], alpha);
            myvx(x);
            myvx(z);
            if(*p > 1 || *p == last)
                last = 0;
            last = *p;
            p++;
        }
    }
    else{ // Circular patch
        for(i = 0; i < pos->size[1]; i++)
        {
            vy = (i - pos->size[1]/2) * pos->ss[1];
            xtest = pos->radius[0]*pos->radius[0] * (1 - (vy*vy)/(pos->radius[1] * pos->radius[1]));
            q = p;
            if(xtest > 0)
            {
                xtest = sqrt(xtest);
                x[1] = ((float)i - pos->size[1]/2) * pos->ss[1]; 
                z[1] = x[1];
                x[0] = -xtest;
                z[0] = -x[0];
                if(optionflags[SPLITSCREEN])
                    vcolor[2] = vcolor[1] = vcolor[0] = *p;
                else
                {
                    vcolor[ci] = *p;
                    vcolor[2] = vcolor[1] = vcolor[oi];
                }
                glColor4f(vcolor[0],vcolor[1],vcolor[2], alpha);
                myvx(x);
                myvx(z);
            }
            p++;
        }
        x[0] = z[0] = 0;
        x[1] = z[1] = pos->radius[1];
        
        if(optionflags[SPLITSCREEN])
            vcolor[2] = vcolor[1] = vcolor[0] = *p;
        else
        {
            vcolor[ci] = *p;
            vcolor[2] = vcolor[1];
        }
        glColor4f(vcolor[0],vcolor[1],vcolor[2], alpha);
        myvx(x);
        myvx(x);
    }
    
    glEnd();
    
    glPopMatrix();
    
    
    if(gb->plaid_angle > 0.01)
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        alpha = 0.5;
        oldplaid = gb->plaid_angle;
        gb->plaid_angle = -gb->plaid_angle;
        paint_grating(sst, mode, shift);
        glDisable(GL_BLEND);
        gb->plaid_angle = oldplaid;
        alpha = 1.0;
    }
}

int SaveNSines(Stimulus *st, FILE *fd)
{
    Locator *pos = &st->pos;
    vcoord offset[2],xp[5000],yp[5000],disps[5000];
    int *p,d,*end,i;
    vcoord  w,h,*x,*y,fw,fh,*pdisp;
    float *fp;
    Substim *sst = st->left;
    short colors[5000];
    int ndots[2];
    char name[BUFSIZ];
    static int firstcall = 1;
    int fplaces = 0;
    FILE *tfd;
    
    
    
    
    if(fd == NULL){
        fprintf(stderr,"Can't append nsines %s\n",name);
        return(0);
    }
    
    if(optionflags[RANDOM_PHASE] == 0){ // must be  a test
        printf("Pixel step %.5f deg\n",(pix2deg(st->pos.ss[1] * st->pos.size[1]))/st->pos.size[1]);
        tfd = fopen("Nsines.lum","a");
        fp = st->left->im;
        for(i = 0; i < st->left->pos.size[1]; i++,fp++)
            fprintf(tfd,"%.3f ",*fp);
        fprintf(tfd,"\n");
        fp = st->right->im;
        for(i = 0; i < st->right->pos.size[1]; i++,fp++)
            fprintf(tfd,"%.3f ",*fp);
        fprintf(tfd,"\n");
        fclose(tfd);
    }
    if(firstcall && expt.stimmode == BINOC_INDEP_ONOFF){
        
        fprintf(fd,"NaN\n");
    }
    if(expt.stimmode == THREE_CONTRAST_LEVEL)
        fplaces = 1;
    
    fprintf(fd,"%d",st->left->baseseed);
    for(i = 0; i < st->nfreqs; i++)
    {
        fprintf(fd," %.*f",fplaces,st->left->powers[i]);
    }
    fprintf(fd,"\n");
    fprintf(fd,"%d",expt.allstimid);
    for(i = 0; i < st->nfreqs; i++)
    {
        fprintf(fd," %.0f",st->left->phases[i] * 180/M_PI);
    }
    fprintf(fd,"\n");
    if(st->flag & UNCORRELATE || expt.stimmode == BINOC_INDEP_ONOFF){
        if(expt.stimmode == BINOC_INDEP_ONOFF_LOG || expt.stimmode == THREE_CONTRAST_LEVEL || expt.stimmode == BINOC_INDEP_ONOFF){
            if(expt.stimmode == BINOC_INDEP_ONOFF)
                fprintf(fd,"%.3f",st->right->powers[st->nfreqs]);
            else
                fprintf(fd,"Inf");
            for(i = 0; i < st->nfreqs; i++)
                fprintf(fd," %.*f",fplaces,st->right->powers[i]);
            fprintf(fd,"\n");
        }
        if(expt.stimmode == BINOC_INDEP_ONOFF)
            fprintf(fd,"%.3f",st->left->powers[st->nfreqs]);
        else
            fprintf(fd,"NaN");
        for(i = 0; i < st->nfreqs; i++)
            fprintf(fd," %d",(int)rint((st->left->phases[i]-st->right->phases[i]) * 180/M_PI)%360);
        fprintf(fd,"\n");
    }
    firstcall = 0;
    return(st->nfreqs);
}

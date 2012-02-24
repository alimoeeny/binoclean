#import <stdio.h>
#import "mymath.h"
#import <math.h>
#import "commdefs.h"
#import "stimuli.h"
#import "sprotos.h"
#define FTOFRAC 65536.0
#define TOOBIG 1000

static float lumscale = 127.0;
static int colorshift = 6;
static int splitgabor = 0;

OneStim *NewGabor(Stimulus *st, Substim *sst, Substim *copy)
{
	OneStim *gb;
    
	FreeStimulus(sst);  /* free up other stimulus-structure */
	if(sst->ptr == NULL)
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
	gb = sst->ptr; 
	gb->plaid_angle = 0;
    
    
	gb->im = NULL;
	gb->carray = NULL;
	gb->xpos = NULL;
	gb->ypos = NULL;
	gb->xbell = NULL;
	gb->sx = 2.0;
	gb->sy = 2.0;
	st->pos.size[1] = 80;
	st->pos.size[0] = 20;
	st->type = STIM_GABOR;
	sst->ptr = gb;
	init_gabor(st, sst);
	sst->type = STIM_GABOR;
	return(gb);
}

void free_gabor_ptr(OneStim *ptr)
{
    
    if(ptr != NULL)
    {
        free(ptr->xbell);
        ptr->xbell = NULL;
        free(ptr->xpos);
        ptr->xpos = NULL;
        free(ptr->ypos);
        ptr->ypos = NULL;
        free(ptr->carray);
        ptr->carray = NULL;
    }
}

void free_gabor(Substim *st)
{
	free_gabor_ptr((OneStim *)st->ptr);
}

void init_gabor(Stimulus *st,  Substim *sst)
{
	Locator *pos = &sst->pos;
	OneStim *gb = (OneStim *)sst->ptr;
	short *p;
	int i,j,w,h;
	double val,xval,yval,x,y,cval,cm,deg,sx,sy,disp;
	float iscale[2];
    
	w = st->pos.size[0];
	h = st->pos.size[1];
    
	lumscale = st->background * pos->contrast;
    
	/* need new memory */
	if(w * h >  pos->size[0] * pos->size[1] || gb->im == NULL)
	{
		if(gb->im != NULL)
			free(gb->im);
		gb->im = (float *)malloc((w+1) * (h+1) * sizeof(float));
		if(gb->xpos != NULL)
            free(gb->xpos);
		if(gb->ypos != NULL)
            free(gb->ypos);
		gb->ypos = (vcoord *)malloc((w+1) * (h+1) * sizeof(vcoord));
		gb->xpos = (vcoord *)malloc((w+1) * (h+1) * sizeof(vcoord));
	}
	if(w > pos->size[0] || gb->xbell == NULL)
	{
		if(gb->xbell != NULL)
			free(gb->xbell);
		gb->xbell = (int *)malloc((w+1) * sizeof(float));
	}
	if(h > pos->size[1] || gb->carray == NULL)
	{
		if(gb->carray != NULL)
			free(gb->carray);
		gb->carray = (int *)malloc((h+GAUSSEXTRA) * sizeof(int));
	}
    /*
     * above code checks for differences b/w st->pos and gb->pos to
     * expand memory requirements etc. so dont copy the parameters 
     * until the end
     */
    
	if(pos->ss[0] * pos->size[0] < pos->radius[0] *2)
    {
        pos->radius[0] = st->pos.radius[0] = (pos->ss[0] * pos->size[0])/2;
	}
	memcpy(pos,&st->pos,sizeof(Locator));
}

void calc_gabor(Stimulus *st, Substim *sst,  float disp)
{
	Locator *pos = &sst->pos;
	OneStim *gb = (OneStim *)sst->ptr;
	int i,j;
	float cval,y,f,sy,cm,deg,iscale[2],pixdisp[2],x,sx,ratio,of,ophase;
	float *p,*q,*cend, vx,vy,vfy,*xv,*yv,*end,back,rowval,rowprime,lastvx;
	float *parray = NULL,ov,op,ostep,xprime,cosa,sina,ycos;
	vcoord *vpx,*vpy,h,ih;
	int xval;
	double xtest,ytest,yval,val,cprime;
    
    
	back = st->background;
	lumscale = st->background * pos->contrast;
	colorshift = 0;
    
	if(disp == 0)
		pixdisp[0] = pixdisp[1] = 0;
	else
	{
		pixdisp[0] = disp * cos(pos->angle)/pos->ss[0];
		pixdisp[1] = -disp * sin(pos->angle)/pos->ss[1];
	}
	sx = deg_rad(gb->sx);
    
	cval = 1.0;
	/* s.f. is in cpd, so need to convert to cpradian */
	for(i = 0; i < 2; i++)
	{
		pos->imsize[i] = pos->size[i] * pos->ss[i];
		deg = deg_rad(pix2deg(pos->ss[i] * pos->size[i]));
		iscale[i] = deg/pos->size[i];
	}
    
	for(i = 0; i <= pos->size[0]; i++)
	{
		x = (double)((pixdisp[0] + i - (float)pos->size[0]/2)) * iscale[0];
		cval = exp(-sqr(x/sx));
		gb->xbell[i] = cval;
	}
	f = rad_deg(st->f);
	if(sst->ptr->plaid_angle > 0.1){
        cosa = cos(sst->ptr->plaid_angle);
        sina = iscale[0] * sin(sst->ptr->plaid_angle)/pos->ss[0];
        of = rad_deg(st->f)  * sin(sst->ptr->plaid_angle);
        parray = (float *)(malloc(sizeof(float) * pos->size[0]  *2));
        ostep = 2 * M_PI * deg2pix(f) * iscale[0] * cos(sst->ptr->plaid_angle);
	}
	else{
        of = 1;
	}
	sy = deg_rad(gb->sy);
    
	for(i = 0; i < pos->size[1]; i++)
		gb->carray[i] = TOOBIG;
    /*
     * make sure there is always a vertex at the extreme
     * top of the bounding ellipse
     */
    
	h =(vcoord)(pos->size[1])/2 - (pos->radius[1]/pos->ss[1])-1;
	if(h < 0)
		h = 0;
	op = 0;
	for(i = ih = h; ih < pos->size[1]-h; ih += 1, i++)
	{
        
		vpx = (gb->xpos + (i * pos->size[0]));
		vpy = (gb->ypos + (i * pos->size[0]));
        
        
		vy = (ih - (vcoord)(pos->size[1]-1)/2) * pos->ss[1];
		vx = (-pos->size[0]/2) * pos->ss[0];
		ophase = vx * sin(sst->ptr->plaid_angle)/f;
        
        
		/* Calculate which samples lie within the bounding ellipse */
		xtest = pos->radius[0]*pos->radius[0] * (1 - (vy*vy)/(pos->radius[1] * pos->radius[1]));
		if(xtest > 0)
		{
            /* 
             * calculate no. samples needed to cover inside 
             */
			xtest = sqrt(xtest);
			j = (int)floor(xtest/(pos->ss[0]));
			gb->carray[i] = (j > pos->size[0]/2) ? 0 : (pos->size[0]/2-j);
			y = ((double)(pixdisp[1]  - pos->radius[1]/pos->ss[1])* iscale[1]);
		}
		else
		{
			xtest = 0;
			gb->carray[i] = (vcoord)pos->size[0]/2;
			if(i > pos->size[1]/2)
                vy = pos->radius[1];
			else
                vy = -pos->radius[1];
			y = ((double)(pixdisp[1] + i - pos->size[1]/2)* iscale[1]);
		}
        /* y is angular distance from centre, in radians */
        
        vfy = vy;
        y = ((double)(pixdisp[1] + vy/pos->ss[1])* iscale[1]);
        ycos = y * cosa;
        cval = cos(2 * M_PI * f * y + pos->phase);
        if(splitgabor)
            cprime = cos(2 * M_PI * f * y + pos->phase2);
        else
            cprime = cval;
        ytest = exp(-sqr(y/sy));
        rowval = yval = ytest * lumscale;
        rowval =  ytest * cval * lumscale;
        rowprime =  ytest * cprime * lumscale;
        
        xv  = gb->xbell + gb->carray[i];
        end = xv + 2 * (pos->size[0]/2 -gb->carray[i]);
        vx = (gb->carray[i] - (float)pos->size[0]/2) * pos->ss[0];
        p = (gb->im + (i * pos->size[0]));
        
        /* 
         * set first vertex in the list so that it lies on the
         * bounding ellipse
         */
        if(xtest == 0)
        {
            j = pos->ss[1] * (int)(pos->radius[1]/pos->ss[1]);
            vfy = (pos->radius[1] + j)/2;
            xtest = pos->radius[0]*pos->radius[0] * (1 - (vfy*vfy)/(pos->radius[1] * pos->radius[1]));
            xtest = sqrt(xtest);
            *vpx = -xtest;
            if(i < pos->size[1]/2)
                vfy = -vfy;
            *vpy = vfy;
            y = ((double)(pixdisp[1] + vfy/pos->ss[1])* iscale[1]);
            ytest = exp(-sqr(y/sy));
            yval = (cval * ytest * lumscale);
            val = (yval * *xv) + back;
            //	    *p = dogamma(val);
            *p = val;
        }
        else
	    {
            *vpx = -xtest;
            *vpy = vy;
            vfy = vy;
            val = (yval * *xv) + back;
            //	    *p = dogamma(val);
            *p = val;
	    }
        /* now move to the first interior vertex */
        vpx += gb->carray[i];
        vpy += gb->carray[i];
        p += gb->carray[i];
        
        
        /* then set interior vetices */
        for(; xv <=end; xv++)
        {
            
            *vpx++ = vx;
            *vpy++ = vy;
            if(vx >= 0 && lastvx < 0){
                if(splitgabor){
                    yval = rowprime;
                    val = (yval * *xv) + back;
                    //	    *p = dogamma(val);
                    *p = val;
                    p++;
                    *vpx++ = vx+1;
                    *vpy++ = vy;
                }
                yval = rowval;
            }
            else if(vx > 0)
                yval = rowval;
            else
                yval = rowprime;
            lastvx = vx;
            vx +=  pos->ss[0];
            
            /* calculate greylevel */
            if(parray){
                xprime = ycos + vx * sina;
                ov  = sin((M_PI *2 * f * xprime)) * lumscale * ytest;
                val = ((yval+ov) * *xv)/2 + back;
            }
            else
                val = (yval * *xv) + back;
            //	    *p = dogamma(val);
            *p = val;
            p++;
        }
        /* set last vertex */
        xv--;
        vpx = (gb->xpos + (i+1) * pos->size[0]) -1;
        vpy = (gb->ypos + (i+1) * pos->size[0]) -1;
        p = (gb->im + ((i+1) * pos->size[0])-1);
        val = (yval * *xv) + back;
        *p = val;
        *vpx = xtest;
        *vpy = vfy;
        val = (yval * *xv) + back;
        *p = val;
    }
	if(parray)
        free(parray);
}


void paint_gabor_pair(Stimulus *st)
{
	Locator *pos = &st->pos;
	int i;
	float *pl,*ql,*endl;
	float *pr,*qr,*endr;
	float xl[2],zl[2];
	float vcolor[3];
	float angle;
	OneStim *gbl = (OneStim *)st->left->ptr;
	OneStim *gbr = (OneStim *)st->right->ptr;
    
    
	/*  stencil(1, 0x1, SF_EQUAL, 0x1, ST_KEEP, ST_KEEP, ST_KEEP);*/
	glPushMatrix();
    
	glTranslatef(pos->xy[0],pos->xy[1],0);
	angle = (float) (pos->angle * 1800.0/M_PI);
	glRotatef(angle,0.0,0.0,1.0);
	endl = gbl->im + (pos->size[0]);
	vcolor[2] = 0;
	for(i = 0; i < pos->size[1]; i++)
	{
		if(gbl->carray[i] < TOOBIG)
		{
			xl[1] = (i - pos->size[1]/2) * pos->ss[1]; /*  +pos->xy[0] */
			zl[1] = xl[1]-pos->ss[1];
			xl[0] = zl[0] = (gbl->carray[i] - pos->size[0]/2) * pos->ss[0];
			pl = (gbl->im + (i * pos->size[0]));
			pr = (gbr->im + (i * pos->size[0]));
			endl = pl + (pos->size[0]) - gbl->carray[i];
			pl += gbl->carray[i];
			pr += gbl->carray[i];
			if(i == 0)
			{
				ql = pl;
				qr = pr;
			}
			else
			{
				ql = pl- pos->size[0];
				qr = pr-pos->size[0];
			}
			glBegin(GL_QUAD_STRIP);
			for(; pl < endl; pl++,pr++)
            {
                vcolor[0] = *ql;
                vcolor [2] = vcolor[1] = *qr;
                mycolor(vcolor);
                myvx(zl);
                vcolor[0] = *pl;
                vcolor[2] = vcolor[1];
                mycolor(vcolor);
                myvx(xl);
                xl[0] += pos->ss[0];
                zl[0] = xl[0];
                ql++;
                qr++;
            }
            
			glEnd();
		}
	}
	glPopMatrix();
}

/*
 void paint_gabor_lines(Substim *ssl, Substim  *gbr)
 {
 Locator *pos = &gbl->pos;
 int i;
 float *pl,*ql,*endl;
 float *pr,*qr,*endr;
 float xl[2],zl[2];
 float vcolor[3];
 
 
 cpack(0x7f7f7f7f);
 glClear(GL_COLOR_BUFFER_BIT);
 vcolor[2] = 0;
 for(i = 0; i < pos->size[1]; i++)
 {
 xl[1] = (i - pos->size[1]/2) * pos->ss[1];
 zl[1] = xl[1]-pos->ss[1];
 xl[0] = zl[0] = -(pos->size[0] * pos->ss[0])/2;
 pl = (gbl->im + (i * pos->size[0]));
 pr = (gbr->im + (i * pos->size[0]));
 if(i == 0)
 {
 ql = pl;
 qr = pr;
 }
 else
 {
 ql = (gbl->im + ((i-1) * pos->size[0]));
 qr = (gbr->im + ((i-1) * pos->size[0]));
 }
 endl = pl + (pos->size[0]);
 glBegin(GL_LINE);
 for(; pl <= endl; pl++,pr++)
 {
 vcolor[0] = *pr;
 vcolor[1] = *pl;
 mycolor(vcolor);
 myvx(xl);
 xl[0] += pos->ss[0];
 }
 }
 }
 */
void colorvertex(float *x, float *z, float *p, float *q)
{
    
}

void paint_gabor(Stimulus *st, Substim *sst, int mode, int shift)
{
    
	Locator *pos = &sst->pos;
	OneStim *gb = (OneStim *)sst->ptr;
	int i,ci,offset;
	float *p,*q;
	vcoord vx,vy,*vpx,*vpy,*vpoy,*vpox,*yc,*xc;
	float vcolor[3],*cc,*rc,*end;
	vcoord x[2],z[2],ih;
	float angle;
    
	vcolor[0] = vcolor[1] = vcolor[2] = 0;
	glPushMatrix();
	glTranslatef(pos->xy[0],pos->xy[1],0);
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
	for(i = 1; i < pos->size[1]; i++)
	{
		if(gb->carray[i] >=0)
		{
			x[1] = (i - (float)pos->size[1]/2) * pos->ss[1]; /*  +pos->xy[0] */
			z[1] = x[1]-pos->ss[1];
			x[0] = z[0] = (gb->carray[i] -(float)pos->size[0] /2)* pos->ss[0];
            
			if(gb->carray[i-1] > gb->carray[i])
				offset = gb->carray[i-1];
			else
				offset = gb->carray[i];
			if(offset < TOOBIG)
			{
				p = gb->im + (i * pos->size[0]);
				vpx = gb->xpos + (i * pos->size[0]);
				vpy = gb->ypos + (i * pos->size[0]);
                
				/*
                 * dont use im + (i-1) * size, 
                 * as this would omit the effect of carray
                 */
				vpox = vpx - pos->size[0];
				vpoy = vpy - pos->size[0];
				q = p - pos->size[0];
                
                
				glBegin(GL_QUAD_STRIP);
				xc = &x[0];
				yc = &z[0];
                
				/* set up 1st vertex */
				x[0] = *vpx;
				x[1] = *vpy;
				z[0] = *vpox;
				z[1] = *vpoy;
				if(st->colormode == COLORRGB)
				{
					*rc = *cc = *p;
					mycolor(vcolor);
					myvx(x);
					*rc = *cc = *q;
					mycolor(vcolor);
					myvx(z);
				}
				vpx += offset;
				vpy += offset;
				p += offset;
				end = p + 2 * (pos->size[0]/2 - offset);
				vpox = vpx - pos->size[0];
				vpoy = vpy - pos->size[0];
				q = p - pos->size[0];
				if(st->colormode == COLORRGB)
				{
					for(; p <= end; p++)
					{
						x[0] = *vpx++;
						x[1] = *vpy++;
						z[0] = *vpox++;
						z[1] = *vpoy++;
						*rc = *cc = *p;
						mycolor(vcolor);
						myvx(x);
						*rc = *cc = *q;
						mycolor(vcolor);
						myvx(z);
						/*	    *xc += pos->ss[0];
                         *yc = *xc;*/
						q++;
					}
				}
				/* set last vertex */
				p = gb->im + ((i+1) * pos->size[0])-1;
				vpx = gb->xpos + ((i+1) * pos->size[0])-1;
				vpy = gb->ypos + ((i+1) * pos->size[0])-1;
                
				/*
                 * dont use im + (i-1) * size, 
                 * as this would omit the effect of carray
                 */
				vpox = vpx - pos->size[0];
				vpoy = vpy - pos->size[0];
				q = p - pos->size[0];
				x[0] = *vpx;
				x[1] = *vpy;
				z[0] = *vpox;
				z[1] = *vpoy;
				if(st->colormode == COLORRGB)
				{
					*rc = *cc = *p;
					mycolor(vcolor);
					myvx(x);
					*rc = *cc = *q;
					mycolor(vcolor);
					myvx(z);
				}
				glEnd();
			}
		}
	}
	glPopMatrix();
}


void optimize_gabor(Stimulus *st, Substim *sst)
{
    OneStim *gb = (OneStim *)sst->ptr;
    float period, sample[2],cycles;
    Locator *pos = &st->pos;
    
    if(pos->radius[0] <= 0)
        pos->radius[0] = ceil(deg2pix(5 * gb->sx));
    if(pos->radius[1] <= 0)
        pos->radius[1] = ceil(deg2pix(4 * gb->sy));
    
    
	period = deg2pix(1/st->f);
	sample[0] = sqrt(period) * 2.0;
	/* optimum samples per period */
	sample[1] = sqrt(period) * 1.1;
    
	/* set up number of samples */
	pos->size[0] = (int)ceil(sample[0]);
	cycles = pos->radius[1]/period;
	if(pos->ss[1] > 1.1){
        pos->size[1] = ceil(cycles * sample[1]);
        pos->ss[1] = rint(2 * pos->radius[1]/pos->size[1]);
        pos->imsize[1] = pos->ss[1] * pos->size[1];
	}
	else{
        pos->size[1] = 2 * pos->radius[1];
        pos->ss[1] = 1;
        pos->imsize[1] = pos->ss[1] * pos->size[1];
	}
    
    
	/* finally set up appropriate integer magnification factor */
	pos->ss[0] = rint(2 * pos->radius[0]/pos->size[0]);
    
    pos->imsize[0] = pos->ss[0] * pos->size[0];
	init_gabor(st, sst);
}

#import <stdio.h>
#import <sys/time.h>
#import <sys/types.h>
#import <stdarg.h>
#import <math.h>
#import "mymath.h"
#import "miscw.h"
#import <OpenGL/OpenGL.h>
#import "fpslider.h"
#import <unistd.h>
#import "commdefs.h"
#import "stimuli.h"
#import "WinDefs.h"
#import "sprotos.h"
#import "protos.h"

#define myseed rnd_init
#define myrand rnd_i

//Ali
#define NOEVENT 1


extern int verbose,newmonoc;
extern int testflags[];
extern float **rclfreqs,**rcrfreqs;
extern int **rcdps;

static vcoord vlines[20] = { -10.8, -8.1, -5.4, -2.7, 0, 2.7, 5.4, 8.1, 10.8,0};
static int nvlines = 9;
static vcoord hlines[20] = { -10.8, -8.1, -5.4, -2.7, 0, 2.7, 5.4, 8.1, 10.8,0};
static int nhlines = 9;
double gammaval = 1.0;

static int bytes = 0,bytes2 = 0;
static unsigned long *backrect = NULL,*backrect2 = NULL;
static Rectangle oldrect,oldrect2;
static int lastflag = 0;
static /* Ali GC */ int BackGC;

extern int debug;
extern float clearcolor;
extern int realframecount;
extern int thebuffer,testint,mode,clearcnt;
extern Monitor mon;
extern /* Ali Display */ int D;
extern int optionflag, bothmask, option2flag,cycmask,alphamask;
extern int winsiz[],fixpos[],optionflags[];
extern Expt expt;
extern vcoord psychoff[];
extern vcoord conjpos[],oldpos[];
vcoord lastpos[2];
extern float cmarker_size;
extern OneStim *thecorrug;
extern int freezestimulus;
int nrndcalls = 0;



/*
 * As of version 4.31, gamma correction now all done outside binoc,
 * with /usr/sbin/gamma
 */
float dogamma(float cindex)
{
  float cval;

  return(cindex);
  if(cindex >1.0) /* an old 0-255 spec */
    cval = 255.0 * pow((double)(cindex)/255.0,1.0/gammaval);
  else
    cval = pow((double)(cindex),1.0/gammaval);
  return (cval);
}

void setmask(int type)
{
  int cyc;
  static int lastmask = -1000;
    return;
    
  if(optionflags[STEREO_GLASSES]){
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    return;
  }
  if(optionflags[STIMULUS_IN_OVERLAY])
    cyc = GL_TRUE;
  else
    cyc = GL_FALSE;
  if(type == 0 && lastmask)
    type = lastmask;
  lastmask = type;

  switch(type)
    {
    case ALLPLANES:
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      break;
    case OVERLAY:
      if(optionflags[STIMULUS_IN_OVERLAY])
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      else
	glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE);
      break;
    case BOTHMODE:
      if(optionflag & LEFT_FIXATION_CHECK)
	glColorMask(GL_TRUE, cyc, GL_FALSE, GL_TRUE);
      else if(optionflag & RIGHT_FIXATION_CHECK)
	glColorMask(GL_FALSE, cyc, GL_TRUE, GL_TRUE);
      else
	glColorMask(GL_TRUE, cyc, GL_TRUE, GL_TRUE);
      break;
    case FORCEBOTH:
	glColorMask(GL_TRUE, cyc, GL_TRUE, GL_TRUE);
      break;
    case FORCERIGHT:
	glColorMask(GL_FALSE, cyc, GL_TRUE, GL_TRUE);
      break;
    case FORCELEFT:
	glColorMask(GL_TRUE, cyc, GL_FALSE, GL_TRUE);
      break;
    case ALLMODE:
      if(optionflag & LEFT_FIXATION_CHECK)
	glColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_TRUE);
      else if(optionflag & RIGHT_FIXATION_CHECK)
	glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
      else
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      break;
    case RIGHTMODE:
      if(optionflag & LEFT_FIXATION_CHECK) 
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      else if(optionflags[SPLITSCREEN])
	glColorMask(GL_TRUE, cyc, GL_TRUE, GL_TRUE);
      else
	glColorMask(GL_FALSE, cyc, GL_TRUE, GL_TRUE);
      break;
    case LEFTMODE:
      if(optionflag & RIGHT_FIXATION_CHECK) 
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      else if(optionflags[SPLITSCREEN])
	glColorMask(GL_TRUE, cyc, GL_TRUE, GL_TRUE);
      else
	glColorMask(GL_TRUE, cyc, GL_FALSE, GL_TRUE);
      break;
    }
}
float pix2deg(float val)
{
	float res;
	res = atan(val * mon.pix2cms[0]/mon.viewd);
	return(rad_deg(res));
}

float pix2degy(float val)
{
	float res;
	res = atan(val * mon.pix2cms[1]/mon.viewd);
	return(rad_deg(res));
}

float deg2pix(float val)
{
	float res = mon.viewd * tan(deg_rad(val));
/*
 * Angles > 85 require very large tanential displacements!!
 * and >90 are not physically possible, leading to errors.
 */
	if(val > 85)
	  res = mon.viewd * 11.43;
	return(res * mon.cms2pix[0]);
}

float deg2pixy(float val)
{
	float res = mon.viewd * tan(deg_rad(val));
	return(res * mon.cms2pix[1]);
}

Stimulus *NewStimulus(Stimulus *st)
{
  Stimulus *new = (Stimulus *)malloc(sizeof(Stimulus));
  if(st != NULL)
  {
    memcpy(new,st,sizeof(Stimulus));
    st->next = new;
  }
  else
  {
    memset(new,0,sizeof(Stimulus));
  }
  new->stimversion = 0;
  new->next = NULL;
  new->prev = st;
  new->left = (Substim *)malloc(sizeof(Substim));
  new->left->id[0] = 'L';
    
  new->left->ptr = 0;
  new->left->pos.perimeter = 0;
  new->right = (Substim *)malloc(sizeof(Substim));
  new->right->ptr = 0;
  new->right->pos.perimeter = 0;
  new->right->id[0] = 'R';
  new->last = (Substim *)malloc(sizeof(Substim));
  new->last->ptr = 0;
  new->type = -1;
  new->corrmix = -1;
  new->left->im = new->left->xpos = new->left->ypos = NULL;
  new->right->im = new->right->xpos = new->right->ypos = NULL;
  new->right->imb = new->left->imb = NULL;
  new->right->imc = new->left->imc = NULL;
  new->left->ypl = new->right->ypl = 0;
  new->left->xpl = new->right->xpl = 0;
  new->right->imclen = new->left->imclen = 0;
  new->right->imblen = new->left->imblen = 0;
  new->right->uimage = new->left->uimage = NULL;
  new->right->uimlen = new->left->uimlen = 0;
  new->colormode = COLORRGB;
  new->imprefix = NULL;
  new->contrast_disp = 0;
  new->dwidth = 0;
  new->dlength = 0;
  new->dotrpt = 1;
  new->dotfrac = 0.5;
  if(new->prev == NULL){
    new->right->id[1] = new->left->id[1] = '0';
  }
  else{
    new->right->id[1] = new->left->id[1] = '1';
  }
  return(new);
}

void draw_cross(vcoord *pos, vcoord fixw)
{
  vcoord x[2];

  glBegin(GL_LINES);
    x[0] = fixw +pos[0];
    x[1] = pos[1];
    myvx(x);
    x[0] = pos[0]-fixw;
    myvx(x);
    x[1] = pos[1]+fixw;
    x[0] = pos[0];
    myvx(x);
    x[1] = pos[1]-fixw;
    myvx(x);
  glEnd();
  glLineWidth(1);
}

void draw_pos(vcoord *pos, int fixw, float color)
{
  static vcoord lastpos[2] = {512};
  short vcolor[3];

  if(lastpos[0] != pos[0] || lastpos[1] != pos[1])
    {
  vcolor[0] = vcolor[1] = vcolor[2] = 0;
  mycolor(vcolor);
  glDrawBuffer(GL_FRONT_AND_BACK);
  draw_cross(lastpos, fixw);
/*  glRects(lastpos[0]-fixw,lastpos[1]-fixw,lastpos[0]+fixw,lastpos[1]+fixw);*/
  vcolor[0] = vcolor[1] = vcolor[2] = color;
  mycolor(vcolor);
  draw_cross(pos, fixw);
  lastpos[0] = pos[0];
  lastpos[1] = pos[1];
  glDrawBuffer(GL_BACK);
   }
}

void draw_fix_square(vcoord fixw)
{
  short x[2],off[2],l,r;
  float vcolor[3];
  int i;

      l = (short)(-fixw/2);
      r = l + (short)(fixw);
      //      glRects(l,l,+r,+r);
      if(optionflag & ANTIALIAS_BIT)
	{
	  glLineWidth(fixw);
	  glEnable(GL_LINE_SMOOTH);
	  glEnable(GL_BLEND);
	  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  glBegin(GL_LINES);
	  x[0] = i;
	  x[1] = -fixw/2;
	  myvx(x);
	  x[1] = fixw/2;
	  myvx(x);
	  glEnd();
	  glDisable(GL_LINE_SMOOTH);
	  glDisable(GL_BLEND);
	}
      else{
	   glRects(l,l,+r,+r);
      }
      if(optionflags[FIXPT_GRATING])
	{
	  glLineWidth(1);
	  vcolor[0] = vcolor[1] = vcolor[2] = 0.5;
	  mycolor(vcolor);
	  glBegin(GL_LINES);
	  for(i = -fixw/2; i < fixw/2; i+= 2){
	    x[0] = i;
	    x[1] = -fixw/2;
	    myvx(x);
	    x[1] = fixw/2;
	    myvx(x);
	  }
	    glEnd();
	}
}

void draw_conjpos(vcoord fixw, float color)
{
  vcoord x[2],off[2];
  float linw;
  short vcolor[3];
  float fixwin;



  if(fixw <= 0)
    return;
  linw = 1.0;
  glLineWidth(2.0);
  setmask(OVERLAY);
  vcolor[0] = vcolor[1] = vcolor[2] = expt.st->gammaback;
  mycolor(vcolor);
  setmask(OVERLAY);
 glBegin(GL_LINES);
    x[0] = oldpos[0] + fixw/2;
    x[1] = oldpos[1] + fixw/2;
    myvx(x);
    x[0] = oldpos[0] -(fixw/2);
    x[1] = oldpos[1] - fixw/2;
    myvx(x);
    x[1] = oldpos[1] +fixw/2;
    myvx(x);
    x[1] = oldpos[1] -(fixw/2);
    x[0] = oldpos[0] + fixw/2;
    myvx(x);
  glEnd();
  SetColor(color, 0);
 glBegin(GL_LINES);
    x[0] = conjpos[0] + fixw/2;
    x[1] = conjpos[1] + fixw/2;
    myvx(x);
    x[0] = conjpos[0] -(fixw/2);
    x[1] = conjpos[1] - fixw/2;
    myvx(x);
    x[1] = conjpos[1] +fixw/2;
    myvx(x);
    x[1] = conjpos[1] -(fixw/2);
    x[0] = conjpos[0] + fixw/2;
    myvx(x);
  glEnd();

  glLineWidth(1);

  if(expt.bwptr->cflag & SHOW_FIXWIN){
  SetColor(1.0-color, 0);
 glBegin(GL_LINE_STRIP);
  fixwin = deg2pix(expt.fw);
    x[0] = fixpos[0] + fixwin;
    x[1] = fixpos[1] + fixwin;
    myvx(x);
    x[0] = fixpos[0] -(fixwin);
    myvx(x);
    x[1] = fixpos[1] - fixwin;
    myvx(x);
    x[0] = fixpos[0] + fixwin;
    myvx(x);
    x[1] = fixpos[1] + fixwin;
    myvx(x);
  glEnd();
  }
  if(optionflags[MOVE_RF])
    ShowBox(expt.rf,0);
  oldpos[0] = conjpos[0];
  oldpos[1] = conjpos[1];
  if(optionflags[MOVE_RF])
    ShowBox(expt.rf,RF_COLOR);
  oldpos[0] = lastpos[0];
  oldpos[1] = lastpos[1];
  lastpos[0] = conjpos[0];
  lastpos[1] = conjpos[1];
}

void draw_fix_cross(float fixw)
{
  vcoord x[2],off[2];
  float linw;


  if(fixw <= 0)
    return;
  if((linw = deg2pix(expt.vals[FIX_LINW])) < 1.0)
    linw = 1.0;
  glLineWidth(linw);
 glBegin(GL_LINES);
    x[0] = 1 + fixw/2;
    x[1] = 0;
    myvx(x);
    x[0] = -(fixw/2);
    myvx(x);
    x[1] = 1 +fixw/2;
    x[0] = 0;
    myvx(x);
    x[1] = -(fixw/2);
    myvx(x);
  glEnd();
  glLineWidth(1);
}


void draw_fix(vcoord px, vcoord py, vcoord fixw, float color)
{

  vcoord x[2],off[2],fixp[2],w;
  float vcolor[3],linw;
  short l,r;

  fixp[0] = px;
  fixp[1] = py;

  glPushMatrix();
  glTranslatef(fixp[0],fixp[1],0);
  if(expt.vals[FIXATION_SURROUND] > 0)
    {
      vcolor[0] = vcolor[1] = vcolor[2] = expt.st->gammaback;
      mycolor(vcolor);
      w = (deg2pix(expt.vals[FIXATION_SURROUND])/2);
      mycirc(0,0,w);
    }
  else if(fabs(dogamma(color) - expt.st->gammaback) < 0.01){
    glPopMatrix();
    return;
  }

  if(optionflags[NONIUS_LINES]){
    vcolor[0] = vcolor[1] = vcolor[2] = dogamma(1-color);
    /*  vcolor[1] = expt.overlay_color;*/
    mycolor(vcolor);
    if((linw = deg2pix(expt.vals[FIX_LINW])) < 1.0)
      linw = 2.0;
    glLineWidth(linw);
    setmask(LEFTMODE);
    glBegin(GL_LINES);
    x[1] = fixw;
    x[0] = 0;
    myvx(x);
    x[1] = 2 * fixw;
    myvx(x);
    glEnd();
    setmask(RIGHTMODE);
    glBegin(GL_LINES);
    x[1] = -fixw;
    x[0] = 0;
    myvx(x);
    x[1] = -2 * fixw;
    myvx(x);
    glEnd();
  }

  if(optionflags[VVNONIUS_LINES]){
    vcolor[0] = vcolor[1] = vcolor[2] = dogamma(1-color);
    /*  vcolor[1] = expt.overlay_color;*/
    mycolor(vcolor);
    if((linw = deg2pix(expt.vals[FIX_LINW])) < 1.0)
      linw = 2.0;
    glLineWidth(linw);
    setmask(LEFTMODE);
    glBegin(GL_LINES);
    x[0] = fixw;
    x[1] = 0;
    myvx(x);
    x[0] = 2 * fixw;
    myvx(x);
    glEnd();
    setmask(RIGHTMODE);
    glBegin(GL_LINES);
    x[0] = -fixw;
    x[1] = 0;
    myvx(x);
    x[0] = -2 * fixw;
    myvx(x);
    glEnd();
  }


  if(optionflags[BINOCULAR_FIXPOINT]){
    setmask(ALLPLANES);
  }
  else{
    if(expt.vals[FIXPT_EYE] > 0.5 && !(optionflag & LEFT_FIXATION_CHECK))
      setmask(RIGHTMODE);
    else if(expt.vals[FIXPT_EYE] < -0.5 && !(optionflag & RIGHT_FIXATION_CHECK))
      setmask(LEFTMODE);
    else
      setmask(BOTHMODE);
  }


  vcolor[0] = vcolor[1] = vcolor[2] = dogamma(color);
  /*  vcolor[1] = expt.overlay_color;*/
  mycolor(vcolor);
  if(optionflags[SPLITSCREEN])
       glTranslatef(-psychoff[0]/2,0,0);
  if(optionflag & SQUARE_FIXATION){
    if((px = deg2pix(expt.vals[FIXPT_DISPARITY])) != 0
     || (py = deg2pix(expt.vals[VERTICAL_VERGENCE])) != 0){
      if(expt.vals[FIXPT_EYE] <= 0){
	setmask(LEFTMODE);
	glTranslatef(-px/2,-py/2,0);
	draw_fix_square(fixw);
	glTranslatef(px/2,py/2,0); 
      }
      if(expt.vals[FIXPT_EYE] >= 0){
	setmask(RIGHTMODE);
	glTranslatef(px/2,py/2,0); 
	draw_fix_square(fixw);
	glTranslatef(-px/2,-py/2,0);
      }
    }
    else
      draw_fix_square(fixw);
  }
  else{
    if((px = deg2pix(expt.vals[FIXPT_DISPARITY])) != 0
     || (py = deg2pix(expt.vals[VERTICAL_VERGENCE])) != 0){
      if(expt.vals[FIXPT_EYE] <= 0){
	setmask(LEFTMODE);
	glTranslatef(-px/2,-py/2,0);
	draw_fix_cross(fixw);
	glTranslatef(px/2,py/2,0); 
      }
      if(expt.vals[FIXPT_EYE] >= 0){
	setmask(RIGHTMODE);
	glTranslatef(px/2,py/2,0); 
	draw_fix_cross(fixw);
	glTranslatef(-px/2,-py/2,0);
      }
    }
    else{
      draw_fix_cross(fixw);
    }
  }
  if(optionflags[SPLITSCREEN])
    {
       glTranslatef(psychoff[0],psychoff[1],0);
  if(optionflag & SQUARE_FIXATION)
    draw_fix_square(fixw);
  else
    draw_fix_cross(fixw);
    }
 
  if(optionflags[BINOCULAR_FIXPOINT]){
    setmask(BOTHMODE);
  }
  glPopMatrix();
  if(optionflag & SHOW_CONJUG_BIT)
    draw_conjpos(cmarker_size, 1.0);

}


void setgamma(double val)
{
  short tab[256];
  int i;

  gammaval = val;
  /*
   * Versions after 4.3, gamma correction done outside binoc
   */
  gammaval = 1.0;
/*  for(i = 0; i < 256; i++)
    tab[i] = (255.0 * pow(i/255.0,1.0/val)) + 0.5;
  gammaramp(tab,tab,tab);
*/
}

void SetColor(float cindex, int correct)
{
  float vcolor[3];
  float cval;

  if(correct)
    cval = dogamma(cindex);
  else
    cval = cindex;
  glColor4f(cval,cval,cval,cval);
}

void WipeBackRect(Stimulus *st, int dispflag, float color)
{
  Rectangle *rectptr;

  if(thebuffer)
    rectptr = &oldrect2;
  else
    rectptr = &oldrect;
  SetColor(color,0);
  glRects(rectptr->lx,rectptr->ly,rectptr->rx,
	 rectptr->ry);
  if(rdspair(st) || corrugpair(st) || sqcorrugpair(st))
    memcpy(rectptr,&st->next->rect,sizeof(Rectangle));
  else
    memcpy(rectptr,&st->rect,sizeof(Rectangle));
}

unsigned long *CheckBackrect(int w, int h, int flag)
{
  int i;

  i = (w+1) * (h+1);
  if(flag && (backrect2 == 0 || i > bytes2))
	{
	  if(backrect2 != 0)
	    {
	    free(backrect2);
	    backrect2 = 0;
	  }
	  bytes2 = i;
       backrect2 = (unsigned long *)malloc((bytes2 * sizeof(unsigned long)));
	  return(backrect2);
       }
  else if(backrect == 0 || i > bytes)
	{
	  if(backrect != 0)
	    {
	    free(backrect);
	  }
	  bytes = i;
       backrect = (unsigned long *)malloc((bytes * sizeof(unsigned long)));
	  return(backrect);
       }
  return(flag ? backrect2 : backrect);

}


void test_mem()
{
	    free(backrect2);
       backrect2 = (unsigned long *)malloc((bytes2 * sizeof(unsigned long)));
	    free(backrect);
       backrect = (unsigned long *)malloc((bytes * sizeof(unsigned long)));
}




/*
* clear_screen wipes the screen with color backgound,
* then re-paints the backgroudn stimulus
*/
int clear_screen(Stimulus *st, int flag)
{
  float vcolor[4],cindex;
  int c,w,h,zflag = 0;

  setmask(ALLPLANES);
    vcolor[0] = vcolor[1] = vcolor[2] = vcolor[3] = clearcolor;
    if(!(optionflag & FRAME_ONLY_BIT))
      vcolor[1] = st->gammaback;
    if(!optionflags[GREY_MONOC]){
    if(optionflag & LEFT_FIXATION_CHECK)
      vcolor[RIGHT] = 0;
    else if(optionflag & RIGHT_FIXATION_CHECK)
      vcolor[LEFT] = 0;
    bothmask = BOTHMODE;
    }
    else
      bothmask = FORCEBOTH;
    
    mycolor(vcolor);

 glClearColor(vcolor[0],vcolor[1],vcolor[2],1.0);
 glClear(GL_COLOR_BUFFER_BIT);
 glFlushRenderAPPLE();
 glFinishRenderAPPLE();
 setmask(BOTHMODE);

  /* if fixation marker is off, clear the screen blank - don't draw
   * backgroudn stimulus
   */
  if(st->fix.state == FIX_IS_OFF)
    return(0);
  if(st->next != NULL && st->next->type != STIM_NONE && optionflags[BACKGROUND_ALWAYS])
   {
     st->next->mode = st->mode;
   	w = st->rect.rx - st->rect.lx;
   	h = st->rect.ry - st->rect.ly;

       calc_stimulus(st->next);
      paint_stimulus(st->next);
 }
  if(st->mode & DRAW_FIX_BIT){
    draw_fix(fixpos[0],fixpos[1],st->fix.size, st->fixcolor);
  }
  return(0);
}

void FreeStimulus(Substim *st)
{
  if(st->ptr == 0)
    return;
/*
  if(st->im != NULL){
//    free(st->im);
    st->im = NULL;
  }
*/
  switch(st->type)
  {
    case STIM_GRATING:
    case STIM_GRATING2:
    case STIM_GRATINGN:
      free_grating(st);
      break;
    case STIM_GABOR:
      free_gabor(st);
      break;
    case STIM_SQUARE:
      free_square(st);
      break;
    case STIM_BAR:
      free_bar(st);
      break;
    case STIM_TWOBAR:
    case STIM_ANNULUS:
     case STIM_ANNULUSPLUS:
      free_bar(st);
      break;
    case STIM_PROBE:
      free_probe(st);
      break;
    case STIM_RDS:
    case STIM_CHECKER:
      free_rds(st);
      break;
    case STIM_RLS:
      free_rls(st);
      break;
    case STIM_CORRUG:
      free_corrug(st);
      break;
    case STIM_SQCORRUG:
      free_sqcorrug(st);
      break;  
    case STIM_CYLINDER:	/*j*/
      free_cylinder(st);
      break;
    case STIM_NONE:
      break;
  }
}

int StimulusType(Stimulus *st, int i)
{
  int last;
  float val;
  OneStim *sql,*sqr;

  if(i >= MAXSTIMULUSTYPES)
    return(-1);
  if(i != st->type) switch(i)
  {
    case STIM_IMAGE:
      st->right = NewImage(st,st->right);
      st->left = NewImage(st,st->left);
      st->mode &= (~STIMULUS_NEEDS_CLEAR); //New stim covers old one;
      st->nimseed = 1000;
	  if(st->prev != NULL && st->prev->type == STIM_IMAGE){
		  st->preloaded = st->prev->preloaded;
	      st->imprefix = st->prev->imprefix;
	      st->preload = st->prev->preload;
	      st->imprefix = st->prev->imprefix;
	      st->immode = st->prev->immode;
	      st->left->calculated = 0;
	  }
      break;
    case STIM_GABOR:
      st->right->ptr = NewGabor(st,st->right,st->last);
      st->left->ptr = NewGabor(st,st->left,st->last);
      st->mode &= (~STIMULUS_NEEDS_CLEAR);
      break;
    case STIM_SQUARE:
      st->right->ptr = NewSquare(st,st->right);
      st->left->ptr = NewSquare(st,st->left);
      break;
    case STIM_PROBE:
      st->right->ptr = NewProbe(st,st->right);
      st->left->ptr = NewProbe(st,st->left);
      break;
    case STIM_ANNULUS:
     case STIM_ANNULUSPLUS:
  case STIM_TWOBAR:
    case STIM_BAR:
      st->mode |= STIMULUS_NEEDS_CLEAR;
    case STIM_NONE:
      st->right->ptr = NewBar(st,st->right);
      st->left->ptr = NewBar(st,st->left);
      break;
    case STIM_SUBGRATING1:
    case STIM_SUBGRATING2:
    case STIM_GRATING:
      i = STIM_GRATING;
      st->right->ptr = NewGrating(st,st->right,st->last);
      st->left->ptr = NewGrating(st,st->left, st->last);
      st->preload = 0; // only should be used for image
      break;
    case STIM_GRATING2:
      st->right->ptr = NewGrating2(st,st->right,st->last);
      st->left->ptr = NewGrating2(st,st->left, st->last);
        val = StimulusProperty(st, SF);
	val *= 1.33333333;
	SetStimulus(st,val, SF2, NULL);
      break;
    case STIM_GRATINGN:
      st->right->ptr = NewGratingN(st,st->right,st->last);
      st->left->ptr = NewGratingN(st,st->left, st->last);
      break;
    case STIM_TEST:
      NewRds(st,st->left,st->last);
      NewRds(st,st->right,st->last);
      st->left->seedloop = 1;
      st->right->seedloop = 1;
      st->mode |= STIMULUS_NEEDS_CLEAR;
      break;
    case STIM_RDSSINE:
      NewRds(st,st->left,st->last);
      NewRds(st,st->right,st->last);
      st->left->seedloop = 1;
      st->right->seedloop = 1;
      st->mode |= STIMULUS_NEEDS_CLEAR;
      break;
    case STIM_RLSSINE:
      NewRls(st,st->left,st->last);
      NewRls(st,st->right,st->last);
      st->left->seedloop = 1;
      st->right->seedloop = 1;
      st->mode |= STIMULUS_NEEDS_CLEAR;
      break;
    case STIM_CHECKER:
    case STIM_RDS:
      if(verbose)
	printf("New RDS\n");
      if(i == STIM_CHECKER){
	NewChecker(st,st->left,st->last);
	NewChecker(st,st->right,st->last);
      }
      else{
	NewRds(st,st->left,st->last);
	NewRds(st,st->right,st->last);
      }
      st->mode |= STIMULUS_NEEDS_CLEAR;
      st->pos.contrast_phase = 0.0;
/* 
   if makeing a new RDS and it is a background, and the foreground is
   RDS, then use foreground props to set up background
*/
      if(st->prev != NULL && rdspair(st->prev))
	{
	  val = 1.0 + StimulusProperty(st->prev, STIM_WIDTH);
	  SetStimulus(st, val, STIM_WIDTH, NOEVENT);
	  SetStimulus(st, val, STIM_HEIGHT, NOEVENT);
	  val = StimulusProperty(st->prev, SETCONTRAST);
	  SetStimulus(st, val, SETCONTRAST, NOEVENT);
	  st->pos.contrast_phase = 0;
	  val = StimulusProperty(st->prev, SF);
	  SetStimulus(st, val, SF, NOEVENT);
	  st->flag = st->prev->flag;
	  val = StimulusProperty(st->prev,CORRELATION);
	  SetStimulus(st, val, CORRELATION, NOEVENT);
	  expt.stimvals[BACK_CORRELATION] = val;
	}
      st->preload = 0; // only should be used for image
      break;
    case STIM_RLS:
      NewRls(st,st->left,st->last);
      NewRls(st,st->right,st->last);
      st->mode |= STIMULUS_NEEDS_CLEAR;
      st->pos.contrast_phase = 0.0;
      st->preload = 0; // only should be used for image
/* 
   if makeing a new RDS and it is a background, and the foreground is
   RDS, then use foreground props to set up background
*/
      if(st->prev != NULL && rlspair(st->prev))
	{
	  val = 1.0 + StimulusProperty(st->prev, STIM_WIDTH);
	  SetStimulus(st, val, STIM_WIDTH, NOEVENT);
	  val = StimulusProperty(st->prev, STIM_HEIGHT);
	  SetStimulus(st, val, STIM_HEIGHT, NOEVENT);
	  val = StimulusProperty(st->prev, SETCONTRAST);
	  SetStimulus(st, val, SETCONTRAST, NOEVENT);
	  st->pos.contrast_phase = 0;
	  val = StimulusProperty(st->prev, SF);
	  SetStimulus(st, val, SF, NOEVENT);
	  st->flag = st->prev->flag;
	}
      break;
    case STIM_CORRUG:
      st->left->ptr = NewCorrug(st,st->left,st->last);
      st->right->ptr = NewCorrug(st,st->right,st->last);
      st->mode |= STIMULUS_NEEDS_CLEAR;
      st->pos.contrast_phase = 0.0;
      if(st->prev == NULL) 
	thecorrug = st->left->ptr;
      break;
    case STIM_SQCORRUG:
      sql = st->left->ptr = NewSqcorrug(st,st->left,st->last);
      sqr = st->right->ptr = NewSqcorrug(st,st->right,st->last);
      st->mode |= STIMULUS_NEEDS_CLEAR;
      st->pos.contrast_phase = 0.0;
      break; 
    case STIM_CYLINDER:
      st->left->ptr = NewCylinder(st,st->left,st->last);        /* only need to initialise one but do both for later modification */
      st->right->ptr = NewCylinder(st,st->right,st->last);        /* only need to initialise one but do both for later modification */
      if(st->prev != NULL && st->prev->type == STIM_CYLINDER){
	st->left->ptr->dotsiz[0] = st->prev->left->ptr->dotsiz[0];
	st->left->ptr->dotsiz[1] = st->prev->left->ptr->dotsiz[1];
	st->right->ptr->dotsiz[0] = st->prev->right->ptr->dotsiz[0];
	st->right->ptr->dotsiz[1] = st->prev->right->ptr->dotsiz[1];
	st->left->ptr->velocity = st->prev->left->ptr->velocity;
	st->right->ptr->velocity = st->prev->right->ptr->velocity;
	st->left->ptr->density = st->prev->left->ptr->density;
	st->right->ptr->density = st->prev->right->ptr->density;
	if(optionflag & BACKGROUND_FIXED_BIT){
	  SetStimulus(st,StimulusProperty(st->prev,STIM_SIZE),STIM_SIZE,NULL);
	}
      }
      st->mode |= STIMULUS_NEEDS_CLEAR;
      break;
    case STIM_RADIAL:
      i = STIM_RADIAL;
      st->right->ptr = NewRadial(st,st->right,st->last);
      st->left->ptr = NewRadial(st,st->left, st->last);
      st->mode |= STIMULUS_NEEDS_CLEAR;
      st->pos.contrast_phase = 0.0;
      break;
   }
  else
    return(1);
  st->lasttype = st->type;
  st->type = i;
  st->right->mode = RIGHTMODE;
  st->left->mode = LEFTMODE;
  init_stimulus(st);
  return(0);
}

void wipestim(Stimulus *st, int color)
{
  Locator *pos = &st->pos;

  short xp[2];
  float angle;
  float disp;

  angle = (pos->lastangle * 180.0/M_PI);
  disp = fabs(st->disp);
  glPushMatrix();
  glTranslatef(pos->lastxy[0], pos->lastxy[1], 0);
  glRotatef(angle,0,0,1);
  SetColor(color,0);

  glBegin(GL_POLYGON);
  xp[0] = - pos->imsize[0]/2 - disp;
  xp[1] = - pos->imsize[1]/2 - disp;
  myvx(xp);
  xp[1] += (pos->imsize[1]  + disp *2);
  myvx(xp);
  xp[0] += (pos->imsize[0] + disp *2);
  myvx(xp);
  xp[1] -= (pos->imsize[1] + disp *2);
  myvx(xp);
  glEnd();
  glPopMatrix();
  
}

void clearstim(Stimulus *st, float color, int drawfix)
{

/* just wipes the stimlus area clear to color */

  clearcnt++;
  if(st->mode & STIMULUS_NEEDS_CLEAR || mode & NEED_REPAINT || mode & NEED_REPAINT_AGAIN || testflags[TEST_RC])
    {
/*
 * if change to monocular in an RC sequence, need to do a binocular wipe screen
 * befoe setting the mask
 */
      if(newmonoc){
	optionflag &= (~(LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
	setmask(0);
      }
      if(expt.backim.name){
	PaintBackIm(expt.backim);
      }
      else if(option2flag & WIPE_WHOLE_SCREEN)
	{
	 glClearColor(color, st->gammaback, color, 1.0);
         glClear(GL_COLOR_BUFFER_BIT);
	}
      else
	WipeBackRect(st,st->mode,color);
      if(newmonoc)
	optionflag |= newmonoc;
    }

      if(mode & NEED_REPAINT)
	{
	  mode &= (~NEED_REPAINT);
	  mode |= NEED_REPAINT_AGAIN;
	}
      else
	  mode &= (~NEED_REPAINT_AGAIN);

  if(drawfix && st->prev == NULL)
    draw_fix(fixpos[0],fixpos[1],st->fix.size, st->fixcolor);
  if(expt.vals[GRIDSIZE] > 0.1){
    wipescreen(clearcolor);
      }
}

void init_stimulus(Stimulus *st)
{
  st->pos.phase = deg_rad(st->vals[START_PHASE]);
  //  printf("Init type %d\n",st->type);
  switch(st->type)
    {
    case STIM_GABOR:
       init_gabor(st,st->left);
       init_gabor(st,st->right);
       break;
    case STIM_GRATING:
    case STIM_GRATINGN:
    case STIM_GRATING2:
       init_grating(st,st->left);
       init_grating(st,st->right);
       break;
    case STIM_TEST:
       init_grating(st,st->right);
       init_rds(st,st->left,0.0);
       if(expt.vals[CONTRAST_RATIO]== 0)
	 expt.vals[CONTRAST_RATIO] = 1;
       break;
    case STIM_RDSSINE:
       init_grating(st,st->left);
       init_rds(st,st->right,0.0);
       init_grating(st,st->right);
       init_rds(st,st->left,0.0);
       if(expt.vals[CONTRAST_RATIO]== 0)
	 expt.vals[CONTRAST_RATIO] = 1;
       break;
    case STIM_RLSSINE:
       init_grating(st,st->left);
       init_rls(st,st->right,0.0);
       init_grating(st,st->right);
       init_rls(st,st->left,0.0);
       if(expt.vals[CONTRAST_RATIO]== 0)
	 expt.vals[CONTRAST_RATIO] = 1;
       break;
    case STIM_CHECKER:
    case STIM_RDS:
       init_rds(st,st->left,0.0);
       init_rds(st,st->right,0.0);
       break;
    case STIM_RLS:
       init_rls(st,st->left,0.0);
       init_rls(st,st->right,0.0);
       break;
    case STIM_CORRUG:
       init_corrug(st,st->left,0.0);
       init_corrug(st,st->right,0.0);
       break;
    case STIM_SQCORRUG:
      init_sqcorrug(st,st->left,0.0);
      init_sqcorrug(st,st->right,0.0);
      break;   
   case STIM_CYLINDER:	    /*j*/
       init_cylinder(st,st->left); 	       
       break;  
    case STIM_RADIAL:
       init_radial(st,st->left);
       init_radial(st,st->right);
       break;
    case STIM_BAR:
       init_bar(st,st->left);
       init_bar(st,st->right);
       break;
      case STIM_NONE:
    case STIM_ANNULUS:
     case STIM_ANNULUSPLUS:
    case STIM_TWOBAR:
     case STIM_SQUARE:
     case STIM_PROBE:
       break;
    }
}

void calc_stimulus(Stimulus *st)
{
  OneStim *gb;
  Grating *gr,*gl;
  Substim *rds,*rdsr,*rdsnext,*lstim,*rstim;
  OneStim *corrug,*corrugr;
  OneStim *sqcorrug,*sqcorrugr;
  Bar *bar;
  Substim *sst;
  float disp = 0.0;
  Locator *pos = &st->pos, left, right;
  int w,h,i,mid;
  int flag = st->flag,mode;
  double val,scale,rnd;
  int dps[MAXFREQS],ndots,backdone = 0;
  
/*
 * this should not be necessary. Call at the end should do this
  if(st->next != NULL && optionflags[PAINT_BACKGROUND])
    calc_stimulus(st->next);
 */

  st->right->gammaback = st->left->gammaback = st->gammaback;

  lstim = st->left;
  rstim = st->right;

  switch(st->type){
  case STIM_CHECKER:
  case STIM_RDS:
  case STIM_RLS:
/* 
 * Width and Length Disparities not allowed for this stimulus
 */
    st->dlength = 0;
    st->dwidth = 0;
    break;
	/*  case STIM_IMAGE:
	      st->sf_disp = 0;
	*/
  }


  if(fabs(pos->phase) > 100)
    pos->phase = 0;
  while(pos->phase > (2 * M_PI))
      pos->phase -= (M_PI * 2);
  memcpy(&st->left->pos, &st->pos, sizeof(Locator));
  memcpy(&st->right->pos, &st->pos, sizeof(Locator));

  /* reset those things which are allowed to be different */
  st->left->pos.xy[0] = st->pos.xy[0] + st->disp;
  st->right->pos.xy[0] = st->pos.xy[0] - st->disp;
  st->left->pos.xy[1] = st->pos.xy[1] + st->vdisp;
  st->right->pos.xy[1] = st->pos.xy[1] - st->vdisp;

	 st->left->pos.imsize[1] = 2* st->pos.radius[1] + deg2pix(st->dwidth)/2;       
	 st->right->pos.imsize[1] = 2 * st->pos.radius[1] - deg2pix(st->dwidth)/2;
	 st->left->pos.imsize[0] = 2* st->pos.radius[0] + deg2pix(st->dlength)/2;       
	 
	 st->right->pos.imsize[0] = 2 * st->pos.radius[0] - deg2pix(st->dlength)/2;
	 st->left->pos.radius[1] =  st->pos.radius[1] + deg2pix(st->dwidth)/4;       
	 st->right->pos.radius[1] = st->pos.radius[1] - deg2pix(st->dwidth)/4;
	 st->left->pos.radius[0] =  st->pos.radius[0] + deg2pix(st->dlength)/4;       
	 st->right->pos.radius[0] = st->pos.radius[0] - deg2pix(st->dlength)/4;
/*
       st->left->pos.size[0] = st->left->pos.imsize[0];
       st->right->pos.size[0] = st->right->pos.imsize[0];
       st->left->pos.size[1] = st->left->pos.imsize[1];
       st->right->pos.size[1] = st->right->pos.imsize[1];
*/

       st->left->pos.contrast = st->pos.contrast + st->contrast_disp;
       st->right->pos.contrast = st->pos.contrast - st->contrast_disp;
       st->left->mixdots = st->right->mixdots = st->mixdots;

       if(isdotstim(st)){
	 st->left->pos.sf = st->pos.sf;
	 st->right->pos.sf =  st->pos.sf;
	 st->left->pos.angle = st->pos.angle;
	 st->right->pos.angle = st->pos.angle; 
       }
       else{
	 st->left->pos.sf = st->pos.sf + st->sf_disp;
	 st->right->pos.sf =  st->pos.sf - st->sf_disp;
	 st->left->pos.angle = st->pos.angle + st->ori_disp;
	 st->right->pos.angle = st->pos.angle - st->ori_disp;
       }


    switch(st->type)
    {
    case STIM_PROBE:
    case STIM_ANNULUS:
     case STIM_ANNULUSPLUS:
    case STIM_TWOBAR:
    case STIM_BAR:
       st->left->pos.xy[0] += disp;
       st->right->pos.xy[0] -= disp;
      break;
    case STIM_IMAGE:
      if(optionflags[FAST_SEQUENCE]){
	if(expt.vals[DISTRIBUTION_WIDTH] > 7)
	  st->nimseed = 100;
	else
	  st->nimseed = 200;
      }
      else
	st->nimseed = 1000;
      calc_image(st, st->left);
      calc_image(st, st->right);
      break;
    case STIM_SQUARE:
       st->left->pos.xy[0] += disp;
       st->right->pos.xy[0] -= disp;
      break;
    case STIM_GRATINGN:
      rnd_init(st->left->baseseed+1000);
      if(flag & UNCORRELATE){
	st->right->seed = st->right->baseseed = st->left->baseseed+1;
	st->left->seed = st->left->baseseed;
      }
      else
	st->left->seed = st->right->seed = st->left->baseseed;

       st->left->nfreqs = st->nfreqs;
       st->right->nfreqs = st->nfreqs;
       mid = st->nfreqs/2;
/*
 * only 1*pi here since it is added to one eye and subtracted from the other
 * initially used phasedisp[2], and made all components have equivalent physical
 * disparty to the middle component. 
 *	 scale = st->freqs[i]/st->freqs[mid]; inside loop
 */
       scale = M_PI * st->rolldisp;
       for(i = 0; i < st->nfreqs; i++){
	 st->left->freqs[i] = st->freqs[i];
	 st->right->freqs[i] = st->freqs[i];
	 if(flag & UNCORRELATE){
/*
 * rnd is set 0 - 180, since is is added to left, and removed from right 
 * if issfrc, components are coming on and off at random. Want this to be the
 * same in each eye, so that can look at phase tuning. So, use same seed
 * if BINOC_INDEP_ONOFF_LOG then freqs are present or not independently in
 * the two eyes. This allows monoc tuning curves, and binoc ones (from the trials where both are present).
*/
	   if(issfrc(expt.stimmode) && expt.nstim[0] > 1 && expt.mode == DISP_P){
	     dps[i] = (myrand()  % expt.nstim[0]);
	     rnd =  dps[i] *  M_PI/expt.nstim[0]; 
	     st->right->seed = st->left->seed;
	   }
	   else{
	     rnd = (myrand()  % 180) * M_PI/180; 
	     st->right->seed = st->left->seed +1;
	   }
	   st->left->phases[i] = st->phases[i] + rnd;
	   st->right->phases[i] = st->phases[i] - rnd;
	   if(expt.stimmode == BINOC_INDEP_ONOFF_LOG || expt.stimmode == THREE_CONTRAST_LEVEL || expt.stimmode == BINOC_INDEP_ONOFF)
	     st->right->seed = st->left->seed +1;
	 }
	 else if(flag & ANTICORRELATE){
	   st->left->phases[i] = st->phases[i] + M_PI_2 + scale * st->freqs[i] ;
	   st->right->phases[i] = st->phases[i] - M_PI_2 - scale * st->freqs[i];
	   st->right->seed = st->left->seed;
	 }
	 else{
	   st->left->phases[i] = st->phases[i] + st->phasedisp[0] + scale * st->freqs[i];
	   st->right->phases[i] = st->phases[i] - st->phasedisp[0] - scale * st->freqs[i];
	   st->right->seed = st->left->seed;
	 }
       }
       if(st->f > 0)
       {
	   st->left->pos.phase2 = st->pos.phase2 + (st->phasedisp[1] * st->left->ptr->sf2/st->f);
	   st->right->pos.phase2 = st->pos.phase2 - (st->phasedisp[1] * st->left->ptr->sf2/st->f);
        }
       st->left->pos.phase2 = st->pos.phase2 + (st->phasedisp[1]);
       st->right->pos.phase2 = st->pos.phase2 - (st->phasedisp[1]);
      calc_gratingN(st,st->left,disp);
      calc_gratingN(st,st->right,-disp);
      for(i = 0; i < st->nfreqs; i++){
	rclfreqs[i][expt.framesdone] = st->left->powers[i];
	rcrfreqs[i][expt.framesdone] = st->right->powers[i];
	rcdps[i][expt.framesdone] = dps[i];
      }
      break;
    case STIM_GRATING2:
       gl = (Grating *)st->left->ptr;
       gr = (Grating *)st->right->ptr;
       st->left->pos.phase = st->pos.phase + st->phasedisp[0];
       st->right->pos.phase = st->pos.phase - st->phasedisp[0];
       if(st->f > 0)
	 {
       st->left->pos.phase2 = st->pos.phase2 + (st->phasedisp[1] * gl->sf2/st->f);
       st->right->pos.phase2 = st->pos.phase2 - (st->phasedisp[1] * gl->sf2/st->f);
     }
       st->left->pos.phase2 = st->pos.phase2 + (st->phasedisp[1]);
       st->right->pos.phase2 = st->pos.phase2 - (st->phasedisp[1]);
      calc_grating2(st,st->left,disp);
      calc_grating2(st,st->right,-disp);
      break;
    case STIM_GRATING:
       if(flag & UNCORRELATE){
	 rnd = (myrand()  % 360) * M_PI/180; 
	 st->left->pos.phase = st->pos.phase + rnd;
	 st->right->pos.phase = st->pos.phase - rnd;
       }
       else
       {
	 st->left->pos.phase = st->pos.phase + st->phasedisp[0];
	 st->right->pos.phase = st->pos.phase - st->phasedisp[0];
       }
      calc_grating(st,st->left,disp);
      calc_grating(st,st->right,-disp);
      break;
    case STIM_GABOR:
       st->left->pos.phase = st->pos.phase + st->phasedisp[0]+st->phasedisp[1];
       st->right->pos.phase = st->pos.phase - st->phasedisp[0]-st->phasedisp[1];
       st->left->pos.phase2 = st->pos.phase2 + st->phasedisp[0]-st->phasedisp[1];
       st->right->pos.phase2 = st->pos.phase2 - st->phasedisp[0]+st->phasedisp[1];
      calc_gabor(st,st->left,disp);
      calc_gabor(st,st->right,-disp);
      break;
    case STIM_RDSSINE:
       st->left->pos.phase = st->phasedisp[0];
       st->right->pos.phase = -st->phasedisp[0];
       st->left->ptr->plaid_angle = 0;
       st->right->ptr->plaid_angle = 0;
      mode = rint(expt.vals[SEED_DELAY]);
      val = expt.vals[CONTRAST_RATIO];
      if(mode == 1){
       rds = st->left;
       if(val < 0.99)
	 st->right->pos.contrast = st->pos.contrast * val;
       if(val > 1.1)
	 st->left->pos.contrast = st->pos.contrast / val;
       calc_rds(st,st->left);
       calc_grating(st,st->right,-disp);
      }
      else if(mode == -1){
       rds = st->right;
       if(val < 0.99)
	 st->left->pos.contrast = st->pos.contrast * val;
       if(val > 1.1)
	 st->right->pos.contrast = st->pos.contrast / val;
       calc_rds(st,st->right);
       st->left->pos.xy[0] = st->pos.xy[0];
       st->left->pos.xy[1] = st->pos.xy[1];
       calc_grating(st,st->left,disp);
      }
      else if(mode == 0){
       rds = st->left;
       rdsr = st->right;
       if(flag & UNCORRELATE)
	 rdsr->baseseed = rds->baseseed+1;
       else
	 rdsr->baseseed = rds->baseseed;
       rdsr->seed = rdsr->baseseed;
       rds->seed = rds->baseseed;
       if(val > 1.1){
	 st->left->pos.contrast = st->pos.contrast/val  ;
	 st->right->pos.contrast = st->pos.contrast/val;
       }
       calc_rds(st,st->right);
       calc_rds(st,st->left);
      }
      else if(mode == 2){
       rds = st->right;
       if(val < 0.99){
	 st->left->pos.contrast = st->pos.contrast * val;
	 st->right->pos.contrast = st->pos.contrast * val;
       }
       calc_grating(st,st->right,-disp);
       calc_grating(st,st->left,disp);
      }
       break;
    case STIM_RLSSINE:
       st->left->pos.phase = st->phasedisp[0];
       st->right->pos.phase = -st->phasedisp[0];
       st->left->ptr->plaid_angle = 0;
       st->right->ptr->plaid_angle = 0;
      mode = rint(expt.vals[SEED_DELAY]);
      val = expt.vals[CONTRAST_RATIO];
      if(mode == 1){
       rds = st->left;
       if(val < 0.99)
	 st->right->pos.contrast = st->pos.contrast * val;
       if(val > 1.1)
	 st->left->pos.contrast = st->pos.contrast / val;
       calc_rls(st,st->left);
       calc_grating(st,st->right,-disp);
      }
      else if(mode == -1){
       rds = st->right;
       if(val < 0.99)
	 st->left->pos.contrast = st->pos.contrast * val;
       if(val > 1.1)
	 st->right->pos.contrast = st->pos.contrast / val;
       calc_rls(st,st->right);
       st->left->pos.xy[0] = st->pos.xy[0];
       st->left->pos.xy[1] = st->pos.xy[1];
       calc_grating(st,st->left,disp);
      }
      else if(mode == 0){
       rds = st->left;
       rdsr = st->right;
       if(flag & UNCORRELATE)
	 rdsr->baseseed = rds->baseseed+1;
       else
	 rdsr->baseseed = rds->baseseed;
       rdsr->seed = rdsr->baseseed;
       rds->seed = rds->baseseed;
       if(val > 1.1){
	 st->left->pos.contrast = st->pos.contrast/val  ;
	 st->right->pos.contrast = st->pos.contrast/val;
       }
       calc_rls(st,st->right);
       calc_rls(st,st->left);
      }
      else if(mode == 2){
       rds = st->right;
       if(val < 0.99){
	 st->left->pos.contrast = st->pos.contrast * val;
	 st->right->pos.contrast = st->pos.contrast * val;
       }
       calc_grating(st,st->right,-disp);
       calc_grating(st,st->left,disp);
      }
       break;
    case STIM_TEST:
       st->left->pos.phase = st->phasedisp[0];
       st->right->pos.phase = -st->phasedisp[0];
       st->left->ptr->plaid_angle = 0;
       st->right->ptr->plaid_angle = 0;
       calc_rds(st,st->left);
       break;
    case STIM_CHECKER:
      st->left->type = st->type;
      st->right->type = st->type;
       rds = st->left;
       rdsr = st->right;
       if(flag & UNCORRELATE){
	 rdsr->baseseed = rds->baseseed+1;
	 rds->seed = rds->baseseed;
	 rdsr->seed = rdsr->baseseed;
       }
       else{
	 rds->seed = rdsr->baseseed = rds->baseseed;
       }
/*       
 * if testflags[REPEAT_DOTS], then a different dot pattern is used
 * each frame, BUT the same sequence of dot patterns is repeated
 * from stimulus to stimulus
 */
       if(testflags[REPEAT_DOTS])
	 rds->seed++;
/*
 * phase is no longer used to drift the dots, only used for phase disparity
 */
       st->left->pos.phase = st->phasedisp[0];
       st->right->pos.phase = -st->phasedisp[0];
       if(flag & CROSSED_SEED)
       {
/*
* make sure the base seed is even, and the
* two Locator sets are the same
*/
	 rds->baseseed &= (~0x1);
	 memcpy(&st->right->pos,&st->left->pos,sizeof(Locator));
	   calc_rds_check(st,st->left);
	   st->right->seed = (rds->seed ^ 1);
	   calc_rds_check(st,st->right);
       }
       else if(rds->seedloop == 1)
	 {
	   rdsr->seed = rds->seed = rds->baseseed;
       if(flag & UNCORRELATE)
	 rdsr->seed++;
	 calc_rds_check(st,st->left);
	 calc_rds_check(st,st->right);
	 }
       else if(flag & UNCORRELATE)
	 {
	   rdsr->seed = rds->seed+1;
	 if(rds->ndots > rdsr->ndots)
	   init_rds(st, st->right, rds->density);
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 calc_rds_check(st,st->left);
	 calc_rds_check(st,st->right);
       }
       else if((val = fabs(expt.vals[SEED_DELAY])) > 0.5)
	 {
	   rdsr->seed = rds->seed+(int)(expt.vals[SEED_DELAY])*2;
	   if(st->prev != NULL && (int)(expt.vals[ALTERNATE_STIM_MODE]) == ZERO_BACK_DELAY)
	      rdsr->seed = rds->seed;
	   if(rds->ndots > rdsr->ndots){
	     init_rds(st, st->right, rds->density);
	     init_rds(st, st->right, rds->density);
	   }
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 calc_rds_check(st,st->left);
	 calc_rds_check(st,st->right);
       }
       else
	 {
	 calc_rds_check(st,st->left);
	 rdsr->seed = rds->seed;
	 if(rds->ndots > rdsr->ndots){
	   init_rds(st, st->right, rds->density);
	   init_rds(st, st->left, rds->density);
	 }
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 for(i = 0; i < 2; i++)
	   {
	     if(flag & ANTICORRELATE)
	       rdsr->lum[i] = (1.0 - rds->lum[i]);
	     else
	       rdsr->lum[i] = rds->lum[i];
	 rdsr->dotsiz[i] = rds->dotsiz[i];
       }
	 calc_rds_check(st,st->right);
       }
       if(!freezestimulus){
	 rds->seedctr++;
       }
      if(rds->seedctr >= rds->seedloop && rds->seedloop > 0)
	{
	rds->seed = rds->baseseed;
	rds->seedctr = 0;
      }
       rdsr->seed = rds->seed;
       if(rdspair(st)){
	 rdsnext =  st->next->left;
	 rdsnext->baseseed = rds->baseseed;
	   }
      break;
    case STIM_RDS:
       rds = st->left;
       rdsr = st->right;
       if(flag & UNCORRELATE){
	 rdsr->baseseed = rds->baseseed+1;
	 rds->seed = rds->baseseed;
	 rdsr->seed = rdsr->baseseed;
       }
       else{
	 rds->seed = rdsr->baseseed = rds->baseseed;
       }
/*       
 * if testflags[REPEAT_DOTS], then a different dot pattern is used
 * each frame, BUT the same sequence of dot patterns is repeated
 * from stimulus to stimulus
 */
       if(testflags[REPEAT_DOTS])
	 rds->seed++;
/*
 * phase is no longer used to drift the dots, only used for phase disparity
 * but need phase to be positive so that the dot wrapping algorithm works
 * when the virtual RDS is larger that the actual one
 * make sure phase is set to zero even if phasedisp is zero
 */
       if(st->phasedisp[0] > 0){
	 st->left->pos.phase = 2 * st->phasedisp[0];
	 st->right->pos.phase = 0;
       }
       else if(st->phasedisp[0] < 0){
	 st->left->pos.phase = 0;
	 st->right->pos.phase = - 2 * st->phasedisp[0];
       }
       else{
	 st->left->pos.phase = st->phasedisp[0];
	 st->right->pos.phase = st->phasedisp[0];
       }
	 
       if(flag & CROSSED_SEED)
       {
/*
* make sure the base seed is even, and the
* two Locator sets are the same
*/
	 rds->baseseed &= (~0x1);
	 memcpy(&st->right->pos,&st->left->pos,sizeof(Locator));
	   calc_rds(st,st->left);
	   st->right->seed = (rds->seed ^ 1);
	   calc_rds(st,st->right);
       }
       else if(rds->seedloop == 1)
	 {
	   rdsr->seed = rds->seed = rds->baseseed;
       if(flag & UNCORRELATE)
	 rdsr->seed++;
	 calc_rds(st,st->left);
	 calc_rds(st,st->right);
	 }
       else if(flag & UNCORRELATE)
	 {
	   rdsr->seed = rds->seed+1;
	 if(rds->ndots > rdsr->ndots)
	   init_rds(st, st->right, rds->density);
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 calc_rds(st,st->left);
	 calc_rds(st,st->right);
       }
       else if((val = fabs(expt.vals[SEED_DELAY])) > 0.5)
	 {
	   rdsr->seed = rds->seed+(int)(expt.vals[SEED_DELAY])*2;
	   if(st->prev != NULL && (int)(expt.vals[ALTERNATE_STIM_MODE]) == ZERO_BACK_DELAY)
	      rdsr->seed = rds->seed;
	   if(rds->ndots > rdsr->ndots){
	     init_rds(st, st->right, rds->density);
	     init_rds(st, st->right, rds->density);
	   }
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 calc_rds(st,st->left);
	 calc_rds(st,st->right);
       }
       else
	 {
	 calc_rds(st,st->left);
	 rdsr->seed = rds->seed;
	 if(rds->ndots > rdsr->ndots){
	   init_rds(st, st->right, rds->density);
	   init_rds(st, st->left, rds->density);
	 }
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 for(i = 0; i < 2; i++)
	   {
	     if(flag & ANTICORRELATE)
	       rdsr->lum[i] = (1.0 - rds->lum[i]);
	     else
	       rdsr->lum[i] = rds->lum[i];
	 rdsr->dotsiz[i] = rds->dotsiz[i];
       }
	 calc_rds(st,st->right);
       }
       if(!freezestimulus){
	 rds->seedctr++;
       }
      if(rds->seedctr >= rds->seedloop && rds->seedloop > 0)
	{
	rds->seed = rds->baseseed;
	rds->seedctr = 0;
      }
       rdsr->seed = rds->seed;
       if(rdspair(st)){
	 rdsnext =  st->next->left;
// Add 1000 here in version4.45 now that rand numbers are generated by
// [0 -> 1] * width, so that surround is not just scaled centre
	 rdsnext->baseseed = rds->baseseed+1000;
	   }
      break;
    case STIM_RLS:
      if(expt.stimmode == WHITENOISE16)
	st->dotdist = WHITENOISE16;
      else
	st->dotdist = 0;
       rds = st->left;
       rdsr = st->right;
       if(flag & UNCORRELATE){
	 rdsr->baseseed = rds->baseseed+1;
	 rds->seed = rds->baseseed;
	 rdsr->seed = rdsr->baseseed;
       }
       else{
	 rds->seed = rdsr->baseseed = rds->baseseed;
       }
/*       
 * if testflags[REPEAT_DOTS], then a different dot pattern is used
 * each frame, BUT the same sequence of dot patterns is repeated
 * from stimulus to stimulus
 */
       if(testflags[REPEAT_DOTS])
	 rds->seed++;
       st->left->pos.phase = st->phasedisp[0];
       st->right->pos.phase = -st->phasedisp[0];
       rds->newdots = rdsr->newdots = 1;
       if(rds->seedloop == 1)
	 {
	   if(realframecount > 0)
	     rds->newdots = rdsr->newdots = 0;
	   rdsr->seed = rds->seed = rds->baseseed;
	   if(flag & UNCORRELATE)
	     rdsr->seed++;
	   calc_rls(st,st->left);
	   calc_rls(st,st->right);
	 }
       else if(flag & UNCORRELATE)
	 {
	   rdsr->seed = rds->seed+1;
	 if(rds->ndots > rdsr->ndots)
	   init_rls(st, st->right, rds->density);
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 calc_rls(st,st->left);
	 calc_rls(st,st->right);
       }
       else if(fabs(expt.vals[SEED_DELAY]) > 0.5)
	 {
	   rdsr->seed = rds->seed+(int)(expt.vals[SEED_DELAY]*2);
	 if(rds->ndots > rdsr->ndots)
	   init_rls(st, st->right, rds->density);
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 calc_rls(st,st->left);
	 calc_rls(st,st->right);
       }
       else
	 {
	 rdsr->seed = rds->seed;
	 calc_rls(st,st->left);
	 if(rds->ndots > rdsr->ndots)
	   init_rls(st, st->right, rds->density);
	 else if(rds->ndots < rdsr->ndots)
	   rdsr->ndots = rds->ndots;
	 for(i = 0; i < 2; i++)
	   {
	     if(flag & ANTICORRELATE)
	       rdsr->lum[i] = (1.0 - rds->lum[i]);
	     else
	       rdsr->lum[i] = rds->lum[i];
	 rdsr->dotsiz[i] = rds->dotsiz[i];
       }
	 calc_rls(st,st->right);
       }
      
      rds->seedctr++;
      if(rds->seedctr >= rds->seedloop && rds->seedloop > 0)
	{
	rds->seed = rds->baseseed;
	rds->seedctr = 0;
      }
       rdsr->seed = rds->seed;
       if(rlspair(st)){
	 rdsnext =  st->next->left;
	 rdsnext->baseseed = rds->baseseed;
       }
       if(st->prev == NULL){
	 ndots = (st->left->ndots > MAXFREQS) ? MAXFREQS : st->left->ndots;
	 for(i = 0; i < ndots; i++){
	   rclfreqs[i][expt.framesdone] = st->left->im[i];
	   rcrfreqs[i][expt.framesdone] = st->right->im[i];
	 }
       }
      break;
    case STIM_CORRUG:
      if(corrugpair(st)) {
	st->next->pos.phase = st->pos.phase;
	st->next->f = st->f;
	st->next->pos.sf = st->pos.sf;
      }
      corrug = (OneStim *)st->left->ptr;
      corrugr = (OneStim *)st->right->ptr;
      if(flag & UNCORRELATE)
	rstim->baseseed = lstim->baseseed+1;
      else
	rstim->baseseed = lstim->baseseed;
      st->left->pos.phase = st->pos.phase;
      st->right->pos.phase = st->pos.phase;
      if(lstim->seedloop == 1)
	{
	  rstim->seed = lstim->seed = lstim->baseseed;
	  if(flag & UNCORRELATE)
	    rstim->seed++;
	  calc_corrug(st,st->left);
	  calc_corrug(st,st->right);
	}
      else if(flag & UNCORRELATE)
	{
	  rstim->seed = lstim->seed+1;
	  if(corrug->ndots > corrugr->ndots)
	    init_corrug(st, st->right, corrug->density);
	  else if(corrug->ndots < corrugr->ndots)
	    corrugr->ndots = corrug->ndots;
	  calc_corrug(st,st->left);
	  calc_corrug(st,st->right);
	}
      else
	{
	  rstim->seed = lstim->seed;
	  calc_corrug(st,st->left);
	  calc_corrug(st,st->right);
	  for(i = 0; i < 2; i++)
	    {
	      if(flag & ANTICORRELATE)
		corrugr->lum[i] = (1.0 - corrug->lum[i]);
	      else
		corrugr->lum[i] = corrug->lum[i];
	      corrugr->dotsiz[i] = corrug->dotsiz[i];
	    }
	}
      lstim->seed++;
      if(flag & UNCORRELATE)
	lstim->seed++;
      
      lstim->seedctr++;
      if(lstim->seedctr >= lstim->seedloop && lstim->seedloop > 0)
	{
	  lstim->seed = lstim->baseseed;
	  lstim->seedctr = 0;
	}
      rstim->seed = lstim->seed;
      break;
    case STIM_SQCORRUG:
/*
 * Background fixed does not work for sqcorrugs
 */
      optionflag &=  ~(BACKGROUND_FIXED_BIT);


      if(st->next != NULL && st->next->type == STIM_SQCORRUG)
	st->next->pos.phase = st->pos.phase;
      sqcorrug = (OneStim *)st->left->ptr;
      sqcorrugr = (OneStim *)st->right->ptr;
      if(flag & UNCORRELATE)
	rstim->baseseed = lstim->baseseed+1;
      else
	rstim->baseseed = lstim->baseseed;
      st->left->pos.phase = st->pos.phase;
      st->right->pos.phase = st->pos.phase;
      if(lstim->seedloop == 1)
	{
	  rstim->seed = lstim->seed = lstim->baseseed;
	  if(flag & UNCORRELATE)
	    rstim->seed++;
	  calc_sqcorrug(st,st->left);
	  calc_sqcorrug(st,st->right);
	}
      else if(flag & UNCORRELATE)
	{
	  rstim->seed = lstim->seed+1;
	  if(sqcorrug->ndots > sqcorrugr->ndots)
	    init_sqcorrug(st, st->right, sqcorrug->density);
	  else if(sqcorrug->ndots < sqcorrugr->ndots)
	    sqcorrugr->ndots = sqcorrug->ndots;
	  calc_sqcorrug(st,st->left);
	  calc_sqcorrug(st,st->right);
	}
      else
	{
	  rstim->seed = lstim->seed;
	  calc_sqcorrug(st,st->left);
	  calc_sqcorrug(st,st->right);
	  for(i = 0; i < 2; i++)
	    {
	      if(flag & ANTICORRELATE)
		sqcorrugr->lum[i] = (1.0 - sqcorrug->lum[i]);
	      else
		sqcorrugr->lum[i] = sqcorrug->lum[i];
	      sqcorrugr->dotsiz[i] = sqcorrug->dotsiz[i];
	    }
	}
      lstim->seed++;
      if(flag & UNCORRELATE)
	lstim->seed++;
      
      lstim->seedctr++;
      if(lstim->seedctr >= lstim->seedloop && lstim->seedloop > 0)
	{
	  lstim->seed = lstim->baseseed;
	  lstim->seedctr = 0;
	}
      rstim->seed = lstim->seed;
      break;
      
    case STIM_NONE:
      break;
    case STIM_CYLINDER:
      calc_cylinder(st);
      if(st->next != NULL && st->next->type == STIM_CYLINDER) /* allows dynamic background */
	{
/*
	  st->next->pos.xy[0] = st->pos.xy[0];
	  st->next->pos.xy[1] = st->pos.xy[1];
*/
	  st->next->pos.lastxy[0] = st->pos.lastxy[0];
	  st->next->pos.lastxy[1] = st->pos.lastxy[1];
	  calc_stimulus(st->next);
	  backdone = 1;
	}
      break;
    case STIM_RADIAL:
      mode = rint(expt.vals[SEED_DELAY]);
      val = expt.vals[CONTRAST_RATIO];
      switch(mode){
      case 0:
	st->left->stimmode = CIRCULAR_GRATING;
	st->right->stimmode = RADIAL_GRATING;
	st->left->pos.contrast = st->pos.contrast2/val;
	break;
      case 1:
	st->left->stimmode = RADIAL_GRATING;
	st->right->stimmode = CIRCULAR_GRATING;
	st->left->pos.contrast = st->pos.contrast2/val;
	break;
      case 2:
	st->right->stimmode = SPIRAL_GRATING;
	st->left->stimmode = RADIAL_GRATING;
	st->left->pos.contrast = st->pos.contrast2/val;
	break;
      case 3:
	st->right->stimmode = SPIRAL_GRATING;
	st->left->stimmode = CIRCULAR_GRATING;
	st->left->pos.contrast = st->pos.contrast2/val;
	break;
      case 4:
	st->right->stimmode = RADIAL_GRATING;
	st->left->stimmode = SPIRAL_GRATING;
	st->right->pos.contrast = st->pos.contrast2/val;
	break;
      case 5:
	st->right->stimmode = CIRCULAR_GRATING;
	st->left->stimmode = SPIRAL_GRATING;
	st->right->pos.contrast = st->pos.contrast2/val;
	break;
      case 6:
	st->right->stimmode = RADIAL_GRATING;
	st->left->stimmode = RADIAL_GRATING;
	st->right->pos.contrast = st->pos.contrast/val;
	st->left->pos.contrast = st->pos.contrast2/val;
	break;
      case 7:
	st->right->stimmode = CIRCULAR_GRATING;
	st->left->stimmode = CIRCULAR_GRATING;
	st->right->pos.contrast = st->pos.contrast/val;
	st->left->pos.contrast = st->pos.contrast2/val;
	break;
      case 8:
      default:
	st->right->stimmode = SPIRAL_GRATING;
	st->left->stimmode = SPIRAL_GRATING;
	break;
      }
      calc_radial(st,st->left,disp);
      calc_radial(st,st->right,-disp);
      break;
    }
    if(st->next != NULL && (optionflags[PAINT_BACKGROUND] || rdspair(st)
			    || rlspair(st) || corrugpair(st) || sqcorrugpair(st)))
      {
	if(!(optionflag & BACKGROUND_FIXED_BIT))
	  {
	    st->next->pos.xy[0] = st->pos.xy[0];
	    st->next->pos.xy[1] = st->pos.xy[1];
	    st->next->pos.lastxy[0] = st->pos.lastxy[0];
	    st->next->pos.lastxy[1] = st->pos.lastxy[1];
	  }
	if(backdone == 0)
		calc_stimulus(st->next);
      }
}



void clear_stimulus(Stimulus *st)
{
  glClearColor(st->gammaback, st->gammaback, st->gammaback, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void paint_half(Stimulus *st, int mode, int noback)
{
  int mask;

  setmask(mode);

  if(debug)
    glDrawBuffer(GL_FRONT_AND_BACK);
  switch(st->type)
    {
    case STIM_GABOR:
       paint_gabor(st,st->left,mode,0);
      break;
    case STIM_GRATINGN:
    case STIM_GRATING2:
    case STIM_GRATING:
       paint_grating(st->left,mode,0);
      break;
    case STIM_RDS:
       if(st->next != NULL && st->next->type == STIM_RDS)
	 {
	   if(!noback){
	     if(!(optionflag & BACKGROUND_FIXED_BIT))
	       {
		 st->next->pos.angle = st->pos.angle;
		 st->next->pos.xy[0] = st->pos.xy[0];
		 st->next->pos.xy[1] = st->pos.xy[1];
		 CheckRect(st->next);
	       }
	     paint_rds(st->next,mode);
	   }
      }
        paint_rds(st,mode);
      st->pos.lastxy[0] = st->pos.xy[0];
      st->pos.lastxy[1] = st->pos.xy[1];
      break;
    case STIM_RDSSINE:
      paint_test(st);
      break;
    case STIM_RLSSINE:
      paint_test(st);
      break;
    case STIM_TEST:
      paint_test(st);
      break;
    case STIM_SQUARE:    
        paint_square(st, st->left, mode);
      break;
    case STIM_BAR:
      if(mode == LEFTMODE)
        paint_bar(st, st->left, mode);
      else if(mode == RIGHTMODE)
        paint_bar(st, st->right, mode);
      break;
    case STIM_TWOBAR:
      paint_twobar(st, st->left, mode);
      break;
    case STIM_ANNULUS:
     case STIM_ANNULUSPLUS:
      paint_annulus(st, st->left, mode);
      break;
    case STIM_PROBE:
        paint_probe(st, st->left, mode);
      break;
    case STIM_NONE:
      break;
    }
  if(st->next != NULL && st->next->next != NULL)
    paint_half(st->next->next,mode,0);
  st->mode &= (~BACKCLEARED);
  st->pos.lastangle = st->pos.angle;
  st->pos.lastxy[0] = st->pos.xy[0];
  st->pos.lastxy[1] = st->pos.xy[1];
}


/* 
 * paint stereo stimulus paints stimuli for shutter glasses stereo display
 */
void paint_stereo_stimulus(Stimulus *st)
{
  double scale = 0.85;

  glDrawBuffer(GL_BACK_LEFT);
  if(st->prev == NULL){
    glClearColor(.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  glDrawBuffer(GL_BACK_RIGHT);
  if(st->prev == NULL){
    glClearColor(.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  setmask(BOTHMODE);
  st->left->mode = ALLMODE;
  st->right->mode = ALLMODE;
  if(st->next != NULL && optionflags[PAINT_BACKGROUND])
    paint_stereo_stimulus(st->next);
  switch(st->type)
    {
    case STIM_CYLINDER:	    /*j*/

      glDisable(GL_BLEND);
      glDrawBuffer(GL_BACK_LEFT);
      paint_cylinder(st,JONLEFT,0);
      glDrawBuffer(GL_BACK_RIGHT);
      paint_cylinder(st,JONRIGHT,0 ); 

      glPixelTransferf(GL_RED_SCALE, -1);
      glPixelTransferf(GL_RED_BIAS, 1);
      glPixelTransferf(GL_GREEN_SCALE, -1);
      glPixelTransferf(GL_GREEN_BIAS, 1);
      glPixelTransferf(GL_BLUE_SCALE, -1);
      glPixelTransferf(GL_BLUE_BIAS, 1);

      glEnable(GL_BLEND);
      glReadBuffer(GL_BACK_LEFT);
      glDrawBuffer(GL_BACK_RIGHT);
      glBlendColor(scale, scale, scale,  scale);
      glBlendFunc (GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_COLOR);
      glRasterPos2f(-639,-511);
      glCopyPixels(1,1,1280,1024,GL_COLOR);

      glReadBuffer(GL_BACK_RIGHT);
      glDrawBuffer(GL_BACK_LEFT);
      glPixelTransferf(GL_RED_SCALE, -1);
      glPixelTransferf(GL_RED_BIAS, 1);
      glPixelTransferf(GL_GREEN_SCALE, -1);
      glPixelTransferf(GL_GREEN_BIAS, 1);
      glPixelTransferf(GL_BLUE_SCALE, -1);
      glPixelTransferf(GL_BLUE_BIAS, 1);
      glBlendColor(scale, scale, scale,  scale);
      glBlendFunc (GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_COLOR);
      glRasterPos2f(-639,-511);
      glCopyPixels(1,1,1280,1024,GL_COLOR);

      st->pos.lastxy[0] = st->pos.xy[0];
      st->pos.lastxy[1] = st->pos.xy[1];
      break;
    }
}

void paint_stimulus(Stimulus *st)
{

  float delta;
  double pmatrix[16];
  int mode = 0;
  GLboolean isstereo = 0;

  glGetBooleanv(GL_STEREO, &isstereo);
  if(isstereo){
    paint_stereo_stimulus(st);
    return;
  }

  if(optionflags[SPLITSCREEN])
    {
      st->left->mode = st->right->mode = BOTHMODE;
    }
  else
    {
      st->left->mode = LEFTMODE;
      st->right->mode = RIGHTMODE;
    }
  glGetDoublev(GL_PROJECTION_MATRIX,pmatrix);
//  if(debug)
    glDrawBuffer(GL_FRONT_AND_BACK);
  if(st->next != NULL && optionflags[PAINT_BACKGROUND])
    paint_stimulus(st->next);
  else if(!st->noclear)
    clearstim(st,st->gammaback, optionflag & DRAW_FIX_BIT);
  st->noclear = 0;
  glGetDoublev(GL_PROJECTION_MATRIX,pmatrix);
  glPushMatrix();
  switch(st->type)
    {
    case STIM_GABOR:
      setmask(LEFTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(-psychoff[0]/2,0,0);
       paint_gabor(st,st->left,st->left->mode,0);
       setmask(RIGHTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(psychoff[0]/4,psychoff[1],0);
       paint_gabor(st,st->right,st->right->mode,6);
       setmask(BOTHMODE);
      break;
    case STIM_IMAGE:
      setmask(LEFTMODE);
      paint_image(st, st->left);
      if(debug)
	glFlushRenderAPPLE();
      setmask(RIGHTMODE);
      paint_image(st, st->right);
      if(debug)
	glFlushRenderAPPLE();
      break;
    case STIM_GRATINGN:
    case STIM_GRATING2:
    case STIM_GRATING:
      setmask(LEFTMODE);
       glGetDoublev(GL_PROJECTION_MATRIX,pmatrix);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(-psychoff[0]/2,0,0);
       glGetDoublev(GL_PROJECTION_MATRIX,pmatrix);
       paint_grating(st->left,st->left->mode,0);
            if (debug)
                glFinishRenderAPPLE();
            
       setmask(RIGHTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(psychoff[0],psychoff[1],0);
       paint_grating(st->right,st->right->mode,6);
       setmask(BOTHMODE);
      break;
    case STIM_RDS:
      st->left->mode = LEFTMODE;
      st->right->mode = RIGHTMODE;
      setmask(LEFTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(-psychoff[0]/2,0,0);
       if(st->next != NULL && st->next->type == STIM_RDS)
	 {
	   if(!(optionflag & BACKGROUND_FIXED_BIT))
	     {
	   st->next->pos.angle = st->pos.angle;
	   st->next->pos.xy[0] = st->pos.xy[0];
	   st->next->pos.xy[1] = st->pos.xy[1];
	   CheckRect(st->next);
	 }
        paint_rds(st->next,st->left->mode);
      }
        paint_rds(st,st->left->mode);
	   setmask(RIGHTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(psychoff[0],psychoff[1],0);
       if(st->next != NULL && st->next->type == STIM_RDS)
        paint_rds(st->next,st->right->mode);
       paint_rds(st,st->right->mode);
       setmask(BOTHMODE);
       st->pos.lastxy[0] = st->pos.xy[0];
       st->pos.lastxy[1] = st->pos.xy[1];
      break;
    case STIM_CHECKER:
      st->left->mode = LEFTMODE;
      st->right->mode = RIGHTMODE;
      setmask(LEFTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(-psychoff[0]/2,0,0);
       if(st->next != NULL && st->next->type == STIM_CHECKER)
	 {
	   if(!(optionflag & BACKGROUND_FIXED_BIT))
	     {
	   st->next->pos.angle = st->pos.angle;
	   st->next->pos.xy[0] = st->pos.xy[0];
	   st->next->pos.xy[1] = st->pos.xy[1];
	   CheckRect(st->next);
	 }
        paint_rds_check(st->next,st->left);
      }
        paint_rds_check(st,st->left);
	   setmask(RIGHTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(psychoff[0],psychoff[1],0);
       if(st->next != NULL && st->next->type == STIM_CHECKER)
        paint_rds_check(st->next,st->right);
       paint_rds_check(st,st->right);
       setmask(BOTHMODE);
       st->pos.lastxy[0] = st->pos.xy[0];
       st->pos.lastxy[1] = st->pos.xy[1];
      break;
    case STIM_RLS:
      st->left->mode = LEFTMODE;
      st->right->mode = RIGHTMODE;
      setmask(LEFTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(-psychoff[0]/2,0,0);
       if(st->next != NULL && st->next->type == STIM_RLS)
	 {
/*  
 * if paint background is set, don't force oriention whole background is 
 * painted anyway
 */
	   if(!optionflags[PAINT_BACKGROUND])
	     st->next->pos.angle = st->pos.angle;
	   if(!(optionflag & BACKGROUND_FIXED_BIT))
	     {
	   st->next->pos.xy[0] = st->pos.xy[0];
	   st->next->pos.xy[1] = st->pos.xy[1];
	   CheckRect(st->next);
	 }
        paint_rls(st->next,st->left->mode);
      }
        paint_rls(st,st->left->mode);
	   setmask(RIGHTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(psychoff[0],psychoff[1],0);
       if(st->next != NULL && st->next->type == STIM_RLS)
        paint_rls(st->next,st->right->mode);
       paint_rls(st,st->right->mode);
       setmask(BOTHMODE);
       st->pos.lastxy[0] = st->pos.xy[0];
       st->pos.lastxy[1] = st->pos.xy[1];
      break;
    case STIM_CORRUG:
      st->left->mode = LEFTMODE;
      st->right->mode = RIGHTMODE;
      setmask(LEFTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(-psychoff[0]/2,0,0);
      if(st->next != NULL && st->next->type == STIM_CORRUG)
	{
	  st->next->pos.angle = st->pos.angle;
	  if(!(optionflag & BACKGROUND_FIXED_BIT))
	    {
	      st->next->pos.xy[0] = st->pos.xy[0];
	      st->next->pos.xy[1] = st->pos.xy[1];
	      CheckRect(st->next);
	    }
	  paint_corrug(st->next,st->left->mode);
	}
      paint_corrug(st,st->left->mode);
      setmask(RIGHTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(psychoff[0],psychoff[1],0);
      if(st->next != NULL && st->next->type == STIM_CORRUG)
        paint_corrug(st->next,st->right->mode);
      paint_corrug(st,st->right->mode);
      setmask(BOTHMODE);
      st->pos.lastxy[0] = st->pos.xy[0];
      st->pos.lastxy[1] = st->pos.xy[1];
      break;
    case STIM_SQCORRUG:
      st->left->mode = LEFTMODE;
      st->right->mode = RIGHTMODE;
      setmask(LEFTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(-psychoff[0]/2,0,0);
      if(st->next != NULL && st->next->type == STIM_SQCORRUG)
	{
	  st->next->pos.angle = st->pos.angle;
	  if(!(optionflag & BACKGROUND_FIXED_BIT))
	    {
	      st->next->pos.xy[0] = st->pos.xy[0];
	      st->next->pos.xy[1] = st->pos.xy[1];
	      CheckRect(st->next);
	    }
	  paint_sqcorrug(st->next,st->left->mode);
	}
      paint_sqcorrug(st,st->left->mode);
      setmask(RIGHTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(psychoff[0],psychoff[1],0);
      if(st->next != NULL && st->next->type == STIM_SQCORRUG)
        paint_sqcorrug(st->next,st->right->mode);
      paint_sqcorrug(st,st->right->mode);
      setmask(BOTHMODE);
      st->pos.lastxy[0] = st->pos.xy[0];
      st->pos.lastxy[1] = st->pos.xy[1];
      break; 
    case STIM_TEST:	
      setmask(BOTHMODE);
      paint_test(st);

      break;
    case STIM_RDSSINE:	
      mode = rint(expt.vals[SEED_DELAY]);
      if(mode == 1){
	setmask(RIGHTMODE);
	paint_grating(st->right,st->right->mode,0);
	setmask(LEFTMODE);
	if(st->next != NULL && st->next->type == STIM_RDS)
	  paint_rds(st->next,st->left->mode);
	paint_rds(st,st->left->mode);
      }
      else if(mode == -1){ 
	setmask(LEFTMODE);
	paint_grating(st->left,st->left->mode,0);
	setmask(RIGHTMODE);
	if(st->next != NULL && st->next->type == STIM_RDS)
	  paint_rds(st->next,st->right->mode);
	paint_rds(st,st->right->mode);
     }
      else if(mode == 0){ 
	setmask(LEFTMODE);
	if(st->next != NULL && st->next->type == STIM_RDS)
	  paint_rds(st->next,st->left->mode);
	paint_rds(st,st->left->mode);
	setmask(RIGHTMODE);
	if(st->next != NULL && st->next->type == STIM_RDS)
	  paint_rds(st->next,st->right->mode);
	paint_rds(st,st->right->mode);
      }
      else if(mode == 2){ 
	setmask(RIGHTMODE);
	paint_grating(st->right,st->right->mode,0);
	setmask(LEFTMODE);
	paint_grating(st->left,st->left->mode,0);
      }
      break;
      /*
      setmask(BOTHMODE);
      paint_test(st);
      */
      break;
    case STIM_RLSSINE:	
      mode = rint(expt.vals[SEED_DELAY]);
      if(mode == 1){
	setmask(RIGHTMODE);
	paint_grating(st->right,st->right->mode,0);
	setmask(LEFTMODE);
	if(st->next != NULL && st->next->type == STIM_RDS)
	  paint_rls(st->next,st->left->mode);
	paint_rls(st,st->left->mode);
      }
      else if(mode == -1){ 
	setmask(LEFTMODE);
	paint_grating(st->left,st->left->mode,0);
	setmask(RIGHTMODE);
	if(st->next != NULL && st->next->type == STIM_RDS)
	  paint_rls(st->next,st->right->mode);
	paint_rls(st,st->right->mode);
     }
      else if(mode == 0){ 
	setmask(LEFTMODE);
	if(st->next != NULL && st->next->type == STIM_RDS)
	  paint_rls(st->next,st->left->mode);
	paint_rls(st,st->left->mode);
	setmask(RIGHTMODE);
	if(st->next != NULL && st->next->type == STIM_RDS)
	  paint_rls(st->next,st->right->mode);
	paint_rls(st,st->right->mode);
      }
      else if(mode == 2){ 
	setmask(RIGHTMODE);
	paint_grating(st->right,st->right->mode,0);
	setmask(LEFTMODE);
	paint_grating(st->left,st->left->mode,0);
      }
      break;
    case STIM_SQUARE:    
      setmask(BOTHMODE);
      if(optionflags[SPLITSCREEN])
	glTranslatef(-psychoff[0]/2,0,0);
      paint_square(st, st->left, st->left->mode);
      if(optionflags[SPLITSCREEN])
	glTranslatef(psychoff[0],psychoff[1],0);
      paint_square(st, st->right, st->right->mode);
      break;
    case STIM_BAR:
      setmask(LEFTMODE);
      if(optionflags[SPLITSCREEN])
	glTranslatef(-psychoff[0]/2,0,0);
      paint_bar(st, st->left, st->left->mode);
      setmask(RIGHTMODE);
      if(optionflags[SPLITSCREEN])
	glTranslatef(psychoff[0],psychoff[1],0);
      paint_bar(st, st->right, st->right->mode);
      setmask(BOTHMODE);
      break;
     case STIM_TWOBAR:
      if(st->next != NULL && st->next->type != STIM_NONE)
	paint_stimulus(st->next);
      setmask(LEFTMODE);
      if(optionflags[SPLITSCREEN])
	glTranslatef(-psychoff[0]/2,0,0);
      paint_twobar(st, st->left, st->left->mode);
      setmask(RIGHTMODE);
      if(optionflags[SPLITSCREEN])
	glTranslatef(psychoff[0],psychoff[1],0);
      paint_twobar(st, st->right, st->right->mode);
      setmask(BOTHMODE);
      break;
     case STIM_ANNULUS:
     case STIM_ANNULUSPLUS:
       if(debug)
	 glDrawBuffer(GL_FRONT_AND_BACK);
       if(optionflags[PAINT_BACKGROUND]){
	 setmask(LEFTMODE);
	 if(optionflags[SPLITSCREEN])
	   glTranslatef(-psychoff[0]/2,0,0);
	 paint_annulus(st, st->left, st->left->mode);
	 setmask(RIGHTMODE);
	 if(optionflags[SPLITSCREEN])
	   glTranslatef(psychoff[0],psychoff[1],0);
	 paint_annulus(st, st->right, st->right->mode);
	 setmask(BOTHMODE);
       }
      if(debug)
	glFlushRenderAPPLE();
      break;
   case STIM_PROBE:
      setmask(LEFTMODE);
      if(optionflags[SPLITSCREEN])
	glTranslatef(-psychoff[0]/2,0,0);
      if(option2flag & ALT_PROBE_BIT)
	paint_alt_probe(st, st->left, st->left->mode);
      else
	paint_probe(st, st->left, st->left->mode);
      setmask(RIGHTMODE);
      if(optionflags[SPLITSCREEN])
	glTranslatef(psychoff[0],psychoff[1],0);
      if(option2flag & ALT_PROBE_BIT)
	paint_alt_probe(st, st->right, st->right->mode);
      else
        paint_probe(st, st->right, st->right->mode);
      setmask(RIGHTMODE);
      break;


    case STIM_CYLINDER:	    /*j*/
      if(debug)
	glDrawBuffer(GL_FRONT_AND_BACK);

      if(mon.crosstalk[0] > 0 && !(optionflag & LEFT_FIXATION_CHECK))
      {
	setmask(FORCELEFT);
	paint_cylinder(st,JONRIGHT,mon.crosstalk[0]); 
      }
      setmask(LEFTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(-psychoff[0]/2,0,0);
      if(st->next != NULL && st->next->type == STIM_CYLINDER){
	if(!optionflag & BACKGROUND_FIXED_BIT){
	st->next->pos.angle = st->pos.angle;
/*
 *	st->next->pos.xy[0] = st->pos.xy[0];
 *      st->next->pos.xy[1] = st->pos.xy[1];
*/
	}
	CheckRect(st->next);
        paint_cylinder(st->next,JONLEFT, 0);
	if(debug)
	  glFlushRenderAPPLE();
      }
      if(debug)
	glFlushRenderAPPLE();
      paint_cylinder(st,JONLEFT, 0);
      if(mon.crosstalk[1] > 0 && !(optionflag & RIGHT_FIXATION_CHECK)){
	setmask(FORCERIGHT);
	paint_cylinder(st,JONLEFT,mon.crosstalk[1]); 
      }
      setmask(RIGHTMODE);
       if(optionflags[SPLITSCREEN])
	 glTranslatef(psychoff[0],psychoff[1],0);
      if(st->next != NULL && st->next->type == STIM_CYLINDER)
	paint_cylinder(st->next, JONRIGHT,0 );
      paint_cylinder(st,JONRIGHT,0); 
      setmask(RIGHTMODE);
      st->pos.lastxy[0] = st->pos.xy[0];
      st->pos.lastxy[1] = st->pos.xy[1];
      
      if(st->flag & TRACK_DOT){
	delta = fabsf(st->left->ptr->velocity * cos(asin(st->left->ptr->trackball.pos[X])));  	
	st->left->ptr->trackball.pos[X] += (st->left->ptr->trackball.left_right * delta); 		
	if ( (st->left->ptr->trackball.pos[X] >= 1.0) || (st->left->ptr->trackball.pos[X] <= -1.0)) {
	  st->left->ptr->trackball.pos[X] -=(st->left->ptr->trackball.pos[X] * delta);
	  st->left->ptr->trackball.left_right*=-1; 						     
	} 	
	paint_track(st);
      }
      
      break;
    case STIM_NONE:
      break;
    case STIM_RADIAL:
      setmask(LEFTMODE);
       paint_radial(st->left,st->left->mode,0);
      setmask(RIGHTMODE);
       paint_radial(st->right,st->right->mode,0);
      break;
    }
  glPopMatrix();  
  glGetDoublev(GL_PROJECTION_MATRIX,pmatrix);
  st->mode &= (~BACKCLEARED);
  st->pos.lastangle = st->pos.angle;
  st->pos.lastxy[0] = st->pos.xy[0];
  st->pos.lastxy[1] = st->pos.xy[1];
  if(optionflags[PAINT_THIRD_LAST] && st->next != NULL && st->next->next != NULL){
    st->next->next->noclear = 1;
    if(st->next->next->type != STIM_NONE)
      paint_stimulus(st->next->next);
  }
  if(optionflags[PAINT_THIRD_LAST] && st->next != NULL && st->next->type != STIM_NONE){
        st->next->noclear = 1;
        paint_stimulus(st->next);
  }
    if(debug)
      glFlushRenderAPPLE();
  if(st->next != NULL && st->next->type == STIM_PROBE)
    paint_stimulus(st->next);
}

void optimize_stimulus(Stimulus *st)
{
  switch(st->type)
    {
      case STIM_GABOR:
      optimize_gabor(st,st->right);
      optimize_gabor(st,st->left);
        break;
     case STIM_GRATING:
       if(st->f > 9 || st->f > 10){
	 glShadeModel(GL_FLAT);
	 SetStimulus(st, 1.0, XSAMPLES, NULL);
       }
       else{
	 glShadeModel(GL_SMOOTH);
	 SetStimulus(st, 2.0,  XSAMPLES, NULL);
       }
        break;
    default:
      break;
    }
  if(st->next != NULL)
    optimize_stimulus(st->next);

}


void mycirc(vcoord x, vcoord y, vcoord r)
{
  vcoord pt[2];
  float angle;
  int i;
  
  glBegin(GL_POLYGON);
  for(i = 0; i < 10; i++)
    {
      angle = (i * M_PI * 2)/10;
      pt[0] = x + r * sin(angle);
      pt[1] = y + r * cos(angle);
      myvx(pt);
    }
  glEnd();
}

void mycircle(vcoord x, vcoord y, vcoord r, int npts)
{
  vcoord pt[2];
  float angle;
  int i;
  
  glBegin(GL_POLYGON);
  for(i = 0; i < npts; i++)
    {
      angle = (i * M_PI * 2)/npts;
      pt[0] = x + r * sin(angle);
      pt[1] = y + r * cos(angle);
      myvx(pt);
    }
  glEnd();
}

void aarect(vcoord llx, vcoord lly, vcoord urx, vcoord ury)
{
  vcoord x,y;
  float lcolor;

  glVertex2f(llx,lly);
  glVertex2f(llx,ury);
  glVertex2f(urx,lly);
  glVertex2f(urx,ury);
  glVertex2f(llx,lly);
  glVertex2f(urx,lly);
  glVertex2f(llx,ury);
  glVertex2f(urx,ury);
}

int ReadGridVals(){
  int i = 0;
  FILE *fd;

    if((fd = fopen("./gridfile","r")) == NULL)
      return(-1);
    else{
      while(fscanf(fd,"%f %f",&vlines[i],&hlines[i]) > 1)
	i++;
      nvlines = nhlines = i;
    }    
    fclose(fd);
    return(nvlines);
}

int Project(vcoord *x, int eye)
{
  double truex,truey, dx, dy, vscale,hscale;
  if((vscale = expt.mon->trapscale[eye]) > 0){
/* 
   truex = xmv[0] + *x * cosa - *y * sina;
    truey = xmv[1] + *y * cosa + *x * sina;
*/
    if((hscale = expt.mon->trapscale[eye+2]) == 0)
      hscale = 1.0;
      
    truex = x[0];
    truey = x[1];
    dx = (truey * truex * (1-vscale));
    dy = (truey * truey * (1-vscale)) + truey * truex * (1-hscale);
    x[0] += dx;
    x[1] += dy;
  }

}

int grid(vcoord w, vcoord  h, int eye)
{
  int i,j,xc,yc,iw,ih;
  vcoord x[3];
  GLubyte *im1,*im2,*p;

  glLineWidth(expt.linw);
  glBegin(GL_LINES);
  if(expt.vals[GRIDSIZE] > 10){
      for (i = 0; i < nvlines; i++) {
	x[0] = deg2pix(vlines[i]);
	x[1] = -winsiz[1];
	Project(x,eye);
	myvx(x);
	x[1] = winsiz[1];
	Project(x,eye);
	myvx(x);
      }
  }
  else{
      for (i = 0; i < winsiz[0]; i+= w) {
	x[0] = i;
	x[1] = -winsiz[1];
	Project(x,eye);
	myvx(x);
	x[0] = i;
	x[1] = winsiz[1];
	Project(x,eye);
	myvx(x);
	x[0] = -i;
	x[1] = -winsiz[1];
	Project(x,eye);
	myvx(x);
	x[0] = -i;
	x[1] = winsiz[1];
	Project(x,eye);
	myvx(x);
      }
  }
      glEnd();
  glBegin(GL_LINES);
  if(expt.vals[GRIDSIZE] > 10){
    ReadGridVals();
      for (i = 0; i < nhlines; i++) {
	x[1] = deg2pix(hlines[i]);
	x[0] = -winsiz[0];
	Project(x,eye);
	myvx(x);
	x[1] = deg2pix(hlines[i]);
	x[0] = winsiz[0];
	Project(x,eye);
	myvx(x);
      }
  }
  else{
      for (i = 0; i < winsiz[1]; i+=h) {
	x[1] = i;
	x[0] = -winsiz[0];
	Project(x,eye);
	myvx(x);
	x[1] = i;
	x[0] = winsiz[0];
	Project(x,eye);
	myvx(x);
	x[1] = -i;
	x[0] = -winsiz[0];
	Project(x,eye);
	myvx(x);
	x[1] = -i;
	x[0] = winsiz[0]; 
	Project(x,eye);
	myvx(x);
      }
  }
      glEnd();

/*  
 * if ALT_STIM_MODE is set, paint lines in opposite colors in the two eyes
 */
      if(optionflag & GO_BIT && expt.vals[ALTERNATE_STIM_MODE] > 0){
    PaintGammaCheck(100,100);
  }
  return(0);
}



OneStim *GetOneStimPtr(Substim *sst)
{
  if(sst->ptr == NULL){
    sst->ptr = (OneStim *)malloc(sizeof(OneStim));
  }
  sst->ptr->im = NULL;
  sst->ptr->xpos = sst->ptr->ypos = NULL;
  return(sst->ptr);
}


/*
 * converts from SD of Gaussian in orientation space to 
 * vector length (i.e. fraction signal)
 */
float bw2cv(float bw)
{
  float cv,ksd = 28.6462;

  cv = exp(-(bw*bw)/(2 * ksd * ksd));
  return(cv);
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <math.h>
#include "myview.h"
#include <Xm/Text.h>
#include <Xm/TextF.h>

#define TEXTLABEL_WIDGET 101

int fplaces(float x, int n);
static char fplab[50];

extern XtAppContext app_context;

struct MENUDATA{
  char *s;
  void* client_data;
  int menuval;
  int id;
};

typedef struct MENUDATA Menudata;

#define fround(x,y) ((int)(x/y) * y)

static int sliderid = 0;
static Widget lastparent, sliderdialog;

#define POPUP_STRING 8
static char *sliderstrings[] = {
	"Shift Range Up", 
	"Shift Range Down", 
	"Double Range", 
	"Halve Range", 
	"Range x 10", 
	"Range/10", 
	"Center Range", 
	"+/-", 
#ifdef Linux
	"Pop slider up/down", 
#else
	"Pop slider up", 
#endif
	NULL,
};

#define UPRANGE 0
#define DOWNRANGE 1
#define DOUBLERANGE 2
#define HALVERANGE 3
#define RANGEX10 4
#define RANGEDIV10 5
#define CENTRERANGE 6
#define SLIDER_PLUSMINUS 7
#define SLIDER_POPUP 8

struct SliderCounter{
  Fpslider *fp;
  struct SliderCounter *next;
};

static struct SliderCounter *scstart = 0;

Widget BuildMenu(),BuildSimpleMenu(), BuildOptionMenu();
Widget BuildOptiondata(Widget parent, Optiondata *od, int set);
Widget BuildPullrightMenu(Widget parent, char *menu_title, Menudata *items,void (*proc)() );

/*
* setposargs uses implausibly high bits in the x,y
* arguments to decide on attaching to widgets/forms,
* right/left
*/

int setposargs(Arg *args, Widget xp, int yp)
{
	int x,y,n =0;
	x = (int)xp & (~WIDGET_POS_BITS);
	y = yp & (~WIDGET_POS_BITS);

    if(yp & NEXTRIGHTWIDGET_BIT)
    {
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, xp); n++;
     if(y <= 0)
       {
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
    XtSetArg(args[n], XmNtopWidget, xp); n++;
       }
    }
    else if(yp & NEXTLEFTWIDGET_BIT)
    {
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNrightWidget, xp); n++;
    }
    else if(yp & NEXTDOWNWIDGET_BIT)
    {
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNtopWidget, xp); n++;
    if(y > 0)
      {
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNleftOffset, y); n++;
    y = 0;
  }
    else
      {
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, xp); n++;
      }
    }
    else if(yp & NEXTUPWIDGET_BIT)
    {
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNbottomWidget, xp); n++;
    if(y > 0)
      {
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNleftOffset, y); n++;
    y = 0;
     }
    else
      {
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, xp); n++;
      }
    }
    else if(yp & ATTACHRIGHT_BIT)
    {
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightOffset, x); n++;
    }
    else if(x >= 0)
    {
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNleftOffset, x); n++;
    }
    if(y > 0)
    {
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopOffset, y); n++;
    }
    return(n);
}

Widget my_make_button(Widget parent, char *label, void (*proc)(), Widget x, int y, int data)
{
    Arg args[20];
    int n = 0;
    Widget item;

    n = setposargs(args, x, y);
    XtSetArg(args[n], XmNtraversalOn, False);n++;
    XtSetArg(args[n], XmNuserData, data);n++;
    item = XmCreatePushButton (parent, label, args, n);
    XtManageChild (item);
    XtAddCallback (item, XmNactivateCallback, proc, (XtPointer)data);
    return(item);
}

void cycleproc(Widget w, Optiondata *mi, XmAnyCallbackStruct *cbs)
{
  void (*callproc)();
  int menuval;
   Widget menu;
   Optiondata *od;
   int id;

  
  XtVaGetValues(w, XmNuserData, &menuval, 0);
  menu = XtParent(w);
   XtVaGetValues(menu, XmNuserData, &od, 0);
  od->val = menuval;

  if((callproc = (void (*)())mi->proc) != NULL)
    callproc(w, mi->id , menuval);
}


 Widget my_make_cycle(Widget parent, char *label, void (*proc)(),
 Widget x, int y, int data, char **strings, int *mvals)
 {
     Arg args[20];
     int n = 0,i = 0;
     Widget item,menu,widget;
     XmString str;
     Menudata *menudata;


     while(strings[i] != NULL)
       i++;
     n = i+1; /* last menuitem has null string */
     menudata = (Menudata *)malloc(sizeof(Menudata) * n);
     for(i = 0; i < n; i++)
       {
       menudata[i].s = strings[i];
       if(mvals == NULL)
	 menudata[i].menuval = i;
       else
	 menudata[i].menuval = mvals[i];
       menudata[i].client_data = proc;
       menudata[i].id = (int)data;
     }
     item = BuildOptionMenu(parent,  label, proc, strings, 0, data, mvals);
     n = setposargs(args, (Widget )x, y);
     XtSetArg(args[n], 	 XmNuserData, data); n++;
     XtSetValues(item, args, n);
     XtManageChild (item);
     free(menudata);
     return(item);
 }


 Widget my_make_menu_button(Widget parent, char *label, void (*proc)(), MenuItem *items,
		     Widget x, int y)
 {
     Arg args[20];
     int n = 0,i = 0;
     Widget item,bar;

     if(!(y & INBAR_BIT))
	{
     bar = XmCreateMenuBar(parent, "menubar", NULL, 0);
      n = setposargs(args, x, y);
   }
     else
       bar = parent;
     i = XmMENU_BAR;
/*      XtSetArg(args[n],XmNmarginHeight, 0); n++;*/
      XtSetValues(bar, args, n);
     item = BuildMenu(bar, XmMENU_PULLDOWN, label, 'F', items);
     XtManageChild(item);
     XtManageChild(bar);
       XtVaGetValues(bar, XmNrowColumnType, &i, 0);
     if((y & INBAR_BIT))
       return(item);
     else
       return(bar);
 }


 void sliderproc(w, client_data, call_data)
     Widget w;
     caddr_t client_data;
     XmScaleCallbackStruct *call_data;
 {
   int ival;
   Fpslider *fp;
   float val;
   char s[256];
   int (*callproc)(Fpslider *fp, float val, XEvent *event);

   fp = (Fpslider *)client_data;

   ival = call_data->value;

   fp->val = fp->min + (fp->max - fp->min) * ival/fp->width;
   sprintf(s,"%.*f",fp->precision,fp->val);
   XtVaSetValues(fp->text_w, XmNvalue, s, NULL);
   callproc = fp->proc;
   callproc(fp,fp->val,call_data->event);
 }

 void slidertextproc(w, client_data, call_data)
     Widget w;
     caddr_t client_data;
     XmAnyCallbackStruct *call_data;
 {
   int ival;
   Widget scale;
   Fpslider *fp;
   float val;
   char *s;
   int (*callproc)(Fpslider *fp, float val, XEvent *event);

   fp = (Fpslider *)client_data;

   s = XmTextFieldGetString(w);
   sscanf(s,"%f",&val);
   XtFree(s);
   ival = fp->width * (val - fp->min)/(fp->max - fp->min);
   if(ival < 0)
     ival = 0;
   else if(ival > fp->width)
     ival = fp->width;
    XmScaleSetValue(fp->scale,ival);
   if(call_data != NULL) /* really called from text widget */
     {
   callproc = fp->proc;
   fp->val = val;
   callproc(fp,val,NULL);
 }
 }


Sliderdat *FindSubSlider(Fpslider *fp, int code)
{

  int i = 0;
  Sliderdat *fpd = fp->proplist;

   while(fpd->code !=  code && fpd != NULL && i < fp->nprops)
     fpd++,i++;
   if(i < fp->nprops)
     {
     return(fpd);
   }
   else    /* reached end without finding code */
     return(NULL);
}

 int set_fpslider(Fpslider *fp, int code, float val)
 {
   int i = 0;
   Sliderdat *fpd;
   Fpslider *nextfp;
   char s[256];

   if(fp == NULL)
     return(-1);
   if((fpd = FindSubSlider(fp, code)) != NULL)
     fpd->val = val;
   else
     return(-1);
/* if this code is the currently active one, set it*/
   
   nextfp = fp;
   do
   if(fpd->code == nextfp->code) 
     {
     sprintf(s,"%.*f",nextfp->precision, val);
     XtVaSetValues(nextfp->text_w, XmNvalue, s, NULL);
     slidertextproc(nextfp->text_w, (caddr_t)nextfp, NULL);
     nextfp->val = val; 
   }while((nextfp = nextfp->next) != NULL);
   return(i);

 }

int set_fpitemrange(Fpslider *fp, int code, float min, float max)
{
   int i = 0;
   Sliderdat *fpd;
   char s[256];

   if(fp == NULL)
     return(-1);
   fpd = fp->proplist;
   while(fpd->code !=  code && fpd != NULL && i < fp->nprops)
     fpd++,i++;
   if(fpd != NULL || i< fp->nprops)
     {
       fpd->min = min;
       fpd->max = max;
     }
   else    /* reached end without finding code */
     return(-1);
/* if this code is the currently active one, set it*/
   if(i == fp->index) 
       set_fprange(fp, min, max, fpd->val, fpd->places);
   return(i);

}
 void set_fprange(Fpslider *fp, float min, float max, float val, 
 int precision)
 {
     char s[256];
     Dimension ow;
   int ival;

     if(fp == NULL)
       return;
   fp->min = min;
   fp->max = max;
   fp->precision = precision;
   ival = fp->width * (val - fp->min)/(fp->max - fp->min);
   if(ival < 0)
     ival = 0;
   else if(ival > fp->width)
     ival = fp->width;
    XmScaleSetValue(fp->scale,ival);
     sprintf(s,"%.*f",fp->precision, fp->min);
     my_set_label(fp->minlabel,s);
     sprintf(s,"%.*f",fp->precision, fp->max);
     my_set_label(fp->maxlabel,s);
     sprintf(s,"%.*f",fp->precision, val);
     XtVaSetValues(fp->text_w, XmNvalue, s, NULL);
     fp->val = val;
 }

 void set_fpprecision(Fpslider *fp, int precision)
 {
     char s[256];
     Dimension ow;
   int ival;
   fp->precision = precision;
   sprintf(s,"%.*f",fp->precision, fp->min);
   my_set_label(fp->minlabel,s);
   sprintf(s,"%.*f",fp->precision, fp->max);
   my_set_label(fp->maxlabel,s);
   sprintf(s,"%.*f",fp->precision, fp->val);
   XtVaSetValues(fp->text_w, XmNvalue, s, NULL);
 }


 void
 kill_fpslider(Widget w, Fpslider *fp, XmRowColumnCallbackStruct *cbs)
 {
	 free(fp);
 }


 Menudata *MakeScaleMenu(Fpslider *fp)
 {
   int n,i;
   Menudata *menudata;

     n = XtNumber(sliderstrings);
     if(fp->nprops == 1 && fp->prev == NULL && fp->next == NULL)
       n--;
     menudata = (Menudata *)malloc(sizeof(Menudata) * (n+1));
     for(i = 0; i < n; i++)
       {
       menudata[i].s = sliderstrings[i];
       menudata[i].menuval = i;
       menudata[i].client_data = fp;
       menudata[i].id = fp->id;
     }
     menudata[i].s = NULL;
   return(menudata);
 }

/* Change the property controlled by a multi-slider */

 void multimenu_proc(Widget w, Menudata *mi, XmAnyCallbackStruct *cbs)
 {
   int i = 0,n;
   Fpslider *fp = (Fpslider *)mi->client_data;
   Sliderdat *fpd = fp->proplist;
   int (*callproc)(Sliderdat *fps);

 /* first ensure that the value recorded in
 * the proplist for the previous property is
 * up to date
 */

   fp->proplist[fp->index].val = fp->val;

 /* find the proplist for the new property */
   while(fpd->code !=  mi->id && i < fp->nprops)
     fpd++,i++;

    callproc = fp->selproc;
   if(fpd != NULL)
     {
       fp->index = i;
       fp->code = fpd->code;
 /*
 * allow the client to set new range/values for the new
 * property
 */
     if(callproc != NULL)
	 callproc(fpd);
   strcpy(fplab, fpd->label);
   my_set_label(fp->label, fplab);
   set_fprange(fp, fpd->min, fpd->max, fpd->val, fpd->places);
   }

 }

SetSliderType(Fpslider *topslider, int code)
{
  Menudata temp;
  Fpslider *fp = topslider;
  int up = 0;

  if(fp == NULL || fp->nprops <= 1)
    return;
  while(fp != NULL)
    {
      if(fp->code == code)
	{
	  up = 1;
	}
      fp = fp->next;
    }
  if(!up)
    {
  temp.client_data = topslider;
  temp.id = code;
  multimenu_proc(NULL, &temp, NULL);
}
}

int PopSliderDown(Fpslider *fp)
{
  Widget item;
  if(fp == NULL)
    return(-1);
  item = XtParent(fp->rc);
  fp->prev->next = fp->next;
  XtUnmanageChild(fp->rc);
  free(fp);
  XtManageChild(item);
}

int PopSliderUp(Fpslider *fp, int code)
{
  Widget w;
  WidgetList l;
  Sliderdat *fpd;
  int len;
  Fpslider *lastfp = fp;

  if(fp == NULL || fp->nprops <= 1 || (fpd = FindSubSlider(fp, code)) == NULL)
    return(-1);
  
  while(lastfp->next != NULL)
    lastfp = lastfp->next;

  XtVaGetValues(fp->scale, XmNscaleWidth, &len, NULL);
  len = (len >> 16);
#if !defined(Linux) && !defined(Darwin)
  sprintf(sliderstrings[POPUP_STRING],"Pop Down");
#endif
  lastfp->next = make_fpslider(fp->rc, fpd->label, fpd->min, fpd->max, fpd->val, 
		fpd->places, NULL, 0, len, fp->proc, code);
  lastfp->next->state |= POPPED_UP;
  lastfp->next->prev = lastfp;
  lastfp->next->code = code;
  w = fp->rc;
  while(XtParent(w) != NULL)
    {
    XtManageChild(w);
    w = XtParent(w);
  }
  XtVaGetValues(w, XmNchildren, &l, XmNnumChildren, &len, 0);
  XtManageChildren(l, len);
#if !defined(Linux) && !defined(Darwin)
  sprintf(sliderstrings[POPUP_STRING],"Popup");
#endif
}

  change_range(Fpslider *fp, float scale)
 {
	 float maxm, minm, cur,inc,fac;
	 int swidth,frac;
	 double get_order();
	 Sliderdat *fpd;

   if(fp == NULL)
     return(-1);
   fpd = fp->proplist;
   while(fpd->code !=  fp->code && fpd != NULL)
     fpd++;
   if(fpd == NULL)
     return(-1);

	 if(scale != 0.)
		 fp->incr *= scale;
	 minm = fp->val - fp->incr * fp->width/2;
	 maxm = minm + (fp->width-1) * fp->incr;
	 fac = (float)get_order(maxm)/10.;
	 minm = fp->min = fround(minm,fac);
	 maxm = minm + (fp->width-1) * fp->incr;
	 set_fprange(fp,minm, maxm, fp->val, fp->precision);
	 fpd->min = fp->min;
	 fpd->max = fp->max;
 }

 void slidermenu_proc(Widget w, Menudata *mi, XmAnyCallbackStruct *cbs)
 {
   Fpslider *fpcur = (Fpslider *)mi->client_data;
	 Sliderdat *fpd;
   float val;
   XmString str;

   int (*callproc)(Fpslider *fp, float val, XEvent *event);

   switch (mi->menuval)
     {
		 case DOUBLERANGE:
			 change_range(fpcur,2.0);
			 break;
		 case HALVERANGE:
			 change_range(fpcur,0.5);
			 break;
		 case CENTRERANGE:
			 change_range(fpcur,0.0);
			 break;
		 case UPRANGE:
			 shift_range(fpcur,1);
			 break;
		 case DOWNRANGE:
			 shift_range(fpcur,-1);
			 break;
		 case RANGEX10:
			 change_range(fpcur,10.0);
			 break;
		 case RANGEDIV10:
			 change_range(fpcur,0.1);
			 break;
		       case SLIDER_PLUSMINUS:
			 fpd = fpcur->proplist;
			 while(fpd->code !=  fpcur->code && fpd != NULL)
			   fpd++;
			 if(fpd == NULL)
			   return;
			 val = -fpcur->val;
			 set_fprange(fpcur,-fpd->max, fpd->min, val, fpcur->precision);
			 callproc = fpcur->proc;
			 callproc(fpcur,val,NULL);
			 break;
		       case SLIDER_POPUP:
			 if(fpcur->state & POPPED_UP)
			   {
			   PopSliderDown(fpcur);
			   }
			 else
			   {
			   PopSliderUp(fpcur, fpcur->code);
			   }
			 break;
     }

 }

#define COLORARGS 7

 Fpslider *make_multislider(Widget parent, Sliderdat *proplist,
 int start, float val,  Widget x, int y, int len,
 void (*proc)(struct FPSLIDER *fp, float val, XEvent *event),
 void (*selproc)(Sliderdat *proplist),
 int id)
 {
     Widget scale,item,rowcol,menutop,menu;
     Arg args[10],colorargs[10];
     int n;
     char s[256],maxstring[256],space[256];
     Fpslider *fp;
     int loffset, charw = CHARWIDTH, off,cw,ival;
     XmString str;
     struct SliderCounter *sc;
     Dimension ow;
     int ox,i,slen,maxlen,ilong;
     Menudata *menudata,*submenu;
     Sliderdat *fpd = proplist;
     Pixel forepixel,backpixel;
     WidgetList l;
     void sliderproc(),slidertextproc(),killslider(),kill_fpslider();


     i = XtNumber(proplist);
     fp =(Fpslider *) malloc(sizeof(Fpslider));
     fp->state = 0;
     ilong = maxlen = 0;
     for(i = 0; proplist[i].label != NULL; i++)
       if(strlen(proplist[i].label) > maxlen)
	  {
	    maxlen = strlen(proplist[i].label);
	    ilong = i;
	  }
     fp->nprops = i;
     fp->proplist = proplist;
     fp->selproc = selproc;
     i = (start < fp->nprops) ? start : 0;
     fp->index = i;

     fp->id = id;
     fp->max = proplist[i].max;
     fp->min = proplist[i].min;
     fp->precision = proplist[i].places;
     fp->proc = proc;
     fp->val = val;

     if(fp->precision > 1)
       cw = fp->precision + 3;
     else
       cw = 5;

  XtVaGetValues(parent, XmNbackground, &backpixel, XmNforeground, &forepixel, 0);
     XtSetArg(colorargs[0], XmNforeground, forepixel);
     XtSetArg(colorargs[1], XmNborderColor, backpixel);
     XtSetArg(colorargs[2], XmNhighlightColor, backpixel);
     XtSetArg(colorargs[3], XmNarmColor, backpixel);
     XtSetArg(colorargs[4], XmNselectColor, forepixel);
     XtSetArg(colorargs[5], XmNtopShadowColor, forepixel);
     XtSetArg(colorargs[6], XmNbottomShadowColor, forepixel);
     XtSetArg(colorargs[7], XmNbackground, backpixel);
     n = setposargs(args, x, y);
     if(fp->nprops > 1)
       {
     fp->rc = rowcol = XtCreateWidget("sliderholder",
	 xmRowColumnWidgetClass, parent, args, n);
     fp->subrc = rowcol = XtCreateWidget("sliderform",
	 xmRowColumnWidgetClass, fp->rc, args, 0);
     XtSetValues(fp->rc, colorargs, COLORARGS);
   }
     else
       {
     fp->subrc = fp->rc = rowcol = XtCreateWidget("onesliderform",
	 xmRowColumnWidgetClass, parent, args, n);
   }
     XtSetValues(fp->subrc, colorargs, COLORARGS);

     XtVaSetValues(fp->rc, XmNuserData, id, 0);
     XtVaGetValues(rowcol, XmNrowColumnType, &i, 0);
    fp->menu = menutop = XtVaCreateWidget("menuholder",
	 xmRowColumnWidgetClass, rowcol,
	 XmNrowColumnType, XmMENU_BAR,
	 XmNorientation, XmHORIZONTAL,
	 NULL);
     XtSetValues(fp->menu, colorargs, COLORARGS);
     fp->next = NULL;


     submenu  = MakeScaleMenu(fp);
     if(fp->nprops > 1)
       {
       menudata = (Menudata *)malloc(sizeof(Menudata) * (fp->nprops+1));
       maxlen = 0;
     for(i = 0; i < fp->nprops; i++)
       {
       menudata[i].s = fp->proplist[i].label;
       if((slen = strlen(menudata[i].s)) > maxlen)
	 maxlen = slen;
	   menudata[i].menuval = i;
       menudata[i].client_data = fp;
       menudata[i].id = fp->proplist[i].code;
       }
     menudata[i].s = NULL;

       slen = strlen(proplist[fp->index].label);
       for(i = 0; i < maxlen-slen; i++)
	 space[i] = ' ';
     space[i] = 0;
     sprintf(s,"%s%s",proplist[fp->index].label, space);
 /* initially create the widget with the largest label */
     menu = BuildSimpleMenu(menutop, XmMENU_PULLDOWN, 
	 proplist[ilong].label, multimenu_proc, menudata);
     XtSetValues(menu, colorargs, COLORARGS);
     BuildPullrightMenu(menu,
	       "Rescale", submenu, slidermenu_proc);

     XtManageChild(menu);
     }
     else
       {
     menu = BuildSimpleMenu(menutop, XmMENU_PULLDOWN, proplist[ilong].label, slidermenu_proc, submenu);
     XtSetValues(menu, colorargs, COLORARGS);
     XtManageChild(menu);
   }
     XtSetValues(menutop, colorargs, COLORARGS);
     XtManageChild(menutop);
     fp->label = menu;
     sprintf(s,"%.*f",fp->precision, fp->val);
     sprintf(maxstring,"%.*f]",fp->precision, fp->max);

     item = XtVaCreateManagedWidget("slidertext", xmTextFieldWidgetClass, rowcol,
	 XmNleftAttachment, XmATTACH_WIDGET,
	 XmNleftWidget, menutop,
	 XmNcolumns, strlen(maxstring) +1,
	 XmNvalue, s,
	 NULL);
     XtAddCallback (item, XmNactivateCallback, slidertextproc, (XtPointer)fp);
     fp->text_w = item;
     XtSetValues(item, colorargs, COLORARGS);


     off = ox + ow;
     sprintf(s,"[%.*f",fp->precision, fp->min);
     str = XmStringCreateSimple(s);
     fp->minlabel = XtVaCreateManagedWidget("label", xmLabelWidgetClass, rowcol,    
	 XmNleftAttachment, XmATTACH_WIDGET,
	 XmNleftWidget, fp->text_w,
	 XmNlabelString, str,
	 NULL);
     XmStringFree(str);
     XtSetValues(fp->minlabel, colorargs, COLORARGS);


     n = 0;
     XtSetArg(args[n], 	 XmNleftAttachment, XmATTACH_WIDGET); n++;
     XtSetArg(args[n], 	 XmNleftWidget, fp->minlabel); n++;
     XtSetArg(args[n], 	 XmNtraversalOn, False); n++;
     XtSetArg(args[n], 	 XmNvalue, 0); n++;
     fp->scale = XtCreateWidget("fpscale", 
	 xmScaleWidgetClass, rowcol, args, n);
     XtSetValues(fp->scale, colorargs, COLORARGS);

     if(len <= 0)
       XtVaGetValues(fp->scale, XmNscaleWidth, &len, NULL);
     if(len <= 0)
       len = 200;
     else if(len > (1<<16))
       len = (len >>16);
/* len is now the lenght of the slider*/
     ival = len * (val - fp->min)/(fp->max - fp->min);
     if(ival < 0)
       ival = 0;
     else if(ival > len)
       ival = len;

     if(len < 1280){
     XtVaSetValues(fp->scale, XmNscaleWidth, len, XmNmaximum, len, 
		   XmNvalue, ival, NULL);
     }
     XtAddCallback (fp->scale, XmNvalueChangedCallback, sliderproc, (caddr_t)fp);
     XtAddCallback (fp->scale, XmNdragCallback, sliderproc, (caddr_t)fp);
 XtManageChild(fp->scale);
     fp->width = len;
     fp->incr = (fp->max - fp->min)/len;
     str = XmStringCreateSimple(maxstring);
     fp->maxlabel =  XtVaCreateManagedWidget("label", xmLabelWidgetClass, rowcol, 
	 XmNleftAttachment, XmATTACH_WIDGET,
	 XmNleftWidget, fp->scale,
	 XmNleftOffset, 5,
	 XmNlabelString, str,
	 NULL);
    XmStringFree(str);
    XtSetValues(fp->maxlabel, colorargs, COLORARGS);
    XtManageChild(fp->subrc);
    XtManageChild(fp->rc);
    XtAddCallback(rowcol, XmNunmapCallback, kill_fpslider, fp);
 /* 
 * Finally set up the slider to the initial desired function
 */
     if(fp->nprops > 1 && start < fp->nprops)
       multimenu_proc(menu, &menudata[start], NULL);
     XtVaGetValues(menutop, XmNrowColumnType, &i, 0);
     XtVaGetValues(fp->rc, XmNnumChildren, &n, XmNchildren, &l, 0);
     if(n > 0)
       XtVaSetValues(l[0], XmNuserData, fp, NULL);


   return(fp);
 }

 Fpslider *make_fpslider(Widget parent, char *label, float min,
 float max, float val, int precision, Widget x, int y, int len,
 void (*proc)(struct FPSLIDER *fp, float val, XEvent *event),
 int id)
 {
   Fpslider *fp;
     Sliderdat *fpd;

   fpd = (Sliderdat *)malloc(2 * sizeof(Sliderdat));

     fpd->max = max;
     fpd->min = min;
     fpd->places = precision;
     fpd->label = label;
     fpd->code = id;
     (fpd+1)->label = NULL;
    fp =  make_multislider(parent, fpd, 0, val, x,y, len, proc, NULL, id);
   return(fp);
 }


 my_set_rclabel(Widget w, char *s)
 {
   XmString str;
   WidgetClass class;
   Widget item;
   int n;
   WidgetList l;

   XtVaGetValues(w, XmNnumChildren, &n, XmNchildren, &l, 0);
     str = XmStringCreateSimple(s);
     XtVaSetValues(l[0], XmNlabelString, str,NULL);
     XmStringFree(str);
 }

 my_set_label(Widget w, char *s)
 {
   XmString str;
   WidgetClass class;
   Widget item;
   int n;
   WidgetList l;

   if(s == NULL)
     str = XmStringCreateSimple(" ");
   else
     str = XmStringCreateSimple(s);
     XtVaSetValues(w, XmNlabelString, str,NULL);
     XmStringFree(str);
 }


 Widget my_make_label(Widget parent, char *label, Widget x, int y, int w)
 {
   Widget item;
   XmString str;
   Arg args[20];
   int n = 0;

     str = XmStringCreateSimple(label);
     n = setposargs(args, x, y);
     XtSetArg(args[n], 	 XmNlabelString, str); n++;
     item = XtCreateManagedWidget("label", xmLabelWidgetClass, parent,
     	args, n);
     XmStringFree(str);
     return(item);
 }


 Widget my_make_text(Widget parent, char *label,  char *val, 
void (*proc)(), Widget x, int y, int w, int data)
 {
   Widget item1, item, rowcol;
   static int textctr = 0;
   XmString str;
   Dimension siz = 0;
   Arg args[20];
   char buf[256];
   int n,yo;

   if(w <= 0)
     w = 4;
   
   n = setposargs(args, x, y);
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   XtSetArg(args[n], XmNuserData, data); n++;
   if(label != NULL && strlen(label) > 1)
     sprintf(buf,"%s",label);
   else
     sprintf(buf,"textrc%d",textctr++);
     
     rowcol = XtCreateWidget(buf,
	 xmRowColumnWidgetClass, parent, args, n);


   n = 0;
   str = XmStringCreateSimple(label);
   XtSetArg(args[n], XmNlabelString, str); n++;
   XtSetArg(args[n], XmNuserData, TEXTLABEL_WIDGET); n++;
     item1 = XtCreateManagedWidget("text_label", xmLabelGadgetClass, rowcol, args, n);
   
   n = 0;
   XtSetArg(args[n], XmNuserData, data); n++;
   XtSetArg(args[n], XmNcolumns, w); n++;
   if(val && *val)
     XtSetArg(args[n], XmNvalue, val); n++;

     item = XtCreateWidget("text_field", xmTextFieldWidgetClass, rowcol, args, n);
   if(proc != NULL)
     {
    XtAddCallback (item, XmNactivateCallback, proc, (XtPointer)data);
     XtAddCallback (item, XmNvalueChangedCallback, proc, (XtPointer)data);
  }
   XtManageChild(item);
   XmStringFree(str);
     XtManageChild(rowcol);
     return(rowcol);
 }

Numeric *numeric_set(Widget rc, ...)
 {
   va_list args;
   int prop,i,n = 0;
   Numeric *num;
   WidgetList l;
   Widget item;
   XmString str;
   char *s,buf[256];

   XtVaGetValues(rc, XmNnumChildren, &n, XmNchildren, &l, 0);
   num = 0;
   for(i = 0; i< n; i++)
   {
   XtVaGetValues(l[i], XmNuserData, &num, 0);
    if (num != 0)
	 break;
   }

   va_start(args, rc);
   while((prop = va_arg(args, int)) != 0)
   {
   switch(prop)
     {
     case PANEL_LABEL:
       s = (char *)va_arg(args, int);
       str = XmStringCreateSimple(s);
       XtVaSetValues(num->label, XmNlabelString, str, NULL);
       XmStringFree(str);
       break;
     case PANEL_MIN_VALUE:
       num->min = va_arg(args, int);
       break;
     case PANEL_MAX_VALUE:
       num->max = va_arg(args, int);
       break;
     case NUMERIC_VALUE:
       num->val = va_arg(args, int);
       sprintf(buf,"%d",num->val);
       XtVaSetValues(num->text_w, XmNvalue, buf, NULL);
       break;
     case PANEL_READ_ONLY:
       i = va_arg(args, int);
       XtVaSetValues(num->text_w, XmNeditable, !i, 0);
       XtVaSetValues(num->downarrow, XmNsensitive, !i, 0);
       break;
     }
   }
	va_end(args);
     	return(num);
 }


 numeric_set_value(Widget rc, int val)
 {
   char s[256];
   Numeric *num;
   WidgetList l;
   int i,n;

   XtVaGetValues(rc, XmNnumChildren, &n, XmNchildren, &l, 0);
   for(i = 0; i< n; i++)
   {
   XtVaGetValues(l[i], XmNuserData, &num, 0);
   if(num != 0)
	 break;
   }
   if(num == 0)
	 return;
   sprintf(s,"%d",val);
   XtVaSetValues(num->text_w, XmNvalue, s, NULL);
   num->val = val;
 }

int numeric_set_string(Widget rc, char *inst)
 {
   char s[256];
   Numeric *num;
   WidgetList l;
   int i,n;

   XtVaGetValues(rc, XmNnumChildren, &n, XmNchildren, &l, 0);
   for(i = 0; i< n; i++)
   {
   XtVaGetValues(l[i], XmNuserData, &num, 0);
   if(num != 0)
	 break;
   }
   if(num == 0)
	 return;
   sprintf(s,"%s",inst);
   XtVaSetValues(num->text_w, XmNvalue, s, NULL);
 }

char *numeric_get_string(Widget rc)
 {
   char *s;
   Numeric *num;
   WidgetList l;
   int i,n;

   XtVaGetValues(rc, XmNnumChildren, &n, XmNchildren, &l, 0);
   for(i = 0; i< n; i++)
   {
   XtVaGetValues(l[i], XmNuserData, &num, 0);
    if(num != 0)
	 break;
   }
   if(num == 0)
	 return(NULL);
   XtVaGetValues(l[i], XmNvalue, &s, NULL);
   return(s);
 }

 int numeric_get_value(Widget rc)
 {
   char *s;
   Numeric *num;
   WidgetList l;
   int i,n;

   XtVaGetValues(rc, XmNnumChildren, &n, XmNchildren, &l, 0);
   for(i = 0; i< n; i++)
   {
   XtVaGetValues(l[i], XmNuserData, &num, 0);
    if(num != 0)
	 break;
   }
   if(num == 0)
	 return(-1);
   XtVaGetValues(l[i], XmNvalue, &s, NULL);
   sscanf(s,"%d",&num->val);
   return(num->val);
 }

 void upproc(Widget w, Numeric *num, XmArrowButtonCallbackStruct *cbs)
 {
   void (*callproc)();

   callproc = (void (*)())(num->proc);
   num->val++;
   numeric_set_value(num->rc, num->val);
   callproc(num->rc, num->val, num->data);
 }

 void numtextproc(Widget w, Numeric *num, XmAnyCallbackStruct *cbs)
 {
   void (*callproc)();
   char *s;

   callproc = (void (*)())(num->proc);
   s = XmTextFieldGetString(w);
   num->val = atoi(s);
   XtFree(s);
   callproc(num->rc, num->val, num->data);
 }

 void downproc(Widget w, Numeric *num, XmArrowButtonCallbackStruct *cbs)
 {
   void (*callproc)();

   callproc = (void (*)())(num->proc);
   num->val--;
   if(num->val < num->min)
	 num->val = num->min;
   numeric_set_value(num->rc, num->val);
   callproc(num->rc, num->val, num->data);
 }


 Widget my_make_numeric(Widget parent, char *label, 
  void (*proc)(), Widget x, int y, int val, int w, int data)
 {
   Widget item,text,rowcol;
   XmString str1,str2;
   Dimension siz = 0;
   int l = 0,i,n = 0;
   char s[256];
   Arg args[20];
   Numeric *num = (Numeric *)malloc(sizeof(Numeric));
   void downproc(),upproc(),numtextproc();

    n = setposargs(args, x, y);
/*   XtSetArg(args[n], XmNmarginHeight, 0); n++;*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   XtSetArg(args[n], XmNuserData, data); n++;
     rowcol = XtCreateWidget("rowcolumn",
	 xmRowColumnWidgetClass, parent, args, n);
   num->min = 0;

   num->rc = rowcol;
   sprintf(s,"%d",val);
   num->val = val;
   num->label = XtVaCreateManagedWidget(label, xmLabelGadgetClass, rowcol, NULL);

     XtManageChild(num->label);
   if( w <= 0)
     w = 4;
     num->text_w = XtVaCreateManagedWidget(label, xmTextFieldWidgetClass, rowcol,
	 XmNleftAttachment, XmATTACH_WIDGET,
	 XmNleftWidget, num->label,
/*	 XmNmarginHeight, 0,*/
	 XmNcolumns, w,
	 XmNvalue, s,
	 XmNuserData, num,
	 NULL);
     XtManageChild(num->text_w);
     XtAddCallback (num->text_w, XmNactivateCallback, numtextproc, (XtPointer)num);
     num->proc = proc;
     num->data = data;
     item = XtVaCreateManagedWidget("upcounter", xmArrowButtonGadgetClass, rowcol,
	 XmNleftAttachment, XmATTACH_WIDGET,
	 XmNleftWidget, num->text_w,
	 XmNarrowDirection, XmARROW_UP,
	 XmNvalue, val,
	 NULL);
     XtAddCallback (item, XmNactivateCallback, upproc, (XtPointer)num);
     num->downarrow = item;
     XtManageChild(item);

   XtVaGetValues(item, XmNwidth, &siz, 
		 XmNleftOffset, &l, 0);
     item = XtVaCreateManagedWidget("downcounter", xmArrowButtonGadgetClass, rowcol,
	 XmNarrowDirection, XmARROW_DOWN,
	 XmNleftAttachment, XmATTACH_WIDGET,
	 XmNleftWidget, item,
	 NULL);
     XtAddCallback (item, XmNactivateCallback, downproc, (XtPointer)num);
     XtManageChild(item);
     XtManageChild(rowcol);
     return(num->rc);
 }

 Widget
 BuildPullrightMenu(Widget parent, char *menu_title, Menudata *items,void (*proc)() )
 {
     Widget PullDown, cascade, widget,menu;
     int i;
     XmString str;

     XtVaGetValues(parent, XmNsubMenuId, &menu, NULL);

     PullDown = XmCreatePulldownMenu(menu, "_pulldown", NULL, 0);

     str = XmStringCreateSimple(menu_title);
     cascade = XtVaCreateManagedWidget(menu_title,
	 xmCascadeButtonGadgetClass, menu,
	 XmNsubMenuId,   PullDown,
	 XmNlabelString, str,
	 NULL);
     XmStringFree(str);
     /* Now add the menu items */


     for (i = 0; items[i].s != NULL; i++) {
	     widget = XtVaCreateManagedWidget(items[i].s,
		 xmPushButtonGadgetClass, PullDown,
		 NULL);
	     XtAddCallback(widget, XmNactivateCallback,
		 proc, &items[i]);
     }
     return cascade;
 }



 /* Pulldown menus are built from cascade buttons, so this function
  * also includes pullright menus.  Create the menu, the cascade button
  * that owns the menu, and then the submenu items.
  */
 Widget
 BuildPulldownMenu(parent, menu_title, menu_mnemonic, items)
 Widget parent;
 char *menu_title, menu_mnemonic;
 MenuItem *items;
 {
     Widget PullDown, cascade, widget;
     int i;
     XmString str;

     PullDown = XmCreatePulldownMenu(parent, "_pulldown", NULL, 0);

     str = XmStringCreateSimple(menu_title);
     cascade = XtVaCreateManagedWidget(menu_title,
	 xmCascadeButtonGadgetClass, parent,
	 XmNsubMenuId,   PullDown,
	 XmNlabelString, str,
	 XmNmnemonic,    menu_mnemonic,
	 NULL);
     XmStringFree(str);
     /* Now add the menu items */


     for (i = 0; items[i].label != NULL; i++) {
	 /* If subitems exist, create the pull-right menu by calling this
	  * function recursively.  Since the function returns a cascade
	  * button, the widget returned is used..
	  */
	 if (items[i].subitems)
	     widget = BuildPulldownMenu(PullDown,
		 items[i].label, NULL, items[i].subitems);
	 else
	     widget = XtVaCreateManagedWidget(items[i].label,
		 xmPushButtonGadgetClass, PullDown,
		 NULL);
	 /* Whether the item is a real item or a cascade button with a
	  * menu, it can still have a mnemonic.
	  */
 /*        if (items[i].mnemonic)
	     XtVaSetValues(widget, XmNmnemonic, items[i].mnemonic, NULL);
 */
	 /* any item can have an accelerator, except cascade menus. But,
	  * we don't worry about that; we know better in our declarations.
	  */
 /*        if (items[i].accelerator) {
	     str = XmStringCreateSimple(items[i].accel_text);
	     XtVaSetValues(widget,
		 XmNaccelerator, items[i].accelerator,
		 XmNacceleratorText, str,
		 NULL);
	     XmStringFree(str);
	 }
 */
	 /* again, anyone can have a callback -- however, this is an
	  * activate-callback.  This may not be appropriate for all items.
	  */
	 if (items[i].callback)
	     XtAddCallback(widget, XmNactivateCallback,
		 items[i].callback, items[i].callback_data);
     }
     return cascade;
 }


 Widget
 BuildMenu(parent, menu_type, menu_title, menu_mnemonic, items)
 Widget parent;
 int menu_type;
 char *menu_title, menu_mnemonic;
 MenuItem *items;
 {
     Arg args[20];
     Widget menu, cascade, widget;
     int i=0;
     XmString str;
     char buf[256];

     XtSetArg(args[i], XmNtearOffModel, XmTEAR_OFF_ENABLED);
     i++;
     if (menu_type == XmMENU_PULLDOWN)
	 menu = XmCreatePulldownMenu(parent, "_pulldown", args, i);
     else if(menu_type == XmMENU_OPTION)
	 menu = XmCreatePulldownMenu(parent, "_pulldown",NULL,0);
     else if (menu_type == XmMENU_POPUP)
	 menu = XmCreatePopupMenu(parent, "_popup", NULL, 0);
     else {
	 XtWarning("Invalid menu type passed to BuildMenu()");
	 return NULL;
     }

     /* Pulldown menus require a cascade button to be made */
     if (menu_type == XmMENU_PULLDOWN) {
       sprintf(buf,"%s...",menu_title);
	 str = XmStringCreateSimple(buf);
	 cascade = XtVaCreateManagedWidget(menu_title,
	     xmCascadeButtonGadgetClass, parent,
	     XmNsubMenuId,   menu,
	     NULL);
	 XmStringFree(str);
     } else if (menu_type == XmMENU_OPTION) {
	 /* Option menus are a special case, but not hard to handle */
	 Arg args[3];
	 str = XmStringCreateSimple(menu_title);
	 XtSetArg(args[0], XmNsubMenuId, menu);
	 XtSetArg(args[1], XmNlabelString, str);
	 /* This really isn't a cascade, but this is the widget handle
	  * we're going to return at the end of the function.
	  */
	 cascade = XmCreateOptionMenu(parent, menu_title, args, 2);
	 XmStringFree(str);
     }

     /* Now add the menu items */
     for (i = 0; items[i].label != NULL; i++) {
       if(*items[i].label){
	 if(items[i].subitems == POPUPBUTTON)
	   {
	   sprintf(buf,"%s...",items[i].label);
	     str = XmStringCreateSimple(buf);
	     widget = XtVaCreateManagedWidget(items[i].label,
		 xmPushButtonGadgetClass, menu,
		 XmNuserData, items[i].callback_data,
		XmNlabelString, str,
		 NULL);
	     XmStringFree(str);
	 }
	 else if (items[i].subitems)
	     if (menu_type == XmMENU_OPTION) {
		 XtWarning("You can't have submenus from option menu items.");
		 continue;
	     } else
		 widget = BuildMenu(menu, XmMENU_PULLDOWN,
		     items[i].label, NULL, items[i].subitems);
	 else
	     widget = XtVaCreateManagedWidget(items[i].label,
		 xmPushButtonGadgetClass, menu,
		 XmNuserData, items[i].callback_data,
		 NULL);
	 if (items[i].callback)
	     XtAddCallback(widget,
		     XmNactivateCallback,      /* PushButton class */
		 items[i].callback, items[i].callback_data);
       }
     }

     return menu_type == XmMENU_POPUP ? menu : cascade;
 }

 Widget
 BuildSimpleMenu(parent, menu_type, menu_title, proc, items)
 Widget parent;
 int menu_type;
 char *menu_title;
 void (*proc)();
 Menudata *items;
 {
     Widget menu, cascade, widget;
     int i;
     XmString str;

     if (menu_type == XmMENU_PULLDOWN)
	 menu = XmCreatePulldownMenu(parent, "_pulldown", NULL, 0);
     else if(menu_type == XmMENU_OPTION)
       menu = XmCreatePulldownMenu(parent, "_pulldown",NULL,0);
     else if (menu_type == XmMENU_POPUP)
	 menu = XmCreatePopupMenu(parent, "_popup", NULL, 0);
     else {
	 XtWarning("Invalid menu type passed to BuildSimpleMenu()");
	 return NULL;
     }

     /* Pulldown menus require a cascade button to be made */
     if (menu_type == XmMENU_PULLDOWN) {
	 str = XmStringCreateSimple(menu_title);
	 cascade = XtVaCreateWidget(menu_title,
	     xmCascadeButtonGadgetClass, parent,
	     XmNsubMenuId,   menu,
	     XmNlabelString, str,
	     NULL);
	 XmStringFree(str);
     } else if (menu_type == XmMENU_OPTION) {
	 /* Option menus are a special case, but not hard to handle */
	 Arg args[2];
	 str = XmStringCreateSimple(menu_title);
	 XtSetArg(args[0], XmNsubMenuId, menu);
	 XtSetArg(args[1], XmNlabelString, str);
	 /* This really isn't a cascade, but this is the widget handle
	  * we're going to return at the end of the function.
	  */
	 cascade = XmCreateOptionMenu(parent, menu_title, args, 2);
	 XmStringFree(str);
     }

     /* Now add the menu items */
     for (i = 0; items[i].s != NULL; i++) {
	     widget = XtVaCreateManagedWidget(items[i].s,
		 xmPushButtonGadgetClass, menu,
		 XmNuserData, items[i].client_data,
		 NULL);
	    XtAddCallback(widget,
		     XmNactivateCallback,
		 proc, &items[i]);
     }

     XtVaSetValues(menu, XmNbuttonSet, 2, 0);

     /* for popup menus, just return the menu; pulldown menus, return
      * the cascade button; option menus, return the thing returned
      * from XmCreateOptionMenu().  This isn't a menu, or a cascade button!
      */
     return menu_type == XmMENU_POPUP? menu : cascade;
 }


int get_option_value(Widget w)
{
   Widget menu;
   Optiondata *od;
   int id;

   XtVaGetValues(w, XmNuserData, &id, XmNsubMenuId, &menu, 0);
   XtVaGetValues(menu, XmNuserData, &od, 0);
   return(od->val);
}

int free_optiondata(Optiondata *od)
{
  int i;
  if(od->strings){
    for(i = 0; i < od->n; i++){
      if(od->strings[i])
	free(od->strings[i]);
    }
    free(od->strings);
  }
}

int free_option_menu(Widget w, int val)
 {
   Widget menu, rc = XtParent(w), mb;
   XmString str;
   Optiondata *od;
   int x,y,id,i, ret = 0;;

   XtVaGetValues(w, XmNuserData, &id, XmNsubMenuId, &menu, 0);
   if((mb = XmOptionButtonGadget(w)) == NULL)
     return(-1);
   XtVaGetValues(mb, XmNlabelString, &str, 0);
   XtVaGetValues(menu, XmNuserData, &od, 0);
   free_optiondata(od);
 }

int set_option_value(Widget w, int val)
 {
   Widget menu, rc = XtParent(w), mb;
   XmString str;
   Optiondata *od;
   int x,y,id,i, ret = 0;;

   XtVaGetValues(w, XmNuserData, &id, XmNsubMenuId, &menu, 0);
   if((mb = XmOptionButtonGadget(w)) == NULL)
     return(-1);
   XtVaGetValues(mb, XmNlabelString, &str, 0);
   XtVaGetValues(menu, XmNuserData, &od, 0);
   /*   if(od->val == val)
     return(0);
     */
   if(od->vals == NULL)
     i = val;
   else
     for(i = 0; i < od->n; i++)
       if(val == od->vals[i])
	 break;
   if(i >= od->n)
     return(-1);
   if(!od->strings[i] || !*(od->strings[i]))
     return(-1);
   od->val = val;
   str = XmStringCreateSimple(od->strings[i]);
   XtVaSetValues(mb, XmNlabelString, str, 0);
   XmStringFree(str);
   XtManageChild(w);
   return(0);
 }

 Widget 
 BuildOptionMenu(Widget parent, char *title, void (*proc)(), char **strings, int set, int id, int *vals)
 {
     Widget menu, cascade, widget, rc;
     int i =0, count;
     XmString str,str1,str2,str3;
     XmStringTable mstrs;
	 Arg args[20];
     int n = 0;
     Optiondata *od;

     od = (Optiondata *)malloc(sizeof(Optiondata));
     od->strings = strings;
     od->id = id;
     od->proc = proc;
     od->title = title;
     od->vals = vals;
     menu = BuildOptiondata(parent, od, set);
     return(menu);

   }


 Widget
 BuildOptiondata(Widget parent, Optiondata *od, int set)
 {
     Widget menu, cascade, widget, rc;
     WidgetList list;
     int i =0, count,m=0;
     XmString str,str1,str2,str3;
     XmStringTable mstrs;
	 Arg args[20];
     int n = 0;
     void cycleproc();

     od->val = set;
     while(od->strings[i] != NULL)
       i++;
     od->n = n = i; 
     count = n+1;

     mstrs = (XmStringTable)XtMalloc(count * sizeof(XmString));
     for(i = 0; i < n; i++)
       mstrs[i] = XmStringCreateSimple(od->strings[i]);
     mstrs[i] = 0;
     str = XmStringCreateSimple(od->title);

     n = 0;
     /*
      * do this after creating the Wdget, for Lesstif compatibility
       XtSetArg(args[n], XmNlabelString, str);
       n++;
      
     */
     XtSetArg(args[n], XmNbuttons, mstrs);
     n++;
       XtSetArg(args[n], XmNbuttonCount, i);
     n++;

       XtSetArg(args[n], XmNbuttonSet, set);
     n++;
       XtSetArg(args[n], XmNuserData, od->id);
     n++;
/*     sprintf(cbuf,"%s_options",od->title);*/
	 menu = XmCreateSimpleOptionMenu(parent, od->title,
			 args, n);
     XtVaSetValues(menu, XmNlabelString, str, 0);
     n = 0;
     XtVaGetValues(menu, XmNsubMenuId, &cascade, 0);
     XtVaGetValues(cascade, XmNnumChildren, &n, XmNchildren, &list, 0);
     for(i = 0; i < n; i++)
       {
	 m = 0;
     XtVaGetValues(list[i], XmNnumChildren, &m,0);
     XtAddCallback(list[i], XmNactivateCallback, cycleproc, (XtPointer)od);
     if(od->vals != NULL)
       XtVaSetValues(list[i], XmNuserData, (XtPointer)od->vals[i], 0);
     else
       XtVaSetValues(list[i], XmNuserData, (XtPointer)i, 0);
   }
     XtVaSetValues(cascade, XmNuserData, od, 0);
     XtManageChild(menu);

     XmStringFree(str);
     for(i = 0; i < count; i++)
       XmStringFree(mstrs[i]);
     XtFree(mstrs);
     return menu;
 }

 SetXY(Widget w, int x, int y)
 {
     XtVaSetValues(w, 
	 XmNleftAttachment, XmATTACH_FORM,
	 XmNleftOffset, x,
	 XmNtopAttachment, XmATTACH_FORM,
	 XmNtopOffset, y,
	 NULL);
 }


 void DestroyShell(Widget w, Widget shell)
 {
   XtDestroyWidget(shell);
 }

 void CloseShell(Widget w, Widget shell)
 {
  XtUnmanageChild(shell);
  XtPopdown(shell);
 }



 shift_range(Fpslider *fp, int step)
 {
	 float maxm, minm, cur,inc,fac,range;
	 int swidth,frac;

	 range = fp->max - fp-> min;
	 fp->min +=  (step * range);
	 fp->max +=  (step * range);
	 set_fprange(fp, fp->min, fp->max, fp->val, fp->precision);
 }


 int GetHeight(Widget w)
 {
   Dimension h;

   XtVaGetValues(w, XmNheight, &h, 0);
   return((int)(h));
 }

 int GetWidth(Widget w)
 {
   Dimension h;

   XtVaGetValues(w, XmNwidth, &h, 0);
   return((int)(h));
 }



 void my_expose(Widget item)
 {
   XtManageChild(item);
   XtPopup(XtParent(item),XtGrabNone);
 }

 Widget my_make_frame(Widget parent, char *s, int w, int h, int x, int y)
 {
   Widget item;
   item = my_make_popupframe(parent, s, w, h, x, y);
   return(item);
 }

 Widget my_make_popupframe(Widget parent, char *s, int w, int h, int x, int y)
 {
   Widget frame;
   Arg args[20];
   int n = 0;

   if(w > 0)
     {
   XtSetArg(args[n], XmNwidth, w); n++;
 }
   if(h > 0)
     {
   XtSetArg(args[n], XmNheight, h); n++;
 }
   if(x >= 0)
     {
   XtSetArg(args[n], XmNx, x); n++;
 }
   if(y >= 0)
     {
   XtSetArg(args[n], XmNy, y); n++;
 }

   frame = XmCreateFormDialog(parent, s, args, n);
   return(frame);
 }

 Widget my_make_panel(Widget frame)
 {
   Widget form;
   Arg args[20];
   int n = 0;

   form = XmCreateForm(frame, "form", args, n);
   return(form);
 }


Widget my_make_text_window(Widget parent, Arg *args, int n, int id)
{
   Widget text_w;
   int i;
   int k = 0;
	
    text_w = XmCreateScrolledText(parent, "text_win", args, n);
    XtManageChild(text_w);
    return(text_w);
}

int set_textwindow_text(Widget text_w, char *filename, int cols)
{
	FILE *fd;
	struct stat fstat;
	char *text;
	int retval = 0;
	
	if(!filename || !*filename)
		return(-1);
	if(stat(filename, &fstat) == -1 || 
		(fstat.st_mode & S_IFMT) != S_IFREG ||
		!(fd = fopen(filename, "r")))
		fprintf(stderr, "can't read %s\n", filename);
	if(!(text = XtMalloc((unsigned)(fstat.st_size+1))))
	{
		fprintf(stderr, "Insufficient memory for %s\n", filename);
		return(-1);
	}
	if(!fread(text, sizeof(char), fstat.st_size+1, fd))
	  retval = -1;
	text[fstat.st_size] = 0;
	if(cols > 2 && cols < 100)
	  XtVaSetValues(text_w, XmNnumColumns, cols, 0);
	XmTextSetString(text_w, text);
	XtFree(text);
	fclose(fd);
	return(retval);
}

char *save_textwindow_text(Widget text_w, char *filename)
{
	FILE *fd = 0;
	struct stat fstat;
	char *text,buf[256];
	long len;
	
	if(!filename || !*filename)
		sprintf(buf,"Null Filename");
	else if(!(fd = fopen(filename, "w")))
		sprintf(buf, "can't write %s\n", filename);
	if(fd == 0)
		return(buf);
		
	text = XmTextGetString(text_w);
	len = XmTextGetLastPosition(text_w);
	if(fwrite(text, sizeof(char), len, fd) != len)
		sprintf(buf,"Couldn't write whole file %s",filename);
	else
	{
	if(text[len-1] != '\n')
		fputc('\n',fd);
	sprintf(buf,"%ld bytes written to %s", len, filename);
	}
	XtFree(text);
	fclose(fd);
	return(buf);
}



Widget my_make_toggle(Widget parent, char *label, void (*proc)(), int x, int y, int data, char **strings)
{
  Widget dialog,toggle_box,item,w;
  Arg args[20];
  int n = 0,i = 0,hstep;
  Dimension oh;
  XmString str1;
  float value;
  char *ctr;
  
    toggle_box = XtVaCreateWidget("togglebox",
        xmRowColumnWidgetClass, parent,
        XmNpacking,        XmPACK_COLUMN,
        XmNnumColumns,     1,
        XmNuserData, data, 
        NULL);
    i = 0;
    while(strings[i] != NULL){
        w = XtVaCreateManagedWidget(strings[i],
            xmToggleButtonGadgetClass, toggle_box, NULL);
        XtAddCallback(w, XmNvalueChangedCallback, proc, (XtPointer)i);
	i++;
    }
  return(toggle_box);
}

panel_get(Widget w, String str)
{
 int res;
 switch((int)(str)){
 default:
 XtVaGetValues(w, str, &res, 0);
 break;
}
 return(res);
}

Widget my_make_message(Widget parent, char *label, Widget x, int y, int len, int id)
{
  Widget w;
  XmString str;
  Arg args[10];
  int n = 0;
  
  str = XmStringCreateSimple(label);

    n = setposargs(args, x, y);
XtSetArg(args[n], XmNlabelString, str); n++;
XtSetArg(args[n], XmNuserData, id); n++;
  w = XmCreateLabel(parent, "message", args, n);
  XtManageChild(w);
  XmStringFree(str);
  return(w);
}


Widget my_make_bold_message(Widget parent, char *label, Widget x, int y, int len, int id)
{

  XmString str;
  Widget w;
  Arg args[10];
  int n = 0;

  str = XmStringCreateSimple(label);

    n = setposargs(args, x, y);
XtSetArg(args[n], XmNlabelString, str); n++;
XtSetArg(args[n], XmNuserData, id); n++;
  w = XmCreateLabel(parent, "message", args, n);
  XtManageChild(w);
  XmStringFree(str);
  return(w);
}

char *gettextvalue(Widget w)
{
	Widget child;
	int id;
	char *s = NULL;
	
	XtVaGetValues(w, XmNuserData, &id, 0);
	child = FindWidgetChild(w, id);
	if(child != NULL)
	  s = XmTextFieldGetString(child);
	  return(s);
}

int settextvalue(Widget w, char *s)
{
	Widget child;
	WidgetClass class;
	int id;
	
	XtVaGetValues(w, XmNuserData, &id, 0);
	/* find the child of the rowcol toset the text */
	if((class = XtClass(w)) == xmTextWidgetClass || 
		class == xmTextFieldWidgetClass)
	  XmTextSetString(w,s);
	else if((child = FindWidgetChild(w, id)) != NULL)
	  XmTextSetString(child,s);
	else
	  return(-1);
	return(0);
}

int settextlabel(Widget w, char *s)
{
   XmString str;
	Widget child;
	WidgetClass class;
	int id, ret = 0;
	
	XtVaGetValues(w, XmNuserData, &id, 0);
	str = XmStringCreateSimple(s);
	if((class = XtClass(w)) == xmTextWidgetClass || 
		class == xmTextFieldWidgetClass)
	  w = XtParent(w);
	if((child = FindWidgetChild(w, TEXTLABEL_WIDGET)) != NULL)
	    XtVaSetValues(child, XmNlabelString, str, NULL);
	else
	  ret = -1;
	if(ret > 0)
	  XtManageChild(child);
	XmStringFree(str);
	return(ret);
}

int atofitem(Widget w, float val, int i)
{
	char s[BUFSIZ];
	Widget child;
	int id;

	if(w == NULL)
	  return;
	XtVaGetValues(w, XmNuserData, &id, 0);
	child = FindWidgetChild(w, id);
	if(i < 0)
		i = fplaces(val, -i);
	sprintf(s,"%.*f", i, val);
	XmTextSetString(child,s);
}

int texttoi(Widget w)
{
	char *s;
	Widget child;
	int id;
	XmString str;

	XtVaGetValues(w, XmNuserData, &id, 0);
	child = FindWidgetChild(w, id);
	XtVaGetValues(child, XmNvalue, &s, 0);
	id = 0;
	  if(s && *s)
	    sscanf(s,"%d",&id);
	return(id);
}

atoiitem(Widget w, int val)
{
	char s[BUFSIZ];
	Widget child;
	int id;

	XtVaGetValues(w, XmNuserData, &id, 0);
	child = FindWidgetChild(w, id);

	sprintf(s,"%d", val);
	XtVaSetValues(child, XmNvalue, s, NULL);
}

XtArgVal window_get(Widget w, String prop)
{
  XtArgVal res;

  XtVaGetValues(w, prop, &res, NULL);
  return(res);
}

wfatal()
{
  fprintf(stderr,"Fatal Window Creation Error\n");
  exit(1);
}


message_done()
{
}

clearmenu()
{
}

void *menu_get(MenuItem *menu, int prop)
{
   switch(prop)
     {
     case MENU_CLIENT_DATA:
     case MENU_VALUE:
       return(menu->callback_data);
       break;
     case MENU_NITEMS:
       return(XtNumber(menu));
       break;
     default:
       return(0);
       break;
     }
}


int event_id(XEvent *event)
{

  KeySym sym;

  if(event == NULL)
    return(0);
  switch(event->xany.type)
    {
    case ButtonRelease:
    case ButtonPress:
      switch(event->xbutton.button)
	{
	case Button1:
	  return(MS_LEFT);
	case Button2:
	  return(MS_MIDDLE);
	case Button3:
	  return(MS_RIGHT);
	}
        break;
    case MotionNotify:
      if(event->xmotion.state & Button1Mask)
	return(LOC_DRAG);
      else
	return(LOC_MOVE);
      break;
    case KeyPress:
       sym = XLookupKeysym(&event->xkey,0);
          switch(sym)
	    {
	  case XK_KP_Delete:
	  case XK_Delete:
	      return(DELETE_KEY);
	      break;
	      default:
	        return(sym);
	      break;
	    }
      break;
    default:
      return(event->xany.type);
      break;
    }
}


Widget FindWidgetChild(Widget w, int id)
{
  WidgetList l;
  Widget child = NULL;
  Cardinal n = 0;
  int i,dummy[10], dat;
/*
 * otherwise 64 bit Linux overwrites i when getting dat!!
 */

  if(w == NULL)
     return(NULL);

  XtVaGetValues(w, XmNnumChildren, &n, XmNchildren, &l, NULL);

  if(n == 0)
  	return(NULL);
  for(i = 0; i < n; i++)
	{
	  XtVaGetValues(l[i],XmNuserData,&dat,0);
	  if(dat == id)
	  {
	  	child = l[i];
	  	break;
	  }
	}
  if(child == NULL)
  {
	  for(i = 0; i < n; i++)
	  {
	  	child = FindWidgetChild(l[i], id);
	  	if(child != NULL)
		  return(child);
 	}
  }
  else return(child);
}
	
iterate(Widget w, String att, XtArgVal val)
{
  WidgetList l;
  Widget menu;
  Cardinal n = 0;
  int i;

  if(w == NULL)
     return;
  XtVaGetValues(w, XmNnumChildren, &n, NULL);
  XtVaSetValues(w, att, val, 0);
  menu = 0;
  XtVaGetValues(w, XmNsubMenuId, &menu, NULL);
  XtVaGetValues(w, XmNrowColumnType, &i, 0);
  if(menu != 0)
    iterate(menu, att, val);
  if(n > 0)
    {
  XtVaGetValues(w, XmNchildren, &l, NULL);
  for(i = 0; i < n; i++)
	{
	  iterate(l[i],att,val);
	}

    }
}

set_icon(item, image, w, h)
 Widget item;
char *image;
 int w,h;
 {
 	Pixmap bitmap;
 	
 	bitmap = XCreatePixmapFromBitmapData(XtDisplay(item),
 		RootWindowOfScreen(XtScreen(item)),
		image, w, h, 1, 0, 1);
 	XtVaSetValues(item, XmNiconPixmap, bitmap, 
 		NULL);
 }


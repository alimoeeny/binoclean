#include <stdio.h>
#include <sys/time.h>
#include <stdarg.h>
#include <math.h>
#include "mymath.h"
#include "myview.h"
#include <GL/gl.h>
#include "commdefs.h"
#include "stimuli.h" /*j this will include jonfront.h */
#include "WinDefs.h"
#include "misc.h"
#include "protos.h"

#define ISI 5

Sliderdat asliderprops[] =
{
	{0, 10.0, 2, VSACCADE_VALUE, "V Saccade size"},
	{0, 10.0, 2, HSACCADE_VALUE, "H Saccade size"},
	{0.0, 10., 1, TIMEOUT_CODE, "Timeout"},
	{0.0, 10., 1, WRONG_TIMEOUT_CODE, "Wrong to"},
	{0, 500, 0, REWARD_SIZE, "Reward Size"},
	{0, 5.0, 1, SAC_FIX_CRIT, "Saccade fix box"},
	{0, 1.0, 2, SACTARGSIZE, "target size"}, 
	{0, 1.0, 2, STARTVAL, "Startval"},
	{0, 1.0, 2, MAXSTAIRVAL, "Maxval"},
	{0, 0.5, 3, MINSTAIRVAL, "Minval"},
	{0, 0.5, 3, AFC_PROPORTION, "proportion Down/Right"},
	{0, 10, 0, CORRECTION_LEAVE_CRIT, "correct leave"},
	{0, 10, 0, CORRECTION_ENTRY_CRIT, "correct enter"},
	{0, 1.0, 2, COVARY_XPOS, "xpos covariance"},
	{0, 10, 0, BONUS_CRITERION, "Bonus resp level"},
	{0, 10, 0, BONUS2_CRITERION, "Bonus 2 level"},
	NULLPROP,
};


extern char *toggle_strings[];
extern long optionflag,option2flag;
extern Expt expt;
extern AFCstructure afc_s;

int SetProperty(Expt *exp, Stimulus *st, int code, float val);
float GetProperty(Expt *exp, Stimulus *st, int code);
float ExptProperty(Expt *exp, int flag);


Widget aframe = 0;
static Fpslider *aslider;
extern Wurtz *fix;

void handle_pushbuttons(Widget w, XtPointer data, XmPushButtonCallbackStruct *cbs);
void toggle_callback(Widget item, int bit, XmToggleButtonCallbackStruct *cbs);


/********************************************************************************/
Widget afc_popup(Widget parent, Stimulus *st, Cursor cursor, Winloc pos)
{
	Widget dialog,toggle_box,item,w,litem;
	Arg args[20];
	int n = 0,i = 0,x,y,hstep,totalh;
	Dimension oh;
	Fpslider *fps;
	void toggle_callback();

	XmString str1;
	float value;
	char *ctr;

	fix = &(st->fix);
	if(aframe != NULL) /* already exists - just show */
	{
		XtManageChild(aframe);
		XtPopup(XtParent(aframe),XtGrabNone);
		return;
	}

	if(pos.x > 0 || pos.y > 0){
	  XtSetArg(args[n],XmNx, pos.x); n++;
	  XtSetArg(args[n],XmNy, pos.y); n++;
	  XtSetArg(args[n], XmNdefaultPosition, FALSE); n++;
	}
	aframe = XmCreateFormDialog(parent, "wurtz", args, n);
	x = 10; 
	y =5;
	litem = my_make_button(aframe,"Done", CloseShell,
	    NULL, 0, (int)XtParent(aframe));

	AddCodesToSlider(asliderprops);
	aslider = make_multislider(aframe, asliderprops, 0, fix->timeout,
	    litem,NEXTRIGHTWIDGET_BIT, 0, afcfproc, afcfpset, 0);
	toggle_box = XtVaCreateWidget("togglebox",
	    xmRowColumnWidgetClass, aframe,
	    XmNtopAttachment, XmATTACH_WIDGET,
	    XmNtopWidget, litem,
	    XmNuserData, OPTION_CODE,
	    NULL);
	i = FIRST_AFC_BIT;
	while(toggle_strings[i] != NULL && i < LAST_AFC_BIT){
		w = XtVaCreateManagedWidget(toggle_strings[i],
		    xmToggleButtonGadgetClass, toggle_box, 
		    XmNset, (option2flag & (1<<(i-32))) ? TRUE : FALSE,NULL);
		XtAddCallback(w, XmNvalueChangedCallback, (XtCallbackProc)toggle_callback, (XtPointer)i);
		i++;
	}
	XtManageChild(toggle_box);
	XtManageChild(aframe);
	XDefineCursor(XtDisplay(aframe), XtWindow(aframe), cursor);
	fiterate(aframe, 0, 0);
	SetAPanel();
	XtPopup(XtParent(aframe), XtGrabNone);

}

/********************************************************************************/
void SetAItem(Wurtz *fix, Widget w)
{
  int what, i,j,n;
  char s[BUFSIZ];
  WidgetList l;

  XtVaGetValues(w, XmNuserData, &what, 0);
  switch(what)
    {
    case OPTION_CODE:
	XtVaGetValues(w, XmNnumChildren, &n, XmNchildren, &l, 0);
	for(i = 0; i < n; i++)
	  {
	    j = i + FIRST_AFC_BIT;
	    if(j < 32)
	    XtVaSetValues(l[i],
	    XmNset, (optionflag & (1<<j)) ? TRUE : FALSE,NULL);
	    else
	    XtVaSetValues(l[i],
	    XmNset, (option2flag & (1<<(j-32))) ? TRUE : FALSE,NULL);

	  }		  
    }
}

/********************************************************************************/
void SetAPanel()
{
  int i,n =0;
  WidgetList l;

  if(aframe != NULL)
    {
  for(i = 0; i < aslider->nprops; i++)
    set_fpslider(aslider, aslider->proplist[i].code, GetProperty(&expt,expt.st, aslider->proplist[i].code));
  XtVaGetValues(aframe, XmNnumChildren, &n, XmNchildren, &l, 0);
  for(i = 0; i < n; i++)
      SetAItem(&expt.st->fix, l[i]);
  }
}

/********************************************************************************/
void afcfpset(Sliderdat *fpd)
{
	fpd->val = GetProperty(&expt, expt.st, fpd->code);
}

/********************************************************************************/
void afcfproc(Fpslider *fp, float val, XEvent *event)
{
	switch(fp->code)
	{
	case REWARD_SIZE:
		fix->rwsize = (int)val;
		break;
	case TIMEOUT_CODE:
		fix->timeout = val;
		break;
	case WRONG_TIMEOUT_CODE:
		afc_s.wrongtimeout = val;
		break;
	case HSACCADE_VALUE:
		afc_s.sacval[0] = val;
		afc_s.abssac[0] = val;
		break;
	case VSACCADE_VALUE:
		afc_s.sacval[1] = val;
		afc_s.abssac[1] = val;
		break;
	case SAC_FIX_CRIT:
		afc_s.sac_fix_crit = val;
		break;
	case SACTARGSIZE:
		afc_s.targsize = deg2pix(val);
		break;
	case STARTVAL:
		afc_s.startvalue = val;
		afc_s.jstairval = afc_s.startvalue;
		reset_afc_counters();
		afc_s.stairhist = newstairstruct();
		break;
	case MAXSTAIRVAL:
		afc_s.maxstairvalue = val;
		reset_afc_counters();
		afc_s.stairhist = newstairstruct();
		break;
	case MINSTAIRVAL:
		afc_s.minstairvalue = val;
		reset_afc_counters();
		afc_s.stairhist = newstairstruct();
		break;
	default:
		SetProperty(&expt,expt.st,fp->code,val);
		break;
	}
	if(event == NULL || event_is_up(event))
	  SerialSend(fp->code);
}

/********************************************************************************/
stair_hist_s newstairstruct()
{
    stair_hist_s s;
    s = setup_stairstruct(&afc_s.startvalue, afc_s.stairfactor, afc_s.maxstairvalue, afc_s.minstairvalue);
    return(s);
}

/********************************************************************************/

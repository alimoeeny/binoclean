#import <stdio.h>
#import <string.h>
#import <sys/time.h>
#import <stdarg.h>
#import <math.h>
#import "mymath.h"
#import "myview.h"
#import "misc.h"
#import <GL/gl.h>
#import "commdefs.h"
#import "stimuli.h"
#import "WinDefs.h"
#import <Xm/List.h>
#import "protos.h"

extern Expt expt;
extern Display *D;
extern int mode;
extern FILE *seroutfile;
extern char *expname;
extern int option2flag;

#define TOGGLE_BOX 1
#define ONOFFBUTTON 2+MAXCHANS
#define SCALEOPTION 3

char *bwtoggle_strings[] = {
  "Channel1", "Channel2", "Channel3","Channel4",
  "Channel5", "Channel6", "Channel7","Channel8","Channel9","Channel10","Channel11","XY Left", 
"XY Conjugate",  "XY Persist", "graticule",
  "Spike Clear", "No XY Wrap", "Show FixWin",
  NULL
};
static char mssg[BUFSIZ];
static Fpslider *bwslider;


char *scalestrings[] = {
	"40",
	"20",
	"10",
	"5",
	"2",
	"1",
	"0.5",
	"0.2",
	NULL
};

static Sliderdat sliderprops[] =
{
  {0.0, 500.0, 0, TRIGGER_LEVEL, "Trigger (mV)"},
  {0.0, 2.0, 2, FIXATION_WINDOW, "Fixation Criterion"},
  {0.0, 2.0, 2, VERGENCE_WINDOW, "Vergence Criterion"},
  {0.0, 20, 1, STATIC_VERGENCE, "Mean Vergence"},
  {0.0, 20, 1, STATIC_CONJUGATE, "Mean Conjugate"},
  {0.0, 200, 0, UNIT_BINWIDTH, "Spike Bin Width"},
  {0, 10, 3, ELECTRODE_DEPTH, "Electrode Depth"},
  {0, 100, 0, SPIKE_GAIN, "Spike Gain"},
NULLPROP,
};
static Fpslider *bwslider;



Widget bwframe = 0;

void bwtoggle_callback(Widget item, int bit, XmToggleButtonCallbackStruct *cbs);
void SetBWPanel();
void handle_pushbuttons(Widget w, XtPointer data, 
XmPushButtonCallbackStruct *cbs);


Widget bw_popup(Widget parent, Cursor cursor, Winloc pos)
{
  Widget dialog,toggle_box,item,w,litem,rc;
  Arg args[20];
  int n = 0,i = 0,x,y,hstep,totalh;
  Dimension oh;
  Fpslider *fps;
  void bwtoggle_callback();

  XmString str1;
  float value;
  char *ctr;

  if(bwframe != NULL) /* already exists - just show */
  {
   XtManageChild(bwframe);
  SetBWPanel();
   XtPopup(XtParent(bwframe), XtGrabNone);
    return(bwframe);
  }

  AddCodesToSlider(sliderprops);
  if(pos.x > 0 || pos.y > 0){
    XtSetArg(args[n],XmNx, pos.x); n++;
    XtSetArg(args[n],XmNy, pos.y); n++;
    XtSetArg(args[n], XmNdefaultPosition, FALSE); n++;
  }
  bwframe = XmCreateFormDialog(parent, "BrainWave", args, n);
  litem = my_make_button(bwframe,"Done", CloseShell, NULL, 0, (int)XtParent(bwframe));
  item = my_make_button(bwframe,"Clear", handle_pushbuttons, litem, NEXTRIGHTWIDGET_BIT, SEND_CLEAR);
  item = my_make_button(bwframe,"Upload", handle_pushbuttons, item, NEXTRIGHTWIDGET_BIT, UPLOAD_CODE);
   item = my_make_text(bwframe,"","",bw_text_proc,
	   item, NEXTRIGHTWIDGET_BIT, 20, UPLOAD_CODE);
   item = my_make_text(bwframe,"Comment","",bw_text_proc,
	   litem, NEXTDOWNWIDGET_BIT, 40, UFF_COMMENT);
   bwslider = make_multislider(bwframe, sliderprops, 0, 0.0, 
  	item, NEXTDOWNWIDGET_BIT, 0, bwfproc, bwfpset, TRIGGER_LEVEL);
    toggle_box = XtVaCreateWidget("bwtoggle",
        xmRowColumnWidgetClass, bwframe,
	XmNtopAttachment, XmATTACH_WIDGET,
	XmNtopWidget, bwslider->rc,
	XmNuserData, TOGGLE_BOX,
        NULL);

    i = 0;
    while(bwtoggle_strings[i] != NULL){
      if(bwtoggle_strings[i][0] == 'C') /* define channel */
	{
      rc =  XtVaCreateWidget("bwsubtoggle",
        xmRowColumnWidgetClass, toggle_box,
	XmNuserData, i,
			     0);

        w = XtVaCreateManagedWidget(bwtoggle_strings[i],
            xmToggleButtonGadgetClass, rc, 
	XmNset, (expt.bwptr->cflag & (1<<i)) ? TRUE : FALSE,
	XmNuserData, ONOFFBUTTON, NULL);
        XtAddCallback(w, XmNvalueChangedCallback, (XtCallbackProc)bwtoggle_callback, (XtPointer)i);
	item = my_make_cycle(rc, "FSD", bw_cycle_proc, 
		w, NEXTRIGHTWIDGET_BIT, i, scalestrings,NULL);

      XtManageChild(rc);
    }
      else
	{
        w = XtVaCreateManagedWidget(bwtoggle_strings[i],
            xmToggleButtonGadgetClass, toggle_box, 
	XmNset, (expt.bwptr->cflag & (1<<i)) ? TRUE : FALSE,NULL);
        XtAddCallback(w, XmNvalueChangedCallback, (XtCallbackProc)bwtoggle_callback, (XtPointer)i);
      }
	i++;
    }
  expt.bwptr->nchans = i;
  XtManageChild(toggle_box);
   item = my_make_text(bwframe,"UFF prefix","C:\\bw\\data\\eyetest",bw_text_proc,
	   item, NEXTDOWNWIDGET_BIT, 20, UFF_PREFIX);
  item = my_make_numeric(bwframe, "Cluster", bwnumeric_proc,
		  item, NEXTRIGHTWIDGET_BIT, 0, 4, PLOT_CLUSTER);

   XtManageChild(bwframe);
	XDefineCursor(D, XtWindow(bwframe), cursor);
  SetBWPanel();
   fiterate(bwframe,0,0);
   XtPopup(XtParent(bwframe), XtGrabNone);
   return(bwframe);
}



void MakePosVisible(Widget w, int pos)
{
  int top, visible;

  XtVaGetValues(w, 
		XmNtopItemPosition, &top,
		XmNvisibleItemCount, &visible,
		NULL);
  if(pos < top)
    XmListSetPos(w, pos);
  else if(pos >= top+visible)
    XmListSetBottomPos(w, pos);

}


char *getfilename(char *path)
{
	char *p;

	if((p = strrchr(path, '/')) || (p= strrchr(path,'\\')))
		p++;
	else
		p = path;

	return((char *)strcpy((char *)malloc(strlen(p)+1), p));
}

void bwnumeric_proc(Widget w, int val, int data)
{
  char buf[256];
	switch(data)
	{
	case PLOT_CLUSTER:
	  expt.plotcluster = val;
	  plotexptbutton(NULL,NULL,NULL);
	  break;
	}

}


void SetFSD(Widget w, int chan)
{
  if(expt.bwptr->fsd[chan] > 30)
    set_option_value(w,0);
  else if(expt.bwptr->fsd[chan] > 15)
    set_option_value(w,1);
  else if(expt.bwptr->fsd[chan] > 7.5)
    set_option_value(w,2);
  else if(expt.bwptr->fsd[chan] > 2.5)
    set_option_value(w,3);
  else 
    set_option_value(w,4);
}

void SetToggles(Widget rc, int chan)
{
  WidgetList l;
  int n = 0,i,what;
  
  XtVaGetValues(rc, XmNnumChildren, &n, XmNchildren, &l, 0);
  for(i = 0; i < n; i++)
    {
      XtVaGetValues(l[i], XmNuserData, &what, 0);
      if(what < MAXCHANS)
      {
	SetFSD(l[i],chan);
      }
      if(what == ONOFFBUTTON)
		XtVaSetValues(l[i],XmNset, (expt.bwptr->cflag & (1<<chan)) ? TRUE : FALSE,NULL);

    }
}

void SetBWItem(Bwio *ex, Widget w)
{
  int what, i,n;
  WidgetList l;
  char temp[256];
	time_t tval;
	char *t,*r;

  XtVaGetValues(w, XmNuserData, &what, 0);
  switch(what)
    {
    case TOGGLE_BOX:
      XtVaGetValues(w, XmNnumChildren, &n, XmNchildren, &l, 0);
      for(i = 0; i < n; i++)
	{
	  XtVaGetValues(l[i], XmNuserData, &what, 0);
	  if(what < MAXCHANS)
	    SetToggles(l[i],i);
	}
      break;
    case UFF_PREFIX:
      if(option2flag & PSYCHOPHYSICS_BIT && expname)
	settextvalue(w, expname);
      else if(expt.bwptr->prefix != NULL && (r = strchr(expt.bwptr->prefix,'%')) != NULL && *(r+1) == 'd')
	  {
	    strcpy(temp,expt.bwptr->prefix);
	    r = strchr(temp,'%');
	    tval = time(NULL);
	    t = ctime(&tval);
	    t[7]=0;
	    t[10] = 0;
	    sprintf(r,"%s%s",&t[4],&t[8]);
	    settextvalue(w, temp);
	  }
      else
	settextvalue(w, expt.bwptr->prefix);
      break;
    case PLOT_CLUSTER:
      numeric_set_value(w, expt.plotcluster);
      break;
    }

}


void SetBWPanel()
{
  int i,n =0;
  WidgetList l;

  if(bwframe != NULL)
    {
  for(i = 0; i < bwslider->nprops; i++)
    set_fpslider(bwslider, bwslider->proplist[i].code, ExptProperty(&expt, bwslider->proplist[i].code));
  XtVaGetValues(bwframe, XmNnumChildren, &n, XmNchildren, &l, 0);
    for(i = 0; i < n; i++)
      SetBWItem(expt.bwptr, l[i]);
  }
}

void bwtoggle_callback(Widget item, int bit, XmToggleButtonCallbackStruct *cbs)
{


  if(cbs->set)
    expt.bwptr->cflag |= (1<<bit);
  else
    expt.bwptr->cflag &= ~(1<<bit);
  sprintf(mssg,"ch%d%c\n",bit, cbs->set ? '+':'-');
  SerialString(mssg,0);
}

void bwfproc(Fpslider *fp, float val, XEvent *event)
{
  int ival;
  int send = 1;

  if(fp->code < MAXSERIALCODES)
    sprintf(mssg,"%s%.2f\n",serial_strings[fp->code],val);
  switch(fp->code)
    {
    case SPIKE_GAIN:
      expt.spikegain = val;
      break;
    case ELECTRODE_DEPTH:
      send = 0;
      expt.electrode_depth = val;
      sprintf(mssg,"%s%.3f\n",serial_strings[fp->code],val);
      if(event == NULL || event_is_up(event))
	send = 1;
      break;
    case TRIGGER_LEVEL:
      expt.tl = val;
      break;
    case FIXATION_WINDOW:
      expt.fw = val;
      break;
    case VERGENCE_WINDOW:
      expt.vw = val;
      break;
    case STATIC_VERGENCE:
      expt.sv = val;
      break;
    case STATIC_CONJUGATE:
      expt.cj = val;
      break;
    case UNIT_BINWIDTH:
/* binwidth is in units of 1/10th ms on BW  multiplication done there*/
      expt.binw = val;
      break;
    }
  if(fp->code < MAXSERIALCODES && send)
    SerialString(mssg,0);
}

void bwfpset(Sliderdat *fpd)
{
      fpd->val = ExptProperty(&expt,fpd->code);
}

void bw_cycle_proc(Widget w, int id, int val)
{
  float fval = 0.0;

if(id < expt.bwptr->nchans)
  {
  switch(val)
    {
    default:
    case 0:
      sscanf(scalestrings[val],"%f",&expt.bwptr->fsd[id]);
      sprintf(mssg,"ch%d%c,%2s%s\n",id,(expt.bwptr->cflag & (1<<id)) ? '+' : '-',
	      channel_strings[EYESCALE_CODE],scalestrings[val]);
      break;
    case 10: /* a custom range */
      sprintf(mssg,"ch%d,fs%.1f\n",id,fval);
      break;
  }
  SerialString(mssg,0);
  }
}

int SetBWChannel(int chan, char *s)
{
  char *r,*t;
  int i,ival;


  r = s;
  if(*r == '+')
    expt.bwptr->cflag |= (1<<chan);
  else if(*r == '-')
    expt.bwptr->cflag &= ~(1<<chan);
  else
    return(-1);
  r++;

  i = 0;
  while(r && *r)
    {

  while((strncmp(r,channel_strings[i],2) != 0))
    if(channel_strings[++i] == NULL)
      return(-1);
  while(isalpha(*r)) /* does for space and commas */
    r++;
  switch(i)
    {
    case EYESCALE_CODE:
      sscanf(r,"%f",&expt.bwptr->fsd[chan]);
      break;
    case EYECOLOR_CODE:
      sscanf(r,"%d",&ival);
      if(ival > 0)
	expt.bwptr->colors[chan] = ival;
      break;
    }
  t = r;
  r = strchr(t,',');
  if(r)
    r++;

}
  
  
}

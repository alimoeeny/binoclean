#import <stdio.h>
#import <sys/time.h>
#import <stdarg.h>
#import <math.h>
#import "mymath.h"
#import "myview.h"
#import <GL/gl.h>
#import "commdefs.h"
#import "stimuli.h"
#import "misc.h"
#import "protos.h" 

#import "fpslider.h" //Ali
#import "myview.h" //Ali

#define ISI 5

Sliderdat wsliderprops[] =
{
	{0.0, 10., 1, TIMEOUT_CODE, "Timeout"},
	{0.0, 1., 2, SHORT_PREM_CODE, "Short Trial Len"},
	{-100.0, 100.0, 0, BRIGHTSHIFT_CODE, "Brightness Increment"},
	{0, 255, 0, BLANKCOLOR_CODE, "Timeout Color"},
	{0, 5, 2, WURTZ_RT_CODE, "Response Time"},
	{0, 500, 0, REWARD_SIZE, "Reward Size"},
	{0, 1.0, 2, STOP_CRITERION, "Stop Criterion"},
	NULLPROP,
};


extern char *toggle_strings[];
extern long optionflag,option2flag;
extern FILE *plotfd;
extern Expt expt;
extern float stopcriterion;
extern int goodtrials, totaltrials, premtrials, fixtrials,mode;

int SetProperty(Expt *exp, Stimulus *st, int code, float val);
float GetProperty(Expt *exp, Stimulus *st, int code);
float ExptProperty(Expt *exp, int flag);

void fsleep(float f);

Widget wframe = 0, loframe = 0,unframe = 0;
static Fpslider *wslider;
Wurtz *fix;
Log thelog;

void handle_pushbuttons(Widget w, XtPointer data, 
XmPushButtonCallbackStruct *cbs);
void toggle_callback(Widget item, int bit, XmToggleButtonCallbackStruct *cbs);

void wnumeric_proc(Widget w, int data, int val);
void wurtz_text_proc(Widget item, caddr_t client_data,
XmAnyCallbackStruct *cbs);
void SetWPanel();

void wurtzfpset(Sliderdat *fpd)
{
	fpd->val = GetProperty(&expt, expt.st, fpd->code);
}

void wurtzfproc(Fpslider *fp, float val, XEvent *event)
{
    SetProperty(&expt, expt.st, fp->code, val);
}


Widget wurtz_popup(Widget parent, Stimulus *st, Cursor cursor)
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
	if(wframe != NULL) /* already exists - just show */
	{
		XtManageChild(wframe);
		XtPopup(XtParent(wframe),XtGrabNone);
		return(wframe);
	}


	AddCodesToSlider(wsliderprops);
	wframe = XmCreateFormDialog(parent, "wurtz", args, n);
	x = 10; 
	y =5;
	litem = my_make_button(wframe,"Done", CloseShell,
	    NULL, 0, (int)XtParent(wframe));
	item = my_make_button(wframe,"State", handle_pushbuttons,
	     litem, NEXTRIGHTWIDGET_BIT, QUERY_STATE);
	wslider = make_multislider(wframe, wsliderprops, 0, fix->timeout,
	    item,NEXTRIGHTWIDGET_BIT, 0, wurtzfproc, wurtzfpset, 0);
	litem = my_make_numeric(wframe,"Mean Duration", wnumeric_proc,
	    litem, NEXTDOWNWIDGET_BIT, fix->fixframes, 4,WURTZ_DURATION_CODE);
	item = my_make_numeric(wframe,"Response Time", wnumeric_proc,
	    litem, NEXTRIGHTWIDGET_BIT, fix->stimframes, 4,WURTZ_RT_CODE);
	toggle_box = XtVaCreateWidget("togglebox",
	    xmRowColumnWidgetClass, wframe,
	XmNtopAttachment, XmATTACH_WIDGET,
	    XmNtopWidget, litem,
	    XmNuserData, OPTION_CODE,
	    NULL);
	i = FIRST_WURTZ_BIT;
	while(toggle_strings[i] != NULL && i < FIRST_EXPT_BIT){
		w = XtVaCreateManagedWidget(toggle_strings[i],
		    xmToggleButtonGadgetClass, toggle_box, 
		    XmNset, (optionflag & (1<<i)) ? TRUE : FALSE,NULL);
		XtAddCallback(w, XmNvalueChangedCallback, toggle_callback, (XtPointer)i);
		i++;
	}
	XtManageChild(toggle_box);
	XtManageChild(wframe);
	XDefineCursor(XtDisplay(wframe), XtWindow(wframe), cursor);
	fiterate(wframe, 0, 0);
	SetWPanel();
	XtPopup(XtParent(wframe), XtGrabNone);
	return(wframe);

}

void SaveLog(Widget fs, XtPointer client_data,
XmFileSelectionBoxCallbackStruct *cbs)
{
  FILE *fd;
  time_t t;
  int i;

  if((int)client_data == 1){
    for(i = 0; i < 4; i++)
      {
	expt.softoff[i] = thelog.chan[i].softoff;
      }
    SetAllPanel(&expt);
    return;
  }
  else if((int)client_data == 2){
    for(i = 0; i < 4; i++)
      {
	thelog.chan[i].softoff = expt.softoff[i];
      }
    SetWPanel();
  }

  if(expt.logfile == NULL)
    return;
  if((fd = fopen(expt.logfile,"a")) == NULL)
    {
      acknowledge("Can't open log",NULL);
      return;
    }
  
  mode |= SAVED_LOG;
  t = time(NULL);
  if((option2flag & PSYCHOPHYSICS_BIT))
    fprintf(fd,"\nSession end at %s\n",ctime(&t));
  else
    {
    fprintf(fd,"\n%s",ctime(&t));
  fprintf(fd,"we %.2f %dml fluid %d/%d, %d prem %d badfix \n",
	  thelog.weight, thelog.fluid,goodtrials,totaltrials,premtrials,fixtrials);
  fprintf(fd,"re %s %s\n",thelog.restrain, thelog.free);
  fprintf(fd,"rh %.2f %.2f %.2f %d\n",thelog.chan[0].gain, thelog.chan[0].offset,thelog.chan[0].phase,thelog.chan[0].softoff);
  fprintf(fd,"lh %.2f %.2f %.2f %d\n",thelog.chan[1].gain, thelog.chan[1].offset,thelog.chan[1].phase,thelog.chan[1].softoff);
  fprintf(fd,"rv %.2f %.2f %.2f %d\n",thelog.chan[2].gain, thelog.chan[2].offset,thelog.chan[2].phase,thelog.chan[2].softoff);
  fprintf(fd,"lv %.2f %.2f %.2f %d\n",thelog.chan[3].gain, thelog.chan[3].offset,thelog.chan[3].phase,thelog.chan[3].softoff);
}
  fprintf(fd,"co %s\n",thelog.comment);
  fclose(fd);
}

Widget log_popup(Widget parent, Stimulus *st, Cursor cursor)
{
	Widget dialog,toggle_box,item,w,litem,gitem,sitem, pitem;
	Arg args[20];
	int n = 0,i = 0,x,y,hstep,totalh;
	Dimension oh;
	Fpslider *fps;
	void toggle_callback();

	XmString str1;
	float value;
	char *ctr,s[BUFSIZ];

	fix = &(st->fix);
	if(loframe != NULL) /* already exists - just show */
	{
		XtManageChild(loframe);
		XtPopup(XtParent(loframe),XtGrabNone);
		return(loframe);
	}

	loframe = XmCreateFormDialog(parent, "DailyLog", args, n);
	x = 10; 
	y =5;
	litem = my_make_button(loframe,"Done", CloseShell,
	    NULL, 0, (int)XtParent(loframe));
	item = my_make_button(loframe,"Save", SaveLog,
	   litem, NEXTRIGHTWIDGET_BIT, 0);
	item = my_make_button(loframe,"SoftOff->Log", SaveLog,
	   item, NEXTRIGHTWIDGET_BIT, 2);
	item = my_make_button(loframe,"Apply Softoff", SaveLog,
	   item, NEXTRIGHTWIDGET_BIT, 1);
	sprintf(s,"");
	item = my_make_text(loframe,"File",s,wurtz_text_proc,litem,
		NEXTDOWNWIDGET_BIT,10,LOGFILE_CODE);
	litem = my_make_text(loframe,"Weight",s,wurtz_text_proc,item,
		NEXTDOWNWIDGET_BIT,10,WEIGHT_TEXT);
	item = my_make_text(loframe,"Drank:",s,wurtz_text_proc,item,
		NEXTRIGHTWIDGET_BIT,10,FLUID_TEXT);
	litem = my_make_text(loframe,"Restrained from:",s,wurtz_text_proc,litem,
		NEXTDOWNWIDGET_BIT,10,RESTRAIN_TEXT);
	item = my_make_text(loframe,"To:",s,wurtz_text_proc,litem,
		NEXTRIGHTWIDGET_BIT,10,FREE_TEXT);
	litem = my_make_text(loframe,"Comment",s,wurtz_text_proc,litem,
			     NEXTDOWNWIDGET_BIT,30,LOGCOMMENT);
	item = my_make_message(loframe,"Channel  Offset",litem,NEXTDOWNWIDGET_BIT,6,0);
	litem = my_make_text(loframe,"Right H",s,wurtz_text_proc,item,
		NEXTDOWNWIDGET_BIT,5,RIGHTH_OFFSET);
	gitem = my_make_text(loframe,"",s,wurtz_text_proc,litem,
		NEXTRIGHTWIDGET_BIT,5,RIGHTH_GAIN);
	pitem = my_make_text(loframe,"",s,wurtz_text_proc,gitem,
		NEXTRIGHTWIDGET_BIT,5,RIGHTH_PHASE);
	sitem = my_make_text(loframe,"",s,wurtz_text_proc,pitem,
		NEXTRIGHTWIDGET_BIT,5,RIGHTH_SOFTOFF);
	item = my_make_message(loframe,"Gain",gitem,NEXTUPWIDGET_BIT,6,0);
	item = my_make_message(loframe,"Phase",pitem,NEXTUPWIDGET_BIT,6,0);
	item = my_make_message(loframe,"SoftOff",sitem,NEXTUPWIDGET_BIT,6,0);
	litem = my_make_text(loframe," Left H",s,wurtz_text_proc,litem,
		NEXTDOWNWIDGET_BIT,5,LEFTH_OFFSET);
	gitem = my_make_text(loframe,"",s,wurtz_text_proc,litem,
		NEXTRIGHTWIDGET_BIT,5,LEFTH_GAIN);
	pitem = my_make_text(loframe,"",s,wurtz_text_proc,gitem,
		NEXTRIGHTWIDGET_BIT,5,LEFTH_PHASE);
	sitem = my_make_text(loframe,"",s,wurtz_text_proc,pitem,
		NEXTRIGHTWIDGET_BIT,5,LEFTH_SOFTOFF);
	litem = my_make_text(loframe,"Right V",s,wurtz_text_proc,litem,
		NEXTDOWNWIDGET_BIT,5,RIGHTV_OFFSET);
	gitem = my_make_text(loframe,"",s,wurtz_text_proc,litem,
		NEXTRIGHTWIDGET_BIT,5,RIGHTV_GAIN);
	pitem = my_make_text(loframe,"",s,wurtz_text_proc,gitem,
		NEXTRIGHTWIDGET_BIT,5,RIGHTV_PHASE);
	sitem = my_make_text(loframe,"",s,wurtz_text_proc,pitem,
		NEXTRIGHTWIDGET_BIT,5,RIGHTV_SOFTOFF);
	litem = my_make_text(loframe," Left V",s,wurtz_text_proc,litem,
		NEXTDOWNWIDGET_BIT,5,LEFTV_OFFSET);
	gitem = my_make_text(loframe,"",s,wurtz_text_proc,litem,
		NEXTRIGHTWIDGET_BIT,5,LEFTV_GAIN);
	pitem = my_make_text(loframe,"",s,wurtz_text_proc,gitem,
		NEXTRIGHTWIDGET_BIT,5,LEFTV_PHASE);
	sitem = my_make_text(loframe,"",s,wurtz_text_proc,pitem,
		NEXTRIGHTWIDGET_BIT,5,LEFTV_SOFTOFF);
	   XtManageChild(loframe);
	fiterate(loframe,0,0);
	XDefineCursor(XtDisplay(loframe), XtWindow(loframe), cursor);
	SetWPanel();
   XtPopup(XtParent(loframe), XtGrabNone);
   return(loframe);
}

void clearlogitem(int code)
{
  Widget item, titem;

  if((item = FindWidgetChild(unframe, code)) != NULL)
	settextvalue(item, "?");
}

void addcodetobuf(char *buf, int code)
{
  char *s,temp[256];
  Widget item, titem;
  time_t t;

  if((item = FindWidgetChild(unframe, code))){
    if((titem = FindWidgetChild(item, code))){
      s = XmTextFieldGetString(titem);
      sprintf(temp,"%s%s,",serial_strings[code],s);
      strcat(buf,temp);
    }
  }
}


void SaveUnitLog()
{
  char buf[256],*s,temp[256],c;
  Widget item, titem;
  time_t t;
  int i;

  t = time(NULL);
  SerialSend(RF_DIMENSIONS);
  sprintf(buf,"%s=",serial_strings[UFF_COMMENT]);
  addcodetobuf(buf,PENETRATION_TEXT);
  strcat(buf,"\n");
  while(strlen(buf) >= 80)
    {
      i = 80;
      while(buf[i] != ',')
	i--;
      buf[i++] = '\n';
      c = buf[i];
      buf[i] = 0;
      SerialString(buf,0);
      buf[i] = c;
      sprintf(temp,"%s",&buf[i]);
      sprintf(buf,"%s=%s\n",serial_strings[UFF_COMMENT],temp);
    }
  SerialString(buf,0);
  mode &= (~SAVE_UNITLOG);
}

Widget unit_popup(Widget parent, Stimulus *st, Cursor cursor)
{
	Widget dialog,toggle_box,item,w,litem,gitem,sitem, pitem,rc;
	Arg args[20];
	int n = 0,i = 0,x,y,hstep,totalh;
	Dimension oh;
	Fpslider *fps;
	void toggle_callback();

	XmString str1;
	float value;
	char *ctr,s[BUFSIZ];

	fix = &(st->fix);
	if(unframe != NULL) /* already exists - just show */
	{
		XtManageChild(unframe);
		XtPopup(XtParent(unframe),XtGrabNone);
		return(unframe);
	}
	if(parent == NULL)
	  return(NULL);

	unframe = XmCreateFormDialog(parent, "UnitLog", args, n);
	x = 10; 
	y =5;
	litem = my_make_button(unframe,"Done", CloseShell,
	    NULL, 0, (int)XtParent(unframe));
	item = my_make_button(unframe,"Save", SaveUnitLog,
	   litem, NEXTRIGHTWIDGET_BIT, 0);
	item = my_make_button(unframe,"Clear Unit Fields", handle_pushbuttons,
	   item, NEXTRIGHTWIDGET_BIT, CLEAR_LOG_BUTTON);
	sprintf(s,"%s",expt.pnum);
	item = my_make_text(unframe,"Penetration",s,wurtz_text_proc,item,
		NEXTRIGHTWIDGET_BIT,10,PENETRATION_TEXT);
	sprintf(s,"");
	rc = XtVaCreateWidget("unitrc",
			      xmRowColumnWidgetClass, unframe,
			      XmNtopAttachment, XmATTACH_WIDGET,
			      XmNtopWidget, litem,
			      NULL);

	litem = my_make_text(rc,"Orientation",s,wurtz_text_proc,NULL,
		0,10,ORIENTATION);

	   XtManageChild(rc);
	   XtManageChild(unframe);
	fiterate(unframe,0,0);
	XDefineCursor(XtDisplay(unframe), XtWindow(unframe), cursor);
   XtPopup(XtParent(unframe), XtGrabNone);
   return(unframe);
}

char *EyeCoilString(void)
{

  static char buf[BUFSIZ*2];
  char cbuf[BUFSIZ*2];
  
  buf[0] = 0;
  sprintf(buf,"CoilSettings RH %.2f %.2f %.2f",thelog.chan[0].gain,thelog.chan[0].offset,thelog.chan[0].phase);
  sprintf(cbuf," LH %.2f %.2f %.2f",thelog.chan[1].gain,thelog.chan[1].offset,thelog.chan[1].phase);
  strcat(buf,cbuf);
  sprintf(cbuf," RV %.2f %.2f %.2f",thelog.chan[2].gain,thelog.chan[2].offset,thelog.chan[2].phase);
  strcat(buf,cbuf);
  sprintf(cbuf," LV %.2f %.2f %.2f",thelog.chan[3].gain,thelog.chan[3].offset,thelog.chan[3].phase);
  strcat(buf,cbuf);
  return(buf);
}


void wurtz_text_proc(Widget item, caddr_t client_data,
XmAnyCallbackStruct *cbs)
{
  int what,i,n,pen;
  char *s,buf[256];
   WidgetList l;
  float val;

	XtVaGetValues(item, XmNuserData, &what, 0);
	s = XmTextFieldGetString(item);
	sscanf(s,"%i",&i);
	if(cbs->reason == XmCR_ACTIVATE || cbs->reason == XmCR_VALUE_CHANGED)
	  {
	    switch(what)
	      {
	      case PENETRATION_TEXT:
		SetExptString(&expt, expt.st, what, s);
		break;
	      case ORIENTATION:
		if(cbs->reason == XmCR_ACTIVATE && isdigit(*s))
		  {
		  ShowBox(expt.rf,0);
		  expt.rf->angle = i * 10;
		  ShowBox(expt.rf,RF_COLOR);
		}
		break;
	      case SF:
	      case TF:
		if(cbs->reason == XmCR_ACTIVATE && isdigit(*s))
		  {
		  sscanf(s,"%f",&val);
		  SetStimulus(expt.st, val, what, NULL);
		}
		break;
	      case LOGFILE_CODE:
		expt.logfile = myscopy(expt.logfile,nonewline(s));
		break;
	      case WEIGHT_TEXT:
		sscanf(s,"%f",&val);
		thelog.weight = val;
		break;
	      case RESTRAIN_TEXT:
		thelog.restrain = myscopy(thelog.restrain,s);
		break;
	      case LOGCOMMENT:
		thelog.comment = myscopy(thelog.comment,s);
		break;
	      case FREE_TEXT:
		thelog.free = myscopy(thelog.free,s);
		break;
	      case FLUID_TEXT:
		sscanf(s,"%d",&thelog.fluid);
		break;
	      case RIGHTH_OFFSET:
		sscanf(s,"%f",&thelog.chan[0].offset);
		break;
	      case RIGHTH_GAIN:
		sscanf(s,"%f",&thelog.chan[0].gain);
		break;
	      case RIGHTH_PHASE:
		sscanf(s,"%f",&thelog.chan[0].phase);
		break;
	      case RIGHTH_SOFTOFF:
		sscanf(s,"%d",&thelog.chan[0].softoff);
		  break;
	      case RIGHTV_OFFSET:
		sscanf(s,"%f",&thelog.chan[2].offset);
		break;
	      case RIGHTV_GAIN:
		sscanf(s,"%f",&thelog.chan[2].gain);
		break;
	      case RIGHTV_PHASE:
		sscanf(s,"%f",&thelog.chan[2].phase);
		break;
	      case RIGHTV_SOFTOFF:
		sscanf(s,"%d",&thelog.chan[2].softoff);
		  break;
	      case LEFTH_OFFSET:
		sscanf(s,"%f",&thelog.chan[1].offset);
		break;
	      case LEFTH_GAIN:
		sscanf(s,"%f",&thelog.chan[1].gain);
		break;
	      case LEFTH_PHASE:
		sscanf(s,"%f",&thelog.chan[1].phase);
		break;
	      case LEFTH_SOFTOFF:
		sscanf(s,"%d",&thelog.chan[1].softoff);
		  break;
	      case LEFTV_OFFSET:
		sscanf(s,"%f",&thelog.chan[3].offset);
		break;
	      case LEFTV_GAIN:
		sscanf(s,"%f",&thelog.chan[3].gain);
		break;
	      case LEFTV_PHASE:
		sscanf(s,"%f",&thelog.chan[3].phase);
		break;
	      case LEFTV_SOFTOFF:
		sscanf(s,"%d",&thelog.chan[3].softoff);
		  break;
	      }
	  }
	if(cbs->reason == XmCR_ACTIVATE)
	  {
	    switch(what)
	      {
	      case RIGHTH_SOFTOFF:
	      case RIGHTV_SOFTOFF:
	      case LEFTH_SOFTOFF:
	      case LEFTV_SOFTOFF:
		SerialSend(SOFTOFF_CODE);
		break;
	      }
	  }
	XtFree(s);
}

void SetWItem(Wurtz *fix, Widget w)
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
		    j = i + FIRST_WURTZ_BIT;
		    if(j < 32)
		    XtVaSetValues(l[i],
		    XmNset, (optionflag & (1<<j)) ? TRUE : FALSE,NULL);
		    else
		    XtVaSetValues(l[i],
		    XmNset, (option2flag & (1<<(j-32))) ? TRUE : FALSE,NULL);

		  }		  
    case WURTZ_DURATION_CODE:
      if(optionflag & WURTZ_WHOLE_STIMULI)
      numeric_set_value(w, (int)rint((float)fix->fixstims));
      else
      numeric_set_value(w, fix->fixframes);
      break;
    case WURTZ_RT_CODE:
      numeric_set_value(w, fix->stimframes);
      break;
    case WEIGHT_TEXT:
      sprintf(s,"%.2f",thelog.weight);
      break;
    case PENETRATION_TEXT:
      settextvalue(w,expt.pnum);
      break;
    case RESTRAIN_TEXT:
      if(thelog.restrain)
      sprintf(s,"%s",thelog.restrain);
      break;
    case FREE_TEXT:
      if(thelog.free)
      sprintf(s,"%s",thelog.free);      
      break;
    case LOGFILE_CODE:
      sprintf(s,"%s",expt.logfile);
      break;
    case RIGHTH_GAIN:
      sprintf(s,"%.2f",thelog.chan[0].gain);
      break;
    case RIGHTH_PHASE:
      sprintf(s,"%.2f",thelog.chan[0].phase);
      break;
    case RIGHTH_OFFSET:
      sprintf(s,"%.2f",thelog.chan[0].offset);
      break;
    case RIGHTH_SOFTOFF:
      sprintf(s,"%d",thelog.chan[0].softoff);
      break;
    case RIGHTV_GAIN:
      sprintf(s,"%.2f",thelog.chan[2].gain);
      break;
    case RIGHTV_PHASE:
      sprintf(s,"%.2f",thelog.chan[2].phase);
      break;
    case RIGHTV_OFFSET:
      sprintf(s,"%.2f",thelog.chan[2].offset);
      break;
    case RIGHTV_SOFTOFF:
      sprintf(s,"%d",thelog.chan[2].softoff);
      break;
    case LEFTV_GAIN:
      sprintf(s,"%.2f",thelog.chan[3].gain);
      break;
    case LEFTV_PHASE:
      sprintf(s,"%.2f",thelog.chan[3].phase);
      break;
    case LEFTV_OFFSET:
      sprintf(s,"%.2f",thelog.chan[3].offset);
      break;
    case LEFTV_SOFTOFF:
      sprintf(s,"%d",thelog.chan[3].softoff);
      break;
    case LEFTH_GAIN:
      sprintf(s,"%.2f",thelog.chan[1].gain);
      break;
    case LEFTH_PHASE:
      sprintf(s,"%.2f",thelog.chan[1].phase);
      break;
    case LEFTH_OFFSET:
      sprintf(s,"%.2f",thelog.chan[1].offset);
      break;
    case LEFTH_SOFTOFF:
      sprintf(s,"%d",thelog.chan[1].softoff);
      break;
    }
  switch(what)
    {
    case WEIGHT_TEXT:
    case RESTRAIN_TEXT:
    case FREE_TEXT:
    case LOGFILE_CODE:
    case RIGHTH_GAIN:
    case RIGHTH_PHASE:
    case RIGHTH_OFFSET:
    case RIGHTH_SOFTOFF:
    case RIGHTV_GAIN:
    case RIGHTV_PHASE:
    case RIGHTV_OFFSET:
    case RIGHTV_SOFTOFF:
    case LEFTH_GAIN:
    case LEFTH_PHASE:
    case LEFTH_OFFSET:
    case LEFTH_SOFTOFF:
    case LEFTV_GAIN:
    case LEFTV_PHASE:
    case LEFTV_OFFSET:
    case LEFTV_SOFTOFF:
      settextvalue(w, s);
    break;
    }

}

void SetWPanel()
{
  int i,n =0;
  WidgetList l;

  if(wframe != NULL)
    {
  for(i = 0; i < wslider->nprops; i++)
    set_fpslider(wslider, wslider->proplist[i].code, GetProperty(&expt,expt.st, wslider->proplist[i].code));
  XtVaGetValues(wframe, XmNnumChildren, &n, XmNchildren, &l, 0);
    for(i = 0; i < n; i++)
      SetWItem(&expt.st->fix, l[i]);
  }
  if(loframe != NULL)
    {
  XtVaGetValues(loframe, XmNnumChildren, &n, XmNchildren, &l, 0);
    for(i = 0; i < n; i++)
      SetWItem(&expt.st->fix, l[i]);
  }
}



void wnumeric_proc(Widget w, int val, int data)
{
	switch(data)
	{
	case WURTZ_DURATION_CODE:
	  if(optionflag & WURTZ_WHOLE_STIMULI)
	    {
	      fix->fixstims = val;
		fix->fixframes = val * expt.st->nframes;
	      }
	  else
		fix->fixframes = val;
		break;
	case WURTZ_RT_CODE:
		fix->stimframes = val;
		SerialSend(data);
		break;
	}
}

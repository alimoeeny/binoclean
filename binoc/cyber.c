#include <stdio.h> 
#include <termios.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/time.h> 
#include <stdarg.h> 
#include <math.h> 
#include "mymath.h" 
#include "myview.h"
#include "misc.h"
#include <GL/glx.h>
#include <GL/gl.h>
#include <Xm/FileSB.h> 
#include <Xm/MwmUtil.h> 
#include <X11/cursorfont.h>
#include "serial.h"
#include "cyber.h"
void fsleep(float f);

#define CHARCODES_DEFINE 1
#define STAIROUT 1

extern int ttys[];


Cyber cyberprops[8];
static FILE *cyberfd = NULL;

#define NO_SERIAL_PORT 1
#define QUIT_BUTTON 1
#define FIRST_CYBERPROP 2
#define ACN_COUPLE 2
#define ACP_COUPLE 3
#define CYBERSLIDER 4
#define LOWPASS 5
#define PREGAIN 6
#define OUTGAIN 7
#define LAST_CYBERPROP 7
#define CYBERCHANNEL 5

#define CYBERCODES 500
#define DC_OFFSET (CYBERCODES)

void FCloseShell(Widget w, Widget shell);
void handle_pushbuttons(Widget w, XtPointer data, 
XmPushButtonCallbackStruct *cbs);

static Sliderdat cyberslider[] ={
	{ -3, 3, 3, DC_OFFSET, "DC offset"},
	{ -3, 3, 3, 0, "Dummy"},
	NULLPROP,
};

char *prestrings[] = {
"1",
"10",
"100",
NULL,
};

char *outstrings[] = {
"1",
"2",
"5",
"10",
"20",
"50",
"100",
"200",
NULL,
};

char *acstrings[] = {
"GND",
"DC",
"0.1",
"1",
"10",
"30",
"100",
"300",
NULL,
};

char *lowpstrings[] = {
"Bypass",
"100 Hz",
"1 kHz",
"2 kHz",
"4 kHz",
"6 kHz",
"8 kHz",
"10 kHz",
"12 kHz",
"14 kHz",
"16 kHz",
"20 kHz",
NULL,
};

int lowpvals[] = {
0,
100,
1000,
2000,
4000,
6000,
8000,
10000,
12000,
14000,
16000,
20000,
};

static int cyberchan = 1;
static Fpslider *cyberfp;
Widget lform;
int cybertty = -1;

void cyber_cycle_proc(Widget w, int id, int val);


void ctest_button()
{
  char buf[256],cbuf[256],temp[256];
  int i;
  
	  sprintf(cbuf,"");
	  for(i = 0; i < 8; i++){
	    sprintf(temp,"%s%d %d %d %d %d %d %.4f\n","cy",i+1,
		    cyberprops[i].gain[0],cyberprops[i].gain[1],cyberprops[i].ac[0],cyberprops[i].ac[1],
		    cyberprops[i].lp,cyberprops[i].dc);
	    strcat(cbuf,temp);
	  }
	  puts(cbuf);

  sprintf(buf,"AT1S+\n");
  printf("%s",buf);
  SerialString(buf,cybertty);
  if(cyberfd != NULL)
    fprintf(cyberfd,"%s\n",buf);
}

float CyberProperty(Cyber *cp, int code)
{
  float val;

  switch(code){
  case DC_OFFSET:
    val = cp->dc;
    break;
  default:
    val = 0;
    break;
  }
  return(val);
}

void cyberfpset(Sliderdat *fpd)
{     
 fpd->val = CyberProperty(&cyberprops[cyberchan-1], fpd->code);
}

void SendCyberProp(int channel, int prop)
{
  float val;
  char buf[256];
  Cyber *cp = &cyberprops[channel-1];
  /* Channel- because they are numbered 1-8 */

  switch(prop)
    {
    case DC_OFFSET:
      val = cyberprops[channel-1].dc;
      if(val > 0)
	sprintf(buf,"AT1D%d+%d\n",channel,(int)(val*1000000));
      else
	sprintf(buf,"AT1D%d%d\n",channel,(int)(val*1000000));
      break;
    case ACP_COUPLE:
      sprintf(buf,"AT1C%d+%s\n",channel,acstrings[cp->ac[0]]);
      break;
    case ACN_COUPLE:
      sprintf(buf,"AT1C%d-%s\n",channel,acstrings[cp->ac[1]]);
      break;
    case LOWPASS:
      if(cp->lp > 0)
	sprintf(buf,"AT1F%d %d\n",channel,cp->lp);
      else
	sprintf(buf,"AT1F%d -\n",channel);
      break;
    case PREGAIN:
      sprintf(buf,"AT1G%dP%s\n",channel,prestrings[cp->gain[0]]);
      break;
    case OUTGAIN:
      sprintf(buf,"AT1G%dO%s\n",channel,outstrings[cp->gain[1]]);
      break;
    default: /* don't send any output */
      return;
    }
      SerialString(buf,cybertty);
  if(cyberfd != NULL)
    fprintf(cyberfd,"%s\n",buf);
}

void cyberfproc(Fpslider *fp, float val, XEvent *event)
{
  int i;
  char buf[256];

  switch(fp->code)
    {
    case DC_OFFSET:
      cyberprops[cyberchan-1].dc = val;
      SendCyberProp(cyberchan,fp->code);
      break;
    default:
      printf("unknown %d\n",fp->code);
      break;
    }
}

void SendAllCyberProps(int channel)
{
  int i;
  for(i = FIRST_CYBERPROP; i <= LAST_CYBERPROP; i++){
    SendCyberProp(channel, i);
    /* 
     * if this is only 0.01 the cyberamp can't keep up!
     */
    fsleep(0.05);
  }
}

void SendAllCyberChannels()
{
  int i;
  for(i = 1; i <9; i++)
    SendAllCyberProps(i);
}

void SetCyberItem(Cyber *cp, Widget w)
{
  int what;

	XtVaGetValues(w, XmNuserData, &what, 0);
	switch(what)
	{
	case ACP_COUPLE:
	  set_option_value(w, cp->ac[0]);
	  break;
	case ACN_COUPLE:
	  set_option_value(w, cp->ac[1]);
	  break;
	case PREGAIN:
	  set_option_value(w, cp->gain[0]);
	  break;
	case OUTGAIN:
	  set_option_value(w, cp->gain[1]);
	  break;
	case LOWPASS:
	  set_option_value(w, cp->lp);
	  break;
	}

}

void SetCyberPanel(Widget form)
{
	int i,n =0,j = 0;
	WidgetList l;

	if(lform == NULL && form == NULL)
		return;
	if(form == NULL)
	  form = lform;
	XtVaGetValues(form, XmNnumChildren, &n, XmNchildren, &l, 0);
	for(i = 0; i < n; i++)
		SetCyberItem(&cyberprops[cyberchan-1], l[i]);
	set_fpslider(cyberfp, DC_OFFSET, cyberprops[cyberchan-1].dc);
}

void cyber_numeric_proc(Widget w, int val, int data)
{
  if(val > 0 && val < 9)
    cyberchan = val;
  SetCyberPanel(lform);
}


Widget cyber_popup(Widget parent)
{
  Arg args[20];
  int n = 0,i;
  Widget litem,item;
  Fpslider *fps;

	if(lform != NULL)
	{
		XtManageChild(lform);
		XtPopup(XtParent(lform), XtGrabNone);
/* get this working first. 
Need to check with a PC what is coming don the line... */
/*		SendAllCyberChannels();*/
		cyberfd = fopen("cyber.out","w");
		return(lform);
	}
	n = 0;
	lform = XmCreateFormDialog(parent, "Control", args, n);
	litem = my_make_button(lform,"Done", FCloseShell, NULL, 0, (int)XtParent(lform));
	item = my_make_button(lform,"Status", ctest_button, 
	    litem, NEXTRIGHTWIDGET_BIT,(int)XtParent(lform));
	item = my_make_button(lform,"SetAll", SendAllCyberChannels, 
	    item, NEXTRIGHTWIDGET_BIT,(int)XtParent(lform));
	item = my_make_numeric(lform,"Channel",cyber_numeric_proc,
	    item, NEXTRIGHTWIDGET_BIT,1, 1, CYBERCHANNEL);
	litem = my_make_cycle(lform, "AC coupling (Hz)  +:", cyber_cycle_proc, 
		litem, NEXTDOWNWIDGET_BIT, ACP_COUPLE, acstrings,NULL);
	item = my_make_cycle(lform, "-:", cyber_cycle_proc, 
		litem, NEXTRIGHTWIDGET_BIT, ACN_COUPLE, acstrings,NULL);
	item = my_make_cycle(lform, "Prefilter Gain", cyber_cycle_proc, 
		litem, NEXTDOWNWIDGET_BIT, PREGAIN, prestrings,NULL);
	item = my_make_cycle(lform, "Output Gain", cyber_cycle_proc, 
		item, NEXTDOWNWIDGET_BIT, OUTGAIN, outstrings,NULL);
	item = my_make_cycle(lform, "LowPass", cyber_cycle_proc, 
		item, NEXTDOWNWIDGET_BIT, LOWPASS, lowpstrings,lowpvals);
	cyberfp = make_multislider(lform, cyberslider, 0, 0.0, 
       item, NEXTDOWNWIDGET_BIT, 0, cyberfproc, cyberfpset, CYBERSLIDER);
	XtManageChild(lform);
	XtPopup(XtParent(lform), XtGrabNone);
	SetCyberPanel(lform);
	/*	SendAllCyberChannels();*/
	return(lform);
}


void cyber_cycle_proc(Widget w, int id, int val)
{
  char buf[256];
  Cyber *cp = &cyberprops[cyberchan-1];

  switch(id)
    {
    case ACP_COUPLE:
      cp->ac[0] = val;
      break;
    case ACN_COUPLE:
      cp->ac[1] = val;
      break;
    case LOWPASS:
      cp->lp = val;
      break;
    case PREGAIN:
      cp->gain[0] = val;
      break;
    case OUTGAIN:
      cp->gain[1] = val;
      break;
    }
  SendCyberProp(cyberchan, id);
}

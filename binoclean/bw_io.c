//#import <stdio.h>
//#import <string.h>
//#import <sys/time.h>
//#import <stdarg.h>
//#import <math.h>
//#import "mymath.h"
//#import "myview.h"
//#import "misc.h"
//#import <OpenGL/Gl.x>
//#import "commdefs.h"
//#import "stimuli.h"
//#import "WinDefs.h"
//#import "protos.h"
//
//extern Expt expt;
//extern Display *D;
//extern int mode;
//extern FILE *seroutfile;
//extern char *expname;
//extern int option2flag;
//
//#define TOGGLE_BOX 1
//#define ONOFFBUTTON 2+MAXCHANS
//#define SCALEOPTION 3
//
//char *bwtoggle_strings[] = {
//  "Channel1", "Channel2", "Channel3","Channel4",
//  "Channel5", "Channel6", "Channel7","Channel8","Channel9","Channel10","Channel11","XY Left", 
//"XY Conjugate",  "XY Persist", "graticule",
//  "Spike Clear", "No XY Wrap", "Show FixWin",
//  NULL
//};
//static char mssg[BUFSIZ];
//static Fpslider *bwslider;
//
//
//char *scalestrings[] = {
//	"40",
//	"20",
//	"10",
//	"5",
//	"2",
//	"1",
//	"0.5",
//	"0.2",
//	NULL
//};
//
//static Sliderdat sliderprops[] =
//{
//  {0.0, 500.0, 0, TRIGGER_LEVEL, "Trigger (mV)"},
//  {0.0, 2.0, 2, FIXATION_WINDOW, "Fixation Criterion"},
//  {0.0, 2.0, 2, VERGENCE_WINDOW, "Vergence Criterion"},
//  {0.0, 20, 1, STATIC_VERGENCE, "Mean Vergence"},
//  {0.0, 20, 1, STATIC_CONJUGATE, "Mean Conjugate"},
//  {0.0, 200, 0, UNIT_BINWIDTH, "Spike Bin Width"},
//  {0, 10, 3, ELECTRODE_DEPTH, "Electrode Depth"},
//  {0, 100, 0, SPIKE_GAIN, "Spike Gain"},
//NULLPROP,
//};
//static Fpslider *bwslider;
//
//
//
//
//
//void SetBWPanel();
//
//
//
//char *getfilename(char *path)
//{
//	char *p;
//
//	if((p = strrchr(path, '/')) || (p= strrchr(path,'\\')))
//		p++;
//	else
//		p = path;
//
//	return((char *)strcpy((char *)malloc(strlen(p)+1), p));
//}
//
//
//

//
//void bwfpset(Sliderdat *fpd)
//{
//      fpd->val = ExptProperty(&expt,fpd->code);
//}
//
//
//
//int SetBWChannel(int chan, char *s)
//{
//  char *r,*t;
//  int i,ival;
//
//
//  r = s;
//  if(*r == '+')
//    expt.bwptr->cflag |= (1<<chan);
//  else if(*r == '-')
//    expt.bwptr->cflag &= ~(1<<chan);
//  else
//    return(-1);
//  r++;
//
//  i = 0;
//  while(r && *r)
//    {
//
//  while((strncmp(r,channel_strings[i],2) != 0))
//    if(channel_strings[++i] == NULL)
//      return(-1);
//  while(isalpha(*r)) /* does for space and commas */
//    r++;
//  switch(i)
//    {
//    case EYESCALE_CODE:
//      sscanf(r,"%f",&expt.bwptr->fsd[chan]);
//      break;
//    case EYECOLOR_CODE:
//      sscanf(r,"%d",&ival);
//      if(ival > 0)
//	expt.bwptr->colors[chan] = ival;
//      break;
//    }
//  t = r;
//  r = strchr(t,',');
//  if(r)
//    r++;
//
//}
//  
//  
//}

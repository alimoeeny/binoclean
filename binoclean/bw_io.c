#import <stdio.h>
#import <string.h>
#import <sys/time.h>
#import <stdarg.h>
#import <math.h>
#import "mymath.h"
#import "myview.h"
#import "misc.h"
#import "commdefs.h"
#import "stimuli.h"
#import "WinDefs.h"
#import "protos.h"

extern Expt expt;
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


//
//
//
//void SetBWPanel();
//


char *getfilename(char *path)
{
	char *p;
    
	if((p = strrchr(path, '/')) || (p= strrchr(path,'\\')))
		p++;
	else
		p = path;
    
	return((char *)strcpy((char *)malloc(strlen(p)+1), p));
}




//
//void bwfpset(Sliderdat *fpd)
//{
//      fpd->val = ExptProperty(&expt,fpd->code);
//}
//
//
//
int SetBWChannel(char *s)
{
    char *r,*t;
    int i,ival,onoff=0;
    int chan = 0;
    
    if((r = strchr(s,'+')) == NULL && (r = strchr(s,'-')) == NULL)
        return(0);
    sscanf(s,"%d",&chan);
    
    if(*r == '+'){
        expt.bwptr->cflag |= (1<<chan);
        onoff = 1;
    }
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
        
        return(onoff);
    }
    
    
}

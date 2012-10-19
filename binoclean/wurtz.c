#import <stdio.h>
#import <sys/time.h>
#import <stdarg.h>
#import <math.h>
#import "mymath.h"
#import "myview.h"
#import <OpenGL/OpenGL.h>#import <OpenGL/gl.h>
#import "commdefs.h"
#import "stimuli.h"
#import "misc.h"
#import "protos.h" 

#define ISI 5





extern long optionflag,option2flag;
extern FILE *plotfd;
extern Expt expt;
extern float stopcriterion;
extern int goodtrials, totaltrials, premtrials, fixtrials,mode;

int SetProperty(Expt *exp, Stimulus *st, int code, float val);
float GetProperty(Expt *exp, Stimulus *st, int code);
float ExptProperty(Expt *exp, int flag);

void fsleep(float f);

Wurtz *fix;
Log thelog;

void SetWPanel();

void SaveUnitLog()
{
    char buf[256],*s,temp[256],c;
    time_t t;
    int i;
    
    t = time(NULL);
    SerialSend(RF_DIMENSIONS);
    sprintf(buf,"%s=",serial_strings[UFF_COMMENT]);
    //Ali addcodetobuf(buf,PENETRATION_TEXT);
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





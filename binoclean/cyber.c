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
int cybertty = -1;



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

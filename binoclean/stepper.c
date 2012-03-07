#include <stdio.h> 
#include <termios.h> 

#include <term.h>
#define TCGETA TIOCGETA
#define TCSETA TIOCSETA
#undef clear_screen

#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/time.h> 
#include <stdarg.h> 
#include <sys/fcntl.h>
#include <signal.h>
#include <math.h> 
#include "mymath.h" 
#include "myview.h"
#include "misc.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#include "serial.h"
#include "commdefs.h"
#include "stimuli.h"
#include "WinDefs.h"
#include "stimdefs.h"
#include "sprotos.h"


/*
 *
 * Dec  8 2008. New gear box:
 *    188 steps = 1mm. Needs 0.76 steps per micron
 *
 
 * New motor with gear box, 2400 steps = 1mm
 * 24 steps = 1 micron. But in fact this is too big by a factor
 * of 2.5 so use 10 steps per micron 
 * this on controller #3
 * motor mag converts microns to steps
 */

char *motoridstrings[5] = {
    "1",
    "2",
    "3",
    "4",
    NULL
};
int motorids[4] = {1,2, 3, 4};


float motormags[4] = { 0.7933, 0.7933, 0.76, 0.7933};
/*
 * When there is resistance from tough dura, then the moment the motor 
 * is turned off, it bounces back. So there are times when the motor needs 
 *to stay on. The default is to turn it off - preventing heating, and 
 *reducing noise
 */
static int turnoff = 1; 

//#define MOTOR_MAG 1.60
#ifdef MICRODRIVE_VERSIONA

#define MOTOR_MAG 2.6 // Jan 2002, for new screw
char uversion[256] = "Ver 1";
static int motorid = 0;
#else

/*
 * New ball screw, Jun 2006. pitch 1mm. Motor 1.8 degrees/step, so
 * 200 steps/turn  = 1mm. So one step = 1/200 = 5 microns. Need scale factor of 0.2
 * for microns->steps. Why is this constant 0.8 - factor of 4 somewhere.
 *
 */
#define MOTOR_MAG 0.7933
char uversion[256] = "Ver 2";
static int motorid = 1;

#endif

#define MAXDEPTHS 2048

float depths[MAXDEPTHS];
float times[MAXDEPTHS];
static int totalsteps = 0;
static float timeoffset = 0;

float ExptProperty(Expt *exp, int flag);
void ElectrodeDepth(int depth);
void fsleep(float f);

#define CHARCODES_DEFINE 1
#define STAIROUT 1

int electrodeDepth = 0;
#define REVERSED_CABLE 1
static int motorinvert = 1;

extern FILE *penlog;
extern int ttys[];
extern struct timeval progstarttime;
extern int optionflags[];
//Ali #ifdef macosx
extern TTY save_t;
extern TTY save_ta;
//Ali #else
//Ali extern struct termio save_t;
//Ali extern struct termio save_ta;
//Ali #endif
extern float cleardelay;
extern Expt expt;

extern struct timeval cleartime, now;
static float stepvals[10] = {0,4,1,0,0,0,0,0};

static char *stepport = NULL;
static int fixedstepsize = 5;
static int electrodeDepthOffset = 10000;
struct termios Otermios_p;

typedef struct STEPPER{
    float vals[10];
} Stepper;

static Stepper stepper;
static FILE *stepfd = NULL;

#define DELAY_SCALE  1
#define STEP_SIZE  2
#define STEPSLIDER 3

#define STEP_FORWARD 5
#define STEP_BACK 6
#define BIGSTEP_UP 7
#define BIGSTEP_DOWN 8
#define BIGSTEP_CYCLE 9
#define CUSTOM_STEP 10
#define CUSTOM_UP 11
#define CUSTOM_DOWN 12
#define STEP_ELECTRODE_DEPTH 13
#define TEXT_STEP 14
#define SETDEPTH 15
#define STEP_CYCLE 16
#define MOVETODEPTH 17
#define MARK_START 18
#define DELAY_TEXT 19
#define MARK_WM 20
#define MARK_EXIT 21
#define MARK_INFRAGRANULAR 22
#define MARK_GM 23
#define MARK_TOP 24
#define MARK_V1END 25
#define ID_CYCLE 26
#define TOGGLE_ONOFF 27

#define QUERY_BIT (1<<20)
#define QUERY_MARK_WM (MARK_WM | (QUERY_BIT))
#define QUERY_MARK_GM (MARK_GM| (QUERY_BIT))
#define QUERY_MARK_INFRAGRANULAR (MARK_INFRAGRANULAR| (QUERY_BIT))
#define QUERY_MARK_EXIT (MARK_EXIT| (QUERY_BIT))

char *stepstrings[] = {"5","10","20","50","100","200",NULL};
int stepsizes[] = {5,10,20,50,100,200};

void toggle_callback();


int motorPort = -1;

void stepsetup(void);
void setNewPosition(int pos, int diff);
void NewPosition(int pos);
void setCurrentPosition(int pos);
void selectMotor(int motorNum);
float timediff(struct timeval *a, struct timeval *b);

//#define DELAY 10
#define SLIDER_MAG 5
#define SMALL_STEP 10.0
#define MEDIUM_STEP 20.0
#define LARGE_STEP 100.0
#define ELECTRODE_MAX 50000
#define MAX_MOTOR_POS 80000

//Ali #if defined(macosx)
#define CR 13
//Ali #else
//Ali #define CR 13
//Ali #endif

#define FIRST_RATE 8
#define MAX_RATE 30
#define ACCELERATION 200
#define STEP_SEC 1008

void delay(int delay){
    usleep(delay * 1000);
}

static void settimeout()
{
    fprintf(stderr, "Timeout opening port\n");
}

int OpenStepSerial(char *port)
{
    char buf[256];
    char str[256];
    struct termios termios_p;
    
    TTY t;
    
    
    struct itimerval itimer;
    long speed;
    int portstate;
    void (* sigfn)();
    
    int i = 0;
    stepport = port;
    
    if(ttys[0] != 0)
        i = 1;
    /*
     * On SGI, ttyd does not use RTS/CTS, so can use this to tell the 
     * Controller to turn on RTS/CTS (which then sets CTS high)
     * Once this is done, can open with /dev/ttyf1, which uses
     * CTS. CTS then used to poll when the motor has done its move
     */
    //Ali #if defined(macosx)
    if((ttys[i] = open(port,O_RDWR|O_NONBLOCK)) < 0)
        //Ali #else
        //Ali   if((ttys[i] = open("/dev/ttyd1",O_RDWR|O_NONBLOCK)) < 0)
        //Ali #endif
    {
        sprintf(buf,"Can't open %s",port);
        perror(buf);
        return(0);
    }
    alarm(0);
    motorPort = ttys[i];
    if(tcgetattr(ttys[i], &termios_p)) {
        perror("Can't get terminal properties");
        exit(-1);
    }
    if(tcgetattr(ttys[i], &termios_p)) {
        perror("Can't get terminal properties");
        return(-1);
    }
    
    /*
     * after first opening the port, set it to ignore CTS (VTIME = 0)
     * since the controller has the busy bit set at power up.
     * then reopen the port and set it up to wait for CTS
     */
    speed = B9600;
    cfsetospeed(&termios_p, speed);
    cfsetispeed(&termios_p, speed);
    termios_p.c_cc[VMIN] = 1;
    termios_p.c_cc[VTIME] = 0;
    termios_p.c_lflag &= ~(ECHO|ICANON|ISIG|ECHOE|ECHOK|ECHONL|CCTS_OFLOW);
    tcsetattr(motorPort, TCSANOW, &termios_p);
    
    /* send commands to controller to auto select BAUD rate */
    sprintf(str, "%c", CR);
    write(motorPort, str, strlen(str));
    delay(20);
    write(motorPort, str, strlen(str));
    delay(20);
    
    /* set mode to ASCII commands, enable CTS on bit 6 */
    sprintf(str, "O 0A0H%c", CR);
    i = write(motorPort, str, strlen(str)); delay(20);
    i = write(motorPort, str, strlen(str)); delay(20);
    termios_p.c_cc[VMIN] = 1;
    termios_p.c_cc[VTIME] = 0;
    termios_p.c_lflag &= ~(ECHO|ICANON|ISIG|ECHOE|ECHOK|ECHONL);
    // dont need to play wiht this here - just about to close port
    //  termios_p.c_lflag |= CCTS_OFLOW;
    tcsetattr(motorPort, TCSANOW, &termios_p);
    close(motorPort);
    usleep(10000);
    
    /* Now can open a port that Respects RTS/CTS */
    sigfn = signal(SIGALRM, settimeout);
    alarm(10);
    //  itimer.it_interval.tv_sec = 10;
    //  setitimer(ITIMER_REAL, &itimer, NULL);
    
    /*
     * on macosx (with Keyspan serial adapters) this will hang
     * if there is no Controller box attached, unless O_NONBLOCK is set.
     * Alarm should deal with this, but seems not to under macosx
     * problem is that I think this may stop it from blocking when the motor
     * is moved. Need to check this.
     */
    //  if((motorPort = open(port, O_RDWR|O_NONBLOCK)) == -1) {
    if((motorPort = open(port, O_RDWR|O_NONBLOCK)) == -1) {
        
        perror("Can't open serial port");
        acknowledge("Can't open serial port",NULL);
        return(-1);
    }
  	alarm(0);
	//  itimer.it_interval.tv_sec = 0;
	//  setitimer(ITIMER_REAL, &itimer, NULL);
	signal(SIGALRM, sigfn);
    
	if(ioctl(motorPort,TCGETA,&t) < 0)
	{
		perror("iocntl 1");
		exit(-1);	
	}
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cflag &= (~(PARENB|PARODD));
	t.c_ospeed = B9600;
	t.c_ispeed = B9600;
	// This is necessary to get CTS respsected 
    t.c_cflag |= (CCTS_OFLOW);
	t.c_cc[VEOF] = 0; /* 0 buffered char */
	t.c_cc[VEOL] = 0; /* return immediately between */
	t.c_cc[VMIN] = 0; /* 0 buffered char */
	t.c_cc[VTIME] = 0; /* return if no char in 0.l sec */
	if(ioctl(motorPort,TCSETA,&t) < 0)
	{
		perror("Ioctl 2");
		exit(-2);
	}
	usleep(10000);
    
#if !defined(WIN32)
    ioctl(motorPort,TIOCMGET,&portstate);
    if(!(portstate & TIOCM_CTS)){
        acknowledge("Controller CTS is low. Restart",NULL);
    }
#endif
    
    stepsetup();
    notifyPositionChange(electrodeDepth);
    return(motorPort);
}

int steptty_setup(int tty)
{
    
    TTY t;
    
	if(ioctl(tty,TCGETA,&t) < 0)
	{
		perror("iocntl 1");
		exit(-1);	
	}
	
	if(tty == ttys[0])
        save_t = t;
	else if(tty == ttys[1])
        save_ta = t;
	
	t.c_lflag &= ~(ICANON | ECHO);
    
	t.c_cflag &= (~PARENB);
#if defined(IRIX64) || defined(IRIX)
	t.c_ospeed = B9600;
#endif
    
    /*
     *  In This Mode, (VEOF and VEOL = 0) Read() calls return immediately
     *  returning 0 if there are no chars in the input buffer
     */
    
	t.c_cc[VEOF] = 0; /* 0 buffered char */
	t.c_cc[VEOL] = 0; /* return immediately between */
    
	if(ioctl(tty,TCSETA,&t) < 0)
	{
		perror("Ioctl 2");
		exit(-2);
	}
#if defined(IRIX64) || defined(IRIX)
	if(ioctl(tty,SIOC_ITIMER,0) < 0)
	{
		perror("Ioctl 2");
		exit(-2);
	}
#endif
	return(0);
}

void stepsetup(void)
{
    char str[256];
    
    /*
     * need the sleeps below or it doesn't all get processed
     */
    
    sprintf(str, "%c", CR); 
    SerialString(str, motorPort); fsleep(0.01);
    sprintf(str, "O 0A0H%c", CR);
    SerialString(str, motorPort); fsleep(0.01);
    
    sprintf(str, "F %c%c\n", FIRST_RATE, CR);
	SerialString(str,motorPort); fsleep(0.01);
    sprintf(str, "R %c%c", MAX_RATE, CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "S %c%c", ACCELERATION, CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "/B 7%c", CR);                     /* deselect motors */
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 1%c", CR);                      /* set B 1 high */
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "/B 2%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "/B 3%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "/B 4%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 2%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "/B 2%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 3%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 2%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "/B 2%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "/B 3%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 4%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 2%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "/B 2%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 3%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 2%c", CR);
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "B 7%c", CR);                      /* close latch */
	SerialString(str,motorPort);fsleep(0.01);
    sprintf(str, "A 0%c", CR);                      /* set current position to 0 */
	SerialString(str,motorPort);
}


void reopen_button()
{
    close(motorPort);
    OpenStepSerial(stepport);
}

void steptest_button()
{
    char buf[256],cbuf[256],temp[256];
    int i;
    
    printf("Testing Stepper\n");
    sprintf(cbuf,"");
    for(i = 0; i < 8; i++){
        sprintf(temp,"%s%d \n","cy",i+1);
        strcat(cbuf,temp);
    }
    puts(cbuf);
    SerialString(cbuf,motorPort);
    if(stepfd != NULL)
        fprintf(stepfd,"%s\n",cbuf);
    printf("Done\n");
}

void stepproc(float step)
{
    NewPosition(electrodeDepth+(int)(step * 1000));
}


float StepProperty(int code)
{
    float val;
    
    switch(code){
        case STEP_SIZE:
        default:
            val = stepvals[code];
            break;
    }
    return(val);
}
//Ali
#define NOEVENT 0

void SendStepProp(int prop)
{
    float val;
    char buf[256];
    
    switch(prop)
    {
        default:
            val = stepvals[prop];
            break;
    }
    SerialString(buf,motorPort);
    if(stepfd != NULL)
        fprintf(stepfd,"%s\n",buf);
}



void SendAllStepProps(int channel)
{
    int i;
    fsleep(0.05);
}



void SetMotorId(int id)
{
    motorid = id;
}

void selectMotor(int motorNum)
{
    char str[32];
    
    sprintf(str, "/B 7%c", CR); /* open output latch */
    write(motorPort, str, strlen(str)); delay(10);
    sprintf(str, "B 1%c", CR);  /* deselect current motor */
    write(motorPort, str, strlen(str)); delay(10);
    sprintf(str, "B 7%c", CR);  /* close latch */
    write(motorPort, str, strlen(str)); delay(10);
    sprintf(str, "/B 0%c", CR); /* enable all motors */
    write(motorPort, str, strlen(str)); delay(10);
    
    switch(motorNum) {
        case 1:
            /* these commands select motor 1 */
            sprintf(str, "/B 2%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            sprintf(str, "/B 3%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            sprintf(str, "/B 4%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            break;
        case 2:
            /* these commands select motor 2 */
            sprintf(str, "B 2%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            sprintf(str, "/B 3%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            sprintf(str, "/B 4%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            break;
        case 3:
            /* these commands select motor 3 */
            sprintf(str, "/B 2%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            sprintf(str, "B 3%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            sprintf(str, "/B 4%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            break;
        case 4:
            /* these commands select motor 4 */
            sprintf(str, "B 2%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            sprintf(str, "B 3%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            sprintf(str, "/B 4%c", CR);
            write(motorPort, str, strlen(str)); delay(10);
            break;
    }
    
    sprintf(str, "/B 7%c", CR);                             /* open output latch */
    write(motorPort, str, strlen(str)); delay(10);
    sprintf(str, "/B 1%c", CR);                             /* select current motor */
    write(motorPort, str, strlen(str)); delay(10);
    sprintf(str, "B 7%c", CR);                              /* close latch */
    write(motorPort, str, strlen(str)); delay(10);
    
    return;
}


void setCurrentPosition(int pos)
{
    char str[32];
    
	if(motorinvert)
        sprintf(str, "A %d%c", (int)((electrodeDepthOffset-pos) * motormags[motorid]),CR);
	else
        sprintf(str, "A %d%c", (int)((electrodeDepthOffset+pos) * motormags[motorid]),CR);
    write(motorPort, str, strlen(str));     delay(10);
#ifdef DEBUG
	printf("Stepper: %s\n",str);
#endif
    notifyPositionChange(electrodeDepth);
    return;
}

void setNewPosition(int pos, int diff)
{
    int delaySec;
    int delayMsec,i,charctr,j;
    char str[32];
	char waitStr[10],c;
	char readBuf[16];
	char inbuf[256];
	float fdiff = (float)(fabs(diff))/STEP_SEC;
	struct timeval start,end;
    int portstate;
    
    
#ifdef REVERSED_CABLEA
    sprintf(str, "P %d%c", (int)((electrodeDepthOffset+pos) * motormags[motorid]), CR);
#else
    sprintf(str, "P %d%c", (int)((electrodeDepthOffset-pos) * motormags[motorid]), CR);
#endif
#ifdef DEBUG
	printf("Stepper: %s\n",str);
#endif
    write(motorPort, str, strlen(str));     delay(10);
	//	fsleep(fdiff * stepvals[DELAY_SCALE] * 0.1);
    
	if (turnoff){
        /* deselect the current motor after motion is finished
         * this prevents the motor from getting too hot
         */
        sprintf(str, "/B 7%c", CR);                             /* open output latch */
        write(motorPort, str, strlen(str)); delay(10);
        sprintf(str, "B 1%c", CR);                              /* deselect current motor */
        write(motorPort, str, strlen(str)); delay(10);
        sprintf(str, "B 7%c", CR);                              /* close latch */
        write(motorPort, str, strlen(str)); delay(10);
	}
	ElectrodeDepth(electrodeDepth);
    sprintf(str, "B 0%c", CR); /* Free motors */
    write(motorPort, str, strlen(str)); delay(10);
    
    return;
}

void NewPosition(int pos)
{
    char buf[256];
#ifdef REVERSED_CABLEA
    int diff = pos - electrodeDepth; 
#else
    int diff = electrodeDepth - pos; 
#endif
    int old = electrodeDepth;
    int portstate;
    
    if(motorinvert){
        while((electrodeDepthOffset-pos) <= 0){
            electrodeDepthOffset += 10000;
        }
    }
    else{
        while((electrodeDepthOffset+pos) <= 0){
            electrodeDepthOffset += 10000;
        }
    }
    
#ifdef OLDCODE
    while((pos - electrodeDepthOffset) >= 0){
        electrodeDepthOffset -= 10000;
    }
#endif
    
    if(fabs(diff) > 9999){
        sprintf(buf,"Thats a big step (%d) Proceed?",diff);
        acknowledge(buf,"/bgc/bgc/c/binoc/help/bigstep.err");
    }
    if(fabs(diff) > 200){
        sprintf(buf,"Thats a big step (%d) Proceed?",diff);
        if(!confirm_yes(buf,NULL))
            return;
        if(pos < -1000) // leaving brain
            SetWaterAlarm(300);
        
    }
    
#ifdef DEBUG
    printf("Setting new Postion to %d (%d) %d\n",pos,diff,electrodeDepthOffset);
#endif
#if !defined(WIN32)
    ioctl(motorPort,TIOCMGET,&portstate);
    if(!(portstate & TIOCM_CTS)){
        fprintf(stderr,"Stepper Not Ready\n");
        if(optionflags[STEP_CLEAR_SPIKES]){
            memcpy(&cleartime,&now,sizeof(struct timeval));
            /* empirical approximation for the duration of the movement artifact */
            cleardelay = 0.2;
        }
        return;
    }
#endif
    
    sprintf(buf,"%d",pos);
    
    electrodeDepth = pos;
    
    selectMotor(motorids[motorid]);
    setCurrentPosition(old);  
    setNewPosition(electrodeDepth,diff);
	gettimeofday(&now,NULL);
    times[totalsteps] = timediff(&now,&progstarttime) + timeoffset;
    depths[totalsteps] = electrodeDepth;
    if(penlog){
        fprintf(penlog,"ed %d at %.1f\n",electrodeDepth,times[totalsteps]);
        if(diff < -200){
            fprintf(penlog,"Step Back at %s\n",binocTimeString());
        }
    }
    times[totalsteps+1] = 0;
    if(totalsteps < MAXDEPTHS-1)
        totalsteps++;
    if(optionflags[STEP_CLEAR_SPIKES]){
        memcpy(&cleartime,&now,sizeof(struct timeval));
        /* empirical approximation for the duration of the movement artifact */
        cleardelay = 0.11 + fabs(diff) * 0.027 - pow(fabs(diff),1.13) * 0.0113;
    }
    notifyPositionChange(electrodeDepth);
}


void ControlsetNewPosition(int pos, int diff)
{
    int delaySec;
    int delayMsec,i,charctr,j;
    char str[32];
	char waitStr[10],c;
	char readBuf[16];
	char inbuf[256];
	float fdiff = (float)(diff)/STEP_SEC;
	struct timeval start,end;
    
    TTY t;
	int portstate;
    
	sprintf(waitStr, "? P%c", CR);
    if(diff > 0) {
        delaySec = diff / STEP_SEC;
        delayMsec = diff % STEP_SEC;
    }
    else {
        delaySec = -diff / STEP_SEC;
        delayMsec = -diff % STEP_SEC;
    }
    
    sprintf(str, "P %d%c", pos, CR);
    write(motorPort, str, strlen(str));     delay(10);
	charctr = 0;
    
	if(ioctl(motorPort,TCGETA,&t) < 0)
	{
		perror("iocntl 1");
	}
	printf("terminal %d %d %d\n",t.c_cc[VMIN],t.c_cc[VTIME],t.c_cflag);
	gettimeofday(&start,NULL);
	while((i = read(motorPort,&c,1)) > 0)
        inbuf[charctr++] = c;
	i = write(motorPort, waitStr, strlen(waitStr));
	delay(10);
    
#if defined(IRIX64) || defined(IRIX)
	if(ioctl(motorPort,FIONREAD,&portstate) < 0)
	{
		perror("iocntl 1");
	}
#endif
    
	while((i = read(motorPort,&c,1)) <= 0)
        ;
	gettimeofday(&end,NULL);
	printf("Took %.4f\n",timediff(&start,&end));
	readBuf[0] = c;
	j = 1;
	while((i = read(motorPort,&c,1)) > 0)
        readBuf[j++] = c;
    
    if(delaySec) sleep(delaySec);
    else if(delayMsec) delay(delayMsec);
	fsleep(fdiff * stepvals[DELAY_SCALE]);
    
    /* deselect the current motor after motion is finished
     * this prevents the motor from getting too hot
     */
    sprintf(str, "/B 7%c", CR);                             /* open output latch */
    write(motorPort, str, strlen(str)); delay(10);
    sprintf(str, "B 1%c", CR);                              /* deselect current motor */
    write(motorPort, str, strlen(str)); delay(10);
    sprintf(str, "B 7%c", CR);                              /* close latch */
    write(motorPort, str, strlen(str)); delay(10);
    
    return;
}

int shutdown_stepper(void)
{
    FILE *rtf;
    char *home;
    char str[32];
    
    sprintf(str, "/B 7%c", CR);                     /* deselect previous motor */
    write(motorPort, str, strlen(str)); delay(10);
    sprintf(str, "B 1%c", CR);                      /* set B 1 high */
    write(motorPort, str, strlen(str));     delay(10);
    
    /* clear all ports */
    sprintf(str, "/B 2%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "/B 3%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "/B 4%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    
    sprintf(str, "B 2%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "/B 2%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "B 3%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    
    sprintf(str, "B 2%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "/B 2%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "/B 3%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    
    sprintf(str, "B 4%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "B 2%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "/B 2%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    
    sprintf(str, "B 3%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    sprintf(str, "B 2%c", CR);
    write(motorPort, str, strlen(str));     delay(10);
    
    /* all ports cleared */
    sprintf(str, "B 7%c", CR);                      /* close latch */
    write(motorPort, str, strlen(str)); delay(10);
    
    sprintf(str, "B 0%c", CR);                      /* free all motors */
    write(motorPort, str, strlen(str)); delay(10);
    
	restore_setup(motorPort);
	close(motorPort);
    return(0);
}


int plottrack(struct plotdata *plot)
{
    vcoord x[2];
    int i,j,cw,n,y,h,nlin = 0,k,sw,bw;
    double duration, maxdepth = -2000, mindepth = 1000;
    char s[BUFSIZ],*ps;
    int step = 0, first = -1;
    time_t difftime = 0;
    
    gettimeofday(&now,NULL);
    times[totalsteps] = timediff(&now,&progstarttime) + timeoffset;
    depths[totalsteps] = electrodeDepth;
    
    cw = 10;
    x[0] = plot->pos[0] - cw *9;
    x[1] = plot->pos[1];
    h = plot->size[1];
    x[1] += (h/2);
    
    
    SetColor(0.0, 0);
    y = plot->pos[1] + plot->size[1];
    for(i = 0; i < totalsteps; i++){
        {
            if(expt.vals[STEPPER_PLOTLEN] == 0 || (times[totalsteps-1] - times[i]) < expt.vals[STEPPER_PLOTLEN]){
                if(first < 0)
                    first = i;
                if(depths[i] > maxdepth)
                    maxdepth= depths[i];
                if(depths[i] < mindepth)
                    mindepth= depths[i];
            }
        }
    }
    duration = times[totalsteps-1] - times[first];
    duration = times[totalsteps] - times[first];
    
    for(i = first; i <= totalsteps; i++)
    {
        x[0] = plot->offset + (plot->w * (times[i]-times[first]))/duration;
        x[1] = y - plot->size[1] * (depths[i]-mindepth)/(maxdepth-mindepth);
        plotsymbol(x[0],x[1], 5, 0);
    }
    glBegin(GL_LINE_STRIP);
    for(i = first; i < totalsteps; i++)
    {
        x[0] = plot->offset + (plot->w * (times[i] - times[first]))/duration;
        x[1] = y - plot->size[1] * (depths[i]-mindepth)/(maxdepth-mindepth);
        myvx(x);
    }
    glEnd();
    x[1] = plot->pos[1]-2.5*cw;
    x[0] = plot->pos[0];
    sprintf(s,"%.0f",times[first]);
    difftime = progstarttime.tv_sec + (int)(times[first] - timeoffset);
    ps = ctime(&difftime);
    ps[16] = 0;
    sprintf(s,"%5s",&ps[10]);
    mycmv(x);
    printString(s,0);
    x[0] = plot->offset+plot->w;
    sprintf(s,"%.0f",times[totalsteps]);
    ps = ctime(&now.tv_sec);
    ps[16] = 0;
    sprintf(s,"%5s",&ps[10]);
    mycmv(x);
    printString(s,0);
    x[0] = plot->pos[0]-2*cw;
    x[1] = plot->pos[1]+h;
    sprintf(s,"%.0f",mindepth);
    mycmv(x);
    printString(s,0);
    x[1] = plot->pos[1];
    sprintf(s,"%.0f",maxdepth);
    mycmv(x);
    printString(s,0);
    if(maxdepth - mindepth > 4000)
        step = 500;
    else if(maxdepth - mindepth > 1000)
        step = 200;
    else
        step = 100;
    for(i = ((int)((mindepth + step/2)/step) + 1) * step; i < (maxdepth - step/2); i+= step)
    {
        x[1] = y- (h * (i - mindepth))/(maxdepth - mindepth);
        sprintf(s,"%d",i);
        mycmv(x);
        printString(s,0);
    }
    return(i);
}


int ReadPenSteps(char *penfile)
{
    FILE *ifd;
    int d,i,toff;
    float t,maxtime;
    char buf[BUFSIZ];
    
    if((ifd = fopen(penfile,"r")) == NULL){
        fprintf(stderr,"Can't read %s\n",penfile);
        return(-1);
    }
    totalsteps = 0;
    toff = 0;
    while(fgets(buf,BUFSIZ,ifd)){
        i = sscanf(buf,"ed %d at %f",&d,&t);
        if(i ==2){
            if(totalsteps > 0 && t < times[totalsteps-1])
                toff = times[totalsteps-1];
            maxtime = times[totalsteps] = t + toff;
            depths[totalsteps] = d;
            totalsteps++;
        }
        if(!strncmp(buf,"Opened",6) && totalsteps > 1){
            timeoffset = times[totalsteps-1];
        }
        if(!strncmp(buf,"StartDepth",8) && totalsteps > 1){
            totalsteps = 0;
            toff = 0;
        }
    }
    fclose(ifd);
    return(0);
}

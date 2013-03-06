
#include <stdio.h>
//Ali #ifdef Darwin
#include <term.h>
#define TCGETA TIOCGETA
#define TCSETA TIOCSETA
//Ali #endif
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdarg.h>
#include <sys/fcntl.h>
#ifdef IRIX
#include <sys/z8530.h>
#endif
#ifdef IRIX64
#include <sys/z8530.h>
#endif
#define MYEOF (char)17
#include "commdefs.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static int autoreopen = 0;

int ttys[2] = {0};
char serchar;
//Ali #ifdef Darwin
TTY save_t;
TTY save_ta;
//Ali#else
//Ali struct termio save_t;
//Ali struct termio save_ta;
//Ali #endif
FILE *seroutfile,*serintest;
int totalcharsin;
unsigned long charsread = 0;
char longlist[BUFSIZ*10];
static struct timeval moment;
void SerialString(char *s, int tty);

int OpenSerial(char *port)
{
    char buf[256];
    int i = 0;
    
    if(ttys[0] != 0)
        i = 1;
    if((ttys[i] = open(port,O_RDWR|O_NONBLOCK)) < 0)
    {
        sprintf(buf,"Can't open %s",port);
        perror(buf);
        return(0);
        /*    exit(-1);*/
    }
    new_setup(ttys[i]);
#ifdef DEBUG_SERIAL
    serintest = fopen("serial.in");
    totalcharsin = 0;
#endif
    return(ttys[i]);
}


int new_setup(int tty)
{
    
    //Ali #ifdef Darwin
	TTY t;
    //Ali #else
    //Ali 	struct termio t;
    //Ali #endif
    
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
    
    /*	*/
	t.c_cflag &= (~PARENB);
    /* 
     * helmholz and the pentium work at 19.2kps, but ogle and the 486
     * won't go better than 9600
     */
#ifdef IRIX64
	t.c_ospeed = B9600;
    t.c_ospeed = B57600;
    t.c_ospeed = B115200;
#else 
#ifdef IRIX
	t.c_ospeed = B9600;
	/*	t.c_cflag |= (B9600 | CS8);*/
#else
	t.c_cflag |= (B9600 | CS8);
#endif
#endif
#ifdef SPIKE2
	t.c_cflag |= (B9600 | CS8);
	t.c_ospeed = B9600;
#endif
    
    //Ali #ifdef Darwin
    t.c_ospeed = B115200;
    t.c_ispeed = B115200;
	t.c_cflag |= (CS8);
    //Ali #endif
    
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
#if defined(IRIX64)
	if(ioctl(tty,SIOC_ITIMER,0) < 0)
	{
		perror("Ioctl 2");
		exit(-2);
	}
#endif
	return(0);
}

void restore_setup(int tty)
{
    if(tty == ttys[0]){
        if(ioctl(tty,TCSETA,&save_t) < 0)
        {
            perror("ioctrl reset");
            exit(-3);
        }
    }
    else if(tty == ttys[1]){
        if(ioctl(tty,TCSETA,&save_ta) < 0)
        {
            perror("ioctrl reset");
            exit(-3);
        }
    }
}

int closeserial(int tty)
{
    if(tty == 0){
        tty = ttys[0];
        close(tty);
        ttys[0] = 0;
    }
}

char ReadSerial(int tty)
{
    int i,thetty;
    char c,dummy[4];
    /* never leave a single char haning on the end of the stack */
    
#ifdef Linux
    return((char) MYEOF);
#endif  
    if(tty < -1)
    {
        serchar = c = DummySerial();
        return(serchar);
    }
    if(tty == 0)
        tty = ttys[0];
    if((i = read(tty,&c,1)) <= 0)
    {
        return((char) MYEOF);
    }
    else{
#ifdef MONITOR_CLOSE
        if(seroutfile && (c == END_STIM || c == BAD_FIXATION || c == START_STIM)){
            gettimeofday(&moment,NULL);
            fprintf(seroutfile,"I %d %.2f\n",(int)(c),ufftime(&moment));
            fflush(seroutfile);
        }
#endif
        longlist[charsread++] = c;
#ifdef DEBUG_SERIAL
        if(serintest){
            fputc(serintest,c);
            totalcharsin++;
        }
#endif
        if(charsread > (BUFSIZ * 10)) 
            charsread = 0;
        serchar = c;
        return(serchar);
    }
}

char *CheckSerialInput(int length)
{
    static char buf[BUFSIZ];
    
    if(length < charsread){
        longlist[charsread] = 0;
        sprintf(buf,"%*s",length,&longlist[charsread-length]);
        return(buf);
    }
    else
        return(NULL);
}

void SerialString(char *s, int tty)
{
    int i = 0,k = 0,sumerrs = 0;
    static int writerrs = 0;
    
    if(tty == 0)
        tty = ttys[0];
    if(tty >= 0){
        while(s[i] != 0){
            if((k = write(tty,&s[i++],1)) < 0)
                sumerrs++;
            else{
                autoreopen = 1;  // have successfully written
                writerrs = 0;
            }
        }
    }
#ifdef Darwin
    // The line to the PC needs to be reopened every time Spike 2 is restarted
    if(sumerrs && autoreopen && ttys[0] == tty){
        fprintf(stderr,"Error Writing %s to %d\n",s,tty);
        if(writerrs++ < 4){
            ReopenSerial();
            fsleep(0.2); 
        }
        else{
            fprintf(stderr,"Can't Reopen Serial Port %d\n",tty);
        }
    }
#endif
    if(seroutfile != NULL && tty != ttys[1])
        i = fprintf(seroutfile,"%s",s);
}

/*
 * binoc.c all purpose binocular visual physioslogy, psychophsics and training
 * OGL version April 1997, B. Cumming
 * panel manage and monoc interleave
 * background when expt is stopped
 */

#import <stdio.h>
#import <stdlib.h> 
#import <sys/types.h> 
#import <sys/stat.h> 
#import <sys/fcntl.h> 
#import <sys/time.h> 
#import <termios.h> 
#import <stdarg.h> 
#import <unistd.h>
#import <math.h> 
#import <string.h> 
#import "mymath.h" 
#import "myview.h"
#import "misc.h"
#import <OpenGL/gl.h>
#define CHARCODES_DEFINE 1
#define STAIROUT 1
#define NIDAQ 1

#import "commdefs.h"
#import "stimuli.h" 
#import "cyber.h"
#import "WinDefs.h"
#import "sprotos.h"
#import "protos.h"



/*
 INTRIAL = TRIAL_PENDING | POST_STIMULUS_BIT
 */
#define LINESEP 30
#define MONITOR_CLOSE 1

#define testout fprintf

#define LBUTTON 1
#define RBUTTON 2
#define MBUTTON 4
#define SHIFTKEY (1<<3)
#define CNTLKEY (1<<4)
#define MIMIC_FIXATION (1<<5)

#define ODD(n)	((n) % 2)
#define ExptIsRunning() ((TheStim->mode & EXPTPENDING) && !states[EXPT_PAUSED])
#define sign(x) (x > 0 ? 1 : -1)

//Ali

char * VERSION_NUMBER;
char * SHORT_VERSION_NUMBER;

/*GLUquadricObj *gluq;*/
static GLuint base,bigbase,mediumbase;
static int eventstate = 0,window_is_mapped = 0;
static int rndbonus = 10;
static int forcestart = 0;
static int nostore = 0;
static float pursued = 0;
int check_for_monkey = 1;
static int track_resets[] = {XPOS, YPOS, FIXPOS_X, FIXPOS_Y, -1};
float pursuedir = -1;
float totalreward = 0;

int useDIO = 1;
int fullscreenmode = 0;

double oldvelocity = 0;
int oldsoftoff[6] = {0};
int stimflag = 0;
double olddisp  = 0;
int inexptstim = 0;
int newmonoc = 0;
float framehold = 0,calcdur,paintdur,swapwait;
float microsaccade = 0,microsaccdir = 0;
char *replay_expt = NULL;
int gotspikes = 0, endbadctr = 0;
int fixpaint = 0;
int binocrunning = 0,altstimmode = 0;
int bothmask = FORCEBOTH;
int thebuffer = 0,lastcodes[CODEHIST] = {0}, expstate = 0;
int outcodes[CODEHIST] = {0};
int codectr = 0, outctr = 0;
int DIOval = 0;
int Frames2DIO = 0;
int rewardall = 0;
char resbuf[BUFSIZ] = {0};

Monitor mon;
Expt expt;
AFCstructure afc_s;
struct BWSTRUCT thebwstruct;

FILE *testfd = NULL,*logfd = NULL;
char *logname;
char *rcname = NULL;
static    unsigned int first, last,bigfirst,biglast;
static    unsigned int medfirst, medlast,bigfirst,biglast;
static int newtimeout = 1;

#define MAXRF 10
Expstim oldrfs[MAXRF];
int rfctr = 0;

int debug = 0;
int timeout_type = 0;
int mimic_fixation = 0;
int mimic_bias = 0;
static float mimicbadfix = 0.01;
static float mimicbadresp = 0.1;
int freezeexpt = 0;
int freezestimulus = 0;

#define DEBUG_CONJ_TIME (1<<1)

int winsiz[2] = {0,0}; /* let X decide */
int winpos[2] = {0,0};
int fwinsiz[2],fwinoff[2];
vcoord psychoff[2];
static vcoord endpt[2];
vcoord fixpos[2],conjpos[2],oldpos[2],oldfixpos[2],oldstimpos[2];

int testint;
int totalchrs = 0, conjctr;
int leftmask = GL_TRUE, rightmask = GL_TRUE, cycmask = GL_FALSE, alphamask = GL_FALSE;
int monkeypress;
double pmatrix[16];

static int sliderflag = ORIENTATION, ntrialstim;
static float zoom = 1.0, wsum = 0.0;
float cleardelay = 0.3,okdelay = 0;
static float tempxy[2],zll[2];
char ImageOutDir[BUFSIZ] = ".";
FILE *imidxfd;

#define CBUFLEN 4098
static char charbuf[CBUFLEN],mssg[BUFSIZ], conjbuf[BUFSIZ];
static int conjbufctr = -1;
static char lastinput[BUFSIZ];

static char theport[256] = "/dev/tty.KeySerial1";

static char *statusstring = NULL;

char stepperport[256] = "/dev/ttys1";

#define MAXDRAGS 150
static int framelist[MAXDRAGS];
static float poslist[MAXDRAGS];
static int drag=0;
static int charctr = 0;
static int delay = 0;
static Rectangle imrect;
static int stepsize = 8;
struct timeval firstframetime,now, wurtzstart, timeb, timec, lastframetime,sessiontime,trialend,endstimtime,signaltime,wurtzframetime,alarmstart;
static int loopframes = 0;
struct timeval endtrialtime, starttimeout, goodfixtime,fixontime,cjtime;
struct timeval zeroframetime, prevframetime, frametime, cleartime;
struct timeval lastcleartime,lastsertime;
struct timeval progstarttime,calctime,paintframetime;
struct timeval endexptime, changeframetime,lastcalltime,nftime;
int wurtzctr = 0, wurtzbufferlen = 512,lasteyecheck;
float clearcolor = 0;
float lasttf = -1, lastsz = -1, lastsf = -1,lastor=0;
float markercolor = 1.0;
float trialdur = 0,trialdursum=0;
float *stimtimes = NULL, *downtimes = NULL, *starttimes = NULL;
int *fixed = 0;
float *fixx = NULL, *fixy = NULL;
int goodtrials = 0, totaltrials = 0, premtrials = 0, fixtrials = 0, wrongtrials = 0, afctrials = 0,latetrials=0, badtrials = 0;
int avglen = 20,avctr = 0;
float stopcriterion = 0.0;
float cmarker_size = 15;


OneStim *thecorrug;

typedef struct _AppResources {
	int show;
	int wins;
} AppResources;


static int maxframes = 0;

static /*Ali GC */int BackGC;
static int imsize;
int nreps = 1;
unsigned long *fullframe;
unsigned long *imframes[MAXFRAMES];

int verbose = 0;
int rightshift = 6;
unsigned int mode = 0;
int stmode = 0;
static int oldmode = 0;
int runmode = 1;
int testmode = 4;
static int maxloops = 0;
static char *loadfiles[100] = {NULL};

/*Ali */ int forecolor,backcolor,curscolor;
/*Colormap */ int cmap;

int next_frame(Stimulus *st);
void afc_statusline(char *s, int line);
void framefront();
void run_radial_test_loop();
void run_swap_test_loop();
void run_general_test_loop();
void run_rds_test_loop();
void run_gabor_test_loop();
void run_anticorrelated_test_loop();
void set_test_loop();

void expt_over(int flag);
void end_timeout();



long optionflag;
long option2flag = PRETRIAL_BRIGHT;
int optionflags[MAXALLFLAGS];
int defaultflags[MAXALLFLAGS] = {0};
int testflags[NTESTFLAGS] = {0};

int states[NSTATEFLAGS] = {0};
static int stepframe = 0;

char *toggle_strings[] = {
    /* optionflag */
	"Goraud Shading", "Dithering", "Frame Test","Go",
	"AntiAlias", "Square","72Hz","Cnrst Rev",
	"S.E.","Back Fixed","Test mode","Fix cross","Box",
	"Auto Plot","Wurtz Task","PreBack","L Monoc","R Monoc",
	"Checkfix","Verg Check","Whole Stimuli", "Store",
	"Show Val", "Conjuate Tracking", "Trials",
	"Wait for BW", "Disp Clamp", "Clamp+Hold", "Search..", 
	"Vgc Ramps", "RampHold", "See Conj", 
    /* option2flag */
	"Flash", "Interact","FREE",
	"Vary Vergence","+Anti", "+Blank", "+random", "+monoc", "+uncorr",
	"wipe screen",
	"PsychoPhysics",  
	"Alt Probe", "RANDOM","Stair","AFC", "P.Perf", "IFC",
	"Feedback","Flip","Paint back","Fix Sepn","RandExp2",
	"RevExpt2","Binoc FP","RC","+zero","no status","no mirrors",
	"Move RF","Grey Monoc","Contour","Smooth/Polar","Sequence","Xexp2","Fake dFP","+sine","Sp Clear", "+highTF","+highSF","Counterphase","+highSQ","+highX","+ ZeroS","+MonocS","+component","xUncorr","Rand Phase","Track","Rnd FPdir",
	"SplitScreen","Count BadFix","RunSeq","microstim","Tile-XY","Store Expt Only","FixGrat","+FPmove","Rand RelPhase","Always Change","TGauss","Check Frames","Random dPhase","xHigh","Always Backgr","Store LFP","Nonius","+Flip","Online Data","AutoCopy","PlotFlip","FastSeq","4Choice","BackLast","Us0only","Random Contrast","Random Correlation","Indicate Reward Bias","Collapse Expt3",
    "Odd Man Out","Choice by icon","Image Jumps",
    "AutoCopy","Custom Vals Expt2","Stim In Overlay", "reduce serial out", "center staircase", "Paint all frames", "modulate disparity", "stereo Glasses",
    "nonius for V", "Calc once only", "Debug", "Watch Times", "Initial Training", "Check FrameCounts",
    "Show Stim Boxes",
    NULL,
};



char *toggle_codes[] = {
    "gs","di",
    "ft", //Frame test
    "do", //Go/Stop
    "aa", // AntiAliasing
    "sq","xx",
    "cr", // Contrast reversing
    "se", // S.E.M. 
    "bf", // Background Fixed
    "tm","fc","fb","ap","wt","pb","lm","rm","cf","cv",
    "ws","ts","pw","ct","sw","bw","dc","ch","sm","vr",
    "rh","co","fl","ei","xx", "te","ia","ic","ir","im",
    "iu","cs","py","pa","ra",
    "sc", // Staircase
    "afc",
    "pp", // Show Performance String
    "if", // IFC
    "fe", // Feedback
    "ff", // Flip Feedback
    "bp", // Paint background stims
    "fs", // Fixed Bar Separation
    "r2", // Random rewards for expt2
    "i2", // Reverse rewards for expt2
    "bm", // Binocular Fixation point
    "rc", // Reverse Correlation 
    "iz", // Interleave Zero
    "s0", // Hide Status
    "m0", // No Mirrors (for initial training)
    "mr", // Move RF with Conjpos Eye tracking
    "gm", // Grey Monoc (grey field for 'occluded' eye) 
    "cn", // Contour Plot
    "gc", // Smooth Contour plot
    "to", // Plot Sequence
    "x2", // Expt2 is multiplied by Expt 1
    "fp", // Simulate FP movement
    "is", // Interleave Sinewave
    "sp", // Clear Spikes when step microdrive
    "ht", // +High TF
    "hs", // +High SF
    "cp",
    "hq",
    "hx", // +High X value
    "I0",
    "Im",
    "Ic", // Interleave Components
    "Iu", // Interleave Set of uncorrelated stims with each of Expt 2 val 
    "rp", //random phase
    "pt", // Plot Electrode Track
    "rd", // RAND_FP_DIR
    "ss", // SPLITSCREEN
    "cB", //Count Bad fixations not just wrong choices
    "rS", //Treat Stimulus as sequences
    "uS", //microstim on this trial
    "xy", //random tiling of XY-pos
    "sE", //Store Expts only
    "fG", //make fixpt a sq grating (for focussing).
    "iF", //Interleave FP jump
    "rr", //Randomize Relative Phase
    "ac", //Always Change Stim
    "tg", //Temporal Gaussian
    "cF", //Check Frame count
    "rP", // random phase disp
    "hX", // interleave HIGH expt 1 for each val of expt 2
    "ab", // always show background
    "lf", // Store lfp
    "no", // nonius lines
    "fi", // Interleave signflip (for IFC)
    "da", // Make Online Data Files
    "ao", // Auto Store Online Data Files
    "pf", // Flip Expts 1/2 on plot
    "fS", //Fast Sequence
    "af4", // 4 choice targets, for gambling
    "tl", // Paint stim 3 last
    "U0", // microstim zero trials only
    "rC", // random contrast
    "rI", // random interocular correlation
    "sR", // show reward bias
    "C3", //Collapse Psychophysics across Expt3  
    "af3", //Odd man out task
    "Rcd", //Choice direction random
    "ijump", //Image jumps
    
    "cd", // Auto Copy Online Data Files # not shown on panel
    "cb", //Custom vals for expt2
    "pe", //stimulus in Overlay
    "ls", // reduce serialoutput (RC expts)
    "cS", // Center staircases (instead of starting at extremes)
    "FN", // FIXNUM_PAINTED_FRAMES paint all frames in stim, regargless of elapsed time
    "md", // modulate disparity
    "sg", // stereo glasses
    "vn", // Nonius lines for vertical vergence
    "Co", // Calculated once only, at stim start...
    "od", //Debug  
    "WT", //Watch timings
    "IT", //Initial Training
    "CF", //CHECK_FRAMECOUNTS
    "SB", //Show Stim Boxes
    /* 
     * One day, all the BW toogle boxes will be done like the rest
     * with optionflags[] and toggle_codes. Then this will be
     * where their codes are kept.
     "xl","xr","xc","xg","xx","yg","sf"
     */
    NULL,
};

char *mode_codes[] = {
    "db","zb","rw","vb",NULL,
};

char *flag_codes[] = {
    "pc","nc","cs","ac","sq","uc","le","fd","dd","mm","mf","tx","tc","cn","pl","fo","nw","sp", "bo", "oc", "wr",
    NULL,};

char *flagstrings[] = {
	"White Dots", "Black Dots", "Crossed Seed","AntiCorrelate", "Square","Uncorrelated",
	"Direction LEFT", "FIXED dot num", "Flat disp", "Flat surf", "Fix dot",
	"Texture", "Cards", "COUNTDOWN", "PLC", "PrefDir Only", "No wrap (Cyl)", "Hold plane" ,"NullDir Only", "Front Occludes", "RDS wrap",NULL,
};




extern double fakestim;
extern int usenewdirs;
extern int dorpt,rcrpt , lastrpt;
extern int *stimorder,demomode,covaryprop;
//struct plotdata *expplots;
extern FILE *stairfd;
extern int noserialout;
extern unsigned expseed;
extern int triallaps,nquickexpts;
extern int outliers[],framecseq[],seedoffsets[];
extern double framesd,framemean;
extern long frametotal;
extern char serchar,*expname,datprefix[];
extern Thisstim *stimseq;
extern Currentstim currentstim;
extern unsigned long charsread;
extern int trialctr;
extern int command_pending;
extern /*Ali Cursor */ int thecursor;
extern FILE *seroutfile,*penlog,*rcfd,*psychfile;
extern int ranright,  ranleft, ranleftc, ranrightc,trialcnt;
extern int ttys[];
extern int laststimno,stimno;
static int width = 300,height= 400;
extern int *framebuf,framebufctr;
extern float frameseq[];
extern int frameiseqp[];
extern int valstringindex[];
extern int toggleidx[];

AppResources app_resources;
int framesdone = 0;
static int framecount = 0,framelag = 0, framectr;
int realframecount = 0;
int wurtzcount = 0,realwurtzcount = 0, lastwurtzcount = 0;
int duration = 0;
int stimstate = 0;
int fixstate = 0;
int stopgo = 0;
int testcount[5];

/*Ali Display */ int D;
static int      S;


Stimulus *stimptr,*ChoiceStima,*ChoiceStimb;
static Stimulus *TheStim,*tempstim;
/*j some of jons added functions*/
void afc_statusline(char *s, int line);
void paint_target(float color, int flag);



#define resetframectr() (mode |= RESET_FRAME_CTR)


int PrintToggles(FILE *ofd)
{
    int i = 0,j = 0;
    fprintf(ofd,"ON/OFF codes:\n");
    while(togglestrings[i].code != NULL){
        fprintf(ofd, "%s %s\n",togglestrings[i].code,togglestrings[i].label);
        i++;
    }
    fprintf(ofd,"Stimulus codes:\n");
    while(flagstrings[j]){
        fprintf(ofd, "%s %s\n",flag_codes[j],flagstrings[j]);
        j++;
    }
    return(i);
}

char *binocTimeString()
{
    char name[BUFSIZ],*t = NULL ,buf[256];
    time_t tval;
    tval = time(NULL);
    t = ctime(&tval);
    t[19] = 0;
    return (&t[10]);
}


void ShowTime()
{
    vcoord x[2];
    char buf[BUFSIZ];
    int px,py;
    float val;
    
    
    if(issfrc(expt.stimmode))
        return;
    return;  //prefer to have status line printed
    px = (int)(ExptProperty(&expt, PANEL_XPOS));
    py = (int)(ExptProperty(&expt, PANEL_YPOS));
    
    if(px > 400)
        x[0] = -600;
    else
        x[0] = 0;
    x[1] = 100;
    setmask(ALLPLANES);
    mycmv(x);
    SetGrey(TheStim->gammaback);
    myrect(x[0],x[1]-5,x[0]+200,x[1]+50);
    SetGrey(1.0);
    if(TheStim->gammaback > 0){
        if(states[EXPT_PAUSED_BY_QEXP])
            sprintf(buf,"%s: Expt Pausing (%.1f)",binocTimeString(),timediff(&now,&endexptime));
        else if(states[EXPT_PAUSED])
            sprintf(buf,"%s: Expt Paused",binocTimeString());
        else if(check_for_monkey == 0)
            sprintf(buf,"%s: Not Checking Monkey",binocTimeString());
        else
            sprintf(buf,"%s",binocTimeString());
        if(microsaccade)
            sprintf(buf,"Saccade %.2f",microsaccade);
        if((stimstate == IN_TIMEOUT || stimstate == IN_TIMEOUT_W) && timeout_type == SHAKE_TIMEOUT_PART2){
            val = timediff(&now, &starttimeout);
            sprintf(buf,"Shake %.1f",val);
        }
        
        printStringOnMonkeyView(buf, 0);
    }
}


int getframecount()
{
    float tval;
    int frames;
    static int gotzerr = 0;
    char buf[BUFSIZ];
    
    gettimeofday(&frametime, NULL);
    tval = timediff(&frametime, &zeroframetime);
    frames = (int)((tval * mon.framerate) +0.1);
    if(frames == realframecount && frames != 0)
        tval = timediff(&frametime, &prevframetime);
    if(frames > 0){
        gotzerr = 0;
        memcpy(&prevframetime, &frametime, sizeof(struct timeval));
    }
    else if(frames < 0){ // should never happen
        if( !gotzerr){
            sprintf(buf,"Negative frame (%d) count (%d,%d)",frames,stimstate,fixstate);
            acknowledge(buf,NULL);
            gotzerr++;
        }
        if(seroutfile && gotzerr % 1000000 == 1)
            fprintf(seroutfile,"Negative frames at %u %u (%u) %s state %d,%d,%d\n",ufftime(&frametime),ufftime(&zeroframetime),ufftime(&prevframetime),binocTimeString(),stimstate,fixstate,mode&FRAME_BITS);
        return(1000);
    }
    return(frames);
    
}

float getframetime()
{
    float tval;
    int frames;
    
    gettimeofday(&frametime, NULL);
    tval = timediff(&frametime, &zeroframetime);
    return(tval);
    
}

int DrawBox(vcoord bx, vcoord by, vcoord w, vcoord h, float color)
{
    vcoord x[2];
    glLineWidth(expt.linw);
    SetGrey(color);
    /* first draw the stimulus region*/
    glBegin(GL_LINE_LOOP);
    x[0] = deg2pix(bx-w/2);
    x[1] = deg2pix(by-h/2);
    myvx(x);
    x[0] += deg2pix(w);
    myvx(x);
    x[1] += deg2pix(h);
    myvx(x);
    x[0] = deg2pix(bx-w/2);
    myvx(x);
    x[1] = deg2pix(by-h/2);
    myvx(x);
    glEnd();
    return(0);
}


int CheckStrings()
{
	int i,j,err = 0;
    
	for(i = 0; i < expt.totalcodes; i++)
	{
		for(j = i+1; j < expt.totalcodes; j++)
        {
		    if(strcmp(valstrings[i].code,valstrings[j].code) == 0)
            {
                fprintf(stderr,"Duplicate code (%s) at %d and %d\n",valstrings[i].code,i,j);
                err++;
            }
        }
	}
	return(err);
}

extern double gammaval;

void initial_setup()
{
    Locator *pos;
    int i;
    long mtype;
    
	setgamma(1.22);
	defaultflags[COLLAPSE_EXPT3] = 1;
	defaultflags[BINOCULAR_FIXPOINT] = 1;
	defaultflags[STAIR_CENTER] = 1;
    
	for(i = 0; i < MAXALLFLAGS; i++)
        optionflags[i] = defaultflags[i];
	TheStim = NewStimulus(NULL);
	StimulusType(TheStim, STIM_GRATING);
	tempstim = NewStimulus(NULL);
	StimulusType(tempstim, STIM_BAR);
    
	ChoiceStima = NewStimulus(NULL);
	StimulusType(ChoiceStima, STIM_GRATING);
	ChoiceStima->splane = STIM_CHOICE1_BIT;
    
	ChoiceStimb = NewStimulus(NULL);
	StimulusType(ChoiceStimb, STIM_GRATING);
	ChoiceStimb->splane = STIM_CHOICE2_BIT;
    
	/*	gluq = gluNewQuadric();*/
	pos = &TheStim->pos;
	stimptr = TheStim;
	TheStim->incr = M_PI/15;
	TheStim->framerepeat = 1;
	TheStim->colormode = (COLORRGB);
	TheStim->nframes = 30;
	TheStim->f = 2.0;
	TheStim->background = 128;
	TheStim->fix.fixframes = 60;
	TheStim->fix.stimframes = 120;
	TheStim->fix.fixcolor = 0;
	TheStim->fix.stimcolor = 0.8;
	TheStim->fix.timeout = 1.0;
	expt.vals[BLANKCOLOR_CODE] = 1.0;
	expt.vals[TIMEOUT_CONTRAST] = 1.0;
	expt.vals[PLOTSMOOTH] = 1.0;
	afc_s.sacval[0] = 0.0;
	afc_s.sacval[1] = 0.0;
	afc_s.sacval[2] = 0.0;
	afc_s.sacval[3] = 0.0;
	afc_s.gregvals[0] = 0.33;
	afc_s.gregvals[1] = 0.66;
	afc_s.gregvals[2] = 0.6;
	afc_s.gregvals[3] = 0.1;
	afc_s.gregvals[4] = 2;
	TheStim->fix.size = 20;
	pos->contrast = pos->contrast_amp = 1.0;
	pos->contrast_phase = 0.0;
	TheStim->pos.ss[0]  = 20;
	TheStim->pos.ss[1]  = 5;
	TheStim->pos.dotsize = 5;
	TheStim->pos.density = 25;
	pos->phase = 0;
	pos->phase2 = 0;
	TheStim->sf_disp = TheStim->contrast_disp = TheStim->ori_disp = 0;
	TheStim->phasedisp[0] = 0;
	TheStim->disp = 0;
	TheStim->angleinc = 0;
	pos->angle =  0;
	TheStim->mode = (DRAW_FIX_BIT | DOUBLEBUFFERMODE | STIMWIPE);
    
	TheStim->splane = 0x1;
    
	pos->size[0] = rint(width/pos->ss[0]);
	pos->size[1] = rint(height/pos->ss[1]);
	expt.spikegain = 10.0;
	expt.electrode_depth = 0.0;
	expt.vals[FIXATION_OVERLAP] = 100;
	expt.vals[SHAKE_TIMEOUT_DURATION] = 10;
    
	afc_s.performance_1 = (char *)malloc(PERF_STRING_LENGTH+1 * sizeof(char)); /* strings for holding afc performance */
	nullify(afc_s.performance_1, PERF_STRING_LENGTH+1);
	afc_s.performance_2 = (char *)malloc(PERF_STRING_LENGTH+1 * sizeof(char));
	nullify(afc_s.performance_2, PERF_STRING_LENGTH+1);
	
	afc_s.proportion = 0.5;
	afc_s.correction_entry = 3;
	afc_s.correction_leave = 1;
	afc_s.maxstairvalue = MAXSTAIRVALUE;
	afc_s.minstairvalue = MINSTAIRVALUE;
	afc_s.startvalue = afc_s.jstairval = 0.3;
	afc_s.stairfactor = ROOT2;
	afc_s.loopstate = NORMAL_LOOP;
	afc_s.laststate = NORMAL_LOOP;
	afc_s.wrongtimeout = TheStim->fix.timeout/2;
	afc_s.sac_fix_crit = 2.0;
	afc_s.stairhist = newstairstruct();
	afc_s.targsize = deg2pix(0.5);
	afc_s.lasttrial = -(BAD_TRIAL);
	mon.pixels[0] = 1280;
	mon.pixels[1] = 1024;
	mon.crosstalk[0] = 0;
	mon.crosstalk[1] = 0;
	mon.cms2pix[0] = mon.cms2pix[1] = 37.5;
	mon.pix2cms[0] = 1/mon.cms2pix[0];
	mon.pix2cms[1] = 1/mon.cms2pix[1];
	for(i = 0; i < 8; i++)
        mon.trapscale[i] = 0;
	mon.loaded = 0;
	mon.filename = NULL;
	mon.name = NULL;
	mon.viewd = 57;
	mon.framerate = 72.0;
	srandom((i = getpid()));
	mtype = (long)i;
	srand48(mtype);
    
	gettimeofday(&sessiontime,NULL);
    gettimeofday(&now,NULL);
    ExptInit(&expt, TheStim, &mon);
//ExptInit now sets up codesend and nfplaces from valstrings
    return;
	for(i = 0; i < MAXSERIALCODES; i++)
	    switch(i)
    {
		case STIMID:
		case SET_SEED:
		case OPTION_CODE:
		case RC_SEED:
		case STIMULUS_VERSION:
        case CORRELATION:
        case MONOCULARITY_EXPT:
            codesend[i] = SEND_STIMULUS;
            break;
        case UPLOAD_CODE:
        case UFF_PREFIX:
        case UFF_COMMENT:
        case ELECTRODE_DEPTH:
        case QUERY_STATE:
        case SEND_CLEAR:
            codesend[i] = SEND_USER_ONLY;
            break;
        case STIMULUS_MODE:
        case STIMULUS_TYPE_CODE:
        case DISP_X:
        case DISP_Y:
        case STIM_WIDTH:
		case STIM_HEIGHT:
		case SF:
		case TF:
		case ORIENTATION:
        case SET_SEEDLOOP:
		case STATIC_VERGENCE:
		case UNIT_BINWIDTH:
		case DISP_BACK:
        case SETZXOFF:
        case SETZYOFF:
        case EXPTYPE_CODE:
        case EXPTYPE_CODE2:
        case EXPTYPE_CODE3:
        case EXPT2_INCR:
        case EXPT2_MEAN:
        case EXPT2_NSTIM:
        case EXPT_INCR:
        case EXPT_MEAN:
        case NTRIALS_CODE:
        case DOT_DENSITY:
        case DOT_SIZE:
        case VERSION_CODE:
        case FRAMEREPEAT:
        case NFRAMES_CODE:
        case START_PHASE:
        case ISI_CODE:
        case FRAMERATE_CODE:
        case NEXTRAS_CODE:
        case BACKSTIM_TYPE:
        case BACK_CONTRAST:
        case BACK_ORI:
        case BACK_SIZE:
        case BACK_TF:
        case BACK_CORRELATION:
        case TRIAL_START_BLANK:
        case DISTRIBUTION_WIDTH:
        case RC1INC:
        case DISTRIBUTION_MEAN:
        case VIEWD_CODE:
        case XPIXEL_CODE:
        case YPIXEL_CODE:
        case RF_ANGLE:
        case JDEATH:
        case STANDING_DISP:
        case PURSUIT_INCREMENT:
        case INITIAL_APPLY_MAX:
        case FP_MOVE_DIR:
        case REWARD_BIAS:
        case IMAGEJUMPS:
            codesend[i] = SEND_EXPT;
            break;
        case PULSE_WIDTH:
            codesend[i] = SEND_EXPT_NONZERO;
            break;
		case NCOMPONENTS:
		case SF2:
        case TF2:
        case PHASE2:
            codesend[i] = SEND_GRATING2;
            break;
        case FP_MOVE_SIZE:
            codesend[i] = SEND_NON_ZERO;
            break;
        default:
            codesend[i] = SEND_EXPLICIT;
            break;
    }
	for(i = 0; i < MAXTOTALCODES; i++)
    {
	    switch(i)
        {
            case TRAPEZOIDAL_SCALING:
                nfplaces[i] = 7;
                break;
            case OPPOSITE_DELAY:
            case INITIAL_APPLY_MAX:
            case DISP_X:
            case TWOCYL_DISP:
            case DISP_P:
            case EXPT_MEAN:
            case EXPT2_MEAN:
            case EXPT_INCR:
            case EXPT2_INCR:
            case XPIXEL_CODE:
            case YPIXEL_CODE:
                nfplaces[i] = 5;
                break;
            case DISP_BACK:
            case STANDING_DISP:
            case DOT_SIZE:
            case DISP_Y:
            case CORRELATED_DISPARITY:
            case ANTICORRELATED_DISPARITY:
            case FP_MOVE_SIZE:
            case DEPTH_MOD:
            case SETCONTRAST:
            case CONTRAST_LEFT:
            case CONTRAST_RIGHT:
            case CONTRAST_DIFF:
            case STIM_WIDTH:
            case STIM_HEIGHT:
            case VDISP_MOD:
            case CORRELATION:
            case SF:
            case SF_DIFF:
            case SF2:
            case INITIAL_DISPARITY:
            case PURSUIT_INCREMENT:
            case JDEATH:
            case FAKESTIM_SIGNAL:
            case PLAID_RATIO:
                nfplaces[i] = 4;
                break;
            case EXPT2_NSTIM:
            case NTRIALS_CODE:
            case SET_SEED:
            case SET_SEEDLOOP:
            case RC_SEED:
            case SEEDOFFSET:
            case EXPTYPE_NONE:
                nfplaces[i] = 0;
                break;
            default:
                nfplaces[i] = 2;
                break;
        }
    }
	codesend[UPLOAD_CODE] = SEND_USER_ONLY;
	codesend[UFF_PREFIX] = SEND_USER_ONLY;
}


/* ************************ Main ******************************** */
void SetPriority(int priority)
{
    int i;
#ifdef NDPRI
	if((i = schedctl(NDPRI,0,priority)) < 0)
	{
		fprintf(stderr,"Can't set max prioirty\n");
		if((i = schedctl(NDPRI,0,NDPLOMIN)) < 0)
			fprintf(stderr,"Can't set min prioirty\n");
	}
#endif
}

#ifndef NDPNORMMAX
#define NDPNORMMAX 20
#endif
#ifndef NDPNORMMIN
#define NDPNORMMIN 20
#endif









binocmain(argc, argv)
int argc;
char **argv;
{
    char *cl_str_var = NULL,c;
    int priority = (NDPNORMMAX+NDPNORMMIN)/2;
    char *ptr,*windowname;
    
    int i,n = 0,nfiles = 0,k;
    FILE *p,*fd;
    float framerate,val;
    char *command,result[BUFSIZ],buf[BUFSIZ],*s;
    time_t tval; 
    int stereo,reset = 0;
    int nf = 0,w,h;
    char estring[BUFSIZ];
    
    
    /* 
     * set up process with a non-degrading proiority, so that
     * a fast timer can be used, and set up the fast timer
     */
    
	if((i = CheckStrings()) != 0)
		exit(0);
    
#ifdef NIDAQ
    printf("Using DIO\n");
#endif
    
    
    /*
     * read in setup parameters for this machine
     * serial port names, screen size and location
     */
	if((fd = fopen("/local/binoc.setup","r")) != NULL)
    {
	    while(fgets(buf, BUFSIZ, fd) != NULL){
            s = strchr(buf,'=');
            if (!strncmp(buf,"winsize",6) && s){
                if(strchr(s,'x'))
                    sscanf(++s,"%dx%d",&w,&h);
                else
                    sscanf(++s,"%d %d",&w,&h);
                expt.winsiz[0] = w/2;
                expt.winsiz[1] = h/2;
                winsiz[0] = w/2;
                winsiz[1] = h/2;
                s = strchr(buf,'+');
                if(s){
                    i = sscanf(++s,"%d %d",&w,&h);
                    winpos[0] = w;
                    if (i > 1)
                        winpos[1] = h;
                }
            }
            else if(!strncmp(buf,"fullscreen",6) && s){
                sscanf(++s,"%d",&fullscreenmode);
            }
            else if(!strncmp(buf,"tty1",4) && s){
                sscanf(++s,"%s",theport);
            }
            else if(!strncmp(buf,"tty2",4) && s){
                sscanf(++s,"%s",stepperport);
            }
            else if(!strncmp(buf,"noDIO",5)){
                useDIO = 0;
            }
            else if(!strncmp(buf,"stepperid",8) && s){
                sscanf(++s,"%d",&i);
                SetMotorId(i-1); 
            }
            else if(!strncmp(buf,"electrode",8) && s){
                strcpy(estring,++s);
                AddElectrodeString(estring);
            }
            else if(!strncmp(buf,"user",4) && s){
                sscanf(++s,"%s",estring);
                AddUserString(estring);
            }
	    }
    }
    expt.mon = &mon;
	i = 1;
	psychoff[0] = winsiz[0]/2;
	psychoff[1] = 0;
    /*
     * first pass through argv leave any stimfiles named in argv
     */
    printf("VERSION %s\n",VERSION_NUMBER);
	
    while(i < argc) //Ali: for some reason xcode passes these additional arguments that messes up things.
    {
		if(argv[i][0] == '-'){
            if(strncmp(argv[i],"-seed",4) ==  0){
                argc--; 
                argv++;
                sscanf(argv[i],"%u",&expseed);
                argc--;
                argv++;
            }
            else if(strncmp(argv[i],"-codes",3) ==  0){
                PrintCodes(0);
                fflush(stdout);
#ifdef NIDAQ
                DIOClose();
                _Exit(0);
#else
                exit(0);
#endif
            }
            else if(strncmp(argv[1],"-demo",4) ==  0){
                argc--; 
                argv++;
                forcestart = 1;
                demomode = 1;
                mode |= NO_SERIAL_PORT;
            }
            else if(strncmp(argv[i],"-debug",3) ==  0){
                argc--; 
                argv++;
                sscanf(argv[i],"%d",&debug);
                argc--; 
                argv++;
            }
            else if(strncmp(argv[i],"-mimic",6) ==  0){
                mimic_fixation = 1;
                mode |= NO_SERIAL_PORT;
                if(strncmp(argv[i],"-mimicbias",8) ==  0)
                    mimic_bias = 1;
                argc--; 
                argv++;
                k = sscanf(argv[i],"%f",&val);
                if(k  > 0){
                    argc--; 
                    argv++;
                    mimicbadfix = val;
                    printf("Mimic %.2f Bad fixatoins\n",mimicbadfix);
                }
                
            }
            else if(strncasecmp(argv[i],"-nodio",5) ==  0){
                useDIO = 0;
                argc--; 
                argv++;
                i--;
            }
            else if(strncmp(argv[i],"-noserial",5) ==  0){
                mode |= NO_SERIAL_PORT;
                argc--; 
                argv++;
                i--;
            }

            else if(strncmp(argv[i],"-framepause",6) ==  0){
                argc--; 
                argv++;
                sscanf(argv[i],"%f",&framehold);
                argc--; 
                argv++;
            }
            else if(strncmp(argv[i],"-reset",5) ==  0){
		        reset = 1;
            }
            else if(strncmp(argv[i],"-replay",3) ==  0){
                argc--; 
                argv++;
                replay_expt = myscopy(replay_expt,argv[i]);
                argc--; 
                argv++;
            }
            else if(strncmp(argv[i],"-stereo",3) ==  0){
                stereo = 1;
                argc--; 
                argv++;
            }
            else if(strncmp(argv[i],"-po",3) ==  0){
                argc--;  argv++;
                sscanf(argv[i],"%f,%f",&psychoff[0],&psychoff[1]);
                argc--;  argv++;
                i--;
            }
            else if(strncmp(argv[i],"-position",6) ==  0){
                argv++; argc--;
                argv++; argc--;
                i--;
            }
            else if(strncmp(argv[i],"-codes",6) ==  0){
                PrintCodes(0);
                if(argc ==2)
#ifdef NIDAQ
                    DIOClose();
                _Exit(0);
#else
                exit(0);
#endif
            }
            else if(strncmp(argv[1],"-testl",6) ==  0){
                argc--; 
                argv++;
                sscanf(argv[1],"%d",&forcestart);
                argc--; 
                argv++;
            }
            else switch(argv[i][1])
            {
                case 's':
                case 'S':
                    argc--; 
                    argv++;
                    strcpy(theport,argv[1]);
                    argv++; argc--;
                    break;
                case 'v':
                case 'V':
                    if(argc ==2)
#ifdef NIDAQ
                        DIOClose();
                    _Exit(0);
#else
                    exit(0);
#endif
                    break;
                case 'w':
                case 'W':
                    sscanf(argv[i+1],"%dx%d",&winsiz[0],&winsiz[1]);
                    winsiz[0] /= 2;
                    winsiz[1] /= 2;
                    argv++; argc--;
                    expt.winsiz[0] = winsiz[0];
                    expt.winsiz[1] = winsiz[1];
                    argv++; argc--;
                    break;
                    
            }
            i++;
		}
		else
            i++;
	}
    
	initial_setup();
	n = 0;
    
	expt.polygonsmooth = 1;
    
    
    
	if((i = OpenSerial(theport)) <= 0)
		mode |= NO_SERIAL_PORT;
    
    
	OpenPenetrationLog(0); /* open a dummy so ed is ALWAYS saved */
    
	while(argc >1)
	{
        if(argv[1][0] == '-'){
            if(strncmp(argv[1],"-seed",4) ==  0){
                argc--; 
                argv++;
                sscanf(argv[1],"%u",&expseed);
            }
            if(strncmp(argv[1],"-fixframes",6) ==  0){
                optionflags[FIXNUM_PAINTED_FRAMES] = 1;
            }
            else if(strncmp(argv[1],"-go",3) ==  0){
                forcestart = 1;
                if(strncmp(argv[1],"-gotest",3) ==  0)
                    mode |= TEST_PENDING;
                
            }
            else if(strncmp(argv[1],"-demo",4) ==  0){
                forcestart = 1;
                demomode = 1;
            }
            else if(strncmp(argv[1],"-blendonly",7) ==  0){
                expt.polygonsmooth = 0;
            }
            else if(strncmp(argv[1],"-NSDocumentRevision",12) ==  0){
                argc--; argv++;
            }
            else if(strncmp(argv[1],"-testrc",6) ==  0){
                testflags[TEST_RC] = 1;
            }
            else if(strncmp(argv[1],"-fakespikes",6) ==  0){
                testflags[FAKE_SPIKES] = 1;
            }
            else if(strncmp(argv[1],"-anaglyph",6) ==  0){
                testflags[ANAGLYPH] = 1;
            }
            else if(strncmp(argv[1],"-saveim",6) ==  0){
                testflags[SAVE_IMAGES] = 1;
                argc--; 
                argv++;
                strcpy(ImageOutDir,argv[1]);
                sprintf(buf,"%s/pgm.idx",ImageOutDir);
                imidxfd = fopen(buf,"w");
            }
            else switch(argv[1][1])
            {
                case 'd':
                    argc--; 
                    argv++;
                    sscanf(argv[1],"%d",&debug);
                    break;
                case 'b':
                    option2flag ^= PRETRIAL_BRIGHT;
                    break;
                case 'e':
                case 'E':
                    argc--; 
                    argv++;
                    loadfiles[0] = myscopy(loadfiles[0],argv[1]);
                    afc_s.stairhist = newstairstruct(); /* calculates correct starting value and sets up histogram structure */
                    break;
                case 'g':
                    if(strncmp(argv[1],"-gamma",4) == 0)
                    {
                        argc--; 
                        argv++;
                        sscanf(argv[1],"%lf",&gammaval);
                    }
                    break;
                case 'l':
                case 'L':
                    argc--; 
                    argv++;
                    logname = argv[1];
                    if((logfd = fopen(argv[1],"a")) == NULL)
                        fprintf(stderr,"Can't open %s for log\n",argv[1]);
                    else{
                        tval = time(NULL);
                        fprintf(logfd,"Start %s",ctime(&tval));
                    }
                    break;
                case 'm':
                case 'M':
                    i = 2;
                    while(argv[1][i] != 0)
                    {
                        if(argv[1][i] == 'p')
                            option2flag |= PSYCHOPHYSICS_BIT;
                        if(argv[1][i] == 'c'){
                            optionflags[STIMULUS_IN_OVERLAY] = 1;
                        }
                        if(argv[1][i] == 'l'){
                            PrintCodes(0);
#ifdef NIDAQ
                            DIOClose();
#endif
                            exit(0);
                        }
                        if(argv[1][i] == 'c')
                            ttys[0] = -1;
                        i++;
                    }
                    break;
                case 'p':
                case 'P':
                    if(strncmp(argv[0],"-po",3) ==  0)
                        sscanf(argv[1],"%f,%f",&psychoff[0],&psychoff[1]);
                    else
                        priority = atoi(argv[1]);
                    argc--; 
                    argv++;
                    break;
                case 't':
                case 'T':
                    argc--; 
                    argv++;
                    if(argv[1][0] == '-')
                    {
                        testfd = stderr;
                        fprintf(stderr,"Messages to stderr\n");
                    }
                    else if((testfd = fopen(argv[1],"w")) != NULL)
                        fprintf(stderr,"Messages to %s\n",argv[1]);
                    else
                        fprintf(stderr,"Can't open %s\n",argv[1]);
                    break;
                case 'v':
                    verbose = 1;
                    while(argv[1][verbose] == 'v')
                        verbose++;
                    verbose--;
                    printf("Verbose %d\n",verbose);
                    break;
                default:
                    break;
            }
        }
        else
	    {
            if(verbose)
                printf("Reading %s....\n",argv[1]);
            loadfiles[nfiles] = myscopy(loadfiles[nfiles],argv[1]);
            afc_s.stairhist = newstairstruct(); 
            if(verbose)
                printf("Reading %d....\n",loadfiles[nfiles]);
            //Ali: for some reason xcode passes these additional arguments that messes up things.
            if(ReadExptFile(loadfiles[nfiles],2,0,1) < 0){
             fprintf(stderr,"Error Reading %s - Check Path\n",loadfiles[nfiles]);
             //Ali: exit_program();
             } 
             
            nfiles++;
            /* 
             * the exptfile will be re-read after the interface is made below.
             * that is the time to execute any commands. Without this a trailing "\quit"
             * causes an exit on the next call to ReadExptFile
             */
            command_pending = 0;
	    }
        argc--; 
        argv++;
        
	}
    
    
	gettimeofday(&timeb,NULL);
	if(verbose)
        printf("Initializing...\n");
	init_stimulus(TheStim);
	if(TheStim->next != NULL)
		init_stimulus(TheStim->next);
	if(verbose)
        printf("Interface....\n");
    
    
	for(i = 0; i < nfiles; i++){
        /*
         * ReadExptFile may already have been called above. Reset anything here that
         * should be in a fixed state a program start.
         */
        if(i == 0){
            rfctr = 0;
        }
        //Ali: for some reason xcode passes these additional arguments that messes up things.
        /*Ali
         if(ReadExptFile(loadfiles[i],1,1,!nfiles) < 0)
         {
         fprintf(stderr,"Error Reading %s - Check Path\n",loadfiles[i]);
         #ifdef NIDAQ
         DIOClose();
         #endif
         exit_program();
         //	    exit(0);
         }
         */
	}

	setgamma(gammaval);
	SetPriority(priority);
	MakeConnection(0);
    gettimeofday(&progstarttime,NULL);
	
	framerate = mon.framerate;
	mon.framerate = GetFrameRate();
	TheStim->incr = TheStim->incr * framerate/mon.framerate;
	printf("FrameRate %.2f\n",mon.framerate);
	SerialSend(FRAMERATE_CODE);
}


int SendTrialCount()
{
    char buf[BUFSIZ];
    int stim;
    
    if (stimstate == POSTTRIAL)
        stim = stimno;
    else if (stimstate == WAIT_FOR_RESPONSE)
        stim = stimno+1;
    else
        stim = stimno;
    
    sprintf(buf,"STIMC %d %d %d %d %d %d %d %.1f\n",goodtrials, totaltrials, badtrials, latetrials, fixtrials,stim,expt.nreps*expt.nstim[5],totalreward);
    notify(buf);
}

void SendToGui(int code)
{
    int i;
    time_t tval;
    char buf[BUFSIZ];
            
    MakeString(code, buf, &expt, expt.st,TO_GUI);
    notify(buf);
    
}

void SendAllToGui()
{
    int i,j;
    time_t tval;
    char buf[BUFSIZ];
    
    
    if(ChoiceStima->type != STIM_NONE){
        notify("mo=ChoiceU\n");
        for(i = 0; i <= expt.laststimcode; i++)
        {
            buf[0] = '=';
            buf[1] = 0;
            if((j = MakeString(valstrings[i].icode, buf, &expt, ChoiceStima,TO_GUI)) >= 0)
                notify(buf);
        }
    }
    if(ChoiceStimb->type != STIM_NONE){
        notify("mo=ChoiceD\n");
        for(i = 0; i <= expt.laststimcode; i++)
        {
            buf[0] = '=';
            buf[1] = 0;
            if((j = MakeString(valstrings[i].icode, buf, &expt, ChoiceStimb,TO_GUI)) >= 0)
                notify(buf);
        }
    }

    if(expt.st->next != NULL){
        notify("mo=back\n");
        for (i = 0; i < expt.laststimcode;  i++)
        {
            MakeString(valstrings[i].icode, buf, &expt, expt.st->next,TO_GUI);
            notify(buf);
        }
        MakeString(STIMULUS_FLAG, buf, &expt, expt.st->next,TO_GUI);
        notify(buf);
        
    }
    notify("mo=fore\n");
    for(i = 0; i < expt.totalcodes; i++){
        if((j=MakeString(valstrings[i].icode, buf, &expt, expt.st,TO_GUI))>=0)
            notify(buf);
    }
    i =0;

    if(expt.st->imprefix != NULL){
        sprintf(buf,"impref=%s\n",expt.st->imprefix);
        notify(buf);
        if(expt.st->immode == IMAGEMODE_ORBW)
            notify("immode=orbw\n");
        else if(expt.st->immode == BINOCULAR_PLAIN_IMAGES)
            notify("immode=binocular\n");
        if(expt.st->preload)
            notify("imload=preload\n");
        else
            notify("imload=load\n");
    }

    ListExpStims(NULL);
    ListQuickExpts();
}


double GetFrameRate()
{
    double fz = 0;
    //For now always get screen 1 - the one without the main menu bar
    if (winpos[0] > 0)
        fz = GetCurrentFrameRate(1);
    else
        fz = GetCurrentFrameRate(1);

    if (fz <= 0){
        if(expt.vals[FRAMERATE_CODE] > 0)
            fz = expt.vals[FRAMERATE_CODE];
        else
            fz = 60;
    }
    
    return(fz);
}


void SendAll()
{
    int i,code;
    time_t tval;
    
    for(i = 0; i < expt.lastserialcode; i++){
        code = valstrings[i].icode;
        if(codesend[code] != SEND_USER_ONLY)
            SerialSend(code);
    }
    SendPenInfo();
    tval = time(NULL);
    if(seroutfile)
        fprintf(seroutfile,"#SendAll at %s",ctime(&tval));
}

void MakeConnection()
{
	int i;
    
	if(!(mode & SERIAL_OK))
	{
		SerialSignal(BW_IS_READY);
		if(CheckBW(BW_IS_READY,"Initial"))
			mode |= SERIAL_OK;
	}
	if(mode & SERIAL_OK)
	{
        SerialString("NewConnection\n",0);
        SendAll();
        SerialSend(ELECTRODE_DEPTH);
        SerialSend(PEN_START_DEPTH);
        gettimeofday(&sessiontime,NULL);
        SerialSignal(END_EXPT);
	}
}




void glstatusline(char *s, int line)
{
    static char *lines[10] = {NULL};
    vcoord x[2];
    
    if(!(mode & RUNNING) || (optionflags[HIDE_STATUS]))
        return;
    if(line < 10 && s != NULL)
        lines[line] = myscopy(lines[line],s);
    if(s != NULL) /* new string */
        glDrawBuffer(GL_FRONT_AND_BACK);
    if(optionflag & SHOW_STIMVAL_BIT)
        setmask(ALLPLANES);
    else
        setmask(OVERLAY);
    SetGrey(TheStim->gammaback);
    myrect(x[0],x[1]-5,x[0]+400,x[1]+15);
	SetGrey(1.0);
	mycmv(x);
	if(s != NULL){
        printStringOnMonkeyView(s, strlen(s));
        printString(s, strlen(s));
    }
	else if(lines[line] != NULL)
        printStringOnMonkeyView(lines[line], strlen(lines[line]));
	if(states[EXPT_PAUSED]){
        x[1] = -winsiz[1] + 150;
        mycmv(x);
        statusline("Paused");
	}
	if(freezeexpt){
        x[1] = -winsiz[1] + 150;
        mycmv(x);
        statusline("Expt Frozen");
	}
    glDrawBuffer(GL_BACK);
}

void statusline(char *s)
{
    char buf[BUFSIZ];
    
    
    if(s != NULL)
        statusstring = myscopy(statusstring,s);
    else
        s = statusstring;
    glstatusline(s,1);
    notify("status=");
    notify(s);
    notify("\n");
    if(mode & HOLD_STATUS)
        return;
    
}

void ResetProjectionMatrix()
{
    glPopMatrix();
    glPushMatrix();
    return;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmatrix);
    //    glLoadIdentity();
}


int TrialOver()
{
    Stimulus *st = expt.st;
    
    
    if((st->type == STIM_CYLINDER || st->type == STIM_RDS) && expt.vals[INITIAL_DISPARITY] != 0){
        if(st->left->ptr->velocity == 0 && seroutfile)
            fprintf(seroutfile,"resetting velocity to %.2f\n",oldvelocity);
        st->disp = olddisp;
        st->left->ptr->velocity = oldvelocity;
        st->posinc = oldvelocity;
        st->flag = stimflag;
        if(st->left->ptr->velocity < 0.0001 && seroutfile)
            fprintf(seroutfile,"#Cyl velocity %.6f (%.f)\n",st->left->ptr->velocity,oldvelocity);
    }
    if(fabs(expt.vals[PURSUIT_INCREMENT]) > 0.001){
        SerialSend(FIXPOS_XY);
        //	SerialSend(XPOS);
        //	SerialSend(YPOS);
    }
    
}

void StopGo(int go)
{ 
    char buf[256];
    stopgo = go;
    
    if(go == GO)
    {
        mode &= (~BW_ERROR);
        optionflag |= GO_BIT;
        mode |= ANIMATE_BIT;
        monkeypress = 0;
        end_timeout();
        sprintf(buf,"%2s-\n",valstrings[valstringindex[STOP_BUTTON]].code);
        SerialString(buf,0);
        if(!(TheStim->mode & EXPTPENDING) && expt.st->left->ptr->velocity > 0){
            oldvelocity = expt.st->left->ptr->velocity;
            stimflag = expt.st->flag;
            if(expt.st->type == STIM_RDS)
                oldvelocity = expt.st->posinc;
            olddisp = expt.st->disp;
        }
    }
    /* 
     *Requests to stop during the POSTSTIMULUS period are denied - 
     * monkey completed the trial and is about to get the reward
     * so wait for that
     */
    else if(stimstate != POSTSTIMULUS || testflags[PLAYING_EXPT])
    {
        optionflag &= (~GO_BIT);
        mode &= (~(ANIMATE_BIT | TEST_PENDING));
        monkeypress = WURTZ_STOPPED;
        sprintf(buf,"%2s+\n",valstrings[valstringindex[STOP_BUTTON]].code);
        SerialString(buf,0);
        if(stimstate == INSTIMULUS)
            TrialOver();
        start_timeout(SEARCH);
        if(stimstate == IN_TIMEOUT)
            stimstate = STIMSTOPPED;
        SendAllToGui();
    }
}

void ButtonDrag(vcoord *start, vcoord *end, WindowEvent e)
{
    vcoord mpos[2];
    Locator *pos = &TheStim->pos;
    double scale;
    
    mpos[0] = e.mouseX;
    mpos[1] = e.mouseY;
    
    if(mode & PLACE_PLOT_BIT && eventstate & LBUTTON)
    {
        StartOverlay();
        Box(start[0],start[1],end[0],end[1],TheStim->gammaback);
        Box(start[0],start[1],mpos[0],mpos[1],PLOT_COLOR);
        EndOverlay();
        glFinish();
        end[0] = mpos[0];
        end[1] = mpos[1];
    }
    /*
     * in psychophysics mode, the mouse controls stimulus times, not location
     * so dont process drags. If you want to drag a stimulus around interactively
     * turn off stimulus_in_wurtz.
     */
    
    if(option2flag & PSYCHOPHYSICS_BIT && (optionflag & STIM_IN_WURTZ_BIT))
    {
        if(!(eventstate & (RBUTTON | LBUTTON | MBUTTON))) // no mouse button down
        {
            if(isharris(altstimmode) && stimstate == WAIT_FOR_RESPONSE){
                mpos[0] = e.mouseX;
                mpos[1] = e.mouseY;
                glLineWidth(2.0);
                VisLine(0,0,end[0],end[1],TheStim->gammaback);
                if(altstimmode == HARRIS_TASK_ANGLE){
                    scale = 100/sqrt(sqr(mpos[0] - winsiz[0])+sqr(mpos[1]-winsiz[1]));
                    end[0] = winsiz[0] + (mpos[0]-winsiz[0]) * scale;
                    end[1] = winsiz[1] + (mpos[1]-winsiz[1]) * scale;
                }
                else{
                    end[0] = mpos[0];
                    end[1] = mpos[1];
                }
                VisLine(0,0,end[0],end[1],1.0);
                glDrawBuffer(GL_FRONT_AND_BACK);
            }
        }
        return;
    }
    if(eventstate & LBUTTON)
    {
        stmode |= (MOVED_STIMULUS | DRAG_STIMULUS);
        LocateStimulus(stimptr, e.mouseX, e.mouseY);
        sprintf(mssg,"at %.2f,%.2f\n",StimulusProperty(stimptr,SETZXOFF),
                StimulusProperty(stimptr,SETZYOFF));
        glstatusline(mssg,2);
    }
    else if(eventstate & MBUTTON)
    {
    }
    if(eventstate & RBUTTON)
    {
        stmode |= (MOVED_STIMULUS | DRAG_STIMULUS);
        RotateStimulus(e.mouseX,e.mouseY);
        sprintf(mssg,"Orientation %.1f\n",StimulusProperty(stimptr,ORIENTATION));
        glstatusline(mssg,2);
    }
}

void CntrlDrag(vcoord *start, vcoord *end,  WindowEvent e)
{
    vcoord mpos[2];
    float val;
    char s[256];
    mpos[0] = e.mouseX;
    mpos[1] = e.mouseY;
    StartOverlay();
    if(eventstate & LBUTTON)
    {
        ShowBox(expt.rf, TheStim->gammaback);
        expt.rf->pos[0] = mpos[0]-winsiz[0];
        expt.rf->pos[1] = mpos[1]-winsiz[1];
        ShowBox(expt.rf, 1.0);
    }
    else if(eventstate & MBUTTON)
    {
        ShowBox(expt.rf,TheStim->gammaback);
        win2stim(mpos,expt.rf->pos,expt.rf->angle,end);
        expt.rf->size[0] = end[0] < 0 ? -2 * end[0] : 2 * end[0];
        expt.rf->size[1] = end[1] < 0 ? -2 * end[1] : 2 * end[1];
        ShowBox(expt.rf,1.0);
    }
    else if(eventstate & RBUTTON)
    {
        ShowBox(expt.rf,TheStim->gammaback);
        win2stim(mpos,expt.rf->pos,0.,end);
        val = atan2((double)(expt.rf->size[1]),(double)(expt.rf->size[0]));
        expt.rf->angle = (atan2((float)end[1],(float)end[0])-val) * 180/M_PI;
        ShowBox(expt.rf,1.0);
    }
    sprintf(s,"RF: %.2f,%.2f: %.2fx%.2f, %.2f deg",
            pix2deg(expt.rf->pos[0]),
            pix2deg(expt.rf->pos[1]),
            pix2deg(expt.rf->size[0]),
            pix2deg(expt.rf->size[1]),
            expt.rf->angle);
    glstatusline(s,2);
    EndOverlay();
    glFinish();
}

void ShiftDrag(vcoord *start, vcoord *end, WindowEvent e)
{
    vcoord mpos[2];
//    Expstim *es,*esp;
    short *pl;
    int nstim;
    
//    es = expt.plot->stims;
    mpos[0] = e.mouseX;
    mpos[1] = e.mouseY;
    if(eventstate & LBUTTON)
    {
        MyLine(start[0],start[1],end[0],end[1],TheStim->gammaback);
        end[0] = mpos[0];
        end[1] = mpos[1];
        MyLine(start[0],start[1],end[0],end[1],1.0);
    }
    if(eventstate & MBUTTON)
    {
//        StimLine(start,es,TheStim->gammaback);
//        StimLine(mpos,es,BOX_COLOR);
        start[0] = e.mouseX;
        start[1] = e.mouseY;
    }
}


void ButtonReleased(vcoord *start, vcoord *end, WindowEvent e)
{
    vcoord mpos[2];
    char s[256];
    
    if(fixstate == 0 && !(option2flag & PSYCHOPHYSICS_BIT)){
        /*
         * ? allow middle button press to simulate fixation? 
         * OK if a real bad fixation signal back from BW cancels trial....
         */
        if(e.mouseButton == Button2){
            stimstate = PREFIXATION;
            GotChar(START_STIM);
            mimic_fixation |= MIMIC_FIXATION;
        }
    }
    else if(fixstate == WAIT_FOR_MOUSE){
        fixstate = RESPONDED;
        if(e.mouseButton == Button1){
            GotChar(WURTZ_OK);
        }
        if(e.mouseButton == Button2){
            GotChar(WURTZ_LATE);
        }
        if(e.mouseButton == Button3){
            GotChar(WURTZ_OK_W);
        }
        fixstate = RESPONDED;
        stimstate = WAIT_FOR_RESPONSE;
    }
    else if(!(option2flag & PSYCHOPHYSICS_BIT))
    {
        if(e.mouseButton == Button1 || e.mouseButton == Button3)
        {
            stmode |= MOVED_STIMULUS;
            mode |= NEWPOS;
            sprintf(mssg,"At %.2f, %.2f, Ori %.1f",StimulusProperty(stimptr,SETZXOFF),
                    StimulusProperty(stimptr,SETZYOFF),
                    StimulusProperty(stimptr,ORIENTATION));
            glstatusline(mssg,2);
        }
    }
    if(e.mouseButton == Button2 && TheStim->type == STIM_NONE){
        RewardOn(0);
    }
    glDrawBuffer(GL_BACK);
}

void ButtonDown(vcoord *start, vcoord *end, WindowEvent e)
{
    vcoord mpos[2];
    char s[256],buf[BUFSIZ];
    Locator *pos = &TheStim->pos;
    int next,up,id;
    float pause = 0;
    static int lastbutton = 0;
    
    mpos[0] = e.mouseX;
    mpos[1] = e.mouseY;
    gettimeofday(&now, NULL);
    pause = timediff(&now,&firstframetime);
    //  printf("B%d at %.3f\n",e.mouseButton,pause);
    
    
    if(e.mouseButton == Button4 || e.mouseButton == Button5)
        return;
    if(eventstate & LBUTTON && mode & PLACE_PLOT_BIT)
    {
        end[0] = start[0] = mpos[0];
        end[1] = start[1] = mpos[1];
    }
    else if(option2flag & PSYCHOPHYSICS_BIT) switch(stimstate)
    {
        case INSTIMULUS:
        case POSTSTIMULUS:
            stimstate = POSTPOSTSTIMULUS;
            next_frame(TheStim);
        case WAIT_FOR_RESPONSE:
            if(e.mouseButton == Button2){
                if(freezeexpt){
                    stimstate = POSTTRIAL;
                    fixstate =RESPONDED;
                }
                if(stimstate != INSTIMULUS || pause > 0.2)
                    stimstate = WAIT_FOR_RESPONSE;
                if(optionflag & SEARCH_MODE_BIT){
                    stimstate = PREFIXATION;
                    stimstate = POSTTRIAL;
                    TurnBackStim(expt.st);
                }
            }
            else if(e.mouseButton == Button1 || e.mouseButton == Button3)
                
            {
                endpt[0] -= winsiz[0];
                endpt[1] -= winsiz[1];
                up = ButtonResponse(e.mouseButton,0,endpt);
                stimstate = INTERTRIAL;
                stimstate = POSTTRIAL;
                fixstate = RESPONDED;
                if(option2flag & STAIRCASE){
                    SetPsychStair(up,0);
                }
                stimno++;
                sprintf(buf,"TRES G%d 1\n",2-e.mouseButton); //L=1,R=-1
                notify(buf);

            }
            lastbutton = e.mouseButton;
            break;
        case INTERTRIAL:
//            if(e.mouseButton == Button2){
            if(e.mouseButton == lastbutton){
                stimstate = PREFIXATION;
                gettimeofday(&now, NULL);
                if(seroutfile)
                    fprintf(seroutfile,"Trial Start %u\n",ufftime(&now));
            }
            else if(!optionflags[FEEDBACK])
            /*
             * only allow revision of choices if there is no feedback
             */
            {
                if(e.mouseButton == Button1 || e.mouseButton == Button3){
                    up = ButtonResponse(e.mouseButton,1,endpt);
                    if(option2flag & STAIRCASE && stimno >=0){
                        SetPsychStair(up,1);
                    }
                }
            }
            lastbutton = e.mouseButton;
            break;
    }
    else if(fixstate != WAIT_FOR_MOUSE)
    {
        if(e.mouseButton == Button1)
        {
            stmode |= MOVED_STIMULUS;
            LocateStimulus(stimptr, e.mouseX,e.mouseY);
            sprintf(s,"At %.2f,%.2f (%d,%d)",
                    pix2deg(pos->xy[0]),
                    pix2deg(pos->xy[1]),
                    e.mouseX,e.mouseY);
            statusline(s);
        }
        else if(e.mouseButton == Button3)
        {
            RotateStimulus(e.mouseX,e.mouseY);
            stmode |= MOVED_STIMULUS;
        }
    }
}

void CntrlButtonDown(vcoord *start, vcoord *end, WindowEvent e)
{
    vcoord mpos[2];
    float val;
    mpos[0] = e.mouseX;
    mpos[1] = e.mouseY;
    eventstate |= CNTLKEY;
    
    StartOverlay();
    if(e.mouseButton == Button1)
    {
        ShowBox(expt.rf,TheStim->gammaback);
        expt.rf->pos[0] = mpos[0]-winsiz[0];
        expt.rf->pos[1] = mpos[1]-winsiz[1];
        ShowBox(expt.rf,RF_COLOR);
    }
    if(e.mouseButton == Button2)
    {
        ShowBox(expt.rf,TheStim->gammaback);
        win2stim(mpos,expt.rf->pos,expt.rf->angle,end);
        expt.rf->size[0] = end[0] < 0 ? -2 * end[0] : 2 * end[0];
        expt.rf->size[1] = end[1] < 0 ? -2 * end[1] : 2 * end[1];
        ShowBox(expt.rf,RF_COLOR);
    }
    if(e.mouseButton == Button3)
    {
        val = atan2((double)(expt.rf->size[1]),(double)(expt.rf->size[0]));
        ShowBox(expt.rf,TheStim->gammaback);
        win2stim(mpos,expt.rf->pos,0,end);
        expt.rf->angle = (atan2((float)end[1],(float)end[0])-val) * 180/M_PI;
        ShowBox(expt.rf,RF_COLOR);
    }
    if(e.mouseButton == Button4)
    {
        ShowBox(expt.rf,TheStim->gammaback);
        expt.rf->size[0] = expt.rf->size[0] + 10;
        expt.rf->size[1] = expt.rf->size[1] + 10;
        ShowBox(expt.rf,RF_COLOR);
    }
    if(e.mouseButton == Button5)
    {
        ShowBox(expt.rf,TheStim->gammaback);
        expt.rf->size[0] = expt.rf->size[0] - 10;
        expt.rf->size[1] = expt.rf->size[1] - 10;
        ShowBox(expt.rf,RF_COLOR);
    }
    EndOverlay();
    glFlush();
    glFinish();
}


void CntrlButtonRelease(vcoord *start, vcoord *end, WindowEvent e)
{			    
    char s[256],*ss;
    
    sprintf(s,"RF: %.2f,%.2f: %.2fx%.2f, %.2f deg",
            pix2deg(expt.rf->pos[0]),
            pix2deg(expt.rf->pos[1]),
            pix2deg(expt.rf->size[0]),
            pix2deg(expt.rf->size[1]),
            expt.rf->angle);
    
    statusline(s);
    glstatusline(s,2);
    ss = SerialSend(RF_DIMENSIONS);
    if(penlog)
        fprintf(penlog,"%s\n",ss);
}


void ShiftButtonDown(vcoord *start, vcoord *end, WindowEvent e)
{
    vcoord mpos[2];
//    Expstim *es,*esp;
    short *pl;
    int nstim, i = 0;
    
  //  es = expt.plot->stims;
    mpos[0] = e.mouseX;
    mpos[1] = e.mouseY;
    eventstate |= SHIFTKEY;
    
    StartOverlay();
    if(e.mouseButton == Button1)
    {
        end[0] = start[0] = mpos[0];
        end[1] = start[1] = mpos[1];
    }
    if(e.mouseButton == Button2)
    {

    }
}



void ShiftButtonRelease(vcoord *start, vcoord *end, WindowEvent e)
{
    vcoord mpos[2];
//    Expstim *es;
    short *pl;
    
//    es = expt.plot->stims;
    mpos[0] = e.mouseX;
    mpos[1] = e.mouseY;
    StartOverlay();
    if(e.mouseButton == Button1)
    {
        MyLine(start[0],start[1],end[0],end[1],expt.overlay_color);
        end[0] = mpos[0];
        end[1] = mpos[1];
        MyLine(start[0],start[1],end[0],end[1],LINES_COLOR);
        if(expt.nlines < MAXUSERLINES)
        {
            pl = &expt.plot->linedata[expt.nlines*4];
            *pl++ = start[0];
            *pl++ = start[1];
            *pl++ = end[0];
            *pl++ = end[1];
            expt.nlines++;
            ClearStimLine(expt.nlines);
        }
    }
    else if(e.mouseButton == Button2)
    {
//        StimLine(start,es,expt.overlay_color);
//        es->phasemark = StimLine(mpos,es,BOX_COLOR);
//        es->flag |= PHASEMARKER_ON;
    }
    EndOverlay();
    glFinish();
    eventstate &= (~SHIFTKEY);
}


int HandleMouse(WindowEvent e)
{
    int i, j, mask,ctr,nstim,estim = 0,handle,res = 0,nf;
    static vcoord start[2];
    short rgbvec[3],*pl;
    char c,s[256];
    float angle = 90;
    float aval;
//    Expstim *es,*esp;
    Locator *pos = &TheStim->pos;
    
    char cbuf[256];
    static char instring[256];
    static int sctr = 0;
    
    /* we only use events in the display window, except 
     *   F6 whcich sends  a free reward 
     *   F1 which runs one trial
     * and we want to intercept these anywhere
     */
    i = 0;
    handle = 0;
    
    
    switch (e.eventType) {
        case ButtonRelease:
            if(eventstate & SHIFTKEY)
                ShiftButtonRelease(start, endpt, e);
            else if(eventstate & CNTLKEY)
                CntrlButtonRelease(start, endpt, e);
            else 
                ButtonReleased(start, endpt, e);
            eventstate = 0;
            break;
        case ButtonPress:
            eventstate = 0;
            if(e.mouseButton == Button1)
                eventstate |= LBUTTON;
            if(e.mouseButton == Button2){
                eventstate |= MBUTTON;
                if(TheStim->type == STIM_NONE && !(e.modifierKey & ShiftMask) && 
                   !(e.modifierKey & ControlMask))
                    RewardOn(1);
            }
            if(e.mouseButton == Button3)
                eventstate |= RBUTTON;
            if(e.modifierKey & ShiftMask)
                ShiftButtonDown(start, endpt, e);
            else if(e.modifierKey & ControlMask)
                CntrlButtonDown(start, endpt, e);
            else
                ButtonDown(start, endpt, e);
            /*
             * in darwin can get erroneous Button4 events from a middle mouse press
             */
            if(e.mouseButton == Button4)
                TheStim->pos.angle += 0.02;
            if(e.mouseButton == Button5)
                TheStim->pos.angle -= 0.02;
            
            break;
        case MotionNotify:
            /*
             * N.B. Motion events are processed according to the cntrl/shiftkey status at the
             * time thhe button was first pressed, using eventstate. This way releasing the
             * shift key during a drag doesn't cause problems
             */
            if(eventstate & SHIFTKEY){
                ShiftDrag(start, endpt, e);
                if(expt.nlines < MAXUSERLINES)
                {
                    pl = &expt.plot->linedata[expt.nlines*4];
                    *pl++ = start[0];
                    *pl++ = start[1];
                    *pl++ = endpt[0];
                    *pl++ = endpt[1];
                }
            }
            else if(eventstate & CNTLKEY)
                CntrlDrag(start, endpt, e);
            else if(e.modifierKey & ControlMask)
                ShowPos(e.mouseX,e.mouseY);
            else
                ButtonDrag(start, endpt, e);
            break;
    }
    
    
    return(res);
}



void RunOneTrial(){
    states[ONE_TRIAL] = 1;
    StopGo(GO);
}

void SetStopButton(int onoff)
{
    
    if(TheStim->mode & EXPTPENDING) /* cant play with this button  in expts */
        return;
    if((onoff == GO && monkeypress == WURTZ_STOPPED) ||
       (onoff == STOP && monkeypress != WURTZ_STOPPED))
    {
        /* 
         * NB this is confusing. Must do the opposite of what we want here, since
         * the call to handle_pushbuttons toggles ANIMATE_BIT
         * EG: GO unsets ANIMATE_BIT, whihc is then set in handle_pusbuttons
         */
        if(onoff == STOP)
            mode |= ANIMATE_BIT;
        if(onoff == GO)
            mode &= (~ANIMATE_BIT);
    }
}

void exit_program()
{
    /*
     if(!(mode & SAVED_LOG))
     SaveLog();
     */
    if(TheStim->mode & EXPTPENDING){
        expt_over(1);
    }
    closeserial(0);
    closeserial(1);


#ifdef NIDAQ
    DIOClose();
    _Exit(0);
#else
	exit(0);
#endif
}




void Box(int a, int b, int c, int d, float color)
{
    vcoord pt[2];
    glDrawBuffer(GL_FRONT_AND_BACK);
    setmask(OVERLAY);
    SetGrey(color);
    glBegin(GL_LINE_LOOP);
    pt[0] = a-winsiz[0];
    pt[1] = winsiz[1] - b;
    myvx(pt);
    pt[0] = a - winsiz[0]; 
    pt[1] = winsiz[1] - d;
    myvx(pt);
    pt[0] = c - winsiz[0]; 
    pt[1] = winsiz[1] - d;
    myvx(pt);
    pt[0] = c - winsiz[0]; 
    pt[1] = winsiz[1] - b;
    myvx(pt);
    pt[0] = a - winsiz[0]; 
    pt[1] = winsiz[1] - b;
    myvx(pt);
    glEnd();
    glDrawBuffer(GL_BACK);
}



void VisLine(int a, int b, int c, int d, float color)
{
    vcoord pt[2];
    glDrawBuffer(GL_FRONT_AND_BACK);
    setmask(ALLPLANES);
    SetGrey(color);
    glBegin(GL_LINE_STRIP);
    pt[0] = a;
    pt[1] = -b;
    myvx(pt);
    if(optionflags[NO_MIRRORS])
        pt[0] = c - winsiz[0]; 
    else
        pt[0] =  winsiz[0]-c; 
    pt[1] = d - winsiz[1];
    myvx(pt);
    glEnd();
    glDrawBuffer(GL_BACK);
    
}

float getangle(vcoord *wp, vcoord *sp)
{
    float sx,sy,res;
    sy = (wp[1]-winsiz[1])-sp[1];
    sx  = (wp[0]-winsiz[0])-sp[0];
    res = atan2(sy,sx);
    return(res);
}

void win2stim(vcoord *wp, vcoord *sp, float angle, vcoord *result)
{
    int sx,sy;
    float cx,cy,sina,cosa;
    
    sy = (wp[1]-winsiz[1])-sp[1];
    sx  = (wp[0]-winsiz[0])-sp[0];
    sina = sin(angle*M_PI/180);
    cosa = cos(angle*M_PI/180);
    cx = sx * cosa +sy* sina;
    cy = sy * cosa - sx * sina;
    result[0] = (vcoord)cx;
    result[1] = (vcoord)cy;
}

int DrawLine(vcoord a, vcoord b, vcoord c, vcoord d, float color)
{
    vcoord pt[2];
    /*  glDrawBuffer(GL_FRONT_AND_BACK);*/
    setmask(OVERLAY);
    SetGrey(color);
    glBegin(GL_LINE_STRIP);
    pt[0] = deg2pix(a);
    pt[1] = deg2pix(b);
    myvx(pt);
    pt[0] = deg2pix(c);
    pt[1] = deg2pix(d);
    myvx(pt);
    glEnd();
}

int MyLine(int a, int b, int c, int d, float color)
{
    vcoord pt[2];
    /*  glDrawBuffer(GL_FRONT_AND_BACK);*/
    setmask(OVERLAY);
    SetGrey(color);
    glBegin(GL_LINE_STRIP);
    pt[0] = a-winsiz[0];
    pt[1] = b-winsiz[1];
    myvx(pt);
    pt[0] = c - winsiz[0]; 
    pt[1] = d -winsiz[1];
    myvx(pt);
    glEnd();
}


vcoord StimLine(vcoord *pos, Expstim *es, float color)
{
    int sx,sy;
    float cx,cy;
    vcoord pt[2];
    /*
     * convert from root window co-ordinates to stimulus co-ordinates
     */
    glDrawBuffer(GL_FRONT_AND_BACK);
    setmask(OVERLAY);
    SetGrey(color);
    win2stim(pos,es->pos,es->angle,pt);
    glPushMatrix();
    
    glTranslatef(es->pos[0],es->pos[1],0);
    glRotatef(es->angle,0,0,1);
    glBegin(GL_LINE_STRIP);
    pt[0] = -es->size[0]/2;
    myvx(pt);
    pt[0] = es->size[0]/2;
    myvx(pt);
    glEnd();
    glPopMatrix();
       glDrawBuffer(GL_BACK);
    return(pt[1]);
}



int RewardOn(int onoff){
    char buf[256];
    
    sprintf(buf,"%2s%c\n",valstrings[valstringindex[REWARD_SIZE]].code,onoff?'+':'-');
    SerialString(buf,0);
}



void TurnBackStim(Stimulus *st)
{
    if(st->type == STIM_RDS || st->type == STIM_RLS)
        st->right->baseseed = --(st->left->baseseed);
    if(st->next && st->next->type == STIM_RDS)
        st->next->right->baseseed = --(st->next->left->baseseed);
}



int ClearStimLine(int n){
    short *pl;
    
    pl = &expt.plot->linedata[n*4];
    *pl++ = 0;
    *pl++ = 0;
    *pl++ = 0;
    *pl++ = 0;
}



void StartRunning()
{
    int i;
    
    if(optionflag & DITHERMODE) //Ali && confirm_no("Are You Sure You want Dithering?",NULL))
        optionflag &= (~DITHERMODE);
    
    mode |= NEWPOS;
    CheckRect(TheStim);
    mode |= RUNNING;
    setmask(ALLPLANES);
    clear_display(1);
    mode |= CURSOR_IS_ON;
    
    i = mode;
    setstimuli(0);
    ResetCustomVals(i);
    
    
    //Ali SetExpPanel(&expt);
    psychclear(expt.plot,1);
    fprintf(stderr,"Initial Clear done\n");
    gettimeofday(&now, NULL);
    memcpy(&endtrialtime,&now,sizeof(struct timeval));
    memcpy(&goodfixtime,&now,sizeof(struct timeval));
    if(forcestart){
        if(replay_expt){
        }
        else{
            if(forcestart > 1)
                set_test_loop();
            StopGo(GO);
        }
    }
    glGetDoublev(GL_PROJECTION_MATRIX,pmatrix);
    glPushMatrix();
    binocrunning = 1;
    if(TheStim->left->baseseed == 0){
        NewSeed(TheStim);
    }
}

char DummySerial()
{
    gettimeofday(&now,NULL);
    switch(stimstate)
    {
        case POSTSTIMULUS:
        case POSTPOSTSTIMULUS:
        case INTRIAL:
        default:
            return(MYEOF);
        case WAIT_FOR_RESPONSE:
            if(timediff(&now,&timeb) > 1.0)
                return(WURTZ_OK);
    }
    return(MYEOF);
    gettimeofday(&timeb,NULL);
    
}




void one_event_loop()
{
	int i, j, mask,ctr,nstim,estim = 0;
	vcoord end[2],mpos[2];
	short rgbvec[3],*pl;
	/*Ali KeySym */ int sym;
	char c,s[256], *ser;
	float angle = 90;
	float sina,cosa,aval;
	float val;
//	Expstim *es,*esp;
	Locator *pos = &TheStim->pos;
	int statectr = 0,tc;
	static int testlaps = 0;
    
    tc = 0;
    while((c = ReadSerial(ttys[0])) != MYEOF){
        GotChar(c);
        if(tc++ > 2048){
            if((ser = CheckSerialInput(1000)) != NULL)
                fprintf(stderr,"Stuck in ReadSerial:%s\n",ser);
        }
    }
}

#pragma mark Event_Loop

int event_loop(float delay)
{
	int i, j, mask,ctr,nstim,estim = 0;
	vcoord end[2],mpos[2];
	short rgbvec[3],*pl;
	/* Ali KeySym */ int sym;
	char c,s[256], *ser;
	float angle = 90;
	float sina,cosa,aval;
	float val;
	Expstim *es,*esp;
	Locator *pos = &TheStim->pos;
	int statectr = 0,tc;
	static int testlaps = 0;
    struct timeval then;
    
    //Ali 20/6/2011
    //stimstate = PRESTIMULUS;
    
    tc = 0;
    gettimeofday(&then,NULL);
    val = timediff(&then,&nftime); //time since next frame exited
//    if (stimstate == POSTSTIMINTRIAL || stimstate == POSTPOSTSTIMULUS)
//        fprintf(stdout,"loop delay %.5f\n",val);
    while((c = ReadSerial(ttys[0])) != MYEOF){
        GotChar(c);
        if(tc++ > 2048){
            if((ser = CheckSerialInput(1000)) != NULL)
                fprintf(stderr,"Stuck in ReadSerial:%s\n",ser);
        }
    }
    gettimeofday(&now,NULL);
    val = timediff(&now,&then); //time since next frame exited
    if(!window_is_mapped) //first call
        mon.framerate = GetFrameRate();
    window_is_mapped = 1;
 //   ReadInputPipe();
    if(cleartime.tv_sec != 0){
        gettimeofday(&now,NULL);
        val = timediff(&now,&cleartime);
        if((val = timediff(&now,&cleartime)) > cleardelay){
            SerialSend(SEND_CLEAR);
            cleartime.tv_sec = 0;
        }
    }
    if(!(mode & RUNNING) && window_is_mapped)
    {
        if(statectr > 2) /* finally ready */
        {
			StartRunning();
        }
        else
        {
		    if(statectr == 0)
                expfront();
		    else if(statectr == 1 && !demomode)
                //Ali framefront();
                statectr++;
        }
    }
    if(mode & TEST_PENDING)
    {
        testmode = 4;
        if(testmode == 0){
            /*		      run_anticorrelated_test_loop();*/
            if(TheStim->type == STIM_RADIAL)
                run_radial_test_loop();
            else if(TheStim->type == STIM_RDS)
                run_anticorrelated_test_loop();
            else
                run_general_test_loop();
        }
        else if(testmode == 2)
            run_gabor_test_loop();
        else if(testmode == 1 || testmode == 3)
            run_rds_test_loop();
        else if (testmode == 4){
            for (i = 0; i < 20; i++){
                //                  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                paint_frame(WHOLESTIM,1);
                //                   paint_stimulus(TheStim);
                TheStim->pos.phase += TheStim->incr;
                //           calc_stimulus(TheStim);
                //glFinishRenderAPPLE();
                glSwapAPPLE();
                //Ali where the screen update goes
                //TESTRefresh();
            }
        }
        //                  run_general_test_loop();
        else if (testmode == 5){
            paint_stimulus(TheStim);
            TheStim->pos.phase += TheStim->incr;
            calc_stimulus(TheStim);
            glFinishRenderAPPLE();
        }

    }
    else
        next_frame(TheStim);
    if(command_pending && mode & RUNNING){
        ReadExptFile(NULL, 0, 0,0);
    }
    ctr++;
    gettimeofday(&nftime,NULL);
    return(stimstate);
}


void SetDelay(Locator *pos)
{
	int y = pos->xy[1]-winsiz[1],i;
    
	/* image big enough to need some delay when y is small */
	i = pos->imsize[0] * pos->imsize[1] * 0.008 - 180;
	/* image big enough to need some delay when y is small */
	if(y - pos->imsize[1]/2 < i)
		delay = rint((y/0.077) + pos->imsize[1] * 3.0);
	else
		delay = 0;
}

void CheckRect(Stimulus *st)
{
	float i,j,lx,ly,w,h,cosa,sina,dm,size[2];
	Locator *pos = &st->pos;
	int x,y,rx,ry,border = 2;
    
	x = pos->xy[0];
	y = pos->xy[1];
    
	sina = fabs(sin(pos->angle));
	cosa = fabs(cos(pos->angle));
	size[0] = pos->imsize[0] + deg2pix(fabs(st->dlength/2));
	size[1] = pos->imsize[1] + deg2pix(fabs(st->dwidth/2));
	w = (size[0] * cosa + size[1] * sina) + 2 * fabs((int)(st->disp)) + 2 * fabs((int)(st->vdisp));
	h = (size[1] * cosa + size[0] * sina) + 2 *fabs((int)(st->disp))  + 2 * fabs((int)(st->vdisp));
	if(st->type == STIM_CORRUG)
    {
	    dm = deg2pix(StimulusProperty(st,DEPTH_MOD));
	    w += (2 * fabsf(dm));
	    h += (2 * fabsf(dm));
    }
	if(st->type == STIM_RDS){
        w += (2* st->left->dotsiz[0]);
        h += (2* st->left->dotsiz[1]);
	}
	imrect.lx = x - (w+1)/2 - border;
	imrect.rx = imrect.lx + w+2 + 2*border;
	imrect.ly = y - (h+1)/2 - border;
	imrect.ry = imrect.ly + h+2 + 2*border;
	memcpy(&st->rect,&imrect,sizeof(Rectangle));
	if(st->next != NULL && st->next->type != STIM_NONE)
    {
	    pos = &st->next->pos;
        size[0] = pos->imsize[0] + deg2pix(fabs(st->next->dlength/2));
        size[1] = pos->imsize[1] + deg2pix(fabs(st->next->dwidth/2));
	    x = pos->xy[0];
	    y = pos->xy[1];
	    sina = fabs(sin(pos->angle));
	    cosa = fabs(cos(pos->angle));
	    w = (size[0] * cosa + size[1] * sina) + 2 * fabs((int)(st->disp)) + 2 * fabs((int)(st->vdisp));
	    h = (size[1] * cosa + size[0] * sina) + 2 * fabs((int)(st->disp))  + 2 * fabs((int)(st->vdisp));;
	    lx = x - (w+1)/2 -border;
	    rx = lx + w + 2 + 2*border;
	    ly = y - (h+1)/2 - border;
	    ry = ly + h+2 + 2*border;
	    if(lx < imrect.lx)
            imrect.lx = lx;
	    if(rx > imrect.rx)
            imrect.rx = rx;
	    if(ly < imrect.ly)
            imrect.ly = ly;	
	    if(ry > imrect.ry)
            imrect.ry = ry;
	    memcpy(&st->next->rect,&imrect,sizeof(Rectangle));
    }
}

int StartOverlay()
{
    if(!(mode & OVERLAY_ON))
    {
        mode |= OVERLAY_ON;
        glDrawBuffer(GL_FRONT_AND_BACK);
        setmask(OVERLAY);
    }
	return(0);
}

int EndOverlay()
{
    if(mode & OVERLAY_ON)
    {
        mode &= (~OVERLAY_ON);
        glDrawBuffer(GL_BACK);
        setmask(BOTHMODE);
    }
	return(0);
}


int ShowBox(Expstim *ps, float color)
{
    vcoord x[2],cp[2];
    int i,step;
    
    
    
    glPushMatrix();
    if(optionflags[MOVE_RF] && optionflag & SHOW_CONJUG_BIT && ps->type == RF_BOX){
        glTranslatef(ps->pos[0]+oldpos[0],ps->pos[1]+oldpos[1],0);
    }
    else{
        glTranslatef(ps->pos[0],ps->pos[1],0);
    }
    glRotatef(ps->angle,0,0,1);
    glLineWidth(expt.linw);
    SetGrey(color);
    /* first draw the stimulus region*/
    glBegin(GL_LINE_LOOP);
    x[0] = -ps->size[0]/2;
    x[1] = -ps->size[1]/2;
    myvx(x);
    x[0] += ps->size[0];
    myvx(x);
    x[1] += ps->size[1];
    myvx(x);
    x[0] = -ps->size[0]/2;
    myvx(x);
    x[1] = -ps->size[1]/2;
    myvx(x); 
    glEnd();
    if(ps->flag & PHASEMARKER_ON && !optionflags[STIMULUS_IN_OVERLAY])
    {
        glBegin(GL_LINE_STRIP);
        x[0] = -ps->size[0]/2;
        x[1] = ps->phasemark;
        myvx(x);
        x[0] = ps->size[0]/2;
        myvx(x);
        glEnd();
    }
   else if(ps->flag & CENTERMARK_ON) 
    {
        glBegin(GL_LINES);
        x[0] = -5;
        x[1] = 0;
        myvx(x);
        x[0] = 5;
        myvx(x);
        x[0] = 0;
        x[1] = 0;
        myvx(x);
        x[1] = -10;
        myvx(x);
        glEnd();
    }
    glPopMatrix();
	return(0);
}


void RotateStimulus(int x, int y)
{
	int ix,iy;
	float i,j,lx,ly,w,h,cosa,sina;
	Locator *pos = &stimptr->pos;
    
    
	mode |= NEED_REPAINT;
	h = y-winsiz[1] - pos->xy[1];
	w  = x-winsiz[0] - pos->xy[0];
	stimptr->pos.angle = atan2(h,w);
	CheckRect(stimptr);
}

void LocateStimulus(Stimulus *st, vcoord x, vcoord y)
{
	Locator *pos = &st->pos;
    
    
	pos->xy[1] = y-winsiz[1];
	pos->xy[0]  = x-winsiz[0];
    if(!(optionflag & BACKGROUND_FIXED_BIT))
	{
		if(st->next != NULL)
		{
			st->next->pos.xy[1] = y-winsiz[1];
			st->next->pos.xy[0] = x-winsiz[0];
		}
	}
	mode |= NEED_REPAINT;
	CheckRect(st);
}



void ShowPos(int x, int y)
{
    int wx = x - winsiz[0], wy = winsiz[1] - y;
    
    sprintf(mssg,"%.1f %.1f (%d,%d)",pix2deg(wx),pix2deg(wy),x,y);
    glstatusline(mssg,2);
}

void monocwipe()
{
}

void clear_display(int flag)
{
    
    if(!(mode & RUNNING))
    {
        return;
    }
    
    if(optionflag & DITHERMODE)
        glEnable(GL_DITHER);
    else
        glDisable(GL_DITHER);
    
    newtimeout = 1;
    glDrawBuffer(GL_FRONT_AND_BACK);
    clear_screen(TheStim, 1);
    if(stimstate == STIMSTOPPED && clearcolor != TheStim->background)
        search_background();
    if(stimstate == POSTTRIAL && monkeypress == WURTZ_OK_W)
        chessboard(128,128);
    if(flag)
    {
        setmask(OVERLAY);
        redraw_overlay(expt.plot);
    }
    setmask(BOTHMODE);
    if(expt.vals[FIXATION_OVERLAP] > 10)
        draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);

    glDrawBuffer(GL_BACK);
    glFlushRenderAPPLE();
    glFinishRenderAPPLE();
}

void redraw_overlay(struct plotdata  *plot)
{
    Expstim *es;
    short *pl;
    int i;
    
    if(!(mode & RUNNING))
        return;
    
    glLineWidth(expt.linw);
    setmask(OVERLAY);
    /*
     * when STIMULUS_IN_OVERLAY is set, the buffer does 
     */
    if(!optionflags[STIMULUS_IN_OVERLAY]){
        if(testflags[TEST_RC])
            glClearColor(clearcolor,0,clearcolor,clearcolor);
        else
            glClearColor(clearcolor,TheStim->gammaback,clearcolor,clearcolor);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    if(stimstate == STIMSTOPPED)
        ShowTime();


    if((pl = plot->linedata) != NULL)
        for(i = 0; i <= expt.nlines; i++,pl+=4)
            MyLine(pl[0],pl[1],pl[2],pl[3],LINES_COLOR);
    for(i = 0; i < rfctr; i++)
        ShowBox(&oldrfs[i],0.2);
    
    if(optionflags[SPLITSCREEN])
    {
        glPushMatrix();
        expt.rf->flag &= (~CENTERMARK_ON);
        setmask(ALLPLANES);
        ShowBox(expt.rf,0.2);
        glTranslatef(psychoff[0],0,0);
        ShowBox(expt.rf,0.2);
        setmask(OVERLAY);
        glTranslatef(-psychoff[0],psychoff[1],0);
        glPopMatrix();
    }
    else
        ShowBox(expt.rf,RF_COLOR);
    
    if(option2flag & PSYCHOPHYSICS_BIT && stimstate == STIMSTOPPED
       && option2flag & PERF_STRING)
        setmask(ALLPLANES);
        draw_conjpos(cmarker_size,PLOT_COLOR);
    
}

int step_stimulus()
{
    char s[BUFSIZ];
    int oldfreeze;
    
    stepframe++;
    oldfreeze = freezestimulus;
    freezestimulus = 0;
    increment_stimulus(expt.st, &expt.st->pos);
    freezestimulus = oldfreeze;
    clear_screen(expt.st,0);
    paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
    if(isadotstim(expt.st))
        sprintf(s,"%d se%d",stepframe,expt.st->left->seed);
    else
        sprintf(s,"%d",stepframe);
    glstatusline(s,0);
    change_frame();
    fprintf(stderr,"step %d\n",stepframe);
    return(stepframe);
}

void clear_overlay()
{
    if(!(mode & RUNNING))
        return;
    glDrawBuffer(GL_FRONT_AND_BACK);
    setmask(OVERLAY);
    glClearColor(clearcolor,TheStim->gammaback,clearcolor,clearcolor);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawBuffer(GL_BACK);
}

int ASetStimulus(float val, int code, int *event, Stimulus *st)
{
    SetStimulus(st, val, code, event);
}

int SetStimulus(Stimulus *st, float val, int code, int *event)
{
	Locator *pos = &st->pos;
	Stimulus *next;
	int jump,up =0, i;
	char buf[256];
	float fval = 0,pa,oria,tf,velocity;
	OneStim *psine;
	Substim *rds,*lrds;
	int oldflag = st->flag, ival = (int)val;
	float aval, bval, cval, dval, left, right, eccentricity, chord_scale;
	double cosa,sina,meandisp,dm,a,b,total;
	int oldoptionflag = optionflag,icode;
	static int laststimtype[2] = {STIM_NONE};
	static int setblank = 0, setuc = 0, setleft = 0, setright = 0;
    
	
	psine = (OneStim *)(st->left->ptr);
	rds = st->left;
	if(st->type != STIM_NONE && st->prev == NULL){
        laststimtype[0] = st->type;
        if(st->next){
            laststimtype[1] = st->next->type;
            if(laststimtype[1] == STIM_RDS)
                laststimtype[1] = st->next->type;
        }
	}
    
	if(val == INTERLEAVE_EXPT_UNCORR){
        st->flag |= UNCORRELATE;
        setuc = 1;
	}
	else if(setuc){
        st->flag &= (~UNCORRELATE);
        if(st->next)
            st->next->flag &= (~UNCORRELATE);
        setuc = 0;
	}
	else
        st->flag = st->flag; //just for debugging...
    
	if(val == INTERLEAVE_EXPT_LEFT){
        setleft = 1;
        newmonoc = LEFT_FIXATION_CHECK;
	}
	else if(val == INTERLEAVE_EXPT_RIGHT){
        setleft = 1;
        newmonoc = RIGHT_FIXATION_CHECK;
	}
	else if(newmonoc){
        optionflag &= (~(LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
        newmonoc = 0;
	}
    
	if(val == INTERLEAVE_EXPT_BLANK && !expt.st->preload){
        st->type = STIM_NONE;
        setblank = 1;
	}
	else if(st->type == STIM_NONE && setblank && st->prev == NULL){
        /*
         * if preloading image stimuli this doesn't work because
         * setting stimtype is only done at the preload stage
         */
        if(!expt.st->preload){
            st->type = laststimtype[0];
            if(st->next && laststimtype[1])
                st->next->type = laststimtype[1];
            setblank = 0;
        }
	}
    //Ali	if(event == NOEVENT)
    //        up = 0;
    //	else if(event == NULL || event->xbutton.type == ButtonRelease)
    //    {
    //		up = 1;
    //		if(option2flag & EXPT_INTERACTIVE && TheStim->mode & EXPTPENDING &&
    //		   code == expt.mode)
    //            expt.nextval = val;
    //        
    //    }
    if (event == 0)
        up = 1;
	if(st == NULL)
        st = TheStim;
    icode = valstringindex[code];
//    if setblank is set, val may be -1009, so don't set anything to this. Consider same for setuc
    if (setblank == 0){
	switch(code)
	{
        case BACKGROUND_MOVIE:  //need this in setstimulus for Expt sequence modes
        case BACKGROUND_IMAGE:
            SetExptProperty(&expt, expt.st, code,  val);
            break;
      case SEEDOFFSET:
            stimptr->seedoffset = val;
            expt.vals[code] = val;
            break;
        case DOTREPEAT:
            stimptr->dotrpt = val;
            if(stimptr->next != NULL)
                stimptr->next->dotrpt = val;
            break;
        case JNUMDOTS:
            if(stimptr->type == STIM_CYLINDER)
            {
                stimptr->left->ptr->fixdots = val;
                if(stimptr->flag & FIXED_DOTNUM)
                    init_cylinder(stimptr, stimptr->left);
            }
            break;
        case JLIFEFRAMES:
            stimptr->left->ptr->lifeframes = val;
            if(stimptr->type == STIM_CYLINDER && stimptr->flag & COUNTDOWN)
                init_cylinder(stimptr, stimptr->left);
            break;
        case FRAMEREPEAT:
            st->framerepeat = (int)(val);
            break;
        case STIM_POLARANGLE:
            if(st->type == STIM_RADIAL){
                st->left->wedge_angle = deg_rad(val);
                st->right->wedge_angle = deg_rad(val);
                st->left->calculated = st->right->calculated = 0;
            }
            else if(st->type == STIM_RDS){
                eccentricity = sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1]));
                pos->xy[0] = eccentricity * cos((M_PI / 180) * val);
                pos->xy[1] = eccentricity * sin((M_PI / 180) * val);
                pos->angle = (val * M_PI/180.0);
                /*
                 * Even if BACK_FIXED, need orientation of both to be the same, otherwise the
                 * hole is not calculated correctly
                 */
                if(rdspair(stimptr)){
                    stimptr->next->pos.angle = pos->angle;
                }
                CheckRect(stimptr);
            }
            else{
                eccentricity = sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1]));
                pos->xy[0] = eccentricity * cos((M_PI / 180) * val);
                pos->xy[1] = eccentricity * sin((M_PI / 180) * val);
            }
            break;
        case STIM_ECCENTRICITY:
            if(st->type == STIM_TWOBAR)
                eccentricity = sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1]) + sqr(pos->barsep/2));
            else
                eccentricity = sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1]));
            if(eccentricity > 0.00001)
            {
                pos->xy[0] = pos->xy[0] * ( deg2pix(val) / eccentricity );
                pos->xy[1] = pos->xy[1] * ( deg2pix(val) / eccentricity );
            }
            else
                pos->xy[0] = pos->xy[1] = eccentricity * 1.4142;
            if(st->type == STIM_TWOBAR && !(optionflags[FIXED_BARSEP]))
                pos->barsep = pos->barsep * ( deg2pix(val) / eccentricity );
            break;
        case TWOBAR_SEP: 
            pos->barsep = deg2pix(val);
            break;
        case TWOBAR_ANGSEP: /*now theta separation, change name later*/
            if(st->type == STIM_TWOBAR) {
                if(pos->barsep > NOTSET) {
                    eccentricity = sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1]) + sqr(pos->barsep/2));
                    pos->barsep = 2 * eccentricity * sin((M_PI / 180) * (val / 2));
                    chord_scale = eccentricity * cos((M_PI / 180) * (val / 2)) / sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1]));
                    pos->xy[0] = pos->xy[0] * chord_scale;
                    pos->xy[1] = pos->xy[1] * chord_scale;
                }
                else {
                    pos->barsep = 2 * sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1])) * tan((M_PI / 180) * (val / 2)); /*assumes pos->xy is correct for the val read in from the file (so stored barsep and pos->xy give a sensible eccentricity)*/
                }
            }
            else
                pos->barsep = deg2pix(val);
            break;
        case BACKSTIM_TYPE:
            st = TheStim->next;
        case STIMULUS_TYPE_CODE:
            if(st->next && rdspair(st) && ival == STIM_RLS)
                StimulusType(st->next,ival);
            if(st->next && rlspair(st) && ival == STIM_RDS)
                StimulusType(st->next,ival);
            StimulusType(st,ival);
            if(ival == STIM_SUBGRATING1)
            {
                SetStimulus(st, expt.stimvals[SF], SF,NULL);
                SetStimulus(st, expt.stimvals[TF], TF,NULL);
                SetStimulus(st, expt.stimvals[SD_X], SD_X,NULL);
            }
            else if(ival == STIM_SUBGRATING2)
            {
                SetStimulus(st, expt.stimvals[SF2], SF,NULL);
                SetStimulus(st, expt.stimvals[TF2], TF,NULL);
                SetStimulus(st, expt.stimvals[SD_X], SD_X,NULL);
            }
            else if(ival == STIM_GRATING2)
            {
                SetStimulus(st, expt.stimvals[SF], SF,NULL);
                SetStimulus(st, expt.stimvals[TF], TF,NULL);
                SetStimulus(st, expt.stimvals[SF2], SF2,NULL);
                SetStimulus(st, expt.stimvals[TF2], TF2,NULL);
                SetStimulus(st, expt.stimvals[SD_X], SD_X,NULL);
            }
            else if(ival == STIM_GRATING)
            {
                optimize_stimulus(st);
            }
            if(code == BACKSTIM_TYPE && up)
                SerialSend(BACKSTIM_TYPE);
            break;
        case GAMMAVAL_CODE:
            gammaval = val;
            /*
             * When replaying, turn off my gamma correction so that save images are correct for luminance.
             */
            if(testflags[PLAYING_EXPT])
                gammaval = 1.0;
            setgamma(gammaval);
            st->gammaback = dogamma(st->background);
            clearcolor = st->gammaback;
            clear_display(1);
            break;
        case JDEATH:
            if(st->type == STIM_CYLINDER)
                st->left->ptr->deathchance = val;
            break;
        case JVELOCITY:
            /*  Cylinder velocity is in degrees of rotation
             per second, converted to radians per frame*/
            if(st->type == STIM_CYLINDER)
                st->left->ptr->velocity = (val/(mon.framerate*180/M_PI));
            if(st->type == STIM_RDS || st->type == STIM_RLS){
                tf = val * st->pos.sf;
                st->incr = (val *M_PI *2)/(mon.framerate);
                st->posinc = deg2pix(val)/mon.framerate;
                if(rdspair(st))
                    st->next->posinc = st->posinc;
                //	    SetStimulus(st,tf,TF,event);
            }
            if(st->type == STIM_BAR){
                st->posinc = deg2pix(val)/mon.framerate;
            }
            if(st->type == STIM_GRATING){
                tf = pos->sf * val;
                SetStimulus(st, tf, TF, event);
            }
            break;
        case JF_INTENSITY:
            if(st->type == STIM_CYLINDER)
                st->left->ptr->front_intensity = val;
            break;
        case JB_INTENSITY:
            if(st->type == STIM_CYLINDER)
                st->left->ptr->back_intensity = val;
            break;
        case GOODTRIALS:
            goodtrials = (int)val;
            break;
        case BADTRIALS:
            totaltrials = (int)val + goodtrials;
            break;
        case RF_WIDTH:
            expt.rf->size[1] = deg2pix(val);
            break;
        case RF_HEIGHT:
            expt.rf->size[0] = deg2pix(val);
            break;
        case RF_X:
            expt.rf->pos[0] = deg2pix(val);
            break;
        case RF_Y:
            expt.rf->pos[1] = deg2pix(val);
            break;
        case RF_ANGLE:
            expt.rf->angle = val;
            break;
        case SWAPINTERVAL:
            if(val < 1)
                TheStim->swapinterval = 1;
            else
            {
                if(TheStim->swapinterval > 1)
                    fval = TheStim->incr/TheStim->swapinterval;
                else
                    fval = TheStim->incr;
                TheStim->swapinterval = (int)val;
                TheStim->incr = fval * val;
            }
            /*
             * current version does not regulate this, so force it to be 1 ant
             * all times
             */
            TheStim->swapinterval = 1;
            break;
        case ASPECT_RATIO:
            if(val <= 0.0)
                break;
            total = pos->imsize[1] + pos->imsize[0];
            pos->imsize[1] = total/(1+val);
            pos->imsize[0] =  val * total/(1+val);
            pos->radius[0] = pos->imsize[0]/2;
            pos->radius[1] = pos->imsize[1]/2;
            st->length = pix2deg(pos->imsize[0]);
            break;
        case STIM_SIZE:
            if(val <= 0.0)
                break;
            st->length = val;
            fval = deg2pix(val);
            pos->imsize[1] =  rint(fval);
            pos->ss[1] = (int)ceilf(fval/pos->size[1]);
            if(pos->ss[1] <= 0)
                pos->ss[1] = 1.0;
            if(st->type != STIM_BAR && st->type != STIM_TWOBAR)
                pos->radius[1] = pos->imsize[1]/2;
            pos->imsize[0] =  rint(fval);
            pos->radius[0] = pos->imsize[0]/2;
            pos->ss[0] = (int)ceilf(fval/pos->size[0]);
            if(pos->ss[0] <= 0)
                pos->ss[0] = 1;
            init_stimulus(st);
            if(up && st->type  == STIM_GRATING){
                lastsz = val;
                optimize_stimulus(st);
            }
            else if(st->type == STIM_GRATINGN){
                pos->ss[1] = 1;
                pos->size[1] = (int)((fval/pos->ss[1])+0.5);
                init_stimulus(st);
            }
            if(st->type == STIM_IMAGE){
                pos->imsize[0] = pos->imsize[1] = deg2pix(val);
            }
            if(st->type == STIM_RADIAL){
                SetStimulus(st,val,STIM_HEIGHT,event);
            }
            CheckRect(TheStim);
            mode |= NEWDRAG;
            st->left->size = st->right->size = val;
            if(st->prev == NULL && st->next != NULL && expt.vals[BACK_ADD_SIZE] > 0)
                SetStimulus(st->next,val+expt.vals[BACK_ADD_SIZE],code,event);
            break;
        case STIM_WIDTH:
            if((fval = deg2pix(val)) < 0)
                break;
            pos->imsize[1] =  rint(fval);
            /* 
             * Aug 95, don't add bar width to radius, this prevents using a
             * stationary bar
             */
            if(st->type == STIM_BAR || st->type == STIM_TWOBAR)
            {
                pos->imsize[1] = rint(fval + deg2pix(1/(2*st->f)));
                pos->radius[1] = rint(fval/2);
            }
            /*
             * to reset the stimulus size by changing the size of the
             *  quadrilateral strip, leaving the actual calculated vertices
             *  map size unchanged use the following:
             else if(st->type != STIM_RDS)
             pos->ss[1] = (int)ceilf(fval/pos->size[1]);
             if(pos->ss[1] <= 0)
             pos->ss[1] = 1.0;
             
             but AUg 95 changed so that xsampling remains the same;
             */
            else if(st->type == STIM_GRATINGN){
                pos->ss[1] = 1;
                pos->size[1] = (int)((fval/pos->ss[1])+0.5);
                init_stimulus(st);
            }
            else if(st->type == STIM_GRATING || st->type == STIM_GRATING2)
            {
                pos->size[1] = (int)((fval/pos->ss[1])+0.5);
                init_stimulus(st);
            }
            else if(st->type != STIM_RDS)
            {
                pos->size[1] = (int)(fval+0.5);
                init_stimulus(st);
            }
            if(st->type != STIM_BAR && st->type != STIM_TWOBAR)
                pos->radius[1] = pos->imsize[1]/2;
            mode |= NEWDRAG;
            if(up && st->type  == STIM_GRATING)
                optimize_stimulus(st);
            if(st->type == STIM_IMAGE){
                pos->imsize[1] = fval;
                pos->size[1] = rint(fval);
            }
            if(st->type == STIM_RADIAL){
                st->left->calculated = st->right->calculated = 0;
                pos->size[1] = fval;
                pos->imsize[1] = fval +  st->left->ptr->rsigma*3;
                pos->radius[1] = pos->imsize[1]/2;
            }
            break;
        case STIM_HEIGHT:
            if((fval = deg2pix(val)) < 0)
                break;
            st->length = val;
            fval = deg2pix(val);
            pos->imsize[0] =  rint(fval);
            pos->radius[0] = pos->imsize[0]/2;
            pos->ss[0] = (int)ceilf(fval/pos->size[0]);
            if(pos->ss[0] <= 0)
                pos->ss[0] = 1;
            if(rlspair(st)){
                SetStimulus(st->next, val, code, event);
            }
            if(st->type == STIM_IMAGE){
                pos->size[0] = pos->imsize[0] = fval;
            }
            if(st->type == STIM_RADIAL){
                /*
                 * for the RADIAL stimulus, radius records the radius of the ring centre,
                 * while imsize is made large enough to accommodate the full stimulus width,
                 * which is wider by 3 * rsigma
                 */
                if(st->left->ptr->rsigma < 0.001)
                    st->left->ptr->rsigma = pos->imsize[0]/10;
                st->left->ptr->radius = fval/2;
                pos->size[0] = pos->size[1] = fval;
                pos->imsize[0] = fval +  st->left->ptr->rsigma*3.2;
                pos->imsize[1] = fval +  st->left->ptr->rsigma*3.2;
                pos->radius[1] = pos->imsize[1]/2;
                pos->radius[0] = pos->imsize[0]/2;
                if(st->right->ptr->rsigma < 0.001)
                    st->right->ptr->rsigma = pos->imsize[0]/10;
                st->right->ptr->radius = fval/2;
                st->left->calculated = st->right->calculated = 0;
                init_stimulus(st);
            }
            st->left->size = StimulusProperty(st,STIM_SIZE);
            st->right->size = st->left->size;
            mode |= NEWDRAG;
            break;
        case ORI_BANDWIDTH:
            st->left->orbw = val;
            st->right->orbw = val;
            break;
        case ORI_LEFT:
            left = val * M_PI/180.0;
            right = pos->angle - st->ori_disp;
            pos->angle = (left + right)/2;
            st->ori_disp = (left - right)/2;
            break;
        case SF_LEFT:
            left = val;
            right = st->f - st->sf_disp;
            st->pos.sf = st->f = (left + right)/2;
            st->sf_disp = (left - right)/2;
            break;
        case SF_RIGHT:
            right = val;
            left = st->f + st->sf_disp;
            st->pos.sf = st->f = (left + right)/2;
            st->sf_disp = (left - right)/2;
            break;
        case LRBINTERLEAVE:
        case MONOCULARITY_EXPT:
            /*
             * this is normally set via ExptProperty, but is called here
             * for RC sequences, so does not call setoption, SerialSend, etc
             */
            
            /* 0 = Binoc, -1 = LEFT, 1 = RIGHT */
            optionflag &= (~MONOCULAR_MODE);
            // if setblank, make mode binocular, so that both eyes are wiped
            newmonoc = 0;
            if(setblank){
                ;
            }
		    
            else if(val < -0.4){
                optionflag |= LEFT_FIXATION_CHECK;
                newmonoc = LEFT_FIXATION_CHECK;
            }
            else if (val > 0.4){
                optionflag |= RIGHT_FIXATION_CHECK;
                newmonoc = RIGHT_FIXATION_CHECK;
            }
            mode |= NEED_REPAINT;
            break;
        case ORI_RIGHT:
            right = val * M_PI/180.0;
            left = pos->angle + st->ori_disp;
            pos->angle = (left + right)/2;
            st->ori_disp = (left - right)/2;
            break;
        case ORIENTATION:
            if(val > INTERLEAVE_EXPT){
                pos->angle = val * M_PI/180.0;
                mode |= NEWDRAG;
                CheckRect(stimptr);
            }
            else
                pos->angle = val;
            if(st->type == STIM_IMAGE && st->left->orbw < 0)
                pos->angle -= M_PI/2;
            st->left->pos.angle = st->pos.angle + st->ori_disp;
            st->right->pos.angle = st->pos.angle - st->ori_disp;
            if(st->type == STIM_CYLINDER && event == NULL && covaryprop != ORIENTATION)
                SetTargets();
            break;
            if((rdspair(st) || rlspair(st)) && !(optionflag & BACKGROUND_FIXED_BIT))
                st->next->pos.angle = pos->angle;
        case SPINRATE:
            st->angleinc = val;
            break;
        case PLAID_ANGLE:
            if(st->type == STIM_GRATING2 || st->type == STIM_GRATING || st->type == STIM_SQUARE || st->type == STIM_GABOR || st->type == STIM_RLS)
            {
                st->left->ptr->plaid_angle =  deg_rad(val);
                st->right->ptr->plaid_angle =  deg_rad(val);
            }
            break;
        case ORI2:
            if(st->type == STIM_GRATING2 || st->type == STIM_GRATING)
            {
                psine = (OneStim *)(st->left->ptr);
                oria = pos->angle - psine->plaid_angle/2;
                pa = (val * M_PI/180.0) - oria;
                pos->angle = oria + pa/2;
                psine->plaid_angle =  pa;
                psine = (OneStim *)(st->right->ptr);
                psine->plaid_angle = pa;
            }
            if(st->type == STIM_RADIAL){
                st->left->ptr->ori2 = deg_rad(val);
                st->right->ptr->ori2 = deg_rad(val);
                st->left->calculated = st->right->calculated = 0;
            }
            break;
        case PHASE2:
            if(st->type == STIM_GRATING2 || st->type == STIM_GABOR || st->type == STIM_RLS)
            {
                pos->phase2 = val;
            }
            break;
        case TF2:
            if(st->type == STIM_GRATING2 || st->type == STIM_RADIAL || st->type == STIM_GRATING)
            {
                psine = (OneStim *)(st->left->ptr);
                psine->incr = (val *M_PI *2)/(mon.framerate);
                if(TheStim->swapinterval > 1)
                    psine->incr *= TheStim->swapinterval;
                fval = psine->incr;
                psine = (OneStim *)(st->right->ptr);
                psine->incr = fval;
            }
            break;
        case SF2:
            if(st->type == STIM_GRATING2)
            {
                psine = (OneStim *)(st->left->ptr);
                psine->sf2 = val;
                psine->incr = st->incr * psine->sf2/st->f;
                psine = (OneStim *)(st->right->ptr);
                psine->sf2 = val;
                psine->incr = st->incr * psine->sf2/st->f;
            }
            if(st->type == STIM_GRATINGN){
                st->left->ptr->sf2 = val;
                st->right->ptr->sf2 = val;
                SetGratingFrequencies(st);
            }
            if(st->type == STIM_GRATING){
                st->left->ptr->sf2 = val;
                st->right->ptr->sf2 = val;
            }
            break;
        case SF:
            st->f = pos->sf = val;
            if((st->type == STIM_GRATING || st->type == STIM_GRATING2) && val > 0){
                lastsf = val;
            }
            if(up){
                optimize_stimulus(st);
                /*  
                 * for the "radial" stim type, SF must allow integral number of cycles per lap
                 * also need to re-calculate the structure image. 
                 */
                if(st->type == STIM_RADIAL){
                    if(val > 0){
                        a = rint(2 * M_PI * st->left->ptr->radius * pix2deg(val));
                        st->f = pos->sf = deg2pix(a/(2 * M_PI * st->left->ptr->radius));
                    }
                    init_stimulus(st);
                }
                if(st->type == STIM_GRATINGN)
                    SetGratingFrequencies(st);
            }
            break;
        case SET_COUNTERPHASE:
            if(val > 0.5)
                optionflag |= CONTRAST_REVERSE_BIT;
            else
                optionflag &= (~CONTRAST_REVERSE_BIT);
            break;
        case BACK_TF:
            if(st->next != NULL)
                st->next->incr = (val *M_PI *2)/(mon.framerate);
            break;
        case TF:	
            if(val == 0)
                SetStimulus(TheStim,TheStim->vals[START_PHASE],SETPHASE,NOEVENT);
            st->incr = (val *M_PI *2)/(mon.framerate);
            if(TheStim->swapinterval > 1)
                st->incr *= TheStim->swapinterval;
            if(st->type == STIM_GRATING){
                if(val < 0){
                    val = 0;
                    st->incr = 0;
                }
                if(val > 0)
                    lasttf = val;
            }
            break;
        case DISP_A:
            meandisp = StimulusProperty(st,DISP_X);
            dm = StimulusProperty(st,DEPTH_MOD);
            a = StimulusProperty(st,DISP_A);
            b = StimulusProperty(st,DISP_B);
            SetStimulus(st, (val-b)/2, DEPTH_MOD, event);
            SetStimulus(st, (val+b)/2, DISP_X, event);
            break;
        case DISP_B:
            meandisp = StimulusProperty(st,DISP_X);
            dm = StimulusProperty(st,DEPTH_MOD);
            a = StimulusProperty(st,DISP_A);
            b = StimulusProperty(st,DISP_B);
            SetStimulus(st, (a-val)/2, DEPTH_MOD, event);
            SetStimulus(st, (a+val)/2, DISP_X, event);
            break;
        case DEPTH_MOD:
            /*
             * NB confusing. st->depth_mod is in degrees, and is the true depth mod.
             * st->left->depth_mod is in pixels, and is half the depth mod, because
             * the pixel value is added to one side, subtracted from the other.
             */
            switch(st->type)
	    {
            case STIM_CORRUG:
                st->left->ptr->depth_mod = deg2pix(val)/2;
                st->right->ptr->depth_mod = deg2pix(val)/2;
                st->depth_mod = val;
                break;
            case STIM_GRATING:
            case STIM_RDS:
            case STIM_IMAGE:
                st->depth_mod = val;
                break;
            case STIM_SQCORRUG:
                st->left->ptr->depth_mod = deg2pix(val)/2;
                st->right->ptr->depth_mod = deg2pix(val)/2; 
                st->depth_mod = val;
                break;
	    }
            break;
        case VDISP_MOD:
            st->vdisp_mod = val;
            break;
            break;
        case DISP_GRAD:
            if(st->type == STIM_CORRUG)
            {
                st->left->ptr->disp_grad = deg2pix(val)/2;
                st->right->ptr->disp_grad = deg2pix(val)/2;
                st->left->ptr->depth_mod = deg2pix(val)/2;
                st->right->ptr->depth_mod = deg2pix(val)/2;
            }
            break;
        case SD_X:
            st->left->ptr->sy = val;
            st->right->ptr->sy = val;
            st->left->ptr->pixelsy = deg2pix(val);
            st->right->ptr->pixelsy = deg2pix(val);
            st->right->calculated = 0;
            st->left->calculated = 0;
            break;
        case SD_BOTH:/*j*/
            SetStimulus(st,val,SD_X,event);
            SetStimulus(st,val,SD_Y,event);
            break;
        case SD_Y:/*j*/
            st->left->ptr->sx = val;
            st->right->ptr->sx = val;
            if(st->type == STIM_RADIAL){
                st->left->ptr->rsigma = deg2pix(val);
                st->right->ptr->rsigma = deg2pix(val);
                init_stimulus(st);
            }
            break;
        case SET_SEEDLOOP:
            st->left->seedloop = (int)val;
            st->right->seedloop = (int)val;
            break;
        case SET_SEED:
            st->left->seed = st->left->baseseed = (int)val;
            st->right->seed = st->right->baseseed = (int)val;
            break;
        case SEED_SHIFT:
            if(st->type == STIM_RDS)
            {
                st->left->xshift = deg2pix(val);
                st->right->xshift = deg2pix(val);
            }
            else if(st->type == STIM_SQCORRUG)
            {
                st->ucgap = deg2pix(val);
            }
            else if(st->type == STIM_IMAGE){
                st->left->xshift = val;
                st->right->xshift = val;
            }
            break;
        case PHASE_AS_DISP:
            /*
             * phase is added half to each eye. 2PI/2 = PI.
             */
            if(st->type == STIM_GRATING2 || st->type == STIM_GRATING){
                expt.vals[DISP_P] = st->phasedisp[0] = val * M_PI * st->f;
                if(st->type == STIM_GRATING2)
                    st->phasedisp[1] = val * M_PI * st->left->ptr->sf2;
            }
            else if(st->type == STIM_GRATINGN){
                st->rolldisp = val;
            }
            /*
             * This not active yet, but will make all Pd horizontal for RDS
             */	  else if(st->type == STIM_RDS && 0){
                 expt.vals[DISP_P] = st->phasedisp[0] = val * M_PI * st->f;
                 st->phaseangle =  pos->angle - M_PI_2;
             }
             else{
                 if(fabs(sin(pos->angle))  > 0.01)
                     expt.vals[DISP_P] = st->phasedisp[0] = val * M_PI * st->f/sin(pos->angle);
                 else
                     expt.vals[DISP_P] = st->phasedisp[0] = 0;
             }
            break;
        case PHASE_AS_RELDISP:
            /*
             * phase is added half to each eyey. 2PI/2 = PI.
             */
            expt.vals[DISP_P] = st->phasedisp[0] = -val * M_PI * st->f;
            if(st->next)
                st->next->phasedisp[0] = -val * M_PI * st->f;
            break;
        case DISP_P:
            expt.vals[DISP_P] = st->phasedisp[0] = deg_rad(val)/2;
            if(st->type == STIM_RDS)
                st->phaseangle = 0;
            else if(st->type == STIM_RLS){
                st->phasedisp[0] = deg2pix(val/2);
                st->phaseangle = 0;
            }
            break;
            
            /*
             * For GRATINGN stimulus types, disp phase 2 is what is added to the middle frequency component. 
             */
        case DISP_P2:
            if(val > INTERLEAVE_EXPT && (st->type == STIM_GRATING2 || st->type == STIM_GRATINGN || st->type == STIM_GABOR || st->type == STIM_GRATING))
                st->phasedisp[1] = deg_rad(val)/2;
            else if (val > INTERLEAVE_EXPT && st->type == STIM_RLS)
                   st->phasedisp[1] = deg2pix(val/2);
                   
            break;
        case DISP_BACK:
            if(st->next == NULL && st->prev == NULL)
                break;
	        else if(val == INTERLEAVE_EXPT_UNCORR){
                st->next->flag &= (~ANTICORRELATE);
                st->next->flag |= UNCORRELATE;
            }
            else if(st->next == NULL && st->prev != NULL){
                expt.vals[DISP_BACK] = val;
                st->disp = deg2pix(val)/2;
                CheckRect(stimptr);
            }
	        else
            {
                expt.vals[DISP_BACK] = val;
                st->next->disp = deg2pix(val)/2;
                CheckRect(stimptr);
                if(event != NOEVENT && mode & SERIAL_OK)
                {
                    sprintf(buf,"%s=%.2f\n",valstrings[icode].code,val);
                    SerialString(buf,0);
                    st = st->next;
                }
            }
            if(up)
                SerialSend(BACK_CORRELATION);
            break;
        case BACK_VDISP:
            if(st->next == NULL && st->prev == NULL)
                break;
	        else if(val == INTERLEAVE_EXPT_UNCORR){
                st->next->flag &= (~ANTICORRELATE);
                st->next->flag |= UNCORRELATE;
            }
            else if(st->next == NULL && st->prev != NULL){
                expt.vals[BACK_VDISP] = val;
                st->vdisp = deg2pix(val)/2;
                CheckRect(stimptr);
            }
	        else
            {
                expt.vals[BACK_VDISP] = val;
                st->next->vdisp = deg2pix(val)/2;
                CheckRect(stimptr);
                if(event != NOEVENT && mode & SERIAL_OK)
                {
                    sprintf(buf,"%s=%.2f\n",valstrings[icode].code,val);
                    SerialString(buf,0);
                    st = st->next;
                }
            }
            if(up)
                SerialSend(BACK_CORRELATION);
            break;
        case BACK_SIZE:
            if(st->next == NULL)
                break;
            else
            {
                expt.vals[BACK_SIZE] = val;
                SetStimulus(st->next, val, STIM_WIDTH, NULL);
                SetStimulus(st->next, val, STIM_HEIGHT, NULL);
                CheckRect(stimptr);
            }
            break;
        case BACK_HEIGHT:
            if(st->next == NULL)
                break;
            else
            {
                expt.vals[code] = val;
                SetStimulus(st->next, val, STIM_HEIGHT, NULL);
                CheckRect(stimptr);
            }
            break;
        case BACK_WIDTH:
            if(st->next == NULL)
                break;
            else
            {
                expt.vals[code] = val;
                SetStimulus(st->next, val, STIM_WIDTH, NULL);
                CheckRect(stimptr);
            }
            break;
        case BACK_CONTRAST:
            if(st->next == NULL)
                break;
            else
            {
                expt.vals[BACK_CONTRAST] = val;
                SetStimulus(st->next, val, SETCONTRAST, NULL);
            }
            break;
        case DISP_RAMP:
            if (isharris(altstimmode)){
                st->dispincr = deg2pix(val)/mon.framerate;
                st->disp = st->meandisp;
            }
            else{
                st->dispincr = deg2pix(val)/TheStim->nframes;
                st->disp = st->meandisp - st->dispincr * TheStim->nframes/2;
            }
            break;
        case CORRELATION:
            if(st->type == STIM_RDS || st->type == STIM_RLS){
                st->correlation = val;
                rds = st->right;
                lrds = st->left;
                /*
                 * When setting corr to zero, make back corr zero too.
                 * Disable this behavior by setting BACKGROUND_FIXED_BIT
                 */
                
                if(!(optionflag & BACKGROUND_FIXED_BIT)){
                    if(st->next != NULL && expt.stimvals[BACK_CORRELATION] > 0)
                        SetStimulus(st->next, val, CORRELATION, NULL);
                }
            }
            else
                st->correlation = 1;
            expt.vals[CORRELATION] = val;
            if(val < -0.001)
            {
                st->flag &= (~UNCORRELATE);
                
                st->flag |= ANTICORRELATE;
                if(st->type == STIM_RDS || st->type == STIM_RLS){
                    rds->corrdots = rint(rds->ndots * -val);
                    lrds->corrdots = rint(rds->ndots * -val);
                }
            }
            else if(val > 0.001)
            {
                st->flag &= (~ANTICORRELATE);
                st->flag &= (~UNCORRELATE);
                if(st->type == STIM_RDS || st->type == STIM_RLS){
                    rds->corrdots = rint(rds->ndots * val);
                    lrds->corrdots = rint(lrds->ndots * val);
                }
            }
            else
            {
                st->flag |= UNCORRELATE;
                st->flag &= (~ANTICORRELATE);
                if(st->type == STIM_RDS || st->type == STIM_RLS){
                    rds->corrdots = rint(rds->ndots * val);
                    lrds->corrdots = rint(lrds->ndots * val);
                }
            }
            break;
        case BACK_CORRELATION:
            if(st->next){
                if(val < -0.01)
                {
                    st->next->flag &= (~UNCORRELATE);
                    st->next->flag |= ANTICORRELATE;
                }
                else if(val > 0.01)
                {
                    st->next->flag &= (~ANTICORRELATE);
                    st->next->flag &= (~UNCORRELATE);
                }
                else
                {
                    st->next->flag |= UNCORRELATE;
                    st->next->flag &= (~ANTICORRELATE);
                }
            }
            break;
        case STANDING_DISP:
            if(st->type == STIM_CYLINDER)
                st->left->ptr->standing_disp = deg2pix(val)/2;
            CheckRect(stimptr);
            break;
        case ANTICORRELATED_DISPARITY:
            st->flag |= ANTICORRELATE;
            if(st->next != NULL)
                st->next->flag |= ANTICORRELATE;
        case CORRELATED_DISPARITY:
            if(code != ANTICORRELATED_DISPARITY)
            {
                if(st->next != NULL)
                    st->next->flag &= (~ANTICORRELATE);
                st->flag &= (~ANTICORRELATE);
            }
        case ORTHOG_DISP:
            // ? use stim ori or RF ori?
            if(val > INTERLEAVE_EXPT){
                fval = GetProperty(&expt, st, PARALELL_DISP);
                cosa = cos(expt.rf->angle * M_PI/180.0);
                sina = sin(expt.rf->angle * M_PI/180.0);
                bval = fval * sina - val * cosa;
                cval = val * sina + fval * cosa;
                SetStimulus(st,bval,DISP_Y,event);
                SetStimulus(st,cval,DISP_X,event);
            }
            break;
        case OPPOSITE_DELAY:
            SetStimulus(st,val,DISP_X,event);
            SetProperty(&expt, expt.st, SEED_DELAY, expt.stimvals[SEED_DELAY] * -1);
            break;
        case STIMORTHOG_POS:
            fval = 0; // for now, force other direction to be centered
            cval = StimulusProperty(st,ASPECT_RATIO);
            if(cval > 1){
                cosa = cos(expt.st->pos.angle);
                sina = sin(expt.st->pos.angle);
            }
            else{
                cosa = cos(expt.st->pos.angle+M_PI_2);
                sina = sin(expt.st->pos.angle+M_PI_2);
            }
            bval = (val * sina - fval * cosa);
            cval = (fval * sina + val * cosa);
            bval =  pix2deg(expt.rf->pos[0]) - bval;
            cval +=  pix2deg(expt.rf->pos[1]);
            SetStimulus(st,bval,XPOS,event);
            SetStimulus(st,cval,YPOS,event);
            break;
        case ABS_ORTHOG_POS:
            fval = GetProperty(&expt, st, PARA_POS);
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            bval = -(val * sina - fval * cosa);
            cval = (fval * sina + val * cosa);
            SetStimulus(st,bval,RF_X,event);
            SetStimulus(st,cval,RF_Y,event);
            break;
        case ORTHOG_POS:
            fval = GetProperty(&expt, st, PARA_POS);
        case RF_ORTHO:
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            bval = (val * sina - fval * cosa);
            cval = (fval * sina + val * cosa);
            bval =  pix2deg(expt.rf->pos[0]) - bval;
            cval +=  pix2deg(expt.rf->pos[1]);
            if(code == RF_ORTHO){
                SetStimulus(st,bval,RF_X,event);
                SetStimulus(st,cval,RF_Y,event);
                SerialSend(RF_SET);
            }
            //Set Stimulus to RF centre too.
            SetStimulus(st,bval,XPOS,event);
            SetStimulus(st,cval,YPOS,event);
            break;
        case PARA_POS:
            fval = GetProperty(&expt, st, ORTHOG_POS);
        case RF_PARA:
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            bval = (fval * sina - val * cosa);
            cval = (val * sina + fval * cosa);
            bval =  pix2deg(expt.rf->pos[0]) - bval;
            cval +=  pix2deg(expt.rf->pos[1]);
            if(code == RF_PARA){
                SetStimulus(st,bval,RF_X,event);
                SetStimulus(st,cval,RF_Y,event);
                SerialSend(RF_SET);
            }
            else{
                SetStimulus(st,bval,XPOS,event);
                SetStimulus(st,cval,YPOS,event);
            }
            break;
        case PARALELL_DISP:
            // ? use stim ori or RF ori?
            fval = GetProperty(&expt, st, ORTHOG_DISP);
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            bval = val * sina - fval * cosa;
            cval = fval * sina + val * cosa;
            SetStimulus(st,bval,DISP_Y,event);
            SetStimulus(st,cval,DISP_X,event);
            break;
        case ABS_PARA_POS:
            fval = GetProperty(&expt, st, ORTHOG_POS);
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            bval = -(val * sina - fval * cosa);
            cval = (fval * sina + val * cosa);
            SetStimulus(st,bval,RF_X,event);
            SetStimulus(st,cval,RF_Y,event);
            break;
        case DISP_X:
            if(val > INTERLEAVE_EXPT){
                st->disp = deg2pix(val)/2;
                CheckRect(stimptr);
                if(st->prev != NULL)
                    expt.vals[DISP_BACK] = val;
                code = DISP_X;
                st->meandisp = st->disp;
            }
            else
                st->disp = 0;
            break;
        case RELDISP:
            if(st->next != NULL){
                meandisp = (st->disp + st->next->disp)/2;
                st->meandisp = st->disp = meandisp + deg2pix(val)/4;
                st->next->disp = meandisp - deg2pix(val)/4;
            }
            else
                st->disp = deg2pix(val)/2;
            CheckRect(stimptr);
            break;
        case RELVDISP:
            if(st->next != NULL){
                meandisp = (st->vdisp + st->next->vdisp)/2;
                st->vdisp = meandisp + deg2pix(val)/4;
                st->next->vdisp = meandisp - deg2pix(val)/4;
            }
            else
                st->disp = deg2pix(val)/2;
            CheckRect(stimptr);
            break;
        case DISP_Y:
            st->vdisp = deg2pix(val)/2;
            CheckRect(stimptr);
            break;
        case XMAXSAMPLE_RATIO:
            st->sample_ratio[1] = val;
            fval = st->sample_ratio[0];
            SetStimulus(st, fval, XMINSAMPLE_RATIO, NULL);
            break;
        case XMINSAMPLE_RATIO:
            st->sample_ratio[0] = val;
            if(st->type != STIM_RDS)
            {
                fval = deg2pix(1/st->f)/val;
                if(fval < st->sample_ratio[1])
                    SetStimulus(st, fval, XSAMPLES, NULL);
            }
            break;
        case DOT_DENSITY:
            if(st->type == STIM_RDS || st->type == STIM_RDSSINE)
            {
                if(val > 0){
                    pos->density = val;
                    init_rds(st,st->left,val);
                    init_rds(st,st->right,val);
                }
				
            }
            else if(st->type == STIM_CORRUG)
            {
                if(val > 0){
                    pos->density = val;
                    init_corrug(st,st->left,val);
                    init_corrug(st,st->right,val);
                }
				
            }
            else if(st->type == STIM_SQCORRUG)
            {
                if(val > 0){
                    pos->density = val;
                    init_sqcorrug(st,st->left,val);
                    init_sqcorrug(st,st->right,val);
                }
				
            }
            else if(st->type == STIM_CYLINDER)
            {
                if(val > 0) {
                    pos->density=val;
                    st->left->ptr->density = val;
                }	
            }
            break;
        case XSAMPLES:/*j*/
            if(st->type == STIM_GRATING || st->type == STIM_GRATING2 || st->type == STIM_GABOR)
                if(up)
                {
                    if(val > 1.1 && (optionflag & GORAUD_BIT))
                    {
                        glShadeModel(GL_SMOOTH);
                        pos->ss[1] = val;
                    }
                    else
                    {
                        glShadeModel(GL_FLAT);
                        pos->ss[1] = 1.0;
                    }
                    pos->size[1] = (int)ceilf(pos->imsize[1]/pos->ss[1]);
                    init_stimulus(st);
                }
            break;
        case DOT_SIZE:
            fval = deg2pix(val);
            if(st->type == STIM_RDS || st->type == STIM_RLS || st->type == STIM_RDSSINE || st->type == STIM_CHECKER)
            {
                if(fval > 0.5){
                    st->left->dotsiz[0] = fval;
                    st->left->dotsiz[1] = fval;
                    st->right->dotsiz[0] = fval;
                    st->right->dotsiz[1] = fval;
                    st->pos.dotsize = fval;
                }
            }
            else if(st->type == STIM_CORRUG || st->type == STIM_SQCORRUG ||
                    st->type == STIM_CYLINDER)
            {
                if(fval > 0.5){
                    st->left->ptr->dotsiz[0] = fval;
                    st->left->ptr->dotsiz[1] = fval;
                    st->right->ptr->dotsiz[0] = fval;
                    st->right->ptr->dotsiz[1] = fval;
                    st->pos.dotsize = fval;
                }
            }
            break;
        case YSAMPLES:/*j*/
            if(st->type == STIM_GRATING || st->type == STIM_GRATING2 || st->type == STIM_GABOR)
                if(up)
                {
                    pos->ss[0] = val;
                    pos->size[0] = (int)ceilf(pos->imsize[0]/pos->ss[0]);
                    init_stimulus(st);
                }
            break;
        case PHASEREL:
	        pos->dphase = deg_rad(val);
            break;
        case DOT_POSX:
            pos->locn[0] = deg2pix(val);
            break;
        case SETPHASE:
            pos->phase = deg_rad(val);
            /* 
             * for CM TF expt, need to start at phase =0, so that 36Hz is an alernation.
             * Otherwise, 36Hz "alternates" between grey screen and grey screen!
             */
            if(st->prev && rdspair(st->prev))
                i = 0;
            if(optionflag & CONTRAST_REVERSE_BIT && expt.mode != TF && st->incr > 0)
                pos->contrast_phase = M_PI_2;
            else
                pos->contrast_phase = 0;
            if(fabsf(val) < 10)
                pos->locn[0] = 0;
            if(st->type == STIM_BAR){
                st->right->pos.phase = pos->phase;
                st->left->pos.phase = pos->phase;
            }
		    
            if(st->type == STIM_GRATING2)
		    {
                psine = (OneStim *)(st->left->ptr);
                pos->phase2 = pos->dphase + (pos->phase);
		    }
            if(st->type == STIM_RADIAL){
                st->left->calculated = st->right->calculated = 0;
            }
            break;
        case CONTRAST2:
            pos->contrast2 = val;
            break;
        case PLAID_RATIO:
            // ratio > 1 = contrast1 > contrast2
            bval = StimulusProperty(st, SETCONTRAST);
            dval = StimulusProperty(st, CONTRAST2);
            aval =  (bval > dval) ? bval : dval;
            if (val < 0){
                bval = aval + val;
                dval = aval;
            }
            else{
                bval = aval;
                dval = aval-val;
            }
            SetStimulus(st, bval, SETCONTRAST,  event);
            SetStimulus(st, dval, CONTRAST2,  event);
            break;
        case CONTRAST_RATIO:
            if(st->type == STIM_RDSSINE || st->type == STIM_RADIAL && st->prev == NULL){
                expt.vals[CONTRAST_RATIO] = val;
                if(up)
                    SerialSend(CONTRAST_DIFF);
            }
            else if(st->type == STIM_GRATING2){
                /*
                 fval = (pos->contrast > pos->contrast2) ? pos->contrast : pos->contrast2;
                 if(val > 1){
                 pos->contrast = fval/val;
                 pos->contrast2 = fval;
                 }
                 else{
                 pos->contrast = fval;
                 pos->contrast2 = fval * val;
                 }
                 */
                fval = pos->contrast +  pos->contrast2;
                
                expt.vals[CONTRAST_RATIO] = val;
                
                pos->contrast_amp = pos->contrast;
            }
            else{
                /*
                 * take the lowest of the two eye's contrasts. 
                 * Keep this a fixed value.
                 * make one eyes contrast higher than this. Which eye
                 * depends of whether ratio > 1 or < 1
                 * if they ar both zero, leave alone
                 */
                bval = StimulusProperty(st, CONTRAST_RIGHT);
                dval = StimulusProperty(st, CONTRAST_LEFT);
                if(bval < 0.01 && dval < 0.01)
                    break;
                if(dval < bval)
                    bval = dval;
                SetStimulus(st, bval, CONTRAST_RIGHT,  event);
                SetStimulus(st, bval, CONTRAST_LEFT,  event);
                if(val >= 100){
                    SetStimulus(st, 0, CONTRAST_RIGHT,  event);
                }
                else if(val < 0.01){
                    SetStimulus(st, 0, CONTRAST_LEFT,  event);
                }
                else if(val >= 1){
                    cval = bval * val;
                    SetStimulus(st, cval, CONTRAST_LEFT,  event);
                }
                else if(val < 1){
                    cval = bval * 1.0/val;
                    SetStimulus(st, 1.0/cval, CONTRAST_RIGHT,  event);
                }
            }
            break;
        case FB_RELATIVE_CONTRAST:
            if(st->next != NULL){
                cval = StimulusProperty(st, SETCONTRAST);
                bval = StimulusProperty(st->next, SETCONTRAST);
            if(val < 0){
                SetStimulus(st, 1 - (cval+bval), SETCONTRAST, NULL);
                SetStimulus(st->next, (cval+bval), SETCONTRAST, NULL);
            }
            else{
                SetStimulus(st, (cval+bval), SETCONTRAST, NULL);
                SetStimulus(st->next, 1 - (cval+bval), SETCONTRAST, NULL);
            }
            }
            break;
        case CONTRAST_PAIRS:
            
            /*
             * 1 is high in both eyes,
             * 0 is low in both eyes
             * 0.5 is high in RIGHT eye
             * -0.5 is high in Left eye
             */
            if(fabs(val - 1) < 0.1){
                SetStimulus(st, expt.stimvals[SETCONTRAST], CONTRAST_LEFT,  event);
                SetStimulus(st, expt.stimvals[SETCONTRAST], CONTRAST_RIGHT,  event);
            }
            if(fabs(val - 0) < 0.1){
                SetStimulus(st, expt.stimvals[CONTRAST2], CONTRAST_LEFT,  event);
                SetStimulus(st, expt.stimvals[CONTRAST2], CONTRAST_RIGHT,  event);
            }
            if(fabs(val - .5) < 0.1){
                SetStimulus(st, expt.stimvals[CONTRAST2], CONTRAST_LEFT,  event);
                SetStimulus(st, expt.stimvals[SETCONTRAST], CONTRAST_RIGHT,  event);
            }
            if(fabs(val + 0.5) < 0.1){
                SetStimulus(st, expt.stimvals[CONTRAST2], CONTRAST_RIGHT,  event);
                SetStimulus(st, expt.stimvals[SETCONTRAST], CONTRAST_LEFT,  event);
            }
            break;
        case CONTRAST_LEFT:
            if(val > 1)
                val = 1.0;
            if(val < -1)
                val = -1;
            bval = StimulusProperty(st, CONTRAST_RIGHT);
            dval = (val - bval)/2;
            cval = (val+bval)/2;
            SetStimulus(st, cval, SETCONTRAST,  event);
            SetStimulus(st, dval, CONTRAST_DIFF, event);
            break;
        case CONTRAST_RIGHT:
            if(val > 1)
                val = 1.0;
            if(val < -1)
                val = -1;
            bval = StimulusProperty(st, CONTRAST_LEFT);
            dval = (bval-val)/2;
            cval = (val+bval)/2;
            SetStimulus(st, cval, SETCONTRAST,  event);
            SetStimulus(st, dval, CONTRAST_DIFF, event);
            break;
        case CONTRAST_DIFF:
            if(st->type == STIM_RDSSINE || st->type == STIM_RADIAL)
                expt.vals[CONTRAST_RATIO] = val;
            else{
                st->contrast_disp = val;
                expt.vals[CONTRAST_RATIO] = 1.0;
            }
            break;
        case SF_DIFF:
            st->sf_disp = val/2;
            break;
        case SIZE_DISP:
            st->dlength = val;
            st->dwidth = val;
            mode |= NEED_REPAINT;
            break;
        case LENGTH_DISP:
            mode |= NEED_REPAINT;
            if(isinduced(expt.vals[ALTERNATE_STIM_MODE] )){
                if(val > 1){
                    st->left->vscale = 1.0;
                    st->right->vscale = val;
                }
                else{
                    st->left->vscale = 1.0/val;
                    st->right->vscale = 1.0;
                }
            }
            else{
                st->dlength = val;
            }
            break;
        case WIDTH_DISP:
            st->dwidth = val;
            mode |= NEED_REPAINT;
            break;
        case WIDTH_L:
            left = val;
            a = StimulusProperty(st, STIM_WIDTH);
            right = a - st->dwidth/2;
            SetStimulus(st, (left + right)/2, STIM_WIDTH, event);
            mode |= NEED_REPAINT;
            st->dwidth = (left - right);
            break;
        case HEIGHT_L:
            left = val;
            a = StimulusProperty(st, STIM_HEIGHT);
            right = a - st->dlength/2;
            SetStimulus(st, (left + right)/2, STIM_HEIGHT, event);
            st->dlength = (left - right);
            mode |= NEWDRAG;
            mode |= NEED_REPAINT;
            break;
        case SIZE_L:
            left = val;
            a = StimulusProperty(st, STIM_SIZE);
            right = a - (st->dwidth + st->dlength)/4;
            SetStimulus(st, (left + right)/2, STIM_SIZE, event);
            st->dwidth = (left - right);
            st->dlength = (left - right);
            mode |= NEWDRAG;
            break;
        case SIZE_R:
            right = val;
            a = StimulusProperty(st, STIM_SIZE);
            left = a + (st->dwidth + st->dlength)/4;
            SetStimulus(st, (left + right)/2, STIM_SIZE, event);
            st->dwidth = (left - right);
            st->dlength = (left - right);
            mode |= NEWDRAG;
            mode |= NEED_REPAINT;
            break;
        case WIDTH_R:
            right = val;
            a = StimulusProperty(st, STIM_WIDTH);
            left = a + st->dwidth/2;
            SetStimulus(st, (left + right)/2, STIM_WIDTH, event);
            st->dwidth = (left - right);
            mode |= NEWDRAG;
            break;
        case HEIGHT_R:
            right = val;
            a = StimulusProperty(st, STIM_HEIGHT);
            left = a + st->dlength/2;
            SetStimulus(st, (left + right)/2, STIM_HEIGHT, event);
            st->dlength = (left - right);
            mode |= NEWDRAG;
            mode |= NEED_REPAINT;
            break;
        case DORI_BOTH:
            st->ori_disp = deg_rad(val)/2;
        case DORI_BACK:
            if(st->next != NULL)
                st->next->ori_disp = deg_rad(val)/2;
            break;
        case BACK_ORI:
            if(st->next != NULL)
                st->next->pos.angle = deg_rad(val);
            break;
        case ORIENTATION_DIFF:
            st->ori_disp = deg_rad(val)/2;
            break;
        case SETCONTRAST:
            if(val > 10)
                val = 1.0;
            if(val < -1)
                val = -1;
            pos->contrast_amp = val;
            pos->contrast = pos->contrast_amp * cos(pos->contrast_phase);
            if(expt.vals[GRIDSIZE] > 0.1){
            }
            break;
        case SETFIXCOLOR:
            /*		st->fix.fixcolor = st->fixcolor = val;*/
            st->fix.fixcolor = val;
            TheStim->mode |= NEW_FIXATION_BIT;
            break;
        case FIX_OFF_CODE:
            st->fix.offcolor = val;
            TheStim->mode |= NEW_FIXATION_BIT;
            break;
        case SACTARGSIZE:
            afc_s.targsize = deg2pix(val);
            break;
        case SETFIXSIZE:
            st->fix.size = deg2pix(val);
            TheStim->mode |= NEW_FIXATION_BIT;
            break;
        case SETZOOM:
            zoom = val;
            break;
        case SETZXOFF:
            fval = deg2pix(val);
            LocateStimulus(st, fval+winsiz[0],pos->xy[1]+winsiz[1]);
            break;
        case SETZYOFF:
            fval = deg2pix(val);
            LocateStimulus(st, pos->xy[0]+winsiz[0],fval+winsiz[1]);
            break;
        case SETOVERLAYCOLOR:
            expt.overlay_color = val;
            if(up)
            {
                clear_display(1);
            }
            break;
        case SETCLEARCOLOR:
            expt.vals[SETCLEARCOLOR] = val;
            clearcolor = dogamma(val);
            break;
        case SETBACKCOLOR:
            if(val > 1)
                val = val/255.0;
            expt.vals[SETCLEARCOLOR] = st->background = val;
            expt.vals[BLANKCOLOR_CODE] = val;
            st->gammaback = dogamma(st->background);
            clearcolor = st->gammaback;
            next = st;
            while((next = next->next))
            {
                next->background = val;
                next->gammaback  = st->gammaback;
            }
            ChoiceStima->background = val;
            ChoiceStimb->background = val;
            ChoiceStima->gammaback  = st->gammaback;
            ChoiceStimb->gammaback  = st->gammaback;
            if(up)
            {
                //Ali SetPanelColor(st->gammaback);
                clear_display(1);
            }
            break;
        case MODE_CODE:
            st->mode = (int)val;
            break;
        case STATIC_VERGENCE:
            expt.sv = val;
            break;
        case STATIC_CONJUGATE:
            expt.cj = val;
            break;
        case RAMP_COEFFICIENT:
            expt.clamp_rrate = val;
            break;
        case FIXPOS_X:
            expt.vals[code] = val;
            fixpos[0] = deg2pix(val);
            break;
        case FIXPOS_Y:
            expt.vals[code] = val;
            fixpos[1] = deg2pix(val);
            break;
        case REWARD_SIZE:
            st->fix.rwsize = val;
            break;
        case BLANKCOLOR_CODE:
            expt.vals[BLANKCOLOR_CODE] = val;
            break;
        case BRIGHTSHIFT_CODE:
            st->fix.stimcolor = st->fix.fixcolor +val;
            break;
        case STOP_CRITERION:
            stopcriterion = val;
            break;
        case RUNAVERAGE_LENGTH:
            avglen = (int)val;

            break;
        case MODULATION_F:
            if(optionflags[MODULATE_DISPARITY] < 2){
                if(val > 0)
                    optionflags[MODULATE_DISPARITY] = 1;
                else
                    optionflags[MODULATE_DISPARITY] = 0;
            }
            st->vals[code] = val;
            break;
        case NLOWCOMPONENTS:
            if(st->type == STIM_GRATINGN){
            a = expt.stimvals[SF2] - expt.stimvals[SF]; //step
            b = expt.stimvals[SF] - a * ceil((expt.stimvals[NCOMPONENTS]-1)/2);
            st->nfreqs = val;
            st->f = b + a * floor(st->nfreqs/2);
            st->left->ptr->sf2 = st->f + a;
            st->right->ptr->sf2 = st->f + a;
            for(i = 0; i < st->nfreqs; i++){
                st->freqs[i] = st->f + a * (i - floor(st->nfreqs/2));
            }
            }
            break;
        case NHIGHCOMPONENTS:
            if(st->type == STIM_GRATINGN){
            a = expt.stimvals[SF2] - expt.stimvals[SF]; //step
            b = expt.stimvals[SF] + a * ceil((expt.stimvals[NCOMPONENTS]-1)/2);
            st->nfreqs = val;
            st->f = b - a * floor(st->nfreqs/2);
            st->left->ptr->sf2 = st->f + a;
            st->right->ptr->sf2 = st->f + a;
            for(i = 0; i < st->nfreqs; i++){
                st->freqs[i] = st->f + a * (i - floor(st->nfreqs/2));
            }
            }
            break;
        case NCOMPONENTS:
            st->nfreqs = (int)val;
            a = StimulusProperty(st,SF2);
            for(i = 0; i < st->nfreqs; i++){
                st->freqs[i] = st->f + (a - st->f) * (i - floor(st->nfreqs/2));
            }
            break;
        case START_PHASE:
            st->vals[code] = val;
            break;
        default:
            return(-1);
	}
    }
    
	if(st->type == STIM_CYLINDER){/*j*/
		if (code != JDEATH && code != DISP_X && code != JVELOCITY && code != JF_INTENSITY && code != JB_INTENSITY && code != STANDING_DISP) {
            init_cylinder(st,st->left);	
		}
	}
	if(up)
	{
	    if(mode & RUNNING && stimstate != STIMSTOPPED)
        {
            /* if changing backstim, re-paint  removed 24/22/98. Always doing backstim now*/
            /*	  if(st->prev != NULL) 
             clear_display(0);*/
            switch(code)
            {
                case DISP_P:
                case SF:
                default:
                    break;
                case FIXPOS_Y:
                case FIXPOS_X:
                    clear_display(0);
                    break;
                case BACK_VDISP:
                case DISP_BACK:
                case DISP_X:
                case DISP_Y:
                    mode |= NEWPOS;
                    break;
                case SETBACKCOLOR:
                    mode |= NEWPOS;
                    break;
            }
        }
	}
    
	if(st->next != NULL && optionflags[PAINT_BACKGROUND] && st->next->type != STIM_NONE)
    {
	    switch(code)
        {
            case STIM_ECCENTRICITY:
                SetStimulus(st->next, val, code, event);
                break;
        }
    }
	if((rdspair(st) || corrugpair(st) || sqcorrugpair(st) || rlspair(st)) && st->prev == NULL)
    {
	    switch(code)
        {
            case CONTRAST_DIFF:
            case SETCONTRAST:
            case SET_SEED:
            case SET_SEEDLOOP:
            case DOT_DENSITY:
            case DOT_SIZE:
            case XSAMPLES:
            case YSAMPLES:
                SetStimulus(st->next, val, code, event);
                break;
            case CORRELATION:
                if(fabs(expt.stimvals[BACK_CORRELATION]) > 0 && !(optionflag & BACKGROUND_FIXED_BIT))
                    SetStimulus(st->next, val, code, event);
                break;
        }
    }
	if(code >= MAXTOTALCODES)
		return(-1);
	else if(event != NOEVENT && (!(optionflag & FRAME_ONLY_BIT)) && (mode & SERIAL_OK) && st->prev == NULL && !noserialout)
	{
        if(st->flag != oldflag || optionflag != oldoptionflag)
            SerialSend(OPTION_CODE);
        SerialSend(code);
	}
	else if(st->prev != NULL && event != NOEVENT && !noserialout
            && !(inexptstim && optionflags[FAST_SEQUENCE])){
        switch(code){
            case DISP_X:
                SerialSend(DISP_BACK);
                break;
            case STIM_SIZE:
                SerialSend(BACK_SIZE);
                break;
            case CORRELATION:
                SerialSend(BACK_CORRELATION);
                break;
            case ORIENTATION:
                SerialSend(BACK_ORI);
                break;
            case TF:
                SerialSend(BACK_TF);
                break;
        }
	}
	return(0);
}



void nsine_background()
{
    int i;
    tempstim->type = STIM_GRATINGN;
    memcpy(tempstim->freqs,expt.st->freqs,(sizeof(float) * MAXFREQS));
    tempstim->pos.contrast = 1.2;
    tempstim->pos.ss[1] = 1;
    SetStimulus(tempstim,pix2deg(winsiz[0] * 2),STIM_WIDTH,NULL);
    SetStimulus(tempstim,pix2deg(winsiz[1] * 2),STIM_HEIGHT,NULL);
    tempstim->nfreqs = expt.st->nfreqs;
    tempstim->pos.angle = M_PI/2;
    tempstim->pos.xy[0] = 0;
    tempstim->pos.xy[1] = 0;
    tempstim->background  = 0.5;  
    if(optionflags[RANDOM_PHASE]){
        for(i = 0; i < tempstim->nfreqs; i++)
            tempstim->phases[i] = (myrnd_i() %360) * M_PI/180;
    }
    calc_stimulus(tempstim);
    paint_stimulus(tempstim);
    //AliGLX  mySwapBuffers();
    paint_stimulus(tempstim);
}

void search_background()
{
    int i,j,xstep,ystep,rnd,nb;
    int oldoption = optionflag;
    float val,vborder,hborder,rndval;
    int forcecalc = 1;
    ystep = xstep = 70;
    
    /*
     * paint binocular search screen
     */
    if(expt.st->type == STIM_GRATINGN && issfrc(expt.stimmode) && expt.mode == SF){
        nsine_background();
        return;
    }
    
    
    optionflag &= (~(ANTIALIAS_BIT | LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
    
    /*
     * background is white, so need black bars
     */
    
    if(newtimeout < 5 || forcecalc){
        tempstim->type = STIM_BAR;
        tempstim->pos.contrast = -1.0 * expt.vals[TIMEOUT_CONTRAST];
        tempstim->pos.angle = 0;
        tempstim->disp = 0;
        tempstim->pos.phase = 0;
        tempstim->pos.radius[0] = 50;
        tempstim->pos.radius[1] = 50;
        tempstim->pos.xy[0] = 0; 
        tempstim->pos.xy[1] = 0;
        tempstim->left->pos.xy[0] = 0; 
        tempstim->left->pos.xy[1] = 0;
        tempstim->right->pos.xy[0] = 0; 
        tempstim->right->pos.xy[1] = 0;
        vborder = hborder = 50;
        tempstim->f = 5;
        tempstim->pos.sf = 5;
        tempstim->mode &= (~STIMULUS_NEEDS_CLEAR);
        tempstim->fixcolor = tempstim->background = expt.vals[BLANKCOLOR_CODE];
        tempstim->next = NULL;
        tempstim->left->mode = LEFTMODE;
        tempstim->right->mode = RIGHTMODE;
        tempstim->left->pos.contrast = tempstim->right->pos.contrast = tempstim->pos.contrast;
        tempstim->left->pos.radius[0] = tempstim->right->pos.radius[0] = tempstim->pos.radius[0];
        tempstim->left->pos.radius[1] = tempstim->right->pos.radius[1] = tempstim->pos.radius[1];
        nb = (1280/xstep) * (1024/ystep);
        tempstim->left->pos.phase = tempstim->right->pos.phase = tempstim->pos.phase;
        tempstim->left->nbars = tempstim->right->nbars = nb;
        tempstim->noclear = 1;
        
 
        rndval = (float)(newtimeout*expt.st->angleinc);
        init_stimulus(tempstim);
        srandom(TheStim->left->baseseed);
        nb = 0;
        for(i = xstep/2 - 1280/2+hborder; i < 1280/2-hborder; i += xstep)
        {
            tempstim->pos.xy[0] = i;
            for(j = ystep/2-1024/2+vborder; j < 1024/2-vborder; j += ystep)
            {
                rnd = random();
                val = (float)(rnd%134) + rndval;
                if(rnd & 0x100)
                    val = (val * val)/99.7;
                else
                    val = -(val * val)/99.7;
                tempstim->left->imb[nb] =  (expt.rf->angle + val) * 2 * M_PI/360.0;
                tempstim->left->ypos[nb] = j;
                tempstim->left->xpos[nb] = i;
                tempstim->right->imb[nb] =  (expt.rf->angle + val) * 2 * M_PI/360.0;
                tempstim->right->ypos[nb] = j;
                tempstim->right->xpos[nb] = i;
                nb++;
                if (tempstim->f < 5 || tempstim->left->pos.sf < 5 ||tempstim->right->pos.sf < 5 || tempstim->right->pos.radius[0] > 200)
                {
                    //	    printf("Backgr pattern f = %.3f\n",tempstim->f);
                }
            }
        }
        tempstim->left->nbars = tempstim->right->nbars = nb;
        newtimeout++;
    }
    gettimeofday(&now,NULL);
    if(timediff(&now,&lastcleartime) > 60)
        newtimeout = 1;
    if(newtimeout < 5 || forcecalc){
        setmask(ALLMODE);
        clearcolor = expt.vals[BLANKCOLOR_CODE];
        glClearColor(clearcolor,TheStim->gammaback,clearcolor,clearcolor);
        glClear(GL_COLOR_BUFFER_BIT);
        ShowTime();
        paint_stimulus(tempstim);
        optionflag = oldoption;
        srandom(TheStim->left->baseseed);
        setmask(bothmask);
        gettimeofday(&lastcleartime,NULL);
    }
}

void start_timeout(int mode)
{
    int i,j,xstep,ystep,rnd;
    float val;
    struct timeval estart;
    
    
    newtimeout = 1;
    if(optionflags[INITIAL_TRAINING])
        optionflags[INITIAL_TRAINING] = 2;
    if(mode == SHAKE_TIMEOUT)
        mode = timeout_type = SHAKE_TIMEOUT_PART1;
    if(timeout_type == SHAKE_TIMEOUT_PART1 || timeout_type == SHAKE_TIMEOUT)
    {
        timeout_type = SHAKE_TIMEOUT_PART1;
        SetStimulus(expt.st,0.0, SETBACKCOLOR,NULL);
        mode = SHAKE_TIMEOUT_PART1;
        TheStim->fixcolor = 0;
    }
    else if(TheStim->fix.timeout > 0 || mode != BAD_FIXATION){
        glDrawBuffer(GL_FRONT_AND_BACK);
        setmask(ALLMODE);
        TheStim->fixcolor = expt.vals[BLANKCOLOR_CODE];
        if(optionflag & FRAME_ONLY_BIT)
        {
            clearcolor = dogamma(expt.vals[SETCLEARCOLOR]);
            glClearColor(clearcolor,clearcolor,clearcolor,clearcolor);
        }
        else
        {
            clearcolor = expt.vals[BLANKCOLOR_CODE];
            glClearColor(clearcolor,TheStim->gammaback,clearcolor,clearcolor);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glFlushRenderAPPLE();
        glDrawBuffer(GL_BACK);
    }
	gettimeofday(&starttimeout,NULL);
	gettimeofday(&lastsertime,NULL);
    
    switch (mode){   /*j monkey needs to know what he has done wrong */   
	    default:
	    case SEARCH:
            search_background();
            break;
        case SHAKE_TIMEOUT_PART1:
            if(stimstate != STIMSTOPPED){
                fixstate = BADFIX_STATE;
                stimstate = POSTTRIAL;
            }
            break;
        case SHAKE_TIMEOUT_PART2:
            SetStimulus(expt.st,0.5, SETBACKCOLOR,NULL);
            SetStimulus(expt.st,1.0, BLANKCOLOR_CODE,NULL);
            glDrawBuffer(GL_FRONT_AND_BACK);
            setmask(ALLMODE);
            search_background();
               glDrawBuffer(GL_BACK);
            break;
	    case BAD_FIXATION:
            fixstate = BADFIX_STATE;
            stimstate = POSTTRIAL;
            /*	
             * BC changed to give normal timeout pattern for
             * bad saccades, chessboard for wrong response
             */
            /*	if(SACCREQD(afc_s) && fixstate != BADFIX_STATE)
             chessboard(128,128);
             else
             */
            if(TheStim->fix.timeout > 0.01)
                search_background();
            break;
        case WURTZ_OK_W:
            chessboard(128,128);
            stimstate = POSTTRIAL;
            break;
	}
	glFlushRenderAPPLE();
	if(stimstate != IN_TIMEOUT && stimstate != POSTTRIAL)
        stimstate = STIMSTOPPED;
	BackupStimFile();

	if(ExptIsRunning()){
        gettimeofday(&estart,NULL);
        /*
         * Some expts it is crucial that reset is call before prepare
         * e.g. if animal broke fixation during a pursuit trial
         */
        ResetExpStim(1);
        PrepareExptStim(1,11);
        gettimeofday(&now,NULL);
	}
    /*	fiterate(toplevel, 0, 0);*/
}

void end_timeout()
{
    char buf[256];
    
    
    
    if(timeout_type == SHAKE_TIMEOUT_PART2 || timeout_type == SHAKE_TIMEOUT || timeout_type == SHAKE_TIMEOUT_PART1){
        SetStimulus(expt.st,0.5, SETBACKCOLOR,NULL);
        SetStimulus(expt.st,1.0, BLANKCOLOR_CODE,NULL);
        glDrawBuffer(GL_FRONT_AND_BACK);
        setmask(ALLMODE);
        search_background();
    }
    
    
    
    TheStim->mode &= (~(INTRIAL));
    clearcolor = dogamma(expt.vals[SETCLEARCOLOR]);
    timeout_type = 0;
    if(monkeypress == WURTZ_STOPPED)
        return;
    TheStim->fixcolor = TheStim->fix.offcolor;
    TheStim->mode |= NEW_FIXATION_BIT;
    stimstate = INTERTRIAL;
    clear_display(1);
    TheStim->fix.state = FIX_IS_OFF;
    mode &= (~HOLD_STATUS);
    
    /*
     * this paints the choice target stimuli before the first stim of an expt.
     * and I don't see why it is here anyway.... Feb 2006. bgc
     if(SACCREQD(afc_s)){
     paint_target(TheStim->background,1);
     optionflag |= (DRAW_FIX_BIT);
     }
     */
    sprintf(buf,"%2s-\n",valstrings[valstringindex[STOP_BUTTON]].code);
    SerialString(buf,0);
    newtimeout = 1;
       glDrawBuffer(GL_BACK);
    
}

void SendMovements()
{
    char buf[BUFSIZ],temp[BUFSIZ];
    int i;
    
    sprintf(buf, "%s",serial_strings[DRAGGED_POS]);
    for(i = 0; i < drag; i++)
    {
        sprintf(temp,"%d:%.2f ",framelist[i],poslist[i]);
        strcat(buf,temp);
    }
    SerialString(buf,0);
}


    static int stimchanged = 0;
void WriteSignal()
{
    char c = ' ';

    struct timeval atime;
    float val;
    
    
	if(mode & WURTZ_FRAME_BIT)
    {
	    c = START_TRIAL;
        write(ttys[0],&c,1);
        gettimeofday(&wurtzframetime,NULL);
        if(mimic_fixation == 1)
            fixstate = GOOD_FIXATION;
    }
	if(mode & FIRST_FRAME_BIT)
    {
#ifdef NIDAQ
        if (optionflags[MICROSTIM])
            DIOWriteBit(1, 1);
        DIOWriteBit(2, 1);
#endif
        stimchanged = 0;
	    c = FRAME_SIGNAL;
        write(ttys[0],&c,1);
        gettimeofday(&firstframetime,NULL);
        val = timediff(&firstframetime,&changeframetime);
        memcpy(&zeroframetime, &firstframetime, sizeof(struct timeval));
        expstate = 0;
        framesdone = 0;
        framectr = 0;

        if(seroutfile)
            fprintf(seroutfile,"O 5 %u\n",ufftime(&firstframetime));
	}
	if(mode & STIM_FRAME_BIT)
    {
	    c = START_STIM;
        write(ttys[0],&c,1);
        gettimeofday(&firstframetime,NULL);
        if(mimic_fixation == 1)
            fixstate = GOOD_FIXATION;
	}
	if(mode & LAST_FRAME_BIT)
    {
#ifdef NIDAQ
            DIOval = 0;
            DIOWriteBit(2,0); 
#endif        
        gettimeofday(&endstimtime,NULL);
 	    c = END_STIM;
        if(seroutfile)
            fprintf(seroutfile,"O %d %u %u %.3f%c\n",(int)(c),ufftime(&endstimtime),
                    ufftime(&endstimtime)-ufftime(&zeroframetime),timediff(&endstimtime,&firstframetime),InExptChar);

        expstate = END_STIM;
        write(ttys[0],&c,1);
#ifdef FRAME_OUTPUT
                if (Frames2DIO)
	    DIOWriteBit(3,1);
#endif
	}
	if(mode & STIMCHANGE_FRAME)
    {
	    c = STIM_CHANGE;
	    write(ttys[0],&c,1);
        stimchanged = 1;
#ifdef NIDAQ
	    DIOWriteBit(0, 1);                  
#endif
	    if(!optionflags[REDUCE_SERIAL_OUTPUT]){
            if(seroutfile)
                fprintf(seroutfile,"ds+ %u\n",ufftime(&frametime));
	    }
	    if(rcfd){
            fprintf(rcfd,"ds+ %u\n",ufftime(&frametime));
	    }
    }
    
	mode &= (~(FRAME_BITS | STIMCHANGE_FRAME));
	outcodes[++outctr%CODEHIST] = c;
}

int change_frame()
{
	char c = FRAME_SIGNAL;
	char buf[BUFSIZ];
	int lastframe,oldmode = mode;
	static int framesswapped = 0;
	vcoord x[2];
    
    
#if defined(WIN32NOT)
    /*
     * Under windows, repeated calls to change frame when no painting has been done
     * allows the graphics pipleline to get behind the CPU, so that next time real
     * painting is called for, it hangs while all the swaps get implemented.
     */
	static struct timeval lasttime;
	float val;
    
	gettimeofday(&now,NULL);
	if((val = timediff(&now, &lasttime)) < 0.0001/mon.framerate){
        memcpy(&lasttime, &now, sizeof(struct timeval));
        return(0);
	}
	memcpy(&lasttime, &now, sizeof(struct timeval));
#endif

	if(mode & LAST_FRAME_BIT)
	{
		if(!(optionflag & FRAME_ONLY_BIT) || (optionflag & WAIT_FOR_BW_BIT))
			c = END_STIM;
		else
			mode &= (~FRAME_BITS);
	}
    
	else if(mode & FIRST_FRAME_BIT || mode & WURTZ_FRAME_BIT)
    {
	    resetframectr();
	    expstate = 0;
        lastwurtzcount = wurtzcount;
        framesswapped = 0;
	}
	/*
	 * N.B. Need all three of these calls to make it work smoothly.
	 * otherwise things seem to get piled up in the pipeline blocking
	 * Xevnet handling
	 */
	if(framehold > 0.01){
        x[0] = 0;
        x[1] = 0;
        mycmv(x);
        sprintf(buf,"%d",framesswapped);
        printString(buf,1);
	}
    //	—glFlushRenderAPPLE();
    //AliGLX	mySwapBuffers();
	glFinishRenderAPPLE();
    glSwapAPPLE();
    gettimeofday(&changeframetime,NULL);

	framesswapped++;
#ifdef NIDAQ
    if (stimchanged){
        DIOWriteBit(0, 0);                 
        stimchanged = 0;
    }
#endif
    //This does nothing any more. But interferes with last frame timing. 
    //This should be in nextframe/runexptstim
    if(oldmode & LAST_FRAME_BIT && !(mode & LAST_FRAME_BIT))
        stimstate = POSTSTIMULUS;


    if(mode & FRAME_BITS)
    {
#ifdef FRAME_OUTPUT
        if (Frames2DIO)
	    DIOWriteBit(3,1);
#endif
	    if(!(mode & STIMCHANGE_FRAME))
            glFinishRenderAPPLE(); /* block until buffer swapped */
        gettimeofday(&changeframetime,NULL);
	    WriteSignal();
	    if(c == END_STIM){
            sprintf(buf,"%s%d\n",serial_strings[NFRAMES_CODE],framesdone);
            SerialString(buf,0);
	    }
    }
    
#ifdef FRAME_OUTPUT
	else if (Frames2DIO)
    {
        DIOWriteBit(3,1); //without 8 written in RunExptStim
    }
#endif
	thebuffer = !thebuffer;
	if(mode & RESET_FRAME_CTR)
    {
	    mode &= (~RESET_FRAME_CTR);
	    memcpy(&zeroframetime, &changeframetime, sizeof(struct timeval));
        /* 
         *       framecount = 1 = first frame and counting
         *       framecount = 0 = not running a set of frames
         */
	    framecount = 1;
    }
	realframecount = getframecount();
    
	if(stmode & DRAG_STIMULUS)
    {
	    stmode &= (~DRAG_STIMULUS);
	    framelist[drag] = realframecount;
	    poslist[drag] = StimulusProperty(TheStim, SETZXOFF);
	    if(++drag > MAXDRAGS)
            drag = 0;
    }
	if(framehold > 0.01){
        fsleep(framehold);
	}
	return(0);
}


int SetRandomPhase( Stimulus *st,     Locator *pos)
{
    int iphase,i;
    
    if(expt.stimmode == FOUR_PHASES){
        iphase = myrnd_i() %4;
        /* 
         * tests for stimtype must use expt.stimtype, not st->
         * an interleaved blank changes st->. This is called before
         * st->type is reset on the next frame.
         */
        if(expt.stimtype == STIM_BAR){
            if(iphase == 0){
                pos->contrast = 1;
                st->contrast_disp = 0;
            }
            else if (iphase == 1){
                pos->contrast = -1;
                st->contrast_disp = 0;
            }
            else if (iphase == 2){
                pos->contrast = 0;
                st->contrast_disp = 2;
            }
            else if (iphase == 3){
                pos->contrast = 0;
                st->contrast_disp = -2;
            }
            pos->phase = 0;
        }
        else{
            pos->phase = iphase * M_PI/2;
            pos->phase2 = (myrnd_i() %4) * M_PI/2;
        }
    }
    else if(expt.stimmode == TWO_PHASES){
        iphase = myrnd_i() %2;
        pos->phase = iphase  * M_PI;
        pos->phase2 = (myrnd_i() %2) * M_PI;
    }
    else{
        iphase = (myrnd_i() %360);
        pos->phase = (iphase * M_PI)/180;
        pos->phase2 = (myrnd_i() %360);
        pos->phase2 *= (M_PI/180);
        for(i = 0; i < st->nfreqs; i++)
            st->phases[i] = (myrnd_i() %360) * M_PI/180;
    }
    frameiseqp[expt.framesdone] = iphase;
    if(pos->phase != 0)
        iphase = 0;
}
void SetRandomCorrelation(Stimulus *st)
{
    double arnd,brnd;
    arnd = mydrand(); //left
    if(st->flag & ANTICORRELATE)
        arnd *= -1;
    st->correlation = arnd;
    if(rdspair(st) || rlspair(st)){
        st->next->correlation = arnd;
    }
}

void SetRandomContrast(Stimulus *st)
{
	double arnd,brnd;
	arnd = mydrand(); //left
	brnd = mydrand(); //right
	st->pos.contrast = st->pos.contrast_amp * (arnd+brnd)/2;
	st->contrast_disp = st->pos.contrast_amp * (arnd-brnd)/2;
	if(rdspair(st) || rlspair(st)){
	    st->next->pos.contrast = st->pos.contrast_amp * (arnd+brnd)/2;
	    st->next->contrast_disp = st->pos.contrast_amp * (arnd-brnd)/2;
	}
}



void increment_stimulus(Stimulus *st, Locator *pos)
{
    Substim *rds,*rdsb;
    OneStim *psine = st->left->ptr;
    int i,seedframe;
    float fval,newx,newy,dx,dy,x,y;
    Thisstim *stp;
    static double dispphase = 0,idisp = 0;
    double rphase,rnd,arnd,brnd;
    int period,asmode,newseed=0,ival,iphase;
    char buf[BUFSIZ];
    static int dispchanged = 0;
    
    if(freezestimulus > 1)
        return;
    
    asmode = (int)(expt.vals[ALTERNATE_STIM_MODE]);
    if(st->next != NULL && optionflags[PAINT_BACKGROUND])
        increment_stimulus(st->next,&st->next->pos);
    
    seedframe = (int)rint(expt.vals[CHANGE_SEED]);
    
    /*
     * check whether the time has come to move the FixPoint, or shift the
     * stimulus. N.B. this only needs to be done once, don't repeat it if 
     * incrementing the background stimulus. Hence only do it if st->prev == NULL
     */
    if(st->prev == NULL){
        if((i = (int)rint(expt.vals[FP_MOVE_FRAME])) > 0
           && i != seedframe){
            if(realframecount%i == (i-1)){
                gettimeofday(&now,NULL);
                if((realframecount/i) &1) // Odd number
                {
                    newx = oldfixpos[0];
                    newy = oldfixpos[1];
                    dx = dy = 0;
                    if(optionflags[RAND_FP_DIR]){
                        do{
                            expt.vals[FP_MOVE_DIR] = drand48() * M_PI * 2;
                            fval = expt.vals[FP_MOVE_DIR];
                            dx  = expt.vals[FP_MOVE_SIZE] * cos(fval);
                            dy = expt.vals[FP_MOVE_SIZE] * sin(fval);
                            newx = fixpos[0] + deg2pix(dx);
                            newy = fixpos[1] + deg2pix(dy);
                        }while(newy < 8 && newy > -8 && newx > -8 && newx < 8);
                    }
                }
                else{
                    SerialSend(FP_MOVE_DIR);
                    if(optionflags[RAND_FP_DIR]){
                        do{
                            expt.vals[FP_MOVE_DIR] = drand48() * M_PI * 2;
                            fval = expt.vals[FP_MOVE_DIR];
                            dx  = expt.vals[FP_MOVE_SIZE] * cos(fval);
                            dy = expt.vals[FP_MOVE_SIZE] * sin(fval);
                            newx = fixpos[0] + deg2pix(dx);
                            newy = fixpos[1] + deg2pix(dy);
                        }while(newy < 8 && newy > -8 && newx > -8 && newx < 8);
                    }
                    else{
                        fval = expt.vals[FP_MOVE_DIR];
                        dx  = expt.vals[FP_MOVE_SIZE] * cos(fval);
                        dy = expt.vals[FP_MOVE_SIZE] * sin(fval);
                        newx = fixpos[0] + deg2pix(dx);
                        newy = fixpos[1] + deg2pix(dy);
                    }
                    
                }
                if(optionflags[SIMULATE_FP_MOVE]){
                    SetStimulus(stimptr,expt.stimvals[XPOS]+dx, XPOS,NULL);
                    SetStimulus(stimptr,expt.stimvals[YPOS]+dy, YPOS,NULL);
                    mode |= STIMCHANGE_FRAME;
                }
                else{
                    NewFixPos(newx, newy);
                }
            }
        }
    }
    if(st->type == STIM_RADIAL)
    {
        if (seedframe > 0 && realframecount >= seedframe && stimstate == INSTIMULUS &&
            (realframecount < seedframe + st->left->ptr->lifeframes || st->left->ptr->lifeframes == 0)){
            if(st->left->show_increment == 0)
                mode |= STIMCHANGE_FRAME;
            st->left->show_increment = 1;
            st->right->show_increment = 1;
        }
        else{
            st->left->show_increment = st->right->show_increment = 0;
        }
    }
    /*
     * to make a static RDS change seed after a certain number of frames, set RANDOM_PHASE and
     * CHANGE_SEED. That way CHANGE_SEED can be used to do iniial dispaity expts with RDS
     */
    
    if((optionflags[RANDOM_PHASE] || st->left->seedloop == 0)&& 
       (st->type == STIM_RDS || st->type == STIM_RLS || st->type == STIM_CHECKER))
    {
        rds = st->left;
        if (seedframe > 0 && realframecount%seedframe == (seedframe-1)){
            gettimeofday(&now,NULL);
            mode |= STIMCHANGE_FRAME;
        }
        /*
         * Dec '05, now done here, not in calc_rds, so that freeze/step works
         */
        if(testflags[REPEAT_DOTS])
            rds->seed++;
        else if(rds->seedloop == 0 || 
                (mode & STIMCHANGE_FRAME && seedframe > 0))
        {
            rds->baseseed += 2;
        }
        
        /*
         * always stepping 2 is easier. But need to make seed delay * 2 also....
         *	  if(st->flag & UNCORRELATE)
         *         rds->baseseed++;
         */
    }
    if(st->type == STIM_CYLINDER || st->type == STIM_RDS && st->left->seedloop == 1){
        if(realframecount == 0)
            dispchanged = 0;
        if (seedframe > 0){
            if(realframecount< seedframe){
                if(expt.type2 == INITIAL_MOVEMENT){
                    dx = sin(expt.vals[FP_MOVE_DIR]) * expt.vals[INITIAL_MOVEMENT];
                    dy = cos(expt.vals[FP_MOVE_DIR]) * expt.vals[INITIAL_MOVEMENT];
                    x = expt.vals[XPOS] + (realframecount-seedframe) * dx;
                    y = expt.vals[YPOS] + (realframecount-seedframe) * dy;
                    TheStim->pos.xy[0] = deg2pix(x);
                    TheStim->pos.xy[1] = deg2pix(y);
                }
                else{
                    st->nchanges = 0;
                    if(st->left->ptr->deathchance == 0){
                        st->posinc = 0;
                        st->left->ptr->velocity = 0;
                    }
                    idisp = st->disp;
                    /*
                     * the background cylinder has idisp applied if back disp <APPLY_MAX then set to zero
                     June 2 2010 - Never apply the Initial disp to the background. When would we want this?
                     */
                    /*
                     if(st->next != NULL && st->next->type == STIM_CYLINDER &&
                     fabs(st->next->disp) < deg2pix(expt.vals[INITIAL_APPLY_MAX]/2)){
                     
                     SetStimulus(st->next,expt.vals[INITIAL_DISPARITY],DISP_X,NOEVENT);
                     }
                     */
                    /*
                     * only do Initial disparity manip for foreground stimulus
                     */
                    if(st->prev == NULL && fabs(olddisp) < deg2pix(expt.vals[INITIAL_APPLY_MAX]/2)){
                        SetStimulus(st,expt.vals[INITIAL_DISPARITY],DISP_X,NOEVENT);
                        idisp = st->disp;
                        dispchanged++;
                        if(realframecount == 0)
                            SerialSend(DISP_X);
                    }
                    // set nchanges to 1 so that no signal sent if no change
                    else if(st->left->ptr->deathchance > 0)
                        st->nchanges = 1;
                }
            }
            else{
                if(st->prev == NULL){
                    st->disp = olddisp;
                    if(st->next != NULL && st->next->type == STIM_CYLINDER){
                        st->next->disp = deg2pix(expt.vals[DISP_BACK])/2;
                    }
                    st->left->ptr->velocity = oldvelocity;
                    st->posinc = oldvelocity;
                }
                if(expt.stimmode == REMOVE_BACKSURFACE && st->flag & FLAT_DISPARITY && st->flag & FLAT_SURFACES)
                    st->flag |= FRONT_ONLY;
                if(st->nchanges++ == 0){
                    mode |= STIMCHANGE_FRAME;
                    SerialSend(DISP_X);
                    if(fabsf(expt.vals[INITIAL_MOVEMENT]) > 0.01){
                        SerialSend(XPOS);
                        SerialSend(YPOS);
                    }
                    
                    if(dispchanged){
                        sprintf(buf,"Disparity Changed %.4lf to %.4f",pix2deg(2*idisp),pix2deg(2*st->disp));
                        glstatusline(buf,1);
                    }
                }
            }
        }
    }
    
    
    
    
    
    
    /* 
     * when backjump is set, the background disparity is modified during a trial
     * if the badkdisp is no zero it is set to zero after backjump frames. If it
     * is zero, is is set to a nonzero lvevel equal to the first value of the expt 
     * set. So if i & 1 backdisp should be not equal to expt.vals[DISP__BACK].
     * if not i&1, should be equal to expt.vals[BACK_DISP]. If st->next->disp
     * is not waht it shoudl be this is the first frame, so send a signal
     */
    if(isastim(st->next) && expt.vals[BACKJUMP] > 0 && stimstate == INSTIMULUS && realframecount > expt.vals[BACKJUMP])
    {
        i = realframecount/expt.vals[BACKJUMP];
        if(i & 1)
        {
            if(fabs(st->next->disp - deg2pix(expt.vals[DISP_BACK])/2) < 0.001)
                mode |= STIMCHANGE_FRAME;
            if(fabs(expt.vals[DISP_BACK]) < 0.0001)
            {
                stp = getexpval(0);
                st->next->disp = deg2pix(stp->vals[0]/2);
            }
            else
                st->next->disp = 0;
        }
        else if(fabs(st->next->disp - deg2pix(expt.vals[DISP_BACK])/2) > 0.001)
        {
            mode |= STIMCHANGE_FRAME;
            st->next->disp = deg2pix(expt.vals[DISP_BACK]/2);
        }
    }
    
    /*
     * N.B. dispincr takes priority over REVERSE_CORRELATE
     * so leaving RC set is not a problem
     */
	if(optionflag & CONTRAST_REVERSE_BIT)
	{
		pos->contrast_phase += st->incr;
		pos->contrast = pos->contrast_amp * cos(pos->contrast_phase);
        
        /*
         * Setting Contrast reverse AND Flash makes the contrast reversal a square wave in time
         */
		if(option2flag & FLASH_BIT){
            if(pos->contrast <= 0)
                pos->contrast = -pos->contrast_amp;
            else
                pos->contrast = pos->contrast_amp;
		}
		if (rdspair(st)){
            if(st->incr == 0){
                st->next->pos.contrast_phase += st->next->incr;
                st->next->pos.contrast = st->next->pos.contrast_amp * cos(st->next->pos.contrast_phase);
                if(option2flag & FLASH_BIT){
                    if(cos(st->next->pos.contrast_phase) > -0.5){
                        st->next->pos.contrast = 1;
                    }
                    else{
                        st->next->pos.contrast = 0;
                    }
                }
            }
            else{
                st->next->pos.contrast_phase = pos->contrast_phase;
                st->next->pos.contrast = pos->contrast;
            }
		}
		else if (st->type == STIM_RADIAL){
            pos->phase2 += st->left->ptr->incr;
            if(option2flag & FLASH_BIT)
                pos->contrast2 = pos->contrast_amp * sign(cos(pos->phase2));
            else
                pos->contrast2 = pos->contrast_amp * cos(pos->phase2);
            
		}
	}
	else if(option2flag & FLASH_BIT)
    {
	    if(optionflags[FAST_SEQUENCE]){ // don't do flashing here
            pos->contrast = pos->contrast_amp;
            if(st->next)
                st->next->pos.contrast = st->next->pos.contrast_amp;
            if(framecseq[realframecount+1])
                mode |= STIMCHANGE_FRAME;
	    }
	    else{
            pos->contrast_phase += st->incr;
            i = pos->contrast_phase/M_PI;
            if(i &1){
                pos->contrast = 0.0;
                expt.st->mode |= STIMULUS_NEEDS_CLEAR;
            }
            else
                pos->contrast = pos->contrast_amp;
	    }
    }
	else if(st->dispincr != 0)
	{
        st->disp += st->dispincr;
	}
	else if(optionflags[REVERSE_CORRELATE])
	{
        if(expt.mode == DISP_X)
            st->disp = deg2pix(frameseq[framectr])/2;
        else if(expt.mode == ORIENTATION)
            st->pos.angle = frameseq[framectr] * M_PI/180.0;
        framectr++;
	}
	else
	{
        /*
         * if FAST_SEQUENCE is set do this in the Expt loop, so that if Fr > 1, 
         * correlation only changes with other params
         */
        if (optionflags[RANDOM_CORRELATION] && !optionflags[FAST_SEQUENCE]){
            SetRandomCorrelation(st);
        }
        if (optionflags[RANDOM_CONTRAST]){
            SetRandomContrast(st);
        }
        else if (expt.type3 != RANDOM_CONTRAST_EXPT){
			pos->contrast = pos->contrast_amp;
            if(st->next)
                st->next->pos.contrast = st->next->pos.contrast_amp;
        }
		pos->phase += st->incr;
		if(optionflags[RANDOM_PHASE]){
            /* make sure these phases come from this seed so can be reconstructed*/
            myrnd_init(st->left->baseseed);
            SetRandomPhase(st, pos);
		}
		pos->locn[0] += st->posinc;
		if((st->type == STIM_BAR || st->type == STIM_TWOBAR) && !(st->mode & EXPTPENDING) &&
		   (option2flag & EXPT_INTERACTIVE))
            pos->phase = M_PI/2;
		else if (st->type == STIM_GRATING2 || st->type == STIM_GRATING)
        {
		    pos->phase2 += psine->incr;
		}
		else if (st->type == STIM_GRATINGN)
        {
		    if (st->left->ptr->velocity > 0){
                for(i = 0; i < st->nfreqs; i++)
                    st->phases[i] += st->left->incrs[i];
		    }
		    else{
                for(i = 0; i < st->nfreqs; i++)
                    st->phases[i] += st->incr;
		    }
        }
		else if (rdspair(st) && !optionflags[PAINT_BACKGROUND])
        {
		    rds = st->left;
		    rdsb = st->next->left;
		    st->next->pos.locn[0] += st->next->posinc;
        }
	}
	if(st->angleinc != 0)
    {
	    st->pos.angle += (st->angleinc * M_PI/(180 * mon.framerate));
    }
    
    
    
    /*
     * remember st->disp is half the disparity. It is added to one eye and
     * subtracted from the other
     */
    if(optionflags[MODULATE_DISPARITY] && fabs(st->depth_mod) > 0 && (st->type == STIM_GRATING || st->type == STIM_RDS) || st->type == STIM_IMAGE){
        st->modphase += (st->vals[MODULATION_F] *M_PI *2)/(mon.framerate);
        
        if(st->left->seedloop == 1)
            newseed = 0;
        if(optionflag & CONTRAST_REVERSE_BIT)
        {
            if(pos->contrast < 0){
                if(st->modphase > M_PI/2 && asmode == SEED_CHANGE_WITH_CONTRAST) // just changed sign
                    newseed = 1;
                st->modphase = 0;
            }
            else{
                if(st->modphase < M_PI/2 && asmode == SEED_CHANGE_WITH_CONTRAST) // just changed sign
                    newseed = 1;
                st->modphase = M_PI;
            }
            if(newseed){
                st->left->baseseed += 2;
            }
        }
        
        /*
         * gaussian onset, starting at -2sd, sd = 5 frames
         */
        if(optionflags[TEMPORAL_GAUSS]){
            if(st->framectr < 10)
                st->tenvelope = exp(-sqr((10-st->framectr))/50);
            else if((ival = st->nframes - st->framectr) < 10 && 
                    asmode != ASYMMETRICAL_ENVELOPE)
                st->tenvelope = exp(-sqr((10-ival))/50);
            else
                st->tenvelope = 1;
        }
        else
            st->tenvelope = 1;
        if(st->type == STIM_GRATING){
            if(optionflags[RANDOM_DEPTH_PHASE]){
                
                period = (int)rint(1000/mon.framerate);
                rphase = myrnd_i() % period;
                rphase = rphase * 2 * M_PI/period;
                st->phasedisp[0] = cos(rphase) * deg_rad(st->depth_mod);
            }
            else
                st->phasedisp[0] = expt.vals[DISP_P] + cos(st->modphase) * deg_rad(st->depth_mod);
        }
        else if(st->type == STIM_RDS){
            if(optionflags[MODULATE_DISPARITY] == 2){
                SetDotPlanes(st,st->vals[MODULATION_F], st->depth_mod * st->tenvelope);
                st->disp = st->meandisp;
            }
            else
                st->disp = expt.st->meandisp + st->tenvelope * deg2pix(cos(st->modphase) * st->depth_mod/2);
        }
        else if(st->type == STIM_IMAGE){
            st->disp = expt.st->meandisp + st->tenvelope * deg2pix(cos(st->modphase) * st->depth_mod/2);
        }
        
    }
    else if(optionflags[RANDOM_PHASE] && !isadotstim(st)){
	    st->left->baseseed += 2;
    }
    /*
     * only play with the fixation point if this call is for the foreground
     * stimulus, otherwise this gets done twice
     */
    if(fabs(expt.vals[PURSUIT_INCREMENT]) > 0.001 && st->prev == NULL){
        dx = sin(expt.vals[FP_MOVE_DIR]) * expt.vals[PURSUIT_INCREMENT] * pursuedir;
        dy = cos(expt.vals[FP_MOVE_DIR]) * expt.vals[PURSUIT_INCREMENT] * pursuedir;
        dx = sin(expt.vals[FP_MOVE_DIR]) * expt.vals[PURSUIT_INCREMENT];
        dy = cos(expt.vals[FP_MOVE_DIR]) * expt.vals[PURSUIT_INCREMENT];
        if(altstimmode != MOVE_STIM_ONLY){
            /*
             * need to send new fix pos so that monk is rewarded for correct position
             */
            fixpos[0] += (deg2pix(dx));
            fixpos[1] += (deg2pix(dy));
            SerialSend(FIXPOS_XY);
        }
        if(pursuedir > 0)
            pursued += fabsf(expt.vals[PURSUIT_INCREMENT]);
        TheStim->pos.xy[0] += (deg2pix(dx));
        TheStim->pos.xy[1] += (deg2pix(dy));
        
    }
    st->framectr++;
}


void PaintBackIm(PGM im)
{
    float z = 1.0,c;
    
    if(expt.vals[BACKGROUND_ZOOM] > 1)
        z = expt.vals[BACKGROUND_ZOOM];
    glPixelZoom(z,-z);
    
    if(expt.st->pos.contrast < 0.99){
        c = expt.st->pos.contrast;
        glPixelTransferf(GL_RED_BIAS,0.5-c/2);
        glPixelTransferf(GL_RED_SCALE,c);
        glPixelTransferf(GL_BLUE_BIAS,0.5-c/2);
        glPixelTransferf(GL_BLUE_SCALE,c);
        glPixelTransferf(GL_GREEN_BIAS,0.5-c/2);
        glPixelTransferf(GL_GREEN_SCALE, c);
        //    glPixelTransferf(GL_GREEN_SCALE, 1.0);
    }
    else{
        glPixelTransferf(GL_RED_BIAS,0);
        glPixelTransferf(GL_RED_SCALE,1);
        glPixelTransferf(GL_BLUE_BIAS,0);
        glPixelTransferf(GL_BLUE_SCALE,1);
        glPixelTransferf(GL_GREEN_BIAS,0);
        glPixelTransferf(GL_GREEN_SCALE, 1);
    }
    glRasterPos2f(-im.w/2,im.h/2);
  //  glRasterPos2i(-im.w * z/2,im.h * z/2);
    glDrawPixels(im.w, im.h, GL_LUMINANCE, GL_UNSIGNED_BYTE, im.ptr);
    
}
void wipescreen(float color)
{
    setmask(bothmask);
    if(expt.backim.name && optionflags[PAINT_BACKGROUND] && expt.stimmode == BUTTSEXPT){
        PaintBackIm(expt.backim);
    }
    else{
        if(optionflags[STIMULUS_IN_OVERLAY])
            glClearColor(color,color,color,color);
        else
            glClearColor(color,clearcolor,color,color);
        glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    }
    if(expt.vals[GRIDSIZE] > 0.1){
        if(optionflag & CONTRAST_REVERSE_BIT){
            expt.st->pos.contrast_phase += expt.st->incr;
            if(expt.st->pos.contrast_phase > M_PI)
                expt.st->pos.contrast_phase = -M_PI;
            SetGrey(1.0);
            if(expt.st->pos.contrast_phase > 0){
                setmask(LEFTMODE);
                grid(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]),0);
            }
            else{
                setmask(RIGHTMODE);
                grid(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]),1);
            }
        }
        else{
            SetGrey(1.0);
            setmask(LEFTMODE);
            grid(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]),0);
            if((int)(expt.vals[ALTERNATE_STIM_MODE]) == 1)
                SetGrey(0.0);
            
            setmask(RIGHTMODE);
            grid(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]),1);
        }
    }
    
}

void paint_frame(int type, int showfix)
{
    struct timeval atime,btime,ctime;
    float r = 0;
    
    gettimeofday(&atime, NULL);
    // mode |= NEED_REPAINT;
    if(!optionflags[CALCULATE_ONCE_ONLY])
        calc_stimulus(TheStim);
    gettimeofday(&calctime, NULL);
    setmask(ALLMODE);
    
    
    if(testflags[TEST_RC] && expt.st->type != STIM_IMAGE){
        
        if(expt.st->flag & UNCORRELATE)
            wipescreen(1.0);
        else
            wipescreen(0.0);
        return;
    }
    if(fixpaint > 0){
        fixpaint--;
        draw_fix(oldfixpos[0],oldfixpos[1], TheStim->fix.size, TheStim->background);
    }
    else
        draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->background);
    
    /*
     * Drawing the fixation marker above SHOULD be the first call to the graphics
     * pipeline since swapbuffers was called, so this is where the CPU blocks 
     * waiting for the vertical re-trace. Record the frame time here. 
     */
    gettimeofday(&paintframetime, NULL);
    swapwait = timediff(&paintframetime,&calctime);
    calcdur =  timediff(&calctime,&atime);
    
    /* if the middle button is down, dont paint - quick check for stim  effect */
    
    setmask(BOTHMODE);
    if(debug == 3)
        glDrawBuffer(GL_FRONT_AND_BACK);
    if (TheStim->noclear == 0)
        clearstim(TheStim,TheStim->gammaback, 0);
    TheStim->noclear = 1;
    if(SACCREQD(afc_s) && afc_s.target_in_trial > 0){
        paint_target(expt.targetcolor, 0);
    }
    if(option2flag & PSYCHOPHYSICS_BIT || !(eventstate & MBUTTON) || (eventstate & CNTLKEY)){
        if(type == STIM_BACKGROUND && isastim(TheStim->next))
            paint_stimulus(TheStim->next);
        else{
            if (optionflags[PAINT_BACKGROUND] && expt.backim.ptr != NULL)
                TheStim->noclear = 0;
            paint_stimulus(TheStim);
        }
    }
    else
        wipescreen(clearcolor);
    setmask(ALLMODE);
    if(showfix)
        draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
    gettimeofday(&btime, NULL);
    draw_conjpos(cmarker_size,PLOT_COLOR);
    if (optionflags[FEEDBACK] && expt.vals[SACCADE_AMPLITUDE] > 0){
        setmask(OVERLAY);
        DrawBox(fixpos[0]+afc_s.sacval[0],fixpos[1]+afc_s.sacval[1],2,2,RF_COLOR);
        r = fabs(afc_s.sacval[0])+fabs(afc_s.sacval[1]);
        DrawLine(fixpos[0]+r-0.5,fixpos[1]+r,fixpos[0]+r+0.5,fixpos[1]+r,RF_COLOR);
        if (afc_s.jstairval> 0)
        DrawLine(fixpos[0]+r,fixpos[1]+r-0.5,fixpos[0]+r,fixpos[1]+r+0.5,RF_COLOR);
    }
    if(debug)
        
        glFlushRenderAPPLE();
    paintdur = timediff(&btime,&paintframetime);
    if(optionflags[STIMULUS_IN_OVERLAY])
    {
        if(optionflags[SPLITSCREEN])
        {
            glPushMatrix();
            glTranslatef(psychoff[0]/2,0,0);
            ShowBox(expt.rf,0.0);
            glTranslatef(-psychoff[0],psychoff[1],0);
            ShowBox(expt.rf,0.0);
            glPopMatrix();
        }
        else
            ShowBox(expt.rf,RF_COLOR);
    }
    
}

int CheckFix()
{
    int oldstate = stimstate;
    int i = (optionflag & FIXATION_CHECK);
    
    if((optionflag & FIXATION_CHECK) && fixstate == BADFIX_STATE)
	{
        TrialOver();
        start_timeout(BAD_FIXATION);
        return(-1);
	}
    return(0);
    
}

int RunBetweenTrials(Stimulus *st, Locator *pos)
{
    if(!(optionflag & STIM_IN_WURTZ_BIT)){
        if(expt.st->type == STIM_IMAGE && expt.st->preload && expt.st->preloaded)
            expt.st->framectr = myrnd_i() % expt.st->nframes;
        paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
        increment_stimulus(st, pos);
        loopframes++;
        return(1);
    }
    else
        return(0);
}

/*
 * StartTrial() is called at the beginning of a new fixation trial
 */
int StartTrial()
{
    static int lastnudge = 1;
    int rnd;
    
    /*
     *  special fudge to allow stimuli to be changed during every trial,
     * but not necessarily start every trial with the same stim.
     * becuase stimno is always adjusted in opposite directions, the total
     * count doesn't get more than one off.
     */
    
    if(rcfd){
 //       fprintf(rcfd,"Trial %d\n",ufftime(&now));
    }
    
    
    SerialSignal(GOOD_FIXATION);
    if(optionflags[ALWAYS_CHANGE_STIM] && optionflag & SEARCH_MODE_BIT){
        rnd = myrnd_i();
        if(rnd & 1 && stimno > 2){
            lastnudge = ! lastnudge;
            stimno += ((lastnudge - 0.5) * 2);  // + or - 1
        }
    }
    return(0);
}

/*
 * StartStimulus() is called at the beginning of each stimulus in a
 * trial
 */

int StartStimulus(Stimulus *st)
{
    if((option2flag & EXPT_INTERACTIVE))
        SetStimulus(st,90.0,SETPHASE,NOEVENT);
    else
        SetStimulus(st,st->vals[START_PHASE],SETPHASE,NOEVENT);
    if(st->type == STIM_RADIAL){
        if(expt.vals[CHANGE_SEED] > 0)
            st->right->show_increment = st->left->show_increment = 0;
        else
            st->right->show_increment = st->left->show_increment = 1;
    }
    return(0);
}

void ShowInfo()
{
    vcoord x[2];
    Thisstim *stp;
    char buf[BUFSIZ];
    
    if(optionflag & SEARCH_MODE_BIT && optionflags[FEEDBACK]){
        stp = getexpval(expt.stimno);
        if(stp->vals[EXP_PSYCHVAL] < 0)
            sprintf(buf,"Left (%.3f)",stp->vals[EXP_PSYCHVAL]);
        else
            sprintf(buf,"Right (%.3f)",stp->vals[EXP_PSYCHVAL]);
        statusline(buf);
    }
}

void testcolor()
{
    float x[2],color;
    setmask(ALLPLANES);
    glDrawBuffer(GL_FRONT_AND_BACK);
    glClearColor(expt.st->gammaback, expt.st->gammaback, expt.st->gammaback, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    
    for(color = 0; color <= 1; color += 0.05)
    {
        SetColor(color, 1);
        x[0] = color * 40;
        x[1] = 100;
        myvx(x);
        x[0] = color * 40;
        x[1] = 300;
        myvx(x);
    }
    glEnd();
       glDrawBuffer(GL_BACK);
}


float SetFixColor(Expt expt)
{
    
    if (optionflags[SHOW_REWARD_BIAS])
      	optionflag |= (SQUARE_FIXATION);
    if (optionflags[SHOW_REWARD_BIAS] && expt.biasedreward < 0){
        expt.st->fixcolor = expt.st->fix.offcolor;
    }
    else if (optionflags[SHOW_REWARD_BIAS] && expt.biasedreward == 0){
        expt.st->fixcolor = expt.st->fix.fixcolors[0];
        optionflag &= (~SQUARE_FIXATION);
    }
    else if (stimstate == INTERTRIAL)
        expt.st->fixcolor = expt.st->fixcolor;
    else
        expt.st->fixcolor = expt.st->fix.fixcolor;
}

int next_frame(Stimulus *st)
{
    
    static float lastval=0;
    static int laststate,fctr = 0;
    float val=0,t2;
    Locator *pos = &st->pos;
    int i,oldstimstate = stimstate;
    vcoord x[2];
    static int stimctr = 0;
    static int *confirmer_state = NULL;
    char buf[BUFSIZ];
    static int waitcount = 0;
    Thisstim *stp,*istp;
    char c;
    int len = 100;
    double o, drand48(),duration = 0,dx,dy = 0;
    static int rc = 0, lastframecount = 0, warned = 0;
    time_t t;
    struct tm *ltime;
    static double lasto = 0,lastt = 0;
    char exptchr = ' ';
    int nf;
    
    
    gettimeofday(&now,NULL);
    t2 = timediff(&now,&lastcalltime);
    memcpy(&lastcalltime,&now,sizeof(struct timeval));
    /* some things need checking whatever the weather */
    if(stimno == NEW_EXPT)
        InitExpt();
    if(abs(expt.st->sf_disp) > 0.01)
	    markercolor = 1.0;
    
    if (ExptIsRunning())
        exptchr = '@';
    else
        exptchr = ' ';
    
    markercolor = 1.0;
    glstatusline(NULL,1);
#ifdef MONITOR_CLOSE
    if(seroutfile && laststate != stimstate){
        fprintf(seroutfile,"#State %d %d VS%.1f%c\n",stimstate,fixstate,afc_s.sacval[1],exptchr);
        fflush(seroutfile);
    }
#endif
    t2 = timediff(&now,&endstimtime);
    if(t2 > 3600 && !(option2flag & PSYCHOPHYSICS_BIT) && check_for_monkey) /* 1hr sec gone with no trial */
    {
        time(&t);
        ltime = localtime(&t);
        if(ltime->tm_hour > 19){
            printf("Warning - no stimuli completed for 1hour  at %2d:%2d\n",ltime->tm_hour,ltime->tm_min);
            system("/bgc/bgc/etc/monkeywarn `hostname`");
            memcpy(&endstimtime,&now,sizeof(struct timeval));
        }
        
    }
    if(timeout_type == SHAKE_TIMEOUT)
        start_timeout(SHAKE_TIMEOUT);
    if (stimno > 1 && !ExptIsRunning()){
        fprintf(seroutfile,"#Not in Expt S%d\n",stimstate);
        i = TheStim->mode & EXPTPENDING;
        i = states[EXPT_PAUSED];
        i = ExptIsRunning();
    }
    switch(stimstate)
    {
        case STIMSTOPPED:
#ifdef NIDAQ
            DIOval = 0;
            DIOWriteBit(2,  0);
            DIOWriteBit(1,  0);
            DIOWriteBit(0,  0);

#endif
            if(rdspair(expt.st))
                i = 0;
            TheStim->fixcolor = expt.vals[BLANKCOLOR_CODE];
            markercolor = 1.0;
            if(TheStim->mode & EXPTPENDING && mode & ANIMATE_BIT)
                stimstate = INTERTRIAL;
            if(timeout_type == SHAKE_TIMEOUT_PART2){
                ShowTime();
                if((val = timediff(&now, &starttimeout)) > expt.vals[SHAKE_TIMEOUT_DURATION]){
                    SetStimulus(expt.st,0.5, SETBACKCOLOR,NULL);
                    SetStimulus(expt.st,1.0, BLANKCOLOR_CODE,NULL);
                    glDrawBuffer(GL_FRONT_AND_BACK);
                    setmask(ALLMODE);
                    search_background();
                    end_timeout();
                       glDrawBuffer(GL_BACK);
                }
            }
            if(timeout_type == SHAKE_TIMEOUT_PART1)
                timeout_type = SHAKE_TIMEOUT_PART2;
            if(expt.vals[GRIDSIZE] > 20){
                setmask(ALLMODE);
                chessboard((int)(expt.vals[GRIDSIZE]), (int)(expt.vals[GRIDSIZE]));
            }
            else if(expt.vals[GRIDSIZE] > 0.1){
                if(optionflag & CONTRAST_REVERSE_BIT){
                    expt.st->pos.contrast_phase += expt.st->incr;
                    i = (int)(expt.st->pos.contrast_phase) % 21;
                    expt.vals[TIMEOUT_CONTRAST] = ((float)(i)/10.0)-1;
                    if((int)(expt.vals[ALTERNATE_STIM_MODE]) == QUICK_CAL){
                        i = (int)(expt.st->pos.contrast_phase) % 5;
                        expt.vals[TIMEOUT_CONTRAST] = ((float)(i)/2)-1;
                    }
                }
                setmask(ALLMODE);
                chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                if((int)(expt.vals[ALTERNATE_STIM_MODE]) == CROSS_TALK){	
                    glClearColor(0,0,0,0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    if(optionflag & CONTRAST_REVERSE_BIT)
                        i = (int)(expt.st->pos.contrast_phase) % 8;
                    else
                        i = (int)(expt.st->vals[START_PHASE]) % 8;
                    switch(i){
                        case 0:
                            expt.vals[TIMEOUT_CONTRAST] = 1;
                            setmask(ALLMODE);
                            chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                            break;
                        case 1:
                        case 3:
                        case 5:
                        case 7:
                            expt.vals[TIMEOUT_CONTRAST] = 0;
                            setmask(ALLMODE);
                            chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                            break;
                        case 4:
                            expt.vals[TIMEOUT_CONTRAST] = -1;
                            setmask(ALLMODE);
                            chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                            break;
                        case 2:
                            expt.vals[TIMEOUT_CONTRAST] = -1;
                            setmask(LEFTMODE);
                            chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                            expt.vals[TIMEOUT_CONTRAST] = 1;
                            setmask(RIGHTMODE);
                            chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                            break;
                        case 6:
                            expt.vals[TIMEOUT_CONTRAST] = 1;
                            setmask(LEFTMODE);
                            chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                            expt.vals[TIMEOUT_CONTRAST] = -1;
                            setmask(RIGHTMODE);
                            chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                            break;
                    }
                }
                else{
                    setmask(FORCEBOTH);
                    glClearColor(0,0,0,0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    setmask(BOTHMODE);
                    chessboard(deg2pix(expt.vals[GRIDSIZE]), deg2pix(expt.vals[GRIDSIZE]));
                }
            }
            else
                search_background();
        draw_conjpos(cmarker_size,PLOT_COLOR);
            glSwapAPPLE();
            gettimeofday(&now,NULL);
            if ((optionflag & SHOW_CONJUG_BIT) && (val = timediff(&now,&lastsertime)) > 2){
                lastsertime = now;
                SerialSend(BW_IS_READY);
            }

            if(testflags[PLAYING_EXPT]){
                ReplayExpt("Show");
            }
            ReadCommandFile(expt.cmdinfile);
            break;
        case INTERTRIAL:
#ifdef NIDAQ
            DIOWriteBit(2,  0);
            DIOWriteBit(1,  0);
            DIOWriteBit(0,  0);
//            DIOval = 0; DIOWrite(0);
#endif
            newtimeout = 1;
            if(rdspair(expt.st))
                i = 0;
            if(debug) glstatusline("InterTrial",3);
            if(TheStim->mode & EXPT_OVER)
            {
                expt_over(1);
                if (demomode == 0)
                    stimstate = STIMSTOPPED;
                if(testflags[PLAYING_EXPT]){
                    ReplayExpt(NULL);
                }
                break;
            }
            if(testflags[PLAYING_EXPT]){
                ReplayExpt(NULL);
            }
            
            TheStim->fixcolor = TheStim->fix.offcolor;
            if(rdspair(expt.st))
                i = 0;
            if(option2flag & PSYCHOPHYSICS_BIT){ /* wait for button press */
                ShowInfo();
            }
            else if((val = timediff(&now, &endtrialtime)) > expt.isi && 
                    (demomode == 0 || (TheStim->mode & EXPTPENDING)))
            {
                stimstate=PREFIXATION;
                break;
            }
            //      printf("%.3f\n",val-lastval);
            if(rdspair(expt.st))
                i = 0;
            setmask(OVERLAY);
            redraw_overlay(expt.plot);
            if(rdspair(expt.st))
                i = 0;
            if(ExptIsRunning() && (i = PrepareExptStim(1,12)) < 0){
                /*
                 * only put up the warning once
                 */

                    sprintf(buf,"Error Setting Stimulus %d %d",stimno,stimorder[stimno]);
                    if(seroutfile)
                        fprintf(seroutfile,"%s\n",buf);
   
                stimstate = INTERTRIAL;
            }
            
            SetFixColor(expt);
            
            
            if(rdspair(expt.st))
                i = 0;
            setmask(bothmask);
            wipescreen(clearcolor);
            if(rdspair(expt.st))
                i = 0;
            if(!(optionflag & STIM_IN_WURTZ_BIT)){
                paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
                increment_stimulus(st, pos);
            }
            else
                draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
            if (SACCREQD(afc_s) && val < expt.vals[CHOICE_TARGET_DURATION] && monkeypress == WURTZ_OK)
                paint_target(expt.targetcolor,2);
            if(rdspair(expt.st))
                i = 0;
            change_frame();
            //	glFinishRenderAPPLE();
            if(rdspair(expt.st))
                i = 0;
            break;
        case PREFIXATION:
            newtimeout = 1;
            if(rdspair(expt.st))
                i = 0;
            sprintf(buf,"Prefix %d %d",(optionflag & FIXATION_CHECK), fixstate);
            if(debug) glstatusline("Prefix",3);
            if(!ExptIsRunning())
                stimctr = 0;
            if(seroutfile && optionflags[DEBUG_OUTPUT])
                fprintf(seroutfile,"#Trial at %u (%d)%c\n",ufftime(&now),fixstate,exptchr);
            if(testflags[PLAYING_EXPT])
                fixstate = GOOD_FIXATION;
            if (optionflags[SHOW_REWARD_BIAS])
                optionflag |= (SQUARE_FIXATION);
            if (optionflags[SHOW_REWARD_BIAS] && expt.biasedreward < 0){
                TheStim->fixcolor = TheStim->fix.offcolor;
            }
            else if (optionflags[SHOW_REWARD_BIAS] && expt.biasedreward == 0){
                TheStim->fixcolor = TheStim->fix.fixcolors[0];
                optionflag &= (~SQUARE_FIXATION);
            }
            else
                TheStim->fixcolor = TheStim->fix.fixcolor;
            pursued = 0;
            clearcolor = TheStim->gammaback;
            if(laststate == INTERTRIAL) /* first call */
            {
                memcpy(&fixontime, &now, sizeof(struct timeval));
                if(ExptIsRunning() && (i = PrepareExptStim(1,13)) < 0){

                        printf("Prepare returns %d\n",i);

                    stimstate = INTERTRIAL;
                }
                SetFixColor(expt);
                if(demomode == 0)
                    fixstate = 0;
                mode |= WURTZ_FRAME_BIT;
            }
            
            if(!(optionflag & FIXATION_CHECK) || fixstate == GOOD_FIXATION || demomode)
            {
                expstate = 0;
                stimstate = PRESTIMULUS;
                StartTrial();
                memcpy(&goodfixtime, &now, sizeof(struct timeval));
                break;
            }
            else if((val = timediff(&now, &fixontime)) > 1.0)
            {
                memcpy(&endtrialtime, &now, sizeof(struct timeval));
                stimstate = INTERTRIAL;
                /*
                 * will get here when monkey is not looking. During initial training this
                 * "arms" the reward, so monkey will get on immediately after preperiod of first trial
                 */
                if(optionflags[INITIAL_TRAINING])
                    optionflags[INITIAL_TRAINING] = 2;
                
            }
            
            wipescreen(clearcolor);
            RunBetweenTrials(st, pos);
            draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
            val = timediff(&now, &endtrialtime);
            if (SACCREQD(afc_s) && val < expt.vals[CHOICE_TARGET_DURATION] && monkeypress == WURTZ_OK)
                paint_target(expt.targetcolor,2);
            change_frame();
            break;
        case PRESTIMULUS:
            //Ali CheckKeyboard(D, allframe);
            mode &= (~FIXATION_OFF_BIT);
            microsaccade = 0;
            if(rdspair(expt.st))
                expt.framesdone = 0;
            expt.framesdone = 0;
            expt.stimvals[FIXPOS_X] = expt.vals[FIXPOS_X];
            expt.stimvals[FIXPOS_Y] = expt.vals[FIXPOS_Y];
            if (!ExptIsRunning()){
                if(expt.st->type == STIM_RDS)
                    precalc_rds_disps(expt.st);
            }
            else if (expt.st->type == STIM_CYLINDER){
                if(expt.st->left->ptr->velocity < 0.0001 && seroutfile)
                    fprintf(seroutfile,"#Cyl velocity %.6f (%.f)%c\n",expt.st->left->ptr->velocity,oldvelocity,exptchr);
            }
            
            if(debug) glstatusline("PreStim",3);
            clearcolor = TheStim->gammaback;
            if (optionflags[SHOW_REWARD_BIAS])
                optionflag |= (SQUARE_FIXATION);
            if (optionflags[SHOW_REWARD_BIAS] && expt.biasedreward < 0){
                TheStim->fixcolor = TheStim->fix.offcolor;
            }
            else if (optionflags[SHOW_REWARD_BIAS] && expt.biasedreward == 0){
                TheStim->fixcolor = TheStim->fix.fixcolors[0];
                optionflag &= (~SQUARE_FIXATION);
            }
            else
                TheStim->fixcolor = TheStim->fix.fixcolor;
            if(CheckFix() < 0)
                break;
            //Ali  CheckKeyboard(D, allframe);
            if(laststate != PRESTIMULUS){
                mode |= STIM_FRAME_BIT; /* send trial signal at next frame */
#ifdef MONITOR_CLOSE
                if(seroutfile)
                    fprintf(seroutfile,"#Presstim VS%.1f %.3f%c\n",afc_s.sacval[1],timediff(&now,&goodfixtime),exptchr);
#endif
            }
            CheckFix();
            if(SACCREQD(afc_s) && afc_s.target_in_trial > 0){
                paint_target(expt.targetcolor, 0);
            }
            //Ali CheckKeyboard(D, allframe);
            if((val = timediff(&now, &goodfixtime)) > expt.preperiod &&
               val > expt.vals[TRIAL_START_BLANK])
            {
                redraw_overlay(expt.plot);
                //Ali CheckKeyboard(D, allframe);
                if(optionflags[INITIAL_TRAINING] == 2){
                    SerialSignal(FREE_REWARD);
                    optionflags[INITIAL_TRAINING] = 1;
                    /*
                     * give a reward and wait some more, so that interval to next reward
                     * is same as subsequent waits..
                     */
                    memcpy(&goodfixtime, &now, sizeof(struct timeval));
                    break;
                }
                //Ali CheckKeyboard(D, allframe);
                if (ExptIsRunning() && (mode & ANIMATE_BIT) )
                {
                    inexptstim = 1;
                    if(optionflags[RUN_SEQUENCE] && expt.stimpertrial > 2){
                        framesdone = RunExptStimSeq(TheStim, TheStim->nframes, expt.stimpertrial,D);
                        stimstate = POSTSTIMULUS;
                    }
                    else{
                        //Ali CheckKeyboard(D, allframe);
                        framesdone = RunExptStim(TheStim, TheStim->nframes, D, -1);
#ifdef MONITOR_CLOSE
                        if(seroutfile)
                            fprintf(seroutfile,"#Over%d: %d %d%c\n",framesdone,fixstate, stimctr,exptchr);
#endif
                        inexptstim = 0;
                        if(debug == 4)
                            testcolor();
                        if(fixstate == GOOD_FIXATION && ++stimctr < expt.stimpertrial){
                            oldstimstate = stimstate;
                            ResetExpStim(1);
                            stimstate = PRESTIMULUS;
                            if(expt.isi > 0)
                                stimstate = POSTSTIMINTRIAL;
                            else
                                stimstate = PRESTIMULUS;
                            /*
                             * This is where stimno is incremented when there are > 1 stim per trial and
                             * we are still in a trial
                             */
                            if(!freezeexpt)
                                stimno++;
                            /*
                             * need to get spikes back from BW before preparing next expt stim
                             */
                            if(seroutfile){
                                fprintf(seroutfile,"#Pre %d%c\n",stimno,exptchr);
#ifdef MONITOR_CLOSE
                                fprintf(seroutfile,"#Spikes %d\n",gotspikes);
                                fflush(seroutfile);
#endif
                            }
                            if(optionflag & WAIT_FOR_BW_BIT){
                                gettimeofday(&timeb,NULL);
                                val = 0;
                                while((c = ReadSerial(ttys[0])) != MYEOF)
                                    GotChar(c);
                                gettimeofday(&now,NULL);
                                val = timediff(&now,&timeb);
#ifdef MONITOR_CLOSE
                                if(seroutfile){
                                    fprintf(seroutfile,"#Done%c\n",exptchr);
                                    fflush(seroutfile);
                                }
#endif
                            }
                            if((i = PrepareExptStim(1,14)) < 0)
                                stimstate = INTERTRIAL;
                            else{
                                memcpy(&goodfixtime, &now, sizeof(struct timeval));
                                gotspikes = 0;
                            }
                        }
                        else{
                            if(testflags[PLAYING_EXPT]){
                                ResetExpStim(1);
                                PrepareExptStim(1,15);
                            }
                            stimctr = 0;
                        }
                    }
                    loopframes = 0;
                    if(option2flag & PSYCHOPHYSICS_BIT && stimstate == POSTSTIMULUS){
                        ShowTrialCount(0.0, wsum);
                        stimstate = WAIT_FOR_RESPONSE;
                        if(isharris(altstimmode)){
                            o = drand48();
                            endpt[0] = winsiz[0] + len * sin(o * M_PI * 2);
                            endpt[1] = winsiz[1] + len * cos(o * M_PI * 2);
                            
                            VisLine(0,0,endpt[0],endpt[1],1.0);
                        }
                    }
                    if(!(option2flag & PSYCHOPHYSICS_BIT) && (optionflag & FIXATION_CHECK) && (mimic_fixation & MIMIC_FIXATION)){
                        fixstate = WAIT_FOR_MOUSE;
                        mimic_fixation &= (~MIMIC_FIXATION);
                    }
                    
                    if(!(option2flag & PSYCHOPHYSICS_BIT) && !(optionflag & FIXATION_CHECK)){
                        fixstate = RESPONDED;
                        stimstate = WAIT_FOR_RESPONSE;
                        
                        gettimeofday(&endtrialtime, NULL);
                    }
                } // end ExptItsRunning
                else if(mode & ANIMATE_BIT)
                {
                    fprintf(stderr,"Not in Expt %d %d\n",TheStim->mode & EXPTPENDING,states[EXPT_PAUSED]);
                    drag = 0;
                    mode |= NEED_REPAINT; /* make sure background done */
                    expt.st->framectr = 0;
                    StartStimulus(TheStim);
                    paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
                    mode |= FIRST_FRAME_BIT;
                    change_frame();
                    stimstate = INSTIMULUS;
                    rc = 0;
                    lastframecount = 0;
                    break;
                }
                else if(testflags[PLAYING_EXPT]){
                    ReplayExpt(NULL);
                }
            }
            else if(optionflag & BACKGROUND_IN_PREPERIOD)
            {
                paint_frame(STIM_BACKGROUND, !(mode & FIXATION_OFF_BIT));
                change_frame();
            }
            else
            {	
                if(option2flag & PSYCHOPHYSICS_BIT && val < expt.vals[TRIAL_START_BLANK]){
                    setmask(bothmask);
                    clearstim(TheStim,clearcolor, optionflag & DRAW_FIX_BIT);
                    draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fix.offcolor);
                }
                else{
                    wipescreen(clearcolor);
                    if(!RunBetweenTrials(st, pos))
                        draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
                }
                change_frame();
                glFinishRenderAPPLE();
            }

            break;
        case POSTSTIMINTRIAL:
            if(rdspair(expt.st))
                i = 0;
            if(debug) glstatusline("Poststim",3);
#ifdef MONITOR_CLOSE
            if(seroutfile && laststate != stimstate)
                fprintf(seroutfile,"#Poststimintrial%c\n",exptchr);
#endif
            CheckFix();
            if(testflags[PLAYING_EXPT]){
                ReplayExpt(NULL);
            }
            if(fixstate == BADFIX_STATE){
                stimctr = 0;
                stimstate = POSTTRIAL;
            }
            else if((val = timediff(&now, &endstimtime)) > expt.postperiod)
                stimstate = PRESTIMULUS;
            if (laststate != POSTSTIMINTRIAL){ // first call
                val = timediff(&now, &endstimtime);
                val = timediff(&now, &timeb);
                val = timediff(&now, &nftime);
                if (val > 0.02){
                    fprintf(stderr,"ISI delay %.3f\n",val);
                    if(seroutfile)
                        fprintf(seroutfile,"#ISI delay %.3f\n",val);
                }
            }
            memcpy(&goodfixtime, &now, sizeof(struct timeval));
            break;
        case INSTIMULUS:
            if(rdspair(expt.st))
                i = 0;
            if(debug) glstatusline("InStim",3);
            if(CheckFix() < 0){
                TrialOver();
                break;
            }
            
            
            paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
            increment_stimulus(st, pos);
            lastframecount = rc;
            nf = expt.st->framerepeat;
            /* 
             *on the mac when the focus is in the display win, vertical blank is ignored
             *this makes a mess of interacting with the stimulus. So put in a forced delay
             *here
             */
            
            if(nf < 1)
                nf = 1;
            
            while((rc = getframecount()) < lastframecount +nf)
                rc = getframecount();
            change_frame();
            expt.framesdone++;
            if(mode & NEWDRAG)
            {
                CheckRect(TheStim);
                mode &= (~NEWDRAG);
            }
            framesdone++;
            if(optionflags[FIXNUM_PAINTED_FRAMES] && framesdone >= TheStim->nframes ||
               (!optionflags[FIXNUM_PAINTED_FRAMES] && (realframecount = getframecount()) >= TheStim->nframes))
            {
                memcpy(&endstimtime, &now, sizeof(struct timeval));
                stimstate = POSTSTIMULUS;
                mode |= LAST_FRAME_BIT;
            }
            else if(realframecount/mon.framerate > expt.vals[FIXATION_OVERLAP])
            {
                TheStim->fixcolor = TheStim->gammaback;
                mode |= FIXATION_OFF_BIT;
            }
            break;
        case POSTSTIMULUS:
            if(rdspair(expt.st))
                i = 0;
            if(debug) glstatusline("Poststim",3);
#ifdef MONITOR_CLOSE
            if(seroutfile){
                fprintf(seroutfile,"#Poststim VS%.1f (%.3f)%c\n",afc_s.sacval[1],timediff(&now,&goodfixtime),exptchr);
                fflush(seroutfile);
            }
#endif
            mode &= (~FIXATION_OFF_BIT);
            if (!option2flag &AFC) // don't mess with color before response
                TheStim->fixcolor = TheStim->fix.fixcolor;
            if(CheckFix() < 0)
                break;
            if((val = timediff(&now, &endstimtime)) > expt.postperiod)
            {
                if(!(TheStim->mode & EXPTPENDING && !states[EXPT_PAUSED]) && ++stimctr < expt.stimpertrial)
                    stimstate = PRESTIMULUS;
                else{
                    stimstate = POSTPOSTSTIMULUS;
                    if (!option2flag &AFC) // don't mess with color before response
                        TheStim->fixcolor = TheStim->fix.offcolor;
                    if(!(TheStim->mode & EXPTPENDING))
                        stimctr = 0;
                }
                break;
            }
            setmask(bothmask);
            glClearColor(clearcolor,clearcolor,clearcolor,clearcolor);
            glClear(GL_COLOR_BUFFER_BIT);
            RunBetweenTrials(st, pos);
            if(expt.vals[FIXATION_OVERLAP] > 10)
                draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
            if(SACCREQD(afc_s) && afc_s.target_in_trial > 0){
                paint_target(expt.targetcolor, 0);
            }

            change_frame();
            if(testflags[PLAYING_EXPT]){
                if((i = ReplayExpt(NULL)) == INTERTRIAL)
                    stimstate = i;
            }
            break;
        case POSTPOSTSTIMULUS: /* after postperiod clear both buffers with overlay */
            if(rdspair(expt.st))
                i = 0;
            setmask(ALLMODE);
            if(debug) glstatusline("PostPostStim",3);
            gettimeofday(&now, NULL);
            okdelay = timediff(&now,&endstimtime);
#ifdef MONITOR_CLOSE
            if(seroutfile)
                fprintf(seroutfile,"#PostPoststim%.3f%c\n",okdelay,exptchr);
#endif
            if (expt.mode != BACKGROUND_IMAGE){
                glClearColor(clearcolor,clearcolor,clearcolor,clearcolor);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            RunBetweenTrials(st, pos);
            if(expt.vals[FIXATION_OVERLAP] > 10)
                draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
            else
                draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->gammaback);
            setmask(OVERLAY);
            SetGrey(1.0);
            redraw_overlay(expt.plot);
            change_frame();
            if(laststate == POSTSTIMULUS)
                gettimeofday(&endtrialtime, NULL);
            setmask(ALLMODE);
            glClearColor(clearcolor,clearcolor,clearcolor,clearcolor);
            glClear(GL_COLOR_BUFFER_BIT);
            RunBetweenTrials(st, pos);
            if(expt.vals[FIXATION_OVERLAP] > 10)
                draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
            else
                draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->gammaback);
            setmask(OVERLAY);
            redraw_overlay(expt.plot);
            
            if (expt.vals[CHOICE_TARGET_DURATION])
                paint_target(expt.targetcolor,2);
            
            change_frame();
            SerialSignal(WURTZ_OK);
            CheckStimDuration(stimstate);
            glDrawBuffer(GL_BACK);
            glFinishRenderAPPLE();
            stimstate = WAIT_FOR_RESPONSE;

            break;
        case WAIT_FOR_RESPONSE:
            if(rdspair(expt.st))
                i = 0;
            if(laststate != stimstate)
                waitcount = 0;
            val = timediff(&now, &endtrialtime);
#ifdef MONITOR_CLOSE
            if(seroutfile)
                fprintf(seroutfile,"#WaitFor %.2f VS%.1f%c\n",val,afc_s.sacval[1],exptchr);
#endif
#if defined(WIN32) 
            if(waitcount > 2 && option2flag & PSYCHOPHYSICS_BIT)
                break;
#endif
            if(debug){
                sprintf(buf,"Wait %.4f",val);
                printf("%s\n",buf);
                glstatusline(buf,3);
            }
            if(freezestimulus)
                return(framecount);
            wipescreen(clearcolor);
            if (fixstate == RESPONDED)
                val = timediff(&now, &endtrialtime);
            
            RunBetweenTrials(st, pos);
            if(option2flag & PSYCHOPHYSICS_BIT || fixstate == WAIT_FOR_MOUSE){
                TheStim->fixcolor = TheStim->fix.fixcolor;
            }
            else if(mimic_bias) //always respond one way
            {
                stp = getexpval(expt.stimno);
                
                if(stp->vals[EXP_PSYCHVAL] < 0)
                    GotChar(WURTZ_OK_W);
                else
                    GotChar(WURTZ_OK);
                
            }
            else if(mimic_fixation && (o = mydrand()) < mimicbadfix && stimno > 0
                    && waitcount == 0 && o != lasto){
                lasto = 0;
                if (dorpt && dorpt == lastrpt){
                    printf("ob stuck at =%.3f\n",o);
                }
                else{
                    if(option2flag & AFC)
                        stimno++; // will be decremented in GotChar
                    GotChar(BAD_FIXATION);
                    printf("ob=%.3f\n",o);
                }
                lastrpt = dorpt;
            }
            else if(mimic_fixation && val > TheStim->fix.rt-0.02){
                if(SACCREQD(afc_s)){
                    printf("og=%.3f\n",o);
                    if((o = drand48()) < expt.spikegain)
                        GotChar(WURTZ_OK);
                    else{
                        GotChar(WURTZ_OK_W);
                        monkeypress = WURTZ_OK_W;
                    }
                }
                else 
                    GotChar(WURTZ_OK);
            }
            else if(demomode)
                ;  // in demo, hit F3 again to remove targets
            else if(val > TheStim->fix.rt)
            {
                if(seroutfile)
                    fprintf(seroutfile,"#LATE (%.3f) stimno %d%c\n",okdelay,stimno,exptchr);
                stimstate = POSTTRIAL;
                fctr = 0;
                TheStim->fixcolor = TheStim->fix.offcolor;
                memcpy(&endtrialtime,&now,sizeof(struct timeval));
                afc_s.lasttrial = BAD_TRIAL;
                SerialSignal(WURTZ_LATE);
                fixstate = WURTZ_LATE;
                SerialSignal(END_TRIAL);
                fixstate = RESPONDED;
                notify("TRES L\n");
                change_frame();
                oldstimpos[0] = TheStim->pos.xy[0];
                oldstimpos[1] = TheStim->pos.xy[1];
                if (stimno >=0) { // dont do this if not in expt
                    ResetExpStim(1); //before stimno is incremented
                    ShuffleStimulus(WURTZ_LATE);
                }
            }
#if defined(Linux) || defined(WIN32)
            else
                GotChar(WURTZ_OK);
#endif
            if(isharris(altstimmode)){
                VisLine(0,0,endpt[0],endpt[1],1.0);
            }
            if(SACCREQD(afc_s) && fixstate != RESPONDED && stimstate != POSTTRIAL){
                paint_target(expt.targetcolor, 0);
            }
            else if(expt.vals[FIXATION_OVERLAP] > 10){
                if(fabsf(TheStim->fixcolor-TheStim->background) < 0.1)
                    draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fix.offcolor);
                else
                    draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
            }
            else
                draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->gammaback);
            
            if(fixstate == RESPONDED && ! (option2flag & PSYCHOPHYSICS_BIT))
            {
                /* 
                 * if this is the trial exiting the correction loop loopstate will
                 * be normal. Incremnet stimno so that the next stimulus is a different one.
                 * but the trial should be counted as a wrong one, based on the trial that
                 * took us into the correction loop.
                 */
                //Call ResetExptStim with offset 1 as this is before stimno is incrmemented
                if(!(option2flag & PSYCHOPHYSICS_BIT))
                    ResetExpStim(1);
                SetFixColor(expt);
                /*
                 * This is where stimno is incremented for the last stim of the trial
                 */
                if(!states[EXPT_PAUSED] && TheStim->mode & EXPTPENDING 
                   && afc_s.loopstate != CORRECTION_LOOP
                   && !optionflags[RUN_SEQUENCE] && !freezeexpt)
                    stimno++;
                if(seroutfile)
                    fprintf(seroutfile,"#stimno %d%c\n",stimno,exptchr);
                fixstate = INTERTRIAL;
                SendTrialCount();
                fsleep(0.05);
// if late resp just detected above, fixstate==RESPONDED but stimstate = POSTTRIAL                
                if(monkeypress == WURTZ_OK_W && rewardall == 0)
                    start_timeout(monkeypress);
                else if (expt.vals[CHOICE_TARGET_DURATION] > 0 && monkeypress == WURTZ_OK)
                    paint_target(expt.targetcolor,2);
                stimstate = POSTTRIAL;
            }
            else if (CheckFix() < 0){ /* Bad saccade gives BAD_FIXATION code here */
                if(!(option2flag & PSYCHOPHYSICS_BIT))
                    ResetExpStim(0);
                ShuffleStimulus(BAD_FIXATION);
            }
            else{
                if (expt.vals[CHOICE_TARGET_DURATION] > 0 && monkeypress == WURTZ_OK)
                    paint_target(expt.targetcolor,2);
            }
    
            if(testflags[PLAYING_EXPT]){
                ReplayExpt(NULL);
                if(stimstate == WAIT_FOR_RESPONSE) //not changed
                    stimstate = PRESTIMULUS;
            }
            
#ifdef WIN32
            if(waitcount++ <2)
                change_frame();
#else
            waitcount++;
            change_frame();
#endif
            break;
        case POSTTRIAL:
            ShowTrialCount(0, -1);
            if(rdspair(expt.st))
                i = 0;
            if(seroutfile){
                fprintf(seroutfile,"#PostTrial, last %d stimno%d%c\n",laststate,stimno,exptchr);
                fflush(seroutfile);
            }
            if(option2flag & AFC)
                CountReps(stimno);
            if((option2flag & PSYCHOPHYSICS_BIT) || fixstate == BAD_FIXATION){
                ResetExpStim(0);
                if(ExptIsRunning())
                    PrepareExptStim(1,10);
                SetFixColor(expt);
                if(fixstate == BAD_FIXATION && TheStim->fix.timeout > 0){
                    search_background();
                    change_frame();
                    search_background();
                }
            }            
            else{
                setmask(bothmask);
                wipescreen(clearcolor);
                RunBetweenTrials(st, pos);
                if (expt.vals[CHOICE_TARGET_DURATION] > 0  && monkeypress == WURTZ_OK)
                    paint_target(expt.targetcolor,2);
                change_frame();
            }
            if(debug) glstatusline("PostTrial",3);
            if(fabs(expt.vals[PURSUIT_INCREMENT]) > 0.001 && fixstate != BAD_FIXATION){
                /*
                 * N.B. at this moment changes in PURSUIT INCREMENT as part of an expt will not
                 * have been made yet for the next trial. This works becuase the after pursuing in one direction, the next trial must be a pursuit back. 
                 */
                dx = (expt.vals[PURSUIT_INCREMENT]) * (expt.st->nframes-1) * sin(expt.vals[FP_MOVE_DIR]);
                dy = cos(expt.vals[FP_MOVE_DIR]) * (expt.vals[PURSUIT_INCREMENT]) * (expt.st->nframes-1);
                
                
                if(pursued > 0.1 || stimno & 1){
                    pursuedir = -1;
                    if(altstimmode != MOVE_STIM_ONLY){
                        expt.vals[FIXPOS_X] = expt.fixpos[0] + dx;
                        expt.vals[FIXPOS_Y] = expt.fixpos[1] + dy;
                    }
                    TheStim->pos.xy[0] = deg2pix(expt.vals[XPOS] + dx);
                    TheStim->pos.xy[1] = deg2pix(expt.vals[YPOS] + dy);
                    //	    printf("P- %.3f\n",expt.vals[PURSUIT_INCREMENT]);
                }
                else{
                    pursuedir = 1;
                    expt.vals[FIXPOS_X] = expt.fixpos[0];
                    expt.vals[FIXPOS_Y] = expt.fixpos[1];
                    TheStim->pos.xy[0] = deg2pix(expt.vals[XPOS]);
                    TheStim->pos.xy[1] = deg2pix(expt.vals[YPOS]);
                    //	    printf("P+ %.3f\n",expt.vals[PURSUIT_INCREMENT]);
                }
                SerialSend(PURSUIT_INCREMENT);
                fixpos[0] = deg2pix(expt.vals[FIXPOS_X]);
                if(seroutfile){
                    fprintf(seroutfile,"#fx %.2f,%.2f (%.2f,%.2f) pi %.3f(%.0f) xo %.3f\n",expt.stimvals[FIXPOS_X],
                            expt.stimvals[FIXPOS_Y],dx,dy,
                            expt.vals[PURSUIT_INCREMENT],pursuedir,pix2deg(TheStim->pos.xy[0]));
                    fprintf(seroutfile,"#efx %.2f,%.2f stimno %d\n",expt.fixpos[0],expt.fixpos[1],stimno);
                }
            }
            else{
                expt.vals[FIXPOS_X] = expt.stimvals[FIXPOS_X];
                expt.vals[FIXPOS_Y] = expt.stimvals[FIXPOS_Y];
                fixpos[0] = deg2pix(expt.vals[FIXPOS_X]);
            }
            pursued = 0;
            fixpos[1] = deg2pix(expt.vals[FIXPOS_Y]);
            SerialSend(FIXPOS_XY);
            draw_fix(fixpos[0],fixpos[1], TheStim->fix.size, TheStim->fixcolor);
            
            /* stimseq[].result is used in human psychophysics for staircases */
            if(!(option2flag & PSYCHOPHYSICS_BIT))
                stimseq[trialctr].result = monkeypress;
            else
                ShowInfo();
            if(trialctr < TRIALBUFFERLEN)
                trialctr++;
            else{
                trialctr = 0;
                triallaps++;
            }
            stimseq[trialctr].a = stimseq[trialctr].b = 0;
            /*
             * if expt stim is prepared during a timout, this undoes the setting
             * of saccval...
             afc_s.sacval[0] =  afc_s.abssac[0];
             afc_s.sacval[1] =  afc_s.abssac[1];
             */
            if (fixstate == BADFIX_STATE)
                CheckStimDuration(fixstate);

            if(fixstate == BADFIX_STATE && TheStim->fix.timeout > 0)
                stimstate = IN_TIMEOUT;
            else if(monkeypress == WURTZ_OK_W)
                stimstate = IN_TIMEOUT_W;
            else{
                stimstate = INTERTRIAL;
                // ONE_TRIAL meand one _good trial, so only stop if not a timeout
                if(states[ONE_TRIAL]){
                    StopGo(STOP);
                    states[ONE_TRIAL] = 0;
                }
            }
            if(stopgo == STOP)
                StopGo(STOP);
            if(stairfd){
                fprintf(stairfd,"Post%d(%d) ",stimno,stimorder[stimno]);
            }
            if (stimstate != POSTTRIAL)
                ReadCommandFile(expt.cmdinfile);
            break;
        case IN_TIMEOUT:
        case IN_TIMEOUT_W:
            if(rdspair(expt.st))
                i = 0;
            if(debug) glstatusline("Timeout",3);
            duration = TheStim->fix.timeout; 
            if(stimstate == IN_TIMEOUT_W)
                duration = afc_s.wrongtimeout; 
            else if (timeout_type == SHAKE_TIMEOUT_PART1)
                duration = expt.vals[SHAKE_TIMEOUT_DURATION]; 
            else if (timeout_type == SHAKE_TIMEOUT_PART2)
                duration = 1;
            markercolor = 0;
            if(monkeypress == WURTZ_OK_W)
            {
                markercolor = 0;
                //	  mySwapBuffers();
                setmask(BOTHMODE);
                chessboard(128,128);
            }
            if((val = timediff(&now, &starttimeout)) > duration)
            {
                if(timeout_type == SHAKE_TIMEOUT_PART1){
                    timeout_type = SHAKE_TIMEOUT_PART2;
                    start_timeout(SHAKE_TIMEOUT_PART2);
                }
                else{
                    if (duration > 0)
                        end_timeout();
                    stimstate = INTERTRIAL;
                }
                fctr = 0;
            }
            
            redraw_overlay(expt.plot);
            if(timeout_type == SHAKE_TIMEOUT_PART2 && 0)
                ShowTime();
            change_frame();
            break;
    }
    lastval = val;
    laststate = oldstimstate;
    if(debug == 4){
        testcolor();
    }
    gettimeofday(&nftime,NULL);
    return(framecount);
}

void expback()
{
}

void exprun()
{
}



void expfront()
{
    //Ali	if(display_win == (Window)NULL)
    //		makewindow(NULL);
	
	mode |= WINDOWS_RAISED;
}

/*
 void MyFunction (AUX_EVENTREC *event ) {
 GLint x, y;
 x = event -> data[AUX_MOUSEX];
 y = event -> data[AUX_MOUSEY];
 }
 */



void set_test_loop()
{
    if(mode & TEST_PENDING)
        mode &= (~TEST_PENDING);
    else
        mode |= TEST_PENDING;
}


void test_dot(vcoord x, vcoord y, vcoord w){
    glVertex2f(x-w,y-w);
    glVertex2f(x+w,y-w);
    glVertex2f(x+w,y+w);
    glVertex2f(x-w,y+w);
}

void lwrect(vcoord llx, vcoord lly, vcoord urx, vcoord ury)
{
    vcoord x,y;
    
    x = (llx+urx)/2;
    y = (lly+ury)/2;
    glVertex2f(x,lly);
    glVertex2f(x,ury);
    glVertex2f(llx,y);
    glVertex2f(urx,y);
    
}

void rotrect(vcoord *line, vcoord x, vcoord y)
{
    glVertex2f(x+line[0],y+line[1]);
    glVertex2f(x+line[2],y+line[3]);
}

#ifdef Darwin
void aarotrect(vcoord *line, vcoord x, vcoord y)
{
    
    /*
     if(y+line[1] > winsiz[1] || y+line[1] < -winsiz[1])
     return;
     */
    glBegin(GL_POLYGON);
    glVertex2f(x+line[0],y+line[1]);
    glVertex2f(x+line[2],y+line[3]);
    glVertex2f(x+line[4],y+line[5]);
    glVertex2f(x+line[6],y+line[7]);
    glEnd();
}
#else
void aarotrect(vcoord *line, vcoord x, vcoord y)
{
    
    /*
     if(y+line[1] > winsiz[1] || y+line[1] < -winsiz[1])
     return;
     */
    
    glVertex2f(x+line[0],y+line[1]);
    glVertex2f(x+line[6],y+line[7]);
}
#endif


void inrect(vcoord llx, vcoord lly, vcoord urx, vcoord ury)
{
    vcoord x,y;
    float lcolor;
    llx += 0.5;
    lly += 0.5;
    urx -= 0.5;
    ury -= 0.5;
    x = (llx + urx)/2;
    /*   glVertex2f(x,lly);
     glVertex2f(x,ury);
     */
    for(x = llx; x < urx; x += 1.0)
    {
        glVertex2f(x,lly);
        glVertex2f(x,ury);
    }
    glVertex2f(urx,lly);
    glVertex2f(urx,ury);
    /*      glVertex2f(llx,lly);
     glVertex2f(llx,ury);
     glVertex2f(urx,lly);
     glVertex2f(urx,ury);
     glVertex2f(llx,ury);
     glVertex2f(urx,ury);
     glVertex2f(llx,lly);
     glVertex2f(urx,lly);
     */
}


void run_rds_test_loop()
{
    int i,j,fw,fh,ival,frame = 0;
    int *p,d,*end;
    vcoord *x,*y,xp[2],dw,dh,w,h;
    Locator *pos = &TheStim->pos;
    char cbuf[BUFSIZ];
    Stimulus *st;
    int shift = pos->xy[1];
    float vcolor[4],fixw;
    Substim *gb = TheStim->left;
    Substim *sst = TheStim->left;
    float val,angle;
    vcoord rect[8],crect[4];
    double sina,cosa,o;
    char c;
    int ncalls[4];
    
    vcolor[0] = vcolor[1] = vcolor[2] = vcolor[3] = 1.0;
    st = TheStim;
    x = sst->xpos;
    y = sst->ypos;
    w = sst->dotsiz[0]/2;
    h = sst->dotsiz[1]/2;
    dw = ((w*2)-1)/2;
    dh = ((h*2)-1)/2;
    dw = w;
    dh = h;
    fw = sst->dotsiz[0];
    fh = sst->dotsiz[1];
    
    setmask(ALLPLANES);
    SetColor(TheStim->background,1);
    clearcolor = TheStim->gammaback;
    glDrawBuffer(GL_FRONT_AND_BACK);
    glClearColor(clearcolor, clearcolor, clearcolor,clearcolor);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlushRenderAPPLE();
    glPushMatrix();
    
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0);
    if(testmode == 3){
        for(frame = 0; frame < 72; frame++){
            testcalc_rds(st->next,st->next->left, testmode);
            testcalc_rds(st,st->left, testmode);
            testcalc_rds(st->next,st->next->right, testmode);
            testcalc_rds(st,st->right, testmode);
            glDrawBuffer(GL_BACK);
            glClearColor(0.5, 0.5, 0.5,1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            paint_stimulus(st);
            //AliGLX	mySwapBuffers();
        }
        return;
    }
    
    /*      if(optionflag & ANTIALIAS_BIT)
     {
     glEnable(GL_LINE_SMOOTH);
     glLineWidth(1.0);
     if(strncmp(getRenderer(),"IMPACT",5) == 0)
     {
     glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glEnable(GL_BLEND);
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     }
     }
     */
    glDrawBuffer(GL_BACK);
    glClearColor(0.5, 0.5, 0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
	SetGrey(1.0);
    calc_stimulus(TheStim);
    gettimeofday(&timeb,NULL);
    
    glGetIntegerv(GL_RGBA_MODE, &ival);
    //AliGLX    printf("%s RGBA %d ",getRenderer(),ival);
    glGetIntegerv(GL_RED_BITS, &ival);
    printf(" Red_Bits %d ",ival);
    glGetIntegerv(GL_SUBPIXEL_BITS, &ival);
    printf("Subpix %d\n",ival);
    o = drand48();
    
    angle = rad_deg(pos->angle);
    
	cosa = cos(pos->angle);
	sina = sin(pos->angle);
	dh = dh - 0.5;
	rect[0] = -dw * cosa - dh * sina;
	rect[1] = -dh * cosa + dw * sina;
	rect[2] = -dw * cosa + dh * sina;
	rect[3] = dh * cosa + dw * sina;
	rect[4] = dw * cosa + dh * sina;
	rect[5] = dh * cosa - dw * sina;
	rect[6] = dw * cosa - dh * sina;
	rect[7] = -dh * cosa - dw * sina;
	dh = dh+0.5;
	crect[0] = -dh * sina;
	crect[1] = -dh * cosa;
	crect[2] = dh * sina;
	crect[3] = dh * cosa;
    
    glTranslatef((pos->xy[0]),pos->xy[1],0);
	glRotatef(angle,0.0,0.0,1.0);
    
    
    
    for(frame = 0; frame < 72; frame++)
	{
        glClear(GL_COLOR_BUFFER_BIT);
        glColor4f(vcolor[0],vcolor[1],vcolor[2],vcolor[3]);
        if(optionflag & ANTIALIAS_BIT)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LINE_SMOOTH);
	    p = sst->im;
	    end = (sst->im+sst->ndots);
	    x = sst->xpos;
	    y = sst->ypos;
	    if(optionflag & ANTIALIAS_BIT)
        {
      		glLineWidth(1.0);
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_LINE_SMOOTH);
            glBegin(GL_LINES);
            glColor4f(vcolor[0],vcolor[1],vcolor[2],vcolor[3]);
            i = 0;
            for(;p < end; p++,x++,y++)
            {
                if((i = !i))
                    glColor3f(vcolor[0],vcolor[1],vcolor[2]);
                else	
                    glColor3f(0.0,0.0,0.0);
                aarotrect(rect,*x+drand48(),*y);
            }
            glEnd();
            p = sst->im;
            end = (sst->im+sst->ndots);
            x = sst->xpos;
            y = sst->ypos;
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
        }
	    i = 0;
	    glLineWidth(dw*2);
	    glColor4f(vcolor[0],vcolor[1],vcolor[2],vcolor[3]);
	    i = 0;
        
        /*
         on octane 2, this AA has no cost!!
         */
	    glEnable(GL_BLEND);
	    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glEnable(GL_LINE_SMOOTH);
	    glBegin(GL_LINES);
	    for(;p < end; p++,x++,y++){
            if((i = !i))
                glColor3f(vcolor[0],vcolor[1],vcolor[2]);
            else	
                glColor3f(0.0,0.0,0.0);
            rotrect(crect, *x+o, *y+o);
	    }
	    glEnd();
        
        /*
         Much slower, and get funny interaction in middle
         
         glEnable(GL_BLEND);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glEnable(GL_POLYGON_SMOOTH);
         glBegin(GL_QUADS);
         
         for(;p < end; p++,x++,y++){
         if((i = !i))
         glColor3f(vcolor[0],vcolor[1],vcolor[2]);
         else	
         glColor3f(0.0,0.0,0.0);
         test_dot(*x+o, *y+o,dh);
         }
         glEnd();
         glDisable(GL_POLYGON_SMOOTH);
         */
        
	    c = STIM_CHANGE;
	    write(ttys[0],&c,1);
	    if(!optionflags[CALCULATE_ONCE_ONLY])
            calc_stimulus(TheStim);
        //AliGLX	    mySwapBuffers();
	    glFlushRenderAPPLE();
	    c = FRAME_SIGNAL;
	    write(ttys[0],&c,1);
	}
    
    glPopMatrix();
    gettimeofday(&now,NULL);
    val = timediff(&now,&timeb);
    sprintf(mssg,"B%d dots(%dx%d) * 72 = %.3f seconds",sst->ndots,fw,fh,val);
    statusline(mssg);
    
    
    gettimeofday(&timeb,NULL);
    for(frame = 0; frame < 72; frame++){
        ncalls[0] = calc_rds(st->next,st->next->left);
        ncalls[1] = calc_rds(st,st->left);
        ncalls[2] = calc_rds(st->next,st->next->right);
        ncalls[3] = calc_rds(st,st->right);
    }
    gettimeofday(&now,NULL);
    val = timediff(&now,&timeb);
    printf("Randoms calls %d,%d,%d,%d\n",ncalls[0],ncalls[1],ncalls[2],ncalls[3]);
    printf("Calc took %.3f,",val);
    
    gettimeofday(&timeb,NULL);
    for(frame = 0; frame < 72; frame++){
        testcalc_rds(st->next,st->next->left, testmode);
        testcalc_rds(st,st->left, testmode);
        testcalc_rds(st->next,st->next->right, testmode);
        testcalc_rds(st,st->right, testmode);
        if(testmode == 3)
            paint_stimulus(st);
        //AliGLX	mySwapBuffers();
    }
    gettimeofday(&now,NULL);
    val = timediff(&now,&timeb);
    printf(" %.3f,",val);
    
    gettimeofday(&timeb,NULL);
    for(frame = 0; frame < 72; frame++){
        calc_stimulus(TheStim);
    }
    gettimeofday(&now,NULL);
    val = timediff(&now,&timeb);
    printf(" %.3f,",val);
    
    printf("\n");
}


void run_radial_test_loop(){
    int i,j,fw,fh,ival,frame = 0,w,h;
    int d,*end;
    vcoord *x,*y,xp[2],dw,dh;
    Locator *pos = &TheStim->pos;
    char cbuf[BUFSIZ];
    Stimulus *st = TheStim;
    int shift = pos->xy[1];
    float vcolor[4],fixw;
    Substim *gb = TheStim->left;
    Substim *sst = TheStim->left;
    float val,angle;
    vcoord rect[8],crect[4];
    double sina,cosa;
    char c;
    GLubyte *im1,*im2,*p,col;
    int ndots = 0,nframes = 72;
    int *iy;
    double *ax,*ay,xv,*pd,sd,s;
    float disp = 0;
    int nrpt = st->framerepeat;
    
    
    for(frame = 0; frame < nframes; frame++){
        
        /*
         calc_radial(st,st->left,disp);
         calc_radial(st,st->right,-disp);
         */
        calc_stimulus(st);
        if(frame > 5){
            st->left->show_increment = 1;
            st->right->show_increment = 1;
        }
        else{
            st->left->show_increment = 0;
            st->right->show_increment = 0;
        }
        
        for(i = 0; i < nrpt; i++){
            setmask(LEFTMODE);
            paint_radial(st->left,st->left->mode,0);
            setmask(RIGHTMODE);
            paint_radial(st->right,st->right->mode,0);
            change_frame();
        }
        for(i = 0; i < nrpt; i++){
            setmask(LEFTMODE);
            paint_radial(st->right,st->left->mode,0);
            setmask(RIGHTMODE);
            paint_radial(st->left,st->right->mode,0);
            change_frame();
        }
        if(!(mode & ANIMATE_BIT))
            return;
    }
    printf("Radial test\n");
    
}

void run_gabor_test_loop()
{
    int i,j,fw,fh,ival,frame = 0,w,h;
    int d,*end;
    vcoord *x,*y,xp[2],dw,dh;
    Locator *pos = &TheStim->pos;
    char cbuf[BUFSIZ];
    Stimulus *st = TheStim;
    int shift = pos->xy[1];
    float vcolor[4],fixw;
    Substim *gb = TheStim->left;
    Substim *sst = TheStim->left;
    float val,angle;
    vcoord rect[8],crect[4];
    double sina,cosa;
    char c;
    GLubyte *im1,*im2,*p,col;
    int ndots = 0,nframes = 72;
    int *iy;
    double *ax,*ay,xv,*pd,sd,s;
    
    w = (int)(2 *pos->radius[0]);
    h = (int)(2 * pos->radius[0]);
    
    setmask(ALLPLANES);
    im1 = (GLubyte *)malloc(w * h * sizeof(GLubyte));
    im2 = (GLubyte *)malloc(w * h * sizeof(GLubyte));
    ax = (double *)malloc((w + nframes) * sizeof(double));
    ay = (double *)malloc((h+nframes) * sizeof(double));
    iy = (int *)malloc((h+nframes) * sizeof(int));
    sd = StimulusProperty(TheStim,SD_X);
    for(i = 0; i < w+nframes; i++){
        s = sin(2 * M_PI * i/deg2pix(1/st->f))/2 ;
        ax[i] = s * exp(sqr(pos->radius[0]-i)/(-2*sqr(deg2pix(sd))));
    }
    
    sd = StimulusProperty(TheStim,SD_Y);
    for(i = 0; i < h+nframes; i++){
        ay[i] = exp(sqr(pos->radius[0]-i)/(-2*sqr(deg2pix(sd))));
        iy[i] = (int)(255 * ay[i]);
    }
    
    
    p = im1;
    for(i = 0; i < w; i++){
        col = 255 * ax[i];
        for(j = 0; j < h; j++){
            *p++ = (GLubyte)(col);
        }
    }
    p = im1;
    
    gettimeofday(&timeb,NULL);
    ndots = w;
    glDrawBuffer(GL_BACK);
    glRasterPos2i(-w/2,-h/2);
    if(optionflag & WURTZ_WHOLE_STIMULI){
        ndots = w * h;
        for(frame = 0; frame < nframes; frame++)
        {
            p = im1;
            for(i = 0; i < w; i++){
                xv = ax[i+frame] * 255;
                for(j = 0; j < h; j++){
                    *p++ = (GLubyte)(127 + (ay[j] * xv));
                }
            }
            glDrawPixels(w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, im1);
            //AliGLX	  mySwapBuffers();
        }
    }
    else if(option2flag & FLASH_BIT){
        ndots = w;
        col = frame * 3;
        for(frame = 0; frame < nframes; frame++)
        {
            p = im1;
            for(i = 0; i < w; i++){
                col = frame;
                for(j = 0; j < h; j++){
                    *p++ = (GLubyte)(col);
                }
            }
            glDrawPixels(w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, im1);
            glFlushRenderAPPLE();
            //AliGLX	  mySwapBuffers();
        }
    }
    else if(option2flag & RANDOM){
        ndots = w;
        col = frame * 3;
        for(frame = 0; frame < nframes; frame++)
        {
            if(frame & 1)
                glDrawPixels(w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, im1);
            else
                glDrawPixels(w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, im2);
            glFlushRenderAPPLE();
            //AliGLX	  mySwapBuffers();
        }
    }
    else if(optionflags[PAINT_BACKGROUND]){
        /* Just calculate, no rendering */
        ndots = w * h;
        for(frame = 0; frame < nframes; frame++)
        {
            col = frame;
            p = im1;
            for(i = 0; i < w; i++){
                xv = ax[i+frame] * 255;
	    	    p = &im1[i*h];
                pd = ay;
                for(j = 0; j < h; j++){
                    //	    *p++ = (GLubyte)(*ay + xv);
                    //	    *p++ = (GLubyte)(ay[j] * xv);
                    //	    *p++ = (GLubyte)(ay[j] + xv);
                    p[j] = 0.5 + ay[j] * xv;
                }
            }
        }
    }
    else{
        ndots = w;
        for(frame = 0; frame < nframes; frame++)
        {
            p = im1;
            for(i = 0; i < w; i++){
                col = 255 * fabs(sin((double)(i+frame)/10));
                for(j = 0; j < h; j++){
                    *p++ = (GLubyte)(col);
                }
            }
            glDrawPixels(w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, im1);
            glFlushRenderAPPLE();
            //AliGLX	  mySwapBuffers();
        }
    }
    gettimeofday(&now,NULL);
    val = timediff(&now,&timeb);
    sprintf(mssg,"B%d dots(%dx%d) * 72 = %.3f seconds",ndots,w,h,val);
    statusline(mssg);
    free(im2);
    free(im1);
    free(ax);
    free(ay);
}

void run_anticorrelated_test_loop()
{
    int i,j;
    Locator *pos = &TheStim->pos;
    char cbuf[BUFSIZ];
    Stimulus *st;
    int shift = pos->xy[1];
    
    st = TheStim;
    setmask(ALLPLANES);
    glClearColor(clearcolor,clearcolor,clearcolor,clearcolor);
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(-shift,0,0);
    st->flag &= (~ANTICORRELATE);
    calc_stimulus(TheStim);
    if(st->next != NULL && st->next->type == STIM_RDS)
    {
        st->next->flag &= (~ANTICORRELATE);
        calc_stimulus(TheStim);
        paint_rds(st->next,st->right->mode);
    }
    paint_rds(st,st->right->mode);
	glTranslatef(shift,0,0);
	if(st->next != NULL && st->next->type == STIM_RDS)
        paint_rds(st->next,st->left->mode);
    paint_rds(st,st->left->mode);
    
    glTranslatef(shift,0,0);
    st->flag |= ANTICORRELATE;
    calc_stimulus(TheStim);
    if(st->next != NULL && st->next->type == STIM_RDS)
    {
        st->next->flag |= ANTICORRELATE;
        calc_stimulus(TheStim);
        paint_rds(st->next,st->right->mode);
    }
    paint_rds(st,st->right->mode);
    
	setmask(BOTHMODE);
	st->pos.lastxy[0] = st->pos.xy[0];
	st->pos.lastxy[1] = st->pos.xy[1];
	pos->phase += TheStim->incr;
	glPopMatrix();
	glFlushRenderAPPLE();
	glFinishRenderAPPLE();
    //AliGLX	mySwapBuffers();
}

void run_general_test_loop()
{
    int i,j;
    Locator *pos = &TheStim->pos;
    char cbuf[BUFSIZ];
    
    calc_stimulus(TheStim);
    gettimeofday(&zeroframetime, NULL);
    for(i = 0; i < TheStim->nframes; i++)
    {
        paint_stimulus(TheStim);
        pos->phase += TheStim->incr;
        calc_stimulus(TheStim);
        change_frame();
        //AliGLX      mySwapBuffers();
    }
    j = getframecount();
    sprintf(cbuf,"Took %d real frames",j);
    statusline(cbuf);
    gettimeofday(&zeroframetime, NULL);
    setmask(bothmask);
    for(i = 0; i < TheStim->nframes; i++)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        paint_stimulus(TheStim);
        pos->phase += TheStim->incr;
        calc_stimulus(TheStim);
        //AliGLX      mySwapBuffers();
    }
    i = getframecount();
    sprintf(cbuf,"Took %d,%d real frames",j,i);
    statusline(cbuf);
}

void run_swap_test_loop()
{
    int i,j;
    Locator *pos = &TheStim->pos;
    char cbuf[BUFSIZ];
    
    calc_stimulus(TheStim);
    gettimeofday(&zeroframetime, NULL);
    for(i = 0; i < TheStim->nframes; i++)
    {
        wipescreen(clearcolor);
        draw_fix(fixpos[0],fixpos[1],expt.st->fix.size,expt.st->fix.fixcolor);
        change_frame();
        /*
         —glFlushRenderAPPLE();
         glFinishRenderAPPLE();
         mySwapBuffers();
         */
    }
    gettimeofday(&now, NULL);
    j = getframecount();
    sprintf(cbuf,"Took %d real frames %.3f sec",j,timediff(&now,&zeroframetime));
    statusline(cbuf);
}


void fsleep(float f)
{
	struct timeval start,stop;
    
	gettimeofday(&start,NULL);
	gettimeofday(&stop,NULL);
	while(timediff(&stop,&start) < f)
		gettimeofday(&stop,NULL);
    
}





float StimulusProperty(Stimulus *st, int code)
{
	Locator *pos = &st->pos;
	float value = 0.0,rval,lval,x,y;
	double cosa,sina;
	OneStim *psine;
	Substim *rds;
    
	rds = st->left;
	switch(code)
	{
        case SEEDOFFSET:
            value = st->seedoffset;
            break;
        case BLACKDOT_FRACTION:
            value = st->dotfrac;
            break;
        case MIXAC:
            value = st->corrmix;
            break;
        case STIMULUS_VERSION:
            value = st->stimversion;
            break;
        case DOTREPEAT:
            value = st->dotrpt;
            break;
        case MODULATION_F:
            if(optionflags[MODULATE_DISPARITY] == 2)
                value = -st->vals[code];
            else
                value = st->vals[code];
            break;
        case NHIGHCOMPONENTS:
        case NLOWCOMPONENTS:
        case NCOMPONENTS:
            value = st->nfreqs;
            break;
        case START_PHASE:
            value = st->vals[code];
            break;
            /*
             * N.B. the usual rotation equations are not quite applicable because
             * of the way theta is defined. When theta = 0, orhtogonal disparity
             * is vertical, not horizontal.
             * When theta = 90 (vertical), wanted othog disp to have same sign
             * as x disp. So:
             * x = O sina + P cosa
             * y = P sina - O cosa.
             * Hence P = y sina + x cosa, and O = x sina - y cosa;
             */
        case VSR:
            value = expt.vals[VSR];
            break;
        case PARALELL_DISP:
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            x = StimulusProperty(st,DISP_X);
            y = StimulusProperty(st,DISP_Y);
            value = y * sina + x * cosa;
            break;
        case ORTHOG_DISP:
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            x = StimulusProperty(st,DISP_X);
            y = StimulusProperty(st,DISP_Y);
            value = x * sina -  y * cosa;
            break;
        case ORTHOG_POS:
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            x = pix2deg(expt.rf->pos[0]) - StimulusProperty(st,XPOS);
            y = StimulusProperty(st,YPOS) - pix2deg(expt.rf->pos[1]);
            value = x * sina +  y * cosa;
            break;
        case ABS_ORTHOG_POS:
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            x =  -StimulusProperty(st,XPOS);
            y = StimulusProperty(st,YPOS);
            value = x * sina +  y * cosa;
            break;
        case STIMORTHOG_POS:
            x = StimulusProperty(st,ASPECT_RATIO);
            if(x > 1){
                cosa = cos(expt.st->pos.angle);
                sina = sin(expt.st->pos.angle);
            }
            else{
                cosa = cos(expt.st->pos.angle-M_PI_2);
                sina = sin(expt.st->pos.angle-M_PI_2);
            }
            x = pix2deg(expt.rf->pos[0]) - StimulusProperty(st,XPOS);
            y = StimulusProperty(st,YPOS) - pix2deg(expt.rf->pos[1]);
            value = x * sina +  y * cosa;
            break;
        case ORTHOG_POSL:
            value = StimulusProperty(st,ORTHOG_POS) - StimulusProperty(st,ORTHOG_DISP)/2;
            break;
        case ORTHOG_POSR:
            value = StimulusProperty(st,ORTHOG_POS) + StimulusProperty(st,ORTHOG_DISP)/2;
            break;
        case PARA_POS:
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            x = pix2deg(expt.rf->pos[0]) - StimulusProperty(st,XPOS);
            y = StimulusProperty(st,YPOS) - pix2deg(expt.rf->pos[1]);
            value = y * sina - x * cosa;
            break;
        case ABS_PARA_POS:
            cosa = cos(expt.rf->angle * M_PI/180.0);
            sina = sin(expt.rf->angle * M_PI/180.0);
            x =  -StimulusProperty(st,XPOS);
            y = StimulusProperty(st,YPOS);
            value = y * sina -  x * cosa;
            break;
        case FRAMEREPEAT:
            value = (float)(st->framerepeat);
            break;
        case STIM_POLARANGLE:
            value = get_polarangle(pos->xy[0], pos->xy[1]);
            if(st->type == STIM_RADIAL){
                value = rad_deg(st->left->wedge_angle);
                value = rad_deg(st->right->wedge_angle);
            }
            break;	
        case STIM_ECCENTRICITY:
            if(st->type == STIM_TWOBAR)
                value = pix2deg(sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1]) + sqr(pos->barsep/2)));
            else
                value = pix2deg(sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1])));
            break;	
        case TWOBAR_SEP:
            value = pix2deg(pos->barsep);
            break;
        case TWOBAR_ANGSEP:
            if(st->type == STIM_TWOBAR)
                value = 2 * atan((pos->barsep/2) / sqrt(sqr(pos->xy[0]) + sqr(pos->xy[1]))) * (180 / M_PI);
            else
                value = pix2deg(pos->barsep);
            break;
        case STIMULUS_TYPE_CODE:
            value = st->type;
            break;
        case BACKSTIM_TYPE:
            value = st->next->type;
            break;
        case CORRELATION:
            if(st->type == STIM_RDS || st->type == STIM_RLS || st->type == STIM_CORRUG || st->type == STIM_SQCORRUG || st->type == STIM_RDSSINE || st->type == STIM_RLSSINE ||
               st->type == STIM_GRATINGN || st->type == STIM_CHECKER){
                if(st->flag & UNCORRELATE)
                    value = 0;
                else if(st->flag & ANTICORRELATE)
                    value = fabs(st->correlation) * -1;
                else
                    value = st->correlation;
            }
            else if(st->type == STIM_GRATING && st->flag & UNCORRELATE)
                value = 0;
            else if(st->type == STIM_IMAGE && st->flag & UNCORRELATE)
                value = 0;
            else
                value = 1.0;
            break;
        case BACK_CORRELATION:
            if(st->next){
                if(st->next->flag & ANTICORRELATE)
                    value = -1;
                else if(st->next->flag &  UNCORRELATE)
                    value = 0;
                else
                    value = 1;
            }
            else
                value = 0;
            break;
        case GAMMAVAL_CODE:
            value = gammaval;
            break;
        case STANDING_DISP:
            if(st->type == STIM_CYLINDER)
                value = pix2deg(st->left->ptr->standing_disp) * 2;
            break;
        case JDEATH:
            value = st->left->ptr->deathchance;
            break;
        case FRAME_DISPLACEMENT:
            value = pix2deg(st->posinc);
            break;
        case JVELOCITY:
            if(st->type == STIM_CYLINDER)
                value = (st->left->ptr->velocity * mon.framerate * 180/M_PI);
            else if(st->type == STIM_RDS || st->type == STIM_RLS){
                value = pix2deg(st->posinc * mon.framerate);
            }
            else if(st->type == STIM_GRATING){
                value = StimulusProperty(st,TF)/pos->sf;
            }
            else if(st->type == STIM_BAR){
                value = pix2deg(st->posinc * mon.framerate);
            }
            break;
        case JF_INTENSITY:
            if(st->type == STIM_CYLINDER)
                value = st->left->ptr->front_intensity;
            break;
        case JB_INTENSITY:
            if(st->type == STIM_CYLINDER)
                value = st->left->ptr->back_intensity;
            break;
        case JNUMDOTS:
            if(st->type == STIM_CYLINDER)
                value = stimptr->left->ptr->numdots;
            break;
        case JLIFEFRAMES:
            value = stimptr->left->ptr->lifeframes;
            break;
        case GOODTRIALS:
            value = goodtrials;
            break;
        case BADTRIALS:
            value = totaltrials - goodtrials;
            break;
        case DEG2PIX:
            value = 512/pix2deg(512);
            break;
        case RF_WIDTH:
            value = pix2deg(expt.rf->size[1]);
            break;
        case RF_HEIGHT:
            value = pix2deg(expt.rf->size[0]);
            break;
        case RF_X:
            value = pix2deg(expt.rf->pos[0]);
            break;
        case RF_Y:
            value = pix2deg(expt.rf->pos[1]);
            break;
        case RF_ANGLE:
            value = (expt.rf->angle);
            break;
        case SWAPINTERVAL:
            value = TheStim->swapinterval;
            break;
        case XMINSAMPLE_RATIO:
            value = st->sample_ratio[0];
            break;
        case XMAXSAMPLE_RATIO:
            value = st->sample_ratio[1];
            break;
        case ASPECT_RATIO:
            value = pix2deg(pos->imsize[0])/pix2deg(pos->imsize[1]);
            break;
        case STIM_SIZE:
            if(pos->imsize[0] == pos->imsize[1])
                value = pix2deg(pos->imsize[0]);
            else
                value = 2 * pix2deg(sqrt(pos->radius[1] * pos->radius[0]));
            if(st->type == STIM_IMAGE){
                value = sqrt((pix2deg(pos->imsize[0]) * pix2deg(pos->imsize[1])));
            }
            if(st->type == STIM_RADIAL){
                value = sqrt((pix2deg(pos->size[0]) * pix2deg(pos->size[1])));
            }
            break;
        case STIM_WIDTH:
            value = pix2deg(pos->imsize[1]);
            value = pix2deg(2 * pos->radius[1]);
            if(st->type == STIM_RADIAL){
                value = pix2deg(pos->size[1]);
            }
            if(st->type == STIM_IMAGE){
                value = pix2deg(pos->imsize[1]);
            }
            break;
        case STIM_HEIGHT:
            value = pix2deg(pos->imsize[0]);
            value = st->length;
            if(st->type == STIM_RADIAL){
                value = pix2deg(pos->size[0]);
            }
            if(st->type == STIM_IMAGE){
                value = pix2deg(pos->imsize[0]);
            }
            break;
        case ORIENTATION:
            value = pos->angle * 180.0/M_PI;
            break;
        case ORI_BANDWIDTH:
            value = (st->left->orbw + st->right->orbw)/2;
            break;
        case BACK_ORI:
            if(st->next)
                value = st->next->pos.angle * 180.0/M_PI;
            break;
        case BACK_TF:
            if(st->next)
                value = mon.framerate * st->next->incr/(M_PI * 2);
            break;
        case ORI_LEFT:
            value = (pos->angle-st->ori_disp) * 180.0/M_PI;
            break;
        case ORI_RIGHT:
            value = (pos->angle +st->ori_disp) * 180.0/M_PI;
            break;
        case SF_LEFT:
            value = (st->f + st->sf_disp);
            break;
        case SF_RIGHT:
            value = (st->f - st->sf_disp);
            break;
        case STIM_PERIOD:
	        value = 1/st->f;
            break;
        case SF:
            value = st->f;
            break;
        case SF2:
            if(st->type == STIM_GRATING2 || st->type == STIM_GRATINGN || st->type ==STIM_GRATING)
            {
                value = st->left->ptr->sf2;
            }
            else
                value = 0.0;
            break;
        case TF2:
            if(st->type == STIM_GRATING2 || st->type == STIM_RADIAL)
            {
                psine = (OneStim *)(st->left->ptr);
                value = mon.framerate * psine->incr/(M_PI * 2);
                if(TheStim->swapinterval > 1)
                    value /= TheStim->swapinterval;
            }
            else
                value = 0.0;
            break;
        case PHASE2:
            if(st->type == STIM_GRATING2 || st->type == STIM_GABOR || st->type == STIM_RLS)
            {
                value = pos->phase2;
            }
            else
                value = 0.0;
            break;
        case ORI2:
            if(st->type == STIM_GRATING2 || st->type == STIM_GRATING)
            {
                psine = (OneStim *)(st->left->ptr);
                value = (pos->angle+psine->plaid_angle) * 180.0/M_PI;
            }
            if(st->type == STIM_RADIAL){
                value = (st->left->ptr->ori2) * 180.0/M_PI;
            }
            break;
        case SPINRATE:
            value = st->angleinc;
            break;
        case PLAID_ANGLE:
            if(st->type == STIM_GRATING2 || st->type == STIM_GRATING || st->type == STIM_SQUARE || st->type == STIM_GABOR || st->type == STIM_RLS)
            {
                value = rad_deg(st->left->ptr->plaid_angle);
            }
            else
                value = 0.0;
            break;
        case TF:/*j*/
            value = mon.framerate * st->incr/(M_PI * 2);
            if(TheStim->swapinterval > 1)
                value /= TheStim->swapinterval;
            break;
            
        case DISP_A:
            value = StimulusProperty(st,DEPTH_MOD) + StimulusProperty(st,DISP_X);
            break;
        case DISP_B:
            value = StimulusProperty(st,DISP_X) - StimulusProperty(st,DEPTH_MOD);
            break;
        case DEPTH_MOD:
            /*
             * NB confusing. st->depth mod is in degrees, and is the true depth mod.
             * st->left->depth mod is in pixels, and is half the depth mod, because
             * the pixel value is added to one side, subtracted from the other.
             */
            if(st->type == STIM_CORRUG)
                value = pix2deg(st->left->ptr->depth_mod *2);
            else if(st->type == STIM_SQCORRUG)
                value = pix2deg(st->left->ptr->depth_mod *2);
            else
                value = st->depth_mod;
            break;
        case VDISP_MOD:
            /*
             * NB confusing. st->depth mod is in degrees, and is the true depth mod.
             * st->left->depth mod is in pixels, and is half the depth mod, because
             * the pixel value is added to one side, subtracted from the other.
             */
            value = st->vdisp_mod;
            break;
        case DISP_GRAD:
            if(st->type == STIM_CORRUG)
                value = pix2deg(st->left->ptr->disp_grad * 2);
            break;
        case SD_X:/*j*/
            if(st->type == STIM_GABOR)
                value = ((OneStim *)(st->left->ptr))->sy;
	        else if(st->type == STIM_GRATING2 || st->type == STIM_GRATING)
                value = ((OneStim *)(st->left->ptr))->sy;
            else if(st->type == STIM_BAR)
                value = st->pos.radius[1]/st->pos.ss[1];
            else if(st->type == STIM_TWOBAR)
                value = st->pos.radius[1]/st->pos.ss[1];
            else if(st->type == STIM_RADIAL)
                value = st->left->ptr->sy;
            else if(st->type == STIM_RLS)
                value = st->left->ptr->sy;
            else if(st->type == STIM_CHECKER)
                value = st->left->ptr->sy;
            break;
        case SD_Y:/*j*/
            if(st->type == STIM_GABOR || st->type == STIM_RLS)
                value = st->left->ptr->sx;
            else if(st->type == STIM_RADIAL)
                value = pix2deg(st->left->ptr->rsigma);
            else if(st->type == STIM_CHECKER)
                value = st->left->ptr->sx;
            break;
        case SD_BOTH:/*j*/
            value = (st->left->ptr->sx + st->left->ptr->sy)/2;
            break;
        case SEED_SHIFT:
            if(st->type == STIM_RDS)
                value = pix2deg(st->left->xshift);
            else if(st->type == STIM_SQCORRUG)
                value = pix2deg(st->ucgap);
            else if(st->type == STIM_IMAGE)
                value = st->left->xshift;
            else
                value = 0;
            break;
        case DISP_X:
            value = pix2deg(st->disp) *2;
            break;
        case DISP_RAMP:
            if(isharris(altstimmode))
                value = pix2deg(st->dispincr) * mon.framerate;
            else
                value = pix2deg(st->dispincr) * TheStim->nframes;
            break;
        case RELDISP:
	        if(st->next != NULL)
                value = pix2deg(st->disp-st->next->disp) *2;
            else
                value = pix2deg(st->disp) *2;
            break;
        case DISP_Y:
            value = pix2deg(st->vdisp) *2;
            break;
        case PHASE_AS_DISP:
            if(st->type == STIM_GRATING2 || st->type == STIM_GRATING)
                value = st->phasedisp[0]/(M_PI * st->f);
            else if(st->type == STIM_GRATINGN){
                value = st->rolldisp;
            }
            else if(st->type == STIM_CHECKER)
                value = st->phasedisp[0];
            else
                value = sin(st->pos.angle) * st->phasedisp[0]/(M_PI * st->f);
            break;
        case SET_COUNTERPHASE:
            if(optionflag & CONTRAST_REVERSE_BIT)
                value = 1;
            else
                value = 0;
            break;
        case PHASE_AS_RELDISP:
            value = -st->phasedisp[0]/(M_PI * st->f);
            break;
        case DISP_P:
            /*
             * Phase disp recorded in radians in the UFF files, so report it that way here
             */
            if (st->type == STIM_RLS)
                value = pix2deg(st->phasedisp[0]*2);
            else
                value = st->phasedisp[0]*2;
            break;
        case DISP_P2:
            if (st->type == STIM_RLS)
                value = pix2deg(st->phasedisp[1]*2);
            else
                value = st->phasedisp[1]*2;
            break;
        case DOT_DENSITY:
            if(st->type == STIM_CYLINDER || st->type == STIM_CORRUG || st->type == STIM_SQCORRUG)
                value = st->left->ptr->density;
            else
                value = st->left->density;
            SetDotDistribution();
            break;
            
        case XSAMPLES:/*j*/
            if(st->type == STIM_GRATING || st->type == STIM_GRATING2 || st->type == STIM_GABOR)
                value = pos->ss[1];
            break;
        case DOT_SIZE:
            if(st->type == STIM_RDS || st->type == STIM_RLS || st->type == STIM_RDSSINE || st->type == STIM_CHECKER)
                value = pix2deg(st->left->dotsiz[0]);
            else if(st->type == STIM_CYLINDER || st->type == STIM_CORRUG || st->type == STIM_SQCORRUG)
                value = pix2deg(st->left->ptr->dotsiz[0]);
            break;
        case YSAMPLES:
            if(st->type == STIM_GRATING || st->type == STIM_GRATING2 || st->type == STIM_GABOR)
                value = pos->ss[0];
            break;
        case SETZOOM:
            value = zoom;
            break;
        case SETZXOFF:
            value = pix2deg(pos->xy[0]);
            break;
        case SETZYOFF:
            value = pix2deg(pos->xy[1]);
            break;
        case CONTRAST2:
            value = pos->contrast2;
            break;
        case CONTRAST_LEFT:
            value = pos->contrast_amp + st->contrast_disp;
            break;
        case CONTRAST_RIGHT:
            value = pos->contrast_amp - st->contrast_disp;
            break;
        case SETCONTRAST:
            value = pos->contrast_amp;
            break;
        case FB_RELATIVE_CONTRAST:
            if(st->next != NULL && st->next->type != STIM_NONE){
                rval = StimulusProperty(st, SETCONTRAST);
                lval = StimulusProperty(st->next, SETCONTRAST);
                if(rval < lval){
                    value = rval+lval;
                }
                else{
                    value = -(rval+lval);
                }
            }
            else{
                value = NOTSET;
            }
            break;
        case CONTRAST_PAIRS:
            rval = StimulusProperty(st, CONTRAST_RIGHT);
            lval = StimulusProperty(st, CONTRAST_LEFT);
            if(lval-rval > 0.01)
                value = -0.5;
            else if(lval-rval < -0.01)
                value = 0.5;
            else if(fabs(rval - expt.stimvals[SETCONTRAST]) < 0.01)
                value = 1;
            else if(fabs(rval - expt.stimvals[CONTRAST2]) < 0.01)
                value = 0;
            else
                value = -10000; /* should not happen */
            break;
        case PLAID_RATIO:
            // ratio > 1 = contrast1 > contrast2
            // ratio > 0 = contrast1 > contrast2
            lval = StimulusProperty(st, SETCONTRAST);
            rval = StimulusProperty(st, CONTRAST2);
            value = lval - rval;
            break;
        case CONTRAST_RATIO:
            if(st->type == STIM_RDSSINE || st->type == STIM_GRATING2)
                value = expt.vals[CONTRAST_RATIO];
            else{
                rval = StimulusProperty(st, CONTRAST_RIGHT);
                lval = StimulusProperty(st, CONTRAST_LEFT);
                if(rval != 0)
                    value = lval/rval;
                else
                    value = 0;
            }
            break;
        case CONTRAST_DIFF:
            if(st->type == STIM_RDSSINE || st->type == STIM_RADIAL)
                value = expt.vals[CONTRAST_RATIO];
            else
                value = st->contrast_disp;
            break;
        case SF_DIFF:
            value = st->sf_disp*2;
            break;
        case SIZE_DISP:
            value = (st->dlength + st->dwidth)/2;
            break;
        case LENGTH_DISP:
            if(isinduced(expt.vals[ALTERNATE_STIM_MODE] )){
                
                value = (st->left->vscale > st->right->vscale) ? st->left->vscale : st->right->vscale;
            }
            else
                value = st->dlength;
            break;
        case WIDTH_DISP:
            value = st->dwidth;
            break;
        case HEIGHT_R:
            lval = StimulusProperty(st, STIM_HEIGHT);
            value = lval - st->dlength/2;
            break;
        case HEIGHT_L:
            lval = StimulusProperty(st, STIM_HEIGHT);
            value = lval + st->dlength/2;
            break;
        case WIDTH_R:
            lval = StimulusProperty(st, STIM_WIDTH);
            value = lval - st->dwidth/2;
            break;
        case WIDTH_L:
            lval = StimulusProperty(st, STIM_WIDTH);
            value = lval + st->dwidth/2;
            break;
        case DORI_BACK:
            if(st->next != NULL)
                value = rad_deg(st->next->ori_disp) * 2;
            else
                value = 0;
            break;
        case ORIENTATION_DIFF:
        case DORI_BOTH:
            value = rad_deg(st->ori_disp) * 2;
            break;
        case SETBACKCOLOR:
            value = st->background;
            break;
        case SETOVERLAYCOLOR:
            value = expt.overlay_color;
            break;
        case DOT_POSX:
            value = pix2deg(pos->locn[0]);
            break;
        case SETPHASE:
            value = rad_deg(st->pos.phase);
            break;
        case PHASEREL:
            value = rad_deg(st->pos.dphase);
            break;
        case SETFIXCOLOR:
            value = st->fix.fixcolor;
            break;
        case FIX_OFF_CODE:
            value = st->fix.offcolor;
            break;
        case SETFIXSIZE:
            value = pix2deg(st->fix.size);
            break;
        case SACTARGSIZE:
            value = pix2deg(afc_s.targsize);
            break;
        case WURTZ_DURATION_CODE:
            /*	  if(optionflag & WURTZ_WHOLE_STIMULI)
             value = st->fix.fixstims;
             else
             */
            value = st->fix.fixframes;	  
            break;
        case SET_SEEDLOOP:/*j*/
            value = st->left->seedloop;
            break;
        case SET_SEED:/*j*/
	        value = stimptr->left->baseseed;
            break;
        case BACK_SIZE:
            if(st->next == NULL)
                value = 0.0;
	        else
                value = StimulusProperty(st->next,STIM_HEIGHT);
            break;
        case BACK_HEIGHT:
            if(st->next == NULL)
                value = 0.0;
	        else
                value = StimulusProperty(st->next,STIM_HEIGHT);
            break;
        case BACK_WIDTH:
            if(st->next == NULL)
                value = 0.0;
	        else
                value = StimulusProperty(st->next,STIM_WIDTH);
            break;
        case BACK_CONTRAST:
            if(st->next == NULL)
                value = 0.0;
	        else
                value = StimulusProperty(st->next,SETCONTRAST);
            break;
        case DISP_BACK:
            if(st->next == NULL && st->prev == NULL)
                value = 0.0;
            else if(st->next == NULL && st->prev != NULL)
                value = pix2deg(st->disp) *2;
	        else{
                if(st->next->flag & UNCORRELATE && (st->next->type == STIM_RDS || st->next->type) == STIM_RLS)
                    value = INTERLEAVE_EXPT_UNCORR;
                else
                    value = pix2deg(st->next->disp) *2;
            }
            break;
        case BACK_VDISP:
            if(st->next == NULL && st->prev == NULL)
                value = 0.0;
            else if(st->next == NULL && st->prev != NULL)
                value = pix2deg(st->vdisp) *2;
	        else{
                if(st->next->flag & UNCORRELATE)
                    value = INTERLEAVE_EXPT_UNCORR;
                else
                    value = pix2deg(st->next->vdisp) *2;
            }
            break;
        case RAMP_COEFFICIENT:
            value = expt.clamp_rrate;
            break;
        case FIXPOS_X:
            value = pix2deg(fixpos[0]);
            break;
        case FIXPOS_Y:
            value = pix2degy(fixpos[1]);
            break;
        case VIEWD_CODE:
            value = mon.viewd;
            break;
        case REWARD_SIZE:
            value = st->fix.rwsize;
            break;
        case TIMEOUT_CODE:
            value = st->fix.timeout;
            break;
        case SACCADE_AMPLITUDE:
            value = sqrt(sqr(afc_s.abssac[0]) + sqr(afc_s.abssac[1]));
            break;
        case HSACCADE_VALUE:	/*j*/
            value = afc_s.sacval[0];
            break;
        case VSACCADE_VALUE:	/*j*/
            value = afc_s.sacval[1];
            break;
        case SAC_FIX_CRIT:	/*j*/
            value = afc_s.sac_fix_crit;
            break;
        case WRONG_TIMEOUT_CODE:	/*j*/
            value = afc_s.wrongtimeout;
            break;
        case STARTVAL:	/*j*/
            value = afc_s.startvalue;
            break;
        case MAXSTAIRVAL:	/*j*/
            value = afc_s.maxstairvalue;
            break;
        case MINSTAIRVAL:	/*j*/
            value = afc_s.minstairvalue;
            break;
        case CORRECTION_LEAVE_CRIT:
            value = afc_s.correction_leave;
	        break;
        case CORRECTION_ENTRY_CRIT:
            value = afc_s.correction_entry;
	        break;
        case AFC_PROPORTION:
            value = afc_s.proportion;
	        break;
        case BRIGHTSHIFT_CODE:
            value = st->fix.stimcolor - st->fix.fixcolor;
            break;
        case BLANKCOLOR_CODE:
            value = expt.vals[BLANKCOLOR_CODE];
            break;
        case STOP_CRITERION:
            value = stopcriterion;
            break;
        case RUNAVERAGE_LENGTH:
            value = avglen;
            break;
        default:
            value = NOTSET;
            break;
	}
	return(value);
}

void NewFixPos(float x, float y)
{
    char cbuf[BUFSIZ];
    float fx,fy;
    
    fx = pix2deg(x);
    fy = pix2deg(y);
    
    if(optionflags[NO_MIRRORS])
        sprintf(cbuf,"%2s %.4f %.4f\n",serial_strings[FIXPOS_XY],-fx,fy);
    else
        sprintf(cbuf,"%2s %.4f %.4f\n",serial_strings[FIXPOS_XY],fx,fy);
    /*
     oldfixpos[0] = expt.vals[FIXPOS_X];
     oldfixpos[1] = expt.vals[FIXPOS_Y];
     */
    oldfixpos[0] = fixpos[0];
    oldfixpos[1] = fixpos[1];
    fixpos[0] = x;
    fixpos[1] = y;
    SerialString(cbuf,0);
    fixpaint = 2;
    mode |= STIMCHANGE_FRAME;
}

void setoption()
{
    static long old = 0,old2 = 0;
    long new,new2,gone,gone2;
    
    new = (optionflag & (~old));
    gone = ((~optionflag) & old);
    gone2 = ((~option2flag) & old2);
    new2 = (option2flag & (~old2));
    

    
    if(new & MONOCULAR_MODE)
    {
        optionflag &= (~(LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
        optionflag |= new;
    }
    if(new & WURTZ_WHOLE_STIMULI || gone & WURTZ_WHOLE_STIMULI)
    {
        /*      SetWPanel();*/
    }
    
    if(new2 & PSYCHOPHYSICS_BIT && option2flag & PSYCHOPHYSICS_BIT)
        srandom(getpid());
    
    
    /*
     * only redraw the list and window if not in an experiment
     */
    if((old2 | new2) & (INTERLEAVE_BLANK | INTERLEAVE_MONOCULAR| INTERLEAVE_ONE_MONOCULAR | INTERLEAVE_UNCORRELATED | INTERLEAVE_RANDOM) && (mode & RUNNING) &&
       !(TheStim->mode & EXPTPENDING))
    {
        setstimuli(1);
        //Ali ListExpStims(NULL);
    }
    
    //ALi  if(new & STORE_WURTZ_BIT)
    //    SetRunButton(NULL);
    old = optionflag;
    old2 = option2flag;
}



int BackupStimFile()
{
    static int ctr = 0;
    int i = ctr;
    char cbuf[256];
    
    if (ctr & 3)
        i = 0;
    else if (ctr & 4)
        i = 1;
    else if (ctr & 8)
        i = 2;
    else if (ctr & 16)
        i = 3;
    else if (ctr & 32)
        i = 4;
    else if (ctr & 64)
        i = 5;
    
    sprintf(cbuf,"./lean%d.stm",i);
    SaveExptFile(cbuf,SAVE_STATE);
    ctr = (ctr+1)%16;
    return(i);
}

void SaveExptFile(char *filename,int flag)
{
	char cbuf[BUFSIZ],*s;
	char spacestr[BUFSIZ];

    
    int i,j,go = 0,code = 0,use = 0;
	FILE *ofd;
	static int write_fail_acknowledged;
    
	for(i = 0; i < BUFSIZ; i++)
        spacestr[i] = ' ';
	if(seroutfile){
        fprintf(seroutfile,"#Save %d %s VS%.1f\n",flag,filename,afc_s.sacval[1]);
	}
    printf("Saving expt %s (cwd is %s)\n",filename,getcwd(cbuf, BUFSIZ));
	if((ofd = fopen(filename,"w")) != NULL)
    {
	    cbuf[0] = '=';
	    cbuf[1] = 0;
	    if(flag != SAVE_STATE && flag != QUICK_SAVE)
            fprintf(ofd,"clearquick\n");
	    if(flag == SAVE_STATE)
            fprintf(ofd,"#State\n");
	    if(expt.mon->filename)
            fprintf(ofd,"monitor=%s\n",expt.mon->filename);
	    fprintf(ofd,"%s=0\n",serial_strings[MODE_CODE]);
	    if((j = MakeString(MODE_CODE, cbuf, &expt, TheStim, TO_FILE)) >= 0)
            fprintf(ofd,"%s\n",cbuf);
	    /* first clear optionflag, then write out the bits that are set */
	    fprintf(ofd,"op=0\n");
	    cbuf[0] = '=';
	    cbuf[1] = 0;
	    if((j = MakeString(OPTION_CODE, cbuf, &expt, TheStim, TO_FILE)) >= 0)
            fprintf(ofd,"%s\n",cbuf);
	    fprintf(ofd,"mo=fore\n");
	    for(i = 0; i <= LAST_STIMULUS_CODE; i++)
        {
            cbuf[0] = '=';
            cbuf[1] = 0;
            if((j = MakeString(i, cbuf, &expt, TheStim, TO_FILE)) >= 0)
                fprintf(ofd,"%s%.*s #%s\n",cbuf,15-strlen(cbuf),spacestr,serial_names[i]);
        }
	    cbuf[0] = '=';
		cbuf[1] = 0;
		if((j = MakeString(STIMULUS_FLAG, cbuf, &expt, TheStim, TO_FILE)) >= 0)
			fprintf(ofd,"%s\n",cbuf);
		if(TheStim->next != NULL && TheStim->next->type != STIM_NONE)
		{
			fprintf(ofd,"mo=back\n");
			for(i = 0; i <= LAST_STIMULUS_CODE; i++)
			{
				cbuf[0] = '=';
				cbuf[1] = 0;
				if((j = MakeString(i, cbuf, &expt, TheStim->next,TO_FILE)) >= 0)
                    fprintf(ofd,"%s%.*s #Back %s\n",cbuf,15-strlen(cbuf),spacestr,serial_names[i]);
            }
			cbuf[0] = '=';
			cbuf[1] = 0;
			if((j = MakeString(STIMULUS_FLAG, cbuf, &expt, TheStim, TO_FILE)) >= 0)
				fprintf(ofd,"%s%.*s #Back %s\n",cbuf,15-strlen(cbuf),spacestr,serial_names[i]);
		}
		if(TheStim->next != NULL && TheStim->next->next != NULL && TheStim->next->next->type != STIM_NONE)
		{
			fprintf(ofd,"mo=st3\n");
			for(i = 0; i <= LAST_STIMULUS_CODE; i++)
			{
				cbuf[0] = '=';
				cbuf[1] = 0;
				if((j = MakeString(i, cbuf, &expt, TheStim->next->next,TO_FILE)) >= 0)
                    fprintf(ofd,"%s%.*s #ST3 %s\n",cbuf,15-strlen(cbuf),spacestr,serial_names[i]);
            }
			cbuf[0] = '=';
			cbuf[1] = 0;
			if((j = MakeString(STIMULUS_FLAG, cbuf, &expt, TheStim, TO_FILE)) >= 0)
				fprintf(ofd,"%s\n",cbuf);
            
		}
		if(ChoiceStima->type != STIM_NONE){
			fprintf(ofd,"mo=ChoiceU\n");
			for(i = 0; i <= LAST_STIMULUS_CODE; i++)
			{
				cbuf[0] = '=';
				cbuf[1] = 0;
				if((j = MakeString(i, cbuf, &expt, ChoiceStima,TO_FILE)) >= 0)
                    fprintf(ofd,"%s%.*s #Choice 1 %s\n",cbuf,15-strlen(cbuf),spacestr,serial_names[i]);
            }
		}
		if(ChoiceStimb->type != STIM_NONE){
			fprintf(ofd,"mo=ChoiceD\n");
			for(i = 0; i <= LAST_STIMULUS_CODE; i++)
			{
				cbuf[0] = '=';
				cbuf[1] = 0;
				if((j = MakeString(i, cbuf, &expt, ChoiceStimb,TO_FILE)) >= 0)
                    fprintf(ofd,"%s%.*s #Choice 2 %s\n",cbuf,15-strlen(cbuf),spacestr,serial_names[i]);
            }
		}
        
		fprintf(ofd,"mo=fore\n");
        
		if(optionflag & CLAMP_HOLD_BIT)
			optionflag |= CLAMP_EXPT_BIT;
		for(i = LAST_STIMULUS_CODE; i < MAXSAVECODES; i++)
		{
			cbuf[0] = '=';
			cbuf[1] = 0;
			go = 0;
            code = valstringindex[i];
            use = !(valstrings[code].group & 512);
            if (use == 0)
                s = serial_strings[i];
			if((j = MakeString(i, cbuf, &expt, TheStim,TO_FILE)) >= 0 && use){
                switch(i){
                        
                        
                        /* 
                         * for some parameters, intended as convenienve options online,
                         * it is best not to save them to the file
                         * as loading them up just causes trouble
                         */
                    case STIM_SIZE:
                    case BACK_SIZE:
                    case QUERY_STATE:
                    case CONTRAST_LEFT:
                    case CONTRAST_RIGHT:
                    case SF_LEFT:
                    case SF_RIGHT:
                    case WIDTH_L:
                    case WIDTH_R:
                    case HEIGHT_L:
                    case HEIGHT_R:
                    case CONTRAST_PAIRS:
                    case SEND_CLEAR:
                    case SD_BOTH:
                    case FB_RELATIVE_CONTRAST:
                    case PHASE_AS_RELDISP:
                    case DISP_A:
                    case DISP_B:
                    case STIM_POLARANGLE:
                    case STIM_ECCENTRICITY:
                    case ORTHOG_POSR:
                    case ORTHOG_POSL:
                    case INITIAL_MOVEMENT:
                    case INITIAL_DURATION:
                        go = 0;
                        break;
                    case SACCADE_DETECTED:
                        if(flag == SAVE_STATE)
                            go = 1;
                        else
                            go = 0;
                        break;
                    case PHASE_AS_DISP:
                        if(expt.st->type == STIM_GRATINGN)
                            go = 1;
                        else
                            go = 0;
                        break;
                    case PENETRATION_TEXT:
                    case PENXPOS:
                    case PENYPOS:
                    case PENNUMCOUNTER:
                    case VWHERE:
                    case GOODTRIALS:
                    case ORTHOG_POS:
                    case PARA_POS:
                    case BADTRIALS:
                    case RELDISP:
                        if(flag == SAVE_STATE)
                            go = 1;
                        else
                            go = 0;
                        break;
                        /*
                         * This list NOT save in QuckExpts. Might want to change these during
                         * course of expt
                         */
                    case RF_X:
                    case RF_Y:
                    case RF_ANGLE:
                    case RF_WIDTH:
                    case RF_HEIGHT:
                    case QUICKEXPT_CODE:
                    case TRIGGER_LEVEL:
                    case REWARD_SIZE:
                    case CHANNEL_CODE:
                    case SOFTOFF_CODE:
                    case UFF_PREFIX:
                    case EARLY_RWTIME:
                    case STIMID:
                    case ELECTRODE_DEPTH:
                        if(flag == QUICK_SAVE) //Record expt state
                            go = 0;
                        else
                            go = 1;
                        break;
                    case SET_SEED:
                        if(flag == SAVE_STATE ||
                           (flag == QUICK_SAVE && TheStim->left->baseseed <= 1000))
                            go = 1;
                        else
                            go = 0;
                        break;
                    default:
                        go = 1;
                        break;
                        
                }
                if(go)
                    fprintf(ofd,"%s\t#%s\n",cbuf,valstrings[code].label);
			}
		}
		if(expt.st->imprefix != NULL){
            fprintf(ofd,"impref=%s\n",expt.st->imprefix);
            if(expt.st->immode == IMAGEMODE_ORBW)
                fprintf(ofd,"immode=orbw\n");
		}
        /*
         * used not to write_expvals into QUICKexps, but I _think_ this is only
         * becuase this included qe= lines, which we do not want. The other lines,
         * for custom EXPVALs are wanted
         */
        
		write_expvals(ofd,flag);
		if(expt.hemisphere)
            fprintf(ofd,"RightHemisphere\n");
		if(flag != QUICK_SAVE){
            write_menus(ofd);
            write_helpfiles(ofd);
		}
        

		if(flag == SAVE_STATE){
            for(i = 0; i < rfctr; i++)
                fprintf(ofd,"oldrf %.2f %.2f %d %d %.2f\n",oldrfs[i].pos[0],oldrfs[i].pos[1],oldrfs[i].size[0],oldrfs[i].size[1],oldrfs[i].angle);
		}

		if(expt.st->type == STIM_IMAGE){
            if(expt.st->preload)
                fprintf(ofd,"imload=preload\n");
            else
                fprintf(ofd,"immload=load\n");
            if(expt.st->immode == IMAGEMODE_ORBW)
                fprintf(ofd,"immode=orbw\n");
            else
                fprintf(ofd,"immode=plain\n");
		}
		
		fclose(ofd);
        
	}
	else
	{
        if(!write_fail_acknowledged){
            sprintf(cbuf,"can't open %s",filename);
            acknowledge(cbuf,NULL);
            write_fail_acknowledged++;
        }
	}
}

void SaveExpt()
{
	char *filename,cbuf[256];
	int i,j;
    
	gettimeofday(&timeb,NULL);
	SaveExptFile(filename,0);
	loadfiles[0] = myscopy(loadfiles[0],filename);
	gettimeofday(&timec,NULL);
	if(testfd != NULL)
        fprintf(testfd,"Saving %s, %.3f seconds\n",filename,timediff(&timec,&timeb));
	sprintf(cbuf,"Setup Saved in %s",filename);
	statusline(cbuf);
}





void select_stimulus(int type)
{
    switch(type)
    {
        case STIM_FOREGROUND:
            stimptr = TheStim;
            break;
        case STIM_BACKGROUND:
            if(TheStim->next == NULL)
            {
                NewStimulus(TheStim);
                StimulusType(TheStim->next, STIM_NONE);
                TheStim->next->prev = TheStim;
                TheStim->next->splane = 0x2;
            }
            stimptr = TheStim->next;
            StimulusType(stimptr,stimptr->type);
            CheckRect(TheStim);
            break;
        case STIM_NUM3:
            if(TheStim->next == NULL)
            {
                NewStimulus(TheStim);
                StimulusType(TheStim->next, STIM_NONE);
                TheStim->next->prev = TheStim;
                TheStim->next->splane = 0x2;
            }
            if(TheStim->next->next == NULL)
            {
                NewStimulus(TheStim->next);
                StimulusType(TheStim->next->next, STIM_NONE);
                TheStim->next->next->prev = TheStim->next;
                TheStim->next->next->splane = STIM_THIRD_BIT;
            }
            stimptr = TheStim->next->next;
            StimulusType(stimptr,stimptr->type);
            CheckRect(TheStim);
            break;
        case STIM_CHOICE1:
            stimptr = ChoiceStima;
            StimulusType(stimptr,stimptr->type);
            break;
        case STIM_CHOICE2:
            stimptr = ChoiceStimb;
            StimulusType(stimptr,stimptr->type);
            break;
    }
 
}


void CheckMode(int cmode)
{
	int new;
	Locator *pos = &TheStim->pos;
    
	switch(cmode)
	{
        case COLORRGB:
            rightshift = 8;
            break;
        case COLOR6BIT:
            rightshift = 6;
            break;
        default:
            rightshift = 0;
            break;
	}
}

float timediff(struct timeval *a, struct timeval *b)
{
	float res;
	if(a->tv_sec == 0 || b->tv_sec == 0)
        return(-1000);
	res = a->tv_sec - b->tv_sec + (a->tv_usec - b->tv_usec)/1000000.0;
	return(res);
}

void addtime(struct timeval *a, struct timeval *b, float sec)
{
	int wholesec = floor(sec);
    
	b->tv_sec = a->tv_sec + wholesec;
	b->tv_usec = a->tv_usec + rint(((sec-wholesec) * 1000000.0));
	if(b->tv_usec > 1000000)
	{
		b->tv_usec -= 1000000;
		b->tv_sec++;
	}
}


/*
 * GLblock(1210000) (1.21 million) causes a pause of 1 sec.
 * = 20k delays 1 frame
 */

void GLblock(int n)
{
	int i;
	short x[2];
    
	SetColor(TheStim->background,1);
	glBegin(GL_POINTS);
	for(i = 0; i < n; i++)
	{
		x[0] = 0;
		x[1] = 0;
		glVertex2s(x[0],x[1]);
	}
	glEnd();
	glFinishRenderAPPLE(); /* wait to finish */
}

void WurtzTrial()
{
	int i,len;
	if(monkeypress == WURTZ_STOPPED)
        return;
    
    
    stmode &= (~MOVED_STIMULUS);
	TheStim->mode |= (NEW_FIXATION_BIT);
	if((TheStim->mode & EXPTPENDING)|| mode & ANIMATE_BIT)
		TheStim->mode |= TRIAL_PENDING;
    
	TheStim->fix.state = FIX_IS_UP;
	TheStim->fixcolor = TheStim->fix.fixcolor;
	lastwurtzcount = wurtzcount = 0;
	if(TheStim->fix.stimvar > 0)
        duration = TheStim->fix.fixframes + random() % TheStim->fix.stimvar;
	else
        duration = TheStim->fix.fixframes;
	if(optionflag & WURTZ_WHOLE_STIMULI)
	{
		ntrialstim = (int)(0.5 + (float)(duration)/(TheStim->nframes));
        /* 26 July changed so that just don't count wurtzcout up during isi */
		duration = (TheStim->nframes);
	}
	else
        stimstate = PRESTIMULUS;
	realframecount = framecount = 0;
}

float StimDuration()
{
    float val = timediff(&endstimtime,&firstframetime);
    return(val);
}

float StimTime(struct timeval *event)
{
    float val = timediff(event,&firstframetime);
    return(val);
}

int ShowTrialCount(float down, float sum)
{
    int i,*iptr;
    char result = '0',buf[256];
    float val;
    
    
	if(sum < 0)
    {
	    if(wurtzctr > avglen)
        {
            iptr = &fixed[wurtzctr-avglen];
            sum = 0;
            for(i = 0; i < avglen; i++)
                if(*iptr++ == (int)WURTZ_OK)
                    sum += 1.0;
        }
        else
            sum = 0;
    }
	else{
	    if(wurtzctr > avglen)
            iptr = &fixed[wurtzctr-avglen];
	    else
            iptr = &fixed[0];
	    sum = 0;
	    for(i = 0; i < avglen && i < wurtzctr; i++)
            if(*iptr++ == (int)WURTZ_OK)
                sum += 1.0;
	}
	val = StimDuration();
    if (val < 0)
        val = timediff(&now, &firstframetime);
	if(debug)
        sprintf(mssg,"%s(%d) Frames: %d/%d (%.3f sec) %d/%d %d late %d bad (%.2f), %0f/%d",binocTimeString(),ufftime(&now),framesdone,TheStim->nframes,val,goodtrials,totaltrials,
                totaltrials-(goodtrials+fixtrials),fixtrials,down,sum,avglen);
	else
        sprintf(mssg,"%s Frames: %d/%d (%.3f sec) %d/%d %d late %d bad (%.2f), %.0f/%d",binocTimeString(),framesdone,TheStim->nframes,val,goodtrials,totaltrials,
                totaltrials-(goodtrials+fixtrials),fixtrials,down,sum,avglen);
    
    
    sprintf(buf," rw%.1f ",totalreward);
    strcat(mssg,buf);
	if(TheStim->mode & EXPTPENDING)
    {
	    sprintf(buf,"Ex: %d/%d",stimno,expt.nreps*expt.nstim[5]);
	    strcat(mssg,buf);
    }
    strcat(mssg,resbuf);
	if(endbadctr)
    {
	    sprintf(buf,"Bad: %d",endbadctr);
	    strcat(mssg,buf);
    }
	if(optionflag & FRAME_ONLY_BIT)
    {
	    sprintf(mssg,"Ex: %ld/%d %.1f +- SD %.2f %d,%d outliers",frametotal,expt.nreps*expt.nstim[5],framemean,framesd,outliers[0],outliers[1]);
    }
	statusline(mssg);
	return(0);
}

int ShowLastCodes()
{
    int i;
    char *s, buf[BUFSIZ];
    
    if((s = CheckSerialInput(100)) != NULL){
        strcpy(buf,s);
        for(i = 0; i < 100; i++){
            if(buf[i] == END_STIM)
                buf[i] = '$';
        }
        fprintf(seroutfile,"Input%s\n",buf);
    }
    fprintf(seroutfile,"Codes: ");
    for(i = 0; i < CODEHIST; i++)
        fprintf(seroutfile," %d",lastcodes[(codectr-i)%CODEHIST]);
    fprintf(seroutfile,"\n Outcodes:");
    for(i = 0; i < CODEHIST; i++)
        fprintf(seroutfile," %d",outcodes[(outctr-i)%CODEHIST]);
    fprintf(seroutfile,"\n");
    return(lastcodes[codectr]);
}

static char lastchar = 0;
void ShowConjugReadState(char *line)
{
    char *s,buf[BUFSIZ];
    int i;
    
    return; //don't need this any more
    fprintf(stderr,"Char %d (%d) last conjug %d buf %s.last%c\n",totalchrs,
            strlen(line),conjctr,conjbuf,lastchar);
    if((s = CheckSerialInput(100)) != NULL){
        fprintf(stderr,"S okay\n");
        fprintf(stderr,"%s\n",s);
        strcpy(buf,s);
        for(i = 0; i < 100; i++){
            if(buf[i] == CONJUG_OUT)
                buf[i] = '$';
        }
        fprintf(stderr,"Input %s\n",buf);
    }
    fprintf(stderr,"Buf %s\n",charbuf);
}


double  RunTime(void )
{
    double t;
    gettimeofday(&now, NULL);
    t = timediff(&now,&progstarttime);
    return(t);
    
}
#define MBUFLEN 512
int GotChar(char c)
{
	int i,res = 0,*iptr;
	float down, *ptr,t1,t2,t3,val,start,sacval[4],sacdir,sacsiz;
	float choiceaxis,tansac;
	static float intended;
	char result = '0',buf[MBUFLEN],str[BUFSIZ],xresult = 0;
	static int cctr = 0,lastcharcnt;
	int jonresult,presult =-1;
	int stim_direction;
	int topline;
	int monkey_dir,x,y,aid;
	float oldrw,sacth;
	int sign,code,trueafc = 0;
    
	totalchrs++;
    
	if(c == '\n')
	{
        if(strncmp(charbuf,"CLOSING",6) == NULL)
            ReopenSerial();
        else if (!strncmp(charbuf,"SavedOnlineData",10)){
            if (optionflags[ONLINE_DATA])
                SerialString("WriteOnlineData\n",0);
        }
        else
            code = InterpretLine(charbuf,&expt,1);
		charctr = 0;
		if(code == SACCADE_DETECTED){
            sacth = expt.vals[SACCADE_DETECTED];
            if(sacth < 0.01)
                sacth = 1000;
            /* BW sends H, V components   */ 
            sscanf(charbuf,"%*2s%f %f",&sacval[0],&sacval[1]);
            /*
             * need to reverse H component even to get axis corrrect for oblique choice axes
             * NB this assumes mirrors - no option for No Mirror flag
             */
            sacdir = atan2(sacval[1],-sacval[0]);
            sacsiz = sqrt(sqr(sacval[1])+sqr(sacval[0]));
            val = timediff(&now,&wurtzstart);
            if (fabs(expt.vals[SACCADE_PUNISH_DIRECTION]) > 0.1){
                choiceaxis = deg2rad(expt.vals[SACCADE_PUNISH_DIRECTION]);
            }
            else{
                choiceaxis = atan2(afc_s.abssac[1],afc_s.abssac[0]);
            }
            /* tansac < 1 = dir of saccade within 45 degreees of choice axis */
            tansac = fabs(tan(sacdir-choiceaxis));
            if(seroutfile){
                fputs(charbuf,seroutfile);
                fprintf(seroutfile," Sa%2f(%.2f),%.2f (%.2f,%d,%d)\n",sacsiz,sacth,sacdir,val,stimstate,expt.allstimid);
            }
            // if experimenter is moving fixation point, don't punish saccades!
            if(val > 0.2 && sacsiz > sacth && tansac  < 1 && stimstate == INSTIMULUS && expt.vals[FP_MOVE_SIZE] == 0){
                if(seroutfile){
                    fprintf(seroutfile,"#Saccade\n");
                }
                microsaccade = sacsiz;
                microsaccdir = sacdir;
                fixstate = SACCADE_DETECTED;
                c = BAD_FIXATION;
                write(ttys[0],&c,1);
                GotChar(BAD_FIXATION);
                sprintf(buf,"%.2s:%.2f %.2f\n",serial_strings[SACCADE_DETECTED],sacsiz,sacdir);
                statusline(buf);
                SerialString(buf,0);
                for( i = 0; i < strlen(charbuf); i++)
                    charbuf[i] = 0;
                return(BAD_FIXATION);
            }
		}
		for( i = 0; i < strlen(charbuf); i++)
		    charbuf[i] = 0;
	}
	else if(c== START_EXPT){ /* this is sent when BW starts up send everything */
        
#ifdef NIDAQ  
        // trigger data collection for Spike2
//	    DIOWrite(0x7);
        DIOval = 0;
        DIOWriteBit(0,1);
        DIOWriteBit(2,1);

	    fsleep(0.01);
        DIOWriteBit(0,0);
        DIOWriteBit(2,0);
//   	    DIOval = 0;  DIOWrite(0);
#endif
		MakeConnection();
	}
    
	else if(c == CONJUG_OUT){
        if(debug & DEBUG_CONJ_TIME){
            gettimeofday(&now,NULL);
            fprintf(stderr,"CT %.4f\n",timediff(&now,&cjtime));
            memcpy(&cjtime,&now, sizeof(struct timeval));
        }
        conjctr = totalchrs;
        for(i = 0; i < 4; i++)
            conjbuf[i] = 0;
        conjbufctr = 0;
	}
	else if(conjbufctr >= 0 && conjbufctr < 4){
        conjbuf[conjbufctr++] = c;
        if(c == CONJUG_OUT){
            if(seroutfile){
                gettimeofday(&now,NULL);
                fprintf(seroutfile,"#Double Sj at %d",ufftime(&now));
            }
            conjbufctr = 0;
        }
        if(conjbufctr == 4){
            i = 0;
            lastchar = c;
            charctr = 0;
            x = ((conjbuf[1]-64) << 6) | ((conjbuf[0]-64));
            y = ((conjbuf[3]-64) << 6) | ((conjbuf[2]-64));
            if(optionflags[NO_MIRRORS])
                conjpos[0] = x - 1024;
            else
                conjpos[0] = 1024 - x;
            conjpos[1] = y - 1024;
            conjbufctr = -1;
        }
	}
	else if(c == SPIKES_OUT){
        i = 0;
        c = 0;
        while(c != SPIKES_OUT){
            if((c = ReadSerial(ttys[0])) != MYEOF)
                conjbuf[i++] = c;
        }
	}
	else if(isserialcode(c) && monkeypress != WURTZ_STOPPED)
	{
#ifdef DEBUG
        printf("Code %d\n",(int)c);
#endif
		switch(c)
		{
            case 10: /* = '\n' -  dont use it */
                break;
            case BAD_TRIAL_STATE:
                if(seroutfile){
                    fprintf(seroutfile,"#BadState:");
                    ShowLastCodes();
                }
                break;
            case CONJUG_OUT:
                cctr = 3;
                fprintf(stderr,"Conjug read should not be here!\n");
                break;
            case WURTZ_STOPPED:
                TheStim->fix.state = FIX_IS_OFF;
                TheStim->fixcolor = TheStim->fix.offcolor;
                monkeypress = -2;
                TheStim->mode &= (~(INTRIAL));
                TheStim->mode |= (NEW_FIXATION_BIT);
                res = c;
                break;
            case END_STIM:
                i = codectr;
                break;
            case START_STIM:
                fixstate = GOOD_FIXATION;
                SerialSend(GOOD_FIXATION);
                /* 
                 * for intial training, give one reward as soon as fixating
                 */
                
                /*
                 * apr 99 don't need any of this with the new
                 * structure. START_STIM from BW jsut says that
                 * monkey is fixating after hearing that fix is on.
                 * the next_frame loopt deals with
                 *  everything else
                 */
                if((TheStim->type == STIM_RDS || TheStim->type == STIM_RADIAL) && (i = (int)rint(expt.vals[CHANGE_SEED])) > 0)
                {
                    sprintf(buf,"%2s=%2s\n",serial_strings[STIMCHANGE_CODE],
                            serial_strings[CHANGE_SEED]);
                    SerialString(buf,0);
                }
                else if(TheStim->type == STIM_RDS && (i = (int)rint(expt.vals[BACKJUMP])) > 0)
                {
                    sprintf(buf,"%2s=%2s\n",serial_strings[STIMCHANGE_CODE],
                            serial_strings[BACKJUMP]);
                    SerialString(buf,0);
                }
                else  if(expt.vals[FP_MOVE_SIZE] > 0)
                {
                    if(optionflags[SIMULATE_FP_MOVE])
                        i = 2;
                    else
                        i = 0;
                    sprintf(buf,"%2s=%2s\n",serial_strings[STIMCHANGE_CODE],
                            jumpstrings[i]);
                    SerialString(buf,0);
                }
                else if(optionflags[RUN_SEQUENCE] && expt.stimpertrial > 2){
                    sprintf(buf,"%2s=%2s\n",serial_strings[STIMCHANGE_CODE],
                            serial_strings[RC_STIMCHANGE]);
                }
                
                
                TheStim->fixcolor = TheStim->fix.fixcolor;
                TheStim->fix.state = FIX_IS_UP;
                gettimeofday(&wurtzstart,NULL);
                WurtzTrial();
                intended = (float)(duration)/mon.framerate;
                if((option2flag & EXPT_INTERACTIVE))
                    SetStimulus(TheStim,90.0,SETPHASE,NOEVENT);
                else
                    SetStimulus(TheStim,TheStim->vals[START_PHASE],SETPHASE,NOEVENT);
                //			framesdone=0;
                drag = 0;
                break;
            case END_PREM:
                /*			if(monkeypress != WURTZ_STOPPED)
                 end_timeout();*/
                break;
            case BAD_FIXATION:
                if (fixstate == BADFIX_STATE) // Already received code
                    break;
                fixstate = BADFIX_STATE;
            case WURTZ_LATE:
            case WURTZ_OK:
            case WURTZ_OK_W:
#ifdef MONITOR_CLOSE
#endif		  
                if(seroutfile)
                    fprintf(seroutfile,"#Gch%d\n",(int)(c));
                gettimeofday(&endtrialtime, NULL);
                
#ifdef NIDAQ
                DIOWriteBit(2,  0);
                DIOWriteBit(1,  0);
                DIOWriteBit(0,  0);
//                DIOWrite(0); DIOval = 0;
#endif
                expstate = c;
                gettimeofday(&now,NULL);
                if(c != BAD_FIXATION)
                    fixstate = RESPONDED;
                if(fixed == NULL){
                    fixed = (int *)malloc(sizeof(int) * (wurtzbufferlen));
                    fixy = (float *)malloc(sizeof(float) * (wurtzbufferlen));
                    fixx = (float *)malloc(sizeof(float) * (wurtzbufferlen));
                }
                fixed[wurtzctr] = (int)c;
                fixx[wurtzctr] = expt.stimvals[FIXPOS_X];
                fixy[wurtzctr] = expt.stimvals[FIXPOS_Y];
                
                fixed[wurtzctr+1] = -1;
			    if(c== WURTZ_OK){
                    if(goodtrials %50 == 0 && goodtrials > 0 && penlog)
                        fprintf(penlog,"Rewards %d of %d\n",goodtrials,totaltrials);
                    goodtrials++;
                    if( (afc_s.sacval != 0) && (option2flag & AFC)){
                        afctrials++;
                        afc_s.lasttrial = c;
                    }
                    totalreward += expt.vals[REWARD_SIZE];
                }
			    else if(c==BAD_FIXATION)
                {
                    badtrials++;
                }
			    else if(c==WURTZ_OK_W){ /* can only get when sacval != 0 */
                    wrongtrials++;
                    if (option2flag & AFC)
                        afctrials++;
                    afc_s.lasttrial = c;
			    }
                totaltrials++;
                srandom(totaltrials);
                
                trialdur = down = timediff(&now,&wurtzframetime);
                trialdur = StimTime(&now);
                trialdursum += trialdur;
                
                if(c== WURTZ_OK){
                    if(option2flag & AFC) 
                        result = 'G';
                    else
                        result = 'F';
                    jonresult = CORRECT;
                    presult = 1;
                    trialdursum = 0;
                }
                else if(c==BAD_FIXATION){
                    result = 'B';
                    jonresult = FOUL;
                    presult = 3;
                    if(optionflags[COUNT_BADFIX_FOR_BONUS])
                        afc_s.goodinarow = 0;
                    val = expt.st->nframes/mon.framerate;
                    if (trialdursum > val && expt.stimmode ==BUTTSEXPT){
                        stimno++;
                        if (seroutfile)
                            fprintf(seroutfile,"Got %.2f sec. Moving on to %d\n",trialdursum,stimno);
                        trialdursum = 0;
                    }
                }
                else if(c== WURTZ_LATE || fixstate == WURTZ_LATE){
                    result = 'L';
                    jonresult = FOUL;
                    presult = 2;
                    latetrials++;
                    trialdursum = 0;
                    
                }
                else if(c== WURTZ_OK_W){
                    result = 'W';
                    jonresult = WRONG;
                    presult = 0;
                    trialdursum = 0;
                    /*				start_timeout(c);*/
                }
                else
                    result = '?'; /* shouldn't happen */
                
                trialdur = down = timediff(&now,&wurtzframetime);
                start = timediff(&now,&progstarttime);
                /* 
                 * now saccades can be in any direction, need to take into account
                 * abssacval
                 */
                if(fabs(afc_s.sacval[0]) > fabs(afc_s.sacval[1]))
                    aid = 0;
                
                else
                    aid = 1;
                if(afc_s.newdirs)
                    sign = afc_s.stimsign;
                else
                    sign = (afc_s.sacval[aid] *  afc_s.abssac[aid])*afc_s.sign;
                if(sign < 0)
                    sign = -1;
                else if(sign > 0)
                    sign = 1;
                else
                    sign = 0;
                if(psychfile != NULL){
                    if(afc_s.loopstate == CORRECTION_LOOP && (option2flag & AFC))
                        presult += 100;
                    if(!SACCREQD(afc_s))
                        presult +=50;
                    if(abs(afc_s.ccvar) > 0.01 && expt.type2 == EXPTYPE_NONE)
                        fprintf(psychfile,"R%d %s=%.5f %s=%.5f",
                                presult,serial_strings[expt.mode],expt.currentval[0], 
                                serial_strings[covaryprop],GetProperty(&expt,expt.st,covaryprop));
                    else if(expt.type3 == FAKESTIM_EXPT && expt.type2 == EXPTYPE_NONE)
                        fprintf(psychfile,"R%d %s=%.5f %s=%.5f",
                                presult,serial_strings[expt.mode],expt.currentval[0], 
                                serial_strings[FAKESTIM_SIGNAL],fakestim);
                    else if(expt.currentval[0] == INTERLEAVE_EXPT_FLIP)
                        fprintf(psychfile,"R%d %s=%.5f %s=%.5f",
                                presult,serial_strings[expt.mode],expt.currentval[0], 
                                serial_strings[expt.type2],expt.currentval[3]);
                    else
                        fprintf(psychfile,"R%d %s=%.5f %s=%.5f",
                                presult,serial_strings[expt.mode],expt.currentval[0], 
                                serial_strings[expt.type2],expt.currentval[1]);
                    fprintf(psychfile," sn=%d %.2f %.2f %.2f",sign,start,down,expt.vals[REWARD_SIZE]);
                    if(microsaccade >0)
                        sprintf(str,"%s(%,4f)=%.2f",serial_strings[SACCADE_DETECTED],microsaccdir, microsaccade); 
                    else
                        sprintf(str,"%s=%d",serial_strings[SET_SEED],expt.st->left->baseseed);
                    sprintf(str,"%s %s",str,EyePosString());
                    if(expt.type3 != EXPTYPE_NONE)
                        fprintf(psychfile," %s=%.2f %s\n",serial_strings[expt.type3],expt.currentval[2],str);
                    else if(expt.mode == TWOCYL_DISP)
                        fprintf(psychfile," %s=%.2f %s\n",serial_strings[DISP_X],expt.currentval[2],str);
                    else if(expt.vals[ONETARGET_P] > 0)
                        fprintf(psychfile," %s=%.2f %s\n",serial_strings[TARGET_RATIO],expt.vals[TARGET_RATIO],str);
                    else
                        
                        fprintf(psychfile," %s %s=%d\n",str,serial_strings[STIMID],expt.allstimid);
                    fflush(psychfile);
                }
                if(seroutfile != NULL) 
                {
                    if(c == WURTZ_OK || c == WURTZ_OK_W){
                    }
                    if(expt.flag & TIMES_EXPT2)
                        fprintf(seroutfile,"R%c %s=%.5f %s=%.5f",
                                result,serial_strings[expt.mode],expt.currentval[0],
                                serial_strings[expt.type2],expt.currentval[1]);
                    else if(abs(afc_s.ccvar) > 0.01)
                        fprintf(seroutfile,"R%c %s=%.5f %s=%.5f",
                                result,serial_strings[expt.mode],expt.currentval[0],
                                serial_strings[covaryprop],GetProperty(&expt,expt.st,covaryprop));
                    else if(expt.type2 == OPPOSITE_DELAY &&expt.flag & ADD_EXPT2){
                        fprintf(seroutfile,"R%c %s=%.5f %s=%.5f",
                                result, serial_strings[expt.mode],expt.currentval[1],serial_strings[SEED_DELAY],GetProperty(&expt,expt.st,SEED_DELAY));
                    }
                    else if(expt.flag & ADD_EXPT2 && expt.expno != EXP_FIRST)
                    {
                        fprintf(seroutfile,"R%c %s=%.5f",
                                result, serial_strings[expt.type2],expt.currentval[1]);
                    }
                    else
                        fprintf(seroutfile,"R%c %s=%.5f",
                                result, serial_strings[expt.mode],expt.currentval[0]);
                    fprintf(seroutfile," sn=%d",sign);
                    buf[0] = '=';
                    buf[1] = 0;
                    if((i =MakeString(STIMULUS_FLAG, buf,&expt, TheStim, TO_FILE)) >= 0)
                        fprintf(seroutfile," %s",buf);
                    if(optionflags[MICROSTIM])
                        fprintf(seroutfile,"+%2s",togglestrings[toggleidx[MICROSTIM]].code);
                    buf[0] = '=';
                    buf[1] = 0;
                    if((i =MakeString(STIMULUS_TYPE_CODE, buf,&expt, TheStim, TO_FILE)) >= 0)
                        fprintf(seroutfile," %s",buf);
                    
                    fprintf(seroutfile," %.2f %.2f %.2f",start,down,expt.vals[REWARD_SIZE]);
                    if(expt.type2 == DISTRIBUTION_CONC || expt.mode == DISTRIBUTION_CONC)
                        fprintf(seroutfile,"%2s=%.2f,%2s=%.0f,%2s=%.2f,%d ",
                                serial_strings[DISTRIBUTION_MEAN],expt.vals[DISTRIBUTION_MEAN],
                                serial_strings[DISTRIBUTION_WIDTH],expt.vals[DISTRIBUTION_WIDTH],
                                serial_strings[RC1INC],expt.vals[RC1INC],currentstim.seqseed);
                    fprintf(seroutfile," se=%d",expt.st->left->baseseed);
                    
                    if(afc_s.loopstate == CORRECTION_LOOP && (option2flag & AFC))
                        fprintf(seroutfile," Corloop");
                    if (microsaccade > 0)
                        fprintf(seroutfile," Saccade");
                    if (!(expt.st->mode & EXPTPENDING))// mark lines not in expt
                        fprintf(seroutfile,"*");
                    fprintf(seroutfile,"\n");
                    fflush(seroutfile);
                }
                /*
                 * if the monkey makes a bad saccade after the stimulus is done,
                 * or a late saccade, need to re-run that stimulus. This is taken
                 * care of for premature BAD_FIX's in exploop, but when the stimulus
                 * apparently finished OK need to take special action
                 */
                
                if(jonresult == FOUL && option2flag & AFC && TheStim->fix.state != FIX_IS_UP && TheStim->mode & EXPTPENDING){
                    stimno--;
                }
                if(wurtzctr >= avglen)
                {
                    iptr = &fixed[wurtzctr-avglen+1];
                    wsum = 0;
                    for(i = 0; i < avglen; i++)
                        if(*iptr++ == (int)WURTZ_OK)
                            wsum += 1.0;
                    wsum = wsum/avglen;
                    if(wsum < stopcriterion && ++avctr >= avglen &&
                       c != WURTZ_OK)
                    {
                        SetStopButton(STOP);
                        avctr = 0;
                    }
                }
                else
                    avctr = wurtzctr;
  //              ShowTrialCount(down, wsum);
                if(downtimes == NULL)
                    downtimes = (float *)malloc(sizeof(float) * (wurtzbufferlen));
                if(stimtimes == NULL)
                    stimtimes = (float *)malloc(sizeof(float) * (wurtzbufferlen));
                if(starttimes == NULL)
                    starttimes = (float *)malloc(sizeof(float) * (wurtzbufferlen));
                downtimes[wurtzctr] = down;
                
                
                down = timediff(&lastframetime,&wurtzframetime);
                if(down < 0) /* premature */
                    down = intended;
                stimtimes[wurtzctr] = down;
                starttimes[wurtzctr]= timediff(&wurtzframetime, &sessiontime);
                
                /*
                 * after writing, close and reopen logfd to make sure
                 * the timestamp is altered, allowing external programs to 
                 * check for changes
                 */
                if(logfd){
                    fprintf(logfd,"%.1f %c %.3f%s\n",starttimes[wurtzctr],result,downtimes[wurtzctr],binocTimeString());
                    fflush(logfd);
                    /*
                     close(logfd);
                     logfd = fopen(logname,"a");
                     */
                }
		        wurtzctr++;
                /*
                 *  Alloc new memory early so that writing, for example -1 to fixed[ctr+1] does
                 * not overwrite buffer end.
                 */
                if(wurtzctr >= (wurtzbufferlen-5))
                {
                    ptr = (float *)malloc(sizeof(float) * (wurtzbufferlen + 512));
                    memcpy(ptr,downtimes,sizeof(float) * wurtzbufferlen);
                    free(downtimes);
                    downtimes = ptr;
                    ptr = (float *)malloc(sizeof(float) * (wurtzbufferlen + 512));
                    memcpy(ptr,stimtimes,sizeof(float) * wurtzbufferlen);
                    free(stimtimes);
                    stimtimes = ptr;
                    ptr = (float *)malloc(sizeof(float) * (wurtzbufferlen + 512));
                    memcpy(ptr,starttimes,sizeof(float) * wurtzbufferlen);
                    free(starttimes);
                    starttimes = ptr;
                    
                    ptr = (float *)malloc(sizeof(float) * (wurtzbufferlen + 512));
                    memcpy(ptr,fixx,sizeof(float) * wurtzbufferlen);
                    free(fixx);
                    fixx = ptr;
                    
                    ptr = (float *)malloc(sizeof(float) * (wurtzbufferlen + 512));
                    memcpy(ptr,fixy,sizeof(float) * wurtzbufferlen);
                    free(fixy);
                    fixy = ptr;
                    
                    
                    iptr = (int *)malloc(sizeof(int) * (wurtzbufferlen + 512));
                    memcpy(iptr,fixed,sizeof(int) * wurtzbufferlen);
                    free(fixed);
                    fixed = iptr;
                    wurtzbufferlen += 512;
                }
  

                if(SACCREQD(afc_s)){
                    afc_s.jlaststairval = afc_s.jstairval; /* this is before nextval changes */ 
                    if(afc_s.type == MAGONE_SIGNTWO){
                        if(afc_s.jsignval < 0)
                            stim_direction = JONLEFT;
                        else if(afc_s.jsignval > 0)
                            stim_direction = JONRIGHT;
                        else
                            stim_direction = AMBIGUOUS;
                    }
                    else
                        stim_direction = find_direction(afc_s.jlaststairval * afc_s.sign);/*j finds if should have been left or right. NB -1 is up/right*/
                    if (expt.vals[TARGET_RATIO] > 0.99)
                        trueafc = 1;
                    monkey_dir = monkey_direction(jonresult, afc_s);
                    if(seroutfile)
                        fprintf(seroutfile,"MD %d %d %.1f %.3f",monkey_dir,jonresult,afc_s.sacval[1],timediff(&now,&endstimtime));
                    if(afc_s.loopstate != CORRECTION_LOOP && (option2flag & AFC))
                    {
                        inc_psychdata(monkey_dir, &expt, expt.stimno); 
                        afccounters(stim_direction, jonresult);
                    }
                    else{
                        if(seroutfile)
                            fprintf(seroutfile,"MX %d\n",afc_s.loopstate);
                    }
                    /*    
                     * if correlation is 0, dont coun't "errors" towards correction loop
                     */
                    
                    if(expt.mode == DISP_X && expt.type2 == CORRELATION && expt.vals[CORRELATION] == 0)
                        afc_s.loopstate = loopstate_counters(AMBIGUOUS, jonresult);
                    else
                        afc_s.loopstate = loopstate_counters(stim_direction * afc_s.sign, jonresult);
                    if(option2flag & PERF_STRING)
                        performance_string(afc_s.jlaststairval, jonresult, afc_s.loopstate, &afc_s.performance_1, &afc_s.performance_2, (afc_s.sacval[0] + afc_s.sacval[1]));
                    if(option2flag & STAIRCASE){ 
                        SetMonkeyStaircase(jonresult, &afc_s);
                        if(STAIROUT)
                            printstair(afc_s.stairhist.staircounter,  afc_s.stairhist.startbin,  afc_s.stairhist.lowbin, afc_s.stairhist.highbin, afc_s.stairhist.histstructure);
                    }    	
                    /*j NB if not in staircase mode gets value in  from the experiment settings (val) */
                    if (trueafc)
                        sprintf(resbuf," %c%d",result,monkey_dir);
                    else
                        sprintf(resbuf," %c%d tr<1",result,monkey_dir);
                        

                }
                else{
                    monkey_dir = 0;
                    sprintf(resbuf," %c%d",result,monkey_dir);
                }
                sprintf(buf,"TRES %c%d %d\n",result,monkey_dir,trueafc);
                notify(buf);

                res = (int)c;
                
                /*
                 * if the monkey gets n right in a row, put up the
                 * if show reward bias is set, biased reward may be an expt type
                 * reward size
                 */
                oldrw = TheStim->fix.rwsize;
                if(option2flag & AFC && expt.biasedreward == 0)
                {
                    if(optionflags[SHOW_REWARD_BIAS])
                        TheStim->fix.rwsize = expt.vals[REWARD_SIZE3];
                    else if(afc_s.goodinarow >= (afc_s.bonuslevel2) && afc_s.bonuslevel2 > 0)
                        TheStim->fix.rwsize = expt.vals[REWARD_SIZE3];		 
                    else if(afc_s.goodinarow >= (afc_s.bonuslevel) && afc_s.bonuslevel > 0)
                        TheStim->fix.rwsize = expt.vals[REWARD_SIZE2];
                    else if (afc_s.bonuslevel == 0 && afc_s.bonuslevel2 == 0)
                        TheStim->fix.rwsize = oldrw;
                    else
                        TheStim->fix.rwsize = expt.vals[REWARD_SIZE1];
                }
                else if(option2flag & AFC){
                }
                else if(rndbonus > 0 && (i = random())%rndbonus == 0 && !optionflags[INITIAL_TRAINING]){
                    TheStim->fix.rwsize = oldrw * 3;
                }
                SerialSend(REWARD_SIZE);
                expt.vals[REWARD_SIZE] = TheStim->fix.rwsize;
                TheStim->fix.rwsize = oldrw;		    
                
                if(stimstate == WAIT_FOR_RESPONSE && monkeypress != WURTZ_STOPPED)
                {
                    monkeypress = c;
                    if((t1 = timediff(&now,&wurtzstart)) < 0.001)
                        monkeypress = -3;
                }
                else
                    monkeypress = c;
                /*
                 * what is happening when front thinks there was a premature release but
                 * the time is long?
                 */
                down = timediff(&now,&wurtzframetime);
                t1 = timediff(&now,&wurtzstart);
                t2 = timediff(&now,&endstimtime);
                t3 = timediff(&wurtzframetime,&wurtzstart);
                
                if(stmode & MOVED_STIMULUS)
                {
                    SerialSend(ORIENTATION);
                    SerialSend(SETZXOFF);
                    SerialSend(SETZYOFF);
                    stmode &= (~MOVED_STIMULUS);
                }
                
                /* Send BW the actual number of frames calculated */
                sprintf(buf,"%s%d\n",serial_strings[NFRAMES_DONE],framesdone);
                SerialString(buf,0);
                SerialSignal(END_TRIAL);
                if(seroutfile != NULL){
                    fprintf(seroutfile,"ET %d %.2f %.2f %.2f %.2f\n",c,down, t1,t2,t3);
                    fflush(seroutfile);
                }
                mode &= (~HOLD_STATUS);
                ExptTrialOver(c);
                    SendTrialCount();
                break;
            case START_EXPT: /* this is sent when BW starts up send everything */
                MakeConnection();
                break;
            case BW_IS_READY:
                charctr = 0;
                break;
		}
		lastcodes[++codectr%CODEHIST] = c;
	}
	else
	{
		charbuf[charctr++] = c;
		if(charctr >= CBUFLEN)
        {
		    fprintf(stderr,"%s\nSerial Input too long\n",charbuf);
		    if(seroutfile)
                fprintf(seroutfile,"%s\nSerial Input too long\n",charbuf);
            InterpretLine(charbuf, &expt,1);
            charctr = 0;
            acknowledge("Serial Input too long",NULL);
		}
		charbuf[charctr] = 0;
	}
	return(res);
}

void SerialSignal(char flag)
{
    char c = (char)flag;
	if(mode & NO_SERIAL_PORT)
		return;
	write(ttys[0],&c,1);
	outcodes[++outctr%CODEHIST] = c;
}


void paint_target(float color, int flag)
{
    float lcolor = color,contrast;
    float bcolor = 1-color;
    int showa = 0,showb = 0,oldoption;
    
    if(flag == 1)
        glDrawBuffer(GL_FRONT_AND_BACK);
    
    // Choice targets always boxes...  
    oldoption = optionflag;
    optionflag |= SQUARE_FIXATION;
    if(option2flag & AFC){
        showa = showb = 1;
        if (flag == 2) // only show  correct target
            showb = 0;
    }
    else if(afc_s.sacval[0]+afc_s.sacval[1] > 0)
        showa = 1;
    else
        showb = 1;
    /*
     * Choicea is shown for the UP target. So if sacval is negative, and target ratio <1
     * show this at lower contrast.
     */
    if(ChoiceStima->type != STIM_NONE && showa){
        contrast = ChoiceStima->pos.contrast;
        if((afc_s.sacval[0]+afc_s.sacval[1]) * afc_s.sign < 0)
            ChoiceStima->pos.contrast = contrast * expt.vals[TARGET_RATIO];
        ChoiceStima->noclear = 1;
        increment_stimulus(ChoiceStima,&(ChoiceStima->pos));
//        if (ChoiceStima->left->seedloop == 0)
 //           ChoiceStima->left->baseseed+=2;
        calc_stimulus(ChoiceStima);
        paint_stimulus(ChoiceStima);
        ChoiceStima->pos.contrast = contrast;
    }
    if(ChoiceStimb->type != STIM_NONE && showb){
        contrast = ChoiceStimb->pos.contrast;
        if((afc_s.sacval[0]+afc_s.sacval[1]) * afc_s.sign > 0)
            ChoiceStimb->pos.contrast = contrast * expt.vals[TARGET_RATIO];
        ChoiceStimb->noclear = 1;
        increment_stimulus(ChoiceStimb,&(ChoiceStimb->pos));
//        if (ChoiceStimb->left->seedloop == 0)
//            ChoiceStimb->left->baseseed+=2;
        calc_stimulus(ChoiceStimb);
        paint_stimulus(ChoiceStimb);
        ChoiceStimb->pos.contrast = contrast;
    }
    draw_fix(fixpos[0]+deg2pix(afc_s.sacval[0]+afc_s.sacval[2]),fixpos[1]+deg2pix(afc_s.sacval[1]+afc_s.sacval[3]), afc_s.targsize, color);
    
    lcolor = TheStim->background + (color - TheStim->background) * expt.vals[TARGET_RATIO];
    if(option2flag & AFC && flag != 2){
        draw_fix(fixpos[0]+deg2pix(afc_s.sacval[4]+afc_s.sacval[2]),fixpos[1]+deg2pix(afc_s.sacval[5]+afc_s.sacval[3]), afc_s.targsize, lcolor);
        if(optionflags[FOUR_CHOICES]){
            draw_fix(fixpos[0]-deg2pix(afc_s.sacval[0]+afc_s.sacval[2]),fixpos[1]-deg2pix(afc_s.sacval[1]+afc_s.sacval[3]), afc_s.targsize, bcolor);
            draw_fix(fixpos[0]+deg2pix(afc_s.sacval[0]-afc_s.sacval[2]),fixpos[1]+deg2pix(afc_s.sacval[1]-afc_s.sacval[3]), afc_s.targsize, bcolor);
        }
        if(optionflags[SHOW_REWARD_BIAS] == 1 && fabs(expt.biasedreward) > 0.1){
            draw_fix(fixpos[0]+deg2pix(afc_s.abssac[0]+afc_s.abssac[2]),fixpos[1]+deg2pix(afc_s.abssac[1]+afc_s.abssac[3]), afc_s.targsize, 1.0);
            draw_fix(fixpos[0]-deg2pix(afc_s.abssac[0]-afc_s.abssac[2]),fixpos[1]-deg2pix(afc_s.abssac[1]-afc_s.abssac[3]), afc_s.targsize, 0.0);
        }
    }

    if(flag)
        glDrawBuffer(GL_BACK);
    optionflag = oldoption;
}

void afc_statusline(char *s, int line)
{
    vcoord x[2];
    x[0] = -640;
    x[1] = -475 + line;
    if(!(mode & RUNNING))
        return;
    glDrawBuffer(GL_FRONT_AND_BACK);
    setmask(OVERLAY);
    SetGrey(expt.overlay_color);
    myrect(-640,-450+line,100,-475+line);
    SetGrey(1.0);
	mycmv(x);
	printString(s,1);
    glDrawBuffer(GL_BACK);
}


void PaintGammaCheck(int iw, int ih)
{
    int xc,yc,i,j,k;
    GLubyte *im1,*im2,*p;
    
    SetColor(0.5,1);
    myrect((float)(-600), (float)(-500), (float)(iw*2-600), (float)(ih*2)-500);
    
    im1 = (GLubyte *)malloc(iw * ih * sizeof(GLubyte));
    p = im1;
    k = 0;
    for(i = 0; i < ih; i++){
        k = ((i/3) &1) * 3;
        for(j = 0; j < iw; j++){
            *p++ = 255 * ((k++/3) & 1);
        }
    }
    glDrawPixels(iw, ih, GL_LUMINANCE, GL_UNSIGNED_BYTE, im1);
    free(im1);
}


void chessboard(float w, float h)
{
    int squares=8;
    float x, y;
    int i, j;
    float a,b,c;
    static float lastc = 0,laps = 0;
    
    
    c = TheStim->pos.contrast_amp;
    c = expt.vals[TIMEOUT_CONTRAST];
    a = (c+1)/2;
    b = 1-a;
    x = winsiz[0]/w;
    y = 1 + (winsiz[1]-1)/h;
    if(lastc != c){
        printf("Lum %.3f %.3f gamma %.3f\n",a,b,gammaval);
    }
    lastc = c;
    
    
    for (i = -x; i < x; i++) {
        for (j = -y; j < y; j++) {
            if (ODD(i + j))
                SetColor(a,1);
            else
                SetColor(b,1);
            myrect((float)(i*w), (float)(j*h), (float)(i*w+w), (float)(j*h+h));
        }
    }
    
}

void printString(char *s, int size)
{
    // Ali: we decided that at least for now we display this info on the mainGUI not the monkey screen
    printf("%s\n",s);
    updateInfoText(s);
    return;
}

void printStringOnMonkeyView(char *s, int size)
{
    displayOnMonkeyView(s, -500, -450);
    glPushAttrib(GL_LIST_BIT);
    if(size == 1)
        glListBase(mediumbase);
    else if(size == 2)
        glListBase(bigbase);
    else
        glListBase(base);
    glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *)s);
    glPopAttrib();
}


void WriteFrameData()
{
    FILE *ofd;
    int i;
    
    /*
     * The first one is often garbage because of the state of the spike array
     * in brainwave
     */
    
    if((ofd = fopen("./frames.dat","a")) != NULL)
    {
        for(i =1; i < framebufctr; i++)
            fprintf(ofd,"%d\n",framebuf[i]);
        fclose(ofd);
    }
}

void expt_over(int flag)
{
    int i;
    char buf[256];
    FILE *ofd;
    time_t tval;
    double t;
    
    mode &= (~BW_ERROR);
    if(stimstate == WAIT_FOR_RESPONSE && flag != CANCEL_EXPT){
        TheStim->mode |= EXPT_OVER; // make sure this is called again after response
        return;
    }
    stimstate = INTERTRIAL;
    TrialOver();
    gettimeofday(&endexptime,NULL);
    pursuedir = 1;
    StimulusType(expt.st, expt.stimtype); /* reset stim type */
    if(mode & MORE_PENDING && mode & AUTO_NEXT_EXPT)
    {
        if((i = ReadExptFile(NULL, 0,0,0)) >= 0)
        {
            TheStim->mode &= (~(EXPTPENDING | EXPT_OVER | INTRIAL));
            if(flag == CANCEL_EXPT) //Ali  && confirm_no("Run Next expt?",NULL))
                ReadExptFile(NULL, 1,1, 1);
            else
            {
                runexpt(NULL,NULL,NULL);
                return;
            }
        }
    }
    if(penlog && optionflag & STORE_WURTZ_BIT && flag != CANCEL_EXPT){
        fprintf(penlog,"Expt(%s) %s x %s %s N %d",expt.bwptr->prefix,
                serial_names[expt.mode],serial_names[expt.type2],
                stimulus_names[expt.st->type],expt.nreps,expt.nsaved);
        if(expt.type3 != EXPTYPE_NONE)
            fprintf(penlog," EX3 %s",serial_names[expt.type3]);
        fprintf(penlog,"\n");
        
        fflush(penlog);
        PrintPenLog(1);
    }
    Stim2PsychFile();
    
    /*
     * Set things back to how they were befre the experiment
     * NB stimvals XPOS,YPOS can change, so use expt.vals for these.
     */
    expt.st->preloaded = 0;
    if(expt.stimtype == STIM_GRATING2)
    {
	    expt.st->type = STIM_GRATING2;
	    SetProperty(&expt,expt.st,SF,expt.stimvals[SF]);
	    SetProperty(&expt,expt.st,TF,expt.stimvals[TF]);
	    SetProperty(&expt,expt.st,SF2,expt.stimvals[SF2]);
	    SetProperty(&expt,expt.st,TF2,expt.stimvals[TF2]);
    }
    SetProperty(&expt,expt.st,expt.mode,expt.stimvals[expt.mode]);
    SetProperty(&expt,expt.st,expt.type2,expt.stimvals[expt.type2]);
    SetProperty(&expt,expt.st,ORIENTATION,expt.stimvals[ORIENTATION]);
    SetProperty(&expt,expt.st,PLAID_ANGLE,expt.stimvals[PLAID_ANGLE]);
    SetProperty(&expt,expt.st,SETZXOFF,expt.vals[SETZXOFF]);
    SetProperty(&expt,expt.st,SETZYOFF,expt.vals[SETZYOFF]);
    SetProperty(&expt,expt.st,MODULATION_F,expt.stimvals[MODULATION_F]);
    if(expt.type3 == FIXPOS_Y)
        SetProperty(&expt,expt.st,FIXPOS_Y,expt.mean3);
    expt.stimvals[FIXPOS_X] = expt.fixpos[0];
    expt.stimvals[FIXPOS_Y] = expt.fixpos[1];
    SetProperty(&expt,expt.st,FIXPOS_Y,expt.fixpos[1]);
    SetProperty(&expt,expt.st,FIXPOS_X,expt.fixpos[0]);
    expt.vals[TARGET_RATIO] = expt.stimvals[TARGET_RATIO];
    
    expt.st->flag = expt.stflag;
    if(seroutfile){
        fprintf(seroutfile,"Resetting: ");
        i = 0;
        while(track_resets[i] >= 0){
            fprintf(seroutfile,"%2s->%.4f ",serial_strings[track_resets[i]],GetProperty(&expt,expt.st,track_resets[i]));
            i++;
        }
    }
    expt.vals[PLC_MAG] = expt.stimvals[PLC_MAG];
    
    if(optionflags[MODULATE_DISPARITY] == 2){
        if(expt.st->vals[MODULATION_F] > 0 && fabs(expt.st->depth_mod) > 0)
            optionflags[MODULATE_DISPARITY] = 1;
        else
            optionflags[MODULATE_DISPARITY] = 0;
    }
    if(!(optionflag & FRAME_ONLY_BIT)){
        sprintf(buf,"Expt over at %s",binocTimeString());
        statusline(buf);
    }
    TheStim->mode &= (~(EXPTPENDING | EXPT_OVER | INTRIAL));
    //Ali  SetRunButton(NULL);
    
    if(!(optionflag & FRAME_ONLY_BIT) || (optionflag & WAIT_FOR_BW_BIT))
    {
        if(flag == CANCEL_EXPT)
            SerialSignal(CANCEL_EXPT);
        else
            SerialSignal(END_EXPT);
    }
    if(flag == CANCEL_EXPT){
        if(seroutfile != NULL){
            tval = time(NULL);
            fprintf(seroutfile,"Cancelled at %u\n",ufftime(&now));
            fprintf(seroutfile,"Run ended at %s\n",ctime(&tval));
        }
    }
    else{
        gettimeofday(&now,NULL);
        if(seroutfile != NULL){
            tval = time(NULL);
            fprintf(seroutfile,"et at %u\n",ufftime(&now));
            fprintf(seroutfile,"Run ended at %s\n",ctime(&tval));
            fflush(seroutfile);
        }
    }
    if(demomode == 0){
        SetStopButton(STOP);
        clear_display(1);
    }
    glFinishRenderAPPLE();
    
    /*
     * work on expt file _AFTER_ clearing the display and telling BW that the expt is over
     */
    if(rcfd){
        fclose(rcfd);
        rcfd = NULL;
        if(optionflags[ONLINE_AUTO_COPY]){
            sprintf(buf,"cp %s/%s /bgc/data/online/%s",datprefix,rcname,rcname);
            system(buf);
            sprintf(buf,"/bgc/bgc/perl/rc2tab /bgc/data/online/%s",rcname);
            system(buf);
        }
    }
    
    //Ali  if(flag)
    //    framefront();
    if(SACCREQD(afc_s)){
        paint_target(TheStim->background,1);
        optionflag |= (DRAW_FIX_BIT);
    }
    //Ali ListExpStims(NULL);
    if(demomode == 0)
        stimstate = STIMSTOPPED;
    
    // was only else here. But surely need this set to zero even if not running demo? 
    stimno = 0;
    if(option2flag & PSYCHOPHYSICS_BIT  && flag != CANCEL_EXPT){
        PrintPsychData(expt.logfile);
    }
    sprintf(buf,"%2s+\n",serial_strings[STOP_BUTTON]);
    SerialString(buf,0);
    optionflag &= (~GO_BIT);
    if(!testflags[PLAYING_EXPT])
        mode &= (~ANIMATE_BIT);
    if(optionflag & FRAME_ONLY_BIT)
        WriteFrameData();
    SaveExptFile("./leaneo.stm",SAVE_STATE);
    SendAllToGui();
    notify("\nEXPTOVER\n");
}

void Stim2PsychFile()
{
    float t,val,version;

    sscanf(VERSION_NUMBER,"binoclean.%f",&version);
    
    

    if(psychfile){

        fprintf(psychfile,"R5 %s=%.2f %s=%.2f %s=%.2f",
                serial_strings[STIM_SIZE],GetProperty(&expt,expt.st,STIM_SIZE), 
                serial_strings[SETCONTRAST],GetProperty(&expt,expt.st,SETCONTRAST),
                serial_strings[JVELOCITY],GetProperty(&expt,expt.st,JVELOCITY));
        t = RunTime();
        fprintf(psychfile," %.2lf %.2f %.2f",t,
                GetProperty(&expt,expt.st,XPOS),
                GetProperty(&expt,expt.st,YPOS));
        fprintf(psychfile," %s=%.2f %s=%.2f x=0 x=0 x=0 x=0\n",serial_strings[XPOS],GetProperty(&expt,expt.st,XPOS),serial_strings[YPOS],GetProperty(&expt,expt.st,YPOS));
        
        fprintf(psychfile,"R5 %s=%.2f %s=%.2f %s=%.2f",
                serial_strings[ORIENTATION],GetProperty(&expt,expt.st,ORIENTATION), 
                serial_strings[SF],GetProperty(&expt,expt.st,SF),
                serial_strings[DOT_SIZE],GetProperty(&expt,expt.st,DOT_SIZE));
        fprintf(psychfile," %.2lf %.2f %.2f",t,
                GetProperty(&expt,expt.st,XPOS),
                GetProperty(&expt,expt.st,YPOS));
        fprintf(psychfile," %s=%.4f %s=%.4f x=0 x=0 x=0 x=0\n",serial_strings[INITIAL_APPLY_MAX],GetProperty(&expt,expt.st,INITIAL_APPLY_MAX),serial_strings[JDEATH],GetProperty(&expt,expt.st,JDEATH));
        
        fprintf(psychfile,"R5 %s=%.4f %s=%.2f By=%.2f",
                serial_strings[VERSION_CODE],version, 
                serial_strings[REWARD_BIAS],GetProperty(&expt,expt.st,REWARD_BIAS),
                GetProperty(&expt,expt.st->next,YPOS));
        fprintf(psychfile," 0 %.2f %.2f",
                GetProperty(&expt,expt.st,BACK_ORI),
                GetProperty(&expt,expt.st,BACK_SIZE));
        fprintf(psychfile," %s=%.0f %s=%.2f x=0 x=0 x=0 x=0\n",serial_strings[STIMULUS_MODE],GetProperty(&expt,expt.st,STIMULUS_MODE),serial_strings[BACK_CONTRAST],GetProperty(&expt,expt.st,BACK_CONTRAST));
        
        
        fprintf(psychfile,"R5 %s=%.4f %s=%.2f By=%.2f",
                serial_strings[VERSION_CODE],version, 
                serial_strings[REWARD_BIAS],GetProperty(&expt,expt.st,REWARD_BIAS),
                GetProperty(&expt,expt.st->next,YPOS));
        fprintf(psychfile," 0 %.2f %.2f",
                GetProperty(&expt,expt.st,BACK_ORI),
                GetProperty(&expt,expt.st,BACK_SIZE));
        fprintf(psychfile," %s=%.0f usenewdir=%d x=0 x=0 x=0 x=0\n",serial_strings[STIMULUS_MODE],GetProperty(&expt,expt.st,STIMULUS_MODE),usenewdirs);
        
        
        if(expt.st->next && expt.st->next->type != STIM_NONE){
            fprintf(psychfile,"R8 %s=%.2f %s=%.2f %s=%.2f",
                    serial_strings[XPOS], GetProperty(&expt,expt.st->next,XPOS),
                    serial_strings[YPOS], GetProperty(&expt,expt.st->next,YPOS),
                    serial_strings[REWARD_BIAS],GetProperty(&expt,expt.st,REWARD_BIAS),
                    GetProperty(&expt,expt.st->next,YPOS));
            fprintf(psychfile," 0 %.2f %.2f",
                    GetProperty(&expt,expt.st,BACK_ORI),
                    GetProperty(&expt,expt.st,BACK_SIZE));
            fprintf(psychfile," %s=%.4f %s=%.2f x=0 x=0 x=0 x=0\n",serial_strings[INITIAL_APPLY_MAX],GetProperty(&expt,expt.st,INITIAL_APPLY_MAX),serial_strings[BACK_CONTRAST],GetProperty(&expt,expt.st,BACK_CONTRAST));
        }
        if(expt.st->type == STIM_IMAGE){
            fprintf(psychfile,"R6 se0=%d se1=%d se2=%d %d %d %d se6=%d imver=%.2f x=0 x=0 x=0 x=0\n",
                    seedoffsets[0],seedoffsets[1],seedoffsets[2],
                    seedoffsets[3],seedoffsets[4],seedoffsets[5],
                    seedoffsets[6],expt.st->stimversion);
            fprintf(psychfile,"R6 se7=%d se8=%d se9=%d %d %d %d se13=%d imver=%.2f x=0 x=0 x=0 x=0\n",
                    seedoffsets[7],seedoffsets[8],seedoffsets[9],
                    seedoffsets[10],seedoffsets[11],seedoffsets[12],
                    seedoffsets[13],expt.st->stimversion);
            
        }
        fflush(psychfile);
    }
    
    
}



void HideCursor(void)
{
}

void ReopenSerial(void)
{
    int i;
    
#ifdef NIDAQ
    printf("Writing to DIO\n");
    DIOWriteBit(2,  1);
    DIOWriteBit(1,  1);
    DIOWriteBit(0,  1);
//    DIOWrite(0x7); 
    fsleep(0.01);
    DIOWriteBit(2,  0);
    DIOWriteBit(1,  0);
    DIOWriteBit(0,  0);
    //DIOWrite(0); DIOval = 0;
#endif
    closeserial(0);
    if((i = OpenSerial(theport)) <= 0){
        mode |= NO_SERIAL_PORT;
        fprintf(stderr,"Can't Open %s (%d)\n",theport,i);
    }
    else{
        printf("Reopened %s (%d)\n",theport,i);
        fsleep(0.15);
        if(!(mode & SERIAL_OK))
            MakeConnection();
        else{
            SerialSignal(BW_IS_READY);
            if(!CheckBW(BW_IS_READY,"Reopen"))
                mode &= (~SERIAL_OK);
        }
    }
}



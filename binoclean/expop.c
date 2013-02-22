#import <stdio.h>
#import <sys/time.h>
#import <string.h>
#import <stdlib.h>
#import <stdarg.h>
#import <math.h>
#import "mymath.h"
#import "misc.h"
#import "myview.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import "commdefs.h"
#import "stimuli.h"
#import "cyber.h"
#import "WinDefs.h"

#import <sys/stat.h>

#import <sys/dir.h>

#import "stimdefs.h"

#import "protos.h"


#define MAXREPS 500
#define MAXTRIALS 300
#define ZEROBLOCKING 10

#define HIGH 1
#define MONOC 2

#define NEXT_EXPT 1
#define CLEAR_EXPT 2
#define CLEAR_LINES 3
#define PLACE_PLOT 4
#define PRINT_PLOT 5
#define CLEAR_SCREEN 6
#define POPDOWN_PANEL 7
#define EXPTYPE 8
#define EXPTYPE2 9
#define DONEBUTTON 10
#define APPLYBUTTON 11
#define EDIT_EXPVALS 12
#define CANCELBUTTON 13
#define SHOW_TRIALCOUNT 14
#define CLEAR_PSYCHDATA 15
#define POPDOWN_ALLPANEL 16
#define UPDATE_PANEL 17
#define POPUP_EXPT 18
#define POPUP_AFC 19
#define POPUP_WURTZ 20
#define POPUP_BW 21
#define POPUP_LOG 22
#define POPUP_UNITLOG 23
#define POPUP_CODES 24
#define POPUP_GRAPHS 25
#define POPUP_CYBER 26
#define CLEAR_EXPT_ONLY 27
#define STORE_STATE 28
#define RESTORE_STATE 29
#define POPUP_EXPTS 30
#define EDIT_EXPMENUS 31
#define POPUP_STEPPER 32
#define POPUP_PEN_LOG 33
#define EXPTYPE3 34
#define EDIT_EXP2VALS 35
#define REFRESHBUTTON 36
#define CLEAR_OPTIONS 37
#define RELOADDEPTHBUTTON 38
#define POPUP_MYCODES 39
#define RESTART_SERIAL 40

#define EXPT_LOAD1 41
#define EXPT_LOAD2 (EXPT_LOAD1+1)
#define EXPT_LOAD3 (EXPT_LOAD1+2)
#define EXPT_LOAD4 (EXPT_LOAD1+3)
#define EXPT_LOAD5 (EXPT_LOAD1+4)
#define EXPT_LOAD6 (EXPT_LOAD1+5)
#define EXPT_LOAD7 (EXPT_LOAD1+6)
#define EXPT_LOAD8 (EXPT_LOAD1+7)
#define EXPT_LOAD9 (EXPT_LOAD1+8)
#define EXPT_LOAD10 (EXPT_LOAD1+9)
#define MAXQUICKEXP 200
#define CLOSE_EXPT (EXPT_LOAD1+MAXQUICKEXP)

#define MAXRCFRAMES 50000

//Ali

#define ZERO 0
#define MANUALEVENT 1
#define TRUE 1
#import "stimuli.h"
char * VERSION_NUMBER;


#define MAXRF 10
extern Expstim oldrfs[];
extern int Frames2DIO;
extern float pursuedir;
extern FILE *imidxfd;
extern char *replay_expt, ImageOutDir[];
extern int rfctr,mimic_fixation;
extern char *rcname;
extern int wurtzctr,*fixed,lasteyecheck;
extern float calcdur,paintdur,swapwait;
extern int debug,timeout_type;
extern double olddisp,oldvelocity;
extern int imageseed[],stimflag;
extern int DIOval;
extern int rewardall,check_for_monkey;
extern int freeToGo;
extern int lastbutton;

double fakestim =0;

int usenewdirs=0;
static int pcmode = BRAINWAVE;
static char **expmenustrings;
static int protrudemm = 0;
static int coarsemm = 0;
static float lastdir,lastpi;
static int textcode = 0;
static int seedoffset = 0;
static PGM pgimage;
static int firstseed = 0;
#define MAXBACKIM 10000
static PGM backims[MAXBACKIM+1];
static int backloaded = 0;
static int flips[2] = {0};
int seedoffsets[100] = {0};
int covaryprop = -1;
int maxseed = 0;

#define MAXHELPFILES 200
static char *helpfiles[MAXHELPFILES] = {NULL};
static char *helplabels[MAXHELPFILES] = {NULL};
static int nhelpfiles = 0;
static int longnames[100] = {MIXAC, EXPT1_MAXSIG, FAKESTIM_SIGNAL, HIGHXTYPE, MONKEYNAME, 0};

FILE *imoutfd = NULL;
int command_pending;
int prevstim = -1;
int noserialout = 0;
int demomode = 0;
Position panelx = 0,panely = 0;

static Numeric *xpenv;
static int totalframe = 0;
static int ifcstim = 0;
static int ifcanswer = 0;
static double ifcval = 0;
static int ifcsum = 0,framesum,realframesum;
static int dominanteye = LEFT;
static int rwduration = 200;
static int repetitions_set = 0;
static int rcframe,newtoggles = 0;
static int frameparams[3] = {XPOS, YPOS, CORRELATION};
static int xrnd,yrnd;
static int popmode = EDIT_EXPVALS;
int dorpt = 0,rcrpt = 0, lastrpt = 0;

extern int framesdone,freezeexpt,freezestimulus,binocrunning;
extern int altstimmode, gotspikes, endbadctr;
int realframes;
static int extratypes[10];
static float extravals[10];
extern char serchar;
extern unsigned long charsread;
extern char longlist[];
static char *extralabels[10];
extern Cyber cyberprops[];
extern int ttys[];
extern int realframecount;
extern vcoord psychoff[];
extern FILE *seroutfile,*logfd,*stairfd;
char *psychfilename = NULL;
FILE *psychfile = NULL;
FILE *psychfilelog = NULL;
extern float framehold;
extern int testmode;
FILE *cmdhistory;
int onlineversion = 10000; // always > any version #

FILE *rcfd = NULL,*spikefd = 0;
int rcctr = 0,onlinedat = 0;
FILE *penlog = NULL;
FILE *psychlog = NULL;
int trialcnt = 0;
extern int verbose;
static float stimdursum, stimpos[4];
static int stimdurn;
extern float lasttf,lastsz,lastsf,trialdur,lastor;
extern /*Ali Cursor*/ int thecursor;
extern Stimulus *stimptr,*ChoiceStima,*ChoiceStimb;
extern int fixstate, stimstate;
char *expname = NULL;
/* Ali Cursor */ int thecursor;
extern int optionflags[],defaultflags[],states[],testflags[];

#define NSAVES 5
static float savevals[NSAVES][MAXTOTALCODES+1];
int resetval[TOTALCODES] = {0};
int watchcodes[MAXTOTALCODES];
int showcodes[MAXTOTALCODES] = {0};
static int savetype[NSAVES *2] = {0};
unsigned long *rndbuf;

// don't need online data files on Mac when using Spike2
// char datprefix[256] = "/local/data/online";
char datprefix = NULL;

extern char stepperport[];
static char cyberport[256] = "/dev/ttyf1";
static int skiplines = 0,stopline = 0;
extern int cybertty;

int valstringindex[MAXTOTALCODES];
unsigned expseed = 0;
int *framebuf = NULL, framebufctr = 0;
double frametestsum,framessq,framesd,framemean;
long frametotal = 0;
int outliers[2];
int clearcnt;
static int stimulus_is_prepared = 0;
float dframeseq[MAXFRAMES*4]; // tracks when change happens
float frameseq[MAXFRAMES*4];
float frameseqb[MAXFRAMES*4];
float frameseqc[MAXFRAMES*4];
float frameseed[MAXFRAMES*4];
int frameiseq[MAXFRAMES*4];
int frameiseqb[MAXFRAMES*4];
int frameiseqp[MAXFRAMES*4];
int framecseq[MAXFRAMES*4];
Thisstim *stimseq = NULL;
int trialctr = 0;
int triallaps = 0;
int reading_quickexp = 0;
static float **rcstimvals = NULL;
float **rcrfreqs = NULL;
float **rclfreqs = NULL;
int **rcdps = NULL;
static int *rcstimid = NULL;
static int **rcstimxy = NULL;
static float *rcstimframetimes = NULL;
static float *sframetimes = NULL;


int alloptions = GO_BIT | STORE_WURTZ_BIT | LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK | SEARCH_MODE_BIT | FIXATION_CHECK  | SQUARE_RDS | RAMP_HOLD_BIT;
int all2options =  FLASH_BIT | EXPT_INTERACTIVE | INTERLEAVE_BLANK | INTERLEAVE_ONE_MONOCULAR | INTERLEAVE_UNCORRELATED | STAIRCASE | AFC;
int allstimflags = FRONT_ONLY | HOLD_PLANE | FLAT_DISPARITY | FLAT_SURFACES;
int allflags[MAXALLFLAGS] = {0};
/* BCNEW */

extern int ranright,  ranleft, ranleftc, ranrightc, expstate;
extern char *flagstrings[];
extern int codectr,stimstate,monkeypress,bothmask,alphamask;
extern int lastcodes[];
extern float clearcolor;
extern long optionflag,option2flag;
extern int mode,wurtzcount,duration;
extern char *toggle_strings[];
extern char *toggle_codes[];
extern char *flag_codes[];
extern char *mode_codes[];
//extern char *serial_strings[];
extern char *stimulus_names[];
extern Monitor mon;
extern Expt expt;
extern AFCstructure afc_s;
extern Log thelog;
extern struct BWSTRUCT thebwstruct;
extern FILE *testfd;
extern struct timeval signaltime,now,endstimtime,firstframetime,zeroframetime,frametime,alarmstart;
extern struct timeval calctime,paintframetime,changeframetime;
extern vcoord conjpos[],fixpos[];
static time_t lastcmdread;

static char mssg[BUFSIZ];
static short *linedata = NULL;
struct timeval endsigtime,bwtime;
unsigned long bwticks =0;
#define NALLSLIDERS 3
static int allstartcode[NALLSLIDERS] = {ORIENTATION, STIM_WIDTH, STIM_HEIGHT};
static int sliderflag = POSTPERIOD_CODE;
static float trialeyepos[4][BUFSIZ];
int stimno = 0, laststimno,firststimno,penstimno,reason;
extern FILE *plotfd;

extern char *bwtoggle_strings[], *scalestrings[];
char *modestrings[] = {"+","",NULL};
char *eyestrings[] = {"L","R",NULL};
char *vstrings[] = {"Not Set","V1","V2","Ant Lun","MT","?MT","MST","STS","??","Vc",NULL};
char *userstrings[100] = {"Not Set","bgc","tanabes","Ali",NULL};
char *electrodestrings[100] = {"Not Set","FHC-PtIR","AlphaOmega","24Contact 100u","24Contact 50u", "24ContactCNT 100u", "24ContactCNT 50u","8ContactCNT 150u", "16ContactCNT 100u", "MicroProbe+polyamide", NULL};
int userid = 0;
int electrodeid = 0;

int eyevals[2] = {LEFT, RIGHT};
char *plot3strings[] = {"All","1","2",NULL};
int plot3vals[] = {0, 1, 2};

extern MenuItem stimulus_menu[];
extern OneStim *thecorrug;

#define POPDOWNI 8
#define NEXPSLOTS 8
#define MAXQUICK_SUB 20
int nquickexpts = 0;
int nsubexpts[MAXQUICK_SUB] = {0};
int nallsubs = 0;
int nsubmenus = 0;
static int quicksubid[MAXQUICKEXP] = {-1};
static char *quicknames[MAXQUICKEXP] = {NULL};
static char *quicksubnames[MAXQUICKEXP] = {NULL};
static char *defaultexps[6] = {"none","flash_bar","move_bar","qrds","qsine","monocd"};

int toggleidx[MAXALLFLAGS];

char *incrstrings[5] = {
    "Lin",
    "+-",
    "Log",
    "L+-",
    NULL
};

char *stimtypestrings[N_STIMULUS_TYPES+1] = {
    "RDS",
    "Sine",
    "Bar",
    "twobar",
    "None",
    "2xSine",
    "Cylndr",
    "Gabors",
    "Corrug",
    "Sqcorrug",
    "sqwave",
    "probe",
    "test",
    "rect",
    "circle",
    "RLS",
    "Annulus",
    "RDS/Sine",
    "1DSineS",
    "RLS/Sine",
    "Radial",
    "Image",
    "RDSgrid",
    NULL
};

int stimtypevals[N_STIMULUS_TYPES] = {
    STIM_RDS,
    STIM_GRATING,
    STIM_BAR,
    STIM_TWOBAR,
    STIM_NONE,
    STIM_GRATING2,
    STIM_CYLINDER,
    STIM_GABOR,
    STIM_CORRUG,
    STIM_SQCORRUG,
    STIM_SQUARE,
    STIM_PROBE,
    STIM_TEST,
    STIM_RECT,
    STIM_CIRCLE,
    STIM_RLS,
    STIM_ANNULUS,
    STIM_RDSSINE,
    STIM_GRATINGN,
    STIM_RLSSINE,
    STIM_RADIAL,
    STIM_IMAGE,
    STIM_CHECKER,
};

/*
 char *stimstrings[2+1] = {"foregr","backgr",NULL};
 int whatstims[2] = {STIM_FOREGROUND, STIM_BACKGROUND};
 */
char *stimstrings[5+1] = {"foregr","backgr","Third","ChoiceU/R","ChoiceD/L",NULL};
int whatstims[5] = {STIM_FOREGROUND, STIM_BACKGROUND,STIM_NUM3,STIM_CHOICE1,STIM_CHOICE2};


int showexp[3][TOTALCODES+1] = {0};
int nexpshow[3] = {0, 0,0};
int expshoworder[3][TOTALCODES+1];


static Winloc winposns[10];

typedef struct exptmenu{
    char *label;
    int val;
}Exptmenu;

Currentstim currentstim;

Exptmenu firstmenu[] = {
    {"Ori",  ORIENTATION},
    {"Disp",DISP_X},
    {"V Disp",DISP_Y},
    {"Back Disp", DISP_BACK},
    {"SF", SF},
    {"TF",TF,},
    {"TF (Linear)",TFLIN},
    {"Length", STIM_HEIGHT},
    {"Width", STIM_WIDTH},
    {"Size",STIM_SIZE},
    {"RF ends",END_OFFSET,},
    {"Ends Length",END_LENGTH,},
    {"RF Sides",SIDE_OFFSET,},
    {"Ortho pos",ORTHOG_POS},
    {"X pos",SETZXOFF},
    {"Para pos",PARA_POS},
    {"Y pos",SETZYOFF},
    {"Orhto Disp",ORTHOG_DISP},
    {"Para Disp",PARALELL_DISP},
    {"Corr disp",CORRELATED_DISPARITY},
    {"dPhase", DISP_P},
    {"DispCycles",DISP_AS_CYCLES,},
    {"Fix X",FIXPOS_X,},
    {"Fix Y",FIXPOS_Y,},
    {"Seed Shift",SEED_SHIFT,},
    {"Monocs", MONOCULARITY_EXPT,},
    {"Conjug",STATIC_CONJUGATE,},
    {"Dot Dens",DOT_DENSITY},
    {"Dot Size",DOT_SIZE},
    {"L Cntrst",CONTRAST_LEFT,},
    {"d SF",SF_DIFF,},
    {"d Ori",ORIENTATION_DIFF,},
    {"Depth mod",DEPTH_MOD,},
    {"Disp grad",DISP_GRAD},
    {"Phase",SETPHASE},
    {"Initial Phase",START_PHASE},
    {"L Ori",ORI_LEFT},
    {"L Orthopos",ORTHOG_POSL},
    {"Rel Disp",RELDISP},
    {"d Ori (F+B)",DORI_BOTH},
    {"Back d Ori",DORI_BACK},
    {"Seed",SET_SEED},
    {"Correlation",CORRELATION},
    {"Disp Ramp",DISP_RAMP},
    {"FP jump size",FP_MOVE_SIZE},
    {"FP jump dir",FP_MOVE_DIR},
    {"Contrast",SETCONTRAST},
    {"Verg",FIXPT_DISPARITY},
    {"VVerg",VERTICAL_VERGENCE},
    {"DispMod F",MODULATION_F},
    {"IFC scale",IFCSCALE},
    {"VDisp Mod",VDISP_MOD},
    {"Disparity 1",DISP_A},
    {"Disparity 2",DISP_B},
    {"dWidth",WIDTH_DISP},
    {"dLength",LENGTH_DISP},
    {"dSize",SIZE_DISP},
    {"Width L",WIDTH_L},
    {"Length L",HEIGHT_L},
    {"Contrast Ratio",CONTRAST_RATIO},
    {"Contrast Difference (L-R)", CONTRAST_DIFF},
    {"Plaid Contrast Diff",PLAID_RATIO},
    {"SD Y",SD_Y},
    {"SD Both",SD_BOTH},
    {"Polar Angle",STIM_POLARANGLE},
    {"Change Time",CHANGE_SEED},
    {"dp as dx",PHASE_AS_DISP},
    {"dPhase 2",DISP_P2},
    {"ORI2",ORI2},
    {"Seed Delay",SEED_DELAY},
    {"Back Vdisp",BACK_VDISP},
    {"SF Right",SF_RIGHT},
    {"Aspect Ratio",ASPECT_RATIO},
    {"Ori Bandwidth",ORI_BANDWIDTH},
    {"Dist Corr",DISTRIBUTION_CONC},
    {"N pulses",STIM_PULSES},
    {"Pulse Width",PULSE_WIDTH},
    {"Backgr Image",BACKGROUND_IMAGE},
    {"Backgr Movie",BACKGROUND_MOVIE},
    {"Speed",JVELOCITY},
    {"Two Cyl Disp",TWOCYL_DISP},
    {"Stimulus Mode",STIMULUS_MODE},
    {"StimOPos",STIMORTHOG_POS},
    {"SeedRange",SEEDRANGE},
    {NULL, 0},
};

#define NEXPTS1 83

Exptmenu secondmenu[] = {
    {"None",EXPTYPE_NONE},
    {"Disp",DISP_X},
    {"Para Disp",PARALELL_DISP},
    {"Orthog Disp",ORTHOG_DISP},
    {"V Disp",DISP_Y},
    {"Back Disp",DISP_BACK},
    {"Correltn",CORRELATION},
    {"RDS/Sine",RDSXSINE},
    {"Back RDS/Blank",RDSBNONE},
    {"Speed (Lin)",LINEAR_SPEED},
    {"Speed (Log)",JVELOCITY},
    {"L-R Monoc",LRINTERLEAVE},
    {"L-Binoc ",LBINTERLEAVE},
    {"R-Binoc ",RBINTERLEAVE},
    {"L-R-Binoc",LRBINTERLEAVE},
    {"Para pos",PARA_POS},
    {"X pos",SETZXOFF},
    {"Y pos",SETZYOFF},
    {"X pos (log)",XOFFLOG},
    {"SF",SF},
    {"SF (Linear)",SFLIN},
    {"TF (Linear)",TFLIN},
    {"RF sign",RF_SIGN},
    {"vergence",STATIC_VERGENCE},
    {"Ped Disp",STANDING_DISP},
    {"Nplanes",NPLANES},
    {"Clamp",CLAMP_DISPARITY_CODE},
    {"Ori",ORIENTATION},
    {"StimType",STIMULUS_TYPE_CODE},
    {"R Cntrst",CONTRAST_RIGHT},
    {"Fix Surround",FIXATION_SURROUND},
    {"Monocularity",MONOCULARITY_EXPT},
    {"Contrast",SETCONTRAST},
    {"Back Contrast",BACK_CONTRAST},
    {"Back Size",BACK_SIZE},
    {"Size",STIM_SIZE},
    {"Size (log)",LOG_SIZE},
    {"Width",STIM_WIDTH},
    {"Length",STIM_HEIGHT},
    {"Depth Mod",DEPTH_MOD},
    {"Disp grad",DISP_GRAD},
    {"R Ori",ORI_RIGHT},
    {"R Orthopos",ORTHOG_POSR},
    {"Ecc.",STIM_ECCENTRICITY},
    {"Polar Angle",STIM_POLARANGLE},
    {"ORI2",ORI2},
    {"Seed",SET_SEED},
    {"Conjug",STATIC_CONJUGATE},
    {"Nframes",NFRAMES_CODE},
    {"Back Corr",BACK_CORRELATION},
    {"L/R Contrast",CONTRAST_RATIO},
    {"AC Disp",ANTICORRELATED_DISPARITY},
    {"Fix X",FIXPOS_X},
    {"Fix Y",FIXPOS_Y},
    {"Seed delay",SEED_DELAY},
    {"Jump Type",JUMPTYPE},
    {"Back Ori",BACK_ORI},
    {"Back TF",BACK_TF},
    {"dPhase",DISP_P},
    {"dPhase 2",DISP_P2},
    {"Initial Phase",START_PHASE},
    {"Dot Size",DOT_SIZE},
    {"Contrast Pairs",CONTRAST_PAIRS},
    {"Relative Contrast",FB_RELATIVE_CONTRAST},
    {"Opposite Delay",OPPOSITE_DELAY},
    {"Alternate Stim",ALTERNATE_STIM_MODE},
    {"Pulf Delay",DELAY},
    {"Rel Disp",RELDISP},
    {"Rel V Disp",RELVDISP},
    {"VDisp Mod",VDISP_MOD},
    {"Disparity 2",DISP_B},
    {"Width R",WIDTH_R},
    {"Length R",HEIGHT_R},
    {"N Components",NCOMPONENTS},
    {"dp as dx",PHASE_AS_DISP},
    {"dp as dx both",PHASE_AS_RELDISP},
    {"SD X",SD_X},
    {"Counterphase ",SET_COUNTERPHASE},
    {"SF LEFT",SF_LEFT},
    {"Ori Bandwidth",ORI_BANDWIDTH},
    {"Dist Width",DISTRIBUTION_WIDTH},
    {"Time Ori BW",TEMPORAL_ORI_BANDWIDTH},
    {"Dist Corr",DISTRIBUTION_CONC},
    {"Initial Disp",INITIAL_DISPARITY},
    {"ChTarget Ratio",TARGET_RATIO},
    {"XposCorr",COVARY_XPOS},
    {"Nlow Components",NLOWCOMPONENTS},
    {"Nhigh Components",NHIGHCOMPONENTS},
    {"Pursuit Increment",PURSUIT_INCREMENT},
    {"Backgr Image",BACKGROUND_IMAGE},
    {"Backgr Movie",BACKGROUND_MOVIE},
    {"Phase",SETPHASE},
    {"High X Val",HIGHX},
    {"Initial Movement",INITIAL_MOVEMENT},
    {"Reward Bias",REWARD_BIAS},
    {"%ACorr",MIXAC},
    {"pBlackDot",BLACKDOT_FRACTION},
    {"Seed Loop", SET_SEEDLOOP},
    {"Orientation Disparity", ORIENTATION_DIFF},
    {"Seed Offset", SEEDOFFSET},
    {"Jump Direction", FP_MOVE_DIR},
    {"Jump Size", FP_MOVE_SIZE},
    {"Plaid Ori", PLAID_ANGLE},
    {"Dot Dens",DOT_DENSITY},
    {NULL, -1}
};

#define NEXPTS2 69
/*
 *  N.B. Expts added to second menu must also have entry in setsecondexp()
 */
Exptmenu thirdmenu[] = {
    {"None",EXPTYPE_NONE},
    {"uStim",MICROSTIM_EXPT},
    {"Flip X",XPOS},
    {"Flip Y",YPOS},
    {"Flip Ori",ORIENTATION},
    {"Flip PolarAngle",STIM_POLARANGLE},
    {"Disparity",DISP_X},
    {"Flip Disp",RELDISP},
    {"Flip dPhase",DISP_P},
    {"AC",CORRELATION},
    {"nf",NFRAMES_CODE},
    {"L/R",MONOCULARITY_EXPT},
    {"L/R/B",LRBINTERLEAVE},
    {"sl 1/0",SET_SEEDLOOP},
    {"Apsect Ratio",ASPECT_RATIO},
    {"Spin Rate",SPINRATE},
    {"FixY",FIXPOS_Y},
    {"FixX",FIXPOS_X},
    {"ContrastRC",0},
    {"TargetPos",TARGET_OFFSET},
    {"Phase",SETPHASE},
    {"Annulus/RDS",SETANNULUS},
    {"Fs Repeat",FAST_SEQUENCE_RPT},
    {"Target Ratio",TARGET_RATIO},
    {"BandWidth",ORI_BANDWIDTH},
    {"Size",STIM_SIZE},
    {"StimType",STIMULUS_TYPE_CODE},
    {"Random Contrast",RANDOM_CONTRAST_EXPT},
    {"Initial Duration",INITIAL_DURATION},
    {"BackGround Size",BACK_SIZE},
    {"Contrast",SETCONTRAST},
    {"Stimulus Mode",STIMULUS_MODE},
    {"FakeStim",FAKESTIM_EXPT},
    {"Tone Time",TONETIME},
    {"Seed",SET_SEED},
    {"Seed Offset",SEEDOFFSET},
    {"Plaid Ori",PLAID_ANGLE},
    {"%AC mixture",MIXAC},
    {"Dot Density",DOT_DENSITY},
    {NULL, -1}
};
#define NEXPTS3 39

int nexptypes[3] = {NEXPTS1, NEXPTS2,NEXPTS3};
#define NPLOTDATA (nexptypes[0]+3)
static struct plotdata *expplots=NULL;


static double expval[MAXSTIM];
static double expavals[MAXSTIM*4], expbvals[MAXSTIM*4];
static double fastvals[MAXSTIM*4],fastbvals[MAXSTIM*4],fastcvals[MAXSTIM*4];
static int uncompleted[MAXSTIM],completed[MAXSTIM];


/*
 * keeping track of repeated seeds. For kernel cross validattion.
 * each time a new stim for type i is shown, unrepeatn[i] is incremented, and
 * unrepeated[i][unrepeatn[i]] records the seed that was used.
 */
#define RPTLISTLEN 100
static int unrepeated[MAXSTIM][RPTLISTLEN] = {0};
static int unrepeatn[MAXSTIM] = {0};

#define NEXPTS 3
int *stimorder,*seedorder;
static int *isset, nstimorder, nisset,*stim3order,*stim2order;



/*
 * FindCode(*s)  return the numerical index of the code matching string *s
 * this is the index to serial_strings, not valstrings (whose order can be changed)
 */
int FindCode(char *s)
{
    int i=0,j,code;
    
    
    while((j=longnames[i++]) > 0){
        
        if(strncmp(s, serial_strings[j], strlen(serial_strings[j])) ==0)
            return(j);    
    }
    i = 0;
    while(commstrings[i].label != NULL){
        if(strncmp(s, commstrings[i].code, strlen(commstrings[i].code)) ==0)
            return(commstrings[i].icode);
        i++;
    }
    i = 0;
    for(i = 0; i < expt.totalcodes; i++)
    {
        if(strncmp(s, valstrings[i].code, strlen(valstrings[i].code)) ==0)
            return(valstrings[i].icode);
    }
    return(-1);
}

/*
 * Want this to generate a new seed if the program has just been started, or
 * if it is called later, so use getpid and allstimid
 */
int NewSeed(Stimulus *st)
{
    int i;
    
    i = 1000 + (getpid()+expt.allstimid) % 1000;
    st->left->baseseed = i - (i&0x1);// force it to be even
    st->right->baseseed = i - (i&0x1);
    return(i);
}



int PrintInfo(FILE *fd)
{
 //   fprintf(fd,"%p\n",&expplots[72]);
}

int SetTargets()
{
    double val,aval, cosa, sina, osum = 0;
    int j,sgn = 1;
    
    sgn = afc_s.sign;
    if((expt.mode == ORIENTATION || expt.type2 == ORIENTATION) && SACCREQD(afc_s)){
        for (j = 0; j < expt.nstim[0]; j++)
            osum += expval[expt.nstim[2]+j];
        osum = osum/j;
        
        if(expt.mode == ORIENTATION){
            val = expval[expt.nstim[2]];
            aval = expval[expt.nstim[0]+expt.nstim[2]-1];
            val = expt.mean-45;
            aval = expt.mean+45;
            if(usenewdirs){
                val = expt.mean+135;
                afc_s.newdirs = 1;
            }
            else
                afc_s.newdirs = 0;
            afc_s.sacdir[0] = val * M_PI/180;
            afc_s.sacdir[1] = aval * M_PI/180;
        }
        else{
            afc_s.newdirs = 0;
            val = expt.mean2-45;
            aval = expt.mean2+45;
            val = expval[expt.nstim[2]+expt.nstim[0]];
            aval = expval[expt.nstim[0]+expt.nstim[2]+expt.nstim[1]-1];
        }
        if(optionflags[FLIP_FEEDBACK]){
            SetStimulus(ChoiceStima, val,  ORIENTATION, NULL);
            SetStimulus(ChoiceStimb, aval,  ORIENTATION, NULL);
        }
        else{
            SetStimulus(ChoiceStima, aval,  ORIENTATION, NULL);
            SetStimulus(ChoiceStimb, val,  ORIENTATION, NULL);
        }
        /*
         *  if choice 1 is close to horizontal (<25 degreees) use vertical/horizontal choice targets
         * if it is close to oblique use left/right choice targets
         */
        /* 
         * July 2007 allow for different target locations. 
         * sacdir[0] is for the negative target, sacdir[1] is for the positive target
         */
        if (afc_s.newdirs){
            cosa = cos(afc_s.sacdir[1]);
            sina = sin(afc_s.sacdir[1]);
            SetStimulus(ChoiceStima, cosa * expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStima, sina * expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[1],  YPOS, NULL);
            cosa = cos(afc_s.sacdir[0]);
            sina = sin(afc_s.sacdir[0]);
            SetStimulus(ChoiceStimb, cosa*expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStimb, sina * expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[1],  YPOS, NULL);
            
        }
        else if(fabs(tan(val*M_PI/180.0)) > 0.46){  
            SetStimulus(ChoiceStima, sgn * expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStimb, -sgn * expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStima, expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStimb, expt.fixpos[1],  YPOS, NULL);
            afc_s.abssac[1] = 0;
            afc_s.abssac[0] = expt.vals[SACCADE_AMPLITUDE];
        }
        else{
            SetStimulus(ChoiceStima, sgn*expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStimb, -sgn*expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStima, expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStimb, expt.fixpos[0],  XPOS, NULL);
            afc_s.abssac[0] = 0;
            afc_s.abssac[1] = expt.vals[SACCADE_AMPLITUDE];
        }
    }
    else if (expt.mode == PLAID_RATIO){
        aval = expt.stimvals[ORIENTATION];
        val = aval + 90;
        SetStimulus(ChoiceStima, aval,  ORIENTATION, NULL);
        SetStimulus(ChoiceStimb, val,  ORIENTATION, NULL);
        if(fabs(tan(aval*M_PI/180.0)) < 1.4){  
            SetStimulus(ChoiceStima, expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStimb, -expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStima, expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStimb, expt.fixpos[1],  YPOS, NULL);
            afc_s.abssac[1] = 0;
            afc_s.abssac[0] = expt.vals[SACCADE_AMPLITUDE];
        }
        else{
            SetStimulus(ChoiceStima, expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStimb, -expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStima, expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStimb, expt.fixpos[0],  XPOS, NULL);
            afc_s.abssac[0] = 0;
            afc_s.abssac[1] = expt.vals[SACCADE_AMPLITUDE];
        }
    }
    else if (expt.mode == CONTRAST_DIFF){
        val = expt.stimvals[ORIENTATION_DIFF];
        aval = expt.stimvals[ORIENTATION] - abs(expt.stimvals[ORIENTATION_DIFF]/2);
        val = aval + abs(expt.stimvals[ORIENTATION_DIFF]);
        SetStimulus(ChoiceStima, aval,  ORIENTATION, NULL);
        SetStimulus(ChoiceStimb, val,  ORIENTATION, NULL);
        if(fabs(tan(aval*M_PI/180.0)) < 1.4){  
            SetStimulus(ChoiceStima, expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStimb, -expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStima, expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStimb, expt.fixpos[1],  YPOS, NULL);
            afc_s.abssac[1] = 0;
            afc_s.abssac[0] = expt.vals[SACCADE_AMPLITUDE];
        }
        else{
            SetStimulus(ChoiceStima, expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStimb, -expt.vals[SACCADE_AMPLITUDE]+expt.fixpos[1],  YPOS, NULL);
            SetStimulus(ChoiceStima, expt.fixpos[0],  XPOS, NULL);
            SetStimulus(ChoiceStimb, expt.fixpos[0],  XPOS, NULL);
            afc_s.abssac[0] = 0;
            afc_s.abssac[1] = expt.vals[SACCADE_AMPLITUDE];
        }
    }
    if((expt.mode == DISP_X || expt.mode == TWOCYL_DISP) && expt.st->type == STIM_CYLINDER && SACCREQD(afc_s)){
        val = sqrt(sqr(afc_s.abssac[0]) + sqr(afc_s.abssac[1]));
        aval = StimulusProperty(expt.st, ORIENTATION);
        /*
         * changed July 24 2007. Seems that with NO mirrors off, sign was wrong.
         */
        afc_s.abssac[0] = -val * sin(aval * M_PI/180);
        afc_s.abssac[1] = val * cos(aval * M_PI/180);
    }
    if((expt.mode == DISP_BACK) && expt.st->type == STIM_CYLINDER && SACCREQD(afc_s)){
        val = sqrt(sqr(afc_s.abssac[0]) + sqr(afc_s.abssac[1]));
        aval = StimulusProperty(expt.st, BACK_ORI);
        afc_s.abssac[0] = -val * sin(aval * M_PI/180);
        afc_s.abssac[1] = val * cos(aval * M_PI/180);
    }
    if(expt.vals[FORCE_CHOICE_ANGLE] >= -0.1 && expt.vals[FORCE_CHOICE_ANGLE] <= 91){
        afc_s.abssac[0] = cos(expt.vals[FORCE_CHOICE_ANGLE] * M_PI/180) * expt.vals[SACCADE_AMPLITUDE];
        afc_s.abssac[1] = sin(expt.vals[FORCE_CHOICE_ANGLE] * M_PI/180) * expt.vals[SACCADE_AMPLITUDE];
        SetStimulus(ChoiceStima, afc_s.abssac[0]+expt.fixpos[0],  XPOS, NULL);
        SetStimulus(ChoiceStima, afc_s.abssac[1]+expt.fixpos[1],  YPOS, NULL);
        SetStimulus(ChoiceStimb, -afc_s.abssac[0]+expt.fixpos[0],  XPOS, NULL);
        SetStimulus(ChoiceStimb, -afc_s.abssac[1]+expt.fixpos[1],  YPOS, NULL);

    }

    afc_s.sacval[2] = expt.vals[TARGET_XOFFSET];
    afc_s.sacval[3] = expt.vals[TARGET_YOFFSET];
    if(optionflags[FLIP_FEEDBACK]){
        afc_s.abssac[0] = -afc_s.abssac[0];
        afc_s.abssac[1] = -afc_s.abssac[1];
    }
    return(0);
    
}


void SetExpVals()
{
    int i,j,k;
    
    for(i = 0; i < expt.nstim[5]; i++)
    {
        j = i2expi(expt.flag, expt.nstim, i,0);
        k = i2expi(expt.flag, expt.nstim, i,1);
        expavals[i] = expval[j];
        expbvals[i] = expval[k];
    }
}

double *AdjustEyePos(int len)
{
    int i,j,n,k;
    float sum[4];
    static double adj[4]= {0};
    
    /*
     * Go backwards from wurtzctr until have enough good trials
     */
    j = wurtzctr;
    n = 0;
    
    for(i = 0;i< 4; i++)
        sum[i] = 0;
    while(n < len && j >= 0){
        if(fixed[j] == WURTZ_OK || fixed[j] == WURTZ_OK_W){
            k = j % BUFSIZ;
            for(i = 0;i< 4; i++)
                sum[i] += trialeyepos[i][k];
            n++;
        }
        j--;
    }
    if(n==0)
        n++;
    for(i = 0;i< 4; i++)
        adj[i] = rint(sum[i]/n);
    return(adj);
}

char *EyePosString()
{
    static char ebuf[BUFSIZ];
    int i = (wurtzctr-1)%BUFSIZ;
    
    if (i < 0)
        i = 0;
    sprintf(ebuf," rh=%.3f rv=%.3f lh=%.3f lv=%.3f",trialeyepos[0][i],trialeyepos[2][i],trialeyepos[1][i],trialeyepos[3][i]);
    return(ebuf);
    
}

int CheckEyeDrift()
{
    double *err;
    int i,ngood = 0;
    
    for(i = lasteyecheck; i < wurtzctr; i++)
        if(fixed[i] == WURTZ_OK || fixed[i] == WURTZ_OK_W){
            ngood++;
        }
    if(expt.vals[AUTO_ZERO]>1 && ngood >= expt.vals[AUTO_ZERO])
    {
        lasteyecheck = wurtzctr;
        if((err = AdjustEyePos((int)(expt.vals[AUTO_ZERO]))) == NULL)
            return(-1);
        for(i = 0; i < 4; i++)
            expt.softoff[i] -= err[i]/2;
        SerialSend(SOFTOFF_CODE);
    }
    return(ngood);
}



void write_expvals(FILE *ofd, int flag)
{
    int i;
    
    if(mode & CUSTOM_EXPVAL)
        for(i = 0; i < expt.nstim[0]; i++)
            fprintf(ofd,"E%d=%.3f\n",i,expval[i]);
    if(optionflags[CUSTOM_EXPVALB])
        for(i = expt.nstim[0]; i < expt.nstim[0]+expt.nstim[1]; i++)
            fprintf(ofd,"EB%d=%.3f\n",i-expt.nstim[0],expval[i]);
    if(flag == QUICK_SAVE)
        return;
    for(i = 1; i <= nquickexpts; i++){
//        if(quicksubid[i] >= 0)
//            fprintf(ofd,"qe=\"%s\"%s\n",quicksubnames[quicksubid[i]],quicknames[i]);
//        else
            fprintf(ofd,"qe=%s\n",quicknames[i]);
    }
}

write_helpfiles(FILE *ofd)
{
    int i = 0;
    fprintf(ofd,"mycodes=");
    for(i = 0; i < MAXTOTALCODES; i++)
    {
        if(showcodes[i]){
            fprintf(ofd,"+%2s",serial_strings[i]);
        }
    }
    fprintf(ofd,"\n");
    for(i = 0; i < nhelpfiles; i++){
        fprintf(ofd,"helpfile=\"%s\"%s\n",helplabels[i],helpfiles[i]);
    }
}

int ResetCustomVals(int imode)
{
    int i=0;
    if(imode & CUSTOM_EXPVAL){
        mode |= CUSTOM_EXPVAL;
        for(i = 0; i < expt.nstim[0]; i++)
            expval[i] = expt.customvals[i];
    }
    return(i);
}



#define NSTAIRS 20
static int nstep[NSTAIRS],reverses[NSTAIRS],laps[NSTAIRS];
static int lastup[NSTAIRS],laststim[NSTAIRS];
static int nextstim[NSTAIRS];
static int laststep[NSTAIRS],lastreverses[NSTAIRS];
static int lastlastup[NSTAIRS],lastlaststim[NSTAIRS];

char *StairPerfString()
{
    int i;
    char cbuf[BUFSIZ];
    char tmp[BUFSIZ];
    
    sprintf(cbuf,"Rev");
    for(i = 0; i < 2 * expt.nstim[1]; i++){
        sprintf(tmp,": %d,%d",laps[i],reverses[i]);
        strcat(cbuf,tmp);
    }
    sprintf(tmp,"  Trials %d",stimno);
    strcat(cbuf,tmp);
    afc_statusline(cbuf, 50);
    return(cbuf);
}

/*
 * afc->magid contains the Id number for the parameter setting magnigute
 * afc->sign determines the stimulus sign
 */

int mygreg_setstair(int result, AFCstructure *afc, double *expvals)
{
    double scale = 2, rnd,arnd;
    int nstim = expt.nstim[0]+expt.nstim[2];
    
    arnd = rnd = mydrand();
    
    
    scale = afc->gregvals[4];
    if(fabs(afc->sign) < 0.9)
        afc->sign = 1;
    
    if(result == CORRECT){
        if(rnd < afc->gregvals[0]){
            afc->magid -= afc->stairsign;
        }
        rnd = mydrand();
        if(rnd < afc->gregvals[2])
            afc->sign = -1 * afc->sign;
    }
    else if(result == WRONG){
        if(rnd < afc->gregvals[1]){
            afc->magid += afc->stairsign;
        }
        rnd = mydrand();
        if(rnd < afc->gregvals[3])
            afc->sign = -1 * afc->sign;
    }
    else if(result == FOUL){
        if(rnd > 0.5)
            afc->sign = -1 * afc->sign;
    }
    
    if(afc->type == MAGONE_SIGNTWO){
        if(afc->magid < 0)
            afc->magid = 0;
    }
    else{
        if(afc->magid < afc->nmags/2)
            afc->magid = afc->nmags/2;
    }
    if(afc->magid >= afc->nmags)
        afc->magid = afc->nmags -1;
    if(expvals[afc->magid] == INTERLEAVE_EXPT_ZERO)
        afc->vals[0] = 0;
    else
        afc->vals[0] = fabs(expvals[afc->magid]);
    
    if(afc->type == MAGONE_SIGNTWO){
        if(afc->sign < 0)
            afc->vals[1] = expvals[nstim];
        else
            afc->vals[1] = expvals[nstim+1];
        afc->jstairval = afc->vals[0];
        if(stairfd)
            fprintf(stairfd,"Res %d(%.2f) (rnd %.2f,%.2f), Val was %.2f, now %.2f ",result,trialdur,arnd,rnd,afc->jlaststairval,afc->jstairval);
        else{
            fprintf(stdout,"Res %d (rnd %.2f,%.2f), Val was %.2f, now %.2f ",result,arnd,rnd,afc->jlaststairval,afc->jstairval);
            OpenStaircaseLog();
        }
    }
    else{
        if(afc->sign < 0)
            afc->stimid = afc->nmags - afc->magid-1;
        else
            afc->stimid =  afc->magid;
        afc->vals[0] = expval[afc->stimid];
        afc->jstairval = afc->vals[0];
    }
    return(0);
}

int SetMonkeyStaircase(int jonresult, AFCstructure *afc)
{
    
    if(stairfd == NULL)
        OpenStaircaseLog();
    //  greg_setstair(jonresult, &afc_s);
    
    mygreg_setstair(jonresult, &afc_s, expval);
    afc->stimid = FindStimId(&expt, &afc_s);
    /*
     * set both of these, since which is used depends on whether last trial
     * was bad or not.
     */
    stimorder[stimno] = afc->stimid;
    stimorder[stimno+1] = afc->stimid;
    if(stairfd)
        fprintf(stairfd,"Stim %d(%d) id %d",stimno+1,expt.laststimno,afc->stimid);
    else
        printf("Stim %d(%d) id %d",stimno+1,expt.laststimno,afc->stimid);
    
    return(afc->stimid);
}


int SetAltStair(int up, int revise)
{
    int next = 0,i;
    int maxid = expt.nstim[0] - 1;
    long rnd;
    int nexps = 1;
    int nstairs = 2;
    int expid = 1;
    int stimid,istep,stop;
    int nreverses = 3;
    int stimctr = stimno;
    int exp3flag;
    
    char c = ' ';
    
    if(!revise)
        afc_s.jlaststairval = afc_s.jstairval;
    
    greg_setstair(afc_s.result, &afc_s);
    afc_s.stimid = FindStimId(&expt, &afc_s);
    if(revise)
        stimorder[stimno] = afc_s.stimid;
    else
        stimorder[stimno+1] = afc_s.stimid;
    return(afc_s.stimid);
}



int SetPsychStair(int up, int revise){
    
    int next = 0,i;
    int maxid = expt.nstim[0] - 1;
    long rnd;
    int nexps = 1;
    int nstairs = 2;
    int expid = 1;
    int stimid,istep,stop;
    int nreverses = 3;
    int stimctr = stimno;
    int exp3flag;
    
    char c = ' ';
    
    /*
     * for human psychophysics staircases are two interleaved staircases
     * One starts at each end of the range. For the first 4 trials, the staircases
     * alternate using stairid = (stimctr&1);
     * subsequentyl, which
     * staircase is used on any one trial is random, so alternation cannot 
     * be exploited by the subject.
     */
    static int stairid = 0, laststairid = 0;
    
    if(revise && stimno > 0){
        c = '*';
        stimctr = stimno -1;
    }
    else{
        c = ' ';
    }
    
    
    if(expt.mode == CORRELATION){
        i = SetAltStair(up, revise);
        return(i);
    }
    nstairs = expt.blksize;
    
    
    
    if(expt.nstim[1] > 1){
        nexps = expt.nstim[1];
        if(expt.nstim[4] > 1)
            nexps = (expt.nstim[1] * expt.nstim[4]);
    }
    
    
    
    /*
     * at the start of the staircase use a large stepsize so that it doesn't
     * matter if it starts at very suprathreshold values. This way can set a 
     * smallish increment and large nstim = no bias introduced by protocol
     */
    
    if(stimctr == 0){
        nextstim[1] = stimorder[1];
        for(i = 0; i < 2 * nstairs * nexps; i++)
            reverses[i] = laps[i] = 0;
        for(i = 2; i < nstairs * nexps; i += 2){
            rnd = myrnd_i(); 
            if(optionflags[STAIR_CENTER]){
                stimorder[i] = expt.nstim[0]/2 - 5 + rnd%11;
                stimorder[i + 1] = expt.nstim[0]/2 - 5 + (rnd >> 4) %11;
            }
            else{
                stimorder[i+ (rnd&1)] = expt.nstim[0]- 1 - ((rnd >>2)&1);
                stimorder[i + (!(rnd&1))] = (rnd >> 1) &1;
            }
            nextstim[i] = stimorder[i];
            nextstim[i+1] = stimorder[i+1];
        }
    }
    
    if(revise){
        reverses[stairid] = lastreverses[stairid];
        nstep[stairid] = laststep[stairid];
        lastup[stairid] = lastlastup[stairid];
        laststim[stairid] = lastlaststim[stairid];
        stairid = laststairid;
    }
    else{
        lastreverses[stairid] = reverses[stairid];
        laststep[stairid] = nstep[stairid];
        lastlastup[stairid] = lastup[stairid];
        lastlaststim[stairid] = laststim[stairid];
        laststairid = stairid;
    }
    
    if(stimctr < 2 * nexps){
        if(!revise)
            stairid = (stimctr % (nstairs * nexps));
        if(expt.nstim[0] > 60)
            istep = 15;
        else if(expt.nstim[0] > 40)
            istep = 10;
        if(expt.nstim[0] > 16)
            istep = 7;
        else if(expt.nstim[0] > 10)
            istep = 5;
        else
            istep = 3;
        for(i = 0; i < nstairs * nexps; i++)
            nstep[i] = istep;
    }
    else if(up != lastup[stairid] && nstep[stairid] > 1){
        if(nstep[stairid] > 10)
            nstep[stairid] -= 5;
        else if(nstep[stairid] > 6)
            nstep[stairid] -= 3;
        else if(nstep[stairid] > 4)
            nstep[stairid] -= 2;
        else
            nstep[stairid]--;
    }
    else{ /* nstep = 1 and stimctr > 4 = now reversals count */
        if(up != lastup[stairid]){
            printf("Reverse %d: up %d last %d\n",stairid, up,lastup[stairid]);
            reverses[stairid]++;
        }
    }
    expid = stairid/nstairs;
    stimseq[stimctr].result = up;
    stimseq[stimctr].stairid = stairid;
    stimid = (stimorder[stimctr] & ~(ORDER_BITS)) % expt.nstim[0];
    if(up < 0)
        next = stimid+nstep[stairid];
    else
        next = stimid-nstep[stairid];
    /* 
     * step back 5 incrs after 3 reversals. N.B. "up" should have opposite signs fo
     * the two staircases, so this should set "next" to opposite extremes.
     */
    
    /*
     * if Search mode is set, don't count reversals, just let it run forever.
     * used for demos to use program for a method of adjustment
     */
    
    if(reverses[stairid] >= nreverses && !(optionflag & SEARCH_MODE_BIT)){ 
        next = stimid + 3 * up;
        reverses[stairid] = 0;
        if(seroutfile)
            fprintf(seroutfile,"Resetting %d to %d\n",stairid,next);
        printf("Resetting %d\n",stairid);
        laps[stairid]++;
    }
    if(stairfd)
        fprintf(stairfd,"%d Now %d (%.2f) Next %d is %d up%c%d (R%d,%d)\n",stimctr,stimid,expval[stimid],stairid, next,c,up,reverses[stairid],laps[stairid]);
    printf("%d Now %d (%.2f) Next %d is %d up%c%d (R%d,%d)\n",stimctr,stimid,expval[stimid],stairid, next,c,up,reverses[stairid],laps[stairid]);
    if(seroutfile)
        fprintf(seroutfile,"%d Now  %d (%.2f) Next %d is %d up%c %d (R%d,%d)\n",stimctr,stimid,expval[stimid],stairid, next,c,up,reverses[stairid],laps[stairid]);
    
    
    stop = 1;
    for(i = 0; i < nstairs * nexps; i++){
        if(laps[i] < expt.nreps)
            stop = 0;
    }
    if(stop)
        expt.st->mode |= EXPT_OVER;
    
    
    /* 
     * set lastup to 0 if wanted a stimulus over the limit. That way, asking again 
     * for the same direction will count as a reversal, so that being stuck at 
     * the limits will count, and bring the run to an end
     */
    lastup[stairid] = up;
    if(next < 0){
        next = 0;
        lastup[stairid] = 0;
    }
    if(next >= expt.nstim[0]){
        lastup[stairid] = 0;
        next = expt.nstim[0]-1;
    }
    
    /*  if we are up against the limit, count it as a reversal */
    if(stimctr > 4 && ((next == maxid && laststim[stairid] == maxid) ||
                       (next == 0 && laststim[stairid] == 0))){
        if(nstep[stairid] > 1)
            nstep[stairid] = 1;
        else
            reverses[stairid]++;
    }
    nextstim[stairid] = next;
    laststim[stairid] = stimorder[stimctr];
    
    if(seroutfile)
        fprintf(seroutfile,"ID %d next %d\n",stairid,next);
    if(stimctr > 4){
        rnd = myrnd_i();
        stairid = (rnd % (nstairs * nexps));
    }
    else{
        stairid = (stimctr+1)% (nstairs * nexps);
    }
    
    
    /* 
     * This shouldn't happen, but it does seem too.....
     */
    if(nextstim[stairid] >= expt.nstim[0])
        nextstim[stairid] = expt.nstim[0]-1;
    if(nextstim[stairid] < 0)
        nextstim[stairid] = 0;
    
    exp3flag = 0;
    expid = stairid/nstairs;
    if(expid >= expt.nstim[1] && expt.nstim[1] > 1){
        expid = expid % expt.nstim[1];
        exp3flag = 1;
    }
    
    if(nexps > 1)
        stimorder[stimctr+1] = nextstim[stairid] + (expid * expt.nstim[0]);
    else
        stimorder[stimctr+1] = nextstim[stairid];
    if(exp3flag)
        stimorder[stimctr+1] |= STIMULUS_EXTRA_EXPT3;
    
    
    if(stimorder[stimctr+1] < 0 || stimorder[stimctr+1] > (expt.nstim[0] * expt.nstim[1]))
        fprintf(stderr,"Stimulus %d pos %d is bad\n",stimorder[stimctr+1],stimctr);
    if(next > expt.nstim[0])
        fprintf(stderr,"Next too big %d\n",next);
    if(seroutfile)
        fflush(seroutfile);
    
    return(next);
}
void write_menus(FILE *ofd)
{
    int i,k;
    
    if(showexp[0][TOTALCODES]){ //only some expts on menu
        fprintf(ofd,"exps=");
        for(k = 0; k < nexpshow[0]; k++){
            for(i = 0; i < nexptypes[0]; i++)
                if(firstmenu[i].val == expshoworder[0][k])
                    fprintf(ofd,"+%s",serial_strings[firstmenu[i].val]);
        }
        for(i = 0; i < nexptypes[0]; i++)
            if(!showexp[0][firstmenu[i].val] && firstmenu[i].val < MAXTOTALCODES)
                fprintf(ofd,"-%.2s",serial_strings[firstmenu[i].val]);
        if(showexp[1][TOTALCODES]){ //only some expts on menu
            fprintf(ofd,":");
            for(k = 0; k < nexpshow[1]; k++){
                for(i = 0; i < nexptypes[1]; i++)
                    if(secondmenu[i].val == expshoworder[1][k])
                        fprintf(ofd,"+%.2s",serial_strings[secondmenu[i].val]);
            }
            for(i = 0; i < nexptypes[1]; i++)
                if(!showexp[1][secondmenu[i].val] && secondmenu[i].val < MAXTOTALCODES)
                    fprintf(ofd,"-%.2s",serial_strings[secondmenu[i].val]);
        }
        fprintf(ofd,"\n");
    }
}


unsigned int ufftime(struct timeval *thetime)
{
    unsigned int ticks;
    ticks = bwticks + timediff(thetime,&bwtime) * 10000;
    return(ticks);
}


void PrintCodes(int mode)
{
    char s[BUFSIZ*2],tmp[BUFSIZ],ctype = 'N';
    int i,showcode = 1;
    
    sprintf(s,"");
    for(i = 0; i < expt.totalcodes; i++)
    {
        sprintf(tmp,"CODE %s %d %s%c %d\n",valstrings[i].code,valstrings[i].icode,valstrings[i].label,valstrings[i].ctype,valstrings[i].group);
        notify(tmp);
    }
    sprintf(tmp,"CODE OVER\n");
    notify(tmp);
    i = 0;
    while(commstrings[i].code != NULL){
        sprintf(tmp,"SCODE %s %d %s\n",commstrings[i].code,commstrings[i].icode,commstrings[i].label);
        notify(tmp);
        i++;
    }
    SendExptTypesToGui();
    SendToggleCodesToGui();
}


int SendToggleCodesToGui()
{
    int i;
    char buf[BUFSIZ],tmp[BUFSIZ];
    
    i = 0;
    while(togglestrings[i].code != NULL){
        sprintf(buf,"TOGGLE %s %s\n",togglestrings[i].code,togglestrings[i].label);
        notify(buf);
        i++;
    }
    for (i = 0; i < N_STIMULUS_TYPES; i++){
        sprintf(buf,"STIMTYPE %d %s\n",i,stimulus_names[i]);
        notify(buf);
    }
    
}
// sends list of available expt types
//not current exot vals
int SendExptTypesToGui()
{
    int i;
    char s[BUFSIZ],tmp[BUFSIZ];
    
    sprintf(s,"Expts1 ");
    i = 0;
    while(firstmenu[i].label != NULL){
        sprintf(tmp,"%d ",firstmenu[i++].val);
        strcat(s,tmp);
    }
    strcat(s,"\n");
    notify(s);
    sprintf(s,"Expts2 ");
    i = 0;
    while(secondmenu[i].label != NULL){
        sprintf(tmp,"%d ",secondmenu[i++].val);
        strcat(s,tmp);
    }
    strcat(s,"\n");
    notify(s);
    sprintf(s,"Expts3 ");
    i = 0;
    while(thirdmenu[i].label != NULL){
        sprintf(tmp,"%d ",thirdmenu[i++].val);
        strcat(s,tmp);
    }
    strcat(s,"\n");
    notify(s);
    
    //    PrintToggles(stdout);
}

void PrintPenLog(int scroll)
{
    char s[BUFSIZ*100],tmp[BUFSIZ],depth[BUFSIZ];
    int i,newdepth =0;
    FILE *fd;
    
    sprintf(s,"pen%d.log",expt.ipen);
    expt.penfile = myscopy(expt.penfile,s);
    if((fd = fopen(s,"r")) != NULL){
        strcat(s,":\n");
        while(fgets(tmp,BUFSIZ,fd)){
            if(!strncmp(tmp,"ed ",3)){
                strcpy(depth,tmp);
                newdepth = 1;
            }
            else if(strlen(s) + strlen(tmp) < BUFSIZ * 100 && !strstr(tmp,"bwticks"))
            {
                if(newdepth)
                    strcat(s,depth);
                newdepth = 0;
                strcat(s,tmp);
            }
        }
        fclose(fd);
    }
    if(expt.ipen == 0){
        sprintf(tmp,"\nNext penetration is %d",GetLastPen()+1);
        strcat(s,tmp);
    }
}

int InitRndArray(long seed, int len)
{
    int i;
    static int rndbuflen = 0;
    struct timeval then;
    
    if(len > rndbuflen){
        if(rndbuf != NULL){
            free(rndbuf);
        }
        rndbuf= (unsigned long *)(malloc(len * sizeof(unsigned long)));
        rndbuflen = len;
    }
    gettimeofday(&then,NULL);
    for(i = 0; i < len; i++)
        rndbuf[i] = random();
    gettimeofday(&now, NULL);
    printf("%d rands took %.4f\n",len,timediff(&now,&then));
    return(len);
}

void ExptInit(Expt *ex, Stimulus *stim, Monitor *mon)
{
    int i,j,code;
    struct plotdata *plot;
    time_t tval;
    int codes[BUFSIZ],ncodes = 0;;
    
    
    tval = time(NULL);
    lastcmdread = time(NULL);
    winposns[STEPPER_WIN].x = 80;
    winposns[STEPPER_WIN].y = 10;
    winposns[PENLOG_WIN].x = 80;
    winposns[PENLOG_WIN].y = 10;
    
    i = 0;
    while(togglestrings[i].code != NULL){
        if (togglestrings[i].group == 1 && togglestrings[i].icode == SQUARE_RDS)
            toggleidx[togglestrings[i].icode] = i;
        else if (togglestrings[i].group == 3)
            toggleidx[togglestrings[i].icode] = i;
        i++;
    }

    i = 0;
    while((code = valstrings[i].icode) >= 0){
        codes[i] = 0;
        if (valstrings[i].icode > ncodes)
            ncodes = valstrings[i].icode;
        i++;
    }
    ncodes = ncodes+1;
    expt.totalcodes = i;
    expt.maxcode = ncodes-1;
// serial_stings[i] gives the string associated with code i
    serial_strings = (char**)(malloc(sizeof(char *) * ncodes));
    for (i = 0; i < ncodes; i++)
        serial_strings[i] = NULL;
    
    i = 0;
    j = 0;
    while((code = valstrings[i].icode) >= 0){
//        if (codes[code] > 0) //already done
//            fprintf(stdout,"Already Done %d,%s\n", code, valstrings[code].code);
//            else
//                valstrings[code].code = valstrings[i].code;
        if (valstrings[i].code != NULL){
        valstringindex[code] = i; // The valstring element that has code icode in
  //      codes[code] = valstrings[i].code;
        serial_names[code] = valstrings[i].label;
        serial_strings[code] = valstrings[i].code;
        if(code == LAST_STIMULUS_CODE)
            expt.laststimcode = i;
        if(code == MAXSERIALCODES)
            expt.lastserialcode = i;
        if(code == MAXSAVECODES)
            expt.lastsavecode = i;
        nfplaces[code] = valstrings[i].nfplaces;
        codesend[code] = valstrings[i].codesend;
        if(strlen(serial_strings[code]) > 2)
            longnames[j++] = code;
        }
        i++;
    }
    
    i = 0;
    j = 0;
    while(serial_strings[i] != NULL){
        if(strlen(serial_strings[i]) > 2)
            longnames[j++] = i;
        i++;
    }
    for(i = 2; i < MAXWINS; i++){
        winposns[i].x = 80;
        winposns[i].y = 200;
    }
    if(cmdhistory == NULL){
        if((cmdhistory = fopen("./binoc.history","a")) != NULL)
            fprintf(cmdhistory,"\nReopened %s",ctime(&tval));
    }
    gettimeofday(&bwtime,NULL);
    stimseq = (Thisstim *)malloc((TRIALBUFFERLEN+1) * sizeof(Thisstim));
    pgimage.ptr = NULL;
    pgimage.name = NULL;
    ex->verbose = 0;  //controls NSlog
    ex->biasedreward = 0;
    ex->backim.name = NULL;
    ex->backim.ptr = NULL;
    ex->hasdata = 0;
    ex->st = stim;
    ex->penfile = NULL;
    ex->backprefix = NULL;
    ex->cmdinfile = NULL;
    for(i = 0; i < MAXBACKIM; i++)
        backims[i].ptr = NULL;
    
    if(ex->pnum == NULL)
        ex->pnum = myscopy(ex->pnum, "?");
    ReadHelpDir("/bgc/bgc/c/binoc/help");
    if(ex->helpfile == NULL)
        ex->helpfile = myscopy(ex->helpfile, "/bgc/bgc/c/binoc/helpfile");
    ex->blksize = 1;
    ex->stimpertrial = 1;
    ex->type2 = EXPTYPE_NONE;	
    ex->type3 = EXPTYPE_NONE;
    ex->mon = mon;
    ex->nstim[2] = 0;
    ex->nstim[4] = 1;
    ex->trials_per_stim = 1;
    ex->plotcluster = 1;
    ex->bwptr = &thebwstruct;
    ex->bwptr->nchans = 16;
    /* Default Color Scheme */
    ex->bwptr->colors[0] = 14; /*Bright Green */
    ex->bwptr->colors[1] = 13; /* Bright Red */
    ex->rf = (Expstim *)malloc(sizeof(Expstim));
    ex->rf->flag |= CENTERMARK_ON;
    ex->rf->size[0] = 100;
    ex->rf->size[1] = 50;
    ex->rf->type = RF_BOX;
    ex->rf->angle = 0;
    ex->nlines = 0;
    ex->linw = 1.0;
    ex->targetcolor = 1.0;
    ex->vals[PREWARD] = 0.5; // on ambiguous trials
    ex->vals[FIX_LINW] = 0.016;
    ex->vals[TARGET_RATIO] = 1;
    ex->vals[HIGHSF] = 16;
    ex->vals[HIGHTF] = 36;
    ex->vals[IFCSCALE] = 1;
    ex->vals[PLC_MAG] = 0.5;
    ex->vals[FAST_SEQUENCE_RPT] = 1;
    ex->vals[TARGET_XOFFSET] = 0;
    ex->vals[TARGET_YOFFSET] = 0;
    ex->vals[BACKGROUND_ZOOM]  = 1.0;
    ex->vals[VSR]  = 1.0;
    ex->vals[COVARIATE]  = SETZXOFF;
    ex->vals[FORCE_CHOICE_ANGLE] = -1000; //off
    
    for(i = 2; i < ex->bwptr->nchans; i++)
        ex->bwptr->colors[i] = 1 + ((i+1)%15);
    
    watchcodes[STIMULUS_MODE] = SEND_NON_ZERO;
    i = 0;
    while(firstmenu[i].label != NULL){
        expshoworder[0][i] = firstmenu[i].val;
        if(i < 30)
            showexp[0][firstmenu[i++].val] = 1;
        else
            showexp[0][firstmenu[i++].val] = 0;
    }
    nexpshow[0] = nexptypes[0] = i;
    
    if((expt.st->type == STIM_RDS || expt.st->type == STIM_RLS) &&
       expt.st->left->seedloop == 0)
        expt.vals[CHANGE_SEED] = 0;
    
    i = 0;
    while(secondmenu[i].label != NULL){
        expshoworder[1][i] = secondmenu[i].val;
        if(i < 30)
            showexp[1][secondmenu[i++].val] = 1;
        else
            showexp[1][secondmenu[i++].val] = 0;
    }
    nexpshow[1] = nexptypes[1] = i;
    
    
    i = 0;

    

    
    expplots = (struct plotdata *)malloc(sizeof(struct plotdata) * (NPLOTDATA+1));
    pursuedir = 1;
    for(i = 0; i <= NPLOTDATA; i++)
    {
        expplots[i].nstim[0] = 0;
        expplots[i].nstim[1] = 0;
        expplots[i].stims = NULL;
        expplots[i].label = NULL;
        expplots[i].nlabels = 0;
        expplots[i].pos[0] = 0;
        expplots[i].pos[1] = 0;
        expplots[i].size[0] = 200;
        expplots[i].size[1] = 200;
        expplots[i].fplaces = 1;
    }
    expplots[0].linedata = (short *)malloc((MAXUSERLINES+1) * 4 * sizeof(short));
    ex->vals[EARLY_RWSIZE] = 0.05;
    ex->plot = &expplots[0];
    ex->rcframes = (int *)malloc(MAXRCFRAMES * sizeof(int));
    ex->rccounts = (int *)malloc(MAXRCFRAMES * sizeof(int));
    rcframe = 0;
    for(i = 0; i < MAXRCFRAMES; i++)
        ex->rcframes[i] = ex->rccounts[i] = 0;
    ex->rcn = (int *)malloc(MAXFRAMES * sizeof(int));
    ex->rcvals = (float *)malloc(MAXFRAMES * sizeof(float));
    rcstimid = (int *)malloc(MAXFRAMES * sizeof(int));
    
    rcstimvals = (float **)malloc(10 * sizeof(float *));
    for(i = 0; i < 10; i++)
        rcstimvals[i] = (float *)malloc(MAXFRAMES * sizeof(float));
    
    rclfreqs = (float **)malloc(MAXFREQS * sizeof(float *));
    rcrfreqs = (float **)malloc(MAXFREQS * sizeof(float *));
    rcdps = (int **)malloc(MAXFREQS * sizeof(int *));
    for(i = 0; i < MAXFREQS; i++){
        rclfreqs[i] = (float *)malloc(MAXFRAMES * sizeof(float));
        rcrfreqs[i] = (float *)malloc(MAXFRAMES * sizeof(float));
        rcdps[i] = (int *)malloc(MAXFRAMES * sizeof(int));
    }
    
    rcstimxy = (int **)malloc(2 * sizeof(float *));
    for(i = 0; i < 2; i++)
        rcstimxy[i] = (int *)malloc(MAXFRAMES * sizeof(int));
    rcstimframetimes = (float *)malloc(MAXFRAMES * sizeof(int));
    sframetimes = (float *)malloc(MAXFRAMES * sizeof(int));

    afc_s.lasttrial = -(BAD_TRIAL);
    afc_s.targsize = 20;
    if(ex->nstim[0] <= 0)
    {
        ex->nstim[0] = 5;
        ex->nreps = 1;

    }
    nisset = MAXSTIM;
    isset = (int *)malloc(sizeof(int) * nisset);
    nstimorder = MAXSTIM * MAXREPS;
    seedorder = (int *)malloc(sizeof(int) * nstimorder);
    stimorder = (int *)malloc(sizeof(int) * nstimorder);
    stim2order = (int *)malloc(sizeof(int) * nstimorder);
    stim3order = (int *)malloc(sizeof(int) * nstimorder);
    psychclear(ex->plot,1);
    
    if(expseed == 0)
        expseed = getpid();
    
    j = 0;
    while(firstmenu[j].label != NULL){
        if((i = firstmenu[j].val) >= MAXSERIALCODES)
        {
            if(i > MAXTOTALCODES)
                printf("Expt %s %d(%d) > MAXTOTALCODES\n",firstmenu[j].label,j,i);
            else
                printf("Expt %s %d(%d,%s: %s) > MAXSERIALCODES\n",firstmenu[j].label,j,i,serial_strings[i],serial_names[i]);
        }
        j++;
    }
    j = 0;
    while(secondmenu[j].label != NULL){
        if((i = secondmenu[j].val) >= MAXSERIALCODES)
        {
            if(i > MAXTOTALCODES)
                printf("Expt %s %d(%d) > MAXTOTALCODES\n",secondmenu[j].label,j,i);
            else
                printf("Expt %s %d(%d,%s: %s) > MAXSERIALCODES\n",secondmenu[j].label,j,i,serial_strings[i],serial_names[i]);
        }
        j++;
    }
    afc_s.proportion = 0.5;
    afc_s.sign=1;
    afc_s.target_in_trial = 0;
    
    SetExptString(ex, ex->st, MONKEYNAME, "none");
}






int ReadCommandFile(char *file)
{
    struct stat statbuf;
    FILE *fin;
    char *s;
    
    if(file == NULL)
        return(0);
    if(stat(file, &statbuf) == -1)
        return(0);
    
    if(statbuf.st_mtime > lastcmdread) /* file has been modified */
    { 
        fprintf(stdout,"last read was at %s",ctime(&lastcmdread));
        fprintf(stdout,"cmdfile modified at %s",ctime(&statbuf.st_mtime));
        if(seroutfile){
            fprintf(seroutfile,"#cmdfile modified at %s,",nonewline(ctime(&statbuf.st_mtime)));
            fprintf(seroutfile," since %s",ctime(&lastcmdread));
        }
        
        usleep(100000);
        fin = fopen(file,"r");
        while((s = fgets(mssg, BUFSIZ, fin)) != NULL){
            if(mssg[0] == '\!'){
                ReadCommand(&mssg[1]);
            }
            else 
                InterpretLine(mssg, &expt,3);
        }
        fclose(fin);
        lastcmdread = time(NULL);
        fprintf(stdout,"Time now %s",ctime(&lastcmdread));
        if (statbuf.st_mtime > lastcmdread) // clock mismatch
            lastcmdread = statbuf.st_mtime;
    }
    
    
}

int ReadHelpDir(char *dir)
{
    DIR *dirp;
    struct stat s_buf;
    char name[BUFSIZ],cbuf[BUFSIZ];
    FILE *fd;
    
#if defined(Cygwin) || defined(SunOS)
    struct dirent *dp;
#else
    struct direct *dp;
#endif
    
    if(stat(dir, &s_buf) == -1 || !(s_buf.st_mode & S_IREAD))
        return(0);
    if (!(s_buf.st_mode & S_IFDIR))
        return(0);
    if(!(dirp = opendir(dir)))
        return(0);
    while(dp = readdir(dirp)){
        if(strstr(dp->d_name,".hlp") && dp->d_name[strlen(dp->d_name)-1] == 'p'){
            sprintf(name,"%s/%s",dir,dp->d_name);
            if((fd = fopen(name,"r"))){
                fgets(cbuf,BUFSIZ,fd);
                close(fd);
                helplabels[nhelpfiles] = myscopy(helplabels[nhelpfiles],nonewline(cbuf));
                helpfiles[nhelpfiles] = myscopy(helpfiles[nhelpfiles],name);
                nhelpfiles++;
            }
        }
    }
    closedir(dirp);
}


void ListQuickExpts()
{
    int i = 0;
    char buf[BUFSIZ];
    
    for (i = 0; i < nquickexpts; i++){
        sprintf(buf,"qe=%s\n",quicknames[i]);
        notify(buf);
    }
    if (expt.showflags != NULL){
        sprintf(buf,"pf=%s\n",expt.showflags);
        notify(buf);
    }
            
}

void ListExpStims(int w)
{
    int i;
    char buf[256],cbuf[256];

    if(!(mode & RUNNING) || freeToGo == 0)
        return;

    notify("ECLEAR\n");
    for(i = 0; i < (expt.nstim[0]+expt.nstim[2]); i++)
    {
        MakePlotLabel(&expt, cbuf, i, 0);
        sprintf(buf, "E%d=%s\n",i,cbuf);
        notify(buf);
    }
    
    notify("EBCLEAR\n");
    for(i = expt.nstim[0]+expt.nstim[2]; i < (expt.nstim[0]+expt.nstim[2]+expt.nstim[1]) ; i++)
    {
        MakePlotLabel(&expt, cbuf, i, 0);
        sprintf(buf, "EB%d=%s\n",i-(expt.nstim[0]+expt.nstim[2]),cbuf);
        notify(buf);
        
    }
    notify("ECCLEAR\n");
    for(i = 0; i < expt.nstim[4]; i++){
        sprintf(buf,"EC%d=%.2f\n",i,expt.exp3vals[i]);
        notify(buf);
    }
    notify("EDONE\n");
}


int GetTotal(struct plotdata *plot, int cluster, int type)
{
    int total = 0,n,i;
    
    for(i = 0; i < plot->nstim[5]; i++)
    {
        if((n = plot->stims[i].nreps[cluster][type]) > 0)
            total += n;
    }
    return(total);
}

void psychclear(struct plotdata *plot, int allflag)
{
    Expstim *es = plot->stims;
    int i;
    
    if(allflag)
        clear_afccounters();
    return;
    expt.hasdata = 0;
}


/*
 * i2expi works out which entry in expval has the simulus value associated with
 * a particular index value
 *
 */
int i2expi(int flag, int *nstim, int ival, int type)
{
    int i;
    
    
    if(type == 2)
        return(ival/nstim[3]);
    else
        ival = ival % nstim[3];
    
    if(flag & ADD_EXPT2)
        i = ival;
    if(flag & TIMES_EXPT2)
    {
        if(type == 0) /* send val of expt 1 at this val of expt2 */
            i = nstim[2] + (ival - nstim[2])%nstim[0];
        else /* send val of expt2 */
            i = nstim[0] + nstim[2] + (ival - nstim[2])/nstim[0];
    }
    else
        i = ival;
    
    return(i);
}

double i2expval(int ival, int extras, int type, int skipx)
{
    int i;
    int nstim[5];
    double result;
    
    for(i = 0; i < 5; i++)
        nstim[i] = expt.nstim[i];
    nstim[2] = extras;
    
    i = i2expi(expt.flag, nstim, ival, type);
    result = expval[i];
    if(i < extras)
        result = extravals[i+skipx];
    return(result);
}



int OpenPsychLog(char *name)
{
    char buf[BUFSIZ]; 
    time_t tval;
    static char psychlogname[BUFSIZ];
    
    if(name == NULL && psychlog == NULL)
        name = expt.logfile;
    if(name != NULL && psychlog != NULL && strcmp(psychlogname,name)){
        fclose(psychlog);
        psychlog = NULL;
    }
    
    if(psychlog == NULL){
        strcpy(psychlogname,name);
        sprintf(buf,"%s.plog",name);
        if((psychlog = fopen(buf,"a")) == NULL){
            fprintf(stderr,"Error opening %s\n",buf);
            return(-1);
        }
        tval = time(NULL);
        fprintf(psychlog,"\nReopened %s",ctime(&tval));
    }
    return(0);
}

int OpenLogfile(char *name)
{
    FILE *fd;
    char s[BUFSIZ],*t,*r,ina[256],inb[256];
    int chan, ischan;
    
    if((fd = fopen(name,"r")) != NULL)
    {
        while(fgets(s,BUFSIZ,fd) != NULL)
        {
            chan = -1;
            if(!strncmp(s,"we",2))
                sscanf(&s[3],"%f %d",&thelog.weight,&thelog.fluid);
            else if(!strncmp(s,"re",2))
            {
                sscanf(&s[3],"%s %s",ina,inb);
                thelog.restrain = (char *)myscopy(thelog.restrain, ina);
                thelog.free = (char *)myscopy(thelog.free,inb);
            }
            else if(!strncmp(s,"rh",2))
                chan = 0;
            else if(!strncmp(s,"rv",2))
                chan = 2;
            else if(!strncmp(s,"lh",2))
                chan = 1;
            else if(!strncmp(s,"lv",2))
                chan = 3;
            if(chan >= 0) /* recorded softoffs, not actual */
            {
                r = &s[2];
                sscanf(r,"%f %f %f %d",&(thelog.chan[chan].gain), &thelog.chan[chan].offset,
                       &(thelog.chan[chan].phase),&thelog.chan[chan].softoff);
            }
        }
        fclose(fd);
        return(0);
    }
    else
        return(-1);
}


void InterpretChannelLine(char *line, int chan)
{
    int i = 0;
    char *s;
    float val;
    
    while(channel_strings[i] != NULL)
    {
        if(strncmp(line, channel_strings[i],2) == 0)
            break;
        else    
            i++;
    }
    s = &line[2];
    if(chan < MAXCHANS)
        switch(i)
    {
        case EYESCALE_CODE:
            sscanf(s,"%f",&val);
            if(val > 0.0001)
                expt.bwptr->fsd[chan] = val;
            else if(expt.bwptr->fsd[chan] < 0.1)
                expt.bwptr->fsd[chan] = 5;
            break;
        case EYECOLOR_CODE:
            sscanf(s,"%d",&expt.bwptr->colors[chan]);
            break;
    }
}

int CheckPenetration()
{
    char buf[BUFSIZ];
    
    if(expt.pnum[0] == '\?'){
        sprintf(buf,"Penetration not set, last was %d",GetLastPen());
        acknowledge(buf,NULL);
        return(-1);
    }
    return(0);
}

int SendPenInfo(){
    char buf[BUFSIZ],xbits[BUFSIZ];
    
    sprintf(buf,"Experimenter %s\n",userstrings[userid]);
    if(expt.driveadapter)
        sprintf(xbits,"AngleAdapter ");
    else
        sprintf(xbits,"");
    if(expt.hemisphere == 1)
        strcat(xbits,"RightHemisphere ");
    strcat(xbits,"\n");
    strcat(buf, xbits);
    SerialString(buf,0);
    //Ali  if(protrudemm == 0 && protitem != NULL)
    //    protrudemm = texttoi(protitem);
    if(protrudemm){
        sprintf(buf,"Tube Protrudes %d mm ",protrudemm);
        if(coarsemm){
            sprintf(xbits,"at %d mm. ",coarsemm);
            strcat(buf,xbits);
        }
        SerialString(buf,0);
    }
    if(electrodeid){
        sprintf(buf," Electrode %s\n",electrodestrings[electrodeid]);
        SerialString(buf,0);
    }
}

// this function's job is to make sure all the necessary values for writing a pen log is in sync with the cocoa GUI
int UpdatePenInfo_Ali(float _penXpos, float _penYpos, int _angleAdapter, int _hemisphere, int _userid, int _protrudemm, int _electrodeid, int _impedance, int _penNumber)
{
    expt.vals[PENXPOS] = _penXpos;
    expt.vals[PENYPOS] = _penYpos;
    expt.driveadapter = _angleAdapter;
    expt.hemisphere = _hemisphere;
    userid = _userid;
    protrudemm = _protrudemm;
    electrodeid = _electrodeid;
    expt.vals[IMPEDANCE] = _impedance;
    expt.vals[PENNUMCOUNTER] = _penNumber;
    
    SendToGui(PENXPOS);
    SendToGui(PENYPOS);
    SendToGui(PENETRATION_TEXT);
    SendToGui(PENNUMCOUNTER);
    return 0;
}

int OpenPenetrationLog(int pen){
    char buf[BUFSIZ],xbits[BUFSIZ];
    
    time_t tval;
    
    if(pen < 0) /* == 0 used for default so that ed is never lost */
        pen = GetLastPen();
    if(expt.driveadapter)
        sprintf(xbits," AngleAdapter");
    else
        sprintf(xbits,"");
    if(expt.hemisphere == 1)
        strcat(xbits," RightHemisphere");
    
    if(pen > 0 && pen != expt.ipen){
        if(penlog)
            fclose(penlog);
        sprintf(buf,"pen%d.log",pen);
        expt.ipen = pen;
        expt.penfile = myscopy(expt.penfile,buf);
        if((penlog = fopen(buf,"a")) != NULL){
            tval = time(NULL);
            fprintf(penlog,"Opened %s pen %d %.1f,%.1f%s\n",nonewline(ctime(&tval)),expt.ipen,expt.vals[PENXPOS],expt.vals[PENYPOS],xbits);
            fprintf(stdout,"Logfile is %s\n",buf);
        }
        sprintf(buf,"%d %.1f,%.1f",expt.ipen,(expt.vals[PENXPOS]),(expt.vals[PENYPOS]));
        expt.pnum = myscopy(expt.pnum,buf);
        
    }
    else if(penlog){
        tval = time(NULL);
        fprintf(penlog,"Opened %s pen %d %.1f,%.1f%s\n",nonewline(ctime(&tval)),expt.ipen,(expt.vals[PENXPOS]),(expt.vals[PENYPOS]),xbits);
    }
    if(penlog){
        fprintf(penlog,"Experimenter %s\n",userstrings[userid]);
        
        //Ali      if(protrudemm == 0 && protitem != NULL)
        //	protrudemm = texttoi(protitem);
        if(protrudemm){
            fprintf(penlog,"Tube Protrudes %d mm",protrudemm);
            if(coarsemm)
                fprintf(penlog," at %d mm.",coarsemm);
            fprintf(penlog,"\n",protrudemm);
        }
        if(electrodeid)
            fprintf(penlog,"Electrode %s\n",electrodestrings[electrodeid]);
    }
    fflush(penlog);
    return(0);
}

int SetExptString(Expt *exp, Stimulus *st, int flag, char *s)
{
    int chan,pen,i,duplicate = 0;
    time_t tval;
    char *t,*r,buf[256],name[BUFSIZ],sfile[BUFSIZ],path[BUFSIZ];
    
    s = nonewline(s);
    if (*s == NULL)
        return(0);
    switch(flag)
    {
        case MONKEYNAME:
            expt.monkey = myscopy(expt.monkey,s);
            sprintf(expt.cwd,"/local/%s",expt.monkey);
            if (chdir(expt.cwd)!=0)
            {
                sprintf(buf,"Can't chdir to %s",expt.cwd);
                acknowledge(buf);
                sprintf(expt.cwd,"/local");
                if (chdir(expt.cwd)) {
                    acknowledge("Can't chdir to /local either");
                }
            }
            
            break;
            
        case HELPFILE_PATH:
            if(nhelpfiles == 0)
                expt.helpfile = myscopy(expt.helpfile,s);
            if(LabelAndPath(s,sfile,path,name) > 1)
                helplabels[nhelpfiles] = myscopy(helplabels[nhelpfiles],sfile);
            else
                helplabels[nhelpfiles] = myscopy(helplabels[nhelpfiles],name);
            helpfiles[nhelpfiles] = myscopy(helpfiles[nhelpfiles],path);
            for(i = 0; i < nhelpfiles; i++)
                if(!strcmp(helpfiles[nhelpfiles],helpfiles[i]))
                    duplicate++;
            if(!duplicate)
                nhelpfiles++;
            break;
        case PENETRATION_TEXT:
            expt.pnum = myscopy(expt.pnum,s);
            nonewline(expt.pnum);
            i = sscanf(s,"%d ",&pen);
            if(i > 0 && pen > 0 && pen != expt.ipen && strchr(s,' ')){
                if(penlog)
                    fclose(penlog);
                sprintf(buf,"pen%d.log",pen);
                if((penlog = fopen(buf,"a")) != NULL){
                    tval = time(NULL);
                    fprintf(penlog,"Opened %s",ctime(&tval));
                }
                expt.ipen = pen;
            }
            else if(penlog){
                tval = time(NULL);
                fprintf(penlog,"Opened %s pen %s\n",nonewline(ctime(&tval)),s);
            }
            break;
        case UFF_PREFIX:
            expt.bwptr->prefix = (char *)myscopy(expt.bwptr->prefix,nonewline(s));
            expname = (char *)myscopy(expname,nonewline(s));
            t = getfilename(expt.bwptr->prefix);
            
            
            if(!(option2flag & PSYCHOPHYSICS_BIT) && pcmode != SPIKE2){
                /*
                 * make local and network directories for online data files
                 expname = (char *)myscopy(expname,t);
                 sprintf(buf,"%s/%s",datprefix,expname);
                 
                 if(!isdir(buf)){
                 sprintf(buf,"mkdir %s/%s",datprefix,expname);
                 sprintf(name,"%s/%s",datprefix,expname);
                 system(buf);
                 sprintf(buf,"chgrp bgcdiv %s/%s",datprefix,expname);
                 chmod(name,02774);
                 }
                 sprintf(buf,"/bgc/data/online/%s",expname);
                 if(!isdir(buf)){
                 sprintf(buf,"mkdir /bgc/data/online/%s",expname);
                 system(buf);
                 sprintf(name,"/bgc/data/online/%s",expname);
                 chmod(name,02774);
                 }
                 */
                // Don't need this any more now we use spike2
            }
            if(seroutfile != NULL)
                fclose(seroutfile);
            
            tval = time(NULL);
            if(option2flag & PSYCHOPHYSICS_BIT){
                seroutfile = fopen("./psychtest.out","a");
                tval = time(NULL);
                fprintf(seroutfile,"Reopened %s",ctime(&tval));
            }
            else
            {
                if(testflags[PLAYING_EXPT])
                    sprintf(sfile,"%s_replay",t);
                else
                    strcpy(sfile,t);
                if((seroutfile = fopen(sfile,"a")) != NULL){
                    sprintf(buf,"Serial Out to %s/%s",expt.cwd,sfile);
                    statusline(buf);
                    fprintf(seroutfile,"Reopened %s by binoc Version %s",ctime(&tval),VERSION_NUMBER);
                    sprintf(buf,"Reopened %s",ctime(&tval));
                    SerialString(buf,NULL);
                }
                else
                {
                    sprintf(buf,"Can't open serial file %s:",sfile);
                    acknowledge(buf,NULL);
                }
            }
            if(t != NULL){
                free(t);
                t = NULL;
            }
            
            BackupStimFile();
            if(flag != TO_FILE && (mode & UFF_FILE_OPEN))
                CheckPenetration();
            SerialSend(RF_DIMENSIONS);
            expt.vals[VWHERE] = 0;
            rcctr = 0;
            break;
        case USERID:
            i = 0;
            nonewline(s);
            while(userstrings[i] && strcmp(s,userstrings[i]))
                i++;
            if(userstrings[i]){
                userid = i;
            }
            break;
        case ELECTRODE_TYPE:
            i = 0;
            nonewline(s);
            while(userstrings[i] && strcmp(s,userstrings[i]))
                i++;
            if(electrodestrings[i]){
                electrodeid = i;
            }
            SendPenInfo();
            break;
        case LOGFILE_CODE:
            if(expt.logfile == NULL || strcmp(s,expt.logfile))
            {
                expt.logfile = (char *)myscopy(expt.logfile,nonewline(s));
                OpenLogfile(expt.logfile);
            }
            if(option2flag & PSYCHOPHYSICS_BIT){
                if(seroutfile != NULL)
                    fclose(seroutfile);
                seroutfile = fopen("./psychtest.out","a");
                tval = time(NULL);
                fprintf(seroutfile,"Reopened %s",ctime(&tval));
                OpenPsychLog(expt.logfile);
            }
            break;
        case CHANNEL_CODE:
            sscanf(s,"%d",&chan);
            if((t = strchr(s,'+')) != NULL || (t = strchr(s,'-')) != NULL)
                SetBWChannel(chan,t);
            sprintf(buf,"ch%s\n",s);
            SerialString(buf,NULL);
            t = strchr(s,',');
            while(t && *t){
                InterpretChannelLine(++t,chan);
                r = t;
                t = strchr(r,',');
            }		
            break;
        case BACKGROUND_IMAGE:
            expt.backprefix = myscopy(expt.backprefix,s);
            break;
    }
    return(0);
}

void ShowTrialsNeeded(){
    char cbuf[BUFSIZ];
    
    if (freeToGo){
    sprintf(cbuf,"%d stim  x %d rpts = %d, %d trials",expt.nstim[5],expt.nreps,expt.nstim[5]*expt.nreps,(expt.nstim[5]*expt.nreps)/expt.stimpertrial);
    statusline(cbuf);
    }
}

int RecalcRepeats(Expt *exp)
{
    int i,diff;
    if(expt.vals[RC_REPEATS] < 1){
        return(0);
    }
    else{
        for(i = 0; i < exp->nstim[5]; i++){
            diff = uncompleted[i]+unrepeatn[i] - (exp->nreps - completed[i]);
            if(diff > 0)
                uncompleted[i] -= diff;
        }
    }
    
}

int SetProperty(Expt *exp, Stimulus *st, int code, float val)
{
    int i;
    if((i = SetExptProperty(exp, st, code, val)) < 0)
        i = SetStimulus(st, val, code, NULL);
    if (i < 0 && code <= expt.maxcode && valstringindex[code] > 0){
        expt.vals[code] = val;
        i = code;
    }
    return(i);
}

void ElectrodeDepth(int depth)
{
    SetProperty(&expt, expt.st, ELECTRODE_DEPTH, (float)(depth)/1000);
}

int SetExptProperty(Expt *exp, Stimulus *st, int flag, float val)
{
    Wurtz *fix = &(expt.st->fix);
    float x,y,theta,old,xymove;
    int i,new,ival = (int)rint(val);
    char cbuf[BUFSIZ];
    double dx,dy,sz;
    int postframes;
    int evtype = NULL;
    
    if(exp->cmdtype == 2) // from verg
        evtype = 0; //= up
    
    
    switch(flag)
    {
        case IMAGEJUMPS:
            expt.st->jumps = (int)val;
            break;
        case REWARD_BIAS:
            expt.biasedreward = val;
            break;
        case VERBOSE_CODE:
            expt.verbose = (int)(val);
            break;
        case BACK_PPOS:
            if(expt.st->next)
                SetStimulus(expt.st->next,val,PARA_POS, NOEVENT);
            break;
        case BACK_OPOS:
            if(expt.st->next)
                SetStimulus(expt.st->next,val,ORTHOG_POS, NOEVENT);
            break;
        case INITIAL_DURATION:
            if(expt.vals[CHANGE_SEED] > 0 && val > 0){ //only defined with CHANGE_SEED
                postframes = expt.st->nframes - expt.vals[CHANGE_SEED];
                expt.st->nframes = postframes+val;
                expt.vals[CHANGE_SEED] = val;
                expt.vals[flag] = val;
            }
            break;
        case TWOCYL_DISP:	
            SetStimulus(expt.st, val, DISP_X, NOEVENT);
            if(fabsf(val) > expt.vals[INITIAL_APPLY_MAX]){
                if (expt.stimmode == TWOCYL_BACKZERO || expt.stimmode == TWOCYL_ANDIDISP)
                    SetStimulus(expt.st, 0, DISP_BACK, NOEVENT);
                else
                    SetStimulus(expt.st, val, DISP_BACK, NOEVENT);
            }
            else{
                sz = GetProperty(&expt, expt.st, BACK_SIZE);
                if (sz < 0.02 && expt.stimmode == TWOCYL_ANDIDISP){
                    SetProperty(&expt, expt.st,  INITIAL_DISPARITY, expt.vals[HIGHX]);
                    SerialSend(INITIAL_DISPARITY);
                }
                else
                    SetStimulus(expt.st, expt.vals[HIGHX], DISP_BACK, NOEVENT);
            }
            SerialSend(DISP_X);
            SerialSend(DISP_BACK);
            break;
        case SETANNULUS:
            if(val == STIM_RDS){
                optionflags[PAINT_THIRD_LAST] = 1;
                optionflags[PAINT_BACKGROUND] = 1;
            }
            else if(val == STIM_ANNULUS){
                optionflags[PAINT_THIRD_LAST] = 0;
                optionflags[PAINT_BACKGROUND] = 1;
            }
            else if(val == STIM_NONE){
                optionflags[PAINT_THIRD_LAST] = 0;
                optionflags[PAINT_BACKGROUND] = 0;
            }
            SerialSend(OPTION_CODE);
            SerialSend(SETANNULUS);
            break;
        case BACKGROUND_MOVIE:
        case BACKGROUND_IMAGE:
            if((val < backloaded && val >= 0) || 1){ // don;t need to check if load each movie
                if (expt.stimmode == BUTTSEXPT){ // change at start of ExptStim
                    expt.vals[flag] = val;
                }
                else{
                    if(val < backloaded && val >= 0)
                       expt.backim = backims[(int)(val)];
                    expt.vals[flag] = val;
                }
            }
            break;
        case PANEL_XPOS:
            panelx = (Position)val;
            break;
        case PANEL_YPOS:
            panely = (Position)val;
            break;
        case STIMULUS_MODE:
            expt.stimmode = (int)val;
            if(expt.st->type == STIM_GRATINGN)
                SetGratingFrequencies(expt.st);
            if(expt.stimmode == FLAT_CYLINDER){
                expt.st->flag |= (FLAT_SURFACES | FLAT_DISPARITY | NO_WRAP);
            }
            if(expt.stimmode == NORMAL_CYLINDER){
                expt.st->flag &= (~(FLAT_SURFACES | FLAT_DISPARITY | NO_WRAP));
                expt.stimmode = 0;
            }
            break;
        case STIMID:
            expt.allstimid = (int)val;
            break;
        case JUMPTYPE:
            if(val == JUMP_FP_PARA || val == JUMP_STIM_PARA)
                SetProperty(exp, st, FP_MOVE_DIR, expt.rf->angle);
            else
                SetProperty(exp, st, FP_MOVE_DIR, expt.rf->angle+90);
            if(val == JUMP_STIM_ORTHO || val == JUMP_STIM_PARA)
                optionflags[SIMULATE_FP_MOVE] = 1;
            else
                optionflags[SIMULATE_FP_MOVE] = 0;
            if(val == JUMP_STIM_CHANGE){
                expt.vals[CHANGE_SEED] = expt.vals[FP_MOVE_FRAME];
            }
            else if(fabs(expt.vals[CHANGE_SEED] - expt.vals[FP_MOVE_FRAME]) < 1)
                expt.vals[CHANGE_SEED] = 0;
            break;
        case WEIGHT_TEXT:
            thelog.weight = val;
            break;
        case RCFILE_CTR:
            rcctr = (int)val;
            break;
        case RESTRAIN_TEXT:
            if(thelog.restrain)
                sprintf(thelog.restrain,"%.2f",val);
            break;
        case FP_MOVE_SIZE:
            expt.vals[flag] = val;
            break;
        case FP_MOVE_DIR:
            expt.vals[flag] = deg2rad(val);
            break;
        case EXPT_LINW:
            expt.linw  = (int)val;
            break;
        case FP_MOVE_FRAME:
        case CHANGE_SEED:
        case FIX_LINW:
        case ELECTRODE_DEPTH:
        case FIXATION_OVERLAP:
        case BACKJUMP:
        case FIXATION_SURROUND:
        case SETCLEARCOLOR:
        case BLANKCOLOR_CODE:
        case RANDOM_X:
        case RANDOM_ORI:
        case RF_SIGN:
        case TRIGGER_LEVEL1:
        case TRIGGER_LEVEL2:
        case TRIGGER_LEVEL3:
        case TRIGGER_LEVEL4:
        case PLOTSMOOTH:
        case RC_DELAY:
        case TARGET_RATIO:
        case HIGHSF:
        case HIGHTF:
        case HIGHX:
        case EARLY_RWTIME:
        case STEPPER_PLOTLEN:
        case TIMEOUT_CONTRAST:
        case TRIAL_START_BLANK:
        case TEST_VALUE1:
        case TEST_VALUE2:
        case BACK_ADD_SIZE:
        case PENXPOS:
        case PENYPOS:
        case FIXPT_DISPARITY:
        case IFCSCALE:
        case FIXPT_EYE:
        case AUTO_ZERO:
        case FIXWIN_HEIGHT:
        case VERTICAL_VERGENCE:
        case SACCADE_DETECTED:
        case SACCADE_THRESHOLD:
        case SACCADE_PUNISH_DIRECTION:
        case INITIAL_DISPARITY:
        case INITIAL_APPLY_MAX:
        case PLC_MAG:
        case PULSE_WIDTH:
        case STIM_PULSES:
        case SHAKE_TIMEOUT_DURATION:
        case FAST_SEQUENCE_RPT:
        case FASTB_RPT:
        case TARGET_XOFFSET:
        case TARGET_YOFFSET:
        case BACKGROUND_ZOOM:
        case RC_REPEATS:
        case PEN_START_DEPTH:
        case COVARIATE:
        case BLANK_P:
        case UNCORR_P:
        case ONETARGET_P:
        case FAKESTIM_EXPT:
        case FAKESTIM_SIGNAL:
        case TONETIME:
        case SEEDRANGE:
        case INTERTRIAL_MIN:
            expt.vals[flag] = val;
            break;
        case NIMPLACES:
            expt.st->nimplaces = val;
            break;
        case HIGHXTYPE:
            expt.hightype = val;
            break;
        case EXPT1_MAXSIG:
            expt.vals[flag] = val;
            setstimuli(1);
            break;
        case PURSUIT_INCREMENT:
            expt.vals[flag] = val;
            break;
        case INITIAL_MOVEMENT:
            expt.vals[flag] = val;
            dx = sin(expt.vals[FP_MOVE_DIR]) * val * expt.vals[CHANGE_SEED];
            dy = cos(expt.vals[FP_MOVE_DIR]) * val * expt.vals[CHANGE_SEED];
            if (fabs(val) > 0){
            expt.st->pos.xy[0] = deg2pix(expt.vals[XPOS] - dx);
            expt.st->pos.xy[1] = deg2pix(expt.vals[YPOS] - dy);
            }
            break;
        case REWARD_SIZE1:
        case REWARD_SIZE2:
        case REWARD_SIZE3:
            if(val < 1) //Ali  || confirm_no("Sure you want a big reward (>1)?", NULL))
                expt.vals[flag] = val;
            break;
        case DISTRIBUTION_WIDTH:
        case RC1INC:
        case DISTRIBUTION_CONC:
        case DISTRIBUTION_MEAN:
            expt.vals[flag] = val;
            SetDotDistribution();
            break;
        case TEMPORAL_ORI_BANDWIDTH:
            expt.vals[flag] = val;
            /*
             * use Dw to send this val to Bw, but don't reset Dw when reading
             * Tw from a setup file
             */
            if(val > 0 || expt.type2 == flag)
                expt.vals[DISTRIBUTION_WIDTH] = val;
            break;
        case ALTERNATE_STIM_MODE:
            expt.vals[flag] = val;
            altstimmode = (int)val;
            break;
        case PREWARD:
            expt.vals[flag] = val;
            break;
        case SEED_DELAY:
            expt.vals[flag] = val;
            expt.st->left->calculated = expt.st->right->calculated = 0;
            break;
        case DELAY:
            expt.vals[flag] = rint(val);
            break;
        case PENNUMCOUNTER:
            expt.newpen = (int)val;
            break;
        case GRIDSIZE:
            expt.vals[flag] = val;
            if(val > 0.1)
                printf("Grid is %.1f pixels\n",deg2pix(expt.vals[GRIDSIZE]));
            if(val > 10){
                ReadGridVals();
            }
            break;
        case FIXATION_WINDOW:
            expt.fw = val;
            expt.vals[FIXWIN_HEIGHT] = val;
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
        case REWARD_SIZE:
            if(val < 1 ) //Ali || !confirm_no("Sure you want a big reward (>1)?",NULL))
                expt.st->fix.rwsize = val;
            break;
        case WURTZ_RT_CODE:
            expt.st->fix.rt = val;
            break;
        case BRIGHTSHIFT_CODE:
            expt.st->fix.stimcolor = expt.st->fix.fixcolor +val;
            break;
        case EXPT_BLKSIZ:
            expt.blksize = (int)val;
            break;
        case EXPT_STIMPERTRIAL:
            expt.stimpertrial = (int)val;
            break;
        case LRBINTERLEAVE: 
        case MONOCULARITY_EXPT: 
            /* 0 = Binoc, -1 = LEFT, 1 = RIGHT */
            optionflag &= (~MONOCULAR_MODE);
            // only change fixation criteria if the fixation point is monocular
            if(val < -0.4)
                optionflag |= LEFT_FIXATION_CHECK;
            else if (val > 0.4)
                optionflag |= RIGHT_FIXATION_CHECK;
            ResetFixWin();
            setoption();
            clear_display(1);
            SerialSend(OPTION_CODE);
            break;
        case NPLANES:
            st->flag &= (~(FRONT_ONLY | BACK_ONLY));
            if(val > 0.9 && val < 1.1)
                st->flag |= FRONT_ONLY;
            else if(val > -0.1 && val < 0.1)
                st->flag |= BACK_ONLY;
            break;
        case SPIKE_GAIN:
            expt.spikegain = val;
            break;
        case TRIGGER_LEVEL:
            exp->tl = val;
            expt.vals[TRIGGER_LEVEL1] = val;
            expt.vals[TRIGGER_LEVEL2] = val;
            expt.vals[TRIGGER_LEVEL3] = val;
            expt.vals[TRIGGER_LEVEL4] = val;
            break;
        case ISI_CODE:
            exp->isi = val;
            exp->isiframes = val * mon.framerate;
            break;
        case POSTPERIOD_CODE:
            if(val > 0.9)
                exp->postperiod = val * 1/mon.framerate; 
            else
                exp->postperiod = val;
            break;
        case PREPERIOD_CODE:
            if(val > 0.9)
                exp->preperiod = val * 1/mon.framerate; 
            else
                exp->preperiod = val;
            break;
        case EXPT3_NSTIM:
        case EXPT2_NSTIM:
        case NTRIALS_CODE:
            if(expt.plot == NULL){
                expt.plot = &expplots[0];
            }
            
            if((flag == NTRIALS_CODE && (int)val != exp->nstim[0]) ||
               (flag == EXPT2_NSTIM && (int)val != exp->nstim[1]) ||
               optionflags[INTERLEAVE_SINE] || optionflags[INTERLEAVE_ZERO_ALL]
               || optionflags[INTERLEAVE_ONE_SINE])
                new = 1;
            else
                new = 0;
            if(flag == NTRIALS_CODE){
                exp->nstim[0] = (int)val;
                mode &= (~CUSTOM_EXPVAL);
            }
            if(flag == EXPT3_NSTIM){
                if((int)val != exp->nstim[4])
                    new = 1;
                exp->nstim[4] = (val < 1) ? 1 : (int)val;
                optionflags[CUSTOM_EXPVALC] = 0;
            }
            if(flag == EXPT2_NSTIM){
                exp->nstim[1] = (int)val;
                if(optionflags[INTERLEAVE_SINE])
                    exp->nstim[1]++;
                optionflags[CUSTOM_EXPVALB] = 0;
            }
            
            
            if(new)
            {

                psychclear(expt.plot, 0);
            }
            setstimuli(1);
            ListExpStims(NULL);
            break;
        case CLAMP_DISPARITY_CODE:
            exp->clamp = exp->cclamp = val;
            break;
        case RAMP_AMPLITUDE_CODE:
            exp->ramp = val;
            break;
        case RAMP_VERGENCE_CODE:
        case RAMP_HOLD_CODE:
            exp->ramp = exp->cramp = val;
            break;
        case NFRAMES_CODE:
            exp->st->nframes = (int)(val);
            break;
        case STIMULUS_DURATION_CODE:
            exp->st->nframes = rint((val *mon.framerate));
            break;
        case NREPS_CODE:
            /* 
             * If experiment is running, and the number of repetitions is reduced, 
             * no need to recalcuate order. If number is increased beyond the number where
             * the sequence was calculated at the start, recacluating the sequence 
             * would disrupt the existin one, so don't permit it.
             * However, if expt is just ending, then we can change this ready for the 
             * next (i.e. if we a reading continuaion lines from an exp file)
             */
            if(exp->st->mode & EXPTPENDING){
                if(val <= repetitions_set || exp->st->mode & EXPT_OVER){
                    exp->nreps = (int)val;
                    RecalcRepeats(exp);
                }
                //Ali	    else
                //Ali	      numeric_set_value(nrepsw,exp->nreps);
            }
            else
            {
                //don't allow increase during expt - it would mess up order
                exp->nreps = (int)val;
                setstimulusorder(0);
            }
            ShowTrialsNeeded();
            /*
             * Why is setstimuli called here? nreps doesn't change tha - should only
             * reset the order. 
             setstimuli(0);
             *  changed BGC Feb 02 2001
             */
            break;
        case WURTZ_DURATION_CODE:
            /*	  if(optionflag & WURTZ_WHOLE_STIMULI)
             fix->fixstims = (int)val;
             else*/
            fix->fixframes  = (int)val;
            break;
        case EXPTYPE_CODE:
            setexp(NULL,0,(int)val);
            if(mode & RUNNING)
                redraw_overlay(expt.plot);
            break;
        case EXPTYPE_CODE2:
            setsecondexp(NULL,0,(int)val);
            if(mode & RUNNING)
                redraw_overlay(expt.plot);
            break;
        case EXPTYPE_CODE3:
            expt.type3 = val;
            if(mode & RUNNING)
                redraw_overlay(expt.plot);
            CheckExpts();
            break;
        case EXPT3_MEAN:
        case EXPT2_MEAN:
        case EXPT_MEAN:
            if(flag == EXPT_MEAN)
            {
                old = exp->mean;
                exp->mean = val;
            }
            else if(flag == EXPT2_MEAN)
            {
                old = exp->mean2;
                exp->mean2 = val;
                optionflags[CUSTOM_EXPVALB] = 0;
            }
            else if(flag == EXPT3_MEAN)
            {
                old = exp->mean3;
                exp->mean3 = val;
            }
            if(old != val)
                psychclear(expt.plot,1);
            setstimuli(1);
            ListExpStims(NULL);
            SetTargets();
            break;
        case UNIT_BINWIDTH:
            if(val > 10)
                exp->binw = val;
            else if(val > 0)
                exp->binw = val * 1000/mon.framerate;
            else
                exp->binw = 100;
            break;
        case EXPT_INCR:
            old = exp->incr;
            if(optionflags[REVERSE_CORRELATE])
                expt.incr = (val * expt.st->framerepeat)/expt.st->nframes;
            else
                expt.incr = val;
        case EXPT2_INCR:
        case EXPT3_INCR:
            if(flag == EXPT2_INCR)
            {
                old = exp->incr2;
                exp->incr2 = val;
                optionflags[CUSTOM_EXPVALB] = 0;
            }
            if(flag == EXPT3_INCR)
                exp->incr3 = val;
            if(val != old)
            {
                if(option2flag & STAIRCASE)
                    reset_afc_counters();
                psychclear(expt.plot,1);
            }
            setstimuli(1);
            ListExpStims(NULL);
            break;
        case TIMEOUT_CODE:
            if(val > 500) // must be ms, intended for brainwave
                exp->st->fix.timeout = val/1000;
            else
                exp->st->fix.timeout = val;
            break;
        case SACCADE_AMPLITUDE:
            if(val == 0){
                afc_s.abssac[0] = 0;
                afc_s.abssac[1] = 0;
                afc_s.sacval[0] = 0;
                afc_s.sacval[1] = 0;
                SerialSend(VSACCADE_VALUE);
                SerialSend(HSACCADE_VALUE);
                SerialSend(TARGET2_POS);
                option2flag &= (~AFC);
            }
            else if(afc_s.abssac[0] > afc_s.abssac[1]){
                afc_s.abssac[0] = val;
                afc_s.abssac[1] = 0;
            }
            else{
                afc_s.abssac[0] = 0;
                afc_s.abssac[1] = val;
            }
            expt.vals[flag] = val;
            SetTargets();
            break;
        case VSACCADE_VALUE:
        case HSACCADE_VALUE:
            i = (flag == HSACCADE_VALUE) ? 0 : 1;
            afc_s.sacval[i] = val;
            afc_s.abssac[i] = val;
            if(val > 0)
                expt.vals[SACCADE_AMPLITUDE] = val;
            break;
        case BONUS_CRITERION:
            afc_s.bonuslevel = (int)val;
            break;
        case BONUS2_CRITERION:
            afc_s.bonuslevel2 = (int)val;
            break;
        case SACTARGSIZE:
            afc_s.targsize = deg2pix(val);
            break;
        case SAC_FIX_CRIT:
            afc_s.sac_fix_crit = val;
            break;
        case WRONG_TIMEOUT_CODE:
            afc_s.wrongtimeout = val;
            break;
        case STARTVAL:
            afc_s.startvalue = val;
            break;
        case MAXSTAIRVAL:
            afc_s.maxstairvalue = val;
            break;
        case MINSTAIRVAL:
            afc_s.minstairvalue = val;
            break;
        case CORRECTION_LEAVE_CRIT:
            afc_s.correction_leave = (int)val;
            break;
        case CORRECTION_ENTRY_CRIT:
            afc_s.correction_entry = (int)val;
            break;
        case AFC_PROPORTION:
            afc_s.proportion = val;
            break;
        case COVARY_XPOS:
            afc_s.ccvar = val;
            break;
        case SHORT_PREM_CODE:
            exp->st->fix.minprem = val;
            break;
        case XPIXEL_CODE:
            mon.pix2cms[0] = val;
            mon.cms2pix[0] = 1/mon.pix2cms[0];
            break;
        case YPIXEL_CODE:
            mon.pix2cms[1] = val;
            mon.cms2pix[1] = 1/mon.pix2cms[1];
            break;
        case VIEWD_CODE:
            mon.viewd = val;
            break;
        case TRAPEZOIDAL_SCALING:
            if(val > 0.1){
                mon.trapscale[0] = val;
                mon.trapscale[1] = 1/val;
            }
            else{
                mon.trapscale[0] = 0;
                mon.trapscale[1] = 0;
            }
            break;
        case SIDE_OFFSET:
            theta = expt.st->pos.angle;
            if(expt.vals[RF_SIGN] < 0)
                xymove = -val;
            else
                xymove = val;
            x = pix2deg(expt.rf->pos[0]) - sin(theta) * xymove;
            y = pix2deg(expt.rf->pos[1]) + cos(theta) * xymove;
            SetStimulus(st, x, SETZXOFF, NOEVENT);
            SetStimulus(st, y, SETZYOFF, NOEVENT);
            break;
        case END_LENGTH:
            theta = expt.st->pos.angle;
            if(expt.vals[RF_SIGN] < 0)
                xymove = -val/2;
            else
                xymove = val/2;
            x = pix2deg(expt.rf->pos[0]) + cos(theta) * xymove;
            y = pix2deg(expt.rf->pos[1]) + sin(theta) * xymove;
            SetStimulus(st, val, STIM_HEIGHT, NOEVENT);
            SetStimulus(st, x, SETZXOFF, NOEVENT);
            SetStimulus(st, y, SETZYOFF, NOEVENT);
            break;
        case END_OFFSET:
            theta = expt.st->pos.angle;
            if(expt.vals[RF_SIGN] < 0){
                x = pix2deg(expt.rf->pos[0]) + cos(theta) * -val;
                y = pix2deg(expt.rf->pos[1]) + sin(theta) * -val;
            }
            else{
                x = pix2deg(expt.rf->pos[0]) + cos(theta) * val;
                y = pix2deg(expt.rf->pos[1]) + sin(theta) * val;
            }
            SetStimulus(st, x, SETZXOFF, NOEVENT);
            SetStimulus(st, y, SETZYOFF, NOEVENT);
            break;
        default:
            return(SetStimulus(st, val, flag, evtype));
    }
    
    
    /*
     * Only get here if this is not set by SetStimulus.
     * This switch is used to send stuff to Brainwave that needs to go
     * To brainwave.
     */
    
    switch(flag)
    {

        case STIMULUS_MODE:
            SerialSend(flag);
            SerialSend(STIMULUS_FLAG);
        case MONOCULARITY_EXPT:
            SerialSend(OPTION_CODE);
            break;
        case END_OFFSET:
        case SIDE_OFFSET:
            SerialSend(SETZXOFF);
            SerialSend(SETZYOFF);
            break;
        case NFRAMES_CODE:
            SerialSend(STIMULUS_DURATION_CODE);
            break;
        case TEMPORAL_ORI_BANDWIDTH:
            SerialSend(DISTRIBUTION_WIDTH);
            break;
        case SHORT_PREM_CODE:
        case TRIGGER_LEVEL:
        case HSACCADE_VALUE:
        case VSACCADE_VALUE:
        case PREPERIOD_CODE:
        case POSTPERIOD_CODE:
        case CLAMP_DISPARITY_CODE:
        case RAMP_AMPLITUDE_CODE:
        case STIMULUS_DURATION_CODE:
        case SAC_FIX_CRIT:
        case FIXATION_SURROUND:
        case FIXATION_WINDOW:
        case STATIC_CONJUGATE:
        case VERGENCE_WINDOW:
        case STATIC_VERGENCE:
        case ELECTRODE_DEPTH:
        case SPIKE_GAIN:
        case REWARD_SIZE:
        case PREWARD:
        case WURTZ_RT_CODE:
        case EARLY_RWTIME:
        case SEED_DELAY:
        case ORI_BANDWIDTH:
        case DISTRIBUTION_WIDTH:
        case DISTRIBUTION_CONC:
        case DISTRIBUTION_MEAN:
        case FIXWIN_HEIGHT:
        case SACCADE_DETECTED:
        case SACCADE_THRESHOLD:
        case SACCADE_PUNISH_DIRECTION:
        case INITIAL_DISPARITY:
        case INITIAL_MOVEMENT:
        case INITIAL_APPLY_MAX:
            
        case UNIT_BINWIDTH:
        case FP_MOVE_DIR:
        case FP_MOVE_SIZE:
        case FAST_SEQUENCE_RPT:
        case BACK_PPOS:
        case BACK_OPOS:
        case REWARD_BIAS:
        case TONETIME:
        case TARGET_RATIO:
            SerialSend(flag);
            break;
        case PURSUIT_INCREMENT:
            if (lastdir != expt.vals[flag] * pursuedir && pursuedir < 0)
                cbuf[0] = 0;
            SerialSend(flag);
            break;
        default: //Even if not sent to bw, put it in the disk record.
            if(seroutfile)
            {
                cbuf[0] = 0;
                if((i = MakeString(flag, cbuf, &expt, expt.st,0)) >= 0){
                    strcat(cbuf,"*\n\0");
                    fputs(cbuf,seroutfile);
                }
            }
            break;
    }
    return(0);
    
}

float GetProperty(Expt *exp, Stimulus *st, int code)
{
    float val;
    
    if(st == NULL)
        return(NOTSET);
    
    if((val = ExptProperty(exp, code)) == NOTSET)
        val = StimulusProperty(st, code);
    if (val == NOTSET && code < expt.maxcode && valstringindex[code] > 0){
        val = expt.vals[code];
    }
    return(val);
}

float ExptProperty(Expt *exp, int flag)
{
    float val=0;
    Position x;
    
    switch(flag)
    {
        case IMAGEJUMPS:
            val = expt.st->jumps;
            break;
        case REWARD_BIAS:
            val = expt.biasedreward;
            break;
        case VERBOSE_CODE:
            val = expt.verbose;
            break;
        case BACK_OPOS:
            if(expt.st->next)
                val = StimulusProperty(expt.st->next,ORTHOG_POS);
            break;
        case BACK_PPOS:
            if(expt.st->next)
                val = StimulusProperty(expt.st->next,PARA_POS);
            break;
        case TWOCYL_DISP:	
            val = StimulusProperty(expt.st,DISP_X);
            break;
        case SETANNULUS:
            if(optionflags[PAINT_BACKGROUND] && optionflags[PAINT_THIRD_LAST]){
                val = expt.st->next->type;
            }
            else if(optionflags[PAINT_BACKGROUND])
                val = STIM_ANNULUS;
            else
                val = STIM_NONE;
            break;
        case PANEL_XPOS:
        case PANEL_YPOS:
            //Ali      if(allframe){
            if(1){
                if(x > 1<<16)
                    x = (x>16);
                if(x < 2560)
                    val = (float)x;
                else
                    val = 0;
            }
            else
                val = 0;
            break;
        case STIMID:
            val = expt.allstimid;
            break;
        case STIMULUS_MODE:
            val = expt.stimmode;
            break;
        case NTRIALS_CODE:
            val = exp->nstim[0];
            break;
        case EXPT3_NSTIM:
            val = exp->nstim[4];
            break;
        case EXPT2_NSTIM:
            val = exp->nstim[1];
            break;
        case FRAMERATE_CODE:
            val = mon.framerate;
            break;
        case RCFILE_CTR:
            val = (float)rcctr;
            break;
        case WEIGHT_TEXT:
            val = thelog.weight;
            break;
        case RESTRAIN_TEXT:
            if(thelog.restrain)
                sscanf(thelog.restrain,"%f",&val);
            break;
        case STIMULUS_DURATION_CODE:
            val = (float)(expt.st->nframes)/expt.mon->framerate;
            break;
        case FP_MOVE_SIZE:
            val = expt.vals[flag];
            break;
        case FP_MOVE_DIR:
            val = rad2deg(expt.vals[flag]);
            break;
        case FP_MOVE_FRAME:
        case CHANGE_SEED:
        case FIX_LINW:
        case PREWARD:
        case FIXATION_OVERLAP:
        case FIXATION_SURROUND:
        case BACKJUMP:
        case SETCLEARCOLOR:
        case RANDOM_X:
        case RANDOM_ORI:
        case SEED_DELAY:
        case DELAY:
        case TRIGGER_LEVEL1:
        case TRIGGER_LEVEL2:
        case TRIGGER_LEVEL3:
        case TRIGGER_LEVEL4:
        case ELECTRODE_DEPTH:
        case PLOTSMOOTH:
        case RC_DELAY:
        case GRIDSIZE:
        case TARGET_RATIO:
        case HIGHSF:
        case HIGHTF:
        case HIGHX:
        case EARLY_RWTIME:
        case STEPPER_PLOTLEN:
        case REWARD_SIZE1:
        case REWARD_SIZE2:
        case REWARD_SIZE3:
        case TIMEOUT_CONTRAST:
        case ALTERNATE_STIM_MODE:
        case TRIAL_START_BLANK:
        case TEST_VALUE1:
        case TEST_VALUE2:
        case BACK_ADD_SIZE:
        case PENXPOS:
        case PENYPOS:
        case FIXPT_DISPARITY:
        case IFCSCALE:
        case FIXPT_EYE:
        case DISTRIBUTION_WIDTH:
        case DISTRIBUTION_CONC:
        case DISTRIBUTION_MEAN:
        case RC1INC:
        case TEMPORAL_ORI_BANDWIDTH:
        case AUTO_ZERO:
        case SACCADE_DETECTED:
        case SACCADE_THRESHOLD:
        case SACCADE_PUNISH_DIRECTION:
        case FIXWIN_HEIGHT:
        case INITIAL_DISPARITY:
        case INITIAL_MOVEMENT:
        case INITIAL_APPLY_MAX:
        case VERTICAL_VERGENCE:
        case PLC_MAG:
        case PULSE_WIDTH:
        case STIM_PULSES:
        case SHAKE_TIMEOUT_DURATION:
        case FAST_SEQUENCE_RPT:
        case FASTB_RPT:
        case TARGET_XOFFSET:
        case TARGET_YOFFSET:
        case BACKGROUND_MOVIE:
        case BACKGROUND_IMAGE:
        case BACKGROUND_ZOOM:
        case RC_REPEATS:
        case PEN_START_DEPTH:
        case COVARIATE:
        case BLANK_P:
        case UNCORR_P:
        case INITIAL_DURATION:
        case EXPT1_MAXSIG:
        case ONETARGET_P:
        case FAKESTIM_EXPT:
        case FAKESTIM_SIGNAL:
        case TONETIME:
        case SEEDRANGE:
        case INTERTRIAL_MIN:
            val = expt.vals[flag];
            break;	
        case NIMPLACES:
            val = expt.st->nimplaces;
            break;	
        case HIGHXTYPE:
            val = expt.hightype;
            break;
        case PURSUIT_INCREMENT:
            val = expt.vals[flag];
            break;	
        case PENNUMCOUNTER:
            val = expt.ipen;
            break;
        case RC_SEED:
            val = expt.st->rcseed;
            break;
        case EXPT_LINW:
            val = (float)(expt.linw);
            break;
        case WURTZ_RT_CODE:
            val = expt.st->fix.rt;
            break;
        case EXPT_BLKSIZ:
            val = (float)(expt.blksize);
            break;
        case EXPT_STIMPERTRIAL:
            val = (float)(expt.stimpertrial);
            break;
        case NPLANES:
            if(exp->st->flag & FRONT_ONLY)
                val = 1;
            else if(exp->st->flag & BACK_ONLY)
                val = 0;
            else 
                val = 2;
            break;
        case MONOCULARITY_EXPT:
            if(optionflag & LEFT_FIXATION_CHECK)
                val = -1;
            else if(optionflag & RIGHT_FIXATION_CHECK)
                val = 1;
            else
                val = 0;
            break;
        case PLOTW:
            val = expt.plot->size[0];
            break;
        case PLOTH:
            val = expt.plot->size[1];
            break;
        case PLOTX:
            val = expt.plot->pos[0];
            break;
        case PLOTY:
            val = expt.plot->pos[1];
            break;
        case SPIKE_GAIN:
            val = expt.spikegain;
            break;
        case NFRAMES_CODE:
            val = (float)exp->st->nframes;
            break;
        case ISI_CODE:
            val = (float) exp->isi;
            break;
        case POSTPERIOD_CODE:
            val = (float) exp->postperiod;
            break;
        case PREPERIOD_CODE:
            val = (float) exp->preperiod;
            break;
        case CLAMP_DISPARITY_CODE:
            val = (float) exp->clamp;
            break;
        case RAMP_HOLD_CODE:
        case RAMP_VERGENCE_CODE:
        case RAMP_AMPLITUDE_CODE:
            val = (float) exp->ramp;
            break;
        case MODE_CODE:
            val = exp->st->mode;
            break;
        case EXPTYPE_CODE:
            val = exp->mode;
            break;
        case EXPTYPE_CODE2:
            val = exp->type2;
            break;
        case EXPTYPE_CODE3:
            val = exp->type3;
            break;
        case OPTION_CODE:
            val = optionflag;
            break;
        case STATIC_VERGENCE:
            val = exp->sv;
            break;
        case STATIC_CONJUGATE:
            val = exp->cj;
            break;
        case FIXATION_WINDOW:
            val = exp->fw;
            break;
        case VERGENCE_WINDOW:
            val = exp->vw;
            break;
        case TRIGGER_LEVEL:
            val = exp->tl;
            break;
        case EXPT_MEAN:
            val = exp->mean;
            break;
        case EXPT3_MEAN:
            val = exp->mean3;
            break;
        case EXPT2_MEAN:
            val = exp->mean2;
            break;
        case UNIT_BINWIDTH:
            val = expt.binw;
            break;
        case EXPT2_INCR:
            val = exp->incr2;
            break;	
        case EXPT3_INCR:
            val = exp->incr3;
            break;	
        case EXPT_INCR:
            if(optionflags[REVERSE_CORRELATE])
                val = (expt.incr * expt.st->nframes)/expt.st->framerepeat;
            else
                val = exp->incr;
            break;	
        case XPIXEL_CODE:
            val = mon.pix2cms[0];
            mon.cms2pix[0] = 1/mon.pix2cms[0];
            break;
        case YPIXEL_CODE:
            val = mon.pix2cms[1];
            mon.cms2pix[1] = 1/mon.pix2cms[1];
            break;
        case VIEWD_CODE:
            val = mon.viewd;
            break;
        case TRAPEZOIDAL_SCALING:
            val = mon.trapscale[0];
            break;
        case SHORT_PREM_CODE:
            val = exp->st->fix.minprem;
            break;
        case COVARY_XPOS:
            val = afc_s.ccvar;
            break;
        case BONUS_CRITERION:
            val = afc_s.bonuslevel;
            break;
        case BONUS2_CRITERION:
            val = afc_s.bonuslevel2;
            break;
        case NREPS_CODE:
            val = exp->nreps;
            break;
        default:  //either doesn't exist, or need to go to StimulusProperty()
            val = NOTSET;
            break;
    }
    return(val);
}

void setextras()
{
    expt.nstim[2] = 0;
    
    /*
     * order:   blank, zzero, uncorr, L, R
     */
    if(option2flag & INTERLEAVE_BLANK){
        if(expt.type2 == MONOCULARITY_EXPT && expt.mean2 == 0 && !optionflags[GREY_MONOC]){
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_LEFT | STIMULUS_EXTRA_BLANK;
            extralabels[expt.nstim[2]] = "L Blank";
            extravals[expt.nstim[2]] = INTERLEAVE_EXPT_LEFT;
            expt.nstim[2]++;
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_RIGHT | STIMULUS_EXTRA_BLANK;
            extralabels[expt.nstim[2]] = "R Blank";
            extravals[expt.nstim[2]] = INTERLEAVE_EXPT_RIGHT;
        }
        else{
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_BLANK;
            extralabels[expt.nstim[2]] = "blank";
            extravals[expt.nstim[2]] = INTERLEAVE_EXPT_BLANK;
        }
        expt.nstim[2]++;
    }
    if(optionflags[INTERLEAVE_ZERO]){
        if(expt.st->type == STIM_CYLINDER || (expt.st->type == STIM_RDS && expt.mode != TF && expt.mode != CORRELATION)){
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_ZEROCOH;
            extralabels[expt.nstim[2]] = "ZeroCoh";
        }
        else if(expt.st->type == STIM_RADIAL && expt.mode == ORI2){
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_ZERO;
            extralabels[expt.nstim[2]] = "No Change";
        }
        else{
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_ZERO;
            if(expt.st->type == STIM_SQCORRUG && (expt.mode == ORIENTATION && expt.type2 == START_PHASE) || (expt.mode == START_PHASE && expt.type2 == ORIENTATION))
                extralabels[expt.nstim[2]] = "Phase 90";
            else if(expt.st->type == STIM_CORRUG)
                extralabels[expt.nstim[2]] = "0 Mod";
            else
                extralabels[expt.nstim[2]] = "Zero";
        }
        extravals[expt.nstim[2]] = INTERLEAVE_EXPT_ZERO;
        expt.nstim[2]++;
    }
    if(optionflags[INTERLEAVE_FLIP]){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_FLIP;
        extralabels[expt.nstim[2]] = "Flip";
        expt.nstim[2]++;
        if(expt.nstim[1] > 1){
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_FLIP;
            extralabels[expt.nstim[2]] = "Flip";
            expt.nstim[2]++;
        }
        frameparams[1] = DISP_X;
    }
    if(option2flag & INTERLEAVE_UNCORRELATED){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_UNCORRELATE;
        extralabels[expt.nstim[2]] = "Uncorr";
        extravals[expt.nstim[2]] = INTERLEAVE_EXPT_UNCORR;
        expt.nstim[2]++;
    }
    if(optionflags[INTERLEAVE_HIGHTF]){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_HIGHTF;
        if(optionflags[INTERLEAVE_HIGHSF]){
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_HIGHTF | STIMULUS_EXTRA_HIGHSF;
            extralabels[expt.nstim[2]] = "HighTFSF";
        }
        extralabels[expt.nstim[2]] = "HighTF";
        extravals[expt.nstim[2]] = INTERLEAVE_EXPT_HIGH;
        expt.nstim[2]++;
    }
    else if(optionflags[INTERLEAVE_HIGH]){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_HIGH;
        if(optionflags[INTERLEAVE_HIGHSF]){
            extratypes[expt.nstim[2]] = STIMULUS_EXTRA_HIGH | STIMULUS_EXTRA_HIGHSF;
            extralabels[expt.nstim[2]] = "High+SF";
        }
        if(expt.st->type == STIM_SQCORRUG && (expt.mode == ORIENTATION && expt.type2 == START_PHASE) || (expt.mode == START_PHASE && expt.type2 == ORIENTATION))
            extralabels[expt.nstim[2]] = "Phase 270";
        else
            extralabels[expt.nstim[2]] = "HighX";
        extravals[expt.nstim[2]] = INTERLEAVE_EXPT_HIGH;
        expt.nstim[2]++;
    }
    else if(optionflags[INTERLEAVE_HIGHSF]){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_HIGHSF;
        extralabels[expt.nstim[2]] = "HighSF";
        extravals[expt.nstim[2]] = INTERLEAVE_EXPT_HIGH;
        expt.nstim[2]++;
    }
    else if(optionflags[INTERLEAVE_HIGHSQ]){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_HIGHSQ;
        extralabels[expt.nstim[2]] = "HighSq";
        expt.nstim[2]++;
    }
    if(option2flag & INTERLEAVE_ONE_MONOCULAR){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_LEFT;
        extralabels[expt.nstim[2]] = "Left";
        extravals[expt.nstim[2]] = INTERLEAVE_EXPT_LEFT;
        expt.nstim[2]++;
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_RIGHT;
        extralabels[expt.nstim[2]] = "Right";
        extravals[expt.nstim[2]] = INTERLEAVE_EXPT_RIGHT;
        expt.nstim[2]++;
    }
    if(option2flag & INTERLEAVE_RANDOM){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_RANDOM;
        extralabels[expt.nstim[2]] = "Rand";
        expt.nstim[2]++;
    }
    if(optionflags[ADD_FPJUMP]){
        extratypes[expt.nstim[2]] = STIMULUS_EXTRA_FPJUMP;
        extralabels[expt.nstim[2]] = "FPjump";
        expt.nstim[2]++;
    }
    expt.fastextras = expt.nstim[2];
    if(optionflags[FAST_SEQUENCE]){ // No nominal extras
        expt.nstim[2] = 0;
    }
}

/*
 * CheckExpts is called after changes to expt type or stimuli, to make sure
 * that special parameters that depend on multiple expt types are kept up to
 * date.
 */
void CheckExpts()
{
    int nvals;
    
    if(expt.type2 == TEMPORAL_ORI_BANDWIDTH)
        expt.vals[RC1INC] = 10;
    if(expt.type2 == DISTRIBUTION_WIDTH){
        nvals = expt.mean2 + expt.incr2 * (float)(expt.nstim[1]-1)/2;
        if(nvals > 1)
            expt.vals[RC1INC] = expt.incr/(nvals-1);
        else
            expt.vals[RC1INC] = expt.incr;
    }
    switch(expt.type2){
        case TEMPORAL_ORI_BANDWIDTH:
        case DISTRIBUTION_WIDTH:
        case DISTRIBUTION_CONC:
            SerialSend(RC1INC);
            break;
    }
}

GLubyte *GetStimImage(int x, int y, int w, int h, char eye)
{
    static GLubyte *pix = NULL;
    int i,len,j,size;
    
    glReadBuffer(GL_FRONT);
    /*
     * just alloc once, and make it big enough for any image;
     */
    
    if(pix == NULL){
        size = expt.winsiz[0] * expt.winsiz[1] * 4 * sizeof(GLubyte);
        pix = (GLubyte *)malloc(size);
    }
    if(pix != NULL){
        if(eye == 'L')
            glReadPixels(x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, pix);
        if(eye == 'R')
            glReadPixels(x, y, w, h, GL_BLUE, GL_UNSIGNED_BYTE, pix);
    }
    return(pix);
}

/*
 * SaveImage saves the current stimulus image. 
 * if the first bit of type is set, a pgm file is created
 * if the second bit of type is set, a file listing dot locations and
 * disparities is written.
 */
int SaveImage(Stimulus *st, int type)
{
    GLubyte *pix;
    FILE *ofd;
    char imname[BUFSIZ];
    int x,y,w,h,i,done = 0,n = 0;
    static int imstimid = 0;
    char eyec[3] = "LR";
    Stimulus *rst = st;
    
    if(rdspair(st))
        rst = st->next;
    w = rst->rect.rx - rst->rect.lx;
    h = rst->rect.ry - rst->rect.ly;
    x = expt.winsiz[0] + rst->rect.lx;
    y = expt.winsiz[1] + rst->rect.ly;
    
    
    /*
     * make sure w,h are multiples of 4 to avoid word-boundary problems
     */
    w = w + 4-(w%4);
    h = h + 4-(h%4);
    
    if(type & 1){  
        for(i = 0; i < 2; i++){
            sprintf(imname,"%s/%sim%d%c.pgm",ImageOutDir,expname,imstimid,eyec[i]);
            if((pix = GetStimImage(x, y, h, w,eyec[i])) != NULL){
                if((ofd = fopen(imname,"w")) == NULL)
                    fprintf(stderr,"Can't write image to %s\n",imname);
                else{
                    fprintf(ofd,"P5 %d %d 255\n",h,w);
                    fwrite(pix, sizeof(GLubyte), w*h, ofd);
                    done++;
                    fclose(ofd);
                    fprintf(stderr,"Seed %d,%d written to %s (dx%.3f)\n",st->left->baseseed,st->left->seed,imname,st->disp);
                }
            }
        }
        n = st->left->ndots;
    }
    if(type & (1<<1)){
        if(imoutfd == NULL){
            sprintf(imname,"%s/%s.im.txt",ImageOutDir,expname);
            imoutfd = fopen(imname,"w");
        }
        if(st->type == STIM_RDS || st->type == STIM_RLS || st->type == STIM_CHECKER|| st->type == STIM_GRATINGN){
            
            if(st->type == STIM_RDS)
                n = SaveRds(st, imoutfd);
            else if(st->type == STIM_GRATINGN)
                n = SaveNSines(st, imoutfd);
            else
                n = SaveRls(st, imoutfd);
            fflush(imoutfd);
            done++;
        }
    }
    
    if(done){
        if(imidxfd){
            fprintf(imidxfd,"%d %d %d %d %d %d %d %d %d\n",expt.allstimid,imstimid,x,y,h,w,st->left->baseseed,n,seedoffset);
            fflush(imidxfd);
        }
        imstimid++;
    }
    
    return(0);
}


int ReadCommand(char *s)
{
    int retval = 0, line, start, stop,i,ival;
    char *r,buf[BUFSIZ],command_result[BUFSIZ],c;
    char imname[BUFSIZ];
    float val;
    
    sprintf(command_result,"");
    if(!strncasecmp(s,"quit",4))
        quit_binoc();
    else if(!strncasecmp(s,"getrow",4)){
        sscanf(s,"%*s %d %d %d",&line,&start,&stop);
    }
    else if(!strncasecmp(s,"step",4)){
        sprintf(command_result,"step to %d",step_stimulus());
    }
    else if(!strncasecmp(s,"lskip",4)){
        i = sscanf(s,"%*s %d %d",&skiplines,&stop);
        if(i > 1){
            stopline = skiplines+stop;
            sprintf(command_result,"Skip to line %d",skiplines);
        }
    }
    else if(!strncasecmp(s,"print",4) && (r = strchr(s,' '))){
        printf("%s",++r);
    }
    else if(!strncasecmp(s,"go",2)){
        StopGo(GO);
    }
    else if(!strncasecmp(s,"openuff",7)){
        SerialSend(UFF_PREFIX);
    }
    else if(!strncasecmp(s,"reopenserial",10)){
        ReopenSerial();
        SendAll();
    }
    else if(!strncasecmp(s,"savefile=",9)){
        SaveExptFile(&s[9],0);
    }
    else if(!strncasecmp(s,"stop",2)){
        StopGo(STOP);
    }
    else if(!strncasecmp(s,"estop",5)){
        expt_over(0);
    }
    else if(!strncasecmp(s,"ecancel",7)){
        if(expt.st->mode & EXPTPENDING) // if verg sends cancel, but not in expt, ignore
            expt_over(CANCEL_EXPT);
        else {
            notify("\nEXPTOVER\n");
        }
    }
    else if(!strncasecmp(s,"expt",4)){
        runexpt(NULL,NULL,NULL);
    }
    else if(!strncasecmp(s,"clear",4)){
        //expbuttons(NULL, (XtPointer)(CLEAR_EXPT), NULL);
    }
    else if(!strncasecmp(s,"panel",2)){
        //Ali framefront();
        printf("nothing !");
    }
    else if(!strncasecmp(s,"framepause",7)){
        sscanf(s,"%*s %f",&framehold);
        sprintf(command_result,"Frames pause %.2f",framehold);
    }
    else if(!strncasecmp(s,"back",2)){
        expback();
    }
    else if(!strncasecmp(s,"mimic",4)){
        mimic_fixation = !mimic_fixation;
        sprintf(command_result,"mimic %s",mimic_fixation ? "On":"Off");
    }
    else if(!strncasecmp(s,"nomonkey",7)){
        check_for_monkey = 0;
        printf("Not Checking for running without monkey\n");
    }
    else if(!strncmp(s,"test",4)){

            glDrawBuffer(GL_BACK);
//        statusline("test");

        
//        ShowTrialsNeeded();
//        setstimuli(1);
//        ListExpStims(NULL);
        //CheckOption(57);
//    setoption();
//    SerialSend(OPTION_CODE);
    }
    else if(!strncasecmp(s,"rndinit",4)){
        InitRndArray(expt.st->left->baseseed,10000000);
    }
    else if(!strncasecmp(s,"xyfsd",5)){
        sscanf(s,"xyfsd=%f",&val);
        expt.bwptr->fsd[10] = val;
        if(expt.bwptr->cflag & (1<<10))
            c = '+';
        else
            c = '-';
        sprintf(buf,"ch10%c,fs%.3f\n",c,val);
        SerialString(buf,NULL);
    }
    else if(!strncasecmp(s,"debug",4)){
        sscanf(s,"%*s %d",&debug);
        sprintf(command_result,"debug %d",debug);
    }
    else if(!strncasecmp(s,"saveim",6)){
        if(!testflags[PLAYING_EXPT]){
            testflags[SAVE_IMAGES] = 2;
            if((i = sscanf(s,"saveim%d",&ival)) > 0)
                testflags[SAVE_IMAGES] = ival;
            sprintf(buf,"./%spgm.idx",expname);
            imidxfd = fopen(buf,"a");
            sprintf(imname,"%s/%sim.txt",ImageOutDir,expname);
            sprintf(command_result,"Images -> %sim.txt",expname);
            if(imoutfd){
                fclose(imoutfd);
                imoutfd = fopen(imname,"a"); // in case it is the same
            }
            else
                imoutfd = fopen(imname,"w"); // First call - start a new file
        }
    }
    else if(!strncasecmp(s,"btest",4)) /* simulate broken BW */
    {
        SerialSignal(START_TRIAL);
        fsleep(0.01);
        SerialSignal(START_TRIAL);
        fsleep(0.01);
        SerialSignal(START_TRIAL);
        fsleep(0.01);
        SerialSignal(START_TRIAL);
        fsleep(0.01);
        SerialSignal(START_TRIAL);
    }
    else if(!strncasecmp(s,"track",2)){
        optionflags[PLOT_ELECTRODE_TRACK] = !optionflags[PLOT_ELECTRODE_TRACK];
        sprintf(command_result,"PlotTracj %s",optionflags[PLOT_ELECTRODE_TRACK] ? "On":"Off");
    }
    else
        retval = -1;

    return(retval);
}


void AddElectrodeString(char *s)
{
    int i = 0;
    while(electrodestrings[i] != NULL && i < 100)
        i++;
    electrodestrings[i] = (char *)malloc(strlen(s)+2);
    strcpy(electrodestrings[i],s);
    electrodestrings[i+1] = NULL;
}

void AddUserString(char *s)
{
    int i = 0;
    while(userstrings[i] != NULL && i < 100)
        i++;
    userstrings[i] = (char *)malloc(strlen(s)+2);
    strcpy(userstrings[i],s);
    userstrings[i+1] = NULL;
}



double pos2phase(Stimulus *st)
{
    double x;
    
    x = (st->pos.phase - M_PI_2)/(2 * M_PI * st->f);
    return(x);
}

void setexp(int w, int id, int val)
{
    int i;
    struct plotdata *plot;
    //    XmString str;
    //    Widget item;
    float fval;
    
    expt.vals[EXPT1_MAXSIG] = 0;
    expt.flag &= (~LOGINCR);
    setextras();
    plot = expt.plot;
    //    for(i = 0; i < plot->nstim[0]; i++)
    //        plot->stims[i].flag &= (~BOX_ON);
    //    if(val != TF && expt.mode == TF && exframe != NULL)
    //    {
    //		item = FindWidgetChild(exframe,NFRAMES_CODE);
    //		numeric_set(item, PANEL_LABEL, "# Frames", NUMERIC_VALUE, expt.st->nframes, NULL);
    //    }
    
    plot->fplaces = nfplaces[val];
    expt.mean = GetProperty(&expt, expt.st, val);
    if (val != expt.mode){ //Changed type, reset some parameters that shouldn't carry over
        expt.hightype = expt.mode;
    }
    expt.mode = val;
    switch(val)
    {
        case DISP_AS_CYCLES:
            setexp(NULL, 0, DISP_X);
            if((fval = sin(expt.st->pos.angle)) != 0.0)
            {
                expt.incr = 1/(expt.st->f * 4 * fval);
                expt.mean = 0;
            }
            expt.nstim[0] = 5;
            break;
        case ORIENTATION:
            expt.mode = ORIENTATION;
            //            set_fprange(meanslider,                        0.0, 360.0, expt.mean, 0);
            expt.incr = 22.5;
            expt.nstim[0] = 9;
            break;
        case SF:
            expt.flag |= LOGINCR;
            expt.mean = 1.0;
            expt.incr = 0.5;
            //            set_fprange(meanslider,0.0, 30.0, expt.mean, 0);
            expt.plot->fplaces = 2;
            break;
        case DISP_X:
            if(expt.type2 == ANTICORRELATED_DISPARITY)
                val = CORRELATED_DISPARITY;
        case CORRELATED_DISPARITY:
            if(expt.type2 == CORRELATION)
                val = DISP_X;
            expt.mode = val;
            //            set_fprange(meanslider,-1.0, 1.0, expt.mean, 3);
            //            set_fprange(incrslider,                        0, 1.0, expt.incr, 3);
            break;
        case DISP_Y:
            expt.incr = 0.2;
            //            set_fprange(meanslider,
            //                        -1.0, 1.0, expt.mean, 3);
            //            set_fprange(incrslider,
            //                        0, 1.0, expt.incr, 3);
            break;
        case DOT_SIZE:
            expt.incr = 0.1;
            expt.mean = 0.2;
            expt.nstim[0] = 3;
            //            set_fprange(meanslider,
            //                        -1.0, 1.0, expt.mean, 3);
            //            set_fprange(incrslider,
            //                        0, 1.0, expt.incr, 3);
            break;
        case ORTHOG_DISP:
            expt.incr = 0.2;
            //            set_fprange(meanslider,
            //                        -1.0, 1.0, expt.mean, 3);
            //            set_fprange(incrslider,
            //                        0, 1.0, expt.incr, 3);
            break;
        case DISP_BACK:
            expt.mode = DISP_BACK;
            //            set_fprange(meanslider,                        0.0, 60.0, expt.mean, 0);
            break;
        case STIM_WIDTH:
            //            set_fprange(meanslider,0.0, 60.0, expt.mean, 0);
            break;
        case STIM_HEIGHT:
            //            set_fprange(meanslider,                        0.0, 60.0, expt.mean, 0);
            break;
        case SETZXOFF:
            break;
        case DISP_P:
            expt.mean = 30;
            expt.incr = 60;
            expt.nstim[0] = 6;
            //            set_fprange(meanslider,                        0.0, 60.0, expt.mean, 0);
            break;
        case END_OFFSET:
            SetProperty(&expt,expt.st,ORIENTATION,(float)(expt.rf->angle/10));
            expt.mean = pix2deg(expt.rf->size[0]/2) + GetProperty(&expt, expt.st,STIM_HEIGHT)/2;
            //            set_fprange(meanslider,                        -10.0, 10.0, expt.mean, 2);
            expt.incr = pix2deg(expt.rf->size[0])/10;
            //            set_fprange(incrslider,-10.0, 10.0, expt.incr, 2);
            break;
        case SIDE_OFFSET:
            expt.st->pos.phase = 0;
            if(rdspair(expt.st))
                expt.mean = 0.0;
            else if(expt.st->type == STIM_BAR)
            {
                option2flag |= FLASH_BIT;
                SetProperty(&expt,expt.st,ORIENTATION,expt.rf->angle);
                SetProperty(&expt,expt.st,STIM_WIDTH,(float)(0.0));
                expt.mean = pix2deg(expt.rf->size[1]/2);
            }
            else
                expt.mean = pix2deg(expt.rf->size[1]/2) + GetProperty(&expt, expt.st,STIM_WIDTH)/2;
            expt.mode = SIDE_OFFSET;
            //            set_fprange(meanslider,-10.0, 10.0, expt.mean, 2);
            expt.incr = expt.mean/5;
            //            set_fprange(incrslider,-10.0, 10.0, expt.incr, 2);
            break;
        case MODULATION_F:
            expt.flag |= LOGINCR;
            break;
        case TF:
            expt.flag |= LOGINCR;
        case TFLIN:
            expt.mode = TF;
            plot->fplaces = 1;
            expt.mode = TF;
            if(val == TF){
                expt.mean = 4.5;
                expt.incr = 2.25;
                expt.nstim[0] = 7;
            }
            else if(val == TFLIN){
                if(lasttf > 0){
                    expt.mean = lasttf/2;
                    expt.incr = lasttf;
                }
                expt.nstim[0] = 2;
            }
            //            set_fprange(meanslider,0.0, 24, expt.mean, 0);
            break;
        case MONOCULARITY_EXPT:
            expt.incr = 1;
            expt.mean = 0.5;
            expt.nstim[0] = 2;
            break;
            
        case CONTRAST_LEFT:
            expt.mean = 0.5;
            expt.incr = 0.25;
            expt.nstim[0] = 3;
            expt.mode = CONTRAST_LEFT;
            expt.flag |= LOGINCR;
            break;
        case STIM_SIZE:
            expt.incr = 0.5;
            expt.nstim[0] = 5;
            plot->fplaces = 2;
            expt.mode = val;
            expt.mean = (StimulusProperty(expt.st,STIM_WIDTH) + StimulusProperty(expt.st,STIM_HEIGHT))/2;
            break;
        case START_PHASE:
            plot->fplaces = 0;
            expt.mode = val;
            expt.mean = 15;
            expt.incr = 30;
            expt.nstim[0] = 6;
            break;
        case SETPHASE:
            plot->fplaces = 0;
            expt.mode = val;
            expt.mean = 30;
            expt.incr = 60;
            expt.nstim[0] = 6;
            break;
        case STIMULUS_MODE:
            expt.mode = val;
            expt.mean = 16.5;
            expt.incr = 1;
            expt.nstim[0] = 2;
            break;
        case DISP_A:
        case DISP_B:
            expt.mode = val;
            plot->fplaces = nfplaces[val];
            break;
        default:
            plot->fplaces = nfplaces[val];
            expt.mode = val;
            expt.mean = StimulusProperty(expt.st,val);
    }
    switch(val)
    {
        case STIM_SIZE:
        case SF:
        case TF:
        case STIM_WIDTH:
        case STIM_HEIGHT:
            //            SetSliderType(allfps[0], val);
            break;
            
    }
    expt.nextval = expt.mean;

    plot = expt.plot;
    /*  plot->fplaces = fplaces(expt.incr,2);*/
    plot->fplaces = nfplaces[val];
    if(plot->fplaces > 10)
        plot->fplaces = 1;
    setstimuli(1);

    psychclear(expt.plot,1);

    setstimuli(0);
    CheckExpts();
    //    SetExpPanel(&expt);
    /* newly deleted
     if(expt.nreps > 0 && mode & UFF_FILE_OPEN)
     optionflag |= STORE_WURTZ_BIT;
     */
    //SetRunButton(NULL);
    //SetWPanel();
}


void setsecondexp(int w, int id, int val)
{
    
    int i,type;
    struct plotdata *plot;
    float fval;
    
    expt.vals[EXPT1_MAXSIG] = 0;
    optionflag &= (~(CLAMP_EXPT_BIT | CLAMP_HOLD_BIT));
    if(optionflags[TIMES_EXPT]){
        expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
        expt.flag |= TIMES_EXPT2;
    }
    else{
        expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
        expt.flag |= ADD_EXPT2;
    }
    expt.flag &= (~ALTERNATE_EXPTS); /* only when forced */
    expt.flag &= (~EXPT2_BITS);
    expt.flag &= (~LOGINCR2);
    expt.type2 = val;
    if(expt.nstim[1] == 0)
        expt.nstim[1] = 1;
    
    switch(val)
    {
        case JUMPTYPE:
            expt.type2 = JUMPTYPE;
            if(expt.st->type == STIM_RDS){
                expt.mean2 = 2;
                expt.incr2 = 1;
                expt.nstim[1] = 5;
            }
            else{
                expt.mean2 = 1.5;
                expt.incr2 = 1;
                expt.nstim[1] = 4;
            }
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= (TIMES_EXPT2 );
            break;
        case RF_SIGN:
            expt.type2 = RF_SIGN;
            expt.mean2 = 0;
            expt.incr2 = 2;
            expt.nstim[1] = 2;
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= (TIMES_EXPT2 );
            break;
        case CONTRAST_PAIRS:
            expt.type2 = CONTRAST_PAIRS;
            expt.mean2 = 0.25;
            expt.incr2 = 0.5;
            expt.nstim[1] = 4;
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= (TIMES_EXPT2 );
            break;
        case START_PHASE:
            expt.type2 = START_PHASE;
            expt.mean2 = 45;
            expt.incr2 = 90;
            expt.nstim[1] = 4;
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= (TIMES_EXPT2);
            break;
        case LOG_SIZE:
            expt.flag |= (LOGINCR2);
        case STIM_SIZE:
            expt.type2 = STIM_SIZE;
            if(expt.mode == TF){
                expt.mean2 = 3;
                expt.incr2 = 2;
                expt.nstim[1] = 2;
            }
            else{
                expt.mean2 = 5;
                expt.incr2 = 1;
            }
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= (TIMES_EXPT2);
            break;
        case STIMULUS_TYPE_CODE: /* usually a 2-grating expt */
            expt.mean2 = 12;
            expt.incr2 = 2;
            expt.nstim[1] = 3;
            expt.flag |= (TIMES_EXPT2);
            expt.type2 = val;
            break;
        case RDSXSINE: /* usually a 2-grating expt */
            expt.mean2 = 2.5;
            expt.incr2 = 1;
            expt.nstim[1] = 2;
            expt.flag |= (TIMES_EXPT2);
            expt.type2 = STIMULUS_TYPE_CODE;
            break;
        case RDSBNONE: /* usually a 2-grating expt */
            expt.mean2 = 1;
            expt.incr2 = 2;
            expt.nstim[1] = 2;
            expt.flag |= (TIMES_EXPT2);
            expt.type2 = BACKSTIM_TYPE;
            break;
        case MONOCULARITY_EXPT:
        case LRINTERLEAVE:
        case LBINTERLEAVE:
        case RBINTERLEAVE:
        case LRBINTERLEAVE:
            if(val == LRINTERLEAVE){
                if(dominanteye == LEFT)
                    expt.incr2 = 2;
                else
                    expt.incr2 = -2;
            }
            else
                expt.incr2 = 1;
            switch(val)
        {
            default:
            case LRINTERLEAVE:
                expt.mean2 = 0;
                expt.nstim[1] = 2;
                break;
            case LBINTERLEAVE:
                expt.mean2 = -0.5;
                expt.nstim[1] = 2;
                break;
            case RBINTERLEAVE:
                expt.mean2 = 0.5;
                expt.nstim[1] = 2;
                break;
            case LRBINTERLEAVE:
                expt.mean2 = 0;
                expt.nstim[1] = 3;
                break;
        }
            expt.flag |= (TIMES_EXPT2 );
            expt.type2 = MONOCULARITY_EXPT;
            break;
        case CLAMP_DISPARITY_CODE:
            optionflag |= (CLAMP_EXPT_BIT | CLAMP_HOLD_BIT);
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2 | ALTERNATE_EXPTS);
            expt.nstim[1] = 2;
            expt.incr2 = 0.2;
            expt.mean2 = 0.0;
            break;
        case FIXATION_SURROUND:
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2 );
            expt.nstim[1] = 2;
            expt.mean2 = 1.0;
            expt.incr2 = 2.0;
            break;
        case TFLIN:
            expt.type2 = TF;
            expt.flag |= (TIMES_EXPT2 );
            break;
        case NFRAMES_CODE:
            expt.flag |= LOGINCR2;
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2);
            break;
        case SF:
        case JVELOCITY:
            expt.flag |= LOGINCR2;
        case SEED_DELAY:
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2 );
            expt.mean2 = 0;
            expt.incr2 = 1;
            expt.nstim[1] = 5;
            break;
        case DISP_P2:
        case LINEAR_SPEED:
        case SFLIN:
        case DEPTH_MOD:
        case DISP_GRAD:
        case PARALELL_DISP:
        case ORTHOG_DISP:
        case ORIENTATION:
        case STATIC_CONJUGATE:
        case SET_SEED:
        case STIM_WIDTH:
        case STIM_HEIGHT:
        case BACK_ORI:
        case BACK_TF:
        case DOT_SIZE:
        case ALTERNATE_STIM_MODE:
        case RELDISP:
        case RELVDISP:
        case VDISP_MOD:
        case HIGHX:
        case REWARD_BIAS:
        case SET_SEEDLOOP:
        case MIXAC:
        case BLACKDOT_FRACTION:
            if(val == SFLIN)
                val = SF;
            if(val == LINEAR_SPEED)
                val = JVELOCITY;
            if(val == RELDISP){
                expt.nstim[2] = 2;
                expt.mean2 = 0;
            }
            expt.flag |= (TIMES_EXPT2);
            expt.flag &= (~ALTERNATE_EXPTS);
            break;
        case PHASE_AS_DISP:
        case DISP_X:
        case PURSUIT_INCREMENT:
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2);
            break;
        case ORI_BANDWIDTH:
        case DISP_P:
        case NCOMPONENTS:
        case DELAY:
        case DISP_B:
        case DISTRIBUTION_CONC:
        case DISTRIBUTION_WIDTH:
        case TEMPORAL_ORI_BANDWIDTH:
        case INITIAL_DISPARITY:
        case INITIAL_MOVEMENT:
        case TARGET_RATIO:
        case NHIGHCOMPONENTS:
        case NLOWCOMPONENTS:
        case COVARY_XPOS:
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2);
            break;
        case DISP_BACK:
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2);
            break; 
        case BACK_CORRELATION:
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2 );
            expt.nstim[1] = 2;
            expt.mean2 = .5;
            expt.incr2 = 1;
            break;
        case CORRELATION:
            expt.type2 = val;
            expt.flag |= (TIMES_EXPT2);
            expt.flag &= (~ALTERNATE_EXPTS);
            expt.nstim[1] = 2;
            expt.mean2 = 0;
            expt.incr2 = 2;
            if(expt.mode == CORRELATED_DISPARITY)
                expt.mode = DISP_X;
            break;
        case SET_COUNTERPHASE: 
            /*  set to +- 1*/
            expt.type2 = val;
            expt.nstim[1] = 2;
            expt.mean2 = 0.5;
            expt.incr2 = 1;
            expt.flag |= (TIMES_EXPT2);
            expt.flag &= (~ALTERNATE_EXPTS);
            break;
        case ANTICORRELATED_DISPARITY:
            expt.type2 = val;
            expt.flag |= ADD_EXPT2;
            expt.nstim[1] = expt.nstim[0];
            expt.mean2 = expt.mean;
            expt.incr2 = expt.incr;
            if(expt.mode == DISP_X)
                expt.mode = CORRELATED_DISPARITY;
            break;
        case OPPOSITE_DELAY:
            expt.type2 = val;
            expt.flag |= ADD_EXPT2;
            expt.nstim[1] = expt.nstim[0];
            expt.mean2 = expt.mean;
            expt.incr2 = expt.incr;
            break;
        case XOFFLOG:
            expt.flag |= LOGINCR2;
            expt.mean2 = GetProperty(&expt,expt.st,SETZXOFF);
            expt.type2 = SETZXOFF;
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= TIMES_EXPT2;
            break;
        case SETCONTRAST:
        case ORI2:
        case STIM_ECCENTRICITY:
        case STIM_POLARANGLE:
        case DISP_Y:
        case SETZXOFF:
        case SETZYOFF:
            expt.mean2 = GetProperty(&expt,expt.st,val);
        case ORI_RIGHT:
        case STANDING_DISP:
        case BACK_SIZE:
        case HEIGHT_R:
        case BACK_CONTRAST:
        case FB_RELATIVE_CONTRAST:
            expt.type2 = val;
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= TIMES_EXPT2;
            break;
        case WIDTH_R:
            expt.type2 = val;
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= TIMES_EXPT2;
            if(expt.mode == WIDTH_L){
                expt.mean2 = expt.mean;
                expt.incr2 = expt.incr;
                expt.nstim[1] = expt.nstim[0];
            }
            break;
        case FIXPOS_Y:
        case ORTHOG_POS:
        case PARA_POS:
            expt.type2 = val;
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= ADD_EXPT2;
            break;
        case NPLANES:
            expt.type2 = val;
            expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
            expt.flag |= (TIMES_EXPT2 );
            expt.mean2 = 1.5;
            expt.incr2 = 1.0;
            expt.nstim[1] = 2;
            break;
        case CONTRAST_RIGHT:
            expt.flag |= (TIMES_EXPT2 );
            expt.flag |= LOGINCR2;
            expt.mean2 = 0.5;
            expt.incr2 = 0.25;
            expt.nstim[1] = 3;
            break;
        case CONTRAST_RATIO:
            expt.flag |= (TIMES_EXPT2);
            /*      expt.flag |= LOGINCR2;*/
            expt.nstim[1] = 2;
            break;
        case STATIC_VERGENCE:
            expt.flag |= (TIMES_EXPT2 );
            expt.mean2 = 3;
            expt.incr2 = 4.0;
            expt.nstim[1] = 3;
            break;
        case BACKGROUND_IMAGE:
        case BACKGROUND_MOVIE:
            expt.flag |= (TIMES_EXPT2);
            break;
        default:
            i = 0;
            expt.type2 = EXPTYPE_NONE;
            optionflags[PLOTFLIP] = 0;
            while((type = secondmenu[i++].val) >= 0){
                if(type == val){
                    expt.type2 = type;
                    expt.flag |= (TIMES_EXPT2); //Default is times
                }
            }
            if(expt.type2 == EXPTYPE_NONE){ /* Not found*/
                expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
                expt.nstim[1] = 0;
            }
            break;
    }
    if(expt.type2 == expt.mode && expt.mode != EXPTYPE_NONE){
        expt.flag &= (~TIMES_EXPT2);
        expt.flag |= ADD_EXPT2;
    }
    if(expt.flag & TIMES_EXPT2)
        optionflags[TIMES_EXPT] = 1;
    else
        optionflags[TIMES_EXPT] = 0;
    
    if(option2flag & PSYCHOPHYSICS_BIT)
        expt.flag &= (~ALTERNATE_EXPTS);

    psychclear(expt.plot,1);
    setstimuli(0);
    CheckExpts();
    ListExpStims(NULL);
}


/*
 * ResetExpt turns off items that are never intended to carry over from
 * one experiment to the next, so that expt files must always set these 
 * explicitly if they are required
 */
void ResetExpt()
{
    expt.nstim[4] = 1;
    expt.type3 = EXPTYPE_NONE;
    /*
     * with cylinder pairs, do sometimes want backfixed on from one expt to the next
     */
    if(expt.st->type == STIM_CYLINDER &&  expt.st->next && expt.st->next->type == STIM_CYLINDER)
        optionflag = optionflag; 
    else
        optionflag &= (~BACKGROUND_FIXED_BIT);
    SetProperty(&expt,expt.st,CONTRAST_DIFF,0);
    SetProperty(&expt,expt.st,FAST_SEQUENCE_RPT,1);
    expt.vals[ALTERNATE_STIM_MODE] = 0;
    expt.vals[STIMULUS_MODE] = 0;
    expt.vals[EXPT1_MAXSIG] = 0;
    expt.vals[FP_MOVE_FRAME] = 0;
    optionflags[TILE_XY] = 0;
    expt.biasedreward = 0;  
    afc_s.proportion = 0.5;
    expt.st->jumps = 0;
}


void checkstimbuffers(int nstim, int nreps)
{
    int *temp;
    
    if(nstim+1 > nisset)
    {
        temp = (int *)malloc(sizeof(int) * nisset);
        memcpy(temp, isset, sizeof(int) * nisset);
        nisset = nstim+1;
        isset = (int *)malloc(sizeof(int) * nisset);
        memcpy(isset, temp, sizeof(int) * nisset);
        
    }
    if(nstim*(nreps+1) > nstimorder)
    {
        temp = (int *)malloc(sizeof(int) * nstimorder);
        memcpy(temp, stimorder, sizeof(int) * nstimorder);
        nstimorder = nstim * (nreps+1);
        stimorder = (int *)malloc(sizeof(int) * nstimorder);
        memcpy(stimorder, temp, sizeof(int) * nstimorder);
    }
}

/*
 * setorderbits checks whether a stimulus is an
 * extra stimulus, or a stimulus belonging to expt 2 when being
 * run side by side with expt1 (rather than a 2x2 expt)
 * and sets appropriate bits in the stimorder so that
 * the right thing is done at exploop time
 */

void setorderbits(int stimi)
{
    int i,ex;
    double drnd,pv;
    
    /*
     * This is a bit crazy. Need to set up an array that maps
     * from order # to the bit that is set, then just use
     * that array...
     */
    stimorder[stimi] &= (~ORDER_BITS);
    i = stimorder[stimi];
    if(stimorder[stimi] >= expt.nstim[3] && expt.nstim[4] > 1)
        stimorder[stimi] = (stimorder[stimi]) | STIMULUS_EXTRA_EXPT3;
    
    if(i%expt.nstim[3] < expt.nstim[2]){
        ex = extratypes[i%expt.nstim[3]];
        if(ex == STIMULUS_EXTRA_FLIP){
        }
        stimorder[stimi] |= ex;
    }
    else if(i%expt.nstim[3] >= expt.nstim[0] && expt.flag & ADD_EXPT2)
        stimorder[stimi] |= STIMULUS_EXTRA_EXPT;
    /*
     if(stairfd & (mode & EXPTPENDING)){
     fprintf(stairfd,"Stim %d:%d Changed to %d ",i,stimi, stimorder[stimi]);
     }
     */
}



int SetFirstStairVal()
{
    int j,stimid;
    
    j = 1;
    afc_s.jstairval = INTERLEAVE_EXPT_ZERO;
    while(afc_s.jstairval < INTERLEAVE_EXPT && j < expt.nstim[0]){
        stimid = expt.nstim[0]+expt.nstim[2]-j;
        afc_s.jstairval = expval[stimid];
        j++;
    }
    return(stimid);
}


int setexp3stim()
{
    double val;
    int i,j,ok=0;
    
    if(expt.type3 != EXPTYPE_NONE)
        switch(expt.type3){
            case ORI_BANDWIDTH:
                expt.exp3vals[0] = 10;
                expt.exp3vals[1] = 130;
                expt.nstim[4] = 2;
                break;
            case SET_SEEDLOOP:
            case MICROSTIM_EXPT:
                expt.exp3vals[0] = 0;
                expt.exp3vals[1] = 1;
                expt.nstim[4] = 2;
                break;
            case STIM_POLARANGLE:
                expt.exp3vals[0] = StimulusProperty(expt.st,STIM_POLARANGLE);
                expt.exp3vals[1] = expt.exp3vals[0] + 90;
                expt.nstim[4] = 2;
                break;
            case LRBINTERLEAVE:
                expt.exp3vals[0] = -1;
                expt.exp3vals[1] = 0;
                expt.exp3vals[2] = 1;
                expt.nstim[4] = 3;
                break;
            case MONOCULARITY_EXPT:
            case CORRELATION:
                expt.exp3vals[0] = 1;
                expt.exp3vals[1] = -1;
                expt.nstim[4] = 2;
                break;
            case DISP_P:
                expt.stimvals[DISP_P] = StimulusProperty(expt.st,DISP_P);
                expt.exp3vals[0] = rad_deg(expt.stimvals[DISP_P]);
                expt.exp3vals[1] = rad_deg(expt.stimvals[DISP_P] + M_PI);
                expt.nstim[4] = 2;
                break;
                break;
                /*
                 * for these paramaters, take the current value, and interleave + and -
                 * that value
                 */
            case ORIENTATION:
            case RELDISP:
            case SPINRATE:
                val = expt.stimvals[expt.type3] = StimulusProperty(expt.st,expt.type3);
                
            case XPOS:
            case YPOS:
                expt.exp3vals[0] = val;
                expt.exp3vals[1] = -val;
                expt.nstim[4] = 2;
                break;
            case ASPECT_RATIO:
                expt.exp3vals[0] = StimulusProperty(expt.st,expt.type3);
                expt.exp3vals[1] = 1/expt.exp3vals[0];
                expt.nstim[4] = 2;
                break;
            case FIXPOS_Y:
                val = expt.mean3 = StimulusProperty(expt.st,FIXPOS_Y);
                for(i = 0; i < expt.nstim[4]; i++)
                    expt.exp3vals[i] = expt.incr3 * i +val-(expt.incr3 * (expt.nstim[4]-1))/2;
                break;
            case FIXPOS_X:
                val = expt.mean3 = StimulusProperty(expt.st,FIXPOS_X);
                for(i = 0; i < expt.nstim[4]; i++)
                    expt.exp3vals[i] = expt.incr3 * i +val-(expt.incr3 * (expt.nstim[4]-1))/2;
                break;
            case SETANNULUS:
                expt.nstim[4] = 3;
                expt.exp3vals[0] = STIM_ANNULUS;
                expt.exp3vals[1] = STIM_RDS;
                expt.exp3vals[2] = STIM_NONE;
                break;
            case FAKESTIM_EXPT:
                expt.nstim[4] = 2;
            case TONETIME:
            case STIMULUS_MODE:
            case TARGET_RATIO:
            case SETCONTRAST:
            case BACK_SIZE:
            case NFRAMES_CODE:
            case STIMULUS_TYPE_CODE:
            case INITIAL_DURATION:
            case RANDOM_CONTRAST_EXPT:
            case FAST_SEQUENCE_RPT:
            case DISP_X:
            case STIM_SIZE: // This group the user sets ranges. See also setstimuli()
            case SET_SEED:
            case SEEDOFFSET:
            case PLAID_ANGLE:
            default:
                ok = 0;
                for(j = 0; j < NEXPTS3; j++)
                    if (thirdmenu[j].val == expt.type3)
                        ok = 1;
                if(ok ==0){
                    expt.nstim[4] = 1;
                    expt.type3 = EXPTYPE_NONE;
                }
                else if(optionflags[CUSTOM_EXPVALC] == 0){
                    for(i = 0; i < expt.nstim[4]; i++){
                        val = (expt.incr3 * i);
                        val =  expt.mean3-(expt.incr3 * (expt.nstim[4]-1))/2;
                        expt.exp3vals[i] = (expt.incr3 * i) +expt.mean3-(expt.incr3 * (expt.nstim[4]-1))/2;
                    }
                }
                break;
        }
    return(expt.nstim[4]);
}

/*
 * stimulusorder is now complicated because of mulitpe experimnets.
 * stimulus values are listed in expval as:
 * nstim[2], nstim exp1 vals, nstim[1] exp2 vals
 *
 * if ADDING a second expt, the number in stimorder is simple th
 * number of the relevant stimulus in expval
 *
 * if MULTIPLYING two expts (all nstim * nstim[1] combinations)
 * then stimorder is nstim[2] + exp1 val + nstim[1] * exp2 val
 */

void setstimulusorder(int warnings)
{
    int i, nreps,j = 0,nstim,n,tw,a,b;
    int thisblk, blksize,k,rnd,last = -1, lastctr = 0,nblk = 0;
    int loop, nset, errs = 0;
    char cbuf[BUFSIZ],buf[BUFSIZ];
    int twoseq[MAXSTIM*MAXREPS],donetwo[BUFSIZ],threeseq[BUFSIZ];
    int ifcstimno = 0,nstimtotal = 0;
    int ntoset = 0;
    int maxrpts = 3;
    int tried = 0;
    int stimcount[TRIALBUFFERLEN],exoff = 0;
    int m, maxcnt,tx,ix;
    int maxout[BUFSIZ], stillneed[BUFSIZ],ni,ns,nneed;
    int oddrnd = 0,evenrnd = 0,tm = 0;
    int txsum[BUFSIZ];
    Thisstim *stp;
    int nmin,e1,e2,skip = 0;
    float val;
    int reptwo = 0,jt = 0;
    int seed, baseseed, nrpt, rpts[MAXSTIM][MAXREPS];
    int rptid[MAXSTIM][MAXREPS];
    FILE *out;
    int noneed = 0;
    int rcrpt = 0;
    
    if(!(mode & RUNNING))
        return;
    if(option2flag & (PSYCHOPHYSICS_BIT | AFC))
        expt.flag &= (~ALTERNATE_EXPTS);
    
    if(expt.type2 == PURSUIT_INCREMENT)
        reptwo = 1;
    
    flips[0] = flips[1] = 0;
    if((nreps = expt.nreps) == 0)
        nreps = ZEROBLOCKING;
    repetitions_set = nreps;
    nset = nreps+1;
    baseseed = expt.st->left->baseseed & 0x1;
    
    maxrpts = 3;
    if(optionflags[ALWAYS_CHANGE_STIM])
        maxrpts = 0;
    /* set an order for one more set of stimuli than required so that
     * it is OK to run a few trials over the end, when > 1 stimulus is
     * run in each trail
     */
    nstim = expt.nstim[0]+ expt.nstim[2];
    if(expt.type2 != EXPTYPE_NONE && (expt.flag & ADD_EXPT2))
        nstim += expt.nstim[1];
    else if(expt.type2 != EXPTYPE_NONE && (expt.flag & TIMES_EXPT2))
        nstim = (expt.nstim[0] * expt.nstim[1]) + expt.nstim[2];
    
    /*
     * For IFC pairs, if an interleaved stimulus is selected, this is used
     * for the control interval
     * if nstim[2] > 0 means there there is some interleaved stimulus. 
     * Use the first of these for the control interval. = 0
     */
    if(expt.nstim[2]> 0)
        ifcstimno = nstim - expt.nstim[2];
    
    /*
     * in most cases (like microstim), expt 3 has fixed mean,increment and nstim.
     * these are forced here...
     */
    setexp3stim();
    
    nstimtotal = nstim * expt.nstim[4];
    expt.nstim[5] = nstimtotal;

    
    ifcstimno = 0;
    expt.nstim[3] = nstim;
    SetExpVals();
    
    
    if(seroutfile) fprintf(seroutfile,"#SO Plot n set to %d\n",nstim);
    checkstimbuffers(nstimtotal, nreps);
    for(i = 0; i < nstimtotal; i++){
        isset[i] = 0;
        uncompleted[i] = nreps/2;
        completed[i] = 0;
    }
    
    
    srandom(expseed++);
    myrnd_init(expseed);
    if(expt.blksize > 0 && expt.blksize <= nreps)
        blksize = expt.blksize;
    else
        blksize = 1;
    if(option2flag & PSYCHOPHYSICS_BIT)
        blksize = nreps;
    if(blksize > nreps)
        blksize = nreps;
    thisblk = 1;
    
    /* 
     * make the sequence longer than necessary in case there are some left
     * over stimuli at the end of the last trial
     * don't do this if one per trial. Can mess up total counts with psych because
     * of shufflestim. 
     */
    if(expt.stimpertrial > 1){
        // bgc 2011. Used to check against nstim, not 1. Surely an error? 
        ntoset = nstimtotal*nreps+expt.stimpertrial;
        nset = (ntoset +nstimtotal -1)/nstimtotal;
    }
    else{
        ntoset = nstimtotal*nreps;
        nset = nreps;
    }
    if(ntoset > TRIALBUFFERLEN){
        sprintf(cbuf,"%d trials is too many",ntoset);
        acknowledge(cbuf,NULL);
        return;
    }
    expt.nstim[6] = ntoset;
    
    seed = expt.st->left->baseseed;
    for(i = 0; i< ntoset; i++){
        stim3order[i] = 0;
        stim2order[i] = 0;
        seedorder[i] = seed;
        seed += 200;
    }
    
    
    if(optionflag & SEARCH_MODE_BIT)
    {
        for(i = 0; i< ntoset; i++)
            if((k = i%nstim) < expt.nstim[2])
                stimorder[i] = k;
            else if(expt.flag & TIMES_EXPT2)
                stimorder[i] = ((k-expt.nstim[2]) % expt.nstim[1]) * expt.nstim[0] + ((k-expt.nstim[2])/expt.nstim[1])%expt.nstim[0];
            else
                stimorder[i] = k;
        
    }
    else if(expt.flag & ALTERNATE_EXPTS && expt.flag & TIMES_EXPT2){
        for(tw = 0; tw < (expt.nstim[1]) * expt.nstim[4]; tw++){
            donetwo[tw] = 0;
            twoseq[tw] = 0;		
            threeseq[tw] = 0;
        }
        for(i = 0; i< ntoset && noneed < 10;)
        {
            tried = 0;
            thisblk = blksize * (1 + (i/(nstim * blksize)));
            if(thisblk >= nset)
                thisblk = nset;
            if(optionflags[RUN_SEQUENCE] && expt.stimpertrial > nstim){
                thisblk += (1+expt.stimpertrial/nstim);
            }
            
            /*
             * set up a pseurorandom sequence for the order of exp2's
             * twoseq is a list of values 
             */
            for(tw = 0; tw < (expt.nstim[1]) * expt.nstim[4]; tw++){
                donetwo[tw] = 0;
                twoseq[tw] = 0;		
                threeseq[tw] = 0;
            }
            if(optionflags[RUN_SEQUENCE] && expt.nstim[4] > 1){
                nblk = 1+expt.stimpertrial/(expt.nstim[1]);
                for(tw = 0; tw < (expt.nstim[1]) * expt.nstim[4]; tw++){
                    maxcnt = 0;
                    do{
                        rnd = myrnd_i();
                        n = twoseq[tw] = rnd % expt.nstim[1];
                        if((tw+i)%expt.stimpertrial == 0)
                            tm = threeseq[tw] = (rnd >> 8) % expt.nstim[4];
                        else
                            threeseq[tw] = tm;
                        j = tm * expt.nstim[1] + n;
                    }while(donetwo[j] >= nblk && maxcnt++ < 1000);
                    donetwo[j]++;
                }
            }
            else{
                for(tw = 0; tw < (expt.nstim[1]) * expt.nstim[4]; tw++){
                    maxcnt = 0;
                    do{
                        rnd = myrnd_i();
                        n = twoseq[tw] = rnd % expt.nstim[1];
                        m = threeseq[tw] = (rnd >> 8) % expt.nstim[4];
                        j = m * expt.nstim[1] + n;
                    }while(donetwo[j] && maxcnt++ < 1000);
                    donetwo[j] = 1;
                    if(reptwo){
                        twoseq[++tw] = n;
                        threeseq[tw] = m;
                    }
                }
            }
            
            /*
             * expt.nstim[4] allows a third expeiment type to be multiplied with
             * expts 1 and 2;
             * interleaved single stimuli are repeated for the third expt.
             * so total number of trials is 
             *  ((expt.nstim[0] * expt.nstim[1]) + expt.nstim[2]) * expt.nstim[4]
             * N.B. interleaves (expt.nstim[2]) are repeated for each value of third expt,
             * but not for each value of second expt. 
             */
            for(m = 0; m < expt.nstim[4]; m++){
                for(k = 0; k < (expt.nstim[1]); k++)
                {
                    ix = m * expt.nstim[1] + k;
                    tw = twoseq[ix];
                    tx = threeseq[ix];
                    tried = 0;
                    
                    if(tw == 0){
                        ns = (expt.nstim[0] + expt.nstim[2]);
                        exoff = 0;
                    }
                    else{
                        ns = (expt.nstim[0]);
                        exoff = expt.nstim[2];
                    }
                    
                    nneed = 0;
                    for(j = 0; j < ns; j++){
                        ni = exoff + j + tx * expt.nstim[3] + tw * expt.nstim[0];
                        if(isset[ni] < thisblk){
                            stillneed[nneed++] = ni;
                        }
                    }
                    /*
                     * may not need any for this exp2/exp3 combo, but will do some interleaves
                     * on later passes
                     */
                    if(nneed){
                        rnd = myrnd_i();
                        j = rnd % nneed;
                        stim3order[i] = tx;
                        ni = stimorder[i] = stillneed[j];
                        isset[ni]++;
                        setorderbits(i++);
                        noneed = 0;
                    }
                    else
                        noneed++;
                    
                    /*
                     do
                     {
                     rnd = myrnd_i();
                     if(tw == 0){
                     j = rnd % (expt.nstim[0] + expt.nstim[2]);
                     exoff = 0;
                     }
                     else{
                     j = rnd % (expt.nstim[0]);
                     exoff = expt.nstim[2];
                     }
                     stim3order[i] = tx;
                     stimorder[i] = exoff + j + tx * expt.nstim[3] + tw * expt.nstim[0];
                     j = stimorder[i] & (~ORDER_BITS);
                     if(optionflags[RUN_SEQUENCE]){
                     tried = 0;
                     thisblk = 1000000;
                     }
                     
                     }while(isset[j] >= thisblk && tried++ < nstimtotal * 500);
                     if(tried > nstimtotal * 500){
                     maxout[i] = 1;
                     }
                     else
                     maxout[i] = 0;
                     ni = j;
                     
                     isset[j]++;
                     setorderbits(i++);
                     */
                    thisblk = blksize * (1 + (i/(nstim * blksize)));
                    if(thisblk >= nset)
                        thisblk = nset;
                    if(optionflags[RUN_SEQUENCE] && expt.nstim[4] > 1)
                        thisblk += (1+ (expt.stimpertrial/nstim));
                }
            }
            
        }
    }
    else if(option2flag & IFC)
        for(i = 0; i< (nstim-1) * nreps *2;)
        {
            loop = 0;
            thisblk = blksize * (1 + (i/((nstim-1) * blksize * 2)));
            if(thisblk >= nset)
                thisblk = nset;
            
            do{
                last = j;
                rnd = myrnd_i();
                /*
                 * The comparison stimulus is stimulus 0. 
                 *      j = (rnd %(nstim-1)) + 1; 
                 *	ensures that the IFC was never a same-same comparison.
                 * usually no point in doing this as the order is randomized anyway.
                 * if the comparison stimulus is of the interleaved type, should have
                 *	      j = (rnd %(nstim-1));
                 */
                j = (rnd %(nstim-1))+1;
                rnd = myrnd_i();
                if(rnd & 1)
                    oddrnd++;
                else
                    evenrnd++;
                if((rnd & 1))
                {
                    stimorder[i] = j;
                    stimorder[i+1] = ifcstimno;
                }
                else
                {
                    stimorder[i] = ifcstimno;
                    stimorder[i+1] = j;
                }
                if(j == last && nstim > 2)
                    lastctr++;
                else
                    lastctr = 0;
            }while(isset[j] >= thisblk || lastctr > 10 && loop++ < 20);
            if(isset[j] > thisblk){
                errs++;
            }
            
            isset[j]++;
            setorderbits(i++);
            setorderbits(i++); /* increment twice for IFC */
        }
    /* 
     * Finally, deal with case where Expt 2 is not multiplied
     * N.B. Expt 3 takes all of expts 1,2 and interleaved, and 
     * does them under two different contitions. (E.G.  the whole 
     * stimulus set with and without microstimulation.
     */
    
    else {
        if(optionflags[RUN_SEQUENCE] && expt.nstim[4] > 1)
            nblk = 1+expt.stimpertrial/nstim;
        for(i = 0; i< ntoset;)
        {
            loop = 0;
            do{
                thisblk = blksize * (1 + ((i+skip)/(nstimtotal * blksize)));
                if(thisblk >= nset)
                    thisblk = nset;
                if(optionflags[RUN_SEQUENCE] && expt.stimpertrial > nstim)
                    thisblk += nblk;
                last = j;
                /*
                 * set value for exp3 only at start of a trial. Have to keep track of when
                 * have enough exp3 trials for this block.
                 */
                if(!optionflags[RUN_SEQUENCE] || i%expt.stimpertrial == 0){
                    if(expt.nstim[4] > 1){
                        for(k = 0; k < expt.nstim[4]; k++)
                            txsum[k] = 0;	
                        for(k = 0; k < nstimtotal; k++){
                            txsum[k/nstim] += isset[k];
                        }
                        do{
                            tx = myrnd_i() % expt.nstim[4];
                        }while(txsum[tx] >= ((thisblk-nblk) * nstim));
                    }
                    else
                        tx = 0;
                    
                }
                //	      j = stimorder[i] = (myrnd_i() % (nstim * expt.nstim[4]));
                j = stimorder[i] = (myrnd_i() % nstim)+ nstim * tx;
                twoseq[i] = j/expt.nstim[0];
                /*
                 * if the value of j picked above is already set, then so will its partner be
                 * so this loop would not end if we were to ender it
                 */
                if(reptwo && isset[j] < thisblk){
                    tw = j/expt.nstim[0];
                    tw = (expt.nstim[1]-1) - tw; // reverse sign
                    do{
                        jt = myrnd_i() %expt.nstim[0];
                        jt = expt.nstim[0] * tw + jt;
                        if(jt < 0) // can happen with single interleaves
                            stimorder[i+1] = 0;
                        else
                            stimorder[i+1] = jt;
                    }while(isset[jt] >= thisblk);
                    //		}while(isset[jt] >= thisblk+1);
                    // can't be too strict about thisblk with reptwo ? why
                    twoseq[i+1] = jt/expt.nstim[0];
                    if(twoseq[i] != expt.nstim[1]-1 - twoseq[i+1])
                        fprintf(stderr,"Sequence Error!\n");
                }
                stim3order[i] = tx;
                /* don't allow three repeats of the same stimulus */
                if(optionflags[RUN_SEQUENCE]){
                    thisblk = 1000000;
                    lastctr = 0;
                }
                else{
                    if(j == last && nstim > 1)
                        lastctr++;
                    else
                        lastctr = 0;
                }
            }while(isset[j] >= thisblk || (i == 0 && stimorder[i] < expt.nstim[2]) || lastctr > maxrpts && loop++ < 30);
            if(reptwo) i++;
            
            /* 
             * if 30 iterations fail to satisfy, there is probably an error and we should
             * move on
             */
            isset[j]++;
            if(reptwo)
                isset[jt]++;
            setorderbits(i++);
            if(isharris(altstimmode)){
                if(expavals[j] == 0 && expbvals[j] == 0 && expt.incr > 0 && expt.incr2 > 0){
                    i--;
                    skip++;
                    ntoset--;
                }
                
            }
            
        }
    }
    
    /*
     * Check that have correctly generated nreps repetitions
     */
    if(expt.vals[RC_REPEATS] > 0){
        rcrpt = expt.vals[RC_REPEATS]+1;
        for(j = 0; j < nstimtotal; j++)
            permute(&rpts[j][0], nreps);
        for(i = 0; i < nstimtotal; i++){
            stimcount[i] = 0;
        }
        for(i = 0; i < ntoset; i++){
            k = stimorder[i] & (~ORDER_BITS);
            rptid[k][stimcount[k]] = i;
            stimcount[k]++;
        }
//#define WATCHSEQ 1
#ifdef WATCHSEQ
        out = fopen("preseed.test","w");
        for(j = 0; j < ntoset; j++){
            fprintf(out,"%d %d\n",seedorder[j],stimorder[j]);
        }
        fclose(out);
#endif
        n = floor(nreps/(rcrpt));
        for(j = 0; j < nstimtotal; j++){
            for(k = 0; k < n; k++){
                a = rpts[j][k*rcrpt];
                for (m = 1; m < rcrpt; m++){
                    b = rpts[j][(k*rcrpt)+m];
                    seedorder[rptid[j][b]] = seedorder[rptid[j][a]];
                }
            }
        }
#ifdef WATCHSEQ
        out = fopen("seed.test","w");
        for(j = 0; j < ntoset; j++){
            fprintf(out,"%d %d\n",seedorder[j],stimorder[j]);
        }
        fclose(out);
#endif
    }
    
    
    for(i = 0; i < ntoset; i++){
        stimcount[i] = 0;
    }
    for(i = 0; i < ntoset; i++){
        k = stimorder[i] & (~ORDER_BITS);
        twoseq[i] = k/expt.nstim[0];
        stim2order[i] = k/expt.nstim[0];
        if(reptwo && (i & 1) && twoseq[i] != expt.nstim[1] - 1-twoseq[i-1])
        {
            printf("Sequence error for #2 - no repeat at %d (%d,%d)\n",i,twoseq[i],twoseq[i-1]);
            if(seroutfile)
                fprintf(seroutfile,"Seq2Rerr %d\n",i);
        }
        if(k >= expt.nstim[5])
            fprintf(stderr,"Stim %d is %d\n",i,k);
        stimcount[k]++;
        if(maxout[i] && verbose)
            fprintf(stderr,"Maxout %d at %d\n",i,k);
        
    }
    if(optionflags[RUN_SEQUENCE]){
        sprintf(cbuf,"");
        nmin = expt.nreps;
        for(i = 0; i < expt.nstim[5]; i++){
            if(stimcount[i] < nmin)
                nmin = stimcount[i];
            if(stimcount[i] < nset * 0.8){
                stp = getexpval(i);
                sprintf(buf,"%.2f %.2f %.2f %d reps\n",stp->vals[0],stp->vals[1],stp->vals[2],stimcount[i]);
                strcat(cbuf,buf);
            }
        }
        if(warnings){
            if(strlen(cbuf) > 10)
                acknowledge(cbuf,NULL);
        }
        sprintf(cbuf,"Min reps %d\n",nmin);
        statusline(cbuf);
        glstatusline(cbuf,1);
    }
    else{
        for(i = 0; i < expt.nstim[5]; i++){
#ifdef WATCHSEQ
            printf("Stim %d has %d reps\n",i,stimcount[i]);
#endif
            if(stimcount[i] != nset)
                fprintf(stderr,"Stim %d has %d reps (%d)\n",i,stimcount[i],nset);
        }
    }
    if(seroutfile){
        fprintf(seroutfile,"#Seq");
        for(i = 0; i < ntoset; i++){
            fprintf(seroutfile," %d",stimorder[i] & (~ORDER_BITS));
            if (i%100 == 99){ //try to avoid v long lines. 
                fprintf(seroutfile,"\n#SeqC");
            }
        }
        fprintf(seroutfile,"\n#nset %d = %d * %d \n",ntoset,nstimtotal,nreps);
    }
    
    /*
     * for human psychophysics staircases are two interleaved staircases
     * one starting from each end. rnd&1 is 0 or 1, so this randomizes which
     * of the two staircases comes first. Each staircase starts at the extreme
     * value or one less than this (0 or1; nstim-1 or nstim-2)
     */
    
    if(option2flag & PSYCHOPHYSICS_BIT && option2flag & STAIRCASE){
        rnd = myrnd_i();
        if(afc_s.type == MAGONE_SIGNTWO){
            stimorder[0] = SetFirstStairVal();
            afc_s.magid = stimorder[0];
        }
        else if(optionflags[STAIR_CENTER]){
            stimorder[0] = expt.nstim[0]/2 - 5 + rnd%11;
            stimorder[1] = expt.nstim[0]/2 - 5 + (rnd >4) %11;
        }
        else{
            stimorder[rnd&1] = expt.nstim[0]- 1 - ((rnd >2)&1);
            stimorder[!(rnd&1)] = (rnd > 1) &1;
        }
    }
    else if(option2flag & STAIRCASE){ // Monkey Staircase
        stimorder[0] = SetFirstStairVal();
        afc_s.magid = stimorder[0];
    }
    stimorder[i] = expt.nstim[2];
    if(seroutfile && option2flag & PSYCHOPHYSICS_BIT)
        fprintf(seroutfile,"IFC %d odd %d even\n",oddrnd,evenrnd);
#ifdef WATCHSEQ
    for (i = 0; i < ntoset; i++){
        printf("%d ",stimorder[i]);
    }
    printf("\n");
#endif
    
}

int permute(int *in, int n)
{
    int done[MAXSTIM],undone[MAXSTIM],seq[MAXSTIM];
    int i,j,k;
    
    j = 0;
    for(i = 0; i < n; i++)
        in[i] = i;
    while(j < n){
        i = myrnd_i() % (n-j);
        if(i > 0){
            k = in[j];
            in[j] = in[j+i];
            in[j+i] = k;
        }
        j++;
    }
#ifdef CHECKPERMUTE
    for(i = 0; i < n; i++)
        printf("%d ",in[i]);
    printf("\n");
#endif
    
}

int CountReps(int start)
{
    int i, ntoset,counts[1024],stim;
    
    
    ntoset = expt.nstim[6];
    for(i = 0; i < expt.nstim[5]; i++)
        counts[i] = 0;
    for(i = start; i < ntoset; i++)
    {
        stim = (stimorder[i] & ~(ORDER_BITS));
        counts[stim]++;
    }
    if(seroutfile){
        fprintf(seroutfile,"Remaining(%d): ",stimno);
        for(i = 0; i < expt.nstim[5]; i++)
            fprintf(seroutfile,"%d ",counts[i]);
        fprintf(seroutfile,"\n");
        
    }
    
}

void setstimuli(int flag)
{
    int i,j,offset,ns,ni,ok;
    double val,loginc,val2,ratio;
    char c = 'v',cbuf[BUFSIZ];
    
    
    setextras();
    if(optionflags[INTERLEAVE_MONOC_ALL] && expt.nstim[0] < 2)
        expt.nstim[0] = 2;
    if(expt.nstim[4] < 1)
        expt.nstim[4] = 1;
    
        
    if(expt.type2 != EXPTYPE_NONE && (expt.flag & ADD_EXPT2))
        expt.nstim[3] = expt.nstim[0]+ expt.nstim[2]+expt.nstim[1];
    else if(expt.type2 != EXPTYPE_NONE && (expt.flag & TIMES_EXPT2))
        expt.nstim[3] = (expt.nstim[0] * expt.nstim[1]) + expt.nstim[2];
    else
        expt.nstim[3] = expt.nstim[0]+ expt.nstim[2];
    switch(expt.type3){
            
        case MICROSTIM_EXPT:
        case FAKESTIM_EXPT:
        case XPOS:
        case ORIENTATION:
        case DISP_P:
        case RELDISP:
        case CORRELATION:
        case SET_SEEDLOOP:
        case MONOCULARITY_EXPT:
        case ASPECT_RATIO:
        case YPOS:
        case STIM_POLARANGLE:
        case SPINRATE:
            expt.nstim[4] = 2;
            break;	
        case STIMULUS_MODE:
        case NFRAMES_CODE:
        case STIMULUS_TYPE_CODE:
        case INITIAL_DURATION:
        case RANDOM_CONTRAST_EXPT: 
        case FAST_SEQUENCE_RPT: 
        case TARGET_RATIO:
        case DISP_X:
        case BACK_SIZE: //
        case STIM_SIZE: //
        case FIXPOS_Y: // This can be changed by the user
        case FIXPOS_X: // This can be changed by the user
        case SETCONTRAST: // This can be changed by the user
        case TONETIME:
        case SET_SEED:
        case SEEDOFFSET:
        case PLAID_ANGLE:
            break;
        default:
            ok  = 0;
            for (j = 1; j < NEXPTS3; j++)
                if (thirdmenu[j].val == expt.type3)
                    ok = 1;
            if (ok == 0)
                expt.nstim[4] = 1;
            break;
    }
    
    expt.nstim[5] = expt.nstim[3] * expt.nstim[4];

    if(seroutfile) fprintf(seroutfile,"#SS Plot n set to %d\n",expt.nstim[3]);
    
    ShowTrialsNeeded();
    for(i = 0; i < expt.nstim[2]; i++)
        expval[i] = expt.mean;
    
    ns = expt.nstim[0]-1;
    if(optionflags[INTERLEAVE_HIGH_ALL] && expt.type2 == ORI_BANDWIDTH)
        optionflags[INTERLEAVE_HIGH_ALL] = 2;
    
    if(optionflags[INTERLEAVE_ZERO_ALL])
        ns--;
    if(optionflags[INTERLEAVE_HIGH_ALL] == 1)
        ns -= 1;
    if(optionflags[INTERLEAVE_MONOC_ALL]){
        if(expt.st->type == STIM_RDSSINE || expt.type2 == CONTRAST_PAIRS)
            ns -= 1;
        else
            ns -= 2;
    }
    if(optionflags[INTERLEAVE_UNCORR_ALL])
        ns -= 1;
    if(expt.flag & LOGINCR)
    {
        loginc = log10(expt.mean/(expt.mean-expt.incr));
        if(expt.mean > expt.incr)
            ratio = expt.mean/(expt.mean-expt.incr);
        for(i = 0; i< expt.nstim[0]; i++)
        {
            j = i + expt.nstim[2];
            val = log10(expt.mean) + loginc *( i - (float)(ns)/2);
            expval[j] = pow(10.0,val);
//            expt.plot->stims[j].x[0] = expval[j];
        }
        
    }
    else
    {
        val = expt.mean - ((ns) * expt.incr)/2;
        for(i = expt.nstim[2]; i< (expt.nstim[0] + expt.nstim[2]); i++)
        {
            expval[i] = val;
            val += expt.incr;
        }
    }
    if(expt.vals[EXPT1_MAXSIG] > 0){
        expval[0+expt.nstim[2]] = -expt.vals[EXPT1_MAXSIG];
        expval[expt.nstim[0]-1+expt.nstim[2]] = expt.vals[EXPT1_MAXSIG];
    }
    
    if(optionflags[INTERLEAVE_HIGH_ALL] ==1)
        expval[expt.nstim[2] + expt.nstim[0]-1] = INTERLEAVE_EXPT_HIGH;
    if(optionflags[INTERLEAVE_ZERO_ALL]){
        expval[expt.nstim[2]] = INTERLEAVE_EXPT_ZERO;
    }
    if(optionflags[INTERLEAVE_MONOC_ALL]){
        if(expt.st->type == STIM_RDSSINE || expt.type2 == CONTRAST_PAIRS){
            expval[expt.nstim[2] + expt.nstim[0]-1] = INTERLEAVE_EXPT_COMPONENT_ONE;
        }
        else{
            expval[expt.nstim[2] + expt.nstim[0]-1] = INTERLEAVE_EXPT_RIGHT;
            expval[expt.nstim[2] + expt.nstim[0]-2] = INTERLEAVE_EXPT_LEFT;
        }
        if(optionflags[INTERLEAVE_UNCORR_ALL]){
            expval[expt.nstim[2] + expt.nstim[0]-3] = INTERLEAVE_EXPT_UNCORR;
        }
    }
    if(optionflags[INTERLEAVE_UNCORR_ALL]){
        expval[expt.nstim[2] + expt.nstim[0]-1] = INTERLEAVE_EXPT_UNCORR;
        if(expt.type2 == DISP_BACK){
            expval[expt.nstim[2] + expt.nstim[0] +expt.nstim[1]-1] = INTERLEAVE_EXPT_UNCORR;
        }
    }
    
    for(i = 0; i < expt.nstim[2]; i++){
        if(extratypes[i] & STIMULUS_EXTRA_HIGH)
            expval[i] = expt.vals[HIGHX];
    }
    
    offset = expt.nstim[0] + expt.nstim[2];
    ns = expt.nstim[1];
    ni = 0;
    if(optionflags[INTERLEAVE_ZERO_ALL] && expt.type2 == CONTRAST_RIGHT){
        expval[offset+ --ns] = 0;
    }
    
    
    if(!optionflags[CUSTOM_EXPVALB]){
        if(expt.flag & LOGINCR2)
        {
            loginc = log10(expt.mean2/(expt.mean2-expt.incr2));
            for(i = offset; i < offset+ns; i++)
            {
                val = log10(expt.mean2) + loginc *( (i-offset) - (float)(ns-1)/2);
                expval[i] = pow(10.0,val);
            }
            if(optionflags[INTERLEAVE_SINE])
                expval[offset+expt.nstim[1]-1] = INTERLEAVE_EXPT;
            if(optionflags[INTERLEAVE_ONE_SINE]){
                if(expt.st->type == STIM_GRATING2){
                    expval[offset+expt.nstim[1]-1] = INTERLEAVE_EXPT_SFONE;
                    expval[offset+expt.nstim[1]-2] = INTERLEAVE_EXPT_SFTWO;
                }
            }
        }
        else
        {
            val2 = expt.mean2 - ((expt.nstim[1]-1) * expt.incr2)/2;
            for(i = offset; i < offset+expt.nstim[1]; i++)
            {
                expval[i] = val2;
                val2 += expt.incr2;
            }
        }
    }
    ni = 1;
    
    if(optionflags[INTERLEAVE_HIGH_ALL] ==2)
        expval[offset+expt.nstim[1]-1] = expt.vals[HIGHX];
    
    /*
     if(optionflags[INTERLEAVE_ZERO_ALL] && expt.type2 == CONTRAST_RIGHT){
     expval[offset+expt.nstim[1]-ni++] = INTERLEAVE_EXPT_ZERO;
     }
     */
    if(optionflags[INTERLEAVE_SINE])
        expval[offset+expt.nstim[1]-ni++] = INTERLEAVE_EXPT;
    if(optionflags[INTERLEAVE_ONE_SINE]){
        expval[offset+expt.nstim[1]-ni++] = INTERLEAVE_EXPT_SFONE;
        expval[offset+expt.nstim[1]-ni++] = INTERLEAVE_EXPT_SFTWO;
    }
    if(optionflags[INTERLEAVE_UNCORR_ALL] && expt.type2 == DISP_BACK){
        expval[offset + expt.nstim[1]-ni++] = INTERLEAVE_EXPT_UNCORR;
    }

    /*
     * dont do this every time, - only if change expt 2
     optionflags[CUSTOM_EXPVALB] = 0;
     mode &= (~CUSTOM_EXPVAL);
     */
    SetExpVals();
    setexp3stim();
    if(flag)
        return;
    setstimulusorder(0);
}


void LoadBackgrounds()
{
    int i,len,nf = 0,j,start,ns,offset, frpt = 1;
    char name[BUFSIZ],cbuf[BUFSIZ];
    int seoffset = 0;
    
    if (expt.vals[FAST_SEQUENCE_RPT] > 1)
        frpt = (int)(expt.vals[FAST_SEQUENCE_RPT]);
    backims[MAXBACKIM] = expt.backim; // Store original
    if(expt.mode == BACKGROUND_MOVIE && optionflags[FAST_SEQUENCE]){
        nf = expt.vals[FAST_SEQUENCE_RPT]/2;
        ns = expt.nstim[0];
        offset = 0;
    }
    else if(expt.type2 == BACKGROUND_MOVIE){
        nf = expt.st->nframes/frpt;
        ns = 1;
        offset = expt.nstim[0];
    }
    else if(expt.mode == BACKGROUND_MOVIE){
        nf = expt.st->nframes/frpt;
        ns = expt.nstim[0];
        offset = 0;
    }
    else if(expt.mode == BACKGROUND_IMAGE){
        if(optionflags[FAST_SEQUENCE])    
            nf = 2;
        else
            nf = 1;
        ns = expt.nstim[0];
        offset = 0;
    }
    
    for(j = 0; j < ns; j++){
        //AliGLX    mySwapBuffers();
        start = expval[offset+expt.nstim[2] + j];
        if(expt.st->seedoffset > 0){
            start = 1;
            seoffset = expt.st->seedoffset;
        }
        else
            seoffset = 0;
        sprintf(cbuf,"Loading Movie %d\n",start+seoffset);
        glstatusline(cbuf,1);
        for(i = 0; i < nf; i++){
            sprintf(name,"%s%.*d.pgm",expt.backprefix,expt.st->nimplaces,i+start+seoffset);
            //    sprintf(name,"%s%d.pgm",expt.backprefix,i+1);
            len = ReadPGM(name,&backims[i]);
        }
    }
    backloaded = i+start;
}



int MakeString(int code, char *cbuf, Expt *ex, Stimulus *st, int flag)
{
    
    char *scode = valstrings[valstringindex[code]].code; //char code matching icode code
    char temp[BUFSIZ],cadd[BUFSIZ];
    float val;
    double *f;
    int ret = 0,ival =0,i,pcflag =0,nstim = 0,icode = 0;
    time_t tval;
    char *t,*r,c = ' ';
    
    
    if (flag == TO_GUI || flag == TO_FILE)
        sprintf(temp,"=");
    else
        sprintf(temp,"");
    sprintf(cbuf,"");
    
    
    icode = valstringindex[code];
    switch(code)
    {
        case USENEWDIRS:
            sprintf(cbuf,"%s=%d",serial_strings[code],usenewdirs);
            break;
        case FIXCOLORS:
            sprintf(cbuf,"fixcolors=%.2f %.2f %.2f %.2f\n",expt.st->fix.fixcolors[0],expt.st->fix.fixcolors[1],expt.st->fix.fixcolors[2],expt.st->fix.fixcolors[3]);
            break;
        case RF_SET:
            c = '*';
        case RF_DIMENSIONS:
            sprintf(cbuf,"%s=",serial_strings[UFF_COMMENT]);
            sprintf(temp,"rf%.2f,%.2f:%.2fx%.2f,%.1fdeg%c ",pix2deg(expt.rf->pos[0]),
                    pix2deg(expt.rf->pos[1]),pix2deg(expt.rf->size[0]),pix2deg(expt.rf->size[1]),expt.rf->angle,c);
            strcat(cbuf,temp);
            sprintf(temp,"pe%d %.1f,%.1f ",expt.ipen,(expt.vals[PENXPOS]),(expt.vals[PENYPOS]));
            strcat(cbuf,temp);
            sprintf(temp,"%2s=%.2f,%2s=%.2f ",serial_strings[FIXPOS_X],expt.vals[FIXPOS_X],serial_strings[FIXPOS_Y],expt.vals[FIXPOS_Y]);
            strcat(cbuf,temp);
            strcat(cbuf,"\n\0");
            break;
        case SOFTOFF_CODE:
            sprintf(cbuf,"%s%s%.2f %.2f %.2f %2f", scode,
                    temp,expt.softoff[0],expt.softoff[1],
                    expt.softoff[2],expt.softoff[3]);
            break;
        case UKA_VALS:
            f = afc_s.gregvals;
            sprintf(cbuf,"%s%s%.2f %.2f %.2f %.2f %.2f", scode,
                    temp,f[0],f[1],f[2],f[3],f[4]);
            break;
        case TRAPEZOIDAL_SCALING:
            sprintf(cbuf,"%s%s%.6f %.6f", scode,
                    temp,expt.mon->trapscale[0], expt.mon->trapscale[2]);
            break;
        case VERSION_CODE:
            sprintf(cbuf,"%s%s%s", scode,
                    temp,VERSION_NUMBER);
            break;
        case EARLY_RWTIME:
            sprintf(cbuf,"%s=%.2f %.2f", scode,expt.vals[EARLY_RWTIME],expt.vals[EARLY_RWSIZE]);
            break;
        case USERID:
            sprintf(cbuf,"%s=%s", scode,userstrings[userid]);
            break;
        case BACKGROUND_IMAGE:
            if(flag != TO_BW){
                if(expt.backprefix)
                    sprintf(cbuf,"%s=%s", scode,expt.backprefix);
                else
                    sprintf(cbuf,"");
            }
            else{
                sprintf(cbuf,"%s%.0f", scode,expt.vals[code]);
            }
            break;
        case UFF_PREFIX:
            if(expt.bwptr->prefix != NULL)
            {
                if(flag != TO_FILE && (r = strchr(expt.bwptr->prefix,'%')) != NULL && *(r+1) == 'd')
                {
                    strcpy(&temp[5],expt.bwptr->prefix);
                    r = strchr(&temp[5],'%');
                    tval = time(NULL);
                    t = ctime(&tval);
                    if(t[8]==' ')
                        t[8] = '0';
                    t[7]=0;
                    t[10] = 0;
                    sprintf(r,"%s%s",&t[4],&t[8]);
                    sprintf(cbuf,"%s%s%s",serial_strings[UFF_PREFIX],temp,&temp[5]);
                }
                else 
                    sprintf(cbuf,"%s%s%s",serial_strings[UFF_PREFIX],temp,expt.bwptr->prefix);
                if(option2flag & PSYCHOPHYSICS_BIT && expname != NULL)
                    sprintf(cbuf,"%s%s%s",serial_strings[UFF_PREFIX],temp,expname);
                
                if(flag != TO_FILE)
                {
                    if(expt.bwptr->prefix[strlen(expt.bwptr->prefix)-1] != '\\')
                    {
                        mode |= SAVE_UNITLOG;
                    }
                    else
                    {
                        mode &= (~SAVE_UNITLOG);
                    }	
                }
            }
            else 
                ret = -1;
            break;
        case MONKEYNAME:
            if(expt.monkey != NULL)
                sprintf(cbuf,"%s%s%s",serial_strings[MONKEYNAME],temp,expt.monkey);
            else
                ret = -1;
            break;
            
        case LOGFILE_CODE:
            if(expt.logfile != NULL)
                sprintf(cbuf,"%s%s%s",serial_strings[LOGFILE_CODE],temp,expt.logfile);
            else
                ret = -1;
            break;
        case STIMULUS_TYPE_CODE:
            sprintf(cbuf,"%s%s%s",scode,temp,stimulus_names[st->type]);
            break;
        case BACKSTIM_TYPE:
            if(st->next)
                sprintf(cbuf,"%s%s%s",scode,temp,stimulus_names[st->next->type]);
            break;
        case FRAMERATE_CODE:
            sprintf(cbuf,"%s%s%.2f",scode,temp,expt.mon->framerate);
            break;
        case NREPS_CODE:
            sprintf(cbuf,"%s%s%d",scode,temp,expt.nreps);
            break;
        case NEXTRAS_CODE:
            if(optionflags[FAST_SEQUENCE])
                sprintf(cbuf,"%s%s%d",scode,temp,expt.fastextras);
            else
                sprintf(cbuf,"%s%s%d",scode,temp,expt.nstim[2]);
            break;
        case NTRIALS_CODE:
            sprintf(cbuf,"%s%s%d",scode,temp,expt.nstim[0]);
            break;
        case EXPT2_NSTIM:
            sprintf(cbuf,"%s%s%d",scode,temp,expt.nstim[1]);
            break;
        case EXPT3_NSTIM:
            sprintf(cbuf,"%s%s%d",scode,temp,expt.nstim[4]);
            break;
        case POSTPERIOD_CODE:
            sprintf(cbuf,"%s%s%.2f",scode,temp,expt.postperiod);
            break;
        case QUERY_STATE:
        case SEND_CLEAR:
            sprintf(cbuf,"%2s", scode);
            break;
        case CLAMP_DISPARITY_CODE:
            sprintf(cbuf,"%s%s%.2f",scode,temp,
                    expt.cclamp);
            /*		if((optionflag & CLAMP_EXPT_BIT) || flag == TO_FILE)
             sprintf(cbuf,"%s%s%.2f",scode,temp,
             expt.cclamp);
             else
             sprintf(cbuf,"%sxx",scode);
             */
            break;
        case RAMP_VERGENCE_CODE:
            if((optionflag & (RAMP_EXPT_BIT | CLAMP_HOLD_BIT)) || flag != TO_BW)
                sprintf(cbuf,"%s%s%.2f",scode,temp,
                        expt.cramp);
            else if (optionflag & (RAMP_HOLD_BIT))
                return(-1);
            else
                sprintf(cbuf,"%sxx",scode); /* turns off ramp */
            break;
        case RAMP_HOLD_CODE:
            if((optionflag & (RAMP_HOLD_BIT)) || flag != TO_BW)
                sprintf(cbuf,"%s%s%.2f",scode,temp,
                        expt.cramp);
            else if (optionflag & (RAMP_EXPT_BIT))
                return(-1);
            else
                sprintf(cbuf,"%sxx",scode); /* turns off ramp */
            break;
        case PREPERIOD_CODE:
            sprintf(cbuf,"%s%s%.3f",scode,temp,
                    expt.preperiod);
            break;
        case ISI_CODE:
            if(expt.isi < (expt.preperiod + expt.postperiod))
                expt.isi = (expt.preperiod + expt.postperiod);
            sprintf(cbuf,"%s%s%.3f",scode,temp,expt.isi);
            break;
        case STIMULUS_FLAG:
            if(flag == TO_FILE || flag == TO_GUI)
            {
                sprintf(cbuf,"%s%s",scode,temp);
                i = 0;
                while(flag_codes[i] != NULL)
                {
                    if(st->flag & (1<<i))
                    {
                        sprintf(temp,"+%s",flag_codes[i]);
                        strcat(cbuf,temp);
                    }
                    i++;
                }
            }
            break;
        case MODE_CODE:
            if(flag != TO_BW)
            {
                sprintf(cbuf,"%s%s",scode,temp);
                i = 0;
                while(mode_codes[i] != NULL)
                {
                    if(st->mode & (1<<i)) 
                    {
                        sprintf(temp,"+%s",mode_codes[i]);
                        strcat(cbuf,temp);
                    }
                    i++;
                }
            }
 //           else
 //               sprintf(cbuf,"%s%s%d",scode,temp,st->mode);
            break;
        case SHOWFLAGS_CODE:
            sprintf(cbuf,"%s",expt.showflags);
            break;
        case EXPTSTRING:
            sprintf(cbuf,"%s%s%s",serial_strings[EXPTYPE_CODE3],temp,serial_strings[expt.type3]);
            if(expt.type3 != EXPTYPE_NONE && expt.nstim[4] < 50){
                for(i = 0; i < expt.nstim[4]; i++){
                    sprintf(cadd," %.3f",expt.exp3vals[i]);
                    strcat(cbuf,cadd);
                }
            }
            sprintf(cadd,",%2s%s%d",serial_strings[EXPT3_NSTIM],temp,expt.nstim[4]);
            strcat(cbuf,cadd);
            if(expt.flag & ALTERNATE_EXPTS)
                strcat(cbuf," alternate");
            break;
        case EXPTVALS:
            if(optionflags[FAST_SEQUENCE]){
                if(expt.mode == DISTRIBUTION_CONC ||
                   (expt.vals[DISTRIBUTION_CONC] > 0 && (expt.mode == PLAID_RATIO || expt.mode == CONTRAST_DIFF))){
                    fastvals[0] = INTERLEAVE_SIGNAL_FRAME;
                    // +1 to make room for fastvals[0]
                    nstim = 1+expt.vals[DISTRIBUTION_WIDTH];
                }
                else
                    nstim = expt.nstim[3];
                sprintf(cbuf,"mtei=");
                for(i = 0; i < nstim+expt.fastextras; i++){
                    if (nfplaces[expt.mode] == 0)
                        sprintf(cadd,"%.0f ",fastvals[i]);
                        else
                    sprintf(cadd,"%.4f ",fastvals[i]);
                    strcat(cbuf,cadd);
                }
                strcat(cbuf,"\nmte2=");
                for(i = 0; i < nstim+expt.fastextras; i++){
                    if (nfplaces[expt.type2] == 0)
                        sprintf(cadd,"%.0f ",fastbvals[i]);
                    else
                    sprintf(cadd,"%.4f ",fastbvals[i]);
                    strcat(cbuf,cadd);
                }
                strcat(cbuf,"\n");
                strcat(cbuf,"\nmte3=");
                for(i = 0; i < nstim+expt.fastextras; i++){
                    if (nfplaces[expt.type3] == 0)
                    sprintf(cadd,"%.0f ",fastcvals[i]);
                    else
                    sprintf(cadd,"%.4f ",fastcvals[i]);
                    strcat(cbuf,cadd);
                }
                strcat(cbuf,"\n");
                if (optionflags[TILE_XY]){
                strcat(cbuf,"\nmtxo=");
                for(i = 0; i < expt.st->jumps; i++){
                    val = (i-expt.st->jumps/2)*expt.vals[FP_MOVE_SIZE];
                    sprintf(cadd,"%.4f ",val);
                    strcat(cbuf,cadd);
                }
                strcat(cbuf,"\n");
                }
            }
            break;
        case OPTION_CODE:
            /*
             * This need special treatment, since the PC has 16-bit integers
             * and the SGI has 32-bits. Most of the bites in optionflag can be
             * ignored by BW, so need to covert the parts we need
             */
            if(flag == TO_FILE || flag == TO_GUI)
            {
                sprintf(cbuf,"%s=0",scode);
                i = 0;
                while(togglestrings[i].code != NULL)
                {
                    if (togglestrings[i].group == 3)
                    {
                        if(optionflags[togglestrings[i].icode]){
                            sprintf(temp,"+%s",togglestrings[i].code);
                            strcat(cbuf,temp);
                        }
                        else if(defaultflags[togglestrings[i].icode]){
                            sprintf(temp,"-%s",togglestrings[i].code);
                            strcat(cbuf,temp);
                        }
                    }
                    else if(togglestrings[i].group == 2)
                    {
                        if(option2flag & togglestrings[i].icode)
                        {
                            sprintf(temp,"+%s",togglestrings[i].code);
                            strcat(cbuf,temp);
                        }
                    }
                    else if(togglestrings[i].group == 1){
                        if(optionflag & togglestrings[i].icode)
                        {
                        sprintf(temp,"+%s",togglestrings[i].code);
                        strcat(cbuf,temp);
                        }
                    }
                    i++;
                }
                for(i = 0; i < NTESTFLAGS; i++){
                    if(testflags[i]){
                        sprintf(temp,"\ntestflag %d",i);
                        strcat(cbuf,temp);
                    }
                }
            }
            else
            {
                pcflag = 0;
                if(optionflag & FIXATION_CHECK)
                    ival |= CHECK_CONJUGATE;
                if(optionflag & LEFT_FIXATION_CHECK)
                    ival |= CHECK_LEFT;
                if(optionflag & RIGHT_FIXATION_CHECK)
                    ival |= CHECK_RIGHT;
                if(optionflag & VERGENCE_CHECK)
                    ival |= VERGENCE_CHECK_IO;
                if(optionflag & STORE_WURTZ_BIT)
                    ival |= STORE_WURTZ_DATA;
                if(optionflag & WURTZ_TRACK_BIT)
                    ival |= CONJUGATE_RAMP_BIT;
                if(optionflag & CLAMP_HOLD_BIT)
                    ival |= CLAMP_HOLD;
                if(optionflag & CLAMP_EXPT_BIT)
                    ival |= CLAMP_OPTION;
                if(optionflag & RAMP_HOLD_BIT)
                    ival |= RAMP_HOLD;
                if(optionflag & RAMP_EXPT_BIT)
                    ival |= RAMP_OPTION;
                if(optionflag & SHOW_CONJUG_BIT)
                    ival |= SEND_CONJPOS;
                if(optionflag & FRAME_ONLY_BIT)
                    ival |= FRAME_TEST_BIT;
                if(mode & MOVED_STIMULUS)
                    ival |= STIMULUS_CHANGED;
                if(option2flag & FLASH_BIT)
                    ival |= STIMULUS_FLASHING;
                if(option2flag & AFC)
                    ival |= AFC_SET;
                if(optionflag & BACKGROUND_IN_PREPERIOD)
                    pcflag |= PREBACK_BIT;
                if(optionflags[REVERSE_CORRELATE])
                    pcflag |= RC_BIT;
                if(expt.flag & TIMES_EXPT2)
                    pcflag |= TIMES_EXPT2;
                if(optionflags[BINOCULAR_FIXPOINT])
                    pcflag |= BINOC_FIX_BIT;
                if(optionflags[MICROSTIM])
                    pcflag |= MICROSTIM_BIT;
                if(optionflag & CONTRAST_REVERSE_BIT)
                    pcflag |= CONTRAST_REVERSE_BW;
                if(optionflags[RANDOM_PHASE])
                    pcflag |= RANDOM_PHASE_BW;
                if(optionflags[RUN_SEQUENCE])
                    pcflag |= RUN_SEQUENCE_BW;
                if(!(optionflag & STIM_IN_WURTZ_BIT)){
                    pcflag |= STIM_BETWEEN_TRIALS_BW;
                }
                if(optionflags[STORE_LFP])
                    pcflag |= STORE_LFP_BW;
                
                sprintf(cbuf,"%s%s%d,%d",scode,temp,ival,pcflag);
                
            }
            break;
        case STIMULUS_OPTIONS:/* only needed for BW */
            if(flag == TO_BW)
            {
                ival = expt.st->flag;
                if(optionflag & SQUARE_RDS)
                    ival |= STIMULUS_IS_SQUARE;
                sprintf(cbuf,"%s%s%d",scode,temp,ival);
            }
            else
                ret=-1;
            break;
        case STIMULUS_DURATION_CODE:
            sprintf(cbuf,"%s%.3f",scode, 
                    (expt.st->nframes)/expt.mon->framerate);
            break;
        case TIMEOUT_CODE:
            if(flag == TO_BW)
                sprintf(cbuf,"%s%.0f",scode,
                        (1000 * expt.st->fix.timeout));
            else
                sprintf(cbuf,"%s=%.3f",scode,expt.st->fix.timeout);
            break;
        case WRONG_TIMEOUT_CODE:
            sprintf(cbuf,"%s%s%.3f",scode,temp,afc_s.wrongtimeout);
            break;
        case SHORT_PREM_CODE:
            if(flag != TO_BW)
                sprintf(cbuf,"%s%s%.3f",scode,temp,expt.st->fix.minprem);
            else
                sprintf(cbuf,"%s%0f",scode,(10000 * expt.st->fix.minprem));
            break;
        case WURTZ_RT_CODE:
            if(flag != TO_BW)
                sprintf(cbuf,"%s%s%.2f",scode,temp,expt.st->fix.rt);
            else
                sprintf(cbuf,"%s%.3f",scode, 10000 * expt.st->fix.rt);
            break;
        case CHANNEL_CODE:
            cbuf[0] = 0;
            for(i = 10; i < expt.bwptr->nchans; i++)
            {
                sprintf(temp,"ch%d%c,%2s%.2f,%.2s%d\n",i, 
                        (expt.bwptr->cflag & (1<<i)) ? '+' : '-',
                        channel_strings[EYESCALE_CODE],expt.bwptr->fsd[i],
                        channel_strings[EYECOLOR_CODE],expt.bwptr->colors[i]);
                strcat(cbuf,temp);
            }
            /*
             for(j = expt.bwptr->nchans; j < ; j++)
             {
             sprintf(temp,"ch%d%c\n",j, 
             (expt.bwptr->cflag & (1<<j)) ? '+' : '-');
             strcat(cbuf,temp);
             }
             cbuf[strlen(cbuf)-1] = 0;
             */
            break;
        case ELECTRODE_DEPTH:
            val = ExptProperty(ex, code);
            sprintf(cbuf,"%s%s%.3f",scode,temp,val);
            break;
        case EXPTYPE_CODE:
            i = expt.mode;
        case EXPTYPE_CODE2:
        case EXPTYPE_CODE3:
            if(code == EXPTYPE_CODE2)
                i = expt.type2;
            if(code == EXPTYPE_CODE3)
                i = expt.type3;
            sprintf(cbuf,"%s%s%s",scode,temp,serial_strings[i]);
            break;
        case COVARIATE:
            i = expt.vals[COVARIATE];
            sprintf(cbuf,"%s%s%s",scode,temp,serial_strings[i]);
            break;
        case HIGHXTYPE:
            i = expt.vals[expt.hightype];
            if (i > 0 && i < expt.maxcode)
                sprintf(cbuf,"%s%s%s",scode,temp,serial_strings[i]);
            else
                sprintf(cbuf,"%s%s0",scode,temp);
            break;
#if defined(USING_SOLENOID)
        case REWARD_SIZE:
            if(flag != TO_BW)
                sprintf(cbuf,"%s%s%d",scode,temp,expt.st->fix.rwsize);
            else
                sprintf(cbuf,"%s%.0f",scode, floor((float)(expt.st->fix.rwsize)/rwduration));
#elseif !defined(OXFORD)
        case REWARD_SIZE:
            sprintf(cbuf,"%s%s%.2f",scode,temp,expt.st->fix.rwsize);
            break;
#endif
            /*
             * if training with no mirrors, then there is no reversal of the image. 
             * this means change in fix x cause brainwave to move the fix box the
             * wrong way. Deal with this by reversing the fixation X we tell BW about
             */
        case FIXPOS_X:
            val = GetProperty(&expt,expt.st,code);
            if(flag != TO_FILE && optionflags[NO_MIRRORS])
                sprintf(cbuf,"%s%s%.4f",scode,temp,-val);
            else
                sprintf(cbuf,"%s%s%.4f",scode,temp,val);
            break;
        case PENETRATION_TEXT:
            sprintf(cbuf,"%s%s%d %.1f,%.1f",scode,temp,expt.ipen,
                    (expt.vals[PENXPOS]),(expt.vals[PENYPOS]));
            break;
        case EXPT2_INCR:
            if(flag != TO_BW)
                sprintf(cbuf,"%s%s%.4f%s",scode,temp,expt.incr2,(expt.flag & LOGINCR2) ? "log" : "lin");
            else
                sprintf(cbuf,"%s%s%.4f",scode,temp,expt.incr2);
            break;
        case EXPT3_INCR:
            if(flag != TO_BW)
                sprintf(cbuf,"%s%s%.4f%s",scode,temp,expt.incr3,(expt.flag & LOGINCR2) ? "log" : "lin");
            else
                sprintf(cbuf,"%s%s%.4f",scode,temp,expt.incr3);
            break;
        case EXPT_INCR:
            if(flag != TO_BW)
                sprintf(cbuf,"%s%s%.4f%s",scode,temp,expt.incr,(expt.flag & LOGINCR) ? "log" : "lin");
            else
                sprintf(cbuf,"%s%s%.4f",scode,temp,expt.incr);
            break;
        case DISP_P:
            /*
             * DISP_P is represtented in radians. Save it in degrees to the file
             */
            val = StimulusProperty(st,code);
            if(flag != TO_BW){
                val = val * 180/M_PI;
                sprintf(cbuf,"%s%s%.1f",scode,temp,val);
            }
            else
                sprintf(cbuf,"%s%s%.4frad",scode,temp,val);
            break;
        case GAMMAVAL_CODE:
            val = ExptProperty(ex, code);
            sprintf(cbuf,"%s%s%.*f %.4f",scode,temp,nfplaces[code],val,expt.mon->truegamma);
            break;
        case TARGET2_POS:
            if(afc_s.newdirs)
                sprintf(cbuf,"%s%s%.4f %.4f",scode,temp, afc_s.sacval[4], afc_s.sacval[5]);
            else
                sprintf(cbuf,"%s%s%.4f %.4f",scode,temp, -afc_s.sacval[0], -afc_s.sacval[1]);
            break;
        case HSACCADE_VALUE:
        case VSACCADE_VALUE:
            i = (code == HSACCADE_VALUE) ? 0 : 1;
            if(flag != TO_BW)
                sprintf(cbuf,"%s%s%.4f",scode,temp, afc_s.abssac[i]);
            else{
                if(i==0 && optionflags[NO_MIRRORS])
                    sprintf(cbuf,"%s%s%.4f %.4f %d",scode,temp, -afc_s.sacval[i], afc_s.sacval[i+2],afc_s.sign);
                else
                    sprintf(cbuf,"%s%s%.4f %.4f %d",scode,temp, afc_s.sacval[i], afc_s.sacval[i+2], afc_s.sign);
            }
            if(rewardall)
                strcat(cbuf,"!");
            break;
        case PURSUIT_INCREMENT:
            val = GetProperty(ex, ex->st, code) * pursuedir;
            sprintf(cbuf,"%s%s%.*f",scode,temp,nfplaces[code],val);
            break;
        case TRIGGER_LEVEL1:
            if(flag != TO_BW)
                return(-1);
        default:
            val = ExptProperty(ex, code);
            if(!(val > NOTSET))
                val = StimulusProperty(st,code);
            if(val > NOTSET)
            {
                if(fabs(val) < 0.0001 && fabs(val) > 0 && nfplaces[code] < 6)
                    sprintf(cbuf,"%s%s%.6f",scode,temp,val);
                else if(fabs(val) < 0.1 && fabs(val) > 0 && nfplaces[code] < 4)
                    sprintf(cbuf,"%s%s%.4f",scode,temp,val);
                else
                    sprintf(cbuf,"%s%s%.*f",scode,temp,nfplaces[code],val);
            }
            else
            {
                sprintf(cbuf,"");
                ret = -1;
            }
            break;
    }
    if (flag == TO_GUI) {
        strcat(cbuf,"\n");
    }
    return(ret);
}

int confirm_no(char *s, *help)
{
    return(1);   
}

int confirm_yes(char *s, *help)
{
    return(1);   
}

void runexpt(int w, Stimulus *st, int *cbs)
{
    int oldflag = optionflag,old2flag = option2flag;
    if(expt.st->mode & EXPTPENDING)
    {
        expt_over(CANCEL_EXPT);
        return;
    }
    if(option2flag & PSYCHOPHYSICS_BIT)
    {
        psychclear(expt.plot,1);
    }
    optionflag |= GO_BIT;
    monkeypress = 0;
    setextras();
    if(optionflag & STORE_WURTZ_BIT && !testflags[PLAYING_EXPT]) /* make sure its not silly */
    {
        oldflag = optionflag;
        old2flag = option2flag;
        if(optionflag & SEARCH_MODE_BIT && confirm_no("Sure You Don't want random Order?",NULL))
            optionflag &= (~SEARCH_MODE_BIT);
        if(option2flag & EXPT_INTERACTIVE && confirm_no("Sure You want interactive Expt?",NULL))
            option2flag &= (~EXPT_INTERACTIVE);
        if(SACCREQD(afc_s) && !(option2flag & AFC) && confirm_no("Sure You Don't want AFC?",NULL))
            option2flag |= (AFC);
        if(optionflags[FAST_SEQUENCE] && expt.stimpertrial > 1){
            acknowledge("You have Nper > 1? (Fast Seq is ON)",NULL);
        }
        if(!(optionflag & FIXATION_CHECK) && confirm_no("Sure You Don't want Fixation check?",NULL))
            optionflag |= FIXATION_CHECK;
        if(optionflag != oldflag || option2flag != old2flag) /* was a mistake */
        {
            statusline("Expt Not run");
            return;
        }
    }
    if(expt.type2 == BACKGROUND_IMAGE || expt.type2 == BACKGROUND_MOVIE ||
       expt.mode == BACKGROUND_IMAGE || expt.mode == BACKGROUND_MOVIE)
    {
        LoadBackgrounds();
    }
    if(expt.mode == END_OFFSET || expt.mode == SIDE_OFFSET && !(rdspair(expt.st)))
        SetProperty(&expt,expt.st,ORIENTATION,expt.rf->angle);
//    ListExpStims(NULL);
    expt.plot->trialctr = 0;
    expt.laststim = -1;
    expt.st->mode |= EXPTPENDING;
    expt.st->fix.state = FIX_IS_OFF;
    expt.st->fixcolor = expt.st->fix.offcolor;
    stimno = NEW_EXPT;
    statusline("Expt Starting..");
    SaveExptFile("./leaneb.stm",1);
    afc_s.lasttrial = -(BAD_TRIAL);
    firststimno = stimno;
    if(!option2flag & PSYCHOPHYSICS_BIT){
        SerialSignal(BW_IS_READY);
        CheckBW(BW_IS_READY,"Expt Start");
    }
    StopGo(GO);
    expt.st->mode |= EXPTPENDING;
    if(framebuf != NULL)
    {
        free(framebuf);
        framebuf = NULL;
    }
    if(seroutfile)
        fprintf(seroutfile,"#Start Expt at %d %sx%s %d%c%d (%d)\n",
                ufftime(&now),serial_strings[expt.mode],serial_strings[expt.type2],
                expt.nstim[0],(expt.flag & TIMES_EXPT2) ? 'x' : '+',expt.nstim[1],expt.nstim[4]);
    InitExpt();
}



char *SerialSend(int code)
{
    char *scode = valstrings[valstringindex[code]].code, cbuf[BUFSIZ];
    int i;
    
    cbuf[0] = 0;
    
    if(mode & NO_SERIAL_PORT)
        return(cbuf);
    if((i = MakeString(code, cbuf, &expt, expt.st,0)) >= 0)
        strcat(cbuf,"\n\0");
    switch(code)
    {
        case FIXPOS_XY:
            sprintf(cbuf,"%2s %.4f %.4f %.4f %.4f\n",serial_strings[FIXPOS_XY],
                    GetProperty(&expt,expt.st,FIXPOS_X),GetProperty(&expt,expt.st,FIXPOS_Y),GetProperty(&expt,expt.st,XPOS),GetProperty(&expt,expt.st,YPOS));
            SerialString(cbuf,0);
            break;
        case BACKGROUND_MOVIE:
            SerialSend(BACKGROUND_IMAGE); // ONly need # of first image
            break;
        case TEMPORAL_ORI_BANDWIDTH:
            SerialSend(DISTRIBUTION_WIDTH);
            break;
        case PHASE_AS_DISP:
            SerialString(cbuf,0);
            SerialSend(DISP_P);
            SerialSend(SF);
            break;
        case FP_MOVE_SIZE:
            SerialString(cbuf,0);
            SerialSend(FP_MOVE_DIR);
            break;
        case RELDISP:
            SerialSend(DISP_X);
            SerialSend(DISP_BACK);
            break;
        case RELVDISP:
            SerialSend(DISP_Y);
            break;
        case ORI_LEFT:
        case ORI_RIGHT:
            SerialSend(ORIENTATION);
            SerialSend(ORIENTATION_DIFF);
            break;
        case SF_LEFT:
        case SF_RIGHT:
            SerialSend(SF);
            SerialSend(SF_DIFF);
            break;
        case CONTRAST_RATIO:
        case CONTRAST_PAIRS:
        case CONTRAST_LEFT:
        case CONTRAST_RIGHT:
            SerialSend(SETCONTRAST);
            SerialSend(CONTRAST2);
            SerialSend(CONTRAST_DIFF);
            break;
        case HEIGHT_R:
        case HEIGHT_L:
            SerialSend(STIM_HEIGHT);
            SerialSend(LENGTH_DISP);
            break;
        case SIZE_R:
        case SIZE_L:
            SerialSend(STIM_HEIGHT);
            SerialSend(LENGTH_DISP);
        case WIDTH_R:
        case WIDTH_L:
            SerialSend(STIM_WIDTH);
            SerialSend(WIDTH_DISP);
            break;
        case STIM_SIZE:
            if(seroutfile)
                fprintf(seroutfile,"%s",cbuf);
            SerialSend(STIM_WIDTH);
            SerialSend(STIM_HEIGHT);
            break;
        case OPTION_CODE:
            if(seroutfile)
                fprintf(seroutfile,"PC");
            SerialString(cbuf,0);
            SerialSend(STIMULUS_OPTIONS);
            cbuf[0] = 0;
            if(seroutfile && (i = MakeString(code, cbuf, &expt, expt.st,TO_FILE)) >= 0)
                fprintf(seroutfile,"%s\n",cbuf);
            break;
        case STIMULUS_FLAG:
            cbuf[0] = 0;
            if(seroutfile && (i = MakeString(code, cbuf, &expt, expt.st,TO_FILE)) >= 0)
                fprintf(seroutfile,"%s\n",cbuf);
            break;
        case UFF_PREFIX:
            SerialString(cbuf,0);
            if(penlog)
                fprintf(penlog,"%s File %s\n",binocTimeString(),expt.bwptr->prefix);
            if(mode & UFF_FILE_OPEN){
                SerialSend(RF_DIMENSIONS);
                SerialSend(XPIXEL_CODE);
                SerialSend(YPIXEL_CODE);
                SerialSend(VIEWD_CODE);
            }
            break;
        case RF_ANGLE:
            SerialString(cbuf,0);
            SerialSend(RF_DIMENSIONS);
            break;
        case TWOCYL_DISP:
            SerialSend(DISP_X);
            SerialSend(DISP_BACK);
            SerialString(cbuf,0);
            break;
        case STIMULUS_TYPE_CODE:
            SerialString(cbuf,0);
            break;
        case CHANNEL_CODE:
            i = 0; //for debugger
        default:
            SerialString(cbuf,0);
            break;
    }
    return(cbuf);
}



/* 
 * InitExpt is called at the beginning of an experiment
 * buit AFTER any X-event generated by runexp() have been
 * processed. Doesn't need to wait for a monkeypress.
 * Screen Clear must be performed now, since
 * this is called after hthe display window has
 * been exposed. Attempts to clear before this leave the
 * Panels etc. in the back buffer.
 */
void InitExpt()
{
    int i,j,code;
    char cbuf[BUFSIZ*100],c,buf[BUFSIZ],rcnamebuf[BUFSIZ],*ts;
    float tval,val;
    time_t tstart;
    Thisstim *stp;
    
    
    trialctr = 0;
    stimno = 0;
    endbadctr = 0;
    freezeexpt = 0;
    expt.ncalc = expt.noverflow = 0;
    rcrpt = expt.vals[RC_REPEATS];
    
    if(fabs(afc_s.sign) < 0.9){
        afc_s.sign = (myrnd_i() & 2)-1;
    }
    j = 1;
    afc_s.jstairval = INTERLEAVE_EXPT_ZERO;
    afc_s.magid = expt.nstim[0]-1;
    afc_s.nmags = expt.nstim[0];
    afc_s.stairsign = 1;
    afc_s.signflipp = 0;
    if (optionflags[CHOICE_BY_ICON]){
        afc_s.signflipp = 0.5;
        afc_s.target_in_trial = 1;
    }
    else{
        afc_s.signflipp = 0;
        afc_s.target_in_trial = 0;
    }
    
    
    if(expt.mode == CORRELATION || expt.mode == DISTRIBUTION_CONC)
        afc_s.type = MAGONE_SIGNTWO;
    else if(expt.mode == ORI_BANDWIDTH){
        afc_s.type = MAGONE_SIGNTWO;
        afc_s.stairsign = -1;
    }
    else
        afc_s.type = ONEUP;
    mode &= (~BW_ERROR);
    HideCursor();

    if(!(mode & SERIAL_OK))
        MakeConnection(4);
    expt.cramp = expt.ramp;
    expt.expseed = 1;
    
    setextras();
    for(i = 0; i< expt.nstim[5]; i++)
        isset[i] = 0;
    setstimulusorder(1);
    stimdurn = 0;
    stimdursum = 0;
    /*
     * This list of properties is recorded fo the stimulus before an expt run
     * starts playing with it. expt.stimvals[i] can then be used to reset the 
     * stimulus property to whatever it was before the experiment started
     */
    for(code = 0; code < expt.lastsavecode; code++){
        i = valstrings[code].icode;
        switch(i){
            case SETZXOFF:
            case SETZYOFF:
                expt.vals[i] = GetProperty(&expt,expt.st,i);
            case DISP_X:
            case DEPTH_MOD:
            case STIM_WIDTH:
            case STIM_HEIGHT:
            case ORIENTATION:
            case SF:
            case SF2:
            case TF:
            case TF2:
            case SD_X:
            case SETCONTRAST:
            case CONTRAST2:
            case CORRELATION:
            case BACK_CORRELATION:
            case SEED_DELAY:
            case MODULATION_F:
            case CHANGE_SEED:
            case JVELOCITY:
            case NCOMPONENTS:
            case ORI_BANDWIDTH:
            case PLAID_RATIO:
            case ORIENTATION_DIFF:
                expt.stimvals[i] = GetProperty(&expt,expt.st,i);
                break;
            case TARGET_RATIO:
                expt.stimvals[i] = expt.vals[i];
                break;
        }
    }
    expt.fixpos[0] = expt.vals[FIXPOS_X];
    expt.fixpos[1] = expt.vals[FIXPOS_Y];
    expt.stimvals[CHANGE_SEED] = expt.vals[CHANGE_SEED];
    stimpos[0] = GetProperty(&expt,expt.st,XPOS);
    stimpos[1] = GetProperty(&expt,expt.st,YPOS);
    stimpos[2] = GetProperty(&expt,expt.st,ORTHOG_POS);
    stimpos[3] = GetProperty(&expt,expt.st,PARA_POS);
    expt.stimtype = expt.st->type;
    expt.stimvals[expt.mode] = GetProperty(&expt,expt.st,expt.mode);
    expt.stimvals[expt.type2] = GetProperty(&expt,expt.st,expt.type2);
    expt.stimvals[PLC_MAG] = expt.vals[PLC_MAG];
    expt.stflag = expt.st->flag;
    reset_afc_counters();
    if(mode & SERIAL_OK)
    {
        if(!(optionflag & FRAME_ONLY_BIT))
        {
            for(i = 0; i < expt.lastserialcode; i++)
            {
                code = valstrings[i].icode;
                if(codesend[code] <= SEND_EXPT)
                    SerialSend(code);
                else if(codesend[code] == SEND_EXPT_NONZERO && expt.vals[code] != 0)
                    SerialSend(code);
                if(codesend[code] == SEND_GRATING2 && expt.st->type == STIM_GRATING2)
                    SerialSend(code);
            }
            if(expt.st->next != NULL && expt.st->next->type != STIM_NONE){
                sprintf(cbuf,"%s=back=%s,%s=%.2f,%s=%.2f,%s=%.2f,%s=%.2f,%s=%.2f,%s=%.2f,%s=%.2f",
                        serial_strings[UFF_COMMENT],
                        stimulus_names[expt.st->next->type],
                        serial_strings[STIM_WIDTH],
                        StimulusProperty(expt.st->next, STIM_WIDTH),
                        serial_strings[STIM_HEIGHT],
                        StimulusProperty(expt.st->next, STIM_HEIGHT),
                        serial_strings[CORRELATION],
                        StimulusProperty(expt.st->next, CORRELATION),
                        serial_strings[XPOS],
                        StimulusProperty(expt.st->next, XPOS),
                        serial_strings[YPOS],
                        StimulusProperty(expt.st->next, YPOS),
                        serial_strings[JVELOCITY],
                        StimulusProperty(expt.st->next, JVELOCITY),
                        serial_strings[DISP_X],
                        StimulusProperty(expt.st->next, DISP_X));
                sprintf(buf,",%2s=%.2f,%2s=%.1f", serial_strings[SETCONTRAST],
                        StimulusProperty(expt.st->next, SETCONTRAST),
                        serial_strings[ORIENTATION],StimulusProperty(expt.st->next, ORIENTATION));
                
                strcat(cbuf,buf);
                strcat(cbuf,"\n");
            }
            else
                sprintf(cbuf,"%s=noback\n",serial_strings[UFF_COMMENT]);
            SerialString(cbuf,0);
        }
        SerialSend(ELECTRODE_DEPTH);
        SerialSend(USENEWDIRS);
        if(expt.st->imprefix != NULL){
            sprintf(cbuf,"imprefix=%s\n",expt.st->imprefix);
            SerialString(cbuf,0);
        }
        
    }
    if(option2flag & INTERLEAVE_VERGENCE)
    {
        if(option2flag & ALT_PROBE_BIT)
        {
            tval = expt.cj;
            expt.cj -= expt.ramp;
            SerialSend(STATIC_CONJUGATE);
            expt.cj = tval;
        }
        else
        {
            tval = expt.sv;
            expt.sv -= expt.ramp;
            SerialSend(STATIC_VERGENCE);
            expt.sv=tval;
        }
    }
    /* clear input buffer first */
    expt.nextstim = stimorder[0];
    lastbutton = -1000;
    if(option2flag & INTERLEAVE_ONE_MONOCULAR)
    {
        optionflag &= (~MONOCULAR_MODE);
        setoption();
        clear_display(1);
        SerialSend(OPTION_CODE);
    }
    cbuf[0] = 0;
    MakeString(OPTION_CODE,cbuf, &expt, expt.st, TO_FILE);
    sprintf(buf,"%sop=%s\n",serial_strings[MANUAL_TDR],cbuf);
    SerialString(buf,0);
    cbuf[0] = 0;
    MakeString(EXPTSTRING,cbuf, &expt, expt.st, TO_FILE);
    sprintf(buf,"%set=%s,%s=%0f\n",serial_strings[MANUAL_TDR],cbuf,serial_strings[FAST_SEQUENCE_RPT],expt.vals[FAST_SEQUENCE_RPT]);
    SerialString(buf,0);
    
    /*
     if(expt.mode == MONOCULARITY_EXPT)
     {
     val = expval[i2expi(expt.flag, expt.nstim, expt.nextstim,0)];
     SetExptProperty(&expt,expt.st, expt.mode, val);
     }
     if(expt.type2 == MONOCULARITY_EXPT)
     {
     val = expval[i2expi(expt.flag, expt.nstim, expt.nextstim,1)];
     SetExptProperty(&expt,expt.st, expt.type2, val);
     }
     */
    
    while((c = ReadSerial(0) != MYEOF))
        GotChar(c);
    if(!(optionflag & FRAME_ONLY_BIT) || (optionflag & WAIT_FOR_BW_BIT))
        SerialSignal(START_EXPT);
    if(seroutfile){
        fprintf(seroutfile,"Expt %d\n",expt.nstim[5] * expt.nreps);
        fprintf(seroutfile,"\nStimulus %s\n",DescribeStim(expt.st));
    }
    if(psychfile){
        Stim2PsychFile();
        gettimeofday(&now,NULL);
        fprintf(psychfile,"R4 %s=%.2f %s=%.2f sn=0",
                serial_strings[COVARY_XPOS],afc_s.ccvar, 
                serial_strings[TARGET_RATIO],expt.vals[TARGET_RATIO]);
        tval = RunTime();
        ts = binocTimeString();
        ts[3] = '.';
        ts[6] = 0;
        fprintf(psychfile," %ld %s %d",now.tv_sec,ts,expt.nstim[5] * expt.nreps);
        fprintf(psychfile," %s=%.2f %s=%.2f x=0 x=0 x=0 x=0\n",serial_strings[XPOS],GetProperty(&expt,expt.st,XPOS),serial_strings[YPOS],GetProperty(&expt,expt.st,YPOS));
    }
    if(psychfilelog){
        tstart = time(NULL);
        fprintf(psychfilelog,"Expt at %s by binoc Version %s\n",nonewline(ctime(&tstart)),VERSION_NUMBER);
        fprintf(psychfilelog,"\nStimulus %s\n",DescribeStim(expt.st));
        for(j = 0; j < expt.totalcodes; j++){
            cbuf[0] = 0;
            if((i = MakeString(valstrings[j].icode, cbuf, &expt, expt.st,0)) >= 0)
                fprintf(psychfilelog,"%s\n",cbuf);
        }
        fflush(psychfilelog);
    }
    if(cmdhistory){
        fprintf(cmdhistory,"%sExpt %s x %s\n",
                optionflag & STORE_WURTZ_BIT ? "Save": "NoSave",
                serial_strings[expt.mode],serial_strings[expt.type2]);
    }
    
    if(expt.st->type == STIM_IMAGE && 
       (expt.mode == ORI_BANDWIDTH || expt.type2 == ORI_BANDWIDTH))
        expt.st->immode = IMAGEMODE_ORBW;
    clearcolor = dogamma(expt.vals[SETCLEARCOLOR]);
    stimstate = INTERTRIAL;
    stimulus_is_prepared = 0;
    /* 
     * make sure expt starts with even value of seed, for STC expts
     */
    if(expt.st->left->baseseed & 0x1)
        expt.st->left->baseseed++;
    if(optionflags[CHECK_FRAMECOUNTS] == 0)
        optionflags[CHECK_FRAMECOUNTS] = 1;
    
    
    for(i = 0; i < MAXSTIM; i++)
        unrepeatn[i] = 0;
    SetTargets();
    PrepareExptStim(1,6);
    stp = getexpval(0);
    if(optionflags[FAST_SEQUENCE]){
        /*
         * Figure out stim properites for each fast stim id
         * then write a string to the serial output, so that the
         * codes can be decoded later. Call PrepareExptStim again to 
         * reset the sequence. (Need to call first also, to make sure
         * nfast and fastextras is set correctly
         */
        for(i = 0; i < expt.nfast + expt.fastextras; i++)
            SetFrameStim(i, i, expt.fastincr, stp, expt.fastnstim);
        MakeString(EXPTVALS,cbuf, &expt, expt.st, TO_FILE);
        SerialString(cbuf,0);
        if(option2flag & FLASH_BIT)
            sprintf(buf,"%2s=fl\n",serial_strings[STIMCHANGE_CODE]);
        stimulus_is_prepared = 0;
        PrepareExptStim(1,6);
    }

    else{
        expt.vals[BLANK_P] = 0;
        expt.vals[UNCORR_P] = 0;
    }
    clear_display(1);
    if(option2flag & IFC){
        for(i = 0; i < 2 * expt.nstim[3] * expt.nreps; i++){
            printf("%d,",stimorder[i]);
        }
        printf("\n");
    }
    
    if(expt.st->type == STIM_GRATINGN){
        SetGratingFrequencies(expt.st);
        if(seroutfile){
            fprintf(seroutfile,"#freqs:");
            for(i = 0; i < expt.st->nfreqs; i++)
                fprintf(seroutfile," %.3f",expt.st->freqs[i]);
            fprintf(seroutfile,"\n");
        }
    }
    
    //	if(optionflags[RUN_SEQUENCE]){
    // Don't need this any more now we use spike2
    if(datprefix && expname && !(option2flag & PSYCHOPHYSICS_BIT && pcmode != SPIKE2)
       && optionflags[ONLINE_DATA]){
        do{
            if(expt.type2 != EXPTYPE_NONE)
                sprintf(cbuf,"%s/%s.%sX%s.rc%d",expname,stimulus_names[expt.st->type],serial_strings[expt.mode],serial_strings[expt.type2],rcctr++);
            else
                sprintf(cbuf,"%s/%s.%s.rc%d",expname,stimulus_names[expt.st->type],serial_strings[expt.mode],rcctr++);
            rcname = myscopy(rcname,cbuf);
            sprintf(cbuf,"%s/%s",datprefix,rcname);
            if((rcfd = fopen(cbuf,"r")) != NULL)
                fclose(rcfd);
        }while(rcfd != NULL);
        onlinedat = 1;
        if((rcfd = fopen(cbuf,"w")) == NULL){
            sprintf(buf,"Can't write to %s",cbuf);
            acknowledge(buf,NULL);
            sprintf(cbuf,"./%s.%sX%s.rc%d",stimulus_names[expt.st->type],serial_strings[expt.mode],serial_strings[expt.type2],rcctr++);
            rcfd = fopen(cbuf,"w");
            
        }
        if(rcfd){
            if(!optionflags[REVERSE_CORRELATE] && expt.type3 != EXPTYPE_NONE)
                frameparams[0] = expt.type3;
            else if(expt.type2 == CONTRAST_PAIRS)
                frameparams[0] = SETCONTRAST;
            else
                frameparams[0] = XPOS;
            fprintf(rcfd,"%s %s %s %s %s\n",serial_strings[expt.mode],
                    serial_strings[expt.type2],serial_strings[frameparams[0]],
                    serial_strings[(expt.type3 == EXPTYPE_NONE) ? frameparams[1] : expt.type3],
                    serial_strings[frameparams[2]]);
            fprintf(rcfd,"n5 %d\n",expt.nstim[5]);
            cbuf[0] = 0;
            for(i = 0; i < expt.totalcodes; i++){
                switch(valstrings[i].icode){
                    case EXPTYPE_CODE2:
                    case EXPTYPE_CODE3:
                    case EXPTYPE_CODE:
                    case NFRAMES_CODE:
                    case MONOCULARITY_EXPT:
                    case TF:
                    case SF:
                    case SF2:
                    case STIM_WIDTH:
                    case STIM_HEIGHT:
                    case CONTRAST_RATIO:
                    case FRAMERATE_CODE:
                    case SET_SEEDLOOP:
                    case OPTION_CODE:
                    case NCOMPONENTS:
                    case STIMULUS_MODE:
                        cbuf[0] = ' ';
                        cbuf[1] = 0;
                        MakeString(valstrings[i].icode,cbuf, &expt, expt.st, TO_FILE);
                        fprintf(rcfd,"%s\n",cbuf);
                        break;
                }
            }
            if(optionflags[RUN_SEQUENCE]){
                fprintf(rcfd,"%s %d\n",serial_strings[EXPT_STIMPERTRIAL],expt.stimpertrial);
            }
            else if(optionflags[FAST_SEQUENCE])
                fprintf(rcfd,"%s %d\n",serial_strings[EXPT_STIMPERTRIAL],expt.st->nframes);
            
            fflush(rcfd);
            /*
             * nfsnobody cannot do this - not a member of the group
             *	  sprintf(buf,"chgrp bgcdiv %s",rcname);
             *  system(buf);
             */
        }
    }
    else
        onlinedat = 0;
    gettimeofday(&zeroframetime, NULL);

    /*
     * force saving of an image of the last frame in the last stim of the 
     * experiment, for cross checking.
     */
    if(expt.mode == DISTRIBUTION_CONC || expt.type2 == DISTRIBUTION_CONC
       || (expt.mode == DISP_X && expt.nstim[0] == 1) && expt.st->type != STIM_IMAGE){
        if(testflags[SAVE_IMAGES] == 0)
            testflags[SAVE_IMAGES] = 2;
    }
    if(psychlog){
        tstart = time(NULL);
        fprintf(psychlog,"\nExpt Start at %s",ctime(&tstart));
    }
    ifcsum = 0;
    framesum = 0;
    realframesum = 0;
    if(expt.st->left->ptr->velocity > 0.00001 && expt.st->type == STIM_CYLINDER){
        oldvelocity = expt.st->left->ptr->velocity;
        olddisp = expt.st->disp;
        stimflag = expt.st->flag;
    }
    else if(expt.st->left->ptr->velocity > 0.00001 && expt.st->type == STIM_RDS){
        oldvelocity = expt.st->posinc;
        olddisp = expt.st->disp;
        stimflag = expt.st->flag;
    }
    else if(seroutfile){
        fprintf(seroutfile,"#Velocity 0 at Start Expt, was %.2f\n",oldvelocity);
    }
    if(expt.mode == FIXPOS_X || expt.type2 == FIXPOS_Y || expt.mode == FIXPOS_Y)
    {
        sprintf(cbuf,"#%s\n",EyeCoilString());
        SerialString(cbuf,0);
    }
    SendAllToGui();
    notify("\nEXPTSTART\n");

}

void CheckPsychVal(Thisstim *stp)
{
    double val, expt1crit;
    
    if (optionflags[EXPTMEAN_FOR_PSYCH])
        expt1crit = expt.mean;
    else
        expt1crit = 0;
    
    if (expt.mode == CONTRAST_DIFF){
        if ((val = StimulusProperty(expt.st,ORIENTATION_DIFF)) < 0){
            stp->vals[EXP_PSYCHVAL] = stp->vals[0]-expt1crit;
        }
        else {
            stp->vals[EXP_PSYCHVAL] = -(stp->vals[0]-expt1crit);
        }
        if (fabs(stp->vals[EXP_PSYCHVAL]/expt.incr) < 0.001)
            stp->vals[EXP_PSYCHVAL] = 0;
    }
}

Thisstim *getexpval(int stimi)
{
    int i = stimi & (~ORDER_BITS),j;
    float vals[2],temp,vtemp;
    static Thisstim stimret;
    int rnd;
    double drnd,val,expt1crit = 0;
    
    stimret.a = stimret.b = 0;
    
    /* 
     *   i = extras + (stim1 * stim2) if xExpt2 or
     *   i = extras + (stim1 + stim2) if +Expt2 or
     *  if expt3 is set (e.g. ustim), this can be doubled.
     *
     * i%expt.nstim[3] gives the stimulus index for expts 1,2.
     * i/expt.nstim[3] give the level for experiment 3.
     */
    
    if(stimi & STIMULUS_EXTRA_EXPT3){
        stimret.stimno[2] = i/expt.nstim[3];
        stimret.vals[EXP_THIRD] = expt.exp3vals[stimret.stimno[2]];
    }
    else{
        stimret.stimno[2] = 0;
        stimret.vals[EXP_THIRD] = expt.exp3vals[0];
    }
    
    if (optionflags[EXPTMEAN_FOR_PSYCH])
        expt1crit = expt.mean;
    else
        expt1crit = 0;
    i = i % expt.nstim[3];
    if(i < expt.nstim[2])
        stimret.interleave = 1;
    else
        stimret.interleave = 0;
    
    if(expt.flag & TIMES_EXPT2 && expt.nstim[1] > 0  && expt.type2 != EXPTYPE_NONE)
    {
        j = (i - expt.nstim[2])%expt.nstim[0];
        stimret.stimno[0] = j;
        stimret.vals[0] = expval[j + expt.nstim[2]];
        j = (i - expt.nstim[2])/expt.nstim[0];
        stimret.vals[1] = expval[j+expt.nstim[2]+expt.nstim[0]];
        
        if(optionflags[RANDOM_EXPT2] && expt.type2 == CONTRAST_RATIO
           && (rnd = myrnd_i()) & 1 && stimret.vals[1] > 0)
            stimret.vals[1] = 1/stimret.vals[1];
        
        stimret.vals[EXP_PSYCHVAL] = 0;
        stimret.stimno[1] = j;
        stimret.expno = EXP_FIRST;
        stimret.vals[EXP_PSYCHVAL] = stimret.vals[0]-expt1crit;
        
        if(optionflags[RANDOM_EXPT2] && expt.type2 == DELAY && stimret.vals[1] != 0){
            if((rnd = lrand48() & 1) > 0)
                stimret.vals[EXP_PSYCHVAL] = -stimret.vals[EXP_PSYCHVAL];
        }
        stimret.vals[SIGNAL_STRENGTH] = fabs(stimret.vals[EXP_PSYCHVAL]);
        
        /*
         * When Correlation is varying, the reward is based on value of second expt
         * (usually disparity). This is shown equally at two values, so reward can
         * still be determined by the notional disparty even when correlation = 0.
         * when interleave a 0 correlation, care may be required.....
         */
        if(expt.mode == CORRELATION || expt.mode == DISTRIBUTION_CONC){
            stimret.vals[EXP_PSYCHVAL] = stimret.vals[1];
            if(expt.st->next != NULL && 
               expt.st->next->type != STIM_NONE && expt.st->next->disp != 0)
            {
                temp = stimret.vals[1]; /* the disparity */
                vtemp = GetProperty(&expt,expt.st->next,DISP_X);
                stimret.vals[EXP_PSYCHVAL] = temp - vtemp;
            }
            if(expt.type2 == ORIENTATION)
                stimret.vals[EXP_PSYCHVAL] = stimret.vals[1] - expt.mean2;
            stimret.vals[SIGNAL_STRENGTH] = stimret.vals[0];
            
            /*
             * for stimulus with 0 signal, randomize reward, but allow
             * the proportion of up/down trials rewarded to be controlled
             * so that biases can be trained out.
             */
            if(stimret.vals[0] == 0 || stimret.vals[0] == INTERLEAVE_EXPT_ZERO){
                if(afc_s.loopstate == CORRECTION_LOOP)
                    stimret.vals[EXP_PSYCHVAL] = afc_s.jlaststairval;
                else if((drnd = mydrand()) <= afc_s.proportion)
                    stimret.vals[EXP_PSYCHVAL] = -1; 
                else
                    stimret.vals[EXP_PSYCHVAL] = 1;
                afc_s.jsignval = 0;
                stimret.vals[SIGNAL_STRENGTH] = 0;
            }
        }
        if(expt.mode == ORIENTATION && expt.type2 == ORI_BANDWIDTH){
            if(expval[expt.nstim[0] + expt.nstim[2]]  < 0){
                if(stimret.vals[1] < 0)
                    stimret.vals[EXP_PSYCHVAL] = -1; 
                else
                    stimret.vals[EXP_PSYCHVAL] = +1;
            }
            else
                stimret.vals[EXP_PSYCHVAL] = stimret.vals[0] - expt.mean;
            /*
             * For broadband stimuli (defined by bw > 100) don't necessarily reward 50/50
             * but allow this to be set with afc-s.proportion, for training bad monkeys.
             */
            if(stimret.vals[1] > MAXORBW){
                if(afc_s.loopstate == CORRECTION_LOOP)
                    stimret.vals[EXP_PSYCHVAL] = afc_s.jlaststairval;
                else if((drnd = mydrand()) <= afc_s.proportion)
                    stimret.vals[EXP_PSYCHVAL] = -1; 
                else
                    stimret.vals[EXP_PSYCHVAL] = 1;
                afc_s.jsignval = 0;
                stimret.vals[SIGNAL_STRENGTH] = 0;
            }
            else
                stimret.vals[SIGNAL_STRENGTH] = bw2cv(stimret.vals[1]);
        }
        if(expt.mode == ORI_BANDWIDTH && expt.type2 == ORIENTATION)
            stimret.vals[EXP_PSYCHVAL] = stimret.vals[1] - expt.mean2;
    }
    else if(expt.flag & ADD_EXPT2 && expt.type2 != EXPTYPE_NONE)
    {
        if(i >= expt.nstim[2] + expt.nstim[0])
        {
            stimret.vals[0] = expt.mean;
            stimret.stimno[0] = 0;
            stimret.vals[1] = expval[i];
            if(expt.mode == expt.type2){
                stimret.vals[0] = expval[i];
            }
            stimret.stimno[1] = i- (expt.nstim[2] + expt.nstim[0]);
            stimret.expno = EXP_SECOND;
            stimret.vals[EXP_PSYCHVAL] = stimret.vals[1];
            /* 
             * sometimes want to interleave some stimuli with a signal
             * but reward at random. E.G. to see if there is a systematic
             * response to Anticorrelated stimuli
             */
            if(optionflags[RANDOM_EXPT2] && !(option2flag & PSYCHOPHYSICS_BIT))
                stimret.vals[EXP_PSYCHVAL] = 0;
            else if(optionflags[REVERSE_EXPT2])
                stimret.vals[EXP_PSYCHVAL] = -stimret.vals[EXP_PSYCHVAL];
        }
        else
        {
            stimret.vals[0] = expval[i];
            stimret.stimno[0] = i;
            stimret.vals[1] = expt.mean2;
            stimret.stimno[1] = 0;
            stimret.expno = EXP_FIRST;
            stimret.vals[EXP_PSYCHVAL] = stimret.vals[0];
        }
    }
    else // just expt1 is set
    {
        stimret.stimno[1] = 0;
        stimret.vals[0] = stimret.vals[1] = expval[i];
        stimret.expno = ONLY_ONE_EXPT;
        if((expt.mode == DISP_X || expt.mode == CORRELATION) 
           && expt.st->next != NULL && 
           expt.st->next->type != STIM_NONE && expt.st->next->disp != 0)
        {
            temp = stimret.vals[0]; /* the disparity */
            vtemp = GetProperty(&expt,expt.st->next,DISP_X);
            stimret.vals[EXP_PSYCHVAL] = temp - vtemp;
        }
        else if(expt.mode == PLAID_RATIO){
            if (fabs(stimret.vals[0]) < 0.0001){
                stimret.vals[0] = 0;
            }
            stimret.vals[EXP_PSYCHVAL] = stimret.vals[0] - expt.mean;
            if (fabs(stimret.vals[EXP_PSYCHVAL]) < 0.0001){
                stimret.vals[EXP_PSYCHVAL] = 0;
            }
        }
        else if(expt.mode == ORIENTATION){
            stimret.vals[EXP_PSYCHVAL] = stimret.vals[0] - expt.mean;
            if (fabs(stimret.vals[EXP_PSYCHVAL]) < 0.0001){
                stimret.vals[EXP_PSYCHVAL] = 0;
            }
        }
        else
            stimret.vals[EXP_PSYCHVAL] = stimret.vals[0];
        stimret.vals[SIGNAL_STRENGTH] = fabs(stimret.vals[0]);
        if(stimret.vals[EXP_PSYCHVAL] == 0 || stimret.vals[0] == INTERLEAVE_EXPT_ZERO){
            if(afc_s.loopstate == CORRECTION_LOOP)
                stimret.vals[EXP_PSYCHVAL] = afc_s.jlaststairval;
            else if((drnd = drand48()) <= afc_s.proportion) // flip sign to match 2D expts above
                stimret.vals[EXP_PSYCHVAL] = -1; 
            else
                stimret.vals[EXP_PSYCHVAL] = 1; //up/left on binoc screen
            afc_s.jsignval = 0;
            stimret.vals[SIGNAL_STRENGTH] = 0;
        }
    }
    if (expt.mode == CONTRAST_DIFF){
        if ((val = StimulusProperty(expt.st,ORIENTATION_DIFF)) < 0){
            stimret.vals[EXP_PSYCHVAL] = stimret.vals[0]-expt1crit;
        }
        else {
            stimret.vals[EXP_PSYCHVAL] = -(stimret.vals[0]-expt1crit);
        }
    }

    
    if(expt.type2 == JUMPTYPE && expt.mode == FP_MOVE_SIZE && stimret.vals[1] ==1){
        stimret.vals[0] = -expt.mean;
    }
    if(stimi & STIMULUS_EXTRA_ZERO){
        stimret.vals[0] = 0;
        if(expt.type2 == BACKSTIM_TYPE){
            stimret.vals[0] = NOTSET;
            stimret.vals[1] = STIM_RDS;
        }
    }
    
    if(stimi & STIMULUS_EXTRA_ZERO && expt.st->type == STIM_CORRUG){
        if((expt.mode == ORIENTATION && expt.type2 == START_PHASE) || (expt.mode == START_PHASE || expt.type2 == ORIENTATION))
            stimret.vals[1] = 90;
        else
            stimret.vals[0] = 0;
    }
    if(stimi & STIMULUS_EXTRA_ZERO && expt.st->type == STIM_RADIAL){
        if(expt.mode == ORI2){
            stimret.vals[0] = expt.mean;
            expt.vals[CHANGE_SEED] =0;
        }
    }
    if(stimi & (STIMULUS_EXTRA_HIGHSQ | STIMULUS_EXTRA_HIGH)){
        if(expt.st->type == STIM_SQCORRUG && (expt.mode == ORIENTATION && expt.type2 == START_PHASE) || (expt.mode == START_PHASE || expt.type2 == ORIENTATION))
            stimret.vals[1] = 270;
        else
            stimret.vals[1] = expt.vals[HIGHX];
    }
    
    if(stimret.vals[0] == INTERLEAVE_EXPT_ZERO){
        stimret.vals[0] = 0;
    }
    if(stimi &  STIMULUS_EXTRA_FLIP  && expt.st->type == STIM_CYLINDER & expt.mode == TWOCYL_DISP){
        stimret.vals[0] = INTERLEAVE_EXPT_FLIP;
    }
    
    /*
     if(option2flag & STAIRCASE){
     if(expt.mode == CORRELATION){
     stimret.vals[0] = fabs(afc_s.jstairval);
     stimret.vals[1] = fabs(stimret.vals[1]) * afc_s.sign;
     stimret.vals[EXP_PSYCHVAL] = stimret.vals[1];
     afc_s.jstairval = stimret.vals[0];
     }
     else{
     stimret.vals[0] = fabs(afc_s.jstairval) * afc_s.sign;
     stimret.vals[EXP_PSYCHVAL] = stimret.vals[0];
     afc_s.jstairval = stimret.vals[0];
     }
     }
     else{
     afc_s.sign = 1;
     }
     */
    return(&stimret);
}

/*
 * if monkey does an indeterminate response in psychophysics
 * need to rerun that stimulus. Don't do it straight away though,
 * don't want animal to get a sneak preview of the stimulus. So swap
 * it with one a few further down the list
 * Only do this if we're doing psychophysics...
 */

void ShuffleStimulus(int state)
{
    int i, temp, trialsleft;
    int blklen = expt.nstim[3] * expt.blksize;
    
    if(seroutfile)
    {
        fprintf(seroutfile,"#Shuf stimno %d type %d (fix %d state %d,%d)\n",stimno, afc_s.lasttrial,fixstate,state,afc_s.loopstate);
        CountReps(stimno);
    }
    if(state == BAD_FIXATION && fixstate == INTERTRIAL)
        return;
    if(option2flag & STAIRCASE)
        return;
    if(afc_s.loopstate == CORRECTION_LOOP || !(SACCREQD(afc_s)))
        return;
    if((trialsleft = (expt.nstim[5] * expt.nreps) - stimno) > blklen && blklen > 0)
        i = random() % blklen;
    else if(trialsleft > 0)
        i = random() % trialsleft;
    else
        i = 0;
    if(seroutfile){
        fprintf(seroutfile,"#Swapping %d(%d) with %d(%d)\n",
                stimno,stimorder[stimno],stimno+i,stimorder[stimno+i]);
    }
    temp = stimorder[stimno];
    stimorder[stimno] = stimorder[stimno + i];
    stimorder[stimno+i] = temp;
    stimseq[trialctr].a = stimorder[stimno];  
    stimseq[trialctr].b = stimorder[stimno+i];  
    temp = seedorder[stimno];
    seedorder[stimno] = seedorder[stimno + i];
    seedorder[stimno+i] = temp;
    /*
     * if this stim is going to be repeated later, decrement stimno
     * which will be incremented before next trial
     */
    if(afc_s.lasttrial == SAC_BAD_TRIAL || state == WURTZ_LATE)
        stimno--;
    stimulus_is_prepared = 0;
    CountReps(stimno);
}

/* Set correct sacval for this stimulus*/
void SetSacVal(float stimval, int index)
{
    float val = stimval,temp,vtemp,d;
    double cosa,sina;
    int i,sign;
    
    if(SACCREQD(afc_s)){
        if(usenewdirs == 0)
            afc_s.newdirs = 0;
        
        /*
         if(option2flag & STAIRCASE)
         val = afc_s.jstairval * afc_s.sign;
         else 
         */
        
        /* only need this when NOT in an expt so remove for now*/
// But unless check for in expt, can mess up expt, esp if MAGONE_SIGNTWO
// when it can prevent exiting the loop
        if(afc_s.loopstate == CORRECTION_LOOP){
            //	val = expt.mean + ((expt.nstim[0]-1) * expt.incr)/2;
//            val = (float)copysign((double)val, (double)afc_s.jstairval); /* same direction as one got wrong */
        }
        
        
        if(  (option2flag & RANDOM) && (afc_s.loopstate == NORMAL_LOOP) ){
            val = fabs(stimval);
            if(mydrand() <= afc_s.proportion)
                val *= -1; /*j swaps over*/
        }
        
        temp = fabsf(afc_s.sacval[0]);
        vtemp = fabsf(afc_s.sacval[1]);
        /* 
         * for zero stimulus values, reward at random
         */
        if(val == INTERLEAVE_EXPT_ZERO)
            val = 0;
        
        rewardall = 0;
        if (val < 0.00001 && val > -0.00001){ // zero signal N.B.  may not get here when EXP_PSCHVAL is set to +/-1 in getexpval
            if(afc_s.loopstate == CORRECTION_LOOP)
                afc_s.sacval[0] = afc_s.sacval[0];  //don't change'
            else if ( (d = mydrand()) >= afc_s.proportion)
            {
                afc_s.sacval[0] = -afc_s.abssac[0];
                afc_s.sacval[1] = -afc_s.abssac[1];
                if(afc_s.newdirs){
                    cosa = cos(afc_s.sacdir[0]);
                    sina = sin(afc_s.sacdir[0]);
                    afc_s.sacval[0] = expt.vals[SACCADE_AMPLITUDE] * cosa;
                    afc_s.sacval[1] = expt.vals[SACCADE_AMPLITUDE] * sina;
                    cosa = cos(afc_s.sacdir[1]);
                    sina = sin(afc_s.sacdir[1]);
                    afc_s.sacval[4] = expt.vals[SACCADE_AMPLITUDE] * cosa;
                    afc_s.sacval[5] = expt.vals[SACCADE_AMPLITUDE] * sina;
                }
                sign = -1;
            }
            else
            {
                afc_s.sacval[0] = afc_s.abssac[0];
                afc_s.sacval[1] = afc_s.abssac[1];
                if(afc_s.newdirs){
                    cosa = cos(afc_s.sacdir[0]);
                    sina = sin(afc_s.sacdir[0]);
                    afc_s.sacval[4] = expt.vals[SACCADE_AMPLITUDE] * cosa;
                    afc_s.sacval[5] = expt.vals[SACCADE_AMPLITUDE] * sina;
                    cosa = cos(afc_s.sacdir[1]);
                    sina = sin(afc_s.sacdir[1]);
                    afc_s.sacval[0] = expt.vals[SACCADE_AMPLITUDE] * cosa;
                    afc_s.sacval[1] = expt.vals[SACCADE_AMPLITUDE] * sina;
                }
                sign = 1;
            }
            if (expt.vals[PREWARD] > 0.5){
                temp = 2 * (expt.vals[PREWARD]-0.5);
                if ( (d = mydrand()) < temp){
                    rewardall = 1;
                }
            }
        }
        else if (val > 0)
        {
            afc_s.sacval[0] = afc_s.abssac[0];
            afc_s.sacval[1] = afc_s.abssac[1];
            if(afc_s.newdirs){
                cosa = cos(afc_s.sacdir[1]);
                sina = sin(afc_s.sacdir[1]);
                afc_s.sacval[0] = expt.vals[SACCADE_AMPLITUDE] * cosa;
                afc_s.sacval[1] = expt.vals[SACCADE_AMPLITUDE] * sina;
                // set target for wrong choice
                cosa = cos(afc_s.sacdir[0]);
                sina = sin(afc_s.sacdir[0]);
                afc_s.sacval[4] = expt.vals[SACCADE_AMPLITUDE] * cosa;
                afc_s.sacval[5] = expt.vals[SACCADE_AMPLITUDE] * sina;
            }
            sign = 1;
        }
        else
        {
            afc_s.sacval[0] = -afc_s.abssac[0];
            afc_s.sacval[1] = -afc_s.abssac[1];
            if(afc_s.newdirs){
                cosa = cos(afc_s.sacdir[0]);
                sina = sin(afc_s.sacdir[0]);
                afc_s.sacval[0] = expt.vals[SACCADE_AMPLITUDE] * cosa;
                afc_s.sacval[1] = expt.vals[SACCADE_AMPLITUDE] * sina;
                cosa = cos(afc_s.sacdir[1]);
                sina = sin(afc_s.sacdir[1]);
                afc_s.sacval[4] = expt.vals[SACCADE_AMPLITUDE] * cosa;
                afc_s.sacval[5] = expt.vals[SACCADE_AMPLITUDE] * sina;
            }
            sign = -1;
        }
        /* 
         * no task for extra interleaved
         */
        if(expt.stimno & STIMULUS_EXTRA_ZERO || expt.stimno & STIMULUS_EXTRA_BLANK){
            afc_s.sacval[0] = afc_s.sacval[1] = 0;
            sign = 0;
        }
        if(afc_s.newdirs == 0){
            afc_s.sacval[4] = -afc_s.sacval[0];
            afc_s.sacval[5] = -afc_s.sacval[1];
        }
        if (afc_s.sign < 0){
            afc_s.sacval[0] = -afc_s.sacval[0];
            afc_s.sacval[1] = -afc_s.sacval[1];
            afc_s.sacval[4] = -afc_s.sacval[4];
            afc_s.sacval[5] = -afc_s.sacval[5];
            
        }
        SerialSend(HSACCADE_VALUE); 
        SerialSend(VSACCADE_VALUE); 
        SerialSend(TARGET2_POS); 
        afc_s.stimsign = sign; 
        
        /*
         * If mode == CORRELATION or DISTRIBUTION_CONC (both type MAGONE_SIGNTWO), the stairval is unsigned, and the sign is assigned
         *  separately
         */
        
        if(afc_s.type != MAGONE_SIGNTWO)
            afc_s.jstairval = val;
        else{
            afc_s.jsignval = val;
        }
    }
    
    covaryprop = expt.vals[COVARIATE];
    if((fabs(afc_s.ccvar) > 0.001 || expt.type2 == COVARY_XPOS) && SACCREQD(afc_s))
    {
        /* 
         * for 0 signal randomize covariation separately from reward, so that 
         * reward is random for both positions
         */
        if (val < 0.0001 && val > -0.0001){
            if ( (d = mydrand()) >= 0.5)
                SetProperty(&expt,expt.st,covaryprop,afc_s.ccvar);
            else
                SetProperty(&expt,expt.st,covaryprop,-afc_s.ccvar);
            
        }
        else if(val < 0)
            SetProperty(&expt,expt.st,covaryprop,afc_s.ccvar);
        else
            SetProperty(&expt,expt.st,covaryprop,-afc_s.ccvar);
        SerialSend(covaryprop);
    }
    else if(fabs(afc_s.ccvar) > 0.1){ 
        /*
         *  if no saccade required, then used a fixed abs value 
         * NB + stimvals associated with neg saccades so sign is inverted
         */
        if(val < 0)
            SetProperty(&expt,expt.st,covaryprop,afc_s.ccvar);
        else
            SetProperty(&expt,expt.st,covaryprop,-afc_s.ccvar);
    }
    if(expt.biasedreward != 0){
        if(sign == expt.biasedreward)
            expt.st->fix.rwsize = expt.vals[REWARD_SIZE2];
        else
            expt.st->fix.rwsize = expt.vals[REWARD_SIZE1];
        expt.vals[REWARD_SIZE] = expt.st->fix.rwsize;
        SerialSend(REWARD_SIZE);
    }
    else if (optionflags[SHOW_REWARD_BIAS]){
        expt.st->fix.rwsize = expt.vals[REWARD_SIZE3];
        expt.vals[REWARD_SIZE] = expt.st->fix.rwsize;
        SerialSend(REWARD_SIZE);
    }
    
}  

float ResetFixWin()
{
    float val = expt.fw;
    char cbuf[BUFSIZ];
    
    if(!optionflags[BINOCULAR_FIXPOINT]){
        if(optionflag & (RIGHT_FIXATION_CHECK | LEFT_FIXATION_CHECK))
            val = expt.fw * 1.5;
        else
            val = expt.fw;
        sprintf(cbuf,"%2s%.3f",serial_strings[FIXATION_WINDOW],val);
        SerialString(cbuf,0);
    }
    return(val);
}

int SetDotDistribution(void)
{
    vcoord x;
    int i;
    float val = expt.vals[DISTRIBUTION_CONC];
    float mean;
    char buf[BUFSIZ];
    
    mean = expt.vals[DISTRIBUTION_MEAN];
    if(expt.st->type == STIM_RDS && optionflags[RANDOM_DEPTH_PHASE])
    {
        if((expt.st->ndisps = expt.vals[DISTRIBUTION_WIDTH]) > MAXDISPS){
            sprintf(buf,"Can't have more that %d disparities",MAXDISPS);
            acknowledge(buf,NULL);
        }
        for(i = 0; i < expt.st->ndisps; i++){
            x = mean + (1+2*i-(expt.st->ndisps)) * expt.vals[RC1INC]/2;
            expt.st->disps[i] = deg2pix(x)/2;
        }
        if(onlineversion == 437 && testflags[PLAYING_EXPT])
            expt.st->disps[20] = 0;
        expt.st->mixdots = expt.st->left->ndots * (1 -val);
    }
    else
        expt.st->mixdots = 0;
    return(expt.st->mixdots);
}

int SetDotPlanes(Stimulus *st, double tf, double depth_mod)
{
    int nplanes,i,ck;
    
    nplanes = rint(mon.framerate/tf);
    if(nplanes > MAXDISPS)
        nplanes = MAXDISPS;
    
    if(optionflag & CONTRAST_REVERSE_BIT)
        nplanes = 2;
    if((st->ndisps = nplanes) > MAXDISPS){
        acknowledge("Too many disparities",NULL);
    }
    if(st->pos.contrast > 0)
        ck = 2;
    else
        ck = -2;
    
    /*
     * N.B. because the disparity of the whole RDS is done by translation, these 
     * disps, which are added to individual dots, are relative to the 
     * main disparity. The disps are actually half the disparity, since they are
     * added to one eye and subtracted from the other.
     */
    for(i = 0; i < nplanes; i++){
        st->disps[i] = deg2pix(depth_mod/ck * cos(deg_rad(st->vals[START_PHASE])+(i * M_PI * 2)/nplanes));
    }
    return(nplanes);
}

char *ShowStimVals(Thisstim *stp)
{
    char cbuf[BUFSIZ],ebuf[BUFSIZ],tmp[BUFSIZ];
    int i;
    
    if(expt.flag & TIMES_EXPT2){
        if(expt.type2 == ORI_BANDWIDTH){
            sprintf(cbuf,"%s=%.*f,%s=%.*f (%.3f)",serial_strings[expt.mode],
                    nfplaces[expt.mode],stp->vals[0],serial_strings[expt.type2],
                    nfplaces[expt.type2],stp->vals[1],stp->vals[SIGNAL_STRENGTH]);
        }
        else{
            sprintf(cbuf,"%s=%.*f,%s=%.*f ",serial_strings[expt.mode],
                    nfplaces[expt.mode],stp->vals[0],serial_strings[expt.type2],
                    nfplaces[expt.type2],stp->vals[1]);
        }
    }
    else if(stp->expno == EXP_SECOND)
        sprintf(cbuf,"%s=%.*f ",serial_strings[expt.type2],nfplaces[expt.type2],stp->vals[1]);
    else
        sprintf(cbuf,"%s=%.*f ",serial_strings[expt.mode],nfplaces[expt.mode],stp->vals[0]);
    if(stimorder[stimno] & STIMULUS_EXTRA_FLIP){
        strcat(cbuf,extralabels[0]);
        sprintf(tmp,"%s=%.*f ",serial_strings[DISP_X],nfplaces[DISP_X],GetProperty(&expt,expt.st,DISP_X));
        strcat(cbuf,tmp);
    }
    else if(stimorder[stimno] & STIMULUS_EXTRA_BLANK)
        strcat(cbuf,"Blank");
    else if((i = stimorder[stimno] & (~ORDER_BITS)) < expt.nstim[2])
        strcat(cbuf,extralabels[i]);
    if(expt.type3 != EXPTYPE_NONE){
        if(expt.type3 < MAXTOTALCODES)
            sprintf(ebuf,"%2s=%.2f",serial_strings[expt.type3],GetProperty(&expt,expt.st,expt.type3));
        else
            sprintf(ebuf,"%d=%.2f",expt.type3,GetProperty(&expt,expt.st,expt.type3));
    }
    else if(expt.vals[TARGET_RATIO] < 1)
        sprintf(ebuf,"tr%.2f",expt.vals[TARGET_RATIO]);
    else
        sprintf(ebuf,"");
    if (SACCREQD(afc_s)){
        if (stp->vals[EXP_PSYCHVAL] > 0){
            strcat(cbuf,"+");
        }
        else if (stp->vals[EXP_PSYCHVAL] < 0){
            strcat(cbuf,"-");
        }
    }
    
    if(optionflags[MICROSTIM]){
        if (expt.type3 == FAKESTIM_EXPT){
            sprintf(tmp," ustim%.3f",fakestim);
            strcat(cbuf,tmp);
        }
        else
            strcat(cbuf," ustim");
    }
    else
        strcat(cbuf,ebuf);
    if(afc_s.loopstate == CORRECTION_LOOP)
        strcat(cbuf,"**");
    else if(!expt.st->mode & EXPTPENDING)
        strcat(cbuf,"*rolling*");
        
    statusline(cbuf);
    return(cbuf);
}




int SetFrameStim(int i, long lrnd, double inc, Thisstim *stp, int *nstim)
{
    int nv = expt.nfast;
    int rnd = lrnd % (nv + expt.fastextras);
    double minval = expt.minval, pb = 0,frnd = 1, forcex = 0;
    int nextra = expt.fastextras,xoff = 0;
    
    if(expt.vals[BLANK_P] > 0 && nextra){ //need to have +blank and pblank set
        nextra--;
        xoff++;
        if(expt.vals[UNCORR_P] > 0)
        {
            nextra--;
            xoff++;
        }
        frnd = mydrand();
    }
    else{
        frnd = expt.vals[BLANK_P] + 1;
        xoff = 0;
    }
    
    rnd = lrnd % (nv + nextra);
    frameiseq[i] = rnd+xoff;
    
    if(expt.mode == DISTRIBUTION_CONC)
        frameseqb[i] = stp->vals[0];
    else if(expt.type2 == DISTRIBUTION_CONC)
        frameseqb[i] = stp->vals[1];
    
    if(frnd < expt.vals[BLANK_P]+expt.vals[UNCORR_P]){
        if(expt.vals[UNCORR_P] > 0 && frnd < expt.vals[UNCORR_P]){
            frameiseq[i] = 1;
            frameseq[i] = INTERLEAVE_EXPT_UNCORR;
            fastvals[1] = frameseq[i];
            fastbvals[1] = frameseq[i];
        }
        else{
            frameiseq[i] = 0;
            frameseq[i] = INTERLEAVE_EXPT_BLANK;
            fastvals[0] = frameseq[i];
            fastbvals[0] = frameseq[i];
        }
        forcex = 1;
    }
    else if(expt.mode == DISTRIBUTION_CONC){
        /*
         * For this expt, disparities are always over the same range, but a greater
         * PROPORTION of frames contains the signal disparity.
         * iseq == 0 means that stim was at the value set by expt2.
         * stims 1 -> are the random stimuli for the subspace map
         */
        if(rnd == 0){
            frameiseq[i] = 1;
        }
        frameseq[i] = minval + (rnd-nextra) * inc;
        if(((lrnd >> 8) & 0xff) < 255 * stp->vals[0]){
            frameseq[i] = stp->vals[1];
            frameiseq[i] = 0;
        }
        else{
            if(rnd < nextra)
                frameseq[i] = i2expval(rnd,nextra, 0, xoff);
            frameiseq[i] = rnd+1;
        }
        frameseqb[i] = stp->vals[0];
    }
    else if(expt.mode == SEEDRANGE){
        if (expt.type3 == SEEDOFFSET && expt.stimmode == IMAGETRIALMIX){
            if (expt.st->seedoffset == 0 && expt.vals[DISTRIBUTION_WIDTH] > 1){
                i = myrnd_i() % (int)(expt.vals[DISTRIBUTION_WIDTH]);
                i = (i+expt.vals[DISTRIBUTION_MEAN]);
                expt.st->seedoffset = i * 1000;
                SerialSend(SEEDOFFSET);
            }
        }
        
        
        if (expt.nstim[1] > 1)
                frameseqb[i] = expval[i2expi(expt.flag,nstim,rnd+xoff,1)- nstim[2]];
        if (optionflags[FAST_SEQUENCE] && expt.vals[DISTRIBUTION_WIDTH] < 1){
            nv = (int)(expt.vals[SEEDRANGE]-1);
            rnd = lrnd % (nv + nextra);        
        }
        else if(expt.st->seedoffset > 999) //use fixed movie
            rnd = i/expt.vals[FAST_SEQUENCE_RPT];
        else if(expt.vals[DISTRIBUTION_WIDTH] < 1){
            rnd = i/expt.vals[FAST_SEQUENCE_RPT];
            nv = (int)(expt.vals[SEEDRANGE]-1);
            rnd = lrnd % (nv + nextra);
        }
        else if(expt.stimmode == IMAGETRIALMIX && expt.vals[DISTRIBUTION_WIDTH] > 1){
            rnd = i/expt.vals[FAST_SEQUENCE_RPT];
        }
        else{
            nv = (int)(expt.vals[SEEDRANGE]-1);
            rnd = lrnd % (nv + nextra);
        }
        frameiseq[i] = 1+rnd;
        frameseq[i] = frameiseq[i];
        expt.fasttype = SET_SEED;
    }
    else if(rnd < nextra){
        frameseq[i] = i2expval(rnd,nextra, 0, xoff);
    }
    else if(expt.type3 == SETCONTRAST){
        frameseq[i] = rnd * inc;
    }
    else if(expt.type2 == DISTRIBUTION_WIDTH){
        frameseq[i] = stp->vals[0] + (rnd-nextra) * inc;
    }
    else if(expt.type2 == TEMPORAL_ORI_BANDWIDTH){
        if(nv * inc >= 180)
            frameseq[i] = 0 + rnd * inc;
        else
            frameseq[i] = stp->vals[0] + (((rnd-nextra)-nv/2)) * inc;
    }
    else if(expt.type2 == DISTRIBUTION_CONC){
        /*
         * For this expt, disparities are always over the same range, but a greater
         * PROPORTION of frames contains the signal disparity.
         */
        frameseq[i] = minval + (rnd-nextra) * inc;
        if(((lrnd >> 8) & 0xff) < 255 * stp->vals[1]){
            frameseq[i] = stp->vals[0];
            frameiseq[i] = 0;
        }
        else
            frameiseq[i] = rnd+1;
        frameseqb[i] = stp->vals[1];
        
    }
    else if (expt.vals[DISTRIBUTION_CONC] > 0 && expt.mode == CONTRAST_DIFF || expt.mode == PLAID_RATIO){
        frameseq[i] = minval + (rnd-nextra) * inc;
        frameiseq[i] = rnd+1;
    }
    else if(expt.type2 != EXPTYPE_NONE && expt.nstim[1] > 1){
        /*
         * normally, expval[0...nstim[2]] has the "extras", but in a fast sequence
         * these are done by "fastextras". So expval[0] is the first non-extra
         * v 4.51 had ths wrong
         * i2expi returnes values over the whole range, but should only get here
         * if its not an extra, so returned values are always >= nstim[2];
         * UNLESS, we have forced a percentagle off blanks, in which case they
         * are >= than nstim[2]-xoff
         */
        frameseq[i] = expval[i2expi(expt.flag,nstim,rnd+xoff,0) - nstim[2]];
        frameseqb[i] = expval[i2expi(expt.flag,nstim,rnd+xoff,1)- nstim[2]];
    }
    else
        frameseq[i] = expval[rnd-nextra];
    if (!forcex){
        fastvals[frameiseq[i]] = frameseq[i];
        fastbvals[frameiseq[i]] = frameseqb[i];
    }
}



int PrepareExptStim(int show, int caller)
{
    int i, stimres,cnt,j,nvals,nv,nstim[10],k,frpt,ncycles,drop,iperiod;
    Substim *rds;
    Thisstim *stp,*istp,thisstim;
    float val,rval,xval,yval,inc, minval,psychval,bvals[MAXFRAMES];
    char cbuf[256];
    int extra = -1,lastid = 0,ivals[MAXFRAMES];
    int frameset[MAXFRAMES],rnd,nframes;
    long lrnd;
    char ebuf[BUFSIZ];
    double tf,depth_mod,scale,odiff,mean,drnd;
    Stimulus *st = expt.st;
    struct timeval then;
    static int realstim[2] = {0};
    int brpt = 0,id,startf = 1;
    int isig,pw,sigframes[MAXFRAMES],laps=0;
    int code;
    int imid = 0;
    
    
    if(mode & BW_ERROR)
        return(-1);
    if(stimno == NEW_EXPT)
        InitExpt();
    if(stimulus_is_prepared && caller != 7) //7 forces prep
        return(stimno);
    
#ifdef MONITOR_CLOSE
    if(seroutfile){
        fprintf(seroutfile,"PrEx %d\n",caller);
    }
#endif
    fakestim = 0;
    expt.laststimno = stimno;
    if(expt.type2 == OPPOSITE_DELAY){
        SetProperty(&expt, expt.st, SEED_DELAY, expt.stimvals[SEED_DELAY]);
    }
    if(expt.vals[RANDOM_ORI] > 0.001)
    {
        rval = mydrand();
        rval = (rval *2) - 1;
        val = expt.vals[RANDOM_ORI] * rval;
        SetProperty(&expt, expt.st, ORIENTATION, val + expt.stimvals[ORIENTATION]);
    }
    if(expt.st->type == STIM_RDS)
    {
        rds = expt.st->left;
        if(rds->seedloop == 1 && rds->baseseed > 1000)
        {
            expt.st->left->baseseed += 2;
            expt.st->right->baseseed = ++(expt.st->left->baseseed);
        }
    }
    if(expt.st->type == STIM_IMAGE)
    {
        if(st->immode == IMAGEMODE_ORBW && st->left->seedloop == 1 && st->left->baseseed > 1000)
        {
            expt.st->left->baseseed++;
        }
    }
    else if(expt.st->next && expt.st->next->type == STIM_RDS){
        rds = expt.st->next->left;
        if(rds->seedloop == 1 && rds->baseseed > 1000)
        {
            rds->baseseed++;
            expt.st->right->baseseed = ++(rds->baseseed);
        }
    }
    else if(expt.st->type == STIM_RADIAL){
        if(expt.vals[CHANGE_SEED] > 0)
            st->right->show_increment = st->left->show_increment = 0;
    }
    else if(st->type != STIM_IMAGE){
        /*
         * Need to initialize seed for random phase sequences etc for
         * non-rds stimulist
         */
        expt.st->right->baseseed = ++(expt.st->left->baseseed);
        myrnd_init(expt.st->left->baseseed);
    }
    
    
    expt.allstimid++;
    if(optionflags[REVERSE_CORRELATE])
    {
        nframes = expt.st->nframes/expt.st->framerepeat;
        expt.st->rcseed++;
        srand48(expt.st->rcseed);
        for(i = 0; i < nframes; i++)
            frameset[i] = 0;
        cnt = 1; /* number of times each stimulus can repeat */
        for(i = 0; i < expt.st->nframes;)
        {
            do{
                rnd = lrand48() % nframes;
            }while(frameset[rnd] >= cnt);
            frameset[rnd]++;
            for(j = 0; j < expt.st->framerepeat; j++){
                frameseq[i] = expt.mean + (expt.incr * (rnd - ((float)(nframes-1)/2)));
                frameiseq[i] = rnd;
                i++;
            }
            
        }
        if(seroutfile){
            fprintf(seroutfile,"framesq ");
            for(i = 0; i < expt.st->nframes; i++)
                fprintf(seroutfile,"%d ",frameiseq[i]);
            fprintf(seroutfile,"\n");
        }
        
    }
    expt.laststim = expt.stimno;
    
    
    /*	
     * Badfix/Prem trials during psychophyics Need the stimulus order to be changed * so that monkey does not see the same stimulus twice. During the staircase,
     * this is not necessary.
     */
    if(SACCREQD(afc_s) && !(option2flag & STAIRCASE))
    {
        if(afc_s.lasttrial == BAD_TRIAL || afc_s.lasttrial == SAC_BAD_TRIAL)
            ShuffleStimulus(0); /* ? obsolete  BADFIX and Late already call this*/
    }
    
    expt.stimno = stimorder[stimno];
    expt.stimid = expt.stimno & (~ORDER_BITS);
    expt.expstimid[2] = stim3order[stimno];
    
    if(seroutfile)
        fprintf(seroutfile,"#id %d\n",expt.stimno);
    
    expt.nextstim = stimorder[stimno+1];
    if(stimno > 0){
        lastid = (stimorder[stimno-1] & (~ORDER_BITS));
        if(option2flag & IFC){
            /*
             * stimno & 1 = second stimulus. if lastid == 0, means first stim was no
             * signal, hense this stim is the signal value.
             * !(stimno &1) means first stimulus. If this ==0, the next stimulus has
             * the signal value. 
             */
            if(stimno & 1){
                if(lastid == 0){  //This stim (#2) is stimulus
                    ifcstim = expt.stimid;
                    ifcanswer = 1;
                }
                else{ //This stim (#2) is control
                    istp = getexpval(lastid);
                    ifcval = istp->vals[0];
                    ifcstim = lastid;
                    ifcanswer = 0;
                }
            }
            else{
                if(expt.stimid == 0){  // This (#1) is control
                    ifcstim = (stimorder[stimno+1] & (~ORDER_BITS));
                    istp = getexpval(stimorder[stimno+1]);
                    ifcval = istp->vals[0];
                    ifcanswer = 1;
                }
                else{  // This (#1) is signal
                    ifcstim = expt.stimid;
                    ifcanswer = 0;
                }
            }
        }
    }
    
    expt.stimtype = expt.st->type;
    if(stimno > 0 && expt.st->next != NULL && !(stimorder[stimno-1] & INTERLEAVE_BLANK))
        expt.backtype = expt.st->next->type;
    /*
     *  set the extra stimulus types after expts 1 and 2, to avoid
     * resetting. e.g. is expt2 is correlation, then the interleaved 
     * uncorrelated stimulus get reset toa correlation number in vals[1]
     */
    
    stp = getexpval(expt.stimno);
    memcpy(&thisstim,stp,sizeof(Thisstim));
    stp = &thisstim;
    if(expt.st->type == STIM_GRATING && expt.mode == DEPTH_MOD && (option2flag & IFC) && (stimorder[stimno] & STIMULUS_EXTRA_RANDOM)){
        depth_mod = expval[ifcstim] * expt.vals[IFCSCALE];
        SetProperty(&expt, expt.st, DEPTH_MOD, depth_mod);
        extra = RANDOM;
    }
    if(stimorder[stimno] & STIMULUS_EXTRA_FLIP){
        if(expt.mode == TWOCYL_DISP){
            xval = expval[expt.nstim[2]];
            expt.vals[TWOCYL_DISP] == INTERLEAVE_EXPT_FLIP;
            if(stimorder[stimno] & 1){
                SetProperty(&expt, expt.st, DISP_X, xval);
                SetProperty(&expt, expt.st->next, DISP_X, -xval);
                stp->vals[EXP_PSYCHVAL] = xval;
            }
            else{
                SetProperty(&expt, expt.st, DISP_X, -xval);
                SetProperty(&expt, expt.st->next, DISP_X, xval);
                stp->vals[EXP_PSYCHVAL] = -xval;
            }
        }
    }
    if(stimorder[stimno] & STIMULUS_EXTRA_HIGH)
    {
        extra = HIGH;
        if(expt.st->type == STIM_SQCORRUG && (expt.mode == ORIENTATION && expt.type2 == START_PHASE) || (expt.mode == START_PHASE && expt.type2 == ORIENTATION)){
            SetStimulus(expt.st, 270, START_PHASE, NULL);
            SetStimulus(expt.st, expt.mean, ORIENTATION, NULL);
            expt.hightype = START_PHASE;
        }
        else if(expt.st->type == STIM_IMAGE){
            expt.hightype = ORI_BANDWIDTH;
        }
        SetStimulus(expt.st, expt.mean, expt.mode, NULL);
        SetStimulus(expt.st, expt.vals[HIGHX], expt.hightype, NULL);
    }
    if(stimorder[stimno] & STIMULUS_EXTRA_HIGHSF)
    {
        extra = HIGH;
        /*
         * "HighSF" for an RDS means a transparent RDS. In IFC expts this is matched
         * to the disparities contained in the other interval.
         * Need to put code in here to make control match DM and FREQ of signal for
         * a 2-D IFC expt
         */
        if(expt.st->type == STIM_RDS){
            if(expt.mode == MODULATION_F && (option2flag & IFC))
                tf = expval[ifcstim];
            else
                tf = StimulusProperty(expt.st,MODULATION_F);
            if((expt.mode == DEPTH_MOD || expt.mode == IFCSCALE) && (option2flag & IFC)){
                if(expt.mode == IFCSCALE)
                    depth_mod = expt.stimvals[DEPTH_MOD] * expval[ifcstim];
                else{
                    depth_mod = expval[ifcstim] * expt.vals[IFCSCALE];
                    if(option2flag & RANDOM){
                        rnd = myrnd_i();
                        if(option2flag & ALT_PROBE_BIT){
                            if(rnd & 1)
                                depth_mod = expval[ifcstim];
                        }
                        else{
                            i = rnd %3;
                            scale = (1 + expt.vals[IFCSCALE])/(2 * expt.vals[IFCSCALE]);
                            scale = expt.vals[IFCSCALE] * pow(scale,i-1);
                            depth_mod = expval[ifcstim] * scale;
                        }
                    }
                }
                optionflags[MODULATE_DISPARITY] = 2;
                SetProperty(&expt, expt.st, DEPTH_MOD, depth_mod);
                SetProperty(&expt, expt.st, MODULATION_F, expt.st->vals[MODULATION_F]);
                SetDotPlanes(expt.st,tf, depth_mod);
            }
        }
        else
            SetStimulus(expt.st, expt.vals[HIGHSF], SF, NULL);
    }
    if(stimorder[stimno] & STIMULUS_EXTRA_HIGHTF)
    {
        extra = HIGH;
        SetStimulus(expt.st, expt.vals[HIGHTF], TF, NULL);
    }
    if(stimorder[stimno] & STIMULUS_EXTRA_ZEROCOH)
    {
        extra = UNCORRELATE;
        expt.stimvals[JDEATH] = StimulusProperty(expt.st,JDEATH);
        expt.stimvals[SET_SEEDLOOP] = StimulusProperty(expt.st,SET_SEEDLOOP);
        expt.oldstimflag = expt.st->flag;
        expt.st->flag |= FLAT_SURFACES;
        SetStimulus(expt.st, 1.0, JDEATH, NULL);
        SetStimulus(expt.st, 0, SET_SEEDLOOP, NULL);
    }
    if(stimorder[stimno] & STIMULUS_EXTRA_ZERO)
    {
        extra = ZERO;	 
        if(expt.st->type == STIM_CORRUG){
            /*
             * why record this again here? risks losing it if badfix produces the
             * interleaved zero twice in a row. Perhaps here for psych where dm is
             * changing (IFC). So, added trap for dm not 0
             expt.stimvals[DEPTH_MOD] = StimulusProperty(expt.st,DEPTH_MOD);
             */
            if((depth_mod = StimulusProperty(expt.st,DEPTH_MOD)) > 0)
                expt.stimvals[DEPTH_MOD] = depth_mod;
            
            SetStimulus(expt.st, 0.0, DEPTH_MOD, NULL);
        }
        if(expt.st->type == STIM_SQCORRUG && (expt.mode == ORIENTATION && expt.type2 == START_PHASE) || (expt.mode == START_PHASE || expt.type2 == ORIENTATION)){
            SetStimulus(expt.st, 90, START_PHASE, NULL);
            SetStimulus(expt.st, expt.mean, ORIENTATION, NULL);
        }
        else if(expt.type2 == BACKSTIM_TYPE)
        {
            StimulusType(expt.st->next, STIM_RDS);
            StimulusType(expt.st, STIM_NONE);
        }
        else{
            expt.stimvals[expt.mode] = StimulusProperty(expt.st,expt.mode);
            SetStimulus(expt.st, 0.0, expt.mode, NULL);
        }
    }
    else if(stimorder[stimno] & STIMULUS_EXTRA_BLANK)
    {
        if(expt.st->type != STIM_NONE){
            realstim[0] = expt.st->type;
            if(st->next)
                realstim[1] = expt.st->next->type;
        }
        expt.st->type = STIM_NONE;
        if(expt.st->next != NULL)
            expt.st->next->type = STIM_NONE;
        SerialSend(STIMULUS_TYPE_CODE);
    }
    else if(stimorder[stimno] & STIMULUS_EXTRA_HIGHSQ)
    {
        //	    expt.st->type = STIM_SQCORRUG;
        if(expt.st->type == STIM_SQCORRUG && (expt.mode == ORIENTATION && expt.type2 == START_PHASE) || (expt.mode == START_PHASE && expt.type2 == ORIENTATION)){
            SetStimulus(expt.st, 270, START_PHASE, NULL);
            SetStimulus(expt.st, expt.vals[HIGHSF], SF, NULL);
        }
        else{
            StimulusType(expt.st, STIM_SQCORRUG);
            SerialSend(STIMULUS_TYPE_CODE);
            SetStimulus(expt.st, expt.stimvals[DEPTH_MOD], DEPTH_MOD, NULL);
            SetStimulus(expt.st, expt.vals[HIGHSF], SF, NULL);
        }
    }
    if(expt.type3 == MICROSTIM_EXPT || expt.type3 == FAKESTIM_EXPT){
        if(stimorder[stimno] & STIMULUS_EXTRA_EXPT3)
        {
            if(!optionflags[USTIM_ZERO_ONLY] || stp->vals[SIGNAL_STRENGTH] <= expt.vals[INITIAL_APPLY_MAX])
                optionflags[MICROSTIM] = 1;
            SerialSend(OPTION_CODE);
            if(expt.type3 == FAKESTIM_EXPT)
                SetStimulus(expt.st,expt.exp3vals[1] , covaryprop, NULL);
            fakestim = expt.vals[FAKESTIM_SIGNAL];
        }
        else
        {
            optionflags[MICROSTIM] = 0;
            SerialSend(OPTION_CODE);
            if(expt.type3 == FAKESTIM_EXPT)
                SetStimulus(expt.st,expt.exp3vals[0] , covaryprop, NULL);
            fakestim = 0;
        }
    }
    else if(expt.type3 == TONETIME){
        if(stp->vals[SIGNAL_STRENGTH] <= expt.vals[INITIAL_APPLY_MAX])
            SetProperty(&expt,expt.st,TONETIME,expt.exp3vals[stim3order[stimno]]);
        else
            SetProperty(&expt,expt.st,TONETIME,0);
    }
    else if(expt.type3 == XPOS){
        /*
         * April 2005 changed so that backstim is not moved if BACK_FIXED is set
         * Before this seems to have forced both, which should not have been necessary.
         * unless there was an expt where BACK_FIXED had to be set for another reason
         */
        if(stimorder[stimno] & STIMULUS_EXTRA_EXPT3){
            SetStimulus(expt.st, expt.vals[XPOS] * -1, XPOS, NULL);
            if(!(optionflag & BACKGROUND_FIXED_BIT))
                SetStimulus(expt.st->next, expt.vals[XPOS] * -1, XPOS, NULL);
        }
        else{
            SetStimulus(expt.st, expt.vals[XPOS], XPOS, NULL);
            if(!(optionflag & BACKGROUND_FIXED_BIT))
                SetStimulus(expt.st->next, expt.vals[XPOS], XPOS, NULL);
        }
        sprintf(ebuf,"%2s=%.2f",serial_strings[XPOS],GetProperty(&expt,expt.st,XPOS));
    }
    else if(expt.type3 == RELDISP || expt.type3 == DISP_P){
        if(stimorder[stimno] & STIMULUS_EXTRA_EXPT3)
            SetStimulus(expt.st, expt.exp3vals[0], expt.type3, NULL);
        else
            SetStimulus(expt.st, expt.exp3vals[1], expt.type3, NULL);
        sprintf(ebuf,"%2s=%.2f",serial_strings[expt.type3],GetProperty(&expt,expt.st,expt.type3));
    }
    else if(expt.type3 == ORIENTATION){
        if(expt.st->next)
            odiff = StimulusProperty(expt.st->next, ORIENTATION)  - StimulusProperty(expt.st, ORIENTATION);
        if(stimorder[stimno] & STIMULUS_EXTRA_EXPT3){
            SetStimulus(expt.st, expt.stimvals[ORIENTATION] * -1, ORIENTATION, NULL);
        }
        else
            SetStimulus(expt.st, expt.stimvals[ORIENTATION], ORIENTATION, NULL);
        if(expt.st->next)
            SetStimulus(expt.st->next, odiff + StimulusProperty(expt.st,ORIENTATION), ORIENTATION, NULL);
        
        sprintf(ebuf,"%2s=%.2f",serial_strings[ORIENTATION],GetProperty(&expt,expt.st,ORIENTATION));
    }
    else if(expt.type3 == SET_SEED){ //sets seed tgat deternmines sequence for trial
        if (expt.exp3vals[stim3order[stimno]] > 999){
            SetProperty(&expt,expt.st,expt.type3,  expt.exp3vals[stim3order[stimno]]);
        }
        else{
            expt.expseed++;
            SetProperty(&expt,expt.st,expt.type3,  expt.expseed);
        }
        sprintf(ebuf,"%2s=%.2f",serial_strings[expt.type3],GetProperty(&expt,expt.st,expt.type3));
        myrnd_init(expt.st->left->baseseed);
    }

    else if(expt.type3 != EXPTYPE_NONE){
        SetProperty(&expt,expt.st,expt.type3,  expt.exp3vals[stim3order[stimno]]);
        sprintf(ebuf,"%2s=%.2f",serial_strings[expt.type3],GetProperty(&expt,expt.st,expt.type3));
    }

    else
        sprintf(ebuf,"");
    printStringOnMonkeyView(ebuf, 0);

    if(expt.vals[ONETARGET_P] > 0){
        if ((drnd = mydrand()) < expt.vals[ONETARGET_P])
            expt.vals[TARGET_RATIO] = 0;
        SerialSend(TARGET_RATIO);
    }
    
    if(optionflags[ADD_FPJUMP] && stimorder[stimno]  & STIMULUS_EXTRA_FPJUMP)
    {
        optionflags[SIMULATE_FP_MOVE] = 0;
    }
    
    if(option2flag & INTERLEAVE_RANDOM)
    {
        if(stimorder[stimno]  & STIMULUS_EXTRA_RANDOM){
            if(expt.mode == DEPTH_MOD){
                optionflags[RANDOM_DEPTH_PHASE] = 0;
                if(stimorder[stimno]  & STIMULUS_EXTRA_RANDOM)
                    optionflags[RANDOM_DEPTH_PHASE] = 1;
            }
            else{	
                optionflags[RANDOM_PHASE] = 0;
                if(stimorder[stimno]  & STIMULUS_EXTRA_RANDOM)
                    optionflags[RANDOM_PHASE] = 1;
            }
        }
        else
            optionflags[RANDOM_PHASE] = 0;
        SerialSend(OPTION_CODE);
    }
    if(option2flag & INTERLEAVE_ONE_MONOCULAR)
    {
        optionflag &= (~MONOCULAR_MODE);
        if(stimorder[stimno]  & STIMULUS_EXTRA_LEFT)
            optionflag |= LEFT_FIXATION_CHECK;
        else if(stimorder[stimno]  & STIMULUS_EXTRA_RIGHT)
            optionflag |= RIGHT_FIXATION_CHECK;
        // allow a bigger fixation window for monoc trials, if the fixation point is monocular
        ResetFixWin();
        setoption();
        clear_display(1);
        SerialSend(OPTION_CODE);
    }
    
    /* work out stimulus values in case its a double experiment */
    CheckPsychVal(stp);
    val = stp->vals[EXP_PSYCHVAL];
    
    
    /* ifcanswer != stimo &1 means this stimulus is the IFC control */
    if(ifcanswer != (stimno &1) && option2flag & IFC && optionflags[INTERLEAVE_FLIP]){
        if(ifcanswer == 1)
            stp->vals[0] = ifcval * -1;
        else
            stp->vals[0] = ifcval;
    }
    
    
    if(stimorder[stimno] & STIMULUS_EXTRA_UNCORRELATE || stp->vals[0] == INTERLEAVE_EXPT_UNCORR)
    {
        extra = UNCORRELATE;
        expt.st->flag &= (~ANTICORRELATE);
        expt.st->flag |= UNCORRELATE;
        if((rdspair(expt.st) || rlspair(expt.st)) && expt.type2 != DISP_BACK){
            expt.st->next->flag &= (~ANTICORRELATE);
            expt.st->next->flag |= UNCORRELATE;
        }
    }
    
    if(stp->vals[1] == INTERLEAVE_EXPT_UNCORR){
        if(rdspair(expt.st) || rlspair(expt.st)){
            expt.st->next->flag &= (~ANTICORRELATE);
            expt.st->next->flag |= UNCORRELATE;
        }
    }
    if(rdspair(expt.st)){
        i = 0;  //dummy, for debugger
    }
    
    
    /* set right saccade sign for responses */
    memcpy(&stimseq[trialctr],stp,sizeof(Thisstim));
    psychval = val;
    
    //If in a corrrection loop, stimulus will be repeated, so keep afc_sign the same
    if (optionflags[CHOICE_BY_ICON]  &&  afc_s.loopstate != CORRECTION_LOOP){
        if((drnd = drand48()) < afc_s.signflipp)
            afc_s.sign = -1;
        else {
            afc_s.sign = 1;
        }
        SetTargets();
    }

    SetSacVal(psychval,expt.stimid);
    
    
    
    /* Set up all the stimulus paramters careful not to unset something that
     *  was set for "extra" stimuli. E.g. if we're interleaving an uncorrelated
     *  stimulus, don't set the correlation to +- 1
     */
    stimres = 0;
    expt.expno = stp->expno;
    
    if((tf = fabs(stp->vals[0] - INTERLEAVE_EXPT_HIGH)) < 0.5)
        stp->vals[0] = expt.vals[HIGHX];
    
    if(rdspair(expt.st)){
        i = 0;  //dummy, for debugger
    }
    if(expt.st->next != NULL){
        if(optionflag & BACKGROUND_FIXED_BIT)
            SetStimulus(expt.st->next,expt.st->next->vals[START_PHASE],SETPHASE,NOEVENT);
        else
            SetStimulus(expt.st->next,expt.st->vals[START_PHASE],SETPHASE,NOEVENT);
    }
    
    if(rdspair(expt.st)){
        i = 0;  //dummy, for debugger
    }
    
    
    /*
     * if we are in an Endstim Erro condition, don't proceed
     */
    
    /*
     * Here is where the expt 1 values are set.
     */
    if(stp->expno != EXP_FIRST && stp->vals[0] > INTERLEAVE_EXPT
       && extra != HIGH && expt.mode != BACKGROUND_IMAGE && expt.mode != BACKGROUND_MOVIE)
    {
        stimres = SetProperty(&expt,expt.st,expt.mode,stp->vals[0]+fakestim);
        if(stimres < 0)
            fprintf(stderr,"Ex1StimError %d %.4f\n",expt.mode,stp->vals[0]);
    }
    else if (expt.mode == BACKGROUND_IMAGE && expt.stimmode == BUTTSEXPT){
        stimres = SetProperty(&expt,expt.st,expt.mode,stp->vals[0]+fakestim);
        if(stimres < 0)
            fprintf(stderr,"Ex1StimError %d %.4f\n",expt.mode,stp->vals[0]);
    }
    if(expt.mode == SF && stp->vals[0] == INTERLEAVE_EXPT_ZERO){
        stp->vals[0] = 0;
    }
    else if(expt.mode == ORTHOG_POS && stp->vals[0] == INTERLEAVE_EXPT_ZERO){
        prevstim = expt.st->type;
        StimulusType(expt.st, STIM_NONE);
        SerialSend(STIMULUS_TYPE_CODE);
    }
    else if(stp->vals[0] == INTERLEAVE_EXPT_ZERO){
        stp->vals[0] = 0;
    }
    
    if(rdspair(expt.st)){
        i = 0;  //dummy, for debugger
    }
    if(optionflags[INTERLEAVE_MONOC_ALL] || stp->vals[0] == INTERLEAVE_EXPT_COMPONENT_ONE){
        optionflag &= (~MONOCULAR_MODE);
        val = fabs(remainder(stp->vals[1],1));
        if(stp->vals[0] == INTERLEAVE_EXPT_LEFT){
            optionflag |= LEFT_FIXATION_CHECK;
            extra = MONOC;
        }
        else if(stp->vals[0] == INTERLEAVE_EXPT_RIGHT){
            optionflag |= RIGHT_FIXATION_CHECK;
            extra = MONOC;
        }
        else if(stp->vals[0] == INTERLEAVE_EXPT_COMPONENT_ONE && stp->vals[1] < 0.5 && expt.type2 == SEED_DELAY)
            optionflag |= LEFT_FIXATION_CHECK;
        else if(stp->vals[0] == INTERLEAVE_EXPT_COMPONENT_ONE && fabs(remainder(stp->vals[1],1)) > 0.2 && expt.type2 == CONTRAST_PAIRS)
            optionflag |= LEFT_FIXATION_CHECK;
        else if(stp->vals[0] == INTERLEAVE_EXPT_COMPONENT_ONE)
            optionflag |= RIGHT_FIXATION_CHECK;
        
        setoption();
        clear_display(1);
        SerialSend(OPTION_CODE);
    }
    
    /*
     * Here is where the expt 2 values are set.
     */
    if(rdspair(expt.st)){
        i = 0;  //dummy, for debugger
    }
    if(stimres >= 0 && stp->expno != ONLY_ONE_EXPT)
    {
        if(stp->vals[1] == INTERLEAVE_EXPT_SFONE){
            if(expt.type2 == CONTRAST_RATIO)
                SetProperty(&expt, expt.st, CONTRAST_RATIO, 0.000001);
            else{
                prevstim = STIM_GRATING2;
                StimulusType(expt.st,STIM_GRATING);
                SetProperty(&expt, expt.st, SF, expt.stimvals[SF]);
                SetProperty(&expt, expt.st, TF, expt.stimvals[TF]);
                SerialSend(STIMULUS_TYPE_CODE);
            }
        }
        else if(stp->vals[1] == INTERLEAVE_EXPT_SFTWO){
            if(expt.type2 == CONTRAST_RATIO)
                SetProperty(&expt, expt.st, CONTRAST_RATIO, 100000);
            else{
                prevstim = STIM_GRATING2;
                StimulusType(expt.st,STIM_GRATING);
                SetProperty(&expt, expt.st, SF, expt.stimvals[SF2]);
                SetProperty(&expt, expt.st, TF, expt.stimvals[TF2]);
                SerialSend(STIMULUS_TYPE_CODE);
            }
        }
        else if(expt.type2 == JVELOCITY && stp->vals[1] == INTERLEAVE_EXPT){
            prevstim = STIM_RDS;
            StimulusType(expt.st, STIM_GRATING);
            SerialSend(STIMULUS_TYPE_CODE);
        }
        else if(expt.type2 == OPPOSITE_DELAY && stp->expno != EXP_SECOND)
            ;
        else if(expt.type2 == PURSUIT_INCREMENT){
            
            if(pursuedir < 0 && lastpi != expt.vals[PURSUIT_INCREMENT])
                val = GetProperty(&expt,expt.st,expt.type2);
            fprintf(seroutfile,"PE(%d) %.2f, %.2f lastd %.2f, lastpi %.2f, PD %.0f\n",stimno,expt.vals[PURSUIT_INCREMENT], stp->vals[1], lastdir, lastpi, pursuedir);
            expt.vals[PURSUIT_INCREMENT] = stp->vals[1];
            lastdir = expt.vals[PURSUIT_INCREMENT] * pursuedir;
            stimres = SetProperty(&expt, expt.st, expt.type2, stp->vals[1]);
            stp->vals[1] = GetProperty(&expt,expt.st,expt.type2);
            if(expt.vals[PURSUIT_INCREMENT] * pursuedir > 0 && stp->vals[1] < 0)
                val = GetProperty(&expt,expt.st,expt.type2);
            if( pursuedir < 0){
                if(expbvals[stim2order[stimno]] < 0)
                    val = GetProperty(&expt,expt.st,expt.type2);
                else
                    val = GetProperty(&expt,expt.st,expt.type2);
                
            }
            lastpi = expt.vals[PURSUIT_INCREMENT];
        }
        else if(extra != UNCORRELATE || expt.type2 != CORRELATION){
            stimres = SetProperty(&expt, expt.st, expt.type2, stp->vals[1]);
            stp->vals[1] = GetProperty(&expt,expt.st,expt.type2);
        }
        else if(extra == MONOC && expt.type2 == HEIGHT_R){
            stimres = SetProperty(&expt, expt.st, STIM_HEIGHT, stp->vals[1]);
            SetProperty(&expt, expt.st, LENGTH_DISP, 0);
            stp->vals[1] = GetProperty(&expt,expt.st,STIM_HEIGHT);
        }
    }
    if(stimres < 0)
        fprintf(stderr,"Ex2StimError %d %.4f\n",expt.type2,stp->vals[1]);
    
    
    if(stp->expno == EXP_FIRST && stimres >= 0 && stp->vals[0] > INTERLEAVE_EXPT
       && extra != HIGH)
        stimres = SetProperty(&expt, expt.st,expt.mode,stp->vals[0]+fakestim);
    
    /*
     * set it here in case something has changed becuase of expt values. E.G. if COVARY_XPOS has
     * changed, need to re-set xpos
     */
    SetSacVal(psychval,expt.stimid);
    
    if(expt.type2 == PURSUIT_INCREMENT && altstimmode == MOVE_STIM_ONLY){
        SetProperty(&expt, expt.st, SETZXOFF, -36 * stp->vals[1]);
    }
    
    if(stimres < 0)
        fprintf(stderr,"Ex1(b)StimError %d %.4f\n",expt.mode,stp->vals[0]);
    
    /*
     * set phase after setting other props, in case a value was changed above
     * that affects phase (e.g. vals[START_PAHSE]). These will then be reflected
     * here.
     */
    if(optionflags[RANDOM_RELPHASE]){
        if(expt.st->type == STIM_GRATING2){
            rnd = myrnd_i() % 4;
            rnd = rnd * 90;
            SetStimulus(expt.st,(float)rnd,SETPHASE,NULL);
            rnd = myrnd_i() % 6;
            rnd = rnd * 30;
            SetStimulus(expt.st,(float)rnd,PHASEREL,NULL);
        }
        else{
            rnd = myrnd_i() % 4;
            rnd = rnd * 90;
            SetStimulus(expt.st,(float)rnd,SETPHASE,NULL);
            /*
             * for psychophysiccs with 2 gabors, use 360 different phase values.
             */
            if(expt.st->next != NULL && expt.st->next->type == STIM_GABOR){
                rnd = myrnd_i() % 360;
                SetStimulus(expt.st,(float)rnd,SETPHASE,NULL);
            }
        }
    }
    else{
        SetStimulus(expt.st,expt.st->vals[START_PHASE],SETPHASE,NULL);
        if(optionflags[MODULATE_DISPARITY] == 1){
            expt.st->modphase = deg_rad(st->vals[START_PHASE]);
            if(st->type == STIM_GRATING)
                st->phasedisp[0] = cos(st->modphase) * st->depth_mod;
            else if(st->type == STIM_RDS)
                st->disp = expt.st->meandisp + st->tenvelope * deg2pix(cos(st->modphase) * st->depth_mod)/2;
        }
    }
    
    /*
     * This needs to be done after setting exp 1 and 2, otherwise phase gets reset
     */
    if(expt.st->type == STIM_SQCORRUG && (expt.mode == ORIENTATION && expt.type2 == START_PHASE) || (expt.mode == START_PHASE && expt.type2 == ORIENTATION)){
        if(extra == ZERO){
            SetStimulus(expt.st, 90, START_PHASE, NULL);
            SetStimulus(expt.st, expt.mean, ORIENTATION, NULL);
            SetStimulus(expt.st,90,SETPHASE,NULL);
        }
        else if (extra == HIGH){
            SetStimulus(expt.st, 270, START_PHASE, NULL);
            SetStimulus(expt.st, expt.mean, ORIENTATION, NULL);
            SetStimulus(expt.st,270,SETPHASE,NULL);
        }
    }
    
    if(rdspair(expt.st)){
        i = 0;  //dummy, for debugger
    }
    expt.stp = stp;
    expt.currentval[0] = stp->vals[0];
    expt.currentval[1] = stp->vals[1];
    expt.currentval[2] = stp->vals[EXP_THIRD];
    expt.currentval[3] = StimulusProperty(expt.st,frameparams[1]);
    expt.currentval[4] = StimulusProperty(expt.st,frameparams[2]);
    
    
    
    if(optionflags[TILE_XY] && !optionflags[FAST_SEQUENCE]){
        rnd = myrnd_i();
        xrnd = rnd &3;
        yrnd = (rnd >4) & 3;
        val = (xrnd-1.5) * expt.stimvals[STIM_WIDTH];
        SetProperty(&expt, expt.st, SETZXOFF, val + stimpos[0]);
        val = (yrnd-1.5) * expt.stimvals[STIM_HEIGHT];
        SetProperty(&expt, expt.st, SETZYOFF, val + stimpos[1]);
    }
    /* 
     * DON't TRY RANDOM X when changeing PA - stimvals SETZXOFF is not
     * necessarily the actual stimulus X
     */
    if(expt.vals[RANDOM_X] > 0.0001)
    {
        
        rval = mydrand();
        rval = (rval *2) - 1;
        val = expt.vals[RANDOM_X] * rval;
        if(expt.type2 == STIM_POLARANGLE)
            xval = GetProperty(&expt, expt.st, SETZXOFF);
        else
            xval = expt.stimvals[SETZXOFF];
        SetProperty(&expt, expt.st, SETZXOFF, val + xval);
    }
    if(stimres < 0) /* error setting stimulus */
    {
        expt.st->type = expt.stimtype;
        if(expt.st->next != NULL)
            expt.st->next->type = expt.backtype;
        return(STIM_ERROR);
    }
    else if(show)/* send list of stimvariables to PC*/
        for(i = 0; i < expt.lastserialcode; i++){
            code = valstrings[i].icode;
            if(codesend[code] == SEND_STIMULUS || code == expt.mode || code == expt.type2)
                SerialSend(code);
            else if(codesend[code] == SEND_NON_ZERO && GetProperty(&expt,expt.st,code) != 0)
                SerialSend(code);
            else if (code == JVELOCITY && expt.st->type == STIM_CYLINDER)
                SerialSend(code);
        }
    CheckPsychVal(stp);
    SetSacVal(stp->vals[EXP_PSYCHVAL],expt.stimid);
    ShowStimVals(stp);
    if(afc_s.loopstate == CORRECTION_LOOP){
        SerialString("CLOOP\n",0);
    }
    
    penstimno = laststimno;
    laststimno = stimno;
    stimulus_is_prepared++;
    expt.stimvals[XPOS] = GetProperty(&expt,expt.st,XPOS);
    expt.stimvals[YPOS] = GetProperty(&expt,expt.st,YPOS);
    afc_s.vals[0] = stp->vals[0];
    afc_s.vals[1] = stp->vals[1];
    if(option2flag & STAIRCASE){
        if(stairfd){
            fprintf(stairfd," %d(%d,%d) %s\n",stimno,expt.stimno,stimorder[stimno],cbuf);
            fflush(stairfd);
        }
        else
            fprintf(stdout," %d(%d,%d) %s\n",stimno,expt.stimno,stimorder[stimno],cbuf);
    }
    
    
    
    if(expt.st->type == STIM_NONE)
        rcstimvals[5][0] = -1000;
    else
        rcstimvals[5][0] = GetProperty(&expt,expt.st,MONOCULARITY_EXPT);
    rcstimvals[0][0] = expt.currentval[0];
    rcstimvals[1][0] = expt.currentval[1];
    rcstimvals[2][0] = expt.currentval[2];
    rcstimvals[3][0] = expt.currentval[3];
    rcstimvals[4][0] = expt.currentval[4];
    if(rdspair(expt.st)){
        i = 0;  //dummy, for debugger
    }
    if(expt.vals[FAST_SEQUENCE_RPT] > 1 && expt.vals[FAST_SEQUENCE_RPT] < 60){
        brpt = frpt = (int)(expt.vals[FAST_SEQUENCE_RPT]);
        if(expt.vals[FASTB_RPT] > frpt)
            brpt = (int)(expt.vals[FASTB_RPT]);
        
    }
    else
        frpt = 1;
    
    if(expt.vals[RC_REPEATS] > 0){
        val = (float)(unrepeatn[expt.stimid])/(unrepeatn[expt.stimid]+uncompleted[expt.stimid]);
        if(unrepeatn[expt.stimid] <= 0 || rcrpt == 0)
            val = 0;
#ifdef WATCHSEQ
        printf("Stim %d not done %d, unrep %d, val %.3f (%d)",expt.stimid,uncompleted[expt.stimid],unrepeatn[expt.stimid],val,stimno);
#endif
        if(mydrand() < val){
            id = myrnd_i() %  unrepeatn[expt.stimid];
            dorpt = unrepeated[expt.stimid][id];
            lastid = unrepeated[expt.stimid][unrepeatn[expt.stimid]-1];
            /*
             * put the one to be repeated at the end of the list. If the trial is completed
             * then decrementing unrepeatn will remove it.
             */
            unrepeated[expt.stimid][unrepeatn[expt.stimid]-1] = dorpt;	    
            unrepeated[expt.stimid][id] = lastid;
            if(seroutfile){
                if(dorpt == 0){
                    //		printf("stimid%d: %d/%d, Unrep = ",expt.stimid,id, unrepeatn[expt.stimid]);
                    for(i = 0; i < unrepeatn[expt.stimid]; i++)
                        fprintf(seroutfile," %d",unrepeated[expt.stimid][i]);
                    fprintf(seroutfile,"\n");
                    fprintf(seroutfile,"stimid%d: %d/%d, Unrep %d\n",expt.stimid,id, unrepeatn[expt.stimid],unrepeated[expt.stimid][0]);
                    
                }
                fprintf(seroutfile,"Repeat exval %d, %d (#%d, id%d)\n",expt.stimid,dorpt,stimno,id);
            }
        }
        else
            dorpt = 0;
        
        /*
         * dorpt = 2 for testing presetting the seed sequence
         */
        dorpt = 2;
        if(dorpt == 2){
            expt.st->left->baseseed = seedorder[stimno];
        }
        else if(dorpt){
#ifdef WATCHSEQ
            printf("repeating %d id%d\n",dorpt,id);
#endif
            /*
             * currenstim.lastseed should always be the biggest yet. If not, probably doing 2 repeats in a row
             * so don't copy it
             */
            if(expt.st->left->baseseed > currentstim.lastseed)
                currentstim.lastseed = expt.st->left->baseseed;
            expt.st->left->baseseed = dorpt;
        }
        else{
            expt.st->left->baseseed+=2;
#ifdef WATCHSEQ;	   
            printf("firsttime %d\n",expt.st->left->baseseed);
#endif
        }
        if(expt.st->left->baseseed > maxseed)
            maxseed = expt.st->left->baseseed;
        if (expt.st->left->baseseed < maxseed && dorpt == 0){
            fprintf(seroutfile, "Seed back from %d ot %d\n",maxseed,expt.st->left->baseseed);
            printf("Seed Seq error\n");
            maxseed = expt.st->left->baseseed;
        }
        fprintf(seroutfile,"se%d*\n",expt.st->left->baseseed);
        myrnd_init(expt.st->left->baseseed);
        srand48(expt.st->left->baseseed);
        currentstim.seqseed = expt.st->left->baseseed;
    }
    
    /*
     * optionflags[FAST_SEQUENCE] runs a random sequence of stimuli, but without
     * writing es/bs markers, or sending any serial output.
     * 
     * if expt.type2 == DISTRIBUTION_WIDTH, then the distance spanned by expt.incr
     * is divided into expt.mean2 bins. If stp->vals[1] > expt.mean2, this means
     * that that the range of stimuli is reduced, moving the mean in the direction of the 
     * null stimulus (stim 1 of expt 1). If st->vals[1] < expt.mean2, the range
     * is reduced, moving the mean towards stim0 of expt1
     *
     * beware nasty kludge to handle Orientation distributions, where the stimulus
     * parameters are circular. Truncation of (expt.nstim[1]-1)/2 means the
     * max nvals is always even. 
     */
    
    if (expt.type3 == SEEDOFFSET && expt.stimmode == IMAGETRIALMIX){
        if (expt.st->seedoffset == 0 && expt.vals[DISTRIBUTION_WIDTH] > 1){
            i = myrnd_i() % (int)(expt.vals[DISTRIBUTION_WIDTH]);
            i = (i+expt.vals[DISTRIBUTION_MEAN]);
            expt.st->seedoffset = i * 1000;
            SerialSend(SEEDOFFSET);
        }
    }
        
    
    if(optionflags[FAST_SEQUENCE] && expt.mode == STIM_PULSES){
        expt.fasttype = CORRELATION;
        resetval[CORRELATION] = 1;
        for(i = 0; i < expt.st->nframes; i++)
        {
            frameseq[i] = 0;
        }
        for(i = 0; i < expt.vals[STIM_PULSES]; i++){
            nv = rint(mydrand() * (expt.st->nframes - expt.vals[PULSE_WIDTH]));
            for(j = nv; j < nv+expt.vals[PULSE_WIDTH]; j++)
                frameseq[j] = 1;
        }
    }
    else if(optionflags[FAST_SEQUENCE] && expt.mode == PULSE_WIDTH){
        resetval[expt.type2] = 1;
        expt.fasttype = expt.type2;
        for(i = 0; i < expt.st->nframes; i++)
        {
            frameseq[i] = 0;
        }
        nv = (expt.st->nframes - expt.vals[PULSE_WIDTH])/2;
        for(j = nv; j < nv+expt.vals[PULSE_WIDTH]; j++)
            frameseq[j] = stp->vals[1];
    }
    else if(expt.vals[DISTRIBUTION_CONC] > 0 && (nv = expt.vals[DISTRIBUTION_WIDTH]) > 1
            && expt.mode == PLAID_RATIO)
    {
        optionflags[FAST_SEQUENCE] = 1;
        inc = expt.vals[RC1INC];
        minval = expt.vals[DISTRIBUTION_MEAN] - ((nv-1) * inc/2);
        expt.fasttype = expt.mode;
        expt.fastbtype = ORIENTATION;
        expt.fastctype = EXPTYPE_NONE;
        for(i = 0; i < expt.st->nframes+1; i++){
            rval = mydrand();
            //use EXP_SPCYHVAL to determine whehter to add interleaves.  In case mean not zero
            if(rval > expt.vals[DISTRIBUTION_CONC] && fabs(stp->vals[SIGNAL_STRENGTH]) < expt.vals[INITIAL_APPLY_MAX]){
                lrnd = myrnd_i();
                frameseq[i] = 1; // full contrast for grating 1
                frameseqb[i] = minval + (lrnd%nv) * inc;
                frameiseq[i] = 1+(lrnd%nv);
                dframeseq[i] = 1;
            }
            else{
                frameseqb[i] = expt.stimvals[ORIENTATION];
                frameseq[i] = stp->vals[0];
                frameiseq[i] = 0;
                dframeseq[i] = 1;
            }
            rcstimid[i] = frameiseq[i];
        }
        expt.nfast = nv;
        expt.minval = minval;
        expt.fastincr = inc;
    }
    else if(expt.vals[DISTRIBUTION_CONC] > 0 && (nv = expt.vals[DISTRIBUTION_WIDTH]) > 1
            &&  expt.mode == CONTRAST_DIFF)
    {
        optionflags[FAST_SEQUENCE] = 1;
        inc = expt.vals[RC1INC];
        minval = expt.vals[DISTRIBUTION_MEAN] - ((nv-1) * inc/2);
        expt.fasttype = expt.mode;
        expt.fastbtype = ORIENTATION;
        expt.fastctype = expt.type2;
        for(i = 0; i < expt.st->nframes+1; i++){
            rval = mydrand();
            if(rval > expt.vals[DISTRIBUTION_CONC] && fabs(stp->vals[SIGNAL_STRENGTH]) < expt.vals[INITIAL_APPLY_MAX]){
                lrnd = myrnd_i();
                frameseq[i] = 0; // same ori in each eye
                frameseqb[i] = minval + (lrnd%nv) * inc;
                frameseqc[i] = 0; //same contrast in each eye
                frameiseq[i] = 1+(lrnd%nv);
                dframeseq[i] = 1;
            }
            else{
                frameseqb[i] = expt.stimvals[ORIENTATION];
                frameseq[i] = stp->vals[0];
                frameseqc[i] = stp->vals[1];
                frameiseq[i] = 0;
                dframeseq[i] = 1;
            }
            rcstimid[i] = frameiseq[i];
        }
        expt.nfast = nv;
        expt.minval = minval;
        expt.fastincr = inc;
    }
    else if(optionflags[FAST_SEQUENCE]){
        expt.fastbtype = EXPTYPE_NONE;
        expt.fastctype = EXPTYPE_NONE;
        nvals = expt.mean2 + expt.incr2 * (float)(expt.nstim[1]-1)/2;
        nv = stp->vals[1];
        memcpy(nstim,expt.nstim,sizeof(expt.nstim));
        expt.fasttype = expt.mode;
        if(expt.type3 == SETCONTRAST){ // A special fast seq
            nv = nvals = expt.vals[DISTRIBUTION_WIDTH]; 
            inc = expt.vals[RC1INC];
            expt.fastextras = 0;
            expt.fasttype = SETCONTRAST;
            expt.fastbtype = EXPTYPE_NONE;
        }
        else if(expt.type2 == TEMPORAL_ORI_BANDWIDTH){
            nvals = nvals &~ 0x1; /* so 19 -> 18 */
            inc = (2 * expt.incr/nvals);
            inc = expt.vals[RC1INC];
            if(nv * inc >= 180)
                nv = (180/inc);
        }
        else if(expt.type2 == DISTRIBUTION_WIDTH){
            if(stp->stimno[0]) /* stim 1 or higher */
                inc = -expt.vals[RC1INC];
            //	      inc = -expt.incr/(nvals-1);
            else  // stim 0
                //	      inc = expt.incr/(nvals-1);
                inc = expt.vals[RC1INC];
        }
        else if(expt.type2 == DISTRIBUTION_CONC){
            inc = expt.vals[RC1INC];
            if((nv = expt.vals[DISTRIBUTION_WIDTH]) <= 0)
                nv = 2;
            minval = expt.vals[DISTRIBUTION_MEAN] - ((nv-1) * inc/2);
        }
        else if(expt.mode == DISTRIBUTION_CONC){
            inc = expt.vals[RC1INC];
            if((nv = expt.vals[DISTRIBUTION_WIDTH]) <= 0)
                nv = 2;
            minval = expt.vals[DISTRIBUTION_MEAN] - ((nv-1) * inc/2);
            expt.fasttype = expt.type2;
        }
        else if(expt.type2 != EXPTYPE_NONE && expt.nstim[1] > 1){
            nv = expt.nstim[0] * expt.nstim[1]; //nstim[2] added below
            expt.fasttype = expt.mode;
            expt.fastbtype = expt.type2;
        }
        else
            nv = expt.nstim[0]; //nstim[2] added below
        expt.nfast = nv;
        expt.minval = minval;
        expt.fastincr = inc;
        
        
        nstim[2] = expt.fastextras;
        nstim[3] = expt.nstim[3] + expt.fastextras;
        for(i = 0; i < 5; i++)
            expt.fastnstim[i] = nstim[i];
        
        mean = 0;
        if(expt.vals[FAST_SEQUENCE_RPT] > 1 && expt.vals[FAST_SEQUENCE_RPT] < 60){
            brpt = frpt = (int)(expt.vals[FAST_SEQUENCE_RPT]);
            if(expt.vals[FASTB_RPT] > frpt)
                brpt = (int)(expt.vals[FASTB_RPT]);
            
        }
        else
            frpt = 1;
        /*
         * FLASH_BIT and FAST_SEQ sets up a repeating pattern of stimuli
         * useful to allow interuption of prdicable sequences
         */
        if(option2flag & FLASH_BIT){
            iperiod = rint(mon.framerate/expt.st->tf);
            ncycles = expt.st->nframes/iperiod;
            
            drop = ncycles/2 + myrnd_i() % (ncycles/2);
            for(i = 0; i < expt.st->nframes; i++)
            {
                lrnd = i%iperiod <= frpt-1 ? 1 : 0;
                if(lrnd & i/iperiod == drop){
                    lrnd = 0;
                    if(i%iperiod == 0) // first frame
                        framecseq[i] = 1;
                    else
                        framecseq[i] = 0;	      
                }
                else
                    framecseq[i] = 0;
                SetFrameStim(i, lrnd, inc, stp, nstim);
                rcstimid[i] = frameiseq[i];
            }
            
        }
        else{
            if(frpt < 1) // make sure, otherwise this blocks!
                frpt = 1;
            id = 0;
            for(i = 0; i < expt.st->nframes+1; i+= frpt){
                //	    lrnd = rnd_ri((long)(nv + expt.fastextras));
                lrnd = myrnd_i();
                SetFrameStim(i, lrnd, inc, stp, nstim);
                ivals[id] = frameiseq[i];
                bvals[id++] = frameseqb[i];
                for(j = 0; j < frpt; j++)
                {
                    k = i+j;
                    if(k < MAXFRAMES){
                        rcstimvals[0][k] = frameseq[i];
                        rcstimvals[1][k] = frameseqb[i];
                        rcstimid[k] = frameiseq[i];
                        frameseq[k] = frameseq[i];
                        if(expt.mode == BACKGROUND_MOVIE){
                            frameseed[k] = frameseq[i];
                            frameseq[k] = frameseq[i]+j/2;;
                            rcstimid[k] = -10000;
                            rcstimid[i] = frameiseq[i];
                            rcstimvals[0][k] = frameseq[k]+j/2;
                        }
                        else if(expt.mode == BACKGROUND_IMAGE){
                            frameseed[k] = frameseq[i];
                            frameseq[k] = frameseq[i];
                            rcstimid[k] = -10000;
                            rcstimid[i] = frameiseq[i];
                            rcstimvals[0][k] = frameseq[k];
                        }
                        frameseqb[k] = frameseqb[i];
                        mean += frameseq[i];
                        dframeseq[k] = 0;
                    }
                }
                dframeseq[i] = 1;
                
                
            }
            /*
             * if this is zero signal Dc Trial, and a Pulse Width is set, include a pulse
             * in the sequence, at the signal disparity
             */
            if(expt.mode == DISTRIBUTION_CONC && 
               expt.vals[PULSE_WIDTH] > 1 && fabs(stp->vals[0]) < 0.0001 ){
                k = 0;
                pw = (int)(expt.vals[PULSE_WIDTH]+0.5);
                /* first, find the signal frames that are already set */
                for(i = 0; i < expt.st->nframes-pw; i++)
                {
                    if(fabs(frameseq[i]-stp->vals[1]) < 0.0001){
                        sigframes[k++] = i;
                        isig = frameiseq[i];
                    }
                }
                rval = mydrand();
                if(seroutfile)
                    fprintf(seroutfile,"#rnd%.5f %ld\n",rval,expt.st->left->baseseed);
                
                if(rval <= 0.4){ // add a pulse
                    i = (lrnd >> 16) % k;
                    for(k = sigframes[i]+1; k < sigframes[i]+pw; k++)
                    {
                        rcstimid[k] = frameiseq[k] = 0;
                        frameseq[k] = stp->vals[1];
                    }
                    
                }
                else if (rval <= 0.8){ // add extra frames;
                    i = 0;
                    while(i < pw-1){
                        k = lrand48() % expt.st->nframes;
                        if(frameiseq[k] != 0 && frameiseq[k] != isig){
                            rcstimid[k] = frameiseq[k] = 0;
                            frameseq[k] = stp->vals[1];
                            i++;
                        }
                    }
                }
            }
            
            
            rcstimid[i] = -1;
            if(brpt > frpt){
                id = 0;
                for(i = 0; i < expt.st->nframes; i+=brpt){
                    for(j = 0; j < brpt; j++)
                    {
                        k = i+j;
                        if(k < MAXFRAMES){
                            rcstimvals[1][k] = bvals[id];
                            frameseqb[k] = bvals[id];
                            frameiseqb[k] = ivals[id];
                        }
                    }
                    id++;
                }
            }
            else if(expt.type2 == BACKGROUND_MOVIE){
                startf = expt.vals[BACKGROUND_MOVIE];
                for(i = 0; i < expt.st->nframes; i++){
                    rcstimvals[1][i] = i+startf;
                    frameseqb[i] = i+startf;
                    frameiseqb[i] = i+startf;
                    for (k = 1; k < frpt; k++){
                        rcstimvals[1][i++] = i+startf;
                        frameseqb[i] = i+startf;
                        frameiseqb[i] = i+startf;
                    }
                }
            }
        }
        
        
        mean /= expt.st->nframes;
        if(seroutfile && verbose){
            fprintf(seroutfile,"#Seqmean %.5lf",mean);
            for(i = 0; i < expt.st->nframes; i++)
                fprintf(seroutfile," %.2f",frameseq[i]);
            fprintf(seroutfile,"\n");
            
        }
    }
    
    if(expt.type2 == BACKGROUND_MOVIE){
        startf = expt.vals[BACKGROUND_MOVIE];
        if (expt.st->seedoffset > 0 && expt.vals[DISTRIBUTION_WIDTH] > 1){
            i = myrnd_i() % (int)(expt.vals[DISTRIBUTION_WIDTH]);
            i = (i+expt.vals[DISTRIBUTION_MEAN]);
            expt.st->seedoffset = expt.st->seedoffset * i;
            expt.vals[BACKGROUND_MOVIE] = 0; // store ims in lowest slots
            sprintf(cbuf,"Movie %d",expt.st->seedoffset);
            glstatusline(cbuf,1);
        }
        else
            expt.st->seedoffset = startf;
        for(i = 0; i < expt.st->nframes; i++){
            imid = i/frpt+1;
            rcstimvals[1][i] = imid;
            frameseqb[i] = imid;
            frameiseqb[i] = imid;
            for (k = 1; k < frpt; k++){
                rcstimvals[1][i++] = imid;
                frameseqb[i] = imid;
                frameiseqb[i] = imid;
            }
        }
        SerialSend(SEEDOFFSET);
// Do this after setting sequence in case changed seedoffset
        LoadBackgrounds();
    }
    if(expt.mode == BACKGROUND_MOVIE){
        startf = expt.vals[BACKGROUND_MOVIE];
        for(i = 0; i < expt.st->nframes; i++){
            imid = i+startf;
            rcstimvals[1][i] = imid;
            frameseqb[i] = imid;
            frameiseqb[i] = imid;
            for (k = 1; k < frpt; k++){
                rcstimvals[1][i++] = imid;
                frameseqb[i] = imid;
                frameiseqb[i] = imid;
            }
        }
        SerialSend(SEEDOFFSET);
    }
    
    if(expt.stimvals[PLC_MAG] < 1){
        
        if(fabs(val = StimulusProperty(expt.st, DISP_X)) < expt.vals[INITIAL_APPLY_MAX])
            expt.vals[PLC_MAG] = 1;
        else
            expt.vals[PLC_MAG] = expt.stimvals[PLC_MAG];
    }
    
    
    if(expt.st->next != NULL && expt.st->next->type == STIM_IMAGE){
        st->next->imprefix = st->imprefix;
        st->next->preload = st->preload;
        st->next->imprefix = st->imprefix;
        st->next->immode = st->immode;
        st->next->left->calculated = st->next->right->calculated = 0;
    }
    
    
    
    if(optionflags[CALCULATE_ONCE_ONLY] || (expt.st->type == STIM_IMAGE && !expt.st->preload))
        calc_stimulus(expt.st);
    if(rdspair(expt.st)){
        i = 0;  //dummy, for debugger
    }
    
    /*
     * if its a fast sequence, then it will have to take care of
     * interleaving blanks, uncorrs
     */
    if(optionflags[FAST_SEQUENCE]){
        if(stimorder[stimno] & STIMULUS_EXTRA_BLANK){
            expt.st->type = realstim[0];
            if(expt.st->next)
                expt.st->next->type = realstim[1];
            SerialSend(STIMULUS_TYPE_CODE);
        }
        if(stimorder[stimno] & STIMULUS_EXTRA_UNCORRELATE){
            expt.st->flag &= (~UNCORRELATE);
            if(expt.st->next)
                expt.st->next->flag &= (~UNCORRELATE);
        }
    }
    if(expt.st->type == STIM_IMAGE  && expt.st->preload){
        gettimeofday(&then,NULL);
        expt.st->preloaded = 0;

        frpt = expt.vals[FAST_SEQUENCE_RPT];
        if (optionflags[RAND_FP_DIR]){
            expt.vals[FP_MOVE_DIR] = drand48() * M_PI * 2;
            if(expt.st->jumps)
                SerialSend(FP_MOVE_DIR);
        }
        
        if (frpt < 1 || optionflags[FAST_SEQUENCE] == 0  && expt.st->immode != IMAGEMODE_ORBW)
            frpt = 1;
        for(i = 0; i < expt.st->nframes+1; i+=frpt){
            if(optionflags[FAST_SEQUENCE]){
                if(expt.fastbtype != EXPTYPE_NONE)
                    SetStimulus(expt.st,frameseqb[i],expt.fastbtype,NOEVENT);
                SetStimulus(expt.st,frameseq[i],expt.fasttype,NOEVENT);
            }
            else
                frameseq[i] = 0;
            if (expt.st->jumps > 0){
                nv = expt.st->jumps/2;
                if(optionflags[TILE_XY]){
                    rnd = myrnd_i();
                    rcstimxy[0][i] = (rnd & 0xffff) % expt.st->jumps;
                    rcstimxy[0][i] = (rnd) % expt.st->jumps;
                    rcstimxy[1][i] = (rnd>>16) % expt.st->jumps;
                    expt.st->xyshift[0] = (rnd % expt.st->jumps - nv) * expt.vals[FP_MOVE_SIZE];
                    expt.st->xyshift[1] = ((rnd>>16) % expt.st->jumps - nv) * expt.vals[FP_MOVE_SIZE];
                }
                else{
                    laps = floor(i / expt.st->jumps );
                    expt.st->xyshift[0] = laps * expt.vals[FP_MOVE_SIZE] * cos(expt.vals[FP_MOVE_DIR]);
                    expt.st->xyshift[1] = laps * expt.vals[FP_MOVE_SIZE] * sin(expt.vals[FP_MOVE_DIR]);
                    if ((i-expt.vals[FAST_SEQUENCE_RPT] > laps * expt.st->jumps) && expt.vals[FAST_SEQUENCE_RPT] > 0){
                        expt.st->xstate = INTERLEAVE_EXPT_BLANK; 
                        frameseq[i] = INTERLEAVE_EXPT_BLANK;
                    }
                    else {
                        expt.st->xstate = 0;
                        frameseq[i] = 0;
                    }
                }
            }
            expt.st->forceseed = 0; 
            st->framectr = i;
            st->left->calculated = st->right->calculated = 0;
            calc_image(expt.st,expt.st->left);
            if(expt.st->flag & UNCORRELATE)
                calc_image(expt.st,expt.st->right);

            for (j = 1; j < frpt; j++){
                st->framectr = i+j;
                st->left->calculated = st->right->calculated = 0;
                if (expt.st->immode == IMAGEMODE_ORBW)
                    expt.st->forceseed = expt.st->stimid; 
                calc_image(expt.st,expt.st->left); 
                imageseed[i+j] = imageseed[i];
                rcstimxy[0][i+j] = rcstimxy[0][i];
                rcstimxy[1][i+j] = rcstimxy[1][i];
            }
            if(expt.st->next != NULL && expt.st->next->type == STIM_IMAGE){
                st->next->preloaded = 0;
                st->next->imprefix = st->imprefix;
                st->next->preload = st->preload;
                st->next->imprefix = st->imprefix;
                st->next->immode = st->immode;
                st->next->left->calculated = st->next->right->calculated = 0;
                calc_image(expt.st->next,expt.st->next->left);
            }
        }
        expt.st->preloaded = expt.st->nframes;
        expt.st->xstate = 0;
        if(expt.st->next != NULL && expt.st->next->type == STIM_IMAGE)
            expt.st->next->preloaded = expt.st->nframes;
        
        expt.st->framectr = 0;
        gettimeofday(&now, NULL);
        if(seroutfile)
            fprintf(seroutfile,"preload took %.4f\n",timediff(&now,&then));
        //Ali #ifdef macosx
        sprintf(cbuf,"imve %.4f,%d %.4f %.4f %s %d\n",st->stimversion,st->seedoffset,st->pix2deg,timediff(&now,&then),st->builddate, st->envelopetype);
        SerialString(cbuf,0);
        seedoffsets[expt.stimid] = st->seedoffset;
        expt.st->forceseed = 0;
        //Ali #endif
    }
    else  if(expt.st->type == STIM_IMAGE){
        expt.st->prepared = 0;
        calc_stimulus(expt.st);
        expt.st->prepared = 1;
        //Ali #ifdef macosx
        sprintf(cbuf,"imve %.4f,%d\n",st->stimversion,st->seedoffset);
        SerialString(cbuf,0);
        //Ali #endif
    }
    else  if(expt.st->type == STIM_RDS){
        if(optionflags[RANDOM_DEPTH_PHASE])
            SetDotDistribution();
        precalc_rds_disps(expt.st);
    }
    if (expt.type2 != EXPTYPE_NONE && expt.nstim[1] > 1)
        val = stp->vals[1];
    else
        val = 0;
    
    sprintf(cbuf,"exvals %.4f %.4f %.4f %d\n",stp->vals[0],stp->vals[1],stp->vals[EXP_THIRD],expt.st->left->baseseed);
    SerialString(cbuf,0);
    notify(cbuf);
    
    /*
     * if interleaving target offsets, change color and rewards accordingly.
     */
    if(expt.type3 == TARGET_OFFSET){
        if(stp->vals[2] > 0){
            expt.targetcolor = 1.0;
        }
        else{
            expt.targetcolor = 0;
        }
    }
    else
        expt.targetcolor = 1;
    
    if(seroutfile){
        fprintf(seroutfile,"##prep%c\n",InExptChar);
    }
    return(stimno);
}

/*
 * If stimtype has been changed for interleaving a blank or uncorrelated
 * stimulus, reset to the previous type
 * Always called _AFTER_ stimno has been incremented, so stimno-1 is the 
 * stimulus that was just run
 */

void ResetExpStim(int offset)
{
    int stim = stimno -1 + offset;
    int i;
    
    /*
     * don't reset if not in an expt
     * NB stimno can be 0 here in an expt, if the first trial is a badfix
     *used to be && exptpending but seems to me (Mar 2012) should be ||
     */
    stimulus_is_prepared = 0;
    if(stim < 0 || !(expt.st->mode | (EXPTPENDING)))
        return;
    if(stimorder[stim] & STIMULUS_EXTRA_ZEROCOH){
        SetStimulus(expt.st, expt.stimvals[JDEATH], JDEATH, NULL);
        SetStimulus(expt.st, expt.stimvals[SET_SEEDLOOP], SET_SEEDLOOP, NULL);
        expt.st->flag = expt.oldstimflag;
    }
    if(stimorder[stim] & STIMULUS_EXTRA_ZERO &&
       expt.mode != DISP_A && expt.mode != DISP_B)
        SetStimulus(expt.st, expt.stimvals[DEPTH_MOD], DEPTH_MOD, NULL);
    
    if(stimorder[stim] & STIMULUS_EXTRA_ZERO && expt.mode == ORI2)
        expt.vals[CHANGE_SEED] = expt.stimvals[CHANGE_SEED];
    if(stimorder[stim] & STIMULUS_EXTRA_FPJUMP)
        optionflags[SIMULATE_FP_MOVE] = 1;
    /*
     * when STIMMODE == BUTTSEXPT leave up current backim between trials
     */
    if(expt.mode == BACKGROUND_MOVIE || expt.mode == BACKGROUND_IMAGE && expt.stimmode != BUTTSEXPT){
        expt.backim = backims[MAXBACKIM];
    }
    if(expt.st->type == STIM_NONE)
    {
        expt.st->type = expt.stimtype;
        SerialSend(STIMULUS_TYPE_CODE);
        if(expt.st->next != NULL)
            expt.st->next->type = expt.backtype;
    }
    if(stimorder[stim] & STIMULUS_EXTRA_HIGHSQ){
        //    expt.st->type = expt.stimtype;
        StimulusType(expt.st, expt.stimtype);
        SerialSend(STIMULUS_TYPE_CODE);
        SetStimulus(expt.st, expt.stimvals[DEPTH_MOD], DEPTH_MOD, NULL);
        SetStimulus(expt.st, expt.stimvals[SF], SF, NULL);
    }
    if(stimorder[stim] & STIMULUS_EXTRA_HIGHSF){
        SetStimulus(expt.st, expt.stimvals[SF], SF, NULL);
        optionflags[MODULATE_DISPARITY] = 0;
        SetStimulus(expt.st, expt.stimvals[MODULATION_F], MODULATION_F, NULL);
        SetStimulus(expt.st, expt.stimvals[DEPTH_MOD], DEPTH_MOD, NULL);
    }
    if(stimorder[stim] & STIMULUS_EXTRA_HIGH){
        SetStimulus(expt.st, expt.stimvals[expt.hightype], expt.hightype, NULL);
    }
    
    if((expt.st->type == STIM_CYLINDER || expt.st->type == STIM_RDS) && expt.vals[INITIAL_DISPARITY] != 0){
        expt.st->disp = olddisp;
        expt.st->flag = stimflag;
    } /* do this before resetting correlation */
    
    if((option2flag & INTERLEAVE_UNCORRELATED || optionflags[INTERLEAVE_UNCORR_ALL]) && (expt.st->flag & UNCORRELATE) || expt.mode == CORRELATION)
    {
        SetStimulus(expt.st, expt.stimvals[CORRELATION], CORRELATION, NULL);
        SetStimulus(expt.st, expt.stimvals[BACK_CORRELATION], BACK_CORRELATION, NULL);
    }
    for(i = 0; i < MAXTOTALCODES; i++){
        if(resetval[i]){
            SetStimulus(expt.st, expt.stimvals[i], i, NULL);
            resetval[i] = 0;
        }
    }
    
    /*
     * seem to need this call to send stimtype in subspace maps
     * where the nominal stimulus was blank.
     */
    
    if(expt.stimid - expt.nstim[2] < 0 && (option2flag & (INTERLEAVE_BLANK | INTERLEAVE_UNCORRELATED)))
        SerialSend(STIMULUS_TYPE_CODE);
    if(expt.type2 == JUMPTYPE || expt.mode == FP_MOVE_DIR || expt.mode == FP_MOVE_SIZE || fabs(expt.vals[PURSUIT_INCREMENT]) > 0.01){
        SetStimulus(expt.st, expt.stimvals[XPOS], XPOS, NULL);
        SetStimulus(expt.st, expt.stimvals[YPOS], YPOS, NULL);
    }
    if(prevstim >= 0){
        StimulusType(expt.st, prevstim);
        SetProperty(&expt, expt.st, STIM_WIDTH, expt.stimvals[STIM_WIDTH]);
        if(prevstim == STIM_GRATING2){
            SetProperty(&expt, expt.st, SF2, expt.stimvals[SF2]);
            SetProperty(&expt, expt.st, TF2, expt.stimvals[TF2]);
            SetProperty(&expt, expt.st, SF, expt.stimvals[SF]);
            SetProperty(&expt, expt.st, TF, expt.stimvals[TF]);
        }
        prevstim = -1;
        SerialSend(STIMULUS_TYPE_CODE);
    }
    if(optionflags[MICROSTIM]){
        optionflags[MICROSTIM] = 0;
        SerialSend(OPTION_CODE);
    }
    expt.st->disp = expt.st->meandisp;
    if(optionflags[MODULATE_DISPARITY])
        expt.st->ndisps = 0;
    expt.vals[PLC_MAG] = expt.stimvals[PLC_MAG];
    expt.vals[TARGET_RATIO] = expt.stimvals[TARGET_RATIO];
    if(oldvelocity > 0 && expt.st->left->ptr->velocity == 0 && expt.vals[CHANGE_SEED] > 0)
    {
        if(expt.st->type == STIM_CYLINDER){
            expt.st->left->ptr->velocity = oldvelocity;
            expt.st->flag = stimflag;
        }
        if(expt.st->type == STIM_RDS)
            expt.st->posinc = oldvelocity;
        expt.st->disp = olddisp;
    }
    
    
    if(expt.st->type == STIM_IMAGE && optionflags[FAST_SEQUENCE]){
        SetStimulus(expt.st,expt.stimvals[ORI_BANDWIDTH],ORI_BANDWIDTH,NULL);
    }
    stimulus_is_prepared = 0;
}

int ExpStimOver(int retval, int lastchar)
{
    int i;
    int ntotal;
    char c,buf[BUFSIZ];
 //   Expstim *es,*exs;
    struct plotdata *plot;
    
    if(expt.st->type == STIM_NONE){
        expt.st->type = expt.stimtype;
        if(expt.st->next != NULL)
            expt.st->next->type = expt.backtype;
        if(expt.st->type != STIM_NONE)
            SerialSend(STIMULUS_TYPE_CODE);
    }
    if(retval == 0){ // Stim completed successfully
        if(expt.st->type == STIM_RDS && expt.st->ndisps > 1){
            PrintRDSDispCounts(seroutfile);
        }
    }
    
    expt.laststim = expt.stimno;
    gotspikes = 0;
    if(expstate == END_STIM && fixstate != BAD_FIXATION)
    {
        sprintf(buf,"EndStim %d ss %d i %d!!",fixstate,stimstate,lastchar);
        i = CheckBW(END_STIM,buf);
        if(seroutfile){
            fprintf(seroutfile,"PR %p %d\n",expt.plot,expt.plot->nstim[5]);
        }
    }
    else if(optionflag & WAIT_FOR_BW_BIT && expstate == 0 && retval != BAD_TRIAL)
    {
        sprintf(buf,"END_STIM NOT SENT fs %d ss %d i %d!!",fixstate,stimstate,i);
        acknowledge(buf,NULL);
    }
    

    
    
    /* check here for serial input. Usually get spikes here */
    while((c = ReadSerial(0)) != MYEOF)
        i = GotChar(c);
    

    //	ResetExpStim();
    ntotal = expt.nstim[5] * expt.nreps;
    /*
     * in IFC mode, each trial has two stimuli, so there are
     * twice as many. But the reference is never shown with
     * itself, hence expt.nstim[3] -1
     */
    if(option2flag & IFC)
        ntotal = 2 * (expt.nstim[3]-1) * expt.nreps;
    if(expt.nreps == 0)
    {
        if((stimno +1) > (ntotal = expt.nstim[5] * ZEROBLOCKING))
        {
            setstimulusorder(0);
            stimno = 0;
        }
    }
    else if((stimno+1) >= ntotal && retval != BAD_TRIAL)
        expt.st->mode |= EXPT_OVER;
    CheckFix(); /* if was a bad trial, set the timeout */
    
    if(!(optionflag & FIXATION_CHECK) && !(optionflag & WAIT_FOR_BW_BIT))
        fixstate = RESPONDED;
    /*
     * If this is a re-run of an expt file on disk, read in some more from
     * that file to determine the next stimulus
     * May 2005. Best if this is done after returning control to the main
     * loop, otherwise can reach call to display next stimulus before this is over,
     * and it gets skipped.
     if(replay_expt)
     ReplayExpt(NULL);
     */
    SerialString("EndStim\n",0);

    return(retval);
}


int RunExptStimSeq(Stimulus *st, int nframes, int nstims, /* Ali Display */D)
{
    int finished = 0,j,i = 0, nreps, ntotal, retval =0;
    int framecount,rc;
    Substim *rds;
    char c,buf[BUFSIZ],tmp[BUFSIZ];
    float val;
//    Expstim *stim;
    struct plotdata *plot;
//    Expstim *es,*exs;
    int istim = 0;
    int framesperstim = 1;
    int preframes = 0;
    int postframes = 0;
    
    if(st->type == STIM_RDS)
        rds = st->left;
    
    for(i = 0; i <MAXFRAMES; i++)
        rcstimvals[0][i] = NOTSET;
    if(!(optionflag & FIXATION_CHECK))
        fixstate = GOOD_FIXATION;
    
    totalframe = 0;
    framesdone = 0;
    c = START_STIM;
    write(ttys[0],&c,1);
    /*
     * Making every frame a stimulus causes all sorts of trouble. 
     */
    if(optionflags[REDUCE_SERIAL_OUTPUT]){
        noserialout = 1;
    }
    for(istim = 0; istim < nstims; istim++){
        /*
         c = START_STIM;
         write(ttys[0],&c,1);
         */
        finished = 0;
        framecount = 0;
        framesdone = 0;
        /* record nframes */
        if(istim == 0)
            mode |= FIRST_FRAME_BIT;
        else
            mode |= STIMCHANGE_FRAME;
        stimstate = INSTIMULUS;
        clearcnt = 0;
        if(totalframe < MAXFRAMES)
            rcstimvals[6][totalframe] = 1;
        while((finished < 2))
        {
            mode |= NEED_REPAINT;
            paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
            change_frame();
            gettimeofday(&now,NULL);
            rcstimframetimes[totalframe] = timediff(&now,&firstframetime);
            if(framecount == 0)
                sframetimes[istim] = rcstimframetimes[totalframe];
            increment_stimulus(expt.st, &expt.st->pos);
            if(totalframe < MAXFRAMES){
                rcstimvals[0][totalframe] = expt.currentval[0];
                rcstimvals[1][totalframe] = expt.currentval[1];
                if(expt.type3 != EXPTYPE_NONE)
                    rcstimvals[2][totalframe] = expt.currentval[2];
                else
                    rcstimvals[2][totalframe] = StimulusProperty(expt.st,frameparams[0]);
                rcstimvals[3][totalframe] = StimulusProperty(expt.st,frameparams[1]);
                //	rcstimvals[4][totalframe] = expt.stp->stimno[0];
                rcstimvals[4][totalframe] = StimulusProperty(expt.st,frameparams[2]);
                rcstimvals[5][totalframe] = GetProperty(&expt,expt.st,MONOCULARITY_EXPT);
                rcstimid[totalframe] = (expt.stimno & (~(ORDER_BITS))) + expt.expstimid[2] * expt.nstim[3];
                
                rcstimxy[0][totalframe] = xrnd;
                rcstimxy[1][totalframe] = yrnd;
            }
            framesdone = totalframe;
            if(++totalframe > MAXFRAMES)
                totalframe--;
            rcstimvals[6][totalframe] = 0;
            if(st->left->seedloop == 1 && st->type == STIM_IMAGE){
                framecount = getframecount();
            }
            if(finished == 1){
                finished = 2;
            }
            else if(framecount >= (nframes-1)) /* last frame coming.. */
            {
                if(istim == nstims-1)
                    mode |= LAST_FRAME_BIT;
                finished = 1;
            }
            framecount++;
            /* running human psychophysics, stimuili are terminated buy a button press */
            if(option2flag & PSYCHOPHYSICS_BIT)
            {
                //Ali	    if(XCheckTypedWindowEvent(D, 0/* AliGLX myXWindow() */, ButtonPress, &e))
                //	      {
                //		finished=RESPONDED;
                //	      }
            }
            while((c = ReadSerial(0)) != MYEOF)
                if((i = GotChar(c)) == BAD_FIXATION || i == WURTZ_STOPPED)
                {
                    retval = BAD_TRIAL;
                    fixstate = BADFIX_STATE;
                    finished = 2;
                }
        }/* end while !finished */
        noserialout = 0;
        stimulus_is_prepared = 0;
        if(retval == BAD_TRIAL){
            /*
             * If the trial stops because of bad fixation, reset the stimulus counter so
             * that we run the same sequence again.
             */
            stimno = stimno - istim;
            break;
        }
        stimno++;
        ResetExpStim(0);
        if(istim < nstims-1)
            if((i = PrepareExptStim(0, 1)) < 0)
                return(BAD_TRIAL);
        
        if(expt.isi > 0){
            for(i = 0; i < expt.isi * mon.framerate; i++){
                wipescreen(clearcolor);
                draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
                change_frame();
                if(totalframe < MAXFRAMES)
                    rcstimid[totalframe++] = -1;
                if(totalframe > MAXFRAMES)
                    totalframe--;
            }
        }
        
    }/* end for istim */
    
    /*
     * If stimuli are only shown within trials, safe to get one ready for the 
     * beginning of the next trial. 
     */
    
    sframetimes[istim] = -10000;
    if((optionflag & STIM_IN_WURTZ_BIT)){
        i = PrepareExptStim(0,2);
    }
    
    /* 
     * the last frame is always an extra one - after painting n frames still wait for
     * another frame to sync END_STIM with.
     */
    
    framesperstim = 1+ nframes +  expt.isi * mon.framerate;
    ExpStimOver(retval,i);
    if(totalframe < MAXFRAMES){
        rcstimvals[0][totalframe] = NOTSET;
        rcstimid[totalframe] = -1;
    }
    sprintf(buf,"%srS=",serial_strings[MANUAL_TDR]);
    for(i = 0; i < nstims; i++){
        if(rcstimid[i*framesperstim] > -10000){
            sprintf(tmp,"%d ",rcstimid[i*framesperstim]);
            strcat(buf,tmp);
        }
    }
    strcat(buf,"\n");
    SerialString(buf,0);
    
    if(expt.vals[FASTB_RPT] > expt.vals[FAST_SEQUENCE_RPT]){
        sprintf(buf,"%srB=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < nstims; i++){
            sprintf(tmp,"%d ",frameiseqb[i*framesperstim]);
            strcat(buf,tmp);
        }
        strcat(buf,"\n");
        SerialString(buf,0);
    }
    if(optionflags[TILE_XY]){
        sprintf(buf,"%srX=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < nstims; i++){
            if((j = rcstimxy[0][i*framesperstim]) > 3)
                acknowledge("impossible Position id",NULL);
            sprintf(tmp,"%d",j);
            strcat(buf,tmp);
        }
        strcat(buf,"\n");
        SerialString(buf,0);
        sprintf(buf,"%srY=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < nstims; i++){
            sprintf(tmp,"%d",rcstimxy[1][i*framesperstim]);
            strcat(buf,tmp);
        }
        strcat(buf,"\n");
        SerialString(buf,0);
    }
    
    if(retval < 0)
        return(retval);
    else
        return(framesdone);
}


int paint_strobehalf(int stimmode, int order)
{
    
    int delay = rint(expt.vals[DELAY]);
    
    if(delay == 0){
        if(stimmode == 8){
            if(order > 0)
                paint_frame(WHOLESTIM, 0);
            else{
                optionflag |= LEFT_FIXATION_CHECK;
                optionflag &= (~(RIGHT_FIXATION_CHECK));
                paint_half(expt.st->next,LEFTMODE,0);
                paint_half(expt.st,LEFTMODE,0);
            }
        }
        else if(order < 0)
            paint_frame(WHOLESTIM, 0);
    }
    /*
     * delay * order < 0 means delay < 0 and order > 0 (second) or
     *  delay > 0 and order < 0 (first)
     * delay positive means paint RIGHT eye second. so delay > 0 order > 0
     * mean RIGHT eye
     */
    else if(delay * order < 0){
        if(expt.st->next != NULL){
            if(stimmode == 3 || stimmode == 4) 
                paint_half(expt.st->next,RIGHTMODE,0);
            paint_half(expt.st->next,LEFTMODE,0);
            if(expt.st->next->next != NULL)
                paint_half(expt.st->next->next,BOTHMODE,0);
        }
        optionflag |= LEFT_FIXATION_CHECK;
        optionflag &= (~(RIGHT_FIXATION_CHECK));
        paint_half(expt.st,LEFTMODE,1);
    }
    else{
        if(expt.st->next != NULL){
            if(stimmode == 4) 
                paint_half(expt.st->next,LEFTMODE,0);
            if(stimmode != 3)
                paint_half(expt.st->next,RIGHTMODE,0);
        }
        optionflag |= RIGHT_FIXATION_CHECK;
        optionflag &= (~(LEFT_FIXATION_CHECK));
        paint_half(expt.st,RIGHTMODE,1);
    }
    if(stimmode == 3)
        optionflag &= (~(LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
    if(fabs(expt.st->fix.fixcolor - expt.st->background) > 0.01)
        draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
    return(0);
}


int RunStrobedStim(Stimulus *st, int n, /*Ali Display */ int D)
{
    int finished = 0,j,i = 0, nreps, ntotal, retval =0;
    int framecount,rc;
    Substim *rds;
    char c,buf[256];
    float val,period,step;
//    Expstim *stim;
    struct plotdata *plot;
//    Expstim *es,*exs;
    int stimmode = rint(expt.vals[ALTERNATE_STIM_MODE]);
    int preframes =(int)((expt.preperiod * mon.framerate) + 0.5);
    int postframes =(int)((expt.postperiod * mon.framerate) + 0.5);
    int frameperiod = preframes + postframes +2;
    int nframes = n;
    int delay = rint(expt.vals[DELAY]);
    int altframes = 0, frames = 0;
    
    
    nframes = n&(~1); // force to be even
    
    if(st->type == STIM_RDS)
        rds = st->left;
    framecount = 0;
    /* record nframes */
    mode |= FIRST_FRAME_BIT;
    stimstate = INSTIMULUS;
    clearcnt = 0;
    
    /*
     * even when postframes = preframes = 2, at least two frames are painted
     * so frameperiod cannot be less than 2
     */
    if(delay == 0){
        postframes = frameperiod-2;
    }
    else if(delay > 0){
        preframes = delay -1;
        postframes = frameperiod - (preframes+2);
    }
    else{
        preframes = fabsf(delay) -1;
        postframes = frameperiod - (preframes+2);
    }
    if(!(optionflag & FIXATION_CHECK))
        fixstate = GOOD_FIXATION;
    
    setmask(BOTHMODE);
    clearstim(st,st->gammaback, optionflag & DRAW_FIX_BIT);
    /*
     * stimmode determines various modes of stroboscopic painting.
     *  1  background, fix point, and stim are treated identically
     *  2
     *  3 background/fixpt painted both eyes whenever First eye stimulus is painted.
     *  4 background and fixpoint painted both eyes when L or R is painted
     *  5 everything painted at once, 
     *  6  background, fix point, and stim are treated identically
     *  7  oscillating dot
     */
    
    while((finished < 2))
    {
        calc_stimulus(expt.st);
        if(stimmode == 2){
            setmask(BOTHMODE);
            clearstim(st,st->gammaback, optionflag & DRAW_FIX_BIT);
            paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
            setmask(BOTHMODE);
            draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
            change_frame();
            setmask(BOTHMODE);
            clearstim(st,st->gammaback, optionflag & DRAW_FIX_BIT);
            if(expt.st->next != NULL){
                paint_half(expt.st->next,RIGHTMODE,0);
                paint_half(expt.st->next,LEFTMODE,0);
            }
            change_frame();
            draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
            framesdone = ++framecount;
        }
        else if (stimmode > 2){
            optionflag &= (~(LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
            setmask(BOTHMODE);
            clearstim(st,st->gammaback, 0);
            paint_strobehalf(stimmode,-1);
            change_frame();
            optionflag &= (~(LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
            setmask(BOTHMODE);
            if(stimmode == 8 && framesdone %4 == 2){
                SetStimulus(expt.st,st->vals[START_PHASE],SETPHASE,NULL);
                frames = postframes;
            }
            else
                frames = preframes;
            for(i = 0; i < frames; i++){
                wipescreen(clearcolor);
                change_frame();
            }
            clearstim(st,st->gammaback, 0);
            if(stimmode == 8 && delay == 0)
                increment_stimulus(expt.st, &expt.st->pos);
            paint_strobehalf(stimmode,1);
            change_frame();
            optionflag &= (~(LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
            /* postframes and incrementing framecount is done at the end for everybody */
            framesdone = ++framecount;
            if(stimmode ==7 && framesdone %4 ==3){
                altframes = 2 * fabs(delay);
                for(i = 0; i < altframes; i++){
                    wipescreen(clearcolor);
                    change_frame();
                }
            }
        }
        else{
            setmask(BOTHMODE);
            clearstim(st,st->gammaback, 0);
            if(stimmode == 3 || stimmode == 4){
                if(expt.st->next != NULL)
                    paint_half(expt.st->next,RIGHTMODE,0);
            }
            optionflag |= LEFT_FIXATION_CHECK;
            if(expt.st->next != NULL){
                paint_half(expt.st->next,LEFTMODE,0);
            }
            paint_half(expt.st,LEFTMODE,0);
            if(stimmode == 3)
                optionflag &= (~LEFT_FIXATION_CHECK);
            if(fabs(expt.st->fix.fixcolor - expt.st->background) > 0.01)
                draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
            change_frame();
            optionflag &= (~LEFT_FIXATION_CHECK);
            setmask(BOTHMODE);
            for(i = 0; i < preframes; i++){
                wipescreen(clearcolor);
                change_frame();
            }
            clearstim(st,st->gammaback, 0);
            if(stimmode == 4){
                if(expt.st->next != NULL)
                    paint_half(expt.st->next,LEFTMODE,0);
            }
            optionflag |= RIGHT_FIXATION_CHECK;
            if(expt.st->next != NULL && stimmode != 3){
                paint_half(expt.st->next,RIGHTMODE,0);
            }
            paint_half(expt.st, RIGHTMODE,1);
            framesdone = ++framecount;
            if(stimmode == 4)
                optionflag &= (~RIGHT_FIXATION_CHECK);
            if(fabs(expt.st->fix.fixcolor - expt.st->background) > 0.01  && stimmode != 3)
                draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
            optionflag &= (~RIGHT_FIXATION_CHECK);
            if(finished == 1)
                mode |= LAST_FRAME_BIT;
            change_frame();
        }
        
        if(stimmode == 8 && framesdone %4 == 3)
            frames = preframes;
        else
            frames = postframes;
        if((stimmode == 7 || stimmode == 8) && (framesdone %4 == 3 || delay ==0))
            SetStimulus(expt.st,st->vals[START_PHASE],SETPHASE,NULL);
        else
            increment_stimulus(expt.st, &expt.st->pos);
        framesdone = ++framecount;
        
        setmask(BOTHMODE);
        for(i = 0; i < frames; i++){
            /*
             * N.B. This paint half gets obliterated by the wipe. Just makes sure 
             * these take up whole video frames
             */
            paint_half(expt.st, RIGHTMODE,0);
            wipescreen(clearcolor);
            change_frame();
        }
        if(rc/mon.framerate > expt.vals[FIXATION_OVERLAP])
        {
            expt.st->fixcolor = expt.st->gammaback;
            mode |= FIXATION_OFF_BIT;
        }
        if(finished == 1){
            finished = 2;
        }
        else if(framecount >= (nframes-2)) 
        /* 
         * N.B. fixed number of pictures, not video frames. Note that framecount is
         * incremented twice per lap, so prepare to stop at n-2
         */
        {
            finished = 1;
        }
        /* running human psychophysics, stimuili are terminated buy a button press */
        if(option2flag & PSYCHOPHYSICS_BIT)
        {
            //Ali	    if(XCheckTypedWindowEvent(D, 0/* AliGLX myXWindow()*/, ButtonPress, &e))
            //	      {
            //		finished=RESPONDED;
            //	      }
        }
        while((c = ReadSerial(0)) != MYEOF)
            if((i = GotChar(c)) == BAD_FIXATION || i == WURTZ_STOPPED)
            {
                retval = BAD_TRIAL;
                fixstate = BADFIX_STATE;
                finished = 2;
            }
    }/* end while !finished */
    if(n > nframes){ // odd number of frames named 
        calc_stimulus(expt.st);
        setmask(BOTHMODE);
        clearstim(st,st->gammaback, 0);
        paint_strobehalf(stimmode,-1);
        change_frame();
        framecount++;
    }
    
    
    glFinishRenderAPPLE(); /* block until buffer swapped */
    gettimeofday(&endstimtime,NULL);
    /* 
     * getframecount counts from after the first frame to after the last frame, so
     * its n-1 frames. ie. If you paint over n-1 inter-frame spaces. So add one 
     * to rc for calculating the strobe period;
     */
    rc = getframecount(); 
    
    printf("Paint rate %d/%d (%.4f)\n",framecount,rc,getframetime());
    if(n > nframes)
        rc = rc-1;
    period = 1000 * (int)(0.5 + (float)(rc+1)/(framecount/2))/mon.framerate;
    val = StimDuration();
    i = (int)((val * mon.framerate) + 0.1);
    if(expt.st->type == STIM_BAR)
        step = pix2deg(expt.st->pos.radius[1] * expt.st->incr/(M_PI));
    else
        step = pix2deg(expt.st->posinc * mon.framerate)/mon.framerate;
    printf("Step %.3f every %.1f (%d,%d) ms = %.2f deg/sec dur %.3f frames %d,%d\n",step,period,preframes,postframes,(step * 1000)/period,val,rc,i);
    stimseq[trialctr].vals[2] = step;
    stimseq[trialctr].vals[3] = period;
    stimseq[trialctr].vals[4] = expt.vals[DELAY];
    stimseq[trialctr].vals[5] = frameperiod; // == period in frames
    
    optionflag &= (~(LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK));
    setmask(BOTHMODE);
    wipescreen(clearcolor);
    
    draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
    change_frame();
    setmask(BOTHMODE);
    wipescreen(clearcolor);
    draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
    change_frame();
    stimdursum += StimDuration();
    stimdurn++;
    realframes = realframecount = getframecount();
    if(stimstate == INSTIMULUS)
        stimstate = POSTSTIMULUS;
    
    if(retval == BAD_TRIAL || retval < 0)
    {
        afc_s.lasttrial = BAD_TRIAL;
        /*    ShuffleStimulus();*/
    }

    ExpStimOver(retval,i);
    
    /* 
     * if IFC and stimno is even, this is the first of a pair. Prepare
     * and show the second stimulus of the pair.
     */
    
    if(option2flag & IFC && (!(stimno & 1)))
    {
        stimno++;
        ResetExpStim(0);
        PrepareExptStim(1,3);
        fsleep(expt.isi);
        RunStrobedStim(st, n, D);
    }
    return(framecount);
}

int RunHarrisStim(Stimulus *st, int n, /*Ali Display */ int D, /*Ali Window */ int win)
{
    int finished = 0,j,i = 0, nreps, ntotal, retval =0;
    int framecount,rc,lastframecount;
    Substim *rds;
    char c,buf[256];
    float val;
//    Expstim *stim;
    struct plotdata *plot;
//    Expstim *es,*exs;
    int framecounts[MAXFRAMES];
    float frametimes[MAXFRAMES];
    float swapwaits[MAXFRAMES];
    vcoord startpos[0];
    
    
    if(st->type == STIM_RDS)
        rds = st->left;
    framecount = 0;
    framesdone = 0;
    expt.st->framectr = 0;
    if(optionflags[TEMPORAL_GAUSS]){
        expt.st->tenvelope = 0;
    }
    else
        expt.st->tenvelope = 1;
    /* record nframes */
    mode |= FIRST_FRAME_BIT;
    stimstate = INSTIMULUS;
    clearcnt = 0;
    
    if(!(optionflag & FIXATION_CHECK))
        fixstate = GOOD_FIXATION;
    startpos[0] = expt.st->pos.xy[0];
    startpos[1] = expt.st->pos.xy[1];
    rc = 0;
    while((finished < 2))
    {
        lastframecount = rc;
        paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
        change_frame();
        expt.st->pos.xy[0] += st->posinc;
        expt.st->disp += st->dispincr;
        if((framesdone = ++framecount) > MAXFRAMES)
            framesdone = MAXFRAMES;
        if(freezestimulus)
            rc = 0;
        else if(!optionflags[FIXNUM_PAINTED_FRAMES]){
            rc = getframecount(); /* real video frames */
            if(expt.st->framerepeat > 1){
                while(rc < lastframecount + expt.st->framerepeat)
                    rc = getframecount();	
            }
        }
        else
            rc = framesdone;
        
        if(rc/mon.framerate > expt.vals[FIXATION_OVERLAP])
        {
            expt.st->fixcolor = expt.st->gammaback;
            mode |= FIXATION_OFF_BIT;
        }
        /*
         * last frame coming.. 
         */
        
        if(finished == 1){
            finished = 2;
        }
        else if(rc >= (n-1) && n < MAXFRAMES) 
        {
            mode |= LAST_FRAME_BIT;
            finished = 1;
        }
        /* running human psychophysics, stimuili are terminated buy a button press */
        if(option2flag & PSYCHOPHYSICS_BIT || freezestimulus)
        {
            frametimes[framesdone] = timediff(&frametime,&zeroframetime);
            framecounts[framesdone] = rc;
            //Ali	    if(XCheckTypedWindowEvent(D, 0 /* AliGLX  myXWindow()*/, KeyPress, &e)){
            //	    }
            //	    if(XCheckTypedWindowEvent(D, 0 /* AliGLX myXWindow()*/, ButtonPress, &e))
            //	      {
            //		realframes = getframecount();
            //		finished=RESPONDED;
            //	      }
        }
        while((c = ReadSerial(0)) != MYEOF)
            if((i = GotChar(c)) == BAD_FIXATION || i == WURTZ_STOPPED)
            {
                retval = BAD_TRIAL;
                fixstate = BADFIX_STATE;
                finished = 2;
            }
    }/* end while !finished */
    if((optionflag & STIM_IN_WURTZ_BIT)){
        wipescreen(clearcolor);
    }
    draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
    change_frame(); 
    expt.st->pos.xy[0] = startpos[0];
    
    stimdursum += StimDuration();
    stimdurn++;
    realframes = realframecount = getframecount();
    if(stimstate == INSTIMULUS)
        stimstate = POSTSTIMULUS;
    
    /* reset stimulus type in case it was set to blank */
    if(retval == BAD_TRIAL || retval < 0)
    {
        afc_s.lasttrial = BAD_TRIAL;
    }
    else
        afc_s.lasttrial = WURTZ_OK;

    ExpStimOver(retval,i);
    
    /*
     if(retval != BAD_TRIAL && afc_s.loopstate != CORRECTION_LOOP)
     stimno++;
     */
    /* 
     * if IFC and stimno is even, this is the first of a pair. Prepare
     * and show the second stimulus of the pair. If a response button
     * is hit during the first stimulus, stimno is incremented to an odd
     * number. In this case just increment stimno one more time - no need to 
     * show second stimulus.
     */
    if(finished == RESPONDED && option2flag & IFC && (stimno &1))
        stimno++;
    else if(option2flag & IFC && (!(stimno & 1)) && finished != RESPONDED)
    {
        stimno++;
        ResetExpStim(0);
        /* if subject responds during first stimulus, skip the second */
        if(stimstate != RESPONDED){
            PrepareExptStim(1,4);
            fsleep(expt.isi);
            RunExptStim(st, n, D, win);
        }
    }
    return(framecount);
}


static float frametimes[MAXFRAMES],fframecounts[MAXFRAMES];
static int framecounts[MAXFRAMES];

int RunExptStim(Stimulus *st, int n, /*Ali Display */ int D, /*Window */ int win)
{
    int finished = 0,j,i = 0, nreps, ntotal, retval =0;
    int framecount,rc,lastframecount;
    Substim *rds;
    char c,buf[BUFSIZ*20],tmp[BUFSIZ*20];
    float val;
//    Expstim *stim;
    struct plotdata *plot;
//    Expstim *es,*exs;
    int lastframesdone;
    float tval;
    float swapwaits[MAXFRAMES],calctimes[MAXFRAMES],painttimes[MAXFRAMES];
    float forcewaits[MAXFRAMES];
    struct timeval lastframetime,pretime,forcetime;
    int nframes = n,rpt = 1;
    int noverflow = expt.noverflow;
    char cbuf[20560];
    int cctr = 0;
        int framesperstim = 1;
    
    cbuf[0] = 0;
    
    currentstim.stimid = expt.stimid;
    currentstim.seed = firstseed = expt.st->left->baseseed;
    if(expt.vals[ALTERNATE_STIM_MODE] > 0.5){
        i = rint(expt.vals[ALTERNATE_STIM_MODE]);
        switch(i){
            case HARRIS_TASK:
            case HARRIS_TASK_ANGLE:
                framesdone = RunHarrisStim(st, n, D, win);
                return(framesdone);
                break;
            case ASYMMETRICAL_ENVELOPE: 
            case ADD_UNCORR_STRIP: 
            case SEED_CHANGE_WITH_CONTRAST: 
            default:
                break;
            case 7:
            case 6:
            case 5:
            case 4:
            case 3:
            case 2:
            case 1:
                framesdone = RunStrobedStim(st, n, D);
                return(framesdone);
                break;
        }
    }
    
    if(st->left->ptr->velocity > 0.00001){
        stimflag = st->flag;
        oldvelocity = st->left->ptr->velocity;
        if (st->type == STIM_RDS)
            oldvelocity = st->posinc;
        olddisp = st->disp;
    }
    else if(seroutfile){
        fprintf(seroutfile,"#Velocity 0 before stim starts, was %.2f\n",oldvelocity);
    }
    SerialSend(SET_SEED);
    if(st->type == STIM_RDS)
        rds = st->left;
    framecount = 0;
    framesdone = 0;
    expt.st->framectr = 0;
    if(optionflags[TEMPORAL_GAUSS]){
        expt.st->tenvelope = 0;
    }
    else
        expt.st->tenvelope = 1;
    /* record nframes */
    mode |= FIRST_FRAME_BIT;
    stimstate = INSTIMULUS;
    clearcnt = 0;
    
    if(!(optionflag & FIXATION_CHECK))
        fixstate = GOOD_FIXATION;
    sframetimes[0] = -10000;
    if(optionflags[RANDOM_PHASE])
        SetRandomPhase(expt.st, &(expt.st->pos));
    
    rc = 0;
    rpt = (st->framerepeat < 1) ? 1 : st->framerepeat;
    
    if (expt.stimmode == BUTTSEXPT){
        expt.backim = backims[(int)(expt.vals[BACKGROUND_IMAGE])];
    }
    
    
    /*
     * Human Psych trials have different requirements - often have stimuli 
     * taking > 1 frame to calculate/pain. No need to send warning to brainwave
     * on penultimate frame.
     */
    if(option2flag & PSYCHOPHYSICS_BIT){
        while((finished < 2))
        {
            lastframecount = rc;
            if(optionflags[FAST_SEQUENCE]){
                SetStimulus(expt.st,frameseq[framesdone],expt.fasttype,NOEVENT);
            }
            else
                rcstimid[framesdone] = st->stimid;
            if(framecount >1 && optionflags[CALCULATE_ONCE_ONLY]){
                if(!(mode & FIXATION_OFF_BIT))
                    draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fixcolor);
            }
            else
                paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));
            change_frame();
            
            if(optionflags[WATCH_TIMES]){
                swapwaits[framesdone] = swapwait;
                calctimes[framesdone] = calcdur;
                painttimes[framesdone] = paintdur;
            }
            if(freezestimulus)
                rc = 0;
            else if(!optionflags[FIXNUM_PAINTED_FRAMES]){
                rc = getframecount(); /* real video frames */
                while(rc < lastframecount + expt.st->framerepeat && rc > 0)
                    rc = getframecount();
                frametimes[framesdone] = timediff(&frametime,&zeroframetime);
            }
            else{
                rc = framesdone;
                frametimes[framesdone] = timediff(&frametime,&zeroframetime);
            }
            
            if((framesdone = ++framecount) >= MAXFRAMES)
                framesdone = MAXFRAMES-1;
            
            expt.framesdone = framesdone;
            /*
             * if testflags[SAVE_IMAGES] == 1, save images of each frame to disk
             * if testflags[SAVE_IMAGES] == 2, save images of last frame of last 
             * stimulus in the the expt, then clear the flag;
             * if testflags[SAVE_IMAGES] == 7, save images of last frame each stimulus 
             */
            if(testflags[SAVE_IMAGES]){
                if(testflags[SAVE_IMAGES] == 1){
                    SaveImage(expt.st,1);
                    rc = framesdone;
                }
                else if(testflags[SAVE_IMAGES] == 3 || testflags[SAVE_IMAGES] == 5 || testflags[SAVE_IMAGES] == 6){
                    SaveImage(expt.st,2);
                    rc = framesdone;
                }
                else if(testflags[SAVE_IMAGES] == 2 && stimno+1 >= expt.nstim[5] * expt.nreps){
                    if(finished == 1){ // last frame
                        SaveImage(expt.st,3); //save both types - max info
                        testflags[SAVE_IMAGES] = 0;
                    }
                    rc = framesdone;
                }
                if(testflags[SAVE_IMAGES] == 7 && finished == 1){
                    SaveImage(expt.st,3); //save both types - max info
                    testflags[SAVE_IMAGES] = 0;
                }
            }
            /*
             * Increment the stimulus _AFTER_ saving any images, so that corrrect seed
             * number etc is recorded with the image
             */
            
            if(freezestimulus < 2)
                increment_stimulus(expt.st, &expt.st->pos);
            
            
            
            if(rc/mon.framerate > expt.vals[FIXATION_OVERLAP])
            {
                expt.st->fixcolor = expt.st->gammaback;
                mode |= FIXATION_OFF_BIT;
            }
            /*
             * last frame coming.. 
             */
            
            if(finished == 1){
                finished = 2;
            }
            else if(rc >= (n-1) && n < MAXFRAMES) 
            {
                mode |= LAST_FRAME_BIT;
                finished = 1;
            }
            /* running human psychophysics, stimuili are terminated buy a button press
             * But only buttons 1-3. Button 4 and 5 (mac) are the scroll button,which is often hit just after the middle button
             */
            if(option2flag & PSYCHOPHYSICS_BIT || freezestimulus)
            {
                frametimes[framesdone] = timediff(&frametime,&zeroframetime);
                framecounts[framesdone] = rc;
                if(rc >= n && n < MAXFRAMES) 
                    finished = 2;
                //Ali	    if(XCheckTypedWindowEvent(D, 0 /* AliGLX myXWindow()*/, KeyPress, &e)){	    }
                //	    if(XCheckTypedWindowEvent(D, 0 /* AliGLX myXWindow()*/, ButtonPress, &e))
                //	      {
                //		if(e.mouseButton == Button1 ||		   e.mouseButton == Button2 ||		   e.mouseButton == Button3){		realframes = getframecount();		finished=RESPONDED;}
                //	      }
            }
            while((c = ReadSerial(0)) != MYEOF)
                if((i = GotChar(c)) == BAD_FIXATION || i == WURTZ_STOPPED)
                {
                    retval = BAD_TRIAL;
                    fixstate = BADFIX_STATE;
                    finished = 2;
                }
        }/* end while !finished */
        optionflags[CHECK_FRAMECOUNTS] = 0;
    } /* end if PSYCHOPHYSCIS */
    else{
        
        gettimeofday(&pretime,NULL);
        rc = 1; /* one frame will be done by the first time this is checked */
        lastframecount = 0;
        lastframesdone = framesdone = 0;
        framecount = 0;
        while((finished < 2))
        {
            if(debug ==4)
                testcolor();
            lastframecount = rc;
            if(expt.type2 == BACKGROUND_MOVIE || expt.mode == BACKGROUND_MOVIE)
                expt.st->mode |= STIMULUS_NEEDS_CLEAR;
            if(optionflags[FAST_SEQUENCE]){
                if(expt.fastctype != EXPTYPE_NONE)
                    SetStimulus(expt.st,frameseqc[framesdone],expt.fastctype,NOEVENT);
                if(expt.fastbtype != EXPTYPE_NONE && dframeseq[framesdone])
                    SetStimulus(expt.st,frameseqb[framesdone],expt.fastbtype,NOEVENT);
                if(expt.type3 == RANDOM_CONTRAST_EXPT && dframeseq[framesdone])
                    SetRandomContrast(expt.st);
                if(optionflags[RANDOM_CORRELATION] && dframeseq[framesdone])
                    SetRandomCorrelation(expt.st);
                SetStimulus(expt.st,frameseq[framesdone],expt.fasttype,NOEVENT);
                if(frameseed[0]){
                    expt.st->next->left->seed = frameseed[framesdone];
                    expt.st->next->right->seed = frameseed[framesdone];
                    expt.st->next->left->baseseed = frameseed[framesdone];
                    expt.st->next->right->baseseed = frameseed[framesdone];
                }
            }
            else if(expt.type2 == BACKGROUND_MOVIE || expt.mode == BACKGROUND_MOVIE)
                {
                    expt.backim = backims[(int)(frameseqb[framesdone])];
                    expt.vals[BACKGROUND_MOVIE] = frameseqb[framesdone];
                }
            else if (framesdone < MAXFRAMES)
                rcstimid[framesdone] = st->stimid;
            
            if(optionflags[RANDOM_CONTRAST] || expt.type3 == RANDOM_CONTRAST_EXPT){
                rcstimvals[7][framesdone] = st->pos.contrast-st->contrast_disp; //right
                rcstimvals[8][framesdone] = st->pos.contrast+st->contrast_disp; //left
                if(rcstimvals[7][framesdone] < 0 || rcstimvals[8][framesdone] < 0)
                    fprintf(stderr,"Negative Contrast frame %d\n",framesdone);
            }
            if(optionflags[RANDOM_CORRELATION]){
                rcstimvals[9][framesdone] = st->correlation; 
            }
            if(finished)
                lastframecount = rc;
            lastframesdone = framesdone;
            
            
            
            paint_frame(WHOLESTIM, !(mode & FIXATION_OFF_BIT));

#ifdef FRAME_OUTPUT
            if (Frames2DIO){
            //DIOval | 8 is written in change_frame();
            if(rc == n-1)
                DIOWriteBit(3,0);
            else
                DIOWriteBit(3,0);
            }
#endif
            
            if(!optionflags[FIXNUM_PAINTED_FRAMES]){
                if(framesdone){
                    rc = getframecount()+2; /* real video frames since FIRST*/
                    memcpy(&forcetime,&frametime,sizeof(struct timeval));
                    while(rc < lastframecount + expt.st->framerepeat){
                        rc = getframecount()+2;
                        forcewaits[framesdone] = timediff(&frametime,&forcetime);
                    }
                }
            }
            else
                rc = framesdone;
            
            change_frame();
            if (optionflags[FIXNUM_PAINTED_FRAMES]){
                framecounts[framesdone] = getframecount();
                tval = timediff(&changeframetime, &zeroframetime);
                fframecounts[framesdone] = (tval * mon.framerate);
            }
            else
                framecounts[framesdone] = rc;
            
            if(optionflags[WATCH_TIMES]){
                swapwaits[framesdone] = swapwait;
                calctimes[framesdone] = calcdur;
                painttimes[framesdone] = paintdur;
            }
            
            /*
             * on the first frame, change_frame() blocks until the vertical retrace at the
             * start of the frame, and then records zeroframetime. On subsequent frames, 
             * change_frame does not block the CPU: this happens at the beginning
             * of paint_frame(), which is when frametime is recorded. Note this means that
             * the time recorded is actually when the PREVIOUS frame was displayed. Its
             * when you make the call to start drawing frame 2 that the CPU blocks until
             * the vertical retrace painting frame 1 is reached. 
             * So frame framesdone is incremented _AFTER_ recording the frametime.
             * 
             * Sequence:
             * paint_frame(0)  frametimes[0], paints frame[0]
             * change_frame()  blocks CPU until retrace, records zeroframetime
             * paint_frame(1)  calculates frame 1, frametimes[1] paints frame 1
             * change_frame()
             * paint_frame(2)  Calculates frame 2, blocks, frametimes[2], paints frame 2.
             * Note that because the change_frame() blocks the CPU for the first frame,
             * calling paint_frame for the second does NOT block - that Vertical retrace 
             * has already passed. This means that frametimes[1] records the time taken
             * just to calculate frame 1. frametimes[0] will be slightly negative.
             *
             * so video frames elapsed after change_frame is getframecount+2: Add one because zeroframetime is when frame 1 appears. Add another because the count is always 1 frame behind (the last time the CPU blocked at paint_stimulus
             *
             */
            if(freezestimulus)
                rc = 0;
            
            frametimes[framesdone] = timediff(&paintframetime,&zeroframetime);
            if(optionflags[FAST_SEQUENCE]){
                sframetimes[framesdone] = frametimes[framesdone];
                sframetimes[framesdone+1] = -10000;
                if(frameseq[framesdone] < INTERLEAVE_EXPT) // Check interleave is painted
                    sframetimes[framesdone+1] = -10000;
            }
            if(framesdone ==1){
                gettimeofday(&now, NULL);
                val = timediff(&now,&zeroframetime);
                val = timediff(&calctime,&zeroframetime);
            }
            if(framesdone ==2)
                val = timediff(&zeroframetime,&pretime);
            
            if((framesdone = ++framecount) > MAXFRAMES)
                framesdone = MAXFRAMES;
            
            expt.framesdone = framesdone;
            
            /*
             * if testflags[SAVE_IMAGES] == 1, save images of each frame to disk
             * if testflags[SAVE_IMAGES] == 2, save images of last frame of last 
             * stimulus in the the expt, then clear the flag;
             */
            if(testflags[SAVE_IMAGES] && expt.st->type != STIM_IMAGE){
                if(testflags[SAVE_IMAGES] == 1){
                    SaveImage(expt.st,1);
                    rc = framesdone;
                }
                else if(testflags[SAVE_IMAGES] == 3 || testflags[SAVE_IMAGES] == 5 || testflags[SAVE_IMAGES] == 6){
                    SaveImage(expt.st,2);
                    rc = framesdone;
                }
                else if(testflags[SAVE_IMAGES] == 4 && finished == 1){
                    SaveImage(expt.st,1);
                }
                else if(testflags[SAVE_IMAGES] == 2 && stimno+1 >= expt.nstim[5] * expt.nreps){
                    if(finished == 1){ // last frame
                        SerialSend(SET_SEED);
                        SaveImage(expt.st,3);
                        testflags[SAVE_IMAGES] = 0;
                    }
                    rc = framesdone;
                }
            }
            /*
             * Increment the stimulus _AFTER_ saving any images, so that corrrect seed
             * number etc is recorded with the image
             */
            
            if(freezestimulus < 2)
                increment_stimulus(expt.st, &expt.st->pos);
            
            
            if(rc/mon.framerate > expt.vals[FIXATION_OVERLAP])
            {
                expt.st->fixcolor = expt.st->gammaback;
                mode |= FIXATION_OFF_BIT;
            }
            /*
             * last frame coming.. 
             */
            
            if(finished == 1){
                finished = 2;
            }
            else if(rc >= (n-1) && n < MAXFRAMES) 
            {
                mode |= LAST_FRAME_BIT;
                finished = 1;
            }
            /* running human psychophysics, stimuili are terminated buy a button press */
            if(option2flag & PSYCHOPHYSICS_BIT || freezestimulus)
            {
                frametimes[framesdone] = timediff(&frametime,&zeroframetime);
                framecounts[framesdone] = rc;
                if(rc >= n && n < MAXFRAMES) 
                    finished = 2;
                //Ali	    if(XCheckTypedWindowEvent(D, 0 /* AliGLX myXWindow()*/, KeyPress, &e)){	    }
                //	    if(XCheckTypedWindowEvent(D, 0 /* AliGLX myXWindow()*/, ButtonPress, &e))	      {		realframes = getframecount(); finished=RESPONDED;	      }
            }
#ifdef FRAME_OUTPUT
            if(finished == 2  && Frames2DIO) //about to exit loop
                DIOWriteBit(3, 0);
#endif
            /*
             * once finished == 2 don't check the serial line. Might read spikes before finishing painting the stimulus.
             */
            if(finished < 2){
                while((c = ReadSerial(0)) != MYEOF){
                    cbuf[cctr++] = c;
                    if(cctr >= 20560)
                        cctr--;
                    if((i = GotChar(c)) == BAD_FIXATION || i == WURTZ_STOPPED)
                    {
                        retval = BAD_TRIAL;
                        fixstate = BADFIX_STATE;
                        finished = 2;
                        if(mode & LAST_FRAME_BIT && seroutfile != NULL)
                        {
                            fprintf(seroutfile,"#Bad on last frame\n");
                        }
                    }
                }
            }
            ReadInputPipe();
            //Ali	if(n >= MAXFRAMES && XCheckTypedWindowEvent(D, 0 /* AliGLX myXWindow()*/, KeyPress, &e)){	}
            //Ali	i = CheckKeyboard(D, allframe);
            //Ali	j = CheckKeyboard(D, stepform);
            //	if(j == (int)(XK_c) || i == (int)(XK_c))
            //	  finished = 2;
            //	else if (j != 0 || i != 0)
            //	  c = i;
        }/* end while !finished */
    }/* end Not PSCYHOPHISCS */
    if((optionflag & STIM_IN_WURTZ_BIT)){
        wipescreen(clearcolor);
        draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
        change_frame();
        wipescreen(clearcolor); //wipe for next swap too
    }
#ifdef NIDAQ
    DIOWriteBit(0,0); //clear stimchange pin
#endif
    
    SerialSend(SET_SEED); // get this recorded at stim end also
    if(cctr && 0) // Don't do this normally
        printf("Serial %d: %s\n",cctr,cbuf);
    framecounts[framesdone] = rc;
    frametimes[framesdone] = timediff(&endstimtime,&zeroframetime);
    if(debug==4)
        testcolor();
    
    
    /*
     * if replaying an expt, leave the final stimulus up
     * if in a real expt and using back movies, go back to grey at end stim
     */
    if((expt.mode == BACKGROUND_MOVIE || expt.mode) == BACKGROUND_IMAGE && expt.stimmode != BUTTSEXPT){
        expt.backim = backims[MAXBACKIM];
    }
    if(stimno+1 >= expt.nstim[5] * expt.nreps && testflags[PLAYING_EXPT]){
        fsleep(2);
    }
    
    if(option2flag & IFC){
        //    if(expt.vals[TRIAL_START_BLANK] >0 && expt.vals[TRIAL_START_BLANK] >= expt.isi - 0.02)
        if(expt.vals[TRIAL_START_BLANK] >0)
            clearstim(expt.st,expt.st->gammaback, 0);
        draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.offcolor);
    }
    else
        draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.fixcolor);
    change_frame();
    gettimeofday(&lastframetime, NULL);
    if(framecount < MAXFRAMES-1)
        frametimes[framecount+1] = timediff(&paintframetime,&zeroframetime);
    
    if((option2flag & PSYCHOPHYSICS_BIT)&& seroutfile){
        fprintf(seroutfile,"#Frames:");
        for(i = 0; i <= framecount+1; i++)
            fprintf(seroutfile,"%.3f ",frametimes[i]);
        fprintf(seroutfile,"(%.3f,%.3f,%.3f)\n",StimDuration(),timediff(&endstimtime,&frametime),timediff(&firstframetime,&zeroframetime));
    }
    
    if(optionflags[FAST_SEQUENCE]){
        sprintf(buf,"%srS=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < framesdone; i++){
            if(rcstimid[i] >= 0){
                sprintf(tmp,"%d ",rcstimid[i]);
                if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                    strcat(buf,tmp);
            }
        }
        strcat(buf,"\n");
        SerialString(buf,0);
        //    fputs(buf,stdout);
        if(optionflags[RANDOM_PHASE]){
            sprintf(buf,"%srP=",serial_strings[MANUAL_TDR]);
            for(i = 0; i < framesdone; i++){
                if(frameiseqp[i] >= 0){
                    sprintf(tmp,"%d ",frameiseqp[i]);
                    if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                        strcat(buf,tmp);
                }
            }
            strcat(buf,"\n");
            SerialString(buf,0);
        }
        if(optionflags[TILE_XY]){
            sprintf(buf,"%srX=",serial_strings[MANUAL_TDR]);
            for(i = 0; i < framesdone; i++){
                sprintf(tmp,"%d ",rcstimxy[0][i*framesperstim]);
                strcat(buf,tmp);
            }
            strcat(buf,"\n");
            SerialString(buf,0);
            sprintf(buf,"%srY=",serial_strings[MANUAL_TDR]);
            for(i = 0; i < framesdone; i++){
                sprintf(tmp,"%d ",rcstimxy[1][i*framesperstim]);
                strcat(buf,tmp);
            }
            strcat(buf,"\n");
            SerialString(buf,0);
        }
        
    }
    if(optionflags[RANDOM_CONTRAST] || expt.type3 == RANDOM_CONTRAST_EXPT){
        sprintf(buf,"%scR=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < framesdone; i++){
            sprintf(tmp,"%x ",(int)(rcstimvals[7][i] * 256));
            if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                strcat(buf,tmp);
        }
        strcat(buf,"\n");
        SerialString(buf,0);
        sprintf(buf,"%scL=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < framesdone; i++){
            sprintf(tmp,"%x ",(int)(rcstimvals[8][i] * 256));
            if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                strcat(buf,tmp);
        }
        strcat(buf,"\n");
        SerialString(buf,0);
    }
    if(optionflags[RANDOM_CORRELATION]){
        sprintf(buf,"%scI=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < framesdone; i++){
            sprintf(tmp,"%x ",(int)(fabs(rcstimvals[9][i]) * 256));
            if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                strcat(buf,tmp);
        }
        strcat(buf,"\n");
        SerialString(buf,0);
        sprintf(buf,"%scL=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < framesdone; i++){
            sprintf(tmp,"%x ",(int)(rcstimvals[8][i] * 256));
            if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                strcat(buf,tmp);
        }
        strcat(buf,"\n");
        SerialString(buf,0);
    }
    if(expt.st->type == STIM_IMAGE){
        sprintf(buf,"%sse=",serial_strings[MANUAL_TDR]);
        for(i = 0; i < framesdone; i++){
            if(imageseed[i] >= 0){
                sprintf(tmp,"%d ",imageseed[i]);
                if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                    strcat(buf,tmp);
            }
        }
        strcat(buf,"\n");
        SerialString(buf,0);
    }
    
    if(framesdone * rpt < n && retval != BAD_TRIAL){  
        /* 
         * Not all frames completed
         * framecounts is number of real video frame elapsed, each
         * time a new image is painted. So this gets ahead of expected
         * framecounts if display slow.
         */
        sprintf(buf,"%sFn=",serial_strings[MANUAL_TDR]);
        j = 1;
        for(i = 0; i < framesdone && framecounts[i] < n * 2; i++,j++){
            while(framecounts[i] > j * rpt){ //skipped frames
                sprintf(tmp,"%d ",framecounts[i]);	
                if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                    strcat(buf,tmp);
                j++;
            }
        }
        strcat(buf,"\n");
        SerialString(buf,-1);
        if(seroutfile){
            fprintf(seroutfile,"%s",buf);
            fprintf(seroutfile,"#Frames:");
            for(i = 0; i < framesdone; i++)
                fprintf(seroutfile," %d",framecounts[i]);
            fprintf(seroutfile,"\n");
        }
        /*
         * if optionflags[CHECK_FRAMECOUNTS] ==2, only check the first stimulus
         */
        if(optionflags[CHECK_FRAMECOUNTS] &&   framesdone * rpt < n * 0.9){ 
            if(optionflags[CHECK_FRAMECOUNTS] < 2){
                sprintf(buf,"Only completed %d/%d frames. Continue Checking?",framesdone,n);
                if(!confirm_yes(buf,NULL) )
                    optionflags[CHECK_FRAMECOUNTS] = 0;
            }
            else if (stimno ==  0){
                sprintf(buf,"Only completed %d/%d frames. Will not Check again.",framesdone,n);
                acknowledge(buf,NULL);
            }
        }
    }
    
//    CheckStimDuration(retval);
    /*
     * also check if frames all done, but took too long (in case forcing all frames
     */
    if(optionflags[CHECK_FRAMECOUNTS] && retval != BAD_TRIAL && 
       frametimes[framesdone-1] > (1.1 * n)/expt.mon->framerate){ 
        if(optionflags[CHECK_FRAMECOUNTS] < 2){
            sprintf(buf,"%d frames took %.1f. Continue Checking?",framesdone,frametimes[framesdone-1]);
            if(!confirm_yes(buf,NULL)){
                optionflags[CHECK_FRAMECOUNTS] = 0;
            }  
        }
        else if (stimno == 0){
            sprintf(buf,"%d frames took %.1f. Won't check again",framesdone,frametimes[framesdone-1]);
            acknowledge(buf,NULL);
        }
    }
    
    if(rcfd && optionflags[WATCH_TIMES]){
        fprintf(rcfd,"#%dFrames:",framesdone);
        for(i = 0; i < framesdone+1; i++)
            fprintf(rcfd," %d",framecounts[i]);
        fprintf(rcfd,"\n");
        fprintf(rcfd,"#Calc");
        for(i = 0; i < framesdone; i++)
            fprintf(rcfd," %.4f",calctimes[i]);
        fprintf(rcfd,"\n");
        fprintf(rcfd,"#Paint:");
        for(i = 0; i < framesdone; i++)
            fprintf(rcfd," %.4f",painttimes[i]);
        fprintf(rcfd,"\n");
        fprintf(rcfd,"#Swap:");
        for(i = 0; i < framesdone; i++)
            fprintf(rcfd," %.4f",swapwaits[i]);
        fprintf(rcfd,"\n");
        fprintf(rcfd,"#Wait:");
        for(i = 0; i < framesdone; i++)
            fprintf(rcfd," %.4f",forcewaits[i]);
        fprintf(rcfd,"\n");
    }
    
    stimdursum += StimDuration();
    stimdurn++;
    realframes = realframecount = getframecount();
    if(stimstate == INSTIMULUS)
        stimstate = POSTSTIMULUS;
    
    /* reset stimulus type in case it was set to blank */
    if(retval == BAD_TRIAL || retval < 0)
    {
        afc_s.lasttrial = BAD_TRIAL;
        stimulus_is_prepared = 0; // make sure PrepareExptStim is called. 
        /*    ShuffleStimulus();*/
    }

    ExpStimOver(retval,i);
    
    /*
     if(retval != BAD_TRIAL && afc_s.loopstate != CORRECTION_LOOP)
     stimno++;
     */
    /* 
     * if IFC and stimno is even, this is the first of a pair. Prepare
     * and show the second stimulus of the pair. If a response button
     * is hit during the first stimulus, stimno is incremented to an odd
     * number. In this case just increment stimno one more time - no need to 
     * show second stimulus.
     */
    if(finished == RESPONDED && option2flag & IFC && (stimno &1))
        stimno++;
    else if(option2flag & IFC && (!(stimno & 1)) && finished != RESPONDED)
    {
        stimno++;
        ResetExpStim(0);
        /* if subject responds during first stimulus, skip the second */
        if(stimstate != RESPONDED){
            if(expt.vals[TRIAL_START_BLANK] > 0){
                gettimeofday(&now, NULL);
                val = timediff(&now,&lastframetime);
                fsleep(expt.vals[TRIAL_START_BLANK] - val);
                clearstim(expt.st,expt.st->gammaback, 0);
                draw_fix(fixpos[0],fixpos[1], expt.st->fix.size, expt.st->fix.offcolor);
                change_frame();
            }
            PrepareExptStim(1,5);
            gettimeofday(&now, NULL);
            val = timediff(&now,&lastframetime);
            fsleep(expt.isi-val);
            RunExptStim(st, n, D, win);
        }
    }
    totalframe = framecount;
    if(expt.noverflow > noverflow && seroutfile)
        fprintf(seroutfile,"#Overflow %d/%d\n",expt.noverflow,expt.ncalc);
    if(expt.noverflow > 0.05 * expt.ncalc){
        sprintf(buf,"%d/%d Contrast Overflow!!",expt.noverflow,expt.ncalc);
        acknowledge(buf,"/bgc/bgc/c/binoc/help/overflow.1");
    }
    return(framecount);
}


int CheckStimDuration(int retval)
{
    int i = 0,j =0, n = 0, rpt =0,nrpt = 0,nf=0,k=0;
    char buf[BUFSIZ * 10],tmp[BUFSIZ*10];
    float val;
    float framevals[MAXFRAMES], diffmax, diffmin;
    
    rpt = (expt.st->framerepeat < 1) ? 1 : expt.st->framerepeat;

    sprintf(buf,"#du%.3f(%d:%.3f)",frametimes[framesdone],framesdone,(framesdone-0.5)/expt.mon->framerate);
    SerialString(buf,0);
    if (optionflags[FIXNUM_PAINTED_FRAMES]){
        if(frametimes[framesdone]  > (framesdone-0.5)/expt.mon->framerate){
            fprintf(stderr,"%d frames took %.3f\n",framesdone,frametimes[framesdone]);
            sprintf(buf,"%sFi=",serial_strings[MANUAL_TDR]);
            for( i = 1; i < framesdone-1; i++){
                val = frametimes[i]-frametimes[i-1];
                framevals[i] = val;
                sprintf(tmp,"%d ",(int)(round(val*1000)));
                strcat(buf,tmp);
            }
            strcat(buf,"\n");
            SerialString(buf,-1);
            sprintf(buf,"%sFn=",serial_strings[MANUAL_TDR]);
            j=0;
            for(i = 0; i < framesdone; i++){
                sprintf(tmp,"%.1f ",fframecounts[i]);
                if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                    strcat(buf,tmp);
                if (i > 1 && fframecounts[i]-fframecounts[i-1] > 1.5)
                    fprintf(stderr,"Skip at %d:%.1f\n",i,fframecounts[i]);
            }
            strcat(buf,"\n");
            SerialString(buf,-1);
            nrpt = 0;
            diffmax = 1.2/mon.framerate;
            diffmin = 0.5/mon.framerate;
            sprintf(buf,"%sFl=",serial_strings[MANUAL_TDR]);
            for( i = 1; i < framesdone-1; i++){
                if (framevals[i] > diffmax && framevals[i+1]+framevals[i] > diffmax *2){
                    nf = round((framevals[i+1]+framevals[i])*mon.framerate) -2;
                    for (k = 0; k < nf; k++){
                        sprintf(tmp,"%d ",i+nrpt++);
                        strcat(buf,tmp);
                    }
                }
            }
            strcat(buf,"\n");
            SerialString(buf,0);
        }
    }
    else if(frametimes[framesdone]  > (n-0.5)/expt.mon->framerate){
        if (seroutfile)
            fprintf(seroutfile," #long(%d)",n);
        if (retval != BAD_TRIAL){
            sprintf(buf,"%sFi=",serial_strings[MANUAL_TDR]);
            for( i = 1; i < framesdone-1; i++){
                val = frametimes[i]-frametimes[i-1];
                sprintf(tmp,"%d ",(int)(round(val*1000)));
                strcat(buf,tmp);
            }
            strcat(buf,"\n");
            if(seroutfile)
                fprintf(seroutfile,"%s",buf);
            if(frametimes[framesdone]  > (n+1.5)/expt.mon->framerate){
                printf("V long %.3f",frametimes[framesdone]);
            }
            if(optionflags[FIXNUM_PAINTED_FRAMES] ==0){
                for(i = 1; i < framesdone; i++){
                    if (framecounts[i]-framecounts[i-1] > 1){
                        printf("skip at %d\n",i);
                    }
                }
            }
            
            sprintf(buf,"%sFn=",serial_strings[MANUAL_TDR]);
            if(optionflags[FIXNUM_PAINTED_FRAMES]){
                j=0;
                for(i = 0; i < framesdone; i++){
                    sprintf(tmp,"%.1f ",fframecounts[i]);                        if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                        strcat(buf,tmp);
                }
                strcat(buf,"\n");
                SerialString(buf,0);
            }
            else{
                j=0;
                for(i = 0; i < framesdone && framecounts[i] < n * 2; i++,j++){
                    while(framecounts[i] > j * rpt){ //skipped frames
                        sprintf(tmp,"%d ",framecounts[i]);
                        if(strlen(buf)+strlen(tmp) < BUFSIZ*2)
                            strcat(buf,tmp);
                        j++;
                    }
                }
                strcat(buf,"\n");
                SerialString(buf,0);
            }
            
        }
        //        if(seroutfile)
        //            fprintf(seroutfile,"%s",buf);
        
        if(seroutfile){
            if(retval == BAD_TRIAL)
                fprintf(seroutfile,"BAD");
            fprintf(seroutfile,"\n");
        }
    }
}

int ExptTrialOver(int type)
{
    int stimi = currentstim.stimid,i;
    
    
    if(stimi >= MAXSTIM)
        stimi = 0;
    if(stimi < 0)
        stimi = 0;
    
    if (dorpt == 2 || afc_s.loopstate == CORRECTION_LOOP)
        return(0);
    if(type == WURTZ_OK || type == WURTZ_OK_W){
        completed[stimi]++;
        if (expt.st->mode & EXPTPENDING){ // only count rpts in expt
            if(dorpt == 1){ //if == 2, then seed set from list made at start
                if(seroutfile)
                    fprintf(seroutfile,"Repeated %d=%d (%d,%d) (%d) stim %d\n", unrepeated[stimi][unrepeatn[stimi]-1],dorpt,unrepeatn[stimi]-1,completed[stimi],currentstim.lastseed,expt.stimid,stimi);
                if(unrepeatn > 0)
                    unrepeatn[stimi]--;
#ifdef WATCHSEQ
                else
                    fprintf(seroutfile,"Repeat when unrepeatn was 0, %d\n",dorpt);
#endif
                expt.st->left->baseseed = currentstim.lastseed;
            }
            else{
                if(unrepeatn[stimi] < RPTLISTLEN && expt.vals[RC_REPEATS] > 0)
                    unrepeatn[stimi]++;
                if(unrepeatn[stimi] > expt.nreps && expt.nreps > 1)
                    acknowledge("Repeat list too long",NULL);
                unrepeated[stimi][unrepeatn[stimi]-1] = currentstim.seqseed;
            }
        }
        if(seroutfile){
            fprintf(seroutfile,"Repneed %d (%d done %d) exval %d", dorpt ? currentstim.lastseed : currentstim.seqseed,
                    uncompleted[stimi],unrepeatn[stimi],stimi);
            for(i = 0; i < unrepeatn[stimi]; i++)
                fprintf(seroutfile," %d",unrepeated[stimi][i]);
            fprintf(seroutfile,"\n");
        }
        if(uncompleted[stimi])
            uncompleted[stimi]--;
    }
    else{ // if BADFIX etc, reset the seed
        if(currentstim.lastseed > expt.st->left->baseseed)
            expt.st->left->baseseed = currentstim.lastseed;
    }
    dorpt = 0;
}


/* allow BW 1 second to say it is ready */
int CheckBW(int signal, char *msg)
{
    char c,s[BUFSIZ],signame[256],last;
    int timeout = 0,ctr = 0, i;
    static int ok = 1;
    static int *confirmer_state = NULL;
    float val;
    
    last = lastcodes[codectr%CODEHIST];
    if(signal == END_STIM && pcmode == SPIKE2)
        return(1);
    if(signal == END_STIM && last == BW_IS_READY)
    {
        last = lastcodes[(codectr-1)%CODEHIST];
    }
    if(testflags[PLAYING_EXPT] || mimic_fixation || demomode || option2flag & PSYCHOPHYSICS_BIT)
        return(1);
    if(optionflag & WAIT_FOR_BW_BIT)
    {
        gettimeofday(&now,NULL);
        if(last == signal && signal == END_STIM)
        {
            memcpy(&endsigtime,&signaltime,sizeof(struct timeval));
            return(1);
        }
        /* 	 
         * if END_STIM was send, and BAX_FIX is recieved back, it means that
         * fixation was broken before END_STIM was processed at the BW end
         */
        while((c = ReadSerial(0)) != signal && timeout < 100
              && !(signal == END_STIM && c == BAD_FIXATION))
        {
            if(c == MYEOF) /* nothing coming in */
                fsleep(0.01), timeout++;
            else if(c == CONJUG_OUT){
                fprintf(stderr,"CONJUG in Check at %d\n",ufftime(&now));
                GotChar(c);
            }
            else 
            {
                /* if there is serial input, BW is busy, not lost */
                timeout = 0;
                s[ctr++] = c;
            }
        }
        /* process any other input */
        if(timeout < 100) /* got the signal */
        {
            gettimeofday(&signaltime,NULL);
            if(signal == END_STIM)
                memcpy(&endsigtime,&signaltime,sizeof(struct timeval));
            lastcodes[++codectr%CODEHIST] = c;
        }
        /* 
         * When END_STIM/BAD_FIXATION cross like ships in the night, there will 
         * not be any spikes coming, so set gotspikes
         */
        if(signal == END_STIM && c == BAD_FIXATION){
            if (seroutfile){
            fprintf(seroutfile,"End/Bad combination %d %u %d",trialcnt,ufftime(&now),stimstate);
            }
            endbadctr++;
            glstatusline("End/Bad",3);
            gotspikes = 1;
        }
        s[ctr] = 0;
        for(i = 0; i < ctr; i++){
            if(s[i] == CONJUG_OUT)
                fprintf(stderr,"Sent %s\n",s);
            GotChar(s[i]);
        }
        /*
         * if this fails, don't keep on putting up the warning, wait until at
         * least one good signal is received before warning again
         */
        
        if(timeout >= 100)
        {
            if(signal == END_STIM)
            {
                mode |= BW_ERROR;
                StopGo(STOP);
            }
//            if(confirmer_state && *confirmer_state == 0) /* has been acknowleged */
                ok = 1;
            if(ok)
            {
                if(msg != NULL)
                {
                    if(signal == END_STIM)
                    {
                        val = timediff(&now,&endsigtime);
                        sprintf(signame,"%s %.2f last %d",msg,val,last);
                    }
                    else
                        sprintf(signame,"%s",msg);
                }
                else switch(signal)
                {
                    case END_STIM:
                        sprintf(signame,"End");
                        break;
                    default:
                        sprintf(signame,"Gen");
                        break;
                }
                sprintf(s,"Trial %d %s Serial Line Not Responding",stimno,signame);
                if(seroutfile != NULL){
                    val = timediff(&now,&bwtime);
                    fprintf(seroutfile,"Endstim Error %d %u",trialcnt,ufftime(&now));
                    ShowLastCodes();
                }
                //Ali w = 
                acknowledge(s,"/bgc/bgc/c/binoc/help/SerialError");
                ok = 0;
            }
            
            return(0);
        }
        else /* got a BW_IS_READY SIGNAL */
            ok = 1;
    }
    return(1);
}



/* wait for n frames */
void framepause(int nf)
{
    int i;
    
    if(nf <=0)
        return;
    for(i = 0; i < nf; i++)
    {
        //AliGLX		mySwapBuffers();
        GLblock(1);
    }
}

static int oldnquick;

int ReplayExpt(char *name)
{
    static FILE *rfd = NULL;
    int setseed = 0,code = 0,lcheck = 100,i;
    char buf[BUFSIZ],*s;
    static char lastbuf[BUFSIZ],stimbuf[BUFSIZ];
    static int linectr = 0;
    static float vval = 0;
    Thisstim estim;
    
    if(name != NULL && !strcmp(name,"Show")){
        sprintf(buf,"Line %d at %s: %s",linectr,stimbuf,lastbuf);
        glstatusline(buf,3);
        return(0);
    }
    if(rfd == NULL){
        if(name != NULL){
            if((rfd = fopen(name,"r")) == NULL)
            {
                sprintf(buf,"Can't open replay %s;",name);
                acknowledge(buf,NULL);
                return(-1);
            }
            vval = 0;
            setgamma(1.0);
            testflags[PLAYING_EXPT] = 1;
            linectr = 0;
            printf("Replaying %s\n",name);
        }
        else{
            return(0);
        }
    }
    if(freezeexpt){
        if(freezestimulus)
            paint_frame(WHOLESTIM, 0);
        return(1);
    }
    while(fgets(buf, BUFSIZ, rfd) != NULL){
        linectr++;
        if((s = strstr(buf,"by binoc Version ")) != NULL)
        {
            sscanf(s,"by binoc Version %f",&vval);
            if(vval > 0)
                onlineversion = vval * 100;
        }
        if(linectr < skiplines){
            sprintf(lastbuf,"Line %d %s",linectr,buf);
            glstatusline(lastbuf,3);
        }
        else if(buf[0] == '\\')
            ReadCommand(&buf[1]);
        else if(linectr == stopline){
            stopline += 100; // stop every 100 lines after this
        }
        else if(!strncmp(buf,"#qe ",4) && strlen(buf) > 5 && replay_expt){
            reading_quickexp = 1;
            ResetExpt();
            ReadExptFile(nonewline(&buf[4]),1,1,0);
            mode |= MORE_PENDING;
        }
        else if(!strncmp(buf,"#freeze",5)){
            freezeexpt = 1;
            freezestimulus = 2;
            return(1);
        }
        else if(!strncmp(buf,"#seedoffset",7)){
            if((i = sscanf(buf,"#seedoffset:%d",&code)) > 0)
                seedoffset = code;
        }
        else if(!strncmp(buf,"#saveim",7)){
            if(!strncmp(buf,"#saveima",8))
                testflags[SAVE_IMAGES] = 3;
            else
                testflags[SAVE_IMAGES] = 1;
            if((i = sscanf(buf,"#saveim%d",&code)) > 0)
                testflags[SAVE_IMAGES] = code;
            
            if((s = strchr(buf,':')) != NULL){
                /*
                 * Always open the index file in append mode, in case the program has crushed.  
                 * This file is any used manually in any case.
                 */
                strcpy(ImageOutDir,nonewline(&s[1]));
                sprintf(buf,"%s/pgm.idx",ImageOutDir);
                imidxfd = fopen(buf,"a");
                
                printf("Saving images to %s\n",ImageOutDir);
                fprintf(imidxfd,"Mon %.4f %.4f %.1f",mon.pix2cms[0],mon.pix2cms[1],mon.viewd);
            }
        }
        else if(!strncmp(buf,"#saveoff",7)){
            testflags[SAVE_IMAGES] = 0;
            sprintf(buf,"Images saved in %s",ImageOutDir);
            acknowledge(buf,NULL);
        }
        else if(!strncmp(buf,"Expt ",5) && vval < 4.27)
            runexpt(NULL,NULL,NULL);
        else if(!strncmp(buf,"#Start Expt",9)){
            expt.st->mode &= (~EXPTPENDING);
            runexpt(NULL,NULL,NULL);
        }
        else if(!strncmp(buf,"et at",5))
            expt_over(1);
        else if(!strncmp(buf,"Cancelled",6))
            expt_over(CANCEL_EXPT);
        /*
         * "O 3" = End Stim sent, which means current stimulus was completed, so run it
         * now.
         */
        else if(!strncmp(buf,"O 3",3)){ 
            estim.vals[0] = GetProperty(&expt,expt.st,expt.mode);
            estim.vals[1] = GetProperty(&expt,expt.st,expt.type2);
            strcpy(stimbuf,&buf[4]);
            ShowStimVals(&estim);
            sprintf(buf,"Line %d %s st = %s id%d",linectr,stimbuf,stimulus_names[expt.st->type],expt.allstimid);
            
            glstatusline(buf,3);
            printf("Stim %s id%d\n",stimulus_names[expt.st->type],expt.allstimid);
            fixstate = GOOD_FIXATION;
            if(stimstate == STIMSTOPPED || stimstate == INTERTRIAL)
                stimstate = PREFIXATION;
            return(1);
        }
        else if(!strncmp(buf,"RW ",3)){ 
            GotChar(WURTZ_OK_W);
            //      return(1);
        }
        else if(!strncmp(buf,"RG ",3)){ 
            GotChar(WURTZ_OK);
            //      return(1);
        }
        else if(!strncmp(buf,"RF ",3) || !strncmp(buf,"RB ",3)){ 
            GotChar(WURTZ_OK);
            //      return(1);
        }
        else if(!strncmp(buf,"End/Bad C",7)){
        }
        else if(!strncmp(buf,"Stimulus",7)){
        }
        else if(!strncmp(buf,"Run End",6)){
        }
        else if(!strncmp(buf,"so",2)){ //Ignore software offsets
        }
        /*
         * prior to version 4.28, frames done was recorded to the serial output file
         * with nf, not Nf, but you can tell this since there was no decimal point
         */
        else if(!strncmp(buf,"rw",2)){
            if(stimstate == WAIT_FOR_RESPONSE || stimstate == POSTSTIMULUS){
                stimstate = INTERTRIAL;
                return(stimstate);
            }
        }
        else if(!strncmp(buf,"nf",2)){ 
            if(strchr(buf,'.') != NULL)
                InterpretLine(buf, &expt,0);
        }
        else{
            if(!strncmp(buf,serial_strings[OPTION_CODE],2)){
                option2flag = 0;
                optionflag = 0;
                for(i = 0; i <= MAXALLFLAGS; i++)
                    optionflags[i] = 0;
            }
            code = InterpretLine(buf, &expt,0);
            if(code == SET_SEED){
                setseed = StimulusProperty(expt.st,SET_SEED);
                if(seedoffset && stimstate == INTERTRIAL)
                    SetStimulus(expt.st,setseed+seedoffset,SET_SEED,NULL);
            }
            else if(code < MAXTOTALCODES)
                strcpy(lastbuf,buf);
            if(stimstate == STIMSTOPPED)
                printf("Stopped (%d) by %s\n",linectr,buf);
        }
        if(linectr % lcheck == 0)
            strcpy(lastbuf,buf);
    }
    fclose(rfd);
    testflags[PLAYING_EXPT] = 0;
    sprintf(buf,"%s Finished",replay_expt);
    puts(buf);
    acknowledge(buf,NULL);
    rfd = NULL;
    return(0);
}

/*
 * reset = reset the quick expt list
 * new = 1 means this is anew file, finish any closing commands first
 * new = 2 means that this will be followed by other files, so close wheever
 * reading stops
 */


int ReadExptFile(char *name, int new, int show, int reset)
{
    static FILE *exfd = NULL;
    char *s = NULL;
    struct plotdata *plot = expt.plot;
    static int stillreading = 0;
    int setseed = 0,code = 0,oldseed;
    int line,framewait = 0;
    int oldnhelp = 0,i,oldmode;
    float val;
    int savedstate = 0;
    struct timeval then;
    
    oldnquick = nquickexpts;
    oldnhelp = nhelpfiles;
    newtoggles = 0;
    oldseed = expt.st->left->baseseed;
    gettimeofday(&then,NULL);
    
    if(new == 1 && exfd != NULL)
    {
        if(mode && MORE_PENDING) //execute close commands of last exp
            ReadExptFile(NULL,0,0,0);
        stillreading = 0;
        reading_quickexp = 0;
        if(exfd != NULL){
            fclose(exfd);
            exfd = NULL;
        }
    }
    if(!stillreading){
        /*
         * used to force nquickexpts to zero here, so that a new menu could be
         * built. But now qe's  in a qe get added to the list
         * might not it is set to zero before a loadfile
         */
    }
    if(exfd == NULL && name != NULL)/* file not already open */
    {
        if((exfd = fopen(name,"r")) == NULL)
        {
            sprintf(mssg,"Can't open expt %s;",name);
            acknowledge(mssg,NULL);
            return(-1);
        }
    }
    else if(exfd == NULL && s == NULL)
    {
        
        if(nquickexpts == 0)
            nquickexpts = oldnquick;
        return(-1);
    }
    if(command_pending){
        ReadCommand(&mssg[1]);
        command_pending = 0 ;
    }
    if(reset){
        //	  nhelpfiles = 0;
        nquickexpts = 0;
    }
    while((s = fgets(mssg, BUFSIZ, exfd)) != NULL)
    {
        line++;
        if(mssg[0] == '\\'){
            command_pending = 1;
            return(0);
        }
        if(mssg[0] == '\!'){
            ReadCommand(&mssg[1]);
        }
        else if(sscanf(mssg,"pause%f",&val) > 0){
            framewait = val * mon.framerate;
            /* 
             * if auto_next expt, expt_over will start the next before from next_frame
             * unless we set STIMSTOPPED now
             */
            states[EXPT_PAUSED_BY_QEXP]  =1;
            if (mode &= AUTO_NEXT_EXPT)
                stimstate = STIMSTOPPED;
            oldmode = expt.st->mode;
            expt.st->mode &= (~EXPTPENDING);
            for(i = 0; i < framewait; i++){
                next_frame(expt.st);
            }
            states[EXPT_PAUSED_BY_QEXP]  =0;
            expt.st->mode = oldmode;
        }
        else if(!strncmp(mssg,"pause",5) || !strncmp(mssg,"next",4) || !strncmp(mssg,"over",4))
        {
            stillreading++;
            mode |= MORE_PENDING;
            if(!strncmp(mssg,"over",4)){
                mode |= WAIT_NEXT_EXPT;
                nquickexpts = oldnquick;
            }
            else
                mode &= (~WAIT_NEXT_EXPT);
            if(!strncmp(mssg,"next",4))
                mode |= AUTO_NEXT_EXPT;
            else{
                mode &= (~AUTO_NEXT_EXPT);
            }
            
            if(!setseed && expt.st->left->baseseed < 1001)
                NewSeed(expt.st);
            if(new == 2){
                fclose(exfd);
                exfd = NULL;
            }
            return(0);
        }
        else if(!strncmp(mssg,"#State",6)){
            savedstate = 1;
        }
        else if(!strncmp(mssg,"Stimulus",7)){
        }
        else{
            if(verbose)
                puts(mssg);
            code = InterpretLine(mssg, &expt,0);
            if(seroutfile)
                fflush(seroutfile);
            if(code == SET_SEED){
                if(reading_quickexp && expt.st->left->seedloop == 0){
                    expt.st->left->baseseed = oldseed;
                    setseed = 0;
                }
                else
                    setseed = 1;
            }
        }
        if(line % 20 == 0)
            code = line;
        gettimeofday(&now,NULL);
        //	  printf("%d %.3f %s \n",line,timediff(&now,&then),mssg);
    }
    fclose(exfd);
    exfd = NULL;
    
    if(!setseed && expt.st->left->baseseed < 1001)
        NewSeed(expt.st);
    stillreading = 0;
    if(savedstate)
        SerialSend(UFF_PREFIX);
    mode &= (~MORE_PENDING);
    if(seroutfile)
        fflush(seroutfile);
    if(show){
        if(reading_quickexp && nquickexpts > oldnquick)
            //make_quickmenu();
            printf("nothing!");
        else if(nquickexpts == 0)
            nquickexpts = oldnquick;
        else if(!reading_quickexp)
            //make_quickmenu();
            printf("nothing!");
        SendAll();
        SerialSend(SEND_CLEAR);
    }
    reading_quickexp = 0;
    if(nhelpfiles == 0)
        nhelpfiles = oldnhelp;
    return(0);
}

void SetPlotLabels(struct plotdata *plot)
{
    char buf[256],abuf[BUFSIZ];
    int i,j,k,n4,nplots;
    float val;
    
    
    for( n4 = 0; n4 < expt.nstim[4]; n4++){
        if(expt.nstim[4] > 1){
            sprintf(abuf," %s %.2f",serial_strings[expt.type3],expt.exp3vals[n4]);
        }
        else
            sprintf(abuf,"");
        if(expt.flag & ADD_EXPT2)
        {
            plot->label[0] = myscopy(plot->label[0],serial_strings[expt.mode]);
            plot->label[1] = myscopy(plot->label[1],serial_strings[expt.type2]);
        }
        else if(expt.flag & TIMES_EXPT2){
            //    for(n4 =0; n4 < expt.nstim[4]; n4++)
            if(optionflags[PLOTFLIP])
                nplots = expt.nstim[0];
            else
                nplots = expt.nstim[1];
            for(i =0; i < nplots; i++)
            {
                if(optionflags[PLOTFLIP])
                    j = i + expt.nstim[2];
                else
                    j = (expt.nstim[0]*i) + expt.nstim[2];
                if(expt.type2 == MONOCULARITY_EXPT)
                {
                    if((val=plot->stims[j].x[1]) < -0.4)
                        sprintf(buf,"Left%s",abuf);
                    else if(val > 0.4)
                        sprintf(buf,"Right%s",abuf);
                    else 
                        sprintf(buf,"Binoc%s",abuf);
                }
                else if(expt.type2 == SEED_DELAY && expt.st->type == STIM_RDSSINE)
                {
                    if(rint((val=plot->stims[j].x[1])) == -1)
                        sprintf(buf,"L Sine,R RDS");
                    else if(val == 1)
                        sprintf(buf,"L RDS,R Sine");
                    else if(val == 0)
                        sprintf(buf,"RDS");
                    else if(val == 2)
                        sprintf(buf,"Sine");
                    else 
                        sprintf(buf,"??");
                }
                else if(expt.type2 == CONTRAST_PAIRS)
                {
                    if(rint((val=plot->stims[j].x[1])) == 1)
                        sprintf(buf,"L,R %.2f",StimulusProperty(expt.st,SETCONTRAST));
                    else if(val == 0)
                        sprintf(buf,"L,R %.2f",StimulusProperty(expt.st,CONTRAST2));
                    else if(val == 0.5)
                        sprintf(buf,"L %.2f,R %.2f",StimulusProperty(expt.st,CONTRAST2),StimulusProperty(expt.st,SETCONTRAST));
                    else if(val == -0.5)
                        sprintf(buf,"L %.2f,R %.2f",StimulusProperty(expt.st,SETCONTRAST),StimulusProperty(expt.st,CONTRAST2));
                    else 
                        sprintf(buf,"??");
                }
                else if(expt.type2 == STIMULUS_TYPE_CODE){
                    val=plot->stims[j].x[1];
                    k = (int)(val+0.1);
                    if(k >= 0 && k < MAXSTIMULUSTYPES)
                        sprintf(buf,"%s",stimulus_names[k]);
                    else
                        sprintf(buf,"%d=NotAStim",k);
                }
                else if(expt.type2 == JUMPTYPE){
                    val=plot->stims[j].x[1];
                    if((k = (int)(val+0.1)) < NJUMPTYPES && k >= 0)
                        sprintf(buf,"%s",jumpnames[k]);
                    else
                        sprintf(buf,"???");
                    
                }
                else if(expt.type2 == CORRELATION)
                {
                    if((val=plot->stims[j].x[1]) < -0.4)
                        sprintf(buf,"Anti%s",abuf);
                    else if(val > 0.4)
                        sprintf(buf,"Corr%s",abuf);
                    else 
                        sprintf(buf,"uncorr%s",abuf);
                }
                else if((expt.type2 == DISP_P2 || expt.type2 == CONTRAST_RATIO)
                        && plot->stims[j].x[1] < -1000)
                {
                    if(plot->stims[j].x[1] == -1003)
                        sprintf(buf,"SF %.2f",StimulusProperty(expt.st,SF));
                    if(plot->stims[j].x[1] == -1004)
                        sprintf(buf,"SF %.2f",StimulusProperty(expt.st,SF2));
                }
                else
                {
                    if(optionflags[PLOTFLIP])
                        sprintf(buf,"%s=%.2f%s",serial_strings[expt.mode],plot->stims[j].x[0],abuf);
                    else
                        sprintf(buf,"%s=%.2f%s",serial_strings[expt.type2],plot->stims[j].x[1],abuf);
                }
                MakePlotLabel(plot, buf, j, 0);
                
                k = i + n4 * expt.nstim[1];
                if(k < plot->nlabels) // Can be violated with PLOTFLIP
                    plot->label[k] = myscopy(plot->label[k],buf);
            }
        }
        else{
            sprintf(buf,"%s%s",serial_strings[expt.mode],abuf);
            plot->label[n4] = myscopy(plot->label[n4],buf);
        }
    }
    if(optionflag & CLAMP_HOLD_BIT)
    {
        plot->label[0] = myscopy(plot->label[0],"Neg Fix");
        plot->label[1] = myscopy(plot->label[1],"Neg Clamp");
        plot->label[2] = myscopy(plot->label[2],"Pos Fix");
        plot->label[3] = myscopy(plot->label[3],"Pos Clamp");
    }
}

void MakePlotLabel(struct plotdata *plot, char *s, int i, int flip)
{
    int j;
    float val;
    if(i < expt.nstim[2])
        sprintf(s,"%s",extralabels[i]);
    else
    {
        if(expval[i] == INTERLEAVE_EXPT_UNCORR)
            sprintf(s,"xUncorr");
        else if(expt.flag & TIMES_EXPT2 && (j = i-(expt.nstim[0]+expt.nstim[2])) >= 0) // expt2
        {
            if(!flip)
                i = expt.nstim[2] + j +  expt.nstim[0];
            if(expt.type2 == STIMULUS_TYPE_CODE)
            {
                j = (int)(expt.st->type);
                if(j < N_STIMULUS_TYPES)
                    sprintf(s,"%s",stimulus_names[j]);
                else if(j == STIM_SUBGRATING1)
                    sprintf(s,"sf%.1f",expt.stimvals[SF]);
                else if(j == STIM_SUBGRATING2)
                    sprintf(s,"sf%.1f",expt.stimvals[SF2]);
                else
                    sprintf(s,"??");
            }
            else if(expt.type2 == MONOCULARITY_EXPT){
                if((val=expval[i]) < -0.4)
                    sprintf(s,"Left");
                else if(val > 0.4)
                    sprintf(s,"Right");
                else 
                    sprintf(s,"Binoc");
            }
            else if((expt.type2 == DISP_P2 || expt.type2 == CONTRAST_RATIO)
                    && expval[i] < -1000)
            {
                if(expval[i] == -1003)
                    sprintf(s,"SF %.2f",StimulusProperty(expt.st,SF));
                if(plot->stims[i].x[1] == -1004)
                    sprintf(s,"SF %.2f",StimulusProperty(expt.st,SF2));
                
            }
            else{
                sprintf(s,"%.*f",nfplaces[expt.mode],expval[i]);
            }
        }
        else{ //expt
            if(expval[i] <= INTERLEAVE_EXPT)
                sprintf(s,"0");
            else{
                sprintf(s,"%.*f",nfplaces[expt.mode],expval[i]);
            }
        }
    }
}

int FindStimId(Expt *ex, AFCstructure *afc)
{
//    Expstim *es;
    int stimid = 0;
    int i,j,k,imin = 0;
    double diff, mindiff = 100000, stimval = 0;
    
    
    if(afc->vals[0] == INTERLEAVE_EXPT_ZERO)
        afc->vals[0] = 0;
    
    if(afc->type == MAGONE_SIGNTWO){
        for(i = 0; i < ex->nstim[5]; i++){
            j = i2expi(ex->flag, expt.nstim, i, 0);
            k = i2expi(ex->flag, expt.nstim, i, 1);
            stimval = expval[j];
            if(stimval < -1000)
                stimval = 0;
            if(fabs(afc->vals[1] - expval[k]) < 0.0001){
                diff = fabs(afc->jstairval - stimval);
                if(diff < mindiff){
                    imin =i;
                    mindiff = diff;
                }
            }
        }
    }
    else{
        for(i = 0; i < ex->nstim[0]+expt.nstim[2]; i++){
            j = i2expi(ex->flag, expt.nstim, i, 0);
            diff = fabs(afc->vals[0] - expval[j]);
            if(diff < mindiff){
                imin =i;
                mindiff = diff;
            }
        }
    }
    stimid = imin;
    return(stimid);
}



void inc_psychdata(int response_direction, Expt *ex, int jstimno)
{
 //   Expstim *es;
    int stimid = jstimno & (~ORDER_BITS);
    int i,j,k,imin = 0,e3=0;
    double diff, mindiff = 100000, stimval = 0;
    
    if(jstimno & STIMULUS_EXTRA_FLIP){
        if(ex->st->disp < 0)
            stimid = 0;
        else
            stimid = 1;
    }
    e3 = stimid/expt.nstim[3];
    if((option2flag & STAIRCASE) && !(option2flag & PSYCHOPHYSICS_BIT))
        stimid = FindStimId(&expt, &afc_s);
    
    if(stimid > ex->nstim[5]) /* something wrong */
        return;
    if(rcfd)
        fprintf(rcfd,"RespDir %d\n",response_direction); 
    return;    

}



int ShowFlag(char *s, int flag)
{
    char c = *s++;
    int bit = 0, bit2 = 0,i = 0,j =0;
    
    if(flag){
        while(flag_codes[i] != NULL)
        {
            if(!strncmp(s,flag_codes[i],2))
                break;
            i++;
        }
        if(flag > 10000)
            return(i);
        if(flag_codes[i]){
            if(c == '+')
                allstimflags |= (1<<i);
            else if(c == '-')
                allstimflags &= (~(1<<i));
        }
        return(i);
    }
    
    i = 0;
    while(togglestrings[i].code!= NULL)
    {
        if(!strncmp(s,togglestrings[i].code,strlen(togglestrings[i].code)))
            break;
        i++;
    }
    if(togglestrings[i].group ==1)
        bit = togglestrings[i].icode;
    else if(togglestrings[i].group ==2)
        bit2 = togglestrings[i].icode;
    else if(togglestrings[i].group ==3)
    {
        j = togglestrings[i].icode;
        if(j >= MAXALLFLAGS)
            fprintf(stderr,"Unrecognized flag %s",s);
        if(c == '+')
            allflags[j] = 1;
        else
            allflags[j] = 0;
        return(i);
    }
    
    if(c == '+')
    {
        alloptions |= bit;
        all2options |= bit2;
    }
    else if(c == '-')	
    {	
        alloptions &= (~bit);
        all2options &= (~bit2);
    }
    return(i);
}

int CheckOption(int i)
{
    int flag;
    GLboolean isstereo;
    
    
    if(i >= MAXOPTIONBITS){
        flag = i - MAXOPTIONBITS;
        switch(flag){
            case TIMES_EXPT:
                if( optionflags[flag]){
                    expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
                    expt.flag |= TIMES_EXPT2;
                }
                else{
                    expt.flag &= (~(ADD_EXPT2 | TIMES_EXPT2));
                    expt.flag |= ADD_EXPT2;
                }
            case INTERLEAVE_ZERO:
            case INTERLEAVE_HIGH:
            case INTERLEAVE_HIGHSF:
            case INTERLEAVE_SINE:
            case INTERLEAVE_ONE_SINE:
            case INTERLEAVE_MONOC_ALL:
            case INTERLEAVE_HIGH_ALL:
            case INTERLEAVE_UNCORR_ALL:
            case INTERLEAVE_ZERO_ALL:

                setstimuli(1);
                ListExpStims(NULL);
                break;
            case STEREO_GLASSES:
                if(optionflags[flag]){
                    glEnable(GL_STEREO);
                    glEnable(GL_BLEND);
                    glGetBooleanv(GL_STEREO, &isstereo);
                    printf("Stereo On\n");
                }
                else{
                    glDisable(GL_STEREO);
                    glDisable(GL_BLEND);
                }
                break;
                
        }
        
        
    }
    return(0);
    
}


int ChangeFlag(char *s)
{
    char c = *s;
    int bit = 0, bit2 = 0,i = 0;
    
    s++;
    
    i = 0;
    while(togglestrings[i].code!= NULL)
    {
        if(!strncmp(s,togglestrings[i].code,strlen(togglestrings[i].code)))
            break;
        i++;
    }
    if(togglestrings[i].code == NULL)
        return(-1); //invalid code
    
    if(togglestrings[i].group ==1){
        switch(togglestrings[i].icode){
            case SQUARE_RDS:
                if(c == '+')
                    expt.st->flag |= STIMULUS_IS_SQUARE;
                else
                    expt.st->flag &= ~(STIMULUS_IS_SQUARE);
                break;
        }
        bit = togglestrings[i].icode;
    }
    else if(togglestrings[i].group ==3)
    {
        if(c == '+'){
            optionflags[togglestrings[i].icode] = 1;
            /*
             * framecounts are either checked for every stim (-CF), or only for first 
             * of expt (+CF). When variable is 0  gets reset to 1. So setting to 2 keeps
             * it on check expt first only.
             */
            if(togglestrings[i].icode == CHECK_FRAMECOUNTS){
                optionflags[CHECK_FRAMECOUNTS] = 2;
                printf("Frame Checking First stim of expt only\n");
            }
        }
        else
            optionflags[togglestrings[i].icode] = 0;
        CheckOption(i);
        return(i);
    }
    else if(togglestrings[i].group ==2)
        bit2 = togglestrings[i].icode;
    
    if(c == '+')
    {
        optionflag |= bit;
        option2flag |= bit2;
    }
    else if(c == '-')	
    {	
        optionflag &= (~bit);
        option2flag &= (~bit2);
    }
    return(i);
}



#define POPUP -1
#define POPDOWN -2

int str2code(char *s)
{
    int i;
    for(i = 0; i < expt.totalcodes; i++)
    {
        if(strlen(valstrings[i].code) > 2 &&
           strncmp(valstrings[i].code, s, strlen(valstrings[i].code)) ==0)
            return(valstrings[i].icode);
    }
    for(i = 0; i < expt.totalcodes; i++)
    {	
        if(strlen(valstrings[i].code) == 2 &&
           strncmp(valstrings[i].code, s, 2) ==0)
            return(valstrings[i].icode);
    }
    return(-1);
}

float readval(char *s, 	Stimulus *TheStim)
{
    float val,fval, addval = 0;
    int ok;
    char *p;
    ok = 1;
    if(!isanumber(*s)){
        val = 1.0;
        addval = 0;
        if((p = strchr(&s[2],'+')) > 0)
            sscanf(++p,"%f",&addval);
        if(s[2] == '+')
        {
            sscanf(&s[3],"%f",&addval);
        }	      
        else if(s[2] == '*')
        {
            sscanf(&s[3],"%f",&val);
        }	      
        else{
            sscanf(&s[2],"%f",&val);
        }
        if(!strncmp(s,"sz",2)){
            if(lastsz >= 0)
                val = lastsz * val;
            else
                ok = 0;
        }
        else if(!strncmp(s,"tf",2)){
            if(lasttf >= 0)
                val = addval + lasttf *val;
            else
                ok = 0;
        }
        else if(!strncmp(s,"sf",2)){
            if(lastsf >= 0)
                val = lastsf * val;
            else
                ok=0;
        }
        else if(!strncmp(s,"or",2)){
            val = addval+lastor * val;
        }
        else if(!strncmp(s,"sp",2)){
            val = StimulusProperty(expt.st, STIM_PERIOD) * val;
        }
        else if(!strncmp(s,serial_strings[DEPTH_MOD],2)){
            val = addval + StimulusProperty(expt.st,DEPTH_MOD) * val;
        }
        else if(!strncmp(s,serial_strings[DISP_X],2)){
            val = addval + StimulusProperty(expt.st,DISP_X) * val;
        }
        else if(!strncmp(s,serial_strings[STIM_POLARANGLE],2)){
            val = addval + get_polarangle(expt.rf->pos[0],expt.rf->pos[1]) * val;
        }
        else if(!strncmp(s,serial_strings[STIM_ECCENTRICITY],2)){
            val = sqrt(sqr(pix2deg(expt.rf->pos[0])) + sqr(pix2deg(expt.rf->pos[1]))) * val;
        }
        else if(!strncmp(s,serial_strings[RF_ANGLE],2))
            val = addval+expt.rf->angle;
        else if(*s=='r' || *s == 's')
        {
            
            switch(s[1])
            {
                case 'P':
                case 'O':
                    val = addval;
                    break;
                case 'o':
                    val = addval + expt.rf->angle * val;
                    break;
                case 'w':
                    val = addval + pix2deg(expt.rf->size[0]) * val;
                    break;
                case 'h':
                    val = addval + pix2deg(expt.rf->size[1]) * val;
                    break;
                case 'y':
                    val = addval + pix2deg(expt.rf->pos[1]) *val;
                    break;
                case 'x':
                    val = addval + pix2deg(expt.rf->pos[0]) *val;
                    break;
                case 'v': /* get the existing value for expt type */
                    fval = StimulusProperty(TheStim,expt.mode);
                    val = addval + (fval * val);
                    break;
            }
            
        }
    }
    else
        sscanf(s,"%f",&val);
    if(ok)
        return(val);
    else
        return(NOTSET);
}

int ReadMonitorSetup(char *name)
{
    FILE *fd;
    char buf[BUFSIZ],*s;
    int nf = 0,w,h;
    
    if((fd = fopen(name,"r")) == NULL)
    {
        fprintf(stderr,"Can't Read Monitor %s\n",name);
        return(0);
    }
    expt.mon->loaded = 0;
    expt.mon->filename = myscopy(expt.mon->filename,name);
    while(fgets(buf, BUFSIZ, fd) != NULL){
        s = strchr(buf,'=');
        if(!strncmp(buf,serial_strings[XPIXEL_CODE],2) ||
           !strncmp(buf,serial_strings[YPIXEL_CODE],2) ||
           !strncmp(buf,serial_strings[VIEWD_CODE],2))
            InterpretLine(buf, &expt,0),nf++;
        else if(!strncmp(buf,"framerate",9) && s)
            sscanf(++s,"%f",&expt.vals[FRAMERATE_CODE]);
        else if(!strncmp(buf,serial_strings[FRAMERATE_CODE],2) && s)
            sscanf(++s,"%f",&expt.vals[FRAMERATE_CODE]);
        else if (!strncmp(buf,serial_strings[GAMMAVAL_CODE],2) && s)
            sscanf(++s,"%lf",&expt.mon->gamma);
        else if (!strncmp(buf,"winsize",6) && s){
            sscanf(++s,"%d %d",&w,&h);
            expt.winsiz[0] = w/2;
            expt.winsiz[1] = h/2;
        }
        else if (!strncmp(buf,"name",4) && s) 
            expt.mon->name = myscopy(expt.mon->name,++s);
        else if (!strncmp(buf,"crosstalk",7) && s){
            sscanf(++s,"%lf %lf",&expt.mon->crosstalk[0],&expt.mon->crosstalk[1]);
            printf("Crosstalk %.2lf, %.2lf\n",expt.mon->crosstalk[0],expt.mon->crosstalk[1]);
        }
    }
    fclose(fd);
    expt.mon->framerate = GetFrameRate();
    if(nf > 2)
        expt.mon->loaded = 1;
    printf("%s pixel = %.4f degrees FrameRate %.2f\n",name,pix2deg(1),expt.mon->framerate);
    return(expt.mon->loaded);
}

int LabelAndPath(char *s, char *sublabel, char *path, char *name)
{
    static char *results[3];
    char *t;
    int ret = 1;
    
    
    if(*s == '"'){
        strcpy(sublabel,s+1);
        t = strrchr(sublabel,'"');
        *t = 0;
        t = strrchr(s,'"');
        s = t;
        ret = 2;
        t++; // now points at '/'
    }
    else{
        sublabel[0] = 0;
        t = s;
    }
    
    if(path)
        strcpy(path,t);
    if((t = strrchr(s,'/')) == NULL)
        t = &s[0];
    else
        t++;
    if(name)
        strcpy(name,t);
    nonewline(name);
    nonewline(path);
    return(ret);
}



int KeyPressed(char c)
{
    static char instring[256];
    static int sctr = 0;


    switch(c){
        case 4: // F1
            if (demomode > 0)
                ReadExptFile(NULL, 0, 0 , 0);
            break;
        case 5: // F2
            RunOneTrial();
            break;
        case 6: // F3
            if (demomode > 0)
                runexpt(NULL,NULL,NULL);
            break;
        case 9: //F6
            SerialSignal(FREE_REWARD);
            break;
        case 27: //ESC
            if (demomode != 0)
                exit_program();
            break;
            case '\x7f':
            case '\b':
                if (sctr > 0)
            instring[--sctr] = 0;
            break;
            case '\r':
            case '\n':
            InterpretLine(instring, &expt, 0);
            sctr = 0;
            break;
        default:
            instring[sctr++] = c;
            break;
    }
}

int ReadConjPos(Expt*ex, char *line)
{
    int x,y;
    float ch,cv;
    
    sscanf(line,"$%3x%3x",&x,&y);
    ch = ((float)(x)/60)-20;
    cv = ((float)(y)/60)-20;
    conjpos[0] = (vcoord)deg2pix(ch);
    conjpos[1] = (vcoord)deg2pix(cv);
    
    return(0);
}
/*
 * Interpretline pases text strings from files, the serial line, and the GUI input pipe;
 * frompc = 1 mean it came form the serial line
 * frompc = 2 means it came from the GUI input line
 * frompc = 3 means generated within binoclean, eg in ReadCommandLine
 */


int InterpretLine(char *line, Expt *ex, int frompc)
{
    int i,n,len,ival,total,j,vals[MAXBINS],k,code,icode,oldmode,x,y;
    int bins,prebins,postbins,chan;
    char *s,*t,c,buf[BUFSIZ],nbuf[BUFSIZ],outbuf[BUFSIZ];
    float val,fval, addval = 0,in[10],aval,bval;
    Stimulus *TheStim = expt.st;
    static int lineflag = 0,lastticks = 0;
    MenuItem *new_menu = NULL;
    char *newnames = NULL;
    char *stimname = NULL;
    int flag = 0,ok,nq=0,found = 0,ret = -1;
    FILE *fd;
    char *sublabel,qlabel[BUFSIZ],qpath[BUFSIZ],qname[BUFSIZ];
    PGM *pgm;
    time_t nowtime;
    
    float fxpos[4];
    double dval;
    
    
    
    //	printf("%s\n",line);
    gettimeofday(&now,NULL);
/*
 * exp->cmdtype controls whether or not this line is send down the serial line. If it has come from verg, needs 
 * to go. So SetExptProberpty will set the event type to match
 */
    ex->cmdtype = frompc;
    if(lineflag & BACKSTIM_BIT)
        TheStim = ex->st->next;
    TheStim = stimptr;
    s = strchr(line, '=');
    if((t = strchr(line,015))  != NULL){
        if (verbose){
            fprintf(stderr,"^M in %s",line);
        }
        *t = 0;
    }
    else if(line[0] == '\\' || line[0] == '\!'){
        ReadCommand(&line[1]);
    }
    else if(line[0] == '\$'){
        ReadConjPos(ex,line);
    }
    else if(!strcmp(line,"whatsup")){
        sendNotification();
    }
    else if(!strncmp(line,"centerstim",8)){
        SetProperty(&expt,expt.st,SETZXOFF,GetProperty(&expt,expt.st,RF_X));
        SetProperty(&expt,expt.st,SETZYOFF,GetProperty(&expt,expt.st,RF_Y));
       return(0);
    }
    else if(!strncmp(line,"centerxy",8) || !strncmp(line,"sonull",6)){
        sprintf(buf,"%s\n",line);
        SerialString(buf,0);
        return(0);
    }
    else if(!strncmp(line,"freerwd",7)){
        SerialSignal(FREE_REWARD);
    }
    else if(!strncmp(line,"newexpt",7)){
        ResetExpt();
    }
    else if(!strncmp(line,"quicksave",7)){
        sscanf(&line[9],"%d",&i);
        sprintf(buf,"./q%dexp.stm",i);
        SaveExptFile(buf,QUICK_SAVE);
    }
    else if(!strncmp(line,"offdelay",8)){
        sprintf(buf,"%s\n",line);
        SerialString(buf,0);
        return(0);
    }
    else if(!strncmp(line,"showrwbias",10)){
        optionflags[SHOW_REWARD_BIAS] = 2;
        if((s = strchr(line,'=')) != 0){
            sscanf(++s,"%d",&optionflags[SHOW_REWARD_BIAS]);
        }
        return(0);
    }
    else if(!strncmp(line,"slider",6) && frompc){ //old commands that we don't want to drop down to codes below
        return(0);
    }
    else if(!strncmp(line,"splitctr",10) && frompc){
        if(seroutfile)
            fprintf(seroutfile,"%s",line);
        return(0);
    }	
    else if(!strncmp(line,"HeadStrain",10)){
        if(seroutfile)
            fprintf(seroutfile,"HeadStrain\n");
        return(0);
    }	
    else if(!strncmp(line,"RightHemisphere",10)){
        expt.hemisphere = 1;
        return(0);
    }
    else if(!strncmp(line,"NewMatlab",9)){
        PrintCodes(0);
        return(0);
    }
    else if(!strncmp(line,"QueryState",9)){
        SendAllToGui();
        if(!(expt.st->mode & EXPTPENDING)){ //if not in expt make sure verg knows 
            notify("\nEXPTOVER\n");
        }
        else
            notify("\nEXPTSTART\n");
        return(0);
    }
    else if(!strncmp(line,"usershake",9)){
        if(seroutfile)
            fprintf(seroutfile,"usershake %.2f\n",ufftime(&now));
        if(timeout_type == 0){
            start_timeout(SHAKE_TIMEOUT);
        }
        else{
            end_timeout();
        }
    }
    else if(!strncmp(line,"monkeyshake",10)){
        if(expt.vals[SHAKE_TIMEOUT_DURATION] > 0){
            if(seroutfile)
                fprintf(seroutfile,"monkeshake %.2f\n",ufftime(&now));
            if(timeout_type == 0)
                timeout_type = SHAKE_TIMEOUT;
            else if(timeout_type == SHAKE_TIMEOUT_PART2){
                start_timeout(SHAKE_TIMEOUT);
            }  // got shake during shake timeout
        }
        return(0);
    }
    
    else if(!strncmp(line,"stepper",6)  && (s = strchr(line,'=')) != 0){
        sscanf(++s,"%d %d",&x,&y);
        winposns[STEPPER_WIN].x = x;
        winposns[STEPPER_WIN].y = y;
        return(0);
    }
    else if(!strncmp(line,"penwin",6)  && (s = strchr(line,'=')) != 0){
        sscanf(++s,"%d %d",&x,&y);
        winposns[PENLOG_WIN].x = x;
        winposns[PENLOG_WIN].y = y;
        return(0);
    }
    else if(!strncmp(line,"bwwinxy",6)  && (s = strchr(line,'=')) != 0){
        sscanf(++s,"%d %d",&x,&y);
        winposns[BW_WIN].x = x;
        winposns[BW_WIN].y = y;
        return(0);
    }
    else if(!strncmp(line,"afcwinxy",6)  && (s = strchr(line,'=')) != 0){
        sscanf(++s,"%d %d",&x,&y);
        winposns[AFC_WIN].x = x;
        winposns[AFC_WIN].y = y;
        return(0);
    }
    else if(!strncmp(line,"optionwinxy",6)  && (s = strchr(line,'=')) != 0){
        sscanf(++s,"%d %d",&x,&y);
        winposns[OPTIONS_WIN].x = x;
        winposns[OPTIONS_WIN].y = y;
        return(0);
    }
    else if(!strncmp(line,"psychfile",6)  && (s = strchr(line,'=')) != 0){
        if (strlen(s) < 2){
            sprintf(outbuf,"psychfile=%s\n",psychfilename);
            notify(outbuf);
            return(0);
        }
        psychfilename = myscopy(psychfilename,++s);
        nonewline(psychfilename);
        if((t = strstr(s,"DATE")) != NULL){
            *t = 0;
            time(&nowtime);
            t= ctime(&nowtime);
            t[10] = 0;
            t[24] = 0;
            t[7] = 0;
            if(t[8]==' ')
                t[8] = '0';
            sprintf(buf,"%s%s%s%s",s,&t[8],&t[4],&t[20]);
            psychfilename = myscopy(psychfilename,buf);
            sprintf(outbuf,"Psych Log: %s\n",buf);
            statusline(outbuf);
            strcpy(nbuf,buf);
            if(seroutfile){
                fprintf(seroutfile,"Psych Log: %s\n",buf);
            }
            psychfile = fopen(buf,"a");
            sprintf(buf,"%s.log",s);
            psychfilelog = fopen(buf,"a");
        }
       else{
            psychfile = fopen(nonewline(s),"w");
            sprintf(nbuf,"%s",nonewline(s));
        }
        if (psychfile == NULL){
            sprintf(buf,"Can't open %s\n",nbuf);
            acknowledge(buf,NULL);
        }
        return(0);
    }
    else if(!strncmp(line,"ack:",4))
    {
        acknowledge(&line[4],NULL);
        return(0);
    }	
    else if(!strncmp(line,"StimInOverlay",10)){
        optionflags[STIMULUS_IN_OVERLAY] = 1;
        return(0);

    }
    else if(!strncmp(line,"spike2",6)){
        pcmode = SPIKE2;
        printf("PC is Spike2\n");
        return(0);
    }
    else if(!strncmp(line,"spkgain",6)){ /* from spike 2 */
        return(0);
    }
    else if(!strncmp(line,"lfpgain",6)){
        return(0);
    }
    else if(!strncmp(line,"epos:",5))
    {
        if(seroutfile && wurtzctr > 0){
            fprintf(seroutfile,"%s %d\n",line,fixed[wurtzctr-1]);
            fflush(seroutfile);
        }
        /*
         *  epos comes uncorrected for fixpos, and is rh lh rv lv
         * expt.stimvals[FIXPOS_Y] is what is changed in fy expts
         */
        sscanf(line,"%*s %f %f %f %f",&in[0],&in[1],&in[2],&in[3]);
        
        fxpos[2] = fxpos[3] = expt.stimvals[FIXPOS_Y];
        fxpos[0] = fxpos[1] = expt.stimvals[FIXPOS_X];
        i = (wurtzctr-1) % BUFSIZ;
        for(j = 0; j < 4; j++)
            trialeyepos[j][i] = in[j]-fxpos[j];
        if(psychfile && 0){
            fprintf(psychfile,"R10 rh=%.3f rv=%.3f lh=%.3f 0 0 0 lv=%.3f xx=0\n",trialeyepos[0][i],trialeyepos[2][i],trialeyepos[1][i],trialeyepos[3][i]);
        }
        CheckEyeDrift();
        return(0);
    }	
    else if(!strncmp(line,"helpfile",5) && (s = strchr(line,'=')) != 0)
    {
        SetExptString(&expt,expt.st, HELPFILE_PATH, ++s);
        return(0);
    }
    else if(!strncmp(line,"mycodes",5) && (s = strchr(line,'=')) != 0)
    {
        s++;
        j = 0;
        while(s[j] != 0){
            if(s[j] == '+' || s[j] == '-'){
                if((i = str2code(&s[++j])) >= 0)
                    showcodes[i] = 1;
            }
            j++;
        }
    }
    else if(!strncmp(line,"monitor",5) && (s = strchr(line,'=')) != 0)
    {
        sscanf(++s,"%s",buf);
        commstrings[MONITOR_FILE].value = myscopy(commstrings[MONITOR_FILE].value,buf);
        ReadMonitorSetup(buf);
        return(-1);
    }
    else if(!strncmp(line,"clearquick",7)){
        nquickexpts = 0;
        for(i = 0; i < MAXQUICK_SUB; i++)
            nsubexpts[i] = 0;
        nallsubs = 0;
        return(0);
    }
    else if(!strncmp(line,"test",4)){
        if(!strncmp(line,"testgabor",8)){
            //	    handle_pushbuttons(NULL,(XtPointer)(TEST_BUTTON), NULL);
        }
        else
            sscanf(line,"test %d",&testmode);
        return(0);
    }
    else if(!strncmp(line,"ack",3)){
        acknowledge(&line[4],NULL);
        return(-1);
    }
    else if(!strncmp(line,"split",5)){
        i = sscanf(line,"split %d %d",&ival,&len);
        if(i ==2)
            psychoff[1] = len;
        if(i > 0)
            psychoff[0] = ival;
        return(0);
    }
    else if(!strncmp(line,"backim",5)  && (s = strchr(line,'='))){
        ReadPGM(nonewline(++s),&expt.backim);
    }
    else if(!strncmp(line,"impref",5)  && (s = strchr(line,'='))){
        if(strlen(s) > 2)
            expt.st->imprefix = myscopy(expt.st->imprefix,nonewline(++s));
        else
            return(MAXTOTALCODES);
    }
    else if(!strncmp(line,"imload",5)  && (s = strchr(line,'='))){
// used to be done with immode, but best to have one attribute per code for sending to verg
        ok = 1;
        if(!strncmp(++s,"preload",4)){
            expt.st->preload = 1;
        }
        else if(!strncmp(s,"load",4)){
            expt.st->preload = 0;
        }
        else
            ok = 0;
    }
    else if(!strncmp(line,"immode",5)  && (s = strchr(line,'='))){
        ok = 1;
        if(!strncmp(++s,"orbw",4))
            expt.st->immode = IMAGEMODE_ORBW;
        else if(!strncmp(s,"plain",4)){
            expt.st->immode = 0;
            expt.st->nimseed = 0;
        }
        else if(!strncmp(s,"binocular",4)){
            expt.st->immode = BINOCULAR_PLAIN_IMAGES;
            expt.st->nimseed = 0;
        }
        else if(!strncmp(s,"preload",4)){
            expt.st->preload = 1;
        }
        else if(!strncmp(s,"load",4)){
            expt.st->preload = 0;
        }
        else
            ok = 0;
        if(ok){// recognized command
            if(seroutfile)
                fprintf(seroutfile,"%s\n",line);
            return(MAXTOTALCODES);
        }
        else
            return(-1);
    }
    else if(!strncmp(line,"oldrf",5)){
        sscanf(line,"oldrf %f %f %d %d %f",&oldrfs[rfctr].pos[0],&oldrfs[rfctr].pos[1],&oldrfs[rfctr].size[0],&oldrfs[rfctr].size[1],&oldrfs[rfctr].angle);
        oldrfs[rfctr].flag = CENTERMARK_ON;
        if(++rfctr >= MAXRF)
            rfctr = MAXRF-1;
    }
    
    else if(!strncmp(line,"exps",4) && (s = strchr(line,'=')))
    {
        if(!reading_quickexp){
            nexpshow[0] = 0;
            showexp[0][TOTALCODES]=1;
            for(j = 0; j < MAXTOTALCODES; j++)
                showexp[0][j] = 0;
        }
        j = k = 0;
        while(s[j] != 0){
            if(s[j] == '+'){
                sscanf(&s[++j],"%s",buf);
                if((code = str2code(buf)) != EXPTYPE_NONE && !showexp[k][code]){
                    expshoworder[k][nexpshow[k]++] = code;
                    showexp[k][code] = 1;
                }
            }
            else if(s[j] == '-'){
                sscanf(&s[++j],"%2s",buf);
            }
            else if(s[j++] == ':'){
                if(!reading_quickexp){
                    showexp[1][TOTALCODES]=1;
                    nexpshow[1] = 0;
                    for(i = 0; i < MAXTOTALCODES; i++)
                        showexp[1][i] = 0;
                }
                k = 1;
                if(!showexp[k][EXPTYPE_NONE]){
                    showexp[k][EXPTYPE_NONE] = 1;
                    expshoworder[k][nexpshow[k]++] = EXPTYPE_NONE;
                }
            }
        }
        /* don't let this one be removed */
        showexp[1][EXPTYPE_NONE] = 1;
        return(0);
    }
    else if(line[0] == '\?'){
        i = FindCode(&line[1]);
        val = GetProperty(ex,stimptr,i);
        if((s = strchr(line,':'))){
            if((t = strchr(line,'>'))){
                sscanf(++t,"%f",&fval);
                if(val > fval){
                    ret = InterpretLine(++s,ex,0);
                }
            }
            else if((t = strchr(line,'<'))){
                sscanf(++t,"%f",&fval);
                if(val < fval){
                    ret = InterpretLine(++s,ex,0);
                }
            }
        }
        return(ret);
    }
    
    /*
     * ignore anything after a #
     */
    if((t = strchr(line,'#')) != NULL){
        *t-- = 0;
        while(isspace(*t) && t >= line)
            *t-- = 0;
    }
    
    /* skip over non-printing codes and leading whitespace */
    while((isserialcode(*line) || *line == ' ') && *line != 0)
        line++;
    if(*line == '#' || strlen(line) < 3)
        return(-1);
    
    code = FindCode(line);

    
    /* 
     * these are now all set in the monitor file: don't allow them to be set in 
     * user files...
     */
    if(expt.mon->loaded && (!strncmp(line,serial_strings[XPIXEL_CODE],2) ||
                            !strncmp(line,serial_strings[YPIXEL_CODE],2) ||
                            !strncmp(line,serial_strings[GAMMAVAL_CODE],2) ||
                            !strncmp(line,serial_strings[VIEWD_CODE],2)))
        return(code);
    
    
    if(code >= 0){
        icode = valstringindex[code];
        s = &line[strlen(valstrings[icode].code)];
    }
    else{
        icode = -1;
        s = &line[2];
    }
    
    while(*s != 0 && (isspace(*s) || *s == '='))
        s++;
    j = 0;
    i = 0;
    // string with no value means report back current value
    
    if(strlen(s) == 0 && code < MAXTOTALCODES){
        MakeString(code,buf,&expt, expt.st, TO_GUI);
        notify(buf);
        return(code);
    }
    if (strcmp(line,"EDONE") == NULL){
//send back current state once expt loaded
        SendAllToGui();
        ListExpStims(NULL);
        ShowTrialsNeeded();
    }
    
    if(line[0] == 'E' && isdigit(line[1]))
    {
        sscanf(&line[1],"%d",&i);
        s = strchr(line,'=');
        if(i < expt.nstim[0] && s != NULL)
        {
            s++;
            if((val = readval(s,TheStim)) > NOTSET){
                expval[i+expt.nstim[2]] = val;
                expt.customvals[i] = val;
                mode |= CUSTOM_EXPVAL;
            }
        }
        if (frompc < 2)
            ListExpStims(NULL);
        
        return(MAXTOTALCODES);
    }
    else if(line[0] == 'E' && line[1] == 'C' && isdigit(line[2])){
        sscanf(&line[2],"%d",&i);
        s = strchr(line,'=');
        if(i < expt.nstim[4] && s != NULL)
        {
            s++;
            sscanf(s,"%lf",&dval);
            expt.exp3vals[i] = dval;
            optionflags[CUSTOM_EXPVALC] = 1;
        }
        if (frompc < 2)
            ListExpStims(NULL);
        code = EXPT3_NSTIM;
    }
    else if(line[0] == 'E' && line[1] == 'B' && isdigit(line[2]))
    {
        sscanf(&line[2],"%d",&i);
        s = strchr(line,'=');
        if(i < expt.nstim[1] && s != NULL)
        {
            s++;
            sscanf(s,"%lf",&expval[expt.nstim[0]+i+expt.nstim[2]]);
            optionflags[CUSTOM_EXPVALB] = 1;
        }
        if (frompc < 2)
            ListExpStims(NULL);
    }
    else switch(code)
    {
        case COMMAND_FILE:
            s = strchr(line,'=');
            if(s)
                expt.cmdinfile = myscopy(expt.cmdinfile,++s);
            nonewline(expt.cmdinfile);
            break;
        case USENEWDIRS:
            sscanf(++s,"%d",&usenewdirs);
            break;
        case FIXCOLORS:
            n = sscanf(++s,"%f %f %f %f",&in[0],&in[1],&in[2],&in[3]);
                for(i = 0; i < n; i++)
                    expt.st->fix.fixcolors[i] = in[i];
            break;
        case ELECTRODE_DEPTH:
            if (*s == '+'){
                sscanf(s,"%f",&fval);
                stepproc(fval);
                sprintf(buf,"%2s=%.3f\n",valstrings[icode].code,expt.vals[ELECTRODE_DEPTH]);
                notify(buf);
            }
            break;
            
        case UFF_TIME:
        {
            sscanf(s,"%lu",&bwticks);
            gettimeofday(&bwtime,NULL);
            if(penlog && (bwticks - lastticks > 6000000 || bwticks < lastticks)){
                fprintf(penlog,"%lu bwticks = %s\n",bwticks,binocTimeString());
                lastticks = bwticks;
            }
            if(seroutfile){
                fprintf(seroutfile,"#%s\n",s);
                fprintf(seroutfile,"#%lu bwticks = %s\n",bwticks,binocTimeString());
            }
        }
            break;
        case QUICKEXPT_CODE:
            
            /*try setting sublabel to NULL afterwards so that all items
             * appear on main menu.
             * not setting up the submenu in build_quickmenu
             */
            LabelAndPath(s, qlabel, qpath, qname);
            t = qname;
            quicknames[nquickexpts] = myscopy(quicknames[nquickexpts],s);
            nquickexpts++;
            /* Do nothing if this is already on the list */
            //Ali	  if(oldnquick && quick_menu){
            //	    for(i = 0; i <= oldnquick; i++){
            //	    if(quick_menu[i].label && !strcmp(t,quick_menu[i].label))
            //	      return(0);
            //	  }
            //	  }
            //	  sublabel = NULL;
            //	  if(qlabel){
            //	    sublabel = myscopy(sublabel, qlabel);
            //	  }
            //	  
            //	  if(quick_menu == NULL){
            //	    quick_menu = (MenuItem *)malloc(sizeof(MenuItem) * (200));
            //	    memset(quick_menu, 0, (sizeof(MenuItem) * (200)));
            //	  }
            /*
             new_menu = (MenuItem *)malloc(sizeof(MenuItem) * (nquickexpts+10));
             memset(new_menu, 0, (sizeof(MenuItem) * (nquickexpts+10)));
             
             if(quick_menu != NULL)
             {
             memcpy(new_menu, quick_menu, (sizeof(MenuItem) * (nquickexpts+5)));
             free(quick_menu);
             quick_menu = NULL;
             }
             quick_menu = new_menu;
             
             for(i = 0; i < MAXQUICK_SUB; i++){
             new_menu = (MenuItem *)malloc(sizeof(MenuItem) * (nquickexpts+10));
             memset(new_menu, 0, (sizeof(MenuItem) * (nquickexpts+10)));
             if(quick_submenu[i] != NULL)
             {
             memcpy(new_menu, quick_submenu[i], (sizeof(MenuItem) * (nquickexpts+5)));
             free(quick_submenu[i]);
             quick_submenu[i] = NULL;
             }
             quick_submenu[i] = new_menu;
             }
             */
            //	  for(i = 0; i < MAXQUICK_SUB; i++){
            //	    if(quick_submenu[i] == NULL){
            //	      quick_submenu[i] = (MenuItem *)malloc(sizeof(MenuItem) * (100));
            //	      memset(quick_submenu[i], 0, (sizeof(MenuItem) * (100)));
            //	    }
            //	  }
            //
            //	  if(nquickexpts == 0 && !reading_quickexp)
            //	    {
            //              for(i = 0; i < nsubmenus; i++){
            //		quicksubnames[i] = myfree(quicksubnames[i]);
            //	      }
            //	      nsubmenus = 0;
            //	      for(i = 0; i < MAXQUICK_SUB; i++){
            //		quick_submenu[i][0].label = myscopy(quick_submenu[i][0].label,"Submenus");
            //		quick_submenu[i][0].callback = expbuttons;
            //		quick_submenu[i][0].callback_data = (void *)(CLOSE_EXPT);
            //		quick_submenu[i][0].subitems = NULL;
            //		quick_submenu[i][1].subitems = NULL;
            //		quick_submenu[i][1].label = NULL;
            //	      }
            //
            //	      quick_menu[0].label = myscopy(quick_menu[0].label,"End");
            //	      quick_menu[0].callback = expbuttons;
            //	      quick_menu[0].callback_data = (void *)(CLOSE_EXPT);
            //	      quick_menu[0].subitems = NULL;
            //	      quick_menu[1].subitems = NULL;
            //	      quick_menu[1].label = NULL;
            //	      nquickexpts++;
            //	    }
            //
            //	     if(sublabel){
            //	       found = 0;
            //              for(i = 0; i < nsubmenus; i++){
            //		if(!strcmp(sublabel,quicksubnames[i])){
            //		  found++;
            //		  break;
            //		}
            //              }
            //	      if(!found){
            //		if(nsubmenus >= MAXQUICK_SUB)
            //		  nsubmenus--;
            //		i = nsubmenus;
            //		nq = nquickexpts-nallsubs+nsubmenus++;
            //		quick_menu[nq].label = myscopy(quick_menu[nq].label,sublabel);
            //		quicksubnames[i] = myscopy(quicksubnames[i],sublabel);
            //		quick_menu[nq].callback = NULL;
            //		quick_menu[nq].callback_data = (void *)i;
            //		quick_menu[nq].subitems = quick_submenu[i];
            //		quick_menu[nq+1].label = NULL;
            //	      }
            //	      else{
            //		nonewline(t);
            //		  for(j = 0; j <= nsubexpts[i]; j++){
            //		    if(quick_submenu[i][j].label && !strcmp(t,quick_submenu[i][j].label))
            //		      return(0);
            //		  }
            //	      }
            //	      quicknames[nquickexpts] = myscopy(quicknames[nquickexpts],qpath);
            //	      quicksubid[nquickexpts] = i;
            //	      quick_submenu[i][nsubexpts[i]].label = myscopy(quick_submenu[i][nsubexpts[i]].label,nonewline(t));
            //	      quick_submenu[i][nsubexpts[i]+1].label = NULL;
            //	      quick_submenu[i][nsubexpts[i]].callback_data = (void *)(EXPT_LOAD1+nquickexpts-1);
            //	      quick_submenu[i][nsubexpts[i]].callback = expbuttons;
            //	      nsubexpts[i]++;
            //	      nquickexpts++;
            //	      nallsubs++;
            //	      free(sublabel);
            //	     }
            //	     else{
            //	       nq = nquickexpts-nallsubs+nsubmenus;
            //	       quick_menu[nq].label = myscopy(quick_menu[nquickexpts].label,t);
            //	       quick_menu[nq].callback = expbuttons;
            //	       quick_menu[nq].callback_data = (void *)(EXPT_LOAD1+nquickexpts-1);
            //	       quick_menu[nq].subitems = NULL;
            //	       quick_menu[nq+1].subitems = NULL;
            //	       quick_menu[nq+1].label = NULL;
            //	       quicknames[nquickexpts] = myscopy(quicknames[nquickexpts],nonewline(s));
            //	       quicksubid[nquickexpts] = -1;
            //	       nquickexpts++;
            //	     }
            break;
        case CYBER_CHANNELS:
            sscanf(s,"%d",&chan);
            chan--;
            sscanf(s,"%*d %d %d %d %d %d %f",&cyberprops[chan].gain[0],
                   &cyberprops[chan].gain[1],&cyberprops[chan].ac[0],&cyberprops[chan].ac[1],&cyberprops[chan].lp,&cyberprops[chan].dc);
            break;
        case CONJUGATE_POSITION:
            sscanf(s,"%f,%f",&val,&fval);
            conjpos[0] = (vcoord)deg2pix(val);
            conjpos[1] = (vcoord)deg2pix(fval);
            break;
        case TRAPEZOIDAL_SCALING:
            i = sscanf(s,"%f %f %f %f",&val,&fval,&aval,&bval);
            if(val > 0.1){
                mon.trapscale[0] = val;
                mon.trapscale[1] = 1/val;
            }
            if(i > 1 && fval > 0.1){
                mon.trapscale[2] = fval;
                mon.trapscale[3] = 1/fval;
            }
            if(i > 2 && aval > 0.1){
                mon.trapscale[4] = aval;
                mon.trapscale[5] = 1/aval;
            }
            if(i > 3 && bval > 0.1){
                mon.trapscale[6] = bval;
                mon.trapscale[7] = 1/bval;
            }
            break;
        case SOFTOFF_CODE:
            sscanf(s,"%lf %lf %lf %lf",&expt.softoff[0],
                   &expt.softoff[1],&expt.softoff[2],
                   &expt.softoff[3]);
            //ALi SetWPanel();
            if (frompc == 1) // from spike 2
                SendToGui(SOFTOFF_CODE);
            else
                SerialSend(code);
            if (frompc == 3) //read from file
                notify(line);
            return(code);
        case UKA_VALS:
            i = sscanf(s,"%f %f %f %f %f",&in[0],&in[1],&in[2],&in[3],&in[4]);
            for(j = 0; j < 4; j++){
                if(i > j)
                    afc_s.gregvals[j] = in[j];
            }
            break;
        case STIMULUS_FLAG:
            while(s[j] != 0)
            {
                if(( c = s[j]) == '+' || c == '-')
                {
                    j++;
                    k = 0;
                    while(flag_codes[k] != NULL)
                    {
                        if(!strncmp(&s[j],flag_codes[k],2))
                        {
                            if(c == '+')
                                TheStim->flag |= (1<<k);
                            else
                                TheStim->flag &= (~(1<<k));
                        }
                        k++;
                    }
                    
                }
                else
                    j++;
            }
            break;
        case SHOWFLAGS_CODE:
            expt.showflags = myscopy(expt.showflags,s);
            
            break;
        case OPTION_CODE:
            if(isdigit(s[0])){
                sscanf(s,"%ld",&optionflag);
                // if seting optionflag to 0, set all flags to 0.
                if(optionflag == 0){
                    option2flag = 0;
                    for(i = 0; i <= MAXALLFLAGS; i++)
                        optionflags[i] = 0;
                }
            }
            if (strchr(s,'+') || strchr(s,'-')) {
                
            while(s[j] != 0)
            {
                if(s[j] == '+' || s[j] == '-')
                    ChangeFlag(&s[j++]);
                else
                    j++;
            }
            }
            setoption();
            SerialSend(OPTION_CODE);
            break;
        case MODE_CODE:
            oldmode = TheStim->mode;
            if(isdigit(s[0]))
                sscanf(s,"%d",&TheStim->mode);
            else if(!strncasecmp(s,"st3",3))
            {
                lineflag |= BACKSTIM_BIT;
                if(ex->st->next == NULL)
                {
                    ex->st->next = NewStimulus(ex->st);
                    ex->st->next->next->splane = 0x2;
                }
                if(ex->st->next->next == NULL)
                {
                    ex->st->next->next = NewStimulus(ex->st->next);
                    ex->st->next->next->splane = STIM_THIRD_BIT;
                }
                stimptr = ex->st->next->next;
                TheStim = ex->st->next->next;
            }
            else if(!strncasecmp(s,"ChoiceU/R",7)){
                stimptr = TheStim = ChoiceStima;
            }
            else if(!strncasecmp(s,"ChoiceD/L",7)){
                stimptr = TheStim = ChoiceStimb;
            }
            else if(!strncasecmp(s,"bac",3))
            {
                lineflag |= BACKSTIM_BIT;
                if(ex->st->next == NULL)
                {
                    ex->st->next = NewStimulus(ex->st);
                }
                stimptr = ex->st->next;
                TheStim = ex->st->next;
            }
            else if(!strncasecmp(s,"for",3))
            {
                lineflag &= ~(BACKSTIM_BIT);
                TheStim = expt.st;
                stimptr = expt.st;
            }
            else 
            {
                while(s[j] != 0)
                {
                    if(s[j] == '+' || s[j] == '-')
                    {
                        c = s[j++];
                        i = 0;
                        while(mode_codes[i] != NULL)
                        {
                            if(!strncmp(&s[j],mode_codes[i],2))
                                break;
                            i++;
                        }
                        if(mode_codes[i] != NULL)
                        {
                            if(c == '+')
                                TheStim->mode |= (1<<i);
                            if(c == '-')
                                TheStim->mode &= (~(1<<i));
                        }
                    }
                    j++;
                }
            }
            TheStim->mode &= (~ONLINEBITS);
            break;
        case BACKSTIM_TYPE:
        case STIMULUS_TYPE_CODE:
            for(i = 0; i < N_STIMULUS_TYPES; i++)
                if(!strncasecmp(s,stimulus_names[i],strlen(s)-1))
                    break;
            if(i >= N_STIMULUS_TYPES)
                i = STIM_NONE;
            if(code == BACKSTIM_TYPE){
                if(TheStim->next == NULL){
                    NewStimulus(TheStim);
                    StimulusType(TheStim->next, i);
                    TheStim->next->prev = TheStim;
                    TheStim->next->splane = 0x2;
                }
                else
                    StimulusType(TheStim->next, i);
            }
            else if(code == STIMULUS_TYPE_CODE){
                StimulusType(TheStim, i);
                if(i == STIM_RDS && TheStim->next && TheStim->next->type == STIM_RLS)
                    StimulusType(TheStim->next, i);
                if(i == STIM_RLS && TheStim->next && TheStim->next->type == STIM_RDS)
                    StimulusType(TheStim->next, i);
                if( i == STIM_RDS && expt.stimmode == RDSRLSSETSL){
                    SetStimulus(TheStim, 0, SET_SEEDLOOP,  NULL);
                    optionflag &= (~SQUARE_RDS);
                }
                if( i == STIM_RLS && expt.stimmode == RDSRLSSETSL){
                    SetStimulus(TheStim, 1, SET_SEEDLOOP, NULL);
                    optionflag |= SQUARE_RDS;
                }
            }
            SerialSend(code);
            break;
        case SPIKE_TIMES:
            /*		printf("Spikes Times%s\n",s);*/
            break;

        case UFF_PREFIX: //don't send to Spike2 when get new name. Wait for Open button
            SetExptString(ex, TheStim, code, s);
            break;
        case PENETRATION_TEXT:
        case LOGFILE_CODE:
        case CHANNEL_CODE:
        case USERID:
        case BACKGROUND_IMAGE: 
        case MONKEYNAME:
            SetExptString(ex, TheStim, code, s);
            SerialSend(code);
            break;
        case QUERY_STATE:
            gettimeofday(&now,NULL);
            sprintf(buf,"%s\nfs%d\nss%d\nes%d tc%d %u",&line[2],fixstate,stimstate,expstate,trialcnt,ufftime(&now));
            acknowledge(buf,NULL);
            if(seroutfile)
                fprintf(seroutfile,"%2s%s\n",valstrings[icode].code,buf);
            return(code);
        case EXPTYPE_CODE3:
        case EXPTYPE_CODE2:
        case EXPTYPE_CODE:
        case HIGHXTYPE:
        case COVARIATE:
            if(isdigit(*s))
                sscanf(s,"%d",&j);
            else{
                if(s[strlen(s)-1] == '\n')
                    s[strlen(s)-1] = 0;
                if((t = strchr(s,' ')))
                    *t = 0;
                j = str2code(s);
            }
            if(j < MAXTOTALCODES){
                /*  make sure the exptype is shown */
                if(code == EXPTYPE_CODE){
                    for(i = 0; i < nexptypes[0]; i++){
                        if(firstmenu[i].val == j && showexp[0][j] == 0){
                            showexp[0][j] = 1;
                            expshoworder[0][nexpshow[0]] = j;
                            nexpshow[0]++;
                        }
                    }
                }
                if(code == EXPTYPE_CODE2){
                    for(i = 0; i < nexptypes[1]; i++){
                        if(secondmenu[i].val == j && showexp[1][j] == 0){
                            showexp[1][j] = 1;
                            expshoworder[1][nexpshow[1]] = j;
                            nexpshow[1]++;
                        }
                    }
                }
                if(code == EXPTYPE_CODE3){
                }
                SetExptProperty(ex, TheStim,code, (float)(j));
            }
            break;
        case RELDISP:
        case DISP_BACK:
        case DISP_X:
        case DEPTH_MOD:
        case FP_MOVE_DIR:
        case SETZXOFF:
        case SETZYOFF:
        case STIM_WIDTH:
        case STIM_HEIGHT:
        case STIM_SIZE:
        case EXPT_INCR:
        case EXPT3_INCR:
        case EXPT2_INCR:
        case EXPT_MEAN:
        case EXPT2_MEAN:
        case EXPT3_MEAN:
        case ORIENTATION:
        case BACK_ORI:
        case SF:
        case SF2:
            if((val = readval(s,TheStim)) > NOTSET)
                ok = 1;
            else
                ok = 0;
            /*
             ok = 1;
             if(!isanumber(*s)){
             val = 1.0;
             addval = 0;
             if(s[2] == '+')
             {
             sscanf(&s[3],"%f",&addval);
             }	      
             else{
             sscanf(&s[2],"%f",&val);
             }
             if(!strncmp(s,"sz",2)){
             if(lastsz >= 0)
             val = lastsz * val;
             else
             ok = 0;
             }
             else if(!strncmp(s,"tf",2)){
             if(lasttf >= 0)
             val = addval + lasttf *val;
             else
             ok = 0;
             }
             else if(!strncmp(s,"sf",2)){
             if(lastsf >= 0)
             val = lastsf * val;
             else
             ok=0;
             }
             else if(!strncmp(s,"sp",2)){
             val = StimulusProperty(expt.st, STIM_PERIOD) * val;
             }
             else if(!strncmp(s,"dx",2)){
             val = StimulusProperty(expt.st, DISP_X) * val;
             }
             else if(!strncmp(s,serial_strings[STIM_POLARANGLE],2)){
             val = addval + get_polarangle(expt.rf->pos[0],expt.rf->pos[1]) * val;
             }
             else if(!strncmp(s,serial_strings[STIM_ECCENTRICITY],2)){
             val = sqrt(sqr(pix2deg(expt.rf->pos[0])) + sqr(pix2deg(expt.rf->pos[1]))) * val;
             }
             else if(*s=='r' || *s == 's' || *s == 'R')
             {
             
             switch(s[1])
             {
             case 'P':
             case 'O':
             val = addval;
             break;
             case 'o':
             val = addval + expt.rf->angle;
             break;
             case 'w':
             val = addval + pix2deg(expt.rf->size[0]) * val;
             break;
             case 'h':
             val = addval + pix2deg(expt.rf->size[1]) * val;
             break;
             case 'y':
             val = addval + pix2deg(expt.rf->pos[1]);
             break;
             case 'x':
             val = addval + pix2deg(expt.rf->pos[0]);
             break;
             case 'v':
             fval = StimulusProperty(TheStim,expt.mode);
             val = addval + (fval * val);
             break;
             }
             
             }
             }
             else
             sscanf(s,"%f",&val);
             */
            if(code == EXPT_INCR){
                if(strstr(s,"log"))
                    expt.flag |= LOGINCR;
                else if(strstr(s,"lin"))
                    expt.flag &= (~LOGINCR);
            }
            if(code == EXPT2_INCR){
                if(strstr(s,"log"))
                    expt.flag |= LOGINCR2;
                else if(strstr(s,"lin"))
                    expt.flag &= (~LOGINCR2);
            }
            if(ok)
                SetExptProperty(ex, TheStim,code, val);
            break;
        case SETFIXCOLOR:
        case SETBACKCOLOR:
        case BLANKCOLOR_CODE:
        case SETOVERLAYCOLOR:
            sscanf(s,"%f",&val);
            if(val > 2)
                val = val/255.0;
            SetExptProperty(ex, TheStim,code, val);
            break;
        case TRIAL_COUNT:
            sscanf(s,"%d",&trialcnt);
            break;
        case SACCADE_DETECTED:
            sscanf(s,"%f",&val);
            if(frompc != 1)
                expt.vals[SACCADE_DETECTED] = val;
            break;
        case DISP_P:
            sscanf(s,"%f",&val);
            if(strstr(s,"rad"))
                val = val * 180/M_PI;
            SetExptProperty(ex, TheStim,code, val);
            break;
        case UFF_COMMENT:
            SerialString(line,NULL);
            break;
        
        case TF:
            if(!strncmp(s,"tf",2)){
                SetExptProperty(ex, TheStim,code, lasttf);
                break;
            }
            sscanf(s,"%f",&val);
            if(lasttf < 0)
                lasttf = val;
            // otherwise go on to the default...	    
        default:
            if(code < MAXTOTALCODES)
            {
                sscanf(s,"%f",&val);
                SetProperty(ex, TheStim,code, val);
            }
            else if(line[0] != '\n')
            {
                fprintf(stderr,"At %u Don't recognize (%d):%s\n",ufftime(&now),(int)line[0],line);
                i = 0;
                while(line[i])
                    if(line[i++] == CONJUG_OUT)
                        fprintf(stderr,"Char %d is Conjug out\n",i);
                if(optionflag & SHOW_CONJUG_BIT)
                    ShowConjugReadState(line);
                code = -1;
            }
            break;
    }
    
    /*
     * Only lines with no # character are allowed to have mulitple codes
     * on a line. 
     * some strings are multi fields. Don't treat the rest of the line
     * as more input codes
     */
    
    if((t = strchr(s,'#')) == NULL){
        switch(code){
            default:
                if((t = strchr(s,' ')) != NULL && *(++t))
                    InterpretLine(t, ex,0);
                break;
            case SOFTOFF_CODE:
            case QUERY_STATE:
            case UFF_PREFIX:
            case CYBER_CHANNELS:
                break;
        }
    }
    if (frompc != 2 && code >= 0){  // send to verg if it came from Spike2 or binoc GUI
        notify(line);
        notify("\n");
    }
    return(code);
}


int ButtonResponse(int button, int revise, vcoord *locn)
{
    float val;
    int res = -1, stim,retval = 0;
    Expstim *es = expt.plot->stims;
    Thisstim *stp;
    char c,buf[BUFSIZ],sbuf[BUFSIZ],rbuf[BUFSIZ];
    static int lastbutton = -1;
    static int laststim = -1;
    int stimid = expt.stimid;
    int ifcfirst = 0;
    int correct = 0;
    float rt;
    int sign = 0, aid = 0;
    
    stim = expt.stimid;
    
    //  if(option2flag & IFC){
    /*
     * left button % stored as spike 0., type 0.
     * this count is divided by nreps at plotting time, so each
     * button press scores 100% here
     */
    if (psychfile){
        fprintf(psychfile,"R%d\n",correct);
    }
    sign = afc_s.stimsign;
    if(2-button == sign)
        res = 1;
    else
        res = 0;

    PrintPsychLine(res, sign);
    if(expt.stimid >= 0){
        if(option2flag & IFC)
            res = ifcanswer;
        /*
         * use stp->vals, not GetProperty, in case this is IFC
         */
        if(expt.type2 != EXPTYPE_NONE)
            sprintf(rbuf," %s=%4f",serial_strings[expt.type2],stp->vals[1]);
        else
            sprintf(rbuf,"");
        if(expt.type3 != EXPTYPE_NONE)
            sprintf(sbuf," %s=%.4f",serial_strings[expt.type3],stp->vals[2]);
        else
            sprintf(sbuf,"");
        
        framesum += framesdone;
        realframesum += realframes;
        fputs(buf,stdout);
        if(seroutfile)
            fputs(buf,seroutfile);
        if(psychlog){
            if(!revise)
                PrintRDSDispCounts(psychlog);
            if(expt.st->type == STIM_IMAGE)
                sprintf(buf," %s",expt.st->imname);
            else if(isharris(expt.vals[ALTERNATE_STIM_MODE]))
                sprintf(buf," %.0f %.0f %d",locn[0],locn[1],expt.st->nframes);
            else
                sprintf(buf,"");
            fprintf(psychlog,"%.4f %c%s%s %.3f %.3f %d%s\n",stp->vals[0],c,rbuf,sbuf,StimTime(&now),StimDuration(),res,buf);
            fflush(psychlog);
        }
        if(framesdone < 0.9 * realframes && expt.vals[ALTERNATE_STIM_MODE] < 0.5 && optionflags[FRAMESWARN]){
            sprintf(buf,"Only completed %d of %d frames",framesdone,realframes);
            acknowledge(buf,NULL);
        }
    }
    return(retval);
}

char *DescribeStim(Stimulus *st)
{
    int i,n,code;
    static char dbuf[512];
    char buf[256];
    float val = 0,a,b,c,d;
    
    if(stimdurn > 0)
        val = stimdursum/stimdurn; 
    sprintf(dbuf,"%s %s=%.3f (%.0f frames %.2f)",stimulus_names[st->type],serial_strings[STIMULUS_DURATION_CODE],GetProperty(&expt,st,STIMULUS_DURATION_CODE),GetProperty(&expt,st,NFRAMES_CODE),val);
    if(optionflag & SQUARE_RDS)
    {
        sprintf(buf,"+%s",togglestrings[toggleidx[SQUARE_RDS]].code);
        strcat(dbuf,buf);
    }
    strcat(dbuf,",");
    if(option2flag & IFC)
    {
        sprintf(buf,"%2s=%.2f,%2s=%.2f,",serial_strings[IFCSCALE],expt.vals[IFCSCALE],serial_strings[ISI_CODE],expt.vals[ISI_CODE]);
        strcat(dbuf,buf);
    }
    for(code = 0; code < expt.totalcodes; code++)
    {
        i = valstrings[code].icode;
        switch(i){
            case ORIENTATION:
            case STIM_WIDTH:
            case STIM_HEIGHT:
            case FIXPOS_X:
            case FIXPOS_Y:
            case FRAMERATE_CODE:
            case SETZXOFF:
            case SETZYOFF:
            case NTRIALS_CODE:
            case NREPS_CODE:
            case FIXATION_SURROUND:
            case TRIAL_START_BLANK:
                sprintf(buf,"%2s=%.*f,",serial_strings[i],nfplaces[i],GetProperty(&expt,st,i));
                strcat(dbuf,buf);
                break;
            default: /* these depend on stim type */
                if(st->type == STIM_GRATING) switch(i)
                {
                    case ORIENTATION_DIFF:
                    case SF:
                    case PLAID_ANGLE:
                    case TF:
                        sprintf(buf,"%2s=%.*f,",serial_strings[i],nfplaces[i],GetProperty(&expt,st,i));
                        strcat(dbuf,buf);
                        break;
                }
                if(st->type == STIM_RDS) switch(i)
                {
                    case MODULATION_F:
                        sprintf(buf,"%2s=%.*f,xx=%.3f",serial_strings[i],nfplaces[i],GetProperty(&expt,st,i),(expt.st->vals[MODULATION_F] *M_PI *2)/(mon.framerate));
                        strcat(dbuf,buf);
                        break;
                    case DEPTH_MOD:
                    case DOT_SIZE:
                    case DISP_X:
                    case DISP_Y:
                    case DOT_DENSITY:
                    case SET_SEEDLOOP:
                        sprintf(buf,"%2s=%.*f,",serial_strings[i],nfplaces[i],GetProperty(&expt,st,i));
                        strcat(dbuf,buf);
                        break;
                }
                if(st->type == STIM_CORRUG) switch(i)
                {
                    case SF:
                    case DOT_SIZE:
                    case DOT_DENSITY:
                    case DISP_X:
                    case DEPTH_MOD:
                    case TF:
                    case FRAMERATE_CODE:
                        sprintf(buf,"%2s=%.*f,",serial_strings[i],nfplaces[i],GetProperty(&expt,st,i));
                        strcat(dbuf,buf);
                        break;
                }
        }
    }
    if(optionflags[INTERLEAVE_HIGHTF]){
        sprintf(buf,"%2s=%.*f,",serial_strings[HIGHTF],nfplaces[HIGHTF],GetProperty(&expt,st,HIGHTF));
        strcat(dbuf,buf);
    }
    if(expt.vals[ALTERNATE_STIM_MODE] > 0.5){
        sprintf(buf,",As=%.0f",expt.vals[ALTERNATE_STIM_MODE]);
        strcat(dbuf,buf);
    }
    if(st->next != NULL && st->next->type == STIM_RDS){
        sprintf(buf,"\nBack:%2s=%s,%s=%.2f,%s=%.0f,%s=%.2f",serial_strings[STIMULUS_TYPE_CODE],stimulus_names[st->next->type],
                serial_strings[CORRELATION],StimulusProperty(st->next,CORRELATION),
                serial_strings[SET_SEEDLOOP],StimulusProperty(st->next,SET_SEEDLOOP),
                serial_strings[JVELOCITY],StimulusProperty(st->next,JVELOCITY));
        strcat(dbuf,buf);
    }
    if(st->next != NULL && (st->next->type == STIM_GABOR || st->next->type == STIM_GRATING)){
        sprintf(buf,"\nBack:%2s=%s,%s=%.2f,%s=%.2f,%s=%.2f,%s=%.0f,%s=%.2f",serial_strings[STIMULUS_TYPE_CODE],stimulus_names[st->next->type],
                serial_strings[ORIENTATION],StimulusProperty(st->next,ORIENTATION),
                serial_strings[XPOS],StimulusProperty(st->next,XPOS),
                serial_strings[YPOS],StimulusProperty(st->next,YPOS),
                serial_strings[SF],StimulusProperty(st->next,SF),
                serial_strings[TF],StimulusProperty(st->next,TF));
        strcat(dbuf,buf);
    }
    if(expt.vals[ALTERNATE_STIM_MODE] > 0.5 && trialctr > 0){
        a = b = c = d = 0;
        for(i = 0; i < trialctr; i++){
            a += stimseq[i].vals[2];
            b += stimseq[i].vals[3];
            c += stimseq[i].vals[4];
            d += stimseq[i].vals[5];
        }
        a /=i; // step size
        b /=i; // period
        c /=i; // delay
        d /=i; // period in frames
        sprintf(buf,"\nStep X=%.4f (T=%.0f, dt=%.0f) every %.1f ms = %.2f deg/sec\n",a,d,c,b,(a * 1000)/b);
        strcat(dbuf,buf);
    }
    else if(expt.vals[ALTERNATE_STIM_MODE] > 0.5 ){
        a = pix2deg(expt.st->posinc * mon.framerate)/mon.framerate;
        d = 2 + rint(expt.postperiod * mon.framerate);
        c = expt.vals[DELAY];
        b = (d * 1000)/mon.framerate;
        sprintf(buf,"\nStep X=%.4f (T=%.0f, dt=%.0f) every %.1f ms = %.2f deg/sec\n",a,d,c,b,(a * 1000)/b);
        strcat(dbuf,buf);
    }
    return(dbuf);
}

int PrintPsychData(char *filename)
{
    FILE *fd = stdout;
    float val,lastval;
    int res = -1, stim,i,id, first, ntrials;
    Expstim *es = &expt.plot->stims[expt.nstim[2]];
    time_t tval;
    Thisstim *stp;
    char buf[BUFSIZ],cbuf[BUFSIZ];
    
    
    if(expt.laststim == -1) /* no trials run  */
        return(0);
    return(0); //shouldn't need this any more
    
    if(filename && *filename)
    {
        if((fd = fopen(filename,"a")) == NULL)
        {
            sprintf(buf,"!!!!!!!!!!!!  Exp %d: %s Couldn't open %s!!!!!!!!\n",expt.plotcluster,expname,filename);
            acknowledge(buf,NULL);
            puts(buf);
            fd = stdout;
        }
    }
    stim = stimorder[expt.laststim & (~ORDER_BITS)]-expt.nstim[2];
    val = expval[expt.laststim & (~ORDER_BITS)];
    if(expt.hasdata)
        fprintf(fd,"\nExperiment %d:* %s\n",expt.plotcluster+(int)(expt.tl),expname);
    else
        fprintf(fd,"\nExperiment %d: %s\n",expt.plotcluster+(int)(expt.tl),expname);
    fprintf(fd,"\nVersion %s\n",VERSION_NUMBER);
    fprintf(fd,"\nStimulus %s\n",DescribeStim(expt.st));
    cbuf[0] = 0;
    MakeString(OPTION_CODE,cbuf, &expt, expt.st, TO_FILE);
    fprintf(fd,"%s\n",cbuf);
    if(psychlog){
        fprintf(psychlog,"\nVersion %s\n",VERSION_NUMBER);
        fprintf(psychlog,"\nStimulus %s\n",DescribeStim(expt.st));
        fprintf(psychlog,"%s\n",cbuf);
    }
    
    if(expt.type2 != EXPTYPE_NONE && expt.nstim[1] > 0)
        fprintf(fd,"Trials: %s, n, left, right x%s\n",serial_names[expt.mode],serial_names[expt.type2]);
    else
        fprintf(fd,"Trials: %s, n, left, right\n",serial_names[expt.mode]);
    if(expt.type2 != EXPTYPE_NONE && expt.nstim[1] > 0)
    {
        lastval = 100000;
        for(i = expt.nstim[2]; i < expt.plot->nstim[5]; i++,es++)
        {
            stp = getexpval(i);
            if(stp->vals[1] != lastval && expt.type2 != EXPTYPE_NONE && expt.nstim[1] > 0){
                if(expt.type3 == XPOS)
                    fprintf(fd,"Condition: %2s=%.4g %2s=%.4g\n",serial_strings[expt.type2],stp->vals[1],serial_strings[XPOS], (i >= expt.nstim[3]) ? expt.vals[XPOS] *-1: expt.vals[XPOS]);
                else if(expt.type3 == ORIENTATION)
                    fprintf(fd,"Condition: %2s=%.4g %2s=%.4g\n",serial_strings[expt.type2],stp->vals[1],serial_strings[ORIENTATION], (i >= expt.nstim[3]) ? expt.stimvals[ORIENTATION] *-1: expt.stimvals[ORIENTATION]);
                else if(expt.type2 != EXPTYPE_NONE)
                    fprintf(fd,"Condition: %2s=%.4g \n",serial_strings[expt.type2],stp->vals[1]);
            }
                if(expt.type3 == XPOS){
                    id = expt.type3;
                    if(i >= expt.nstim[3])
                        fprintf(fd," %s%.2f",serial_strings[id],expt.vals[id] * -1);
                    else
                        fprintf(fd," %s%.2f",serial_strings[id],expt.vals[id]);
                }
                else if(expt.type3 == ORIENTATION){
                    id = expt.type3;
                    if(i >= expt.nstim[3])
                        fprintf(fd," %s%.2f",serial_strings[id],expt.stimvals[id] * -1);
                    else
                        fprintf(fd," %s%.2f",serial_strings[id],expt.stimvals[id]);
                }
                else if(expt.type3 != EXPTYPE_NONE){
                    id = expt.type3;
                    if(i >= expt.nstim[3])
                        fprintf(fd," %s%.2f",serial_strings[id],expt.exp3vals[1]);
                    else
                        fprintf(fd," %s%.2f",serial_strings[id],expt.exp3vals[0]);
                }
                fprintf(fd,"\n");
            }
            lastval = stp->vals[1];
    }
    else
    {
        for(i = first; i < expt.plot->nstim[5]; i++,es++){
            stp = getexpval(i);
            if(stp->vals[1] != lastval && expt.type2 != EXPTYPE_NONE && expt.nstim[1] > 0){
                fprintf(fd,"Condition: %2s=%.4g %2s=%.4g\n",serial_strings[expt.type2],stp->vals[1],serial_strings[XPOS], (i >= expt.nstim[3]) ? expt.vals[XPOS] *-1: expt.vals[XPOS]);
            }
            fprintf(fd,"\n");
            lastval = stp->vals[1];
        }
    }
    tval = time(NULL);
    fprintf(fd,"Run ended %s Took %.2f\n",nonewline(ctime(&tval)),timediff(&now,&bwtime));
    if(seroutfile)
        fprintf(seroutfile,"Run ended %s",ctime(&tval));
    if(psychlog){
        fprintf(psychlog,"Run ended %s\n",ctime(&tval));
        fflush(psychlog);
    }
    expt.hasdata++;
    if(fd != stdout)
    {
        fclose(fd); 
        printf("Exp %d: %s Saved to %s\n",expt.plotcluster,expname,filename);
        if(filename && *filename && expname && *expname)
            sprintf(buf,"Exp %d: %s Saved to %s",expt.plotcluster,expname,filename);
        else
            sprintf(buf,"Exp %d: Saved, but check names ",expt.plotcluster);
        acknowledge(buf,NULL);
    }
    return(0);
}


int isdir(char *path)
{
    struct stat s_buf;
    
    if(stat(path, &s_buf) >= 0 && S_ISDIR(s_buf.st_mode) && (s_buf.st_mode & S_IWRITE))
        return(1);
    else
        return(0);
}


int ReadPGM(char *name, PGM *pgm)
{
    FILE *imfd;
    
    char buf[BUFSIZ];
    static char lastname[BUFSIZ];
    static int astate;
    int w,h,imax,len;
    int ackw;
    
    if((imfd = fopen(name,"r")) == NULL){
    
        if(strlen(name) > 1){
    	printf("Can't Read Image %s\n",name);
            if(seroutfile)
    	fprintf(seroutfile,"Can't Read Image %s\n",name);
          if(psychlog)
    	fprintf(psychlog,"Can't Read Image %s\n",name);
          strcpy(lastname,name);
        }
          return(0);
      }
    pgm->name = myscopy(pgm->name,name);
    fgets(buf, BUFSIZ, imfd);
    sscanf(buf,"P5 %d %d %d",&w,&h,&imax);
    pgm->w = w;
    pgm->h = h;
    if(w * h > pgm->imlen){ //imlen is the length that has been allocaed
        if(pgm->ptr != NULL)
            free(pgm->ptr);
        pgm->ptr = (GLubyte *)malloc(w * h);
        pgm->imlen = w * h;
    }
    len = fread(pgm->ptr,1,w*h,imfd);
    fclose(imfd);
    return(pgm->imlen);
}

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>

#define MAXFREQS 80
#define MAXCHANS 20
#define MAXCLUSTERS 8

#define Pointer void*

typedef struct monitor{
    float pix2cms[2];
    float cms2pix[2];
    int pixels[2],loaded;
    float viewd;
    float framerate;
    double trapscale[8]; // Trapezoidal scaling for displaced projectors
    char *filename,*name;
    double gamma,truegamma; // gamma correction that screen should be set to, and actual
    double crosstalk[2];
} Monitor;


#include "stimdefs.h"
#include "jondefs.h"
typedef struct  {
    float pos[XY];
    float countdown;
    int left_right;
    float proportion;
    float offset;
    int primitive_num[2];
    float rectposition[3];
    float position[XY];
    float dot;
    long lrnd;
} ball_s;




#include "jonfront.h"

#ifndef Rectangle
typedef struct MyRectangle{
    short lx,ly,rx,ry;
} Rectangle;
#endif

#ifdef WINDOWS
#define Rectangle myRectangle
#define rnd_i random
#endif
typedef struct LOCTN{
    int size[2];   /* size of vertex array */
    vcoord xy[2],lastxy[2], /* position in pixels */
    ss[2],     /* scale factors for vertex->pixel */
    imsize[2]; /* image size in pixels*/
    float radius[2]; /* size of stencil , pixels */
    float barsep;
    float phase,contrast,angle, contrast_amp, contrast_phase,lastangle,phase2,contrast2,sf,dphase;
    float density, dotsize;/*j*/
    float pixsf;
    int *perimeter;
    float locn[2];
}Locator;


typedef struct WURTZ{
    float stimcolor,fixcolor,blankcolor,offcolor,size;
    int stimframes,fixframes,fixstims;
    int state;
    int stimvar;
    float sacval[2];
    float timeout,minprem,rt;
    float rwsize;
    vcoord origfixpos[2];
    float stairfactor; /*j usually set to ROOT2*/
    float fixcolors[5]; /* for special tasks */
}Wurtz;

typedef struct NEWGABOR{
    float *im;
    float sx,sy;
    float *xbell;
    int *carray;
    vcoord *vxarray,*vyarray; /* arrary of vertex locations */
}Gabor;

typedef struct GRATING{
    float *im;
    float *imb;
    
    int ndots,corrdots;
    float dotsiz[2];
    float disp,depth_mod,disp_grad,lum[2];
    int mode;
    vcoord *xpos, *ypos;
    int seed,baseseed,seedloop,seedctr;
    float density, xshift,correlation;
    
    float sf2,incr,phase2,ori2,phaseoff,sy,plaid_angle;
}Grating;

typedef struct BAR{
    float *im;
    float *imb;
    
    int ndots,corrdots;
    float dotsiz[2];
    float disp,depth_mod,disp_grad,lum[2];
    int mode;
    vcoord *xpos, *ypos;
    int seed,baseseed,seedloop,seedctr;
    float density, xshift,correlation;
    
    float sf2,incr,phase2,ori2,phaseoff,sy,plaid_angle;
    
    int x;
    
}Bar;

typedef struct RDS{
    int *im;
    float *imb;
    int ndots,corrdots;
    float dotsiz[2];
    float disp,depth_mod,disp_grad,lum[2];
    int mode;
    vcoord *xpos, *ypos;
    int seed,baseseed,seedloop,seedctr;
    float density, xshift,correlation;
    
    float sf2,incr,phase2,ori2,phaseoff,sy,plaid_angle;
    
    int x;
    
}Rds;



typedef struct SQCORRUG{
    int *im;
    float *imb;
    int ndots,corrdots;
    float dotsiz[2];
    float disp,depth_mod,disp_grad,lum[2];
    int mode;
    vcoord *xpos, *ypos;
    int seed,baseseed,seedloop,seedctr;
    float density, xshift,correlation;
    float sf2,incr,phase2,ori2,phaseoff,sy,plaid_angle;
    
    int x;
    
}Sqcorrug;

typedef struct ONESTIM{
    int *iim;
    float *im,*imb;
    int ndots,corrdots;
    float dotsiz[2];
    float disp,depth_mod,disp_grad,lum[2];
    int mode;
    vcoord *xpos, *ypos, *zpos;
    int seed,baseseed,seedloop,seedctr;
    float density, xshift,correlation;
    float sf2,incr,phase2,ori2,phaseoff,sy,plaid_angle;
    float sx, *xbell;
    int x;
    int *carray;
    float radius,rsigma;
    int numdots, fixdots; 				        
    float dotsize[XY], olddotsize[XY];
    float velocity;
    int direction_r;
    float front_intensity; 
    float back_intensity; 
    float deathchance; /* for each dot, range 0.0 - 1.0 */
    int lifeframes;
    ball_s trackball; /* ball used for fixation tracking */
    ball_s *balls; /* points to structures which hold random numbers */
    unsigned long **primitive; /* pointer to array of primitives holding rectvalues */
    float standing_disp;
    float pixelsy;
}OneStim;


typedef struct CURRENTSTIM{
    int stimid;
    int val;
    double expval[5];
    int seqseed,seed,lastseed;
}Currentstim;


typedef struct SUBSTIM{
    Locator pos;
    OneStim *ptr;
    short type;
    short mode;
    short lasttype;
    short newdots;
    float density, xshift,correlation;
    int ndots,corrdots,mixdots;
    float dotsiz[2];
    float disp,lum[4],pdisp,disprem;
    vcoord *xpos, *ypos, *xposa, *yposa;
    int xpl,ypl,xpla,ypla;
    int seed,baseseed,seedloop,seedctr;
    int *iim,*iimb;
    int iimlen,imlen,imblen,imclen;
    float *im,*imb,*imc;
    int calculated;
    int nfreqs;
    int stimmode;
    int show_increment;
    float wedge_angle;
    double freqs[MAXFREQS];
    double phases[MAXFREQS];
    double incrs[MAXFREQS];
    double powers[MAXFREQS];
    float gammaback;
    char id[5];
    float vscale;
    GLubyte *uimage;
    int imw,imh,uimlen;
    int autoscale;
    double size;
    double orbw;  // Orientation Bandwidth
    unsigned long bits[10]; //for recording 1-D noise pattern
    int npaint,nh,nw,nbars,npainta,imagei;
    float tf;
    float boundarypos;
    float boundaryV;

} Substim;

#define NSTIMFLAGS 1
#define MAXDISPS 40

struct STIMULUS{
    Locator pos;
    float lastangle;
    Wurtz fix;
    int mode,type,flag,lasttype;
    float background,fixcolor, gammaback;
    int colormode;
    float disp,origdisp,phasedisp[2],f,incr,angleinc,vdisp,length,width;
    float posinc, locn;
    float contrast_disp, sf_disp, ori_disp,meandisp,dispincr;
    float correlation;
    int screenw,screenh,nframes;
    int splane,swapinterval;
    int zoff[2];
    int sample_ratio[2];
    int rcseed,framerepeat;
    Rectangle rect; /* for rectcopy bits */
    Substim *left,*right,*last; /* pointers to stimulus-specific stuff */
    struct STIMULUS *next,*prev;
    int flags[NSTIMFLAGS];
    float vals[TOTALCODES];
    float depth_mod,depth_modf,modphase;
    int ndisps;
    float disps[MAXDISPS];
    int framectr;
    double tenvelope;
    int noclear,nfreqs;
    double vdisp_mod;
    double dlength,dwidth;
    double phases[MAXFREQS];
    double freqs[MAXFREQS];
    double contrasts[MAXFREQS];
    double freqmode; //0 = automatic,1 = manual
    float ucgap;
    double rolldisp;
    char *imprefix, *imname,*builddate;
    int immode,preload,preloaded,nimseed,nimplaces,forceseed;
    float stimid;
    int mixdots;
    int dotdist;
    float phaseangle;
    int nchanges;
    int dotrpt;  // random samples for dot dist repeated
    int prepared; 
    int seedoffset;
    int xstate;  //State flag for extras - blank, UC etc.
    int npix;
    float tf,stimversion;
    float pix2deg, imsd,corrmix,dotfrac;
    int calculated, painted;
    float xyshift[2];
    int jumps;
    int envelopetype;
    float envelope;
    int modifier;
    int nphases;
    float plaid_angle;
    int componentjumps[MAXFREQS]; //Apply phase jump or not
    int phasesign;
    int aamode;
};

typedef struct STIMULUS Stimulus;

struct BWSTRUCT{
    int cflag,nbins,displaycompress;
    float sv,fw,tl; /*static vergence, fix window, trigger level, vergence window */
    int replaycompress,nchans;
    float fsd[MAXCHANS];
    int colors[MAXCHANS];
    char *prefix;
};

typedef struct BWSTRUCT Bwio;

struct EXPSTIM{
    float angle;
    vcoord pos[2];
    int size[2],flag,nreps[MAXCLUSTERS][2],nbins,phasemark;
    int nsaved[MAXCLUSTERS];
    int psychdata[2];
    float x[2],y;
    float spcnt[MAXCLUSTERS][2],sumsq[MAXCLUSTERS][2];
    int binvals[MAXBINS];
    int resps[5],nsame;
    int type;
};

typedef struct EXPSTIM Expstim;

#define TITLELEN 256

struct plotdata{
    int nstim[10],binwidth,fplaces,nstim2,flag,extras,ndata,nlabels;
    int size[2],pos[2],w,offset;
    Expstim *stims;
    short *linedata;
    float maxrate,xrange[2],range;
    char **label;
    int disk_data;
    int trialctr, nw;
    int *trialcnts;
    char title[TITLELEN];
};


struct INCHAN{
    float offset, gain, phase;
    int softoff;
};

struct DAILYLOG{
    struct INCHAN chan[4];
    float weight;
    int fluid;
    char *restrain,*free,*comment;
};

typedef struct DAILYLOG Log;

struct THISSTIM{
    float vals[10];
    int stimno[3],expno;
    int result,a,b;
    int stairid;
    int interleave;
};

typedef struct THISSTIM Thisstim;

struct PGIMAGE{
    GLubyte *ptr;
    int w,h,imlen;
    char *name;
    
};

typedef struct PGIMAGE PGM;

struct EXPERIMENT{
    float mean, incr, isi, mean2, incr2,mean3,incr3;
    float nreps; // can be fraction for Fastseq trials
    int nframes, nstim[10],mode, nrepsi,flag,isiframes,stimno,nstim2,stimid;
    int laststim,ncycles,nlines, trials_per_stim, extras, stimtype, backtype;
    int nextstim,plotcluster,type2,type3,blksize,oldstimflag,stimpertrial;
    int laststimno;
    float overlay_color,binw;
    float spikegain,electrode_depth;
    float postperiod,preperiod,clamp,ramp,cramp;
    float sv,fw,tl,cclamp,clamp_rrate,cj,vw,nextval, jnextval;
    float stimvals[TOTALCODES],currentval[6];
    char *strings[TOTALCODES];
    Stimulus *st;
    Monitor *mon;
    Thisstim *stp;
    Bwio *bwptr;
    struct plotdata *plot;
    char *plotname,*logfile,*upload,*pnum,*penfile,*helpfile;
    Expstim *rf;
    int expno;
    float vals[TOTALCODES];
    float linw;
    int hasdata;
    int *rcframes;
    float *rcvals;
    int *rccounts,*rcn,ipen,nsaved,newpen;
    float exp3vals[MAXSTIM];
    int plot3type;
    float customvals[MAXSTIM];
    float custombvals[MAXSTIM];
    int expstimid[4];
    PGM backim; // pathname for a background image
    int winsiz[2],framesdone,allstimid,nvals;
    int polygonsmooth;  //Available or Not
    int fasttype,fastbtype,fastctype; // what is varied in FAST_SEQUENCE
    int stimmode,ncalc,noverflow; 
    int fastextras,driveadapter;
    float fixpos[2];
    double softoff[6];
    int hemisphere,electrode;
    double minval, fastincr;
    int nfast;
    int fastnstim[6];
    float targetcolor;
    char *backprefix;
    char *cmdinfile;
    int biasedreward;
    int cmdtype;
    int stflag;
    int hightype;
    char *showflags;
    char *monkey;
    int verbose;
    int laststimcode,lastserialcode,totalcodes,lastsavecode;
    int maxcode; //largest value of any code = length of serial_strings
    char cwd[BUFSIZ*2];
    int expseed;
    char *username;
    int codesent;
    char *loadfile;
};

typedef struct EXPERIMENT Expt;



//#include "stimpro.h"

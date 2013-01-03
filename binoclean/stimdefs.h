#define NOTSET -10000
#define SCREENW 400
#define SCREENH 512
#define MAXFRAMES 700

#define INTERLEAVE_EXPT -1000
#define INTERLEAVE_EXPT_LEFT -1001
#define INTERLEAVE_EXPT_RIGHT -1002
#define INTERLEAVE_EXPT_SFONE -1003
#define INTERLEAVE_EXPT_SFTWO -1004
#define INTERLEAVE_EXPT_UNCORR -1005
#define INTERLEAVE_EXPT_HIGH -1006
#define INTERLEAVE_EXPT_ZERO -1007
#define INTERLEAVE_EXPT_COMPONENT_ONE -1008
#define INTERLEAVE_EXPT_BLANK -1009
#define INTERLEAVE_SIGNAL_FRAME -1010
#define INTERLEAVE_EXPT_FLIP -1011
#define DEG2INT (2048.0/10.0)
/*
 * Stimulus Types. But should be defined in commdefs.h, hence removed, bgc 6/14/2004
 #define STIM_GABOR 1
 #define STIM_RDS 2
 #define STIM_GRATING 3
 #define STIM_BAR 4
 #define STIM_CIRCLE 5
 #define STIM_RECT 6
 #define STIM_TEST 7
 #define STIM_SQUARE 8
 #define STIM_PROBE 9
 #define STIM_GRATING2 10
 #define STIM_CYLINDER 11
 
 #define STIM_NONE 0
 */

#define MAXMEM 25000000  /* 25 Meg or so for image allocation */
#define MAXPLOTS 40

/* 
 * bits set in mode (front.c)
 */

#define NULLMODE 0
#define ANIMATE_BIT 1
#define FIRST_FRAME_BIT (1<<1)
#define LAST_FRAME_BIT (1<<2)
#define RESET_FRAME_CTR  (1<<3)
#define NEED_REPAINT (1<<4)
#define NEED_REPAINT_AGAIN (1<<5)
#define NEWPOS (1<<4)
#define NEWPOSBACK (1<<5)
#define TEST_PENDING (1<<6)
#define FIXATION_OFF_BIT (1<<7)
#define SHOWTIMES (1<<8)
#define MODE_PENDING (1<<9)
#define BW_ERROR (1<<10)
#define NEWDRAG (1<<11)
#define SERIAL_OK (1<<12)
#define CURSOR_IS_ON (1<<13)
#define NO_SERIAL_PORT (1<<14)
#define OVERLAY_ON (1<<15)
#define SAVED_LOG (1<<16)
#define MORE_PENDING (1<<17)
#define WURTZ_FRAME_BIT (1<<18)
#define AUTO_NEXT_EXPT (1<<19)
#define PLACE_PLOT_BIT (1<<20)
#define RUNNING (1<<21)
#define EVENSTIMULUS (1<<22)
#define WINDOWS_RAISED (1<<23)
#define STIMCHANGE_FRAME (1<<24)
#define UFF_FILE_OPEN (1<<25)
#define CUSTOM_EXPVAL (1<<26)
#define NEWDRAG2 (1<<27)
#define STIM_FRAME_BIT (1<<28)
#define WAIT_NEXT_EXPT (1<<29)
#define SAVE_UNITLOG (1<<30)
#define HOLD_STATUS (1<<31)
#define rad2deg(x) ((x) * 57.29577951308)
#define deg2rad(x) ((x) * 0.017453292)
#define min2rad(x) ((x) * 0.000290888208665)

#define FRAME_BITS (FIRST_FRAME_BIT | LAST_FRAME_BIT | WURTZ_FRAME_BIT | STIM_FRAME_BIT | STIMCHANGE_FRAME)

/* bits set in stmode*/
#define MOVED_STIMULUS 1
#define DRAG_STIMULUS (1<<1)

/*
 * bits set in lineflag (expop.c)
 */

#define BACKSTIM_BIT (1<<25)

/*
 * bits set in TheStim->mode
 */

#define DOUBLEBUFFERMODE (1)
#define STIMULUS_NEEDS_CLEAR (1<<1)
#define RECTWRITEMODE (1<<2)
#define VERBOSE (1<<3)
#define PRECALCMODE (1<<16)
#define SEPARATEMODE (1<<17)
#define EXPTPENDING (1<<18)
#define STIMWIPE (1<<19)
#define TRIAL_PENDING (1<<23)
#define NEW_FIXATION_BIT (1<<26)
#define BACKCLEARED (1<<27)
#define EXPT_OVER (1<<29)
#define POST_STIMULUS_BIT (1<<30)

#define INTRIAL (POST_STIMULUS_BIT | TRIAL_PENDING)

#define MODEBITS (DOUBLEBUFFERMODE | ZBUFFERMODE | RECTWRITEMODE \
| PRECALCMODE)

#define PENDING_BITS (EXPTPENDING | TRIAL_PENDING)
#define ONLINEBITS (PENDING_BITS | NEW_FIXATION_BIT | BACKCLEARED)



/* bits set in expt.flag */
#define LOGINCR 1
#define ADD_EXPT2 (1<<1)
#define TIMES_EXPT2 (1<<2)
#define ALTERNATE_EXPTS (1<<3)
#define LOGINCR2 (1<<4)

#define EXPT2_BITS (ADD_EXPT2 | TIMES_EXPT2 | ALTERNATE_EXPTS)

#define GAUSSTART 50
#define GAUSSEXTRA (GAUSSTART *2)

/* bits set in the stimulus order numbers */

#define STIMULUS_EXTRA_RIGHT (1<<31)
#define STIMULUS_EXTRA_LEFT (1<<30)
#define STIMULUS_EXTRA_BLANK (1<<29)
#define STIMULUS_EXTRA_EXPT (1<<28)
#define STIMULUS_EXTRA_UNCORRELATE (1<<27)
#define STIMULUS_EXTRA_ZEROCOH (1<<26)
#define STIMULUS_EXTRA_ZERO (1<<25)
#define STIMULUS_EXTRA_HIGH (1<<24)
#define STIMULUS_EXTRA_HIGHSF (1<<23)
#define STIMULUS_EXTRA_HIGHSQ (1<<22)
#define STIMULUS_EXTRA_HIGHTF (1<<21)
#define STIMULUS_EXTRA_RANDOM (1<<20)
#define STIMULUS_EXTRA_EXPT3 (1<<19)
#define STIMULUS_EXTRA_FPJUMP (1<<18)
#define STIMULUS_EXTRA_FLIP (1<<17)

#define ORDER_BITS (STIMULUS_EXTRA_RIGHT | STIMULUS_EXTRA_LEFT | STIMULUS_EXTRA_EXPT | STIMULUS_EXTRA_BLANK | STIMULUS_EXTRA_UNCORRELATE | STIMULUS_EXTRA_ZEROCOH | STIMULUS_EXTRA_ZERO | STIMULUS_EXTRA_HIGH | STIMULUS_EXTRA_HIGHSF | STIMULUS_EXTRA_HIGHSQ | STIMULUS_EXTRA_HIGHTF | STIMULUS_EXTRA_RANDOM | STIMULUS_EXTRA_EXPT3 | STIMULUS_EXTRA_FPJUMP| STIMULUS_EXTRA_FLIP)

/*#define LUMSCALE 31.0
 #define MEANLUM 31
 */
#define MEANLUM 127
#define LUMSCALE 127.0
#define LEFTMODE 1
#define RIGHTMODE 2
#define BOTHMODE 3
#define ALLPLANES 4
#define OVERLAY 5
#define ALLMODE 6
#define FORCEBOTH 7
#define FORCELEFT 8
#define FORCERIGHT 9
#define WHITEMODE (1<<3)
#define BLACKMODE (1<<4)
#define LEFTDOT (1<<5)
#define RIGHTDOT (1<<6)
#define ACORRMODE (1<<7)

#define COLOR8BIT 1
#define COLOR6BIT 2
#define COLOR12BIT 3
#define COLORRGB 4

#define LEFT 0
#define RIGHT 2
#define LEFTMASK 0xff
#define RIGHTMASK 0xff0000
#define CYCLOMASK 0xff00
#define BOTHMASK (LEFTMASK | RIGHTMASK)
#define ALLMASK (0xffffff)

#define ONLY_ONE_EXPT 0
#define EXP_FIRST 1
#define EXP_SECOND 2
#define EXP_THIRD 3


float deg2pix(float val);
float pix2deg(float val);
float deg2pixy(float val);
float pix2degy(float val);



#define FIX_IS_OFF 0
#define FIX_IS_UP 1
#define FIX_IS_DIM 2

/* return values */
#define NEW_EXPT -4
#define LASTSTIM -3
#define STIM_ERROR -5
#define BAD_TRIAL -2
#define NOT_READY -6
#define SAC_BAD_TRIAL -9

#define ALLSTIMULI 0
#define ONESTIMULUS 1
#define DIVIDEINCR 2
#define INCREASEINCR 4


/*
 * expt.stimmode values - various special painting options
 */
// for random dot distributions
#define DOTDIST_BALENCED 0
#define DOTDIST_FIXED 1
#define DOTDIST_RANDOM 2
// for nsines
#define UNNORMALIZED 3
#define RANDOM_POWER 4
#define RANDOM_ONOFF 6
#define RANDOM_ONOFF_LOG 7
#define INDEP_ONOFF 8
#define INDEP_ONOFF_LOG 9
#define BINOC_INDEP_ONOFF_LOG 10
#define THREE_CONTRAST_LEVEL 11
#define BINOC_INDEP_ONOFF 12
//for RLS
#define WHITENOISE16 5   //16 levels of grey
//for grating subspace expts
#define FOUR_PHASES 13
#define TWO_PHASES 14
#define REMOVE_BACKSURFACE 15
#define FLAT_CYLINDER 16
#define NORMAL_CYLINDER 17
#define TWOCYL_BACKZERO 18
#define PARALLEL_AC 19
#define ORTHOG_AC 20
#define PARALLEL_UC 21
#define ORTHOG_UC 22
#define ORTHOG_BLANK 23
#define TWOCYL_ANDIDISP 24
#define RDSRLSSETSL 25
#define BUTTSEXPT 26

#define issfrc(x) (x == RANDOM_ONOFF || x == RANDOM_ONOFF_LOG || x == INDEP_ONOFF || x == INDEP_ONOFF_LOG || x == BINOC_INDEP_ONOFF_LOG || x== THREE_CONTRAST_LEVEL || x == BINOC_INDEP_ONOFF)

/* flags used for MakeString */
#define TO_BW 0
#define TO_FILE 1
#define TO_GUI 2

/*
 *  these Constants depend upon the strings defined in
 * char * toggle_strings, in front.c
 */

#define NULL_BIT 0
#define GORAUD_BIT 1
#define DITHERMODE (1L<<5)
#define FRAME_ONLY_BIT (1L<<2)
#define GO_BIT (1L<<3)
#define ANTIALIAS_BIT (1<<4)
#define SQUARE_RDS (1L<<1)
#define HZ72_BIT (1L<<6)
#define CONTRAST_REVERSE_BIT (1L<<7)
#define SE_BIT (1L<<8)
#define BACKGROUND_FIXED_BIT (1L<<9)
#define TEST_BIT (1L<<10)
#define DRAW_FIX_BIT (1L<<11)
#define SQUARE_FIXATION (1L<<12)
#define AUTO_PLOT (1L<<13)

#define FIRST_WURTZ_BIT 14
#define WURTZFLAG (1L<<FIRST_WURTZ_BIT)
#define MONKEY_RUNS_STIMULUS (1L<<(FIRST_WURTZ_BIT+1))
#define BACKGROUND_IN_PREPERIOD (1L<<(FIRST_WURTZ_BIT+1))
#define FIXATION_SHIFT (FIRST_WURTZ_BIT+2)
#define LEFT_FIXATION_CHECK (1L<<(FIXATION_SHIFT))
#define RIGHT_FIXATION_CHECK (1L<<(FIRST_WURTZ_BIT+3))
#define FIXATION_CHECK (1L<<(FIRST_WURTZ_BIT+4))
#define VERGENCE_CHECK (1L<<(FIRST_WURTZ_BIT+5))
#define WURTZ_WHOLE_STIMULI (1L<<(FIRST_WURTZ_BIT+6))
#define STORE_WURTZ_BIT (1L<<(FIRST_WURTZ_BIT+7))
#define PLOT_WURTZ_BIT (1<<(FIRST_WURTZ_BIT+8))
#define SHOW_STIMVAL_BIT PLOT_WURTZ_BIT
#define WURTZ_TRACK_BIT (1<<(FIRST_WURTZ_BIT+9))
#define STIM_IN_WURTZ_BIT (1<<(FIRST_WURTZ_BIT+10))

#define FIRST_EXPT_BIT FIRST_WURTZ_BIT+11
#define WAIT_FOR_BW_BIT (1L<<FIRST_EXPT_BIT)
#define CLAMP_EXPT_BIT (1L<<(FIRST_EXPT_BIT+1))
#define CLAMP_HOLD_BIT (1L<<(FIRST_EXPT_BIT+2))
#define SEARCH_MODE_BIT (1L<<(FIRST_EXPT_BIT+3))
#define RAMP_EXPT_BIT (1L<<(FIRST_EXPT_BIT+4))
#define RAMP_HOLD_BIT (1L<<(FIRST_EXPT_BIT+5))
#define SHOW_CONJUG_BIT (1L<<(FIRST_EXPT_BIT+6))

#define MONOCULAR_MODE (LEFT_FIXATION_CHECK | RIGHT_FIXATION_CHECK)
/* bits set in option2flag */
#define FLASH_BIT (1L)
#define EXPT_INTERACTIVE (1L<<1)
#define INTERLEAVE_MONOCULAR (1L <<2)
#define INTERLEAVE_VERGENCE (1L <<3)
#define INTERLEAVE_ANTICORRELATE (1L <<4)
#define INTERLEAVE_BLANK (1L<<5)
#define INTERLEAVE_RANDOM (1L<<6)
#define INTERLEAVE_ONE_MONOCULAR (1L<<7)
#define INTERLEAVE_UNCORRELATED (1L<<8)
#define WIPE_WHOLE_SCREEN (1L<<9)

#define PSYCHOPHYSICS_BIT (1L<<10)
#define ALT_PROBE_BIT (1L<<11)

#define FIRST_AFC_BIT ((FIRST_EXPT_BIT+19))
#define FIRST_AFC_NUM (FIRST_AFC_BIT-32)
#define RANDOM (1L<<FIRST_AFC_NUM)
#define STAIRCASE (1L<<(FIRST_AFC_NUM+1))
#define AFC (1L<<(FIRST_AFC_NUM+2))
#define PERF_STRING (1L<<(FIRST_AFC_NUM+3))
#define IFC (1L<<(FIRST_AFC_NUM+4))
#define LAST_AFC_BIT (FIRST_AFC_BIT+5)

#define MAXOPTIONBITS 49
#define FEEDBACK 0
#define FLIP_FEEDBACK 1
// keep 2 free for SQUARE_RDS
#define FIXED_BARSEP 3
#define RANDOM_EXPT2 4
#define REVERSE_EXPT2 5
#define BINOCULAR_FIXPOINT 6
#define REVERSE_CORRELATE 7
#define INTERLEAVE_ZERO 8
#define HIDE_STATUS 9
#define NO_MIRRORS 10
#define MOVE_RF 11
#define GREY_MONOC 12
#define CONTOUR_PLOT 13
#define SMOOTH_CONTOUR 14
#define PLOT_POLAR SMOOTH_CONTOUR
#define PLOT_SEQUENCE 15
#define TIMES_EXPT 16
#define SIMULATE_FP_MOVE 17
#define INTERLEAVE_SINE 18
#define STEP_CLEAR_SPIKES 19
#define INTERLEAVE_HIGHTF 20
#define INTERLEAVE_HIGHSF 21
#define COUNTERPHASE 22
#define INTERLEAVE_HIGHSQ 23
#define INTERLEAVE_HIGH 24
#define INTERLEAVE_ZERO_ALL 25
#define INTERLEAVE_MONOC_ALL 26
#define INTERLEAVE_COMPONENT 27
#define INTERLEAVE_ONE_SINE 27
#define INTERLEAVE_UNCORR_ALL 28
#define RANDOM_PHASE 29
#define PLOT_ELECTRODE_TRACK 30
#define RAND_FP_DIR 31
#define SPLITSCREEN 32
#define COUNT_BADFIX_FOR_BONUS 33
#define RUN_SEQUENCE 34
#define MICROSTIM 35
#define TILE_XY 36
#define STORE_EXPT_ONLY 37
#define FIXPT_GRATING 38
#define ADD_FPJUMP 39
#define RANDOM_RELPHASE 40
#define ALWAYS_CHANGE_STIM 41
#define TEMPORAL_GAUSS 42
#define FRAMESWARN 43
#define RANDOM_DEPTH_PHASE 44
#define INTERLEAVE_HIGH_ALL 45
#define BACKGROUND_ALWAYS 46
#define STORE_LFP 47
#define NONIUS_LINES 48
#define INTERLEAVE_FLIP 49
#define ONLINE_DATA 50
#define ONLINE_AUTO_COPY 51
#define PLOTFLIP 52
#define FAST_SEQUENCE 53
#define FOUR_CHOICES 54
#define PAINT_THIRD_LAST 55
#define USTIM_ZERO_ONLY 56
#define RANDOM_CONTRAST 57
#define RANDOM_CORRELATION 58
#define SHOW_REWARD_BIAS 59
#define COLLAPSE_EXPT3 60
#define ODD_MAN_OUT_TASK 61
#define CHOICE_BY_ICON 62
#define JUMP_IMAGE 63
#define PAINT_BACKGROUND 64

#define LAST_CODED_OPTION 64


#define AUTO_COPY_ONLINE_FILES (LAST_CODED_OPTION+1)
#define CUSTOM_EXPVALB (LAST_CODED_OPTION+2)
#define STIMULUS_IN_OVERLAY (LAST_CODED_OPTION+3)
#define REDUCE_SERIAL_OUTPUT (LAST_CODED_OPTION+4)
#define STAIR_CENTER (LAST_CODED_OPTION+5)
#define FIXNUM_PAINTED_FRAMES (LAST_CODED_OPTION+6)
#define MODULATE_DISPARITY (LAST_CODED_OPTION+7)
#define STEREO_GLASSES (LAST_CODED_OPTION+8)
#define VVNONIUS_LINES (LAST_CODED_OPTION+9)
#define CALCULATE_ONCE_ONLY (LAST_CODED_OPTION+10)
#define DEBUG_OUTPUT (LAST_CODED_OPTION+11)
#define WATCH_TIMES (LAST_CODED_OPTION+12)
#define INITIAL_TRAINING (LAST_CODED_OPTION+13)
#define CHECK_FRAMECOUNTS (LAST_CODED_OPTION+14)
#define SHOWSTIMBOXES (LAST_CODED_OPTION+15)
#define CUSTOM_EXPVALC (LAST_CODED_OPTION+16)
#define EXPTMEAN_FOR_PSYCH (LAST_CODED_OPTION+17)
#define MAXALLFLAGS (LAST_CODED_OPTION+18)

#define MAXTOGGLES (MAXALLFLAGS + MAXOPTIONBITS)

#define ADAPTER_TOGGLE (MAXALLFLAGS+1)
#define RHEMISPHERE_TOGGLE (MAXALLFLAGS+2)
#define LAMINARTRODE_TOGGLE (MAXALLFLAGS+3)

#define FAKE_SPIKES 0
#define TEST_RC 1
#define PLAYING_EXPT 2
#define SAVE_IMAGES 3
#define REPEAT_DOTS 4
#define ANAGLYPH 5
#define NTESTFLAGS 6

#define SAVE_RDS_BINOC 3  // Save x,y,color for each eye
#define SAVE_LAST_STIM 4
#define SAVE_RDS_DISPS 5 // Save x,y,color,disparity, left eye

/* bits that do not have toggles */
#define TOGGLE_EYE_BIT (1L <<29)
#define PRETRIAL_BRIGHT (1L<<30)

#define MAXBINS 500
#define MAXUSERLINES 50
#define RF_COLOR 1.0
#define LINES_COLOR 1.0
#define BOX_COLOR 1.0
#define PLOT_COLOR 1.0

/* bits set in Expstim->flag */
#define BOX_ON 1
#define PHASEMARKER_ON (1<<1)
#define CENTERMARK_ON (1<<2)
#define PSTH_ON (1<<3)
#define PSTH2_ON (1<<4)

#define STIM_BOX 0
#define RF_BOX 1


#define rdspair(p) ((p->type == STIM_RDS && p->next != NULL && p->next->type == STIM_RDS))
#define rlspair(p) ((p->type == STIM_RLS && p->next != NULL && p->next->type == STIM_RLS))
#define corrugpair(p) ((p->type == STIM_CORRUG && p->next != NULL && p->next->type == STIM_CORRUG))
#define sqcorrugpair(p) ((p->type == STIM_SQCORRUG && p->next != NULL && p->next->type == STIM_SQCORRUG))
#define isdotstim(p) ((p->type == STIM_RDS || p->type == STIM_CYLINDER || p->type == STIM_CORRUG))

/*
 * set of definitions to allow whole program to operate in
 * ints, floasts, or shorts, by chanign these defienitions
 *
 * currently using floats.
 */

#define myvx(x) glVertex2f(x[0],x[1])
#define mycmv(x) glRasterPos2f(x[0],x[1])
#define vcoord float
#define myrect glRectf
#define mycolor(x) glColor3f((float)(x[0]),(float)(x[1]),(float)(x[2]))
//#define mycolor(x) glColor4f((float)(x[0]),(float)(x[1]),(float)(x[2]),(float)(x[3]))
#define SetGrey(x) glColor3f(x,x,x)

#define testout fprintf

#define NOEVENT 1
#define MANUALEVENT 2

#define GO 1
#define STOP 2

#define QUIT_BUTTON 1
#define RUN_BUTTON 2
#define ANIMATE_BUTTON 3
#define CLEAR_LOG_BUTTON 4

/* 
 * these codes must not overlap with all those defined in commdefs.h,
 * but they are unique to binoc
 */

#define EVERYTHING_SLIDER TOTALCODES
#define OTHERBITS_SLIDER (TOTALCODES+1)
#define QUIT_NOCONFIRM (TOTALCODES+2)
#define WHICHSTIM (TOTALCODES+3)
#define STIMTYPE (TOTALCODES+4)
#define NOCONFIRM (TOTALCODES+5)
#define ANALYZE_PSF (TOTALCODES+6)
#define CLEAR_SOFTOFF (TOTALCODES+7)
#define PAUSE_EXPT (TOTALCODES+8)
#define RESUME_EXPT (TOTALCODES+9)
#define NULL_SOFTOFF (TOTALCODES +10)
#define CENTRE_STIM_ON_RF (TOTALCODES +11)
#define EXP3PLOT (TOTALCODES + 12)
#define SAVE_RF (TOTALCODES + 13)
#define COPY_ONLINE (TOTALCODES + 14)
#define REPLAY_EXPT (TOTALCODES + 15)
#define QUIT_END_PEN (TOTALCODES + 16)
#define UNDO_NULL_SOFTOFF (TOTALCODES +17)

/* stimulus and fixation states */

#define STIMSTOPPED 0
#define INTERTRIAL 1
#define PREFIXATION 2
#define PRESTIMULUS 3
#define INSTIMULUS 4
#define POSTSTIMULUS 5
#define IN_TIMEOUT 7
#define POSTPOSTSTIMULUS 9
#define BADFIX_STATE 11
#define POSTTRIAL 12
#define POSTSTIMINTRIAL 13
#define IN_TIMEOUT_W 14
#define SHAKE_TIMEOUT 15
#define SHAKE_TIMEOUT_PART2 16
#define SHAKE_TIMEOUT_PART1 17

#define IFNEEDED 0
#define FORCE 1


#define isastim(x) (x != NULL && x->type != STIM_NONE)
#define isadotstim(x) (x != NULL && x->type == STIM_RDS || x->type == STIM_RLS || x->type == STIM_CORRUG || x->type == STIM_SQCORRUG)
#define isanumber(c) (isdigit(c) || c == '-' || c =='+' || c == '*' || c == '.')

// Need something else for 64 bit version.  drand48? 
#define mydrand rnd_01d

#if 1
#define myrnd_init rnd_init
#define myrnd_i rnd_i
long rnd_i(void);
#else
#define myrnd_init srandom
#define myrnd_i random

#endif


//#define rnd_i arc4random


#define sinf(x) sin(x)
#define cosf(x) cos(x)
#define powf(x,y) pow(x,y)

#define CODEHIST 30
#define TRIALBUFFERLEN 10000


#define MAXSTIM 1200



#define ONE_TRIAL 0
#define EXPT_PAUSED 1
#define EXPT_PAUSED_BY_QEXP 2
#define NSTATEFLAGS 3


#define SAVE_STATE 1
#define QUICK_SAVE 2


/*
 * Ways to handle the multiple data sets if Expt 3 is on.
 */

#define PLOT3_ALL 0
#define PLOT3_FIRST 1
#define PLOT3_SECOND 2



#define RADIAL_GRATING 1
#define CIRCULAR_GRATING 2
#define SPIRAL_GRATING 3



/*
 *  ALTERNATE_STIM_MODE a flag for doing a variety 
 *
 * 1-7 Stroboscopic variations
 * 
 */

#define SEED_CHANGE_WITH_CONTRAST 9
#define ADD_UNCORR_STRIP 10
#define UNCORR_VSCALE 11
#define INDUCED_EFFECT 12
#define INDUCED_MASKED 13
#define INDUCED_MIMIC 14
#define HARRIS_TASK 15
#define HARRIS_TASK_ANGLE 16
#define ASYMMETRICAL_ENVELOPE 17
#define CROSS_TALK 18
#define QUICK_CAL 19
#define ZERO_BACK_DELAY 20
#define MOVE_STIM_ONLY 21

#define isinduced(x) (x > 11.9 && x < 14.1)
#define isharris(x) (x > 14.9 && x < 16.1)


#define EXP_PSYCHVAL 2
#define SIGNAL_STRENGTH 4

//modes for image types = sets rules for building pathname
#define IMAGEMODE_ORBW 1
#define BINOCULAR_PLAIN_IMAGES 2

#define MAXORBW 126


#define BRAINWAVE 0
#define SPIKE2 1



#define ButtonRelease 1
#define ButtonPress 2
#define MotionNotify 3

#define Button1 1
#define Button2 2
#define Button3 3
#define Button4 4
#define Button5 5

#define ShiftMask 1
#define ControlMask 2
#define AltMask 4
#define InExptChar  ((expt.st->mode & EXPTPENDING) ? '@' : ' ')

#ifndef WindowEventdefined 

typedef struct windowevent{
    int eventType;
    int mouseX; 
    int mouseY; 
    int mouseButton;
    int modifierKey;
} WindowEvent;

#define WindowEventdefined 1

#endif



#include "stimdefs.h"


#ifndef COMMDEFS_INCLUDED
#define COMMDEFS_INCLUDED 1

/* speical characters */
#define START_EXPT (char)1
#define END_EXPT (char)2
#define END_STIM (char)3
#define BW_IS_READY (char)4
#define FRAME_SIGNAL (char)5
#define START_STIM (char)6
#define WURTZ_LATE (char)7
#define WURTZ_OK (char)8
#define WURTZ_PREM (char)9
/* nb 10 is '\n',*/
#define BAD_FIXATION (char)11
#define START_TRIAL (char)12
/* nb 13 is 'cr',*/
#define END_PREM (char)14
#define WURTZ_STOPPED (char)15
#define END_TRIAL (char)16
/* these two codes are the same because they are both one-way */
#define CONJUG_OUT (char)18
#define FREE_REWARD (char)18
#define CANCEL_EXPT 19
#define SPIKES_OUT 14
#define WURTZ_OK_W 20
#define WURTZ_LATE_SAC 21
#define BAD_VERGENCE 22
#define BAD_TRIAL_STATE 23
#define GOOD_FIXATION 24
#define NSPECIAL_CODES 25

/*
 * N.B. These codes only go SG->BW, and duplicate codes
 *which go only BW - > SG
 */
#define STIM_CHANGE 22
#define BACK_JUMP 22
#define BACK_BACK 11

#define INTERSTIMULUS (char)17
#define MYEOF (char)17

#define FRAMERATE 60.0
#define FIXEDDELAY 0.000001
#define SERIALDELAY 92L /* in BW ticks */

/* codes for the meaning of each of the serial strings */
#define SETZXOFF 0
#define SETZYOFF 1
#define XPOS SETZXOFF
#define YPOS SETZYOFF
#define STIMULUS_TYPE_CODE 2
#define SD_X 3
#define SD_Y 4
#define DISP_P 5
#define DISP_X 6
#define STIM_WIDTH 7
#define STIM_HEIGHT 8
#define SF  9
#define XSAMPLES 10
#define YSAMPLES 11
#define TF 12
#define SETPHASE 13
#define ORIENTATION 14
#define SET_SEEDLOOP 15
#define SET_SEED 16
#define SETCONTRAST 17
#define DISP_Y 18
#define SF2 19
#define TF2 20
#define PHASE2 21
#define PLAID_ANGLE 22
#define JVELOCITY 23
#define DOT_VELOCITY 23
#define JF_INTENSITY 24
#define JB_INTENSITY 25
#define JNUMDOTS 26
#define JLIFEFRAMES 27
#define JDEATH 28
#define CONTRAST2 29
#define CONTRAST_DIFF 30
#define ORIENTATION_DIFF 31
#define SF_DIFF 32
#define DEPTH_MOD 34
#define DISP_GRAD 33
#define TWOBAR_ANGSEP 35
#define DISP_RAMP 36
#define PHASE2DISP 37
#define DISP_P2 37
#define PHASEREL 38
#define PHASE_AS_DISP 39
#define CORRELATION 40
#define DOT_SIZE 41
#define BVELOCITY 42
#define BPOSITION 43
#define LAST_STIMULUS_CODE 43


#define SETFIXCOLOR (LAST_STIMULUS_CODE+1)
#define SETZOOM (LAST_STIMULUS_CODE+2)
#define SETBACKCOLOR (LAST_STIMULUS_CODE+3)
#define DELAY (LAST_STIMULUS_CODE+4) 
#define PREPERIOD_CODE (LAST_STIMULUS_CODE+5)
#define EXPTYPE_CODE (LAST_STIMULUS_CODE+6)
#define FRAMERATE_CODE (LAST_STIMULUS_CODE+7)
#define STIMULUS_DURATION_CODE (LAST_STIMULUS_CODE+8)
#define CLAMP_DISPARITY_CODE (LAST_STIMULUS_CODE+9)
#define POSTPERIOD_CODE (LAST_STIMULUS_CODE+10)
#define MODE_CODE (LAST_STIMULUS_CODE+11)
#define RAMP_VERGENCE_CODE (LAST_STIMULUS_CODE+12)
#define NTRIALS_CODE (LAST_STIMULUS_CODE+13)
#define UFF_PREFIX (LAST_STIMULUS_CODE+14)
#define HIGHX (LAST_STIMULUS_CODE+15)
#define STIMULUS_MODE (LAST_STIMULUS_CODE+16)
#define WURTZ_RT_CODE (LAST_STIMULUS_CODE+17)
#define INITIAL_APPLY_MAX (LAST_STIMULUS_CODE+18)
#define OPTION_CODE (LAST_STIMULUS_CODE+19)
#define NREPS_CODE (LAST_STIMULUS_CODE+20)
#define TRIGGER_LEVEL (LAST_STIMULUS_CODE+21)
#define FIXATION_WINDOW (LAST_STIMULUS_CODE+22)
#define DISPLAYFLAG_CODE (LAST_STIMULUS_CODE+23)
#define CHANNEL_CODE (LAST_STIMULUS_CODE+24)
#define STATIC_VERGENCE (LAST_STIMULUS_CODE+25)
#define WURTZ_DURATION_CODE (LAST_STIMULUS_CODE+26)
#define UFF_TIME (LAST_STIMULUS_CODE+27)
#define BACK_CONTRAST (LAST_STIMULUS_CODE+28)
#define EXPT_INCR (LAST_STIMULUS_CODE+29)
#define EXPT_MEAN (LAST_STIMULUS_CODE+30)
#define ISI_CODE (LAST_STIMULUS_CODE+31)
#define UNIT_BINWIDTH (LAST_STIMULUS_CODE+32)
#define BINNED_SPIKE_DATA (LAST_STIMULUS_CODE+33)
#define SPIKE_TIMES (LAST_STIMULUS_CODE+34)
#define NFRAMES_CODE (LAST_STIMULUS_CODE+35)
#define DISP_BACK (LAST_STIMULUS_CODE+36)
#define RAMP_COEFFICIENT (LAST_STIMULUS_CODE+37)
#define FIXPOS_X (LAST_STIMULUS_CODE+38)
#define FIXPOS_Y (LAST_STIMULUS_CODE+39)
#define SETFIXSIZE (LAST_STIMULUS_CODE+40)
#define STATIC_CONJUGATE (LAST_STIMULUS_CODE+41)
#define REWARD_SIZE (LAST_STIMULUS_CODE+42)
#define RAMP_HOLD_CODE (LAST_STIMULUS_CODE+43)
#define TIMEOUT_CODE (LAST_STIMULUS_CODE+44)
#define SOFTOFF_CODE (LAST_STIMULUS_CODE+45)
#define QUERY_STATE (LAST_STIMULUS_CODE+46)
#define STOP_BUTTON (LAST_STIMULUS_CODE+47)
#define RAMP_AMPLITUDE_CODE (LAST_STIMULUS_CODE+48)
#define TRIAL_START_BLANK (LAST_STIMULUS_CODE+49)
#define UPLOAD_CODE (LAST_STIMULUS_CODE+50)
#define VERGENCE_WINDOW (LAST_STIMULUS_CODE+51)
#define SHORT_PREM_CODE (LAST_STIMULUS_CODE+52)
#define ELECTRODE_DEPTH (LAST_STIMULUS_CODE+53)
#define UFF_COMMENT (LAST_STIMULUS_CODE+54)
#define SEED_SHIFT (LAST_STIMULUS_CODE+55)
#define CONJUGATE_POSITION (LAST_STIMULUS_CODE+56)
#define DEG2PIX (LAST_STIMULUS_CODE+57)
#define DRAGGED_POS (LAST_STIMULUS_CODE+58)
#define STIMULUS_OPTIONS (LAST_STIMULUS_CODE+59)
#define SACCADE_VALUE (LAST_STIMULUS_CODE+60)
#define HSACCADE_VALUE SACCADE_VALUE
#define WRONG_TIMEOUT_CODE (LAST_STIMULUS_CODE+61)
#define STARTVAL (LAST_STIMULUS_CODE+62)
#define MAXSTAIRVAL (LAST_STIMULUS_CODE+63)
#define MINSTAIRVAL (LAST_STIMULUS_CODE+64)
#define STANDING_DISP (LAST_STIMULUS_CODE+65)
#define SAC_FIX_CRIT (LAST_STIMULUS_CODE+66)
#define SACTARGSIZE (LAST_STIMULUS_CODE+67)
#define VSACCADE_VALUE (LAST_STIMULUS_CODE+68)
#define DOT_DENSITY (LAST_STIMULUS_CODE+69)
#define VSR (LAST_STIMULUS_CODE+70)
#define SEND_CLEAR (LAST_STIMULUS_CODE+71)
#define EXPTYPE_CODE2 (LAST_STIMULUS_CODE+72)
#define EXPT2_MEAN LAST_STIMULUS_CODE+73
#define EXPTYPE_NONE LAST_STIMULUS_CODE+74
#define EXPT2_NSTIM LAST_STIMULUS_CODE+75
#define EXPT2_INCR LAST_STIMULUS_CODE+76
#define ANTICORRELATED_DISPARITY LAST_STIMULUS_CODE+77
#define CORRELATED_DISPARITY LAST_STIMULUS_CODE+78
#define DISTRIBUTION_MEAN LAST_STIMULUS_CODE+79
#define NPLANES  LAST_STIMULUS_CODE+80
#define MONOCULARITY_EXPT LAST_STIMULUS_CODE+81
#define VERGENCE_CORRECTION LAST_STIMULUS_CODE+82
#define FIXATION_SURROUND LAST_STIMULUS_CODE+83
#define BACK_SIZE LAST_STIMULUS_CODE+84
#define FIXATION_OVERLAP LAST_STIMULUS_CODE+85
#define PREWARD LAST_STIMULUS_CODE+86
#define ACQUISITION_CODE LAST_STIMULUS_CODE+87
#define RELDISP LAST_STIMULUS_CODE+88
#define ORI_LEFT LAST_STIMULUS_CODE+89
#define ORI_RIGHT LAST_STIMULUS_CODE+90
#define VERSION_CODE LAST_STIMULUS_CODE+91
#define STIM_SIZE LAST_STIMULUS_CODE+92
#define BACK_CORRELATION LAST_STIMULUS_CODE+93
#define SEED_DELAY LAST_STIMULUS_CODE+94
#define TRIGGER_LEVEL1 LAST_STIMULUS_CODE+95
#define TRIGGER_LEVEL2 LAST_STIMULUS_CODE+96
#define TRIGGER_LEVEL3 LAST_STIMULUS_CODE+97
#define TRIGGER_LEVEL4 LAST_STIMULUS_CODE+98
#define RC_SEED LAST_STIMULUS_CODE+99
#define FRAMEREPEAT LAST_STIMULUS_CODE+100
#define STIMCHANGE_CODE LAST_STIMULUS_CODE+101
#define FIXPOS_XY  LAST_STIMULUS_CODE+102
#define RF_ANGLE LAST_STIMULUS_CODE+103
#define BACKSTIM_TYPE LAST_STIMULUS_CODE+104
#define FP_MOVE_SIZE LAST_STIMULUS_CODE+105
#define FP_MOVE_DIR  LAST_STIMULUS_CODE+106
#define START_PHASE  LAST_STIMULUS_CODE+107
#define BACK_ORI  LAST_STIMULUS_CODE+108
#define BACK_TF  LAST_STIMULUS_CODE+109
#define EARLY_RWTIME  LAST_STIMULUS_CODE+110
#define NEXTRAS_CODE  LAST_STIMULUS_CODE+111
#define MANUAL_TDR  LAST_STIMULUS_CODE+112
#define EXPTYPE_CODE3 (LAST_STIMULUS_CODE+113)
#define MODULATION_F (LAST_STIMULUS_CODE+114)
#define FIXPT_DISPARITY (LAST_STIMULUS_CODE+115)
#define RELVDISP (LAST_STIMULUS_CODE+116)
#define VDISP_MOD (LAST_STIMULUS_CODE+117)
#define LENGTH_DISP (LAST_STIMULUS_CODE+118)
#define WIDTH_DISP (LAST_STIMULUS_CODE+119)
#define NCOMPONENTS (LAST_STIMULUS_CODE+120)
#define STIMID (LAST_STIMULUS_CODE+121)
#define ORI2 (LAST_STIMULUS_CODE+122)
#define EXPT3_NSTIM (LAST_STIMULUS_CODE+123)
#define SPINRATE (LAST_STIMULUS_CODE+124)
#define BACK_WIDTH (LAST_STIMULUS_CODE+125)
#define BACK_HEIGHT (LAST_STIMULUS_CODE+126)
#define DOT_POSX (LAST_STIMULUS_CODE+127)
#define ORI_BANDWIDTH (LAST_STIMULUS_CODE+128)
#define RC1INC (LAST_STIMULUS_CODE+129)
#define DISTRIBUTION_WIDTH (LAST_STIMULUS_CODE+130)
#define FIXWIN_HEIGHT (LAST_STIMULUS_CODE+131)
#define EXPT3_INCR (LAST_STIMULUS_CODE+132)
#define DISTRIBUTION_CONC (LAST_STIMULUS_CODE+133)
#define SACCADE_THRESHOLD (LAST_STIMULUS_CODE+134)
#define SACCADE_DETECTED (LAST_STIMULUS_CODE+135)
#define INITIAL_DISPARITY (LAST_STIMULUS_CODE+136)
#define VERTICAL_VERGENCE (LAST_STIMULUS_CODE+137)
#define XPIXEL_CODE (LAST_STIMULUS_CODE+138)
#define YPIXEL_CODE (LAST_STIMULUS_CODE+139)
#define VIEWD_CODE (LAST_STIMULUS_CODE+140)
#define STIM_PULSES (LAST_STIMULUS_CODE+141)
#define PULSE_WIDTH (LAST_STIMULUS_CODE+142)
#define PURSUIT_INCREMENT (LAST_STIMULUS_CODE+143)
#define BACKGROUND_IMAGE (LAST_STIMULUS_CODE+144)
#define FASTB_RPT (LAST_STIMULUS_CODE+145)
#define BACKGROUND_ZOOM (LAST_STIMULUS_CODE+146)
#define RC_REPEATS (LAST_STIMULUS_CODE+147)
#define AFC_PROPORTION (LAST_STIMULUS_CODE+148)
#define INITIAL_MOVEMENT (LAST_STIMULUS_CODE+149)
#define BACK_OPOS (LAST_STIMULUS_CODE+150)
#define BACK_PPOS (LAST_STIMULUS_CODE+151)
#define REWARD_BIAS (LAST_STIMULUS_CODE+152)
#define TARGET_XOFFSET  (LAST_STIMULUS_CODE+153)
#define TARGET_YOFFSET  (LAST_STIMULUS_CODE+154)
#define TARGET2_POS  (LAST_STIMULUS_CODE+155)
#define MIXAC (LAST_STIMULUS_CODE+156)
#define TONETIME (LAST_STIMULUS_CODE+157)
#define IMAGEJUMPS (LAST_STIMULUS_CODE+158)
#define IMAGEINDEX (LAST_STIMULUS_CODE+159)
#define SACCADE_PUNISH_DIRECTION (LAST_STIMULUS_CODE+160)
#define CHAMBER_ADAPTER (LAST_STIMULUS_CODE+161)
#define PURSUIT_AMPLITUDE (LAST_STIMULUS_CODE+162)
#define PURSUIT_FREQUENCY (LAST_STIMULUS_CODE+163)
#define MICROSTIM_PHASE_ONSET (LAST_STIMULUS_CODE+164)
#define MICROSTIM_PHASE_DURATION (LAST_STIMULUS_CODE+165)
#define MICROSTIM_PERIODIC (LAST_STIMULUS_CODE+166)
#define IMAGEXPOS (LAST_STIMULUS_CODE+167)
#define IMAGEYPOS (LAST_STIMULUS_CODE+168)
#define NBARS (LAST_STIMULUS_CODE+169)
#define VERGVERSION_CODE (LAST_STIMULUS_CODE+170)
#define EXPT_NAME (LAST_STIMULUS_CODE+171)
#define STIMULUS_TAG (LAST_STIMULUS_CODE+172)
#define STIM3_TYPE (LAST_STIMULUS_CODE+173)
//Add above here anything that needs to go to Spike2


#define MAXSERIALCODES (LAST_STIMULUS_CODE+174)/* 204 */
#define OPPOSITE_DELAY  MAXSERIALCODES
#define FAST_SEQUENCE_RPT MAXSERIALCODES+1
#define BRIGHTSHIFT_CODE MAXSERIALCODES+2
#define BLANKCOLOR_CODE MAXSERIALCODES+3
#define DOTREPEAT MAXSERIALCODES+4
#define SACCADE_AMPLITUDE MAXSERIALCODES+5
#define FIX_OFF_CODE MAXSERIALCODES+6
#define LOGFILE_CODE MAXSERIALCODES+7
#define STOP_CRITERION MAXSERIALCODES+8
#define RUNAVERAGE_LENGTH MAXSERIALCODES+9
#define SIDE_OFFSET MAXSERIALCODES+10
#define END_OFFSET MAXSERIALCODES+11
#define XMINSAMPLE_RATIO MAXSERIALCODES+12
#define HELPFILE_PATH MAXSERIALCODES+13
#define XMAXSAMPLE_RATIO MAXSERIALCODES+14
#define STIMULUS_VERSION MAXSERIALCODES+15
#define PENETRATION_TEXT MAXSERIALCODES+16
#define RF_PARA MAXSERIALCODES+17
#define PLC_MAG MAXSERIALCODES+18
#define TARGET_OFFSET MAXSERIALCODES+19
#define DIRECTION_TEXT MAXSERIALCODES+20
#define CELLTYPE_TEXT  MAXSERIALCODES+21
#define STIMULUS_FLAG  MAXSERIALCODES+22
#define SWAPINTERVAL  MAXSERIALCODES+23
#define TRAPEZOIDAL_SCALING  MAXSERIALCODES+24
#define RF_WIDTH MAXSERIALCODES+25
#define RF_HEIGHT MAXSERIALCODES+26
#define RF_X MAXSERIALCODES+27
#define RF_Y MAXSERIALCODES+28
#define RF_ORTHO MAXSERIALCODES+29
#define SPIKE_GAIN MAXSERIALCODES+30

//#define PLOTW MAXSERIALCODES+31
//#define PLOTH MAXSERIALCODES+32
//#define PLOTX MAXSERIALCODES+33
//#define PLOTY MAXSERIALCODES+34
#define TRIAL_COUNT MAXSERIALCODES+35
#define GOODTRIALS MAXSERIALCODES +36
#define BADTRIALS MAXSERIALCODES +37
#define DISP_AS_CYCLES MAXSERIALCODES +38
#define QUICKEXPT_CODE MAXSERIALCODES +39
#define GAMMAVAL_CODE MAXSERIALCODES +40
#define CORRECTION_LEAVE_CRIT MAXSERIALCODES +41
#define CORRECTION_ENTRY_CRIT MAXSERIALCODES +42
#define BLANK_P MAXSERIALCODES +43
#define COVARY_XPOS MAXSERIALCODES +44
#define BONUS_CRITERION MAXSERIALCODES +45
#define BONUS2_CRITERION MAXSERIALCODES+46
#define EXPT_BLKSIZ MAXSERIALCODES+47
#define CONTRAST_LEFT MAXSERIALCODES+48
#define CONTRAST_RIGHT MAXSERIALCODES+49
#define TWOBAR_SEP MAXSERIALCODES+50
#define FIX_LINW MAXSERIALCODES+51
#define SETCLEARCOLOR MAXSERIALCODES+52
#define BACKJUMP MAXSERIALCODES+53
#define SF_LEFT MAXSERIALCODES+54
#define SF_RIGHT MAXSERIALCODES+55
#define RANDOM_X MAXSERIALCODES+56
#define RANDOM_ORI MAXSERIALCODES+57
#define DORI_BOTH  MAXSERIALCODES+58
#define DORI_BACK  MAXSERIALCODES+59
#define STIM_ECCENTRICITY  MAXSERIALCODES+60
#define STIM_POLARANGLE  MAXSERIALCODES+61
#define OLDLEFT_ORI MAXSERIALCODES+62
#define OLDRIGHT_ORI MAXSERIALCODES+63
#define CYBER_CHANNELS MAXSERIALCODES+64
#define RF_SIGN MAXSERIALCODES+65
#define END_LENGTH MAXSERIALCODES+66
#define EXPT_LINW MAXSERIALCODES+67
//#define PLOTSMOOTH MAXSERIALCODES+68
#define SHOWFLAGS_CODE MAXSERIALCODES+69
#define EXPT_STIMPERTRIAL MAXSERIALCODES+70
//#define RC_DELAY MAXSERIALCODES+71
#define FIXCOLORS MAXSERIALCODES+72
#define CONTRAST_RATIO MAXSERIALCODES+73
//#define ADDEXPTS_CODE  MAXSERIALCODES+74
//#define COMMAND_IN MAXSERIALCODES+75
#define GRIDSIZE MAXSERIALCODES+76
#define ORTHOG_DISP MAXSERIALCODES+77
#define PARALELL_DISP MAXSERIALCODES+78
#define RDSXSINE MAXSERIALCODES+79
#define CHANGE_SEED MAXSERIALCODES+80
#define DISP_A MAXSERIALCODES+81
#define DISP_B MAXSERIALCODES+82
#define FP_MOVE_FRAME MAXSERIALCODES+83
#define XOFFLOG MAXSERIALCODES+84
#define LRBINTERLEAVE MAXSERIALCODES+85
#define LRINTERLEAVE MAXSERIALCODES+86
#define LBINTERLEAVE MAXSERIALCODES+87
#define RBINTERLEAVE MAXSERIALCODES+88
#define SFLIN MAXSERIALCODES+89
#define TFLIN MAXSERIALCODES+90
#define LOG_SIZE MAXSERIALCODES+91
#define EXPT3_MEAN MAXSERIALCODES+92
#define UNCORR_P MAXSERIALCODES +93
#define ONETARGET_P MAXSERIALCODES+94
#define STIMORTHOG_POS MAXSERIALCODES+95
#define PLAID_RATIO MAXSERIALCODES+96
#define NSPIKE_PRINT MAXSERIALCODES+97
#define PRINTYSCALE MAXSERIALCODES+98
#define NSPIKE_SKIP MAXSERIALCODES+99
#define AUTO_ZERO MAXSERIALCODES+100
#define OLD_ORTHOG_DISP (MAXSERIALCODES+101)
#define OLD_PARA_DISP MAXSERIALCODES+102
#define ORTHOG_POS MAXSERIALCODES+103
#define PARA_POS MAXSERIALCODES+104
#define LINEAR_SPEED MAXSERIALCODES+105
#define WEIGHT_TEXT MAXSERIALCODES+106
#define RESTRAIN_TEXT MAXSERIALCODES+107
#define RDSBNONE MAXSERIALCODES+108
#define JUMPTYPE MAXSERIALCODES+109
#define TARGET_RATIO MAXSERIALCODES+110
#define HIGHSF MAXSERIALCODES+111
#define HIGHTF MAXSERIALCODES+112
#define CONTRAST_PAIRS MAXSERIALCODES+113
//#define STEPPER_PLOTLEN MAXSERIALCODES+114
#define NFRAMES_DONE MAXSERIALCODES+115
#define PLOT_CLUSTER MAXSERIALCODES+116
#define REWARD_SIZE1 MAXSERIALCODES+117
#define REWARD_SIZE2 MAXSERIALCODES+118
#define REWARD_SIZE3 MAXSERIALCODES+119
#define TIMEOUT_CONTRAST MAXSERIALCODES+120
#define RCFILE_CTR MAXSERIALCODES+121
#define ALTERNATE_STIM_MODE MAXSERIALCODES+122
#define MICROSTIM_EXPT MAXSERIALCODES+123
#define TEST_VALUE1 MAXSERIALCODES+124
#define TEST_VALUE2 MAXSERIALCODES+125
#define BACK_ADD_SIZE MAXSERIALCODES+126
#define RC_STIMCHANGE MAXSERIALCODES+127
#define PENNUMCOUNTER MAXSERIALCODES+128
#define PENXPOS MAXSERIALCODES+129
#define PENYPOS MAXSERIALCODES+130
#define VWHERE MAXSERIALCODES+131
#define IFCSCALE MAXSERIALCODES+132

#define HEIGHT_L MAXSERIALCODES+134
#define HEIGHT_R MAXSERIALCODES+135
#define WIDTH_L MAXSERIALCODES+136
#define WIDTH_R MAXSERIALCODES+137
#define SIZE_L MAXSERIALCODES+138
#define SIZE_R MAXSERIALCODES+139
#define SIZE_DISP MAXSERIALCODES+140 //Width, Length disps stored
#define FIXPT_EYE MAXSERIALCODES+141
#define SD_BOTH MAXSERIALCODES+142
#define UKA_VALS MAXSERIALCODES+143
//#define PHASE_AS_RELDISP MAXSERIALCODES+144
//#define PANEL_XPOS MAXSERIALCODES+145
#define FB_RELATIVE_CONTRAST MAXSERIALCODES+146
#define BACK_VDISP MAXSERIALCODES+147
//#define PANEL_YPOS MAXSERIALCODES+148
#define SET_COUNTERPHASE MAXSERIALCODES+149
#define USERID MAXSERIALCODES+150
#define TEMPORAL_ORI_BANDWIDTH MAXSERIALCODES+151
#define SHAKE_TIMEOUT_DURATION MAXSERIALCODES+152
#define ORTHOG_POSL MAXSERIALCODES+153
#define ORTHOG_POSR MAXSERIALCODES+154
#define COVARIATE MAXSERIALCODES+155
#define INITIAL_DURATION MAXSERIALCODES+156
#define EXPT1_MAXSIG MAXSERIALCODES+157
#define FAKESTIM_EXPT MAXSERIALCODES+158
#define FAKESTIM_SIGNAL MAXSERIALCODES+159
#define BLACKDOT_FRACTION MAXSERIALCODES+160
#define HIGHXTYPE MAXSERIALCODES+161
#define MONKEYNAME MAXSERIALCODES+162
#define SIGNALFRAME_P MAXSERIALCODES+163
#define COMMAND_FILE MAXSERIALCODES+164
#define NPHASES MAXSERIALCODES+165
#define JUMP_SF_COMPONENTS MAXSERIALCODES+166
#define SET_SF_COMPONENTS MAXSERIALCODES+167
#define SET_TF_COMPONENTS MAXSERIALCODES+169
#define PSYCHMON_VARS MAXSERIALCODES+170
#define EXPTRESET_FILE MAXSERIALCODES+171
#define SET_SF_CONTRASTS MAXSERIALCODES+172
#define MAXSAVECODES MAXSERIALCODES+173 //364
//Add above here anything that needs to be saved to .stm files
//Below here are temporary/convenience/state parameters that do not need saving

#define ASPECT_RATIO MAXSAVECODES
//#define HIDDEN_OPTIONS MAXSAVECODES+1
#define NLOWCOMPONENTS MAXSAVECODES+2
#define NHIGHCOMPONENTS MAXSAVECODES+3
#define LAST_SEED MAXSAVECODES+4
#define FIXPOS_XSHIFT MAXSAVECODES+5
#define NEWDISP MAXSAVECODES+6
#define BACKGROUND_MOVIE MAXSAVECODES+7
#define SETANNULUS MAXSAVECODES+8
#define PEN_START_DEPTH MAXSAVECODES+9
#define TWOCYL_DISP MAXSAVECODES+10
#define RANDOM_CONTRAST_EXPT MAXSAVECODES+11
#define FRAME_DISPLACEMENT MAXSAVECODES+12
#define ABS_ORTHOG_POS MAXSAVECODES+13
#define ABS_PARA_POS MAXSAVECODES+14
#define VERBOSE_CODE MAXSAVECODES+15
#define SEEDOFFSET MAXSAVECODES+16
#define SEEDRANGE (MAXSAVECODES+17)
#define NIMPLACES (MAXSAVECODES+18)
#define USENEWDIRS (MAXSAVECODES+19)
#define CHOICE_TARGET_DURATION (MAXSAVECODES+20)
#define FORCE_CHOICE_ANGLE (MAXSAVECODES+21)
#define INTERTRIAL_MIN (MAXSAVECODES+22)
#define IMPEDANCE  (MAXSAVECODES+23)
#define PROTRUSION (MAXSAVECODES+24)
#define HEMISPHERE (MAXSAVECODES+25)
#define COARSEMM (MAXSAVECODES+26)
#define PSYCH_VALUE (MAXSAVECODES+27)
#define EXPT_PREFIX (MAXSAVECODES+28)
#define LEFTX (MAXSAVECODES+29)
#define LEFTY (MAXSAVECODES+30)
#define RIGHTX (MAXSAVECODES+31)
#define RIGHTY (MAXSAVECODES+32)
#define ELECTRODE_TYPE (MAXSAVECODES +33)
#define ONLINEPREFIX (MAXSAVECODES+34)
#define PSYCHFILE (MAXSAVECODES+35)
#define IMAGELOAD_TYPE (MAXSAVECODES+36)
#define IMAGELOAD_PREFIX (MAXSAVECODES+37)
#define IMAGELOAD_MODE (MAXSAVECODES+38)
#define MONITOR_FILE  (MAXSAVECODES+39)
#define SEQUENCE_PAUSE (MAXSAVECODES+40)
#define MAXTOTALCODES (MAXSAVECODES+43)  //372


/*
 *  MAXTOTALCODES is the total number of values with string codes
 * anything not needing a string (e.g. EARLY_RWSIZE) is > MAXTOTALCODES
 * anything that verg and binoc need t communicat needs to be above
 */


/* 
 * some redefinitions to use some spare variables for things only
 * adjusted in psychophysics
 */

#define PSYCH_EXPOFFSET TRIGGER_LEVEL

#define OLDFIXEDLORI 173
#define OLDFIXEDRORI 174
#define STIM_PERIOD MAXTOTALCODES+20
#define RF_DIMENSIONS MAXTOTALCODES +21
#define STIMULUS_LABEL_WIDGET MAXTOTALCODES+22
#define WAIT_FOR_RESPONSE MAXTOTALCODES+23
#define RESPONDED MAXTOTALCODES+24
#define SETOVERLAYCOLOR MAXTOTALCODES+25
#define WAIT_FOR_MOUSE MAXTOTALCODES+26

#define LOGCOMMENT MAXTOTALCODES+27
#define SFxDT MAXTOTALCODES+32
#define DT34F MAXTOTALCODES+33
#define CYCLES2 MAXTOTALCODES+34
#define BXD MAXTOTALCODES+35
#define DTXO MAXTOTALCODES+36
#define TEST_BUTTON MAXTOTALCODES+37
#define DTSV  MAXTOTALCODES+38
#define BD0  MAXTOTALCODES+39
#define BDP  MAXTOTALCODES+40
#define SFxLR MAXTOTALCODES+41
#define OXD MAXTOTALCODES+44
#define RASTER_PREPERIOD MAXTOTALCODES+45
#define DXD MAXTOTALCODES+46
#define DMXO MAXTOTALCODES+47
#define SDM MAXTOTALCODES+48
#define XODM MAXTOTALCODES+49
#define YODM MAXTOTALCODES+50
#define TEST_AC MAXTOTALCODES+51
#define EXPT_EXPNO MAXTOTALCODES+52
#define LOXRO MAXTOTALCODES+55
#define CJXO MAXTOTALCODES+56
#define FSD_BUTTON MAXTOTALCODES+57
#define EYE_DOMINANCE MAXTOTALCODES+58
#define SZEM MAXTOTALCODES+59
#define RF_SET MAXTOTALCODES +60
#define RESET_STATE MAXTOTALCODES +61
#define EXPTSTRING MAXTOTALCODES +65
#define OTBW MAXTOTALCODES+66
#define ODPD MAXTOTALCODES+67
#define EARLY_RWSIZE MAXTOTALCODES+68
#define RESET_TRIALS MAXTOTALCODES +69
#define END_TIMEOUT MAXTOTALCODES +71
#define TOTALCODES MAXTOTALCODES+72
/*
 *  TOTALCODES is the total numer of CHAMBER_AD in Expt 
 * anything not needed in Expt is > TOTALCODES
 */


#define MYFLAGS_CODE TOTALCODES
#define SHOW_COMMAND_HISTORY TOTALCODES+1
#define EXPTVALS TOTALCODES+2

#define XY_FSD 10

/* bw toggle codes */
#define BW_UFF_IS_OPEN 0

#define isserialcode(x) ((int)(x) >= 0 && x <= NSPECIAL_CODES && x != 10 && x != 13)


/* experiment types that do not have one of the above codes */


/* by default send all codes at the start of an expt */
#define SEND_STIMULUS 0
#define SEND_EXPT 1
#define SEND_EXPLICIT 2
#define SEND_USER_ONLY 3
#define SEND_GRATING2 4
#define SEND_NON_ZERO 5
#define SEND_EXPT_NONZERO 6
#define SEND_NEVER 1000  // never goes down serial line, just for setup


/*
 * Stimulus Types
 */
#define STIM_NONE 0
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
#define STIM_CORRUG 12
#define STIM_SQCORRUG 13
#define STIM_TWOBAR 14
#define STIM_RLS 15
#define STIM_ANNULUS 16
#define STIM_RDSSINE 17
#define STIM_GRATINGN 18
#define STIM_RLSSINE 19
#define STIM_RADIAL 20
#define STIM_IMAGE 21
#define STIM_CHECKER 22
#define N_STIMULUS_TYPES 23
#define STIM_SUBGRATING1 N_STIMULUS_TYPES
#define STIM_SUBGRATING2 N_STIMULUS_TYPES+1
#define STIM_CHANGES N_STIMULUS_TYPES+2
#define STIM_ANNULUSPLUS N_STIMULUS_TYPES+3
#define MAXSTIMULUSTYPES N_STIMULUS_TYPES+4

#define STIM_FOREGROUND MAXSTIMULUSTYPES+1
#define STIM_BACKGROUND MAXSTIMULUSTYPES+2
#define WHOLESTIM MAXSTIMULUSTYPES+3
#define STIM_NUM3 MAXSTIMULUSTYPES+4
#define STIM_CHOICE1 MAXSTIMULUSTYPES+5
#define STIM_CHOICE2 MAXSTIMULUSTYPES+6

#define STIM_THIRD_BIT (1<<2)
#define STIM_CHOICE1_BIT (1<<3)
#define STIM_CHOICE2_BIT (1<<4)



/*
 * Each stimulus/Experimental Parameter has a 2 character code
 * for communication via the serial line
 * this file, common to BrainWave and the SG code,
 * lists the codes. The constants are all defined
 * in commdefs.h
 */

typedef struct stringcode {
    char *code;
    char *label;
    int icode;
    char *value;
    
} StringCode;

typedef struct togglecode {
    char *label;
    char *code;
  int on;  //if on
  int icode;  // bit set
  int group; //optionflag 1 or 2 or optionflags
    int active; // not shown if off
    
} ToggleCode;

typedef struct valuecode {
    char *code;
    char *label;
    int icode;
    int group;
  char ctype; //'C' if this is a string
    int nfplaces; //#places for printing
    int codesend;
    
} ValueCode;

#define EXPT_NOT_ALLOWED 2048


#ifdef CHARCODES_DEFINE

#define NCODES MAXTOTALCODES



ToggleCode togglestrings[] = {
//  {"Goraud Shading",    "gs", 0, GORAUD_BIT, 1},
//  {"Dithering",    "di", 0, DITHERMODE, 1},
/* group 1 fixation and trial structure */
    {"Go",   "do",  0, GO_BIT, 1},
    {"Store",    "ts", 0, STORE_WURTZ_BIT, 1},
    {"Fix cross",    "fc", 0, DRAW_FIX_BIT, 1},
    {"Box",    "fb", 0, SQUARE_FIXATION, 1},
    {"Checkfix",    "cf", 0, FIXATION_CHECK, 1},
    {"Whole Stimuli",    "ws", 0, WURTZ_WHOLE_STIMULI, 1},
    {"Trials",    "sw", 0, STIM_IN_WURTZ_BIT, 1},
    {"Wait for BW",    "bw", 0, WAIT_FOR_BW_BIT, 1},
//    {"Wurtz Task",    "wt", 0, WURTZFLAG, 1}, //not used anywhere
    {"L Monoc",    "lm", 0, LEFT_FIXATION_CHECK, 1},
    {"R Monoc",    "rm", 0, RIGHT_FIXATION_CHECK, 1},
    {"Binoc FP",    "bm",  0, BINOCULAR_FIXPOINT, 3},
    //   {"RC",    "rc",  0, REVERSE_CORRELATE, 3},
    //   {"no status",    "s0",  0, HIDE_STATUS, 3},
    {"Grey Monoc",    "gm",  0, GREY_MONOC, 3},
    {"Fake dFP",    "fp",  0, SIMULATE_FP_MOVE, 3},
    {"no mirrors",    "m0",  0, NO_MIRRORS, 3},
    {"Check Vergence",    "cv", 0, VERGENCE_CHECK, 1},
    {"Vary Vergence",    "te", 0, INTERLEAVE_VERGENCE, 2},
    {"See Conjug Pos",  "co", 0, SHOW_CONJUG_BIT, 1},
    {"Move RF with conjug",    "mr",  0, MOVE_RF, 3},

    
    {"PsychoPhysics","lbl1",0,0, 0},
    {"AFC", "afc", 0, AFC, 2,},
    //   {"P.Perf",     "pp",  0, PERF_STRING, 2},
    {"Indicate Reward Bias",    "sR",  0, SHOW_REWARD_BIAS, 3},
//    {"Collapse Expt3",    "C3",  0, COLLAPSE_EXPT3, 3},
    {"Odd Man Out",    "af3", 0, ODD_MAN_OUT_TASK, 3},
    {"Choice by icon",    "Rcd", 0, CHOICE_BY_ICON, 3},
    {"Icon In Trial",    "Iit", 0, ICON_IN_TRIAL, 3},
    {"Expt1 Mean as Crit",    "epy", 0, EXPTMEAN_FOR_PSYCH, 3},
    {"Flip",    "ff",  0, FLIP_FEEDBACK, 3},
    {"Count BadFix",    "cB",  0, COUNT_BADFIX_FOR_BONUS, 3},
    {"Human Psych",    "py", 0, PSYCHOPHYSICS_BIT, 2},
    {"IFC",    "if",  0, IFC, 2},
    {"Feedback",    "fe",  0, FEEDBACK, 3},
    {"Nonius",    "no",  0, NONIUS_LINES, 3},
    {"nonius for V",    "vn", 0, VVNONIUS_LINES, 3},
    {"Grating Fixpt",    "fG",  0, FIXPT_GRATING, 3},
//    {"4Choice",    "af4", 0, FOUR_CHOICES, 3},

    {"Experiment Sequence","lbl2",0,0, 0},
    {"FastSeq",    "fS",  0, FAST_SEQUENCE, 3},
    {"Xexp2",    "x2",  0, TIMES_EXPT, 3},
    {"+Blank",    "ic", 0, INTERLEAVE_BLANK, 2},
    {"+monoc",    "im", 0, INTERLEAVE_ONE_MONOCULAR, 2},
    {"xMonoc",    "Im", 0, INTERLEAVE_MONOC_ALL, 3},
    {"+uncorr",    "iu", 0, INTERLEAVE_UNCORRELATED, 2},
    {"xUncorr",    "Iu",  0, INTERLEAVE_UNCORR_ALL, 3},
    {"+zero",    "iz",  0, INTERLEAVE_ZERO, 3},
    {"xZeroS",    "I0", 0, INTERLEAVE_ZERO_ALL, 3},
    {"+random",    "ir", 0, INTERLEAVE_RANDOM, 2},
    {"+highX",    "hx",  0, INTERLEAVE_HIGH, 3},
    {"xHigh",    "hX",  0, INTERLEAVE_HIGH_ALL, 3},
    {"+highTF",    "ht",  0, INTERLEAVE_HIGHTF, 3},
    {"+highSF",    "hs",  0, INTERLEAVE_HIGHSF, 3},
    {"+highSQ",    "hq", 0, INTERLEAVE_HIGHSQ, 3},
    {"+Flip",    "fi",  0, INTERLEAVE_FLIP, 3},
    {"+component",    "Ic",  0, INTERLEAVE_COMPONENT, 3},
    {"+FPmove",    "iF",  0, ADD_FPJUMP, 3},
    //   {"RunSeq",    "rS",  0, RUN_SEQUENCE, 3},
    {"Tile-XY",    "xy",  0, TILE_XY, 3},
    {"microstim",    "uS",  0, MICROSTIM, 3},
    {"Ustim only 0 signal",    "U0",  0, USTIM_ZERO_ONLY, 3},
    {"Random Contrast",    "rC",  0, RANDOM_CONTRAST, 3},
    {"Random Correlation",    "rI",  0, RANDOM_CORRELATION, 3},
    //   {"AutoCopy",    "cd",  0, AUTO_COPY_ONLINE_FILES, 3},
    {"Custom Vals Expt2",    "cb", 0, CUSTOM_EXPVALB, 3},
    {"Search..",    "sm", 0, SEARCH_MODE_BIT, 1},
    {"Online Data",    "da",  0, ONLINE_DATA, 3},

    {"Stimulus Rendering","lbl3",0,0, 0},
    {"AntiAlias",  "aa", 0, ANTIALIAS_BIT, 1},
    {"Square", "sq", 0, SQUARE_RDS, 1},
    {"Cnrst Rev",    "cr",  0, CONTRAST_REVERSE_BIT, 1},
//   {"S.E.",    "se",  0, SE_BIT, 1},
    {"Paint back",    "bp",  0, PAINT_BACKGROUND, 3},
   {"Back Fixed",    "bf",  0, BACKGROUND_FIXED_BIT, 1},
    {"wipe screen",    "cs", 0, WIPE_WHOLE_SCREEN, 2},
    {"RANDOM",    "ra", 0, RANDOM, 2},
    {"Stair",    "sc",  0, STAIRCASE, 2},
    {"RandExp2",    "r2",  0, RANDOM_EXPT2, 3},
    {"RevExpt2",    "i2",  0, REVERSE_EXPT2, 3},
    {"Image Jumps",    "ijump", 0, JUMP_IMAGE, 3},
    //  {"Contour",    "cn",  0, CONTOUR_PLOT, 3},
    //  {"Smooth/Polar",    "gc",  0, SMOOTH_CONTOUR, 3},
    //  {"Sequence",    "to",  0, PLOT_SEQUENCE, 3},
    {"Counterphase",    "cp", 0, COUNTERPHASE, 3},
    {"Rand Phase",    "rp",  0, RANDOM_PHASE, 3},
    {"Rand Init Phase",    "rIp",  0, RANDOM_INITIAL_PHASE, 3},
    {"Rand RelPhase",    "rr",  0, RANDOM_RELPHASE, 3},
    {"Random dPhase",    "rP",  0, RANDOM_DEPTH_PHASE, 3},
//    {"Track",    "pt",  0, PLOT_ELECTRODE_TRACK, 3},
//   {"Vgc Ramps",    "vr", 0, RAMP_EXPT_BIT, 1},
//   {"RampHold",    "rh", 0, RAMP_HOLD_BIT, 1},
   {"Flash",    "fl", 0, FLASH_BIT, 2},
   {"Interact",    "ei", 0, EXPT_INTERACTIVE, 2},
    {"modulate disparity",    "md", 0, MODULATE_DISPARITY, 3},


//   {"Alt Probe",    "pa", 0, ALT_PROBE_BIT, 2},
//   {"Store Expt Only",    "sE",  0, STORE_EXPT_ONLY, 3},
//   {"Always Change",    "ac",  0, ALWAYS_CHANGE_STIM, 3},
//   {"TGauss",    "tg",  0, TEMPORAL_GAUSS, 3},
    
    {"Special Options","lbl4",0,0, 0},
    {"Spike Clear when Move",    "sp",  0, 1, STEP_CLEAR_SPIKES, 3},
   {"Check Frames",    "cF",  0, FRAMESWARN, 3},
   {"Always Backgr",    "ab",  0, BACKGROUND_ALWAYS, 3},
    {"Rnd FPdir",    "rd",  0, RAND_FP_DIR, 3},
//   {"Store LFP",    "lf",  0, STORE_LFP, 3},
//   {"AutoCopy",    "ao",  0, ONLINE_AUTO_COPY, 3},
//   {"PlotFlip",    "pf",  0, PLOTFLIP, 3},
   {"BackLast",    "tl",  0, PAINT_THIRD_LAST, 3},
   {"Stim In Overlay",    "pe", 0, STIMULUS_IN_OVERLAY, 3},
   {"reduce serial out",    "ls", 0, REDUCE_SERIAL_OUTPUT, 3},
   {"center staircase",    "cS", 0, STAIR_CENTER, 3},
   {"Paint all frames",    "FN", 0, FIXNUM_PAINTED_FRAMES, 3},
//   {"stereo Glasses",    "sg", 0, STEREO_GLASSES, 3},
   {"Calc once only",    "Co", 0, CALCULATE_ONCE_ONLY, 3},
   {"Initial Training",    "IT", 0, INITIAL_TRAINING, 3},
   {"Check FrameCounts",    "CF", 0, CHECK_FRAMECOUNTS, 3},
   {"Show Stim Boxes",    "SB", 0, SHOWSTIMBOXES, 3},
    {"Frame Test",    "ft", 0, FRAME_ONLY_BIT, 1},
    {"SplitScreen",    "ss",  0, SPLITSCREEN, 3},
    {"Debug",    "od", 0, DEBUG_OUTPUT, 3},
    {"Watch Times",    "WT", 0, WATCH_TIMES, 3},
    {"Monitor State",    "MS", 0, MONITOR_STATE, 3},
    {"Test mode",    "tm", 0, TEST_BIT, 1},
    {"Fix Sepn",    "fs",  0, FIXED_BARSEP, 3},
    {"Record RLS",    "srls",  0, SAVE_RLS, 3},
    {"Manual Expt",    "exm",  0, MANUAL_EXPT, 3},
    {"Reverse Back Contrast",    "bac",  0, MANUAL_EXPT, 3},
    //   {"Auto Plot",    "ap", 0, AUTO_PLOT, 1},
    //   {"PreBack",    "pb", 0, BACKGROUND_IN_PREPERIOD, 1},
    //   {"Show Val",    "pw", 0, PLOT_WURTZ_BIT, 1},
    //   {"Conjuate Tracking",    "ct", 0, WURTZ_TRACK_BIT, 1},
    //   {"Disp Clamp",    "dc", 0, CLAMP_EXPT_BIT, 1},
    //   {"Clamp+Hold",    "ch", 0, CLAMP_HOLD_BIT, 1},
   {NULL, NULL, 0, 0, 0}
};



/* codes for BW to tell Front things 
 * N.B. THESE CODES MUST BE DIFFERNT FORM toggle_codes in front.c
 */
char *bwtoggle_codes[] = {
    "fo", /* file is open */
    NULL,
};

/*
 * bit codes for groups
 * groups  1 stim prop
 *         2 binocular stim prop (disparity etc)
 *         4 Dot stimuli 
 *         8 fixation point/eye movement
 *         16 Expt setup
 *         32 General Display
 *         64 Background stim
 *         128 commands
 *         256 RF properties
 *         512 Indirect properties
 *         1024 Internal commands (sb+)
 *         2048 can't be an expt type. (Checked in setsecondexpt)
 *         4096 Verg only
 */
#define VERGONLY 4096
#define INTERNALCOMMAND 1024
#define INDIRECT 512
#define RF_PROPERTIES 256
#define COMPOUND_GRATING (4096 << 1)

ValueCode valstrings[] = {
  {"xo","X offset",  XPOS, 1, 'N' ,2, SEND_EXPT},
    {"yo","Y offset",  YPOS, 1, 'N' ,2, SEND_EXPT},
  {"lo", "Log File", LOGFILE_CODE, 2, 'C' ,2, SEND_EXPLICIT},
  {"st", "Stimulus", STIMULUS_TYPE_CODE, 1,'C' ,2, SEND_EXPT},
  {"sx",     "Sigma X",  SD_X, 1, 'N' ,2, SEND_EXPLICIT},
  {"sy",     "Sigma Y",  SD_Y, 1, 'N' ,2, SEND_EXPLICIT},
  {"dp",     "Diparate phase", DISP_P, 3, 'N' ,5, SEND_EXPT},
  {"dx",     "Disparity ", DISP_X, 3, 'N' ,5, SEND_EXPT},
  {"sz",     "Size (H and W)",STIM_SIZE, 1+INDIRECT , 'N' ,2, SEND_EXPLICIT},
  {"wi",     "Width ", STIM_WIDTH, 1, 'N' ,4, SEND_EXPT},
  {"hi",     "Height ", STIM_HEIGHT, 1, 'N' ,4, SEND_EXPT},
  {"sf",     "Spatial Frequency", SF,  1, 'N' ,4, SEND_EXPT},
  {"xn",     "X sampling", XSAMPLES, 1, 'N' ,0, SEND_EXPLICIT},
  {"yn",     "Y Sampling", YSAMPLES, 1, 'N' ,0, SEND_EXPLICIT},
  {"tf",     "Temporal Frequency", TF, 1, 'N' ,2, SEND_EXPT},
  {"ph",     "Spatial Phase",  SETPHASE, 1, 'N' ,2, SEND_EXPT},
  {"or",     "Orientation",  ORIENTATION, 1, 'N' ,1, SEND_EXPT},
  {"sl",     "Seed Loop", SET_SEEDLOOP, 5, 'N' ,0, SEND_EXPT},
  {"se",     "Seed", SET_SEED, 5, 'N' ,0, SEND_STIMULUS},
  {"co",     "Contrast ", SETCONTRAST, 1, 'N' ,4, SEND_EXPLICIT},
  {"dy",     "Vertical Disparity", DISP_Y, 3, 'N' ,4, SEND_EXPT},
  {"f2",     "SF2", SF2, 1+COMPOUND_GRATING, 'N' ,2, SEND_GRATING2},
  {"t2",     "TF2", TF2, 1+COMPOUND_GRATING, 'N' ,2, SEND_GRATING2},
  {"p2",     "Phase2", PHASE2, 1+COMPOUND_GRATING, 'N' ,2, SEND_EXPLICIT},
  {"a2",     "Plaid angle", PLAID_ANGLE, 1+COMPOUND_GRATING, 'N' ,2, SEND_EXPLICIT},
  {"jv",     "J velocity", JVELOCITY, 5, 'N' ,2, SEND_EXPLICIT},
  {"bpos",     "Boundary Position", BPOSITION, 5, 'N' ,2, SEND_EXPLICIT},
  {"bjv",     "Boundary velocity", BVELOCITY, 5, 'N' ,2, SEND_EXPLICIT},
  {"fi",     "front intensity",JF_INTENSITY, 5, 'N' ,2, SEND_EXPLICIT},
  {"bi",     "back intensity", JB_INTENSITY, 5, 'N' ,2, SEND_EXPLICIT},
  {"jn",     "cyl num dots", JNUMDOTS, 5, 'N' ,2, SEND_EXPLICIT},
  {"jl",     "dotlife (cyl)",JLIFEFRAMES, 5, 'N' ,2, SEND_EXPLICIT}, 
  {"jx",     "death chance",  JDEATH, 5, 'N' ,4, SEND_EXPT},
  {"nbars",     "Nnumber of Bars", NBARS, 5, 'N' ,0, SEND_EXPLICIT},
  {"c2",     "contrast 2", CONTRAST2, 1, 'N' ,4, SEND_EXPLICIT},
  {"ic",     "Delta contrast (L-R)", CONTRAST_DIFF, 3, 'N' ,4, SEND_EXPLICIT},
  {"od",     "Delta ori (L-R)", ORIENTATION_DIFF, 3, 'N' ,2, SEND_EXPT_NONZERO},
  {"ds",     "Delta SF (L-R)", SF_DIFF, 3, 'N' ,4, SEND_EXPLICIT},
  {"dg",     "disp grad", DISP_GRAD, 3, 'N' ,4, SEND_EXPLICIT},
  {"dm",     "depth modulation", DEPTH_MOD, 3, 'N' ,4, SEND_EXPLICIT},
//  {"ta",     "twobar angle", TWOBAR_ANGSEP, 1, 'N' ,2, SEND_EXPLICIT},
  {"dr",     "Disp Ramp", DISP_RAMP, 3, 'N' ,2, SEND_EXPLICIT},
  {"dq",     "pDisp 2", PHASE2DISP, 3, 'N' ,2, SEND_EXPLICIT},
  {"rp",     "Rel Phase",  PHASEREL, 3, 'N' ,2, SEND_EXPLICIT},
  {"Pd",     "Phase Disp as Disp", PHASE_AS_DISP, 3, 'N' ,2, SEND_EXPLICIT},
  {"ce",     "Correlation", CORRELATION, 3, 'N' ,4, SEND_STIMULUS},
  {"dw",     "Dot Width", DOT_SIZE, 5, 'N' ,4, SEND_EXPLICIT},
  {"fc",     "Fixation Color", SETFIXCOLOR, 8, 'N' ,2, SEND_EXPLICIT},
//  {"zx",     "Zoom",  SETZOOM, 32, 'N' ,2, SEND_EXPLICIT},
  {"bc",     "BackGround Color ", SETBACKCOLOR, 32, 'N' ,2, SEND_EXPLICIT},
  {"dt",     "Delay", DELAY, 32 , 'N' ,2, SEND_EXPLICIT},
  {"pr",     "PREPERIOD", PREPERIOD_CODE, 16, 'N' ,2, SEND_EXPLICIT}, 
  {"et",     "Expt1 Type",  EXPTYPE_CODE, 16 , 'C' ,2, SEND_EXPT},
  {"fz",     "FRAMERATE",  FRAMERATE_CODE, 32 , 'N' ,2, SEND_EXPLICIT},
  {"du",     "Stimulus Duration", STIMULUS_DURATION_CODE, 16 , 'N' ,2, SEND_EXPLICIT},
  {"vc",     "CLAMP_DISPARITY", CLAMP_DISPARITY_CODE, 0 , 'N' ,2, SEND_EXPLICIT},
  {"po",     "POSTPERIOD", POSTPERIOD_CODE, 16 , 'N' ,2, SEND_EXPLICIT},
  {"mo",     "Mode",MODE_CODE, 0 , 'N' ,2, SEND_EXPLICIT},
  {"vr",     "RAMP_VERGENCE",RAMP_VERGENCE_CODE, 0 , 'N' ,2, SEND_EXPLICIT},
  {"nt",     "N Stim (Expt 1)",NTRIALS_CODE, 32 , 'N' ,2, SEND_EXPLICIT},
  {"uf",     "Uff Prefix",UFF_PREFIX, 32 , 'C' ,2, SEND_USER_ONLY},
  {"hx",     "High X Value for Interleaves",HIGHX, 16 , 'N' ,2, SEND_EXPLICIT},
  {"sM",     "Stimulus mode",STIMULUS_MODE, 1 , 'N' ,2, SEND_EXPT},
  {"wr",     "WURTZ task response period", WURTZ_RT_CODE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"mD",     "Max Signal for Near-zero manip", INITIAL_APPLY_MAX, 17 , 'N' ,5, SEND_EXPT},
  {"op",     "optionflag setup", OPTION_CODE, 16 , 'N' ,2, SEND_STIMULUS},
  {"nr",     "N Repetitions", NREPS_CODE, 16 , 'N' ,2, SEND_EXPT},
  {"tl",     "trigger level", TRIGGER_LEVEL, 0 , 'N' ,2, SEND_EXPLICIT},
  {"fw",     "fixation window", FIXATION_WINDOW, 8 , 'N' ,2, SEND_EXPLICIT},
  {"df",     "display flag",DISPLAYFLAG_CODE, 0 , 'N' ,2, SEND_EXPLICIT},
  {"ch",     "channel Set code",CHANNEL_CODE, 32 , 'N' ,2, SEND_EXPLICIT},
  {"sv",     "Static Vergence",STATIC_VERGENCE, 0 , 'N' ,2, SEND_NON_ZERO},
  {"wf",     "mean fixation period",WURTZ_DURATION_CODE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"bt",     "BW Time",UFF_TIME, 0 , 'N' ,2, SEND_EXPLICIT},
  {"Bc",     "Background Contrast",BACK_CONTRAST, 4+INDIRECT, 'N' ,3, SEND_EXPT},
  {"ei",     "Expt incr", EXPT_INCR, 16 , 'N' ,5, SEND_EXPT},
  {"em",     "Expt Mean", EXPT_MEAN, 16 , 'N' ,5, SEND_EXPT},
  {"is",     "ISI", ISI_CODE, 16 , 'N' ,2, SEND_EXPLICIT},
  {"it",     "InterTrial Duration", INTERTRIAL_MIN, 16 , 'N' ,2, SEND_EXPLICIT},
  {"bw",     "bin width", UNIT_BINWIDTH, 0 , 'N' ,2, SEND_EXPLICIT},
  {"ub",     "binned unit data",BINNED_SPIKE_DATA, 0 , 'N' ,2, SEND_EXPLICIT},
// {"ut",     "unit times",SPIKE_TIMES, 0 , 'N' ,2, SEND_EXPLICIT},
  {"nf",     "N Frames",NFRAMES_CODE, 16 , 'N' ,2, SEND_EXPLICIT},
  {"bd",     "backgound disparity", DISP_BACK, 64 , 'N' ,4, SEND_EXPT},
  {"rk",     "ramp coefficient", RAMP_COEFFICIENT, 0 , 'N' ,2, SEND_EXPLICIT},
  {"fx",     "fixation x pos ",FIXPOS_X, 8 , 'N' ,2, SEND_EXPLICIT},
  {"fy",     "fixation y pos ",FIXPOS_Y, 8 , 'N' ,2, SEND_EXPLICIT},
  {"fs",     "fixation size ",SETFIXSIZE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"cj",     "mean conjugate",STATIC_CONJUGATE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"rw",     "Reward Size", REWARD_SIZE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"vh",     "Vergence RAMP and Hold", RAMP_HOLD_CODE, 0 , 'N' ,2, SEND_EXPLICIT},
  {"ti",     "Timeout",  TIMEOUT_CODE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"so",     "Software Offsets",SOFTOFF_CODE, 32 , 'N' ,2, SEND_EXPLICIT},
  {"qs",     "Query State", QUERY_STATE, 128 , 'N' ,2, SEND_USER_ONLY},
  {"sb",     "Stop Button", STOP_BUTTON, 1024 , 'N' ,2, SEND_EXPLICIT},
  {"ra",     "Ramp Amplitude", RAMP_AMPLITUDE_CODE, 0 , 'N' ,2, SEND_EXPLICIT},
  {"lb",     "Initial Blank Period", TRIAL_START_BLANK, 16 , 'N' ,2, SEND_EXPT},
//  {"ul",     "UpLoad Directory*", UPLOAD_CODE, 0 , 'N' ,2, SEND_EXPLICIT},
  {"vw",     "Vergence Window",VERGENCE_WINDOW, 8 , 'N' ,2, SEND_EXPLICIT},
  {"sp",     "Short Prem Trial Length",SHORT_PREM_CODE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"ed",     "electrode depth", ELECTRODE_DEPTH, 32 | EXPT_NOT_ALLOWED, 'N' ,2, SEND_USER_ONLY},
  {"cm",     "Comment", UFF_COMMENT, 128 , 'C' ,2, SEND_USER_ONLY},
  {"ss",     "Seed Shift/UC gap", SEED_SHIFT, 7 , 'N' ,0, SEND_EXPLICIT},
  {"cp",     "Conjugate Position",CONJUGATE_POSITION, 8 , 'N' ,2, SEND_EXPLICIT},
  {"pd",     "Deg2pix", DEG2PIX, 32 , 'N' ,2, SEND_EXPLICIT},
  {"yx",     "DRAGGED_POS",DRAGGED_POS, 1024 , 'N' ,2, SEND_EXPLICIT},
  {"o2",     "optionflag2",STIMULUS_OPTIONS, 32 , 'N' ,2, SEND_EXPLICIT},
  {"sq",     "H Saccade Size",SACCADE_VALUE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"to",     "WRONG_TIMEOUT",WRONG_TIMEOUT_CODE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"s0",     "STARTVAL", STARTVAL, 8 , 'N' ,2, SEND_EXPLICIT},
  {"sh",     "MAXSTAIRVAL",MAXSTAIRVAL, 8 , 'N' ,2, SEND_EXPLICIT},
  {"sm",     "MINSTAIRVAL",MINSTAIRVAL, 8 , 'N' ,2, SEND_EXPLICIT},
  {"js",     "STANDING_DISP",STANDING_DISP, 1 , 'N' ,4, SEND_EXPT},
  {"jc",     "Saccade Box size",SAC_FIX_CRIT, 8 , 'N' ,2, SEND_EXPLICIT},
  {"jt",     "SACTARGSIZE",SACTARGSIZE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"vs",     "V Saccade Size",VSACCADE_VALUE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"dd",     "DOT_DENSITY", DOT_DENSITY, 3, 'N' ,2, SEND_EXPT},
  {"Vs",     "VSR",VSR, 1 , 'N' ,2, SEND_EXPLICIT},
  {"cl",     "SEND_CLEAR",SEND_CLEAR, 128 , 'N' ,0, SEND_USER_ONLY},
  {"e2",     "Expt 2 Type",EXPTYPE_CODE2, 16 , 'C' ,0, SEND_EXPT},
  {"m2",     "Expt 2 Mean",EXPT2_MEAN, 16 , 'N' ,5, SEND_EXPT},
  {"e0",     "EXPTYPE_NONE",EXPTYPE_NONE, 16 , 'N' ,0, SEND_EXPLICIT},
  {"n2",     "Expt 2 Nstims",EXPT2_NSTIM, 16 , 'N' ,0, SEND_EXPT},
  {"i2",     "Expt 2 increment",EXPT2_INCR, 16 , 'N' ,5, SEND_EXPT},
  {"ac",     "ANTICORRELATED_DISPARITY",ANTICORRELATED_DISPARITY, 3+INDIRECT , 'N' ,4, SEND_EXPLICIT},
  {"cd",     "CORRELATED_DISPARITY",CORRELATED_DISPARITY, 3+INDIRECT, 'N' ,4, SEND_EXPLICIT},
  {"Dm",     "Distribution Mean",DISTRIBUTION_MEAN, 3 , 'N' ,2, SEND_EXPLICIT},
  {"np",     "N Planes",NPLANES , 3 , 'N' ,2, SEND_EXPLICIT},
  {"me",     "MONOCULARITY_EXPT",MONOCULARITY_EXPT, 128 , 'N' ,0, SEND_STIMULUS},
  {"cv",     "VERGENCE_CORRECTION",VERGENCE_CORRECTION, 8 , 'N' ,2, SEND_EXPLICIT},
  {"fh",     "FIXATION_SURROUND",FIXATION_SURROUND, 8 , 'N' ,2, SEND_EXPLICIT},
  {"bh",     "Back Size",BACK_SIZE, 64+INDIRECT , 'N' ,2, SEND_EXPT},
  {"fd",     "Fixation Overlap duration",FIXATION_OVERLAP, 8 , 'N' ,2, SEND_EXPLICIT},
  {"fr",     "P(reward)",PREWARD, 8 , 'N' ,2, SEND_EXPLICIT},
  {"aq",     "Acquision ON/OFF",ACQUISITION_CODE, 128 , 'N' ,2, SEND_EXPLICIT},
  {"rd",     "Relative Disparity",RELDISP, 512+1 , 'N' ,2, SEND_EXPLICIT},
  {"oL",     "ORI_LEFT",ORI_LEFT, 512+5 , 'N' ,2, SEND_EXPLICIT},
  {"oR",     "ORI_RIGHT",ORI_RIGHT, 512+5 , 'N' ,2, SEND_EXPLICIT},
  {"ve",     "Version",VERSION_CODE, 32 , 'C' ,2, SEND_EXPT},
  {"vve",     "Verg Version",VERGVERSION_CODE, 32 , 'C' ,2, SEND_EXPT},
  {"cb",     "Backround Correlation",BACK_CORRELATION, 64+INDIRECT, 'N' ,2 , SEND_EXPT},
  {"sd",     "Seed Delay",SEED_DELAY, 5 , 'N' ,0, SEND_EXPLICIT},
// {"xx",     "Trigger 1",TRIGGER_LEVEL1, 0 , 'N' ,2, SEND_EXPLICIT},
//  {"xx",     "Trigger 2",TRIGGER_LEVEL2, 0 , 'N' ,2, SEND_EXPLICIT},
//  {"xx",     "Trigger 3",TRIGGER_LEVEL3, 0 , 'N' ,2, SEND_EXPLICIT},
//  {"xx",     "Trigger 4",TRIGGER_LEVEL4, 0 , 'N' ,2, SEND_EXPLICIT},
  {"cs",     "RC seed",RC_SEED, 32 , 'N' ,0, SEND_STIMULUS},
  {"fn",     "frame repeat",FRAMEREPEAT, 16 , 'N' ,0, SEND_EXPT},
  {"cx",     "Change type",STIMCHANGE_CODE, 128 , 'C' ,0, SEND_EXPLICIT},
  {"fp",     "NewFixPos",FIXPOS_XY , 8 , 'N' ,2, SEND_EXPLICIT},
  {"Ro",     "RF Ori",RF_ANGLE, 128 , 'N' ,2, SEND_EXPT},
  {"Bs",     "BackStim",BACKSTIM_TYPE, 64 , 'C' ,0, SEND_EXPT},
    {"St3",     "Stim3 Type",STIM3_TYPE, 64 , 'C' ,0, SEND_EXPLICIT},
  {"Fs",     "FP Move Size",FP_MOVE_SIZE, 8 , 'N' ,4, SEND_NON_ZERO},
  {"Fa",     "FP Move Dir",FP_MOVE_DIR , 8 , 'N' ,2, SEND_EXPT},
  {"ip",     "Starting Phase",START_PHASE , 1 , 'N' ,2, SEND_EXPLICIT},
  {"bo",     "BackGround Ori",BACK_ORI , 64 , 'N' ,2, SEND_EXPT},
  {"Bt",     "BackGround TF",BACK_TF , 64 , 'N' ,2, SEND_NON_ZERO},
  {"Er",     "Early Reward Time",  EARLY_RWTIME , 8 , 'N' ,2, SEND_EXPLICIT},
  {"nx",     "N Extras",NEXTRAS_CODE , 16 , 'N' ,2, SEND_EXPT},
  {"mt",     "Manual TDR Entry",MANUAL_TDR , 128 , 'C' ,2, SEND_EXPLICIT},
  {"e3",     "Expt 3",EXPTYPE_CODE3, 8 , 'C' ,2, SEND_EXPT},
  {"mf",     "Modulation Frequency",MODULATION_F, 1 , 'N' ,2, SEND_EXPLICIT},
  {"zf",     "Disparity of Fixpt",FIXPT_DISPARITY, 8 , 'N' ,2, SEND_EXPLICIT},
  {"rv",     "Relatve Vdisp",RELVDISP, 512+3 , 'N' ,2, SEND_EXPLICIT},
  {"vm",     "Vdisp mod",VDISP_MOD, 3 , 'N' ,4, SEND_EXPLICIT},
  {"hd",     "Length Disparity",LENGTH_DISP, 3 , 'N' ,4, SEND_EXPLICIT},
  {"wd",     "Width Disparity",WIDTH_DISP, 3 , 'N' ,4, SEND_EXPLICIT},
  {"nc",     "N Components/Boundaries",NCOMPONENTS, 1 , 'N' ,2, SEND_EXPLICIT},
  {"id",     "Stim ID",STIMID, 1 , 'N' ,0, SEND_STIMULUS},
  {"O2",     "Ori 2",ORI2, 1 , 'N' ,2, SEND_EXPLICIT},
  {"n3",     "Nstim 3",EXPT3_NSTIM, 16 , 'N' ,0, SEND_EXPT},
  {"Sr",     "Spin Rate",SPINRATE, 1 , 'N' ,2, SEND_EXPLICIT},
  {"Bw",     "Back Width",BACK_WIDTH, 64 , 'N' ,4, SEND_EXPLICIT},
  {"Bh",     "Back Height",BACK_HEIGHT, 64 , 'N' ,4, SEND_EXPLICIT},
  {"xd",     "Dot X Pos",DOT_POSX, 1 , 'N' ,2, SEND_EXPLICIT},
  {"ob",     "Orientation Bandwidth",ORI_BANDWIDTH, 1 , 'N' ,0, SEND_EXPLICIT},
  {"Ri",     "Incr for FastSeq",RC1INC, 16 , 'N' ,4, SEND_EXPT},
  {"Dw",     "Distribution Width",DISTRIBUTION_WIDTH, 1 , 'N' ,2, SEND_EXPT},
  {"fH",     "Fixwin Height",FIXWIN_HEIGHT, 8 , 'N' ,2, SEND_EXPLICIT},
  {"i3",     "Incr 3",EXPT3_INCR, 16 , 'N' ,5, SEND_EXPLICIT},
  {"Dc",     "Distribution Conc",DISTRIBUTION_CONC, 1 , 'N' ,2, SEND_EXPLICIT},
  {"St",     "Saccade Threshold",SACCADE_THRESHOLD, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Sa",     "Saccade Detected",SACCADE_DETECTED, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Id",     "Initial Disparity",INITIAL_DISPARITY, 17 , 'N' ,4, SEND_EXPLICIT},
  {"vv",     "Vertical Vergence",VERTICAL_VERGENCE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"px",     "XPixel Size (cm)",XPIXEL_CODE, 32 , 'N' ,5, SEND_EXPT},
  {"py",     "YPIXEL_CODE",YPIXEL_CODE, 32 , 'N' ,5, SEND_EXPT},
  {"vd",     "VIEWD_CODE",VIEWD_CODE, 32 , 'N' ,2, SEND_EXPT},
  {"sP",     "Stim pulses",STIM_PULSES, 1 , 'N' ,2, SEND_EXPLICIT},
  {"pw",     "pulse width",PULSE_WIDTH, 1 , 'N' ,2, SEND_EXPT_NONZERO},
  {"pi",     "Pursuit Increment",PURSUIT_INCREMENT, 8 , 'N' ,4, SEND_EXPT},
  {"puA",     "Pursuit Amplitude",PURSUIT_AMPLITUDE, 8 , 'N' ,4, SEND_EXPT},
  {"puF",     "Pursuit Increment",PURSUIT_FREQUENCY, 8 , 'N' ,4, SEND_EXPT},
  {"USp",     "Microstim Phase Onset",MICROSTIM_PHASE_ONSET, 8 , 'N' ,4, SEND_EXPT},
   {"USd",     "Microstim Phase Duration",MICROSTIM_PHASE_DURATION, 8 , 'N' ,4, SEND_EXPT},
   {"USf",     "Periodic Microstim Frequncy",MICROSTIM_PERIODIC, 8 , 'N' ,4, SEND_EXPT},
  {"IB",     "Backgr Image",BACKGROUND_IMAGE, 0 , 'C' ,0 , SEND_EXPLICIT},
  {"Br",     "Fast Rpt B", FASTB_RPT, 16 , 'N' ,2, SEND_EXPLICIT},
//  {"bz",     "Backgr Zoom",BACKGROUND_ZOOM, 32 , 'N' ,2, SEND_EXPLICIT},
  {"rr",     "Rc repeats",RC_REPEATS, 16 , 'N' ,2, SEND_EXPLICIT},
  {"ap",     "AFC Proportion",AFC_PROPORTION, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Im",     "Initial Movement",INITIAL_MOVEMENT, 17 , 'N' ,2, SEND_EXPLICIT},
  {"bO",     "Backgr Ortho pos",BACK_OPOS,512+64 , 'N' ,2, SEND_EXPLICIT},
  {"bP",     "Backgr Para pos",BACK_PPOS, 512+64 , 'N' ,2, SEND_EXPLICIT},
  {"rb",     "Reward Bias",REWARD_BIAS, 8 , 'N' ,2, SEND_EXPT},
  {"Tx",     "Target X off",TARGET_XOFFSET , 16 , 'N' ,2, SEND_EXPLICIT},
  {"Ty",     "Target Y off",TARGET_YOFFSET , 16 , 'N' ,2, SEND_EXPLICIT},
  {"T2",     "Target 2 pos",TARGET2_POS , 16 , 'N' ,2, SEND_EXPLICIT},
  {"mixac",     "AC Mixture",MIXAC, 1 , 'N' ,2, SEND_EXPLICIT},
  {"ijump",     "Image Jumps",IMAGEJUMPS, 1 , 'N' ,0, SEND_EXPT},
    {"imx",     "Image X pos",IMAGEXPOS, 3 , 'N' ,0, SEND_EXPT},
    {"imy",     "Image Y pos",IMAGEYPOS, 3 , 'N' ,0, SEND_EXPT},
    
  {"Tt",     "Tone time",TONETIME, 16 , 'N' ,2, SEND_EXPLICIT},
  {"iD",  "Opposite Delay",OPPOSITE_DELAY , 1 , 'N' ,5, SEND_EXPLICIT},
    {"baddir",  "Saccade Punish Direcion",SACCADE_PUNISH_DIRECTION , 8 , 'N' ,1, SEND_EXPLICIT},
  {"Fr", "FAST_SEQUENCE rpt +1 ",FAST_SEQUENCE_RPT, 16 , 'N' ,2, SEND_EXPLICIT},
  {"db", "Wurtz Brightness Shift, +2 ",BRIGHTSHIFT_CODE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"tc", "Timeout color, +3",BLANKCOLOR_CODE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Dr",  "dot repeat ",DOTREPEAT, 1 , 'N' ,2, SEND_EXPLICIT},
  {"sa", "saccade amplitude ",SACCADE_AMPLITUDE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"fo", "Color when fixation off",FIX_OFF_CODE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"lo", "Logfile +7 ",LOGFILE_CODE, 128 , 'C' ,2, SEND_EXPLICIT},
  {"sc", "stop criterion +8 ", STOP_CRITERION, 128 , 'N' ,2, SEND_EXPLICIT},
  {"rl", "running average length +9 ", RUNAVERAGE_LENGTH, 0 , 'N' ,2, SEND_EXPLICIT},
  {"os", "SIDE_OFFSET",SIDE_OFFSET, 512 , 'N' ,2, SEND_EXPLICIT},
  {"eo", "END_OFFSET,",END_OFFSET, 512 , 'N' ,2, SEND_EXPLICIT},
  {"xr", "XMINSAMPLE_RATIO",XMINSAMPLE_RATIO, 1 , 'N' ,2, SEND_EXPLICIT},
  {"he", "helpfile ",HELPFILE_PATH, 128 , 'C' ,2, SEND_EXPLICIT},
  {"xm", "XMAXSAMPLE_RATIO",	XMAXSAMPLE_RATIO, 1 , 'N' ,2, SEND_EXPLICIT},
  {"Sv", "Stim Version + 15",STIMULUS_VERSION, 128 , 'N' ,2, SEND_STIMULUS},
  {"pe", "PENETRATION, +16 ",PENETRATION_TEXT, 128 , 'N' ,2, SEND_EXPLICIT},
  {"RP", "RF PARALELL ",RF_PARA, 512 , 'N' ,2, SEND_EXPLICIT},
  {"pl", "Size of Proximal Luminance Covariace ",PLC_MAG, 1 , 'N' ,2, SEND_EXPLICIT},
  {"To", "Target offset ",TARGET_OFFSET, 8 , 'N' ,2, SEND_EXPLICIT},
//  {"di", "DIRECTIONALIGY, +20 ",DIRECTION_TEXT, 128 , 'N' ,2, SEND_EXPLICIT},
//  {"ct", "CELL_TYPE, +21 ",CELLTYPE_TEXT , 128 , 'N' ,2, SEND_EXPLICIT},
  {"fl", "Stimulus Flag +22",STIMULUS_FLAG , 1 , 'C' ,2, SEND_EXPLICIT},
//  {"si", "swap interval +23 ",SWAPINTERVAL , 32 , 'N' ,2, SEND_EXPLICIT},
//  {"Tz", "Trapezoidal scaling for projectors ",TRAPEZOIDAL_SCALING , 32 , 'N' ,2, SEND_EXPLICIT},
  {"Rw", "RF Width +25 ",RF_WIDTH, 256 , 'N' ,2, SEND_EXPLICIT},
  {"Rh", "RF Height +26 ",RF_HEIGHT, 256 , 'N' ,2, SEND_EXPLICIT},
  {"Rx", "RF x +27 ",RF_X, 256 , 'N' ,2, SEND_EXPLICIT},
  {"Ry", "RF y +28 ",RF_Y, 256 , 'N' ,2, SEND_EXPLICIT},
  {"RO", "RF ORTHO ",RF_ORTHO, 512 , 'N' ,2, SEND_EXPLICIT},
  {"sg", "Spike Gain  ",SPIKE_GAIN, 0 , 'N' ,2, SEND_EXPLICIT},
//  {"Pw", "Plot Width ",PLOTW, 0 , 'N' ,2, SEND_EXPLICIT},
//  {"Ph", "Plot Height",PLOTH, 0 , 'N' ,2, SEND_EXPLICIT},
//  {"Px", "Plot X",PLOTX, 0 , 'N' ,2, SEND_EXPLICIT},
//  {"Py", "Plot Y",PLOTY, 0 , 'N' ,2, SEND_EXPLICIT},
  {"nT", "Trial Count",TRIAL_COUNT, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Tg", "good trials ", GOODTRIALS, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Tb", "bad trials ", BADTRIALS, 8 , 'N' ,2, SEND_EXPLICIT},
  {"dc", "disp X as cycle of sinewave ", DISP_AS_CYCLES, 1 , 'N' ,2, SEND_EXPLICIT},
  {"qe", "quickexp code ", QUICKEXPT_CODE, 128 , 'N' ,2, SEND_EXPLICIT},
  {"ga", "gamma correction val ", GAMMAVAL_CODE, 32 , 'N' ,2, SEND_EXPLICIT},
  {"lc", "CorLoop Exit crit", CORRECTION_LEAVE_CRIT, 8 , 'N' ,2, SEND_EXPLICIT},
  {"ec", "CorLoop Enter crit", CORRECTION_ENTRY_CRIT, 8 , 'N' ,2, SEND_EXPLICIT},
  {"pb", "p(Blank)", BLANK_P, 16, 'N' ,2, SEND_EXPLICIT},
  {"cc", "Covary Coefficient", COVARY_XPOS, 24, 'N' ,2, SEND_EXPLICIT},
  {"bl",     "BONUS_CRITERION", BONUS_CRITERION, 8, 'N' ,2, SEND_EXPLICIT},
  {"b2",     "BONUS2_CRITERION", BONUS2_CRITERION, 8, 'N' ,2, SEND_EXPLICIT},
  {"bs",     "EXPT_BLKSIZ", EXPT_BLKSIZ, 8, 'N' ,2, SEND_EXPLICIT}, 
  {"cL",     "CONTRAST_LEFT", CONTRAST_LEFT, 512+3, 'N' ,4, SEND_EXPLICIT},
  {"cR",    "CONTRAST_RIGHT",CONTRAST_RIGHT, 512+3 , 'N' ,4, SEND_EXPLICIT},
//  {"tb",     "Twobar sep",TWOBAR_SEP, 1 , 'N' ,2, SEND_EXPLICIT},
  {"Fl",     "Fixation Linw",FIX_LINW, 8 , 'N' ,2, SEND_EXPLICIT},
  {"cg",     "SETCLEARCOLOR",SETCLEARCOLOR, 32 , 'N' ,2, SEND_EXPLICIT},
  {"bj",     "BACKJUMP",BACKJUMP, 8 , 'N' ,2, SEND_EXPLICIT},
  {"lf",     "SF left",SF_LEFT, 512+3 , 'N' ,2, SEND_EXPLICIT},
  {"rf",     "SF Right",SF_RIGHT, 512+3 , 'N' ,2, SEND_EXPLICIT},
  {"rx",     "X randomization",RANDOM_X, 8 , 'N' ,2, SEND_EXPLICIT},
  {"ox",     "Ori randomization",RANDOM_ORI, 8 , 'N' ,2, SEND_EXPLICIT},
  {"bb",     "dOri F+B",DORI_BOTH , 3 , 'N' ,2, SEND_EXPLICIT},
  {"dk",     "dOri Back",DORI_BACK , 66 , 'N' ,2, SEND_EXPLICIT},
 // {"xc",     "Stim eccentricity",STIM_ECCENTRICITY , 513 , 'N' ,2, SEND_EXPLICIT},
//  {"pa",     "Polar Angle",STIM_POLARANGLE , 513 , 'N' ,2, SEND_EXPLICIT},
//  {"o0",    "Old Lori",OLDLEFT_ORI, 514 , 'N' ,2, SEND_EXPLICIT},
//  {"o1",    "Old Rori",OLDRIGHT_ORI, 514 , 'N' ,2, SEND_EXPLICIT},
//  {"xx",     "Cyberamp channels",CYBER_CHANNELS, 128 , 'N' ,2, SEND_EXPLICIT},
//  {"rs",     "RF Sign",RF_SIGN, 256 , 'N' ,2, SEND_EXPLICIT},
//  {"el",     "End Length",END_LENGTH, 512 , 'N' ,2, SEND_EXPLICIT},
  {"lw",     "Line Width for online plott",EXPT_LINW, 32 , 'N' ,2, SEND_EXPLICIT},
//  {"ps",     "Plot Smoothing for online RC",PLOTSMOOTH, 0 , 'N' ,2, SEND_EXPLICIT},
  {"pf",     "Flags Shown on panel",SHOWFLAGS_CODE, 128 , 'C' ,2, SEND_EXPLICIT},
  {"pt",     "Stimuli per fixation Trial",EXPT_STIMPERTRIAL, 8 , 'N' ,2, SEND_EXPLICIT},
//  {"ri",     "RC Correlation delay",RC_DELAY, 8 , 'N' ,2, SEND_EXPLICIT},
  {"cr",     "Contrast Ratio",CONTRAST_RATIO, 3 , 'N' ,2, SEND_EXPLICIT},
//  {"tx",     "Expt 2 x or + ",ADDEXPTS_CODE , 16 , 'N' ,2, SEND_EXPLICIT},
// {"ci",     "Command in",COMMAND_IN, 128 , 'C' ,2, SEND_EXPLICIT},
  {"gr",     "Grid Size",GRIDSIZE, 32 , 'N' ,2, SEND_EXPLICIT},
  {"dO",     "Orthog Disp",ORTHOG_DISP, 515 , 'N' ,2, SEND_EXPLICIT},
  {"dP",     "Paralell Disp",PARALELL_DISP, 515 , 'N' ,2, SEND_EXPLICIT},
//  {"sr",     "RDS/Sine",RDSXSINE, 512 , 'N' ,2, SEND_EXPLICIT},
  {"ns",     "Change Seed",CHANGE_SEED, 16 , 'N' ,2, SEND_EXPLICIT},
  {"d1",     "Disparity 1",DISP_A, 513 , 'N' ,2, SEND_EXPLICIT},
  {"d2",     "Disparity 2",DISP_B, 513 , 'N' ,2, SEND_EXPLICIT},
  {"Ff",     "FP Move Frame",FP_MOVE_FRAME, 8 , 'N' ,2, SEND_EXPLICIT},
  {"lX",     "Xpos (log)",XOFFLOG, 528 , 'N' ,2, SEND_EXPLICIT},
  {"mx",     "L-R-Binoc",LRBINTERLEAVE, 528 , 'N' ,2, SEND_EXPLICIT},
  {"LR",     "L-R-Monoc",LRINTERLEAVE, 528 , 'N' ,2, SEND_EXPLICIT},
  {"LB",     "L-Binoc",LBINTERLEAVE, 528 , 'N' ,2, SEND_EXPLICIT},
  {"RB",     "R-Binoc",RBINTERLEAVE, 528 , 'N' ,2, SEND_EXPLICIT},
  {"lS",     "SF-Lin",SFLIN, 528 , 'N' ,2, SEND_EXPLICIT},
  {"lT",     "TF-Lin",TFLIN, 528 , 'N' ,2, SEND_EXPLICIT},
  {"oS",     "Log Size",LOG_SIZE, 528 , 'N' ,2, SEND_EXPLICIT},
  {"m3",     "Expt 3 Mean",EXPT3_MEAN, 16 , 'N' ,2, SEND_EXPT},
  {"pU",     "p(Uncorr)",UNCORR_P , 16 , 'N' ,2, SEND_EXPLICIT},
    {"pS",  "p(Signal frame)", SIGNALFRAME_P, 16, 'N', 2, SEND_EXPLICIT},
  {"pT",     "p(One Target)",ONETARGET_P, 16 , 'N' ,2, SEND_EXPLICIT},
  {"sO",     "StimOrthog Pos",STIMORTHOG_POS, 513 , 'N' ,2, SEND_EXPLICIT},
  {"pR",     "Plaid Contrast Ratio",PLAID_RATIO, 1 , 'N' ,4, SEND_EXPLICIT},
  {"Al",     "Avg Len for eyepos auto-zero",AUTO_ZERO, 8 , 'N' ,2, SEND_EXPLICIT},
//  {"xx",     "Old Orthog disp",OLD_ORTHOG_DISP, 512 , 'N' ,2, SEND_EXPLICIT},
//  {"xx",     "Old Paralell disp",OLD_PARA_DISP, 512 , 'N' ,2, SEND_EXPLICIT},
  {"Op",     "Orthog Offset",ORTHOG_POS, 513 , 'N' ,2, SEND_EXPLICIT},
  {"Pp",     "Paralell Offset",PARA_POS, 513 , 'N' ,2, SEND_EXPLICIT},
  {"lv",    "Linear Velocity expt",LINEAR_SPEED, 528 , 'N' ,2, SEND_EXPLICIT},
  {"we",     "Weight",WEIGHT_TEXT, 128 , 'N' ,2, SEND_EXPLICIT},
  {"hr",     "Head Restrained",RESTRAIN_TEXT, 128 , 'N' ,2, SEND_EXPLICIT},
  {"r0",     "Rds x None back",RDSBNONE, 528 , 'N' ,2, SEND_EXPLICIT},
  {"Jt",     "jump Type",JUMPTYPE, 1 , 'N' ,2, SEND_EXPLICIT},
  {"tr",     "Target Ratio",TARGET_RATIO, 8 , 'N' ,2, SEND_EXPLICIT},
  {"hs",     "High SF val",HIGHSF, 16 , 'N' ,2, SEND_EXPLICIT},
  {"ht",     "High TF val",HIGHTF, 16 , 'N' ,2, SEND_EXPLICIT},
  {"cz",     "Contrast Pairs",CONTRAST_PAIRS, 528 , 'N' ,2, SEND_EXPLICIT},
//  {"sL",     "Stepper Plot Length",STEPPER_PLOTLEN, 32 , 'N' ,2, SEND_EXPLICIT},
  {"Nf",     "True Nframes",NFRAMES_DONE, 32 , 'N' ,2, SEND_EXPLICIT},
 // {"pC",     "Plot Cluster/Exp No.",PLOT_CLUSTER, 32 , 'N' ,2, SEND_EXPLICIT},
  {"r1",     "Reward 1",REWARD_SIZE1, 8 , 'N' ,2, SEND_EXPLICIT},
  {"r2",     "Reward 2",REWARD_SIZE2, 8 , 'N' ,2, SEND_EXPLICIT},
  {"r3",     "Reward 3",REWARD_SIZE3, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Tc",     "Timeout Contrast",TIMEOUT_CONTRAST, 8 , 'N' ,2, SEND_EXPLICIT},
  {"nR",     "RC File Counter",RCFILE_CTR, 0 , 'N' ,2, SEND_EXPLICIT},
  {"As",     "Alternative Stimulus",ALTERNATE_STIM_MODE, 16 , 'N' ,2, SEND_EXPLICIT},
  {"Us",     "Microstim Expt",MICROSTIM_EXPT, 16 , 'N' ,2, SEND_EXPLICIT},
  {"T1",     "Test value 1",TEST_VALUE1, 0 , 'N' ,2, SEND_EXPLICIT},
  {"TB",    "Test value 2",TEST_VALUE2, 0 , 'N' ,2, SEND_EXPLICIT},
  {"Ba",    "Size to add for background",BACK_ADD_SIZE, 64 , 'N' ,2, SEND_EXPLICIT},
//  {"xx",    "RC stimulus change",RC_STIMCHANGE, 0 , 'N' ,2, SEND_EXPLICIT},
  {"Pn",     "penetration number",PENNUMCOUNTER, 128 , 'N' ,2, SEND_EXPT},
  {  "Xp",     "Pen X",PENXPOS, 128 , 'N' ,2, SEND_EXPT},
  {  "Yp",    "Pen y",PENYPOS, 128 , 'N' ,2, SEND_EXPT},
  {"Vn",     "Visual Area",VWHERE, 128 , 'N' ,2, SEND_EXPLICIT},
  {"Is",     "Scale factor for IFC expts",IFCSCALE, 16 , 'N' ,2, SEND_EXPLICIT},
  {"hL",     "Height L",HEIGHT_L, 515 , 'N' ,2, SEND_EXPLICIT},
  {"hR",     "Height R",HEIGHT_R, 515 , 'N' ,2, SEND_EXPLICIT},
  {"wL",     "Width L",WIDTH_L, 515 , 'N' ,2, SEND_EXPLICIT},
  {"wR",     "Width R",WIDTH_R, 515 , 'N' ,2, SEND_EXPLICIT},
  {"SL",     "Size L",SIZE_L, 515 , 'N' ,2, SEND_EXPLICIT},
  {"SR",     "Size R",SIZE_R, 515 , 'N' ,2, SEND_EXPLICIT},
  {"Sd",     "Size Disparity",SIZE_DISP, 3, 'N' ,2, SEND_EXPLICIT},
  {"fm",     "FixPt Ocularity",FIXPT_EYE, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Sb",     "Sigma X and Y",SD_BOTH, 513 , 'N' ,2, SEND_EXPLICIT},
  {"uk",     "Uka Stair Coeffs",UKA_VALS, 8 , 'N' ,2, SEND_EXPLICIT},
//  {"Pr",     "Phase Disp as RelDisp",PHASE_AS_RELDISP, 513 , 'N' ,2, SEND_EXPLICIT},
//  {"PX",     "Panel X position",PANEL_XPOS, 0 , 'N' ,2, SEND_EXPLICIT},
  {"rc",     "Relative contrast",FB_RELATIVE_CONTRAST, 513 , 'N' ,2, SEND_EXPLICIT},
  {"by",     "Background Vertical Disp",BACK_VDISP, 64 , 'N' ,2, SEND_EXPLICIT},
//  {"PY",     "Panel Y position",PANEL_YPOS, 0 , 'N' ,2, SEND_EXPLICIT},
  {"sC",     "Counterphase",SET_COUNTERPHASE, 1 , 'N' ,2, SEND_EXPLICIT},
  {"ui",     "User id", USERID, 128 , 'C' ,2, SEND_EXPT},
  {"Tw",     "Time Ori BW",TEMPORAL_ORI_BANDWIDTH, 1 , 'N' ,2, SEND_EXPLICIT},
  {"ts",     "Shake Timeout",SHAKE_TIMEOUT_DURATION, 8 , 'N' ,2, SEND_EXPLICIT},
  {"Ol",     "Orthog Pos L",ORTHOG_POSL, 515 , 'N' ,2, SEND_EXPLICIT},
  {"Or",     "Orthog Pos R", ORTHOG_POSR, 515 , 'N' ,2, SEND_EXPLICIT},
  {"Covariate",     "Covariate",COVARIATE, 16 , 'C' ,2, SEND_EXPLICIT},
  {"iF",     "Initial Frames (Duration)",INITIAL_DURATION, 16 , 'N' ,2, SEND_EXPLICIT},
  {"e1max",     "Expt1 Max",EXPT1_MAXSIG, 16 , 'N' ,2, SEND_EXPLICIT},
  {"dS",     "FakeStim Expt", FAKESTIM_EXPT, 16 , 'N' ,0, SEND_EXPLICIT},
  {"FakeSig",     "FakeStim Signal", FAKESTIM_SIGNAL, 16 , 'N' ,4, SEND_EXPLICIT},
  {"pBlack",     "p(black dot)", BLACKDOT_FRACTION, 16 , 'N' ,2, SEND_EXPLICIT},
  {"hxtype",     "High X type",HIGHXTYPE, 16 , 'N' ,2, SEND_EXPLICIT},
  {"monkey", "monkey name (Directory)",MONKEYNAME, 128 , 'C' ,2, SEND_EXPLICIT},
  {"ar", "Aspect Ratio", ASPECT_RATIO, 513 , 'N' ,2, SEND_EXPLICIT},
//  {"Hi", "Hidden Codes", HIDDEN_OPTIONS, 128 , 'C' ,2, SEND_EXPLICIT},
  {"nl",     "N Low F Components", NLOWCOMPONENTS, 1 , 'N' ,2, SEND_EXPLICIT},
  {"nh",     "N High F Comonents", NHIGHCOMPONENTS, 1,  'N' ,2, SEND_EXPLICIT},
  {"ls",     "Final Seed",LAST_SEED, 1 , 'N' ,2, SEND_EXPLICIT},
  {"dfx",    "fx change", FIXPOS_XSHIFT, 8 , 'N' ,2, SEND_EXPLICIT},
  {"ddx",    "dx change",NEWDISP, 513 , 'N' ,2, SEND_EXPLICIT},
  {"backMov",    "Backgr Movie",BACKGROUND_MOVIE, 16 , 'N' ,2, SEND_EXPLICIT},
  {"xL",    "Xpos Left",LEFTX, 512 , 'N' ,3, SEND_EXPLICIT},
    {"yL",    "Ypos Left",LEFTY, 512 , 'N' ,3, SEND_EXPLICIT},
    {"xR",    "Xpos Right",RIGHTX, 512 , 'N' ,3, SEND_EXPLICIT},
    {"yR",    "Ypos Right",RIGHTY, 512 , 'N' ,3, SEND_EXPLICIT},
  {"annTyp",     "Annulus Type",SETANNULUS, 1 , 'N' ,2, SEND_EXPLICIT},
  {"StartDepth",     "Start Depth",PEN_START_DEPTH, 128 , 'N' ,2, SEND_EXPLICIT},
  {"TwoCylDisp",     "TwoCyl Disp",TWOCYL_DISP, 513 , 'N' ,2, SEND_EXPLICIT},
  {"rC",     "Random Contrasts",RANDOM_CONTRAST_EXPT, 528 , 'N' ,2, SEND_EXPLICIT},
  {"posinc",     "Position Displacement per frame",FRAME_DISPLACEMENT, 513 , 'N' ,2, SEND_EXPLICIT},
  {"aOp",     "absolute OrthoPos",ABS_ORTHOG_POS, 256+512 , 'N' ,2, SEND_EXPLICIT},
  {"aPp",     "absolute ParaPos",ABS_PARA_POS, 256+512, 'N' ,2, SEND_EXPLICIT},
  {"verbose", "Verbosity", VERBOSE_CODE, 128 , 'N' ,2, SEND_EXPLICIT},
    {  "seof", "Seed Offst", SEEDOFFSET, 0, 'N', 0, SEND_EXPLICIT},
    {  "serange", "Seed Range", SEEDRANGE, 0, 'N', 0, SEND_EXPLICIT},
    {  "nimplaces", "NumDigits for image name", NIMPLACES, 0, 'N', 0, SEND_EXPLICIT},
    {  "fixcolors", "Fixpt colors for special tasks", FIXCOLORS, 0, 'N', 0, SEND_NEVER},
    {  "usenewdirs", "Try different targe location rules", USENEWDIRS, 0, 'N', 0, SEND_NEVER},
    {  "choicedur", "Choice target persistance", CHOICE_TARGET_DURATION, 0, 'N', 2, SEND_NEVER},
    {  "cha", "Choice target Angle", FORCE_CHOICE_ANGLE, 0, 'N', 2, SEND_EXPT},
    {  "imi", "Image Index", IMAGEINDEX, 0, 'N', 0, SEND_EXPT},
    {  "adapter", "Adapter", CHAMBER_ADAPTER, 0, 'C', 0, SEND_EXPLICIT},
    {  "eZ", "Electrode Impedance", IMPEDANCE, 0, 'N', 0, SEND_EXPLICIT},
    {  "ePr", "Guide Tube Protrusion", PROTRUSION, 0, 'N', 0, SEND_EXPLICIT},
    {  "hemi", "HemiSphere", HEMISPHERE, 0, 'C', 0, SEND_EXPLICIT},
    {  "coarsemm", "Starting positino on Coarse drive", COARSEMM, 0, 'N', 0, SEND_EXPLICIT},
    {  "psyv", "Psych Value", PSYCH_VALUE, 0, 'N', 0, SEND_EXPLICIT},
    {  "expname", "Expt Name Label", EXPT_NAME, 0, 'C', 0, SEND_EXPT},
    {  "stimtag", "Stimulus Tag (label)", STIMULUS_TAG, 0, 'C', 0, SEND_EXPLICIT},
    {  "exp", "Expt Prefix (local dir)", EXPT_PREFIX, 0, 'C', 0, SEND_EXPLICIT},
    {  "nph", "Number of Random phases", NPHASES, 0, 'N', 0, SEND_EXPLICIT},
    {  "jcomp", "SF Components that jump",  JUMP_SF_COMPONENTS, 0, 'N', 0, SEND_EXPLICIT},
    {  "nsf", "SF Components", SET_SF_COMPONENTS, 0, 'N', 3, SEND_EXPLICIT},
    {  "ntf", "TF Components", SET_TF_COMPONENTS, 0, 'N', 3, SEND_EXPLICIT},
    {  "nco", "Component Contrasts", SET_SF_CONTRASTS, 0, 'N', 3, SEND_EXPLICIT},
    {  "psychmon", "Psych Mon Vars", PSYCHMON_VARS, 0, 'C', 3, SEND_EXPLICIT},
    {"impref", "prefix for image files", IMAGELOAD_PREFIX, 0, 'C', 0, SEND_EXPLICIT},
    {"Electrode", "Electrode Decsription", ELECTRODE_TYPE, 0, 'C', 0, SEND_EXPLICIT},
    {"onlineprefix", "Folder for online files on Mac", ONLINEPREFIX, 0, 'C', 0, SEND_NEVER},
    {"cmdfile", "File for remote commands", COMMAND_FILE, 0, 'C', 0, SEND_NEVER},
    {"psychfile", "Psych results file", PSYCHFILE, 0, 'C', 0, SEND_NEVER},
    {"impref", "prefix for image files", IMAGELOAD_PREFIX, 0, 'C', 0, SEND_EXPLICIT},
    {"immode", "image mode", IMAGELOAD_MODE, 0, 'C', 0, SEND_EXPLICIT},
    {"imload", "image load type", IMAGELOAD_TYPE,  0, 'C', 0, SEND_EXPLICIT},
    {"monitor","Monitor Filename",  MONITOR_FILE, 0, 'C', 0, SEND_EXPLICIT},
    {"ereset","File To run for Reset between Expts",  EXPTRESET_FILE, 0, 'C', 0, SEND_EXPLICIT},
    {"seqpause","Pause between computer repeated blocks",  SEQUENCE_PAUSE, VERGONLY, 'N', 2, SEND_NEVER},
    { NULL, NULL, -1, -1 ,0 ,0, 0},
};



StringCode commstrings[] = {
//   {"uf","DataFile name",  UFF_PREFIX, NULL},
//    {"monitor","Monitor Filename",  MONITOR_FILE, NULL},
//    {"lo", "Log File", LOGFILE_CODE, NULL},
//    {"st", "Stimulus", STIMULUS_TYPE_CODE, NULL},
//    {"monkey", "Monkey name", MONKEYNAME, NULL},
//    {"impref", "prefix for image files", -1, NULL},
//    {"immode", "image mode", -1, NULL},
//    {"imload", "image load type", -1, NULL},
//    {"psychfile", "Psych results file", -1, NULL},
    //    {"cmdfile", "File for remote commands", COMMAND_FILE, NULL},
    //    {"electrode", "Electrode Decsription", ELECTRODE_TYPE, NULL},
    {NULL, NULL, 0, NULL}
};

char **serial_strings;
char **serial_names;

char *mode_names[] = {
    "Balenced Dot Distribution",
    "Fixed Random Dot Distribution",
    "Frame Random Dot Dist",
    "Un-normalized nsines",
    "Random Power nsines",
    "4 Bit Noise",
    "Random Component Nsines",
    "Random Log Component Nsines",
    "Indep Component Nsines",
    "Indep Log Component Nsines",
    "Binoc Indep Log Nsines",
    "Indep Nsines 3 Contrasts",
    "Binoc Indep Sines (Lin)",
    "Four Phases",
    "Two Phases",
    "Remove Back Surface",
    "Flat Cylinder",
    "Normal Cylinder",
    "Zero Dispartiy BackCyl",
    "Parellel AC",
    "Orthog AC",
    "Parallel UC",
    "Orthog UC",
    "Orthog Lines Off",
    "TwoCyl AND Idisp",
    "RDS-sl1/RLS-sl0",
    NULL
};

int codesend[MAXTOTALCODES] = {
	SEND_EXPLICIT};
char *jumpnames[] = {
    "Fx Para",
    "Fx Orhto",
    "St Para",
    "St Ortho",
    "St Alter",
    NULL};


char *jumpstrings[] = {
    "fp", //
    "fr", //fix return
    "sm", //stim move
    "sr", //stim return
    "sa", //stim altered
    NULL};

int nfplaces[MAXTOTALCODES] = {2};

char *stimulus_names[N_STIMULUS_TYPES] = 
{
	"none" ,
	"gabor",
	"rds" ,
	"grating",
	"bar",
    "circle",
	"rectangle",
    "test",
    "square",
    "probe",
    "2grating", 
    "cylinder",
    "corrug",
    "sqcorrug",
    "twobar",
    "rls",
    "annulus",
    "rdssine",
    "nsines",
    "rlssine",
    "radial",
    "image",
    "checker",
};


char *channel_strings[] = {
	"fs",
	"co",
	"sm",
	"dt",
	NULL
};



#else
extern char **serial_strings;
extern char *channel_strings[];
extern char *stimulus_names[];
extern char *bwtoggle_codes[];
extern StringCode commstrings[];
extern ToggleCode togglestrings[];
extern ValueCode valstrings[];
extern char *mode_names[];
extern int nfplaces[MAXTOTALCODES];
extern int codesend[];
extern char **serial_names;
extern char *jumpnames[];
#endif


#define EYESCALE_CODE 0
#define EYECOLOR_CODE 1
#define SMOOTHING_CODE 2
#define DDT_CODE 3
#define EYEOFFSET_CODE 4

/* bits set in the PC version of optionflag */

#define CHECK_LEFT 1
#define CHECK_RIGHT (1<<1)
#define CHECK_CONJUGATE (1<<2)
#define FIXATION_CHECK_IO (CHECK_LEFT | CHECK_RIGHT | CHECK_CONJUGATE)
#define VERGENCE_CHECK_IO (1<<3)
#define STORE_WURTZ_DATA (1<<4)
#define CONJUGATE_RAMP_BIT (1<<5)
#define CLAMP_HOLD (1<<6)
#define CLAMP_OPTION (1<<7)
#define RAMP_HOLD (1<<8)
#define RAMP_OPTION (1<<9)
#define FRAME_TEST_BIT (1<<10)
#define SEND_CONJPOS (1<<12)
#define STIMULUS_CHANGED (1<<13)
#define STIMULUS_FLASHING (1<<14)
#define AFC_SET (1<<15)/*j*/

/* bits for 2nd PC optionflag */
#define PREBACK_BIT 1
#define RC_BIT 2
#define TIMES_EXPT2 (1<<2) // N.B matches definition in stimdefs.h
#define BINOC_FIX_BIT (1<<3)
#define CONTRAST_REVERSE_BW (1<<4)
#define RANDOM_PHASE_BW (1<<5)
#define MICROSTIM_BIT (1<<6)
#define RUN_SEQUENCE_BW (1<<7)
#define STIM_BETWEEN_TRIALS_BW (1<<8)
#define STORE_LFP_BW (1<<9)

#define SHOW_EACH_STIM 1
#define SHOW_ON_LIST (1<<1)
#define SHOW_IF_SET (1<<2)
#define SHOW_FOR_CORRUG (1<<3)
#define SHOW_FOR_DOTSTIM (1<<4)
#define SHOW_FOR_CYLINDER (1<<5)
#define SHOW_ALWAYS (1<<6)
#define SHOW_FOR_2GRATING (1<<7)
#define SHOW_FOR_RADIAL (1<<8)
#define SHOW_FOR_GABOR (1<<9)

/*
 * Bits set int TheStim->flag (Unix) and in stimflag (DOS)
 */

#define CONTRAST_POSITIVE 1
#define CONTRAST_NEGATIVE (1<<1)
#define CROSSED_SEED (1<<2)
#define ANTICORRELATE (1<<3)
#define STIMULUS_IS_SQUARE (1<<4)
#define UNCORRELATE (1<<5)
#define LASTRDSFLAG 5

/*j cylinder properties*/
#define DIRECTION_L (1<<(LASTRDSFLAG+1))
#define FIXED_DOTNUM (1<<(LASTRDSFLAG+2))
#define FLAT_DISPARITY (1<<(LASTRDSFLAG+3))
#define FLAT_SURFACES (1<<(LASTRDSFLAG+4))
#define TRACK_DOT (1<<(LASTRDSFLAG+5))
#define TEXTURE (1<<(LASTRDSFLAG+6))
#define CARDS (1<<(LASTRDSFLAG+7))
#define COUNTDOWN (1<<(LASTRDSFLAG+8))
#define CYL_PLC (1<<(LASTRDSFLAG+9))
#define FRONT_ONLY (1<<(LASTRDSFLAG+10))
#define NO_WRAP (1<<(LASTRDSFLAG+11))
#define HOLD_PLANE (1<<(LASTRDSFLAG+12))
#define BACK_ONLY (1<<(LASTRDSFLAG+13))
#define FRONT_SURFACE_OCCLUDES (1<<(LASTRDSFLAG+14))
#define RDS_WRAP (1<<(LASTRDSFLAG+15))

#define NUM_STIM_FLAGS (LASTRDSFLAG+16)



#define SU_TDR_TYPE_CANCEL_REP 16
#define SU_TDR_TYPE_START_WURTZ 17
#define SU_TDR_TYPE_END_STIM 18
#define SU_TDR_TYPE_MANUAL 19
#define SU_TDR_TYPE_ERROR 20
#define RIGHTH 0
#define LEFTH 1
#define RIGHTV 2
#define LEFTV 3
#define LFPCHAN 4 //N.B. notcounted in NCRCHAN!

#define NCRCHANNELS 4 /* actual number CR channels sampled */
#define VGC_STIM NCRCHANNELS
#define CJG_STIM NCRCHANNELS+1

#define NDATACHANNELS (NCRCHANNELS +2)
#define VERGENCE_POSN NDATACHANNELS
#define CONJUGATE_POSN NDATACHANNELS+1
#define VERGENCE_VERT NDATACHANNELS+2
#define CONJUGATE_VERT NDATACHANNELS+3
#define XYCHAN NDATACHANNELS+4 /* 10 */
#define NDISPLAYCHANNELS (NDATACHANNELS +4) 
/* no toggles for the "vergence limit" display */

#define TOGGLE_OFFSET XYCHAN
#define XY_RIGHT (1L<<(TOGGLE_OFFSET))
#define XY_LEFT (1L<<(TOGGLE_OFFSET+1))
#define XY_CONJUGATE (1L<<(TOGGLE_OFFSET+2))
#define XY_PERSIST (1L<<(TOGGLE_OFFSET+3))
#define XYGRATICULE (1L<<(TOGGLE_OFFSET+4))
#define TIMEGRID (1L<<(TOGGLE_OFFSET+5))
#define AUTO_SPIKE_CLEAR TIMEGRID
#define YGRID (1L<<(TOGGLE_OFFSET+6))
#define XY_NOWRAP YGRID
#define SHOW_FIXWIN (1L<<(TOGGLE_OFFSET+7))

#define AUTOCHANS (TOGGLE_OFFSET+8)
#define VERGENCE_LIMIT_UP (AUTOCHANS)
#define VERGENCE_LIMIT_DOWN (AUTOCHANS+1)
#define TOTALCHANS (AUTOCHANS+2)

#define val_affects_fixn(x) (x == MONOCULARITY_EXPT || x == STATIC_VERGENCE || x == STATIC_CONJUGATE || x == FIXPOS_X || x== FIXPOS_Y)

// Ali #define VERSION_NUMBER "5.15" /* binoc version */
#define MAXFRAMES 500


/*
 * Jump types for FP move/simulate
 */

#define JUMP_FP_PARA 0
#define JUMP_FP_ORTHO 1
#define JUMP_STIM_PARA 2
#define JUMP_STIM_ORTHO 3
#define JUMP_STIM_CHANGE 4
#define NJUMPTYPES 5



#endif



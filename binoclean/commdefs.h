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
#define LAST_STIMULUS_CODE 41


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

#define MAXSERIALCODES (LAST_STIMULUS_CODE+158) /* 199 */
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
#define PLOTW MAXSERIALCODES+31
#define PLOTH MAXSERIALCODES+32
#define PLOTX MAXSERIALCODES+33
#define PLOTY MAXSERIALCODES+34
#define TRIAL_COUNT MAXSERIALCODES+35
#define GOODTRIALS MAXSERIALCODES +36
#define BADTRIALS MAXSERIALCODES +37
#define DISP_AS_CYCLES MAXSERIALCODES +38
#define QUICKEXPT_CODE MAXSERIALCODES +39
#define GAMMAVAL_CODE MAXSERIALCODES +40
#define CORRECTION_LEAVE_CRIT MAXSERIALCODES +41
#define CORRECTION_ENTRY_CRIT MAXSERIALCODES +42
#define BLANK_P MAXSERIALCODES +4
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
#define PLOTSMOOTH MAXSERIALCODES+68
#define SHOWFLAGS_CODE MAXSERIALCODES+69
#define EXPT_STIMPERTRIAL MAXSERIALCODES+70
#define RC_DELAY MAXSERIALCODES+71
#define ISI_SMOOTHPERIOD MAXSERIALCODES+72
#define CONTRAST_RATIO MAXSERIALCODES+73
#define ADDEXPTS_CODE  MAXSERIALCODES+74
#define COMMAND_IN MAXSERIALCODES+75
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
#define STEPPER_PLOTLEN MAXSERIALCODES+114
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
#define PHASE_AS_RELDISP MAXSERIALCODES+144
#define PANEL_XPOS MAXSERIALCODES+145
#define FB_RELATIVE_CONTRAST MAXSERIALCODES+146
#define BACK_VDISP MAXSERIALCODES+147
#define PANEL_YPOS MAXSERIALCODES+148
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
#define MAXSAVECODES MAXSERIALCODES+163 //362

#define ASPECT_RATIO MAXSAVECODES
#define HIDDEN_OPTIONS MAXSAVECODES+1
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

#define MAXTOTALCODES (MAXSAVECODES+16)  //372


/*
 *  MAXTOTALCODES is the total number of values with string codes
 * anything not needing a string (e.g. EARLY_RWSIZE) is > MAXTOTALCODES
 */


/* 
 * some redefinitions to use some spare variables for things only
 * adjusted in psychophysics
 */

#define PSYCH_EXPNO PLOT_CLUSTER
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
#define IMPEDANCE  MAXTOTALCODES +62
#define PROTRUSION  MAXTOTALCODES +63
#define USERNAME  MAXTOTALCODES +64
#define EXPTSTRING MAXTOTALCODES +65
#define OTBW MAXTOTALCODES+66
#define ODPD MAXTOTALCODES+67
#define EARLY_RWSIZE MAXTOTALCODES+68
#define RESET_TRIALS MAXTOTALCODES +69
#define ELECTRODE_TYPE MAXTOTALCODES +70
#define END_TIMEOUT MAXTOTALCODES +71
#define TOTALCODES MAXTOTALCODES+72
/*
 *  TOTALCODES is the total numer of vals in Expt 
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

typedef struct valuecode {
    char *code;
    char *label;
    int icode;
    int group;
  char ctype; //'C' if this is a string
    
} ValueCode;

#define MONITOR_FILE 0


#ifdef CHARCODES_DEFINE

#define NCODES MAXTOTALCODES



StringCode commstrings[] = {
    {"monitor","Monitor Filename",  MONITOR_FILE, NULL},
    {"lo", "Log File", LOGFILE_CODE, NULL},
    {"st", "Stimulus", STIMULUS_TYPE_CODE, NULL},
    {"monkey", "Monkey name", MONKEYNAME, NULL},
    {"impref", "prefix for image files", 0, NULL},
    {"immode", "image mode", 0, NULL},
    {NULL, NULL, 0, NULL}
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
 *         8 fixation point 
 *         16 Expt setup
 *         32 General Display
 *         64 Background stim
 *         128 commands
 *         256 RF properties
 *         512 Indirect properties
 */
ValueCode valstrings[] = {
  {"xo","X offset",  XPOS, 1, 'N'},
    {"yo","Y offset",  YPOS, 1, 'N'},
  {"lo", "Log File", LOGFILE_CODE, 2, 'N'},
  {"st", "Stimulus", STIMULUS_TYPE_CODE, 1,'C'},
  {"sx",     "Sigma X",  SD_X, 1, 'N'},
  {"sy",     "Sigma Y",  SD_Y, 1, 'N'},
  {"dp",     "Diparate phase", DISP_P, 3, 'N'},
  {"dx",     "Disparity ", DISP_X, 3, 'N'},
  {"wi",     "Width ", STIM_WIDTH, 1, 'N'},
  {"hi",     "Height ", STIM_HEIGHT, 1, 'N'},
  {"sf",     "Spatial Frequency", SF,  1, 'N'},
  {"xn",     "X sampling", XSAMPLES, 1, 'N'},
  {"yn",     "Y Sampling", YSAMPLES, 1, 'N'},
  {"tf",     "Temporal Frequency", TF, 1, 'N'},
  {"ph",     "Spatial Phase",  SETPHASE, 1, 'N'},
  {"or",     "Orientation",  ORIENTATION, 1, 'N'},
  {"sl",     "Seed Loop", SET_SEEDLOOP, 5, 'N'},
  {"se",     "Seed", SET_SEED, 5, 'N'},
  {"co",     "Contrast ", SETCONTRAST, 1, 'N'},
  {"dy",     "VDisp", DISP_Y, 3, 'N'},
  {"f2",     "SF2", SF2, 1, 'N'},
  {"t2",     "TF2", TF2, 1, 'N'},
  {"p2",     "Phase2", PHASE2, 1, 'N'},
  {"a2",     "Plaid angle", PLAID_ANGLE, 1, 'N'},
  {"jv",     "J velocity", JVELOCITY, 5, 'N'},
  {"fi",     "front intensity",JF_INTENSITY, 5, 'N'},
  {"bi",     "back intensity", JB_INTENSITY, 5, 'N'},
  {"jn",     "cyl num dots", JNUMDOTS, 5, 'N'},
  {"jl",     "dotlife (cyl)",JLIFEFRAMES, 5, 'N'}, 
  {"jx",     "death chance",  JDEATH, 5, 'N'},
  {"c2",     "contrast 2", CONTRAST2, 1, 'N'},
  {"ic",     "delta contrast", CONTRAST_DIFF, 3, 'N'},
  {"od",     "delta ori", ORIENTATION_DIFF, 3, 'N'},
  {"ds",     "delta SF", SF_DIFF, 3, 'N'},
  {"dg",     "disp grad", DEPTH_MOD, 3, 'N'},
  {"dm",     "depth modulation", DISP_GRAD, 3, 'N'},
  {"ta",     "twobar angle", TWOBAR_ANGSEP, 1, 'N'},
  {"dr",     "Disp Ramp", DISP_RAMP, 3, 'N'},
  {"dq",     "pDisp 2", PHASE2DISP, 3, 'N'},
  {"rp",     "Rel Phase",  PHASEREL, 3, 'N'},
  {"Pd",     "Phase Disp as Disp", PHASE_AS_DISP, 3, 'N'},
  {"ce",     "Correlation", CORRELATION, 3, 'N'},
  {"dw",     "Dot Width", DOT_SIZE, 5, 'N'},
  {"fc",     "Fixation Color", SETFIXCOLOR, 8, 'N'},
  {"zx",     "Zoom",  SETZOOM, 32, 'N'},
  {"bc",     "BackGround Color ", SETBACKCOLOR, 32, 'N'},
  {"dt",     "Delay", DELAY, 32 , 'N'},
  {"pr",     "PREPERIOD", PREPERIOD_CODE, 16, 'N'}, 
  {"et",     "Expt1 Type",  EXPTYPE_CODE, 16 , 'N'},
  {"fz",     "FRAMERATE",  FRAMERATE_CODE, 32 , 'N'},
  {"du",     "STIMULUS DURATION", STIMULUS_DURATION_CODE, 16 , 'N'},
  {"vc",     "CLAMP_DISPARITY", CLAMP_DISPARITY_CODE, 0 , 'N'},
  {"po",     "POSTPERIOD", POSTPERIOD_CODE, 16 , 'N'},
  {"mo",     "Mode",MODE_CODE, 0 , 'N'},
  {"vr",     "RAMP_VERGENCE",RAMP_VERGENCE_CODE, 0 , 'N'},
  {"nt",     "N Stim (Expt 1)",NTRIALS_CODE, 32 , 'N'},
  {"uf",     "Uff Prefix",UFF_PREFIX, 32 , 'N'},
  {"hx",     "High X Value for Interleaves",HIGHX, 16 , 'N'},
  {"sM",     "Stimulus mode",STIMULUS_MODE, 1 , 'N'},
  {"wr",     "WURTZ task response period", WURTZ_RT_CODE, 8 , 'N'},
  {"mD",     "Max disp Initial manip", INITIAL_APPLY_MAX, 17 , 'N'},
  {"op",     "optionflag setup", OPTION_CODE, 16 , 'N'},
  {"nr",     "N Repetitions", NREPS_CODE, 16 , 'N'},
  {"tl",     "trigger level", TRIGGER_LEVEL, 0 , 'N'},
  {"fw",     "fixation window", FIXATION_WINDOW, 8 , 'N'},
  {"df",     "display flag",DISPLAYFLAG_CODE, 0 , 'N'},
  {"ch",     "channel Set code",CHANNEL_CODE, 32 , 'N'},
  {"sv",     "Static Vergence",STATIC_VERGENCE, 0 , 'N'},
  {"wf",     "mean fixation period",WURTZ_DURATION_CODE, 8 , 'N'},
  {"bt",     "Replay End",UFF_TIME, 0 , 'N'},
  {"Bc",     "Background Contrast",BACK_CONTRAST, 64 , 'N'},
  {"ei",     "Expt incr", EXPT_INCR, 16 , 'N'},
  {"em",     "Expt Mean", EXPT_MEAN, 16 , 'N'},
  {"is",     "ISI", ISI_CODE, 16 , 'N'},
  {"bw",     "bin width", UNIT_BINWIDTH, 0 , 'N'},
  {"ub",     "binned unit data",BINNED_SPIKE_DATA, 0 , 'N'},
  {"ut",     "unit times",SPIKE_TIMES, 0 , 'N'},
  {"nf",     "N Frames",NFRAMES_CODE, 16 , 'N'},
  {"bd",     "backgound disparity", DISP_BACK, 64 , 'N'},
  {"rk",     "ramp coefficient", RAMP_COEFFICIENT, 0 , 'N'},
  {"fx",     "fixation x pos ",FIXPOS_X, 8 , 'N'},
  {"fy",     "fixation y pos ",FIXPOS_Y, 8 , 'N'},
  {"fs",     "fixation size ",SETFIXSIZE, 8 , 'N'},
  {"cj",     "mean conjugate",STATIC_CONJUGATE, 8 , 'N'},
  {"rw",     "Reward Size", REWARD_SIZE, 8 , 'N'},
  {"vh",     "Vergence RAMP and Hold", RAMP_HOLD_CODE, 0 , 'N'},
  {"ti",     "TIMEOUT",  TIMEOUT_CODE, 8 , 'N'},
  {"so",     "Software Offsets 62 ",SOFTOFF_CODE, 32 , 'N'},
  {"qs",     "Query State", QUERY_STATE, 128 , 'N'},
  {"sb",     "Stop Button", STOP_BUTTON, 128 , 'N'},
  {"ra",     "Ramp Amplitude", RAMP_AMPLITUDE_CODE, 0 , 'N'},
  {"lb",     "Initial Blank Period", TRIAL_START_BLANK, 16 , 'N'},
  {"ul",     "UpLoad Directory*", UPLOAD_CODE, 0 , 'N'},
  {"vw",     "Vergence Window",VERGENCE_WINDOW, 8 , 'N'},
  {"sp",     "Short Prem Trial Length",SHORT_PREM_CODE, 8 , 'N'},
  {"ed",     "electrode depth", ELECTRODE_DEPTH, 32 , 'N'},
  {"cm",     "Comment", UFF_COMMENT, 128 , 'N'},
  {"ss",     "Seed Shift/UC gap", SEED_SHIFT, 7 , 'N'},
  {"cp",     "Conjugate Position",CONJUGATE_POSITION, 8 , 'N'},
  {"pd",     "Deg2pix", DEG2PIX, 32 , 'N'},
  {"yx",     "DRAGGED_POS",DRAGGED_POS, 0 , 'N'},
  {"o2",     "optionflag2",STIMULUS_OPTIONS, 32 , 'N'},
  {"sq",     "H Saccade Size",SACCADE_VALUE, 8 , 'N'},
  {"to",     "WRONG_TIMEOUT",WRONG_TIMEOUT_CODE, 8 , 'N'},
  {"s0",     "STARTVAL",STARTVAL, 8 , 'N'},
  {"sh",     "MAXSTAIRVAL",MAXSTAIRVAL, 8 , 'N'},
  {"sm",     "MINSTAIRVAL",MINSTAIRVAL, 8 , 'N'},
  {"js",     "STANDING_DISP",STANDING_DISP, 1 , 'N'},
  {"jc",     "Saccade Box size",SAC_FIX_CRIT, 8 , 'N'},
  {"jt",     "SACTARGSIZE",SACTARGSIZE, 8 , 'N'},
  {"vs",     "V Saccade Size",VSACCADE_VALUE, 8 , 'N'},
  {"dd",     "DOT_DENSITY", DOT_DENSITY, 3, 'N'},
  {"Vs",     "VSR",VSR, 1 , 'N'},
  {"cl",     "SEND_CLEAR",SEND_CLEAR, 128 , 'N'},
  {"e2",     "Expt 2 Type",EXPTYPE_CODE2, 16 , 'N'},
  {"m2",     "Expt 2 Mean",EXPT2_MEAN, 16 , 'N'},
  {"e0",     "EXPTYPE_NONE",EXPTYPE_NONE, 16 , 'N'},
  {"n2",     "Expt 2 Nstims",EXPT2_NSTIM, 16 , 'N'},
  {"i2",     "Expt 2 increment",EXPT2_INCR, 16 , 'N'},
  {"ac",     "ANTICORRELATED_DISPARITY",ANTICORRELATED_DISPARITY, 3 , 'N'},
  {"cd",     "CORRELATED_DISPARITY",CORRELATED_DISPARITY, 3 , 'N'},
  {"Dm",     "Distribution Mean",DISTRIBUTION_MEAN, 3 , 'N'},
  {"np",     "NPLANES",NPLANES , 3 , 'N'},
  {"me",     "MONOCULARITY_EXPT",MONOCULARITY_EXPT, 128 , 'N'},
  {"cv",     "VERGENCE_CORRECTION",VERGENCE_CORRECTION, 8 , 'N'},
  {"fh",     "FIXATION_SURROUND",FIXATION_SURROUND, 8 , 'N'},
  {"bh",     "Back Size",BACK_SIZE, 64 , 'N'},
  {"fd",     "Fixation Overlap duration",FIXATION_OVERLAP, 8 , 'N'},
  {"fr",     "P(reward)",PREWARD, 8 , 'N'},
  {"aq",     "Acquision ON/OFF",ACQUISITION_CODE, 128 , 'N'},
  {"rd",     "Relative Disparity",RELDISP, 512+1 , 'N'},
  {"oL",     "ORI_LEFT",ORI_LEFT, 512+5 , 'N'},
  {"oR",     "ORI_RIGHT",ORI_RIGHT, 512+5 , 'N'},
  {"ve",     "Version",VERSION_CODE, 32 , 'C'},
  {"sz",     "Size (H and W)",STIM_SIZE, 1 , 'N'},
  {"cb",     "Backround Correlation",BACK_CORRELATION, 64 , 'N'},
  {"sd",     "Seed Delay",SEED_DELAY, 5 , 'N'},
  {"xx",     "Trigger 1",TRIGGER_LEVEL1, 0 , 'N'},
  {"xx",     "Trigger 2",TRIGGER_LEVEL2, 0 , 'N'},
  {"xx",     "Trigger 3",TRIGGER_LEVEL3, 0 , 'N'},
  {"xx",     "Trigger 4",TRIGGER_LEVEL4, 0 , 'N'},
  {"cs",     "RC seed",RC_SEED, 32 , 'N'},
  {"fn",     "frame repeat",FRAMEREPEAT, 16 , 'N'},
  {"cx",     "Change type",STIMCHANGE_CODE, 128 , 'N'},
  {"fp",     "NewFixPos",FIXPOS_XY , 8 , 'N'},
  {"Ro",     "RF Ori",RF_ANGLE, 128 , 'N'},
  {"Bs",     "BackStim",BACKSTIM_TYPE, 64 , 'C'},
  {"Fs",     "FP Move Size",FP_MOVE_SIZE, 8 , 'N'},
  {"Fa",     "FP Move Dir",FP_MOVE_DIR , 8 , 'N'},
  {"ip",     "Starting Phase",START_PHASE , 1 , 'N'},
  {"bo",     "BackGround Ori",BACK_ORI , 64 , 'N'},
  {"Bt",     "BackGround TF",BACK_TF , 64 , 'N'},
  {"Er",     "Early Reward Time",  EARLY_RWTIME , 8 , 'N'},
  {"nx",     "N Extras",NEXTRAS_CODE , 16 , 'N'},
  {"mt",     "Manual TDR Entry",MANUAL_TDR , 128 , 'N'},
  {"e3",     "Expt 3",EXPTYPE_CODE3, 8 , 'N'},
  {"mf",     "Modulation Frequency",MODULATION_F, 1 , 'N'},
  {"zf",     "Disparity of Fixpt",FIXPT_DISPARITY, 8 , 'N'},
  {"rv",     "Relatve Vdisp",RELVDISP, 512+3 , 'N'},
  {"vm",     "Vdisp mod",VDISP_MOD, 3 , 'N'},
  {"hd",     "Length Disparity",LENGTH_DISP, 3 , 'N'},
  {"wd",     "Width Disparity",WIDTH_DISP, 3 , 'N'},
  {"nc",     "N Components",NCOMPONENTS, 1 , 'N'},
  {"id",     "Stim ID",STIMID, 1 , 'N'},
  {"O2",     "Ori 2",ORI2, 1 , 'N'},
  {"n3",     "Nstim 3",EXPT3_NSTIM, 16 , 'N'},
  {"Sr",     "Spin Rate",SPINRATE, 1 , 'N'},
  {"Bw",     "Back Width",BACK_WIDTH, 64 , 'N'},
  {"Bh",     "Back Height",BACK_HEIGHT, 64 , 'N'},
  {"xd",     "Dot X Pos",DOT_POSX, 1 , 'N'},
  {"ob",     "Orientation Bandwidth",ORI_BANDWIDTH, 1 , 'N'},
  {"Ri",     "Incr for FastSeq",RC1INC, 16 , 'N'},
  {"Dw",     "Distribution Width",DISTRIBUTION_WIDTH, 1 , 'N'},
  {"fH",     "Fixwin Height",FIXWIN_HEIGHT, 8 , 'N'},
  {"i3",     "Incr 3",EXPT3_INCR, 16 , 'N'},
  {"Dc",     "Distribution Conc",DISTRIBUTION_CONC, 1 , 'N'},
  {"St",     "Saccade Threshold",SACCADE_THRESHOLD, 8 , 'N'},
  {"Sa",     "Saccade Detected",SACCADE_DETECTED, 8 , 'N'},
  {"Id",     "Initial Disparity",INITIAL_DISPARITY, 17 , 'N'},
  {"vv",     "Vertical Vergence",VERTICAL_VERGENCE, 8 , 'N'},
  {"px",     "XPixel Size (cm)",XPIXEL_CODE, 32 , 'N'},
  {"py",     "YPIXEL_CODE",YPIXEL_CODE, 32 , 'N'},
  {"vd",     "VIEWD_CODE",VIEWD_CODE, 32 , 'N'},
  {"sP",     "Stim pulses",STIM_PULSES, 1 , 'N'},
  {"pw",     "pulse width",PULSE_WIDTH, 1 , 'N'},
  {"pi",     "Pursuit Increment",PURSUIT_INCREMENT, 8 , 'N'},
  {"IB",     "Backgr Image",BACKGROUND_IMAGE, 16 , 'N'},
  {"Br",     "Fast Rpt B", FASTB_RPT, 16 , 'N'},
  {"bz",     "Backgr Zoom",BACKGROUND_ZOOM, 32 , 'N'},
  {"rr",     "Rc repeats",RC_REPEATS, 16 , 'N'},
  {"ap",     "AFC Proportion",AFC_PROPORTION, 8 , 'N'},
  {"Im",     "Initial Movement",INITIAL_MOVEMENT, 17 , 'N'},
  {"bO",     "Backgr Ortho pos",BACK_OPOS, 64 , 'N'},
  {"bP",     "Backgr Para pos",BACK_PPOS, 64 , 'N'},
  {"rb",     "Reward Bias",REWARD_BIAS, 8 , 'N'},
  {"Tx",     "Target X off",TARGET_XOFFSET , 16 , 'N'},
  {"Ty",     "Target Y off",TARGET_YOFFSET , 16 , 'N'},
  {"T2",     "Target 2 pos",TARGET2_POS , 16 , 'N'},
  {"mixac",     "AC Mixture",MIXAC, 1 , 'N'},
  {"Tt",     "Tone time",TONETIME, 16 , 'N'},
  {"iD",  "Opposite Delay",OPPOSITE_DELAY , 1 , 'N'},
  {"Fr", "FAST_SEQUENCE rpt +1 ",FAST_SEQUENCE_RPT, 16 , 'N'},
  {"db", "Wurtz Brightness Shift, +2 ",BRIGHTSHIFT_CODE, 8 , 'N'},
  {"tc", "Timeout color, +3",BLANKCOLOR_CODE, 8 , 'N'},
  {"Dr",  "dot repeat ",DOTREPEAT, 1 , 'N'},
  {"sa", "saccade amplitude ",SACCADE_AMPLITUDE, 8 , 'N'},
  {"fo", "Color when fixation off",FIX_OFF_CODE, 8 , 'N'},
  {"lo", "Logfile +7 ",LOGFILE_CODE, 128 , 'N'},
  {"sc", "stop criterion +8 ", STOP_CRITERION, 128 , 'N'},
  {"rl", "running average length +9 ", RUNAVERAGE_LENGTH, 0 , 'N'},
  {"os", "SIDE_OFFSET +10 ",SIDE_OFFSET, 256 , 'N'},
  {"eo", "END_OFFSET, +11 ",END_OFFSET, 256 , 'N'},
  {"xr", "XMINSAMPLE_RATIO, +12 ",XMINSAMPLE_RATIO, 1 , 'N'},
  {"he", "helpfile ",HELPFILE_PATH, 128 , 'N'},
  {"xm", "XMAXSAMPLE_RATIO, +14 ",	XMAXSAMPLE_RATIO, 1 , 'N'},
  {"Sv", "Stim Version + 15",STIMULUS_VERSION, 128 , 'N'},
  {"pe", "PENETRATION, +16 ",PENETRATION_TEXT, 128 , 'N'},
  {"RP", "RF PARALELL ",RF_PARA, 256 , 'N'},
  {"pl", "Size of Proximal Luminance Covariace ",PLC_MAG, 1 , 'N'},
  {"To", "Target offset ",TARGET_OFFSET, 8 , 'N'},
  {"di", "DIRECTIONALIGY, +20 ",DIRECTION_TEXT, 128 , 'N'},
  {"ct", "CELL_TYPE, +21 ",CELLTYPE_TEXT , 128 , 'N'},
  {"fl", "Stimulus Flag +22",STIMULUS_FLAG , 1 , 'N'},
  {"si", "swap interval +23 ",SWAPINTERVAL , 32 , 'N'},
  {"Tz", "rapezoidal scaling for projectors ",TRAPEZOIDAL_SCALING , 32 , 'N'},
  {"Rw", "RF Width +25 ",RF_WIDTH, 256 , 'N'},
  {"Rh", "RF Height +26 ",RF_HEIGHT, 256 , 'N'},
  {"Rx", "RF x +27 ",RF_X, 256 , 'N'},
  {"Ry", "RF y +28 ",RF_Y, 256 , 'N'},
  {"RO", "RF ORTHO +29",RF_ORTHO, 256 , 'N'},
  {"sg", "Spike Gain +30 ",SPIKE_GAIN, 0 , 'N'},
  {"Pw", "Plot Width +31 ",PLOTW, 0 , 'N'},
  {"Ph", "Plot Height +32 ",PLOTH, 0 , 'N'},
  {"Px", "Plot X +33 ",PLOTX, 0 , 'N'},
  {"Py", "Plot Y +34 ",PLOTY, 0 , 'N'},
  {"nT", "Trial Count",TRIAL_COUNT, 8 , 'N'},
  {"Tg", "good trials +36 ", GOODTRIALS, 8 , 'N'},
  {"Tb", "bad trials +36 ", BADTRIALS, 8 , 'N'},
  {"dc", "disp X as cycle of sinewave ", DISP_AS_CYCLES, 1 , 'N'},
  {"qe", "quickexp code ", QUICKEXPT_CODE, 128 , 'N'},
  {"ga", "gamma correction val ", GAMMAVAL_CODE, 32 , 'N'},
  {"lc", "CorLoop Exit crti ", CORRECTION_LEAVE_CRIT, 8 , 'N'},
  {"pb", "p(Blank)", BLANK_P, 16, 'N'}, 
  {"cc", "Covary Coefficient", COVARY_XPOS, 24, 'N'}, 
  {"bl",     "BONUS_CRITERION", BONUS_CRITERION, 8, 'N'},
  {"b2",     "BONUS2_CRITERION", BONUS2_CRITERION, 8, 'N'},
  {"bs",     "EXPT_BLKSIZ", EXPT_BLKSIZ, 8, 'N'}, 
  {"c0",     "CONTRAST_LEFT", CONTRAST_LEFT, 512+3, 'N'}, 
  {"c1",    "CONTRAST_RIGHT",CONTRAST_RIGHT, 512+3 , 'N'},
  {"tb",     "Twobar sep",TWOBAR_SEP, 1 , 'N'},
  {"Fl",     "Fixation Linw",FIX_LINW, 8 , 'N'},
  {"cg",     "SETCLEARCOLOR",SETCLEARCOLOR, 32 , 'N'},
  {"bj",     "BACKJUMP",BACKJUMP, 8 , 'N'},
  {"lf",     "SF left",SF_LEFT, 512+3 , 'N'},
  {"rf",     "SF Right",SF_RIGHT, 512+3 , 'N'},
  {"rx",     "X randomization",RANDOM_X, 8 , 'N'},
  {"ox",     "Ori randomization",RANDOM_ORI, 8 , 'N'},
  {"bb",     "dOri F+B",DORI_BOTH , 3 , 'N'},
  {"dk",     "dOri Back",DORI_BACK , 66 , 'N'},
  {"xc",     "Stim eccentricity",STIM_ECCENTRICITY , 513 , 'N'},
  {"pa",     "Polar Angle",STIM_POLARANGLE , 513 , 'N'},
  {"o0",    "Old Lori",OLDLEFT_ORI, 0 , 'N'},
  {"o1",    "Old Rori",OLDRIGHT_ORI, 0 , 'N'},
  {"xx",     "Cyberamp channels",CYBER_CHANNELS, 128 , 'N'},
  {"rs",     "RF Sign",RF_SIGN, 256 , 'N'},
  {"el",     "End Length",END_LENGTH, 256 , 'N'},
  {"lw",     "Line Width for online plott",EXPT_LINW, 32 , 'N'},
  {"ps",     "Plot Smoothing for online RC",PLOTSMOOTH, 0 , 'N'},
  {"pf",     "Flags Shown on panel",SHOWFLAGS_CODE, 128 , 'C'},
  {"pt",     "Stimuli per fixation Trial",EXPT_STIMPERTRIAL, 8 , 'N'},
  {"ri",     "RC Correlation delay",RC_DELAY, 8 , 'N'},
  {"cr",     "Contrast Ratio",CONTRAST_RATIO, 3 , 'N'},
  {"tx",     "Expt 2 x or + ",ADDEXPTS_CODE , 16 , 'N'},
  {"ci",     "Command in",COMMAND_IN, 128 , 'N'},
  {"gr",     "Grid Size",GRIDSIZE, 32 , 'N'},
  {"dO",     "Orthog Disp",ORTHOG_DISP, 515 , 'N'},
  {"dP",     "Paralell Disp",PARALELL_DISP, 515 , 'N'},
  {"sr",     "RDS/Sine",RDSXSINE, 512 , 'N'},
  {"ns",     "Change Seed",CHANGE_SEED, 16 , 'N'},
  {"d1",     "Disparity 1",DISP_A, 513 , 'N'},
  {"d2",     "Disparity 2",DISP_B, 513 , 'N'},
  {"Ff",     "FP Move Frame",FP_MOVE_FRAME, 8 , 'N'},
  {"lX",     "Xpos (log)",XOFFLOG, 528 , 'N'},
  {"mx",     "L-R-Binoc",LRBINTERLEAVE, 528 , 'N'},
  {"LR",     "L-R-Monoc",LRINTERLEAVE, 528 , 'N'},
  {"LB",     "L-Binoc",LBINTERLEAVE, 528 , 'N'},
  {"RB",     "R-Binoc",RBINTERLEAVE, 528 , 'N'},
  {"lS",     "SF-Lin",SFLIN, 528 , 'N'},
  {"lT",     "TF-Lin",TFLIN, 528 , 'N'},
  {"oS",     "Log Size",LOG_SIZE, 528 , 'N'},
  {"m3",     "Expt 3 Mean",EXPT3_MEAN, 16 , 'N'},
  {"pU",     "p(Uncorr)",UNCORR_P , 16 , 'N'},
  {"pT",     "p(One Target)",ONETARGET_P, 16 , 'N'},
  {"sO",     "StimOrthog Pos",STIMORTHOG_POS, 513 , 'N'},
  {"pR",     "Plaid Contrast Ratio",PLAID_RATIO, 1 , 'N'},
  {"Al",     "Avg Len for eyepos auto-ze{ro",AUTO_ZERO, 8 , 'N'},
  {"xx",     "Old Orthog disp",OLD_ORTHOG_DISP, 0 , 'N'},
  {"xx",     "Old Paralell disp",OLD_PARA_DISP, 0 , 'N'},
  {"Op",     "Orthog Offset",ORTHOG_POS, 513 , 'N'},
  {"Pp",     "Paralell Offset",PARA_POS, 513 , 'N'},
  {"lv",    "Linear Velocity expt",LINEAR_SPEED, 528 , 'N'},
  {"we",     "Weight",WEIGHT_TEXT, 128 , 'N'},
  {"hr",     "Head Restrained",RESTRAIN_TEXT, 128 , 'N'},
  {"r0",     "Rds x None back",RDSBNONE, 528 , 'N'},
  {"Jt",     "jump Type",JUMPTYPE, 1 , 'N'},
  {"tr",     "Target Ratio",TARGET_RATIO, 8 , 'N'},
  {"hs",     "High SF val",HIGHSF, 16 , 'N'},
  {"ht",     "High TF val",HIGHTF, 16 , 'N'},
  {"cz",     "Contrast Pairs",CONTRAST_PAIRS, 528 , 'N'},
  {"sL",     "Stepper Plot Length",STEPPER_PLOTLEN, 32 , 'N'},
  {"Nf",     "True Nframes",NFRAMES_DONE, 32 , 'N'},
  {"pC",     "Plot Cluster/Exp No.",PLOT_CLUSTER, 32 , 'N'},
  {"r1",     "Reward 1",REWARD_SIZE1, 8 , 'N'},
  {"r2",     "Reward 2",REWARD_SIZE2, 8 , 'N'},
  {"r3",     "Reward 3",REWARD_SIZE3, 8 , 'N'},
  {"Tc",     "Timeout Contrast",TIMEOUT_CONTRAST, 8 , 'N'},
  {"nR",     "RC File Counter",RCFILE_CTR, 0 , 'N'},
  {"As",     "Alternative Stimulus",ALTERNATE_STIM_MODE, 16 , 'N'},
  {"Us",     "Microstim Expt",MICROSTIM_EXPT, 16 , 'N'},
  {"T1",     "Test value 1",TEST_VALUE1, 0 , 'N'},
  {"TB",    "Test value 2",TEST_VALUE2, 0 , 'N'},
  {"Ba",    "Size to add for background",BACK_ADD_SIZE, 64 , 'N'},
  {"xx",    "RC stimulus change",RC_STIMCHANGE, 0 , 'N'},
  {"Pn",     "penetration number",PENNUMCOUNTER, 128 , 'N'},
  {  "Xp",     "Pen X",PENXPOS, 128 , 'N'},
  {  "Yp",    "Pen y",PENYPOS, 128 , 'N'},
  {"Vn",     "Visual Area",VWHERE, 128 , 'N'},
  {"Is",     "Scale factor for IFC expts",IFCSCALE, 16 , 'N'},
  {"hL",     "Height L",HEIGHT_L, 515 , 'N'},
  {"hR",     "Height R",HEIGHT_R, 515 , 'N'},
  {"wL",     "Width L",WIDTH_L, 515 , 'N'},
  {"wR",     "Width R",WIDTH_R, 515 , 'N'},
  {"SL",     "Size L",SIZE_L, 515 , 'N'},
  {"SR",     "Size R",SIZE_R, 515 , 'N'},
  {"Sd",     "Size Disparity",SIZE_DISP, 3, 'N'},
  {"fm",     "FixPt Ocularity",FIXPT_EYE, 8 , 'N'},
  {"Sb",     "Sigma X and Y",SD_BOTH, 513 , 'N'},
  {"uk",     "Uka Stair Coeffs",UKA_VALS, 8 , 'N'},
  {"Pr",     "Phase Disp as RelDisp",PHASE_AS_RELDISP, 513 , 'N'},
  {"PX",     "Panel X position",PANEL_XPOS, 0 , 'N'},
  {"rc",     "Relative contrast",FB_RELATIVE_CONTRAST, 513 , 'N'},
  {"by",     "Background Vertical Disp",BACK_VDISP, 64 , 'N'},
  {"PY",     "Panel Y position",PANEL_YPOS, 0 , 'N'},
  {"sC",     "Counterphase",SET_COUNTERPHASE, 1 , 'N'},
  {"ui",     "User id",USERID, 128 , 'C'},
  {"Tw",     "Time Ori BW",TEMPORAL_ORI_BANDWIDTH, 1 , 'N'},
  {"ts",     "Shake Timeout",SHAKE_TIMEOUT_DURATION, 8 , 'N'},
  {"Ol",     "Orthog Pos L",ORTHOG_POSL, 515 , 'N'},
  {"Or",     "Orthog Pos R", ORTHOG_POSR, 515 , 'N'},
  {"Covariate",     "Covariate",COVARIATE, 16 , 'C'},
  {"iF",     "Initial Frames (Duration)",INITIAL_DURATION, 16 , 'N'},
  {"e1max",     "Expt1 Max",EXPT1_MAXSIG, 16 , 'N'},
  {"dS",     "FakeStim Expt", FAKESTIM_EXPT, 16 , 'N'},
  {"FakeSig",     "FakeStim Signal", FAKESTIM_SIGNAL, 16 , 'N'},
  {"pBlack",     "p(black dot)", BLACKDOT_FRACTION, 16 , 'N'},
  {"hxtype",     "High X type",HIGHXTYPE, 16 , 'N'},
  {"monkey", "monkey name (Directory)",MONKEYNAME, 128 , 'N'},
  {"ar", "Aspect Ratio", ASPECT_RATIO, 513 , 'N'},
  {"Hi", "Hidden Codes", HIDDEN_OPTIONS, 128 , 'N'},
  {"nl",     "N Low F Components", NLOWCOMPONENTS, 1 , 'N'},
  {"nh",     "N High F Comonents", NHIGHCOMPONENTS, 1,  'N'},
  {"ls",     "Final Seed",LAST_SEED, 1 , 'N'},
  {"dfx",    "fx change", FIXPOS_XSHIFT, 8 , 'N'},
  {"ddx",    "dx change",NEWDISP, 513 , 'N'},
  {"backMov",    "Backgr Movie",BACKGROUND_MOVIE, 16 , 'N'},
  {"annTyp",     "Annulus Type",SETANNULUS, 1 , 'N'},
  {"StartDepth",     "Start Depth",PEN_START_DEPTH, 128 , 'N'},
  {"TwoCylDisp",     "TwoCyl Disp",TWOCYL_DISP, 513 , 'N'},
  {"rC",     "Random Contrasts",RANDOM_CONTRAST_EXPT, 528 , 'N'},
  {"posinc",     "Position Displacement per frame",FRAME_DISPLACEMENT, 513 , 'N', 'N'},
  {"aOp",     "absolute OrthoPos",ABS_ORTHOG_POS, 256+512 , 'N'},
  {"aPp",     "absolute ParaPos",ABS_PARA_POS, 0 , 256+512, 'N'},
  {"verbose", "Verbosity", VERBOSE_CODE, 128 , 'N'},
  { NULL, NULL, -1, -1 ,0},
};

char *serial_strings[NCODES+1] = {

	  "xo",
        "yo",
        "st",
	"sx", /* Gabor, Sigma X 3*/
	"sy", /* Sigma Y 4*/
	"dp", /* Diparate phase 5*/
	"dx", /* Disparity 6*/
	"wi", /* Width */
	"hi", /* Height */
	"sf", /* Spatial Frequency 9 */
	"xn", /* X sampling 10 */
	"yn", /* Y Sampling, 11  */
	"tf", /* Temporal Frequency, 12 */
	"ph", /* Spatial Phase, 13 */
	"or", /* Oreintation, 14 */
	"sl", /*Seed Loop 15 */
	"se", /* SEED ,16*/
	"co", /* Contrast, 17 */
	"dy", /* Veritcal Disparity 18 */
	"f2", /* SF2, 19 */
	"t2", /* TF2, 20 */
	"p2", /* PhaseF2, 21 */
	"a2", /* PLAID_ANGLE, 22 */
	"jv", /* jvelocity (angular velocity) 23*/
	"fi", /* jfront intensity 24*/
	"bi", /* jback intensity 25*/
	"jn", /* jnum dots 26*/
	"jl", /* jlife frames 27*/
	"jx", /* death chance 28*/
	"c2", /* contrast 2 29 */
	"ic", /* interocular contrast diff, 30 */
	"od", /* delta ori, 31 */
	"ds", /* delta SF, 32*/
	"dg", /* depth modulation */
	"dm", /* disparity gradient */
	"ta", /* two bar angularseparation */
	"dr", /* Dipsarity Ramp */
	"dq", /* phase disparity 2 */
	"rp",  /* relative phase of components in 2 grating */
	"Pd",  /* disparity, applied in phase (wraps) */
	"ce", /* correlation */
	"dw", /* Dot Size, 100 */
	"fc", /* Fixation Color, LAST_STIM +1 */
	"zx", /* Zoom, 31 */
	"bc", /* BackGround Color ,32*/
	"dt", /* Delay 33 */
	"pr", /* PREPERIOD, 21 */
	"et", /* Expt Type, 29 */
	"fz", /* FRAMERATE, 23 */
	"du", /* STIMULUS DURATION, 24 */
	"vc", /* CLAMP_DISPARITY, 25 */
	"po", /* POSTPERIOD, 26 */
	"mo", /* Mode */
	"vr", /* RAMP_VERGENCE, 28 */
	"nt",  /* NTRIALS, 22 */
	"uf", /* Uff Prefix, 30 */
    "hx", /* High X value for interleaving */
	"sM", /* stimulus mode */
	"wr", /* WURTZ task response period, 34 */
	"mD", /* max dispartiy for initial disp manipulation, 35 */
	"op", /* optionflag setup, 36 */
	"nr", /* NREPS_CODE, 37 */
	"tl", /* trigger level, 38 */
	"fw", /* fixation window, 39 */
	"df", /*display flag 40 */
	"ch", /*Channel Set code 41 */
	"sv", /*Static Vergence 42 */
	"wf", /* mean fixation period, 33 */
	"bt", /* UFF time 44 */
	"Bc", /* BACKGROUND_CONTRAST, 45 */
	"ei", /* Expt incr, 46 */
	"em", /* Expt Mean, 47 */
	"is", /* ISI, 48 */
	"bw", /* bin width 49 */
	"ub", /* binned unit data 50 */
	"ut", /* unit times 51 */
	"nf", /* N  Frames 52 */
	"bd", /* backgound disparity, 53 */
	"rk", /* ramp coefficient, 54 */
	"fx", /* fixation x pos */
	"fy", /* fixation x pos */
	"fs", /* fixation size */
	"cj", /* static conjugate, 58 */
	"rw", /* Reward Size, 59 */
	"vh", /* Vergence RAMP and Hold, 60 */
	"ti", /* TIMEOUT 61 */
	"so", /* Software Offsets 62 */
	"qs", /* Query State, 63 */
	"sb", /* Stop Button, 65 */
	"ra", /* Ramp Amplitude, 66 */
	"lb", /* Length of Blank at trial start */
	"ul", /* UpLoad Directory, 68 */
	"vw", /* Vergence Window 69 */ 
	"sp", /* Short Prem Trial Lenght, 70 */
	"ed", /* electrode depth, 71 */
	"cm", /* Comment, 72 */
	"ss", /* Seed Shift, 73 */
	"cp", /* Conjugate Position 74 */
	"pd", /* DEG2PIX 75 */
	"yx", /* Postion changes 77 */
	"o2", /* Stimulus option flag, for BW */
	"sq", /* saccade value 83*/
	"to", /* wrong timeout 91*/
	"s0", /* stair startval 92 */
	"sh", /*  stair max ((h)igh) val 93 */
	"sm", /* stair min(m) val 94 */
	"js", /* standing disparity deg 95*/
	"jc", /* saccade fixation criterion (deg) 96*/
	"jt", /* saccade target size 97*/
	"vs", /* vertical saccade val 98*/
	"dd", /* dot density, 99*/
    "Vs", /* VSR */
	"cl", /* Clear Screen, 64 */
	"e2", /* Extype 2 */
    "m2", /* mean2 */
    "e0", /* no expt */
    "n2", /* nstim2 */
    "i2", /* incr2 */
    "ac", /* anticorrelated expt */
    "cd", /* correlated disparity*/
    "Dm", /* Distribution Mean*/
    "np", /* NPLANES*/
    "me", /* monocularity Experiment */
    "cv", /* vergence Correction */
    "fh", /* fixation surround hole*/
    "bh", /* Background size */
    "fd", /* Fixation overlap duration */
    "fr", /* fraction rewarded */
    "aq", /*acquisition ON/OFF */
    "rd", /* relative disparity */
    "oL", /* LEFT ORI */
    "oR", /* RIGHT ORI */
    "ve", /*version number */
    "sz", /* STIM_SIZE +24 */
    "cb", /* Background Correlation +24 */
    "sd", /* Seed Delay */
    "xx", /* Trigger level 1 */
    "xx", /* Trigger level 2 */
    "xx", /* Trigger level 3 */
    "xx", /* Trigger level 4 */
    "cs", /* Seed for reverse correlation */
    "fn", /* frame repeat value for reverse correlation */
    "cx", /* stimchange code */
    "fp", /* fixpos, x and y for change in trial */
    "Ro", /* RF Ori */
    "Bs", /* Back Stim */
    "Fs", /* FP move size */
    "Fa", /* FP move angle*/
    "ip", /* Initial Phase*/
	"bo", /*background orientation */
	"Bt", /*background TF */
	"Er", /* Early Reward */
	"nx", /* N extras */
	"mt", /* Manual TDR */
	"e3", /* Expt Type 3 */
	"mf", /* Modulation Frequency */
	"zf", /* Disparity for fixpt */
	"rv", /* Vertical Reldisp */
	"vm", /* Vertical Depth Mod */
	"hd", /* Height disparity */
	"wd", /* Width disparity */
	"nc", /* N Components */
	"id", /* Stim id */
	"O2", /* Ori 2 +24 */
	"n3", /* Nstim Expt 3 */
	"Sr", /* Spinrate */
	"Bw", /* Background Width */
	"Bh", /* Back height */
	"xd", /* X displacement for dot pattern */
	"ob", /* Orientation Bandwidth */
	"Ri", /* Increment to use for RC1 expts with variable distribution */
	"Dw", /* Distribution Width */
	"fH", /* Fixwin H */
	"i3", /* incr3 */
    "Dc", /* Distribution Concentration */
    "St", /* Saccade Threshold */
    "Sa", /* Saccade Detected */
    "Id", /* Initial Disparity */
    "vv", /* Vertical vergence */
    "px", /* pixel x +4*/
    "py", /* pixel y +5*/
    "vd", /* viewing disatnce + 1*/
	"sP", // Stim Pulses
	"pw", // pulse width
    "pi", /* pursuit increment */
    "IB", /* Image dir for backgrounds */
    "Br", /* background frame rpt */
    "bz", /* Zoom for back images */
    "rr", /* RC repeats - # of times to repeat each seed */
    "ap", /* AFC proportion*/
    "Im", /* Initial Movement */
    "bO", /* backgr Opos */
    "bP", /* backgr Ppos */
    "rb", /* Reward Bias*/
    "Tx", /* Target X Offset */
    "Ty", /* Target Y Offset */
    "T2", /* Target 2 direction(file), position (spike2) */
    "mixac", /* mix corr, AC, dots */
    "Tt", /* Tonetime */
"iD", /* Opposite Delay */
"Fr", /* FAST_SEQUENCE rpt +1 */
"db", /* Wurtz Brightness Shift, +2 */
"tc", /* Timeout color, +3*/
"Dr",  /* dot repeat */
"sa", /* saccade amplitude */
"fo", /* Color when fixation "off"  +6*/
"lo", /* Logfile +7 */
"sc", /* stop criterion +8 */ 
"rl", /* running average length +9 */ 
"os", /* SIDE_OFFSET +10 */
"eo", /* END_OFFSET, +11 */
"xr", /* XMINSAMPLE_RATIO, +12 */
"he", /* helpfile */
"xm", /* XMAXSAMPLE_RATIO, +14 */	
"Sv", /* Stim Version + 15*/
"pe", /* PENETRATION, +16 */
"RP", /* RF PARALELL */
"pl", /* Size of Proximal Luminance Covariace */
"To", /* Target offset */
"di", /* DIRECTIONALIGY, +20 */
"ct", /* CELL_TYPE, +21 */
"fl", /* Stimulus Flag +22*/
"si", /* swap interval +23 */
"Tz", /*trapezoidal scaling for projectors */
"Rw", /* RF Width +25 */
"Rh", /* RF Height +26 */
"Rx", /* RF x +27 */
"Ry", /* RF y +28 */
"RO", /* RF ORTHO +29*/
"sg", /* Spike Gain +30 */
"Pw", /* Plot Width +31 */
"Ph", /* Plot Height +32 */
"Px", /* Plot X +33 */
"Py", /* Plot Y +34 */
"nT", /* Trial Count*/
"Tg", /* good trials +36 */
"Tb", /* bad trials +36 */
"dc", /* disp X as cycle of sinewave */
"qe", /* quickexp code */
"ga", /* gamma correction val */
"lc", /* monocularity Experiment */
"ec", /* monocularity Experiment */
"pb",
"cc", /* contrast/xpos covariance */
"bl", /* bonus level */
"b2", /* bonus level 2*/
"bs", /* Blocksize for expt runs */
"c0", /* left contrast */
"c1",
"tb", /* two bar sep */
"Fl", /* Fixationn Line Width */
"cg", /* clear greylevel */
"bj", /* background jump period*/
"lf", /* Left SF */
"rf", /* Right SF */
"rx", /* random x position */
"ox", /* random x position */
"bb", /* dori both */
"dk", /* dori back */
"xc", /* Stimulus eccentricity */
"pa", /* Polar Angle */
"o0",
"o1",
"xx", /* cyber channels */
"rs", /* RF Sign */
"el", /* End Length */
"lw", /* Line Width for online plots */
"ps", /* Plot Smoothing for Online */
"pf", /*Flags shown */
"pt", /* stim per trial */
"ri", /* RC Correlation delay interval*/
"ib", /* isi smoothing */
"cr", /* contrast ratio */
"tx", /* Times/Add Extp2 */
"ci", /*command in */
"gr", /* Grid size */
"dO", /* orhtog disp */
"dP", /*paralell disp */
"sr", /*rds-sine expt */
"ns", /* New seed */
"d1", /* Disparity 1 in sqcorrug */
"d2", /* Disparity 2 (in sqcorrug) */
"Ff", /* FP move frame */
"lX", /* Log Xpos */
"mx", /* L-R-B */
"LR", /* L-R */
"LB", /* L-B */
"RB", /* R-B */
"lS", /*SF lin */
"lT", /*TF lin */
"oS", /*log size */
"m3", /* mean expt 3*/
"pU", /* pUncorr */
"pT", /* p(Target Ratio = 0)*/
"sO", /* pos orthog to stimulus axis */
"pR", /* Plaid Contrast Ratio */
"xx", /* Spikes to print */
"xx", /* print scale */
"xx", /* spike skip */
"Al", /* Auto-Zero avg length */
"^d", /*OLD_ORHTO_DISP */
"|d", /*OLD_PARA_DISP */
"Op", /*Orthogonal Pos */
"Pp", /*Paralell Pos */
"lv", /*linear veloctiy */
"we", /* Weight */
"hr", /* Head restrained */
"r0", /*RdS x None backstim expt */
"Jt", /* Jump Type */
"tr", /* Target ratio (contrast) */
"hs", /* HIGHSF value */
"ht", /* HIGHTF value */
"cz", /* CONTRAST PAIRING */
"sL", /* Length of time to plot stepper position */
"Nf", /* Actual frames done */
"pC", /* Plot Cluster */
"r1", /* reward 1 */
"r2", /* reward 2 */
"r3", /* reward 3 */
"Tc", /* Timeout Contrast */
"nR", /* number of RC files so far */
"As", /* Alternative Stimulus modes */
"Us", /* Microstim Expt */
"T1", /* Test Value 1, for various tests */
"TB",/* Test Value 2, for various tests */
"Ba",/* Size to add for background */
"b+",/* */
"Pn", /* penetration number */
"Xp", /* Pen X */
"Yp",/* Pen y */
"Vn", /*Visual Area */
"Is", /* Scale factor for IFC expts */
"xx", 
"hL", /* Height in Left eye */
"hR", /* Height in Right eye */
"wL", /* Width in Left eye */
"wR", /* Width in Right eye */
"SL", /* Size in Left eye */
"SR", /* Size in Right eye */
"Sd", /* Size disparity */
"fm", /* FixPt monocularity */
"Sb", /* SD both axes */
"uk", /* Values for Uka/DeAngelis Staircase */
"Pr", /* phase disp in disparity units, applied as reldisp */
"PX", /* Panel X Position */
"rc", /* Relative Contrast */
"by", /* backgound vertical disparity */
"PY", /* Panel Y Position */
"sC", /* Set Counterphase */
"ui", /* User ID */
"Tw", /* Temporal Distribution Width */
"ts", /* timeout for shake */
"Ol", /* Left Orthog pos */
"Or", /* Right Orthog pos */
"Covariate", /* Thing that changes with psych variable */
"iF", /* Initial Frames (Duration)*/
"e1max", /* force max val for expt 1*/
"dS", /* dummy stimulation */
"FakeSig", /* dummy stimulation magnitude*/
"pBlack", /* fraction of black dots*/
"hxtype", /* control +hx interleave */
"monkey",

// these codes are all ones that do not need to be saved
"ar", /* Aspect Ratio */
"Hi", /* Hidden Codes */
"nl", /* Number of Low F components */
"nh", /* Number of High F components */
"ls", /* Last Seed */
"dfx", /* change in fixpos x */
"ddx", /* change in disp x */
"backMov", /* background Movie */
"annTyp", /* Annulus type */
"StartDepth", /* Start Depth */
"TwoCylDisp", /* Disparity for cylinder pair */
"rC", /*random contrast expt */
"posinc", /* specify frame displacement, not velocity */
"aOp", // Opos in absolute units (not relative to RF( 
"aPp", // Opos in absolute units (not relative to RF( 
"verbose",
NULL
};


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

char codesend[NCODES] = {
	SEND_EXPLICIT};

char *serial_names[] = {
"X offset",
"Y offset",
"Stimulus Type ", 
"Sigma X", 
"Sigma Y", 
"Diparate phase",
"Disparity ",
"Width ",
"Height ",
"Spatial Frequency",
"X sampling",
"Y Sampling",
"Temporal Frequency", /*  12 */
"Spatial Phase", /*  13 */
"Orientation", /*  14 */
"Seed Loop",
"Seed",
"Contrast ",
"VDisp",
"SF2",
"TF2",/* 20*/
"Phase2",
"Plaid angle",
"J velocity",
"front intensity",
"back intensity",
"cyl num dots",
"dotlife (cyl)",
"death chance",
"contrast 2",
"delta contrast",
"delta ori",
"delta SF",
"disp grad",
"depth modulation",
"twobar angle",
"Disp Ramp",
"pDisp 2",
"Rel Phase",
"Phase Disp as Disp",
"Correlation",
"Dot Width",
"Fixation Color",
"Zoom",
"BackGround Color ",
"Delay",
"PREPERIOD", 
"Expt1 Type", 
"FRAMERATE", 
"STIMULUS DURATION", 
"CLAMP_DISPARITY", 
"POSTPERIOD", 
"Mode",
"RAMP_VERGENCE",
"N Stim (Expt 1)",
"Uff Prefix",
"High X Value for Interleaves",
"Stimulus mode",
"WURTZ task response period", 
"Max disp Initial manip", 
"optionflag setup", 
"N Repetitions", /*  37 */
"trigger level", /*  38 */
"fixation window", /*  39 */
"display flag",
"channel Set code",
"Static Vergence",
"mean fixation period",
"Replay End",
"Background Contrast",
"Expt incr", 
"Expt Mean", 
"ISI", 
"bin width", 
"binned unit data",
"unit times",
"N Frames",
"backgound disparity", /*  53 */
"ramp coefficient", /*  54 */
"fixation x pos ",
"fixation y pos ",
"fixation size ",
"mean conjugate",
"Reward Size", /*  59 */
"Vergence RAMP and Hold", /*  60 */
"TIMEOUT",  /* 61 */
"Software Offsets 62 ",
"Query State", /*  63 */
"Stop Button", /*  65 */
"Ramp Amplitude", /*  66 */
"Initial Blank Period", /*  67 */
"UpLoad Directory*", /*  68 */
"Vergence Window",
"Short Prem Trial Length",
"electrode depth", /*  71 */
"Comment", /*  72 */
"Seed Shift/UC gap", /*  73 */
"Conjugate Position", /* 74 */
"Deg2pix", /* 75 */
"DRAGGED_POS",
"optionflag2",
"H Saccade Size",
"WRONG_TIMEOUT",
"STARTVAL",
"MAXSTAIRVAL",
"MINSTAIRVAL",
"STANDING_DISP",
"Saccade Box size",
"SACTARGSIZE",
"V Saccade Size",
"DOT_DENSITY",
"VSR",
"SEND_CLEAR",
"Expt 2 Type",
"Expt 2 Mean",
"EXPTYPE_NONE",
"Expt 2 Nstims",
"Expt 2 increment",
"ANTICORRELATED_DISPARITY",
"CORRELATED_DISPARITY",
"Distribution Mean",
"NPLANES",
"MONOCULARITY_EXPT",
"VERGENCE_CORRECTION",
"FIXATION_SURROUND",
"Back Size",
"Fixation Overlap duration",
"P(reward)",
"Acquision ON/OFF",
"Relative Disparity",
"ORI_LEFT",
"ORI_RIGHT",
"Version",
"Size (H and W)",
"Backround Correlation",
"Seed Delay",
"Trigger 1",
"Trigger 2",
"Trigger 3",
"Trigger 4",
"RC seed",
"frame repeat",
"Change type",
"NewFixPos",
"RF Ori",
"BackStim",
  "FP Move Size",
  "FP Move Dir",
  "Starting Phase",
  "BackGround Ori",
  "BackGround TF",
  "Early Reward Time",  
  "N Extras",
  "Manual TDR Entry",
  "Expt 3",
  "Modulation Frequency",
  "Disparity of Fixpt",
  "Relatve Vdisp",
  "Vdisp mod",
  "Length Disparity",
  "Width Disparity",
  "N Components",
  "Stim ID",
  "Ori 2",
  "Nstim 3",
  "Spin Rate",
  "Back Width",
  "Back Height",
  "Dot X Pos",
  "Orientation Bandwidth",
  "Incr for FastSeq",
  "Distribution Width",
  "Fixwin Height",
  "Incr 3",
  "Distribution Conc",
  "Saccade Threshold",
  "Saccade Detected",
  "Initial Disparity",
  "Vertical Vergence",
"XPixel Size (cm)",
"YPIXEL_CODE",
"VIEWD_CODE",
  "Stim pulses",
  "pulse width",
"Pursuit Increment",
"Backgr Image",
"Fast Rpt B", 
"Backgr Zoom",
"Rc repeats",
"AFC Proportion",
  "Initial Movement",
  "Backgr Ortho pos",
  "Backgr Para pos",
  "Reward Bias",
  "Target X off",
  "Target Y off",
"AC Mixture",
"Tone time",
//Max serialcodes

 "Opposite Delay",
"FastSeq Repeat",
"BRIGHTSHIFT_CODE",
"BLANKCOLOR_CODE",
"Dot Sample Repeat",
"Saccade Amplitude",
"FIX_OFF_CODE",
"LOGFILE_CODE",
"STOP_CRITERION",
"RUNAVERAGE_LENGTH",
"SIDE_OFFSET",
"END_OFFSET",
"XMINSAMPLE_RATIO",
"Help File",
"XMAXSAMPLE_RATIO",
"Stimulus Version",
"PENETRATION_TEXT",
"RF P shift",
"PLC magnitude",
"Target Offset",
"DIRECTION_TEXT",
"CELLTYPE_TEXT",
"STIMULUS_FLAG",
"SWAPINTERVAL",
"trapezoidal scaling",
"RF_WIDTH",
"RF_HEIGHT",
"RF_X",
"RF_Y",
"RF O shift",
"SPIKE_GAIN",
"PLOTW",
"PLOTH",
"PLOTX",
"PLOTY",
"Completed Trials",
"GOODTRIALS",
"BADTRIALS",
"DISP_AS_CYCLES",
"QUICKEXPT_CODE",
"Gamma value",
"CORRECTION_LEAVE_CRIT",
"CORRECTION_ENTRY_CRIT",
"P(blank)",
"COVARY_XPOS",
"BONUS_CRITERION",
"BONUS2_CRITERION",
"EXPT_BLKSIZ",
"CONTRAST_LEFT",
"CONTRAST_RIGHT",
"Twobar sep",
"Fixation Linw",
"SETCLEARCOLOR",
"BACKJUMP",
"SF left",
"SF Right",
"X randomization",
"Ori randomization",
"dOri F+B",
"dOri Back",
"Stim eccentricity",
"Polar Angle",
"Old Lori",
"Old Rori",
"Cyberamp channels",
"RF Sign",
"End Length",
"Line Width for online plott",
"Plot Smoothing for online RC",
"Flags Shown on panel",
"Stimuli per fixation Trial",
"RC delay",
"PSTH smoothing",
"Contrast Ratio",
"fix linw",
"Clear color",
  "Grid Size",
  "Orthog Disp",
  "Paralell Disp",
"RDS/Sine",
  "Change Seed",
  "Disparity 1",
  "Disparity 2",
  "FP Move Frame",
  "Xpos (log)",
  "L-R-Binoc",
  "L-R-Monoc",
  "L-Binoc",
  "R-Binoc",
  "SF-Lin",
  "TF-Lin",
  "Log Size",
  "Expt 3 Mean",
  "p(Uncorr)",
  "p(One Target)",
  "StimOrthog Pos",
  "hdisp x vdisp",
  "N spikes to print",
  "Print Scale",
  "Spike skip",
  "Avg Len for eyepos auto-zero",
  "Old Orthog disp",
  "Old Paralell disp",
  "Orthog Offset",
  "Paralell Offset",
  "Linear Velocity expt",
  "Weight",
  "Head Restrained",
  "Rds x None back",
  "jump Type",
  "Target Ratio",
  "High SF val",
  "High TF val",
  "Contrast Pairs",
  "Stepper Plot Length",
  "True Nframes",
  "Plot Cluster/Exp No.",
  "Reward 1",
  "Reward 2",
  "Reward 3",
  "Timeout Contrast",
  "RC File Counter",
  "Alternative Stimulus",
  "Microstim Expt",
  "Test value 1",
  "Test value 2",
  "Size to add for background",
  "RC stimulus change",
  "penetration number",
  "Pen X",
  "Pen y",
  "Visual Area",
  "Scale factor for IFC expts",
  "unused",
  "Height L",
  "Height R",
  "Width L",
  "Width R",
  "Size L",
  "Size R",
  "Size Disparity",
  "FixPt Ocularity",
  "Sigma X and Y",
  "Uka Stair Coeffs",
  "Phase Disp as RelDisp",
  "Panel X position",
  "Relative contrast",
  "Background Vertical Disp",
  "Panel Y position",
  "Counterphase",
  "User id",
  "Time Ori BW",
  "Shake Timeout",
"Orthog Pos L",
"Orthog Pos R",
"Covariate",
"Initial Frames (Duration)",
"Expt1 Max",
"FakeStim Expt", 
"FakeStim Signal", 
"p(black dot)", 

  "Aspect Ratio",
  "Hidden Codes",
  "N Low F Components",
  "N High F Comonents",
  "Final Seed",
"fx change",
"dx change",
"Backgr Movie",
"Annulus Type",
"Start Depth",
"TwoCyl Disp",
"Random Contrasts",
"Position Displacement per frame",
    "Verbosity",
"absolute OrthoPos",
"absolute ParaPos",
    NULL};

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
extern char *serial_strings[];
extern char *channel_strings[];
extern char *stimulus_names[];
extern char *bwtoggle_codes[];
extern StringCode commstrings[];
extern ValueCode valstrings[];
extern char *mode_names[];
extern int nfplaces[MAXTOTALCODES];
extern char codesend[];
extern char *serial_names[];
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




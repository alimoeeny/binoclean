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


/*
 * remaining codes refer to the experiment settings,
 * not individual stimulus components
 */
#define PREPERIOD_CODE (LAST_STIMULUS_CODE+5)
#define EXPTYPE_CODE (LAST_STIMULUS_CODE+6)
#define FRAMERATE_CODE (LAST_STIMULUS_CODE+7)
#define STIMULUS_DURATION_CODE (LAST_STIMULUS_CODE+8)
#define CLAMP_DISPARITY_CODE (LAST_STIMULUS_CODE+9)
#define POSTPERIOD_CODE (LAST_STIMULUS_CODE+10)
#define MODE_CODE (LAST_STIMULUS_CODE+11)
#define RAMP_VERGENCE_CODE (LAST_STIMULUS_CODE+12)
#define NTRIALS_CODE (LAST_STIMULUS_CODE+13)
/*
 * July 15 2002. NTRIALS and EXPTYPE codes swapped in order, so that
 * setting NTRIALS in the code when EXPTYPE is set does not override
 * NTRIALS
 */

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
#define BLANK_P MAXSERIALCODES +43
#define COVARY_XPOS MAXSERIALCODES +44
#define BONUS_CRITERION MAXSERIALCODES +45
#define BONUS2_CRITERION MAXSERIALCODES +46
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
/*
 *  ALTERNATE_STIM_MODE a flag for doing a variety 
 *
 * 1-7 Stroboscopic variations
 * 
 */


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
//+133 unused
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
// Codes after this are convenience only - no need to store
// their value. But they need a serial-string so that they can be set
// Since they are not send to PC, they can be > 2 chars long
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

#define FREE_TEXT MAXTOTALCODES
#define RIGHTH_GAIN MAXTOTALCODES+3
#define RIGHTH_OFFSET MAXTOTALCODES+4
#define RIGHTH_PHASE MAXTOTALCODES+5
#define RIGHTH_SOFTOFF MAXTOTALCODES+6
#define RIGHTV_GAIN MAXTOTALCODES+7
#define RIGHTV_OFFSET MAXTOTALCODES+8
#define RIGHTV_PHASE MAXTOTALCODES+9
#define RIGHTV_SOFTOFF MAXTOTALCODES+10
#define LEFTH_GAIN MAXTOTALCODES+11
#define LEFTH_OFFSET MAXTOTALCODES+12
#define LEFTH_PHASE MAXTOTALCODES+13
#define LEFTH_SOFTOFF MAXTOTALCODES+14
#define LEFTV_GAIN MAXTOTALCODES+15
#define LEFTV_OFFSET MAXTOTALCODES+16
#define LEFTV_PHASE MAXTOTALCODES+17
#define LEFTV_SOFTOFF MAXTOTALCODES+18
#define FLUID_TEXT MAXTOTALCODES+19
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
    char *value;
    
} ValueCode;

#define MONITOR_FILE 0


#ifdef CHARCODES_DEFINE

#ifdef DOSVERSION
#define NCODES MAXSERIALCODES
#else
#define NCODES MAXTOTALCODES
#endif



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
 * N.B. THESE CIDES MUST BE DIFFERNT FORM toggle_codes in front.c
 */
char *bwtoggle_codes[] = {
    "fo", /* file is open */
    NULL,
};

ValueCode valstrings[] = {
    {"xo","X offset",  XPOS},
    {"lo", "Log File", LOGFILE_CODE, NULL},
    {"st", "Stimulus", STIMULUS_TYPE_CODE, NULL},
    {"monkey", "Monkey name", MONKEYNAME, NULL},
    {"impref", "prefix for image files", 0, NULL},
    {"immode", "image mode", 0, NULL},
    {NULL, NULL, 0, NULL}
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
#ifdef DOSVERSION
	NULL
};
#else
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
    "Target 2 pos",
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
    "Plaid Contrast Ratio",
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
    "High X type",
    "Monkey Name",
    
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
    "absolute OrthoPos",
    "absolute ParaPos",
    "Verbosity",
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
#endif

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
#ifndef DOSVERSION
extern char *mode_names[];
extern int nfplaces[MAXTOTALCODES];
extern char codesend[];
extern char *serial_names[];
extern char *jumpnames[];
#endif
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
#endif


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





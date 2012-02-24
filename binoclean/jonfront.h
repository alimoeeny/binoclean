#include "jondefs.h"
#include "janti.h"

#define JONLEFT -1
#define JONRIGHT 1
#define AMBIGUOUS 0

#define SWAPINTER 1
#define LEVELS 80
#define RAMPMIDDLE 200
#define NORMAL_LOOP 1
#define CORRECTION_LOOP 2
#define FOUL 0
#define CORRECT 1
#define WRONG 2

#define ROOT2 1.4142
#define MINSTAIRVALUE 0.01
#define MAXSTAIRVALUE 1.00
#define PERF_STRING_LENGTH 90
#define BLANK 1
#define SEARCH 2

#define ONEUP 0  //simple staicase with one param
#define MAGONE_SIGNTWO 1 // vary magnitude and sign separately


typedef struct DBALLS{
    float pos[XY];
}Dballs;

typedef struct DOTSQUARE{
	float dotsize[XY];
	int framerate;
	int top_intensity;
	int bot_intensity;
	int numpoints[XY];
	int totalpoints;
	float ratio;
	float area;
	float spacing;
	Dballs *balls;
	int signflag; /* either +1 or -1 */
}Dotsquare;

typedef struct{
    float bin;
    int n;
    int correct;
    int foul;
}hist_structure;

typedef struct{
    int highbin, lowbin, currentbin, numbins, startbin;
    int staircounter;
    hist_structure *histstructure;
}stair_hist_s;

typedef struct{
    int twenty_tot;
    int hundred_tot;
    int allcorrect, allwrong;
}AFC_tots;

typedef struct{
    int loopstate,laststate,result;
    int sign;
    
    AFC_tots totals;
    char *performance_1, *performance_2;
    float sacval[6];
    float abssac[4];
    float sacdir[4];
    float newdirs,stimsign;
    float targsize; /*j fix criterion for saccade targets*/
    float sac_fix_crit;
    float maxstairvalue, minstairvalue, startvalue;
    float stairfactor;
    float wrongtimeout; 
    stair_hist_s stairhist;
    float jlaststairval, jnextstairval, jstairval;
    int lasttrial,trialres; /*j if BAD_FIX loop the value*/
    float proportion, ccvar; /* proportion L stimuli when Random */
    int correction_entry, correction_leave; /* correction loop criteria */
    int bonuslevel, goodinarow,bonuslevel2; /* increasing reward for good behaviour */
    double gregvals[5];
    double vals[2];
    int stimid,type,magid,nmags,stairsign;
    double jsignval;
}AFCstructure;

#define SACCREQD(x) (x.abssac[0] != 0 || x.abssac[1] != 0)

/* stair.c prototypes */
int find_direction(float expval);
AFC_tots afccounters(int direction, int response);
int set_loop_state();
float set_stair_level(float lastval, int response, int state, float stairfactor, float maxval, float minval, stair_hist_s *shist);
void performance_string(float expval, int response, int state,  char **s1, char **s2, float sacval);
void nullify(char *s, int length);
void reset_afc_counters();
stair_hist_s setup_stairstruct(float *startvalue, float stairfactor, float maxvalue, float minvalue);
void stair_counters(stair_hist_s *shist, int response);
int loopstate_counters(int direction, int response);
stair_hist_s newstairstruct();
void printstair(int staircounter,  int startbin,  int bincounter_min,  int bincounter_max, hist_structure *pstairbins);


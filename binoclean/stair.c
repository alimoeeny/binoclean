#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include "stimdefs.h"
#include "jonfront.h"

FILE *stairfd = NULL;
extern FILE *seroutfile;
extern int option2flag,optionflags[];
extern AFCstructure afc_s;

static int missed_negative=0,  missed_positive=0, got_correction = 0;

int ranright=0, ranleft=0, ranleftc = 0, ranrightc = 0;

/********************************************************************************/




void OpenStaircaseLog()
{
    time_t tval;
    
    if((stairfd = fopen("stairlog","a")) == NULL){
        fprintf(stderr,"Can't open stairlog\n");
    }
    else{
        tval = time(NULL);
        fprintf(stairfd,"Reopened %s",ctime(&tval));
    }
}

/*
 * Modified staircase as described by Uka and Deangelis
 * If correct, reduce signal (/scale)  with p = gregvals[0];
 * If incorrect increas signal (*scale)  with  p = gregvals[1];
 * If correct p(flip sign) = gregvals[2]
 * If incorrect p (flip sign) = gregvals[3]
 * If response is null, leave magnitue, p(flip) = 0.5
 */
int greg_setstair(int result, AFCstructure *afc)
{
    double scale = 2, rnd,arnd;
    arnd = rnd = drand48();
    
    scale = afc->gregvals[4];
    if(fabs(afc->sign) < 0.9)
        afc->sign = 1;
    
    if(result == CORRECT){
        if(rnd < afc->gregvals[0]){
            afc->jstairval = afc->jlaststairval/scale;
            afc->magid--;
        }
        rnd = drand48();
        if(rnd < afc->gregvals[2])
            afc->sign = -1 * afc->sign;
    }
    else if(result == WRONG){
        if(rnd < afc->gregvals[1]){
            afc->jstairval = afc->jlaststairval * scale;
            afc->magid++;
        }
        rnd = drand48();
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
        if(afc->magid >= afc->nmags)
            afc->magid = afc->nmags -1;
        afc->vals[0] = fabs(afc->jstairval);
        afc->vals[1] = fabs(afc->vals[1]) * afc->sign;
        afc->jstairval = afc->vals[0];
        if(stairfd)
            fprintf(stairfd,"Res %d (rnd %.2f,%.2f), Val was %.2f, now %.2f ",result,arnd,rnd,afc->jlaststairval,afc->jstairval);
        else{
            fprintf(stdout,"Res %d (rnd %.2f,%.2f), Val was %.2f, now %.2f ",result,arnd,rnd,afc->jlaststairval,afc->jstairval);
            OpenStaircaseLog();
        }
    }
    else{
        afc->vals[0] = fabs(afc_s.jstairval) * afc->sign;
    }
    
}


void reset_afc_counters()
{
	missed_negative = missed_positive = 0;
	ranleftc = 0; ranrightc = 0;
	afc_s.loopstate = NORMAL_LOOP;
	afc_s.laststate = NORMAL_LOOP;
	
}

/********************************************************************************/
void nullify(char *s, int length)
{
    int i=0;
    for(i = 0; i < length; i++)
        *(s+i) = NULL;
}

/********************************************************************************/
int find_direction(float expval)
{
    int direction;
    if(expval < 0.00001 && expval > -0.00001)
        direction = AMBIGUOUS;
    else if(expval < 0.0){
        direction = JONRIGHT;
    }
    else if(expval > 0.0){
        direction = JONLEFT;
    }
	
    return(direction);
}

/********************************************************************************/
int monkey_direction(int response, AFCstructure afc_s)
{
    int monkey_direction;
    int direction;
    float *sacval = afc_s.sacval;
    
    if(afc_s.newdirs)
        direction = -afc_s.stimsign;
    else if(fabs(sacval[0]) > fabs(sacval[1]))
        direction = find_direction(sacval[0] * afc_s.abssac[0] * afc_s.sign);
    else
        direction = find_direction(sacval[1] * afc_s.abssac[1] * afc_s.sign);
    
    switch(direction){
        case JONRIGHT:
            switch(response){
                case CORRECT:
                    monkey_direction = JONRIGHT;
                    break;
                case WRONG:
                    monkey_direction = JONLEFT;
                    break;
                default:
                    monkey_direction = FOUL;
                    break;
            }
            break;
        case JONLEFT:
            switch(response){
                case CORRECT:
                    monkey_direction = JONLEFT;
                    break;
                case WRONG:
                    monkey_direction = JONRIGHT;
                    break;
                default:
                    monkey_direction = FOUL;
                    break;
            }
            break;
    }
    return(monkey_direction);
}

/********************************************************************************/
int loopstate_counters(int direction, int response)
{
    int loopstate, count = afc_s.correction_leave+afc_s.correction_entry;
    
    switch (direction){
        case JONLEFT:
            switch(response){
                case CORRECT:
                    if(afc_s.loopstate != CORRECTION_LOOP || ++got_correction >= afc_s.correction_leave){
                        missed_negative = 0; /* correct neg */
                        if(afc_s.loopstate == CORRECTION_LOOP)
                            missed_positive = 0;
                    }
                    break;
                case WRONG:
                    if(afc_s.loopstate != CORRECTION_LOOP)
                        missed_negative++; /* missed neg */
                    break;
                case FOUL:
                    break;
            }
            if(afc_s.loopstate == CORRECTION_LOOP)
                ranleftc++;
            else
                ranleft++;
            break;
        case JONRIGHT:
            switch(response){
                case WRONG:
                    missed_positive++; /* missed pos */
                    break;
                case CORRECT:
                    if(afc_s.loopstate != CORRECTION_LOOP || ++got_correction >= afc_s.correction_leave){
                        missed_positive = 0; /* correct pos */
                        if(afc_s.loopstate == CORRECTION_LOOP)
                            missed_positive = 0;
                    }
                    
                    break;
                case FOUL:
                    break;
            }	   
            if(afc_s.loopstate == CORRECTION_LOOP)
                ranrightc++;
            else
                ranright++;
            break;
        case AMBIGUOUS:
            break;
    }
    afc_s.laststate = afc_s.loopstate;
    if(afc_s.type == ONEUP)
        loopstate = set_loop_state();
    if(afc_s.type == MAGONE_SIGNTWO)
        loopstate = set_loop_state();
    return (loopstate);
    
}
/********************************************************************************/
int set_loop_state()
{
    int loop_state;
    
    /* If getting both wrong don't go into correction loop */
    if(missed_negative >= afc_s.correction_entry && missed_positive >= afc_s.correction_entry)
        missed_negative = missed_positive = 0;
    
    if ((missed_negative >= afc_s.correction_entry) || (missed_positive >= afc_s.correction_entry) && afc_s.correction_entry > 0)
        loop_state = CORRECTION_LOOP;
    else
    {
        loop_state = NORMAL_LOOP;
        got_correction = 0;
    }
    
    return(loop_state);
}

/********************************************************************************/
static int last_twenty[20]={0}, last_hundred[100]={0};
static int twenty_counter=0, hundred_counter=0;
static int allcorrect = 0;

clear_afccounters()
{ 
    int i,j;
    
    for(i = 0; i < 20; i++)
        last_twenty[i] = 0;
    twenty_counter = 0;
    
    for(i = 0; i < 100; i++)
        last_hundred[i] = 0;
    hundred_counter = 0;
    
    afc_s.totals.allcorrect = 0;
    afc_s.totals.allwrong = 0;
    ranleft = 0; ranright = 0;
    ranleftc = 0; ranrightc = 0;
    afc_s.totals.twenty_tot = 0;
    afc_s.totals.hundred_tot = 0;
}

AFC_tots afccounters(int direction, int response)
{
    AFC_tots totals = {0}; /*j struct for hundred_tot and twenty_tot */
    int cnt;
    
    if(twenty_counter > 19)
	    twenty_counter = 0; /*point to first in array*/
    
    if(hundred_counter > 99)
	    hundred_counter = 0; 
    
    
    switch (direction){
        case JONLEFT:
            switch(response){
                case CORRECT:
                    last_twenty[twenty_counter] = 1; 
                    last_hundred[hundred_counter] = 1;
                    afc_s.totals.allcorrect++;
                    afc_s.goodinarow++;
                    break;
                case WRONG:
                    last_twenty[twenty_counter] = 0; 
                    last_hundred[hundred_counter] = 0;
                    afc_s.goodinarow = 0;
                    afc_s.totals.allwrong++;
                    break;
                case FOUL:
                    if(optionflags[COUNT_BADFIX_FOR_BONUS])
                        afc_s.goodinarow = 0;
                    break;
            }
            break;
        case JONRIGHT:
            switch(response){
                case WRONG:
                    last_twenty[twenty_counter] = 0; 
                    last_hundred[hundred_counter] = 0;
                    afc_s.totals.allwrong++;
                    afc_s.goodinarow = 0;
                    break;
                case CORRECT:
                    last_twenty[twenty_counter] = 1; 
                    last_hundred[hundred_counter] = 1;
                    afc_s.totals.allcorrect++;
                    afc_s.goodinarow++;
                    break;
                case FOUL:
                    if(optionflags[COUNT_BADFIX_FOR_BONUS])
                        afc_s.goodinarow = 0;
                    break;
            }	   
            break;
        case AMBIGUOUS:
            switch(response){
                case WRONG:
                    afc_s.goodinarow = 0;
                    break;
                case CORRECT:
                    afc_s.goodinarow++;
                    break;
                case FOUL:
                    if(optionflags[COUNT_BADFIX_FOR_BONUS])
                        afc_s.goodinarow = 0;
                    break;
            }	   
            break;
    }
    if ((response == WRONG || response == CORRECT) && direction != AMBIGUOUS){
	    twenty_counter++;
	    hundred_counter++; 
    }
    afc_s.totals.twenty_tot = 0;
    for(cnt=0; cnt<20; cnt++)
        afc_s.totals.twenty_tot += last_twenty[cnt]; 
    afc_s.totals.hundred_tot = 0;
    for(cnt=0; cnt<99; cnt++)
        afc_s.totals.hundred_tot += last_hundred[cnt];
    if(seroutfile)
        fprintf(seroutfile,"(%d,%d,%d)\n",afc_s.goodinarow,direction,response);
    return(totals);
}

/********************************************************************************/
void performance_string(float expval, int response, int state,  char **s1, char **s2, float sacval)
{
    static int laststate=NORMAL_LOOP, counter=0;
    char c, *temp;
    int direction, ambig_direction;
    static float lastexpval=-999;
    char buf[100];
    
    direction = find_direction(expval);
    ambig_direction = find_direction(sacval);
    
    if(*((*s1))==NULL) /* if s1 has been nullified(cleared) then point to first element */
        counter = 0;
	
    if(counter+9 > PERF_STRING_LENGTH){ /* s2 becomes s1 and s1 points to a cleared string */
        temp = *s2;
        *s2 = *s1;
        *s1 = temp;
        nullify(*s1, PERF_STRING_LENGTH);
        counter=0; /* just clears s1 to a string of NULLS */
    }
    
    
    if(lastexpval != (expval = fabsf(expval)) ){		/* if the value of the cue has changed then print it */
        sprintf(buf, "%4.2f", expval);
        strcat(*s1, buf);					/*j where *s1 is a pointer to a pointer*/
        counter += strlen(buf);
        lastexpval = expval;
    }
    
    /*    if( (state == CORRECTION_LOOP) && (laststate != CORRECTION_LOOP ) )bgc*/
    
    switch (direction){
        case JONLEFT:
            if(fabs(afc_s.sacval[1]) > fabs(afc_s.sacval[0]))
                c = 'U';
            else
                c = 'L'; 
            break;
        case JONRIGHT: 
            if(fabs(afc_s.sacval[1]) > fabs(afc_s.sacval[0]))
                c = 'D';
            else
                c = 'R';
            break;
        case AMBIGUOUS:
            c = 'a';
            break;
    }
    *((*s1)+counter++) = c;
    
    if ( (direction == AMBIGUOUS) ){ /*j sacval is opposite to val of stim*/
        switch(ambig_direction){
            case JONLEFT:
                c = 'R';
                break;
            case JONRIGHT:
                c = 'L';
                break;
        }	
        *((*s1)+counter++) = c;
    }
	
	switch (response){
	    case CORRECT:
            c = '#'; 
            break;
	    case WRONG:
            c = 'x';
            break;
	    case FOUL:
            c = 'o';
            break;
	}
    *((*s1)+counter++) = c;
    
    if( (state == CORRECTION_LOOP))
        *((*s1)+counter++) = '!';
    /*    else if( (state == NORMAL_LOOP) && (laststate != NORMAL_LOOP) )
     *(*s1+counter++) = '@';*/
    else
        *((*s1)+counter++) = ' ';
    laststate = state;
}

/********************************************************************************/
float set_stair_level(float lastval, int response, int state, float stairfactor, float maxval, float minval, stair_hist_s *shist)
{ 
    float newvalue, downvalue, upvalue;
    float val;
    int bin;
    
    bin = shist->currentbin;
    newvalue = lastval;
    
    switch(state){
        case NORMAL_LOOP:    
            if(response == CORRECT){
                /*shist->histstructure[bin].correct++;
                 shist->histstructure[bin].n++;
                 shist->staircounter++;*/
                downvalue = lastval / stairfactor;
                val = drand48();
                if( (val>0.66) && (fabsf(downvalue) > minval)){  
                    newvalue = downvalue;
                    /*if(bin-- > 0){
                     shist->currentbin--;
                     if(shist->currentbin < shist->lowbin)
                     shist->lowbin--;
                     } */ 	    
                }
            }
            else
                if(response == WRONG){ 
                    /*shist->histstructure[bin].n++;
                     shist->staircounter++;*/
                    upvalue = lastval * stairfactor;
                    if(upvalue < maxval)
                        newvalue = upvalue;
                    /*if(bin++ <= shist->numbins){
                     shist->currentbin++;
                     if(shist->currentbin > shist->highbin)
                     shist->highbin++;
                     }*/
                }
            break;
        case CORRECTION_LOOP:						    				     
            upvalue = lastval * stairfactor;
            if(fabsf(upvalue) < maxval)
                newvalue = upvalue;
    }
    return(newvalue);
}
/********************************************************************************/
stair_hist_s setup_stairstruct(float *startvalue, float stairfactor, float maxvalue, float minvalue)
{
    int numbins, i;
    float binvalue;
    stair_hist_s stair_structure;
    float upval;
    int startbin;   
    
    if(*startvalue > maxvalue)
        *startvalue = maxvalue;
    
    
    /* set initial bin + find numbins*/
    for(binvalue=minvalue, numbins=1; binvalue <  maxvalue; numbins++, binvalue*=stairfactor){
        if( (binvalue*stairfactor > *startvalue) && (binvalue < *startvalue) ){ /* finds initial starting bin*/
            startbin = numbins-1;
            *startvalue = binvalue*stairfactor; /*j sets startvalue to be an increment of bins*/
        }    
    }
    
    stair_structure.histstructure = (hist_structure *)malloc(numbins * sizeof(hist_structure));
    
    /* fill with relevant values */
    for(i=0; i<numbins; i++){
        stair_structure.histstructure[i].bin = minvalue * powf(stairfactor, (float)i);
        stair_structure.histstructure[i].n = stair_structure.histstructure[i].correct = stair_structure.histstructure[i].foul = 0;
    }
    stair_structure.staircounter = 0;
    stair_structure.lowbin = stair_structure.highbin = 0;
    stair_structure.numbins = numbins;	
    stair_structure.currentbin = stair_structure.startbin = startbin;
    return(stair_structure);
}

/********************************************************************************/
void stair_counters(stair_hist_s *shist, int response)
{
    int bin = shist->currentbin;
    
    
    if(response == CORRECT){
        shist->histstructure[bin].correct++;
        shist->histstructure[bin].n++;
        shist->staircounter++;
        if(bin++ <= shist->numbins){
            shist->currentbin++;
            if(shist->currentbin > shist->highbin)
                shist->highbin++;
        }
    }
    else
        if(response == WRONG){
            shist->histstructure[bin].n++;
            shist->staircounter++;
            if(bin-- > 0){
                shist->currentbin--;
                if(shist->currentbin < shist->lowbin)
                    shist->lowbin--;
            }  	    
        }
        else
            if(response == FOUL)
                shist->histstructure[bin].foul++;
}

/********************************************************************************/
void printstair(int staircounter,  int startbin,  int bincounter_min,  int bincounter_max, hist_structure *pstairbins)
{
	int startprn, stopprn;
	int i;
	static int lastcounter=0;
    
	if( (staircounter%5) || (lastcounter == staircounter) )
	    return;
    
	startprn = startbin + bincounter_min;
	stopprn = startbin + bincounter_max;
	printf("\n");
	printf("\ntrials: %d\n", staircounter);
	printf("\nlevel\tn\tcorr\tpercent\n");
	for(i=startprn; i < stopprn + 1; i++){
	    if(pstairbins[i].n != 0)
		    printf("%4.2f\t%4d\t%4d\t%.2f\n", pstairbins[i].bin,
                   pstairbins[i].n, pstairbins[i].correct,  (float)pstairbins[i].correct/(float)pstairbins[i].n);
	}
	lastcounter = staircounter;
    
}
/********************************************************************************/

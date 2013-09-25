#include <stdio.h>
#include <sys/time.h>
#include <stdarg.h>
#include <math.h>
#include "mymath.h"
#include "myview.h"
#include <OpenGL/OpenGL.h>
#include "commdefs.h"
#include "stimuli.h" /*j this will include jonfront.h */
#include "WinDefs.h"
#include "misc.h"
#include "protos.h"

#define ISI 5



extern long optionflag,option2flag;
extern Expt expt;
extern AFCstructure afc_s;

int SetProperty(Expt *exp, Stimulus *st, int code, float val);
float GetProperty(Expt *exp, Stimulus *st, int code);
float ExptProperty(Expt *exp, int flag);


extern Wurtz *fix;


/********************************************************************************/
stair_hist_s newstairstruct()
{
    stair_hist_s s;
    s = setup_stairstruct(&afc_s.startvalue, afc_s.stairfactor, afc_s.maxstairvalue, afc_s.minstairvalue);
    return(s);
}

/********************************************************************************/

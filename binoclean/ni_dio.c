#include "NIDAQmxBase.h"
#include <stdio.h>

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { fprintf(stderr,"DAQ errror %d\n",error); } }

static    TaskHandle  taskHandle = 0;
static int isok = 0;

int DIOInit()
{
    int32       error = 0;
    char        errBuff[2048];
    int i = 0;
    
    // Channel parameters
    char        chan[] = "Dev1/port0";
    
    DAQmxErrChk (DAQmxBaseCreateTask ("", &taskHandle));
    if (error) return(-1);
    DAQmxErrChk (DAQmxBaseCreateDOChan(taskHandle,chan,"",DAQmx_Val_ChanForAllLines));
    if (error) return(-1);
    else 
        isok = 1;
    return(0);
}


int DIOWrite(int output)
{
    int32       error = 0;
    uInt32      w_data [1];
    int32       written;
    
    w_data[0] = output;
    if(!isok)
        return(0);
    DAQmxErrChk (DAQmxBaseWriteDigitalU32(taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,w_data,&written,NULL));
}

int DIOClose()
{
    if (taskHandle != 0)
    {
        DAQmxBaseStopTask (taskHandle);
        DAQmxBaseClearTask (taskHandle);
        taskHandle = 0;
    }
    
}

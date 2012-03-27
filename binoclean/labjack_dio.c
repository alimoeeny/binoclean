//
//  labjack_dio.c
//  binoclean
//
//  Created by Ali Moeeny on 3/27/12.
//  Copyright (c) 2012 LSR-NEI. All rights reserved.
//

#include <stdio.h>
#include "u6.h"

static int isok = 0;

HANDLE hDevice = 0;

int DIOInit()
{
    //Opening first found U6 over USB
    if( (hDevice = openUSBConnection(-1)) == NULL )
    {
        return -1;
    }else {
        return 0;
    }
}


int DIOWrite(int output)
{
    if(!isok)
        return(0);
    
    if (eDO(hDevice, output, 1)!=0)
    {
        printf("DIO ERROR while writing");
    }
    return 0;
}

int DIOClose()
{
    if (hDevice!=NULL)
    {
        closeUSBConnection(hDevice);
    }
}

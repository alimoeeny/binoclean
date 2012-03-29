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

static HANDLE hDevice = 0;

int DIOInit()
{
    //Opening first found U6 over USB
    if( (hDevice = openUSBConnection(-1)) == NULL )
    {
        return -1;
    }else {
        isok = 1;
        return 0;
    }
}


int DIOWrite(int output)
{
    if(!isok)
        return(0);
    
    int r = 0;
    
//    for (int i = 0; i < 20; i++) {
//        if (output & i) 
//            r = r + eDO(hDevice, i, 1); 
//        else 
//            r = r + eDO(hDevice, i, 0);
//    }

    switch (output) {
        case 0:
            r = r + eDO(hDevice, 0, 0);
            r = r + eDO(hDevice, 1, 0);
            r = r + eDO(hDevice, 2, 0);
            r = r + eDO(hDevice, 3, 0);
            break;
        case 2:
            r = r + eDO(hDevice, 2, 1);
            break;
        case 4:
            r = r + eDO(hDevice, 3, 1);
            break;
        case 7:
        case 0xF:
            r = r + eDO(hDevice, 0, 1);
            r = r + eDO(hDevice, 1, 1);
            r = r + eDO(hDevice, 2, 1);
            r = r + eDO(hDevice, 3, 1);
            break;

        default:
            break;
    }
    
    if (r!=0)
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

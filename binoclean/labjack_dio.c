//
//  labjack_dio.c
//  binoclean
//
//  Created by Ali Moeeny on 3/27/12.
//  Copyright (c) 2012 LSR-NEI. All rights reserved.
//

#include <stdio.h>
#include "u3.h"

static int isok = 0;

static HANDLE hDevice = 0;

int DIOInit()
{
    //Opening first found U3/U6 USB
    if( (hDevice = openUSBConnection(-1)) == NULL )
    {
        return -1;
    }else {
        isok = 1;
        eDO(hDevice, 1, 0, 0);
        return 0;
    }
}


long DIOFeedback(HANDLE hDevice, uint8 *inIOTypesDataBuff, long inIOTypesDataSize)
{
    uint8 *sendBuff, *recBuff;
    uint16 checksumTotal;
    int sendChars, recChars, sendDWSize, recDWSize;
    int commandBytes, ret, i;
    
    ret = 0;
    commandBytes = 6;
    
    if( ((sendDWSize = inIOTypesDataSize + 1)%2) != 0 )
        sendDWSize++;

    
    sendBuff = (uint8 *)malloc(sizeof(uint8)*(commandBytes + sendDWSize));

    if( sendBuff == NULL)
    {
        ret = -1;
        goto cleanmem;
    }
    
    sendBuff[sendDWSize + commandBytes - 1] = 0;
    
    /* Setting up Feedback command */
    sendBuff[1] = (uint8)(0xF8);  //Command byte
    sendBuff[2] = sendDWSize/2;  //Number of data words (.5 word for echo, 1.5
    //                      words for IOTypes)
    sendBuff[3] = (uint8)(0x00);  //Extended command number
    
    sendBuff[6] = 0;  //Echo
    
    for( i = 0; i < inIOTypesDataSize; i++ )
        sendBuff[i+commandBytes+1] = inIOTypesDataBuff[i];
    
    extendedChecksum(sendBuff, (sendDWSize+commandBytes));
    
    //Sending command to U3
    if( (sendChars = LJUSB_Write(hDevice, sendBuff, (sendDWSize+commandBytes))) < sendDWSize+commandBytes )
    {
        if( sendChars == 0 )
            printf("ehFeedback error : write failed\n");
        else
            printf("ehFeedback error : did not write all of the buffer\n");
        ret = -1;
        goto cleanmem;
    }
    
    
cleanmem:
    free(sendBuff);

    sendBuff = NULL;
    recBuff = NULL;
    
    return ret;
}

int DIOWrite(BYTE output)
{
    printf("this is the old DIO!!!!!\n");
}

int DIOWriteBit(int Channel, BYTE output)
{
    if(!isok)
        return(0);
    
    int r = 0;
    uint8 sendBuff[12],sendDataBuff[12];;
    int State = output&1;
//    for (int i = 0; i < 20; i++) {
//        if (output & i) 
//            r = r + eDO(hDevice, i, 1); 
//        else 
//            r = r + eDO(hDevice, i, 0);
//    }
//r = r + eDO(hDevice, 0, 2, output&1);
//    return;
    
    sendDataBuff[0] = 13;  //IOType is BitDirWrite
    sendDataBuff[1] = Channel + 128;  //IONumber(bits 0-4) + Direction (bit 7)
    
    sendDataBuff[2] = 11;  //IOType is BitStateWrite
    sendDataBuff[3] = Channel + 128*((State > 0) ? 1 : 0);  //IONumber(bits 0-4) + State (bit 7)
    
    DIOFeedback(hDevice, sendDataBuff, 4);
    r=12;

 //   r = LJUSB_Write(hDevice, sendBuff, 12);
//    
//    if (output & 1)
//        r = r + eDO(hDevice, 1, 0, 1);
//    else {
//        r = r + eDO(hDevice, 1, 0, 0);
//    }
//    if (output & 2)
//        r = r + eDO(hDevice, 1, 1, 1);
//    else {
//        r = r + eDO(hDevice, 1, 1, 0);
//    }
//    if (output & 4)
//        r = r + eDO(hDevice, 1, 2, 1);
//    else {
//        r = r + eDO(hDevice, 1, 2, 0);
//    }
//    if (output & 8)
//        r = r + eDO(hDevice, 1, 3, 1);
//    else {
//        r = r + eDO(hDevice, 1, 3, 0);
//    }
        
//    switch (output) {
//        case 0:
//            r = r + eDO(hDevice, 1, 0, 0);
//            r = r + eDO(hDevice, 1, 1, 0);
//            r = r + eDO(hDevice, 1, 2, 0);
//            r = r + eDO(hDevice, 1, 3, 0);
//            break;
//        case 2:
//            r = r + eDO(hDevice, 1, 2, 1);
//            break;
//        case 4:
//            r = r + eDO(hDevice, 1, 3, 1);
//            break;
//        case 7:
//        case 0xF:
//            r = r + eDO(hDevice, 1, 0, 1);
//            r = r + eDO(hDevice, 1, 1, 1);
//            r = r + eDO(hDevice, 1, 2, 1);
//            r = r + eDO(hDevice, 1, 3, 1);
//            break;
//
//        default:
//            break;
//    }
    
    if (r!=12)
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

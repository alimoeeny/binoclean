//
//  main.m
//  binoclean
//
//  Created by Ali Moeeny on 6/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "commdefs.h"
#import "stimuli.h"
#import "protos.h"
#import <sys/time.h>
#include <sys/resource.h>

Stimulus *TheStim, tempstim,*stimptr,*ChoiceStima,*ChoiceStimb;
Expt expt;
Monitor mon;
int binocmain(int argc, char * argv[]);


int main(int argc, char *argv[])
{
    setpriority(PRIO_PROCESS, 0, -10);
    binocmain(argc, argv);
    return NSApplicationMain(argc, (const char **)argv);
}

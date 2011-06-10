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

Stimulus *TheStim, tempstim;

int main(int argc, char *argv[])
{
    TheStim = NewStimulus(NULL);
    StimulusType(TheStim, STIM_GRATING);
    ReadExptFile("/local/demo/stims/bgc.txt", 1, 0, 0);
//    SetStimulus(TheStim, (float)4.0f, STIM_SIZE, 0);
//    SetStimulus(TheStim, 1.0f, SETCONTRAST, 0);
//    SetStimulus(TheStim, 4.0f, SF, 0);
    
    
    return NSApplicationMain(argc, (const char **)argv);
}

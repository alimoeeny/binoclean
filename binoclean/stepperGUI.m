//
//  stepperGUI.m
//  binoclean
//
//  Created by Ali Moeeny on 2/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "stepperGUI.h"

extern int electrodeDepth;
extern char *stepport;

@implementation stepperGUI
@synthesize stepSize;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        self.stepSize = 5;
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    NSLog(@"opening Stepper");
    OpenStepSerial(stepport);
}


- (IBAction) uDriveInitialize
{
    NSLog(@"opening Stepper");
    OpenStepSerial(stepport);
}

- (void) uDriveButtonPress:(id) sender
{
    int step = electrodeDepth + [sender tag] * self.stepSize;
    if (step<300) {
        NewPosition(step);
    }
    else
    {
        NSAlert * a = [[NSAlert alloc] init];
        [a setMessageText:@"Large Step!"];
        [a addButtonWithTitle:@"I know!"];
        [a setInformativeText:[NSString stringWithFormat:@"You want to move the electrode %d micrometers and it is now a good idea!", step]];
        [a beginSheetModalForWindow:[[NSApplication sharedApplication] mainWindow] modalDelegate:nil didEndSelector:nil contextInfo:nil];
    }
}

@end

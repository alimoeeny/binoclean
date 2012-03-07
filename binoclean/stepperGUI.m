//
//  stepperGUI.m
//  binoclean
//
//  Created by Ali Moeeny on 2/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "stepperGUI.h"

extern int electrodeDepth;
extern int motorPort;
void SetMotorId(int id);
extern char stepperport[256];

@implementation stepperGUI
@synthesize stepSize;
@synthesize motorId;

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
    self.motorId = 0;
    if([[NSUserDefaults standardUserDefaults] valueForKey:@"motorid"])
    {
        self.motorId = [[[NSUserDefaults standardUserDefaults] valueForKey:@"motorid"] intValue];
    }
    OpenStepSerial(stepperport);
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
        [a addButtonWithTitle:@"Sure!"];
        [a setInformativeText:[NSString stringWithFormat:@"You want to move the electrode %d micrometers and it is not a good idea!", step]];
        [a beginSheetModalForWindow:[[NSApplication sharedApplication] mainWindow] modalDelegate:nil didEndSelector:nil contextInfo:nil];
    }
}

- (IBAction) reopen:(id)sender
{
    close(motorPort);
    OpenStepSerial(stepperport);
}

- (IBAction) setMotorIdComboChanged:(id)sender
{
    self.motorId = [[sender stringValue] intValue];
    NSLog(@"Setting motorId to: %d", self.motorId);
    SetMotorId(self.motorId);
}

@end

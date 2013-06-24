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

@synthesize upButton;
@synthesize downButton;
@synthesize electrodePosition;
@synthesize newPositionTextField;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        self.stepSize = 5;
    }
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(electrodeMoved:) name:@"electrodeposition" object:nil];
    return self;
}

- (void) electrodeMoved:(NSNotification *) aNotification
{
    [electrodePosition setStringValue:[NSString stringWithFormat:@"%d um", electrodeDepth]];
    [newPositionTextField setStringValue:[NSString stringWithFormat:@"%d", electrodeDepth]];
    [self enableNewMovements];
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
    int step = [sender tag] * self.stepSize;
    [self moveElectrode:step];
}

- (void) uDriveRepositionMove:(id) sender
{
    int step = [[newPositionTextField stringValue] intValue] - electrodeDepth;
    [self moveElectrode:step];
}

- (void) uDriveRepositionSet:(id) sender
{
    int newPos = [[sender stringValue] intValue];
    setCurrentPosition(newPos);
}

- (void) moveElectrode:(int) step
{
    if (abs(step) > 300) {
        NSLog(@"Large Step!");
//        NSAlert * a = [NSAlert alertWithMessageText:@"Large Step!" defaultButton:@"No, Don't move the electrode" alternateButton:@"Sure, Go Ahead!" otherButton:nil informativeTextWithFormat:@"You want to move the electrode %d micrometers and it is quite a long distance.\r Are you sure it is a good idea?!", step];
//        NSAlert * a = [[NSAlert alloc] init];
//        [a setMessageText:@"Large Step!"];
//        [a addButtonWithTitle:@"Sure, Go Ahead!"];
//        [a addButtonWithTitle:@"No, Don't move the electrode"];
//        [a setInformativeText:[NSString stringWithFormat:@"You want to move the electrode %d micrometers and it is quite a long distance.\r Are you sure it is a good idea?!", step]];
//        [a beginSheetModalForWindow:[[NSApplication sharedApplication] mainWindow] modalDelegate:self didEndSelector:@selector(alertDidEnd:returnCode:contextInfo:) contextInfo:nil];
    }
    else
    {
//        ChangePosition(step);
        NewPosition(electrodeDepth + step);
        [self disableNewMovements];
    }
}

- (void) alertDidEnd:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
    if (returnCode==0) {
        NSInteger i =  [contextInfo intValue];
        NewPosition(electrodeDepth + i);
    } else {
        //Nothing!
    }
}

- (void) disableNewMovements
{
    [upButton setHidden:YES];
    [downButton setHidden:YES];
}

- (void) enableNewMovements
{
    [upButton setHidden:NO];
    [downButton setHidden:NO];    
}

- (IBAction) reopen:(id)sender
{
    close(motorPort);
    OpenStepSerial(stepperport);
    [self enableNewMovements];
}

- (IBAction) setMotorIdComboChanged:(id)sender
{
    self.motorId = [[sender stringValue] intValue];
    NSLog(@"Setting motorId to: %d", self.motorId);
    SetMotorId(self.motorId);
}

@end

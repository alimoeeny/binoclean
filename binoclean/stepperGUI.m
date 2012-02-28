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

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}


- (IBAction) uDriveInitialize
{
    OpenStepSerial(stepport);
}

- (void) uDriveButtonPress:(id) sender
{
    NSLog(@"uDrive: %@", [sender tag]);
    NewPosition(electrodeDepth + [sender tag]);
}

@end

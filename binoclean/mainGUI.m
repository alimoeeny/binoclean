//
//  mainGUI.m
//  binoclean
//
//  Created by Ali Moeeny on 7/26/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "mainGUI.h"

char * stimulus_names [];
Expt expt;

@implementation mainGUI

@synthesize playButton;
@synthesize informationTextField;
@synthesize commandHistoryTextField;
@synthesize commandTextField;
@synthesize stimulusValues;

- (id) init
{
    self = [super init];
    if (self)
    {
        self.stimulusValues = [[NSMutableArray alloc] init]; //  arrayWithObjects:@"none", @"cylinder", @"bar", @"sine", @"rds", @"grating", nil] retain];
        for (int i = 0; i < N_STIMULUS_TYPES; i ++)
            [self.stimulusValues addObject:[NSString stringWithUTF8String:stimulus_names[i]]];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateInfoText:) name:@"updateinfotext" object:nil];
    }
    return self;
}

- (void) updateInfoText:(NSNotification *) aNotification
{
    if([[informationTextField stringValue] rangeOfString:[[aNotification userInfo] valueForKey:@"text"]].location == NSNotFound)
    {
        int r = 14;
        NSString * s = [NSString stringWithFormat:@"%@\n%@", [[aNotification userInfo] valueForKey:@"text"], [informationTextField stringValue]];
        NSArray * c = [s componentsSeparatedByString:@"\n"];
        if([c count]>r)
        {
            int b = [s rangeOfString:[c objectAtIndex:r]].location;
            s = [s substringToIndex:b];
        }
        [informationTextField setStringValue:s];
    }
}

- (IBAction) elPosButton:(id)sender
{
    NSLog(@"Opening Electrode Position Control Window");
    stepperGUI * sw = [[stepperGUI alloc] initWithWindowNibName:@"stepperWindow"];
    NSPoint np;
    np.x = 100;
    np.y = 100;
    [[sw window] setFrameOrigin:np];
    [[sw window] makeKeyAndOrderFront:nil];
}

- (IBAction) penLogButton:(id)sender
{
    NSLog(@"Opening PenLog Window");
    penLog * pl = [[penLog alloc] initWithWindowNibName:@"penLogWindow"];
    NSPoint np;
    np.x = 250;
    np.y = 250;
    [[pl window] setFrameOrigin:np];
    [[pl window]makeKeyAndOrderFront:nil];
}


- (IBAction) FreePipeButton:(id)sender
{
    NSLog(@"Freeing the Pipe!");
    (@"Freeing Pipe for Matlab");
    notify("Freeing the Pipe for MATLAB");  
    sendNotification();
}

- (IBAction) brucesNumbers:(id)sender
{
    if ([[(NSTextField *)sender text] isEqualToString:@"Run"]) {
        NSLog(@"RUN");
    }
    else
        NSLog(@"DONT RUN");
}

- (IBAction) textCommand:(id)sender
{
    InterpretLine([[sender stringValue] UTF8String], &expt, 2);
    [commandHistoryTextField setStringValue:[NSString stringWithFormat:@"%@\n%@", 
                                              [sender stringValue],
                                              [commandHistoryTextField stringValue]]];
    NSLog(@"Command: %@", [sender stringValue]);
}

- (IBAction) acknowledgeTest:(id)sender
{
    acknowledge("Just testing!", 1234);
}

@end

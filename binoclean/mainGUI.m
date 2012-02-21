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
@synthesize commandHistoryTextField;
@synthesize commandTextField;
@synthesize stimulusValues;
@synthesize electrodeTypes;

- (id) init
{
    self = [super init];
    if (self)
    {
        self.stimulusValues = [[NSMutableArray alloc] init]; //  arrayWithObjects:@"none", @"cylinder", @"bar", @"sine", @"rds", @"grating", nil] retain];
        for (int i = 0; i < N_STIMULUS_TYPES; i ++)
            [self.stimulusValues addObject:[NSString stringWithUTF8String:stimulus_names[i]]];
        self.electrodeTypes = [[NSArray arrayWithObjects:@"AlphaOmega", @"MicroProbe+polyamide", @"FHC-PtIR", nil] retain];
    }
    return self;
}

- (void) playButtonPress:(id) sender
{
    if ([sender tag])
    {
        [sender setTag:0];
        [sender setState:NSOffState];        
        StopGo(0);
    }
    else
    {
        [sender setTag:1];
        [sender setState:NSOnState];
        StopGo(1);
    }
}

- (IBAction) brucesButton:(id)sender
{
    if ([[(NSButton *)sender title] isEqualToString:@"Run"]) {
        NSLog(@"RUN");
    }
    else
        NSLog(@"DONT RUN");
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
    [commandHistoryTextField setStringValue:[NSString stringWithFormat:@"%@\n%@", [commandHistoryTextField stringValue], [sender stringValue]]];
    NSLog(@"Command: %@", [sender stringValue]);
}


@end

//
//  binocleanAppDelegate.m
//  binoclean
//
//  Created by Ali Moeeny on 6/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "binocleanAppDelegate.h"

Stimulus *TheStim;

@implementation binocleanAppDelegate

@synthesize window = _window;
@synthesize monkeyWindow;
@synthesize runButton;


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (IBAction) runClicked:(id)sender
{
    NSLog(@"run clicked");
    monkeyWindow = [[NSWindow alloc]
                        initWithContentRect:CGRectMake(100, 100, 400, 300)
                        styleMask:NSBorderlessWindowMask
                        backing:NSBackingStoreBuffered
                        defer:YES
                        screen:[self.window screen]];
    [monkeyWindow setLevel:NSFloatingWindowLevel];
    [monkeyWindow setContentView:[[MonkeyGLView alloc] init]];
    [monkeyWindow setTitle:[self.window title]];
    [monkeyWindow makeKeyAndOrderFront:nil];
    
    
    init_stimulus(TheStim);
//	if(TheStim->next != NULL)
//		init_stimulus(TheStim->next);

//    TheStim = NewStimulus(NULL);
//	StimulusType(TheStim, STIM_GRATING);
//	tempstim = NewStimulus(NULL);
//	StimulusType(tempstim, STIM_BAR);

}


@end

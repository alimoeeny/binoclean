//
//  binocleanAppDelegate.h
//  binoclean
//
//  Created by Ali Moeeny on 6/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MonkeyGLView.h"
#import "commdefs.h"
#import "stimuli.h"
#import "sprotos.h"
#import <AppKit/AppKit.h>
#import <sys/types.h>
#import <sys/stat.h>
#import <sys/ioctl.h>
#import <sys/termios.h>

#define OUT_PIPE "/tmp/binocoutputpipe"
#define IN_PIPE "/tmp/binocinputpipe"

void WriteToOutputPipe(char * s);


@interface binocleanAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *_window;
    
    NSWindow * monkeyWindow;
    
    IBOutlet NSButton * runButton;
    NSTimer * mainTimer;

    NSFileHandle * inputPipe;
    NSString * inputLine;
    NSFileHandle * outputPipe;
}

@property (retain) IBOutlet NSWindow *window;
@property (nonatomic, retain) NSWindow * monkeyWindow;
@property (nonatomic, retain) IBOutlet NSButton * runButton;
@property (retain) NSTimer * mainTimer;

@property (retain) NSFileHandle * inputPipe;
@property (retain) NSString * inputLine;
@property (retain) NSFileHandle * outputPipe;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
- (IBAction) stopClicked:(id)sender;
- (IBAction) runClicked:(id)sender;
- (void) mainTimerFire:(NSTimer *)timer;
-(IBAction) setState:(id)Sender;

@end

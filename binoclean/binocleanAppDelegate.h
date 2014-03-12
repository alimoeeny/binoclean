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
#import "mainGUI.h"
#import "MonkeyWindow.h"
#import "HTTPServer.h"
#import "AliHttpConnection.h"

#ifndef __i386__
#import "MTRandom64.h"
#endif

#define OUT_PIPE "/tmp/binocoutputpipe"
#define IN_PIPE "/tmp/binocinputpipe"

void WriteToOutputPipe(NSString * ns);

@interface binocleanAppDelegate : NSObject <NSApplicationDelegate> {
    mainGUI *_window;
    MonkeyWindow * monkeyWindow;
    NSTimer * mainTimer;
    
    NSFileHandle * inputPipe;
    NSString * inputLine;
    NSFileHandle * outputPipe;

    HTTPServer *httpServer;
}

@property (retain) IBOutlet mainGUI *window;
@property (nonatomic, retain) NSWindow * monkeyWindow;
@property (retain) NSTimer * mainTimer;

@property (retain) NSFileHandle * inputPipe;
@property (retain) NSString * inputLine;
@property (retain) NSFileHandle * outputPipe;

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification;
- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
- (void) mainTimerFire:(NSTimer *)timer;


@end

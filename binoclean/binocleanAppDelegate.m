//
//  binocleanAppDelegate.m
//  binoclean
//
//  Created by Ali Moeeny on 6/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "binocleanAppDelegate.h"

Stimulus *TheStim;

void TESTRefresh()
{
    [[NSOpenGLContext currentContext] flushBuffer];
}

@implementation binocleanAppDelegate

@synthesize window = _window;
@synthesize monkeyWindow;
@synthesize runButton;
@synthesize mainTimer;
@synthesize counter;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (IBAction) stopClicked:(id)sender
{
    StopGo(0);
}

- (IBAction) runClicked:(id)sender
{
    NSLog(@"run clicked");
    monkeyWindow = [[NSWindow alloc]
                        initWithContentRect:NSRectFromCGRect(CGRectMake(0, -200, 1280, 1024))
                        styleMask:NSBorderlessWindowMask
                        backing:NSBackingStoreBuffered
                        defer:YES
                        screen:[self.window screen]];
    [monkeyWindow setLevel:NSFloatingWindowLevel];
    [monkeyWindow setContentView:[[MonkeyGLView alloc] init]];
    [monkeyWindow setTitle:[self.window title]];
    [monkeyWindow makeKeyAndOrderFront:nil];
    //[[monkeyWindow contentView] enterFullScreenMode:[NSScreen mainScreen] withOptions:nil]; 
    mainTimer = [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(mainTimerFire:) userInfo:nil repeats:YES];
    
    counter = [NSNumber numberWithInt:0];
    StopGo(1);
    
}

- (void) mainTimerFire:(NSTimer *)timer
{
    //counter = [NSNumber numberWithInt:1 + [counter intValue]];
    //NSLog(@" Counter: %d", [counter intValue]);
    //printf("counter %d\r\n",[counter intValue]);
    NSLog(@"ali");
    glOrtho(-640.0f, 640.0f, -512.0f, 512.0f, 0, 0);
    event_loop();
    [[monkeyWindow contentView] setNeedsDisplay:YES];
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //[[NSOpenGLContext currentContext] flushBuffer];
//    NSOpenGLContext.currentContext()] flushBuffer];
    
}

@end

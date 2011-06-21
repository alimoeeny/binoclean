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
//#import <OpenGL/OpenGL.h>
#import <AppKit/AppKit.h>

void TESTRefresh();


@interface binocleanAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *_window;
    
    NSWindow * monkeyWindow;
    
    IBOutlet NSButton * runButton;
    NSTimer * mainTimer;
    NSNumber * counter;
}

@property (retain) IBOutlet NSWindow *window;
@property (nonatomic, retain) NSWindow * monkeyWindow;
@property (nonatomic, retain) IBOutlet NSButton * runButton;
@property (retain) NSTimer * mainTimer;
@property (retain) NSNumber * counter;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
- (IBAction) stopClicked:(id)sender;
- (IBAction) runClicked:(id)sender;
- (void) mainTimerFire:(NSTimer *)timer;

@end

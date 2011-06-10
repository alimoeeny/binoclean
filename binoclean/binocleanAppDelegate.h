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

@interface binocleanAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *_window;
    
    NSWindow * monkeyWindow;
    
    IBOutlet NSButton * runButton;
    
}

@property (strong) IBOutlet NSWindow *window;
@property (nonatomic, retain) NSWindow * monkeyWindow;
@property (nonatomic, retain) IBOutlet NSButton * runButton;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
- (IBAction) runClicked:(id)sender;

@end

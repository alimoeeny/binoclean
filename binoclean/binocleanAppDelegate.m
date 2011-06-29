//
//  binocleanAppDelegate.m
//  binoclean
//
//  Created by Ali Moeeny on 6/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "binocleanAppDelegate.h"

Stimulus *TheStim;
Expt expt;
int stimstate;
BOOL dataReadyInInputPipe;
char * inputLineChars = NULL;
int winsiz [2];
int outPipe = 0;

void ReadInputPipe()
{
    if(dataReadyInInputPipe)
    {
        InterpretLine(inputLineChars, &expt, 0);
        //free(inputLineChars);
        inputLineChars = NULL;
        dataReadyInInputPipe = NO;
    }
}

void WriteToOutputPipe(char * s)
{
    if (outPipe==0)
    {
        outPipe = open(OUT_PIPE, O_WRONLY);
    }
    write(outPipe, s, strlen(s));
    write(outPipe, 0x3, sizeof(char));
    write(outPipe, 0x4, sizeof(char));
    write(outPipe, 0x19, sizeof(char));
    write(outPipe, 0x17, sizeof(char));
    ioctl(outPipe,TCOFLUSH);
    NSLog(@"Output Pipe: %s", s);
    //close(outPipe);
}

@implementation binocleanAppDelegate

@synthesize window = _window;
@synthesize monkeyWindow;
@synthesize runButton;
@synthesize mainTimer;

@synthesize inputPipe;
@synthesize inputLine;
@synthesize outputPipe;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    unlink(IN_PIPE);
    if (mkfifo(IN_PIPE, S_IRUSR|S_IWUSR|S_IWGRP|S_IRGRP|S_IROTH) == -1) {
        NSLog(@"Can't create the pipe");
        [NSAlert alertWithMessageText:@"Can't create the input pipe" defaultButton:@"Okay" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Can't create the input pipe. You can try to remove it manually by rm /tmp/binocinputpipe "];   
        abort();
    }

    unlink(OUT_PIPE);
    if (mkfifo(OUT_PIPE, S_IRUSR|S_IWUSR|S_IWGRP|S_IRGRP|S_IROTH) == -1) {
        NSLog(@"Can't create the output pipe");
        [NSAlert alertWithMessageText:@"Can't create the output pipe" defaultButton:@"Okay" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Can't create the output pipe. You can try to remove it manually by rm /tmp/binocoutputpipe "];   
        abort();
    }

    open(IN_PIPE, O_RDWR); 
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(dataReadyToRead:) name:NSFileHandleReadCompletionNotification object:nil];
    inputPipe = [[NSFileHandle fileHandleForReadingAtPath:@IN_PIPE] retain];
    [inputPipe readInBackgroundAndNotify];
    outputPipe = [[NSFileHandle fileHandleForWritingAtPath:@IN_PIPE] retain];
    [outputPipe writeData:[@"binocstart" dataUsingEncoding:NSASCIIStringEncoding]];
    
    NSLog(@"");
    monkeyWindow = [[NSWindow alloc]
                    initWithContentRect:NSRectFromCGRect(CGRectMake(0, 0, winsiz[0]*2, winsiz[1]*2))
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
    
    ReadExptFile("/local/demo/stims/bgc.txt", 1, 0, 0);
    StartRunning();
    StopGo(1);
    WriteToOutputPipe("Ready\n");

    
}

- (void) dataReadyToRead:(NSNotification *) notification
{
    NSString * s = [[NSString alloc] initWithData:[[notification userInfo] objectForKey:NSFileHandleNotificationDataItem] encoding:NSASCIIStringEncoding];
    NSLog(@"Input Pipe: %@",s);
    if (inputLineChars)
        inputLineChars = [[NSString stringWithFormat:@"%s%s", inputLineChars, [s UTF8String]] UTF8String];
    else
        inputLineChars = [s UTF8String];
    dataReadyInInputPipe = YES;
    [inputPipe readInBackgroundAndNotify];
    printf(inputLineChars);
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (IBAction) stopClicked:(id)sender
{
    StopGo(0);
}

-(IBAction) setState:(id)Sender
{
    runexpt(NULL,NULL,NULL);
}


- (IBAction) runClicked:(id)sender
{
//    NSLog(@"run clicked");
//    monkeyWindow = [[NSWindow alloc]
//                        initWithContentRect:NSRectFromCGRect(CGRectMake(0, 0, winsiz[0]*2, winsiz[1]*2))
//                        styleMask:NSBorderlessWindowMask
//                        backing:NSBackingStoreBuffered
//                        defer:YES
//                        screen:[self.window screen]];
//    [monkeyWindow setLevel:NSFloatingWindowLevel];
//    [monkeyWindow setContentView:[[MonkeyGLView alloc] init]];
//    [monkeyWindow setTitle:[self.window title]];
//    [monkeyWindow makeKeyAndOrderFront:nil];
//    //[[monkeyWindow contentView] enterFullScreenMode:[NSScreen mainScreen] withOptions:nil]; 
//    mainTimer = [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(mainTimerFire:) userInfo:nil repeats:YES];
//    
//    ReadExptFile("/local/demo/stims/bgc.txt", 1, 0, 0);
//    StartRunning();
//    StopGo(1);
    
}

- (void) mainTimerFire:(NSTimer *)timer
{
    //counter = [NSNumber numberWithInt:1 + [counter intValue]];
    //NSLog(@" Counter: %d", [counter intValue]);
    //printf("counter %d\r\n",[counter intValue]);
    //NSLog(@"ali");
    //glOrtho(-640.0f, 640.0f, -512.0f, 512.0f, 0, 0);
    //glOrtho(-winsiz[0], winsiz[0], -winsiz[1], winsiz[1], 0, 0);

    event_loop();
    //[[monkeyWindow contentView] setNeedsDisplay:YES];
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //[[NSOpenGLContext currentContext] flushBuffer];
//    NSOpenGLContext.currentContext()] flushBuffer];
    
}

- (void) dealloc
{
    if (outPipe!=0) {
        close(outPipe);
        //outPipe = 0;
    }
}

@end

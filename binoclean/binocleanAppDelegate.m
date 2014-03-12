
//
//  binocleanAppDelegate.m
//  binoclean
//
//  Created by Ali Moeeny on 6/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "binocleanAppDelegate.h"

Expt expt;
//int stimstate;
BOOL dataReadyInInputPipe;
char * inputLineChars = NULL;
int winsiz [2];
int winpos [2];
int fullscreenmode;
extern int useDIO;
int freeToGo = 1;

static NSColor * textColor;
static NSColor * textBGColor;

int outPipe = 0;
static NSMutableArray * inputPipeBuffer;
NSString * outputPipeBuffer;
NSMutableDictionary *bold12Attribs;


#ifndef __i386__
MTRandom64 * randEngine;
MTRandom64 * drandEngine;
#endif

void quit_binoc()
{
    [[NSApplication sharedApplication] terminate:nil];
}

void acknowledge(char * a ,int b)
{
    NSLog(@"Acknowledge! %s", a);
    [[NSNotificationCenter defaultCenter] postNotificationName:@"updateinfotext" object:nil userInfo:[NSDictionary dictionaryWithObject:[NSString stringWithUTF8String:a] forKey:@"text"]];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"updatecommandhistory" object:nil userInfo:[NSDictionary dictionaryWithObject:[NSString stringWithUTF8String:a] forKey:@"text"]];
    if([NSApplication sharedApplication])
        if ([[NSApplication sharedApplication] windows])
            if([[[NSApplication sharedApplication] windows] count]>0)
            {
                [NSApp activateIgnoringOtherApps:YES];
                NSSound * snd = [NSSound soundNamed:@"Ping.aiff"];
                [snd play];
                NSAlert * acknowledgeAlert = [[NSAlert alloc] init];
                [acknowledgeAlert setMessageText:@"Acknowledge it!"];
                [acknowledgeAlert addButtonWithTitle:@"I know!"];
                [acknowledgeAlert setInformativeText:[NSString stringWithFormat:@"%@ \n", [NSString stringWithUTF8String:a]]];
                NSWindow * topwin = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
                NSLog(@"topwin:%@, wincount:%d", [topwin title], [[[NSApplication sharedApplication] windows] count]);
                [acknowledgeAlert beginSheetModalForWindow:topwin modalDelegate:nil didEndSelector:nil contextInfo:nil];
            }
}

void displayOnMonkeyView(char *s, int x, int y)
{
    if(!textColor)
        textColor = [NSColor colorWithCalibratedRed:0.5f green:1.0f blue:0.5f alpha:1.0f];
    if(!textBGColor)
       textBGColor = [NSColor colorWithCalibratedRed:0.5f green:0.0f blue:0.5f alpha:1.0f];

    if (!bold12Attribs) {
        bold12Attribs = [[NSMutableDictionary dictionary] retain];
        [bold12Attribs setObject: [NSFont fontWithName: @"Helvetica" size: 20.0f] forKey: NSFontAttributeName];
        [bold12Attribs setObject:textColor  forKey: NSForegroundColorAttributeName];
    }
    GLString * messageTexture = [[GLString alloc] initWithString:[NSString stringWithUTF8String:s]
                                                  withAttributes:bold12Attribs
                                                   withTextColor:textColor
                                                    withBoxColor:textBGColor
                                                 withBorderColor:textColor];
    if (x && y)
        [messageTexture drawAtPoint:NSMakePoint(x, y)];        
    else
        [messageTexture drawAtPoint:NSMakePoint(-500, -450)];
    [messageTexture release];
}


void updateInfoText(char *s)
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"updateinfotext" object:nil userInfo:[NSDictionary dictionaryWithObject:[NSString stringWithUTF8String:s] forKey:@"text"]];
}

void notifyPositionChange(int newPosition)
{
    double delayInSeconds = 0.0;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        [[NSNotificationCenter defaultCenter] postNotificationName:@"electrodeposition" object:nil userInfo:[NSDictionary dictionaryWithObject:[NSString stringWithFormat:@"%d", newPosition] forKey:@"newposition"]]; 
    });
}

void sendNotification()
{
    NSString * s = [NSString stringWithFormat:@"SENDING%06d\n", [outputPipeBuffer length]];
    //    WriteToOutputPipe(s);
    if ([outputPipeBuffer length]>0) {
        if (expt.verbose)
        NSLog(@"%d : %@", strlen([outputPipeBuffer UTF8String]), outputPipeBuffer);
        WriteToOutputPipe([NSString stringWithFormat:@"%@%@", s, outputPipeBuffer]);
        outputPipeBuffer = [[[NSString alloc] init] retain];
    }
    else
        WriteToOutputPipe(s);        
}

void ReadInputPipe()
{
    if(dataReadyInInputPipe)
    {
        dataReadyInInputPipe = NO;
        for (int i = 0; i < inputPipeBuffer.count; i++)
        {
            if(![(NSString *)[inputPipeBuffer objectAtIndex:i] isEqualToString:@""]){
                inputLineChars = [[inputPipeBuffer objectAtIndex:i] UTF8String];
                if (strncmp(inputLineChars, "whatsup", 7) == 0) 
                    sendNotification();
                else if (strncmp(inputLineChars, "eventpause", 10) == 0) 
                {
                    NSLog(@"PAUSED");
                    freeToGo = 0;
                }
                else if (strncmp(inputLineChars, "eventcontinue", 10) == 0) 
                {
                    NSLog(@"FREE TO GO Again");
                    freeToGo = 1;
                }
                else  {
                    //                printf("Interpret:%s",inputLineChars);
                    InterpretLine(inputLineChars, &expt, 2);
                }
            }
        }
        inputLineChars = NULL;
        [inputPipeBuffer removeAllObjects];
    }
}

void WriteToOutputPipe(NSString * ns)
{
    if (outPipe==0)
    {
        outPipe = open(OUT_PIPE, O_WRONLY);
    }
    dispatch_queue_t q = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, NULL);
    dispatch_async(q, ^{
        write(outPipe, [ns UTF8String], strlen([ns UTF8String]));
        ioctl(outPipe,TCOFLUSH);
        if (expt.verbose)
        NSLog(@"Output Pipe:%d: %s", strlen([ns UTF8String]), [ns UTF8String]);
    });
    //close(outPipe);
}

void notify(char * s)
{
    if (!outputPipeBuffer) {
        outputPipeBuffer = [[NSString alloc] init];
    }
    outputPipeBuffer = [[NSString stringWithFormat:@"%@%s", outputPipeBuffer, s] retain];
}


#pragma mark random
void randinit(int seed) // that sets the seed
{
    randEngine = [[MTRandom64 alloc] initWithSeed:seed];
}

uint64_t random_l()  //that returns  random int
{
    if (!randEngine) {
        randEngine = [[MTRandom64 alloc] initWithSeed:(int)[[NSDate date] timeIntervalSince1970]];
    }
    uint64_t q = [randEngine randomUInt64];
    return q;
}

double drand64()  //that returns a float between 0 and 1
{
    if (!drandEngine) {
        drandEngine = [[MTRandom64 alloc] initWithSeed:(int)[[NSDate date] timeIntervalSince1970]];
    }
    double   t = [drandEngine randomDouble0To1Exclusive];
    return t;
}

void drandinit(int seed ) //that sets the state for the drand generator.
{
    drandEngine = [[MTRandom64 alloc] initWithSeed:seed];
}


int  processUIEvents()
{
    // returns 0 if nothing has happened that is not processed yet
    // returns 1 for left and 2 for right mouse button events
    // should not return 3 but if it does it means some other event!
    //in future versions we may want to keep the time everytime it is called and return only the new events
    int result = 0; // 0 for nothing new happend
    NSDate * now = [[NSDate date] retain];
    NSEvent * e = [[[NSApplication sharedApplication] nextEventMatchingMask:NSLeftMouseDownMask | NSRightMouseDownMask
                                                                 untilDate:now
                                                                    inMode:NSEventTrackingRunLoopMode
                                                                   dequeue:YES] retain];
    if (e)
        if ([e type]==NSLeftMouseUp | [e type]==NSLeftMouseDown) {
            result = Button1; // left mouse event (up or down)
        } else if ([e type]==NSRightMouseUp | [e type]==NSRightMouseDown){
            result = Button3; // right mouse event (up or down)
        }
        else
            result = 4; // something else happend

    [e release];
    [now release];
    return result;

    //    [[[NSApplication sharedApplication] delegate] performSelectorOnMainThread:@selector(testselector) // no trailing :
    //                                                        withObject:nil
    //                                                     waitUntilDone:YES
    //                                                             modes:@[NSEventTrackingRunLoopMode]];
}


@implementation binocleanAppDelegate

@synthesize window = _window;
@synthesize monkeyWindow;
@synthesize mainTimer;

@synthesize inputPipe;
@synthesize inputLine;
@synthesize outputPipe;

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
    NSLog([[[NSFileManager alloc] init] currentDirectoryPath]);
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"ApplePersistenceIgnoreState"];
    
    if(useDIO)
        printf("Starting DIO\n");
	/* Try twice - it sometimes fails */
	if(useDIO && DIOInit() < 0){
        acknowledge("Can't Open DIO - Restart Binoc","/bgc/bgc/c/binoc/help/DIOerr");
        fprintf(stderr,"Use binoc -noDIO to ingore this error\n");
        DIOClose();
        exit(1);
	}
    else if (useDIO)
    {
        DIOWriteBit(0,0);
        DIOWriteBit(0,1);
//        DIOWrite(0xF);
    }
    
    BOOL networkMode = YES;
    if (networkMode){
        httpServer = [[HTTPServer alloc] init];
        [httpServer setPort:12345];
        [httpServer setConnectionClass:[AliHttpConnection class]];
        // Serve files from our embedded Web folder
        NSString *webPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Web"];
        NSLog(@"Setting document root: %@", webPath);

        [httpServer setDocumentRoot:webPath];


        NSError *error = nil;
        if(![httpServer start:&error])
        {
            NSLog(@"Error starting HTTP Server: %@", error);
            abort();
        }

    }else{
        unlink(IN_PIPE);
        if (mkfifo(IN_PIPE, S_IRWXU|S_IRWXG|S_IRWXO) == -1) {
            NSLog(@"Can't create the pipe");
            //        [NSAlert alertWithMessageText:@"Can't create the input pipe" defaultButton:@"Okay" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Can't create the input pipe. You can try to remove it manually by rm /tmp/binocinputpipe "];
            abort();
        }

        unlink(OUT_PIPE);
        if (mkfifo(OUT_PIPE, S_IRWXU|S_IRWXG|S_IRWXO) == -1) {
            NSLog(@"Can't create the output pipe");
            //        [NSAlert alertWithMessageText:@"Can't create the output pipe" defaultButton:@"Okay" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Can't create the output pipe. You can try to remove it manually by rm /tmp/binocoutputpipe "];
            abort();
        }

        open(IN_PIPE, O_RDWR);
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(dataReadyToRead:) name:NSFileHandleReadCompletionNotification object:nil];
        inputPipe = [[NSFileHandle fileHandleForReadingAtPath:@IN_PIPE] retain];
        [inputPipe readInBackgroundAndNotify];

        open(OUT_PIPE, O_RDWR);
        outputPipe = [[NSFileHandle fileHandleForWritingAtPath:@IN_PIPE] retain];
        [outputPipe writeData:[@"binocstart" dataUsingEncoding:NSASCIIStringEncoding]];
    }
    // if wisize read from binoc.setup is 0,0 then do a fullscreen otherwise use the winsize
    CGRect r;
    if (fullscreenmode==0)
    {
        r = CGRectMake(winpos[0], winpos[1], winsiz[0]*2, winsiz[1]*2);
        monkeyWindow = [[MonkeyWindow alloc] initWithContentRect:NSRectFromCGRect(r)
                                                   styleMask:NSBorderlessWindowMask
                                                     backing:NSBackingStoreBuffered
                                                       defer:YES
                                                      screen:[self.window screen]];
        [monkeyWindow setLevel:NSFloatingWindowLevel];
        MonkeyGLView * mview = [[MonkeyGLView alloc] init];
        [monkeyWindow setContentView:mview];
        //[monkeyWindow setTitle:[self.window title]];
        [monkeyWindow makeKeyAndOrderFront:nil];
        [monkeyWindow invalidateCursorRectsForView:mview];
    }
    else
    {   
        r = CGRectMake(0, 0, 0, 0);
        monkeyWindow = [[MonkeyWindow alloc] initWithContentRect:NSRectFromCGRect(r)
                                                   styleMask:NSBorderlessWindowMask
                                                     backing:NSBackingStoreBuffered
                                                       defer:YES
                                                      screen:[self.window screen]];
        [monkeyWindow setLevel:NSFloatingWindowLevel];
        [monkeyWindow setContentView:[[MonkeyGLView alloc] init]];
        //[monkeyWindow setTitle:[self.window title]];
        [monkeyWindow makeKeyAndOrderFront:nil];
        
        [[monkeyWindow contentView] enterFullScreenMode:[[NSScreen screens] objectAtIndex:fullscreenmode - 1] withOptions:[NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], NSFullScreenModeAllScreens, [NSNumber numberWithBool:NO], NSFullScreenModeApplicationPresentationOptions, nil]]; 
        NSRect screenFrame = [[[NSScreen screens] objectAtIndex:fullscreenmode -1] frame];
//        NSLog(@"%@",screenFrame);
    }
        
    mainTimer = [NSTimer scheduledTimerWithTimeInterval:0.0100 target:self selector:@selector(mainTimerFire:) userInfo:nil repeats:YES];
    
    StartRunning();
    WriteToOutputPipe(@"SENDINGstart1\n");
}

- (void) dataReadyToRead:(NSNotification *) notification
{
    NSString * s = [[NSString alloc] initWithData:[[notification userInfo] objectForKey:NSFileHandleNotificationDataItem] encoding:NSASCIIStringEncoding];
    if (expt.verbose)
    NSLog(@"Input Pipe: %@", s);
    if (!inputPipeBuffer) {
        inputPipeBuffer = [[NSMutableArray alloc] init];
    }
    NSArray * sLines = [s componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r\n"]];
    for (int i = 0; i < [sLines count]; i++) {
        [inputPipeBuffer addObject:[sLines objectAtIndex:i]];
    }
    dataReadyInInputPipe = YES;
    [inputPipe readInBackgroundAndNotify];
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void) mainTimerFire:(NSTimer *)timer
{
    static struct timeval atime, btime;
    float val,aval,bval;
    int stimstate = 0;
    time_t tval;

    gettimeofday(&btime,NULL);
    aval = timediff(&btime,&atime); //time since last call

  ReadInputPipe();
    gettimeofday(&atime,NULL);
    bval = timediff(&atime,&btime); // time taken in ReadInputPipe
    if (freeToGo) {
       stimstate = event_loop(bval);
    }
    else
    {
//        fprintf(stderr,"waiting for freetogo\n");       
    }    
    gettimeofday(&atime,NULL);
    val = timediff(&atime,&btime);
    if (aval > 0.03 || bval > 0.001){
        tval = time(NULL);
        fprintf(stderr,"#############Long delay (%.8f,%.4f,%.6f) at %s in Timer\n",val,aval,bval,ctime(&tval));
    }
    else if (aval > 0.05 && freeToGo == 0){
        fprintf(stderr,"#######Long delay when free (%.8f,%.4f) in Timer\n",val,aval,bval);
    }
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    NSLog(@"Going down, down ...");

    freeToGo = 0;
    if(expt.st->mode & EXPTPENDING){
        expt_over(1);
    }
    //closeserial(0)  prob not necessary any more
    DIOClose();
    if (outPipe!=0) {
        close(outPipe);
    }
    unlink(IN_PIPE);
    unlink(OUT_PIPE);

    NSLog(@"Gone!");
    return NSTerminateNow;
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename
{
    NSLog(@"Opening file: %@", filename);
    if ([[self monkeyWindow] isKeyWindow]) {
        ReadExptFile([filename UTF8String], 1, 1, 1);
    }
    else {
        double delayInSeconds = 2.5;
        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
        dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
            ReadExptFile([filename UTF8String], 1, 1, 1);
        });
    }
    return YES;
}

@end

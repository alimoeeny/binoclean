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
MTRandom * randEngine;
MTRandom * drandEngine;
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
                [acknowledgeAlert setInformativeText:[NSString stringWithFormat:@"%@ \n %d ", [NSString stringWithUTF8String:a],b]];
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
    double delayInSeconds = 1.0;
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
        dataReadyInInputPipe = NO;
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
    randEngine = [[MTRandom alloc] initWithSeed:seed];
}

unsigned int random_l()  //that returns  random int
{
    if (!randEngine) {
        randEngine = [[MTRandom alloc] initWithSeed:(int)[[NSDate date] timeIntervalSince1970]];
    }
    uint32_t q = [randEngine randomUInt32];
    return q;
}

double drand64()  //that returns a float between 0 and 1
{
    if (!drandEngine) {
        drandEngine = [[MTRandom alloc] initWithSeed:(int)[[NSDate date] timeIntervalSince1970]];
    }
    double   t = [drandEngine randomDouble0To1Exclusive];
    return t;
}

void drandinit(int seed ) //that sets the state for the drand generator.
{
    drandEngine = [[MTRandom alloc] initWithSeed:seed];
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
    //    NSLog(@"%@",screenFrame);
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
    gettimeofday(&btime,NULL);
    aval = timediff(&btime,&atime);

  ReadInputPipe();
    gettimeofday(&atime,NULL);
    bval = timediff(&atime,&btime);
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
        fprintf(stderr,"#############Long delay (%.8f,%.4f,%.6f) in Timer\n",val,aval,bval);
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

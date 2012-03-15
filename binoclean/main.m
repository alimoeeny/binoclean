//
//  main.m
//  binoclean
//
//  Created by Ali Moeeny on 6/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "commdefs.h"
#import "stimuli.h"
#import "protos.h"
#import <sys/time.h>
#include <sys/resource.h>


int binocmain(int argc, char * argv[]);
char * VERSION_NUMBER;


// to prevent screen saver and system going to sleep
void caffeineTimerCallback(CFRunLoopTimerRef timer, void *info)
{
    UpdateSystemActivity(OverallAct);
}

int main(int argc, char *argv[])
{
    setpriority(PRIO_PROCESS, 0, -10);
    
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
    VERSION_NUMBER = [version UTF8String];    

    CFRunLoopTimerContext context = { 0, NULL, NULL, NULL, NULL };    
    CFRunLoopTimerRef caffeineTimer = CFRunLoopTimerCreate(NULL, CFAbsoluteTimeGetCurrent(), 30, 0, 0, caffeineTimerCallback, &context);
    if (caffeineTimer != NULL); {
        CFRunLoopAddTimer(CFRunLoopGetCurrent(), caffeineTimer, kCFRunLoopCommonModes);
    }
    
    binocmain(argc, argv);
    
    return NSApplicationMain(argc, (const char **)argv);
    
    [version release];
    
    CFRunLoopTimerInvalidate(caffeineTimer);
    CFRelease(caffeineTimer);
}

//
//  penLog.m
//  binoclean
//
//  Created by Ali Moeeny on 2/28/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "penLog.h"

extern char *vstrings[];
extern char *userstrings;
extern char *electrodestrings[100];

int GetLastPen(){
    int pen = 1;
    
    if ([[NSUserDefaults standardUserDefaults] valueForKey:@"penNumber"]) {
        pen = [[NSUserDefaults standardUserDefaults] valueForKey:@"penNumber"];
    }
    else {
        char buf[BUFSIZ];
        FILE *fd;
    
        if((fd = fopen("lastpen","r")) != NULL){
            fscanf(fd,"%d",&pen);
            fclose(fd);
            }
        do{
            sprintf(buf,"pen%d.log",pen);
            if((fd = fopen(buf,"r")) != NULL){
                fclose(fd);
                pen++;
            }
        }while(fd != NULL);
    }
    return(pen-1);
}

@implementation penLog

@synthesize userList;
@synthesize electrodeTypes;
@synthesize tubeProtrusion;
@synthesize uDriveX;
@synthesize uDriveY;
@synthesize impedance;
@synthesize adapter;
@synthesize penNumber;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        self.userList = [[NSArray arrayWithObjects:@"Ali", @"Bruce", @"Adrian", @"Guest", nil] retain];
        NSMutableString * s = [NSMutableString stringWithString:@""];
        int i = 0;
        while (electrodestrings[i] != NULL) {
            [s appendString:[NSString stringWithUTF8String:electrodestrings[i]]];
            [s appendString:@"\n"];
            i++;
        }
        self.electrodeTypes = [s componentsSeparatedByString:@"\n"]; //[[NSArray arrayWithObjects:@"AlphaOmega", @"MicroProbe+polyamide", @"FHC-PtIR", nil] retain];
 
        penNumber = [NSString stringWithFormat:@"%d", GetLastPen()];
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

- (IBAction) applyAndSave:(id)sender
{
    NSLog(@"Applying and saving!");
    OpenPenetrationLog([penNumber intValue]);
    [[NSUserDefaults standardUserDefaults] setValue:penNumber forKey:@"penNumber"];
}

@end

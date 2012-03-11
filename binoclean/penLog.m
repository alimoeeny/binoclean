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
int UpdatePenInfo_Ali(float _penXpos, float _penYpos, int _angleAdapter, int _hemisphere, int _userid, int _protrudemm, int _electrodeid, int _impedance, int pennumber);

int GetLastPen(){
    int pen = 1;
    
    if ([[NSUserDefaults standardUserDefaults] valueForKey:@"pennumber"]) {
        pen = [[[NSUserDefaults standardUserDefaults] valueForKey:@"pennumber"] intValue];
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
        pen --;
    }
    return(pen);
}

@implementation penLog

@synthesize userList;
@synthesize electrodeTypes;
@synthesize electrodeValue;
@synthesize tubeProtrusion;
@synthesize uDriveX;
@synthesize uDriveY;
@synthesize impedance;
@synthesize adapter;
@synthesize hemisphereS;
@synthesize hemisphere;
@synthesize penNumber;
@synthesize monkeyNames;
@synthesize monkeyValue;
@synthesize userValue;
@synthesize actualPenLog;

@synthesize penlogloadTimer;

@synthesize monkeySelect;
@synthesize actualPenLogTextField;

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
 
        self.penNumber = [NSString stringWithFormat:@"%d", GetLastPen()];
        
        self.monkeyNames = [[NSArray arrayWithObjects:@"UnKnown", @"Icarus", @"Daedalus", @"JBE", @"Lemieux", @"BigGuy", nil] retain];
        
        self.monkeyValue = @"UnKnown";
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"monkeyname"]) {
            self.monkeyValue = [NSString stringWithFormat:@"%@", [[NSUserDefaults standardUserDefaults] valueForKey:@"monkeyname"]];
        }
        
        self.uDriveX = 0;
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"udrivex"]) { self.uDriveX = [[NSUserDefaults standardUserDefaults] valueForKey:@"udrivex"]; }
        self.uDriveY = 0;
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"udrivey"]) { self.uDriveY = [[NSUserDefaults standardUserDefaults] valueForKey:@"udrivey"]; }
        self.hemisphereS = [[NSArray arrayWithObjects:@"UnKnown", @"Right", @"Left", nil] retain];
        self.hemisphere = @"UnKnown";
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"hemisphere"]) { self.hemisphere = [[NSUserDefaults standardUserDefaults] valueForKey:@"hemisphere"]; }
        self.electrodeValue = @"Not Set";
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"electrodevalue"]) { self.electrodeValue = [[NSUserDefaults standardUserDefaults] valueForKey:@"electrodevalue"]; }
        self.impedance = 0;
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"impedance"]) { self.impedance = [[NSUserDefaults standardUserDefaults] valueForKey:@"impedance"]; }
        self.tubeProtrusion = 0;
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"tubeprotrude"]) { self.tubeProtrusion = [[NSUserDefaults standardUserDefaults] valueForKey:@"tubeprotrude"];}
        self.adapter = @"none";
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"adapter"]) { self.adapter = [[NSUserDefaults standardUserDefaults] valueForKey:@"adapter"];}
        self.userValue = @"Guest";
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"user"]) { self.userValue = [[NSUserDefaults standardUserDefaults] valueForKey:@"user"];}
        self.actualPenLog = @"testing 1 2 3 ...";
        if ([[NSUserDefaults standardUserDefaults] valueForKey:@"actualpenlog"]) { self.actualPenLog = [[NSUserDefaults standardUserDefaults] valueForKey:@"actualpenlog"];}
        
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updatePenLog:) name:@"updatepenlog" object:nil];
                
        NSLog(@" I need to implement the setters for these so that I can also set the ");
        NSLog(@" values bruce uses to write the pen log to disk  ");
        NSLog(@" Also I need to periodically (or on notification) read the actual penlog from disk without interfering with the write process  ");
        NSLog(@" Also I can summarize the log file to make it more legible ");

    }
    
    return self;
}

- (void) loadPenLogTimerFire:(NSTimer*)theTimer
{
//    NSLog(@"checking for the pen log!");
    NSError * error = nil;
    self.actualPenLog = [NSString stringWithContentsOfFile:[NSString stringWithFormat:@"./pen%d.log", [self.penNumber intValue]] encoding:NSUTF8StringEncoding error:&error];
//    NSLog(@"%@",self.actualPenLog);
    [actualPenLogTextField setStringValue:self.actualPenLog];
    if (error) {
        NSLog(@"ERROR: %@", [error localizedDescription]);
    }
    else
    {
        [self.actualPenLogTextField setStringValue: self.actualPenLog];
    }
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    if(!self.penlogloadTimer)
    {
        self.penlogloadTimer = [NSTimer scheduledTimerWithTimeInterval:2 target:self selector:@selector(loadPenLogTimerFire:) userInfo:nil repeats:YES];
    }
    self.actualPenLog = @"I am here";
    [self.actualPenLogTextField setTextColor:[NSColor magentaColor]];
    [self.actualPenLogTextField setStringValue:@"I am here!!"];
}


- (IBAction) applyAndSave:(id)sender
{
    NSLog(@"Applying and saving!");
    int anglA = 0; if (![self.adapter isEqualToString:@""]) anglA = 1;
    int hemis = 0; if ([self.hemisphere isEqualToString:@"Right" ]) hemis = 1;
    int userid = 0; userid = [self.userList indexOfObject:self.userValue];
    int protrudemm = [self.tubeProtrusion intValue];
    int electrodeid = 0; electrodeid = [self.electrodeTypes indexOfObject:self.electrodeValue];
    int _impedance = [self.impedance intValue];
    int pennumber = [self.penNumber intValue];
    UpdatePenInfo_Ali([self.uDriveX floatValue], [self.uDriveY floatValue], anglA, hemis, userid, protrudemm, electrodeid, _impedance, pennumber);
    
         if ([self.monkeyValue isEqualToString:@"Icarus"])   { [[[NSFileManager alloc] init] changeCurrentDirectoryPath:@"/local/icarus"];}
    else if ([self.monkeyValue isEqualToString:@"Daedalus"]) { [[[NSFileManager alloc] init] changeCurrentDirectoryPath:@"/local/dae"];}
    else if ([self.monkeyValue isEqualToString:@"JBE"])      { [[[NSFileManager alloc] init] changeCurrentDirectoryPath:@"/local/jbe"];}
    else if ([self.monkeyValue isEqualToString:@"Lemieux"])   { [[[NSFileManager alloc] init] changeCurrentDirectoryPath:@"/local/lem"];}
    else { 
        acknowledge("We need to know which monkey we are penetrating!\r Don't we?! \r \r NOTHING SAVED !");
        return;
    }
    chdir("/local/lem");
    OpenPenetrationLog([self.penNumber intValue]);
    [[NSUserDefaults standardUserDefaults] setValue:self.penNumber forKey:@"pennumber"];
    [[NSUserDefaults standardUserDefaults] setValue:self.monkeyValue forKey:@"monkeyname"];
    [[NSUserDefaults standardUserDefaults] setValue:self.uDriveX forKey:@"udrivex"];
    [[NSUserDefaults standardUserDefaults] setValue:self.uDriveY forKey:@"udrivey"];
    [[NSUserDefaults standardUserDefaults] setValue:self.hemisphere forKey:@"hemisphere"];
    [[NSUserDefaults standardUserDefaults] setValue:self.electrodeValue forKey:@"electrodevalue"];
    [[NSUserDefaults standardUserDefaults] setValue:self.impedance forKey:@"impedance"];
    [[NSUserDefaults standardUserDefaults] setValue:self.tubeProtrusion forKey:@"tubeprotrude"];
    [[NSUserDefaults standardUserDefaults] setValue:self.adapter forKey:@"adapter"];
    [[NSUserDefaults standardUserDefaults] setValue:self.userValue forKey:@"user"];
    [[NSUserDefaults standardUserDefaults] setValue:self.actualPenLog forKey:@"actualpenlog"];
}

@end

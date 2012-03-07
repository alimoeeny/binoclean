//
//  penLog.h
//  binoclean
//
//  Created by Ali Moeeny on 2/28/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface penLog : NSWindowController {
    NSMutableArray * userList;
    NSString * userValue;
    NSMutableArray * electrodeTypes;
    NSString * electrodeValue;
    NSNumber * tubeProtrusion;
    NSNumber * uDriveX;
    NSNumber * uDriveY;
    NSNumber * impedance;
    NSString * adapter;
    NSArray * hemisphereS;
    NSString * hemisphere;
    NSNumber * penNumber;
    NSArray * monkeyNames;
    NSString * monkeyValue;
    NSString * actualPenLog;

    NSTimer * penlogloadTimer;
//    NSString * penLogFileName;
    
    IBOutlet NSPopUpButton * monkeySelect;
    NSTextField * actualPenLogTextField;
}

@property (nonatomic, retain) NSArray * userList;
@property (nonatomic, retain) NSString * userValue;
@property (nonatomic, retain) NSArray * electrodeTypes;
@property (nonatomic, retain) NSString * electrodeValue;
@property (nonatomic, retain) NSNumber * tubeProtrusion;
@property (nonatomic, retain) NSNumber * uDriveX;
@property (nonatomic, retain) NSNumber * uDriveY;
@property (nonatomic, retain) NSNumber * impedance;
@property (nonatomic, retain) NSString * adapter;
@property (nonatomic, retain) NSArray * hemisphereS;
@property (nonatomic, retain) NSString * hemisphere;
@property (nonatomic, retain) NSNumber * penNumber;
@property (nonatomic, retain) NSArray * monkeyNames;
@property (nonatomic, retain) NSString * monkeyValue;
@property (nonatomic, retain) NSString * actualPenLog;

@property (nonatomic, retain) NSTimer * penlogloadTimer;
//@property (nonatomic, retain) NSString * penLogFileName;

@property (nonatomic, retain) IBOutlet NSPopUpButton * monkeySelect;
@property (nonatomic, retain) IBOutlet NSTextField * actualPenLogTextField;

- (void) loadPenLogTimerFire:(NSTimer*)theTimer;

- (IBAction) applyAndSave:(id)sender;

@end

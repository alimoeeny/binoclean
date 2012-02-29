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
    NSString * tubeProtrusion;
    NSString * uDriveX;
    NSString * uDriveY;
    NSString * impedance;
    NSString * adapter;
    NSString * penNumber;
}

@property (nonatomic, retain) NSArray * userList;
@property (nonatomic, retain) NSString * userValue;
@property (nonatomic, retain) NSArray * electrodeTypes;
@property (nonatomic, retain) NSString * electrodeValue;
@property (nonatomic, retain) NSString * tubeProtrusion;
@property (nonatomic, retain) NSString * uDriveX;
@property (nonatomic, retain) NSString * uDriveY;
@property (nonatomic, retain) NSString * impedance;
@property (nonatomic, retain) NSString * adapter;
@property (nonatomic, retain) NSString * penNumber;

- (IBAction) applyAndSave:(id)sender;

@end

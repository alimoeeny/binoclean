//
//  stepperGUI.h
//  binoclean
//
//  Created by Ali Moeeny on 2/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface stepperGUI : NSWindowController
{
    NSInteger stepSize;
    NSInteger motorId;
}

@property (nonatomic) NSInteger stepSize;
@property (nonatomic) NSInteger motorId;

- (IBAction) uDriveInitialize;
- (IBAction) uDriveButtonPress:(id) sender;
- (IBAction) reopen:(id)sender;
- (IBAction) setMotorIdComboChanged:(id)sender;

@end

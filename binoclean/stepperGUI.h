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
    
    NSButton * upButton;
    NSButton * downButton;
    NSTextField * electrodePosition;
    NSTextField * newPositionTextField;
}

@property (nonatomic) NSInteger stepSize;
@property (nonatomic) NSInteger motorId;

@property (nonatomic, retain) IBOutlet NSButton * upButton;
@property (nonatomic, retain) IBOutlet NSButton * downButton;
@property (nonatomic, retain) IBOutlet NSTextField * electrodePosition;
@property (nonatomic, retain) IBOutlet NSTextField * newPositionTextField;


- (void) enableNewMovements;
- (void) disableNewMovements;
- (void) alertDidEnd:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo;

- (IBAction) uDriveInitialize;
- (IBAction) uDriveButtonPress:(id) sender;
- (IBAction) reopen:(id)sender;
- (IBAction) setMotorIdComboChanged:(id)sender;
- (IBAction) uDriveRepositionMove:(id) sender;
- (IBAction) uDriveRepositionSet:(id) sender;

@end

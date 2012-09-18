//
//  mainGUI.h
//  binoclean
//
//  Created by Ali Moeeny on 7/26/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import "commdefs.h"
#import "stimuli.h"
#import "stepperGUI.h"
#import "penLog.h"

@interface mainGUI : NSWindow{
    NSButton * playButton;
    
    NSTextField * commandHistoryTextField;
    NSTextField * informationTextField;
    NSTextField * commandTextField;
    
    int experiment1MeanValue;
    int experiment1StepValue;
    int experiment1CountValue;
    NSString * experiment1StimulusValue;
    int experiment2MeanValue;
    int experiment2StepValue;
    int experiment2CountValue;
    NSString * experiment2StimulusValue;
    int experiment3MeanValue;
    int experiment3StepValue;
    int experiment3CountValue;
    NSString * experiment3StimulusValue;
    
    NSMutableArray * stimulusValues;
    
    penLog * pl;
    stepperGUI * sw;
}

@property (nonatomic, retain) IBOutlet NSButton * playButton;

@property (nonatomic, retain) IBOutlet NSTextField * experiment1Mean;
@property (nonatomic, retain) IBOutlet NSTextField * experiment1Step;
@property (nonatomic, retain) IBOutlet NSTextField * experiment1Count;
@property (nonatomic, retain) IBOutlet NSPopUpButton * experiment1Stimulus;
@property (nonatomic, retain) IBOutlet NSTableView * experiment1Table;
@property (nonatomic, readwrite) int experiment1MeanValue;
@property (nonatomic, readwrite) int experiment1StepValue;
@property (nonatomic, readwrite) int experiment1CountValue;
@property (nonatomic, retain) NSString * experiment1StimulusValue;

@property (nonatomic, retain) IBOutlet NSTextField * experiment2Mean;
@property (nonatomic, retain) IBOutlet NSTextField * experiment2Step;
@property (nonatomic, retain) IBOutlet NSTextField * experiment2Count;
@property (nonatomic, retain) IBOutlet NSPopUpButton * experiment2Stimulus;
@property (nonatomic, retain) IBOutlet NSTableView * experiment2Table;
@property (nonatomic, readwrite) int experiment2MeanValue;
@property (nonatomic, readwrite) int experiment2StepValue;
@property (nonatomic, readwrite) int experiment2CountValue;
@property (nonatomic, retain) NSString * experiment2StimulusValue;

@property (nonatomic, retain) IBOutlet NSTextField * experiment3Mean;
@property (nonatomic, retain) IBOutlet NSTextField * experiment3Step;
@property (nonatomic, retain) IBOutlet NSTextField * experiment3Count;
@property (nonatomic, retain) IBOutlet NSPopUpButton * experiment3Stimulus;
@property (nonatomic, retain) IBOutlet NSTableView * experiment3Table;
@property (nonatomic, readwrite) int experiment3MeanValue;
@property (nonatomic, readwrite) int experiment3StepValue;
@property (nonatomic, readwrite) int experiment3CountValue;
@property (nonatomic, retain) NSString * experiment3StimulusValue;

@property (nonatomic, retain) IBOutlet NSTextField * informationTextField;
@property (nonatomic, retain) IBOutlet NSTextField * commandHistoryTextField;
@property (nonatomic, retain) IBOutlet NSTextField * commandTextField;
@property (nonatomic, retain) NSMutableArray * stimulusValues;

@property (nonatomic, retain) penLog * pl;
@property (nonatomic, retain) stepperGUI * sw;


- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)windowStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferCreation screen:(NSScreen *)screen;


- (IBAction) elPosButton:(id)sender;
- (IBAction) penLogButton:(id)sender;
- (IBAction) brucesNumbers:(id)sender;
- (IBAction) textCommand:(id)sender;
- (IBAction) FreePipeButton:(id)sender;
- (IBAction) acknowledgeTest:(id)sender;


@end

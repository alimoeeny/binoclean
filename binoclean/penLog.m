//
//  penLog.m
//  binoclean
//
//  Created by Ali Moeeny on 2/28/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "penLog.h"

@implementation penLog

@synthesize userList;
@synthesize electrodeTypes;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        self.userList = [[NSArray arrayWithObjects:@"Ali", @"Bruce", @"Adrian", @"Guest", nil] retain];
        self.electrodeTypes = [[NSArray arrayWithObjects:@"AlphaOmega", @"MicroProbe+polyamide", @"FHC-PtIR", nil] retain];
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

@end

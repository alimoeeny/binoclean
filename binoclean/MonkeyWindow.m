//
//  MonekeyWindow.m
//  binoclean
//
//  Created by Ali Moeeny on 4/2/12.
//  Copyright (c) 2012 LSR-NEI. All rights reserved.
//

#import "MonkeyWindow.h"

@implementation MonkeyWindow

@synthesize crossCursor;



- (id)init
{
    self = [super init];
    if (self) {
        NSPoint hotspot; hotspot.x = 10; hotspot.y = 10;
        NSImage *crossImg = [NSImage imageNamed:@"crosscursorPG20.png"];
        crossCursor = [[NSCursor alloc] initWithImage:crossImg hotSpot:hotspot];
    }
    return self;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

// ---------------------------------

- (BOOL)becomeFirstResponder
{
    return  YES;
}

// ---------------------------------

- (BOOL)resignFirstResponder
{
    return YES;
}


- (void)resetCursorRects
{
    [super resetCursorRects ];
    [self discardCursorRects];
    [self addCursorRect:[self frame] cursor:[self crossCursor]];
}

@end

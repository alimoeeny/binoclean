//
//  MonekeyWindow.h
//  binoclean
//
//  Created by Ali Moeeny on 4/2/12.
//  Copyright (c) 2012 LSR-NEI. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface MonkeyWindow : NSWindow
{
    NSCursor * crossCursor;
}

@property (nonatomic, strong) NSCursor * crossCursor;

- (BOOL) acceptsFirstResponder;
- (BOOL) becomeFirstResponder;
- (BOOL) resignFirstResponder;
- (void)resetCursorRects;

@end

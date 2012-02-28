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
}

@property (nonatomic, retain) NSArray * userList;
@property (nonatomic, retain) NSString * userValue;
@property (nonatomic, retain) NSArray * electrodeTypes;
@property (nonatomic, retain) NSString * electrodeValue;

@end

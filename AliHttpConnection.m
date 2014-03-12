//
//  AliHttpConnection.m
//  TCPtest
//
//  Created by Ali Moeeny on 3/12/14.
//  Copyright (c) 2014 Ali Moeeny. All rights reserved.
//

#import "AliHttpConnection.h"
#import "HTTPMessage.h"
#import "HTTPDataResponse.h"
#import "DDNumber.h"
#import "HTTPLogging.h"

Expt expt;
static NSMutableArray * inputPipeBuffer;
NSString * outputPipeBuffer;
BOOL dataReadyInInputPipe;


// Log levels : off, error, warn, info, verbose
// Other flags: trace
static const int httpLogLevel = HTTP_LOG_LEVEL_WARN; // | HTTP_LOG_FLAG_TRACE;


/**
 * All we have to do is override appropriate methods in HTTPConnection.
 **/

@implementation AliHttpConnection

- (BOOL)supportsMethod:(NSString *)method atPath:(NSString *)path
{
    return YES;
}

- (BOOL)expectsRequestBodyFromMethod:(NSString *)method atPath:(NSString *)path
{
	HTTPLogTrace();

	// Inform HTTP server that we expect a body to accompany a POST request

	if([method isEqualToString:@"POST"])
		return YES;

	return [super expectsRequestBodyFromMethod:method atPath:path];
}

- (NSObject<HTTPResponse> *)httpResponseForMethod:(NSString *)method URI:(NSString *)path
{
    NSData *outputdata = [@"" dataUsingEncoding:NSASCIIStringEncoding];
    if ([method isEqualToString:@"GET"]) {
        if (request.url){
            if (request.url.pathComponents){
                if ([request.url.pathComponents count]>1){
                    //NSString * pq = [request.url.pathComponents[1] componentsSeparatedByString:@"="][1];
                    NSString * command =request.url.pathComponents[1];
                    if (expt.verbose){
                        NSLog(@"Input Pipe: %@", command);
                    }
                    NSArray * sLines = [command componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r\n"]];
                    for (int i = 0; i < [sLines count]; i++) {
                        if (strncmp([sLines objectAtIndex:i], "whatsup", 7) == 0){
                            NSString * s = [NSString stringWithFormat:@"SENDING%06d\n%@", [outputPipeBuffer length], outputPipeBuffer];
                            NSData *outputdata = [s dataUsingEncoding:NSASCIIStringEncoding];
                            outputPipeBuffer = @"";
                        }
                        else{
                            [inputPipeBuffer addObject:[sLines objectAtIndex:i]];
                        }
                    }
                    dataReadyInInputPipe = YES;
                }
            }
        }
    }
    return [[HTTPDataResponse alloc] initWithData:outputdata];
    //	return [super httpResponseForMethod:method URI:path];
}

- (void)prepareForBodyWithSize:(UInt64)contentLength
{
	HTTPLogTrace();

	// If we supported large uploads,
	// we might use this method to create/open files, allocate memory, etc.
}

- (void)processBodyData:(NSData *)postDataChunk
{
	HTTPLogTrace();

	// Remember: In order to support LARGE POST uploads, the data is read in chunks.
	// This prevents a 50 MB upload from being stored in RAM.
	// The size of the chunks are limited by the POST_CHUNKSIZE definition.
	// Therefore, this method may be called multiple times for the same POST request.

	BOOL result = [request appendData:postDataChunk];
	if (!result)
	{
		HTTPLogError(@"%@[%p]: %@ - Couldn't append bytes!", THIS_FILE, self, THIS_METHOD);
	}
}

@end

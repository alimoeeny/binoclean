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
	HTTPLogTrace();

	// Add support for POST
    if ([method isEqualToString:@"GET"]) {
        if (request.url){
            if (request.url.pathComponents){
                if ([request.url.pathComponents count]>1){
                    //NSString * pq = [request.url.pathComponents[1] componentsSeparatedByString:@"="][1];
                    NSString * command =request.url.pathComponents[1];
                    NSLog(@"Request:%@", command);

                    if (expt.verbose)
                        NSLog(@"Input Pipe: %@", command);
                    NSArray * sLines = [command componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r\n"]];
                    for (int i = 0; i < [sLines count]; i++) {
                        [inputPipeBuffer addObject:[sLines objectAtIndex:i]];
                    }
                    dataReadyInInputPipe = YES;
                }
            }
        }
    }
	else if ([method isEqualToString:@"POST"])
	{
		if ([path isEqualToString:@"/post.html"])
		{
			// Let's be extra cautious, and make sure the upload isn't 5 gigs

			return requestContentLength < 50;
		}
	}

	return [super supportsMethod:method atPath:path];
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
	HTTPLogTrace();

	if ([method isEqualToString:@"POST"] && [path isEqualToString:@"/post.html"])
	{
		HTTPLogVerbose(@"%@[%p]: postContentLength: %qu", THIS_FILE, self, requestContentLength);

		NSString *postStr = nil;

		NSData *postData = [request body];
		if (postData)
		{
			postStr = [[NSString alloc] initWithData:postData encoding:NSUTF8StringEncoding];
		}

		HTTPLogVerbose(@"%@[%p]: postStr: %@", THIS_FILE, self, postStr);

		// Result will be of the form "answer=..."

		int answer = [[postStr substringFromIndex:7] intValue];

		NSData *response = nil;
		if(answer == 10)
		{
			response = [@"<html><body>Correct<body></html>" dataUsingEncoding:NSUTF8StringEncoding];
		}
		else
		{
			response = [@"<html><body>Sorry - Try Again<body></html>" dataUsingEncoding:NSUTF8StringEncoding];
		}

		return [[HTTPDataResponse alloc] initWithData:response];
	}

	return [super httpResponseForMethod:method URI:path];
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

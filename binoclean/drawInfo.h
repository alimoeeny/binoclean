//
// File:		drawInfo.h
//
// Abstract:	Creates and maintains the texture with the strings describing
//				the capabilities of the graphics card.
//
//
// Copyright ( C ) 2003-2007 Apple Inc. All Rights Reserved.
//


#import <OpenGL/gl.h>

// call after OpenGL caps are found
void initCapsTexture (GLCaps * displayCaps, CGDisplayCount numDisplays);

// get NSString with caps for this renderer
void drawCaps (GLCaps * displayCaps, CGDisplayCount numDisplays, long renderer, GLfloat width); // view width for drawing location

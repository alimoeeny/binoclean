//
//  MonkeyGLView.m
//  binoclean
//
//  Created by Ali Moeeny on 6/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MonkeyGLView.h"
#import "GLCheck.h"
#import "drawinfo.h"
#import "stimdefs.h"

int winsiz [2];


recVec gOrigin = {0.0, 0.0, 0.0};

// single set of interaction flags and states
GLint gDollyPanStartPoint[2] = {0, 0};
GLfloat gTrackBallRotation [4] = {0.0f, 0.0f, 0.0f, 0.0f};
GLboolean gDolly = GL_FALSE;
GLboolean gPan = GL_FALSE;
GLboolean gTrackball = GL_FALSE;
MonkeyGLView * gTrackingViewInfo = NULL;

// time and message info
CFAbsoluteTime gMsgPresistance = 10.0f;

// error output
GLString * gErrStringTex;
float gErrorTime;

// ==================================

#pragma mark ---- OpenGL Capabilities ----

// GL configuration info globals
// see GLCheck.h for more info
GLCaps * gDisplayCaps = NULL; // array of GLCaps
CGDisplayCount gNumDisplays = 0;

static void getCurrentCaps (void)
{
 	// Check for existing opengl caps here
	// This can be called again with same display caps array when display configurations are changed and
	//   your info needs to be updated.  Note, if you are doing dynmaic allocation, the number of displays
	//   may change and thus you should always reallocate your display caps array.
	if (gDisplayCaps && HaveOpenGLCapsChanged (gDisplayCaps, gNumDisplays)) { // see if caps have changed
		free (gDisplayCaps);
		gDisplayCaps = NULL;
	}
	if (!gDisplayCaps) { // if we do not have caps
		CheckOpenGLCaps (0, NULL, &gNumDisplays); // will just update number of displays
		gDisplayCaps = (GLCaps*) malloc (sizeof (GLCaps) * gNumDisplays);
		CheckOpenGLCaps (gNumDisplays, gDisplayCaps, &gNumDisplays);
		initCapsTexture (gDisplayCaps, gNumDisplays); // (re)init the texture for printing caps
	}
}

#pragma mark ---- Utilities ----

static CFAbsoluteTime gStartTime = 0.0f;

// set app start time
static void setStartTime (void)
{	
	gStartTime = CFAbsoluteTimeGetCurrent ();
}

// ---------------------------------

// return float elpased time in seconds since app start
static CFAbsoluteTime getElapsedTime (void)
{	
	return CFAbsoluteTimeGetCurrent () - gStartTime;
}

#pragma mark ---- Error Reporting ----

// error reporting as both window message and debugger string
void reportError (char * strError)
{
    NSMutableDictionary *attribs = [NSMutableDictionary dictionary];
    [attribs setObject: [NSFont fontWithName: @"Monaco" size: 9.0f] forKey: NSFontAttributeName];
    [attribs setObject: [NSColor whiteColor] forKey: NSForegroundColorAttributeName];
    
	gErrorTime = getElapsedTime ();
	NSString * errString = [NSString stringWithFormat:@"Error: %s (at time: %0.1f secs).", strError, gErrorTime];
	NSLog (@"%@\n", errString);
	if (gErrStringTex)
		[gErrStringTex setString:errString withAttributes:attribs];
	else {
		gErrStringTex = [[GLString alloc] initWithString:errString withAttributes:attribs withTextColor:[NSColor colorWithDeviceRed:1.0f green:1.0f blue:1.0f alpha:1.0f] withBoxColor:[NSColor colorWithDeviceRed:1.0f green:0.0f blue:0.0f alpha:0.3f] withBorderColor:[NSColor colorWithDeviceRed:1.0f green:0.0f blue:0.0f alpha:0.8f]];
	}
}

// ---------------------------------

// if error dump gl errors to debugger string, return error
GLenum glReportError (void)
{
	GLenum err = glGetError();
	if (GL_NO_ERROR != err)
		reportError ((char *) gluErrorString (err));
	return err;
}

#pragma mark ---- OpenGL Utils ----

@implementation MonkeyGLView

// pixel format definition
+ (NSOpenGLPixelFormat*) basicPixelFormat
{
    NSOpenGLPixelFormatAttribute attributes [] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFADoubleBuffer,	// double buffered
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)0, // 16 bit depth buffer,
        NSOpenGLPFAAlphaSize, (NSOpenGLPixelFormatAttribute)8, 
        (NSOpenGLPixelFormatAttribute)nil
    };
    return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

// ---------------------------------


// per-window timer function, basic time based animation preformed here
- (void)animationTimer:(NSTimer *)timer
{
	BOOL shouldDraw = NO;
	if (fAnimate) {
		CFTimeInterval deltaTime = CFAbsoluteTimeGetCurrent () - time;
        
		if (deltaTime > 10.0) // skip pauses
			return;
		else {
			// if we are not rotating with trackball in this window
			if (!gTrackball || (gTrackingViewInfo != self)) {
				[self updateObjectRotationForTimeDelta: deltaTime]; // update object rotation
			}
			shouldDraw = YES; // force redraw
		}
	}
	time = CFAbsoluteTimeGetCurrent (); //reset time in all cases
	// if we have current messages
	if (((getElapsedTime () - msgTime) < gMsgPresistance) || ((getElapsedTime () - gErrorTime) < gMsgPresistance))
		shouldDraw = YES; // force redraw
	if (YES == shouldDraw) 
		[self drawRect:[self bounds]]; // redraw now instead dirty to enable updates during live resize
}

#pragma mark ---- Text Drawing ----

// these functions create or update GLStrings one should expect to have to regenerate the image, bitmap and texture when the string changes thus these functions are not particularly light weight

- (void) updateInfoString
{ // update info string texture
	NSString * string = [NSString stringWithFormat:@"(%0.0f x %0.0f) \n%s \n%s", [self bounds].size.width, [self bounds].size.height, glGetString (GL_RENDERER), glGetString (GL_VERSION)];
	if (infoStringTex)
		[infoStringTex setString:string withAttributes:stanStringAttrib];
	else {
		infoStringTex = [[GLString alloc] initWithString:string withAttributes:stanStringAttrib withTextColor:[NSColor colorWithDeviceRed:1.0f green:1.0f blue:1.0f alpha:1.0f] withBoxColor:[NSColor colorWithDeviceRed:0.5f green:0.5f blue:0.5f alpha:0.5f] withBorderColor:[NSColor colorWithDeviceRed:0.8f green:0.8f blue:0.8f alpha:0.8f]];
	}
}

// ---------------------------------

- (void) createHelpString
{
	NSString * string = [NSString stringWithFormat:@"Cmd-A: animate    Cmd-I: show info \n'h': toggle help    'c': toggle OpenGL caps"];
	helpStringTex = [[GLString alloc] initWithString:string withAttributes:stanStringAttrib withTextColor:[NSColor colorWithDeviceRed:1.0f green:1.0f blue:1.0f alpha:1.0f] withBoxColor:[NSColor colorWithDeviceRed:0.0f green:0.5f blue:0.0f alpha:0.5f] withBorderColor:[NSColor colorWithDeviceRed:0.3f green:0.8f blue:0.3f alpha:0.8f]];
}

// ---------------------------------

- (void) createMessageString
{
	NSString * string = [NSString stringWithFormat:@"No messages..."];
	msgStringTex = [[GLString alloc] initWithString:string withAttributes:stanStringAttrib withTextColor:[NSColor colorWithDeviceRed:1.0f green:1.0f blue:1.0f alpha:1.0f] withBoxColor:[NSColor colorWithDeviceRed:0.5f green:0.5f blue:0.5f alpha:0.5f] withBorderColor:[NSColor colorWithDeviceRed:0.8f green:0.8f blue:0.8f alpha:0.8f]];
}


// ---------------------------------

// draw text info using our GLString class for much more optimized text drawing
- (void) drawInfo
{	
	GLint matrixMode;
	GLboolean depthTest = glIsEnabled (GL_DEPTH_TEST);
	GLfloat height, width, messageTop = 10.0f;
	
    //	height = camera.viewHeight;
    //	width = camera.viewWidth;
    
	// set orthograhic 1:1  pixel transform in local view coords
	glGetIntegerv (GL_MATRIX_MODE, &matrixMode);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
    glLoadIdentity ();
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity ();
    glScalef (2.0f / width, -2.0f /  height, 1.0f);
    glTranslatef (-width / 2.0f, -height / 2.0f, 0.0f);
    
    glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
    [infoStringTex drawAtPoint:NSMakePoint (10.0f, height - [infoStringTex frameSize].height - 10.0f)];
    [camStringTex drawAtPoint:NSMakePoint (10.0f, messageTop)];
    messageTop += [camStringTex frameSize].height + 3.0f;
    
    if (fDrawHelp)
        [helpStringTex drawAtPoint:NSMakePoint (floor ((width - [helpStringTex frameSize].width) / 2.0f), floor ((height - [helpStringTex frameSize].height) / 3.0f))];
    
    if (1) { //(fDrawCaps) {
        long renderer;
        [[self pixelFormat] getValues:&renderer forAttribute:NSOpenGLPFARendererID forVirtualScreen:[[self openGLContext] currentVirtualScreen]];
        drawCaps (gDisplayCaps, gNumDisplays, renderer, width);
    }
    
    // message string
    float currTime = getElapsedTime ();
    if ((currTime - msgTime) < gMsgPresistance) {
        GLfloat comp = (gMsgPresistance - getElapsedTime () + msgTime) * 0.1; // premultiplied fade
        glColor4f (comp, comp, comp, comp);
        [msgStringTex drawAtPoint:NSMakePoint (10.0f, messageTop)];
        messageTop += [msgStringTex frameSize].height + 3.0f;
    }
    // global error message
    if ((currTime - gErrorTime) < gMsgPresistance) {
        GLfloat comp = (gMsgPresistance - getElapsedTime () + gErrorTime) * 0.1; // premultiplied fade
        glColor4f (comp, comp, comp, comp);
        [gErrStringTex drawAtPoint:NSMakePoint (10.0f, messageTop)];
    }
    
    // reset orginal martices
    glPopMatrix(); // GL_MODELVIEW
    glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode (matrixMode);
    
	glDisable (GL_TEXTURE_RECTANGLE_EXT);
	glDisable (GL_BLEND);
	if (depthTest)
		glEnable (GL_DEPTH_TEST);
	glReportError ();
}

#pragma mark ---- IB Actions ----

-(IBAction) animate: (id) sender
{
	fAnimate = 1 - fAnimate;
	if (fAnimate)
		[animateMenuItem setState: NSOnState];
	else 
		[animateMenuItem setState: NSOffState];
}

// ---------------------------------

-(IBAction) info: (id) sender
{
	fInfo = 1 - fInfo;
	if (fInfo)
		[infoMenuItem setState: NSOnState];
	else
		[infoMenuItem setState: NSOffState];
	[self setNeedsDisplay: YES];
}

#pragma mark ---- Method Overrides ----

-(void)keyDown:(NSEvent *)theEvent
{
    NSString *characters = [theEvent characters];
    if ([characters length]) {
        unichar character = [characters characterAtIndex:0];
		switch (character) {
			case 'h':
				// toggle help
				fDrawHelp = 1 - fDrawHelp;
				[self setNeedsDisplay: YES];
				break;
			case 'c':
				// toggle caps
				fDrawCaps = 1 - fDrawCaps;
				[self setNeedsDisplay: YES];
				break;
		}
	}
}

// ---------------------------------

- (void) mouseMoved:(NSEvent *)theEvent
{
    [self resetCursorRects];
}

- (void)mouseDown:(NSEvent *)theEvent // trackball
{
	NSPoint location = [theEvent locationInWindow];
    WindowEvent e;
    e.eventType = ButtonPress;
    e.mouseX = location.x;
    e.mouseY = location.y;
    e.modifierKey = 0;
    if ([theEvent modifierFlags] & NSControlKeyMask) // send to pan
        e.modifierKey = ControlMask;
    else if ([theEvent modifierFlags] & NSShiftKeyMask) // send to pan
        e.modifierKey = ShiftMask;
    else if ([theEvent modifierFlags] & NSAlternateKeyMask) // send to pan
        e.modifierKey = AltMask;
    
    e.mouseButton = Button1;
    
    HandleMouse(e);
    
    //    if ([theEvent modifierFlags] & NSControlKeyMask) // send to pan
    //		[self rightMouseDown:theEvent];
    //	else if ([theEvent modifierFlags] & NSAlternateKeyMask) // send to dolly
    //		[self otherMouseDown:theEvent];
    //	else {
    //		NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    //		location.y = camera.viewHeight - location.y;
    //		gDolly = GL_FALSE; // no dolly
    //		gPan = GL_FALSE; // no pan
    //		gTrackball = GL_TRUE;
    //		startTrackball (location.x, location.y, 0, 0, camera.viewWidth, camera.viewHeight);
    //		gTrackingViewInfo = self;
    //	}
}

// ---------------------------------

- (void)rightMouseDown:(NSEvent *)theEvent // pan
{
    NSPoint location = [theEvent locationInWindow];
    WindowEvent e;
    e.eventType = ButtonPress;
    e.mouseX = location.x;
    e.mouseY = location.y;
    e.modifierKey = 0;
    if ([theEvent modifierFlags] & NSControlKeyMask) // send to pan
        e.modifierKey = ControlMask;
    else if ([theEvent modifierFlags] & NSShiftKeyMask) // send to pan
        e.modifierKey = ShiftMask;
    else if ([theEvent modifierFlags] & NSAlternateKeyMask) // send to pan
        e.modifierKey = AltMask;
    
    e.mouseButton = Button3;
    HandleMouse(e);
}

// ---------------------------------

- (void)otherMouseDown:(NSEvent *)theEvent //dolly
{
    NSPoint location = [theEvent locationInWindow];
    WindowEvent e;
    e.eventType = ButtonPress;
    e.mouseX = location.x;
    e.mouseY = location.y;
    e.modifierKey = 0;
    if ([theEvent modifierFlags] & NSControlKeyMask) // send to pan
        e.modifierKey = ControlMask;
    else if ([theEvent modifierFlags] & NSShiftKeyMask) // send to pan
        e.modifierKey = ShiftMask;
    else if ([theEvent modifierFlags] & NSAlternateKeyMask) // send to pan
        e.modifierKey = AltMask;
    
    e.mouseButton = Button2;
    HandleMouse(e);
}

// ---------------------------------

- (void)mouseUp:(NSEvent *)theEvent
{
	NSPoint location = [theEvent locationInWindow];
    WindowEvent e;
    e.eventType = ButtonRelease;
    e.mouseX = location.x;
    e.mouseY = location.y;
    e.modifierKey = 0;
    e.mouseButton = Button1;
    HandleMouse(e);
	
    //    if (gDolly) { // end dolly
    //		gDolly = GL_FALSE;
    //	} else if (gPan) { // end pan
    //		gPan = GL_FALSE;
    //	} else if (gTrackball) { // end trackball
    //		gTrackball = GL_FALSE;
    //		if (gTrackBallRotation[0] != 0.0)
    ////			addToRotationTrackball (gTrackBallRotation, worldRotation);
    //		gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
    //	} 
    //	gTrackingViewInfo = NULL;
}

// ---------------------------------

- (void)rightMouseUp:(NSEvent *)theEvent
{
	[self mouseUp:theEvent];
}

// ---------------------------------

- (void)otherMouseUp:(NSEvent *)theEvent
{
	[self mouseUp:theEvent];
}

// ---------------------------------

- (void)mouseDragged:(NSEvent *)theEvent
{
	NSPoint location = [theEvent locationInWindow];
    WindowEvent e;
    e.eventType = MotionNotify;
    e.mouseX = location.x;
    e.mouseY = location.y;
    e.modifierKey = 0;
    e.mouseButton = Button1;
    HandleMouse(e);
}

// ---------------------------------

- (void)scrollWheel:(NSEvent *)theEvent
{
    //	float wheelDelta = [theEvent deltaX] +[theEvent deltaY] + [theEvent deltaZ];
    //	if (wheelDelta)
    //	{
    ////		GLfloat deltaAperture = wheelDelta * -camera.aperture / 200.0f;
    ////		camera.aperture += deltaAperture;
    ////		if (camera.aperture < 0.1) // do not let aperture <= 0.1
    ////			camera.aperture = 0.1;
    ////		if (camera.aperture > 179.9) // do not let aperture >= 180
    ////			camera.aperture = 179.9;
    //		[self updateProjection]; // update projection matrix
    //		[self setNeedsDisplay: YES];
    //	}
}

// ---------------------------------

- (void)rightMouseDragged:(NSEvent *)theEvent
{
	[self mouseDragged: theEvent];
}


- (void)otherMouseDragged:(NSEvent *)theEvent
{
	[self mouseDragged: theEvent];
}


- (void) drawRect:(NSRect)rect
{		
	// setup viewport and prespective
    //	[self resizeGL]; // forces projection matrix update (does test for size changes)
    //	[self updateModelView];  // update model view matrix for object
    
	// clear our drawable
    //NSLog(@"time is: %f ", time);
    //    float cc = 0.0f;
    //    cc = ABS(sin(time*5.0f)); //remainderl(time, 1.0f);
    //    glClearColor(cc, cc, cc, 1);
    //	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  	glLoadIdentity ();
    //glOrtho((GLdouble)(-winsiz[0]) , (GLdouble)(winsiz[0]) , (GLdouble)(-winsiz[1]) , (GLdouble)(winsiz[1]) , 0, 0);
    glOrtho(-winsiz[0],winsiz[0],-winsiz[1],winsiz[1],-1.0,1.0);
	// model view and projection matricies already set
    
    //	drawCube (1.5f); // draw scene
    //    glOrtho(-640.0,640.0,-512.0,512.0,-1.0,1.0);
    //    calc_stimulus(TheStim);
    //    paint_stimulus(TheStim);
    
    //    Stimulus * st;
    //    init_stimulus(st);
    //    paint_gabor_pair(st);
    
    //    if (fInfo)
    //		[self drawInfo];
    //    
    //	if ([self inLiveResize] && !fAnimate)
    //		—glFlushRenderAPPLE ();
    //	else
    //		[[self openGLContext] flushBuffer];
    //	glReportError ();
}

// ---------------------------------

// set initial OpenGL state (current context is set)
// called after context is created
- (void) prepareOpenGL
{
    long swapInt = 1;
    
    //    NSOpenGLCPSwapInterval
    //    The swap interval is represented as one long. If the swap interval is set to 0 (the default), 
    //    the flushBuffer method executes as soon as possible, without regard to the vertical refresh rate of the monitor. 
    //    If the swap interval is set to 1, the buffers are swapped only during the vertical retrace of the monitor.
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; // set to vbl sync
    
	// init GL stuff here
    //	glEnable(GL_DEPTH_TEST);
    
	glShadeModel(GL_SMOOTH);    
    //	glEnable(GL_CULL_FACE);
    //	glFrontFace(GL_CCW);
    //	glPolygonOffset (1.0f, 1.0f);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//[self resetCamera];
    //	shapeSize = 7.0f; // max radius of of objects
    
    //Ali 
    //glOrtho((GLdouble)(-winsiz[0]) , (GLdouble)(winsiz[0]) , (GLdouble)(-winsiz[1]) , (GLdouble)(winsiz[1]) , 0, 0);    
    //glOrtho(-640.0f,640.0f,-512.0f,512.0f,0,0);
    
	// init fonts for use with strings
	NSFont * font =[NSFont fontWithName:@"Helvetica" size:12.0];
	stanStringAttrib = [[NSMutableDictionary dictionary] retain];
	[stanStringAttrib setObject:font forKey:NSFontAttributeName];
	[stanStringAttrib setObject:[NSColor whiteColor] forKey:NSForegroundColorAttributeName];
	[font release];
	
	// ensure strings are created
	//[self createHelpString];
	//[self createMessageString];
    
}
// ---------------------------------

// this can be a troublesome call to do anything heavyweight, as it is called on window moves, resizes, and display config changes.  So be
// careful of doing too much here.
- (void) update // window resizes, moves and display changes (resize, depth and display config change)
{
    msgTime	= getElapsedTime ();
    [msgStringTex setString:[NSString stringWithFormat:@"update at %0.1f secs", msgTime]  withAttributes:stanStringAttrib];
	[super update];
	if (![self inLiveResize])  {// if not doing live resize
		[self updateInfoString]; // to get change in renderers will rebuld string every time (could test for early out)
		getCurrentCaps (); // this call checks to see if the current config changed in a reasonably lightweight way to prevent expensive re-allocations
	}
}

// ---------------------------------

-(id) initWithFrame: (NSRect) frameRect
{
	NSOpenGLPixelFormat * pf = [MonkeyGLView basicPixelFormat];
    
	self = [super initWithFrame: frameRect pixelFormat: pf];
    
    NSPoint hotspot; hotspot.x = 10; hotspot.y = 10;
    NSImage *crossImg = [NSImage imageNamed:@"crosscursorPG20.png"];
    crossCursor = [[NSCursor alloc] initWithImage:crossImg hotSpot:hotspot];
    [[self window] setAcceptsMouseMovedEvents:YES];
    return self;
}

// ---------------------------------

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

// ---------------------------------

- (void) awakeFromNib
{
	setStartTime (); // get app start time
	getCurrentCaps (); // get current GL capabilites for all displays
	
	// set start values...
	rVel[0] = 0.3; rVel[1] = 0.1; rVel[2] = 0.2; 
	rAccel[0] = 0.003; rAccel[1] = -0.005; rAccel[2] = 0.004;
	fInfo = 1;
	fAnimate = 1;
	time = CFAbsoluteTimeGetCurrent ();  // set animation time start time
	fDrawHelp = 1;
    
	// start animation timer
	timer = [NSTimer timerWithTimeInterval:(1.0f/60.0f) target:self selector:@selector(animationTimer:) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode]; // ensure timer fires during resize
}

- (void)resetCursorRects
{
    [super resetCursorRects ];
    [self discardCursorRects];
    [self addCursorRect:[self frame] cursor:[self crossCursor]];
}

@end


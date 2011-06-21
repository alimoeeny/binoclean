//#include <stdio.h>
//#include <GL/glx.h>
//
//static Display *xDisplay = 0;
//static XVisualInfo *vInfoMain;
//static Window MainWindow;
//static int xScreen = 0; 
//
//static char __renderer[128] = {0};
//
//char *getRenderer(void)
//{
//  FILE *p;
//  char *command;
//  int n;
//
//  if(strlen(__renderer) > 0)
//    return __renderer;
//  command = "/usr/sbin/glxinfo|grep \"OpenGL renderer string:\"";
//  p = popen(command, "r");
//  n = fscanf(p, "OpenGL renderer string: %s", __renderer);
//  pclose(p);
//  return __renderer;
//
//}

//void setdisplay(Display *dis)
//{
//  xDisplay = dis;
//}
//
//Window myXWindow()
//{
//  return(MainWindow);
//}
//
//static int ErrorHandler(Display *xDisplay, XErrorEvent *event)
//{
//    char buf[80];
//
//    printf("\nReceived X error!\n");
//    printf("\tError code   : %d\n", event->error_code);
//    printf("\tRequest code : %d\n", event->request_code);
//    printf("\tMinor code   : %d\n\n", event->minor_code);
//    XGetErrorText(xDisplay, event->error_code, buf, 80);
//    printf("\tError text : '%s'\n\n", buf);
//    return 0;
//}
//
//static XVisualInfo *FindMainVisual(int depth, int stereo)
//{
//    int list[32], i;
//    XVisualInfo *ptr;
//
//    i = 0;
//    
//    list[i++] = GLX_LEVEL;
//    list[i++] = 0;
//    list[i++] = GLX_DOUBLEBUFFER;
//    list[i++] = GLX_RGBA;
//    list[i++] = GLX_RED_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_GREEN_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_BLUE_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_ALPHA_SIZE;
//    list[i++] = 1;
//    list[i++] = GLX_DEPTH_SIZE;
//    list[i++] = 0;
//    if(stereo)
//	    list[i++] = GLX_STEREO;
//    list[i] = (int)None;
//
////AliGL    if((ptr = glXChooseVisual(xDisplay, xScreen, 
////AliGL			   list)) != NULL)
////AliGL      return(ptr);
//    fprintf(stderr,"Cant find %d Bit double buffered visual with alpha\n",depth);
//    i = 0;
//    
//    list[i++] = GLX_LEVEL;
//    list[i++] = 0;
//    list[i++] = GLX_DOUBLEBUFFER;
//    list[i++] = GLX_RGBA;
//    list[i++] = GLX_RED_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_GREEN_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_BLUE_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_ALPHA_SIZE;
//    list[i++] = 0;
//    list[i++] = GLX_DEPTH_SIZE;
//    list[i++] = 0;
//    if(stereo)
//	    list[i++] = GLX_STEREO;
//    list[i] = (int)None;
//
////AliGL    if((ptr = glXChooseVisual(xDisplay, xScreen, 
////AliGL			   list)) != NULL)
////AliGL      return(ptr);
//    fprintf(stderr,"Cant find 24 Bit double buffered visual\n");
//
//    /* get here if failed: try turning off destination ALPHA for indigos */
//    if(depth > 4)
//      depth = 4;
//
//    i = 0;
//
//
//    list[i++] = GLX_LEVEL;
//    list[i++] = 0;
//    list[i++] = GLX_DOUBLEBUFFER;
//    list[i++] = GLX_RGBA;
//    list[i++] = GLX_RED_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_GREEN_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_BLUE_SIZE;
//    list[i++] = depth;
//    list[i++] = GLX_ALPHA_SIZE;
//    list[i++] = 0;
//    list[i++] = GLX_DOUBLEBUFFER;
//    list[i++] = GLX_DEPTH_SIZE;
//    list[i++] = 0;
//    list[i] = (int)None;
////AliGL    ptr = glXChooseVisual(xDisplay, xScreen, 
////AliGL			   list);
//    if (ptr == NULL)
//      fprintf(stderr,"Cant find 12 Bit double buffered visual\n");
//    return(ptr);
//}
//
//static int WaitForMainWindow(Display *d, XEvent *e, char *arg)
//{
//
//    if (e->type == MapNotify && e->xmap.window == MainWindow) {
//	return GL_TRUE;
//    } else {
//	return GL_FALSE;
//    }
//}
//
//GLenum myInitWindow(char *title, int x, int y, int w, int h, int depth, int stereo, Window parent)
//{
//    XSetWindowAttributes wa;
//    XTextProperty tp;
//    XSizeHints sh;
//    XEvent e;
//    int erb, evb;
//    GLenum overlayFlag;
//    GLXContext MainContext;
//    Atom deleteWindowAtom;
//    Colormap cMapMain;
//    Window wRoot = 0;
//  Atom mwmHints = 0;
//  MotifWmHints hints;
//
//  /*
//    if(!strncmp(getRenderer(), "IMPACT", 6))
//        putenv("SOFT_AA_POLYS=1");
//  */
//    if (!xDisplay) {
//	xDisplay = XOpenDisplay(0);
//	if (!xDisplay) {
//	    fprintf(stderr, "Can't connect to xDisplay!\n");
//	    return GL_FALSE;
//	}
////AliGL	if (!glXQueryExtension(xDisplay, &erb, &evb)) {
////AliGL	    fprintf(stderr, "No glx extension!\n");
////AliGL	    return GL_FALSE;
////AliGL	}
//	xScreen = DefaultScreen(xDisplay);
//	wRoot = RootWindow(xDisplay, xScreen);
//	XSetErrorHandler(ErrorHandler);
//    }
//    else
//      {
//	xScreen = DefaultScreen(xDisplay);
//	wRoot = RootWindow(xDisplay, xScreen);
//	XSetErrorHandler(ErrorHandler);
//      }
// 	if(parent == 0)
//	    parent = wRoot;
//    vInfoMain = FindMainVisual(depth, stereo);
//    if (!vInfoMain) {
//	fprintf(stderr, "Window type not found!\n");
//	return GL_FALSE;
//    }
//
////AliGL    MainContext = glXCreateContext(xDisplay, vInfoMain, None, GL_TRUE);
//    if (!MainContext) {
//	fprintf(stderr, "Can't create a context!\n");
//	return GL_FALSE;
//    }
//
//    cMapMain = XCreateColormap(xDisplay, wRoot, vInfoMain->visual,
//				         AllocNone);
//    wa.colormap = cMapMain;
//    wa.background_pixmap = None;
//    wa.border_pixel = 0;
//    wa.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask |
//		    ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
//    MainWindow = XCreateWindow(xDisplay, parent, x, y, w, h, 0,
//			    vInfoMain->depth, InputOutput,
//			    vInfoMain->visual,
//			    CWBackPixmap|CWBorderPixel|CWEventMask|CWColormap,
//			    &wa);
///*
// * Under RH 7.2 setting this hint causes annoying behaiour where the glxwin
// * jumps to the top whenever the mouse enters it.
// * ALT -F5 then allows the window order to be reset, but its very painful
// * making window 1023 pixels high also solves it
// */
//
//
//#ifndef Linux
//#endif
//    mwmHints = XInternAtom(xDisplay, "_MOTIF_WM_HINTS", 0);
//    if(mwmHints)
//    {
//	hints.flags = MWM_HINTS_DECORATIONS;
//	hints.decorations = 0;
//	
//	XChangeProperty(xDisplay, MainWindow, mwmHints, mwmHints, 
//		32, PropModeReplace, (unsigned char *)&hints, 4);
//    }
//
//    XStringListToTextProperty(&title, 1, &tp);
//    sh.flags = USPosition | USSize;
//    XSetWMProperties(xDisplay, MainWindow, &tp, &tp, 0, 0, &sh, 0, 0);
//    deleteWindowAtom = XInternAtom(xDisplay, "WM_DELETE_WINDOW", False);
//    XSetWMProtocols(xDisplay, MainWindow, &deleteWindowAtom, 1);
//    XMapWindow(xDisplay, MainWindow);
//    XIfEvent(xDisplay, &e, WaitForMainWindow, 0);
//
////AliGL    if (!glXMakeCurrent(xDisplay, MainWindow, MainContext)) {
////AliGL	return GL_FALSE;
////AliGL    }
//    XFlush(xDisplay);
//
//    return GL_TRUE;
//}
//
//void mySwapBuffers()
//{
//    [];
////  	glXSwapBuffers(xDisplay, MainWindow);
//}


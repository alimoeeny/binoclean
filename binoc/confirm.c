#include <stdio.h>
#include <X11/Intrinsic.h>
#include "misc.h"
#include "myview.h"
#include <stdarg.h>
#define SCREENCHARWIDTH 80
#define MAXMSG 5

int confirmer_up[MAXMSG] = {0};

static XtAppContext app_context;
static Frame base_frame = NULL;

static int confirm();
static void yes_no();
char *myscopy(),*mysncopy();
static char *msg[MAXMSG] = {0};
static int init = 0;
void response();

set_alert_frame(Widget frame, XtAppContext app)
{
	base_frame = frame;
	app_context = app;
}

void help_callback(Widget parent, char *help_text, XmAnyCallbackStruct *cbs)
{
	Widget dialog;
	XmString text;
	void help_done();
	Arg args[2];
	int n = 0;
	
	text = XmStringCreateLtoR(help_text, XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[n], XmNmessageString, text); n++;
	dialog = XmCreateInformationDialog(parent, "help", args, n);
	XmStringFree(text);
	free(help_text);
	XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_CANCEL_BUTTON));
	XtSetSensitive(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON),False);
	XtAddCallback(dialog, XmNokCallback, help_done, NULL);
	XtManageChild(dialog);
	XtPopup(XtParent(dialog), XtGrabNone);
}

void help_done(Widget dialog)
{
	XtDestroyWidget(dialog);
}

int make_help(Widget parent, char *name)
{
	char path[BUFSIZ],buf[BUFSIZ*10], *dir;
	FILE *fd = NULL;
	void help_callback();
	
	if(name && *name)
	{
	  dir = useenv(name);
	  fd = fopen(dir, "r");
	  if(fd == NULL)
	    return(-1);
	  buf[0] = 0;
	  while(fgets(path,256,fd) != NULL)
	    strcat(buf,path);
	
	}
	else
	  {
	    sprintf(buf,"No help Available on this option");
	  }
	XtAddCallback(parent, XmNhelpCallback, help_callback, buf);
	return(0);
	
}

void acknowledged(w, answer, reason)
Widget w;
int *answer;
XmAnyCallbackStruct *reason;
{
  if(answer)
    *answer = 0;
}

Widget acknowledge(char *s, char *helpfile)
{
  Widget item;
  XmString t = XmStringCreateLtoR(s,XmSTRING_DEFAULT_CHARSET), yes = XmStringCreateSimple("Acknowledge");
  int i;
  static int msgctr = 0;

  void acknowledged();

  if(base_frame == NULL)
    {
      fprintf(stderr,"ack: %s\n",s);
      return;
    }
  confirmer_up[msgctr] = 1;
  item = XmCreateWarningDialog(base_frame,"Warning", NULL, 0);
  XtVaSetValues(item, XmNmessageString, t,
		XmNcancelLabelString, yes, 
		XmNuserData,&confirmer_up[msgctr], NULL);
  if(helpfile == NULL || (i = make_help(item, helpfile)) < 0)
	XtUnmanageChild(XmMessageBoxGetChild(item, XmDIALOG_HELP_BUTTON));
  XtUnmanageChild(XmMessageBoxGetChild(item, XmDIALOG_OK_BUTTON));
  XtAddCallback(item, XmNcancelCallback, acknowledged, &confirmer_up[msgctr]);
  XtManageChild(item);
  XmStringFree(t);
  XmStringFree(yes);
  if(++msgctr >= MAXMSG)
    msgctr = 0;
  return(item);
}

 my_prompt(Widget parent, int junk, ...)
 {
   va_list args;
   int prop,i,n = 0,answer;
   char *s,msg[1024],*helpfile = 0;
   Arg wargs[10];
  Widget item;
  XmString t, yes = XmStringCreateSimple("OK");
  XmString no = 0, help = 0;

   va_start(args, junk);

   while((prop = va_arg(args, int)) != 0)
     {
       switch(prop)
	 {
	 case ALERT_BUTTON_YES:
	   if((s = (char *)va_arg(args, int)) != NULL)
	     {
	   XmStringFree(yes);
	   yes = XmStringCreateSimple(s);
	 }
	   break;
	 case ALERT_BUTTON_HELP_FILE:
	   helpfile = (char *)va_arg(args, int);
	   break;
	 case ALERT_BUTTON_HELP:
	   if((s = (char *)va_arg(args, int)) != NULL)
	     {
	   help = XmStringCreateSimple(s);
	 }
	   break;
	 case ALERT_BUTTON_NO:
	   if((s = (char *)va_arg(args, int)) != NULL)
	     {
	   no = XmStringCreateSimple(s);
	 }
	   break;
	 case ALERT_MESSAGE_STRINGS:
	   i = 0;
	   while((s = (char *)va_arg(args, int)) != 0)
		 {
		   if(i++ == 0)
		     sprintf(msg,"%s\n",s);
		   else
		     {
		     strcat(msg,s);
		     strcat(msg,"\n\0");
		   }
		 }
	   break;
	 }
     }
    va_end(args);

   t = XmStringCreateLtoR(msg, XmSTRING_DEFAULT_CHARSET);
   n = 0;
   XtSetArg(wargs[n], XmNokLabelString, yes); n++;
   XtSetArg(wargs[n], XmNmessageString, t); n++;
   if(no){
   XtSetArg(wargs[n], XmNcancelLabelString, no); n++;}
 if(help){
   XtSetArg(wargs[n], XmNhelpLabelString, help); n++;}
 
  item = XmCreateWarningDialog(parent,"Confirmer", wargs,n);
  XtAddCallback(item, XmNokCallback, response, &answer);

 if(no)
  XtAddCallback(item, XmNcancelCallback, response, &answer);

  if(help)
 	make_help(item, helpfile);
  answer = 0;

  XtManageChild(item);
  XmStringFree(t);
  XmStringFree(yes);
 if(no)
  XmStringFree(no);
   else
     XtUnmanageChild(XmMessageBoxGetChild(item, XmDIALOG_CANCEL_BUTTON));
  if(help)
  XmStringFree(help);
   else
  XtUnmanageChild(XmMessageBoxGetChild(item, XmDIALOG_HELP_BUTTON));


  XtPopup(XtParent(item), XtGrabNone);
    while (answer == 0 || XtAppPending(app_context))
        XtAppProcessEvent(app_context, XtIMAll);
  return(answer-1);
}

int confirm_yes(char *s, char *helpfile)
{
  int i,j,n, answer = 0;
  Widget item;
  XmString t = XmStringCreateSimple(s), yes = XmStringCreateSimple("OK");
  XmString no = XmStringCreateSimple("No");
  
  if(base_frame == NULL)
    {
      fprintf(stderr,"%s OK?\n",s);
      return;
    }
  item = XmCreateWarningDialog(base_frame,"Confirmer", NULL, 0);
  XtVaSetValues(item, XmNmessageString, t,
		XmNokLabelString, yes,
		XmNcancelLabelString, no,
 NULL);
  XtAddCallback(item, XmNokCallback, response, &answer);
  XtAddCallback(item, XmNcancelCallback, response, &answer);
  if(helpfile == NULL || (i = make_help(item, helpfile)) < 0)
	XtUnmanageChild(XmMessageBoxGetChild(item, XmDIALOG_HELP_BUTTON));
  	
  answer = 0;

  XtManageChild(item);
  XmStringFree(t);
  XmStringFree(yes);
  XmStringFree(no);
  XtPopup(XtParent(item), XtGrabNone);
    while(answer == 0)
        XtAppProcessEvent(app_context, XtIMXEvent);
  return(answer-1);
}

int confirm_no(char *s, char *helpfile)
{
  int i,j,n,answer = 0,retval;

  Widget item;
  XmString t = XmStringCreateSimple(s), yes = XmStringCreateSimple("No");
  XmString no = XmStringCreateSimple("OK");
  
  if(base_frame == NULL)
    {
      fprintf(stderr,"%s OK?\n",s);
      return;
    }
  item = XmCreateQuestionDialog(base_frame,"Confirmer", NULL, 0);
  XtVaSetValues(item, 
		XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
		XmNmessageString, t,
		XmNokLabelString, yes,
		XmNcancelLabelString, no,
 NULL);

  XtAddCallback(item, XmNokCallback, response, &answer);
  XtAddCallback(item, XmNcancelCallback, response, &answer);
  answer = 0;

  if(helpfile == NULL || (i = make_help(item, helpfile)) < 0)
	XtUnmanageChild(XmMessageBoxGetChild(item, XmDIALOG_HELP_BUTTON));

  XmStringFree(t);
  XmStringFree(yes);
  XmStringFree(no);
  XtManageChild(item);
  XtPopup(XtParent(item), XtGrabNone);
    while (answer == 0)
        XtAppProcessEvent(app_context, XtIMXEvent);
  retval = (answer-1);
  return(retval);
}

/* response() --The user made some sort of response to the
 * question posed in AskUser().  Set the answer (client_data)
 * accordingly and destroy the dialog.
 */
void
response(w, answer, reason)
Widget w;
int *answer;
XmAnyCallbackStruct *reason;
{
    switch (reason->reason) {
        case XmCR_OK:
            *answer = 2;
            break;
        case XmCR_CANCEL:
            *answer = 1;
            break;
        case XmCR_HELP:
            *answer = 3;
            break;
        default:
            return;
    }
}




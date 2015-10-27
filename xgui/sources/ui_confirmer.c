/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_confirmer.c,v $
  SHORTNAME      : confirm.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 6.7.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:16 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include "ui.h"

#include <X11/Shell.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>

#include "ui_main.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"

#include "stopIcon.xbm"
#include "exclamation.xbm"

#include "ui_confirmer.ph"


/*****************************************************************************
  FUNCTION : ui_cf_yesNoOk

  PURPOSE  : terminating of the confirmer
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/
static void ui_cf_yesNoOk (Widget widget, int value)
{
    XtDestroyWidget(ui_cf_popup);
    ui_cf_answer = value;
    ui_cf_exit   = TRUE;
}


/*****************************************************************************
  FUNCTION : ui_confirm

  PURPOSE  : Built and display the confirmer, get a return value (which button
             was pressed, kill the window and return the value to the caller.
  RETURNS  : which button was pressed by the user.
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_confirm (char *message, int ok_only)

{
    Widget      form, picture, label;
    Arg		args[5];
    Widget	button;
    Cardinal	n;
    XEvent      event;

    /* Now create Popup */

    ui_cf_popup = 
	XtCreatePopupShell("confirmer", transientShellWidgetClass, ui_toplevel,
			   NULL, 0);
    form = 
	XtCreateManagedWidget("form", formWidgetClass, ui_cf_popup, NULL, 0);

    if (ok_only) {
	n = 0;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						exclamation_bits, 
						exclamation_width,
						exclamation_height)); n++;
    	picture = 
	    XtCreateManagedWidget("exclamation", labelWidgetClass, form, 
				  args, n);     

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, picture); n++;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	XtSetArg(args[n], XtNlabel,     message); n++;
	label = XtCreateManagedWidget("confirmerMsg", labelWidgetClass, form, 
				      args, n);
    
	button = ui_xCreateButtonItem("ok", form, picture, label);
	XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_cf_yesNoOk, 
		      (XtPointer) ((long)1));	

    } else {
	n = 0;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						stopIcon_bits, stopIcon_width,
						stopIcon_height)); n++;
	picture =
	    XtCreateManagedWidget("stop", labelWidgetClass, form, 
				  args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, picture); n++;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	XtSetArg(args[n], XtNlabel,     message); n++;
	label = 
	    XtCreateManagedWidget("confirmerNsg", labelWidgetClass, form, 
				  args, n);

	button = ui_xCreateButtonItem("yes", form, picture, label);
	XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_cf_yesNoOk, 
		      (XtPointer) ((long)1));	

	button = ui_xCreateButtonItem("no", form, button, label);
	XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_cf_yesNoOk, 
		      (XtPointer) ((long)0));	
    }
    XawFormDoLayout(form, True);
    XtRealizeWidget(ui_cf_popup);
    
    /* now move the popup to the middle of the screen */
    {
	Window root;
	int    x, y;
	unsigned int borderWidth, depth, width, height;
	XWindowChanges xChange;
	
	(void) XGetGeometry(ui_display, XtWindow(ui_cf_popup),
			    &root, &x, &y, &width, &height, &borderWidth, &depth);
	
	xChange.x = 
	    (int) ((XDisplayWidth(ui_display, ui_screen) - width) / 2);
	xChange.y =
	    (int) ((XDisplayHeight(ui_display, ui_screen) - height) / 2);
	xChange.stack_mode = (int) Above;
	XConfigureWindow(ui_display, XtWindow(ui_cf_popup), 
			 CWX BIT_OR CWY BIT_OR CWStackMode, &xChange);
    }

    XtPopup(ui_cf_popup, XtGrabExclusive);
    ui_xDontResizeWidget(ui_cf_popup); 

    /* now enter the confirmer loop.
       Everthing behaves the same, because its an copy of XtAppMainLoop(), 
       but it stops UI from processing code from here back to the main loop.
       This is needed, because the confirmer should return values! */

    ui_cf_exit = FALSE;
    while (NOT ui_cf_exit) {
	XtAppNextEvent(ui_appContext, &event);
	(void) XtDispatchEvent(&event);
    }
}


/*****************************************************************************
  FUNCTION : ui_confirmYes

  PURPOSE  : call the confirmer whith buttons NO and YES and STOP sign
  RETURNS  : TRUE, if the user pressed the YES button, else FALSE;
  NOTES    :

  UPDATE   :
*****************************************************************************/

int ui_confirmYes (char *message)

{
    ui_confirm(message, FALSE);
    return(ui_cf_answer);
}


/*****************************************************************************
  FUNCTION :  ui_confirmOk

  PURPOSE  : call the confirmer with button OK and a exclamation mark icon.
  RETURNS  : void.
  NOTES    : This routine is used to show important messages to the user.
             Don't use it to much, because the user will become nervous!

  UPDATE   :
*****************************************************************************/

void ui_confirmOk (char *message)

{
    ui_confirm(message, TRUE);
}






/* end of file */
/* lines: 225 */

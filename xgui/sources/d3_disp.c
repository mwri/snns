/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_disp.c,v $
  SHORTNAME      : disp.c
  SNNS VERSION   : 4.2

  PURPOSE        : event handler and creation of the display window
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:48 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Cardinals.h>

#include "ui.h"
#include "ui_mainP.h"
#include "ui_key.h"
#include "ui_xGraphic.h"
#include "ui_xWidgets.h"
#include "d3_global.h"
#include "d3_main.h"

#include "d3_disp.ph"


/*****************************************************************************
  FUNCTION : d3_eventProc

  PURPOSE  : this is the event handler of the display window
  RETURNS  : 
  NOTES    :

******************************************************************************/


void d3_eventProc (Widget w, Display *display, XEvent *event)
{
/*
    static int doExpose = TRUE;
*/
    switch (event->type)
      {
        case MapNotify:
               break;    
        case UnmapNotify:
               break;
        case Expose:
	       while (XCheckWindowEvent(display, XtWindow(w), ExposureMask, 
					event))
		   /* do nothing */;
	       d3_drawNet();
/*
               if (event->xexpose.count == 0)
                 {
                   if (doExpose)
                     {
                       d3_drawNet ();
                     }
                   else
                     { 
                       doExpose = TRUE;
                     }
                 }
*/
               break;
        case VisibilityNotify:
               break;
        case ConfigureNotify:
               d3_recenter_window (event->xconfigure.width, 
                                  event->xconfigure.height);
               d3_drawNet ();
/*
               doExpose = FALSE;
*/
               break;
        case CirculateNotify:
               break;
        
      }
}





/*****************************************************************************
  FUNCTION : d3_createDisplayWindow

  PURPOSE  : creates the display window
  RETURNS  : 
  NOTES    :

******************************************************************************/



void d3_createDisplayWindow (void)
{
    Widget       d3_frameWidget, d3_displayWidget;
    Arg          arg[25];
    Cardinal     n;
    char         buf[40];
    unsigned long foreground, background;

    if (d3_displayIsReady)
        return;

    sprintf (buf, "SNNS 3D-display");

    n = 0;  
    d3_displayMainWidget = XtCreatePopupShell (buf, topLevelShellWidgetClass, 
                                               ui_toplevel, NULL, ZERO); 
    d3_frameWidget = XtCreateManagedWidget ("form", formWidgetClass, 
                                            d3_displayMainWidget, NULL, ZERO);

    n = 0;  
    XtSetArg (arg[n], XtNwidth, d3_displayXsize); n++;
    XtSetArg (arg[n], XtNheight, d3_displayYsize); n++;
    d3_displayWidget = XtCreateManagedWidget("display", boxWidgetClass, 
                                             d3_frameWidget, arg, n);

    XtAddEventHandler (d3_displayWidget, StructureNotifyMask | ExposureMask, 
                       FALSE, (XtEventHandler) d3_eventProc, ui_display);

    ui_checkWindowPosition(d3_displayMainWidget);
    XtPopup (d3_displayMainWidget, XtGrabNone);
    XtAddEventHandler (d3_displayWidget, StructureNotifyMask | ExposureMask, 
                       FALSE, (XtEventHandler) d3_eventProc, ui_display);
    XtAddEventHandler(d3_displayWidget,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);

    d3_display = ui_display; 
    d3_window = XtWindow (d3_displayWidget);
    d3_gc = XCreateGC (d3_display, d3_window, ZERO, NULL);
    d3_screen = DefaultScreen (d3_display);

    background = WhitePixel (d3_display, d3_screen);
    foreground = BlackPixel (d3_display, d3_screen);
    XSetBackground (d3_display, d3_gc, background);
    XSetForeground (d3_display, d3_gc, foreground);
    d3_textColor = foreground;

    ui_xToggleBackingStore (TRUE, d3_display, d3_window);

    d3_displayIsReady = TRUE;



}

/* end of file */
/* lines: 190 */

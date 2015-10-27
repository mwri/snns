/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_pannels.c,v $
  SHORTNAME      : pannels.c
  SNNS VERSION   : 4.2

  PURPOSE        : this module creates the control pannel
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/03/13 16:31:49 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/AsciiText.h>

#include "ui.h"
#include "ui_xWidgets.h"
#include "ui_key.h"

#include "d3_global.h"
#include "d3_draw.h"
#include "d3_disp.h"
#include "d3_graph.h"
#include "d3_fonts.h"
#include "d3_move.h"
#include "d3_xUtils.h"
#include "d3_setup.h"
#include "d3_units.h"
#include "d3_links.h"
#include "d3_model.h"
#include "d3_project.h"
#include "d3_light.h"
#include "d3_main.h"
#include "o_graph.h"
#include "d3_zgraph.h"
#include "d3_zValue.h"

#include "d3_pannels.ph"


/*****************************************************************************
  FUNCTION : d3_doneProc 

  PURPOSE  : finishes the 3D View and closes all windows
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void d3_doneProc (void)

{
    if (d3_displayIsReady)
        XtDestroyWidget (d3_displayMainWidget);;
    XtDestroyWidget (d3_controlWidget);;
    d3_displayIsReady = FALSE;
    d3_controlIsReady = FALSE;
}


/*****************************************************************************
  FUNCTION : d3_toggleFreezeButton

  PURPOSE  : toggles the freeze button
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void d3_toggleFreezeButton (Widget w, Widget button, caddr_t call_data)

{
    d3_freeze ^= 1;
    ui_xSetToggleState (button, d3_freeze);    
}


/*****************************************************************************
  FUNCTION : d3_createButtonPannel

  PURPOSE  : creates the pannel for the setup, model, light... buttons
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void d3_createButtonPannel (Widget parent)

{
    Widget box, setup, model, project, light, units, links; 
    Widget freeze, display, done, box2, reset;
    int n;
    Arg arg[10];

    n = 0;
    XtSetArg(arg[n], XtNwidth, 250);          n++;
    XtSetArg(arg[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(arg[n], XtNright , XtChainLeft); n++;
    XtSetArg(arg[n], XtNtop   , XtChainTop);  n++;
    XtSetArg(arg[n], XtNbottom, XtChainTop);  n++;
    box = XtCreateManagedWidget("box", boxWidgetClass, parent, arg, (unsigned int) n);

    setup = d3_xCreateButtonItem ("setup", box, NULL, NULL);
    model = d3_xCreateButtonItem ("model", box, setup, NULL);
    project = d3_xCreateButtonItem ("project", box, model, NULL);
    light = d3_xCreateButtonItem ("light", box, project, NULL);
    units = d3_xCreateButtonItem ("units", box, NULL, setup);
    links = d3_xCreateButtonItem ("links", box, units, model);
    reset = d3_xCreateButtonItem ("reset", box, links, project);
    freeze = d3_xCreateButtonItem ("freeze", box, reset, light);

    XtAddCallback (reset, XtNcallback, (XtCallbackProc) d3_resetDisplay, NULL);
    XtAddCallback (project, XtNcallback, 
		   (XtCallbackProc) d3_createProjectPannel, (caddr_t) project);
    XtAddCallback (units, XtNcallback, 
		   (XtCallbackProc) d3_createUnitPannel, (caddr_t) units);
    XtAddCallback (links, XtNcallback, 
		   (XtCallbackProc) d3_createLinkPannel, (caddr_t) links);
    XtAddCallback (setup, XtNcallback, 
		   (XtCallbackProc) d3_createSetupPannel, (caddr_t) setup);
    XtAddCallback (model, XtNcallback, 
		   (XtCallbackProc) d3_createModelPannel, (caddr_t) model);
    XtAddCallback (light, XtNcallback, 
		   (XtCallbackProc) d3_createLightPannel, (caddr_t) light);
    XtAddCallback (freeze, XtNcallback, 
		   (XtCallbackProc) d3_toggleFreezeButton, (caddr_t) freeze);

    n = 0;
    XtSetArg(arg[n], XtNwidth, 50);                       n++;
    XtSetArg(arg[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(arg[n], XtNright , XtChainLeft); n++;
    XtSetArg(arg[n], XtNtop   , XtChainTop); n++;
    XtSetArg(arg[n], XtNbottom, XtChainTop); n++;
    box2 = XtCreateManagedWidget("box2", boxWidgetClass, parent, 
				 arg, (unsigned int) n);

    display = d3_xCreateButtonItem ("display", box2, NULL, NULL);
    done = d3_xCreateButtonItem ("done2", box2, NULL, display);

    XtAddCallback(display, XtNcallback, 
		  (XtCallbackProc) d3_createDisplayWindow, NULL);
    XtAddCallback(done, XtNcallback, (XtCallbackProc) d3_doneProc, NULL);

    if (d3_state.model_mode == solid)
        d3_initZbuffer ();
}


/*****************************************************************************
  FUNCTION : d3_createControlPannel

  PURPOSE  : creates subpannels of the control pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void d3_createControlPannel (Widget parent)

{
    d3_createMovePannel (parent);
    d3_createButtonPannel (parent);
    d3_createZvaluePannel (parent);
}


/*****************************************************************************
  FUNCTION : d3_displayGraphic

  PURPOSE  : creates the control pannel
  RETURNS  : void
  NOTES    : !!! this is entry point from the XGUI !!!
            the function is a callback from 
               ui_xCreatePanel in module ui_main.c  

  UPDATE   :
******************************************************************************/
void d3_displayGraphic (void)

{

    Widget       d3_frameWidget;
    Arg          arg[25];
    Cardinal     n;
    char         buf[40];

    if (d3_controlIsReady){
	XRaiseWindow(XtDisplay(d3_controlWidget), XtWindow(d3_controlWidget));
       return;
    }

    sprintf (buf, "SNNS 3D-control");
    n = 0;  
    d3_controlWidget = XtCreatePopupShell (buf, topLevelShellWidgetClass, 
                                           ui_toplevel, arg, n); 

    n = 0;
    XtSetArg (arg[n], XtNwidth, 300); n++;
    XtSetArg (arg[n], XtNleft  , XtChainLeft); n++;
    XtSetArg (arg[n], XtNright , XtChainRight); n++;
    XtSetArg (arg[n], XtNtop   , XtChainTop); n++;
    XtSetArg (arg[n], XtNbottom, XtChainBottom); n++;

    d3_frameWidget = XtCreateManagedWidget ("form", boxWidgetClass, 
                                            d3_controlWidget, arg, n);
    d3_createControlPannel (d3_frameWidget);

    ui_checkWindowPosition(d3_controlWidget);
    XtAddEventHandler(d3_frameWidget,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    XtPopup (d3_controlWidget, XtGrabNone);
    ui_xDontResizeWidget(d3_controlWidget); 

    d3_controlIsReady = TRUE;
}
/* end of file */

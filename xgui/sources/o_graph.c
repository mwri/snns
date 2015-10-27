/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/o_graph.c,v $
  SHORTNAME      : o_graph.c
  SNNS VERSION   : 4.2

  PURPOSE        : Contains all functions to run GRAPH.
  NOTES          : You may wonder why all functions have the praefix "o_". 
		   That is easy to explaine: The original name of the progam 
		   was oszi.c but we had to change the name because it was 
		   not English enough.
             	   Small parts of graph.c are in the programs ui_controlP.c 
		   and ui_fileP.c

  AUTHOR         : Markus Heuttel and Michael Schmalzl
  DATE           : 3.4.1992

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.26 $
  LAST CHANGE    : $Date: 1998/03/13 16:31:52 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Grip.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/SimpleMenu.h>


#include "ui.h"
#include "kr_ui.h"
#include "ui_xWidgets.h"
#include "ui_textP.h"
#include "ui_main.h"
#include "ui_fileP.h"
#include "ui_mainP.h"
#include "ui_key.h"
#include "ui_color.h"
#include "ui_controlP.h"
#include "ui_confirmer.h"

#include "o_graph.ph"



/*****************************************************************************
  FUNCTION : o_createOszi

  PURPOSE  : creates the  window GRAPH
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
void o_createOszi (void)
{
    Widget       menu,mItem,form,w1,w2,printlab;
    Widget       Done_oszi, clear_oszi, print_oszi;
    Arg          arg[5];
    Cardinal     n;
    char         buf[40];
    Colormap     o_graph_col;
    unsigned long foreground,background;
    int          curr_unit,test_unit,
    count_unit = 0;


    o_graph_error_scale = GRAPH_SSE;

    /* test whether there are output units defined. Since no error can be 
       computed in the opposit case, the graph tool can not be used */
    curr_unit  = krui_getCurrentUnit();
    test_unit  = krui_getFirstUnit();
    do{
	n=krui_getUnitTType(test_unit);
	if( (n == 2) || (n == 7) ) count_unit = 1;
    }while((test_unit  = krui_getNextUnit()) && (count_unit == 0));
    curr_unit  = krui_setCurrentUnit(curr_unit);
 
    if(count_unit == 0){
	ui_confirmOk("No Output units defined!\nAn error can neither be\ncomputed nor displayed!");
	return;
    }


    if(o_open) {
	XRaiseWindow (XtDisplay (o_displayMainWidget), 
		      XtWindow (o_displayMainWidget));
	return;
    }
	
    o_open = 1; 
    o_init();
    
    sprintf (buf, "SNNS graph");
    n = 0;  

    XtSetArg(arg[n],XtNminWidth,460); n++;  
    XtSetArg(arg[n],XtNminHeight,200); n++;
    XtSetArg(arg[n],XtNheight,o_WindowHeight+58); n++;
    XtSetArg(arg[n],XtNwidth,o_WindowWidth+10); n++;
    XtSetArg(arg[n],XtNborderWidth,1); n++;
    
    o_displayMainWidget = XtCreatePopupShell(buf, topLevelShellWidgetClass,
					     ui_toplevel, arg, n); 
    n = 0;  
    form = XtCreateManagedWidget ("form", formWidgetClass, 
				  o_displayMainWidget, arg, n);

    grid_oszi = ui_xCreateToggleItem ("grid",form,NULL,NULL,NULL);
    ui_xSetToggleState (grid_oszi, FALSE) ; 
    XtAddCallback (grid_oszi, XtNcallback, (XtCallbackProc) o_gridProc, NULL);

    print_oszi = ui_xCreateButtonItem ("print",form,grid_oszi,NULL);
    XtAddCallback(print_oszi, XtNcallback, (XtCallbackProc) o_printProc, NULL);

    if(strlen(o_printfile) == 0)sprintf(o_printfile,"./graph.ps");
    printlab = ui_xCreateLabelItem("Print to file:",form,14*8,print_oszi,NULL); 
    o_printW = ui_xCreateDialogItem ("o_printW",form,o_printfile,0,
				     printlab,NULL);

    Done_oszi = ui_xCreateButtonItem ("done",form,NULL,grid_oszi);
    XtAddCallback(Done_oszi,XtNcallback,(XtCallbackProc) o_DoneProc,NULL);

    clear_oszi = ui_xCreateButtonItem ("clear",form,Done_oszi,grid_oszi);
    XtAddCallback(clear_oszi,XtNcallback,(XtCallbackProc) o_clearProc,NULL);

    w1 = ui_xCreateLabelItem ("Scale X:",form,7*8,print_oszi,grid_oszi); 
    w2 = ui_xCreateButtonItem ("prev",form,w1,grid_oszi);
    XtAddCallback(w2,XtNcallback,(XtCallbackProc) o_XForwardProc,NULL);
    w1 = ui_xCreateButtonItem ("next",form,w2,grid_oszi);
    XtAddCallback(w1,XtNcallback,(XtCallbackProc) o_XBackProc,NULL);

    w2 = ui_xCreateLabelItem ("  Scale Y:",form,9*8,w1,grid_oszi); 
    w1 = ui_xCreateButtonItem ("prev",form,w2,grid_oszi); 
    XtAddCallback(w1,XtNcallback,(XtCallbackProc) o_YBackProc,NULL);
    w2 = ui_xCreateButtonItem ("next",form,w1,grid_oszi);
    XtAddCallback(w2,XtNcallback,(XtCallbackProc) o_YForwardProc,NULL);

    w1 = ui_xCreateLabelItem ("  Display:",form,9*8,w2,grid_oszi); 
    o_scaleWidget = ui_xCreateMenuButtonItem("   SSE  ",form,w1,grid_oszi);
    menu  = XtCreatePopupShell("menu",simpleMenuWidgetClass,o_scaleWidget,
			       NULL,ZERO);
    mItem = XtCreateManagedWidget("  SSE",smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)o_set_err_scale,(caddr_t)1);
    mItem = XtCreateManagedWidget("  MSE",smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)o_set_err_scale,(caddr_t)2);
    mItem = XtCreateManagedWidget("SSE/#out", smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)o_set_err_scale,(caddr_t)3);

    o_DisplayWidget = o_xCreateScreenItem("screen",form,o_WindowWidth,
					  o_WindowHeight,NULL,clear_oszi);
    XtAddEventHandler(o_DisplayWidget,StructureNotifyMask | ExposureMask, 
		      FALSE,(XtEventHandler) o_eventProc,o_display);

    XtAddEventHandler(form,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);

    ui_checkWindowPosition(o_displayMainWidget);
    XtPopup (o_displayMainWidget, XtGrabNone);

    o_display = XtDisplay (o_DisplayWidget); 
    o_window = XtWindow (o_DisplayWidget);
    o_fontStruct = XLoadQueryFont(o_display, "6x12");

    o_gc[label_gc] = XCreateGC (o_display, o_window, ZERO, NULL);
    o_gc[train_gc] = XCreateGC (o_display, o_window, ZERO, NULL);
    o_gc[test_gc] = XCreateGC (o_display, o_window, ZERO, NULL);
    XSetFont(o_display,o_gc[label_gc],(*o_fontStruct).fid);
    o_screen = DefaultScreen (o_display);
    o_depth = DisplayPlanes(o_display,o_screen);
    o_graph_col = DefaultColormap(o_display, o_screen);

    if(ui_col_monochromeMode == FALSE){
	/* settings for black learning curve */
	fg.red = 0; fg.green = 0; fg.blue = 0; 
	fg2.red = 65535; fg2.green = 0; fg2.blue = 0;

    }else{
	XSetLineAttributes(o_display, o_gc[test_gc],1,LineDoubleDash,
			   CapButt,JoinMiter);
	fg.red = 0; fg.green = 0; fg.blue = 0;
	fg2.red = 0; fg2.green = 0; fg2.blue = 0;
    }
    background = WhitePixel (o_display, o_screen);
    XSetBackground (o_display, o_gc[train_gc], background);
    XSetBackground (o_display, o_gc[test_gc],  background);
    XSetBackground (o_display, o_gc[label_gc], background);
    XAllocColor(o_display,o_graph_col,&fg);
    XAllocColor(o_display,o_graph_col,&fg2);
    foreground = BlackPixel (o_display, o_screen); 
    XSetForeground (o_display, o_gc[train_gc], fg.pixel); 
    XSetForeground (o_display, o_gc[test_gc],  fg2.pixel); 
    XSetForeground (o_display, o_gc[label_gc], foreground); 

    XSetGraphicsExposures(o_display,o_gc[label_gc],0);
    XSetGraphicsExposures(o_display,o_gc[train_gc],0);
    XSetGraphicsExposures(o_display,o_gc[test_gc],0);

    o_Pixmap = XCreatePixmap(o_display,o_window, (unsigned int) o_PixmapWidth, 
			     (unsigned int) o_PixmapHeight, 
			     (unsigned int) o_depth);
    o_ClearPixmap(o_display,o_Pixmap,o_gc[train_gc],fg.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight); 
    o_ClearPixmap(o_display,o_Pixmap,o_gc[test_gc],fg2.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight); 
    XClearArea(o_display,o_window,o_OsziXPos,o_OsziYPos, 
	       (unsigned int) o_OsziWidth-1, (unsigned int) o_OsziHeight,1);
    o_PressPossible = 1;
}


/*****************************************************************************
  FUNCTION : o_eventProc

  PURPOSE  : Manages the events (resize and exposure)
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_eventProc (Widget w, Display *display, XEvent *event)
{
    switch (event->type) {
    case Expose: {
	if(event->xexpose.count == 0) {
	    XCopyArea(o_display,o_Pixmap,o_window,o_gc[train_gc],0,0,
		      (unsigned int) o_OsziWidth, (unsigned int) o_OsziHeight,
		      o_OsziXPos,o_OsziYPos);
	    XDrawRectangle(o_display,o_window,o_gc[label_gc],o_OsziFrameXPos,
			   o_OsziFrameYPos, (unsigned int) o_OsziWidth+1, 
			   (unsigned int) o_OsziHeight);
	    o_ScaleY(o_display,o_window,o_gc[label_gc]);
	    o_ScaleX(o_display,o_window,o_gc[label_gc]);
	}
	break;
    }
    case ConfigureNotify: {
	o_ResizeOszi(event->xconfigure.width,event->xconfigure.height);
	break;
    }
    }     
}

/*****************************************************************************
  FUNCTION : o_set_err_scale

  PURPOSE  : callback function of the select-button. 
             Selects the type of graph to be plotted
  RETURNS  : void
  NOTES    :

  UPDATE   : 13.04.95
******************************************************************************/
static void o_set_err_scale(Widget w, int type, caddr_t call_data)
{

    switch(type){
    case 1: 
	o_graph_error_scale = GRAPH_SSE;
	ui_xSetLabel(o_scaleWidget,"SSE");
	break;
    case 2:
	o_graph_error_scale = GRAPH_MSE;
	ui_xSetLabel(o_scaleWidget,"MSE");
	break;
    case 3:
	o_graph_error_scale = GRAPH_SSE_DIV_OUT;
	ui_xSetLabel(o_scaleWidget,"SSE/out");
	break;
    }
}


/*****************************************************************************
  FUNCTION : o_clearProc

  PURPOSE  : callback function of the clear-buttom. 
             Clears all curves of the display.
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_clearProc (void)
{
    if(o_PressPossible){
	o_ClearPixmap(o_display,o_Pixmap,o_gc[train_gc],fg.pixel,o_screen,0,0,
		      o_PixmapWidth,o_PixmapHeight);
	o_ClearPixmap(o_display,o_Pixmap,o_gc[test_gc],fg2.pixel,o_screen,0,0,
		      o_PixmapWidth,o_PixmapHeight);
	XClearArea(o_display,o_window,o_OsziXPos,o_OsziYPos,
		   (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight-1,1);
	o_ClearCurves();
	o_LearnStepCount = 0;
    }
}


/*****************************************************************************
  FUNCTION : o_gridProc

  PURPOSE  : callback function of the grid-buttom. 
             Draws a grid into the graph window
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.09.97
******************************************************************************/
static void o_gridProc (void)
{

    oszi_drawgrid = ui_xGetToggleState(grid_oszi);
    if (oszi_drawgrid == TRUE){
	/* draw the grid */
	o_ScaleY(o_display,o_window,o_gc[label_gc]);
	o_ScaleX(o_display,o_window,o_gc[label_gc]);
    }else{           
	/* erase the grid */
	XCopyArea(o_display,o_Pixmap,o_window,o_gc[train_gc],0,0,
		  (unsigned int) o_OsziWidth, (unsigned int) o_OsziHeight-1,
		  o_OsziXPos,o_OsziYPos);
    }
}


/*****************************************************************************
  FUNCTION : o_printProc

  PURPOSE  : callback function of the print-buttom. 
             Dumps window to file.
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.09.97
******************************************************************************/
static void o_printProc (void)
{
    char command[256];
    char message[256];

    if(!o_PressPossible)
	return;

    /* get current filename */
    ui_xStringFromAsciiWidget(o_printW,o_printfile,MAX_NAME_LENGTH);

    /* check whether we would overwrite anything */
    if(ui_fileExist(o_printfile, 0)){
	sprintf(message,"File %s already exist! Overwrite?",o_printfile);
	if(!ui_confirmYes(message))return;
    }

    /* put window on top */
    XFlush(XtDisplay(o_displayMainWidget));
    XRaiseWindow(XtDisplay(o_displayMainWidget), 
		  XtWindow(o_displayMainWidget));

    /* grab and save window */
#ifndef HAVE_XGRABSC
    sprintf(command,"xwd -frame -id %d | xpr -o %s &",
	    (int) XtWindow (o_displayMainWidget),o_printfile);
#else
    sprintf(command,"xgrabsc -nobell -s 2 -id %d -bdrs -o %s &",
	    (int) XtWindow (o_displayMainWidget),o_printfile);
#endif

    system(command);
}

/*****************************************************************************
  FUNCTION : o_DoneProc

  PURPOSE  : callback function of the done-button. 
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_DoneProc (void)
{
    if(o_PressPossible){
	o_clearProc();
	o_graph_error_scale = 1;
	XtDestroyWidget(o_displayMainWidget);
	o_open = 0;
    }
}


/*****************************************************************************
  FUNCTION : o_xCreateScreenItem

  PURPOSE  : Creates the widget on which the curves are drawn.
  RETURNS  : Widget
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static Widget  o_xCreateScreenItem (char *name, Widget parent, Dimension width,
				    Dimension height, Widget left, Widget top)
{ 
    Cardinal n;
    Widget   w;
    Arg      args[15];

    n = 0;
    XtSetArg(args[n], XtNheight,height); n++;
    XtSetArg(args[n], XtNwidth,width); n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;

    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainRight); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainBottom); n++;

    XtSetArg(args[n],XtNresizable,True); n++;

    w = XtCreateManagedWidget(name, formWidgetClass, parent, args, n);

    return(w);
}


/*****************************************************************************
  FUNCTION : o_init

  PURPOSE  : Initializes all variables of GRAPH. 
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_init (void)
{ 
    int i;

    o_WindowWidth = 490;
    o_WindowHeight = 250;
    o_LearnStepCount = 0;
    o_YCounter = 3;
    o_XCounter = 9;

    o_PixelsOfOneLearningStep = o_XScaleValues[o_XCounter].ScreenEinheit / 
	o_XScaleValues[o_XCounter].Einheit; 
    o_PixelsOfOneLearningStep = ((o_PixelsOfOneLearningStep == 0) ? 
				 (1) : (o_PixelsOfOneLearningStep));
    o_LearningStepsOfOnePixel = o_XScaleValues[o_XCounter].Einheit /
	o_XScaleValues[o_XCounter].ScreenEinheit;
    o_LearningStepsOfOnePixel = ((o_LearningStepsOfOnePixel == 1) 
				 ? (0) : (o_LearningStepsOfOnePixel));

    o_AbsoluteScale = o_YScaleValues[o_YCounter].ScreenEinheit /
	o_YScaleValues[o_YCounter].Einheit;

    o_RightOffset      = o_PixelsOfOneLearningStep;
    o_OsziXPos         = o_SpaceLeft;
    o_OsziYPos         = o_SpaceAbove;
    o_OsziHeight       = 200;  
    o_OsziWidth        = 400; 
    o_PixmapWidth      = o_OsziWidth+o_RightOffset;
    o_PixmapHeight     = o_OsziHeight;
    o_OsziFrameXPos   = o_OsziXPos - 1; 
    o_OsziFrameYPos   = o_OsziYPos - 1;
    o_OsziFrameWidth  = o_OsziWidth + 2;
    o_OsziFrameHeight = o_OsziHeight + 2;
    o_MaxYDrawPos      = o_OsziHeight-1;

    o_CurveNo = 0;
    for(i=0;i<MAX_CURVE_NO;i++) {
	o_CurveLengths[i] = -1;
    }
    o_InitCurve();
}


/*****************************************************************************
  FUNCTION : o_InitCurve

  PURPOSE  : Allocates space for a new curve.
  RETURNS  : void
  NOTES    : TestFunction implemented joe 

  UPDATE   : 08.03.95
******************************************************************************/
void o_InitCurve (void)
{
    char buf[80];
 
    if(o_open && (o_CurveLengths[o_CurveNo] != 0)) {
	if((o_CurveLengths[o_CurveNo] == -1)&&
	   (o_CurveLengths[o_CurveNo+1] == -1)) {
	    o_DrawAllowed = 1;
	    o_MaxCurveLengths = (int)(o_OsziWidth/o_PixelsOfOneLearningStep);
	    o_Curve1[o_CurveNo] = 
		(XPoint *)malloc(sizeof(XPoint)*o_MaxCurveLengths);
	    o_Curve2[o_CurveNo] = 
		(MPoint *)malloc(sizeof(MPoint)*o_MaxCurveLengths);
	    o_CurveLengths[o_CurveNo] = 0;
	    o_Curve1[o_CurveNo][0].x = -1;
	    o_Curve1[o_CurveNo][0].y = -1;
	    o_Curve2[o_CurveNo][0].x =  0;
	    o_Curve2[o_CurveNo][0].y =  0;

	    o_Curve1[o_CurveNo+1] = 
		(XPoint *)malloc(sizeof(XPoint)*o_MaxCurveLengths);
	    o_Curve2[o_CurveNo+1]   = 
		(MPoint *)malloc(sizeof(MPoint)*o_MaxCurveLengths);
	    o_CurveLengths[o_CurveNo+1] = 0;
	    o_Curve1[o_CurveNo+1][0].x = -1;
	    o_Curve1[o_CurveNo+1][0].y = -1;
	    o_Curve2[o_CurveNo+1][0].x =  0;
	    o_Curve2[o_CurveNo+1][0].y =  0;
	} else {
	    o_DrawAllowed = 0;
	    sprintf(buf,"The max no of curves you can draw is %d",MAX_CURVE_NO);
	    ui_tw_errorMessage(buf);
	}
    }
}


/*****************************************************************************
  FUNCTION : o_ClearCurves

  PURPOSE  : Deletes all curves. (The curves vanish from display and the 
             storage is set free.
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_ClearCurves (void)
{

    for(o_CurveNo++;o_CurveNo>=0;o_CurveNo--){
	o_CurveLengths[o_CurveNo]= -1;
	free(o_Curve1[o_CurveNo]);
	free(o_Curve2[o_CurveNo]);
    }
    o_CurveNo = 0;
    o_InitCurve();
}


/*****************************************************************************
  FUNCTION : o_ResizeOszi

  PURPOSE  : This function is called when there is a resize event by the 
             X-server. The variables are initialized with their new values,
             the storage for the current curve (only for this one) is 
	     reallocated and the curves are drawn again.
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_ResizeOszi (int box_width, int box_height)
{
    int Length,UsedStorage,NeededStorage,FreeStorage,x;
    int i,j;
 
    o_WindowWidth = box_width;
    o_WindowHeight = box_height;

    o_PixelsOfOneLearningStep = o_XScaleValues[o_XCounter].ScreenEinheit /
	o_XScaleValues[o_XCounter].Einheit;
    o_PixelsOfOneLearningStep = ((o_PixelsOfOneLearningStep == 0) ? 
				 (1) : (o_PixelsOfOneLearningStep));
    o_LearningStepsOfOnePixel = o_XScaleValues[o_XCounter].Einheit /
	o_XScaleValues[o_XCounter].ScreenEinheit;
    o_LearningStepsOfOnePixel = ((o_LearningStepsOfOnePixel == 1) 
				 ? (0) : (o_LearningStepsOfOnePixel));

    o_RightOffset      = o_PixelsOfOneLearningStep;
    o_OsziHeight       = o_WindowHeight-o_SpaceAbove-o_SpaceBelow;  
    o_OsziWidth        = o_WindowWidth-o_SpaceLeft-o_SpaceRight;
    o_PixmapWidth      = o_OsziWidth+o_RightOffset;
    o_PixmapHeight     = o_OsziHeight;
    o_OsziFrameWidth   = o_OsziWidth + 2;
    o_OsziFrameHeight  = o_OsziHeight + 2;
    o_MaxYDrawPos      = o_OsziHeight-1;

    for(i=0;i<=o_CurveNo+1;i++) {
	for(j=0;j<o_CurveLengths[i];j++) {
	    o_Curve1[i][j].y = (int) (o_MaxYDrawPos-(o_Curve2[i][j].y*
						     o_AbsoluteScale));
	}
    }

    Length = ((o_CurveLengths[o_CurveNo]-1 == -1)?
	      (0):(o_CurveLengths[o_CurveNo]-1)); 
    x = ((o_Curve1[o_CurveNo][Length].x == -1)?
	 (0):(o_Curve1[o_CurveNo][Length].x)); 
    UsedStorage = o_CurveLengths[o_CurveNo];
    FreeStorage = (int)((o_OsziWidth-x)/o_PixelsOfOneLearningStep);
    NeededStorage = ((FreeStorage <= 0)?(0):(FreeStorage+UsedStorage));

    if(NeededStorage > UsedStorage) {
	o_Curve1[o_CurveNo] = (XPoint *)realloc(o_Curve1[o_CurveNo],
						NeededStorage*sizeof(XPoint));
	o_Curve2[o_CurveNo] = (MPoint *)realloc(o_Curve2[o_CurveNo],
						NeededStorage*sizeof(MPoint));
	o_Curve1[o_CurveNo+1] = (XPoint *)realloc(o_Curve1[o_CurveNo+1],
						  NeededStorage*sizeof(XPoint));
	o_Curve2[o_CurveNo+1] = (MPoint *)realloc(o_Curve2[o_CurveNo+1],
						  NeededStorage*sizeof(MPoint));
	o_MaxCurveLengths = NeededStorage;
    }

    XFreePixmap(o_display,o_Pixmap);
    o_Pixmap = XCreatePixmap(o_display,o_window,(unsigned int) o_PixmapWidth, 
			     (unsigned int) o_PixmapHeight, 
			     (unsigned int) o_depth);
    o_ClearPixmap(o_display,o_Pixmap,o_gc[train_gc],fg.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight); 
    o_ClearPixmap(o_display,o_Pixmap,o_gc[test_gc],fg2.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight); 
    for(i=0;i<=o_CurveNo;i+=2) {
	XDrawLines(o_display,o_Pixmap,o_gc[train_gc],o_Curve1[i],
		   o_CurveLengths[i],CoordModeOrigin);
	for(j=0;j<o_CurveLengths[i+1]-1;j++){
	    if(o_Curve1[i+1][j].y != -999 && o_Curve1[i+1][j+1].y != -999){
		XDrawLine(o_display,o_Pixmap,o_gc[test_gc],
			  o_Curve1[i+1][j].x,o_Curve1[i+1][j].y,
			  o_Curve1[i+1][j+1].x,o_Curve1[i+1][j+1].y);
	    }
	}
    }
    XClearArea(o_display,o_window,0,0,(unsigned int) o_WindowWidth, 
	       (unsigned int) o_WindowHeight,0); 
    XCopyArea(o_display,o_Pixmap,o_window,o_gc[train_gc],0,0,
	      (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);

    XCopyArea(o_display,o_Pixmap,o_window,o_gc[test_gc],0,0,
	      (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);

    XDrawRectangle(o_display,o_window,o_gc[label_gc],o_OsziFrameXPos,
		   o_OsziFrameYPos,(unsigned int) o_OsziWidth+1, 
		   (unsigned int) o_OsziHeight);

    o_ScaleY(o_display,o_window,o_gc[label_gc]); 
    o_ScaleX(o_display,o_window,o_gc[label_gc]); 

    XtPopup (o_displayMainWidget, XtGrabNone);
}


/*****************************************************************************
  FUNCTION : o_XScaleCurve

  PURPOSE  : After the size of the x-scala has changed, the pixel position
             of the x-values must be calculated again. (Remember: the array
             Curve1 contains the pixel positions of the curves, while the
             array Curve2 contains the number of learnsteps as x-value and
             the learn-error as y-value.)
  RETURNS  : void
  NOTES    : Is called by o_XBackProc and by o_XForwardProc.

  UPDATE   : 08.03.95
******************************************************************************/
static void o_XScaleCurve (void)
{
    int i,j,tmp_Curve1_x;
    double PixelsOfOneLearningStep;
   
    for(i=0;i<=o_CurveNo+1;i++) { 
	PixelsOfOneLearningStep = o_XScaleValues[o_XCounter].ScreenEinheit /
	    o_XScaleValues[o_XCounter].Einheit;
	for(j=0;j<o_CurveLengths[i];j++) {
	    tmp_Curve1_x = o_Curve2[i][j].x*PixelsOfOneLearningStep-1;
	    /* This is necessery to prevent too large values for 
	       o_Curve1[i][j].x, which is of the type "short" (XPoint),
	       when scaling downward.  */ 
	    o_Curve1[i][j].x = (tmp_Curve1_x > 32767) ? (32767):(tmp_Curve1_x);
	}
    }
}


/*****************************************************************************
  FUNCTION : o_XBackProc

  PURPOSE  : callback function which changes the scala of the x-axis.
             The scala of the x-axis is enlarged. The variables have to be 
             initialized with new values and the storage of the current curve 
             (only for this one) has to be reallocated. Before drawing all 
             visible curves, the function o_XScaleCurve() must be called.
  RETURNS  : void
  NOTES    :

  UPDATE   : 18.09.97
******************************************************************************/
static void o_XBackProc (void)
{
    int i,j,Length;
    double PixelsOfOneLearningStep;
    int UsedStorage,FreeStorage,NeededStorage;


    if(!o_PressPossible)
	return;

    o_XCounter = (--o_XCounter < 0) ? (0) : (o_XCounter);

    o_PixelsOfOneLearningStep = 
	PixelsOfOneLearningStep = 
	    o_XScaleValues[o_XCounter].ScreenEinheit /
		o_XScaleValues[o_XCounter].Einheit;
    o_PixelsOfOneLearningStep = ((o_PixelsOfOneLearningStep == 0) ? 
				 (1) : (o_PixelsOfOneLearningStep));
    o_LearningStepsOfOnePixel = o_XScaleValues[o_XCounter].Einheit /
	o_XScaleValues[o_XCounter].ScreenEinheit;
    o_LearningStepsOfOnePixel = ((o_LearningStepsOfOnePixel == 1) ? 
				 (0) : (o_LearningStepsOfOnePixel));

    Length = (o_CurveLengths[o_CurveNo]-1 == -1)?
	(0):(o_CurveLengths[o_CurveNo]-1);
    UsedStorage = o_CurveLengths[o_CurveNo];
    FreeStorage = (int)((o_OsziWidth-(o_Curve2[o_CurveNo][Length].x*
				      PixelsOfOneLearningStep-1))/
			o_PixelsOfOneLearningStep);
    NeededStorage = ((FreeStorage <= 0)?(0):(FreeStorage+UsedStorage));

    if(NeededStorage>o_MaxCurveLengths){
	o_Curve1[o_CurveNo] = 
	    (XPoint *)realloc(o_Curve1[o_CurveNo],NeededStorage*sizeof(XPoint));
	o_Curve2[o_CurveNo] = 
	    (MPoint *)realloc(o_Curve2[o_CurveNo],NeededStorage*sizeof(MPoint));

	o_Curve1[o_CurveNo+1] = 
	    (XPoint *)realloc(o_Curve1[o_CurveNo+1],
			      NeededStorage*sizeof(XPoint));
	o_Curve2[o_CurveNo+1] = 
	    (MPoint *)realloc(o_Curve2[o_CurveNo+1],
			      NeededStorage*sizeof(MPoint));

	o_MaxCurveLengths = NeededStorage;
    }
    o_XScaleCurve();
    XClearArea(o_display,o_window,0,0,(unsigned int) o_WindowWidth,
	       (unsigned int) o_WindowHeight,0); 
    o_ClearPixmap(o_display,o_Pixmap,o_gc[train_gc],fg.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight); 
    o_ClearPixmap(o_display,o_Pixmap,o_gc[test_gc],fg2.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight); 
    for(i=0;i<=o_CurveNo;i+=2) { 
	XDrawLines(o_display,o_Pixmap,o_gc[train_gc],o_Curve1[i],
		   o_CurveLengths[i],CoordModeOrigin);
	for(j=0;j<o_CurveLengths[i+1]-1;j++){
	    if(o_Curve1[i+1][j].y != -999 && o_Curve1[i+1][j+1].y != -999){
		XDrawLine(o_display,o_Pixmap,o_gc[test_gc],
			  o_Curve1[i+1][j].x,o_Curve1[i+1][j].y,
			  o_Curve1[i+1][j+1].x,o_Curve1[i+1][j+1].y);
	    }
	}
    }
    XCopyArea(o_display,o_Pixmap,o_window,o_gc[train_gc],0,0,
	      (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);
    XCopyArea(o_display,o_Pixmap,o_window,o_gc[test_gc],0,0,
	      (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);
    XDrawRectangle(o_display,o_window,o_gc[label_gc],o_OsziFrameXPos,
		   o_OsziFrameYPos,(unsigned int) o_OsziWidth+1,
		   (unsigned int) o_OsziHeight);
    o_ScaleY(o_display,o_window,o_gc[label_gc]); 
    o_ScaleX(o_display,o_window,o_gc[label_gc]); 
}


/*****************************************************************************
  FUNCTION : o_XForwardProc

  PURPOSE  : callback function which changes the scala of the x-axis.
             The scala of the x-axis is lessened. The variables have to be 
             initialized with new values and the storage of the current curve 
             (only for this one) has to be reallocated. Before drawing all 
             visible curves, the function o_XScaleCurve() must be called. 
  RETURNS  : void
  NOTES    :

  UPDATE   : 18.09.97
******************************************************************************/
static void o_XForwardProc (void)
{
    int i,j,Length;
    double PixelsOfOneLearningStep;
    int UsedStorage,FreeStorage,NeededStorage;


    if(!o_PressPossible || (o_XCounter == 14))
	return;

    o_XCounter++;

    o_PixelsOfOneLearningStep = 
	PixelsOfOneLearningStep = 
	o_XScaleValues[o_XCounter].ScreenEinheit /
	o_XScaleValues[o_XCounter].Einheit;
    o_PixelsOfOneLearningStep = ((o_PixelsOfOneLearningStep == 0) ? 
				 (1) : (o_PixelsOfOneLearningStep));
    o_LearningStepsOfOnePixel = o_XScaleValues[o_XCounter].Einheit /
	o_XScaleValues[o_XCounter].ScreenEinheit;
    o_LearningStepsOfOnePixel = ((o_LearningStepsOfOnePixel == 1) ? 
				 (0) : (o_LearningStepsOfOnePixel));

    Length = (o_CurveLengths[o_CurveNo]-1 == -1)?
	(0):(o_CurveLengths[o_CurveNo]-1);

    UsedStorage = o_CurveLengths[o_CurveNo];
    FreeStorage = (int)((o_OsziWidth-(o_Curve2[o_CurveNo][Length].x
				      *PixelsOfOneLearningStep-1)) /
			o_PixelsOfOneLearningStep);
    NeededStorage = ((FreeStorage <= 0)?(0):(FreeStorage+UsedStorage));

    if(NeededStorage){
	o_Curve1[o_CurveNo] = 
	    (XPoint *)realloc(o_Curve1[o_CurveNo],
			      NeededStorage*sizeof(XPoint));
	o_Curve2[o_CurveNo] =
	    (MPoint *)realloc(o_Curve2[o_CurveNo],
			      NeededStorage*sizeof(MPoint));

	o_Curve1[o_CurveNo+1] = 
	    (XPoint *)realloc(o_Curve1[o_CurveNo+1],
			      NeededStorage*sizeof(XPoint));
	o_Curve2[o_CurveNo+1] = 
	    (MPoint *)realloc(o_Curve2[o_CurveNo+1],
			      NeededStorage*sizeof(MPoint));

	o_MaxCurveLengths = NeededStorage;
    }
    o_XScaleCurve();
    XClearArea(o_display,o_window,0,0,(unsigned int) o_WindowWidth,
	       (unsigned int) o_WindowHeight,0); 
    o_ClearPixmap(o_display,o_Pixmap,o_gc[train_gc],fg.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight); 
    o_ClearPixmap(o_display,o_Pixmap,o_gc[test_gc],fg2.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight); 
    for(i=0;i<=o_CurveNo;i+=2) {
	XDrawLines(o_display,o_Pixmap,o_gc[train_gc],o_Curve1[i],
		   o_CurveLengths[i],CoordModeOrigin);
	for(j=0;j<o_CurveLengths[i+1]-1;j++){
	    if(o_Curve1[i+1][j].y != -999 && o_Curve1[i+1][j+1].y != -999){
		XDrawLine(o_display,o_Pixmap,o_gc[test_gc],
			  o_Curve1[i+1][j].x,o_Curve1[i+1][j].y,
			  o_Curve1[i+1][j+1].x,o_Curve1[i+1][j+1].y);
	    }
	}
    }

    XCopyArea(o_display,o_Pixmap,o_window,o_gc[train_gc],0,0,
	      (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);
    XCopyArea(o_display,o_Pixmap,o_window,o_gc[test_gc],0,0,
	      (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);
    XDrawRectangle(o_display,o_window,o_gc[label_gc],
		   o_OsziFrameXPos,o_OsziFrameYPos,
		   (unsigned int) o_OsziWidth+1,(unsigned int)o_OsziHeight);
    o_ScaleY(o_display,o_window,o_gc[label_gc]); 
    o_ScaleX(o_display,o_window,o_gc[label_gc]); 
}


/*****************************************************************************
  FUNCTION : o_YScaleCurve

  PURPOSE  : After the size of the y-scala has changed, the pixel position
             of the y-values must be calculated again. (Remember: the array
             Curve1 contains the pixel positions of the curves, while the
             array Curve2 contains the number of learnsteps as x-value and
             the learn-error as y-value.)
  RETURNS  : void
  NOTES    : Is called by o_YForwardCurve() and o_YBack().

  UPDATE   : 08.03.95
******************************************************************************/
static void o_YScaleCurve (void)
{
    int i,j;
   
    for(i=0;i<=o_CurveNo+1;i++) {
	for(j=0;j<o_CurveLengths[i];j++) {
	    if(o_Curve1[i][j].y != -999){
		o_Curve1[i][j].y = 
		    (int)(o_MaxYDrawPos-(o_Curve2[i][j].y * o_AbsoluteScale));
		if (o_Curve1[i][j].y < 0)
		    o_Curve1[i][j].y = -1;
	    }
	}
    }
}


/*****************************************************************************
  FUNCTION : o_YForwardProc

  PURPOSE  : callback function which changes the scala of the y-axis.
             The scala of the y-axis is enlarged. The variables have to be 
             initialized with new values and the storage of the current curve 
             (only for this one) has to be reallocated. Before drawing all 
             visible curves, the function o_YScaleCurve() must be called.
  RETURNS  : void
  NOTES    :

  UPDATE   : 18.09.97
******************************************************************************/
static void o_YForwardProc (void)
{
    int i,j;


    if(!o_PressPossible)
	return;

    o_YCounter = (++o_YCounter > 14) ? (14) : (o_YCounter);

    o_AbsoluteScale = o_YScaleValues[o_YCounter].ScreenEinheit /
	o_YScaleValues[o_YCounter].Einheit;

    o_YScaleCurve();
    XClearArea(o_display,o_window,0,0,(unsigned int) o_WindowWidth,
	       (unsigned int) o_WindowHeight,0);  
    o_ClearPixmap(o_display,o_Pixmap,o_gc[train_gc],fg.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight);
    o_ClearPixmap(o_display,o_Pixmap,o_gc[test_gc],fg2.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight);
 
    for(i=0;i<=o_CurveNo;i+=2) {
	XDrawLines(o_display,o_Pixmap,o_gc[train_gc],o_Curve1[i],
		   o_CurveLengths[i],CoordModeOrigin);
	for(j=0;j<o_CurveLengths[i+1]-1;j++){
	    if(o_Curve1[i+1][j].y != -999 && o_Curve1[i+1][j+1].y != -999){
		XDrawLine(o_display,o_Pixmap,o_gc[test_gc],
			  o_Curve1[i+1][j].x,o_Curve1[i+1][j].y,
			  o_Curve1[i+1][j+1].x,o_Curve1[i+1][j+1].y);
	    }
	}
    }

    XCopyArea(o_display,o_Pixmap,o_window,o_gc[train_gc],0,0,
	      (unsigned int) o_OsziWidth, (unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);
    XCopyArea(o_display,o_Pixmap,o_window,o_gc[test_gc],0,0,
	      (unsigned int) o_OsziWidth, (unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);
    XDrawRectangle(o_display,o_window,o_gc[label_gc],o_OsziFrameXPos,
		   o_OsziFrameYPos,(unsigned int) o_OsziWidth+1,
		   (unsigned int) o_OsziHeight);
    o_ScaleY(o_display,o_window,o_gc[label_gc]); 
    o_ScaleX(o_display,o_window,o_gc[label_gc]);
}


/*****************************************************************************
  FUNCTION : o_YBackProc

  PURPOSE  : callback function which changes the scala of the y-axis.
             The scala of the y-axis is lessened. The variables have to be 
             initialized with new values and the storage of the current curve 
             (only for this one) has to be reallocated. Before drawing all 
             visible curves, the function o_YScaleCurve() must be called.
  RETURNS  : void
  NOTES    :

  UPDATE   : 18.09.97
******************************************************************************/
static void o_YBackProc (void)
{
    int i,j;


    if(!o_PressPossible)
	return;

    o_YCounter = (--o_YCounter < 0) ? (0) : (o_YCounter);

    o_AbsoluteScale = o_YScaleValues[o_YCounter].ScreenEinheit /
	o_YScaleValues[o_YCounter].Einheit;

    o_YScaleCurve();
    XClearArea(o_display,o_window,0,0,(unsigned int) o_WindowWidth,
	       (unsigned int) o_WindowHeight,0);  
    o_ClearPixmap(o_display,o_Pixmap,o_gc[train_gc],fg.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight);
    o_ClearPixmap(o_display,o_Pixmap,o_gc[test_gc],fg2.pixel,o_screen,0,0,
		  o_PixmapWidth,o_PixmapHeight);

    for(i=0;i<=o_CurveNo;i+=2) {
	XDrawLines(o_display,o_Pixmap,o_gc[train_gc],o_Curve1[i],
		   o_CurveLengths[i],CoordModeOrigin);
	for(j=0;j<o_CurveLengths[i+1]-1;j++){
	    if(o_Curve1[i+1][j].y != -999 && o_Curve1[i+1][j+1].y != -999){
		XDrawLine(o_display,o_Pixmap,o_gc[test_gc],
			  o_Curve1[i+1][j].x,o_Curve1[i+1][j].y,
			  o_Curve1[i+1][j+1].x,o_Curve1[i+1][j+1].y);
	    }
	}
    }
    XCopyArea(o_display,o_Pixmap,o_window,o_gc[train_gc],0,0,
	      (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);
    XCopyArea(o_display,o_Pixmap,o_window,o_gc[test_gc],0,0,
	      (unsigned int) o_OsziWidth,(unsigned int) o_OsziHeight,
	      o_OsziXPos,o_OsziYPos);
    XDrawRectangle(o_display,o_window,o_gc[label_gc],
		   o_OsziFrameXPos,o_OsziFrameYPos,
		   (unsigned int) o_OsziWidth+1,(unsigned int) o_OsziHeight);
    o_ScaleY(o_display,o_window,o_gc[label_gc]); 
    o_ScaleX(o_display,o_window,o_gc[label_gc]);
}


/*****************************************************************************
  FUNCTION : o_draw 

  PURPOSE  : draws the current curve and updates the arrays Curve1 and Curve2.
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
void o_draw (struct ErrorValuesType net_error_values,
	     struct ErrorValuesType test_net_error_values, int drawTest)
{
    double PixelsOfOneLearningStep;
    int error, test_error;
    double net_error, test_net_error;
    int Length   = o_CurveLengths[o_CurveNo];
    int Length_1 = (Length-1==-1)?(0):(Length-1);
    int OldX     = o_Curve1[o_CurveNo][Length_1].x;
    int OldY     = o_Curve1[o_CurveNo][Length_1].y;
    int NewX     = (OldX+o_PixelsOfOneLearningStep);
    int NewY,NewTestY, save_state;
    int OldTestX = o_Curve1[o_CurveNo+1][Length_1].x;
    int OldTestY = o_Curve1[o_CurveNo+1][Length_1].y;
    int NewTestX = (OldTestX+o_PixelsOfOneLearningStep);

    /* compute error value to be plotted */
    switch (o_graph_error_scale){
    case GRAPH_SSE:
	net_error = net_error_values.SSE;
	test_net_error = test_net_error_values.SSE;
	break;
    case GRAPH_MSE:
        net_error = net_error_values.MSE;
	test_net_error = test_net_error_values.MSE;
	break;
    case GRAPH_SSE_DIV_OUT:
	net_error = net_error_values.SSE_div_Out;
	test_net_error = test_net_error_values.SSE_div_Out;
	break;
    }

    /* Do nothing if you are not allowed to! */
    if(!o_DrawAllowed)	return;


    o_LearnStepCount++;

    if(NewX >= o_OsziWidth){
	while((NewX >= o_OsziWidth) && (o_XCounter-- > 0)) {
	    PixelsOfOneLearningStep = 
		o_XScaleValues[o_XCounter].ScreenEinheit /
		o_XScaleValues[o_XCounter].Einheit;
	    NewX = o_Curve2[o_CurveNo][o_CurveLengths[o_CurveNo]-1].x *
		PixelsOfOneLearningStep-1;
	    NewTestX = o_Curve2[o_CurveNo+1][o_CurveLengths[o_CurveNo+1]-1].x *
		PixelsOfOneLearningStep-1; 
	} 
	if(NewX >= o_OsziWidth){
	    o_DrawAllowed = 0;
	}
	o_XCounter++; 
	save_state = o_PressPossible;
	o_PressPossible = 1;
	o_XBackProc();
	o_PressPossible = save_state;
	OldX = o_Curve1[o_CurveNo][Length_1].x;
	NewX = (OldX+o_PixelsOfOneLearningStep);
	OldTestX = o_Curve1[o_CurveNo+1][Length_1].x;
	NewTestX = (OldTestX+o_PixelsOfOneLearningStep);

    } 

    /* the new positon would be outside drawing frame; don't draw */
    if(!o_DrawAllowed) return;


    /* now draw */
    if((o_LearningStepsOfOnePixel == 0) ||
       !(o_LearnStepCount%o_LearningStepsOfOnePixel)){

	error      = (int)(o_MaxYDrawPos-(     net_error*o_AbsoluteScale));
	test_error = (int)(o_MaxYDrawPos-(test_net_error*o_AbsoluteScale)); 
	NewY       = (error<0)?(-1):(error);
	NewTestY   = (test_error<0)?(-1):(test_error);

	/* Save the new points */
	o_Curve1[o_CurveNo  ][Length].x = NewX;
	o_Curve1[o_CurveNo  ][Length].y = NewY;
	o_Curve1[o_CurveNo+1][Length].x = NewTestX;
	if(drawTest)
	    o_Curve1[o_CurveNo+1][Length].y = NewTestY;
	else
	    o_Curve1[o_CurveNo+1][Length].y = -999;

	if(Length == 0) {    /* first time */
	    XDrawPoint(o_display,o_Pixmap,o_gc[train_gc],NewX,NewY);
	    XDrawPoint(o_display,o_window,o_gc[train_gc],
		       NewX+o_SpaceLeft,NewY+o_SpaceAbove);

	    if(drawTest){
		XDrawPoint(o_display,o_Pixmap,o_gc[test_gc],NewTestX,NewTestY);
		XDrawPoint(o_display,o_window,o_gc[test_gc],
			   NewTestX+o_SpaceLeft,NewTestY+o_SpaceAbove);
	    }
	}else{   /* not first time */
	    XDrawLine(o_display,o_Pixmap,o_gc[train_gc],OldX,OldY,NewX,NewY);
	    XDrawLine(o_display,o_window,o_gc[train_gc],
		      OldX+o_SpaceLeft,OldY+o_SpaceAbove,
		      NewX+o_SpaceLeft,NewY+o_SpaceAbove);

	    if(drawTest && (OldTestY != -999)){
		XDrawLine(o_display,o_Pixmap,o_gc[test_gc],OldTestX,
			  OldTestY,NewTestX,NewTestY);
		XDrawLine(o_display,o_window,o_gc[test_gc],
			  OldTestX+o_SpaceLeft,OldTestY+o_SpaceAbove,
			  NewTestX+o_SpaceLeft,NewTestY+o_SpaceAbove);
	    }
	}

	o_Curve2[o_CurveNo][Length].x = o_LearnStepCount;
	o_Curve2[o_CurveNo][Length].y = (double)net_error;
	o_CurveLengths[o_CurveNo]++;

	o_Curve2[o_CurveNo+1][Length].x = o_LearnStepCount;
	o_Curve2[o_CurveNo+1][Length].y = (double)test_net_error;
	o_CurveLengths[o_CurveNo+1]++;
    } 
}


/*****************************************************************************
  FUNCTION : o_ClearPixmap

  PURPOSE  : clears the pixmap
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_ClearPixmap (Display *display, Drawable pixmap, GC gc, uint fg, 
			   int screen, int x, int y, int width, int height)
{
    XSetForeground (display,gc,WhitePixel(o_display, o_screen));
    XFillRectangle(display,pixmap,gc,x,y,(unsigned int) width,
		   (unsigned int) height);
    XSetForeground(display,gc,fg);
}



/*****************************************************************************
  FUNCTION : o_ScaleY

  PURPOSE  : draws the y-axis.
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_ScaleY (Display *my_display, Drawable my_window, GC my_gc)
{
    int buflen;
    char buf[10];
    int count=0;
    int y1,x1,x2,x3;
    int FontWriteYPos = (*o_fontStruct).ascent>>1;

    y1 = o_OsziFrameYPos + o_OsziHeight;
    x1 = o_OsziFrameXPos;
    x2 = x1 - 6;
    x3 = x1 + o_OsziFrameWidth; 
	
    if(oszi_drawgrid)
	XSetLineAttributes(my_display,my_gc,1,LineOnOffDash,CapButt,JoinMiter);
    
    while(y1 >= o_OsziFrameYPos) {
	if(oszi_drawgrid){
	    XDrawLine(my_display,my_window,my_gc,x2,y1,x3,y1);
	}else
	    XDrawLine(my_display,my_window,my_gc,x1,y1,x2,y1);   
	sprintf(buf,"%6.2f",(count*(o_YScaleValues[o_YCounter].Einheit)));
	buflen = strlen(buf);
	XDrawString(o_display,o_window,my_gc,
		    x2-XTextWidth(o_fontStruct,buf,buflen),
		    y1+FontWriteYPos,buf,buflen); 
	count++;
	y1 -= o_YScaleValues[o_YCounter].ScreenEinheit;
    }

    if(oszi_drawgrid)
	XSetLineAttributes(my_display, my_gc, 1, LineSolid, CapButt, JoinMiter);
 }


/*****************************************************************************
  FUNCTION : o_ScaleX

  PURPOSE  : draws the x-axis.
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.03.95
******************************************************************************/
static void o_ScaleX (Display *my_display, Drawable my_window, GC my_gc)
{
    int buflen;
    int count=0;
    int x1,y1,y2,y3;
    char buf[10];
    int o_OsziFrameXPos_AND_o_OsziFrameWidth = o_OsziFrameXPos+o_OsziFrameWidth;
    int FontWriteXPos = (*o_fontStruct).ascent+2;

    x1 = o_OsziFrameXPos;
    y1 = o_OsziFrameYPos+o_OsziHeight;
    y2 = y1 + 5;
    y3 = o_OsziFrameYPos;

    if(oszi_drawgrid)
	XSetLineAttributes(my_display,my_gc,1,LineOnOffDash,CapButt,JoinMiter);
     while(x1 <= o_OsziFrameXPos_AND_o_OsziFrameWidth) {
	if(oszi_drawgrid){
	    XDrawLine(my_display,my_window,my_gc,x1,y2,x1,y3);
	}else
	    XDrawLine(my_display,my_window,my_gc,x1,y1,x1,y2);
	sprintf(buf,"%d",(int)(count*o_XScaleValues[o_XCounter].Einheit));
	buflen = strlen(buf);
	XDrawString(o_display,o_window,my_gc,
		    x1-(XTextWidth(o_fontStruct,buf,buflen)>>1)+1,
		    y2+FontWriteXPos,buf,buflen);  
	x1 += o_XScaleValues[o_XCounter].ScreenEinheit;
	count++;
    }

    if(oszi_drawgrid)
	XSetLineAttributes(my_display,my_gc,1,LineSolid,CapButt,JoinMiter);
 
}

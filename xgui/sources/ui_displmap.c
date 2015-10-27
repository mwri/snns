/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_displmap.c,v $
  SHORTNAME      : ui_displmap
  SNNS VERSION   : 4.2

  PURPOSE        : realize al teh functions necessary to display the projection
                   of two units onto an other unit


  NOTES          :  Functions: 
             -- ui_displMap
                Purpose : Main program to display the MAPs of the network
	     -- ui_map_event:
	        Purpose : event handler for graphic window
             -- ui_displMapBW
                Purpose : Display the weights of the network in black and white
             -- ui_displMapCOL
                Purpose : Display the weights of the network in color
             -- ui_displMapFromUpdate
                Purpose : Interface program, used if net was updated and has 
             -- ui_map_close
                Purpose : notify xgui about the destruction of the window
	     -- ui_map_setup
	        Purpose : Setup panel for the grid width and the color scale 
	     -- ui_map_close_setup
	        Purpose : notify xgui about the destruction of the setup 
		          panel and read out the widget values
	     -- ui_map_scale
	        Purpose : Program to display the scale of colors /scale of 
		          square sizes
	     -- ui_map_zoom_in
	        Purpose : Zomms onto the weight squares
	     -- ui_map_zoom_out
	        Purpose : Zomms away from the weight squares
	     -- ui_map_geometry
	        Purpose : compute the geometry of the displays and the 
		          size of the squares
	     -- ui_map_grid
	        Purpose : draws a grid over the weight squares and the axis


  AUTHOR         : Christian Wehrfritz
  DATE           : 20.05.1994 

  CHANGED BY     : Guenter Mamier 
  RCS VERSION    : $Revision: 1.15 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:20 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG


******************************************************************************/
#include <config.h>
#include <stdlib.h>
#include <stdio.h>			/* For the Syntax message */

#include "ui.h"

#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/TextSrc.h>
#include <X11/Shell.h>
#include <X11/cursorfont.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Viewport.h>

#include "kr_ui.h"
#include "ui_xWidgets.h"
#include "ui_confirmer.h"
#include "ui_funcdispl.h"

#include "ui_displmap.ph"
#include "ui_main.h"
#include "ui_color.h"
#include "ui_mainP.h"
#include "ui_setup.h"
#include "ui_key.h"



/*****************************************************************************
  FUNCTION : ui_displMap

  PURPOSE  : Main program to display the MAPs of the network
  NOTES    : Initializes widgets and switches for color or b&w terminal
  RETURNS  : 
  UPDATE   : 22.09.94
******************************************************************************/
void ui_displMap(Widget w, caddr_t call_data)
{
    Widget donebutton,setbutton,framewidget,zoomin,zoomout;
    int maxstepX,maxstepY;
    Arg		   args[25];   
    Cardinal	   n = 0;
    char           buf[40];
  
    /* do nothing, if no network is defined */
    if( (krui_getNoOfUnits()) == 0){
	ui_confirmOk("No network loaded !");
	return;
    }
  

    /* If a MAP window already exists, just put it on top */

    if(MAP_WINDOW_created == 1){
	XRaiseWindow (XtDisplay (MAP_GRAPH_mainwidget), 
		      XtWindow (MAP_GRAPH_mainwidget));
	return;
    }
  

    /* Create all necessary widgets */

    n = 0;
    sprintf(buf," SNNS Unit Projection\n");
    MAP_GRAPH_mainwidget = 
	XtCreatePopupShell(buf,topLevelShellWidgetClass,ui_toplevel,args,n);
    n = 0;
    framewidget = XtCreateManagedWidget("form",formWidgetClass,
					MAP_GRAPH_mainwidget,args,n);
    donebutton  = ui_xCreateButtonItem("done", framewidget, NULL, NULL);

    zoomin = ui_xCreateButtonItem("zoomin",framewidget,NULL,donebutton);
    setbutton = ui_xCreateButtonItem("setup",framewidget,zoomin,NULL);
    zoomout = ui_xCreateButtonItem("zoomout",framewidget,zoomin,donebutton);
    n = 0;
    XtSetArg(args[n], XtNx, 0);n++;
    XtSetArg(args[n], XtNy, 0);n++;
    XtSetArg(args[n], XtNwidth, 210); n++;
    XtSetArg(args[n], XtNheight, 25); n++;
    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
    XtSetArg(args[n], XtNborderWidth,  0); n++;
    XtSetArg(args[n], XtNhorizDistance, 30);      n++;
    XtSetArg(args[n], XtNfromHoriz, setbutton);      n++;
    XtSetArg(args[n], XtNfromVert, NULL); n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft);  n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop);   n++;
    XtSetArg(args[n], XtNbottom, XtChainTop);n++;
    scale = XtCreateManagedWidget("Label",formWidgetClass,framewidget,args,n);
  
    n = 0;
    XtSetArg(args[n], XtNx, 0);n++;
    XtSetArg(args[n], XtNy, 0);n++;
    XtSetArg(args[n], XtNborderWidth,  1); n++;
    XtSetArg(args[n], XtNwidth,  MAP_windowsizeX+62); n++;
    XtSetArg(args[n], XtNheight, MAP_windowsizeY+62); n++;
    XtSetArg(args[n], XtNallowHoriz, TRUE);n++;
    XtSetArg(args[n], XtNallowVert, TRUE);n++;
    XtSetArg(args[n], XtNforceBars, TRUE);n++;
    XtSetArg(args[n], XtNfromHoriz, NULL);      n++;
    XtSetArg(args[n], XtNfromVert, zoomin); n++;
    XtSetArg(args[n], XtNvertDistance, 10); n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft);  n++;
    XtSetArg(args[n], XtNright , XtChainRight); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop);   n++;
    XtSetArg(args[n], XtNbottom, XtChainBottom);n++;
    view = XtCreateManagedWidget("view",viewportWidgetClass,framewidget,args,n);
    MAP_GRAPH_displaywidget =
	ui_createMyScreen("screen",view,MAP_windowsizeX,MAP_windowsizeY,
			  NULL,scale);
    MAP_WINDOW_created = 1;
  
  
    /* define the routines to be called for widget events */

    XtAddCallback(donebutton, XtNcallback,
		  (XtCallbackProc)ui_map_close,MAP_GRAPH_mainwidget);
    XtAddCallback(setbutton, XtNcallback,(XtCallbackProc)ui_map_setup,NULL);
    XtAddCallback(zoomin, XtNcallback,(XtCallbackProc)ui_map_zoom_in,NULL);
    XtAddCallback(zoomout, XtNcallback,(XtCallbackProc)ui_map_zoom_out,NULL);
    XtAddEventHandler(MAP_GRAPH_displaywidget, 
		      StructureNotifyMask | ExposureMask, GraphicsExpose,
		      (XtEventHandler) ui_map_event,ui_display);
    XtAddEventHandler(framewidget,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
  

    /* set the maximum size of each square and the size of the whole window */
    maxstepX = (int)((float)(MAP_xMax - MAP_xMin)/ MAP_scaleX);
    maxstepY = (int)((float)(MAP_yMax - MAP_yMin)/ MAP_scaleY);

    if(ui_col_monochromeMode){
	/* B&W terminal => variable square size needed */
	MAP_squaresizeX = (maxstepX > MAP_viewsizeX/MAP_BW_squaresizeX)? 
                           MAP_BW_squaresizeX: 
  	                   (int)((float)MAP_viewsizeX/(float)maxstepX);
	MAP_windowsizeX = MAP_squaresizeX*maxstepX+40;
	MAP_squaresizeY = (maxstepY > MAP_viewsizeY/MAP_BW_squaresizeY)? 
                           MAP_BW_squaresizeY: 
  	                   (int)((float)MAP_viewsizeY/(float)maxstepY);
	MAP_windowsizeX = MAP_squaresizeX*maxstepX+40;
    }else{
	/* color terminal => fixed square size */
	MAP_squaresizeX = 
	    ( maxstepX > MAP_viewsizeX/5)?5:(int)(MAP_viewsizeX/maxstepX);
	MAP_squaresizeY = 
	    ( maxstepY > MAP_viewsizeY/5)?5:(int)(MAP_viewsizeY/maxstepY);
	MAP_windowsizeX = MAP_squaresizeX*maxstepX+40;
	MAP_windowsizeY = MAP_squaresizeY*maxstepY+40;
    }
  
    /* Realize the window */
    XtPopup(MAP_GRAPH_mainwidget, XtGrabNone);
  
    MAP_GRAPH_screen = DefaultScreen(ui_display);
    MAP_GRAPH_win    = XtWindow(MAP_GRAPH_displaywidget);
}


/*****************************************************************************
  FUNCTION : ui_map_event

  PURPOSE  : event handler for graphic window
  NOTES    : 
  RETURNS  :
  UPDATE   : 22.09.94
*****************************************************************************/
static void ui_map_event(Widget w, Display *display, XEvent *event)
{
    int zooming    = 0;
    int col_screen = 1;
    int bw_screen  = 0;
  
    switch (event->type){
        case Expose:
	       if((event->xexpose.count == 0) && (w = MAP_GRAPH_displaywidget))
      
		   if(ui_col_monochromeMode){
		       /* this is a  B&W terminals, so use ui_displMAPBW */
		       ui_map_geometry(zooming);
		       ui_displMapBW();
		       ui_map_grid(bw_screen);
		   }else{
		       ui_map_geometry(zooming);
		       ui_displMapCOL();
		       ui_map_grid(col_screen);
		   }
	       break;
	  default:
	       break;
    }
}



/*****************************************************************************
  FUNCTION : ui_dispMapBW

  PURPOSE  : Program to display the MAPs of the network in black and white
  NOTES    : 
  RETURNS  : 
  UPDATE   : 22.09.94
******************************************************************************/
static void ui_displMapBW(void)
{
    FlintType strength, this_x, this_y;
    int xx,yy,xc=0,yc=0;
    uint dx,dy;
    FlintType save_act1,save_act2;
  
    /* save unit activations */
    save_act1 = krui_getUnitActivation(MAP_xUnit);
    save_act2 = krui_getUnitActivation(MAP_yUnit);

   /* clear the window */
    XSetForeground(ui_display, MAP_GRAPH_gc[0], 
		   BlackPixel(ui_display,MAP_GRAPH_screen));
    XSetBackground(ui_display, MAP_GRAPH_gc[0], 
		   WhitePixel(ui_display,MAP_GRAPH_screen));
    XClearWindow(ui_display,MAP_GRAPH_win);
  
  
    /* draw the MAP squares */
    for (this_y=MAP_yMin; this_y<MAP_yMax; this_y+=MAP_scaleY) {
	for (this_x=MAP_xMin; this_x<MAP_xMax; this_x+=MAP_scaleX) {
	    krui_setUnitActivation(MAP_xUnit, this_x);
	    krui_setUnitActivation(MAP_yUnit, this_y);
	    krui_updateNet(NULL,(int)NULL);
	    strength = krui_getUnitOutput(MAP_outputUnit);
	    dx = dy = 
		abs(18*(strength-MAP_minWght)/(MAP_maxWght-MAP_minWght)-9);
	    xx = (int)(xc*MAP_BW_squaresizeX)+20;
	    yy = (int)(yc*MAP_BW_squaresizeY)+20;
	    if(strength >(MAP_maxWght-MAP_minWght)/2 ){
		XFillRectangle(ui_display,MAP_GRAPH_win,
			       MAP_GRAPH_gc[0],xx,yy,dx,dy);
	    }else{
		XDrawRectangle(ui_display,MAP_GRAPH_win,
			       MAP_GRAPH_gc[0],xx,yy,dx,dy);
	    }
	    xc++;
	}
	yc++;xc=0;
    }
  
    /* restore unit activations */
    krui_setUnitActivation(MAP_xUnit,save_act1);
    krui_setUnitActivation(MAP_yUnit,save_act2);

    /* draw the scale */
    ui_map_scale(scale); 
} 


/*****************************************************************************
  FUNCTION : ui_displMapCOL
  
  PURPOSE  : Program to display the MAP of the network in color
  NOTES    :
  RETURNS  : 
  UPDATE   : 22.09.94
******************************************************************************/
static void ui_displMapCOL(void)
{
    FlintType strength, this_x, this_y;
    int xx,yy,xc=0,yc=0;
    uint dx,dy;
    FlintType save_act1,save_act2;
  
    /* save unit activations */
    save_act1 = krui_getUnitActivation(MAP_xUnit);
    save_act2 = krui_getUnitActivation(MAP_yUnit);

    /* clear the window */
    XSetForeground(ui_display, MAP_GRAPH_gc[20], 
		   BlackPixel(ui_display,MAP_GRAPH_screen));
    XSetBackground(ui_display, MAP_GRAPH_gc[20], 
		   WhitePixel(ui_display,MAP_GRAPH_screen));
    XClearWindow(ui_display,MAP_GRAPH_win);

    /* draw the MAP squares */
    dx = MAP_squaresizeX;
    dy = MAP_squaresizeY;
    for (this_y=MAP_yMin; this_y<MAP_yMax; this_y+=MAP_scaleY) {
	for (this_x=MAP_xMin; this_x<MAP_xMax; this_x+=MAP_scaleX) {
	    krui_setUnitActivation(MAP_xUnit, this_x);
	    krui_setUnitActivation(MAP_yUnit, this_y);
	    krui_updateNet(NULL,(int)NULL);
	    strength = krui_getUnitOutput(MAP_outputUnit);
	    xx = (int)(xc*MAP_squaresizeX - 0.5*MAP_squaresizeX)+20;
	    yy = (int)(yc*MAP_squaresizeY - 0.5*MAP_squaresizeY)+20;

	    strength = 18*(strength-MAP_minWght)/(MAP_maxWght-MAP_minWght)-9;
	    strength = (strength > 9)?  9: strength;
	    strength = (strength <-9)? -9: strength;
	    XFillRectangle(ui_display,MAP_GRAPH_win,
			   MAP_GRAPH_gc[(int)(10.0 + (float)strength)],
			   xx,yy,dx,dy);
	    xc++;
	}
	yc++;xc=0;
    }
  
    /* restore unit activations */
    krui_setUnitActivation(MAP_xUnit,save_act1);
    krui_setUnitActivation(MAP_yUnit,save_act2);

    /* draw the scale */
    ui_map_scale(scale);
}

/*****************************************************************************
  FUNCTION : ui_displMapFromUpdate

  PURPOSE  : Interface program, used if net was updated and has to be redrawn.
  NOTES    :
  RETURNS  : 
  UPDATE   : 22.09.94
******************************************************************************/
void ui_displMapFromUpdate(void)
{
  
  if( !MAP_WINDOW_created )return;

  if(ui_col_monochromeMode){
      ui_displMapBW();
  }else{
      ui_displMapCOL();
      ui_map_grid(1);
  }
}

/*****************************************************************************
  FUNCTION : ui_map_zoom_in

  PURPOSE  : Zomms onto the weight squares
  NOTES    :
  RETURNS  : 
  UPDATE   : 20.05.1994
******************************************************************************/
static void ui_map_zoom_in(Widget w)
{
    /* delete the old screen display */
    XClearWindow(ui_display,MAP_GRAPH_win);

    /* compute new display geometry */
    ui_map_geometry(1);

    /* display the new setting */
    if(ui_col_monochromeMode){
	ui_displMapBW();
	ui_map_grid(0);
    }else{
	ui_displMapCOL();
	ui_map_grid(1);
    }
}

/*****************************************************************************
  FUNCTION : ui_map_zoom_out

  PURPOSE  : Zomms away from the weight squares
  NOTES    :
  RETURNS  : 
  UPDATE   : 20.05.1994
******************************************************************************/
static void ui_map_zoom_out(Widget w)
{
    /* delete the old screen display */
    XClearWindow(ui_display,MAP_GRAPH_win);

    /* compute new display geometry */
    ui_map_geometry(-1);

    /* display the new setting */
    if(ui_col_monochromeMode){
	ui_displMapBW();
	ui_map_grid(0);
    }else{
	ui_displMapCOL();
	ui_map_grid(1);
    }
}


/*****************************************************************************
  FUNCTION : ui_map_close

  PURPOSE  : notify xgui about the destruction of the window
  NOTES    :
  RETURNS  : 
  UPDATE   : 20.05.94
******************************************************************************/
static void ui_map_close(Widget dummy, Widget w, caddr_t call_data)
{
  XtDestroyWidget(w);
  MAP_WINDOW_created = 0;
  MAP_gridWidth = 0;
}

/*****************************************************************************
  FUNCTION : ui_map_setup

  PURPOSE  : Setup panel for the grid width and the color scale 
  NOTES    :
  RETURNS  : 
  UPDATE   : 22.09.94
******************************************************************************/
static void ui_map_setup(Widget button, caddr_t call_data)
{
    Widget      grid_lab,MAP_GRAPH_setupwidget, x_lab,y_lab;
    Widget      ui_setupPanel,done,subframe,d1,d2,d3,range_lab;
    Widget      y_range_lab,x_range_lab,out_lab;
    char        buf[80];
    Position    x, y;
    Dimension   width, height;
    Arg	        args[25];   
    Cardinal    n = 0;
    int         titelWidth  = 32;
    int         floatWidth  = 48;
  
  
    /* set Popup arguments */
    n = 0;
    XtSetArg(args[0], XtNwidth, &width);n++;
    XtSetArg(args[1], XtNheight, &height);n++;
    XtGetValues(button, args, n);
    XtTranslateCoords(button, (Position)(width/2), 
		      (Position)(height/2), &x, &y);
    n = 0;
    XtSetArg(args[n], XtNx, x);n++;
    XtSetArg(args[n], XtNy, y+50);n++;
  
    /* Now create Popup */
    MAP_GRAPH_setupwidget = 
	XtCreatePopupShell("Setup", transientShellWidgetClass, 
			   ui_toplevel, args, n); 
  
    ui_setupPanel =
	XtCreateManagedWidget("form", formWidgetClass, 
			      MAP_GRAPH_setupwidget, NULL, ZERO);
    done = ui_xCreateButtonItem("done", ui_setupPanel, NULL, NULL);
    XtAddCallback (done, XtNcallback, (XtCallbackProc)ui_map_close_setup,
		   MAP_GRAPH_setupwidget);
    n = 0;
    XtSetArg(args[n], XtNx, 0);n++;
    XtSetArg(args[n], XtNy, 0);n++;
    XtSetArg(args[n], XtNborderWidth,  1); n++;
    XtSetArg(args[n], XtNfromHoriz, NULL);      n++;
    XtSetArg(args[n], XtNfromVert, done); n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft);  n++;
    XtSetArg(args[n], XtNright , XtChainRight); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop);   n++;
    XtSetArg(args[n], XtNbottom, XtChainBottom);n++;
    subframe = XtCreateManagedWidget("sub",formWidgetClass,
				     ui_setupPanel,args,n);

    /* X unit */

    grid_lab = ui_xCreateLabelItem("X-axis :", subframe,104, NULL, NULL);
    x_lab = ui_xCreateLabelItem("unit ",subframe,56,NULL,grid_lab);
    sprintf(buf,"%d", MAP_xUnit);
    x_dia = ui_xCreateDialogItem("xUnit",subframe,buf,
				 titelWidth,x_lab,grid_lab);
    x_range_lab = ui_xCreateLabelItem("  value range :", subframe, 
				      120, x_dia, grid_lab);
    sprintf(buf,"%g",MAP_xMin);
    x_min = ui_xCreateDialogItem("xMin", subframe, buf, 
				 floatWidth,x_range_lab,grid_lab);
    d1 = ui_xCreateLabelItem("to",subframe,24,x_min,grid_lab);
  
    sprintf(buf,"%g",MAP_xMax);
    x_max = ui_xCreateDialogItem("xMax", subframe, buf, 
				 floatWidth, d1,grid_lab);

    /* Y unit */

    grid_lab = ui_xCreateLabelItem("Y-axis :", subframe,104, NULL, x_max);

    y_lab = ui_xCreateLabelItem("unit ",subframe,56,NULL,grid_lab);
    sprintf(buf,"%d", MAP_yUnit);
    y_dia = ui_xCreateDialogItem("yUnit",subframe,buf,
				 titelWidth,y_lab,grid_lab);
    y_range_lab = ui_xCreateLabelItem("  value range :", subframe, 
				      120, y_dia, grid_lab);
    sprintf(buf,"%g",MAP_yMin);
    y_min = ui_xCreateDialogItem("yMin", subframe, buf, 
				 floatWidth,y_range_lab,grid_lab);
    d2 = ui_xCreateLabelItem("to",subframe,24,y_min,grid_lab);
  
    sprintf(buf,"%g",MAP_yMax);
    y_max = ui_xCreateDialogItem("yMax", subframe, buf, 
				 floatWidth, d2, grid_lab);

    /* output unit */

    grid_lab = ui_xCreateLabelItem("activation pattern", subframe, 160, 
				   NULL, y_lab);
    out_lab = ui_xCreateLabelItem("unit ", subframe, 56, NULL, grid_lab);
    sprintf(buf,"%d", MAP_outputUnit);
    out_dia = ui_xCreateDialogItem("outUnit",subframe,buf,
				   titelWidth,out_lab,grid_lab);
    range_lab = ui_xCreateLabelItem(" output range :", subframe, 
				    120, out_dia, grid_lab);
    sprintf(buf,"%g",MAP_minWght);
    out_min = ui_xCreateDialogItem("outMin", subframe, buf, 
				   floatWidth,range_lab,grid_lab);
    d3 = ui_xCreateLabelItem("to",subframe,24,out_min,grid_lab);
  
    sprintf(buf,"%g",MAP_maxWght);
    out_max = ui_xCreateDialogItem("outMax", subframe, buf, 
				   floatWidth, d3,grid_lab);
  
    n = 0;
    XtSetArg(args[n], XtNx, 0);n++;
    XtSetArg(args[n], XtNy, 0);n++;
    XtSetArg(args[n], XtNwidth, 210); n++;
    XtSetArg(args[n], XtNheight, 25); n++;
    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
    XtSetArg(args[n], XtNborderWidth,  0); n++;
    XtSetArg(args[n], XtNfromHoriz,range_lab);      n++;
    XtSetArg(args[n], XtNfromVert, range_lab); n++;
    XtSetArg(args[n], XtNleft  , XtChainRight);  n++;
    XtSetArg(args[n], XtNright , XtChainRight); n++;
    XtSetArg(args[n], XtNtop   , XtChainBottom);   n++;
    XtSetArg(args[n], XtNbottom, XtChainBottom);n++;
    scale2 = XtCreateManagedWidget("Label2",formWidgetClass,subframe,args,n);
  
    XtRealizeWidget(MAP_GRAPH_setupwidget);
    XtAddEventHandler(scale2, ExposureMask, GraphicsExpose,
		      (XtEventHandler) ui_map_scale,NULL);
  

    /* Realize the window */
    XtPopup(MAP_GRAPH_setupwidget, XtGrabExclusive);
}              


/*****************************************************************************
  FUNCTION : ui_map_close_setup

  PURPOSE  : notify xgui about the destruction of the setup panel and read 
             out the widget values
  NOTES    :
  RETURNS  : 
  UPDATE   : 22.09.94
******************************************************************************/
static void ui_map_close_setup(Widget dummy, Widget w, caddr_t call_data)
{

    /* read the setup values */
    MAP_xUnit      = (int)ui_xIntFromAsciiWidget(x_dia);
    MAP_yUnit      = (int)ui_xIntFromAsciiWidget(y_dia);
    MAP_outputUnit = (int)ui_xIntFromAsciiWidget(out_dia);
    MAP_xMin       = (float)ui_xFloatFromAsciiWidget(x_min);
    MAP_xMax       = (float)ui_xFloatFromAsciiWidget(x_max);
    MAP_yMin       = (float)ui_xFloatFromAsciiWidget(y_min);
    MAP_yMax       = (float)ui_xFloatFromAsciiWidget(y_max);
    MAP_maxWght    = (float)ui_xFloatFromAsciiWidget(out_max);
    MAP_minWght    = (float)ui_xFloatFromAsciiWidget(out_min);


    /* update display according to the new values */
    if(ui_col_colorDisplay){
	MAP_scaleX = ((MAP_xMax-MAP_xMin) / MAP_viewsizeX) * MAP_squaresizeX;
	MAP_scaleY = ((MAP_yMax-MAP_yMin) / MAP_viewsizeY) * MAP_squaresizeY;
	ui_displMapCOL();
	ui_map_scale(scale);
	ui_map_grid(1);
    }else{
	MAP_scaleX = ((MAP_xMax-MAP_xMin) / MAP_viewsizeX) * MAP_BW_squaresizeX;
	MAP_scaleY = ((MAP_yMax-MAP_yMin) / MAP_viewsizeY) * MAP_BW_squaresizeY;
	ui_displMapBW();
	ui_map_scale(scale);
	ui_map_grid(0);
    }

    XtDestroyWidget(w);
}


/*****************************************************************************
  FUNCTION : ui_map_scale

  PURPOSE  : Program to display the scale of colors /scale of square sizes
  NOTES    :
  RETURNS  : 
  UPDATE   : 20.05.94
******************************************************************************/
static void ui_map_scale(Widget w)
{
    Window scale_win;
    int n;
    uint size = 10;
    char buf[80];
  
    scale_win = XtWindow(w);
    XClearWindow(ui_display,scale_win);
  
    if(ui_col_monochromeMode){
	/* on B/W screens display a row of different sized squares */
	sprintf(buf,"%-g",MAP_minWght);
	XDrawString(ui_display,scale_win,MAP_GRAPH_gc[0],0,10,buf,
		    strlen(buf));
	for(n=1; n<=10; n++){
	    size = (uint)(10 - n);
	    XDrawRectangle(ui_display,scale_win,MAP_GRAPH_gc[0],
			   10*(n-1),14+(int)(n/2),size,size);
	}
	for(n=11; n<=19; n++){
	    size = (uint)(n - 10);
	    XFillRectangle(ui_display,scale_win,MAP_GRAPH_gc[0],
			   10*(n-1),14+(int)((20-n)/2),size,size);       
	}
	sprintf(buf,"%g",MAP_maxWght);
	XDrawString(ui_display,scale_win,MAP_GRAPH_gc[0],190-strlen(buf)*7,
		    10,buf,strlen(buf));
    }else{ /* on color screens display the color range */
	sprintf(buf,"%-g",MAP_minWght);
	XDrawString(ui_display,scale_win,MAP_GRAPH_gc[20],0,10,buf,
		    strlen(buf));
	for(n=1; n<=19; n++){
	    XFillRectangle(ui_display,scale_win,MAP_GRAPH_gc[n],
			   10*(n-1),12,size,size);
	}
	sprintf(buf,"%g",MAP_maxWght);
	XDrawString(ui_display,scale_win,MAP_GRAPH_gc[20],190-strlen(buf)*7,
		    10,buf,strlen(buf));
    }
}


/*****************************************************************************
  FUNCTION : ui_map_geometry

  PURPOSE  : compute the geometry of the displays and the size of the squares
  NOTES    :
  RETURNS  : 
  UPDATE   : 22.09.1994
******************************************************************************/
static void ui_map_geometry(int zoom_fact)
{
    int  maxstepsX,maxstepsY;
    int  n;
    XtWidgetGeometry geo_info, geo_data;
    XtWidgetGeometry disp_info, disp_data;
    XtWidgetGeometry view_info, view_data;
    XtGeometryResult geo_result;


    /* do nothing, if no network defined */

    if( krui_getNoOfUnits() == 0)
	return;
    maxstepsX = (int) (MAP_xMax-MAP_xMin)/MAP_scaleX;
    maxstepsY = (int) (MAP_yMax-MAP_yMin)/MAP_scaleY;


    /* set the necessary xlib environment variables */

    if(ui_col_monochromeMode){ 
	/* this is a  B&W terminal */
	MAP_GRAPH_gc[0]  = XCreateGC(ui_display,MAP_GRAPH_win,0,0); 
    } else { 
	/* this is a color terminal */
	for(n=0;n<=20;n++)
	    MAP_GRAPH_gc[n] = XCreateGC(ui_display,MAP_GRAPH_win,0,0); 
    
	/* set the foreground and background colors */
	for(n=0; n<10;n++){
	    XSetForeground(ui_display,MAP_GRAPH_gc[n],
			   (uint) ui_col_rangePixels[(int)(n*1.7)]); 
	    XSetBackground(ui_display,MAP_GRAPH_gc[n], 
			   WhitePixel(ui_display,MAP_GRAPH_screen));
	    XSetForeground(ui_display,MAP_GRAPH_gc[10+n],
			   (uint)  ui_col_rangePixels[(int)(15+n*1.7)]); 
	    XSetBackground(ui_display,MAP_GRAPH_gc[10+n],
			   WhitePixel(ui_display,MAP_GRAPH_screen));
	}
    }


    /* get widget geometry */

    geo_result = XtQueryGeometry(MAP_GRAPH_mainwidget,&geo_data,&geo_info);
  
    if(geo_result != XtGeometryYes){
	geo_info.height = 400;
	geo_info.width  = 400;
    }
    if(zoom_fact != 0){
	XtResizeWidget(MAP_GRAPH_mainwidget,geo_info.width,
		       geo_info.height,geo_info.border_width);
    }


    /* adjust squaresize */

    if(ui_col_monochromeMode){	/* this is a  B&W terminal */
	MAP_BW_squaresizeX = MAP_BW_squaresizeX + zoom_fact;
	MAP_BW_squaresizeY = MAP_BW_squaresizeY + zoom_fact;
	if(MAP_BW_squaresizeX == 0)MAP_BW_squaresizeX = 1;
	if(MAP_BW_squaresizeY == 0)MAP_BW_squaresizeY = 1;
	MAP_windowsizeX = MAP_BW_squaresizeX*maxstepsX+40;
	MAP_windowsizeY = MAP_BW_squaresizeY*maxstepsY+40;
    }else{			/* this is a color terminal */
	MAP_squaresizeX = MAP_squaresizeX + zoom_fact;
	MAP_squaresizeY = MAP_squaresizeY + zoom_fact;
	if(MAP_squaresizeX == 0)MAP_squaresizeX = 1;
	if(MAP_squaresizeY == 0)MAP_squaresizeY = 1;
	MAP_windowsizeX = MAP_squaresizeX*maxstepsX+62;
	MAP_windowsizeY = MAP_squaresizeY*maxstepsY+62;
    }
  

    /* Resize all involved windows so that they fit the data and the screen */

    XtResizeWidget(MAP_GRAPH_displaywidget,MAP_windowsizeX+62,
		   MAP_windowsizeY+62,0);
    MAP_viewsizeX = MAP_squaresizeX * maxstepsX;
    MAP_viewsizeY = MAP_squaresizeY * maxstepsY;
    geo_result = XtQueryGeometry(MAP_GRAPH_displaywidget,
				 &disp_data,&disp_info);
    geo_result = XtQueryGeometry(view,&view_data,&view_info);
    if(view_info.width > disp_info.width)
	XtResizeWidget(view,disp_info.width,
		       disp_info.height,view_info.border_width);
    else
	if(disp_info.width < geo_info.width)
	    XtResizeWidget(view,disp_info.width,
			   disp_info.height,view_info.border_width);
	else
	    XtResizeWidget(view,geo_info.width,
			   geo_info.height,view_info.border_width);

}


/*****************************************************************************
  FUNCTION : ui_map_grid

  PURPOSE  : draws a grid over the MAP squares and the axis
  NOTES    : type == 1 for color display; == 0 for B/W
  RETURNS  : 
  UPDATE   : 20.05.1994
******************************************************************************/
static void 
ui_map_grid(int type)
{
    char x_String[40], y_String[40];
    int  x_steps,y_steps;
    int  n = 0;
    float x_no,y_no,x_foo=0,y_foo=0;
    int actual_sizeX,actual_sizeY;
    XPoint p1,p2,p3,p4;
    static char dash[]={6,6,6};

    /* do nothing, if no network defined */
    if(type == 0){
	if( krui_getNoOfUnits() == 0 || 
	   (MAP_BW_squaresizeX == 0) || (MAP_BW_squaresizeY == 0) )return;
    }else{ 
	if( krui_getNoOfUnits() == 0 || 
	   (MAP_squaresizeX == 0) || (MAP_squaresizeY == 0) )return;
    }

    /* set the square size */
    if(type == 0){
	actual_sizeX = MAP_BW_squaresizeX;
	actual_sizeY = MAP_BW_squaresizeY;
    }else{ 
	actual_sizeX = MAP_squaresizeX;
	actual_sizeY = MAP_squaresizeY;
    }
  
    /* set output grid of units */
    dash[0] = dash[1] = (int)(actual_sizeX*0.5 +0.5);
    XSetDashes(ui_display,MAP_GRAPH_gc[20*type],
	       (int)(actual_sizeX*0.5 +0.5)+20*type,dash,2);
    XSetLineAttributes(ui_display,MAP_GRAPH_gc[20*type],0,LineOnOffDash,
		       CapButt,JoinMiter);

    x_steps = (int)((MAP_xMax - MAP_xMin)/MAP_scaleX);
    y_steps = (int)((MAP_yMax - MAP_yMin)/MAP_scaleY);
    x_no = MAP_xMin;
    y_no = MAP_yMin;

    while(n <= 10){
	sprintf(x_String,"%1.1f",x_no);
	sprintf(y_String,"%1.1f",y_no);
	p1.x = (int)(x_foo)+20;
	p1.y = 10;
	p2.x = (x_steps)*actual_sizeX+30;
	p2.y = (int)(y_foo)+20;
	p3.x = 0; 
	p3.y = p2.y;
	p4.x = p1.x;
	p4.y = (y_steps)*actual_sizeY+20;

	XDrawString(ui_display,MAP_GRAPH_win,MAP_GRAPH_gc[20*type],
		    p1.x,p1.y,x_String,(int)strlen(x_String));
	XDrawString(ui_display,MAP_GRAPH_win,MAP_GRAPH_gc[20*type],
		    p2.x,p2.y,y_String,(int)strlen(y_String));
	XDrawString(ui_display,MAP_GRAPH_win,MAP_GRAPH_gc[20*type],
		    p3.x,p3.y,y_String,(int)strlen(y_String));
	XDrawString(ui_display,MAP_GRAPH_win,MAP_GRAPH_gc[20*type],
		    p4.x,p4.y+20,x_String,(int)strlen(x_String));
	XDrawLine(ui_display,MAP_GRAPH_win,MAP_GRAPH_gc[20*type],
		  p1.x,p1.y,p4.x,p4.y);
	XDrawLine(ui_display,MAP_GRAPH_win,MAP_GRAPH_gc[20*type],
		  p2.x,p2.y,p3.x+10,p3.y);
	n ++; 
	x_foo = (type==0)?x_foo+(float)(x_steps*MAP_BW_squaresizeX)/10.0:
	                  x_foo+(float)(x_steps*MAP_squaresizeX)/10.0;
	y_foo = (type==0)?y_foo+(float)(y_steps*MAP_BW_squaresizeY)/10.0:
	                  y_foo+(float)(y_steps*MAP_squaresizeY)/10.0;
	x_no += (MAP_xMax-MAP_xMin)/10.0;
	y_no += (MAP_yMax-MAP_yMin)/10.0;
    }
    XSetLineAttributes(ui_display,MAP_GRAPH_gc[20*type],0,
		       LineSolid,CapButt,JoinMiter);
}
/* end of file */
 

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_displwght.c,v $
  SHORTNAME      : ui_displwght
  SNNS VERSION   : 4.2

  PURPOSE        : contains all routines to display the weights as Hinton or
                   WV- diagrams


  NOTES          : 

  AUTHOR         : Guenter Mamier 
  DATE           : 05.08.1993 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.18 $
  LAST CHANGE    : $Date: 1998/04/22 16:08:35 $

    Copyright (c) 1990-1998  SNNS Group, IPVR, Univ. Stuttgart, Germany


******************************************************************************/

#include <math.h>

#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Viewport.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_xWidgets.h"
#include "ui_confirmer.h"
#include "ui_funcdispl.h"
#include "ui_main.h"
#include "ui_color.h"
#include "ui_mainP.h"
#include "ui_setup.h"
#include "ui_key.h"

#include "ui_displwght.ph"

/*****************************************************************************
  FUNCTION : ui_displWeights

  PURPOSE  : Main program to display the weights of the network
  NOTES    : Initializes widgets and switches for color or b&w terminal
  RETURNS  : 
  UPDATE   : 29.01.92
******************************************************************************/
void ui_displWeights(Widget w, caddr_t call_data)
{
    Widget donebutton,setbutton,framewidget,zoomin,zoomout;
    Arg		   args[25];   
    Cardinal	   n = 0;
    char           buf[40];


    /* do nothing, if no network is defined */

    if( (maxunits = krui_getNoOfUnits()) == 0){
      ui_confirmOk("No network loaded !");
      return;
    }


    /* If a weight window already exists, just put it on top */

    if(WEIGHT_WINDOW_created == 1){
       XRaiseWindow (XtDisplay (WEIGHT_GRAPH_mainwidget), 
		     XtWindow (WEIGHT_GRAPH_mainwidget));
       return;
    }


    /* Create all necessary widgets */

    dwght_screen = DefaultScreen(ui_display);
    dwght_depth = DefaultDepth(ui_display, dwght_screen);

    n = 0;
    sprintf(buf,"SNNS Weight Visualization\n");
    WEIGHT_GRAPH_mainwidget = 
           XtCreatePopupShell(buf,topLevelShellWidgetClass,ui_toplevel,args,n);
    n = 0;
    framewidget = XtCreateManagedWidget("form",formWidgetClass,
					WEIGHT_GRAPH_mainwidget,args,n);
    donebutton  = ui_xCreateButtonItem("done", framewidget, NULL, NULL);

    if(WEIGHT_gridWidth == 0)
	WEIGHT_gridWidth = ((int)(0.1*maxunits)<1)? 1: 0.1*maxunits;
    minWght = (float)-ui_maxWeight;
    maxWght = (float)ui_maxWeight;
    zoomin = ui_xCreateButtonItem("zoomin",framewidget,NULL,donebutton);
    setbutton = ui_xCreateButtonItem("setup",framewidget,zoomin,NULL);
    zoomout = ui_xCreateButtonItem("zoomout",framewidget,zoomin,donebutton);

    n = 0;
    XtSetArg(args[n], XtNx, 0); n++;
    XtSetArg(args[n], XtNy, 0); n++;
    XtSetArg(args[n], XtNwidth, 210); n++;
    XtSetArg(args[n], XtNheight, 25); n++;
    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
    XtSetArg(args[n], XtNborderWidth,  0); n++;
    XtSetArg(args[n], XtNhorizDistance, 30); n++;
    XtSetArg(args[n], XtNfromHoriz, setbutton); n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft);  n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop);   n++;
    XtSetArg(args[n], XtNbottom, XtChainTop);n++;
    scale = XtCreateManagedWidget("Label",formWidgetClass,framewidget,args,n);

    n = 0;
    XtSetArg(args[n], XtNresizable, TRUE);n++;
    XtSetArg(args[n], XtNfromVert, zoomin); n++;
    XtSetArg(args[n], XtNvertDistance, 10); n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft);  n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop);   n++;
    XtSetArg(args[n], XtNbottom, XtChainTop);n++;
    XtSetArg(args[n], XtNbackground, WhitePixel(ui_display,dwght_screen));n++;
    XtSetArg(args[n], XtNallowHoriz, TRUE);n++;
    XtSetArg(args[n], XtNallowVert, TRUE);n++;
    view = XtCreateManagedWidget("view",viewportWidgetClass,framewidget,args,n);
    WEIGHT_GRAPH_displaywidget =
	ui_createMyScreen("screen",view,WEIGHT_windowsize, WEIGHT_windowsize,
			  NULL,scale);
    WEIGHT_WINDOW_created = 1;

 
    /* define the routines to be called for widget events */

    XtAddCallback(donebutton, XtNcallback,
		  (XtCallbackProc)ui_wght_close,WEIGHT_GRAPH_mainwidget);
    XtAddCallback(setbutton, XtNcallback,(XtCallbackProc)ui_wght_setup,NULL);
    XtAddCallback(zoomin, XtNcallback,(XtCallbackProc)ui_wght_zoom_in,NULL);
    XtAddCallback(zoomout, XtNcallback,(XtCallbackProc)ui_wght_zoom_out,NULL);
    XtAddEventHandler(WEIGHT_GRAPH_mainwidget,
		      StructureNotifyMask | ExposureMask, GraphicsExpose,
		      (XtEventHandler) ui_wght_event,ui_display);
    XtAddEventHandler(WEIGHT_GRAPH_displaywidget,
		      ButtonPressMask|ButtonReleaseMask,FALSE,
		      (XtEventHandler) ui_wght_info,NULL);
    XtAddEventHandler(WEIGHT_GRAPH_displaywidget, 
		      StructureNotifyMask | ExposureMask, GraphicsExpose,
		      (XtEventHandler) ui_wght_event,ui_display);
    XtAddEventHandler(framewidget,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);

    /* set the maximum size of each square and the size of the whole window */

    if(ui_col_monochromeMode){
       /* B&W terminal => variable square size needed */
       WEIGHT_squaresize = (maxunits > WEIGHT_viewsize/HINTON_squaresize)? 
	                   HINTON_squaresize: 
	                   (int)((float)WEIGHT_viewsize/(float)maxunits);
       WEIGHT_windowsize = WEIGHT_squaresize*maxunits+40;
    }else{
       /* color terminal => fixed square size */
       WEIGHT_squaresize = 
              (maxunits > WEIGHT_viewsize/5)?5:(int)(WEIGHT_viewsize/maxunits);
       WEIGHT_windowsize = WEIGHT_squaresize*maxunits+40;
    }

    /* Realize the window */
    XtRealizeWidget(WEIGHT_GRAPH_mainwidget);

    /* set the necessary xlib environment */
    dwght_win    = XtWindow(WEIGHT_GRAPH_displaywidget);
    dwght_gc     = XCreateGC(ui_display,dwght_win,0,0); 
    XSetFont(ui_display, dwght_gc, ui_fontStruct->fid);
    dwght_pix = XCreatePixmap(ui_display,dwght_win,WEIGHT_windowsize,
			      WEIGHT_windowsize, dwght_depth);

    XtPopup(WEIGHT_GRAPH_mainwidget, XtGrabNone);

}




/*****************************************************************************
  FUNCTION : ui_wght_event

  PURPOSE  : event handler for graphic window
  NOTES    : 
  RETURNS  :
  UPDATE   : 20.02.92
*****************************************************************************/
static void ui_wght_event(Widget w, Display *display, XEvent *event)
{

    switch (event->type){
    case Expose:
	if (event->xexpose.count == 0){
	    if(ui_col_monochromeMode){
		ui_displWeightsBW();
	    }else{
		ui_displWeightsCOL();
	    }
	    ui_wght_scale(scale); 
	}
	break;
    case ConfigureNotify:
	if(w != WEIGHT_GRAPH_mainwidget)break;

	XtResizeWidget(w,event->xconfigure.width, event->xconfigure.height, 1);
	XtResizeWidget(view,event->xconfigure.width-20,
		       event->xconfigure.height-73, 1);
	break;
    default:
	break;
    }
}




/*****************************************************************************
  FUNCTION : ui_displWeightsBW

  PURPOSE  : Program to display the weights of the network in black and white
  NOTES    :
  RETURNS  : 
  UPDATE   : 04.08.93
******************************************************************************/
static void ui_displWeightsBW(void)
{
   int  n,next,pred;
   FlintType strength;
   float reference;
   int xx,yy;
   uint dx,dy;


   /* clear the pixmap */
   XSetForeground(ui_display, dwght_gc, WhitePixel(ui_display,dwght_screen));
   XFillRectangle(ui_display, dwght_pix, dwght_gc, 0, 0, WEIGHT_viewsize*2,
		  WEIGHT_viewsize*2);
   XSetForeground(ui_display, dwght_gc, BlackPixel(ui_display,dwght_screen));


   /* draw the weight squares */
   n = 1;
   next = krui_getFirstUnit();
   krui_setCurrentUnit(next);
   reference = (float)HINTON_squaresize / (float)((maxWght-minWght)/2); 
   do{
      if((pred=krui_getFirstPredUnit(&strength)) !=0){
         do{
	    strength = (strength > maxWght)? maxWght: strength;
	    strength = (strength < minWght)? minWght: strength;
	    dx = dy = abs( (int)(reference * (float)strength));
	    xx = (int)(pred*HINTON_squaresize-HINTON_squaresize*0.5-dx*0.5)+20;
	    yy = (int)(next*HINTON_squaresize-HINTON_squaresize*0.5-dy*0.5)+20;

	    if(strength > 0.0){
	       XFillRectangle(ui_display,dwght_pix,dwght_gc,xx,yy,dx,dy);
	    }else{
	       XDrawRectangle(ui_display,dwght_pix,dwght_gc,xx,yy,dx,dy);
	    }
	 }while((pred=krui_getNextPredUnit(&strength)) != 0);
      }
   }while((next=krui_getNextUnit()) != 0);

   /* draw the grid */
   ui_wght_grid(0);


   /* get everything on the screen */
   XCopyArea(ui_display, dwght_pix, dwght_win, dwght_gc, 0, 0,
	     WEIGHT_windowsize, WEIGHT_windowsize, 0, 0);
} 



/*****************************************************************************
  FUNCTION : ui_displWeightsCOL

  PURPOSE  : Program to display the weights of the network in color
  NOTES    :
  RETURNS  : 
  UPDATE   : 04.08.93
******************************************************************************/
static void ui_displWeightsCOL(void)
{
   int  next,pred;
   FlintType strength;
   int xx,yy;
   uint dx,dy;
   int  color;


   /* clear the window */
   XSetForeground(ui_display, dwght_gc, WhitePixel(ui_display,dwght_screen));
   XFillRectangle(ui_display, dwght_pix, dwght_gc, 0, 0, WEIGHT_windowsize*2,
		  WEIGHT_windowsize*2);
   XSetForeground(ui_display, dwght_gc, BlackPixel(ui_display,dwght_screen));
   XSetBackground(ui_display, dwght_gc, WhitePixel(ui_display,dwght_screen));


   /* draw the weight squares */

   dx = dy = WEIGHT_squaresize;
   next = krui_getFirstUnit();
   krui_setCurrentUnit(next);
   do{
      yy = (next-1)*WEIGHT_squaresize+20;
      if((pred=krui_getFirstPredUnit(&strength)) !=0){
         do{
	    xx = (pred-1)*WEIGHT_squaresize+20;
	    color = 30 * (strength-minWght)/(maxWght-minWght);
	    if(color > 30)color = 30;
	    if(color < 0)color = 0;
	    XSetForeground(ui_display,dwght_gc,(uint)ui_col_rangePixels[color]);
	    XFillRectangle(ui_display, dwght_pix, dwght_gc, xx, yy, dx, dy);
	 }while( (pred=krui_getNextPredUnit(&strength)) != 0);
      }
   }while((next=krui_getNextUnit()) != 0);

   /* draw the grid */
   ui_wght_grid(1);

   XCopyArea(ui_display, dwght_pix, dwght_win, dwght_gc, 0, 0,
	     WEIGHT_windowsize, WEIGHT_windowsize, 0, 0);

}




/*****************************************************************************
  FUNCTION : ui_displWeightsFromUpdate

  PURPOSE  : Interface program, used if net was updated and has to be redrawn.
  NOTES    :
  RETURNS  : 
  UPDATE   : 29.01.92
******************************************************************************/
void ui_displWeightsFromUpdate(void)
{

   if( !WEIGHT_WINDOW_created )return;


   if((maxunits != old_maxunits)  && old_maxunits){
      /* New network was loaded with different # of units *
       * ==> new viewport has to be created               */
      XtDestroyWidget(WEIGHT_GRAPH_mainwidget);
      old_maxunits = maxunits;
      WEIGHT_WINDOW_created = 0;
      WEIGHT_viewsize   = 400;
      HINTON_squaresize =  16;
      WEIGHT_squaresize =   0;
      ui_displWeights(WEIGHT_GRAPH_displaywidget,NULL);
   }else{

      /* old network was only updated, so redraw is sufficient */
      if(ui_col_monochromeMode){
         ui_displWeightsBW();
      }else{
         ui_displWeightsCOL();
     }

   }
}



/*****************************************************************************
  FUNCTION : ui_wght_close

  PURPOSE  : notify xgui about the destruction of the window
  NOTES    :
  RETURNS  : 
  UPDATE   : 29.01.92
******************************************************************************/
static void ui_wght_close(Widget dummy, Widget w, caddr_t call_data)
{
   XtDestroyWidget(w);
   XFreePixmap(ui_display,dwght_pix);
   WEIGHT_WINDOW_created = 0;
   WEIGHT_gridWidth = 0;
   old_maxunits = 0;
}



/*****************************************************************************
  FUNCTION : ui_wght_info

  PURPOSE  : Program to display information about the weights
  NOTES    :
  RETURNS  : 
  UPDATE   : 03.08.93
******************************************************************************/
static void ui_wght_info(Widget w, Display *display, XEvent *event)
{
    Widget ui_wei_popup;
    static Bool ui_wei_exit = TRUE;
    int xval,yval;
    int actual_size;
    FlintType con_weight;
    char buf[40];
    Widget label,form;
    uint n;

    Arg args[5];
    

   /* set the square size */
   if(ui_col_monochromeMode)
   	actual_size = HINTON_squaresize;
   else 
   	actual_size = WEIGHT_squaresize;


    switch(event->type){
        case ButtonPress: 

             /* get unit numbers of clicked at connection */
	     xval = (int)((float)(event->xbutton.x-20) /
			  (float)(actual_size))+1;
	     yval = (int)((float)(event->xbutton.y-20) /
			  (float)actual_size)+1;

	     /* do nothing for invalid numbers */
	     if(xval<0 || yval<0)return;
	     if(xval>krui_getNoOfUnits() || yval>krui_getNoOfUnits())return;

	     /* create  label and popup */
	     if(krui_areConnectedWeight(xval,yval,&con_weight))
	        sprintf(buf,"Link %d -> %d \nhas weight %f"
			,xval,yval,con_weight);
	     else
	        sprintf(buf,"No connection from unit %d to unit %d\n",
		       xval,yval);
	     ui_wei_popup = XtCreatePopupShell("info",overrideShellWidgetClass,
					       ui_toplevel,NULL, 0);
	     form = XtCreateManagedWidget("form",formWidgetClass,ui_wei_popup, 
					   NULL,0);
	     n = 0;
	     XtSetArg(args[n], XtNfromHoriz, NULL); n++;
	     XtSetArg(args[n], XtNborderWidth, 0); n++;
	     XtSetArg(args[n], XtNlabel, buf); n++;
	     label = XtCreateManagedWidget("weiMsg",labelWidgetClass,form,
					   args,n);
	     XawFormDoLayout(form, True);
	     XtRealizeWidget(ui_wei_popup);
    
	     /* now move the popup to the mouse position */
	     {
	       Window root;
	       int    x, y;
	       unsigned int borderWidth, depth, width, height;
	       XWindowChanges xChange;

	       (void) XGetGeometry(ui_display, XtWindow(ui_wei_popup),
				   &root, &x, &y, &width, &height, 
				   &borderWidth, &depth);

	       xChange.x = event->xbutton.x_root;
	       xChange.y = event->xbutton.y_root;
	       xChange.stack_mode = (int) Above;
	       XConfigureWindow(ui_display, XtWindow(ui_wei_popup), 
				CWX BIT_OR CWY BIT_OR CWStackMode, &xChange);
	     }

	     XtPopup(ui_wei_popup, XtGrabExclusive);
	     ui_wei_exit = FALSE;
	     while (NOT ui_wei_exit) {
	        XtAppNextEvent(ui_appContext, event);
		if(event->type == ButtonRelease){
		   XtDestroyWidget(ui_wei_popup);
		   ui_wei_exit = TRUE;
		}
	        (void) XtDispatchEvent(event);
	     }
             break;

        default: /* ignore all other events */
             break;
    }
}




/*****************************************************************************
  FUNCTION : ui_wght_setup

  PURPOSE  : Setup panel for the grid width and the color scale 
  NOTES    :
  RETURNS  : 
  UPDATE   : 04.08.93
******************************************************************************/
static void ui_wght_setup(Widget button, caddr_t call_data)
{
 

    Widget      grid_lab,WEIGHT_GRAPH_setupwidget;
    Widget      ui_setupPanel,done,subframe,dummy,range_lab;
    char        buf[80];
    Position    x, y;
    Dimension   width, height;
    Arg		args[25];   
    Cardinal	n = 0;

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

    WEIGHT_GRAPH_setupwidget = 
	XtCreatePopupShell("Setup", transientShellWidgetClass, 
			   ui_toplevel, args, n);
    ui_setupPanel =
	XtCreateManagedWidget("form", formWidgetClass, 
			      WEIGHT_GRAPH_setupwidget, NULL, 0);

    done = ui_xCreateButtonItem("done", ui_setupPanel, NULL, NULL);
    XtAddCallback (done, XtNcallback, (XtCallbackProc)ui_wght_close_setup,
		   WEIGHT_GRAPH_setupwidget);

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
    grid_lab = ui_xCreateLabelItem("grid width   :", subframe,104, NULL, NULL);

    sprintf(buf,"%d", WEIGHT_gridWidth);
    grid_dia = ui_xCreateDialogItem("gridWidth", subframe, buf, 
				    titelWidth, grid_lab,NULL);

    range_lab = ui_xCreateLabelItem("weight range :", subframe, 
				    104, NULL, grid_lab);

    sprintf(buf,"%g",minWght);
    bottomLabel = ui_xCreateDialogItem("bottLab", subframe, buf, 
				       floatWidth,range_lab,grid_lab);
    dummy = ui_xCreateLabelItem("to",subframe,24,bottomLabel,grid_lab);
    
    sprintf(buf,"%g",maxWght);
    topLabel = ui_xCreateDialogItem("topLab", subframe, buf, 
				    floatWidth, dummy,grid_lab);

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

    XtRealizeWidget(WEIGHT_GRAPH_setupwidget);
    XtAddEventHandler(scale2, ExposureMask, GraphicsExpose,
		      (XtEventHandler) ui_wght_scale,NULL);


    /* Realize the window */
    XtPopup(WEIGHT_GRAPH_setupwidget, XtGrabExclusive);

}              


/*****************************************************************************
  FUNCTION : ui_wght_close_setup

  PURPOSE  : notify xgui about the destruction of the setup panel and read 
             out the widget values
  NOTES    :
  RETURNS  : 
  UPDATE   : 29.01.92
******************************************************************************/
static void ui_wght_close_setup(Widget dummy, Widget w, caddr_t call_data)
{
   int test;


   /* read/set the setup values */

   test = (int)ui_xIntFromAsciiWidget(grid_dia);
   if(test > krui_getNoOfUnits()+10)
       ui_confirmOk("Ignoring unplausible large grid size");
   else if(test > 0)
      WEIGHT_gridWidth = test;
   else
      ui_confirmOk("Negative grid size not allowed!!");

   maxWght = (float)ui_xFloatFromAsciiWidget(topLabel);
   minWght = (float)ui_xFloatFromAsciiWidget(bottomLabel);


   /* update display according to the new values */
   ui_wght_scale(scale);
   if(ui_col_colorDisplay){
       ui_displWeightsCOL();
   }else{
       ui_displWeightsBW();
   }

   XtDestroyWidget(w);


}



/*****************************************************************************
  FUNCTION : ui_wght_scale

  PURPOSE  : Program to display the scale of colors /scale of square sizes
  NOTES    :
  RETURNS  : 
  UPDATE   : 04.08.93
******************************************************************************/
static void ui_wght_scale(Widget w)
{
    Window scale_win;
    int n;
    uint size = 10;
    char buf[80];

    scale_win = XtWindow(w);
    XClearWindow(ui_display,scale_win);
    

    if(ui_col_monochromeMode){

       /* on B/W screens display a row of different sized squares */

       sprintf(buf,"%-g",minWght);
       XDrawString(ui_display,scale_win,dwght_gc,0,10,buf,strlen(buf));
       for(n=1; n<=10; n++){
	   size = (uint)(10 - n);
	   XDrawRectangle(ui_display,scale_win,dwght_gc,
			  10*(n-1),14+(int)(n/2),size,size);
       }
       for(n=11; n<=19; n++){
	   size = (uint)(n - 10);
	   XFillRectangle(ui_display,scale_win,dwght_gc,
			  10*(n-1),14+(int)((20-n)/2),size,size);       
       }
       sprintf(buf,"%g",maxWght);
       XDrawString(ui_display,scale_win,dwght_gc,190-strlen(buf)*7,
		   10,buf,strlen(buf));

    }else{ /* on color screens display the color range */

       sprintf(buf,"%-g",minWght);
       XDrawString(ui_display, scale_win, dwght_gc, 0, 10, buf, strlen(buf));
       for(n=1; n<=19; n++){
	   XSetForeground(ui_display,dwght_gc,
		       (uint) ui_col_rangePixels[(int)(n*1.5)]); 
	   XFillRectangle(ui_display,scale_win,dwght_gc,10*(n-1),12,size,size);
       }
       sprintf(buf,"%g",maxWght);
       XSetForeground(ui_display,dwght_gc,BlackPixel(ui_display,dwght_screen));
       XDrawString(ui_display,scale_win,dwght_gc,190-strlen(buf)*7,
		   10,buf,strlen(buf));
   }
}



/*****************************************************************************
  FUNCTION : ui_wght_zoom_in

  PURPOSE  : Zomms onto the weight squares
  NOTES    :
  RETURNS  : 
  UPDATE   : 20.07.1993
******************************************************************************/
static void ui_wght_zoom_in(Widget w)
{

    /* delete the old screen display */
    XClearWindow(ui_display,dwght_win);

    /* compute new display geometry */
    ui_wght_geometry(1);
    
    /* display the new setting */
    if(ui_col_monochromeMode){
	ui_displWeightsBW();
    }else{
	ui_displWeightsCOL();
    }
}




/*****************************************************************************
  FUNCTION : ui_wght_zoom_out

  PURPOSE  : Zomms away from the weight squares
  NOTES    :
  RETURNS  : 
  UPDATE   : 20.07.1993
******************************************************************************/
static void ui_wght_zoom_out(Widget w)
{

    /* delete the old screen display */
    XClearWindow(ui_display,dwght_win);

    /* compute new display geometry */
    ui_wght_geometry(-1);

    /* display the new setting */
    if(ui_col_monochromeMode){
	ui_displWeightsBW();
    }else{
	ui_displWeightsCOL();
    }
}



/*****************************************************************************
  FUNCTION : ui_wght_geometry

  PURPOSE  : compute the geometry of the displays and the size of the squares
  NOTES    :
  RETURNS  : 
  UPDATE   : 04.08.1993
******************************************************************************/
static void ui_wght_geometry(int zoom_fact)
{
    XtWidgetGeometry maxi, geo_data;
    XtGeometryResult geo_result;


    /* get widget geometry */
    geo_result = XtQueryGeometry(WEIGHT_GRAPH_mainwidget,&geo_data,&maxi);
    if(geo_result != XtGeometryYes){
	maxi.height = 400;
	maxi.width  = 400;
    }

    /* adjust squaresize */
    if(ui_col_monochromeMode){ /* this is a  B&W terminal */
	HINTON_squaresize = HINTON_squaresize + zoom_fact;
	if(HINTON_squaresize == 0)HINTON_squaresize = 1;
	WEIGHT_windowsize = HINTON_squaresize*maxunits+40;
	WEIGHT_viewsize = HINTON_squaresize * maxunits;
    }else{ /* this is a color terminal */
	WEIGHT_squaresize = WEIGHT_squaresize + zoom_fact;
	if(WEIGHT_squaresize == 0)WEIGHT_squaresize = 1;
	WEIGHT_windowsize = WEIGHT_squaresize*maxunits+62;
	WEIGHT_viewsize = WEIGHT_squaresize * maxunits;
    }


    /* Resize all involved drawables so that they fit the data and the screen */
    XFreePixmap(ui_display,dwght_pix);
    dwght_pix = XCreatePixmap(ui_display,dwght_win,
			      WEIGHT_windowsize,WEIGHT_windowsize, dwght_depth);

    XtResizeWidget(WEIGHT_GRAPH_displaywidget,WEIGHT_windowsize,
		   WEIGHT_windowsize,1);
    if(WEIGHT_windowsize > maxi.height-73){
       if(WEIGHT_windowsize > maxi.width-20){
	   XtResizeWidget(view,maxi.width-20, maxi.height-73,1);
       }else{
	   XtResizeWidget(view,WEIGHT_windowsize, maxi.height-73,1);
       }
    }else{
       if(WEIGHT_windowsize > maxi.width-20){
	   XtResizeWidget(view,maxi.width-20, WEIGHT_windowsize,1);
       }else{
	   XtResizeWidget(view,WEIGHT_windowsize, WEIGHT_windowsize,1);
       }
    }
}



/*****************************************************************************
  FUNCTION : ui_wght_grid

  PURPOSE  : draws a grid over the weight squares and the axis
  NOTES    : type == 1 for color display; == 0 for B/W
  RETURNS  : 
  UPDATE   : 20.07.1993
******************************************************************************/
static void ui_wght_grid(int type)
{

   char outString[40];
   int  step;
   int  n;
   int actual_size;
   XPoint p1,p2,p3,p4;
   static char dash[]={6,6,6};



   /* set the square size */
   if(ui_col_monochromeMode)
   	actual_size = HINTON_squaresize;
   else 
   	actual_size = WEIGHT_squaresize;


   /* set output grid of units */
   dash[0] = dash[1] = (int)(actual_size*0.5 +0.5);
   XSetForeground(ui_display,dwght_gc,BlackPixel(ui_display,dwght_screen));
   XSetDashes(ui_display,dwght_gc,(int)(actual_size*0.5 +0.5)+20*type,dash,2);
   XSetLineAttributes(ui_display,dwght_gc,0,LineOnOffDash,CapButt,JoinMiter);
   step = WEIGHT_gridWidth;
   n = step;

   p1.y = 10;
   p2.x = (maxunits)*actual_size+30;
   p3.x = 0; 
   p4.y = (maxunits)*actual_size+20;
   while(n <= maxunits){
      sprintf(outString,"%d",n);
      p1.x = (int)(actual_size*n-actual_size*0.5)+20;
      p2.y = p1.x;
      p3.y = p2.y;
      p4.x = p1.x;
      XDrawString(ui_display,dwght_pix,dwght_gc,
		  p1.x,p1.y,outString,(int)strlen(outString));
      XDrawString(ui_display,dwght_pix,dwght_gc,
		  p2.x,p2.y,outString,(int)strlen(outString));
      XDrawString(ui_display,dwght_pix,dwght_gc,
		  p3.x,p3.y,outString,(int)strlen(outString));
      XDrawString(ui_display,dwght_pix,dwght_gc,
		  p4.x,p4.y+20,outString,(int)strlen(outString));
      XDrawLine(ui_display,dwght_pix,dwght_gc,p1.x,p1.y,p4.x,p4.y);
      XDrawLine(ui_display,dwght_pix,dwght_gc,p2.x,p2.y,p3.x+10,p3.y);
      n += step;
   }
   XSetLineAttributes(ui_display,dwght_gc,0,LineSolid,CapButt,JoinMiter);

}
/* end of file */

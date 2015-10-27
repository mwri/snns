/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_inversion.c,v $
  SHORTNAME      : inversion.c
  SNNS VERSION   : 4.2

  PURPOSE  : contains all routines for handling inversion of NN, invented
            by Alexander Linden

  FUNCTIONS: -- ui_inversion
               Purpose : Main program for the inversion
	        Calls   : void ui_confirmOk();
		          Widget ui_xCreateButtonItem();
			  int ui_set_IO_units();
                         various X routines for widget handling

            -- ui_invSetup
               Purpose : Display setup panel for Inversion
               Calls   : Widget ui_xCreateDialogItem();
                         Widget ui_xCreateLabelItem();

            -- ui_inv_popupDone
               Purpose : read new params and close popup display
               Calls   : nothing

            -- ui_invHelp
               Purpose : display help information for inversion
               Calls   : nothing

            -- ui_inv_helpDone
               Purpose : close help window
               Calls   : nothing

            -- ui_set_IO_units
               Purpose : create a chain of input units 
               Calls   : int krui_getUnitTType();
                         void krui_getUnitPosition();

            -- ui_closeDisplay
               Purpose : closing all widgets for inversion
               Calls   : nothing

            -- ui_invNew
               Purpose : restart the inversion
               Calls   : void ui_drawInput();

            -- ui_invEvent
               Purpose : event handler for graphic window 
               Calls   : void ui_drawInput();

            -- ui_inversionFromUpdate
               Purpose : refresh the screen
               Calls   : void ui_drawInput();

            -- ui_drawInput
               Purpose : drawing the input units
               Calls   : int krui_getFirstUnit();
                         void ui_col_setPalette();
                         void krui_getUnitPosition();
                         void ui_drawGrowingThing();
                         position ui_utilPixUpperLeft();
                         position ui_utilPixLowerRight();
                         void ui_xDeleteRect();

            -- ui_start_inversion          
               Purpose : Program to start inversion 
               Calls   : X routines

            -- ui_stop_inversion          
               Purpose : Program to stop inversion 
               Calls   : X routines

            -- ui_do_inversion          
               Purpose : Program for actual inversion algorithm
               Calls   : void ui_confirmOk();
                         void ui_sel_lookForItem();
                         int krui_initInversion();
                         void krui_inv_forwardPass();
                         int krui_inv_backwardPass();
                         void ui_drawInput();

 AUTHOR   : Guenter Mamier
 DATE     : 28.02.92
 VERSION  : 0.20

 CHANGED BY     : Sven Doering
 RCS VERSION    : $Revision: 2.11 $
 LAST CHANGE    : $Date: 1998/03/03 14:10:30 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#ifndef MASPAR_KERNEL

#include <stdio.h>			/* For the Syntax message */
#include <stdlib.h>
#include <math.h>

#include "ui.h"

#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>

#include "ui_xWidgets.h"
#include "ui_main.h"
#include "ui_mainP.h"
#include "ui_netGraph.h"
#include "ui_utilP.h"
#include "ui_display.h"
#include "ui_selection.h"
#include "ui_color.h"
#include "ui_confirmer.h"
#include "ui_xGraphic.h"
#include "ui_key.h"
#include "kr_ui.h"
#include "kr_typ.h"

#include "ui_inversion.ph"


/*****************************************************************************
  FUNCTION : ui_inversion

  PURPOSE  : Main program for the inversion
  NOTES    : callback from the info panel
  RETURNS  :
  UPDATE   : 29.01.92
******************************************************************************/

void ui_inversion(Widget button, caddr_t call_data)
{

    Widget         fbutton,sbutton,stbutton,setbutton,stopbutton,helpbutton;
    Arg		   args[10];
    Cardinal	   n;
    char           buf[40];
    struct UnitList *IUnit, *OUnit;
    struct Ui_DisplayType *dPtr;
    int maxx,maxy;
    int test;

    if (krui_getNoOfUnits() == 0){
	ui_confirmOk("No network loaded !");
	return;
    }


    if(INVERS_CREATED){
       XRaiseWindow(XtDisplay(ui_InvRootWidget), XtWindow(ui_InvRootWidget));
       return;
    }

    if((test = ui_set_IO_units()) == 1){
      ui_confirmOk("No input and/or output units defined !");
      return;
    }


    if ((displayPtr = ui_displ_getFreeItem()) == NULL) {
	ui_confirmOk("No more memory for displays available!");
	return;
    }


    /* now create the widget structure to display input units */

    n = 0;
    displayPtr->gridSize = 37;
    dPtr = ui_displ_listPtr;
    while(dPtr != NULL){
       displayPtr->gridSize = dPtr->gridSize;
       dPtr = dPtr->nextPtr;
    }
 
    sprintf(buf,"SNNS Inversion Display");
    ui_InvRootWidget =
	XtCreatePopupShell(buf, topLevelShellWidgetClass, ui_toplevel,args, n);
    displayPtr->frameWidget = 
	XtCreateManagedWidget("form", formWidgetClass, ui_InvRootWidget,NULL,0);

    fbutton = ui_xCreateButtonItem("done", displayPtr->frameWidget, NULL, NULL);
    sbutton    = ui_xCreateButtonItem("multiStep", displayPtr->frameWidget,
				   fbutton, NULL);
    stopbutton = ui_xCreateButtonItem("stop", displayPtr->frameWidget,
				      sbutton, NULL);
    stbutton   = ui_xCreateButtonItem("new", displayPtr->frameWidget,
				   stopbutton, NULL);
    setbutton  = ui_xCreateButtonItem("setup", displayPtr->frameWidget,
				   stbutton, NULL);
    helpbutton  = ui_xCreateButtonItem("help", displayPtr->frameWidget,
				   setbutton, NULL);


    /* get necessary window size */

    maxy = maxx = 0;
    IUnit = inputs;
    do{
      maxy = (IUnit->gridPos.y > maxy)? IUnit->gridPos.y: maxy;
      maxx = (IUnit->gridPos.x > maxx)? IUnit->gridPos.x: maxx;
      IUnit = IUnit->next;
    }while(IUnit != NULL);
    OUnit = outputs;
    do{
      maxy = (OUnit->gridPos.y > maxy)? OUnit->gridPos.y: maxy;
      maxx = (OUnit->gridPos.x > maxx)? OUnit->gridPos.x: maxx;
      OUnit = OUnit->next;
    }while(OUnit != NULL);
    
    n = 0;
    /* XtSetArg(args[n], XtNwidth,  displayPtr->width); n++; */
    /* XtSetArg(args[n], XtNheight, displayPtr->height); n++; */
    XtSetArg(args[n], XtNwidth,    ++maxx * displayPtr->gridSize); n++;
    XtSetArg(args[n], XtNheight,   ++maxy * displayPtr->gridSize); n++;
    XtSetArg(args[n], XtNfromVert, fbutton); n++;
    XtSetArg(args[n], XtNleft,     XtChainLeft); n++;
    XtSetArg(args[n], XtNright,    XtChainRight); n++;
    XtSetArg(args[n], XtNtop,      XtChainTop ); n++;
    XtSetArg(args[n], XtNbottom,   XtChainBottom); n++;
    displayPtr->widget = 
        XtCreateManagedWidget("display",boxWidgetClass,displayPtr->frameWidget,
			      args,n);


    /* define necessary callbacks */

    XtAddCallback(fbutton, XtNcallback,(XtCallbackProc)ui_closeDisplay,NULL);
    XtAddCallback(sbutton, XtNcallback,(XtCallbackProc)ui_start_inversion,NULL);
    XtAddCallback(stbutton, XtNcallback, (XtCallbackProc) ui_invNew, NULL);
    XtAddCallback(stopbutton, XtNcallback, 
		  (XtCallbackProc) ui_stop_inversion, NULL);
    XtAddCallback(setbutton,  XtNcallback, (XtCallbackProc) ui_invSetup, NULL);
    XtAddCallback(helpbutton, XtNcallback, (XtCallbackProc) ui_invHelp, 
		  (caddr_t) TRUE);
    XtAddEventHandler(displayPtr->widget,StructureNotifyMask | ExposureMask,
		      FALSE, (XtEventHandler) ui_invEvent,inv_display);
    XtAddEventHandler(displayPtr->widget,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    ui_checkWindowPosition(ui_InvRootWidget);
    XtPopup(ui_InvRootWidget, XtGrabNone);

    INVERS_CREATED = 1;
    inv_display   = XtDisplay(ui_InvRootWidget);
    inv_screen    = DefaultScreen(inv_display);
    displayPtr->drawable = XtWindow(displayPtr->widget);
    XSelectInput(inv_display, displayPtr->drawable,  ExposureMask);


}




/*****************************************************************************
  FUNCTION : ui_invSetup

  PURPOSE  : Display setup panel for Inversion
  NOTES    : 
  RETURNS  :
  UPDATE   : 20.02.92
*****************************************************************************/
static void ui_invSetup(Widget button, caddr_t call_data)
{
    Widget      ui_invBox,doneButton,helpButton;
    Widget      eta_lab,delta_lab,pat_lab,rat_lab;
    Arg         args[5];
    Position    x, y;
    Dimension   width, height;
    Cardinal    n;
    char        buf[40];


    /* set Popup arguments */

    n = 0;
    XtSetArg(args[0], XtNwidth, &width);n++;
    XtSetArg(args[1], XtNheight, &height);n++;
    XtGetValues(button, args, n);
    XtTranslateCoords(button, (Position)(width/2), (Position)(height/2), &x,&y);
    n = 0;
    XtSetArg(args[n], XtNx, x);n++;
    XtSetArg(args[n], XtNy, y);n++;


    /* Now create Popup */

    ui_invpop = XtCreatePopupShell("setup", transientShellWidgetClass, 
				   ui_toplevel, args, n);
    ui_invBox = XtCreateManagedWidget("form", formWidgetClass, ui_invpop, 
				      NULL, 0);
    doneButton = ui_xCreateButtonItem("done", ui_invBox, NULL, NULL);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_inv_popupDone, 
		  NULL);
    helpButton = ui_xCreateButtonItem("help", ui_invBox, doneButton, NULL);
    XtAddCallback(helpButton, XtNcallback, (XtCallbackProc) ui_invHelp, FALSE);
    eta_lab = ui_xCreateLabelItem("eta           =", ui_invBox,120,
				  NULL,doneButton);
    sprintf(buf,"%g",INV_eta);
    etaW = ui_xCreateDialogItem("eta_val",ui_invBox,buf,56,eta_lab,doneButton);
    delta_lab = ui_xCreateLabelItem("delta_max     =",ui_invBox,120,
				    NULL,eta_lab);
    sprintf(buf,"%g",INV_delta_max);
    delta_maxW = ui_xCreateDialogItem("delta_max_val",ui_invBox, 
				      buf, 56, delta_lab, etaW);
    pat_lab = ui_xCreateLabelItem("Input pattern =",ui_invBox,120,NULL,
				  delta_lab);
    sprintf(buf,"%g",INPUT_PAT);
    inPatW = ui_xCreateDialogItem("input_pat_val",ui_invBox,buf,56,
				  pat_lab,delta_maxW);
    rat_lab = ui_xCreateLabelItem("2nd approx ratio",ui_invBox,120,NULL,
				  pat_lab);
    sprintf(buf,"%g",RATIO);
    ratioW = ui_xCreateDialogItem("ratio_val",ui_invBox,buf,56,
				  rat_lab,inPatW);


    /* Display popup now */

    ui_checkWindowPosition(ui_invpop);
    XtPopup(ui_invpop, XtGrabExclusive);
    ui_xDontResizeWidget(ui_invpop); 
}





/*****************************************************************************
  FUNCTION : ui_inv_popupDone

  PURPOSE  : read new params and close popup display
  NOTES    : 
  RETURNS  :
  UPDATE   : 20.02.92
*****************************************************************************/
static void ui_inv_popupDone(Widget button, caddr_t call_data)
{
    struct UnitList *IUnit;

    INPUT_PAT     = (float)ui_xFloatFromAsciiWidget(inPatW);
    INPUT_PAT     = (INPUT_PAT > 1.0)? 1.0 :INPUT_PAT;
    INPUT_PAT     = (INPUT_PAT < 0.0)? 0.0 :INPUT_PAT;
    INV_eta       = (float)ui_xFloatFromAsciiWidget(etaW);
    INV_delta_max = (float)ui_xFloatFromAsciiWidget(delta_maxW);
    RATIO         = (float)ui_xFloatFromAsciiWidget(ratioW);
    XtDestroyWidget(ui_invpop);

    IUnit = inputs;
    while(IUnit != NULL){
      IUnit->act = (FlintType)INPUT_PAT;
      IUnit->i_act = (FlintType)INPUT_PAT;
      if(INPUT_PAT == 1.0)
	IUnit->im_act  = 9.2102404;  
      else if(INPUT_PAT == 0.0)
	IUnit->im_act  = -9.2102404; 
      else
	IUnit->im_act  = (FlintType)(-log((double)(1.0/INPUT_PAT - 1.0))); 
      IUnit = IUnit->next;
    }
    ui_drawInput();
}





/*****************************************************************************
  FUNCTION : ui_inv_helpDone

  PURPOSE  : destroy help window
  NOTES    : 
  RETURNS  :
  UPDATE   : 03.03.92
*****************************************************************************/
static void ui_inv_helpDone(Widget button, caddr_t call_data)
{
    XtDestroyWidget(XtParent(XtParent(button)));
}




/*****************************************************************************
  FUNCTION : ui_invHelp

  PURPOSE  : display help information for inversion
  NOTES    : 
  RETURNS  :
  UPDATE   : 03.03.92
*****************************************************************************/
static void ui_invHelp(Widget button, Boolean fromMain, caddr_t call_data)
{
    Widget      ui_invHBox,ui_invHelp,doneButton,lab[10];
    Arg         args[5];
    Position    x, y;
    Dimension   width, height;
    Cardinal    n;
    char        buf[80];

    /* set Popup arguments */

    n = 0;
    XtSetArg(args[0], XtNwidth, &width);n++;
    XtSetArg(args[1], XtNheight, &height);n++;
    XtGetValues(button, args, n);
    XtTranslateCoords(button, (Position)(width), (Position)(height), &x, &y);
    n = 0;
    XtSetArg(args[n], XtNx, x);n++;
    XtSetArg(args[n], XtNy, y+100);n++;


    /* Now create Popup */

    ui_invHelp = XtCreatePopupShell("help", transientShellWidgetClass, 
				   ui_toplevel, args, n);
    ui_invHBox = XtCreateManagedWidget("form", formWidgetClass, ui_invHelp, 
				      NULL, 0);
    doneButton = ui_xCreateButtonItem("done", ui_invHBox, NULL, NULL);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_inv_helpDone, NULL);


    if(fromMain){
       sprintf(buf,"Start/continue inversion with 'STEP'");
       lab[0] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,doneButton);
       sprintf(buf,"Halt inversion run anytime with 'STOP'");
       lab[1] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[0]);
       sprintf(buf,"Reset inversion with 'NEW'");
       lab[2] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[1]);
       sprintf(buf,"Change parameters with 'SETUP'");
       lab[3] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[2]);
       sprintf(buf,"After SETUP continue with 'STEP',");
       lab[4] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[3]);
       sprintf(buf,"  or restart with 'NEW','STEP'");
       lab[5] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[4]);
       sprintf(buf,"HINT: If inversion is performed on an untrained net,");
       lab[6] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[5]);
       sprintf(buf,"      or on a trained net with an average error of more than 0.05,");
       lab[7] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[6]);
       sprintf(buf,"      the algorithm is very likely not to converge !!");
       lab[8] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[7]);
    }else{
       sprintf(buf,"eta           : The learn parameter; should range from 1.0 to 10.0");
       lab[0] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,doneButton);
       sprintf(buf,"delta_max     : The maximum allowed error for a output unit");
       lab[1] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[0]);
       sprintf(buf,"Input  pattern: The initial activation for all input units");
       lab[2] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[1]);
       sprintf(buf,"2nd approx    : The degree to which the target input pattern");
       lab[3] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[2]);
       sprintf(buf,"                is dependent of the initial input pattern");
       lab[4] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[3]);
       sprintf(buf,"                good values range from 0.2 to 0.8");
       lab[5] = ui_xCreateLabelItem(buf,ui_invHBox,strlen(buf)*8,NULL,lab[4]);
    }


    /* Display popup now */

    ui_checkWindowPosition(ui_invHelp);
    XtPopup(ui_invHelp, XtGrabExclusive);
    XFlush(ui_display);


}




/*****************************************************************************
  FUNCTION : ui_set_IO_units

  PURPOSE  : create a chain of input units and the corresponding pattern
  NOTES    : 
  RETURNS  :
  UPDATE   : 29.01.92
*****************************************************************************/
static int ui_set_IO_units(void)
{
    int            next;
    int            u_type;
    struct PosType position;
    struct UnitList *IUnit,*OUnit,*dummy1,*dummy2;
    int            i_count = 0,
                   o_count = 0,
                   i,
                   max_no;

    if((next = krui_getFirstUnit()) == 0)return(0);

    IUnit = (struct UnitList *) malloc(sizeof(struct UnitList));
    inputs = IUnit;
    IUnit->prev = NULL;
    OUnit = (struct UnitList *) malloc(sizeof(struct UnitList));
    outputs = OUnit;
    IUnit->prev = NULL;
    max_no = krui_getNoOfUnits();
    for(i=1; i<= max_no; i++){
      u_type = krui_getUnitTType(next);
      if(u_type == INPUT){
	 i_count++;
 	 krui_getUnitPosition(next,&position);
	 dummy1 = IUnit;
	 IUnit->no      = next;
	 IUnit->gridPos = position;
         IUnit->act     = (FlintType)INPUT_PAT;
         IUnit->i_act   = (FlintType)INPUT_PAT;
	 if(INPUT_PAT == 1.0)
	   IUnit->im_act  = 9.2102404;   /* = -ln(1/0.9999 - 1); */
	 else if(INPUT_PAT == 0.0)
	   IUnit->im_act  = -9.2102404;  /* = -ln(1/0.0001 - 1); */
	 else
	   IUnit->im_act  =(FlintType)( -log((double)(1.0/INPUT_PAT - 1.0))); 
	 IUnit->next    = (struct UnitList *) malloc(sizeof(struct UnitList));
         IUnit          = IUnit->next;	 
	 IUnit->prev    = dummy1;
      }else if(u_type == OUTPUT){
	 o_count++;
 	 krui_getUnitPosition(next,&position);
	 dummy2         = OUnit;
	 OUnit->no      = next;
	 OUnit->gridPos = position;
         OUnit->act     = 0.0;
	 OUnit->next    = (struct UnitList *) malloc(sizeof(struct UnitList));
         OUnit          = OUnit->next;	 
	 OUnit->prev    = dummy2;
      }
      next=krui_getNextUnit();
    }

    if(i_count == 0 || o_count == 0){

      /* Stop inversion if no input or output units are defined */ 
      return(1);
    }else{

      /* Finish the pointer chain */
      dummy1->next = NULL;
      dummy2->next = NULL;
      return(0);
    }

}




/*****************************************************************************
  FUNCTION : ui_closeDisplay

  PURPOSE  : closing all widgets for inversion
  NOTES    : 
  RETURNS  :
  UPDATE   : 29.01.92
*****************************************************************************/
static void ui_closeDisplay(Widget w, caddr_t call_data)
{


    if(INV_RUNNING){
       ui_confirmOk("Stop inversion run before quiting !");
       return;
    }
    XtDestroyWidget(ui_InvRootWidget);
    INVERS_CREATED = 0;
    inputs  = NULL;
    outputs = NULL;
}


/*****************************************************************************
  FUNCTION : ui_invNew

  PURPOSE  : restart the inversion
  NOTES    : 
  RETURNS  :
  UPDATE   : 11.02.92
*****************************************************************************/
static void ui_invNew(Widget w, caddr_t call_data)
{
    struct UnitList *IUnit, *OUnit;

    IUnit = inputs;
    while(IUnit != NULL){
      IUnit->act = (FlintType)INPUT_PAT;
      IUnit->i_act = (FlintType)INPUT_PAT;
      if(INPUT_PAT == 1.0)
	IUnit->im_act  = 9.2102404;   /* = -ln(1/0.9999 - 1); */
      else if(INPUT_PAT == 0.0)
	IUnit->im_act  = -9.2102404;  /* = -ln(1/0.0001 - 1); */
      else
	IUnit->im_act  = (FlintType)(-log((double)(1.0/INPUT_PAT - 1.0))); 
      IUnit = IUnit->next;
    }
    OUnit = outputs;
    while(OUnit != NULL){
      OUnit->act = 0.0;
      OUnit->i_act = 0.0;
      OUnit = OUnit->next;
    }
    ui_drawInput();
    INV_NEW = 1;
}




/*****************************************************************************
  FUNCTION : ui_invEvent

  PURPOSE  : event handler for graphic window
  NOTES    : 
  RETURNS  :
  UPDATE   : 20.02.92
*****************************************************************************/
static void ui_invEvent(Widget w, Display *display, XEvent *event)
{
    switch (event->type){
        case Expose:
               if (event->xexpose.count == 0)
		 ui_drawInput();
	       break;
	default:
	       break;
    }
}





/*****************************************************************************
  FUNCTION : ui_drawInput

  PURPOSE  : drawing the input units
  NOTES    : 
  RETURNS  :
  UPDATE   : 29.01.92
*****************************************************************************/
static void ui_drawInput(void)
{

    int            next;
    int            u_type;
    int            dx,dy;
    int            procent_value;
    struct PosType position,pixpos,pixpos2;
    struct UnitList *IUnit, *OUnit;
    


    /* do nothing if no net is loaded */

    if((next = krui_getFirstUnit()) == 0)return;
    

    /* prepare window for drawing */

    displayPtr->frozen = FALSE;
    XClearWindow(inv_display,displayPtr->drawable);
    if(ui_col_monochromeMode){
       XSetBackground(inv_display,ui_gc,
		      ui_backgroundColor);
       XSetForeground(inv_display,
		      ui_gc,ui_textColor);

       /* draw rectangle for EACH input/output unit */
       do{
         u_type = krui_getUnitTType(next);
         if(u_type == OUTPUT || u_type == INPUT){
 	    krui_getUnitPosition(next,&position);
            ui_drawGrowingThing(displayPtr, position, -100);
         }
       }while((next=krui_getNextUnit()) != 0);
    }else{
       XSetBackground(inv_display,ui_gc,
		   ui_backgroundColor);
       XSetForeground(inv_display,
		   ui_gc, ui_editColor[UI_BLACK]);
       XSetFunction(inv_display, ui_gc, GXcopy);
    }


    /* draw activation of input units */

    IUnit = inputs;
    do{
      position.x = displayPtr->gridSize * (IUnit->gridPos.x-displayPtr->origin.x) + 
	           displayPtr->gridSize/2;
      position.y = displayPtr->gridSize * (IUnit->gridPos.y-displayPtr->origin.y) + 
	           displayPtr->gridSize/2;
      procent_value = (int)(100.0*IUnit->act);
      if(ui_col_monochromeMode){
         pixpos = ui_utilPixUpperLeft(displayPtr,IUnit->gridPos);
         pixpos2 = ui_utilPixLowerRight(displayPtr,IUnit->gridPos,0);
	 dx = dy = (int)((float)(pixpos2.x - pixpos.x)*IUnit->act);
	 pixpos.x += (int)((float)(pixpos2.x-pixpos.x)*0.5*(1.0 - IUnit->act)+0.5);
	 pixpos.y += (int)((float)(pixpos2.y-pixpos.y)*0.5*(1.0 - IUnit->act)+0.5);
	 XFillRectangle(inv_display, displayPtr->drawable, ui_gc,
			pixpos.x,pixpos.y,(unsigned int) dx,(unsigned int) dy);
      }else{
         pixpos = ui_utilPixUpperLeft(displayPtr,IUnit->gridPos);
         pixpos2 = ui_utilPixLowerRight(displayPtr,IUnit->gridPos,0);
	 XSetForeground(inv_display, ui_gc, 
	       ui_col_rangePixels[ui_col_steps+procent_value*ui_col_steps DIV 100]);
	 ui_xDeleteRect(inv_display, displayPtr->drawable, ui_gc,pixpos,pixpos2);
      }
      IUnit = IUnit->next;
    }while(IUnit != NULL);


    /* draw activation of output units */

    OUnit = outputs;
    do{
      position.x = displayPtr->gridSize * (OUnit->gridPos.x-displayPtr->origin.x) + 
	           displayPtr->gridSize/2;
      position.y = displayPtr->gridSize * (OUnit->gridPos.y-displayPtr->origin.y) + 
	           displayPtr->gridSize/2;

      procent_value = (int)(100.0*OUnit->act); 
      if(NOT ui_col_monochromeMode){
         pixpos = ui_utilPixUpperLeft(displayPtr,OUnit->gridPos);
         pixpos2 = ui_utilPixLowerRight(displayPtr,OUnit->gridPos,0);
	 XSetForeground(inv_display, ui_gc, 
	       ui_col_rangePixels[ui_col_steps+procent_value*ui_col_steps DIV 100]);
	 ui_xDeleteRect(inv_display, displayPtr->drawable, ui_gc,pixpos,pixpos2);
      }else{ 
         pixpos = ui_utilPixUpperLeft(displayPtr,OUnit->gridPos);
         pixpos2 = ui_utilPixLowerRight(displayPtr,OUnit->gridPos,0);
	 dx = dy = (int)((float)(pixpos2.x - pixpos.x)*OUnit->act);
	 pixpos.x += (int)((float)(pixpos2.x-pixpos.x)*0.5*(1.0 - OUnit->act)+0.5);
	 pixpos.y += (int)((float)(pixpos2.y-pixpos.y)*0.5*(1.0 - OUnit->act)+0.5);
	 XFillRectangle(inv_display, displayPtr->drawable, ui_gc,
			pixpos.x,pixpos.y,(unsigned int) dx,(unsigned int) dy);
      }
      OUnit = OUnit->next;
    }while(OUnit != NULL);



    /* drawing finished */

    XFlush(inv_display);
    displayPtr->frozen = TRUE;
}





/*****************************************************************************
  FUNCTION : ui_start_inversion

  PURPOSE  : Program to start inversion
  NOTES    : callback from the control panel
  RETURNS  :
  UPDATE   : 29.01.92
******************************************************************************/
static void ui_start_inversion(Widget button, caddr_t call_data)
{
    if (ui_workProcId)
        XtRemoveWorkProc(ui_workProcId); /* kill old workProc */
    ui_workType = 99;
    INV_RUNNING = 1;
    ui_workProcId = XtAppAddWorkProc(ui_appContext, (XtWorkProc) ui_do_inversion,NULL); 
}



/*****************************************************************************
  FUNCTION : ui_stop_inversion

  PURPOSE  : Program to stop inversion
  NOTES    : callback from the control panel
  RETURNS  :
  UPDATE   : 29.01.92
******************************************************************************/
static void ui_stop_inversion(Widget button, caddr_t call_data)
{
    if(ui_workProcId){
      INV_RUNNING = 0;
      ui_drawInput();
      printf("cycle %d inversion error %f, %d error units left\n",
	     INV_cycle,INV_error,INV_units);
      XtRemoveWorkProc(ui_workProcId); /* kill old workProc */
      ui_workProcId = (XtWorkProcId) NULL;
    }
}



/*****************************************************************************
  FUNCTION : ui_do_inversion

  PURPOSE  : Program for actual inversion algorithm
  NOTES    : callback from the control panel
  RETURNS  :
  UPDATE   : 29.01.92
******************************************************************************/

static Boolean ui_do_inversion(Widget button, caddr_t call_data)
{
    struct UnitList *OUnit;
    struct SelectionType *SelDummy;
    int dummy;
    double old_err;
    int selUnits, err_units;
    int err;
    static int growing_errors = 0;


    /* set output pattern */
    
    if(INV_NEW){ 
       INV_cycle = 0;
       selUnits = 0;
       if(krui_getNoOfPatterns() == 0){
	  ui_confirmOk("Please load patterns first !");
          INV_RUNNING = 0;
	  return(TRUE);
       }
       OUnit = outputs;
       while(OUnit != NULL){
	 if((SelDummy = ui_sel_lookForItem(OUnit->no)) != NULL){
	   OUnit->i_act = 1.0;
	   OUnit->act = 1.0;
	   selUnits++;
	 }else{
	   OUnit->i_act = 0.0;
	   OUnit->act = 0.0;
	 }
	 OUnit = OUnit->next;
       }
       if(!selUnits){
          ui_confirmOk("No target output selected !!");
          INV_RUNNING = 0;
          return(TRUE);
       }
       INV_NEW = 0;  
    }


    /* do the inversion */

    if((err = krui_initInversion()) < 0){
       printf("init returned error %d\n",err);
       INV_RUNNING = 0;
       return(TRUE);
    }

    INV_cycle++;
    err_units = 0;
    old_err = INV_error;
    krui_inv_forwardPass(inputs);
    INV_error = krui_inv_backwardPass(INV_eta,INV_delta_max,&err_units,RATIO,
				      inputs,outputs);
    INV_units = err_units;
    if((dummy=(INV_cycle%50)) == 0)
	 printf("cycle %d inversion error %f still %d error unit(s)\n",
	        INV_cycle,INV_error,INV_units);
    if(old_err<= INV_error)growing_errors++;
    else growing_errors = 0;
/*
    if(growing_errors > 50){
       growing_errors = 0;
       ui_drawInput();
       ui_confirmOk(" Inversion error does not diminish!\n Net might not be properly trained !"); 
       INV_RUNNING = 0;
       return(TRUE);
    }
*/

    /* check the result of this inversion step */

    if(INV_units > 0){

       /* while still one unit has an error > delta_max return FALSE,  */
       /* to start another cycle                                       */
       return(FALSE);
    }else{

       /*   algorithm has converged, display discovered activation pattern */
       printf("cycle %d inversion error %f, %d error units left\n",
	      INV_cycle,INV_error,INV_units);
       ui_drawInput();
       INV_RUNNING = 0;
       return(TRUE);
    }
    

}

#endif



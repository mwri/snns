/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_kohonen.c,v $
  SHORTNAME      : ui_kohonen.c
  SNNS VERSION   : 4.2

  PURPOSE        : Creation/Callback routines for Kohonen Window
  NOTES          :

  AUTHOR         : Marcus Ritt
  DATE           : July 13 1993

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.13 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_xWidgets.h"
#include "ui_display.h"
#include "ui_mainP.h"
#include "ui_confirmer.h"
#include "ui_control.h"
#include "ui_controlP.h"
#include "ui_key.h"
#include "ui_netUpdate.h"

#include "ui_kohonen.ph"

/*****************************************************************************
  FUNCTION : kohonen_rem_displayLayerNumber(void)

  PURPOSE  : Display the value of ui_NoOfCurrentLayer in the control panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void kohonen_rem_displayLayerNumber(void)
{
    char  buf[50];

    if (NOT kohonen_open) return;

    sprintf(buf,"%d", noOfCurrentLayer);
    ui_xSetString(ui_LayerWidget, buf);
}

/*****************************************************************************
  FUNCTION : kohonen_rem_moveLayerProc

  PURPOSE  : callback. Called when cursor buttons or GOTO of LAYER is clicked.
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void kohonen_rem_moveLayerProc(Widget w, int moveType, caddr_t call_data)
{
  int  temp;
  Bool doMove = FALSE;

  if (krui_getNoOfInputUnits() == 0) {
    ui_confirmOk("No input units defined!");
    return;
  }
  if (!ui_controlIsCreated) {
    ui_confirmOk("Please create control panel first !"); 
    return;
  }

  switch(moveType) {
  case UI_JUMP:
    temp = ui_xIntFromAsciiWidget(ui_LayerWidget);
    if ((temp>0) AND (temp<= krui_getNoOfInputUnits())) {
      noOfCurrentLayer = temp;
      kohonen_rem_displayLayerNumber();
      doMove = TRUE;
    } else ui_confirmOk("Layer goto: invalid number!");
    break;
  case UI_FIRST:
    if (noOfCurrentLayer!=1) {
      noOfCurrentLayer = 1;
      kohonen_rem_displayLayerNumber();
      doMove = TRUE;
    }
    break;
  case UI_LAST:
    if (noOfCurrentLayer != krui_getNoOfInputUnits()) {
      noOfCurrentLayer = krui_getNoOfInputUnits();
      kohonen_rem_displayLayerNumber();
      doMove = TRUE;
    }
    break;
  case UI_NEXT:
    if (++noOfCurrentLayer > krui_getNoOfInputUnits()) 
      noOfCurrentLayer = 1;
    kohonen_rem_displayLayerNumber();
    doMove = TRUE;
    break;
  case UI_PREVIOUS:
    if (--noOfCurrentLayer < 1)
      noOfCurrentLayer = krui_getNoOfInputUnits(); 
    kohonen_rem_displayLayerNumber();
    doMove = TRUE;
    break;
  }
  if (doMove) ui_rem_stepsProc(NULL, TRUE, NULL);
}


/*****************************************************************************
  FUNCTION : kohonen_tree()

  PURPOSE  : callback. Called when WINNER button is clicked.
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void kohonen_tree(void)
{
    if (!ui_controlIsCreated) {
	ui_confirmOk("Please create control panel first !"); 
	return;
    }
    krui_spanning_tree();
    ui_net_completeRefresh(ui_currentDisplay, UI_GLOBAL); 
}


/*****************************************************************************
  FUNCTION : kohonen_doneProc

  PURPOSE  : callback function of the done-buttom. 

  NOTES    :

  UPDATE   : 
******************************************************************************/
static void kohonen_doneProc(void)
{
    XtDestroyWidget(kohonen_mainWidget);
    kohonen_open= 0;
}


/*****************************************************************************
  FUNCTION : kohonen_createWindow

  PURPOSE  : create the Kohonen window
  NOTES    :

  UPDATE   : 
******************************************************************************/
void kohonen_createWindow (void)
{
    Widget       button,done,box,form,kohonenLabel;
    Cardinal     n;
    Arg          arg[25];
    char         buf[20];

    int fontWidth = 8;
    int titelWidth  =  7 * fontWidth;
    int numberWidth = 10 * fontWidth;
  
    if (!ui_controlIsCreated) {
	ui_confirmOk("Please create control panel first !"); 
	return;
    }

    if (kohonen_open) {
	XRaiseWindow (XtDisplay (kohonen_mainWidget), 
		      XtWindow (kohonen_mainWidget));
	return;
    }

    kohonen_open= 1;
    sprintf(buf, "SNNS Kohonen Window");
    n = 0;  
    kohonen_mainWidget= XtCreatePopupShell(buf,topLevelShellWidgetClass,
					   ui_toplevel,arg,n); 
    box = XtCreateManagedWidget("box",boxWidgetClass,kohonen_mainWidget,arg,n);
    form= XtCreateManagedWidget("connt", formWidgetClass, box, arg, n);

    /************* Kohonen LAYER *********************************************/

    kohonenLabel = ui_xCreateLabelItem("LAYER", form, titelWidth, NULL, NULL);
    sprintf(buf,"%d",1);
    ui_LayerWidget = ui_xCreateDialogItem("Layers", form, 
			     buf, numberWidth, kohonenLabel, NULL);

    button = ui_xCreateButtonItem("goto", form, ui_LayerWidget, NULL);
    XtAddCallback (button, XtNcallback, (XtCallbackProc) 
		   kohonen_rem_moveLayerProc, (caddr_t) UI_JUMP);

    button = ui_xCreateButtonItem("first", form, button, NULL);
    XtAddCallback (button, XtNcallback, (XtCallbackProc) 
		   kohonen_rem_moveLayerProc, (caddr_t) UI_FIRST);

    button = ui_xCreateButtonItem("prev", form, button, NULL);
    XtAddCallback (button, XtNcallback, (XtCallbackProc) 
		   kohonen_rem_moveLayerProc, (caddr_t) UI_PREVIOUS);

    button = ui_xCreateButtonItem("next", form, button, NULL);
    XtAddCallback (button, XtNcallback, (XtCallbackProc) 
		   kohonen_rem_moveLayerProc, (caddr_t) UI_NEXT);

    button = ui_xCreateButtonItem("last", form, button, NULL);
    XtAddCallback (button, XtNcallback, (XtCallbackProc) 
		   kohonen_rem_moveLayerProc, (caddr_t) UI_LAST);

    button = ui_xCreateButtonItem("STree", form, button, NULL);
    XtAddCallback (button, XtNcallback, (XtCallbackProc) 
		   kohonen_tree, kohonenLabel);

    XtAddEventHandler(form,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    XawFormDoLayout(form, TRUE);

    done= ui_xCreateButtonItem("done", box, NULL, NULL);
    XtAddCallback(done,XtNcallback,(XtCallbackProc)kohonen_doneProc,NULL); 

    XtPopup (kohonen_mainWidget, XtGrabNone);

}  

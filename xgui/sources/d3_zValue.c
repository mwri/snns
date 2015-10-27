/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_zValue.c,v $
  SHORTNAME      : zValue.c
  SNNS VERSION   : 4.2

  PURPOSE        : creates the Z-value pannel
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:06 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Toggle.h>

#include "ui.h"

#include "glob_typ.h"
#include "ui_xWidgets.h"
#include "d3_global.h"
#include "d3_xUtils.h"

#include "d3_zValue.ph"


/*****************************************************************************
  FUNCTION : d3_readZvalue

  PURPOSE  : read the current z-value form the widget
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void d3_readZvalue (Widget w, Widget pannel, caddr_t call_data)

{
    ui_actualZvalue = (short) ui_xIntFromAsciiWidget (z_valueWidget);
}


/*****************************************************************************
  FUNCTION : d3_incZvalue

  PURPOSE  : increments the z-value in the dialog widget
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void d3_incZvalue (Widget w, Widget pannel, caddr_t call_data)

{
    int n;
    Arg arg[10];
    char buf[10];

    ui_actualZvalue++;
    sprintf (buf, "%d", ui_actualZvalue);
    n = 0;
    XtSetArg (arg[n], XtNstring, buf); n++;
    XtSetValues (pannel, arg, (unsigned int) n);
}


/*****************************************************************************
  FUNCTION : d3_decZvalue

  PURPOSE  : decrements the z-value in the dialog widget
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void d3_decZvalue (Widget w, Widget pannel, caddr_t call_data)

{
    int n;
    Arg arg[10];
    char buf[10];

    ui_actualZvalue--;
    sprintf (buf, "%d", ui_actualZvalue);
    n = 0;
    XtSetArg (arg[n], XtNstring, buf); n++;
    XtSetValues (pannel, arg, (unsigned int) n);
}


/*****************************************************************************
  FUNCTION : d3_createZvaluePannel

  PURPOSE  : creates the z-value pannel
  RETURNS  : void
  NOTES    : 

  UPDATE   :
******************************************************************************/

void d3_createZvaluePannel (Widget parent)

{

  static XtActionsRec  ActionRec [] = {{"d3_readZvalue", (XtActionProc) d3_readZvalue}};
  XtTranslations Translation;

  Widget pannel, label, button, dummy, up, down;
  int n;
  Arg arg[10];
  char buf[20];

  n = 0;

  XtSetArg(arg[n], XtNwidth, 250); n++;
  XtSetArg(arg[n], XtNleft  , XtChainLeft); n++;
  XtSetArg(arg[n], XtNright , XtChainLeft); n++;
  XtSetArg(arg[n], XtNtop   , XtChainTop); n++;
  XtSetArg(arg[n], XtNbottom, XtChainTop); n++;
  pannel = XtCreateManagedWidget("pannel", formWidgetClass, parent, arg, (unsigned int) n);

  label = ui_xCreateLabelItem ("z-value ", pannel, d3_fontWidth * 7, NULL, NULL); 
  sprintf (buf, "%d", ui_actualZvalue);
  z_valueWidget = ui_xCreateDialogItem ("z_valueWidget", pannel, buf, d3_shortNumberWidth, label, NULL);

  button = d3_xCreateButtonItem ("ok", pannel, z_valueWidget, NULL);
  dummy = ui_xCreateLabelItem (" ", pannel, d3_fontWidth, button, NULL); 
  up = d3_xCreateButtonItem ("up", pannel, dummy, NULL);
  down = d3_xCreateButtonItem ("down", pannel, up, NULL);

  XtAddCallback (button, XtNcallback, (XtCallbackProc) d3_readZvalue, (caddr_t) z_valueWidget);
  XtAddCallback (up, XtNcallback, (XtCallbackProc) d3_incZvalue, (caddr_t) z_valueWidget);
  XtAddCallback (down, XtNcallback, (XtCallbackProc) d3_decZvalue, (caddr_t) z_valueWidget);

  XtAppAddActions (ui_appContext, ActionRec, XtNumber (ActionRec));
  Translation = XtParseTranslationTable ("<Key>Return: d3_readZvalue()");
  XtOverrideTranslations (z_valueWidget, Translation);

}

/* end of file */
/* lines: 186 */

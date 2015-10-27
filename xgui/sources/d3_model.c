/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_model.c,v $
  SHORTNAME      : model.c
  SNNS VERSION   : 4.2

  PURPOSE        : controls the model pannel
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <memory.h>

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
#include "d3_main.h"
#include "d3_zgraph.h"

#include "d3_model.ph"


/*****************************************************************************
  FUNCTION : setModelToggleState

  PURPOSE  : sets the toggles in the model pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void setModelToggleState (void)

{
    switch (temp_state.model_mode) {
        case wire_frame: ui_xSetToggleState (wireButton, TRUE);
                         ui_xSetToggleState (solidButton, FALSE);
                         break;
        case solid     : ui_xSetToggleState (wireButton, FALSE);
                         ui_xSetToggleState (solidButton, TRUE);
                         break;
      }
}


/*****************************************************************************
  FUNCTION : d3_closeModelPannel

  PURPOSE  : closes the model pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void d3_closeModelPannel (Widget w, Widget pannel, caddr_t call_data)

{
    XtDestroyWidget (pannel);
    if (memcmp (&d3_state, &temp_state, 
                sizeof (d3_state_type)) != 0) {
         memcpy (&d3_state, &temp_state, 
                 sizeof (d3_state_type)); 
         d3_drawNet ();
    }
}


/*****************************************************************************
  FUNCTION : setModelWire

  PURPOSE  : sets mode to draw a wireframe net
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void setModelWire (void)

{
    temp_state.model_mode = wire_frame;
    setModelToggleState (); 
    d3_freeZbuffer ();
}


/*****************************************************************************
  FUNCTION : setModelSolid

  PURPOSE  : sets mode to draw a solid net
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void setModelSolid (void)

{
    if (d3_initZbuffer ()) {
        temp_state.model_mode = solid;
        setModelToggleState ();
    }
}


/*****************************************************************************
  FUNCTION : d3_createModelPannel

  PURPOSE  : creates the model pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_createModelPannel (Widget w, Widget button, caddr_t call_data)

{
    int n;
    Arg arg[10];
    Position xPos, yPos;
    Dimension width, height;
    Widget modelPannel, border, pannel, doneButton;

    n = 0;
    XtSetArg (arg[n], XtNwidth, &width);   n++;
    XtSetArg (arg[n], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);
    XtTranslateCoords (button, (Position) (width / 2), (Position) (height / 2), 
                       &xPos, &yPos);

    n = 0;
    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;
    modelPannel = XtCreatePopupShell ("model", transientShellWidgetClass, 
                                      button, arg, (unsigned int) n);

    border = XtCreateManagedWidget("border", boxWidgetClass, 
                                   modelPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget("pannel", boxWidgetClass, 
                                   border, NULL, ZERO);

    wireButton = d3_xCreateToggleItem ("wire", pannel, NULL, NULL, NULL);
    solidButton = d3_xCreateToggleItem ("solid", pannel, NULL, NULL, wireButton);
    doneButton = d3_xCreateButtonItem ("done", border, NULL, NULL);

    XtAddCallback (wireButton, XtNcallback, (XtCallbackProc) setModelWire, NULL);
    XtAddCallback (solidButton, XtNcallback, (XtCallbackProc)setModelSolid, NULL);
    XtAddCallback (doneButton, XtNcallback, (XtCallbackProc)d3_closeModelPannel, 
                   (Widget) modelPannel);

    memcpy (&temp_state, &d3_state, sizeof (d3_state_type)); 

    setModelToggleState ();

    ui_checkWindowPosition(modelPannel);
    XtPopup (modelPannel, XtGrabExclusive);
    ui_xDontResizeWidget(modelPannel); 
}


/* end of file */
/* lines: 208 */

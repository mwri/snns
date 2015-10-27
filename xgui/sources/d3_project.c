/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_project.c,v $
  SHORTNAME      : project.c
  SNNS VERSION   : 4.2

  PURPOSE        : controls the project pannel
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:01 $

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

#include "glob_typ.h"
#include "ui.h"
#include "ui_xWidgets.h"
#include "d3_global.h"
#include "d3_xUtils.h"
#include "d3_main.h"

#include "d3_project.ph"


/*****************************************************************************
  FUNCTION : d3_closeProjectPannel

  PURPOSE  : close the project pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void d3_closeProjectPannel (Widget w, Widget pannel, caddr_t call_data)

{
    temp_state.viewpoint [0] = ui_xFloatFromAsciiWidget (viewpointXwidget);
    temp_state.viewpoint [1] = ui_xFloatFromAsciiWidget (viewpointYwidget);
    temp_state.viewpoint [2] = ui_xFloatFromAsciiWidget (viewpointZwidget);
    XtDestroyWidget (pannel);
    if (memcmp (&d3_state, &temp_state, 
                sizeof (d3_state_type)) != 0) {
         memcpy (&d3_state, &temp_state, 
                 sizeof (d3_state_type)); 
         d3_drawNet ();
    }
}


/*****************************************************************************
  FUNCTION : setProjectToggleState

  PURPOSE  : sets the toggles in the project pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void setProjectToggleState (void)

{
    switch (temp_state.projection_mode)
      {
        case parallel: ui_xSetToggleState (parallelButton, TRUE);
                       ui_xSetToggleState (centralButton, FALSE);
                       break;
        case central : ui_xSetToggleState (parallelButton, FALSE);
                       ui_xSetToggleState (centralButton, TRUE);
                       break;
      }
}


/*****************************************************************************
  FUNCTION : setParallelProjection

  PURPOSE  : sets mode to display the net with parallel projection
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void setParallelProjection (void)

{
    temp_state.projection_mode = parallel;
    setProjectToggleState ();
}


/*****************************************************************************
  FUNCTION : setCentralProjection

  PURPOSE  : sets mode to display the net with central projection
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void setCentralProjection (void)

{
    temp_state.projection_mode = central;
    setProjectToggleState ();
}


/*****************************************************************************
  FUNCTION : createProjectPannel

  PURPOSE  : creates the project pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_createProjectPannel (Widget w, Widget button, caddr_t call_data)

{
    int n;
    Arg arg[10];
    char buf[20];
    Position xPos, yPos;
    Dimension width, height;
    Widget projectPannel, pannel, border, label, done;

    n = 0;
    XtSetArg (arg[0], XtNwidth, &width); n++;
    XtSetArg (arg[1], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);

    XtTranslateCoords (button, (Position) (width / 2), (Position) (height / 2), &xPos, &yPos);

    n = 0;

    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;

    projectPannel = XtCreatePopupShell ("projection", transientShellWidgetClass, button, arg, (unsigned int) n);

    border = XtCreateManagedWidget("border", boxWidgetClass,
                                   projectPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget("pannel", formWidgetClass, 
                                   border, NULL, ZERO);

    parallelButton = d3_xCreateToggleItem ("parallel", pannel, NULL, NULL, NULL);
    centralButton = d3_xCreateToggleItem ("central", pannel, NULL,
                                   parallelButton, NULL);

    label = ui_xCreateLabelItem ("  Viewpoint  X", pannel, d3_fontWidth * 14,
                                   NULL, centralButton); 

    sprintf (buf, "%.4f", d3_state.viewpoint[0]);
    viewpointXwidget = ui_xCreateDialogItem ("viewpointX", pannel,
                                   buf, d3_numberWidth, label, centralButton);

    label = ui_xCreateLabelItem ("             Y", pannel,  d3_fontWidth * 14,
                                   NULL, label); 

    sprintf (buf, "%.4f", d3_state.viewpoint[1]);
    viewpointYwidget = ui_xCreateDialogItem ("viewpointY", pannel,
                                buf, d3_numberWidth, label, viewpointXwidget);

    label = ui_xCreateLabelItem ("             Z", pannel, d3_fontWidth * 14,
                                   NULL, label); 

    sprintf (buf, "%.4f", d3_state.viewpoint[2]);
    viewpointZwidget = ui_xCreateDialogItem ("viewpointX", pannel,
                                   buf, d3_numberWidth, label, viewpointYwidget);

    done = d3_xCreateButtonItem ("done", border, NULL, label);


    XtAddCallback (parallelButton, XtNcallback, (XtCallbackProc) setParallelProjection, NULL);
    XtAddCallback (centralButton, XtNcallback, (XtCallbackProc) setCentralProjection, NULL);
    XtAddCallback (done, XtNcallback, (XtCallbackProc) d3_closeProjectPannel,
                   (Widget) projectPannel);

    memcpy (&temp_state, &d3_state, sizeof (d3_state_type)); 

    setProjectToggleState ();
  
    ui_checkWindowPosition(projectPannel);
    XtPopup (projectPannel, XtGrabExclusive);
    ui_xDontResizeWidget(projectPannel); 

}

/* end of file */
/* lines: 233 */

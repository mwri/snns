/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_setup.c,v $
  SHORTNAME      : setup.c
  SNNS VERSION   : 4.2

  PURPOSE        : creates the setup pannel
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:02 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <math.h>
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

#include "d3_setup.ph"


/*****************************************************************************
  FUNCTION : d3_get_float_input

  PURPOSE  : reads a float value from a dialog widget
  RETURNS  : true if value has changed
  NOTES    :

  UPDATE   :
******************************************************************************/

static int d3_get_float_input (Widget w, float *f)

{
    float temp;

    temp = ui_xFloatFromAsciiWidget (w);
    if (temp != *f) {
         *f = temp;
         return (TRUE);
    } else
         return (FALSE);
}


/*****************************************************************************
  FUNCTION : d3_get_float_rad input

  PURPOSE  : reads a float value from a dialog widget and convert to radiant
  RETURNS  : true if value has changed
  NOTES    :

  UPDATE   :
******************************************************************************/

static int d3_get_float_rad_input (Widget w, float *f)

{    float temp;

    temp = rad (ui_xFloatFromAsciiWidget (w));
    if (temp != *f) {
         *f = temp;
         return (TRUE);
    } else
         return (FALSE);
}


/*****************************************************************************
  FUNCTION : d3_closeSetupPannel

  PURPOSE  : closes the setup pannel and reads all values
  RETURNS  : void
  NOTES    :

  UPDATE   : 8.3.93  Link Trigger
******************************************************************************/

static void d3_closeSetupPannel (Widget w, Widget pannel, caddr_t call_data)

{
    int f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11;

    d3_state.rot_step [0] = rad(ui_xFloatFromAsciiWidget (rotXstepWidget));
    d3_state.rot_step [1] = rad(ui_xFloatFromAsciiWidget (rotYstepWidget));
    d3_state.rot_step [2] = rad(ui_xFloatFromAsciiWidget (rotZstepWidget));

    d3_state.trans_step [0] = ui_xFloatFromAsciiWidget (transXstepWidget);
    d3_state.trans_step [1] = ui_xFloatFromAsciiWidget (transYstepWidget);
    d3_state.trans_step [2] = ui_xFloatFromAsciiWidget (transZstepWidget);

    d3_state.scale_step [0] = ui_xFloatFromAsciiWidget (scaleStepWidget);

    f1 = d3_get_float_rad_input (rotXinitWidget, &d3_state.rot_vec[0]);  
    f2 = d3_get_float_rad_input (rotYinitWidget, &d3_state.rot_vec[1]);  
    f3 = d3_get_float_rad_input (rotZinitWidget, &d3_state.rot_vec[2]);  

    f4 = d3_get_float_input (transXinitWidget, &d3_state.trans_vec[0]);  
    f5 = d3_get_float_input (transYinitWidget, &d3_state.trans_vec[1]);  
    f6 = d3_get_float_input (transZinitWidget, &d3_state.trans_vec[2]);  

    f7 = d3_get_float_input (scaleInitWidget, &d3_state.scale_vec[0]);  

    f8 = d3_get_float_input (aspectWidget, &d3_state.unit_aspect);  
    f9 = d3_get_float_input (linkWidget, &d3_state.link_scale);  

    f10 = d3_get_float_input (posTriggerWidget, &d3_state.pos_link_trigger);  
    f11 = d3_get_float_input (negTriggerWidget, &d3_state.neg_link_trigger);  

    XtDestroyWidget (pannel);

    if (f1 || f2 || f3 || f3 || f5 || f6 || f7 || f8 || f9 || f10 || f11)
         d3_drawNet ();
}


/*****************************************************************************
  FUNCTION : d3_createSetupPannel

  PURPOSE  : creates the setup pannel
  RETURNS  : void
  NOTES    :

  UPDATE   : 8.3.93  Link Trigger
******************************************************************************/

void d3_createSetupPannel (Widget w, Widget button, caddr_t call_data)

{
    int n;
    Arg arg[10];
    char buf[20];
    Position xPos, yPos;
    Dimension width, height;
    Widget setupPannel, pannel, done, init, step, rot_x, rot_y, rot_z;
    Widget trans_x, trans_y, trans_z, scale, aspect;
    Widget link, posTrigger, negTrigger, border;
    Widget dummy1, dummy2, dummy3, dummy4, dummy5, dummy6, dummy7, dummy8, dummy9;

    n = 0;
    XtSetArg (arg[n], XtNwidth, &width); n++;
    XtSetArg (arg[n], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);

    XtTranslateCoords (button, (Position) (width / 2), (Position) (height / 2),
                       &xPos, &yPos);
    n = 0;

    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;

    setupPannel = XtCreatePopupShell ("setup", transientShellWidgetClass,
                                      button, arg, (unsigned int) n);

    border = XtCreateManagedWidget("border", formWidgetClass,
                                   setupPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget("pannel", formWidgetClass, border, NULL, ZERO);

    /* Title Widgets */

    dummy1 = ui_xCreateLabelItem ("     ", pannel, 5 * d3_fontWidth, NULL, NULL); 
    dummy2 = ui_xCreateLabelItem ("   ", pannel, 3 * d3_fontWidth, dummy1, NULL);  
    init = ui_xCreateLabelItem ("  base   ", pannel, 9 * d3_fontWidth,
                                dummy2, NULL); 
    dummy3 = ui_xCreateLabelItem (" ", pannel, d3_fontWidth, init, NULL); 
    step = ui_xCreateLabelItem ("   step  ", pannel, 9 * d3_fontWidth,
                                 dummy3, NULL); 

    /* Rotation Widgets */

    dummy4 = ui_xCreateLabelItem ("rot  ", pannel, 5 * d3_fontWidth,
                                  NULL, dummy1); 
    rot_x = ui_xCreateLabelItem (" X ", pannel, 3 * d3_fontWidth, dummy4, dummy1); 
    sprintf (buf, "%.4f", fmod (deg (d3_state.rot_vec[0]), 360.0));
    rotXinitWidget = ui_xCreateDialogItem ("rotXinitWidget", pannel,
                                  buf, d3_numberWidth, rot_x, dummy1 );
    dummy5 = ui_xCreateLabelItem (" ", pannel, 
                                  d3_fontWidth, rotXinitWidget, dummy1);
    sprintf (buf, "%.4f", fmod (deg (d3_state.rot_step[0]), 360.0));
    rotXstepWidget = ui_xCreateDialogItem ("rotXstepWidget", pannel,
                                  buf, d3_numberWidth, dummy5, dummy1);
    dummy6 = ui_xCreateLabelItem ("    ", pannel, 5 * d3_fontWidth, NULL, dummy4); 
    rot_y = ui_xCreateLabelItem (" Y ", pannel, 3 * d3_fontWidth, dummy6, dummy4); 
    sprintf (buf, "%.4f", fmod (deg (d3_state.rot_vec[1]), 360.0));
    rotYinitWidget = ui_xCreateDialogItem ("rotYinitWidget", pannel,
                                  buf, d3_numberWidth, rot_y, dummy4);
    dummy7 = ui_xCreateLabelItem (" ", pannel, d3_fontWidth,
                                  rotYinitWidget, dummy4); 
    sprintf (buf, "%.4f", fmod (deg (d3_state.rot_step[1]), 360.0));
  rotYstepWidget = ui_xCreateDialogItem ("rotXstepWidget", pannel,
                                  buf, d3_numberWidth, dummy7, dummy4);

    dummy8 = ui_xCreateLabelItem ("    ", pannel, 5 * d3_fontWidth, NULL, dummy6); 
    rot_z = ui_xCreateLabelItem (" Z ", pannel, 3 * d3_fontWidth,
                                  dummy8, dummy6 ); 
    sprintf (buf, "%.4f", fmod (deg (d3_state.rot_vec[2]), 360.0));
    rotZinitWidget = ui_xCreateDialogItem ("rotZinitWidget", pannel,
                                  buf, d3_numberWidth, rot_z, dummy6);
    dummy9 = ui_xCreateLabelItem (" ", pannel, d3_fontWidth,
                                  rotZinitWidget, dummy6);
    sprintf (buf, "%.4f", fmod (deg (d3_state.rot_step[2]), 360.0));
    rotZstepWidget = ui_xCreateDialogItem ("rotZstepWidget", pannel,
                                  buf, d3_numberWidth, dummy9, dummy6);

/* Blank Line */

    dummy1 = ui_xCreateLabelItem ("     ", pannel, 5 * d3_fontWidth,
                                  NULL, dummy8); 
    dummy2 = ui_xCreateLabelItem ("   ", pannel, 3 * d3_fontWidth,
                                  dummy1, dummy8);  
    init = ui_xCreateLabelItem ("         ", pannel, 9 * d3_fontWidth,
                                  dummy2, dummy8); 
    dummy3 = ui_xCreateLabelItem (" ", pannel, d3_fontWidth, init, dummy8); 
    step = ui_xCreateLabelItem ("         ", pannel, 9 * d3_fontWidth,
                                  dummy3, dummy8); 

/* Translation Widgets */

    dummy4 = ui_xCreateLabelItem ("trans", pannel, 5 * d3_fontWidth,
                                  NULL, dummy1); 
    trans_x = ui_xCreateLabelItem (" X ", pannel, 3 * d3_fontWidth,
                                  dummy4, dummy1); 
    sprintf (buf, "%.4f", d3_state.trans_vec[0]);
    transXinitWidget = ui_xCreateDialogItem ("transXinitWidget", pannel,
                                  buf, d3_numberWidth, trans_x, dummy1);
    dummy5 = ui_xCreateLabelItem (" ", pannel, d3_fontWidth,
                                  transXinitWidget, dummy1);
    sprintf (buf, "%.4f", d3_state.trans_step[0]);
    transXstepWidget = ui_xCreateDialogItem ("transXstepWidget", pannel,
                                  buf, d3_numberWidth, dummy5, step);

    dummy6 = ui_xCreateLabelItem ("    ", pannel, 5 * d3_fontWidth, NULL, dummy4); 
    trans_y = ui_xCreateLabelItem (" Y ", pannel, 3 * d3_fontWidth,
                                  dummy6,dummy4); 
    sprintf (buf, "%.4f", d3_state.trans_vec[1]);
    transYinitWidget = ui_xCreateDialogItem ("transYinitWidget", pannel,
                                  buf, d3_numberWidth, trans_y, dummy4);
    dummy7 = ui_xCreateLabelItem (" ", pannel, d3_fontWidth,
                                  transXinitWidget, dummy4);  
    sprintf (buf, "%.4f", d3_state.trans_step[1]);
    transYstepWidget = ui_xCreateDialogItem ("transXstepWidget", pannel,
                                  buf, d3_numberWidth, dummy7, dummy4);

    dummy8 = ui_xCreateLabelItem ("    ", pannel, 5 * d3_fontWidth, NULL, dummy6); 
    trans_z = ui_xCreateLabelItem (" Z ", pannel, 3 * d3_fontWidth,
                                  dummy8, dummy6); 
    sprintf (buf, "%.4f", d3_state.trans_vec[2]);
    transZinitWidget = ui_xCreateDialogItem ("transZinitWidget", pannel,
                                  buf, d3_numberWidth, trans_z, dummy6);
    dummy9 = ui_xCreateLabelItem (" ", pannel, d3_fontWidth,
                                  transZinitWidget, dummy6);
    sprintf (buf, "%.4f", d3_state.trans_step[2]);
    transZstepWidget = ui_xCreateDialogItem ("transZstepWidget", pannel,
                                  buf, d3_numberWidth, dummy9, dummy6);

/* Blank Line */

    dummy1 = ui_xCreateLabelItem ("     ", pannel, 5 * d3_fontWidth,
                                  NULL, dummy8); 
    dummy2 = ui_xCreateLabelItem ("   ", pannel, 3 * d3_fontWidth,
                                  dummy1, dummy8);  
    init = ui_xCreateLabelItem ("         ", pannel, 9 * d3_fontWidth,
                                  dummy2, dummy8); 
    dummy3 = ui_xCreateLabelItem (" ", pannel, d3_fontWidth, init, dummy8); 
    step = ui_xCreateLabelItem ("         ", pannel, 9 * d3_fontWidth,
                                  dummy3, dummy8); 

/* Scale Widgets */

    dummy4 = ui_xCreateLabelItem ("scale", pannel, 5 * d3_fontWidth,
                                  NULL, dummy1); 
    scale = ui_xCreateLabelItem ("   ", pannel, 3 * d3_fontWidth,
                                  dummy4, dummy1); 
    sprintf (buf, "%.4f", d3_state.scale_vec[0]);
    scaleInitWidget = ui_xCreateDialogItem ("scaleInitWidget", pannel,
                                  buf, d3_numberWidth, scale, dummy1);
    dummy5 = ui_xCreateLabelItem (" ", pannel, 1 * d3_fontWidth,
                                  scaleInitWidget, dummy1);
    sprintf (buf, "%.4f", d3_state.scale_step[0]);
    scaleStepWidget = ui_xCreateDialogItem ("scaleStepWidget", pannel,
                                  buf, d3_numberWidth, dummy5, dummy1);

/* Blank Line */

    dummy1 = ui_xCreateLabelItem ("     ", pannel, 5 * d3_fontWidth,
                                  NULL, dummy4); 
    dummy2 = ui_xCreateLabelItem ("   ", pannel, 3 * d3_fontWidth,
                                  dummy1, dummy4);  
    init = ui_xCreateLabelItem ("         ", pannel, 9 * d3_fontWidth,
                                  dummy2, dummy4); 
    dummy3 = ui_xCreateLabelItem (" ", pannel, 1 * d3_fontWidth, init, dummy4); 
    step = ui_xCreateLabelItem ("         ", pannel, 9 * d3_fontWidth,
                                  dummy3, dummy4); 

/* Aspect Widget */

    aspect = ui_xCreateLabelItem ("units aspect", pannel, 16 * d3_fontWidth,
                                  NULL, dummy1); 
    sprintf (buf, "%.4f", d3_state.unit_aspect);
    aspectWidget = ui_xCreateDialogItem ("aspectWidget", pannel,
                                  buf, d3_numberWidth, aspect, dummy1);
/* Link Scale Widget */

    link = ui_xCreateLabelItem ("links scale", pannel, 16 * d3_fontWidth,
                                  NULL, aspect); 
    sprintf (buf, "%.4f", d3_state.link_scale);
    linkWidget = ui_xCreateDialogItem ("linkWidget", pannel,
                                  buf, d3_numberWidth, link, aspect);

/* Positive Link Trigger Widget */

    posTrigger = ui_xCreateLabelItem ("links positive", pannel, 16 * d3_fontWidth,
                                  NULL, link); 
    sprintf (buf, "%.4f", d3_state.pos_link_trigger);
    posTriggerWidget = ui_xCreateDialogItem ("posTriggerWidget", pannel,
                                  buf, d3_numberWidth, posTrigger, link);

/* Negative Link Trigger Widget */

    negTrigger = ui_xCreateLabelItem ("links negative", pannel, 16 * d3_fontWidth,
                                  NULL, posTrigger); 
    sprintf (buf, "%.4f", d3_state.neg_link_trigger);
    negTriggerWidget = ui_xCreateDialogItem ("negTriggerWidget", pannel,
                                  buf, d3_numberWidth, negTrigger, posTrigger);

    done = d3_xCreateButtonItem ("done", border, NULL, pannel);

    XtAddCallback(done, XtNcallback, (XtCallbackProc) d3_closeSetupPannel, (caddr_t) setupPannel);

    ui_checkWindowPosition(setupPannel);
    XtPopup (setupPannel, XtGrabExclusive);
    ui_xDontResizeWidget(setupPannel); 

}





/* end of file */
/* lines: 386 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_light.c,v $
  SHORTNAME      : light.c
  SNNS VERSION   : 4.2

  PURPOSE        : controls the light pannel
  NOTES          : 1.12.1991

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:53 $

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

#include "d3_light.ph"

/*****************************************************************************
  FUNCTION : d3_closeLightPannel 

  PURPOSE  : close the project pannel
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void d3_closeLightPannel (Widget w, Widget pannel, caddr_t call_data)
{
    temp_state.light.position[0] = ui_xFloatFromAsciiWidget (lightXwidget);
    temp_state.light.position[1] = ui_xFloatFromAsciiWidget (lightYwidget);
    temp_state.light.position[2] = ui_xFloatFromAsciiWidget (lightZwidget);

    temp_state.light.Ia = ui_xFloatFromAsciiWidget (lightIaWidget);
    temp_state.light.Ka = ui_xFloatFromAsciiWidget (lightKaWidget);

    temp_state.light.Ip = ui_xFloatFromAsciiWidget (lightIpWidget);
    temp_state.light.Kd = ui_xFloatFromAsciiWidget (lightKdWidget);
    XtDestroyWidget (pannel);
    if (memcmp (&d3_state, &temp_state, 
                sizeof (d3_state_type)) != 0) {
         memcpy (&d3_state, &temp_state, 
                 sizeof (d3_state_type)); 
         d3_drawNet ();
    }

}




/*****************************************************************************
  FUNCTION : d3_createLightPannel

  PURPOSE  : creates the light pannel
  RETURNS  : 
  NOTES    :

******************************************************************************/




void d3_createLightPannel (Widget w, Widget button, caddr_t call_data)
{
    int n;
    Arg arg[10];
    char buf[20];
    Position xPos, yPos;
    Dimension width, height;
    Widget lightPannel, border, pannel, labelX, labelY, labelZ, done;
    Widget labelA, labelI, labelR;

    n = 0;
    XtSetArg (arg[n], XtNwidth, &width); n++;
    XtSetArg (arg[n], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);

    XtTranslateCoords (button, (Position) (width / 2), (Position) (height / 2), 
                       &xPos, &yPos);


    n = 0;

    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;

    lightPannel = XtCreatePopupShell ("light", transientShellWidgetClass, 
                                      button, arg, (unsigned int) n);

    border = XtCreateManagedWidget("border", boxWidgetClass, 
                                   lightPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget("pannel", formWidgetClass, border, NULL, ZERO);

    labelX = ui_xCreateLabelItem ("  Light Source  X", pannel, d3_fontWidth * 17, 
                                  NULL, NULL); 

    sprintf (buf, "%.4f", d3_state.light.position[0]);
    lightXwidget = ui_xCreateDialogItem ("lightX", pannel, buf, d3_numberWidth,                                          labelX, NULL);
    labelY = ui_xCreateLabelItem ("                Y", pannel, d3_fontWidth * 17, 
                                  NULL, labelX); 

    sprintf (buf, "%.4f", d3_state.light.position[1]);
    lightYwidget = ui_xCreateDialogItem ("lightY", pannel, buf, d3_numberWidth, 
                                         labelY, labelX);

    labelZ = ui_xCreateLabelItem ("                Z", pannel, d3_fontWidth * 17, 
                                  NULL, labelY); 

    sprintf (buf, "%.4f", d3_state.light.position[2]);
    lightZwidget = ui_xCreateDialogItem ("lightZ", pannel, buf, d3_numberWidth, 
                                         labelZ, labelY);


    labelA = ui_xCreateLabelItem ("  Ambient Light", pannel, d3_fontWidth * 14,
                                  NULL, labelZ);

    labelI = ui_xCreateLabelItem ("      Intensity", pannel, d3_fontWidth * 15,
                                  NULL, labelA);
    sprintf (buf, "%.4f", d3_state.light.Ia);
    lightIaWidget = ui_xCreateDialogItem ("lightIa", pannel, buf, d3_numberWidth, 
                                          labelI, labelA);  

    labelR = ui_xCreateLabelItem ("     Reflection", pannel, d3_fontWidth * 15, 
                                  NULL, labelI);
    sprintf (buf, "%.4f", d3_state.light.Ka);
    lightKaWidget = ui_xCreateDialogItem ("lightKa", pannel, buf, d3_numberWidth, 
                                          labelR, labelI);  

    labelA = ui_xCreateLabelItem ("  Diffuse Light", pannel, d3_fontWidth * 14, 
                                  NULL, labelR);

    labelI = ui_xCreateLabelItem ("      Intensity", pannel, d3_fontWidth * 15, 
                                  NULL, labelA);
    sprintf (buf, "%.4f", d3_state.light.Ip);
    lightIpWidget = ui_xCreateDialogItem ("lightIp", pannel, buf, d3_numberWidth,
                                          labelI, labelA);  

    labelR = ui_xCreateLabelItem ("     Reflection", pannel, d3_fontWidth * 15,
                                  NULL, labelI);
    sprintf (buf, "%.4f", d3_state.light.Kd);
    lightKdWidget = ui_xCreateDialogItem ("lightKd", pannel, buf, d3_numberWidth,
                                           labelR, labelI);  

    done = d3_xCreateButtonItem ("done", border, NULL, lightIaWidget);

    XtAddCallback (done, XtNcallback, (XtCallbackProc) d3_closeLightPannel, (Widget) lightPannel);

    memcpy (&temp_state, &d3_state, sizeof (d3_state_type)); 


    ui_checkWindowPosition(lightPannel);
    XtPopup (lightPannel, XtGrabExclusive);

}


/* end of file */
/* lines: 200 */

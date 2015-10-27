/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_units.c,v $
  SHORTNAME      : units.c
  SNNS VERSION   : 4.2

  PURPOSE        : controls the unit pannel
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:04 $

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

#include "d3_units.ph"


/*****************************************************************************
  FUNCTION : d3_closeUnitPannel

  PURPOSE  : closes the unit pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void d3_closeUnitPannel (Widget w, Widget pannel, caddr_t call_data)

{
    XtDestroyWidget (pannel);
    if (memcmp (&d3_state.unit_mode, &temp_unit_mode, 
                sizeof (d3_unit_mode_type)) != 0) {
         memcpy (&d3_state.unit_mode, &temp_unit_mode, 
                 sizeof (d3_unit_mode_type)); 
         d3_drawNet ();
    }
}


/*****************************************************************************
  FUNCTION : d3_setUnitMode

  PURPOSE  : sets the toggles in the lower part of the unit pannel
  RETURNS  : void
  NOTES    : return if monochrome display and color selected

  UPDATE   :
******************************************************************************/

static void d3_setUnitMode (Widget w, int mode, caddr_t call_data)

{
    Boolean activflag, initflag, outflag, biasflag;
    Boolean nameflag, numflag, zflag, noneflag;

    if ((d3_state.color_mode == mono_mode) && (mode == select_color))
        return;
    activflag = initflag = outflag = biasflag = FALSE;
    nameflag = numflag = zflag = noneflag = FALSE;
    toggled_unit_mode = mode;
    switch (mode) {
        case activation_on  : activflag = TRUE;
                              break;    
        case init_act_on    : initflag = TRUE;
                              break;    
        case output_on      : outflag = TRUE;
                              break;    
        case bias_on        : biasflag = TRUE;
                              break;    
        case name_on        : nameflag = TRUE;
                              break;    
        case number_on      : numflag = TRUE;
                              break;    
        case zvalue_on      : zflag = TRUE;
                              break;    
        case nothing_on     : noneflag = TRUE;
                              break;    
    }
    ui_xSetToggleState (activation, activflag);    
    ui_xSetToggleState (initact, initflag);    
    ui_xSetToggleState (output, outflag);    
    ui_xSetToggleState (bias, biasflag);    
    ui_xSetToggleState (name, nameflag);    
    ui_xSetToggleState (number, numflag);    
    ui_xSetToggleState (zvalue, zflag);    
    ui_xSetToggleState (nothing, noneflag);    
    switch (selected_unit_mode) {
        case select_size      : temp_unit_mode.size = mode;
                                break;
        case select_color     : if (d3_state.color_mode == mono_mode)
                                   break;
                                temp_unit_mode.color = mode;
                                break;
        case select_toplabel  : temp_unit_mode.top_label = mode;
                                break;
        case select_bottlabel : temp_unit_mode.bottom_label = mode;
                                break;
    }
}


/*****************************************************************************
  FUNCTION : d3_selectUnitMode

  PURPOSE  : sets the toggles in the upper part of the unit pannel
  RETURNS  : void
  NOTES    : return if monochrome display and color selected

  UPDATE   :
******************************************************************************/

static void d3_selectUnitMode (Widget w, int selected, caddr_t call_data)

{
    Boolean sizeflag, colorflag, toplabelflag, bottlabelflag;

    if ((d3_state.color_mode == mono_mode) && (selected == select_color)) {
         ui_xSetToggleState (color, FALSE);    
         return;
    }
    sizeflag = colorflag = toplabelflag = bottlabelflag = FALSE;
    selected_unit_mode = selected;
    switch (selected) {
        case select_size      : sizeflag = TRUE;
                                d3_setUnitMode (w, temp_unit_mode.size, NULL);
                                break;
        case select_color     : colorflag = TRUE;
                                d3_setUnitMode (w, temp_unit_mode.color, NULL);
                                break;
        case select_toplabel  : toplabelflag = TRUE;
                                d3_setUnitMode (w, temp_unit_mode.top_label, NULL);
                                break;
        case select_bottlabel : bottlabelflag = TRUE;
                                d3_setUnitMode (w, temp_unit_mode.bottom_label, NULL);
                                break;
    }
    ui_xSetToggleState (size, sizeflag);    
    ui_xSetToggleState (color, colorflag);    
    ui_xSetToggleState (toplabel, toplabelflag);    
    ui_xSetToggleState (bottlabel, bottlabelflag);    
}


/*****************************************************************************
  FUNCTION : d3_createUnitPannel

  PURPOSE  : creates the unit pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_createUnitPannel (Widget w, Widget button, caddr_t call_data)

{
    int n;
    Arg arg[10];
    Position xPos, yPos;
    Dimension width, height;
    Widget unitPannel, border, pannel1, pannel2, done, dummy;
    static Boolean first_call = TRUE;

    n = 0;
    XtSetArg (arg[n], XtNwidth, &width); n++;
    XtSetArg (arg[n], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);

    XtTranslateCoords (button, 
       (Position) (width / 2), (Position) (height / 2), &xPos, &yPos);

    n = 0;

    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;

    unitPannel = XtCreatePopupShell ("units", transientShellWidgetClass, 
        button, arg, (unsigned int) n);

    border = XtCreateManagedWidget("border", boxWidgetClass,
        unitPannel, NULL, ZERO);

    pannel1 = XtCreateManagedWidget("pannel1", boxWidgetClass, border, NULL, ZERO);

    size = d3_xCreateToggleItem ("size", pannel1, NULL, NULL, NULL);
    color = d3_xCreateToggleItem ("color", pannel1, NULL, NULL, NULL);
    toplabel = d3_xCreateToggleItem ("toplabel", pannel1, NULL, NULL, NULL);
    bottlabel = d3_xCreateToggleItem ("bottlabel", pannel1, NULL, NULL, NULL);

    XtAddCallback(size, XtNcallback, (XtCallbackProc) d3_selectUnitMode, (caddr_t) select_size);
      XtAddCallback(color, XtNcallback, (XtCallbackProc) d3_selectUnitMode, (caddr_t) select_color);
    XtAddCallback(toplabel, XtNcallback, (XtCallbackProc) d3_selectUnitMode, (caddr_t) select_toplabel);
    XtAddCallback(bottlabel, XtNcallback, (XtCallbackProc) d3_selectUnitMode, (caddr_t) select_bottlabel);

    dummy = ui_xCreateLabelItem (" ", border, d3_fontWidth, NULL, NULL);

    pannel2 = XtCreateManagedWidget("pannel2", boxWidgetClass, border, NULL, ZERO);

    activation = d3_xCreateToggleItem ("activation", pannel2, NULL, NULL, NULL);
    initact = d3_xCreateToggleItem ("initact", pannel2, NULL, NULL, NULL);
    output = d3_xCreateToggleItem ("output", pannel2, NULL, NULL, NULL);
    bias = d3_xCreateToggleItem ("bias", pannel2, NULL, NULL, NULL);
    name = d3_xCreateToggleItem ("name", pannel2, NULL, NULL, NULL);
    number = d3_xCreateToggleItem ("number", pannel2, NULL, NULL, NULL);
    zvalue = d3_xCreateToggleItem ("zvalue", pannel2, NULL, NULL, NULL);
    nothing = d3_xCreateToggleItem ("nothing", pannel2, NULL, NULL, NULL);

    XtAddCallback(activation, XtNcallback, (XtCallbackProc) d3_setUnitMode, (caddr_t) activation_on);
    XtAddCallback(initact, XtNcallback, (XtCallbackProc) d3_setUnitMode, (caddr_t) init_act_on);
    XtAddCallback(output, XtNcallback, (XtCallbackProc) d3_setUnitMode, (caddr_t) output_on);
    XtAddCallback(bias, XtNcallback, (XtCallbackProc) d3_setUnitMode, (caddr_t) bias_on);
    XtAddCallback(name, XtNcallback, (XtCallbackProc) d3_setUnitMode, (caddr_t) name_on);
    XtAddCallback(number, XtNcallback, (XtCallbackProc) d3_setUnitMode, (caddr_t) number_on);
    XtAddCallback(zvalue, XtNcallback, (XtCallbackProc) d3_setUnitMode, (caddr_t) zvalue_on);
    XtAddCallback(nothing, XtNcallback, (XtCallbackProc) d3_setUnitMode, (caddr_t) nothing_on);

    done = d3_xCreateButtonItem ("done", border, NULL, NULL);
    XtAddCallback(done, XtNcallback, (XtCallbackProc) d3_closeUnitPannel, (caddr_t) unitPannel);

    memcpy (&temp_unit_mode, &d3_state.unit_mode, 
            sizeof (d3_unit_mode_type)); 

    if (first_call) {
         selected_unit_mode = select_size;
         toggled_unit_mode = temp_unit_mode.size;
         first_call = FALSE;
    }

    d3_selectUnitMode (pannel1, selected_unit_mode, NULL);
    d3_setUnitMode (pannel1, toggled_unit_mode, NULL);
  
    ui_checkWindowPosition(unitPannel);
    XtPopup (unitPannel, XtGrabExclusive);
    ui_xDontResizeWidget(unitPannel); 
}


/* end of file */
/* lines: 301 */

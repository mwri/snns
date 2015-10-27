/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_move.c,v $
  SHORTNAME      : move.c
  SNNS VERSION   : 4.2

  PURPOSE        : create the pannels for rotation, translation and scale
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:58 $

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

#include "glob_typ.h"
#include "ui.h"

#include "ui_xWidgets.h"
#include "d3_global.h"
#include "d3_xUtils.h"
#include "d3_main.h"

#include "d3_move.ph"


/*****************************************************************************
  FUNCTION : d3_transform

  PURPOSE  : sets the state vectors to a new movement
  RETURNS  : new state variable
  NOTES    : callback for all procedures in this module

  UPDATE   :
******************************************************************************/

static void d3_transform  (Widget w, int transformation, caddr_t call_data)

{
    int i;

    if (d3_freeze) 
        return;
    switch (transformation) {
        case ROT_X_LEFT    : d3_state.rot_vec [0] -= d3_state.rot_step [0];
                             break;
        case ROT_X_RIGHT   : d3_state.rot_vec [0] += d3_state.rot_step [0];
                             break;
        case ROT_Y_LEFT    : d3_state.rot_vec [1] -= d3_state.rot_step [1];
                             break;
        case ROT_Y_RIGHT   : d3_state.rot_vec [1] += d3_state.rot_step [1];
                             break;
        case ROT_Z_LEFT    : d3_state.rot_vec [2] -= d3_state.rot_step [2];
                             break;
        case ROT_Z_RIGHT   : d3_state.rot_vec [2] += d3_state.rot_step [2];
                              break;
        case TRANS_X_LEFT  : d3_state.trans_vec [0] -= d3_state.trans_step [0];
                             break;
        case TRANS_X_RIGHT : d3_state.trans_vec [0] += d3_state.trans_step [0];
                             break;
        case TRANS_Y_LEFT  : d3_state.trans_vec [1] -= d3_state.trans_step [1];
                             break;
        case TRANS_Y_RIGHT : d3_state.trans_vec [1] += d3_state.trans_step [1];
                             break;
        case TRANS_Z_LEFT  : d3_state.trans_vec [2] -= d3_state.trans_step [2];
                             break;
        case TRANS_Z_RIGHT : d3_state.trans_vec [2] += d3_state.trans_step [2];
                             break;
        case SCALE_PLUS    : for (i=0; i<3; i++) {
                                 d3_state.scale_vec [i] *= d3_state.scale_step [0]; 
			     }
                             break;
        case SCALE_MINUS   : for (i=0; i<3; i++) {
                                 d3_state.scale_vec [i] /= d3_state.scale_step [0]; 
			     }
                             break;
    }
    d3_drawNet ();
}


/*****************************************************************************
  FUNCTION : create_box_item

  PURPOSE  : creates the border box for the move pannels
  RETURNS  : the box widget
  NOTES    :

  UPDATE   :
******************************************************************************/

static void create_box_item (int height, int width, Widget parent, Widget left, Widget top, Widget *box)

{
    int n;
    Arg arg[20];
  
    n = 0;
    XtSetArg(arg[n], XtNx, border);                        n++;
    XtSetArg(arg[n], XtNy, border);                        n++;
    XtSetArg(arg[n], XtNfromVert , left);                  n++;
    XtSetArg(arg[n], XtNfromHoriz, top);                   n++;
    XtSetArg(arg[n], XtNheight, height);                   n++;
    XtSetArg(arg[n], XtNwidth, width);                     n++;
    XtSetArg(arg[n], XtNleft  , XtChainLeft);              n++;
    XtSetArg(arg[n], XtNright , XtChainRight);             n++;
    XtSetArg(arg[n], XtNtop   , XtChainTop);               n++;
    XtSetArg(arg[n], XtNbottom, XtChainBottom);            n++;
    *box = XtCreateManagedWidget("box", boxWidgetClass, parent, arg, (unsigned int) n);
}


/*****************************************************************************
  FUNCTION : create_scale_button

  PURPOSE  : creates a single scale button
  RETURNS  : scale widget
  NOTES    :

  UPDATE   :
******************************************************************************/

static void create_scale_button (char *label, Widget parent, int xsize, int ysize, Widget *button)

{
    int n;
    Arg arg[10];

    n = 0;
    XtSetArg(arg[n], XtNx, border);                        n++;
    XtSetArg(arg[n], XtNy, border);                        n++;
    XtSetArg(arg[n], XtNheight, ysize);                       n++;
    XtSetArg(arg[n], XtNwidth, xsize);                        n++;
    XtSetArg(arg[n], XtNlabel, label);                       n++;
    *button = XtCreateManagedWidget("button", commandWidgetClass, parent, arg, (unsigned int) n);

}


/*****************************************************************************
  FUNCTION : create_move_label

  PURPOSE  : creates a label for a single pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void create_move_label (char *label, Widget parent)

{
    Widget label_widget;
    int n;
    Arg arg[10];
    int xwdth = 78;

    n = 0;
    XtSetArg(arg[n], XtNborderWidth, 0); n++;
    XtSetArg(arg[n], XtNwidth, xwdth);                       n++;  
    XtSetArg(arg[n], XtNlabel, label);                       n++;

    XtSetArg(arg[n], XtNfromVert , NULL);  n++;
    XtSetArg(arg[n], XtNfromHoriz, NULL);  n++;

    XtSetArg(arg[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(arg[n], XtNright , XtChainRight); n++;
    XtSetArg(arg[n], XtNtop   , XtChainTop); n++;
    XtSetArg(arg[n], XtNbottom, XtChainBottom); n++;

    label_widget = XtCreateManagedWidget("label", labelWidgetClass, parent, arg, (unsigned int) n);

}


/*****************************************************************************
  FUNCTION : create_move_item

  PURPOSE  : creates a single entry for the trans and rotate pannels
  RETURNS  : the 'greater' and 'smaller' widgets
  NOTES    :

  UPDATE   :
******************************************************************************/

static void create_move_item (char *label, Widget parent, Widget *le_widget, Widget *ge_widget)

{
    Widget label_widget;
    int n;
    Arg arg[10];

    n = 0;
    XtSetArg(arg[n], XtNx, border);                        n++;
    XtSetArg(arg[n], XtNy, border);                        n++;
    XtSetArg(arg[n], XtNheight, button_size);              n++;
    XtSetArg(arg[n], XtNwidth, button_size);               n++;
    XtSetArg(arg[n], XtNlabel, "-");                       n++;
    *le_widget = XtCreateManagedWidget("le", commandWidgetClass, parent, arg, (unsigned int) n);

    n = 0;
    XtSetArg(arg[n], XtNx, border);                        n++;
    XtSetArg(arg[n], XtNy, border);                        n++;
    XtSetArg(arg[n], XtNheight, button_size );             n++;
    XtSetArg(arg[n], XtNwidth, 2 * button_size);           n++;
    XtSetArg(arg[n], XtNlabel, label);                     n++;
    label_widget = XtCreateManagedWidget("hot", labelWidgetClass, parent, arg, (unsigned int) n);

    n = 0;
    XtSetArg(arg[n], XtNx, border);                        n++;
    XtSetArg(arg[n], XtNy, border);                        n++;
    XtSetArg(arg[n], XtNheight, button_size);              n++;
    XtSetArg(arg[n], XtNwidth, button_size);               n++;
    XtSetArg(arg[n], XtNlabel, "+");                       n++;
    *ge_widget = XtCreateManagedWidget("ge", commandWidgetClass, parent, arg, (unsigned int) n);

}


/*****************************************************************************
  FUNCTION : create_rotatePannel

  PURPOSE  : creates the pannel and callbacks for the rotation
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void create_rotatePannel (Widget rotate_widget)

{
    Widget ge_widget, le_widget;

    create_move_label ("rotate", rotate_widget);

    create_move_item (" X ", rotate_widget, &le_widget, &ge_widget);
    XtAddCallback(le_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) ROT_X_LEFT);
    XtAddCallback(ge_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) ROT_X_RIGHT);
 
    create_move_item (" Y ", rotate_widget, &le_widget, &ge_widget);
    XtAddCallback(le_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) ROT_Y_LEFT);
    XtAddCallback(ge_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) ROT_Y_RIGHT);

    create_move_item (" Z ", rotate_widget, &le_widget, &ge_widget);
    XtAddCallback(le_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) ROT_Z_LEFT);
    XtAddCallback(ge_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) ROT_Z_RIGHT);

}  


/*****************************************************************************
  FUNCTION : create_transPannel

  PURPOSE  : creates the pannel and callbacks for the translation
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void create_transPannel (Widget trans_widget)

{
    Widget ge_widget, le_widget;

    create_move_label ("trans", trans_widget);

    create_move_item (" X ", trans_widget, &le_widget, &ge_widget);
    XtAddCallback(le_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) TRANS_X_LEFT);
    XtAddCallback(ge_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) TRANS_X_RIGHT);
 
    create_move_item (" Y ", trans_widget, &le_widget, &ge_widget);
    XtAddCallback(le_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) TRANS_Y_LEFT);
    XtAddCallback(ge_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) TRANS_Y_RIGHT);

    create_move_item (" Z ", trans_widget, &le_widget, &ge_widget);
    XtAddCallback(le_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) TRANS_Z_LEFT);
    XtAddCallback(ge_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) TRANS_Z_RIGHT);

}  


/*****************************************************************************
  FUNCTION : create_scalePannel

  PURPOSE  : creates the pannel for scale
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void create_scalePannel (Widget scale_widget)

{
    Widget ge_widget, le_widget;
    int xsize = 35, ysize = 60;

    create_move_label ("scale", scale_widget);

    create_scale_button ("-", scale_widget, xsize, ysize, &le_widget);
    create_scale_button ("+", scale_widget, xsize, ysize, &ge_widget);

    XtAddCallback(ge_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) SCALE_PLUS);
    XtAddCallback(le_widget, XtNcallback, (XtCallbackProc) d3_transform, (caddr_t) SCALE_MINUS);
}


/*****************************************************************************
  FUNCTION : d3_createMovePannel

  PURPOSE  : creates the pannels for for rotation, translation and scale
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_createMovePannel (Widget parent)

{
    Widget scale, rotate, trans;
    int height = 70;
    int width = 100;

    create_box_item (height, width, parent, NULL, NULL, &rotate);
    create_rotatePannel (rotate);

    create_box_item (height, width, parent, rotate, NULL, &trans);
    create_transPannel (trans);

    create_box_item (height, width, parent, trans, NULL, &scale);
    create_scalePannel (scale);
}  





/* end of file */
/* lines: 421 */

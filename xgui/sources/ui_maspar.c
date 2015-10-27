/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_maspar.c,v $
  SHORTNAME      : maspar.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         :
  DATE           :

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:38 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include "ui.h"

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>

#include "kr_ui.h"
#include "krui_typ.h"

#ifdef MASPAR_KERNEL

#include "ui_xWidgets.h"
#include "ui_textP.h"

#include "ui_maspar.ph"


/*****************************************************************************
  FUNCTION : connect_MasPar

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void connect_MasPar (void)

{ 
    err = krui_MasPar (MASPAR_CONNECT);
    if (err == KRERR_NO_ERROR) {
        ui_xSetToggleState (connect, TRUE);
        ui_xSetToggleState (disconnect, FALSE);
    } else {
        ui_tw_errorMessage (krui_error (err));
        ui_xSetToggleState (connect, FALSE);
    }
}


/*****************************************************************************
  FUNCTION : disconnect_MasPar

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void disconnect_MasPar (void)

{ 
    err = krui_MasPar (MASPAR_DISCONNECT);
    if (err == KRERR_NO_ERROR) {
        ui_xSetToggleState (disconnect, TRUE);
        ui_xSetToggleState (connect, FALSE);
    } else {
        ui_tw_errorMessage (krui_error (err));
        ui_xSetToggleState (disconnect, FALSE);
    }
}


/*****************************************************************************
  FUNCTION : standard_Net

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void standard_Net (void)

{
    err = krui_setSpecialNetworkType (NET_TYPE_GENERAL);
    if (err == KRERR_NO_ERROR) {
        ui_xSetToggleState (standard, TRUE);
        ui_xSetToggleState (feedforward, FALSE);
    } else {
        ui_tw_errorMessage (krui_error (err));
        ui_xSetToggleState (standard, FALSE);
    }
}


/*****************************************************************************
  FUNCTION : feedforward_Net

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void feedforward_Net (void)

{
    err = krui_setSpecialNetworkType (NET_TYPE_FF1);
    if (err == KRERR_NO_ERROR) {
        ui_xSetToggleState (standard, FALSE);
        ui_xSetToggleState (feedforward, TRUE);
    } else {
        ui_tw_errorMessage (krui_error (err));
        ui_xSetToggleState (feedforward, FALSE);
    }
}


/*****************************************************************************
  FUNCTION : closeMasparPannel

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void closeMasparPannel (Widget w, Widget pannel, caddr_t call_data)

{
    XtDestroyWidget (pannel);
}


/*****************************************************************************
  FUNCTION : ui_masparPannel

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void ui_masparPannel (Widget w, Widget button, caddr_t call_data)

{
    int n;
    Arg arg[10];
    Position xPos, yPos;
    Dimension width, height;
    Widget masparPannel, border, pannel, label0, label1, done;

    n = 0;
    XtSetArg (arg[n], XtNwidth, &width); n++;
    XtSetArg (arg[n], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);
    XtTranslateCoords (button, (Position) (width / 2), (Position) (height / 2), 
                       &xPos, &yPos);

    n = 0;
    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;
    masparPannel = XtCreatePopupShell ("MasPar", transientShellWidgetClass, 
                                       button, arg, (unsigned int) n);
    border = XtCreateManagedWidget ("border", boxWidgetClass, 
                                     masparPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget ("pannel", formWidgetClass, 
                                     border, NULL, ZERO);
/*
    label0 = ui_xCreateLabelItem ("MasPar          ", pannel, 
                                  16 * 8, NULL, NULL); 
    connect = ui_xCreateToggleItem ("connect", pannel, NULL, label0, NULL);
    disconnect = ui_xCreateToggleItem ("disconnect", pannel, NULL, connect, NULL);
*/
    label1 = ui_xCreateLabelItem ("Network Topology", pannel, 
                                  16 * 8, NULL, label0); 
    standard = ui_xCreateToggleItem ("standard", pannel, NULL, label1, label0);
    feedforward = ui_xCreateToggleItem ("feedforward", pannel, NULL, 
                                  standard, label0);

    done = ui_xCreateButtonItem ("done", border, NULL, label1);

    XtAddCallback (done, XtNcallback, (XtCallbackProc) closeMasparPannel, (Widget) masparPannel);
/*
    XtAddCallback (connect, XtNcallback, (XtCallbackProc) connect_MasPar, NULL);
    XtAddCallback (disconnect, XtNcallback, (XtCallbackProc) disconnect_MasPar, NULL);
*/
    XtAddCallback (standard, XtNcallback, (XtCallbackProc) standard_Net, NULL);
    XtAddCallback (feedforward, XtNcallback, (XtCallbackProc) feedforward_Net, NULL);

    ui_checkWindowPosition(masparPannel);
    XtPopup (masparPannel, XtGrabNone);
    ui_xDontResizeWidget(masparPannel); 
}


#endif /* MASPAR_KERNEL */


/* end of file */
/* lines: 161 */

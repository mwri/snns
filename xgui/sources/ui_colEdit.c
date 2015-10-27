/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_colEdit.c,v $
  SHORTNAME      : colEdit.c
  SNNS VERSION   : 4.2

  PURPOSE        : functions to manipulate the text, background and 
		   selection color
  NOTES          : uses the fixed palette defined in ui_color.c

  AUTHOR         : Ralf Huebner
  DATE           : 27.5.1992

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:13 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Toggle.h>

#include "ui.h"
#include "ui_xWidgets.h"
#include "ui_color.h"
#include "ui_mainP.h"
#include "ui_main.h"

#include "ui_colEdit.ph"


/*****************************************************************************
  FUNCTION : ui_cancelColorEditPannel

  PURPOSE  : callback if the cancel button is pressed
  RETURNS  : void
  NOTES    : closes the pannel

  UPDATE   :
******************************************************************************/

static void ui_cancelColorEditPannel (Widget w,Widget pannel,caddr_t call_data)

{
    XtDestroyWidget (pannel);
}


/*****************************************************************************
  FUNCTION : ui_closeColorEditPannel

  PURPOSE  : sets the selected colors in the current display
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void ui_closeColorEditPannel (Widget w, Widget pannel, caddr_t call_data)

{
    (ui_set_displayPtr->setup).backgroundColorIndex = ui_currentBackCol;
    (ui_set_displayPtr->setup).selectionColorIndex = ui_currentSelCol;
    (ui_set_displayPtr->setup).textColorIndex = ui_currentTextCol;
    XtDestroyWidget (pannel);
}


/*****************************************************************************
  FUNCTION : ui_getDisplayColors

  PURPOSE  : reads the color indices out of the display
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void ui_getDisplayColors (void)

{
    ui_currentBackCol = (ui_set_displayPtr->setup).backgroundColorIndex;
    ui_currentSelCol = (ui_set_displayPtr->setup).selectionColorIndex;
    ui_currentTextCol = (ui_set_displayPtr->setup).textColorIndex;
}


/*****************************************************************************
  FUNCTION : ui_xCreateColButtonItem

  PURPOSE  : creates a widget for a color button
  RETURNS  : Widget
  NOTES    : the color is defined in pixel

  UPDATE   :
******************************************************************************/

static Widget ui_xCreateColButtonItem (Widget parent, unsigned long pixel, 
	int xsize, int ysize, Widget left, Widget top)

{
    Cardinal n;
    Widget   w;
    Arg	     args[15];

    n = 0;
    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNfromHoriz, left);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    XtSetArg(args[n], XtNwidth, xsize); n++;
    XtSetArg(args[n], XtNheight, ysize); n++;
    XtSetArg(args[n], XtNbackground, pixel); n++;
    w = XtCreateManagedWidget("", commandWidgetClass, parent, args, n);
    return(w);
}


/*****************************************************************************
  FUNCTION : ui_createColWidgets

  PURPOSE  : creates the widgets for all colors
  RETURNS  : void
  NOTES    : 
 
  UPDATE   :
******************************************************************************/

static void ui_createColWidgets (Widget parent)

{
    Widget top, left;
    unsigned long pixel;
    int i, ix, iy;

    for (i=0; i<UI_MAX_EDIT_COLS; i++) {
        ix = i MOD UI_VERT_COLS;
        iy = i DIV UI_VERT_COLS;
        if (ix-1 < 0)
           left = NULL;
        else
           left = ui_colWidget[iy * UI_VERT_COLS + ix - 1];
           
        if (iy-1 < 0)
           top = NULL;
        else
           top = ui_colWidget[iy * UI_VERT_COLS - 1];
        pixel = ui_editColor[i];
        ui_colWidget[i] = ui_xCreateColButtonItem (parent, pixel,
                         colWidgetXsize, colWidgetYsize, left, top);
    }
}


/*****************************************************************************
  FUNCTION : ui_createTestPannel

  PURPOSE  : creates the test pannel for the selected colors
  RETURNS  :
  NOTES    : the sizes are absolute and depend on the
             vars in ui_redrawColorWindow

  UPDATE   :
******************************************************************************/

static Widget ui_createTestPannel (Widget parent, Widget left, Widget top)

{
    Cardinal n;
    Widget   w;
    Arg	     args[15];

    n = 0;
    XtSetArg(args[n], XtNheight, 72); n++; /* 58 */
    XtSetArg(args[n], XtNwidth, 234); n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNfromHoriz, left);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget("testPanel", boxWidgetClass, 
                                        parent, args, n);
    return(w);

}


/*****************************************************************************
  FUNCTION : ui_redrawColorWindow

  PURPOSE  : draws the test pannel
  RETURNS  :
  NOTES    : in the first line is the colorscale from red to green
             in the second are units in the selection color
             the background and text color are also shown

  UPDATE   :
******************************************************************************/

static void ui_redrawColorWindow (void)

{
    static int xsize = 16;
    static int ysize = 16;
    static int xspace = 32;
    static int yspace = 32;
    static int xoffset = 14;
    static int yoffset = 14;

    char buf[10];
    int i;

    XSetWindowBackground(ui_display, colWindow,
               ui_editColor[ui_currentBackCol]);
    XClearWindow (ui_display, colWindow);
    for (i=0; i<7; i++) {
        XSetForeground(ui_display, colGC,
                       ui_col_rangePixels[i*5]);
        XFillRectangle(ui_display, colWindow, colGC,
                       xoffset + i*xspace, yoffset, (unsigned int) xsize,                              (unsigned int) ysize);
        XSetForeground(ui_display, colGC, ui_editColor[ui_currentTextCol]);
        sprintf (buf, "u%d", i); 
        XDrawString(ui_display, colWindow, colGC,
		       xoffset + i*xspace, yoffset-2, buf, (int) strlen(buf));
    }
    for (i=0; i<7; i++) {
        XSetForeground(ui_display, colGC, ui_editColor[ui_currentSelCol]);
        XFillRectangle(ui_display, colWindow, colGC,
                       xoffset + i*xspace, yoffset+yspace, (unsigned int) xsize,                       (unsigned int) ysize);
        XSetForeground(ui_display, colGC, ui_editColor[ui_currentTextCol]);
        sprintf (buf, "u%d", i+7); 
        XDrawString(ui_display, colWindow, colGC,
		       xoffset + i*xspace, yoffset+yspace-2, buf, (int) strlen(buf));
    }

}


/*****************************************************************************
  FUNCTION : ui_colorUpdateProc

  PURPOSE  : selects which color to edit
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void ui_colorUpdateProc (Widget w, int color, caddr_t call_data)

{
    switch (currentSelection) {
        case UI_SELECT_BACK_COLOR: ui_currentBackCol = color;
                                   break;
        case UI_SELECT_SEL_COLOR : ui_currentSelCol = color;
                                   break;
        case UI_SELECT_TEXT_COLOR: ui_currentTextCol = color;
                                   break;
    }
    ui_redrawColorWindow();
}


/*****************************************************************************
  FUNCTION : ui_timerProc

  PURPOSE  : redraws the test window
  RETURNS  : void
  NOTES    : callback from a timer. called 1 sec after the edit pannel is
             popped up. this is nessecary, because a redraw function can't
             called directly

  UPDATE   :
******************************************************************************/

static void ui_timerProc (caddr_t client_data, XtIntervalId *timerId)

{
    ui_redrawColorWindow();
}


/*****************************************************************************
  FUNCTION : ui_colorWindowEventProc

  PURPOSE  : redraws the test window
  RETURNS  : void
  NOTES    : called by the event loop if redraw is nessecary

  UPDATE   :
******************************************************************************/

static void ui_colorWindowEventProc (Widget w, Display *display, XEvent *event)

{
    if ((event->type == Expose) AND (event->xexpose.count == 0))
        ui_redrawColorWindow();        
}




/*****************************************************************************
  FUNCTION : ui_selectColorItemProc

  PURPOSE  : toggles the specified button
  RETURNS  : void
  NOTES    :
******************************************************************************/

static void ui_selectColorItemProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &currentSelection, noOfColToggles, colorToggle);
}


/*****************************************************************************
  FUNCTION : ui_createColorEditPannel

  PURPOSE  : creates the color edit pannel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void ui_createColorEditPannel (Widget w, Widget button, caddr_t call_data)

{
    int n, i;
    Arg arg[10];
    Position xPos, yPos;
    Dimension width, height;
    Widget colEditPannel, pannel, border, done;
    Widget cancel, colframe, testPannel;

    n = 0;
    XtSetArg (arg[0], XtNwidth, &width); n++;
    XtSetArg (arg[1], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);

    XtTranslateCoords (button, (Position) (width / 2), (Position) (height / 2), &xPos, &yPos);

    n = 0;
    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;

    colEditPannel = XtCreatePopupShell ("color edit", transientShellWidgetClass, button, arg, (unsigned int) n);

    border = XtCreateManagedWidget("border", boxWidgetClass,
                                   colEditPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget("pannel", formWidgetClass, 
                                   border, NULL, ZERO);
    colframe = XtCreateManagedWidget("colframe", formWidgetClass, 
                                   pannel, NULL, ZERO);

    ui_createColWidgets (colframe);

    done = ui_xCreateButtonItem ("done", border, NULL, NULL);
    cancel = ui_xCreateButtonItem ("cancel", border, NULL, done);

    colorToggle[0] = ui_xCreateToggleItem ("text", pannel, NULL,
                     NULL, colframe);
    colorToggle[1] = ui_xCreateToggleItem ("background", pannel, NULL,
                     NULL, colorToggle[0]);
    
    colorToggle[2] = ui_xCreateToggleItem ("selection", pannel, NULL,
                     NULL, colorToggle[1]);
     
    ui_getDisplayColors();
 
    testPannel = ui_createTestPannel(pannel, colorToggle[0], colframe);
 
    XtAddCallback (done, XtNcallback, (XtCallbackProc) ui_closeColorEditPannel,
                   (caddr_t) colEditPannel);
    XtAddCallback (cancel, XtNcallback, (XtCallbackProc) ui_cancelColorEditPannel, (caddr_t) colEditPannel);

    XtAddCallback (colorToggle[0], XtNcallback, (XtCallbackProc) ui_selectColorItemProc, (caddr_t) UI_SELECT_TEXT_COLOR);
    XtAddCallback (colorToggle[1], XtNcallback, (XtCallbackProc) ui_selectColorItemProc, (caddr_t) UI_SELECT_BACK_COLOR);
    XtAddCallback (colorToggle[2], XtNcallback, (XtCallbackProc) ui_selectColorItemProc, (caddr_t) UI_SELECT_SEL_COLOR);

    for (i=0; i<UI_MAX_EDIT_COLS; i++)
        XtAddCallback (ui_colWidget[i], XtNcallback,
                       (XtCallbackProc) ui_colorUpdateProc,
		       (XtPointer) ((long)i));

    XtAddEventHandler (testPannel, ExposureMask, 
                       FALSE, (XtEventHandler) ui_colorWindowEventProc, ui_display);

    ui_checkWindowPosition(colEditPannel);
    XtPopup (colEditPannel, XtGrabNone);
    ui_xDontResizeWidget(colEditPannel); 

    ui_setToggles(currentSelection, &currentSelection,
                  noOfColToggles, colorToggle);

    colWindow = XtWindow(testPannel);

    colGC = XCreateGC (ui_display, colWindow, 0, 0);
    XSetFont (ui_display, colGC, ui_fontStruct->fid);
    
    XtAppAddTimeOut (ui_appContext, 1000L, (XtTimerCallbackProc) ui_timerProc, NULL);

}

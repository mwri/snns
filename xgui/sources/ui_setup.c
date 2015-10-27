/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_setup.c,v $
  SHORTNAME      : setup.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 26.6.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:43 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>

#include "ui.h"

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>

#include "ui_xWidgets.h"
#include "ui_setupP.h"
#include "ui_display.h"
#include "ui_colEdit.h"

#include "ui_main.h"  /* ui_labelFontWidth */
#include "ui_mainP.h"
#include "ui_color.h"
#include "ui_action.h"

#include "ui_setup.ph"


/*****************************************************************************
  FUNCTION : ui_set_initData 
  
  PURPOSE  : init setup data of specified display
  RETURNS  : void
  NOTES    :
  
  UPDATE   : 1.2.1990
******************************************************************************/

void ui_set_initData (struct Ui_DisplayType *displayPtr)

{
    (displayPtr->setup).showLinkFlg         = FALSE;
    (displayPtr->setup).showDirectionFlg    = FALSE;
    (displayPtr->setup).showWeightFlg       = FALSE;
    (displayPtr->setup).showValue           = UI_ACTIVATION;
    (displayPtr->setup).showTitle           = UI_NUMBER;
    (displayPtr->setup).showTitleFlg        = TRUE;
    (displayPtr->setup).showValueFlg        = TRUE;
    (displayPtr->setup).linkNegTrigger      = 0.0;
    (displayPtr->setup).linkPosTrigger      = 0.0;
    (displayPtr->setup).unitScaleFactor     = 1.0;
    (displayPtr->setup).linkScaleFactor     = 5.0;
    if (NOT ui_col_monochromeMode) {
       (displayPtr->setup).backgroundColorIndex = ui_initalBackgroundColorIndex; 
       (displayPtr->setup).selectionColorIndex = ui_initalSelectionColorIndex; 
       (displayPtr->setup).textColorIndex = ui_initalTextColorIndex;
    }
    else {
       (displayPtr->setup).backgroundColorIndex = UI_WHITE; 
       (displayPtr->setup).selectionColorIndex = UI_YELLOW; 
       (displayPtr->setup).textColorIndex = UI_BLACK;
    }
    sprintf((displayPtr->setup).siteName," ");
}


/*****************************************************************************
  FUNCTION : ui_xCreateSetupPanel

  PURPOSE  : creates a form widget for for all setup elements 
  RETURNS  : void
  NOTES    : some constants are used (see below) to get a relativ positioning
             of the items. This is VERY useful after changings.

  UPDATE   : 28.8.1991
*****************************************************************************/

void ui_xCreateSetupPanel (Widget parent, struct Ui_DisplayType *displayPtr)

{
    /* tabs for data fields and small buttons */

    Widget      topLabel, bottomLabel, lLabel, toggle, toggleLabel, scrollbar;
    Widget      button, mButton, menu, entry;
    char        buf[80];
    float       topOfThumb;
/*    
    int  fontWidth = 8;
*/
    int  titelWidth  = 15 * ui_labelFontWidth;
    int  noWidth     =  4 * ui_labelFontWidth;
    int  numberWidth =  8 * ui_labelFontWidth;
    int  nameWidth   = 15 * ui_labelFontWidth; 

/***************************************************************************/

    ui_setupPanel = 
	XtCreateManagedWidget("sPanel", formWidgetClass, parent, NULL, ZERO);

    /* which display is to setup ? */
    ui_set_displayPtr =  displayPtr;
	
	
/*****************************  OPTIONS  ***********************************/

    topLabel =
	ui_xCreateLabelItem("units top    :", ui_setupPanel, 
			   titelWidth, NULL, NULL);
	

    toggle =
	ui_xCreateToggleItem("on", ui_setupPanel,
			     NULL, topLabel, NULL);
    ui_xSetToggleState(toggle, (ui_set_displayPtr->setup).showTitleFlg);
    XtAddCallback(toggle, XtNcallback, (XtCallbackProc) ui_set_getSetupData, 
		  (caddr_t) UI_UNIT_TOP_GENERAL);

    mButton =
	ui_xCreateMenuButtonItem("show", ui_setupPanel, 
				 toggle, NULL);

    menu = 
	XtCreatePopupShell("menu", simpleMenuWidgetClass, mButton,
			   NULL, ZERO);

    entry =
	XtCreateManagedWidget("name", smeBSBObjectClass, menu,
			      NULL, ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_showProc, (caddr_t) UI_NAME);
    entry =
	XtCreateManagedWidget("number", smeBSBObjectClass, menu,
			      NULL, ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_showProc, (caddr_t) UI_NUMBER);
    entry =
	XtCreateManagedWidget("z-value", smeBSBObjectClass, menu,
			      NULL, ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_showProc, (caddr_t) UI_ZVALUE);

    entry =
        XtCreateManagedWidget("winner", smeBSBObjectClass, menu,
			      NULL, ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_showProc, (caddr_t) UI_WINNER);

    ui_showTopLabel =
	ui_xCreateLabelItem(" ", ui_setupPanel,
			    nameWidth, mButton,   NULL);

	ui_set_showProc(NULL, (ui_set_displayPtr->setup).showTitle, NULL); 
	/* initial value */


    bottomLabel =
	ui_xCreateLabelItem("      bottom :", ui_setupPanel, 
			   titelWidth, NULL, topLabel);

    toggle =
	ui_xCreateToggleItem("on", ui_setupPanel,
			    NULL, bottomLabel, topLabel);
    ui_xSetToggleState(toggle, (ui_set_displayPtr->setup).showValueFlg);
    XtAddCallback(toggle, XtNcallback, (XtCallbackProc) ui_set_getSetupData, 
		  (caddr_t) UI_UNIT_BOTTOM_GENERAL);

    mButton =
	ui_xCreateMenuButtonItem("show", ui_setupPanel,
				 toggle, topLabel);

    menu = 
	XtCreatePopupShell("menu", simpleMenuWidgetClass, mButton,
			   NULL, ZERO);

    entry =
	XtCreateManagedWidget("activation", smeBSBObjectClass, menu,
			      NULL, ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_showProc, 
	(caddr_t) UI_ACTIVATION);
    entry =
	XtCreateManagedWidget("initial act.", smeBSBObjectClass, menu,
			      NULL, ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_showProc, 
	(caddr_t) UI_INITIAL_ACTIVATION);
    entry =
	XtCreateManagedWidget("output", smeBSBObjectClass, menu,
			      NULL, ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_showProc, (caddr_t) UI_OUTPUT);
    entry =
	XtCreateManagedWidget("bias", smeBSBObjectClass, menu,
			      NULL, ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_showProc, (caddr_t) UI_BIAS);

    ui_showBottomLabel =
	ui_xCreateLabelItem(" ", ui_setupPanel,
			    nameWidth, mButton,   topLabel);
    ui_set_showProc(NULL, (ui_set_displayPtr->setup).showValue, NULL); 
	/* initial value */


    lLabel =
	ui_xCreateLabelItem("links        :", ui_setupPanel, 
			   titelWidth, NULL, bottomLabel);

    toggle =
	ui_xCreateToggleItem("on", ui_setupPanel,
			    NULL, lLabel, bottomLabel);
    ui_xSetToggleState(toggle, (ui_set_displayPtr->setup).showLinkFlg);
    XtAddCallback(toggle, XtNcallback, (XtCallbackProc) ui_set_getSetupData, 
		  (caddr_t) UI_LINK_GENERAL);
    toggle =
	ui_xCreateToggleItem("value",     ui_setupPanel,
			    NULL, toggle, bottomLabel);
    ui_xSetToggleState(toggle, (ui_set_displayPtr->setup).showWeightFlg);
    XtAddCallback(toggle, XtNcallback, (XtCallbackProc) ui_set_getSetupData, 
		  (caddr_t) UI_LINK_VALUE);

    toggle =
	ui_xCreateToggleItem("direction",  ui_setupPanel,
			    NULL, toggle, bottomLabel);
    ui_xSetToggleState(toggle, (ui_set_displayPtr->setup).showDirectionFlg);
    XtAddCallback(toggle, XtNcallback, (XtCallbackProc) ui_set_getSetupData, 
		  (caddr_t) UI_LINK_DIRECTION);


    button =
	ui_xCreateButtonItem("layers",    ui_setupPanel,
			     NULL, toggle);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_set_assignLayers, NULL);

    if (ui_col_colorDisplay) {
        mButton =
	    ui_xCreateButtonItem("color", ui_setupPanel,
				     button, toggle);
        XtAddCallback(mButton, XtNcallback, (XtCallbackProc) ui_createColorEditPannel, 
                       (Widget) mButton);
    } else {
        mButton =
	    ui_xCreateMenuButtonItem("color", ui_setupPanel,
				     button, toggle);
        menu = 
	    XtCreatePopupShell("menu", simpleMenuWidgetClass, mButton,
			   NULL, ZERO);

        entry =
	    XtCreateManagedWidget("black on white", smeBSBObjectClass, menu,
			      NULL, ZERO);
        XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_colorProc, 
		(caddr_t) UI_PALETTE_MONO);
        entry =
	    XtCreateManagedWidget("white on black", smeBSBObjectClass, menu,
			      NULL, ZERO);
        XtAddCallback(entry, XtNcallback, (XtCallbackProc) ui_set_colorProc, 
		(caddr_t) UI_PALETTE_MONO_INVERSE);
    }

/*    lLabel =
	ui_xCreateLabelItem("units positiv: ", ui_setupPanel, 
			    titelWidth, NULL, button);
    
    topOfThumb = (float) ((ui_set_displayPtr->setup).unitPosTrigger / ui_maxAct);
    scrollbar = 
	ui_xCreateScrollbarItem("trigger", ui_setupPanel, 200, topOfThumb, 
				topLabel, button);
    XtAddCallback(scrollbar, XtNjumpProc, (XtCallbackProc) ui_thumbed, (caddr_t) UNITS_POS);
    
    
    toggleLabel =
	ui_xCreateLabelItem("units negativ:", ui_setupPanel, 
			   titelWidth, NULL, scrollbar);

    topOfThumb = (ui_set_displayPtr->setup).unitNegTrigger / ui_minAct;
    scrollbar = 
	ui_xCreateScrollbarItem("trigger", ui_setupPanel, 200, topOfThumb,
			       topLabel, scrollbar);
    XtAddCallback(scrollbar, XtNjumpProc, (XtCallbackProc) ui_thumbed, (caddr_t) UNITS_NEG);
*/

    toggleLabel =
	ui_xCreateLabelItem("links positiv:", ui_setupPanel, 
			   titelWidth, NULL, button);

    topOfThumb = (float) (ui_set_displayPtr->setup).linkPosTrigger / ui_maxWeight;
    scrollbar = 
	ui_xCreateScrollbarItem("trigger", ui_setupPanel, 200, topOfThumb,
			       topLabel, button);
    XtAddCallback(scrollbar, XtNjumpProc, (XtCallbackProc) ui_thumbed, (caddr_t) UI_LINKS_POS);


    toggleLabel =
	ui_xCreateLabelItem("links negativ:", ui_setupPanel, 
			   titelWidth, NULL, scrollbar);

    topOfThumb = (float) (ui_set_displayPtr->setup).linkNegTrigger / ui_minWeight;
    scrollbar = 
	ui_xCreateScrollbarItem("trigger", ui_setupPanel, 200, topOfThumb, 
			       topLabel, scrollbar);
    XtAddCallback(scrollbar, XtNjumpProc, (XtCallbackProc) ui_thumbed, (caddr_t) UI_LINKS_NEG);


    if (ui_col_colorDisplay) {
	toggleLabel =
	    ui_xCreateLabelItem("links scale  :", ui_setupPanel, 
				titelWidth, NULL, scrollbar);
	
	topOfThumb = 
	    (float) (ui_set_displayPtr->setup).linkScaleFactor / 10.0;
	scrollbar = 
	    ui_xCreateScrollbarItem("trigger", ui_setupPanel, 200, topOfThumb,
				    topLabel, scrollbar);
	XtAddCallback(scrollbar, XtNjumpProc, (XtCallbackProc) ui_thumbed, 
		(caddr_t) UI_SCALE_LINKS);
    }

    toggleLabel =
	ui_xCreateLabelItem("units scale  :", ui_setupPanel, 
			   titelWidth, NULL, scrollbar);

    topOfThumb = (float) (ui_set_displayPtr->setup).unitScaleFactor / ui_maxAct;
    scrollbar = 
	ui_xCreateScrollbarItem("trigger", ui_setupPanel, 200, topOfThumb,
			       topLabel, scrollbar);
    XtAddCallback(scrollbar, XtNjumpProc, (XtCallbackProc) ui_thumbed, (caddr_t) UI_SCALE_FACTOR);


    toggleLabel =
	ui_xCreateLabelItem("grid width   :", ui_setupPanel, 
			   titelWidth, NULL, scrollbar);

    sprintf(buf,"%d", ui_set_displayPtr->gridSize);
    ui_set_gridWidthWidget =
	ui_xCreateDialogItem("gridWidth", ui_setupPanel, buf, noWidth,
			     topLabel, scrollbar);

    
    toggleLabel =
	ui_xCreateLabelItem("origin (grid):", ui_setupPanel, 
			   titelWidth, NULL, toggleLabel);

    sprintf(buf,"%d", ui_set_displayPtr->origin.x);
    ui_set_originXWidget =
	ui_xCreateDialogItem("origin", ui_setupPanel, buf, numberWidth,
			     topLabel, ui_set_gridWidthWidget);

    sprintf(buf,"%d", ui_set_displayPtr->origin.y);
    ui_set_originYWidget =
	ui_xCreateDialogItem("origin", ui_setupPanel, buf, numberWidth,
			     ui_set_originXWidget, ui_set_gridWidthWidget);

    toggleLabel =
	ui_xCreateLabelItem("subnet number:", ui_setupPanel, 
			   titelWidth, NULL, toggleLabel);

    sprintf(buf,"%d", ui_set_displayPtr->subNetNo);
    ui_set_subnetWidget =
	ui_xCreateDialogItem("subnetNo", ui_setupPanel, buf, numberWidth,
			     toggleLabel, ui_set_originXWidget);

    
    toggleLabel =
	ui_xCreateLabelItem("z-value      :", ui_setupPanel, 
			   titelWidth, NULL, toggleLabel);

    sprintf(buf,"%d", ui_actualZvalue);
    ui_set_zWidget =
	ui_xCreateDialogItem("zValue", ui_setupPanel, buf, numberWidth,
			     toggleLabel, ui_set_subnetWidget);


/***************************************************************************/

    XawFormDoLayout(ui_setupPanel, TRUE);
    ui_setupIsCreated = TRUE;
}









/* end of file */
/* lines: 403 */

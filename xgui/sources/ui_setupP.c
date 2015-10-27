/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_setupP.c,v $
  SHORTNAME      : setupP.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 23.7.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>

#include "ui.h"

#include <X11/Xaw/Toggle.h>

#include "ui_selection.h"
#include "ui_display.h"
#include "ui_xWidgets.h"
#include "ui_setup.h"
#include "ui_layer.h"
#include "ui_mainP.h"
#include "ui_action.h"

#include "ui_setupP.ph"


/*****************************************************************************
  FUNCTION : ui_set_showProc

  PURPOSE  : callback. Called by menu items of SHOW menu buttons
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_set_showProc(Widget w, int value, caddr_t call_data)

{   
    switch (value) {
      case UI_ACTIVATION:
	ui_xSetLabel(ui_showBottomLabel, "activation");
	(ui_set_displayPtr->setup).showValue = UI_ACTIVATION;
	break;
      case UI_INITIAL_ACTIVATION:
	ui_xSetLabel(ui_showBottomLabel, "initial act.");
	(ui_set_displayPtr->setup).showValue = UI_INITIAL_ACTIVATION;
	break;
      case UI_OUTPUT:
	ui_xSetLabel(ui_showBottomLabel, "output");
	(ui_set_displayPtr->setup).showValue = UI_OUTPUT;
	break;
      case UI_BIAS:
	ui_xSetLabel(ui_showBottomLabel, "bias");
	(ui_set_displayPtr->setup).showValue = UI_BIAS;
	break;
      case UI_NAME:
	ui_xSetLabel(ui_showTopLabel, "name");
	(ui_set_displayPtr->setup).showTitle = UI_NAME;
	break;
      case UI_NUMBER:
	ui_xSetLabel(ui_showTopLabel, "number");
	(ui_set_displayPtr->setup).showTitle = UI_NUMBER;
        break;
      case UI_ZVALUE:
	ui_xSetLabel(ui_showTopLabel, "z-value");
	(ui_set_displayPtr->setup).showTitle = UI_ZVALUE;
        break;
    case UI_WINNER:
	ui_xSetLabel(ui_showTopLabel, "winner");
	(ui_set_displayPtr->setup).showTitle = UI_WINNER;
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_set_getSetupData

  PURPOSE  : stores the current setting in the global variable ui_set_displayPtr->setup
  RETURNS  : void
  NOTES    : This is a callback of toggles.

  UPDATE   : 23.7.1990
******************************************************************************/

void ui_set_getSetupData(Widget widget, int toggleNumber, caddr_t call_data)

{
    /* typedef char Boolean !! */
 
    switch (toggleNumber) {

      case UI_UNIT_TOP_GENERAL:
	(ui_set_displayPtr->setup).showTitleFlg = ui_xGetToggleState(widget);
	break;
      case UI_UNIT_BOTTOM_GENERAL:
	(ui_set_displayPtr->setup).showValueFlg = ui_xGetToggleState(widget);
	break;
      case UI_LINK_GENERAL:
	(ui_set_displayPtr->setup).showLinkFlg = ui_xGetToggleState(widget);
	break;
      case UI_LINK_VALUE:
	(ui_set_displayPtr->setup).showWeightFlg = ui_xGetToggleState(widget);
	break;
      case UI_LINK_DIRECTION:
	(ui_set_displayPtr->setup).showDirectionFlg = ui_xGetToggleState(widget);
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_set_assignLayers

  PURPOSE  : popup the layer assignment window.
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_set_assignLayers(Widget w, caddr_t client_data, caddr_t call_data)

{
    Arg		args[5];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;

    /* Upper left corner will be in the center of the calling button */

    n = 0;
    XtSetArg(args[n], XtNwidth, &width); n++;
    XtSetArg(args[n], XtNheight, &height); n++;
    XtGetValues(w, args, n);
    XtTranslateCoords(w, (Position) (width / 2), (Position) (height / 2),
		      &x, &y);

    /* set start value for the buttons */
    ui_layerStartValue = ui_set_displayPtr->layers;
    ui_displayLayerPanel(x,y); /* absolute screen coordinates */
    /* assign return value */
    ui_set_displayPtr->layers = ui_layerReturnValue;
    ui_sel_checkList(ui_set_displayPtr, UI_GLOBAL);
}


#ifdef _UNUSED_FUNCTIONS_
/*****************************************************************************
  FUNCTION : ui_scrolled

  PURPOSE  : Prints the location of the pointer 
  RETURNS  : void
  NOTES    : value_ptr - the amount the bar has been scrolled.

  UPDATE   : 2.8.1990
******************************************************************************/

void ui_scrolled(Widget widget, caddr_t label_ptr, XtPointer value_ptr)

{
    int value = (int) value_ptr;
    char message[80];

    sprintf( message, " Scrolled by %d pixels", value);
    ui_printMessage(message);
}
#endif



/*****************************************************************************
  FUNCTION : ui_thumbed

  PURPOSE  : Prints the location of the thumb as a percentage of the height
  RETURNS  : void
  NOTES    : top_ptr - a pointer to a float containing the location of 
                       of the scrollbar's thumb

  UPDATE   : 2.8.1990
******************************************************************************/

void ui_thumbed(Widget widget, int trigger, XtPointer top_ptr)

{
    float top = *(float *) top_ptr;
    char  buf[80];

    switch (trigger) {
      case UI_LINKS_POS:
	sprintf(buf,"links pos: %8.5f = %.1f%%", 
		((ui_set_displayPtr->setup).linkPosTrigger = 
		 top * ui_maxWeight), (100.0 * top));
	break;
      case UI_LINKS_NEG:
	sprintf(buf,"links neg:  %8.5f = %.1f%%", 
		((ui_set_displayPtr->setup).linkNegTrigger = 
		 top * ui_minWeight), (100.0 * top));
	break;
     case UI_SCALE_FACTOR:
	(ui_set_displayPtr->setup).unitScaleFactor = top * ui_maxAct;
	if ((ui_set_displayPtr->setup).unitScaleFactor <= 0) {
	    (ui_set_displayPtr->setup).unitScaleFactor = 1;
	}
	sprintf(buf,"scale factor: %8.5f = %.1f%%", top * ui_maxAct, (100.0 * top));
	break;
     case UI_SCALE_LINKS:
	(ui_set_displayPtr->setup).linkScaleFactor = top * 10.0;
	if ((ui_set_displayPtr->setup).unitScaleFactor <= 0) {
	    (ui_set_displayPtr->setup).unitScaleFactor = 1;
	}
	sprintf(buf,"scale links: %8.5f = %.1f%%", top * 10.0, (100.0 * top));
	break;
    }
    ui_xSetLabel(ui_setupSliderMessage, buf);
}


/*****************************************************************************
  FUNCTION : ui_set_colorProc

  PURPOSE  : called by menu items of COLOR menu button in monochrome mode
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_set_colorProc(Widget w, int value, caddr_t call_data)

{    
    if (value == UI_PALETTE_MONO) {
        ui_set_displayPtr->setup.backgroundColorIndex = UI_WHITE;
        ui_set_displayPtr->setup.textColorIndex = UI_BLACK;
    } else {
        ui_set_displayPtr->setup.backgroundColorIndex = UI_BLACK;
        ui_set_displayPtr->setup.textColorIndex = UI_WHITE;
    }
}










/* end of file */
/* lines: 267 */

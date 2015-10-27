/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_netUpdate.c,v $
  SHORTNAME      : netUpdat.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <memory.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_netGraph.h"
#include "ui_xGraphic.h" 
#include "ui_xWidgets.h"    
/* ui_xGetDimensions */
#include "ui_display.h"
#include "ui_utilP.h"
#include "ui_displmap.h"
#include "ui_displwght.h"
#include "ui_mainP.h"
#include "ui_selection.h"
#include "ui_colEdit.h"

#include "glob_typ.h"
#include "d3_global.h"         
#include "d3_draw.h"
#include "d3_main.h"
#include "ui_color.h"

#include "ui_netUpdate.ph"


/*****************************************************************************
  FUNCTION : ui_net_drawLink

  PURPOSE  : draw a link
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_net_drawLink (struct Ui_DisplayType *displayPtr, FlagType scope, 
	int sourceNo, int targetNo, FlintType weight, Bool operation)

{
    if (scope == UI_GLOBAL) {
	struct Ui_DisplayType  *dPtr;
	dPtr = ui_displ_listPtr;
	while (dPtr != NULL) {
	    /* all the same subnet number ? */
	    if ((dPtr->subNetNo == krui_getUnitSubnetNo(sourceNo)) AND 
		(krui_getUnitSubnetNo(sourceNo) == krui_getUnitSubnetNo(targetNo)))
		if ((NOT dPtr->frozen) AND ui_utilIsSet(dPtr->flags,1)) /*!*/
		    ui_drawLink(dPtr, sourceNo, targetNo, weight, operation);
	    dPtr = dPtr->nextPtr;
	}
    } else
	/* all the same subnet number ? */
	if ((displayPtr != NULL) AND
            (displayPtr->subNetNo == krui_getUnitSubnetNo(sourceNo)) AND 
	    (krui_getUnitSubnetNo(sourceNo) == krui_getUnitSubnetNo(targetNo)))
	       ui_drawLink(displayPtr, sourceNo, targetNo, weight, operation);
}


/*****************************************************************************
  FUNCTION : ui_net_drawUnit

  PURPOSE  : draw a unit
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_net_drawUnit (struct Ui_DisplayType *displayPtr, FlagType scope, 
	int unitNo, Bool operation)

{
    if (scope == UI_GLOBAL) {
	struct Ui_DisplayType *dPtr;
	dPtr = ui_displ_listPtr;
	while (dPtr != NULL) {
	    /* all same subnet number like display? */
	    if ((dPtr->subNetNo == krui_getUnitSubnetNo(unitNo))) 
		if ((NOT dPtr->frozen) AND ui_utilIsSet(dPtr->flags,1)) /*!*/
		    ui_drawUnit(dPtr, unitNo, operation);
	    dPtr = dPtr->nextPtr;
	}
    } else
	/* all same subnet number like display? */
	if ((displayPtr->subNetNo == krui_getUnitSubnetNo(unitNo)))
	    ui_drawUnit(displayPtr, unitNo, operation);
}


/*****************************************************************************
  FUNCTION : ui_net_drawAllLinksToUnit

  PURPOSE  : This routine draws all links of the specified unit.
  RETURNS  : update of the graphic window  
  NOTES    : operation can be either UI_ERASE or UI_DRAW

  UPDATE   :
*****************************************************************************/

void ui_net_drawAllLinksToUnit (struct Ui_DisplayType *displayPtr, 
	FlagType scope, int unitNo, Bool operation)

{
    struct Ui_DisplayType  *dPtr;
    Bool             successful;
    FlintType        weight;
    int              predUnit;
    Bool             blocked;

    blocked = TRUE;
    if (scope == UI_GLOBAL) {
	dPtr = ui_displ_listPtr;
	while (dPtr != NULL) {
           if ((dPtr->setup).showLinkFlg)
              blocked = FALSE;
           dPtr = dPtr->nextPtr;
	}
    } else
        blocked = NOT ((displayPtr->setup).showLinkFlg);

    if (blocked)
        return;

    (void) krui_setCurrentUnit(unitNo);

    if (krui_getUnitInputType(unitNo) == SITES) {
	for (successful = krui_setFirstSite();
	     successful; successful = krui_setNextSite()) {
	    for(predUnit = krui_getFirstPredUnit(&weight);
		predUnit > 0;
		predUnit = krui_getNextPredUnit(&weight)) {
		ui_net_drawLink(displayPtr, scope, predUnit, unitNo, 
				weight, operation);
	    }
	}
    } else /* direct links */
      {
	for(predUnit = krui_getFirstPredUnit(&weight);
	    predUnit > 0;
	    predUnit = krui_getNextPredUnit(&weight)) {
	    ui_net_drawLink(displayPtr, scope, predUnit, unitNo, 
			    weight, operation);
	}
      }
}




/*****************************************************************************
  FUNCTION : ui_net_drawAllLinksFromUnit

  PURPOSE  : This routine draws all links of the specified unit.
  RETURNS  : update of the graphic window
  NOTES    : operation can be either UI_ERASE or UI_DRAW

  UPDATE   :
*****************************************************************************/

void ui_net_drawAllLinksFromUnit (struct Ui_DisplayType *displayPtr, 
	FlagType scope, int unitNo, FlagType operation)

{
    Bool               successful;
    FlintType          weight;
    int                targetNo;

    (void) krui_setCurrentUnit(unitNo);

    successful = 
	((targetNo = krui_getFirstSuccUnit(unitNo, &weight)) > 0);

    while (successful) {
	ui_net_drawLink(displayPtr, scope, unitNo, targetNo, 
			weight, operation);
	successful = 
	    ((targetNo = krui_getNextSuccUnit(&weight)) > 0);
    }
}


/*****************************************************************************
  FUNCTION : ui_net_updateWhole

  PURPOSE  : draws all units or links, depending of the parameter (see below)
  RETURNS  : update in the graphic window
  NOTES    : whatToDraw:
	     UI_DRAW_UNITS
	     UI_DRAW_LINKS

  UPDATE   :
*****************************************************************************/

void ui_net_updateWhole (struct Ui_DisplayType *displayPtr, FlagType scope, 
	int whatToDraw, Bool erase)

{
    Bool   successful;
    int    unitNo;

    /*!*/
    if (krui_getNoOfUnits() < 1)
	return;

    successful = 
	((unitNo = krui_getFirstUnit()) > 0);

    if (successful) {
	/* ui_can_setBusyCursor(); */
	
	while (successful) {
	    if (whatToDraw == UI_DRAW_LINKS)
		ui_net_drawAllLinksToUnit(displayPtr, scope, unitNo, erase);
	    if (whatToDraw == UI_DRAW_UNITS)
         	ui_net_drawUnit(displayPtr, scope, unitNo, erase);
	    successful = 
		((unitNo = krui_getNextUnit()) > 0);
	} /* end while */

	ui_printMessage("");    /* erase message */
	/* ui_can_setNormalCursor(); */

    } else 	    
	if (whatToDraw == UI_DRAW_UNITS) /* there are no units to draw ! */
	    ui_printMessage("The network is empty!");

    if (whatToDraw == UI_DRAW_UNITS) {
        d3_drawNet ();
	ui_displMapFromUpdate();
	ui_displWeightsFromUpdate();
    }
}


/*****************************************************************************
  FUNCTION : ui_net_doClearWindow

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

static void  ui_net_doClearWindow (struct Ui_DisplayType *displayPtr)

{
    struct PosType pixPos1;
    struct PosType pixPos2;
 	
    pixPos1.x = 0;
    pixPos1.y = 0;

    ui_xGetDimensions(displayPtr->widget, 
		      &(displayPtr->width), &(displayPtr->height));
    
    displayPtr->unitsInX = displayPtr->width  / displayPtr->gridSize;
    displayPtr->unitsInY = displayPtr->height / displayPtr->gridSize;

    pixPos2.x = displayPtr->width  - 1;
    pixPos2.y = displayPtr->height - 1;
	
    XSetFunction(ui_display, ui_gc, GXcopy);
    XSetForeground(ui_display, ui_gc, ui_backgroundColor);
    
    /* clear canvas */
    ui_xDeleteRect(ui_display, displayPtr->drawable, 
		   ui_gc, pixPos1, pixPos2); 
}


/*****************************************************************************
  FUNCTION : ui_net_clearWindow

  PURPOSE  : clear specified window
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_net_clearWindow (struct Ui_DisplayType *displayPtr, 
	FlagType scope)

{
    if (scope == UI_GLOBAL) {
	struct Ui_DisplayType  *dPtr;

	dPtr = ui_displ_listPtr;
	while (dPtr != NULL) {
	    if (NOT dPtr->frozen) ui_net_doClearWindow(dPtr);
	    dPtr = dPtr->nextPtr;
	}	    
    } else 
	ui_net_doClearWindow(displayPtr);
}


/*****************************************************************************
  FUNCTION : ui_net_completeRefresh

  PURPOSE  : new drawing of the entire network
  RETURNS  : update of the graphik window
  NOTES    : To draw the links takes a long time, if the network is big.

  UPDATE   :
*****************************************************************************/

void ui_net_completeRefresh (struct Ui_DisplayType *displayPtr, FlagType scope)

{
    ui_net_clearWindow(displayPtr, scope);
    ui_net_updateWhole(displayPtr, scope, UI_DRAW_LINKS, UI_DRAW);
    ui_net_updateWhole(displayPtr, scope, UI_DRAW_UNITS, UI_DRAW);
    ui_sel_reshowItems(displayPtr, scope);  /* redraw marker of selected items */
}





/* end of file */
/* lines: 420 */

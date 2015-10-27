/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_selection.c,v $
  SHORTNAME      : select.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          : UI_SELECTION_SIZE is no more in use! (defined in ui.h)

  AUTHOR         : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/03/13 16:31:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_utilP.h"
#include "ui_textP.h"
#include "ui_status.h"
#include "ui_display.h"
#include "ui_netUpdate.h"
#include "ui_confirmer.h"
#include "ui_xGraphic.h"
#include "ui_color.h"
#include "ui_event.h"
#include "ui_mainP.h"

#include "ui_selection.ph"


/*****************************************************************************
  FUNCTION : ui_sel_msgNumber

  PURPOSE  : show the number of selected items
  RETURNS  : alteration of the status message
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_sel_msgNumber (void)

{ 
    ui_stat_displayStatus(ui_gridPosMouse);
}


/*****************************************************************************
  FUNCTION : ui_sel_initSelectionList

  PURPOSE  : initialize the selection list and selection counters
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_sel_initSelectionList (void)

{
    ui_sel_numberOfItems          = 0;
    ui_sel_numberOfSelectedItems  = 0;
    ui_sel_listPtr                = NULL;
    ui_sel_freeListPtr            = NULL;
}


/*****************************************************************************
  FUNCTION : ui_sel_createItem

  PURPOSE  : allocate size for one item from the heap and set default values
  RETURNS  : pointer to that new item
  NOTES    :

  UPDATE   :
*****************************************************************************/

static struct SelectionType *ui_sel_createItem (void)

{
    struct SelectionType  *selPtr;

    selPtr = (struct SelectionType *) malloc(sizeof(struct SelectionType));
    if (selPtr != NULL) {
	(*selPtr).unitNo     = 0;
	(*selPtr).subNetNo   = 0;
	(*selPtr).gridPos.x  = 0;
	(*selPtr).gridPos.y  = 0;
	(*selPtr).gridPos.z  = 0;
	(*selPtr).flags      = UI_NO_FLAG;
	(*selPtr).nextPtr    = NULL;
	ui_sel_numberOfItems += 1;
    }
    return (selPtr);
}


/*****************************************************************************
  FUNCTION : ui_sel_lookForItem

  PURPOSE  : look for an selected item with this no
  RETURNS  : pointer to that item, if it was found, else NULL
  NOTES    : By calling this routine it is garanteed that never two items
             with the same position exist.

  UPDATE   :
*****************************************************************************/

struct SelectionType *ui_sel_lookForItem (int unitNo)

{
    struct SelectionType  *selPtr;

    selPtr = ui_sel_listPtr;
    while ((selPtr != NULL) AND ((*selPtr).unitNo != unitNo))
	selPtr = (*selPtr).nextPtr;
    
    if (selPtr != NULL)
	if ((*selPtr).unitNo != unitNo)
	    /* there is no item at this position, set signal */
	    selPtr = NULL;

    return (selPtr);
}


/*****************************************************************************
  FUNCTION : ui_sel_lookForSelectedItem

  PURPOSE  : search from the list item according to SelectionPtr until the end
             of the list or a selected item was found.
  RETURNS  : a pointer to this element or NULL, if no (more) elements are 
             found.
  NOTES    : 

  UPDATE   :
*****************************************************************************/

struct SelectionType *ui_sel_lookForSelectedItem 
				(struct SelectionType *SelectionPtr)

{
    struct SelectionType  *selPtr;

    selPtr = SelectionPtr;
    while ((selPtr != NULL) AND 
	   (ui_utilIsNotSet((*selPtr).flags, UI_SELECTED)))
	selPtr = (*selPtr).nextPtr;

    return (selPtr);
}


/*****************************************************************************
  FUNCTION : ui_sel_freeItem

  PURPOSE  : links the item in the free list
  RETURNS  : 
*****************************************************************************/

static void ui_sel_freeItem (struct SelectionType *selectionPtr)

{
    struct SelectionType  *selPtr;

    if (ui_sel_listPtr == selectionPtr) { /* is first element */
	ui_sel_listPtr        = selectionPtr->nextPtr;
	selectionPtr->nextPtr = ui_sel_freeListPtr;
	ui_sel_freeListPtr    = selectionPtr;
	ui_sel_numberOfSelectedItems -= 1;
    } else { /* second, third, ... element */
	selPtr = ui_sel_listPtr;
	while ((selPtr->nextPtr != selectionPtr) AND
	       (selPtr->nextPtr != NULL)) 
	    selPtr = selPtr->nextPtr;

	if (selPtr->nextPtr == selectionPtr) { /* item was found */
	    selPtr->nextPtr       = selectionPtr->nextPtr;
	    selectionPtr->nextPtr = ui_sel_freeListPtr;
	    ui_sel_freeListPtr    = selectionPtr;
	    ui_sel_numberOfSelectedItems -= 1;
	}
    }
}
	

/*****************************************************************************
  FUNCTION : ui_sel_getFreeItem

  PURPOSE  : looks in the free list for an item. If no one is found, 
             ui_sel_createItem() is called to create a new one.
  RETURNS  : a pointer to a free item or NULL if malloc() fails
*****************************************************************************/

static struct SelectionType *ui_sel_getFreeItem (void)

{
    struct SelectionType  *selPtr;

    if (ui_sel_freeListPtr == NULL) { /* no free item */
	selPtr             = ui_sel_createItem(); /* create a new item */
    } else {
	selPtr             = ui_sel_freeListPtr;
	ui_sel_freeListPtr = selPtr->nextPtr;
    }
    if (selPtr != NULL) {
	/* link it in the list as the first element */
	(*selPtr).nextPtr = ui_sel_listPtr;
	ui_sel_listPtr    = selPtr;
    }
    return (selPtr);
}
	

/*****************************************************************************
  FUNCTION : ui_sel_drawBoxes

  PURPOSE  : draw outline of all selected units
  RETURNS  :  void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_sel_drawBoxes (struct Ui_DisplayType *displayPtr, FlagType scope,
		struct PosType gridPos)

{   
    struct PosType  pos1, pos2, pixOffset;
    struct Ui_DisplayType *dPtr;
    struct SelectionType  *selPtr;

    if (scope == UI_GLOBAL) {
	for (dPtr = ui_displ_listPtr;
	     dPtr != NULL;
	     dPtr = dPtr ->nextPtr) 
	    ui_sel_drawBoxes(dPtr, UI_LOCAL, gridPos);
    } else { /* UI_LOCAL */

	pixOffset.x = displayPtr->gridSize * gridPos.x;
	pixOffset.y = displayPtr->gridSize * gridPos.y;
	
	XSetFunction(ui_display, ui_gc, GXinvert);
	XSetBackground(ui_display, ui_gc, 
		       ui_backgroundColor);
	XSetForeground(ui_display, ui_gc, 
		       ui_textColor);
	
	if (ui_col_colorDisplay)
	    XSetPlaneMask(ui_display, ui_gc, 
			  ui_textColor 
			  BIT_XOR 
			  ui_backgroundColor);
	
	for (selPtr = ui_sel_listPtr;
	     selPtr != NULL;
	     selPtr = selPtr->nextPtr) {
	    
	    if ((NOT displayPtr->frozen) AND 
		(displayPtr->subNetNo == ui_currentDisplay->subNetNo)) {
		
		pos1    = ui_utilGridToPix(displayPtr, selPtr->gridPos);
		pos1.x -= ((ui_selectionSize+2) / 2) - pixOffset.x;
		pos1.y -= ((ui_selectionSize+2) / 2) - pixOffset.y;
		pos2.x  = pos1.x + ui_selectionSize+2 - 1;
		pos2.y  = pos1.y + ui_selectionSize+2 - 1;
		ui_xDrawBox(ui_display, displayPtr->drawable, ui_gc, 
			    pos1, pos2);
	    }
	}
    }

    if (ui_col_colorDisplay)
	XSetPlaneMask(ui_display, ui_gc, AllPlanes);
}


/*****************************************************************************
  FUNCTION : ui_sel_showSelectedItem

  PURPOSE  : show the selection marker of a given unit
  RETURNS  : alteration of the graphic
  NOTES    : When performed a second time the selection marker will disappear,
             because the INVERT raster operation is used to draw the marker.

  UPDATE   :
*****************************************************************************/

static void ui_sel_showSelectedItem (struct Ui_DisplayType *displayPtr,
		FlagType scope, int unitNo, struct PosType gridPos)

{   
    struct PosType  pos1, pos2;
    struct Ui_DisplayType *dPtr;

    if (scope == UI_GLOBAL) {
	for (dPtr = ui_displ_listPtr;
	     dPtr != NULL;
	     dPtr = dPtr ->nextPtr)
	    ui_sel_showSelectedItem(dPtr, UI_LOCAL, unitNo, gridPos);
    } else {
	if ((NOT displayPtr->frozen) AND 
	    ui_isUnitVisibleInDisplay(displayPtr, unitNo)) { 
	    if (ui_col_colorDisplay) {
		XSetFunction(ui_display, ui_gc, GXcopy);
		XSetBackground(ui_display, ui_gc, 
			       ui_backgroundColor);
		XSetForeground(ui_display, ui_gc, 
			       ui_selectionColor);
	    } else {
		XSetFunction(ui_display, ui_gc, GXcopyInverted);
		XSetBackground(ui_display, ui_gc, 
			       ui_backgroundColor);
		XSetForeground(ui_display, ui_gc, 
			       ui_textColor);
	    }
	    
	    pos1    = ui_utilGridToPix(displayPtr, gridPos);
	    pos1.x -= (ui_selectionSize / 2);
	    pos1.y -= (ui_selectionSize / 2);
	    pos2.x  = pos1.x + ui_selectionSize - 1;
	    pos2.y  = pos1.y + ui_selectionSize - 1;
	    /* ui_xDeleteRect(ui_display, displayPtr->drawable, 
	       ui_gc, pos1, pos2); */
	    if (ui_col_colorDisplay)
		ui_xDeleteRect(ui_display, displayPtr->drawable, ui_gc,
			       pos1, pos2);
	    else {
		ui_xDeleteRect(ui_display, displayPtr->drawable, ui_gc,
			       pos1, pos2);
		XSetFunction(ui_display, ui_gc, GXcopy);
		ui_xDrawCrossBox(ui_display, displayPtr->drawable, 
				 ui_gc, pos1, pos2);
	    }
	}
    }
}


/*****************************************************************************
  FUNCTION : ui_sel_doActionSelect

  PURPOSE  : This routine does all the action which are needed to perform a
             selection of a given unit.
  RETURNS  : void
  NOTES    : The actions are:
             - set flag and position
	     - increase selection counters
	     - show the selection in the graohic window

  UPDATE   :
*****************************************************************************/

static void ui_sel_doActionSelect (struct Ui_DisplayType *displayPtr,
	FlagType scope, struct SelectionType *selPtr, int unitNo,
	struct PosType gridPos)

{
    (*selPtr).flags    = UI_SELECTED;
    (*selPtr).unitNo   = unitNo;
    (*selPtr).gridPos  = gridPos;
    (*selPtr).subNetNo = displayPtr->subNetNo;

    ui_sel_numberOfSelectedItems += 1;
   
    ui_sel_showSelectedItem(displayPtr, scope, unitNo, gridPos);
}


/*****************************************************************************
  FUNCTION : ui_sel_selectedOne

  PURPOSE  : select a unit at the given position
  RETURNS  : alteration of the selection list
  NOTES    : If the unit is selected yet, the unit will be still selected
             after calling this routine. If there is no unit, nothing will
	     be made.

  UPDATE   :
*****************************************************************************/

void ui_sel_selectOne (struct Ui_DisplayType *displayPtr, FlagType scope, 
		int unitNo, struct PosType gridPos)

{
    struct SelectionType  *selPtr;
    char   buf[80];

    selPtr = ui_sel_lookForItem(unitNo);  /* already an item ? */	
    if (selPtr != NULL) { /* there is already an item */
	if (ui_utilIsNotSet((*selPtr).flags, UI_SELECTED)) {
	    /* select only, if still not selected; else do nothing */
	    if (ui_isUnitVisibleInAnyDisplay(selPtr->unitNo)) {
		ui_sel_doActionSelect(displayPtr, scope, selPtr, unitNo, gridPos);
	    } else {
		ui_sel_freeItem(selPtr);
	    }
	}
    } else { /* no item at gridPos found */
	if (ui_isUnitVisibleInAnyDisplay(unitNo)) {
	    if ((selPtr = ui_sel_getFreeItem()) == NULL) {
		sprintf(buf,"No memory left to select unit %d!",unitNo);
		ui_confirmOk(buf);
	    } else
		ui_sel_doActionSelect(displayPtr, scope, selPtr, 
				      unitNo, gridPos);
	}
    }
}


/*****************************************************************************
  FUNCTION : ui_sel_unselectedOne

  PURPOSE  : unselect a unit at the given position
  RETURNS  : alteration of the selection list
  NOTES    : see ui_sel_selectOne()

  UPDATE   : 
*****************************************************************************/ 

void ui_sel_unselectOne (struct Ui_DisplayType *displayPtr, FlagType scope,
		int unitNo, struct PosType gridPos)

{
    struct SelectionType  *selPtr;

    selPtr = ui_sel_lookForItem(unitNo); /* Is there an item ? */
    if (selPtr != NULL) { /* an item with this position exists */
	if (ui_utilIsSet((*selPtr).flags, UI_SELECTED)) {
	    /* And is still selected */
	    (*selPtr).flags = UI_NO_FLAG;
	    ui_sel_freeItem(selPtr);
	    ui_net_drawUnit(displayPtr, scope, unitNo, UI_DRAW);
	}
    }
}


/*****************************************************************************
  FUNCTION : ui_sel_selectRect

  PURPOSE  : select all units within a rectangle area determined by the upper
             left and bottom right positions (inclusive border!).
  RETURNS  : alteration of the selection list
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_sel_selectRect (struct Ui_DisplayType *displayPtr, FlagType scope,
		struct PosType upperLeft, struct PosType lowerRight)

{
    struct PosType  gridPos;
    int             unitNo;

    for (unitNo = krui_getFirstUnit();
	 unitNo > 0;
	 unitNo = krui_getNextUnit()) {
	krui_getUnitPosition(unitNo, &gridPos);
	if ((displayPtr->subNetNo == krui_getUnitSubnetNo(unitNo)) AND
	    (gridPos.x >= upperLeft.x) AND
	    (gridPos.x <= lowerRight.x) AND
	    (gridPos.y >= upperLeft.y) AND
	    (gridPos.y <= lowerRight.y))
	    /* the unit is in this area */
	    ui_sel_selectOne(displayPtr, scope, unitNo, gridPos);
    }
    ui_sel_msgNumber(); 
}


/*****************************************************************************
  FUNCTION : ui_sel_unselectRect

  PURPOSE  : unselect all units within a rectangle area determined by the upper
             left and bottom right positions (inclusive border!).
  RETURNS  : alteration of the selection list
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_sel_unselectRect (struct Ui_DisplayType *displayPtr, FlagType scope, 
		struct PosType upperLeft, struct PosType lowerRight)

{
    struct PosType  gridPos;
    int             unitNo;

    for (unitNo = krui_getFirstUnit();
	 unitNo > 0;
	 unitNo = krui_getNextUnit()) {
	krui_getUnitPosition(unitNo, &gridPos);
	if ((displayPtr->subNetNo == krui_getUnitSubnetNo(unitNo)) AND
	    (gridPos.x >= upperLeft.x) AND
	    (gridPos.x <= lowerRight.x) AND
	    (gridPos.y >= upperLeft.y) AND
	    (gridPos.y <= lowerRight.y))
	    /* the unit is in this area */
	    ui_sel_unselectOne(displayPtr, scope, unitNo, gridPos);
    }
    ui_sel_msgNumber(); 
}


/*****************************************************************************
  FUNCTION : ui_sel_reshowItems

  PURPOSE  : show (and erase!) the selection markers of all selected units.
  RETURNS  : alteration of the graphic
  NOTES    : When called a second time, this routine erases all markers, cause
             they were drawn with the XOR raster operation.

  UPDATE   :
*****************************************************************************/

void ui_sel_reshowItems (struct Ui_DisplayType *displayPtr, FlagType scope)

{
    struct SelectionType  *selPtr;

    selPtr = ui_sel_listPtr;
    /* reshow all selected items */
    while ((selPtr != NULL)) {
	if (ui_utilIsSet((*selPtr).flags,UI_SELECTED))
	    ui_sel_showSelectedItem(displayPtr, scope, 
				    (*selPtr).unitNo, (*selPtr).gridPos);
	selPtr = (*selPtr).nextPtr;
    }
    ui_sel_msgNumber(); 
}


/*****************************************************************************
  FUNCTION : ui_sel_checkList

  PURPOSE  : checks, whether selected units should be unselected because of
             1) layers are changed (unit is unvisible now)
	     2) no display with the specified subnetNo exist.
  RETURNS  : alteration of the graphic
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_sel_checkList (struct Ui_DisplayType *displayPtr, FlagType scope)

{
    struct SelectionType  *selPtr, *tmpPtr;
    struct Ui_DisplayType *dPtr;

    dPtr = ui_displ_listPtr;
    while (dPtr != NULL) {
	selPtr = ui_sel_listPtr;
	while ((selPtr != NULL)) {
	    if (NOT ui_isUnitVisibleInDisplay(dPtr, selPtr->unitNo)) {
		/* now this selected unit is unvisible !! */
		/* hide selection marker */
		ui_sel_showSelectedItem(dPtr, UI_LOCAL, 
					selPtr->unitNo, selPtr->gridPos);
		tmpPtr = selPtr;
		selPtr = selPtr->nextPtr;
		ui_sel_freeItem(tmpPtr);
	    } else
		selPtr = selPtr->nextPtr;
	}
	dPtr = dPtr->nextPtr;
    }
    ui_sel_msgNumber();
}


/*****************************************************************************
  FUNCTION : ui_sel_unselectAll

  PURPOSE  : unselect all units.
  RETURNS  : alteration of the selection list and the graphic
  NOTES    : The markers must be visible when calling this routine!

  UPDATE   :
*****************************************************************************/

void ui_sel_unselectAll (struct Ui_DisplayType *displayPtr, FlagType scope)

{
    while (ui_sel_listPtr != NULL) {
	if (ui_utilIsSet(ui_sel_listPtr->flags, UI_SELECTED)) {
	    ui_sel_unselectOne(displayPtr, scope, ui_sel_listPtr->unitNo,
			       ui_sel_listPtr->gridPos);
	} else
	    ui_sel_freeItem(ui_sel_listPtr);
    }
    ui_sel_msgNumber(); 
}
	

/*****************************************************************************
  FUNCTION : ui_sel_resetList

  PURPOSE  : reset of the selection list for the specified mode (UI_EDITOR
             or UI_SIMULATOR). No graphic update is made, so all markers must
	     be unvisible or the network will be dran with 
	     ui_net_completeRefresh().
  RETURNS  : alteration of the selection list
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_sel_resetList (void)

{
    while (ui_sel_listPtr != NULL) {
	ui_sel_listPtr->flags = UI_NO_FLAG;
	ui_sel_freeItem(ui_sel_listPtr);
    }
    ui_sel_msgNumber(); 
}









/* end of file */
/* lines: 678 */

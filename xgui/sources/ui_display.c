/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_display.c,v $
  SHORTNAME      : display.c
  SNNS VERSION   : 4.2

  PURPOSE        : contains routines to handle the list of all network displays
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 26.7.1990

  CHANGED BY     :

  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:19 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "ui_utilP.h"
#include "ui_funcdispl.h"
#include "ui_netUpdate.h"
#include "ui_setupP.h"
#include "ui_setup.h"
#include "ui_mainP.h"

#include "ui_display.ph"   /* private include-file */


/*****************************************************************************
  FUNCTION : ui_displ_createItem

  PURPOSE  : allocate size for one item from the heap and set default values
  RETURNS  : pointer to that new item
  NOTES	   :

  UPDATES  :
*****************************************************************************/

static struct Ui_DisplayType *ui_displ_createItem(void)

{
    struct Ui_DisplayType  *displPtr;

    displPtr = (struct Ui_DisplayType *) malloc(sizeof(struct Ui_DisplayType));
    if (displPtr != NULL) {
	(*displPtr).x            = 1;
	(*displPtr).y            = 1;
	(*displPtr).origin.x     = 0;
	(*displPtr).origin.y     = 0;
	(*displPtr).width        = 450;
	(*displPtr).height       = 300;
	(*displPtr).gridSize     = 37;
	(*displPtr).unitsInX     = (*displPtr).width  / (*displPtr).gridSize;
	(*displPtr).unitsInY     = (*displPtr).height / (*displPtr).gridSize;
	(*displPtr).frozen       = FALSE;
	(*displPtr).subNetNo     = 0;
	(*displPtr).layers       = 1;
	ui_set_initData(displPtr);
	(*displPtr).updateType   = UI_INTELLIGENT;
	(*displPtr).flags        = UI_CLOSED + 1; /*!*/ /* mapped */
	(*displPtr).nextPtr      = NULL;
	(*displPtr).displayNo    = ui_displ_numberOfItems;
    }
    return (displPtr);
}


/*****************************************************************************
  FUNCTION : ui_displ_initDisplayList

  PURPOSE  : initialize the Display list and Display counters
  RETURNS  : void
  NOTES	   :

  UPDATES  :
*****************************************************************************/

void ui_displ_initDisplayList(void)

{
    ui_displ_numberOfItems          = 1;
    ui_displ_listPtr                = NULL;
    ui_displ_freeListPtr            = ui_displ_createItem();
    ui_currentDisplay               = ui_displ_listPtr;
    ui_displ_numberOfItems          = 0;
}


/*****************************************************************************
  FUNCTION : ui_displ_resetList  

  PURPOSE  : links all items in the free list and closes the displays 
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_displ_resetList(void)

{
    struct Ui_DisplayType  *displPtr;

    while (ui_displ_listPtr != NULL) {
	displPtr = ui_displ_listPtr;
	ui_displayDone(NULL, displPtr, NULL);
	/* ui_displ_freeItem() is performed in ui_displayDone() ! */
    }
}

	
/*****************************************************************************
  FUNCTION : ui_displ_freeItem

  PURPOSE  : links the item in the free list
  RETURNS  : void
  NOTES    :

  UPDATES  :
*****************************************************************************/

void ui_displ_freeItem(struct Ui_DisplayType *displayPtr)

{
    struct Ui_DisplayType  *displPtr;
 
    if (ui_displ_listPtr == displayPtr) { /* is first element */
	ui_displ_listPtr     = displayPtr->nextPtr;
	displayPtr->nextPtr  = ui_displ_freeListPtr;
	ui_displ_freeListPtr = displayPtr;
	ui_displ_numberOfItems -= 1;
    } else { /* second, third, ... element */
	displPtr = ui_displ_listPtr;
	while ((displPtr->nextPtr != displayPtr) AND
	       (displPtr->nextPtr != NULL)) 
	    displPtr = displPtr->nextPtr;

	if (displPtr->nextPtr == displayPtr) { /* item was found */
	    displPtr->nextPtr    = displayPtr->nextPtr;
	    displayPtr->nextPtr  = ui_displ_freeListPtr;
	    ui_displ_freeListPtr = displayPtr;
	    ui_displ_numberOfItems -= 1;
	}
    }
}
	

/*****************************************************************************
  FUNCTION : Ui_DisplayType

  PURPOSE  : looks in the free list for an item. If no one is found, 
             ui_displ_createItem() is called to create a new one.
  RETURNS  : a pointer to a free item or NULL if malloc() fails
  NOTES    :

  UPDATES  :
*****************************************************************************/

struct Ui_DisplayType *ui_displ_getFreeItem(void)

{
    struct Ui_DisplayType  *displPtr;

    ui_displ_numberOfItems++;
    if (ui_displ_freeListPtr == NULL) { /* no free item */
	displPtr             = ui_displ_createItem(); /* create a new item */
    } else {
	displPtr             = ui_displ_freeListPtr;
	ui_displ_freeListPtr = displPtr->nextPtr;
    }
    if (displPtr != NULL) {
	/* link it in the list as the first element */
	(*displPtr).nextPtr = ui_displ_listPtr;
	ui_displ_listPtr    = displPtr;
    }
    return(displPtr);
}
	

#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_displ_getDisplayPtrViaDrawable

  PURPOSE  : search through the display list for a display with the specified
             drawable ID
  RETURNS  : struct Ui_DisplayType
  NOTES    : !!! function is not used yet

  UPDATE   : 1.2.1990
******************************************************************************/

/*!*/ /* not used */
static struct Ui_DisplayType *ui_displ_getDisplayPtrViaDrawable(Widget w)


{
    struct Ui_DisplayType *dPtr;

    dPtr = ui_displ_listPtr;
    while((dPtr != NULL) AND (dPtr->widget != w))
	dPtr = dPtr->nextPtr;

    return(dPtr);
}

#endif  /*  _UNUSED_FUNCTIONS_ */


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_displ_getDisplayPtrViaFrameWidget

  PURPOSE  : search through the display list for a display with the specified
             frame widget
  RETURNS  : struct Ui_DisplayType
  NOTES    : !!! function is not used yet

  UPDATE   : 1.2.1990
******************************************************************************/

/*!*/ /* not used */
static struct Ui_DisplayType *ui_displ_getDisplayPtrViaFrameWidget(Widget w)

{
    struct Ui_DisplayType *dPtr;

    dPtr = ui_displ_listPtr;
    while((dPtr != NULL) AND (dPtr->frameWidget != w))
	dPtr = dPtr->nextPtr;

    return(dPtr);
}

#endif /*  _UNUSED_FUNCTIONS_ */


/*****************************************************************************
  FUNCTION : ui_displ_isSomeWhereToShowWeights

  PURPOSE  : Returns TRUE if there is to show the weights of links 
             in any display.
  RETURNS  : Bool
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

Bool ui_displ_isSomeWhereToShowWeights(void)

{
    struct Ui_DisplayType *dPtr;

    dPtr = ui_displ_listPtr;
    while((dPtr != NULL)) {
	if (dPtr->setup.showWeightFlg) return(TRUE);
	dPtr = dPtr->nextPtr;
    }
    return(FALSE);
}


/*****************************************************************************
  FUNCTION : ui_displ_isSomeWhereToShowValues

  PURPOSE  : Returns TRUE if there is to show the unit value numerically 
             in any display.
  RETURNS  : Bool
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

Bool ui_displ_isSomeWhereToShowValues(void)

{
    struct Ui_DisplayType *dPtr;

    dPtr = ui_displ_listPtr;
    while((dPtr != NULL)) {
	if (dPtr->setup.showValueFlg) return(TRUE);
	dPtr = dPtr->nextPtr;
    }
    return(FALSE);
}


/*****************************************************************************
  FUNCTION : ui_displ_freezeDisplay

  PURPOSE  : sets the freeze flag in the display structure
  RETURNS  : void
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_displ_freezeDisplay(Widget w, struct Ui_DisplayType *displayPtr, 
			    caddr_t call_data)

{
    displayPtr->frozen = NOT displayPtr->frozen;
    ui_net_completeRefresh(displayPtr, UI_LOCAL);
}






/* end of file */
/* lines: 301 */

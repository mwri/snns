/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_action.c,v $
  SHORTNAME      : action.c
  SNNS VERSION   : 4.2
 
  PURPOSE        :
  NOTES          :
 
  AUTHOR         : Tilman Sommer
  DATE           : 6.6.1990
 
  CHANGED BY     : Niels Mache
  RCS VERSION    : $Revision: 2.16 $
  LAST CHANGE    : $Date: 1998/05/15 13:12:12 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "ui_selection.h"

#include "kr_ui.h"

#include <X11/Xaw/List.h>

#include "ui_utilP.h"
#include "ui_netUpdate.h"
#include "ui_confirmer.h"
#include "ui_xWidgets.h"
#include "ui_display.h"
#include "ui_controlP.h"
#include "ui_xGraphic.h"   /* ui_xDrawPoint */
#include "ui_mainP.h"
#include "ui_lists.h"
#include "ui_infoP.h"
#include "ui_textP.h"
#include "ui_netGraph.h"
#include "d3_global.h"
#include "d3_lists.h"
#include "d3_main.h"
#include "ui_key.h"
#include "ui_info.h"
#include "ui_layer.h"

#include "ui_action.ph"
         

/*****************************************************************************
  FUNCTION : ui_action_init

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void ui_action_init (void)

{
    sprintf(ui_action_listSite.msg,"Select Site");
    ui_action_listSite.widgetPtr      = NULL;
    ui_action_listSite.listWidget     = NULL;
    ui_action_listSite.listPtr        = NULL;
    ui_action_listSite.noOfAllocItems = 0;
    ui_action_listSite.noOfItems      = 0;
    ui_action_listSite.unitPtr        = NULL;
    ui_action_listSite.listType       = UI_LIST_SITE;                
}


/*****************************************************************************
  FUNCTION : ui_action_isNewUnit

  PURPOSE  : called from ui_action_unitsCopy to determine, whether the unit
             unitNo was just created by the copy command
  RETURNS  : Bool
  NOTES    : 

  UPDATE   : 20.9.1990
******************************************************************************/

static Bool ui_action_isNewUnit (int unitNo)

{
    struct SelectionType     *selPtr;
    	
    for (selPtr = ui_sel_listPtr;
	 (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	 selPtr = selPtr->nextPtr) {
		
	/* check whether there is already a unit at destination */
	if (selPtr->copyNo == unitNo) return(TRUE);
    }
    return(FALSE);
}


/*****************************************************************************
  FUNCTION : ui_action_createLink

  PURPOSE  : creates a link 
  RETURNS  : Bool
  NOTES    : returns TRUE, if operation was successful
             The current unit/site/link pointers will always be restored
	     after the operation was done.
	     siteName may be "" indicating a direct link 

  UPDATE   : 20.9.1990
******************************************************************************/

static Bool ui_action_createLink (int source, int target, char siteName[], 
		FlintType weight)

{
    int       curUnit = krui_getCurrentUnit();
    char      curSiteName[MAX_NAME_LENGTH];
    int       curPredUnit = 0;
    int       isRecLink ;

    if (krui_getUnitInputType(curUnit) == SITES) 
	strcpy(curSiteName, krui_getSiteName());
    else
	curSiteName[0] = '\0';
    
    if (krui_setCurrentUnit(target) != UI_SET_OK) {
	if (curUnit > 0) krui_setCurrentUnit(curUnit); /* restore */
	return(FALSE); /* target doesn't exist */
    }

    if (((krui_getUnitInputType(target) == SITES) AND NOT strlen(siteName)) OR
	((krui_getUnitInputType(target) != SITES) AND strlen(siteName))) {
	if (curUnit > 0) krui_setCurrentUnit(curUnit); /* restore */
	return(FALSE); /* creation impossible */
    }
    
    if (strlen(siteName) AND (krui_setSite(siteName) != UI_SET_OK)) {
	if (curUnit > 0) {
	    krui_setCurrentUnit(curUnit); /* restore */
	    if (krui_getUnitInputType(target) == SITES) {
		(void) krui_setSite(curSiteName);
		if (curPredUnit > 0) (void) krui_isConnected(curPredUnit);
	    }
	}	    
	return(FALSE); /* this site doesn't exist on target */
    }

    if (krui_isConnected(source)) {
	if (curUnit > 0) {
	    krui_setCurrentUnit(curUnit); /* restore */
	    if (krui_getUnitInputType(target) == SITES) {
		(void) krui_setSite(curSiteName);
		if (curPredUnit > 0) (void) krui_isConnected(curPredUnit);
	    }
	}	    
	return(FALSE); /* link already exist */
    }

    isRecLink = krui_areConnected (target, source) ;

    if (isRecLink){
	ui_net_drawLink(ui_currentDisplay, UI_GLOBAL, target, source,
			krui_getLinkWeight(), UI_ERASE);
    }                        

    ui_checkError(krui_createLink(source, weight));
    ui_net_drawLink(ui_currentDisplay, UI_GLOBAL, source, target, weight, UI_DRAW);

    if (isRecLink){
	ui_net_drawLink(ui_currentDisplay, UI_GLOBAL, target, source,
			krui_getLinkWeight(), UI_DRAW);
    }

    if (curUnit > 0) {
	krui_setCurrentUnit(curUnit); /* restore */
	if (krui_getUnitInputType(target) == SITES) 
	    (void) krui_setSite(curSiteName);
	if (curPredUnit > 0) (void) krui_isConnected(curPredUnit);
    }	    
	
    return(TRUE);  /* success */

}


/*****************************************************************************
  FUNCTION : ui_action_deleteLink

  PURPOSE  :
  RETURNS  : Bool
  NOTES    : returns TRUE, if operation was successful
             see ui_action_createLink

  UPDATE   : 20.9.1990
******************************************************************************/

static Bool ui_action_deleteLink (int source, int target, char siteName[])

{
    int       curUnit = krui_getCurrentUnit();
    char      curSiteName[MAX_NAME_LENGTH];
    FlintType w;
    int       curPredUnit = krui_getCurrentPredUnit(&w);
    int       isRecLink = 0 ;

    if (krui_getUnitInputType(curUnit) == SITES) 
	strcpy(curSiteName, krui_getSiteName());
    else
	curSiteName[0] = '\0';

    if (krui_setCurrentUnit(target) != UI_SET_OK) {
	if (curUnit > 0) krui_setCurrentUnit(curUnit); /* restore */
	return(FALSE); /* target doesn't exist */
    }

    if (((krui_getUnitInputType(target) == SITES) AND NOT strlen(siteName)) OR
	((krui_getUnitInputType(target) != SITES) AND strlen(siteName))) {
	if (curUnit > 0) krui_setCurrentUnit(curUnit); /* restore */
	return(FALSE); /* creation impossible */
    }
    
    if (strlen(siteName) AND (krui_setSite(siteName) != UI_SET_OK)) {
	if (curUnit > 0) {
	    krui_setCurrentUnit(curUnit); /* restore */
	    if (krui_getUnitInputType(target) == SITES) {
		(void) krui_setSite(curSiteName);
		if (curPredUnit > 0) (void) krui_isConnected(curPredUnit);
	    }
	}	    
	return(FALSE); /* this site doesn't exist on target */
    }

    if (NOT krui_isConnected(source)) {
	if (curUnit > 0) {
	    krui_setCurrentUnit(curUnit); /* restore */
	    if (krui_getUnitInputType(target) == SITES) {
		(void) krui_setSite(curSiteName);
		if (curPredUnit > 0) (void) krui_isConnected(curPredUnit);
	    }
	}	    
	return(FALSE); /* link doesn't  exist */
    }

    isRecLink = krui_areConnected (target, source) ;

    if (isRecLink){
	ui_net_drawLink(ui_currentDisplay, UI_GLOBAL, target, source,
			krui_getLinkWeight(), UI_ERASE);

    }

    ui_net_drawLink(ui_currentDisplay, UI_GLOBAL, source, target, 
		    krui_getLinkWeight(), UI_ERASE);
    krui_deleteLink();

    if (isRecLink){
	ui_net_drawLink(ui_currentDisplay, UI_GLOBAL, target, source,
			krui_getLinkWeight(), UI_DRAW);

    }

    if (curUnit > 0) {
	krui_setCurrentUnit(curUnit); /* restore */
	if (krui_getUnitInputType(target) == SITES) 
	    (void) krui_setSite(curSiteName);
	if (curPredUnit > 0) (void) krui_isConnected(curPredUnit);
    }
    return(TRUE);
}


/*****************************************************************************
  FUNCTION : ui_action_getSite

  PURPOSE  : cause the user to select a site if there are any. If unitNo
             is 0 ( = UI_GENERAL) the selection list contains all site 
	     names found. Else all sites off the given unit will be listed.
  RETURNS  : Bool
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

static Bool  ui_action_getSite (int unitNo)

{
    char        *name, *funcName;
    Position	x, y;

    ui_action_siteName[0] = '\0';

    if ((unitNo != UI_GENERAL) AND (krui_getUnitInputType(unitNo) != SITES))
	/* specified unit has no sites! */
	return(FALSE); /* direct links */
    
    if ((unitNo == UI_GENERAL) AND 
	NOT krui_getFirstSiteTableEntry(&name, &funcName))
	/* no sites defined */
	return(FALSE);

    if (unitNo != UI_GENERAL) {
	(void) krui_setCurrentUnit(unitNo);
	ui_action_listSite.listType       = UI_LIST_UNIT_SITE; 
    } else
	ui_action_listSite.listType       = UI_LIST_SITE;                
               
    /* display site popup */
    XtTranslateCoords(ui_currentDisplay->widget, 
		      (Position) 10, (Position) 10, &x, &y);
    ui_listPopup(&ui_action_listSite, x, y);
    if (ui_list_returnIndex < 0) 
	return(FALSE); /* direct links */
    else {
	strcpy(ui_action_siteName, ui_list_returnName);
	return(TRUE);  /* site choosed */
    }
}


/*****************************************************************************
  FUNCTION : ui_action_checkNewPositions

  PURPOSE  : This routine is called by actions MOVE and COPY in order to 
             determine first whether to perform the operation or not.
  RETURNS  : TRUE, if the operation can be performed, else FALSE.
  NOTES    : kindOfAction:  UI_ACTION_MOVE or UI_ACTION_COPY

  UPDATE   : 1.2.1990
*****************************************************************************/

static Bool ui_action_checkNewPositions (int kindOfAction)

{
    struct PosType           moveToPosition;
    struct SelectionType     *selPtr;
    int               deltaX = ui_key_eventPos.x - ui_targetUnit.gridPos.x;
    int               deltaY = ui_key_eventPos.y - ui_targetUnit.gridPos.y;
    int               unitNoAtDest;
	
    selPtr = ui_sel_listPtr;
    if(selPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
	return(FALSE);
    }

    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
	
	moveToPosition.x = selPtr->gridPos.x + deltaX;
	moveToPosition.y = selPtr->gridPos.y + deltaY;
	moveToPosition.z = selPtr->gridPos.z;
	
	/* check whether there is already a unit at destination */
	if ((unitNoAtDest = 
	     krui_getUnitNoAtPosition(&moveToPosition, ui_currentDisplay->subNetNo)) > 0)
	    if ((kindOfAction != UI_ACTION_MOVE) OR
		(ui_sel_lookForItem(unitNoAtDest) == NULL)) 
		/* if UI_ACTION_COPY or 
		   UI_ACTION_MOVE and unitNoAtDest isn't selected */
		return(FALSE); 
	selPtr = selPtr->nextPtr;
    }
    return(TRUE);
}


/*****************************************************************************
  FUNCTION : ui_actions_unitsMove

  PURPOSE  : move all selected units. The destination position is determined
             by the position of the target-unit (unit-info-panel) and the
	     position in ui_key_eventPos.
  RETURNS  : void
  NOTES    : First the dest positions are checked.
             Then the first unit to move is considered (depends of the 
	     direction of the move).
	     The move is performed.
	     Finally, draw the whole net.

  UPDATE   :
*****************************************************************************/

void ui_action_unitsMove (void)

{
    struct PosType       moveToPosition; /* target position */
    struct SelectionType *maxUnitSelPtr;       /* ptr to the first unit to move */
    struct SelectionType *selPtr, *mainSelPtr;         
    int                  deltaX = ui_key_eventPos.x - ui_targetUnit.gridPos.x;
    int                  deltaY = ui_key_eventPos.y - ui_targetUnit.gridPos.y;
    int                  maxUnitSelPtrDeltaX;
    int                  maxUnitSelPtrDeltaY;

    if (ui_action_checkNewPositions(UI_ACTION_MOVE)) {

	ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);

	mainSelPtr = ui_sel_listPtr;
	while ((mainSelPtr = ui_sel_lookForSelectedItem(mainSelPtr)) != NULL) {

	    maxUnitSelPtr = NULL;  /* signal, that no such unit was found yet */

	    if (ui_utilIsSet(mainSelPtr->flags, UI_ALREADY_MOVED)) 
		mainSelPtr = mainSelPtr->nextPtr;
	    else {
		/* look for the first unit to move (-> maxUnitSelPtr!)*/
		maxUnitSelPtr = mainSelPtr; /* Consider this one to move first. */
		selPtr  = mainSelPtr; /* start here, because mainSelPtr holds
					 always the first unit not moved. */

		while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
		    
		    if (ui_utilIsNotSet(selPtr->flags, UI_ALREADY_MOVED)) {
			/* The current unit was not moved already. */
			maxUnitSelPtrDeltaX = 
			    selPtr->gridPos.x - maxUnitSelPtr->gridPos.x;
			maxUnitSelPtrDeltaY = 
			    selPtr->gridPos.y - maxUnitSelPtr->gridPos.y;
			
			if ((deltaX * maxUnitSelPtrDeltaX >= 0) AND
			    (deltaY * maxUnitSelPtrDeltaY >= 0))
			    /* Both values have the same sign => the current 
			       unit has to be treated first ! */
			    maxUnitSelPtr     = selPtr;
		    }
		    selPtr = selPtr->nextPtr;
		}
		/* The unit to move was found now. */
		/* move the unit referred by maxUnitSelPtr */
		moveToPosition.x = maxUnitSelPtr->gridPos.x + deltaX;
		moveToPosition.y = maxUnitSelPtr->gridPos.y + deltaY;
		moveToPosition.z = maxUnitSelPtr->gridPos.z;
	
		/* if the current target unit in the info panel is selected,
		   then update its position too. */
		if (maxUnitSelPtr->unitNo == ui_targetUnit.no)
		    ui_targetUnit.gridPos = moveToPosition;

		/* erase unit and links at old position first */
		ui_net_drawAllLinksToUnit(ui_currentDisplay, UI_GLOBAL, 
					  maxUnitSelPtr->unitNo, UI_ERASE);
		ui_net_drawAllLinksFromUnit(ui_currentDisplay, UI_GLOBAL, 
					    maxUnitSelPtr->unitNo, UI_ERASE);
		ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL,
				maxUnitSelPtr->unitNo, UI_ERASE);
	
		/* call kernel */
		krui_setUnitPosition(maxUnitSelPtr->unitNo, &moveToPosition);
 		krui_setUnitSubnetNo(maxUnitSelPtr->unitNo, 
				     ui_currentDisplay->subNetNo);
		/* alter position in selection list */
		maxUnitSelPtr->gridPos = moveToPosition;  
		/* draw unit and links at new position */
		ui_net_drawAllLinksToUnit(ui_currentDisplay, UI_GLOBAL, 
					  maxUnitSelPtr->unitNo, UI_DRAW);
		ui_net_drawAllLinksFromUnit(ui_currentDisplay, UI_GLOBAL, 
					    maxUnitSelPtr->unitNo, UI_DRAW);
		ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL,
				maxUnitSelPtr->unitNo, UI_DRAW);
		/* mark unit as moved */
		ui_utilSetFlag(maxUnitSelPtr->flags, UI_ALREADY_MOVED);
		/* start from beginning of the selection list again */
		mainSelPtr = ui_sel_listPtr;
    }
	    /* In both cases of this if-statement, mainSelPtr helds a 
	       new pointer to continue the move-ooperation. */
	}
	
	/* the move operation is done */
	/* reset all Flags UI_ALREADY_MOVED */
	selPtr = ui_sel_listPtr;
	while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
	    ui_utilResetFlag(selPtr->flags, UI_ALREADY_MOVED);
	    selPtr = selPtr->nextPtr;
	}
        d3_drawNet ();
	ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
    } else 
	ui_confirmOk("Can't move units to this position!");
}


/*****************************************************************************
  FUNCTION : ui_action_unitsCopy

  PURPOSE  : copy all selected units in the current mode to another position.
  RETURNS  : alteration of the network
  NOTES    : The operation is very similar to ui_action_unitsMove(). The copy
             operation takes attention on the flag-word ui_linksToHandle.
	     A special case is the copying of the topological structure, which
	     is handled on the end of the routine separately from the actual
	     copying of the units itsself.

  UPDATE   :
*****************************************************************************/

void ui_action_unitsCopy(void)

{
    struct PosType  moveToPosition;         /* position while copying */

    FlintType weight;  /* used while structureCopy */
    FlintType infoWeight;

     /* Ptrs to step through the selection list */

    struct SelectionType     *mainSelPtr, *selPtr;
    int               deltaX = ui_key_eventPos.x - ui_targetUnit.gridPos.x;
    int               deltaY = ui_key_eventPos.y - ui_targetUnit.gridPos.y;

    Bool              structureCopy;
    int               copyMode;
    int               predUnit;	    


    /* *******************  start ********************************* */

    if (NOT ui_action_checkNewPositions(UI_ACTION_COPY)) {
	ui_confirmOk("Can't copy units to this position!");
	return;
    }

    /* check whether to copy the structure and keep it in mind */
    structureCopy = ui_utilIsSet(ui_key_linksToHandle, UI_STRUCTURE_COPY);

    /* reset in either case the flag UI_STRUCTURE_COPY */
    ui_utilResetFlag(ui_key_linksToHandle, UI_STRUCTURE_COPY);

    /* now transform ui_key_linksTpHandle to kernel values */
    switch (ui_key_linksToHandle BIT_AND UI_ALL) {
      case UI_ALL    : copyMode = ONLY_OUTPUTS ; break; /* input links will be  
							   treated later */
      case UI_NO_FLAG: copyMode = ONLY_UNIT    ; break;
      case UI_OUT    : copyMode = ONLY_OUTPUTS ; break;
      case UI_INPUT  : copyMode = ONLY_INPUTS  ; break;
    }

    if (structureCopy AND (ui_utilIsSet(ui_key_linksToHandle, UI_BACKWARD_BINDING) OR
			   ui_utilIsSet(ui_key_linksToHandle, UI_FORWARD_BINDING)))
	(void) ui_action_getSite(UI_GENERAL);
    else
	ui_action_siteName[0] = '\0';
    
    if(ui_infoIsCreated)
	infoWeight = (FlintType)ui_xFloatFromAsciiWidget(ui_linkWidgets.weight);
    else
	infoWeight = 0.0;

    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* hide markers */

    for (selPtr = ui_sel_listPtr; 
	 (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	 selPtr = selPtr->nextPtr) {
	moveToPosition.x = selPtr->gridPos.x + deltaX;
	moveToPosition.y = selPtr->gridPos.y + deltaY;
	moveToPosition.z = selPtr->gridPos.z;
	/* call kernel */
	selPtr->copyNo   = krui_copyUnit(selPtr->unitNo, copyMode);
	/* Sites are copied as well in either case. */
	krui_setUnitPosition(selPtr->copyNo, &moveToPosition);
	krui_setUnitSubnetNo(selPtr->copyNo, ui_currentDisplay->subNetNo);

	/* draw unit and current links at new position */
	ui_net_drawAllLinksToUnit(ui_currentDisplay, UI_GLOBAL, 
				  selPtr->copyNo, UI_DRAW);
	ui_net_drawAllLinksFromUnit(ui_currentDisplay, UI_GLOBAL, 
				    selPtr->copyNo, UI_DRAW);
	ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL,
			selPtr->copyNo, UI_DRAW);

	if (structureCopy) {
	    /* make bindings to the origin units if required */
	    
	    if (ui_utilIsSet(ui_key_linksToHandle, UI_FORWARD_BINDING))
		/* original unit is source ! */
		ui_action_createLink(selPtr->unitNo, selPtr->copyNo,
				     ui_action_siteName, infoWeight);
	    
	    if (ui_utilIsSet(ui_key_linksToHandle, UI_BACKWARD_BINDING)) 
		/* original unit is target */
		ui_action_createLink(selPtr->copyNo, selPtr->unitNo,
				     ui_action_siteName, infoWeight);
	}		
    }

    /* if COPY ALL: output links are copied by the kernel. Now copy input links */
    
    if (ui_utilIsSet(ui_key_linksToHandle, UI_ALL)) {
	for (mainSelPtr = ui_sel_listPtr;
	     /* outer loop to step through all selected units */
	     (mainSelPtr = ui_sel_lookForSelectedItem(mainSelPtr)) != NULL;
	     mainSelPtr = mainSelPtr->nextPtr) {
	    
	    if (krui_getUnitInputType(mainSelPtr->unitNo) == SITES) {
		bool  successful;
		/* unit with sites: for all sites copy incoming links */
		(void) krui_setCurrentUnit(mainSelPtr->unitNo); /* target */
		for (successful = krui_setFirstSite();
		     successful; successful = krui_setNextSite()) {
		    for(predUnit = krui_getFirstPredUnit(&weight);
			predUnit > 0;
			predUnit = krui_getNextPredUnit(&weight)) {
			if (ui_action_isNewUnit(predUnit)) continue;
			/* there is a link from an old unit to this site */
			ui_action_createLink(predUnit, mainSelPtr->copyNo, 
					     krui_getSiteName(), weight);
		    }
		}
	    } else { /* direct links */
		(void) krui_setCurrentUnit(mainSelPtr->unitNo); /* target */
		for(predUnit = krui_getFirstPredUnit(&weight);
		    predUnit > 0;
		    predUnit = krui_getNextPredUnit(&weight)) {	    
		    if (ui_action_isNewUnit(predUnit)) continue;
		    /* there is a link from an old unit */
		    ui_action_createLink(predUnit, mainSelPtr->copyNo, "", weight);
		}
	    }
	}
    } /* endif UI_ALL */

    /* if structureCopy then copy the link structure of the source */
    
    if (structureCopy) { 

	for (mainSelPtr = ui_sel_listPtr;
	     /* outer loop to step through all selected units */
	     (mainSelPtr = ui_sel_lookForSelectedItem(mainSelPtr)) != NULL;
	     mainSelPtr = mainSelPtr->nextPtr) {

	    for (selPtr = mainSelPtr;
		 /* inner loop */
		 (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
		 selPtr = selPtr->nextPtr) {

		/* check the first direction (mainSel -> sel) */
		if (krui_getUnitInputType(selPtr->unitNo) == SITES) {
		    /* unit with sites: for all sites copy links */
		    bool  successful;
		    (void) krui_setCurrentUnit(selPtr->unitNo); /* target */
		    for (successful = krui_setFirstSite();
			 successful; successful = krui_setNextSite()) 
			if (krui_isConnected(mainSelPtr->unitNo)) 
			    /* there is a link to this site */
			    ui_action_createLink(mainSelPtr->copyNo, selPtr->copyNo,
						 krui_getSiteName(), krui_getLinkWeight());
		    
		} else { /* unit with direct links */
		    (void) krui_setCurrentUnit(selPtr->unitNo); /* target */
		    if (krui_isConnected(mainSelPtr->unitNo)) 
			ui_action_createLink(mainSelPtr->copyNo, selPtr->copyNo, 
					     "", krui_getLinkWeight());
		}
		
		/* check the other direction (sel -> mainSel) */
		if (krui_getUnitInputType(mainSelPtr->unitNo) == SITES) {
		    /* unit with sites: for all sites copy links */
		    bool  successful;
		    (void) krui_setCurrentUnit(mainSelPtr->unitNo); /* target */
		    for (successful = krui_setFirstSite();
			 successful; successful = krui_setNextSite()) 
			if (krui_isConnected(selPtr->unitNo)) 
			    /* there is a link to this site */
			    ui_action_createLink(selPtr->copyNo, mainSelPtr->copyNo,
						 krui_getSiteName(), krui_getLinkWeight());
		    
		} else { /* unit with direct links */
		    (void) krui_setCurrentUnit(mainSelPtr->unitNo); /* target */
		    if (krui_isConnected(selPtr->unitNo)) 
			ui_action_createLink(selPtr->copyNo, mainSelPtr->copyNo,
					     "", krui_getLinkWeight());
		}
	    }
	}
    } /* endif "structureCopy" */
    d3_drawNet ();
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* show markers */
}


/*****************************************************************************
  FUNCTION : ui_action_linksMakeFromSourceUnit

  PURPOSE  : Insert links from the unit under the raster cursor to all 
             selected units.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_linksMakeFromSourceUnit (int op)

{
    int                   sourceNo;
    FlintType             weight;
    struct SelectionType  *selPtr;

    (void) ui_action_getSite(UI_GENERAL);
 
    if ((sourceNo = ui_key_unitNo) <= 0) return;
	
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); 
	
    if(ui_infoIsCreated)
	weight = (FlintType) ui_xFloatFromAsciiWidget(ui_linkWidgets.weight);
    else
	weight = 0.0;

    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (selPtr = ui_sel_listPtr;
	 (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	 selPtr = selPtr->nextPtr) 
	if (op == UI_DRAW)
	    ui_action_createLink(sourceNo, selPtr->unitNo, 
				 ui_action_siteName, weight);
	else
	    ui_action_deleteLink(sourceNo, selPtr->unitNo, 
				 ui_action_siteName);

    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);  
    /* draw selection markers again */
    ui_info_makeUnitInfoPanelConsistent(); 
}


/*****************************************************************************
  FUNCTION : ui_action_linksMakeToTargetUnit

  PURPOSE  : Insert links from all selected units to the unit under the
             raster-cursor.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_linksMakeToTargetUnit (int op)

{
    int                   targetNo;
    struct SelectionType  *selPtr;
    FlintType             weight;

    if ((targetNo = ui_key_unitNo) <= 0) return;

    (void) ui_action_getSite(targetNo);
    
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */
    
    if(ui_infoIsCreated)
	weight = (FlintType) ui_xFloatFromAsciiWidget(ui_linkWidgets.weight);
    else
	weight = 0.0;

    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (selPtr = ui_sel_listPtr;
	 (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	 selPtr = selPtr->nextPtr) 
	if (op == UI_DRAW)
	    ui_action_createLink(selPtr->unitNo, targetNo, 
				 ui_action_siteName, weight);
	else
	    ui_action_deleteLink(selPtr->unitNo, targetNo, 
				 ui_action_siteName);

    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); 
    ui_info_makeUnitInfoPanelConsistent(); 
}


/*****************************************************************************
  FUNCTION : ui_action_unitInsert

  PURPOSE  : Insert one unit with the attributes of the displayed source-unit
             in the unit-info-panel.
  RETURNS  : alteration of the network
  NOTES    : This routine got not the name 'ui_action_unitsInsert()', because
             it always handles with only one unit!
	     The markers normally need not to be deleted, but if the new unit
	     overlaps a marker, a ugly spot will occur. So do it!

  UPDATE   :
*****************************************************************************/

void ui_action_unitInsert (int insertType)

{
    Position	               x, y; /* for f-type list panel */
    static struct SimpleListType  listFType;
    struct UnitAttributeType  unit;
    int                       newUnitNo;

    if ((newUnitNo = ui_key_unitNo) <= 0) {
	/* this position is free */
	ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */
	switch (insertType) {
	  case UI_INSERT_DEFAULT:
	    newUnitNo = krui_createDefaultUnit();
	    break;
	  case UI_INSERT_TARGET:
	    newUnitNo = krui_createDefaultUnit();
	    if (newUnitNo > 0) {
		unit = ui_targetUnit;
		ui_info_getDisplayedUnitAttributes(&ui_targetWidgets, &unit); 
		ui_info_storeAttributes(newUnitNo, unit);
	    }
	    break;
	  case UI_INSERT_FTYPE:
	    ui_info_initUnitStruct(&unit);
	    XtTranslateCoords(ui_currentDisplay->widget, 
			      (Position) 5, (Position) 5, &x, &y);    
	    sprintf(listFType.msg,"Insert f-type unit");
	    listFType.unitPtr  = &unit;
	    listFType.widgetPtr= NULL;
	    listFType.listType = UI_LIST_FTYPE;
	    ui_listPopup((struct SimpleListType *) &listFType, x,y);
	    newUnitNo = krui_createFTypeUnit(unit.ftype);
	    /* if no f-type was selected, unit.ftype holds "?".
	       The kernel will not insert unit, because this is an invalid
	       f-type and newUnitNo will be -20 (Error 20) */
	    break;
	}

	if (newUnitNo > 0) {
	    krui_setUnitSubnetNo(newUnitNo, ui_currentDisplay->subNetNo);
            ui_key_eventPos.z = ui_actualZvalue;
	    krui_setUnitPosition(newUnitNo, &ui_key_eventPos);
	    krui_setUnitLayerNo(newUnitNo, (int) ui_currentDisplay->layers);
	    ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL,
			    newUnitNo, UI_DRAW);
	}
	ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* draw markers */
        d3_drawNet ();
    }
}


/*****************************************************************************
  FUNCTION : ui_action_linksMakeBidirectional

  PURPOSE  : Insert links beetween all selected units, that all existing
             single links become bidirectional links.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_linksMakeBidirectional (void)

{
    struct SelectionType  *selPtr, *mainSelPtr;
    bool                  successful;

    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */

    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (mainSelPtr = ui_sel_listPtr;
	 (mainSelPtr = ui_sel_lookForSelectedItem(mainSelPtr)) != NULL;
	 mainSelPtr = mainSelPtr->nextPtr) { 
	/* main loop */

	for (selPtr = ui_sel_listPtr;
	     (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	     selPtr = selPtr->nextPtr) { 
	    /* inner loop */

	    if (selPtr == mainSelPtr) continue;

	    if (krui_getUnitInputType(selPtr->unitNo) == SITES) {
		(void) krui_setCurrentUnit(selPtr->unitNo); /* target */
		for (successful = krui_setFirstSite(); successful;
		     successful = krui_setNextSite()) {
		    if (krui_isConnected(mainSelPtr->unitNo)) 
			ui_action_createLink(selPtr->unitNo, mainSelPtr->unitNo,
					     krui_getSiteName(), krui_getLinkWeight());
		}
	    } else {
		/* direct links */
		(void) krui_setCurrentUnit(selPtr->unitNo); /* target */
		if (krui_isConnected(mainSelPtr->unitNo)) 
		    ui_action_createLink(selPtr->unitNo, mainSelPtr->unitNo,
					 "", krui_getLinkWeight());
	    }
	}
    }
    ui_info_makeUnitInfoPanelConsistent(); 
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* show markers */
}


/*****************************************************************************
  FUNCTION : ui_action_linksMakeReverse

  PURPOSE  : Alter the Direction of the links beetween all selected units.
  RETURNS  : alteration of the network
  NOTES    : For all units: 
               reverse incoming links from not marked, but selected units
	       reverse outgoing links to selected units
	       mark unit (this means: all incoming links are reversed links)

  UPDATE   :
*****************************************************************************/

void ui_action_linksMakeReverse (void)

{
    struct SelectionType  *selPtr, *mainSelPtr;
    FlintType             weight, weight2;
    Bool                  successful;

    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */

    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (mainSelPtr = ui_sel_listPtr;
	 (mainSelPtr = ui_sel_lookForSelectedItem(mainSelPtr)) != NULL;
	 mainSelPtr = mainSelPtr->nextPtr) { 
	/* main loop */

	for (selPtr = ui_sel_listPtr;
	     (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	     selPtr = selPtr->nextPtr) { 
	    /* inner loop */

	    if (mainSelPtr == selPtr) continue;

	    if (ui_utilIsNotSet(selPtr->flags, UI_ALREADY_REVERSED)) {
		/* CASE 1: */
		/* incoming links */
		/* the other selected unit is not marked */
		
		if ((krui_getUnitInputType(mainSelPtr->unitNo) == SITES) AND
		    (krui_getUnitInputType(selPtr->unitNo) != SITES)) {

		    (void) krui_setCurrentUnit(mainSelPtr->unitNo);
		    for (successful = krui_setFirstSite(); successful;
			 successful = krui_setNextSite()) {
			
			if (krui_isConnected(selPtr->unitNo)) {
			    char siteName[MAX_NAME_LENGTH];
			    strcpy(siteName, krui_getSiteName());
			    weight = krui_getLinkWeight();
			    
			    (void) krui_setCurrentUnit(selPtr->unitNo);
			    if (krui_setSite(siteName) == UI_SET_OK) {
				/* such a site exists */
				if (krui_isConnected(mainSelPtr->unitNo)) {
				    /* bidirectional link: exchange weights */
				    weight2 = krui_getLinkWeight();
				    krui_setLinkWeight(weight);
				    ui_krui_setCurrentLink(selPtr->unitNo, mainSelPtr->unitNo,
							   siteName);
				    krui_setLinkWeight(weight2);
				    
				    /* There are no changes in the graphic, 
				       because the weights are still occuring at
				       the same positions !!! */
				    
				} else {
				    /* unidirectional link: reverse */
				    ui_action_deleteLink(selPtr->unitNo, 
							 mainSelPtr->unitNo,
							 siteName);
				    ui_action_createLink(mainSelPtr->unitNo, 
							 selPtr->unitNo,
							 siteName, weight);
				}
			    }
			}
		    }
		} else {
		    if ((krui_getUnitInputType(mainSelPtr->unitNo) != SITES) AND
			(krui_getUnitInputType(selPtr->unitNo) != SITES)) {
			/* no sites */
			(void) krui_setCurrentUnit(mainSelPtr->unitNo);
			if (krui_isConnected(selPtr->unitNo)) {
			    weight = krui_getLinkWeight();
			    (void) krui_setCurrentUnit(selPtr->unitNo);
			    if (krui_isConnected(mainSelPtr->unitNo)) {
				/* bidirectional link: exchange weights */
				weight2 = krui_getLinkWeight();
				krui_setLinkWeight(weight);
				ui_krui_setCurrentLink(selPtr->unitNo, 
						       mainSelPtr->unitNo,
						       "");
				krui_setLinkWeight(weight2);
				
				/* There are no changes in the graphic, 
				   because the weights are still occuring at
				   the same positions !!! */
				
			    } else {
				/* unidirectional link: reverse */
				ui_action_deleteLink(selPtr->unitNo, mainSelPtr->unitNo,
						     "");
				ui_action_createLink(mainSelPtr->unitNo, selPtr->unitNo,
						     "", weight);
			    }
			}
		    }
		    
		}
		
		/* outgoing links */

		if ((krui_getUnitInputType(selPtr->unitNo) == SITES) AND
		    (krui_getUnitInputType(mainSelPtr->unitNo) == SITES)) {
		    
		    (void) krui_setCurrentUnit(selPtr->unitNo);
		    for (successful = krui_setFirstSite(); successful;
			 successful = krui_setNextSite()) {
			
			if (krui_isConnected(mainSelPtr->unitNo)) {
			    char siteName[MAX_NAME_LENGTH];
			    strcpy(siteName, krui_getSiteName());
			    weight = krui_getLinkWeight();
			
			    (void) krui_setCurrentUnit(mainSelPtr->unitNo);
			    if (krui_setSite(siteName) == UI_SET_OK) {
				/* such a site exists */
				if (krui_isConnected(selPtr->unitNo)) {
				    /* bidirectional link: exchange weights */
				    weight2 = krui_getLinkWeight();
				    krui_setLinkWeight(weight);
				    ui_krui_setCurrentLink(mainSelPtr->unitNo, 
							   selPtr->unitNo,
							   siteName);
				    krui_setLinkWeight(weight2);
				    
				    /* There are no changes in the graphic, 
				       because the weights are still occuring at
				       the same positions !!! */
				    
				} else {
				    /* unidirectional link: reverse */
				    ui_action_deleteLink(mainSelPtr->unitNo, 
							 selPtr->unitNo,
							 siteName);
				    ui_action_createLink(selPtr->unitNo, 
							 mainSelPtr->unitNo,
							 siteName, weight);
				}
			    }
			}
		    }
		} else 		    
		    if ((krui_getUnitInputType(selPtr->unitNo) != SITES) AND
			(krui_getUnitInputType(mainSelPtr->unitNo) != SITES)) {
		    
			/* no sites */
			(void) krui_setCurrentUnit(selPtr->unitNo);
			if (krui_isConnected(mainSelPtr->unitNo)) {
			    weight = krui_getLinkWeight();
			    (void) krui_setCurrentUnit(mainSelPtr->unitNo);
			    if (krui_isConnected(selPtr->unitNo)) {
				/* bidirectional link: exchange weights */
				weight2 = krui_getLinkWeight();
				krui_setLinkWeight(weight);
				ui_krui_setCurrentLink(mainSelPtr->unitNo, 
						       selPtr->unitNo,
						       "");
				krui_setLinkWeight(weight2);
				
				/* There are no changes in the graphic, 
				   because the weights are still occuring at
				   the same positions !!! */
				
			    } else {
				/* unidirectional link: reverse */
				ui_action_createLink(selPtr->unitNo, mainSelPtr->unitNo,
						     "", weight);
				ui_action_deleteLink(mainSelPtr->unitNo, selPtr->unitNo,
						     "");
			    }
			}
		    }
	    }
	}
	/* now mark the unit */
	ui_utilSetFlag(mainSelPtr->flags, UI_ALREADY_REVERSED);
    }
    
    /* the operation is done */
    /* reset all Flags UI_ALREADY_REVERSED */
    selPtr = ui_sel_listPtr;
    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
	ui_utilResetFlag(selPtr->flags, UI_ALREADY_REVERSED);
	selPtr = selPtr->nextPtr;
    }
    ui_info_makeUnitInfoPanelConsistent(); 
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* show markers */
}


/*****************************************************************************
  FUNCTION : ui_action_linksCopy

  PURPOSE  : copy all/input/output Links of all selected units 
             to specified unit.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_linksCopy (void)

{
    FlintType weight;  /* source link weight */

     /* Ptrs to step through the selection list */

    struct SelectionType  *selPtr;
    Bool                  successful;
    int                   unitNo, succNo, predNo;

    /* *******************  START ********************************* */
    
    if ((unitNo = ui_key_unitNo) <= 0) {
	ui_confirmOk("EDIT: no destination to copy links !");
	return;
    }

    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */
    
    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (selPtr = ui_sel_listPtr;
	 (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	 selPtr = selPtr->nextPtr) {
	
	if (ui_utilIsSet(ui_key_linksToHandle, UI_OUT)) {
	    for (succNo = krui_getFirstSuccUnit(selPtr->unitNo, &weight);
		 succNo > 0;
		 succNo = krui_getNextSuccUnit(&weight)) {
		if (krui_getUnitInputType(succNo) == SITES)
		    ui_action_createLink(unitNo, succNo,
					 krui_getSiteName(), weight);
		else
		    ui_action_createLink(unitNo, succNo, "", weight);
	    }
	} /* end if: UI_OUT set */
	
	if (ui_utilIsSet(ui_key_linksToHandle, UI_INPUT) AND
	    (krui_getUnitInputType(selPtr->unitNo) ==
	     krui_getUnitInputType(unitNo))) {
	    
	    (void) krui_setCurrentUnit(selPtr->unitNo); /* target */
	    
	    if (krui_getUnitInputType(selPtr->unitNo) == SITES) {
		for (successful = krui_setFirstSite();
		     successful; successful = krui_setNextSite()) {
		    for(predNo = krui_getFirstPredUnit(&weight);
			predNo > 0;
			predNo = krui_getNextPredUnit(&weight)) 
			ui_action_createLink(predNo, unitNo, 
					     krui_getSiteName(), weight);
		}
	    } else { /* direct links */
		for(predNo = krui_getFirstPredUnit(&weight);
		    predNo > 0;
		    predNo = krui_getNextPredUnit(&weight)) 
		    ui_action_createLink(predNo, unitNo, "", weight);
	    }
	}
    }/* end for: selected units exist */
    ui_info_makeUnitInfoPanelConsistent();
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
}
     

/*****************************************************************************
  FUNCTION : ui_action_linksCopyEnvironment

  PURPOSE  : copy all/input/output Links of all selected units 
             to specified unit.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_linksCopyEnvironment (void)

{
    FlintType weight;  /* source link weight */

     /* Ptrs to step through the selection list */

    struct SelectionType  *selPtr;
    Bool                  successful;
    int                   unitNo, succNo, copySuccNo, predNo, copyPredNo;
    struct PosType        succPos, predPos, copySuccPos, copyPredPos;

    /* *******************  START ********************************* */

    if (ui_targetUnit.no < 1) {
	ui_confirmOk("EDIT: invalid TARGET unit! Can't copy links.");
	return;
    }

    if ((unitNo = ui_key_unitNo) < 1) {
	ui_confirmOk("EDIT: no destination to copy links!");
	return;
    }

    /* there is a unit:
       - step through all selected units and copy their Links 
       - take notice from the flags in ui_key_linksToHandle   
       */
    
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */
    
    /* check successors of ui_targetUnit */
    for (succNo = krui_getFirstSuccUnit(ui_targetUnit.no, &weight);
	 succNo > 0;
	 (void) krui_setCurrentUnit(ui_targetUnit.no), 
	 succNo = krui_getNextSuccUnit(&weight)) {
	
	/* get position and calculate the relative position to
	   ui_targetUnit and add the position of unit unitNo */
	krui_getUnitPosition(succNo, &succPos);
	/* check whether this unit is selected */
	selPtr = ui_sel_lookForItem(succNo);
	if ((selPtr != NULL) AND ui_utilIsSet(selPtr->flags, UI_SELECTED)) {
	    copySuccPos.x = 
		succPos.x - ui_targetUnit.gridPos.x + ui_key_eventPos.x;
	    copySuccPos.y = 
		succPos.y - ui_targetUnit.gridPos.y + ui_key_eventPos.y;
	    /* check whether there exists a unit relative to unit unitNo */
	    if ((copySuccNo = 
		 krui_getUnitNoAtPosition(&copySuccPos,
					  selPtr->subNetNo)) > 0) 
		/* there is a candidate ! */
		ui_action_createLink(unitNo, copySuccNo, 
				     ui_link.siteName, weight);
	}
    }
    
    /* check predecessors of ui_targetUnit */
    (void) krui_setCurrentUnit(ui_targetUnit.no); /* target */

    if (krui_getUnitInputType(selPtr->unitNo) == SITES) {

	for (successful = krui_setFirstSite();
	     successful; successful = krui_setNextSite()) {

	    for (predNo = krui_getFirstPredUnit(&weight);
		 predNo > 0;
		 predNo = krui_getNextPredUnit(&weight)) {
		/* get position and calculate the relative position to
		   ui_targetUnit and add the position of unit unitNo */
		krui_getUnitPosition(predNo, &predPos);
		/* check whether this unit is selected */
		selPtr = ui_sel_lookForItem(predNo);
		if ((selPtr != NULL) AND ui_utilIsSet(selPtr->flags, UI_SELECTED)) {
		    copyPredPos.x = 
			predPos.x - ui_targetUnit.gridPos.x + ui_key_eventPos.x;
		    copyPredPos.y = 
			predPos.y - ui_targetUnit.gridPos.y + ui_key_eventPos.y;
		    /* check whether there exists a unit relative to unit unitNo */
		    if ((copyPredNo = 
			 krui_getUnitNoAtPosition(&copyPredPos,
						  selPtr->subNetNo)) > 0) {
			/* there is a candidate ! */
			ui_action_createLink(copyPredNo, unitNo, 
					     krui_getSiteName(), weight);
		    }
		}
	    }
	}
    } else {
	/* direct links */
	for (predNo = krui_getFirstPredUnit(&weight);
	     predNo > 0;
	     predNo = krui_getNextPredUnit(&weight)) {
	    /* get position and calculate the relative position to
	       ui_targetUnit and add the position of unit unitNo */
	    krui_getUnitPosition(predNo, &predPos);
	    /* check whether this unit is selected */
	    selPtr = ui_sel_lookForItem(predNo);
	    if ((selPtr != NULL) AND ui_utilIsSet(selPtr->flags, UI_SELECTED)) {
		copyPredPos.x = 
		    predPos.x - ui_targetUnit.gridPos.x + ui_key_eventPos.x;
		copyPredPos.y = 
		    predPos.y - ui_targetUnit.gridPos.y + ui_key_eventPos.y;
		/* check whether there exists a unit relative to unit unitNo */
		if ((copyPredNo = 
		     krui_getUnitNoAtPosition(&copyPredPos,
					      selPtr->subNetNo)) > 0) {
		    /* there is a candidate ! */
		    ui_action_createLink(copyPredNo, unitNo, "", weight);
		}
	    }
	}
    }
    ui_info_makeUnitInfoPanelConsistent();
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
}


/*****************************************************************************
  FUNCTION : ui_action_sitesCopy

  PURPOSE  : copy all sites of the target unit shown in the info panel to all
             selected units. Depending on ui_key_linksToHandle the links will
	     be copied too.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_sitesCopy (void)

{
    FlintType weight;  /* source link weight */

     /* Ptrs to step through the selection list */

    struct SelectionType  *selPtr;
    int                   unitNo, predNo;

    /* *******************  START ********************************* */
    
    if ((unitNo = ui_key_unitNo) <= 0) {
	ui_confirmOk("EDIT: no destination to copy links !");
	return;
    }

    if (krui_getUnitInputType(ui_targetUnit.no) != SITES) {
	ui_confirmOk("EDIT: TARGET doesn't have sites!");
	return;
    }

    (void) krui_setCurrentUnit(ui_targetUnit.no); /* target */
    if ((ui_link.siteName == NULL) OR 
	(strlen(ui_link.siteName) == 0) OR
	(krui_setSite(ui_link.siteName) != UI_SET_OK)) {
	ui_confirmOk("EDIT: Invalid site of TARGET!");
	return;
    }
    
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */
	
    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (selPtr = ui_sel_listPtr;
	 (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	 selPtr = selPtr->nextPtr) { 

	if (krui_getUnitInputType(selPtr->unitNo) == DIRECT_LINKS) 
	    continue;
	    
	(void) krui_setCurrentUnit(selPtr->unitNo); /* target */
	
	if (krui_setSite(ui_link.siteName) != UI_SET_OK) 
	    /* site doesn't exist */
	    ui_checkError(krui_addSite(ui_link.siteName));
	
	if ((ui_kernelError == 0) AND
	    ui_utilIsSet(ui_key_linksToHandle, UI_INPUT)) {
	    for(predNo = krui_getFirstPredUnit(&weight);
		predNo > 0;
		predNo = krui_getNextPredUnit(&weight)) 
		ui_action_createLink(predNo, unitNo, 
				     ui_link.siteName, weight);
	}
    
    }/* end for: selected units exist */
    ui_info_makeUnitInfoPanelConsistent();
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
}


/*****************************************************************************
  FUNCTION : ui_action_sitesMake

  PURPOSE  : Depending on op a site will be added to (UI_DRAW) or deleted 
             (UI_ERASE) from all selected unit.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_sitesMake (int op)

{
    struct SelectionType  *mainSelPtr;
    char  buf[MAX_NAME_LENGTH];
    Bool  directLinks = FALSE;

    if (NOT ui_krui_sitesExistInNetwork()) return;
    
    if (NOT ui_action_getSite(UI_GENERAL)) return;

    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (mainSelPtr = ui_sel_listPtr;
	 (mainSelPtr = ui_sel_lookForSelectedItem(mainSelPtr)) != NULL;
	 mainSelPtr = mainSelPtr->nextPtr) { 

	(void) krui_setCurrentUnit(mainSelPtr->unitNo);
	switch (krui_getUnitInputType(mainSelPtr->unitNo)) {
	  case SITES:
	    if (op == UI_DRAW) {
		if (krui_setSite(ui_action_siteName) != UI_SET_OK) 
		    /* site doesn't exist */
		    ui_checkError(krui_addSite(ui_action_siteName));
	    } else {
		if (krui_setSite(ui_action_siteName) == UI_SET_OK) { 
		    /* site exists */
		    ui_checkError(krui_setSiteName(ui_action_siteName));
		    ui_checkError(krui_deleteSite());
		}
	    }
	    break;
	  case DIRECT_LINKS:
	    if (op == UI_DRAW) {
		directLinks = TRUE;
		sprintf(buf,"EDIT (sites add): unit %d has direct links!",
			mainSelPtr->unitNo);
		ui_tw_printMessage(buf);
	    }
	    break;
	  case NO_INPUTS:
	    if (op == UI_DRAW)
		ui_checkError(krui_addSite(ui_action_siteName));
	    break;
	}
    }

    if ((op == UI_DRAW) AND directLinks) 
	ui_confirmOk("Couldn't add site to some units!");
}


/*****************************************************************************
  FUNCTION : ui_action_linksMakeClique

  PURPOSE  : Insert Links beetween all selected units, that they form a
             clique, i.e. every unit is connected to all other units.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_linksMakeClique (int op)

{
    struct SelectionType    *selPtr, *mainSelPtr;
    FlintType               weight;

    (void) ui_action_getSite(UI_GENERAL);

    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */

    if(ui_infoIsCreated)
	weight = (FlintType) ui_xFloatFromAsciiWidget(ui_linkWidgets.weight);
    else
	weight = 0.0;

    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (mainSelPtr = ui_sel_listPtr;
	 (mainSelPtr = ui_sel_lookForSelectedItem(mainSelPtr)) != NULL;
	 mainSelPtr = mainSelPtr->nextPtr) { 
	/* main loop */

	for (selPtr = ui_sel_listPtr;
	     (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	     selPtr = selPtr->nextPtr) { 
	    /* inner loop */
	    if (NOT (selPtr == mainSelPtr)) { /* not the same entry !! */
		
		if (op == UI_DRAW) {
		    ui_action_createLink(selPtr->unitNo, mainSelPtr->unitNo,
					 ui_action_siteName, weight);
		    ui_action_createLink(mainSelPtr->unitNo, selPtr->unitNo,
					 ui_action_siteName, weight);
		} else {
		    ui_action_deleteLink(selPtr->unitNo, mainSelPtr->unitNo,
					 ui_action_siteName);
		    ui_action_deleteLink(mainSelPtr->unitNo, selPtr->unitNo,
					 ui_action_siteName);
		}
	    }
	}
    }
    ui_info_makeUnitInfoPanelConsistent(); 
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* show markers */
}


/*****************************************************************************
  FUNCTION : ui_action_linksSet

  PURPOSE  : Set the weight of all links beetween the selected unit. The 
             value will be taken from the unit-info-panel.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_linksSet (void)

{
    struct SelectionType   *selPtr, *mainSelPtr;
    FlintType              weight, newWeight;
    Bool       siteChoosed = ui_action_getSite(UI_GENERAL);
    Bool       performSet  = FALSE;

    if(!ui_infoIsCreated){
	/* No Information to set link available */
	ui_confirmOk("Set Link value in Info Panel first\n");	
	return;
    }


    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* delete markers */
    newWeight = (FlintType) ui_xFloatFromAsciiWidget(ui_linkWidgets.weight);

    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    for (mainSelPtr = ui_sel_listPtr;
	 (mainSelPtr = ui_sel_lookForSelectedItem(mainSelPtr)) != NULL;
	 mainSelPtr = mainSelPtr->nextPtr) { 
	/* main loop */

	for (selPtr = ui_sel_listPtr;
	     (selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL;
	     selPtr = selPtr->nextPtr) { 
	    /* inner loop */
	    
	    (void) krui_setCurrentUnit(mainSelPtr->unitNo); /* target */
	    if (siteChoosed AND (krui_getUnitInputType(mainSelPtr->unitNo) == SITES)) {
		if (krui_setSite(ui_action_siteName) == UI_SET_OK) 
		    performSet = krui_isConnected(selPtr->unitNo);

	    } else
		performSet =
		    ((NOT siteChoosed) AND 
		     (krui_getUnitInputType(mainSelPtr->unitNo) != SITES)AND
		     (krui_isConnected(selPtr->unitNo)));

	    if (performSet) {
		weight = krui_getLinkWeight();
		krui_setLinkWeight(newWeight);
		ui_net_drawLink(ui_currentDisplay, UI_GLOBAL,
				selPtr->unitNo, mainSelPtr->unitNo, 
				weight, UI_ERASE);
		ui_net_drawLink(ui_currentDisplay, UI_GLOBAL,
				selPtr->unitNo, mainSelPtr->unitNo, 
				newWeight, UI_DRAW);
	    }			
	} /* end inner loop */
    } /* end outer loop */
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
}


/*****************************************************************************
  FUNCTION : ui_action_unitsDelete

  PURPOSE  : Delete all selected units.
  RETURNS  : alteration of the network
  NOTES    : The graphic update is made in dependency of the number of units
             in the network and the number of selected units. If there are
	     many units to delete, the whole graphic is built completly new.
	     If there are only a few units to delete, GUI delete all links
	     from and to the selected units and at last the units itsselves.
	     This way is much faster when dealing with big networks and
	     deleting only a few units.

  UPDATE   :
*****************************************************************************/
void ui_action_unitsDelete (void)

{
    struct SelectionType  *selPtr, *tempPtr;
    struct PosType        unitPos;
    Bool                  deleteFewUnits, zExists, clearEntry;
    int                   act_unit;
    d3_unitPtrType        *unitList, *unitPtr;
    int                   i;
    int                   no_of_sel_units;
    int                   *unit_delete_list;
    int  x, y;
   
    if(ui_sel_listPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
	return;
    }

    /* clear all entries in the translation table if a z-value don't exist */

    /* store all z-values in a list */

    selPtr = ui_sel_listPtr;
        
    unitList = NULL;
    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
	krui_getUnitPosition (selPtr->unitNo, &unitPos);
	unitPtr = unitList;
	zExists = FALSE;
	while (unitPtr != NULL) {
	    if (unitPtr->unitNo == unitPos.z) {
		zExists = TRUE;
		break;
	    }
	    unitPtr = unitPtr->next;
	}
	if (! zExists)
	    d3_insertUnit (&unitList, unitPos.z);           
	selPtr  = selPtr->nextPtr;
    }

    deleteFewUnits = ((3 * ui_sel_numberOfSelectedItems) < krui_getNoOfUnits());
        
    selPtr = ui_sel_listPtr;
    no_of_sel_units = ui_sel_numberOfSelectedItems;

    /* Allocate memory for the list of units to be deleted */
    if ((unit_delete_list = calloc(no_of_sel_units, sizeof(int)))==NULL){
	ui_confirmOk("Error! Can't allocate Memory\n");
	return;
    }
        
    i = 0;
    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
	tempPtr = selPtr;
	selPtr  = selPtr->nextPtr;
	ui_sel_unselectOne(ui_currentDisplay, UI_GLOBAL, 
			   tempPtr->unitNo, tempPtr->gridPos); 
	/* kill the selection */
	/* this will take item tempPtr out of the list. Therefore this
	   construction !! */
	if (deleteFewUnits) {
	    /* erase all links from/to this unit in the graphic */
	    ui_net_drawAllLinksToUnit(ui_currentDisplay, UI_GLOBAL, 
				      tempPtr->unitNo, UI_ERASE);
	    ui_net_drawAllLinksFromUnit(ui_currentDisplay, UI_GLOBAL, 
					tempPtr->unitNo, UI_ERASE);
	    /* erase the unit in the graphic */
	    ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL,
			    tempPtr->unitNo, UI_ERASE);
	}

	unit_delete_list[i] = tempPtr->unitNo;
	i++;
            
    }

    ui_checkError(krui_deleteUnitList(no_of_sel_units, unit_delete_list));

    if (NOT deleteFewUnits)
	ui_net_completeRefresh(ui_currentDisplay, UI_GLOBAL);
        
    /* make sure the unit info panel is consistent after deleting units */
    ui_info_makeUnitInfoPanelConsistent();
    ui_sel_msgNumber();

    /* if all units in a z-layer are deleted then clear table entry*/
    unitPtr = unitList;
    while (unitPtr != NULL) {
	clearEntry = TRUE;
	act_unit = krui_getFirstUnit ();
	while (act_unit != 0)  {
	    krui_getUnitPosition (act_unit, &unitPos);
	    if (unitPos.z == unitPtr->unitNo) {
		clearEntry = FALSE;
		break;
	    }
	    act_unit = krui_getNextUnit ();
	}

	if (clearEntry) {
	    x=y=0;
	    krui_xyTransTable( OP_TRANSTABLE_SET, &x, &y, unitPtr->unitNo );
	}    
	unitPtr = unitPtr->next;
    }
    d3_drawNet ();

    free(unit_delete_list);
}


/*****************************************************************************
  FUNCTION : ui_action_setUnitLayers

  PURPOSE  : displays the layer panel and sets the layer of all selected units
  RETURNS  : void
  NOTES    :

  UPDATE   : 5.9.1990
******************************************************************************/

void ui_action_setUnitLayers (void)

{
    Position	               x, y;
    struct SelectionType      *selPtr;

    if ((selPtr = ui_sel_lookForSelectedItem(ui_sel_listPtr)) == NULL) {
	ui_confirmOk("No Units Selected\n");	
	return;
    }

    /* There are selected units. */

    XtTranslateCoords(ui_currentDisplay->widget, 
		      (Position) 5, (Position) 5, &x, &y);
    /* set start value of the buttons */
    ui_layerStartValue = krui_getUnitLayerNo(selPtr->unitNo);

    ui_displayLayerPanel(x,y); /* absolute screen coordinates */


    selPtr = ui_sel_listPtr;
    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
	/* assign return value */
	krui_setUnitLayerNo(selPtr->unitNo, (int) ui_layerReturnValue);
	selPtr = selPtr->nextPtr;
    }
    ui_sel_checkList(ui_currentDisplay, UI_GLOBAL);
    ui_net_completeRefresh(ui_currentDisplay, UI_GLOBAL);
}



/*****************************************************************************
  FUNCTION : ui_action_setUnitAttribute

  PURPOSE  : set one attribute of all selected units to the correspondent 
             value of the SOURCE displayed in the unit-info-panel
  RETURNS  : alteration of the network
  NOTES    : The argument determines the attribute. The following predefined
             constants exist (defined in ui.h):
	     UI_SET_NAME          1
	     UI_SET_TYPE          2
	     UI_SET_ACT           3
	     UI_SET_BIAS          4
	     UI_SET_ACT_FUNC      5

  UPDATE   :
*****************************************************************************/

void ui_action_setUnitAttribute (FlagType whatToSet)

{
    static struct SimpleListType     listStruct;
    static struct UnitAttributeType  unit;

    struct SelectionType    *selPtr;
    Position	            x, y; /* for list panels */
    int                     err,olderr;

    unit = ui_targetUnit;
    ui_info_getDisplayedUnitAttributes(&ui_targetWidgets, &unit);
    listStruct.unitPtr  = &unit;
    listStruct.widgetPtr= NULL;
    XtTranslateCoords(ui_currentDisplay->widget, 
		      (Position) 5, (Position) 5, &x, &y);    
    switch (whatToSet) {
      case UI_SET_IOTYPE:
	sprintf(listStruct.msg,"Set io-type of selected units");
	listStruct.listType = UI_LIST_IOTYPE;
	ui_listPopup((struct SimpleListType *) &listStruct, x,y);
	break;
      case UI_SET_FTYPE:
	sprintf(listStruct.msg,"Set f-type of selected units");
	listStruct.listType = UI_LIST_FTYPE;
	ui_listPopup((struct SimpleListType *) &listStruct, x,y);
	break;
      case UI_SET_ACT_FUNC:
	sprintf(listStruct.msg,"Set activation-function of selected units");
	listStruct.listType = UI_LIST_ACT_FUNC;
	ui_listPopup((struct SimpleListType *) &listStruct, x,y);
	break;
      case UI_SET_OUT_FUNC:
	sprintf(listStruct.msg,"Set output-function of selected units");
	listStruct.listType = UI_LIST_OUT_FUNC;
	ui_listPopup((struct SimpleListType *) &listStruct, x,y);
	break;
    }

    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* erase all selection markers */

    if ((whatToSet == UI_SET_ACT) OR
	(whatToSet == UI_SET_OUT)) {
	/* reset counter in the control panel */
	ui_rem_resetCounter();
    }

    olderr = KRERR_NO_ERROR;
    err = KRERR_NO_ERROR;
    selPtr = ui_sel_listPtr;

    if(selPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
	
	ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL,
			selPtr->unitNo, UI_ERASE);

	switch (whatToSet) {
	  case UI_SET_NAME:
	    err = krui_setUnitName(selPtr->unitNo, unit.name);
	    if (ui_kernelError == KRERR_SYMBOL) {
		ui_correctName(unit.name);
	       err = krui_setUnitName(selPtr->unitNo, unit.name);
	    }
	    break;
	  case UI_SET_IOTYPE:
	    err = krui_setUnitTType(selPtr->unitNo, unit.iotype);
	    break;
	  case UI_SET_FTYPE:
	    err = krui_setUnitFType(selPtr->unitNo, unit.ftype);
	    break;
	  case UI_SET_ACT_FUNC:
	    err = krui_setUnitActFunc(selPtr->unitNo, unit.actFuncName);
	    break;
	  case UI_SET_OUT_FUNC:
	    err = krui_setUnitOutFunc(selPtr->unitNo, unit.outFuncName);
	    break;
	  case UI_SET_ACT:
	    err = krui_setUnitActivation(selPtr->unitNo, unit.act);
	    break;
	  case UI_SET_I_ACT:
	    krui_setUnitInitialActivation(selPtr->unitNo, unit.i_act);
	    break;
	  case UI_SET_OUT:
	    err = krui_setUnitOutput(selPtr->unitNo, unit.out);
	    break;
	  case UI_SET_BIAS:
	    krui_setUnitBias(selPtr->unitNo, unit.bias);
	    break;
	  case UI_FREEZE:
	    err = krui_freezeUnit(selPtr->unitNo);
	    break;
	  case UI_UNFREEZE:
	    err = krui_unfreezeUnit(selPtr->unitNo);
	}
	ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL,
			selPtr->unitNo, UI_DRAW);
	selPtr = selPtr->nextPtr;           /* next list entry        */
	
	if(err != olderr){
	    ui_checkError(err);
	    olderr = err;
	}
    }
    ui_info_makeUnitInfoPanelConsistent();
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* show all selected items */
}


/*****************************************************************************
  FUNCTION : ui_action_unitSetDefaultType

  PURPOSE  : sets the st-attribute of all selected units according to its
             real topological type i.e. 
	     if the unit has input and no output links -> OUTPUT
	     if the unit has output and no input links -> INPUT
	     if the unit has both kinds of links       -> HIDDEN
  RETURNS  : alteration of the network
  NOTES    : 

  UPDATE   : 26.8.90
*****************************************************************************/

void ui_action_unitSetDefaultType (void)

{
    struct SelectionType    *selPtr;
    Bool                    inputExist, outputExist;
    FlintType               weight;
    
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* erase all selection markers */

    selPtr = ui_sel_listPtr;

    if(selPtr == NULL){
	ui_confirmOk("No Units Selected\n");	
    }

    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
	inputExist  = (krui_getUnitInputType(selPtr->unitNo) != NO_INPUTS);
	outputExist = (krui_getFirstSuccUnit(selPtr->unitNo, &weight) > 0);

	if (inputExist == outputExist)
	    ui_checkError(krui_setUnitTType(selPtr->unitNo, HIDDEN));
	else {
	    if (inputExist AND NOT outputExist)
		ui_checkError(krui_setUnitTType(selPtr->unitNo, OUTPUT));
	    else
		ui_checkError(krui_setUnitTType(selPtr->unitNo, INPUT));
	}
	selPtr = selPtr->nextPtr;           /* next list entry        */
    }
    ui_info_makeUnitInfoPanelConsistent();
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); /* show all selected items */
}


/*****************************************************************************
  FUNCTION : ui_action_graphicsDirection

  PURPOSE  : Set the weight of all links beetween the selected unit. The 
             value will be taken from the unit-info-panel.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_action_graphicsDirection (void)

{
    Bool          showLinkFlg;
    Bool          showDirectionFlg;
    FlintType     linkPosTrigger, linkNegTrigger;

    if (ui_key_unitNo <= 0)
	return;
    
    ui_sel_reshowItems(ui_currentDisplay, UI_LOCAL); /* delete markers */

    /* save old setup, set new values and draw all links */
    showLinkFlg      = (ui_currentDisplay->setup).showLinkFlg;
    showDirectionFlg = (ui_currentDisplay->setup).showDirectionFlg;
    linkPosTrigger   = (ui_currentDisplay->setup).linkPosTrigger;
    linkNegTrigger   = (ui_currentDisplay->setup).linkNegTrigger;

    (ui_currentDisplay->setup).showLinkFlg      = TRUE;
    (ui_currentDisplay->setup).showDirectionFlg = TRUE;
    (ui_currentDisplay->setup).linkPosTrigger   = 0.0;
    (ui_currentDisplay->setup).linkNegTrigger   = 0.0;

    ui_net_drawAllLinksToUnit  (ui_currentDisplay, UI_LOCAL, 
				ui_key_unitNo, UI_DRAW);
    ui_net_drawAllLinksFromUnit(ui_currentDisplay, UI_LOCAL, 
				ui_key_unitNo, UI_DRAW);

    /* restore old values */
    (ui_currentDisplay->setup).showLinkFlg      = showLinkFlg;
    (ui_currentDisplay->setup).showDirectionFlg = showDirectionFlg;
    (ui_currentDisplay->setup).linkPosTrigger   = linkPosTrigger;
    (ui_currentDisplay->setup).linkNegTrigger   = linkNegTrigger;

    ui_sel_reshowItems(ui_currentDisplay, UI_LOCAL);
}


/*****************************************************************************
  FUNCTION : ui_action_graphicsGrid

  PURPOSE  : displays a single point on every raster position visible in 
             ui_currentDisplay.
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void ui_action_graphicsGrid (void)

{
    struct PosType pos;

    ui_sel_reshowItems(ui_currentDisplay, UI_LOCAL);
	
    for (pos.x = ui_currentDisplay->origin.x; 
	 pos.x <= ui_currentDisplay->unitsInX; pos.x++)
	for (pos.y = ui_currentDisplay->origin.y; 
	     pos.y <= ui_currentDisplay->unitsInY; pos.y++) {
	    XSetFunction(ui_display, ui_gc, GXcopy);
	    ui_xDrawPoint(ui_display, ui_currentDisplay->drawable, 
			  ui_gc, ui_utilGridToPix(ui_currentDisplay, pos));
	}

    ui_sel_reshowItems(ui_currentDisplay, UI_LOCAL);
}


/*****************************************************************************
  FUNCTION : ui_action_UnitZcoordinate

  PURPOSE  : sets the Z coordinate for the selected units
  RETURNS  : void
  NOTES    : 

  UPDATE   : 28.8.1991
******************************************************************************/

void ui_action_UnitZcoordinate (void)

{
    struct PosType          unitPos;
    struct SelectionType   *selPtr;

    short                   selected_z;
    int                     act_unit;
    Bool                    clearEntry;
    int  x, y;

    if ((selPtr = ui_sel_lookForSelectedItem(ui_sel_listPtr)) == NULL) {
	ui_confirmOk("No Units Selected\n");	
	return;
    }

    /* There are selected units. */

    /* Check wether all units have the same z-value */

    selPtr = ui_sel_listPtr;
    krui_getUnitPosition (selPtr->unitNo, &unitPos);
    selected_z = unitPos.z;
    selPtr = selPtr->nextPtr;
    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
        krui_getUnitPosition (selPtr->unitNo, &unitPos);
        if (unitPos.z != selected_z)  {
             fprintf (stdout, "D3-ERROR: the selected units dont have the same z-value\n\n");
             return;
	}
	selPtr = selPtr->nextPtr;
    }

    /* set selected units to new z */

    selPtr = ui_sel_listPtr;
    while ((selPtr = ui_sel_lookForSelectedItem(selPtr)) != NULL) {
        krui_getUnitPosition (selPtr->unitNo, &unitPos);
        unitPos.z = ui_actualZvalue;
        krui_setUnitPosition (selPtr->unitNo, &unitPos);

	selPtr = selPtr->nextPtr;
    }

    /* move translation table entry */

    krui_xyTransTable( OP_TRANSTABLE_GET, &x, &y, selected_z );
    krui_xyTransTable( OP_TRANSTABLE_SET, &x, &y, ui_actualZvalue );

    /* if there are no more units with the old z-value then clear the old
       translation table entry                                             */

    clearEntry = TRUE;
    act_unit = krui_getFirstUnit ();
    while (act_unit != 0)  {
        krui_getUnitPosition (act_unit, &unitPos);
        if (unitPos.z == selected_z) {
             clearEntry = FALSE;
             break;
	}
        act_unit = krui_getNextUnit ();
    }

    if (clearEntry) {
        x=y=0;
        krui_xyTransTable( OP_TRANSTABLE_SET, &x, &y, selected_z );
    }    
    
    ui_sel_checkList(ui_currentDisplay, UI_GLOBAL);

    ui_net_completeRefresh(ui_currentDisplay, UI_GLOBAL);

}


/*****************************************************************************
  FUNCTION : ui_action_UnitZsetReference

  PURPOSE  : sets the the reference flag in the unit structure
  RETURNS  : void
  NOTES    : 

  UPDATE   : 28.8.1991
******************************************************************************/

void ui_action_UnitZsetReference (void)

{
    struct PosType             unitPos;
    struct SelectionType      *selPtr;
    int  x, y;

    if ((selPtr = ui_sel_lookForSelectedItem(ui_sel_listPtr)) == NULL){ 
	ui_confirmOk("No Units Selected\n");	
	return;
    }

    selPtr = ui_sel_listPtr;

    krui_getUnitPosition (selPtr->unitNo, &unitPos);

    x=unitPos.x - ui_key_eventPos.x;
    y=unitPos.y - ui_key_eventPos.y;
    krui_xyTransTable( OP_TRANSTABLE_SET, &x, &y, ui_actualZvalue );

    ui_sel_checkList(ui_currentDisplay, UI_GLOBAL);

    ui_net_completeRefresh(ui_currentDisplay, UI_GLOBAL);
}






/* end of file */
/* lines: 2143 */

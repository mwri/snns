/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_event.c,v $
  SHORTNAME      : event.c
  SNNS VERSION   : 4.2

  PURPOSE        : event handler for all graphic windows. This handler only 
                   reacts on mouse and window events. A separate handler of 
                   map events is defined as well.
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 15.5.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.9 $ 
  LAST CHANGE    : $Date: 1998/03/03 14:10:23 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <X11/X.h>

#include "ui.h"

#include "glob_typ.h"	/*  Kernel constant and type definitions  */
#include "kr_ui.h"	/*  Kernel interface functions	*/

#include "ui_xGraphic.h"
#include "ui_status.h"
#include "ui_infoP.h"
#include "ui_netUpdate.h"
#include "ui_selection.h" 
#include "ui_utilP.h"
#include "ui_display.h"
#include "ui_action.h"    /* move, copy */
#include "ui_key.h"       /* ui_key_popMenu() */
#include "ui_confirmer.h"
#include "ui_mainP.h"
#include "ui_colEdit.h"
#include "ui_color.h"
#include "ui_info.h"

#include "ui_event.ph"


/*****************************************************************************
  FUNCTION : ui_can_MapEventProc

  PURPOSE  : notifies all map/unmap events
  RETURNS  : void
  NOTES    : sets the component flags (bit 0) in the display structure 
             indicating, whether drawing in this display is neccessary 

  UPDATE   : 20.9.1990
******************************************************************************/

void ui_can_MapEventProc (Widget w,  struct Ui_DisplayType *displayPtr, 
	XEvent    *event)

{

    switch (event->type) {
      case MapNotify:
	ui_utilSetFlag(displayPtr->flags, 1);
	break;
      case UnmapNotify:
	ui_utilResetFlag(displayPtr->flags, 1);
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_mw_eventProc

  PURPOSE  : handles all events caused by working in the canvas subwindow
  RETURNS  : void    lot of sideeffects
  NOTES    : This is the main event handler for all pointer (mouse) events 
             occurring in the canvas pixwin subwindow.
	     Each accepted event forces an action and also a feedback to the 
	     user. This feedback can be:
	     - the cursor changes
	     - a message will occur, what's going on
	     - the position messages (status) below the canvas will be updated
	     - a side effect will occur (i.e. update of the unit-panel)
	     The mouse cursor must be in the window !!
	     * Click down, dragg and release left mouse button will select all
	     units in a rectangle area.             

  UPDATE   :
*****************************************************************************/

void ui_mw_eventProc (Widget w, struct Ui_DisplayType *displayPtr,
	XEvent    *event)

{
    int        unitNo;
    FlintType  weight;        /* holds temporary the  weight of a link */
   
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
    switch (event->type) {
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
      case ButtonPress:
	if (displayPtr->frozen) return; /* don't accept this event here */
	ui_pixPosMouse.x = (int) event->xbutton.x;
	ui_pixPosMouse.y = (int) event->xbutton.y;
	ui_gridPosMouse  = ui_utilPixToGrid(displayPtr, ui_pixPosMouse);
	ui_unselectFlg   = event->xbutton.state BIT_AND ShiftMask;
/* ------------------------------------------------------------------------ */
	switch (event->xbutton.button) {
/* ------------------------------------------------------------------------ */
	  case 1:
	    /* LEFT MOUSE BUTTON PRESSED */
	    if (event->xbutton.state BIT_AND ControlMask) {
		ui_key_popMenu(displayPtr);
		/* ui_confirmOk("Menu"); */
		break;
	    }
	    if (NOT ui_outlineActive) {
		ui_leftButton   = TRUE;  
		/* feedback to user: change cursor */
		/* store this position for later use */
		/* draw a initial box and set the flag */
		ui_selGridPos1   = ui_gridPosMouse;
		ui_selPixPos1    = ui_pixPosMouse;
		ui_selPixPosBox  = ui_pixPosMouse;
		
		XSetFunction(ui_display, ui_gc, GXinvert);
		XSetBackground(ui_display, ui_gc, ui_backgroundColor); /* back */
		XSetForeground(ui_display, ui_gc, ui_textColor); /* text */
		if (ui_col_colorDisplay)
		    XSetPlaneMask(ui_display, ui_gc, 
				  ui_textColor BIT_XOR ui_backgroundColor);
		ui_xDrawBox(ui_display, displayPtr->drawable, 
			    ui_gc, ui_selPixPos1, ui_selPixPosBox);
		if (ui_col_colorDisplay)
		    XSetPlaneMask(ui_display, ui_gc, AllPlanes);
		ui_pixBoxDrawnFlg = TRUE; /* a box is drawn */
	    }
	    break;
/* ------------------------------------------------------------------------ */
	  case 2:
	    /* MIDDLE MOUSE BUTTON PRESSED */
	    if (not ui_outlineActive) {
		ui_middleButton      = TRUE;
		ui_link_sourceUnitNo = 
		    krui_getUnitNoNearPosition(&ui_gridPosMouse, 
					       displayPtr->subNetNo,
					       9, 
					       displayPtr->gridSize);
	    }
	    break;
/* ------------------------------------------------------------------------ */
	  case 3:
	    /* RIGHT MOUSE BUTTON PRESSED */
	    break;
	}
	break;
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
      case ButtonRelease:
	if (displayPtr->frozen) return; /* don't accept this event here */
	ui_pixPosMouse.x = (int) event->xbutton.x;
	ui_pixPosMouse.y = (int) event->xbutton.y;
	ui_gridPosMouse  = ui_utilPixToGrid(displayPtr, ui_pixPosMouse);
	ui_unselectFlg   = event->xbutton.state BIT_AND ShiftMask;
/* ------------------------------------------------------------------------ */
	switch (event->xbutton.button) {
/* ------------------------------------------------------------------------ */
	  case 1:
	    /* LEFT MOUSE BUTTON RELEASED */
	    if (ui_outlineActive) {
		struct PosType offset;
		/* erase outline */
		offset.x = ui_gridPosMouse.x - ui_targetUnit.gridPos.x;
		offset.y = ui_gridPosMouse.y - ui_targetUnit.gridPos.y;
		ui_sel_drawBoxes(displayPtr, UI_GLOBAL, offset);
		ui_outlineActive = FALSE;
		ui_key_eventPos  = ui_gridPosMouse;
		if (ui_key_action == UI_ACTION_MOVE) {
		    ui_printMessage("Units Move ...");
		    ui_action_unitsMove();
		} else {
		    ui_printMessage("Units Copy ...");
		    ui_action_unitsCopy();
		}
		ui_printMessage(">");
		ui_key_currentState = ui_key_returnUnitState;
	    } else {
		ui_leftButton   = FALSE;
		
		/* erase box, store position and normalize */
		/* normalize: gridPos1 will be the upper left corner */
		
		if (ui_pixBoxDrawnFlg) {
		    XSetFunction(ui_display, ui_gc, GXinvert);
		    XSetBackground(ui_display, ui_gc, ui_backgroundColor); /* back */
		    XSetForeground(ui_display, ui_gc, ui_textColor);
		    if (ui_col_colorDisplay)
			XSetPlaneMask(ui_display, ui_gc, 
				      ui_textColor BIT_XOR ui_backgroundColor);
		    ui_xDrawBox(ui_display, displayPtr->drawable, ui_gc, 
				ui_selPixPos1, ui_selPixPosBox);
		    if (ui_col_colorDisplay)
			XSetPlaneMask(ui_display, ui_gc, AllPlanes);
		    ui_pixBoxDrawnFlg = FALSE;
		}
		
		ui_selGridPos2 = ui_gridPosMouse;
		
		ui_utilNormalizeRect(&ui_selGridPos1, &ui_selGridPos2);
		
		/* depending of the shift state: select or unselect */
		
		if (ui_unselectFlg)  /* unselect */
		    
		    if ((NOT (krui_getUnitNoNearPosition(&ui_selGridPos1,
							 displayPtr->subNetNo,
							 9, 
							 displayPtr->gridSize) > 0)) AND
			(ui_utilAreEqualPositions(ui_selGridPos1, ui_selGridPos2))) { 
			/* user clicked a empty position -> select all */
			/* ui_sel_selectAll(...) not implemented */
			ui_stat_displayStatus(ui_gridPosMouse);
		    } else 
			if (ui_utilAreEqualPositions(ui_selGridPos1,
						     ui_selGridPos2)) {
			    int unitNo;
			    if ((unitNo = 
				krui_getUnitNoNearPosition(&ui_selGridPos1,
							   displayPtr->subNetNo,
							   9, 
							   displayPtr->gridSize)) > 0) {
				krui_getUnitPosition(unitNo, &ui_selGridPos1);
				ui_sel_unselectOne(displayPtr, UI_GLOBAL,
						   unitNo, ui_selGridPos1);
			    }
			} else
			    ui_sel_unselectRect(displayPtr, UI_GLOBAL,
						ui_selGridPos1, ui_selGridPos2); 
		else  /* select */
		    if (ui_utilAreEqualPositions(ui_selGridPos1,
						 ui_selGridPos2)) {
			int unitNo;
			if ((unitNo = 
			     krui_getUnitNoNearPosition(&ui_selGridPos1,
							displayPtr->subNetNo,
							9, 
							displayPtr->gridSize)) > 0) {
			    krui_getUnitPosition(unitNo, &ui_selGridPos1);
			    ui_sel_selectOne(displayPtr, UI_GLOBAL,
					     unitNo, ui_selGridPos1);
			}
		    } else
			ui_sel_selectRect(displayPtr, UI_GLOBAL,
					  ui_selGridPos1, ui_selGridPos2); 
		
		ui_leftButton = FALSE;  /* signal: end of operation */
	    }
	    break;
/* ------------------------------------------------------------------------ */
	  case 2:
	    /* MIDDLE MOUSE BUTTON RELEASED */
	    if ((NOT ui_outlineActive) AND ui_middleButton) {
		int targetUnitNo;

		if ((targetUnitNo = 
		     krui_getUnitNoNearPosition(&ui_gridPosMouse,
						displayPtr->subNetNo,
						9, 
						displayPtr->gridSize)) > 0) { 
		    /* a target unit exist */
		    if (ui_link_sourceUnitNo == 0)
			/* if pressed on an empty position then use
			   old SOURCE instead ! */
			ui_link_sourceUnitNo = ui_sourceUnit.no;

		    /* show data in the unit info panel */
		    ui_info_showSelectedUnit(targetUnitNo);

		    krui_setCurrentUnit(targetUnitNo);
		    if (krui_isConnected(ui_link_sourceUnitNo)) { 
			/* if the source unit exists and already connected 
			   then show all data of the Link */
			weight = krui_getLinkWeight();
			if( ui_infoIsCreated )
			    ui_info_showPredOfTargetUnit(ui_link_sourceUnitNo, 
							 weight);
		    }
		} else { /* released on empty position */
		    if (ui_link_sourceUnitNo != 0) {
			/* pressed on a unit position */
			/* try to display the connection source -> TARGET */
			krui_setCurrentUnit(ui_targetUnit.no);
			if (krui_areConnectedWeight(ui_link_sourceUnitNo,
						    ui_targetUnit.no,&weight) 
			    AND ui_infoIsCreated) { 
			    /* if the source unit exists and already connected 
			       then show all data of the Link */
			    ui_info_showPredOfTargetUnit(ui_link_sourceUnitNo,
							 weight);
			}
		    }
		}
	    }
	    ui_middleButton = FALSE;
	    break;
/* ------------------------------------------------------------------------ */
	  case 3:
	    /* RIGHT MOUSE BUTTON RELEASED */
	    if (NOT ui_outlineActive) {
		struct PosType  gridPos;
		if ((unitNo = 
		    krui_getUnitNoNearPosition(&ui_gridPosMouse,
					       displayPtr->subNetNo,
					       9, 
					       displayPtr->gridSize)) > 0) {
		    krui_getUnitPosition(unitNo, &gridPos);
		    ui_sel_unselectOne(displayPtr, UI_GLOBAL, unitNo, gridPos); 
		    /* unselectOne gives not a message by itsself ! */
		    ui_sel_msgNumber(); 
		} else { /* user clicked on an empty position */
		    ui_sel_unselectAll(displayPtr, UI_GLOBAL); 
		    ui_stat_displayStatus(ui_gridPosMouse);
		}
	    }
	    break;
/* ------------------------------------------------------------------------ */
	}
	break;
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
      case MotionNotify:
	if (displayPtr->frozen) return; /* don't accept this event here */

	ui_pixPosMouse.x = (int) event->xbutton.x;
	ui_pixPosMouse.y = (int) event->xbutton.y;
	ui_gridPosMouse  = ui_utilPixToGrid(displayPtr, ui_pixPosMouse);

	if (ui_utilAreDifferentPositions(ui_gridPosOld, ui_gridPosMouse)) {
	    ui_stat_displayStatus(ui_gridPosMouse);

	    if (ui_outlineActive) {
		struct PosType offset;
		offset.x = ui_gridPosOld.x - ui_targetUnit.gridPos.x;
		offset.y = ui_gridPosOld.y - ui_targetUnit.gridPos.y;
		ui_sel_drawBoxes(displayPtr, UI_GLOBAL, offset);
		offset.x = ui_gridPosMouse.x - ui_targetUnit.gridPos.x;
		offset.y = ui_gridPosMouse.y - ui_targetUnit.gridPos.y;
		ui_sel_drawBoxes(displayPtr, UI_GLOBAL, offset);
	    }
	}

	if ((NOT ui_outlineActive) AND (event->xmotion.state AND 
					(Button1Mask | Button2Mask | 
					 Button3Mask))) {
	    /* at least one of the three mouse buttons is depressed !  
	       ==> MOUSE DRAGG */ 
	    
	    /* only with left button down */
	    if (ui_leftButton) {
		/* draw rubber square on screen while dragging the mouse */
		/* Because two boxes are drawn, don't change the flag
		   ui_pixBoxDrawnFlg ! */
		XSetFunction(ui_display, ui_gc, GXinvert);
		XSetBackground(ui_display, ui_gc, ui_backgroundColor); /* back */
		XSetForeground(ui_display, ui_gc, ui_textColor);
		if (ui_col_colorDisplay)
		    XSetPlaneMask(ui_display, ui_gc, 
				  ui_textColor BIT_XOR ui_backgroundColor);
		ui_xDrawBox(ui_display, displayPtr->drawable, 
			    ui_gc, ui_selPixPos1, ui_selPixPosBox);
		ui_selPixPosBox = ui_pixPosMouse; 
		ui_xDrawBox(ui_display, displayPtr->drawable, 
			    ui_gc, ui_selPixPos1, ui_selPixPosBox);
		if (ui_col_colorDisplay)
		    XSetPlaneMask(ui_display, ui_gc, AllPlanes);
		ui_pixBoxDrawnFlg = TRUE; /* a box is drawn */
	    }
	}
	break;
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
      case EnterNotify:
	/* mouse cursor moved into canvas subwindow */
	if (displayPtr->frozen) return; /* don't accept this event here */
	ui_pixPosMouse.x = (int) event->xbutton.x;
	ui_pixPosMouse.y = (int) event->xbutton.y;
	ui_gridPosMouse  = ui_utilPixToGrid(displayPtr, ui_pixPosMouse);
	ui_stat_displayStatus(ui_gridPosMouse);
	ui_currentDisplay = displayPtr;
	if (ui_outlineActive) {
	    struct PosType offset;
	    offset.x = ui_gridPosMouse.x - ui_targetUnit.gridPos.x;
	    offset.y = ui_gridPosMouse.y - ui_targetUnit.gridPos.y;
	    ui_sel_drawBoxes(displayPtr, UI_GLOBAL, offset);
#ifdef DEBUG
	    XFlush(ui_display);
#endif
	}	
	break;
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
      case LeaveNotify: 
	/* mouse cursor moved out of canvas subwindow */
	if (displayPtr->frozen) return; /* don't accept this event here */
	if (ui_outlineActive) {
	    struct PosType offset;
	    offset.x = ui_gridPosOld.x - ui_targetUnit.gridPos.x;
	    offset.y = ui_gridPosOld.y - ui_targetUnit.gridPos.y;
	    ui_sel_drawBoxes(displayPtr, UI_GLOBAL, offset);
#ifdef DEBUG
	    XFlush(ui_display);
#endif
	}
	break;
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
    }
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
    
    ui_pixPosOld  = ui_pixPosMouse; /* store pos for the next call */
    ui_gridPosOld = ui_gridPosMouse;
}





/* end of file */
/* lines: 479 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_key.c,v $
  SHORTNAME      : key.h
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 6.6.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:31 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ui.h"

#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include "ui_action.h"
#include "ui_utilP.h"
#include "ui_status.h" 
#include "ui_event.h"

#include "glob_typ.h"	/*  Kernel constant and type definitions  */
#include "kr_ui.h"	/*  Kernel interface functions	*/

#include "ui_mainP.h"
#include "ui_confirmer.h"
#include "ui_display.h"
#include "ui_xGraphic.h"
#include "ui_selection.h"
#include "ui_netUpdate.h"
#include "ui_info.h"
#include "ui_control.h"
#include "ui_controlP.h"
#include "d3_pannels.h"
#include "ui_displwght.h"
#include "ui_displmap.h"
#include "o_graph.h"
#include "ui_kohonen.h"
#include "net_analyzer.h"

#include "ui_key.ph"


/*****************************************************************************
  FUNCTION : ui_can_setBusyCursor   (NOT IMPLEMENTED)

  PURPOSE  :
  RETURNS  : void
  NOTES    : 

  UPDATE   : 
******************************************************************************/

static void ui_can_setBusyCursor(void) /* not implemented */

{}


/*****************************************************************************
  FUNCTION : ui_can_setNormalCursor   (NOT IMPLEMENTED)

  PURPOSE  :
  RETURNS  :
  NOTES    : 

  UPDATE   : 
******************************************************************************/

static void ui_can_setNormalCursor(void) /* not implemented */

{}


/*****************************************************************************
  FUNCTION : ui_key_init()

  PURPOSE  :
  RETURNS  : void
  NOTES    : It would be simpler to use normal aggregate initialization, but
             this version is independent from the assignment of the STATE
	     constants!

  UPDATE   : 20.9.1990
******************************************************************************/

void ui_key_init (void)

{
    ui_commands[UI_START_STATE][0] = "Units     ~";
    ui_commands[UI_START_STATE][1] = "Links     ~";
    ui_commands[UI_START_STATE][2] = "Sites     ~";
    ui_commands[UI_START_STATE][3] = "Graphics  ~";
    ui_commands[UI_START_STATE][4] = "Mode      ~";
    ui_commands[UI_START_STATE][5] = "Flags     ~";
    ui_commands[UI_START_STATE][6] = "Help";

    ui_commands[UI_STATE_MODE][0] = "Units";
    ui_commands[UI_STATE_MODE][1] = "Links";
    ui_commands[UI_STATE_MODE][2] = "Quit";
    ui_commands[UI_STATE_MODE][3] = "Help";

    ui_commands[UI_STATE_UNITS][0] = "Set       ~";
    ui_commands[UI_STATE_UNITS][1] = "Copy      ~";
    ui_commands[UI_STATE_UNITS][2] = "Delete";
    ui_commands[UI_STATE_UNITS][3] = "Insert    ~";
    ui_commands[UI_STATE_UNITS][4] = "Move      T";
    ui_commands[UI_STATE_UNITS][5] = "Freeze";
    ui_commands[UI_STATE_UNITS][6] = "Unfreeze";
    ui_commands[UI_STATE_UNITS][7] = "Return";
    ui_commands[UI_STATE_UNITS][8] = "Quit";
    ui_commands[UI_STATE_UNITS][9] = "Help";
    ui_commands[UI_STATE_UNITS][10] ="3D        ~";

    ui_commands[UI_STATE_UNITS_INSERT][0] = "Default unit  !";
    ui_commands[UI_STATE_UNITS_INSERT][1] = "Target unit   !";
    ui_commands[UI_STATE_UNITS_INSERT][2] = "F-type unit   !";
    ui_commands[UI_STATE_UNITS_INSERT][3] = "Return";
    ui_commands[UI_STATE_UNITS_INSERT][4] = "Quit";
    ui_commands[UI_STATE_UNITS_INSERT][5] = "Help";

    ui_commands[UI_STATE_LINKS][0] = "Set";
    ui_commands[UI_STATE_LINKS][1] = "Copy    ~";
    ui_commands[UI_STATE_LINKS][2] = "Delete  ~";
    ui_commands[UI_STATE_LINKS][3] = "Make    ~";
    ui_commands[UI_STATE_LINKS][4] = "Return";
    ui_commands[UI_STATE_LINKS][5] = "Quit";
    ui_commands[UI_STATE_LINKS][6] = "Help";

    ui_commands[UI_STATE_FLAG][0] = "Safety";
    ui_commands[UI_STATE_FLAG][1] = "Quit";
    ui_commands[UI_STATE_FLAG][2] = "Help";

    ui_commands[UI_STATE_LINKS_MAKE][0] = "from Source unit  !";
    ui_commands[UI_STATE_LINKS_MAKE][1] = "to Target unit    !";
    ui_commands[UI_STATE_LINKS_MAKE][2] = "Clique";
    ui_commands[UI_STATE_LINKS_MAKE][3] = "Inverse";
    ui_commands[UI_STATE_LINKS_MAKE][4] = "Double";
    ui_commands[UI_STATE_LINKS_MAKE][5] = "Return";
    ui_commands[UI_STATE_LINKS_MAKE][6] = "Quit";
    ui_commands[UI_STATE_LINKS_MAKE][7] = "Help";

    ui_commands[UI_STATE_UNITS_COPY][0] = "All";
    ui_commands[UI_STATE_UNITS_COPY][1] = "Input";
    ui_commands[UI_STATE_UNITS_COPY][2] = "Output";
    ui_commands[UI_STATE_UNITS_COPY][3] = "None";
    ui_commands[UI_STATE_UNITS_COPY][4] = "Structure  ~";
    ui_commands[UI_STATE_UNITS_COPY][5] = "Return";
    ui_commands[UI_STATE_UNITS_COPY][6] = "Quit";
    ui_commands[UI_STATE_UNITS_COPY][7] = "Help";

    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][0] = "All";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][1] = "Input";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][2] = "Output";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][3] = "None";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][4] = "Back binding";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][5] = "Forward binding";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][6] = "Double binding";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][7] = "Return";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][8] = "Quit";
    ui_commands[UI_STATE_UNITS_COPY_STRUCTURE][9] = "Help";

    ui_commands[UI_STATE_LINKS_COPY][0] = "All          T";
    ui_commands[UI_STATE_LINKS_COPY][1] = "Input        T";
    ui_commands[UI_STATE_LINKS_COPY][2] = "Output       T";
    ui_commands[UI_STATE_LINKS_COPY][3] = "None         T";
    ui_commands[UI_STATE_LINKS_COPY][4] = "Environment  T";
    ui_commands[UI_STATE_LINKS_COPY][5] = "Return";
    ui_commands[UI_STATE_LINKS_COPY][6] = "Quit";
    ui_commands[UI_STATE_LINKS_COPY][7] = "Help";

    ui_commands[UI_STATE_LINKS_DELETE][0] = "from Source unit  !";
    ui_commands[UI_STATE_LINKS_DELETE][1] = "to Target unit    !";
    ui_commands[UI_STATE_LINKS_DELETE][2] = "Clique";
    ui_commands[UI_STATE_LINKS_DELETE][3] = "Return";
    ui_commands[UI_STATE_LINKS_DELETE][4] = "Quit";
    ui_commands[UI_STATE_LINKS_DELETE][5] = "Help";

    ui_commands[UI_STATE_UNITS_SET][ 0] = "Activation          T";
    ui_commands[UI_STATE_UNITS_SET][ 1] = "Initial activation  T";
    ui_commands[UI_STATE_UNITS_SET][ 2] = "Output              T";
    ui_commands[UI_STATE_UNITS_SET][ 3] = "Bias                T";
    ui_commands[UI_STATE_UNITS_SET][ 4] = "Name                T";
    ui_commands[UI_STATE_UNITS_SET][ 5] = "io-Type             T";
    ui_commands[UI_STATE_UNITS_SET][ 6] = "Default io-type";
    ui_commands[UI_STATE_UNITS_SET][ 7] = "Layers";
    ui_commands[UI_STATE_UNITS_SET][ 8] = "Function            ~";
    ui_commands[UI_STATE_UNITS_SET][ 9] = "Return";
    ui_commands[UI_STATE_UNITS_SET][10] = "Quit";
    ui_commands[UI_STATE_UNITS_SET][11] = "Help";

    ui_commands[UI_STATE_UNITS_SET_FUNCTION][0] = "Activation func";
    ui_commands[UI_STATE_UNITS_SET_FUNCTION][1] = "Output func";
    ui_commands[UI_STATE_UNITS_SET_FUNCTION][2] = "F-type";
    ui_commands[UI_STATE_UNITS_SET_FUNCTION][3] = "Return";
    ui_commands[UI_STATE_UNITS_SET_FUNCTION][4] = "Quit";
    ui_commands[UI_STATE_UNITS_SET_FUNCTION][5] = "Help";

    ui_commands[UI_STATE_GRAPHIC][ 0] = "Origin     !";
    ui_commands[UI_STATE_GRAPHIC][ 1] = "Move       T";
    ui_commands[UI_STATE_GRAPHIC][ 2] = "All";
    ui_commands[UI_STATE_GRAPHIC][ 3] = "Complete";
    ui_commands[UI_STATE_GRAPHIC][ 4] = "Units";
    ui_commands[UI_STATE_GRAPHIC][ 5] = "Links";
    ui_commands[UI_STATE_GRAPHIC][ 6] = "Direction  !";
    ui_commands[UI_STATE_GRAPHIC][ 7] = "Grid";
    ui_commands[UI_STATE_GRAPHIC][ 8] = "Return";
    ui_commands[UI_STATE_GRAPHIC][ 9] = "Quit";
    ui_commands[UI_STATE_GRAPHIC][10] = "Help";

    ui_commands[UI_STATE_GETDEST][0] = "Return";
    ui_commands[UI_STATE_GETDEST][1] = "Quit";

    ui_commands[UI_STATE_SITES][0] = "Add";
    ui_commands[UI_STATE_SITES][1] = "Delete";
    ui_commands[UI_STATE_SITES][2] = "Copy    ~";
    ui_commands[UI_STATE_SITES][3] = "Quit";
    ui_commands[UI_STATE_SITES][4] = "Help";

    ui_commands[UI_STATE_SITES_COPY][0] = "with No links";
    ui_commands[UI_STATE_SITES_COPY][1] = "with All links";
    ui_commands[UI_STATE_SITES_COPY][3] = "Quit";
    ui_commands[UI_STATE_SITES_COPY][4] = "Help";

    ui_commands[UI_STATE_UNITS_3D][0] = "Z";
    ui_commands[UI_STATE_UNITS_3D][1] = "Move";
    ui_commands[UI_STATE_UNITS_3D][2] = "Quit";
    ui_commands[UI_STATE_UNITS_3D][3] = "Help";
}


/*****************************************************************************
  FUNCTION : ui_key_abortOutlineAction

  PURPOSE  : finish the current action (move or copy) with a outline of all
             selected units
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

static void ui_key_abortOutlineAction (void)

{
    struct PosType offset;

    ui_outlineActive    = FALSE;

    /* erase outline */
    offset.x = ui_key_eventPos.x - ui_targetUnit.gridPos.x;
    offset.y = ui_key_eventPos.y - ui_targetUnit.gridPos.y;
    ui_sel_drawBoxes(ui_currentDisplay, UI_GLOBAL, offset);
}


/*****************************************************************************
  FUNCTION : ui_key_startOutlineAction

  PURPOSE  : start a action (move or copy) and draw a outline of all selected
             units
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

static void ui_key_startOutlineAction (void)

{
    struct PosType offset;

    ui_key_currentState = UI_STATE_GETDEST;
    ui_outlineActive    = TRUE;

    /* draw initial outline */
    offset.x = ui_key_eventPos.x - ui_targetUnit.gridPos.x;
    offset.y = ui_key_eventPos.y - ui_targetUnit.gridPos.y;
    ui_sel_drawBoxes(ui_currentDisplay, UI_GLOBAL, offset);
}


/*****************************************************************************
  FUNCTION : ui_key_automata

  PURPOSE  : The automata is realized in a big switch statement.
             Further information: see ui_can_KEventProc
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

static void ui_key_automata (struct Ui_DisplayType *displayPtr, 
				struct PosType eventPixPos, char key)

{
    char            string[50];
    char            prefix[25];

    ui_key_currentKey = key;
    ui_key_eventPos   = ui_utilPixToGrid(displayPtr, eventPixPos);
    ui_key_unitNo     =
	krui_getUnitNoNearPosition(&ui_key_eventPos, 
				   displayPtr->subNetNo,
				   9, 
				   displayPtr->gridSize);
    ui_currentDisplay = displayPtr; /* for action routines */
    
    switch (ui_key_currentState) {

       /* ------------------------------------------------------------------- */
	
      case UI_START_STATE:
	
	switch (ui_key_currentKey) {
	  case 'f': /* Flags */
	    ui_key_currentState     = UI_STATE_FLAG; 
	    ui_key_returnFlagState  = UI_START_STATE;
	    break;
	  case 'm': /* Mode */
	    ui_key_currentState     = UI_STATE_MODE; 
	    break;
	  case 'u': /* Units */
	    ui_key_currentState     = UI_STATE_UNITS; 
	    ui_key_returnUnitState  = UI_START_STATE; /* return top level */
	    break;
	  case 'l': /* Links */
	    ui_key_currentState     = UI_STATE_LINKS; 
	    ui_key_returnLinkState  = UI_START_STATE; /* return top level */
	    break;
	  case 'g': /* Graphic */
	    ui_key_currentState     = UI_STATE_GRAPHIC;
	    break;
	  case 's': /* Sites */
	    ui_key_currentState     = UI_STATE_SITES;
	    break;
	  case 'h': /* Help */
	    sprintf(string,"HELP: Keyboard.");
	    ui_printMessage(string);
	    ui_displayHelp("* Keyboard");
	    break;
	  default:
	    ui_key_currentState     = UI_START_STATE; /* stay here */
	}   
	
	break;
	
       /* ------------------------------------------------------------------- */
	
      case UI_STATE_MODE: 
	
	switch (ui_key_currentKey) {
	  case 'u': /* Units */
	    ui_key_currentState     = UI_STATE_UNITS; 
	    ui_key_returnUnitState  = UI_STATE_UNITS; /* stay UNITS mode */
	    break;
	  case 'l': /* Links */
	    ui_key_currentState     = UI_STATE_LINKS; 
	    ui_key_returnLinkState  = UI_STATE_LINKS; /* stay in LINK mode */
	    break;
	  case 'q': /* quit */
	    ui_key_currentState = UI_START_STATE;
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Mode.");
	    ui_printMessage(string);
	    ui_displayHelp("* Mode");
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_MODE; /* stay here */
	}
	
	break;

       /* ------------------------------------------------------------------- */

      case UI_STATE_UNITS: 
	
	switch (ui_key_currentKey) {
	  case 'c':
	    ui_key_currentState    = UI_STATE_UNITS_COPY; 
	    break;
	  case 's':
	    ui_key_currentState     = UI_STATE_UNITS_SET;
	    break;
	  case 'd':
	    ui_key_currentState     = ui_key_returnUnitState;
	    if (NOT ui_safetyFlg OR 
		ui_confirmYes("Units Delete. Are you sure?")) {
		sprintf(string,"Units Delete.");
		ui_printMessage(string);
		ui_can_setBusyCursor();
		ui_action_unitsDelete();
		ui_can_setNormalCursor();
		ui_net_completeRefresh(displayPtr, UI_GLOBAL);
	    }
	    break;
	  case 'i':
	    /* no message, because operation is very fast */
	    ui_key_currentState     = UI_STATE_UNITS_INSERT;
	    break;
	  case 'm':
	    sprintf(string,"Units Move.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_MOVE;
	    ui_key_startOutlineAction();
	    break;
	  case 'f': 
	    sprintf(string,"Units Freeze.");
	    ui_printMessage(string);
	    ui_key_currentState     = ui_key_returnUnitState;
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_FREEZE);
	    ui_can_setNormalCursor();
	    break;
	  case 'u': 
	    sprintf(string,"Units Unfreeze.");
	    ui_printMessage(string);
	    ui_key_currentState     = ui_key_returnUnitState;
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_UNFREEZE);
	    ui_can_setNormalCursor();
	    break;
	  case 'r': /* return */
	    ui_key_currentState     = UI_START_STATE; 
	    break;
	  case 'q': /* quit */
	    if (ui_key_returnUnitState != UI_STATE_UNITS) /* return top level */
		ui_key_currentState = ui_key_returnUnitState;
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Units.");
	    ui_printMessage(string);
	    ui_displayHelp("* Units");
	    break;
	  case '3': 
	    ui_key_currentState    = UI_STATE_UNITS_3D; 
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_UNITS; /* stay here */
	}
	break;

      /* ------------------------------------------------------------------- */

      case UI_STATE_UNITS_INSERT: 
	
	switch (ui_key_currentKey) {
	  case 'd': /* default unit */
	    ui_key_currentState = ui_key_returnUnitState;
	    ui_action_unitInsert(UI_INSERT_DEFAULT);
	    break;
	  case 't': /* target unit */
	    ui_key_currentState = ui_key_returnUnitState;
	    ui_action_unitInsert(UI_INSERT_TARGET);
	    break;
	  case 'f': /* ftype unit */
	    ui_key_currentState = ui_key_returnUnitState;
	    ui_action_unitInsert(UI_INSERT_FTYPE);
	    break;
	  case 'r': /* return */
	    ui_key_currentState     = UI_START_STATE; 
	    break;
	  case 'q': /* quit */
	    if (ui_key_returnUnitState != UI_STATE_UNITS) /* return top level */
		ui_key_currentState = ui_key_returnUnitState;
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Units Insert.");
	    ui_printMessage(string);
	    ui_displayHelp("* Units Insert");
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_UNITS_INSERT; /* stay here */
	}
	break;

       /* ------------------------------------------------------------------- */
	
      case UI_STATE_LINKS: 
	
	switch (ui_key_currentKey) {
	  case 'c':
	    ui_key_currentState    = UI_STATE_LINKS_COPY; 
	    break;
	  case 'd':
	    ui_key_currentState    = UI_STATE_LINKS_DELETE;
	    break;
	  case 'm':
	    ui_key_currentState = UI_STATE_LINKS_MAKE; 
	    break;
	  case 's': /* Links Set */
	    ui_key_currentState = ui_key_returnLinkState;
	    ui_can_setBusyCursor();
	    ui_action_linksSet();
	    ui_can_setNormalCursor();
	    break;
	  case 'r':
	    ui_key_currentState  = UI_START_STATE; 
	    break;
	  case 'q': /* quit */
	    ui_key_currentState = ui_key_returnLinkState;;
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Links."); 
	    ui_printMessage(string);
	    ui_displayHelp("* Links");
	    break;
	  default:
	    ui_key_currentState = UI_STATE_LINKS; /* stay here */
	}
	break;
	
       /* ------------------------------------------------------------------- */

      case UI_STATE_FLAG: 
	
	switch (ui_key_currentKey) {
	    
	  case 's': /* safety flag (confirm when destructive operation) */
	    ui_key_currentState = ui_key_returnFlagState;
	    ui_safetyFlg = (NOT ui_safetyFlg);
	    ui_stat_displayStatus(ui_key_eventPos);
	    break;
	    
	  case 'q': /* quit */
	    ui_key_currentState = ui_key_returnFlagState;
	    break;
	    
	  case 'h': /* help flags */
	    sprintf(string,"HELP: Flags.");
	    ui_printMessage(string);
	    ui_displayHelp("* Flags");
	    break;

	  default:
	    ui_key_currentState     = UI_STATE_FLAG; /* stay here */
	}
	break;

     /* ------------------------------------------------------------------- */
	
      case UI_STATE_LINKS_MAKE: 
	
	switch (ui_key_currentKey) {
	  case 'h': /* help */
	    sprintf(string,"HELP: Links Make.");
	    ui_printMessage(string);
	    ui_displayHelp("* Links Make");
	    break;
	  case 'c':
	    sprintf(string,"Links Make Clique.");
	    ui_printMessage(string);
	    ui_key_currentState = ui_key_returnLinkState;
	    ui_can_setBusyCursor();
	    ui_action_linksMakeClique(UI_DRAW);
	    ui_can_setNormalCursor();
	    break;
	  case 'i':
	    if (NOT ui_safetyFlg OR
		ui_confirmYes("Links Make Inverse. Are you sure?")) {
		sprintf(string,"Links Make Inverse.");
		ui_printMessage(string);
		ui_key_currentState = ui_key_returnLinkState;
		ui_can_setBusyCursor();
		ui_action_linksMakeReverse();
		ui_can_setNormalCursor();
	    }
	    break;
	  case 'd':
	    sprintf(string,"Links Make Double.");
	    ui_printMessage(string);
	    ui_key_currentState = ui_key_returnLinkState;
	    ui_can_setBusyCursor();
	    ui_action_linksMakeBidirectional();
	    ui_can_setNormalCursor();
	    break;
	  case 's':
	    sprintf(string,"Links Make to Source unit.");
	    ui_printMessage(string);
	    ui_key_currentState = ui_key_returnLinkState;
	    ui_can_setBusyCursor();
	    ui_action_linksMakeFromSourceUnit(UI_DRAW);
	    ui_can_setNormalCursor();
	    break;
	  case 't':
	    sprintf(string,"Links Make to Target unit.");
	    ui_printMessage(string);
	    ui_key_currentState = ui_key_returnLinkState;
	    ui_can_setBusyCursor();
	    ui_action_linksMakeToTargetUnit(UI_DRAW);
	    ui_can_setNormalCursor();
	    break;
	  case 'r':
	    ui_key_currentState = UI_START_STATE;
	    break;
	  case 'q': /* quit */
	    ui_key_currentState = ui_key_returnLinkState;;
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_LINKS_MAKE; /* stay here */
	}
	break;
	
      /* ------------------------------------------------------------------- */
  
      case UI_STATE_UNITS_COPY: 


	switch (ui_key_currentKey) {
	  case 'q': /* quit */
	    ui_key_currentState = ui_key_returnUnitState;
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Units Copy.");
	    ui_printMessage(string);
	    ui_displayHelp("* Units Copy");
	    break; 
	  case 'a':
	    ui_key_linksToHandle = UI_ALL;
	    sprintf(string,"Units Copy All.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'i':
	    ui_key_linksToHandle = UI_INPUT;
	    sprintf(string,"Units Copy Input.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'o':
	    ui_key_linksToHandle = UI_OUT;
	    sprintf(string,"Units Copy Output.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'n': /* without copying any links */
	    ui_key_linksToHandle = UI_NO_FLAG;
	    sprintf(string,"Units Copy None.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 's': /* structureCopy */
	    ui_key_currentState  = UI_STATE_UNITS_COPY_STRUCTURE;
	    ui_key_linksToHandle = UI_NO_FLAG;
	    break;
	  case 'r':
	    ui_key_currentState  = UI_START_STATE; 
	    break;
	  default:
	    ui_key_currentState   = UI_STATE_UNITS_COPY; /* stay here */
	}
	break;

      /* ------------------------------------------------------------------- */
  
      case UI_STATE_UNITS_COPY_STRUCTURE: 
	
	if (ui_key_currentKey != 'h') {
	    ui_key_currentState = ui_key_returnUnitState;
	}	
	
	switch (ui_key_currentKey) {
	  case 'q': /* quit */
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Units Copy Structure.");
	    ui_printMessage(string);
	    ui_displayHelp("* Units Copy Structure");
	    break;
	  case 'a':
	    sprintf(string,"Units Copy Structure All.");
	    ui_printMessage(string);
	    ui_key_linksToHandle = UI_STRUCTURE_COPY + UI_ALL;
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'i':
	    ui_key_linksToHandle = UI_STRUCTURE_COPY + UI_INPUT;
	    sprintf(string,"Units Copy Structure Input.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'o':
	    ui_key_linksToHandle = UI_STRUCTURE_COPY + UI_OUT;
	    sprintf(string,"Units Copy Structure Output.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'n': /* without copying any links */
	    ui_key_linksToHandle = UI_STRUCTURE_COPY + UI_NO_FLAG;
	    sprintf(string,"Units Copy Structure None.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'b': /* with back binding */
	    ui_key_linksToHandle = UI_STRUCTURE_COPY + UI_BACKWARD_BINDING;
	    sprintf(string,"Units Copy Structure Back.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'f': /* with forward binding */
	    ui_key_linksToHandle = UI_STRUCTURE_COPY + UI_FORWARD_BINDING;
	    sprintf(string,"Units Copy Structure Forward.");
	    ui_printMessage(string);
	    ui_key_action = UI_ACTION_COPY;
	    ui_key_startOutlineAction();
	    break;
	  case 'd': /* with two-way binding (bidirectional) */
	    ui_key_linksToHandle = UI_STRUCTURE_COPY + UI_TWOWAY_BINDING;
	    sprintf(string,"Units Copy Structure Double.");
	    ui_printMessage(string);
	    ui_key_startOutlineAction();
	    ui_key_action = UI_ACTION_COPY;
	    break;
	  case 'r':
	    ui_key_currentState  = UI_START_STATE; 
	    break;
	  default:
	    ui_key_currentState  = UI_STATE_UNITS_COPY_STRUCTURE; 
	    /* stay here */
	}
	break;

       /* ------------------------------------------------------------------- */

      case UI_STATE_LINKS_COPY: 


	if (ui_key_currentKey != 'h') {
	    ui_key_currentState = ui_key_returnLinkState;
	}	

	
	switch (ui_key_currentKey) /* */  {
	  case 'q': /* quit */
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Links Copy.");
	    ui_printMessage(string);
	    ui_displayHelp("* Links Copy");
	    break;
	  case 'e':
	    ui_can_setBusyCursor();
	    ui_action_linksCopyEnvironment();
	    ui_can_setNormalCursor();
	    break;
	  case 'a':
	    ui_key_linksToHandle = UI_ALL;
	    ui_can_setBusyCursor();
	    ui_action_linksCopy();
	    ui_can_setNormalCursor();
	    break;
	  case 'i':
	    ui_key_linksToHandle = UI_INPUT;
	    ui_can_setBusyCursor();
	    ui_action_linksCopy();
	    ui_can_setNormalCursor();
	    break;
	  case 'o':
	    ui_key_linksToHandle = UI_OUT;
	    ui_can_setBusyCursor();
	    ui_action_linksCopy();
	    ui_can_setNormalCursor();
	    break;
	  case 'n':
	    ui_key_linksToHandle = UI_NO_FLAG;
	    ui_can_setBusyCursor();
	    ui_action_linksCopy();
	    ui_can_setNormalCursor();
	    break;
	  case 'r':
	    ui_key_currentState  = UI_START_STATE; 
	    break;
	  default:
	    ui_key_currentState  = UI_STATE_LINKS_COPY; /* stay here */
	}
	break;


       /* ------------------------------------------------------------------- */
  
      case UI_STATE_LINKS_DELETE: 

	if (ui_key_currentKey != 'h') {
	    ui_key_currentState = ui_key_returnLinkState;
	}	
	
	switch (ui_key_currentKey) /* */  {
	  case 'q': /* quit */
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Links Delete.");
	    ui_printMessage(string);
	    ui_displayHelp("* Links Delete");
	    break;
	  case 'c':
	    if (NOT ui_safetyFlg OR 
		ui_confirmYes("Links Delete Clique. Are you sure?")) {
		ui_can_setBusyCursor();
		ui_action_linksMakeClique(UI_ERASE);
		ui_can_setNormalCursor();
		ui_net_completeRefresh(displayPtr, UI_GLOBAL);
	    }
	    break;
	  case 's':
	    if (NOT ui_safetyFlg OR 
		ui_confirmYes("Links Delete from Source unit. Are you sure?")) {
		ui_can_setBusyCursor();
		ui_action_linksMakeFromSourceUnit(UI_ERASE);
		ui_can_setNormalCursor();
		ui_net_completeRefresh(displayPtr, UI_GLOBAL);
	    }
	    break;
	  case 't':
	    if (NOT ui_safetyFlg OR
		ui_confirmYes("Links Delete from Target unit. Are you sure?")) {
		ui_can_setBusyCursor();
		ui_action_linksMakeToTargetUnit(UI_ERASE);
		ui_can_setNormalCursor();
		ui_net_completeRefresh(displayPtr, UI_GLOBAL);
	    }
	    break;
	  case 'r':
	    ui_key_currentState     = UI_START_STATE; 
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_LINKS_DELETE; /* stay here */
	}
	break;


     /* ------------------------------------------------------------------- */

      case UI_STATE_UNITS_SET:

	if (ui_key_currentKey != 'h') {
	    ui_key_currentState = ui_key_returnUnitState;
	}	

	switch (ui_key_currentKey) /* */  {
	  case 'q': /* quit */
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Units Set.");
	    ui_printMessage(string);
	    ui_displayHelp("* Units Set");
	    break;
	  case 'n': /* name */
	    sprintf(string,"Units Set Name.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_NAME);
	    ui_can_setNormalCursor();
	    break;
	  case 't': /* type */
	    sprintf(string,"Units Set io-Type.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_IOTYPE);
	    ui_can_setNormalCursor();
	    break;
	  case 'd': /* Default type */
	    sprintf(string,"Units Set Default io-type.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_unitSetDefaultType();
	    ui_can_setNormalCursor();
	    break;
	  case 'a': /* activation */
	    sprintf(string,"Units Set Activation.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_ACT);
	    ui_can_setNormalCursor();
	    break;
	  case 'b': /* bias */
	    sprintf(string,"Units Set Bias.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_BIAS);
	    ui_can_setNormalCursor();
	    break;
	  case 'f': /* function ... */
	    ui_key_currentState = UI_STATE_UNITS_SET_FUNCTION;
	    break;
	  case 'i': /* initial activation */
	    sprintf(string,"Units Set Initial activation.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_I_ACT);
	    ui_can_setNormalCursor();
	    break;
	  case 'o': /* output */
	    sprintf(string,"Units Set Output.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_OUT);
	    ui_can_setNormalCursor();
	    break;
	  case 'l': /* layers */
	    sprintf(string,"Units Set Layers.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitLayers();
	    ui_can_setNormalCursor();
	    break;
	  case 'r':
	    ui_key_currentState     = UI_START_STATE; 
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_UNITS_SET; 
	    break;
	}
	break;

      /* ------------------------------------------------------------------- */

      case UI_STATE_UNITS_SET_FUNCTION:

	if (ui_key_currentKey != 'h') {
	    ui_key_currentState = ui_key_returnUnitState;
	}	
	
	switch (ui_key_currentKey) /* */  {
	  case 'q': /* quit */
	    break;
	  case 'h': /* help */
	    sprintf(string,"HELP: Units Set Function.");
	    ui_printMessage(string);
	    ui_displayHelp("* Units Set Function");
	    break;
	  case 'r':
	    ui_key_currentState     = UI_START_STATE; 
	    break;
	  case 'a': /* activation */
	    sprintf(string,"Units Set Activation function.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_ACT_FUNC);
	    ui_can_setNormalCursor();
	    break;
	  case 'f': /* ftype */
	    sprintf(string,"Units Set Function Ftype.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_FTYPE);
	    ui_can_setNormalCursor();
	    break;
	  case 'o': /* output */
	    sprintf(string,"Units Set Output function.");
	    ui_printMessage(string);
	    ui_can_setBusyCursor();
	    ui_action_setUnitAttribute(UI_SET_OUT_FUNC);
	    ui_can_setNormalCursor();
	    break;
	  default:
	    ui_key_currentState = UI_STATE_UNITS_SET_FUNCTION; /* stay here */
	}
	break;

      /* ------------------------------------------------------------------- */

      case UI_STATE_GRAPHIC:

	switch (ui_key_currentKey) {

	  case 'q': /* quit */
	    ui_key_currentState = UI_START_STATE;
	    break;

	  case 'h': /* help flags */
	    sprintf(string,"HELP: Graphics.");
	    ui_printMessage(string);
	    ui_displayHelp("* Graphic");
	    break;

	  case 'o':
	    sprintf(string,"Graphics Origin.");
	    ui_printMessage(string);
	    displayPtr->origin = ui_key_eventPos;
	    ui_net_completeRefresh(displayPtr, UI_LOCAL);
	    ui_key_currentState = UI_START_STATE;
	    break;

	  case 'm':
	    sprintf(string,"Graphics Move.");
	    ui_printMessage(string);
	    displayPtr->origin.x += 
		ui_targetUnit.gridPos.x - ui_key_eventPos.x;
	    displayPtr->origin.y += 
		ui_targetUnit.gridPos.y - ui_key_eventPos.y;
	    ui_net_completeRefresh(displayPtr, UI_LOCAL);
	    ui_key_currentState = UI_START_STATE;
	    break;

	  case 'u':
	    sprintf(string,"Graphic Units.");
	    ui_printMessage(string);
	    ui_sel_reshowItems(ui_currentDisplay, UI_LOCAL);
	    ui_net_updateWhole(displayPtr, UI_LOCAL, 
			       UI_DRAW_UNITS, UI_DRAW);
	    ui_sel_reshowItems(ui_currentDisplay, UI_LOCAL);
	    ui_key_currentState = UI_START_STATE;
	    break;

	  case 'l':
	    sprintf(string,"Graphics Links.");
	    ui_printMessage(string);
	    ui_sel_reshowItems(ui_currentDisplay, UI_LOCAL);
	    ui_net_updateWhole(displayPtr, UI_LOCAL, 
			       UI_DRAW_LINKS, UI_DRAW);
	    ui_sel_reshowItems(ui_currentDisplay, UI_LOCAL);
	    ui_key_currentState = UI_START_STATE;
	    break;

	  case 'a':
	    sprintf(string,"Graphics All.");
	    ui_printMessage(string);
	    /* Don't erase the markers, because ui_net_completeRefresh()
	       already erases the whole graphic area! The routine will draw
	       the markers again by itsself, if neccessary! */
	    ui_net_completeRefresh(displayPtr, UI_LOCAL);
	    ui_key_currentState = UI_START_STATE;
	    break;

	  case 'c':
	    sprintf(string,"Graphics Complete.");
	    ui_printMessage(string);
	    /* Don't erase the markers, because ui_net_completeRefresh()
	       already erases the whole graphic area! The routine will draw
	       the markers again by itsself, if neccessary! */
	    ui_net_completeRefresh(displayPtr, UI_GLOBAL);
	    ui_key_currentState = UI_START_STATE;
	    break;

	  case 'd':
	    sprintf(string,"Graphics Direction.");
	    ui_printMessage(string);
	    ui_action_graphicsDirection();
	    ui_key_currentState = UI_START_STATE;
	    break;

	  case 'g':
	    sprintf(string,"Graphics Grid.");
	    ui_printMessage(string);
	    ui_action_graphicsGrid();
	    ui_key_currentState = UI_START_STATE;
	    break;
	  case 'r':
	    ui_key_currentState     = UI_START_STATE; 
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_GRAPHIC; /* stay here */
	}
	break;

      /* ------------------------------------------------------------------- */

      case UI_STATE_GETDEST:

	switch (ui_key_currentKey) {

	  case 'r': /* quit */
	    ui_key_currentState = UI_START_STATE;
	    ui_key_abortOutlineAction();
	    break;
	  case 'q': /* quit */
	    ui_key_currentState = ui_key_returnUnitState;
	    ui_key_abortOutlineAction();
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_GETDEST; /* stay here */
	}
	break;

      /* ------------------------------------------------------------------- */

      case UI_STATE_SITES:

	switch (ui_key_currentKey) {

	  case 'h': /* help */
	    sprintf(string,"HELP: Sites.");
	    ui_printMessage(string);
	    ui_displayHelp("* Site");
	    break;
	  case 'q': /* quit */
	    ui_key_currentState     = UI_START_STATE;
	    break;
	  case 'a': /* add */
	    ui_key_currentState     = UI_START_STATE;
	    ui_action_sitesMake(UI_DRAW);
	    break;
	  case 'd': /* delete */
	    if (NOT ui_safetyFlg OR
		ui_confirmYes("Sites Delete. Are you sure?")) {
		ui_key_currentState     = UI_START_STATE; 
		ui_action_sitesMake(UI_ERASE);
	    }
	    break;
	  case 'c': /* copy */
	    ui_key_currentState     = UI_STATE_SITES_COPY; 
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_SITES; /* stay here */
	}
	break;

      /* ------------------------------------------------------------------- */

      case UI_STATE_SITES_COPY:

	switch (ui_key_currentKey) {

	  case 'h': /* help */
	    sprintf(string,"HELP: Sites Copy.");
	    ui_printMessage(string);
	    ui_displayHelp("* Sites Copy");
	    break;
	  case 'q': /* quit */
	    ui_key_currentState     = UI_START_STATE;
	    break;
	  case 'a': /* with all links */
	    ui_key_currentState     = UI_START_STATE;
	    ui_key_linksToHandle = UI_ALL;
	    ui_action_sitesCopy();
	    break;
	  case 'n': /* with no links */
	    ui_key_currentState     = UI_START_STATE; 
	    ui_key_linksToHandle = UI_NO_FLAG;
	    ui_action_sitesCopy();
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_SITES; /* stay here */
	}
        break;

     /* ------------------------------------------------------------------- */
     /* ------------------------------------------------------------------- */

      case UI_STATE_UNITS_3D:

	switch (ui_key_currentKey) {

	  case 'z': /* z-value */
	    ui_key_currentState     = UI_START_STATE;
            ui_action_UnitZcoordinate ();
	    break;
	  case 'm': /* move */
	    ui_key_currentState     = UI_START_STATE;
            ui_action_UnitZsetReference ();
	    break;
	  case 'h': /* help */
	    break;
	  case 'q': /* quit */
	    ui_key_currentState     = UI_START_STATE;
	    break;
	  default:
	    ui_key_currentState     = UI_STATE_SITES; /* stay here */
	}
        break;
    }

    sprintf(string," ");

    switch (ui_key_currentState) {
      case UI_START_STATE: 
	sprintf(prefix," ");
	break;
      case UI_STATE_MODE: 
	sprintf(prefix,"Mode");
	break;
      case UI_STATE_UNITS: 
	sprintf(prefix,"Units");
	break;
      case UI_STATE_LINKS: 
	sprintf(prefix,"Links");
	break;
      case UI_STATE_FLAG: 
	sprintf(prefix,"Flag");
	break;
      case UI_STATE_LINKS_MAKE: 
	sprintf(prefix,"Links Make");
	break;
      case UI_STATE_UNITS_INSERT: 
	sprintf(prefix,"Units Insert");
	break;
      case UI_STATE_UNITS_COPY: 
	sprintf(prefix,"Units Copy");
	break;
      case UI_STATE_UNITS_COPY_STRUCTURE:
	sprintf(prefix,"Units Copy Structure");
	break;
       case UI_STATE_UNITS_3D:
	sprintf(prefix,"Units 3D");
	break;
      case UI_STATE_LINKS_COPY: 
	sprintf(prefix,"Links Copy");
	break;
      case UI_STATE_LINKS_DELETE: 
	sprintf(prefix,"Links Delete");
	break;
      case UI_STATE_UNITS_SET: 
	sprintf(prefix,"Units Set");
	break;
      case UI_STATE_UNITS_SET_FUNCTION: 
	sprintf(prefix,"Units Set Function");
	break;
      case UI_STATE_GRAPHIC:
	sprintf(prefix,"Graphic");
	break;
      case UI_STATE_SITES:
	sprintf(prefix,"Sites");
	break;
      case UI_STATE_SITES_COPY:
	sprintf(prefix,"Sites Copy");
	break;
    }
    if (ui_key_currentState == UI_STATE_GETDEST)
	sprintf(string,"click on destination with left button");
    else
	sprintf(string,"%s>",prefix);
    ui_printMessage(string);
}


/*****************************************************************************
  FUNCTION : ui_can_KEventProc

  PURPOSE  : This routine accepts all valid key sequences. Its basic mechanism
             is a finite automaton with 8 states. On each ASCII_EVENT for the
	     canvas, 
  RETURNS  : - alteration of network and graphic via editor commands
             - alteration of status info
	     - alteration of units info panel via 
	       ui_info_makeUnitInfoPanelConsistent()
  NOTES    : The automat changes his behaviour in dependency of the variables:
             - ui_key_unitsReturnState
	     - ui_key_linksReturnState
	     which are set by the commands "Mode Units" and "Mode Links" and
	     the command "... Return" implicitly.

  UPDATE   :	
*****************************************************************************/

void ui_can_KEventProc (Widget w, struct Ui_DisplayType *displayPtr, XEvent *event)

{
    char            string[50];
    struct PosType  eventPixPos;
    KeySym          key;
    static int      loc_mode_switch = 0;

    if (displayPtr->frozen) return; /* don't accept this event here */
    
    if (event->type != KeyPress)
	return;
    
    XLookupString((XKeyEvent *) event, string, 1, &key, NULL);

    if( (key == XK_Control_L) OR (key == XK_Control_R))
	return;


    /* Process window popup requests */
    if( key == XK_Mode_switch){
	loc_mode_switch = 1;
	return;
    }
    if( loc_mode_switch == 1){
	loc_mode_switch = 0;
	ui_key_control(w,1,event);
	return;
    }


    if ((string[0] < '3') OR ((string[0] > '3') AND (string[0] < 'a')) 
      OR (string[0] > 'z'))
	return;

    eventPixPos.x     = (int) event->xkey.x;
    eventPixPos.y     = (int) event->xkey.y;
   
    ui_key_automata(displayPtr, eventPixPos, string[0]);
}


/*****************************************************************************
  FUNCTION : ui_key_menuCallback

  PURPOSE  : callback. Called when a menu item was selected
  RETURNS  : void
  NOTES    : 

  UPDATE   : 20.9.1990
******************************************************************************/

static void ui_key_menuCallback (Widget w, int key, caddr_t call_data)

{
    ui_key_automata(ui_currentDisplay, ui_pixPosMouse, (char) key);
}


/*****************************************************************************
  FUNCTION : ui_key_menuPopdownCallback

  PURPOSE  : finish the menu
  RETURNS  :
  NOTES    : it is neccessary to destroy the menu widget, otherwise the new
             menu will be added to the widget tree, but not be shown!!

  UPDATE   : 20.9.1990
******************************************************************************/

static void ui_key_menuPopdownCallback (Widget w, caddr_t client_data, 
					caddr_t call_data)

{
    XtDestroyWidget(ui_key_menu);
}


/*****************************************************************************
  FUNCTION : ui_key_popMenu

  PURPOSE  : show the menu
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void ui_key_popMenu (struct Ui_DisplayType *displayPtr)

{
    Widget         mItem;
    int            i = 0;
    int            j;
	  
    ui_key_menu = 
	XtCreatePopupShell("popmenu", simpleMenuWidgetClass, 
			   displayPtr->widget, NULL, 0);
    XtAddCallback(ui_key_menu, XtNpopdownCallback,
		  (XtCallbackProc) ui_key_menuPopdownCallback, NULL);
    
    while (ui_commands[ui_key_currentState][i] != NULL) {
	mItem =
	    XtCreateManagedWidget(ui_commands[ui_key_currentState][i],
				  smeBSBObjectClass, ui_key_menu, NULL, 0);

	j = 0;
	while ( ( ( (ui_commands[ui_key_currentState][i])[j] < 'A') OR
		  ( (ui_commands[ui_key_currentState][i])[j] > 'Z') ) AND
		(ui_commands[ui_key_currentState][i])[j] != '\0' AND
		(ui_commands[ui_key_currentState][i])[j] != '3')
	    j++;
	

	if ((ui_commands[ui_key_currentState][i])[j] != '\0') {
	    XtAddCallback(mItem, XtNcallback, 
			  (XtCallbackProc) ui_key_menuCallback, 
	  (XtPointer)((long)tolower((ui_commands[ui_key_currentState][i])[j])));
	}    
	i++;
    }
}



/*****************************************************************************
  FUNCTION : ui_key_control

  PURPOSE  : pop up the various windows
  RETURNS  : void
  NOTES    :

  UPDATE   : 04.01.1994
******************************************************************************/
void ui_key_control(Widget w, Cardinal dummy, XEvent *event)

{
    char         hit_key[2];
    KeySym       key;
    static int   mode_switch_on = 0;
    caddr_t call_data = NULL;


    if (event->type != KeyPress)
	return;
       
    XLookupString((XKeyEvent *) event, hit_key, 1, &key, NULL);


    if( key == XK_Mode_switch ){
	mode_switch_on = 1;
	return;
    }

    if( (!mode_switch_on) && (dummy != 1))
	return;

    switch(hit_key[0]){
       case 'f':
	    ui_displayFilePanel(w, w, call_data);
	    break;
       case 'd':
	    ui_displayGraphic(w, NULL, call_data);
	    break;
       case '3':
	    d3_displayGraphic();
	    break;
       case 'i':
	    ui_displayInfo(w, call_data, call_data);
	    break;
       case 'w':
	    ui_displWeights(w, call_data);
	    break;
       case 'p':
	    ui_displMap(w, call_data);
	    break;
       case 'g':
	    o_createOszi();
	    break;
       case 'c':
	    ui_displayControl(w, call_data, call_data);
	    break;
       case 'k':
	    kohonen_createWindow();
	    break;
       case 'h':
	    ui_displayText(w, call_data, call_data);
	    break;
       case 'a':
	    NA_OpenNetworkAnalyzer(w, NULL, NULL);
	    break;
       case 'q':
	    ui_guiQuit(w, NULL, NULL);
	    break;
       case 'Q':
	    ui_guiDoQuit(w, NULL, NULL);
	    break;
       case 's':
	    if(ui_controlIsCreated)
		ui_rem_defSubPat(w, 0, call_data);
	    break;
       default:
	    break;
    }

    mode_switch_on = 0;

}

/* end of file */
/* lines: 1399 */





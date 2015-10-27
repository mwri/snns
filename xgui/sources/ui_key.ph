/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_key.ph,v $
  SHORTNAME      : key.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 6.6.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:03 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_KEY_DEFINED_
#define _UI_KEY_DEFINED_


/* begin global definition section */


void ui_key_popMenu (struct Ui_DisplayType *displayPtr);
void ui_key_init (void);
void ui_can_KEventProc (Widget w, struct Ui_DisplayType *displayPtr, 
				XEvent *event);
void ui_key_control(Widget w, Cardinal n, XEvent *event);


StateType  ui_key_currentState = UI_START_STATE, /* current state of automat */
    	ui_key_returnUnitState;  /* dito from state UNIT */

struct PosType  ui_key_eventPos;     /* position of current event */
int             ui_key_unitNo;       /* unit at this position, else ZERO */
				/* current key event */
Bool              ui_safetyFlg = FALSE; /* safety on of off */
FlagType          ui_key_linksToHandle; /* which links are to handle:
                                           UI_ALL (input and output)
                                           UI_INPUT UI_OUTPUT UI_NONE */
FlagType          ui_key_action; /* used for MOVE and COPY to tell the
                                    mouse event handler the selected action */
Bool            ui_outlineActive = FALSE; /* waiting for a 
						destination position */


/* end global definition section */


/* begin private definition section */


static void ui_can_setBusyCursor(void); /* not implemented */
static void ui_can_setNormalCursor(void); /* not implemented */
static void ui_key_abortOutlineAction (void);
static void ui_key_startOutlineAction (void);
static void ui_key_automata (struct Ui_DisplayType *displayPtr, 
				struct PosType eventPixPos, char key); 
static void ui_key_menuCallback (Widget w, int key, caddr_t call_data);
static void ui_key_menuPopdownCallback (Widget w, caddr_t client_data, 
				caddr_t call_data);


static char  ui_key_currentKey =  UI_ASCII_BLANK; /* last key pressed */
static char *ui_commands[18][13]; /* [state][command] */
static Widget ui_key_menu;  /* menu with all possibilities to 
					continue the sequenz */
static StateType  ui_key_returnFlagState,  /* state to return from state FLAG */
    			ui_key_returnLinkState;  /* dito from state LINK */


/* end private definition section */


#endif /* _UI_KEY_DEFINED_ */


/* end of file */
/* lines: 28 */

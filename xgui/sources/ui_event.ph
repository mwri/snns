/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_event.ph,v $
  SHORTNAME      : event.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 5.6.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:43 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_EVENT_DEFINED_
#define _UI_EVENT_DEFINED_


/* begin global definition section */


void ui_mw_eventProc (Widget, struct Ui_DisplayType *, XEvent *);
void ui_can_MapEventProc (Widget,  struct Ui_DisplayType *, XEvent *);

unsigned long   ui_col_mainCells[10][3];
struct PosType  ui_pixPosMouse, ui_gridPosMouse; /* current mouse position */


/* end global definition section */


/* begin private definition section */


static int             ui_link_sourceUnitNo;            /* unitNo on position, where
                                                    middle button was
                                                    depressed */
static struct PosType  ui_pixPosOld, ui_gridPosOld;     /* last mouse position */
static Bool            ui_unselectFlg;                  /* SHIFT was pressed */
static Bool            ui_pixBoxDrawnFlg;               /* rubber box is visible */
static Bool            ui_leftButton, ui_middleButton;  /* ... button was depressed */
static struct PosType  ui_selPixPos1, ui_selGridPos1;   /* position, where the left
                                                    button was depressed */
static struct PosType  ui_selGridPos2;                  /* position, where the left
                                                    button was released */
static struct PosType  ui_selPixPosBox;                 /* position of the first
                                                    corner of the rubber box */



/* end private definition section */


#endif /* _UI_EVENT_DEFINED_ */



/* end of file */
/* lines: 27 */

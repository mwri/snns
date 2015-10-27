/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_colEdit.ph,v $
  SHORTNAME      : colEdit.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for ui_colEdit.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 27.5.1992

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:13 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_COLEDIT_DEFINED_
#define _UI_COLEDIT_DEFINED_


/* begin global definition section */


#ifndef ZERO
#define ZERO 0
#endif

#define noOfColToggles     3


void ui_createColorEditPannel (Widget, Widget, caddr_t);

/* end global definition section */


/* begin private definition section */


static void ui_cancelColorEditPannel (Widget, Widget, caddr_t);
static void ui_closeColorEditPannel (Widget, Widget, caddr_t);
static void ui_getDisplayColors (void);
static Widget ui_xCreateColButtonItem (Widget, unsigned long, int, int, 
				Widget, Widget);
static void ui_createColWidgets (Widget);
static Widget ui_createTestPannel (Widget, Widget, Widget);
static void ui_redrawColorWindow (void);
static void ui_colorUpdateProc (Widget, int, caddr_t);
static void ui_timerProc (caddr_t, XtIntervalId *);
static void ui_colorWindowEventProc (Widget, Display *, XEvent *);
static void ui_selectColorItemProc (Widget, int, caddr_t);


static Widget ui_colWidget[UI_MAX_EDIT_COLS]; /* widgets for the edit colors */
static Widget colorToggle[noOfColToggles];    /* widgets for text,         */
                                              /* background and selection */

static int colWidgetXsize = 32;   /* X size of a color edit widget */
static int colWidgetYsize = 16;   /* Y size of a color edit widget */

static Window colWindow;          /* id of the test window */ 

static int ui_currentBackCol;   /* currently selected background color index */
static int ui_currentSelCol;    /* currently selected selection color index */
static int ui_currentTextCol;   /* currently selected background color index */

static int currentSelection = UI_SELECT_BACK_COLOR; /* default selection */

static GC colGC;               /* graphic context for the test window */


/* end private definition section */


#endif /* _UI_COLEDIT_DEFINED_ */


/* end of file */
/* lines: 23 */

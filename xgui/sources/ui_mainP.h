/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_mainP.h,v $
  SHORTNAME      : mainP.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.4.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:19 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_MAINP_DEFINED_
#define _UI_MAINP_DEFINED_



extern void ui_xCreateGC (Window);
extern void ui_printMessage (char *);
extern void ui_displayDone (Widget, struct Ui_DisplayType *, caddr_t);
extern void ui_popupDone (Widget, int, caddr_t);
extern void ui_popupWeights (Widget, caddr_t, caddr_t);
extern void ui_editPopup (Widget, struct SimpleListType *, caddr_t);
extern void ui_editSitePopup (Widget, struct SimpleListType *, caddr_t);
extern void ui_listPopup (struct SimpleListType *, Position, Position);
extern void ui_displayFilePanel (Widget, Widget, caddr_t);
extern void ui_displayLayerPanel (Position, Position);
extern void ui_displayGraphic (Widget, struct Ui_DisplayType *, caddr_t);
extern void ui_displayControl (Widget, caddr_t, caddr_t);
extern void ui_displayInfo (Widget, caddr_t, caddr_t);
extern void ui_displayText (Widget, caddr_t, caddr_t);
extern void ui_displayHelp (char *);
extern void ui_guiQuit (Widget, XtPointer, XtPointer);
extern void ui_guiDoQuit (Widget, XtPointer, XtPointer);
extern void ui_loadFileFromCommandLine (void);
extern void ui_parseCmdLine (int, char *[]);


	/* Handles of popup shells. Used by ui_popupDone() */
extern Widget ui_popControl, ui_popInfo, ui_popResult;

	/* message widget in the control panel */
extern Widget ui_controlMessageWidget;

	/* widgets of jog/random weights panel */
extern Widget  ui_highLimitWidget, ui_lowLimitWidget, ui_correlationLimitWidget;

	/* widget of message displayed when the 
	bubble of a slider in the setup panel was moved */
extern Widget  ui_setupSliderMessage;

extern GC             ui_gc;         /* xgui graphic context */
extern XFontStruct   *ui_fontStruct; 
extern int            ui_screen;     /* X screen */

	/* display for with the setup panel was displayed */
extern struct Ui_DisplayType  *ui_set_displayPtr; /* for xgui */

extern Display       *ui_display;    /* for X */

extern Widget  ui_toplevel;
extern Widget  ui_message;  /* message in the manager panel */

	/* shell widgets of info, control, setup and file panel */
extern Widget  ui_infoPanel, ui_setupPanel;

	/* widgets in the setup panel */
extern Widget  ui_set_gridWidthWidget, ui_set_originXWidget;
extern Widget  ui_set_originYWidget;
extern Widget  ui_set_subnetWidget;
extern Widget  ui_set_zWidget;

	/* panel created yes or no */
extern Bool    ui_setupIsCreated;

	/* data displayed in the file panel */
extern char    ui_edit_actFuncName[];
extern char    ui_edit_outFuncName[];
extern char    ui_edit_FTypeName[];


extern XGCValues      ui_gcValues;   /* initial gc values */



#endif /* _UI_MAINP_DEFINED_ */


/* end of file */
/* lines: 100 */

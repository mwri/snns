/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_mainP.ph,v $
  SHORTNAME      : mainP.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.4.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.13 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:20 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_MAINP_DEFINED_
#define _UI_MAINP_DEFINED_


/* begin global definition section */


void ui_xCreateGC (Window);
void ui_printMessage (char *);
void ui_displayDone (Widget, struct Ui_DisplayType *, caddr_t);
void ui_popupDone (Widget, int, caddr_t);
void ui_popupWeights (Widget, caddr_t, caddr_t);
void ui_editPopup (Widget, struct SimpleListType *, caddr_t);
void ui_editSitePopup (Widget, struct SimpleListType *, caddr_t);
void ui_listPopup (struct SimpleListType *, Position, Position);
void ui_displayFilePanel (Widget, Widget, caddr_t);
void ui_displayLayerPanel (Position, Position);
void ui_displayGraphic (Widget, struct Ui_DisplayType *, caddr_t);
void ui_displayControl (Widget, caddr_t, caddr_t);
void ui_displayInfo (Widget, caddr_t, caddr_t);
void ui_displayText (Widget, caddr_t, caddr_t);
void ui_displayHelp (char *);
void ui_guiQuit (Widget, XtPointer, XtPointer);
void ui_guiDoQuit (Widget, XtPointer, XtPointer);
void ui_loadFileFromCommandLine (void);
void ui_parseCmdLine (int, char *[]);


	/* Handles of popup shells. Used by ui_popupDone() */
Widget ui_popControl, ui_popInfo, ui_popResult;

	/* message widget in the control panel */
Widget ui_controlMessageWidget;

	/* widgets of jog/random weights panel */
Widget  ui_highLimitWidget, ui_lowLimitWidget, ui_correlationLimitWidget;

	/* widget of message displayed when the 
	bubble of a slider in the setup panel was moved */
Widget  ui_setupSliderMessage;





GC             ui_gc;         /* xgui graphic context */
XFontStruct   *ui_fontStruct; 
int            ui_screen;     /* X screen */




	/* display for with the setup panel was displayed */
struct Ui_DisplayType  *ui_set_displayPtr; /* for xgui */

Display       *ui_display;    /* for X */

Widget  ui_toplevel;
Widget  ui_message;  /* message in the manager panel */

	/* shell widgets of info, control, setup and file panel */
Widget  ui_infoPanel, ui_setupPanel;
	/* widgets in the setup panel */
Widget  ui_set_gridWidthWidget, ui_set_originXWidget;
Widget  ui_set_originYWidget;
Widget  ui_set_subnetWidget;
Widget  ui_set_zWidget;

	/* panel created yes or no */
Bool    ui_setupIsCreated;

	/* data displayed in the file panel */
char    ui_edit_actFuncName[MAX_NAME_LENGTH];
char    ui_edit_outFuncName[MAX_NAME_LENGTH];
char    ui_edit_FTypeName[MAX_NAME_LENGTH];

XGCValues      ui_gcValues;   /* initial gc values */


/* end global definition section */

/* begin private definition section */


static void ui_textLook (Widget, XtPointer, XtPointer);
static void ui_textMore (Widget, XtPointer, XtPointer);
static void ui_textKey (Widget, char *);
static void ui_textTopics (Widget, Widget, caddr_t);
static void ui_showHelpMsg (char *);
static void ui_refresh (Widget, struct Ui_DisplayType *, XEvent *);
static void ui_popupSetup (Widget, struct Ui_DisplayType *, caddr_t);
static void ui_listPopupDone (Widget, struct SimpleListType *, caddr_t);
static void ui_helpDone (Widget, struct HelpType*, caddr_t);
static void ui_editPopupDone (Widget, struct SimpleListType *, caddr_t);
static int ui_chkFileName (char *, char *);
static void ui_jog_on(Widget w, caddr_t client_data, caddr_t call_data);
static void ui_jog_toggle_correlation(Widget w, caddr_t client_data, caddr_t call_data);

#ifdef _UNUSED_FUNCTIONS_

static void ui_syntax (XtAppContext, char *);

#endif /* _UNUSED_FUNCTIONS */

	/* Needed for textlook, textmore and textkey */
static off_t ui_lengthOfHelp;

static Bool  ui_filePannelPoppedUp = FALSE;  /* indicates that the file pannel 
                                         is popped up */
static char ui_dfontName[255];
	/* names of possible fonts  */
#define noOfFontNames 8
static Bool  ui_helpOk = FALSE;   /* indicates that help.hdoc is ok */
	/* list and free list of help windows */
static struct HelpType *ui_help_listPtr     = NULL;
static struct HelpType *ui_help_freeListPtr = NULL;
	/* load from command line */
static Bool ui_loadNetbyInit = FALSE;
static Bool ui_loadPatbyInit = FALSE;
static Bool ui_loadCfgbyInit = FALSE;
static Bool  ui_noOfHelpsUp = 0;   /* number of help windows created */
	/* Handles of popup shells. Used by ui_popupDone() */
static Widget ui_popDummy, ui_popSetup, ui_popFile, ui_popEdit, ui_popWeights,
       ui_popCanvas, ui_popList, ui_popLayer, ui_popInit;
static Bool  ui_pop_exit = FALSE;  /* used, when popup window has its own 
			       event dispatching loop */
	/* frame widgets (form) of setup and file panel */
static Widget ui_setupBox, ui_fileBox;
	/* true if font specified in command line */
static Bool hasCmdlineFont = FALSE;


/* end private definition section */


#endif /* _UI_MAINP_DEFINED_ */


/* end of file */
/* lines: 48 */

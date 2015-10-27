/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_file.ph,v $
  SHORTNAME      : file.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 22.5.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_FILE_DEFINED_
#define _UI_FILE_DEFINED_


/* begin global definition section */


void ui_xCreateFilePanel (Widget);


Widget    ui_filePanel;

Bool      ui_fileIsCreated = FALSE; /* file panel created yes or no */


/* end global definition section */


/* begin private definition section */


#define SELECTED_NAME_LENGTH   1024
#define MAX_DIR_ENTRIES 4096
#define DIR_BUFFER_LENGTH 524288


static void ui_changeDirectory (char []);
static void ui_checkPath (char *);
static void ui_expandPath (char *);
static void ui_loadSelectedFile (Widget, Widget, caddr_t);
static void ui_readDirectory (void);
static void ui_saveSelectedFile (Widget, Widget, caddr_t);
static void ui_selectionEventProc (Widget, Display *, XEvent *);
static void ui_setExtString (Widget, int, caddr_t);
static void ui_strSort (int, int);


static int currentFileType = UI_FILE_NET;
static char extMask[10];
static char dirEntries[DIR_BUFFER_LENGTH];
static char sortedDirEntries[DIR_BUFFER_LENGTH];
static char *dirPtr[MAX_DIR_ENTRIES];
static int maxEntries;
static struct stat *stat_buf;

static    Widget fileBox, selectorBox;
static Widget netToggle, patToggle, resToggle, cfgToggle, txtToggle;


/* end private definition section */


#endif /* _UI_FILE_DEFINED_ */



/* end of file */
/* lines: 35 */

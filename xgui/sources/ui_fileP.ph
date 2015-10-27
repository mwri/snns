/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_fileP.ph,v $
  SHORTNAME      : fileP.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 22.5.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.8 $ 
  LAST CHANGE    : $Date: 1998/02/25 15:21:49 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_FILEP_DEFINED_
#define _UI_FILEP_DEFINED_


/* begin global definition section */

void ui_file_saveText (Widget, XtPointer, caddr_t);
void ui_file_loadText (Widget, XtPointer, caddr_t);
void ui_file_saveConfiguration (Widget, XtPointer, caddr_t);
void ui_file_loadConfiguration (Widget, XtPointer, caddr_t);
void ui_file_loadDefaultConfiguration (char []);
void ui_file_savePatterns (Widget, XtPointer, caddr_t);
void ui_file_loadPatterns (Widget, XtPointer, caddr_t);
void ui_file_saveNet (Widget, XtPointer, caddr_t);
int  ui_file_loadNet (Widget, XtPointer, caddr_t);
void ui_file_saveResult (Widget, XtPointer, caddr_t);
void ui_file_loadResult (Widget, XtPointer, caddr_t);
char *ui_getExtension (FlagType);
Bool ui_fileExist (char [], FlagType);
void ui_file_updateShellLabels (void);


	/* widgets of all input fields in the file panel */
Widget ui_fileTXT;
Widget ui_filePAT;
Widget ui_fileCFG;
Widget ui_fileNET;
Widget ui_path;

Bool   ui_loadCorrespondingCfg = TRUE;

char   ui_filenameNET[MAX_NAME_LENGTH] = "\0";
char   ui_filenamePAT[MAX_NAME_LENGTH] = "\0";
char   ui_filenameRES[MAX_NAME_LENGTH] = "\0";
char   ui_filenameCFG[MAX_NAME_LENGTH] = "\0";
char   ui_filenameTXT[MAX_NAME_LENGTH] = "\0";
char   ui_filenameSLNET[MAX_NAME_LENGTH]; /* save or loaded file */

FILE   *ui_textFilePtr = NULL;     /* file ptr of log file */

int    NO_OF_PATTERN_SETS = 0;
int    CURR_PATTERN_SET = 0;
int    CURR_TEST_PATTERN_SET = 0;
char   *PATTERN_SET_FILE[NO_OF_PAT_SETS];

/* end global definition section */


/* begin private definition section */


static void ui_file_doLoadConfiguration (char *);


static char   ui_filenameSLPAT[MAX_NAME_LENGTH]; /* save or loaded file */


/* end private definition section */


#endif /* _UI_FILEP_DEFINED_ */



/* end of file */
/* lines: 34 */

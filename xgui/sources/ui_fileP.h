/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_fileP.h,v $
  SHORTNAME      : fileP.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 22.5.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.8 $ 
  LAST CHANGE    : $Date: 1998/02/25 15:21:48 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_FILEP_DEFINED_
#define _UI_FILEP_DEFINED_


extern void ui_file_saveText (Widget, XtPointer, caddr_t);
extern void ui_file_loadText (Widget, XtPointer, caddr_t);
extern void ui_file_saveConfiguration (Widget, XtPointer, caddr_t);
extern void ui_file_loadConfiguration (Widget, XtPointer, caddr_t);
extern void ui_file_loadDefaultConfiguration (char []);
extern void ui_file_savePatterns (Widget, XtPointer, caddr_t);
extern void ui_file_loadPatterns (Widget, XtPointer, caddr_t);
extern void ui_file_saveNet (Widget, XtPointer, caddr_t);
extern int  ui_file_loadNet (Widget, XtPointer, caddr_t);
extern void ui_file_saveResult (Widget, XtPointer, caddr_t);
extern void ui_file_loadResult (Widget, XtPointer, caddr_t);
extern char *ui_getExtension (FlagType);
extern Bool ui_fileExist (char [], FlagType);
extern void ui_file_updateShellLabels (void);


	/* widgets of all input fields in the file panel */
extern Widget ui_fileTXT;
extern Widget ui_filePAT;
extern Widget ui_fileCFG;
extern Widget ui_fileNET;
extern Widget ui_path;

extern Bool   ui_loadCorrespondingCfg;

extern char   ui_filenameNET[];
extern char   ui_filenamePAT[];
extern char   ui_filenameRES[];
extern char   ui_filenameCFG[];
extern char   ui_filenameTXT[];
extern char   ui_filenameSLNET[];       /* save or loaded file */

extern FILE   *ui_textFilePtr;          /* file ptr of log file */

extern int    NO_OF_PATTERN_SETS;
extern int    CURR_PATTERN_SET;
extern int    CURR_TEST_PATTERN_SET;   
extern char   *PATTERN_SET_FILE[];

#endif /* _UI_FILEP_DEFINED_ */

/* end of file */
/* lines: 63 */


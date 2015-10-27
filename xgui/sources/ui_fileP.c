/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_fileP.c,v $
  SHORTNAME      : fileP.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 22.5.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.14 $ 
  LAST CHANGE    : $Date: 1998/03/03 14:10:24 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

#include "kr_ui.h"

#include "d3_main.h"
#include "d3_global.h"
#include "d3_draw.h"

#include "o_graph.h"

#include "ui_infoP.h"
#include "ui_confirmer.h"
#include "ui_textP.h"
#include "ui_status.h"
#include "ui_display.h"
#include "ui_controlP.h"
#include "ui_utilP.h"
#include "ui_netUpdate.h"
#include "ui_selection.h"
#include "ui_result.h"
#include "ui_config.h"
#include "ui_mainP.h"
#include "ui_inversion.h"
#include "ui_control.h"
#include "ui_main.h"
#include "ui_event.h"
#include "ui_action.h"
#include "ui_xWidgets.h"
#include "ui_class.h"

#include "ui_fileP.ph"


/*****************************************************************************
  FUNCTION : ui_getExtension

  PURPOSE  : returns the extension of the specified fil type
  RETURNS  : char *
  NOTES    :

  UPDATE   : 20.8.1990
******************************************************************************/

char *ui_getExtension (FlagType fileType)

{
    switch (fileType) {
      case UI_FILE_TXT: return(".txt"); break;
      case UI_FILE_PAT: return(".pat"); break;
      case UI_FILE_RES: return(".res"); break;
      case UI_FILE_NET: return(".net"); break;
      case UI_FILE_CFG: return(".cfg"); break;
      default: return(""); break;
    }
}
	  

/*****************************************************************************
  FUNCTION : ui_fileExist

  PURPOSE  : Checks, wheter there is a named file with the right extension.
  RETURNS  : Bool
  NOTES    :

  UPDATE   : 20.8.1990
******************************************************************************/

Bool ui_fileExist(char filename[], FlagType fileType)

{
    FILE   *filePtr;
    char   buf[80+MAX_NAME_LENGTH];

    if (fileType)
	if (strlen(filename) AND (filename[0] == '/'))
	    sprintf(buf, "%s%s", filename, ui_getExtension(fileType));
	else
	    sprintf(buf, "%s/%s%s", ui_pathname, 
		    filename, ui_getExtension(fileType));
    else
	sprintf(buf, "%s", filename);

    if ((filePtr = fopen(buf,"r")) != NULL) 
	fclose(filePtr);
    return(filePtr != NULL);
}


/*****************************************************************************
  FUNCTION : ui_file_updateShellLabels

  PURPOSE  : display the name of the last net-file in the baseFrame label.
             This filename is stored in the variable filenameNet. Call
	     this routine only, if it is sure, that filename holds the
	     name of a net-file!
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_file_updateShellLabels (void)

{
    char    buf[MAX_NAME_LENGTH + 40];

    if (ui_controlIsCreated) {
	sprintf(buf,"snns-control    pattern: %s", ui_filenamePAT);
	XStoreName(ui_display, XtWindow(ui_popControl), buf);
    }
    if (ui_infoIsCreated) {
	sprintf(buf,"snns-info    net: %s", ui_filenameNET);
	XStoreName(ui_display, XtWindow(ui_popInfo), buf);
    }
}


/*****************************************************************************
  FUNCTION : ui_file_saveText

  PURPOSE  : saves the contents of the text window
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_file_saveText (Widget w, XtPointer button, caddr_t call_data)

{
    Bool performSave = TRUE;

    /* examine the filename */

    if (strlen(ui_filenameTXT) == 0) {
	ui_confirmOk("No file specified.");
	ui_printMessage("Save text aborted.");
	return;
    } 

    if (ui_fileExist(ui_filenameTXT, UI_FILE_TXT)) {
	performSave = 
	    ui_confirmYes("This text file already exists! Overwrite?");
    }
    
    if (performSave) {
	/* ui_tw_saveText(ui_filenameTXT); */
	ui_printMessage("Text saved.");
    } else
	ui_printMessage("Saving text aborted.");
}


/*****************************************************************************
  FUNCTION : ui_file_loadText

  PURPOSE  : saves the contents of the text window
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void  ui_file_loadText (Widget w, XtPointer button, caddr_t call_data)

{
    char string  [80+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    char filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];

    /* close old file, if one is open */
    if (ui_textFilePtr != NULL) {
	fclose(ui_textFilePtr);
	ui_textFilePtr = NULL; /* signal: no open text file */
    }
    
    /* examine the filename */

    if (strlen(ui_filenameTXT) == 0) {
	ui_confirmOk("No file specified! Reporting on logfile is off now.");
	return;
    }

    /* a filename is given */

    if (ui_filenameTXT[0] == '/')
	sprintf(filename, "%s%s", ui_filenameTXT, ui_getExtension(UI_FILE_TXT));
    else
	sprintf(filename, "%s/%s%s", ui_pathname, ui_filenameTXT, 
		ui_getExtension(UI_FILE_TXT));

    if (NOT ui_fileExist(filename, 0)) { 
	/* new text file */
	ui_textFilePtr = fopen(filename,"a");
    } else {
	/* a old file exists */
	if (ui_confirmYes("Logfile already exist. Append?"))
	    ui_textFilePtr = fopen(filename,"a");
	else
	    ui_textFilePtr = fopen(filename,"w");
    }
    sprintf(string, "current logfile:\n   %s\n", filename);
    ui_tw_printMessage(string);
}


/*****************************************************************************
  FUNCTION : ui_file_saveConfiguration

  PURPOSE  : save all configuration data (layer names, displays)
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_file_saveConfiguration (Widget w, XtPointer button, caddr_t call_data)

{
    char string[80+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    char filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    Bool performSave = TRUE;
    FILE *filePtr;

    if (strlen(ui_filenameCFG) == 0) {
	ui_confirmOk("No file specified!");
	return;
    }

    if (ui_filenameCFG[0] == '/')
	sprintf(filename, "%s%s", ui_filenameCFG, 
		ui_getExtension(UI_FILE_CFG));
    else
	sprintf(filename, "%s/%s%s", ui_pathname, ui_filenameCFG, 
		ui_getExtension(UI_FILE_CFG));

    if (ui_fileExist(filename, 0)) 
	performSave = ui_confirmYes("File already exist! Overwrite?");   
    
    if (performSave) {
	ui_printMessage("Saving configuration ...");
	filePtr = fopen(filename,"w");
	if (filePtr != NULL) {
	    if (ui_cfg_save(filePtr) > 0) {
		sprintf(string, "Configuration saved to file:\n   %s\n", filename);
		ui_tw_printMessage(string);
	    } else
		ui_confirmOk("Write error! Configuration not saved completely!");
	    fclose(filePtr);
	} else
	    ui_confirmOk("Error! Configuration not saved!");
	ui_printMessage("");
   } 
}
	

/*****************************************************************************
  FUNCTION : ui_file_loadDefaultConfiguration

  PURPOSE  : load a configuration
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_file_loadDefaultConfiguration (char pathName[])
				 
{
    char string  [80+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    char filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    FILE *filePtr;
    int  err;

    sprintf(filename,"%s/default.cfg", pathName);

    if (NOT ui_fileExist(filename, 0)) { 
	if (getenv("XGUILOADPATH") != NULL) 
	    sprintf(filename, "%s/default.cfg", getenv("XGUILOADPATH"));
	if (NOT ui_fileExist(filename,0)) {
	    fprintf (stdout, "Can't load <default.cfg>!\n");
	    return;
	}
    }
    /* a file exists */
    
    ui_printMessage("Loading configuration ...");
    filePtr = fopen(filename,"r");
    if ((err = ui_cfg_load(filePtr)) > 0) {
	sprintf(string, "Configuration loaded from file:\n   %s\n", filename);
	ui_tw_printMessage(string);
    } else {
	sprintf(string, "Read error in configuration file line %d!", -err);
	ui_confirmOk(string);
    }
    ui_printMessage("");
}


/*****************************************************************************
  FUNCTION : ui_file_doLoadConfiguration

  PURPOSE  : load a configuration
  RETURNS  : void
  NOTES    : this file must exist !!!

  UPDATE   :
*****************************************************************************/

static void ui_file_doLoadConfiguration (char *filename)

{
    char string  [80+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    FILE *filePtr;
    int  err;

    /* a file exists */
    
    ui_printMessage("Loading configuration ...");
    filePtr = fopen(filename,"r");
    if ((err = ui_cfg_load(filePtr)) > 0) {
	sprintf(string, "Configuration loaded from file:\n   %s\n", filename);
	ui_tw_printMessage(string);
    } else {
	sprintf(string, "Read error in configuration file line %d!", -err);
	ui_confirmOk(string);
    }
    ui_printMessage("");
}


/*****************************************************************************
  FUNCTION : ui_file_loadConfiguration

  PURPOSE  : load a configuration
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_file_loadConfiguration (Widget w, XtPointer button, caddr_t call_data)

{
    char filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];

    /* check now for errors and react ! */
    if (strlen(ui_filenameCFG) == 0) {
	ui_confirmOk("No file specified !");
	return;
    }

    if (ui_filenameCFG[0] == '/')
	sprintf(filename, "%s%s", ui_filenameCFG, ui_getExtension(UI_FILE_CFG));
    else
	sprintf(filename, "%s/%s%s", ui_pathname, ui_filenameCFG, 
		ui_getExtension(UI_FILE_CFG));

    /* a filename is given */
    if (NOT ui_fileExist(filename, 0)) { 
	ui_confirmOk("No such configuration file exists!");
	return;
    }

    /* a file exists */
    ui_file_doLoadConfiguration(filename);
}


/*****************************************************************************
  FUNCTION : ui_file_savePatterns

  PURPOSE  : save all patterns in the memory in the specified name 
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_file_savePatterns (Widget w, XtPointer button, caddr_t call_data)

{
    char string[80+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    char filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    Bool performSave = TRUE;

    if (strlen(ui_filenamePAT) == 0) {
	ui_confirmOk("No file specified!");
	return;
    }

    if (ui_filenamePAT[0] == '/')
	sprintf(filename, "%s%s", ui_filenamePAT, 
		ui_getExtension(UI_FILE_PAT));
    else
	sprintf(filename, "%s/%s%s", ui_pathname, ui_filenamePAT, 
		ui_getExtension(UI_FILE_PAT));

    if (ui_fileExist(filename, 0)) {
	sprintf(string,"%s already exist! Overwrite?", ui_filenamePAT);
	performSave = ui_confirmYes(string);
    }
    
    if (performSave) {
	ui_printMessage("Saving patterns ...");
	ui_checkError(krui_saveNewPatterns(filename,CURR_PATTERN_SET));
	if (ui_kernelError < 0)
	    ui_confirmOk("Error during saving patterns!");
	else {
	    sprintf(string, "Patterns saved to file:\n   %s\n", filename);
	    ui_tw_printMessage(string);
	    strcpy(ui_filenameSLPAT, ui_filenamePAT);
	    ui_file_updateShellLabels();
	}
    }
}
	

/*****************************************************************************
  FUNCTION : ui_file_loadPatterns

  PURPOSE  : load all patterns into memory and display the first
  RETURNS  : void
  NOTES    : 

  UPDATE   : 19.02.95
*****************************************************************************/

void ui_file_loadPatterns (Widget w, XtPointer button, caddr_t call_data)

{
    char string  [80+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    char filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    char filestring[30];

    /* check now for errors and react ! */
    if (strlen(ui_filenamePAT) == 0) {
	ui_confirmOk("No file specified !");
	return;
    }

    if (ui_filenamePAT[0] == '/')
	sprintf(filename, "%s%s", ui_filenamePAT,ui_getExtension(UI_FILE_PAT));
    else
	sprintf(filename, "%s/%s%s", ui_pathname, ui_filenamePAT, 
		ui_getExtension(UI_FILE_PAT));

    /* a filename is given */
    if (NOT ui_fileExist(filename, 0)) { 
	ui_confirmOk("No such pattern file exists!");
	return;
    }
    /* a file exists */
    
    ui_printMessage("Loading patterns ...");
    ui_checkError(krui_loadNewPatterns(filename,&CURR_PATTERN_SET));
    if (ui_kernelError < 0) {
	ui_confirmOk("Error during loading patterns!");
	ui_noOfCurrentPattern = 0;
	ui_rem_displayPatternNumber();
    } else {
	ui_noOfCurrentPattern = 1;
	CURR_PATTERN_SET = NO_OF_PATTERN_SETS;
	CURR_TEST_PATTERN_SET = NO_OF_PATTERN_SETS;	
	NO_OF_PATTERN_SETS++;
	PATTERN_SET_FILE[CURR_PATTERN_SET] = 
	    (char *)malloc(strlen(filename)+1);
	if(strrchr(filename,'/') == NULL)
	    strcpy(filestring,filename);
	else
	    strcpy(filestring,strrchr(filename,'/')+1);
	strncpy(PATTERN_SET_FILE[CURR_PATTERN_SET],filestring,
		strlen(filestring)-4);
	PATTERN_SET_FILE[CURR_PATTERN_SET][strlen(filestring)-4] = '\0';
	ui_rem_displayPatternNumber();
	if(ui_controlIsCreated)ui_rem_updatePattList();
	ui_rem_defSubPat((Widget) button,1,(caddr_t)1);
	sprintf(string, "Patterns loaded from file:\n   %s\n", filename);
	ui_tw_printMessage(string);
	ui_printMessage("Patterns loaded.");
	strcpy(ui_filenameSLPAT, ui_filenamePAT);
	ui_file_updateShellLabels();
	ui_classUpdatePanel(TRUE);
    }
}


/*****************************************************************************
  FUNCTION : ui_file_saveNet

  PURPOSE  : save the dynamic net structure via kernal routines
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void ui_file_saveNet (Widget w, XtPointer button, caddr_t call_data)

{
    Bool  performSave = TRUE;
    char  filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];

    if (strlen(ui_filenameNET) == 0) {
	ui_confirmOk("No file specified!");
	return;
    }

    if (ui_filenameNET[0] == '/')
	sprintf(filename, "%s%s", ui_filenameNET, 
		ui_getExtension(UI_FILE_NET));
    else
	sprintf(filename, "%s/%s%s", ui_pathname, ui_filenameNET, 
		ui_getExtension(UI_FILE_NET));

    if (ui_fileExist(filename, 0)) 
	performSave =
	    ui_confirmYes("This file already exist! Overwrite?");

    if (performSave) {
	ui_printMessage("Saving network ...");
	ui_checkError(krui_saveNet(filename, ui_filenameNET));
	if (ui_kernelError < 0)
	    ui_confirmOk("Error during saving the network!");
	else {
	    char string[40+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
	    ui_printMessage("Network saved.");
	    strcpy(ui_filenameSLNET, ui_filenameNET);
	    ui_file_updateShellLabels();
	    sprintf(string,"Network saved on file: %s\n",filename);
	    ui_tw_printMessage(string);
	}
    }
}


/*****************************************************************************
  FUNCTION : ui_file_loadNet

  PURPOSE  : loads the dynamic net structure
  RETURNS  : -1 on error condition
              0 for success
              1 when also a configuration file has been loaded
  NOTES    : calls krui_loadNet() 

  UPDATE   :
*****************************************************************************/

int ui_file_loadNet (Widget w, XtPointer button, caddr_t call_data)

{
    char     filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    char     string[MAX_NAME_LENGTH+MAX_NAME_LENGTH + 80]; 
             /* filename plus some text */
    Bool     performLoad = TRUE;
    char     *temp;
    int      ret_val = 0;

    /* check now for errors and react ! */
    if (strlen(ui_filenameNET) == 0) { 
	ui_confirmOk("No file specified");
	return (-1);
    }

    /* a filename is given */

    if (ui_filenameNET[0] == '/')
	sprintf(filename, "%s%s", ui_filenameNET, 
		ui_getExtension(UI_FILE_NET));
    else
	sprintf(filename, "%s/%s%s", ui_pathname, ui_filenameNET, 
		ui_getExtension(UI_FILE_NET));

    if (NOT ui_fileExist(filename, 0)) {
	ui_confirmOk("No such network file exists!");
	return(-1);
    }

    /* a file exists */
    
    if  (krui_getNoOfUnits() != 0) {
	if (NOT ui_confirmYes("Load will erase current network. Load?"))
	    performLoad = FALSE;
    } 

    if (INVERS_CREATED) {
       XtDestroyWidget(ui_InvRootWidget);
       INVERS_CREATED = 0;
    }
   
    if (performLoad) {
	sprintf(string,"Loading network: %s ...", filename);
	ui_printMessage(string);
	if(ui_controlIsCreated)ui_xSetLabel(ui_controlMessageWidget, "");
        d3_clear_xyTranslationTable ();
	ui_checkError(krui_loadNet(filename, &temp));
	if (ui_kernelError < 0){
	    ui_confirmOk("Error during loading the network!");
	} else {
	    ui_printMessage("Network loaded.");  
	    /* message to text window */
	    sprintf(string,"\n\n\nNetwork loaded from file:\n   %s\n",
		    filename);
	    ui_tw_printMessage(string);

	    /* try to load a configuration file with the same name */
            if (ui_loadCorrespondingCfg) {
                ui_loadCorrespondingCfg = TRUE;
	        sprintf(filename, "%s/%s%s", ui_pathname, ui_filenameNET, 
		        ui_getExtension(UI_FILE_CFG));
	        if (ui_fileExist(filename, 0) AND  
		    ui_confirmYes("Load corresponding configuration file")) {
		    ui_file_doLoadConfiguration(filename);
		    ret_val = 1;
	        }
	    }
	}

       /* initialization for graph */
        if(o_open){
          o_CurveNo += ((o_CurveLengths[o_CurveNo] != 0) && 
			(o_CurveNo < MAX_CURVE_NO-2)) ? (2) : (0);
          o_InitCurve();
          o_LearnStepCount = 0;
        }

	/* reset gui */
	ui_sel_resetList();       /* reset selections */
	ui_net_completeRefresh(ui_currentDisplay, UI_GLOBAL); /* show net */
	ui_printMessage("");
	ui_stat_displayStatus(ui_gridPosMouse);
	strcpy(ui_filenameSLNET, ui_filenameNET);
	ui_file_updateShellLabels();
	ui_info_makeUnitInfoPanelConsistent();
	ui_NumberOfLearnParamsChanged();
	ui_NumberOfUpdateParamsChanged();
	ui_NumberOfInitParamsChanged();
    }    
    
    return (ret_val);
}


/*****************************************************************************
  FUNCTION : ui_file_loadResult

  PURPOSE  : dummy callback for future use
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_file_loadResult (Widget w, XtPointer button, caddr_t call_data)

{
    ui_confirmOk("No sense in loading a result file");
}


/*****************************************************************************
  FUNCTION : ui_file_saveResult

  PURPOSE  : create or append result file
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_file_saveResult (Widget w, XtPointer button, caddr_t call_data)

{
    if (krui_getNoOfPatterns() == 0) 
    {
        ui_confirmOk("No patterns loaded");
        return;
    }
    ui_popupResult(w);
}


/* end of file */
/* lines: 694 */

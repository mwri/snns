/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_result.c,v $
  SHORTNAME      : result.c
  SNNS VERSION   : 4.2

  PURPOSE        : creation and management of result file widget

  AUTHOR         : Michael Vogt
  DATE           : 20.5.1992

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:41 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <stdio.h>

#include "ui.h"

#include "krui_typ.h"

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>

#include "ui_xWidgets.h"
#include "ui_confirmer.h"
#include "ui_main.h"
#include "ui_mainP.h"
#include "ui_netUpdate.h"
#include "ui_textP.h"
#include "ui_fileP.h"
#include "ui_utilP.h"
#include "kr_ui.h"
#include "ui_action.h"
#include "ui_control.h"
#include "ui_controlP.h"
#include "ui_result.ph"


/*****************************************************************************
  FUNCTION : ui_performRadioButtons

  PURPOSE  : toggles the buttons in an array of toggle buttons
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
*****************************************************************************/

static void ui_performRadioButtons (Widget array[], int size, int button)

{
	int	i;

	for (i=0; i<size; i++)
		ui_xSetToggleState(array[i], FALSE);
	ui_xSetToggleState(array[button], TRUE);
}


/*****************************************************************************
  FUNCTION : ui_result_Setinput

  PURPOSE  : callback for yes/no radio buttons of input patterns
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
******************************************************************************/

static void ui_result_Setinput (Widget w, int button, caddr_t call_data)

{
    ui_performRadioButtons(ui_resultInputRButtons, 2, button);
    ui_result_input_state = button;
}


/*****************************************************************************
  FUNCTION : ui_result_Setoutput

  PURPOSE  : callback for yes/no radio buttons of output patterns
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
******************************************************************************/

static void ui_result_Setoutput (Widget w, int button, caddr_t call_data)

{
    ui_performRadioButtons(ui_resultOutputRButtons, 2, button);
    ui_result_output_state = button;
}


/*****************************************************************************
  FUNCTION : ui_result_Setmode

  PURPOSE  : callback for create/append file mode selection
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
******************************************************************************/

static void ui_result_Setmode (Widget w, int button, caddr_t call_data)

{
    ui_performRadioButtons(ui_resultModeRButtons, 2, button);
    ui_result_mode_state = button;
}


/*****************************************************************************
  FUNCTION : ui_result_lastcall

  PURPOSE  : callback for setting of values of last call
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
******************************************************************************/

static void ui_result_lastcall (Widget w, int dummy, caddr_t call_data)

{
    char	buf[20];
    ui_performRadioButtons(ui_resultInputRButtons, 2, ui_result_input_state);
    ui_performRadioButtons(ui_resultOutputRButtons, 2, ui_result_output_state);
    ui_performRadioButtons(ui_resultModeRButtons, 2, ui_result_mode_state);
    sprintf(buf,"%d", ui_result_no_start_pat);
    ui_xSetString(ui_resultStartPattern, buf);
    sprintf(buf,"%d", ui_result_no_end_pat);
    ui_xSetString(ui_resultEndPattern, buf);
}


/*****************************************************************************
  FUNCTION : ui_result_default

  PURPOSE  : callback for setting of default values
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
******************************************************************************/

static void ui_result_default (Widget w, int dummy, caddr_t call_data)

{
    ui_result_input_state = UI_RESULT_INPUT_YES;
    ui_result_output_state = UI_RESULT_OUTPUT_NO;
    ui_result_mode_state = UI_RESULT_MODE_CREATE;
    ui_result_no_start_pat = 1;
    ui_result_no_end_pat = krui_getNoOfPatterns();
    ui_result_lastcall (w, dummy, call_data);
}


/*****************************************************************************
  FUNCTION : ui_resultDone

  PURPOSE  : callback for closing result file panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
******************************************************************************/

static void ui_resultDone (Widget w, int dummy, caddr_t call_data)

{
    char string[80+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    char filename[4+MAX_NAME_LENGTH+MAX_NAME_LENGTH];
    Bool performSave = TRUE;
    int maxNpos;

    ui_result_no_start_pat = ui_xIntFromAsciiWidget(ui_resultStartPattern);
    ui_result_no_end_pat = ui_xIntFromAsciiWidget(ui_resultEndPattern);

    if (ui_result_no_start_pat < 1 ||
	ui_result_no_end_pat > krui_getNoOfPatterns() ||
	ui_result_no_start_pat > ui_result_no_end_pat)
    {
	ui_confirmOk("No such pattern numbers");
	return;
    }

    if (strlen(ui_filenameRES) == 0) {
        ui_confirmOk("No file specified!");
	ui_popupDone(w, UI_POPUP_RESULT, call_data);
        return;
    }

    if (ui_filenameRES[0] == '/')
        sprintf(filename, "%s%s", ui_filenameRES, 
                ui_getExtension(UI_FILE_RES));
    else
        sprintf(filename, "%s/%s%s", ui_pathname, ui_filenameRES, 
                ui_getExtension(UI_FILE_RES));

    if (ui_fileExist(filename, 0) &&
	ui_result_mode_state == UI_RESULT_MODE_CREATE) 
    {
        sprintf(string,"%s already exist! Overwrite?", ui_filenameRES);
        performSave = ui_confirmYes(string);
    }
    
    if (performSave) 
    {
	ui_printMessage("Saving result file ...");
	ui_rem_getSubPatPanel();
	ui_checkError(krui_DefTrainSubPat(subIPatSize,subOPatSize,
					  subIPatStep,subOPatStep,&maxNpos));

	/* to ensure that the subpattern fits the network, try one step */
	ui_checkError(krui_DefShowSubPat(subIPatSize,subOPatSize,
					 subIPatPos,subOPatPos));
	ui_checkError(krui_showPattern(OUTPUT_NOTHING));

	if (ui_kernelError == 0)
	    ui_checkError(krui_saveResultParam(filename,
		ui_result_mode_state == UI_RESULT_MODE_CREATE,
		ui_result_no_start_pat,
		ui_result_no_end_pat,
		ui_result_input_state == UI_RESULT_INPUT_YES,
		ui_result_output_state == UI_RESULT_OUTPUT_YES,
		ui_updateParameters,UI_NO_UPDATE_PARAMS));

	if (ui_kernelError < 0)
	{
            ui_confirmOk("Error during saving result file!");
	    ui_printMessage("");
	}
        else 
	{
	    if (ui_result_mode_state == UI_RESULT_MODE_CREATE)
            	sprintf(string, "Result saved to file:\n   %s\n", filename);
	    else
		sprintf(string, "Result appended to file:\n   %s\n", filename);
            ui_tw_printMessage(string);
	    ui_printMessage("Result file saved.");
        }
	ui_net_completeRefresh(NULL, UI_GLOBAL);
    }
    ui_popupDone(w, UI_POPUP_RESULT, call_data);
}


/*****************************************************************************
  FUNCTION : ui_resultCancel

  PURPOSE  : callback for closing result file panel without doing anything
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
******************************************************************************/

static void ui_resultCancel (Widget w, int dummy, caddr_t call_data)

{
    ui_result_no_start_pat = ui_xIntFromAsciiWidget(ui_resultStartPattern);
    ui_result_no_end_pat = ui_xIntFromAsciiWidget(ui_resultEndPattern);
    ui_popupDone(w, UI_POPUP_RESULT, call_data);
}


/*****************************************************************************
  FUNCTION : ui_xCreateResultPanel

  PURPOSE  : creates a form widget for all result file elements 
  RETURNS  : the created widget
  NOTES    : some constants are used (see below) to get a relativ positioning
             of the items. This is VERY useful after changings.

  UPDATE   : 20.5.1992
*****************************************************************************/

static Widget ui_xCreateResultPanel (Widget parent)

{
    Widget	/* topLabel, */ startLabel, endLabel, inputLabel, outputLabel,
		createappendLabel, panel;

    int  fontWidth = 8;

    int  startWidth     =  25 * fontWidth;
    int  includeWidth =  25 * fontWidth;
    int  numberWidth = 7 * fontWidth;

    panel = XtCreateManagedWidget("rPanel", formWidgetClass, parent, 
		NULL, ZERO);

/*
    topLabel = ui_xCreateLabelItem("specify result file format:", panel,
		titelWidth, NULL, NULL);
*/

    startLabel = ui_xCreateLabelItem("start pattern :", panel,
		startWidth, NULL, NULL);

    ui_resultStartPattern = ui_xCreateDialogItem("startPattern", panel,
		" ", numberWidth, startLabel, NULL);
    
    endLabel = ui_xCreateLabelItem("end pattern   :", panel,
		startWidth, NULL, ui_resultStartPattern);

    ui_resultEndPattern = ui_xCreateDialogItem("endPattern", panel,
		" ", numberWidth, endLabel, ui_resultStartPattern);
    
    createappendLabel = ui_xCreateLabelItem("result file mode :", 
		panel, includeWidth, NULL, ui_resultEndPattern);

    ui_resultModeRButtons[UI_RESULT_MODE_CREATE] = 
		ui_xCreateToggleItem("create", panel, NULL,
		createappendLabel, ui_resultEndPattern);
    XtAddCallback(ui_resultModeRButtons[UI_RESULT_MODE_CREATE], XtNcallback, 
		(XtCallbackProc) ui_result_Setmode, (caddr_t) UI_RESULT_MODE_CREATE);

    ui_resultModeRButtons[UI_RESULT_MODE_APPEND] = 
		ui_xCreateToggleItem("append", panel, NULL,
		ui_resultModeRButtons[UI_RESULT_MODE_CREATE], 
		ui_resultEndPattern);
    XtAddCallback(ui_resultModeRButtons[UI_RESULT_MODE_APPEND], XtNcallback, 
		(XtCallbackProc) ui_result_Setmode, (caddr_t) UI_RESULT_MODE_APPEND);

    inputLabel = ui_xCreateLabelItem("include input patterns  :", panel,
		includeWidth, NULL, 
		ui_resultModeRButtons[UI_RESULT_MODE_CREATE]);

    ui_resultInputRButtons[UI_RESULT_INPUT_YES] = 
		ui_xCreateToggleItem("yes", panel, NULL,
		inputLabel, ui_resultModeRButtons[UI_RESULT_MODE_CREATE]);
    XtAddCallback(ui_resultInputRButtons[UI_RESULT_INPUT_YES], XtNcallback, 
		(XtCallbackProc) ui_result_Setinput, (caddr_t) UI_RESULT_INPUT_YES);

    ui_resultInputRButtons[UI_RESULT_INPUT_NO] = 
		ui_xCreateToggleItem("no", panel, NULL,
		ui_resultModeRButtons[UI_RESULT_MODE_CREATE], 
		ui_resultModeRButtons[UI_RESULT_MODE_APPEND]);
    XtAddCallback(ui_resultInputRButtons[UI_RESULT_INPUT_NO], XtNcallback, 
		(XtCallbackProc) ui_result_Setinput, (caddr_t) UI_RESULT_INPUT_NO);

    outputLabel = ui_xCreateLabelItem("include output patterns :", 
		panel, includeWidth, NULL, 
		ui_resultInputRButtons[UI_RESULT_INPUT_YES]);

    ui_resultOutputRButtons[UI_RESULT_OUTPUT_YES] = 
		ui_xCreateToggleItem("yes", panel, NULL,
		outputLabel, ui_resultInputRButtons[UI_RESULT_INPUT_YES]);
    XtAddCallback(ui_resultOutputRButtons[UI_RESULT_OUTPUT_YES], XtNcallback, 
		(XtCallbackProc) ui_result_Setoutput, (caddr_t) UI_RESULT_OUTPUT_YES);

    ui_resultOutputRButtons[UI_RESULT_OUTPUT_NO] = 
		ui_xCreateToggleItem("no", panel, NULL,
		ui_resultModeRButtons[UI_RESULT_MODE_CREATE], 
		ui_resultInputRButtons[UI_RESULT_INPUT_NO]);
    XtAddCallback(ui_resultOutputRButtons[UI_RESULT_OUTPUT_NO], XtNcallback, 
		(XtCallbackProc) ui_result_Setoutput, (caddr_t) UI_RESULT_OUTPUT_NO);

    XawFormDoLayout(panel, TRUE);
    ui_ResultIsCreated = TRUE;

    return panel;
}


/*****************************************************************************
  FUNCTION : ui_popupResult

  PURPOSE  : popup a result file panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.5.1992
******************************************************************************/

void ui_popupResult (Widget button)

{
    Widget      doneButton, cancelButton, defaultButton;
    Widget	ui_resultBox;
    Widget	ui_resultPanel;
    Arg		args[5];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;
    static Bool	first_call = TRUE;

    /* Upper left corner will be in the center of the calling button */

    if (NOT ui_ResultIsCreated) {

	n = 0;
	XtSetArg(args[0], XtNwidth, &width); n++;
	XtSetArg(args[1], XtNheight, &height); n++;
	XtGetValues(button, args, n);
	XtTranslateCoords(button, (Position) (width / 2), 
			  (Position) (height / 2),
			  &x, &y);

	n = 0;
	XtSetArg(args[n], XtNx, x);	n++;
	XtSetArg(args[n], XtNy, y);	n++;

	/* Now create Popup */

	ui_popResult = 
	    XtCreatePopupShell("result file format",
		 transientShellWidgetClass, ui_toplevel, args, n);

	ui_resultBox =
	    XtCreateManagedWidget("form", formWidgetClass, ui_popResult,
				  NULL, ZERO);

	ui_resultPanel = ui_xCreateResultPanel(ui_resultBox);

	doneButton = ui_xCreateButtonItem("done", ui_resultBox, NULL,
		ui_resultPanel);
	XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_resultDone, (caddr_t) UI_POPUP_RESULT);

	cancelButton = ui_xCreateButtonItem("cancel", ui_resultBox, doneButton,
		ui_resultPanel);
	XtAddCallback(cancelButton, XtNcallback, (XtCallbackProc) ui_resultCancel, (caddr_t) UI_POPUP_RESULT);

	defaultButton = ui_xCreateButtonItem("default2", ui_resultBox,
		cancelButton, ui_resultPanel);
	XtAddCallback(defaultButton, XtNcallback, (XtCallbackProc) ui_result_default, NULL);

	ui_checkWindowPosition(ui_popResult);
	XtPopup(ui_popResult, XtGrabExclusive);
        ui_xDontResizeWidget(ui_popResult); 
	if (first_call)
	{
	    ui_result_default(NULL, (int) NULL, NULL);
	    first_call = FALSE;
	}
	else
	{
	    ui_result_lastcall(NULL, (int) NULL, NULL);
	}
    }
}



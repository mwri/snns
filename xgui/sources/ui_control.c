/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_control.c,v $
  SHORTNAME      : control.c
  SNNS VERSION   : 4.2

  PURPOSE        : creates the widget containing all controls of the kernal.
  NOTES          : is called only during initialisation

  AUTHOR         : Tilman Sommer
  DATE           : 20.7.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 1.15 $ 
  LAST CHANGE    : $Date: 1998/04/20 11:55:08 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>

#include "ui.h"

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/MenuButton.h>

#include "kr_ui.h"
#include "ui_controlP.h"
#include "ui_xWidgets.h"
#include "ui_main.h"  /* ui_labelFontWidth */
#include "ui_mainP.h" /* ui_editPopup() */
#include "ui_utilP.h" /* ui_cutTrailingZeros() */
#include "ui_fileP.h"

#ifdef MASPAR_KERNEL
#include "ui_maspar.h"  /* MASPAR Kernel Interface */
#endif


#include "ui_control.ph"

/*****************************************************************************
  FUNCTION : ui_xCreateControlPanel

  PURPOSE  : creates a form widget for user input of unit data
  RETURNS  : void
  NOTES    : some constants are used (see below) to get a relativ positioning
             of the items. This is VERY useful after changings.

  UPDATE   : 19.02.95
*****************************************************************************/

void ui_xCreateControlPanel (Widget parent)

{
    /* tabs for data fields and small buttons */

    static struct SimpleListType  listSiteFType; /* site list in
						    f-type edit panel */
    static struct SimpleListType  listSiteSite;  /* site list in
						    site edit panel */
    static struct SimpleListType  listLearnFunc;  /* func list */
    static struct SimpleListType  listUpdateFunc; /* func list */
    static struct SimpleListType  listInitFunc;   /* func list */
    static struct SimpleListType  listRemapFunc;   /* func list */

    Widget      stepLabel, learnLabel, updateLabel, learn2Label, pattLabel;
    Widget      sButton, button, menu, entry, mItem;
    Widget      mdelButton,museButton, testLabel, initLabel, remapLabel;  
    Arg		args[10];
    Cardinal	n;
    int         i;
    char        buf[20];


/*
    int  fontWidth = 8;
*/
    int  titelWidth  =  7 * ui_labelFontWidth;
    int  numberWidth =  7 * ui_labelFontWidth;
    int  fileWidth   =  26* ui_labelFontWidth;

/***************************************************************************/

    ui_controlPanel = 
	XtCreateManagedWidget("rPanel", formWidgetClass, parent, NULL, ZERO);

/****************************  Steps Line  *********************************/

    stepLabel =	ui_xCreateLabelItem("STEPS", ui_controlPanel,
				    titelWidth, NULL, NULL);

    sprintf(buf,"%d",ui_numberOfSteps);
    ui_numberOfStepsWidget = ui_xCreateDialogItem("numberOfSteps",
						  ui_controlPanel, buf, 
						  numberWidth, stepLabel, NULL);

    sButton = ui_xCreateButtonItem("multiStep", ui_controlPanel, 
				   ui_numberOfStepsWidget, NULL);
    XtAddCallback(sButton, XtNcallback, (XtCallbackProc) ui_rem_stepsProc, 
		  (caddr_t) TRUE);

    sprintf(buf,"%d",ui_numberOfStepsDone);
    ui_numberOfStepsDoneWidget = ui_xCreateLabelItem(buf, ui_controlPanel, 
						     numberWidth, sButton,NULL);
    n = 0;
    XtSetArg(args[n], XtNborderWidth, 1); n++;
    XtSetArg(args[n], XtNinternalHeight, 3); n++;
    XtSetArg(args[n], XtNinternalWidth , 5); n++;
    XtSetArg(args[n], XtNhorizDistance, 5); n++; 
    XtSetValues(ui_numberOfStepsDoneWidget, args, n);
    jogbutton = ui_xCreateButtonItem("jog", ui_controlPanel,
				     ui_numberOfStepsDoneWidget, NULL);
    XtAddCallback(jogbutton, XtNcallback, (XtCallbackProc)ui_popupWeights,NULL);
   
    button = ui_xCreateButtonItem("init", ui_controlPanel, jogbutton, NULL);
    XtAddCallback(button, XtNcallback, (XtCallbackProc)ui_rem_doInitialization,
		  NULL);

    button = ui_xCreateButtonItem("reset", ui_controlPanel, button, NULL);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_resetNet, NULL);

    button = ui_xCreateButtonItem("error", ui_controlPanel, button, NULL);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_errProc, NULL);

    button = ui_xCreateButtonItem("info", ui_controlPanel, button, NULL);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_getKernelInfo, 
		  NULL);

/******************************  CYCLES  ***********************************/

    learnLabel = ui_xCreateLabelItem("CYCLES", ui_controlPanel,
				     titelWidth, NULL, stepLabel);

    sprintf(buf,"%d",ui_numberOfCycles);
    ui_numberOfCyclesWidget = ui_xCreateDialogItem("numberOfCycles", 
						   ui_controlPanel,
						   buf, numberWidth,  
						   learnLabel, stepLabel);

    button = ui_xCreateButtonItem("single", ui_controlPanel, 
				  ui_numberOfCyclesWidget, stepLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_learnProc, 
		  (caddr_t) UI_LEARN_SINGLE);

    button = ui_xCreateButtonItem("all", ui_controlPanel, sButton, stepLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_learnProc, 
		  (caddr_t) UI_LEARN_ALL);

    button = ui_xCreateButtonItem("stop", ui_controlPanel, button, stepLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_stopProc, NULL);

    button = ui_xCreateButtonItem("test", ui_controlPanel, button, stepLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_testProc, NULL);

    button = ui_xCreateToggleItem("shuffle", ui_controlPanel, NULL, button, 
				  stepLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_shuffleProc, 
		  NULL);
    
    button = ui_xCreateMenuButtonItem("editors", ui_controlPanel,
				      button, stepLabel);

    menu = XtCreatePopupShell("menu", simpleMenuWidgetClass, button,
			      NULL, ZERO);
    mItem = XtCreateManagedWidget("edit f-types", smeBSBObjectClass, 
				  menu, NULL, ZERO);
    sprintf(listSiteFType.msg,"Edit f-type table");
    listSiteFType.unitPtr  = NULL;
    listSiteFType.widgetPtr= button;
    listSiteFType.listType = UI_LIST_FTYPE_SITE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_editPopup, 
		  (caddr_t) &listSiteFType);

    mItem = XtCreateManagedWidget("edit sites", smeBSBObjectClass, 
				  menu, NULL, ZERO);
    sprintf(listSiteSite.msg,"Edit site table");
    listSiteSite.unitPtr  = NULL;
    listSiteSite.widgetPtr= button;
    listSiteSite.listType = UI_LIST_SITE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_editSitePopup, 
		  (caddr_t) &listSiteSite); 

    n=0;
    XtSetArg(args[n], XtNborderWidth, 1);n++;
    XtSetArg(args[n], XtNjustify, XtJustifyLeft); n++;
    XtSetArg(args[n], XtNinternalHeight, 0.99); n++;
    XtSetArg(args[n], XtNinternalWidth , 6); n++;
    XtSetArg(args[n], XtNfromVert , stepLabel);  n++;
    XtSetArg(args[n], XtNfromHoriz, button);  n++;
    XtSetArg(args[n], XtNvertDistance, 5); n++; 
    ui_showModeLabel = XtCreateManagedWidget("show", menuButtonWidgetClass, 
					     ui_controlPanel, args,n);
    ui_rem_showModeProc(NULL, ui_patternLoadMode, NULL); 

    n=0;
    XtSetArg(args[n], XtNvertDistance, -2); n++; 
    menu   = XtCreatePopupShell("menu",simpleMenuWidgetClass,ui_showModeLabel,
				args, n);
    entry = XtCreateManagedWidget("none", smeBSBObjectClass, menu,NULL,ZERO);
    XtAddCallback(entry, XtNcallback, (XtCallbackProc) 
		  ui_rem_showModeProc, (caddr_t) OUTPUT_NOTHING);
    entry = XtCreateManagedWidget("out", smeBSBObjectClass, menu,NULL,ZERO);
    XtAddCallback(entry, XtNcallback, 
		  (XtCallbackProc) ui_rem_showModeProc, (caddr_t) OUTPUT_OUT);
    entry = XtCreateManagedWidget("act", smeBSBObjectClass, menu,NULL,ZERO);
    XtAddCallback(entry, XtNcallback, 
		  (XtCallbackProc) ui_rem_showModeProc, (caddr_t) OUTPUT_ACT);


#ifdef PARAGON_KERNEL
    button = ui_xCreateToggleItem("parallel", ui_controlPanel, NULL, 
				  button, stepLabel);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_rem_SwitchParallel, NULL);
#endif
#ifdef MASPAR_KERNEL
    button = ui_xCreateButtonItem("maspar", ui_controlPanel, button, stepLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_masparPannel, 
		  (caddr_t) button);
#endif

/******************************* PATTERN  **********************************/

    pattLabel = ui_xCreateLabelItem("PATTERN", ui_controlPanel,
				    titelWidth, NULL, learnLabel);

    sprintf(buf,"%d",ui_noOfCurrentPattern);
    ui_numberOfPatternWidget = ui_xCreateDialogItem("patternNo",ui_controlPanel,
						    buf, numberWidth, 
						    pattLabel, learnLabel);

    button = ui_xCreateButtonItem("delete", ui_controlPanel, 
				  ui_numberOfPatternWidget, learnLabel);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_rem_deletePatternProc, NULL);

    button = ui_xCreateButtonItem("modify", ui_controlPanel, button,learnLabel);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_rem_modifyPatternProc, NULL);

    button = ui_xCreateButtonItem("new", ui_controlPanel, button, learnLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_rem_newPatternProc, 
		  NULL);

    button = ui_xCreateButtonItem("goto", ui_controlPanel, button, learnLabel);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_rem_moveInPatternsProc, (caddr_t)UI_JUMP);

    button = ui_xCreateButtonItem("first", ui_controlPanel, button, learnLabel);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc)ui_rem_moveInPatternsProc,(caddr_t) UI_FIRST);

    button = ui_xCreateButtonItem("prev", ui_controlPanel, button, learnLabel);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc)ui_rem_moveInPatternsProc, 
		  (caddr_t) UI_PREVIOUS);

    button = ui_xCreateButtonItem("next", ui_controlPanel, button, learnLabel);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_rem_moveInPatternsProc,(caddr_t) UI_NEXT);

    button = ui_xCreateButtonItem("last", ui_controlPanel, button, learnLabel);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc)ui_rem_moveInPatternsProc, (caddr_t)UI_LAST);

    button = ui_xCreateButtonItem("subpat", ui_controlPanel, button,learnLabel);
    XtAddCallback(button,XtNcallback,
		  (XtCallbackProc)ui_rem_defSubPat, (caddr_t)0);


/************************** SECOND PATTERN LINE  ***************************/


    mdelButton = ui_xCreateMenuButtonItem("del",ui_controlPanel,
					  ui_numberOfPatternWidget,pattLabel);
    REM_DFileMenu = XtCreatePopupShell("menu", simpleMenuWidgetClass, 
				       mdelButton, NULL, ZERO);

    if(PATTERN_SET_FILE[CURR_PATTERN_SET] != NULL){
	for(i=0; i<NO_OF_PATTERN_SETS;i++){
	    DELPattFileWidget[i] = XtCreateManagedWidget(PATTERN_SET_FILE[i],
						      smeBSBObjectClass,
						      REM_DFileMenu,NULL,ZERO);
	    XtAddCallback(DELPattFileWidget[i], XtNcallback, 
			  (XtCallbackProc)ui_rem_delPattSet,
			  (XtPointer) ((long)i));

	}
    }else{
	DELPattFileWidget[0] = XtCreateManagedWidget("No Files",
						     smeBSBObjectClass, 
						     REM_DFileMenu,NULL,ZERO);
	XtAddCallback(DELPattFileWidget[0], XtNcallback, 
		      (XtCallbackProc)ui_rem_delPattSet, (caddr_t)0);
    }

    museButton = ui_xCreateMenuButtonItem("use",ui_controlPanel,mdelButton,
					  pattLabel);
    REM_UFileMenu = XtCreatePopupShell("menu",simpleMenuWidgetClass,museButton,
				       NULL, ZERO);

    if(PATTERN_SET_FILE[CURR_PATTERN_SET] != NULL){
	for(i=0; i<NO_OF_PATTERN_SETS;i++){
	    USEPattFileWidget[i] = XtCreateManagedWidget(PATTERN_SET_FILE[i],
						      smeBSBObjectClass,
						      REM_UFileMenu,NULL,ZERO);
	    XtAddCallback(USEPattFileWidget[i], XtNcallback, 
			  (XtCallbackProc)ui_rem_usePattSet, 
			  (XtPointer) ((long)i));
	}
	sprintf(buf,"%s",PATTERN_SET_FILE[CURR_PATTERN_SET]);
    }else{
	USEPattFileWidget[0] = XtCreateManagedWidget("No Files",
						     smeBSBObjectClass, 
						     REM_UFileMenu,NULL,ZERO);
	XtAddCallback(USEPattFileWidget[0], XtNcallback, 
		      (XtCallbackProc)ui_rem_usePattSet, (caddr_t)0);
	sprintf(buf,"Training Pattern File ?");
    }

    ui_showPattFile = ui_xCreateLabelItem(" ", ui_controlPanel, 
					  fileWidth, museButton, pattLabel);
    ui_xSetLabel(ui_showPattFile, buf);

/******************************** TEST *************************************/

    testLabel =	ui_xCreateLabelItem("VALID", ui_controlPanel, titelWidth, NULL, 
				    mdelButton);

    sprintf(buf,"%d",ui_testFrequency);
    ui_testFrequencyWidget = ui_xCreateDialogItem("testFrequency", 
						 ui_controlPanel,buf,numberWidth,
						 testLabel,mdelButton);

    button = ui_xCreateButtonItem("validate",ui_controlPanel, 
				  ui_testFrequencyWidget,mdelButton);
    XtAddCallback(button,XtNcallback,
		  (XtCallbackProc)ui_rem_performValidate, (caddr_t)0);

    museButton = ui_xCreateMenuButtonItem("use",ui_controlPanel,button,
					  mdelButton);
    REM_UTestFileMenu = XtCreatePopupShell("menu",simpleMenuWidgetClass,
					   museButton, NULL, ZERO);

    if(PATTERN_SET_FILE[CURR_TEST_PATTERN_SET] != NULL){
       for(i=0; i<NO_OF_PATTERN_SETS;i++){
	   USETestPattFileWidget[i] = 
	       XtCreateManagedWidget(PATTERN_SET_FILE[i],smeBSBObjectClass,
				     REM_UTestFileMenu,NULL,ZERO);
	    XtAddCallback(USETestPattFileWidget[i], XtNcallback, 
			  (XtCallbackProc)ui_rem_useTestPattSet,
			  (XtPointer) ((long)i));
       }
       sprintf(buf,"%s",PATTERN_SET_FILE[CURR_TEST_PATTERN_SET]);
    }else{
	USETestPattFileWidget[0] = 
	    XtCreateManagedWidget("No Files",smeBSBObjectClass, 
				  REM_UTestFileMenu,NULL,ZERO);
	XtAddCallback(USETestPattFileWidget[0], XtNcallback, 
		      (XtCallbackProc)ui_rem_useTestPattSet, (caddr_t)0);
	sprintf(buf,"Validation Pattern File ?");
    }

    ui_showTestPattFile = ui_xCreateLabelItem(" ", ui_controlPanel, 
					      fileWidth, museButton,mdelButton);
    ui_xSetLabel(ui_showTestPattFile, buf);


/********************************** LEARN PARAMETER ************************/

    learn2Label = ui_xCreateLabelItem("LEARN", ui_controlPanel,
				      titelWidth, NULL, testLabel);

    for (i=0; i<UI_NO_LEARN_PARAMS; i++) {
	sprintf(buf,"%6.4f",ui_learnParameters[i]);
	ui_cutTrailingZeros(buf);
	ui_learnParameterWidgets[i] =
	    ui_xCreateDialogItem("learnParameter", ui_controlPanel,
				 buf, numberWidth, 
				 i==0?learn2Label:ui_learnParameterWidgets[i-1],
				 testLabel);
    }

    button = ui_xCreateMenuButtonItem("func",ui_controlPanel, 
				      ui_learnParameterWidgets[i-1], testLabel);

    sprintf(listLearnFunc.msg,"Select learning function");
    listLearnFunc.unitPtr  = NULL;
    listLearnFunc.widgetPtr= button;
    listLearnFunc.listType = UI_LIST_LEARN_FUNC;
    ui_rem_displayList(button, &listLearnFunc);




/*****************************  UPDATE PARAMETER  **************************/

    updateLabel = ui_xCreateLabelItem("UPDATE", ui_controlPanel,
				      titelWidth, NULL,  learn2Label);

    for (i=0; i<UI_NO_UPDATE_PARAMS; i++) {
	sprintf(buf,"%6.4f",ui_updateParameters[i]);
	ui_cutTrailingZeros(buf);
	ui_updateParameterWidgets[i] =
	    ui_xCreateDialogItem("updateParameter", ui_controlPanel,
				 buf, numberWidth, 
				 i==0?
			           updateLabel:ui_updateParameterWidgets[i-1], 
				 learn2Label);
    }

    button = ui_xCreateMenuButtonItem("func",ui_controlPanel, 
				  ui_updateParameterWidgets[i-1], learn2Label);
    sprintf(listUpdateFunc.msg,"Select update function");
    listUpdateFunc.unitPtr  = NULL;
    listUpdateFunc.widgetPtr= button;
    listUpdateFunc.listType = UI_LIST_UPDATE_FUNC;
    ui_rem_displayList(button, &listUpdateFunc);


/****************************** INIT PARAMETER *****************************/

    initLabel = ui_xCreateLabelItem("INIT", ui_controlPanel,
				      titelWidth, NULL, updateLabel);

    for (i=0; i<UI_NO_INIT_PARAMS; i++) {
	sprintf(buf,"%6.4f",ui_initParameters[i]);
	ui_cutTrailingZeros(buf);
	ui_initParameterWidgets[i] =
	    ui_xCreateDialogItem("initParameter", ui_controlPanel,
				 buf, numberWidth, 
				 i==0?initLabel:ui_initParameterWidgets[i-1],
				 updateLabel);
    }
    button = ui_xCreateMenuButtonItem("func",ui_controlPanel, 
				  ui_initParameterWidgets[i-1], updateLabel);

    sprintf(listInitFunc.msg,"Select init function");
    listInitFunc.unitPtr  = NULL;
    listInitFunc.widgetPtr= button;
    listInitFunc.listType = UI_LIST_INIT_FUNC;
    ui_rem_displayList(button, &listInitFunc);


/****************************** REMAP PARAMETER *****************************/

    remapLabel = ui_xCreateLabelItem("REMAP", ui_controlPanel,
				      titelWidth, NULL, initLabel);

    for (i=0; i<UI_NO_REMAP_PARAMS; i++) {
	sprintf(buf,"%6.4f",ui_remapParameters[i]);
	ui_cutTrailingZeros(buf);
	ui_remapParameterWidgets[i] =
	    ui_xCreateDialogItem("remapParameter", ui_controlPanel,
				 buf, numberWidth, 
				 i==0?remapLabel:ui_remapParameterWidgets[i-1],
				 initLabel);
    }
    button = ui_xCreateMenuButtonItem("func",ui_controlPanel, 
				  ui_remapParameterWidgets[i-1], initLabel);
    sprintf(listRemapFunc.msg,"Select remap function");
    listRemapFunc.unitPtr  = NULL;
    listRemapFunc.widgetPtr= button;
    listRemapFunc.listType = UI_LIST_REMAP_FUNC;
    ui_rem_displayList(button, &listRemapFunc);


/***************************************************************************/

    XawFormDoLayout(ui_controlPanel, True);
    ui_controlIsCreated = TRUE;
    ui_NumberOfLearnParamsChanged();
    ui_NumberOfUpdateParamsChanged();
    ui_NumberOfInitParamsChanged();
    ui_NumberOfRemapParamsChanged();
}




/*****************************************************************************
  FUNCTION : ui_NumberOfLearnParamsChanged

  PURPOSE  : hiding all unused learn parameter widgets
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void ui_NumberOfLearnParamsChanged(void)
{
    int in=UI_NO_LEARN_PARAMS;
    int out=0;
    int i;
    Arg args[5];
    Cardinal n;
    if(!ui_controlIsCreated)
	return;
    krui_getFuncParamInfo(krui_getLearnFunc(), LEARN_FUNC, &in, &out);
    n = 0;
    XtSetArg(args[n], XtNsensitive, TRUE); n++;
    XtSetArg(args[n], XtNmappedWhenManaged, TRUE); n++;
    for(i=0; i < in; i++){
	XtSetValues(ui_learnParameterWidgets[i], args, n);
    }
    n = 0;
    XtSetArg(args[n], XtNsensitive, FALSE); n++;
    XtSetArg(args[n], XtNmappedWhenManaged, FALSE); n++;
    while(i < UI_NO_LEARN_PARAMS) {
	XtSetValues(ui_learnParameterWidgets[i], args, n);
	ui_xSetString(ui_learnParameterWidgets[i],"");
	i++;
    }
}

/*****************************************************************************
  FUNCTION : ui_NumberOfUpdateParamsChanged

  PURPOSE  : hiding all unused update parameter widgets

  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void ui_NumberOfUpdateParamsChanged(void)
{
    int in=UI_NO_UPDATE_PARAMS;
    int out=0;
    int i;
    Arg args[5];
    Cardinal n;
    if(!ui_controlIsCreated)
	return;
    krui_getFuncParamInfo(krui_getUpdateFunc(), UPDATE_FUNC, &in, &out);
    n = 0;
    XtSetArg(args[n], XtNsensitive, TRUE); n++;
    XtSetArg(args[n], XtNmappedWhenManaged, TRUE); n++;
    for(i=0; i < in; i++){
	XtSetValues(ui_updateParameterWidgets[i], args, n);
    }
    n = 0;
    XtSetArg(args[n], XtNsensitive, FALSE); n++;
    XtSetArg(args[n], XtNmappedWhenManaged, FALSE); n++;
    while(i < UI_NO_UPDATE_PARAMS) {
	XtSetValues(ui_updateParameterWidgets[i], args, n);
	ui_xSetString(ui_updateParameterWidgets[i],"");
	i++;
    }
}

/*****************************************************************************
  FUNCTION : ui_NumberOfInitParamsChanged

  PURPOSE  : hiding all unused init parameter widgets
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void ui_NumberOfInitParamsChanged(void)
{
    int in=UI_NO_INIT_PARAMS;
    int out=0;
    int i;
    Arg args[5];
    Cardinal n;
    if(!ui_controlIsCreated)
	return;
    krui_getFuncParamInfo(krui_getInitialisationFunc(), INIT_FUNC, &in, &out);
    n = 0;
    XtSetArg(args[n], XtNsensitive, TRUE); n++;
    XtSetArg(args[n], XtNmappedWhenManaged, TRUE); n++;
    for(i=0; i < in; i++){
	XtSetValues(ui_initParameterWidgets[i], args, n);
    }
    n = 0;
    XtSetArg(args[n], XtNsensitive, FALSE); n++;
    XtSetArg(args[n], XtNmappedWhenManaged, FALSE); n++;
    while(i < UI_NO_INIT_PARAMS) {
	XtSetValues(ui_initParameterWidgets[i], args, n);
	ui_xSetString(ui_initParameterWidgets[i],"");
	i++;
    }
}


/*****************************************************************************
  FUNCTION : ui_NumberOfRemapParamsChanged

  PURPOSE  : hiding all unused remap parameter widgets
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void ui_NumberOfRemapParamsChanged(void)
{
    int i, error, border;
    Arg args[5];
    Cardinal n;
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

    if(!ui_controlIsCreated)
	return;

    if((error = krui_GetPatInfo(&patt_info, &descrip))< 0){
	border = 0;
    }else{
	border = patt_info.no_of_remap_params;
    }

    XtSetArg(args[0], XtNsensitive, TRUE);
    XtSetArg(args[1], XtNmappedWhenManaged, TRUE);
    for(i=0; i<border; i++){
	XtSetArg(args[2], XtNlabel, patt_info.remap_params[i]);
	XtSetValues(ui_remapParameterWidgets[i], args, 3);
    }
    n = 0;
    XtSetArg(args[n], XtNsensitive, FALSE); n++;
    XtSetArg(args[n], XtNmappedWhenManaged, FALSE); n++;
    for(i=border; i<UI_NO_REMAP_PARAMS;i++) {
	XtSetValues(ui_remapParameterWidgets[i], args, n);
	ui_xSetString(ui_remapParameterWidgets[i],"");
    }
}

/* end of file */

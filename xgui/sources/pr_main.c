/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/pr_main.c,v $
  SHORTNAME      : pr_main.c
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the pruning window.
  NOTES          :

  AUTHOR         : Tobias Schreiner
  DATE           : 13.3.94

  CHANGED BY     : Thomas Gern, Guenter Mamier
  RCS VERSION    : $Revision: 2.17 $
  LAST CHANGE    : $Date: 1998/03/10 15:14:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <X11/Intrinsic.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/SimpleMenu.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_xWidgets.h"
#include "prun_f.h"
#include "ui_main.h"
#include "ui_mainP.h"
#include "ui_key.h"

#include "pr_main.ph"


/*****************************************************************************
  FUNCTION : pr_readElements

  PURPOSE  : get the current settings of the pruning panel
  NOTES    :

  UPDATE   :
******************************************************************************/
void pr_readElements(void)
{    
    if(!pr_open) return;

    /* store maximum error increase from dialog */
    pr_maxErrorInc = ui_xFloatFromAsciiWidget (pr_maxErrorIncDialog);
    if (pr_maxErrorInc < 0.0)pr_maxErrorInc = 0.0;

    /* store accepted error from dialog */
    pr_acceptedError = ui_xFloatFromAsciiWidget (pr_acceptedErrorDialog);
    if (pr_acceptedError < 0.0)pr_acceptedError = 0.0;

    /* store value of toggle item for recreating */
    pr_recreate = ui_xGetToggleState (pr_recreateB[1]);

    /* store value of toggle item for refresh */
    pr_refresh = ui_xGetToggleState (pr_refreshB[1]);

    /* store values for training and retraining of network */
    pr_trainCycles = ui_xIntFromAsciiWidget (pr_trainCyclesDialog);
    if (pr_trainCycles < 0)pr_trainCycles = 0;
    pr_retrainCycles = ui_xIntFromAsciiWidget (pr_retrainCyclesDialog);
    if (pr_retrainCycles < 0)pr_retrainCycles = 0;

    /* store minimum error from dialog */
    pr_minError = ui_xFloatFromAsciiWidget (pr_minErrorDialog);
    if (pr_minError < 0.0)pr_minError = 0.0;

    /* store obs init parameter from dialog */
    pr_obs_initParameter = ui_xFloatFromAsciiWidget (pr_obsInitParamDialog);
    if (pr_obs_initParameter < 0)pr_obs_initParameter = 0;

    /* store value of toggle item for input/hidden pruning */
    pr_inputPruning  = ui_xGetToggleState (pr_inputPruningB[1]);
    pr_hiddenPruning = ui_xGetToggleState (pr_hiddenPruningB[1]);
}



/*****************************************************************************
  FUNCTION : pr_doneProc

  PURPOSE  : callback function of the done-button
  NOTES    :

  UPDATE   :
******************************************************************************/
static void pr_doneProc(void)
{

    /* get the current values from the panel*/
    pr_readElements();

    /* close pruning window */
    XtDestroyWidget (pr_mainWidget);
    pr_open = FALSE;

}


/*****************************************************************************
  FUNCTION : pr_createWindow

  PURPOSE  : creates the PRUNING-window
  NOTES    : 

  UPDATE   : 14.11.97
******************************************************************************/
void pr_createWindow (void)
{

    Widget box, formprun, formtrain, formobs, formnodeprun, label,
	frameTitle, done, trainCyclesText, retrainCyclesText,
	minErrorText, obsInitParamText;
    Widget menu,mItem;
    char buf [40];
    char empty[] = "                                                  ";
    int  text_width, text_width2, text_width3, dialog_width, title_width;


    if(pr_open){	/* window is displayed */
	/* get window into foreground */
	XRaiseWindow (XtDisplay (pr_mainWidget), XtWindow (pr_mainWidget));
	return;
    }

    /* window is not yet displayed */

    pr_open = TRUE;

    /* initialize geometry variables */
    text_width = ui_fontStruct->max_bounds.width * 42;
    text_width2 = ui_fontStruct->max_bounds.width * 41;
    text_width3 = ui_fontStruct->max_bounds.width * 26;
    dialog_width = ui_fontStruct->max_bounds.width * 10;
    title_width = ui_fontStruct->max_bounds.width * 52;

    /* open window */
    pr_mainWidget = XtCreatePopupShell("SNNS Pruning", topLevelShellWidgetClass,
				       ui_toplevel, NULL, ZERO);

    /* open box */
    box = XtCreateManagedWidget("box", boxWidgetClass, pr_mainWidget,NULL,ZERO);

    /* --- panel for pruning parameter input --- */
    formprun = XtCreateManagedWidget("panelprun",formWidgetClass,box,NULL,ZERO);

    /* creating frame title */
    frameTitle = ui_xCreateLabelItem("      General Parameters for Pruning", 
				     formprun, title_width,NULL,NULL);

    /* creating button for input of pruning function */
    pr_prunFuncText = ui_xCreateLabelItem("Pruning function: ",formprun,
					  text_width3,NULL,frameTitle);
    sprintf (buf,"%s",krui_getPrunFunc());
    empty[(int)(11-strlen(buf)*0.5)] = '\0';
    strcat(empty,buf);
    strncat(empty,empty,(int)(11-strlen(buf)*0.5));
    pr_prunFuncIcon = ui_xCreateMenuButtonItem(empty,formprun,
					       pr_prunFuncText,frameTitle);

    menu = XtCreatePopupShell("menu",simpleMenuWidgetClass,pr_prunFuncIcon,
			      NULL,ZERO);
    mItem = XtCreateManagedWidget("MagPruning", smeBSBObjectClass,menu,
				  NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)1);
    mItem = XtCreateManagedWidget("OptimalBrainSurgeon", smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)2);
    mItem = XtCreateManagedWidget("OptimalBrainDamage", smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)3);
    mItem = XtCreateManagedWidget("Skeletonization", smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)4);
    mItem = XtCreateManagedWidget("Noncontributing_Units",smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)5);

    /* creating dialog for maximum error increase */
    label = ui_xCreateLabelItem("Maximum error increase in %: ", formprun, 
				text_width, NULL, pr_prunFuncIcon);
    sprintf (buf, "%g", pr_maxErrorInc);
    pr_maxErrorIncDialog = ui_xCreateDialogItem("dialogE", formprun, buf, 
						dialog_width, label, 
						pr_prunFuncIcon);

    /* creating dialog for accepted error */
    label = ui_xCreateLabelItem("Max. accepted SSE: ", formprun,
				text_width, NULL, pr_maxErrorIncDialog);
    sprintf (buf, "%g", pr_acceptedError);
    pr_acceptedErrorDialog = 
	ui_xCreateDialogItem("dialogE", formprun, buf, dialog_width, 
			     label, pr_maxErrorIncDialog);

    /* creating toggle widget for selecting recreating */
    label = ui_xCreateLabelItem("Recreate last pruned element ?", formprun, 
				text_width2, NULL, pr_acceptedErrorDialog);
    pr_recreateB[1] = ui_xCreateToggleItem("yes", formprun, NULL, 
					  label, pr_acceptedErrorDialog);
    ui_xSetToggleState (pr_recreateB[1], pr_recreate);
    XtAddCallback(pr_recreateB[1], XtNcallback, (XtCallbackProc) pr_setrecreate,
		   (caddr_t) 1);
    pr_recreateB[0] = ui_xCreateToggleItem("no",formprun,NULL,pr_recreateB[1],
					   pr_acceptedErrorDialog);
    ui_xSetToggleState(pr_recreateB[0], !pr_recreate);
    XtAddCallback(pr_recreateB[0], XtNcallback, (XtCallbackProc) pr_setrecreate,
	          (caddr_t) 0);

    /* creating toggle widget for selecting display refresh */
    label = ui_xCreateLabelItem("Refresh display after pruning step ?",
				formprun, text_width2, NULL, pr_recreateB[0]);
    pr_refreshB[1] = ui_xCreateToggleItem("yes", formprun, NULL, 
					  label, pr_recreateB[0]);
    ui_xSetToggleState (pr_refreshB[1], pr_refresh);
    XtAddCallback (pr_refreshB[1], XtNcallback, (XtCallbackProc) pr_setrefresh,
		   (caddr_t) 1);
    pr_refreshB[0] = ui_xCreateToggleItem("no", formprun, NULL ,
					  pr_refreshB[1], pr_recreateB[0]);
    ui_xSetToggleState(pr_refreshB[0], !pr_refresh);
    XtAddCallback(pr_refreshB[0], XtNcallback, (XtCallbackProc) pr_setrefresh,
		  (caddr_t) 0);


    /* --- panel for training parameter input --- */
    formtrain=XtCreateManagedWidget("paneltrain",formWidgetClass,box,NULL,ZERO);

    /* creating frame title */
    frameTitle=ui_xCreateLabelItem("      General Parameters for Training", 
				   formtrain,title_width,NULL,NULL);

    /* creating button for input of learning function */
    pr_learnFuncText = ui_xCreateLabelItem("Learning function: ",formtrain,
					   text_width3, NULL, frameTitle);
    sprintf (buf,"%s",krui_getFFLearnFunc());
    strcpy(empty,"                                   ");
    empty[(int)(12-strlen(buf)*0.5)] = '\0';
    strcat(empty,buf);
    strncat(empty,empty,(int)(11-strlen(buf)*0.5));
    pr_learnFuncIcon = ui_xCreateMenuButtonItem(empty,formtrain,
						pr_learnFuncText,frameTitle);

    menu = XtCreatePopupShell("menu",simpleMenuWidgetClass,pr_learnFuncIcon,
			      NULL,ZERO);
    mItem = XtCreateManagedWidget("Std_Backpropagation", smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)6);
    mItem = XtCreateManagedWidget("BackpropBatch", smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)7);
    mItem = XtCreateManagedWidget("BackpropMomentum", smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)8);
    mItem = XtCreateManagedWidget("BackpropWeightDecay", smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)9);
    mItem = XtCreateManagedWidget("Quickprop",smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)10);
    mItem = XtCreateManagedWidget("Rprop",smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)11);
    mItem = XtCreateManagedWidget("SCG",smeBSBObjectClass,
				  menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)pr_setFunc,(caddr_t)12);

    /* creating dialog for learn cycles */
    trainCyclesText=ui_xCreateLabelItem("Learn cycles for first training: ",
				 formtrain, text_width, NULL, pr_learnFuncIcon);
    sprintf (buf, "%d", pr_trainCycles);
    pr_trainCyclesDialog = ui_xCreateDialogItem("dialogT", formtrain, buf, 
				dialog_width,trainCyclesText, pr_learnFuncIcon);

    /* creating dialog for retrain cycles */
    retrainCyclesText = ui_xCreateLabelItem("Learn cycles for retraining: ",
			          formtrain, text_width, NULL, trainCyclesText);
    sprintf (buf, "%d", pr_retrainCycles);
    pr_retrainCyclesDialog = ui_xCreateDialogItem("dialogR", formtrain, buf,
			      dialog_width, retrainCyclesText, trainCyclesText);

    /* creating dialog for minimum error */
    minErrorText = ui_xCreateLabelItem("Minimum error to stop: ",formtrain, 
				       text_width, NULL, retrainCyclesText);
    sprintf (buf, "%g", pr_minError);
    pr_minErrorDialog = ui_xCreateDialogItem("dialogM", formtrain, buf, 
			        dialog_width, minErrorText, retrainCyclesText);


    /* --- panel for obs parameters --- */
    formobs = XtCreateManagedWidget("panelobs",formWidgetClass,box,NULL,ZERO);

    /* creating frame title */
    frameTitle = ui_xCreateLabelItem("            Parameters for OBS", 
				     formobs,title_width,NULL,NULL);

    /* creating dialog for pruning parameter */
    obsInitParamText = ui_xCreateLabelItem("Initial value for matrix: ", 
				   formobs, text_width, NULL, frameTitle);
    sprintf (buf, "%g", pr_obs_initParameter);
    pr_obsInitParamDialog = ui_xCreateDialogItem("dialogI", formobs, buf, 
				 dialog_width, obsInitParamText, frameTitle);


    /* ------ panel for node pruning parameters ------ */
    formnodeprun = XtCreateManagedWidget("panelnodeprun", formWidgetClass, 
					 box, NULL, ZERO);

    /* create frame title */
    frameTitle = ui_xCreateLabelItem("       Parameters for Node Pruning", 
				     formnodeprun,title_width,NULL,NULL); 

    /* create toggle widget for selecting input pruning */
    label = ui_xCreateLabelItem("Input Pruning ?", formnodeprun, text_width2,
				NULL, frameTitle);
    pr_inputPruningB[1] = ui_xCreateToggleItem("yes", formnodeprun, NULL, 
					       label, frameTitle);
    ui_xSetToggleState (pr_inputPruningB[1], pr_inputPruning);
    XtAddCallback(pr_inputPruningB[1], XtNcallback, 
		  (XtCallbackProc) pr_setinputPrun,(caddr_t) 1);
    pr_inputPruningB[0] = ui_xCreateToggleItem("no", formnodeprun, NULL,
					       pr_inputPruningB[1], frameTitle);
    ui_xSetToggleState(pr_inputPruningB[0], !pr_inputPruning);
    XtAddCallback(pr_inputPruningB[0], XtNcallback, 
		  (XtCallbackProc) pr_setinputPrun,(caddr_t) 0);

    /* create toggle widget for selecting hidden pruning */
    label = ui_xCreateLabelItem("Hidden Pruning ?",formnodeprun,
				text_width2, NULL, pr_inputPruningB[0]);
    pr_hiddenPruningB[1] = ui_xCreateToggleItem("yes", formnodeprun, NULL,
						label, pr_inputPruningB[0]);
    ui_xSetToggleState (pr_hiddenPruningB[1], pr_hiddenPruning);
    XtAddCallback(pr_hiddenPruningB[1], XtNcallback, 
		  (XtCallbackProc) pr_sethiddenPrun,(caddr_t) 1);
    pr_hiddenPruningB[0] = ui_xCreateToggleItem("no", formnodeprun, NULL,
						pr_hiddenPruningB[1],
						pr_inputPruningB[0]);
    ui_xSetToggleState (pr_hiddenPruningB[0], !pr_hiddenPruning);
    XtAddCallback(pr_hiddenPruningB[0], XtNcallback, 
		  (XtCallbackProc) pr_sethiddenPrun,(caddr_t) 0);


    /* --- done button --- */
    done = ui_xCreateButtonItem ("done", box, NULL, formnodeprun);
    XtAddCallback(done, XtNcallback, (XtCallbackProc) pr_doneProc, NULL);

    XtAddEventHandler(formprun,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    ui_checkWindowPosition (pr_mainWidget);
    XtPopup (pr_mainWidget, XtGrabNone);
    ui_xDontResizeWidget (pr_mainWidget);
}



/*****************************************************************************
  FUNCTION : pr_setinputPrun

  PURPOSE  : callback function for the input pruning radio buttons
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/
static void pr_setinputPrun(Widget w, int value, caddr_t call_data)
{
    ui_setToggles(value, &pr_inputPruning, 2, pr_inputPruningB);
}



/*****************************************************************************
  FUNCTION : pr_sethiddenPrun

  PURPOSE  : callback function for the recreate radio buttons
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/
static void pr_sethiddenPrun(Widget w, int value, caddr_t call_data)
{
    ui_setToggles(value, &pr_hiddenPruning, 2, pr_hiddenPruningB);
}



/*****************************************************************************
  FUNCTION : pr_setrecreate

  PURPOSE  : callback function for the recreate radio buttons
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/
static void pr_setrecreate(Widget w, int value, caddr_t call_data)
{
    ui_setToggles(value, &pr_recreate, 2, pr_recreateB);
}


/*****************************************************************************
  FUNCTION : pr_setrefresh

  PURPOSE  : callback function for the refresh radio buttons
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/
static void pr_setrefresh(Widget w, int value, caddr_t call_data)
{
    ui_setToggles(value, &pr_refresh, 2, pr_refreshB);
}


/*****************************************************************************
  FUNCTION : pr_setFunc

  PURPOSE  : callback function for the menues
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_setFunc(Widget w, int value, caddr_t call_data)
{
    switch (value){
    case 1: ui_xSetLabel(pr_prunFuncIcon,"     MagPruning      ");
  	    krui_setPrunFunc("MagPruning");
	    break;
    case 2: ui_xSetLabel(pr_prunFuncIcon," OptimalBrainSurgeon ");
  	    krui_setPrunFunc("OptimalBrainSurgeon");
	    break;
    case 3: ui_xSetLabel(pr_prunFuncIcon," OptimalBrainDamage  ");
  	    krui_setPrunFunc("OptimalBrainDamage");
	    break;
    case 4: ui_xSetLabel(pr_prunFuncIcon,"   Skeletonization   ");
  	    krui_setPrunFunc("Skeletonization");
	    break;
    case 5: ui_xSetLabel(pr_prunFuncIcon,"Noncontributing_Units");
  	    krui_setPrunFunc("Noncontributing_Units");
	    break;

    case 6: ui_xSetLabel(pr_learnFuncIcon,"Std_Backpropagation");
  	    krui_setFFLearnFunc("Std_Backpropagation");
	    break;
    case 7: ui_xSetLabel(pr_learnFuncIcon,"BackpropBatch");
  	    krui_setFFLearnFunc("BackpropBatch");
	    break;
    case 8: ui_xSetLabel(pr_learnFuncIcon,"BackpropMomentum");
  	    krui_setFFLearnFunc("BackpropMomentum");
	    break;
    case 9: ui_xSetLabel(pr_learnFuncIcon,"BackpropWeightDecay");
  	    krui_setFFLearnFunc("BackpropWeightDecay");
	    break;
    case 10: ui_xSetLabel(pr_learnFuncIcon,"Quickprop");
  	    krui_setFFLearnFunc("Quickprop");
	    break;
    case 11: ui_xSetLabel(pr_learnFuncIcon,"Rprop");
  	    krui_setFFLearnFunc("Rprop");
	    break;
    case 12: ui_xSetLabel(pr_learnFuncIcon,"SCG");
  	    krui_setFFLearnFunc("SCG");
	    break;
    }
}

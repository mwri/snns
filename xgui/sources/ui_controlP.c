/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_controlP.c,v $
  SHORTNAME      : controlP.c
  SNNS VERSION   : 4.2

  PURPOSE        : routines to control learning, updating the net
  NOTES          : Most of this routines are callbacks of buttons in the control-
                   panel.

  AUTHOR         : Tilman Sommer
  DATE           : 18.7.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 1.58 $ 
  LAST CHANGE    : $Date: 1998/05/15 13:12:14 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ui.h"
#include "ui_mainP.h"
#include "ui_utilP.h"
#include "ui_selection.h"
#include "ui_netUpdate.h"

#include "glob_typ.h"
#include "kr_ui.h"
#include "ext_typ.h"
#include "kr_funcs.h"

#include "ui_fileP.h"
#include "ui_confirmer.h"
#include "ui_textP.h"
#include "ui_xWidgets.h"
#include "ui_display.h"
#include "ui_action.h"
#include "ui_control.h"
#include "ui_status.h"
#include "ui_infoP.h"
#include "d3_main.h"
#include "net_analyzer.h"
#include "ui_key.h"
#include "kr_typ.h"
#include "cc_glob.h"
#include "cc_main.h"
#include "dlvq_learn.h"
#include "ui_inversion.h"
#include "ui_displwght.h"
#include "ui_kohonen.h"
#include "prun_f.h"
#include "pr_main.h"
#include "ui_class.h"

#include "o_graph.h"
#include "ui_main.h"
#include "ui_event.h"

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Shell.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include <X11/Intrinsic.h>
#include "ui_lists.h"


#include "ui_controlP.ph"

#ifdef PARAGON_KERNEL
#include PARAGON_INCLUDE
#include "kr_ipdef.h"
#endif

/*****************************************************************************
  FUNCTION : ui_rem_displayCounter

  PURPOSE  : displays the value in ui_numberOfStepsDone in the control panel
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/
static void ui_rem_displayCounter (void)
{
    char  buf[50];
    if (NOT ui_controlIsCreated) return;

    sprintf(buf,"%d", ui_numberOfStepsDone);
    ui_xSetLabel(ui_numberOfStepsDoneWidget, buf);
}


/*****************************************************************************
  FUNCTION : ui_rem_displayPatternNumber

  PURPOSE  : displays the value in ui_numberOfCurrentPattern in the 
             control panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 19.02.95
*****************************************************************************/
void ui_rem_displayPatternNumber (void)
{
    char  buf[50];

    if (NOT ui_controlIsCreated) return;

    sprintf(buf,"%d", ui_noOfCurrentPattern);
    ui_xSetString(ui_numberOfPatternWidget, buf);
    ui_xSetLabel(ui_showPattFile, PATTERN_SET_FILE[CURR_PATTERN_SET]);
    ui_xSetLabel(ui_showTestPattFile, PATTERN_SET_FILE[CURR_TEST_PATTERN_SET]);
}



/*****************************************************************************
  FUNCTION : ui_rem_resetCounter()

  PURPOSE  : set counter to 0
  RETURNS  : VOID
  NOTES    :

  UPDATE   : 30.7.1990
******************************************************************************/
void ui_rem_resetCounter (void)
{
    ui_numberOfStepsDone = 0;
    ui_rem_displayCounter();
}



/*****************************************************************************
  FUNCTION : ui_rem_incCounter()

  PURPOSE  : increment counter by 1
  RETURNS  :
  NOTES    :

  UPDATE   : 30.7.1990
******************************************************************************/
static void ui_rem_incCounter (void)
{
    ui_numberOfStepsDone += 1;
    ui_rem_displayCounter();
}


/*****************************************************************************
  FUNCTION : ui_rem_resetNet

  PURPOSE  : Resets the net to the initial activation and resets the counter
  NOTES    :

  UPDATE   : 24.8.1990
******************************************************************************/
void ui_rem_resetNet (Widget w, XtPointer button, caddr_t call_data)
{
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
    ui_net_updateWhole(NULL, UI_GLOBAL, UI_DRAW_UNITS, UI_ERASE);
    krui_resetNet();
    ui_rem_resetCounter();
    ui_net_updateWhole(NULL, UI_GLOBAL, UI_DRAW_UNITS, UI_DRAW);
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
}


/*****************************************************************************
  FUNCTION : ui_rem_doInitialization

  PURPOSE  : get the parameters and call the kernel
  RETURNS  : void
  NOTES    : 

  UPDATE   : 19.02.95
******************************************************************************/
void ui_rem_doInitialization (Widget w, caddr_t client_data, caddr_t call_data)
{
    int i;
    char *func_name;
 
    for (i=0; i< UI_NO_INIT_PARAMS; i++) {
	ui_initParameters[i] =
	    ui_xFloatFromAsciiWidget(ui_initParameterWidgets[i]);
    }

    /* check parameters for three special init functions */
    func_name = krui_getInitialisationFunc();
    if((strcmp(func_name,"CPN_Weights_v3.2") == 0) ||
       (strcmp(func_name,"CPN_Weights_v3.3") == 0) ||
       (strcmp(func_name,"KOHONEN_Weights_v3.2") == 0)){
	   if((ui_initParameters[0] != -1 && ui_initParameters[0] != 0  &&
	       ui_initParameters[0] != 1) || (ui_initParameters[1] != -1 && 
					      ui_initParameters[1] != 0  &&
					      ui_initParameters[1] != 1)){
	       ui_confirmOk("Wrong parameters given\nIntervall must be [-1,1], [-1,0], or [0,1]");
	       return;
	   }
    }

    if(o_open){
      o_CurveNo += ((o_CurveLengths[o_CurveNo] != 0) && 
		    (o_CurveNo < MAX_CURVE_NO-2)) ? (2) : (0); 
      o_InitCurve();
      o_LearnStepCount = 0;
    }
 
    ui_checkError(krui_initializeNet(ui_initParameters, UI_NO_INIT_PARAMS));
    if (ui_displ_isSomeWhereToShowWeights())
	ui_net_completeRefresh(NULL, UI_GLOBAL);
    else {
        ui_net_completeRefresh(NULL, UI_GLOBAL);
	ui_net_updateWhole(NULL, UI_GLOBAL, UI_DRAW_UNITS, UI_DRAW);  
    }
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
}


/*****************************************************************************
  FUNCTION : ui_rem_deleteNet

  PURPOSE  : clear SNNS kernel
  RETURNS  : void
  NOTES    : 

  UPDATE   : 18.3.1991
******************************************************************************/

void ui_rem_deleteNet (Widget w, caddr_t client_data, caddr_t call_data)

{
    if (ui_confirmYes
	("CLEAR: The network and all patterns will be lost. Clear?")) {
	/* reset kernel */
	krui_deleteNet();
	/* reset gui */
	ui_sel_resetList();       /* reset selections */
	ui_net_completeRefresh(NULL, UI_GLOBAL); /* show net */
	ui_printMessage("");
	ui_stat_displayStatus(ui_gridPosMouse);
	ui_file_updateShellLabels();
	ui_info_makeUnitInfoPanelConsistent();	
        d3_clear_xyTranslationTable ();
	if (INVERS_CREATED) {
           XtDestroyWidget(ui_InvRootWidget);
           INVERS_CREATED = 0;
       }
    }
}


/*****************************************************************************
  FUNCTION : ui_rem_finishSteps

  PURPOSE  : finish performing update steps
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

static void ui_rem_finishSteps (void)

{
    ui_printMessage("");

    /* reshow selection markers */
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); 
}


/*****************************************************************************
  FUNCTION : ui_rem_performStep

  PURPOSE  : called by pressing the button STEPS
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

static Boolean ui_rem_performStep (caddr_t client_data)

{
    if (ui_numberOfWorkCycles-- < 1) {
	ui_rem_finishSteps();
	if (NA_ContinueTest()) 
	    ui_rem_testProc (NULL, NULL, NULL) ; 
	return(TRUE);
    }
    /* erase the units first, if they have to be drawn with their values
       in numerical form at the bottom (setup parameter) */
    if (ui_displ_isSomeWhereToShowValues())
	ui_net_updateWhole(NULL, UI_GLOBAL, 
			   UI_DRAW_UNITS, UI_ERASE_BOTTOM);
    ui_checkError(krui_updateNet(ui_updateParameters, UI_NO_UPDATE_PARAMS));
    ui_rem_incCounter();
    ui_net_updateWhole(NULL, UI_GLOBAL, UI_DRAW_UNITS, UI_DRAW);
    NA_DrawNextPoint () ;
    return(FALSE);
}


/*****************************************************************************
  FUNCTION : ui_rem_stepsProc

  PURPOSE  : perform one or more steps, depending on the PANEL_CLIENT_DATA
             if STEP is pressed one update step in the current mode
	     if STEPS is pressed: get the number of steps to perform
	     from the panel and go.
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void  ui_rem_stepsProc (Widget widget, Bool multiStepPressed, caddr_t call_data)

{ 
    char   buf[80];
    int    i;

    if (multiStepPressed) 
	/* MultiSTEPS button */
	if ((ui_numberOfWorkCycles = 
	     ui_xIntFromAsciiWidget(ui_numberOfStepsWidget)) <= 0)
	    return;
	
    sprintf(buf,"%d steps. Calculating ...", ui_numberOfWorkCycles);
    ui_printMessage(buf);
   

    for (i=0; i<UI_NO_UPDATE_PARAMS; i++)
	ui_updateParameters[i] = (float) 
	    ui_xFloatFromAsciiWidget(ui_updateParameterWidgets[i]);  

    /* erase selection markers */
    ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL); 

   /* Get new Activation Parameter for Kohonen */
   if (kohonen_open) 
     krui_kohonen_SetExtraParameter(ui_xIntFromAsciiWidget(ui_LayerWidget)); 

    if (ui_workProcId)
	XtRemoveWorkProc(ui_workProcId); /* kill old workProc */
    ui_workType = UI_STEPS;
    ui_workProcId = 
	XtAppAddWorkProc(ui_appContext, (XtWorkProc) ui_rem_performStep, NULL);

}


/*****************************************************************************
  FUNCTION : ui_rem_moveInPatternsProc             

  PURPOSE  : callback. Called when clicked on a cursor button or the GOTO 
             button.
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/
void ui_rem_moveInPatternsProc (Widget w, int moveType, caddr_t call_data)
{
    int                temp, error, i;
    char               buf[50];
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

    Bool  doMove = FALSE;

    if (krui_getNoOfPatterns() <= 0) {
	ui_confirmOk("No patterns in memory!");
	return;
    }

    /* there are patterns; get the mapping straight */
    if((error = krui_GetPatInfo(&patt_info, &descrip)) < 0){
        ui_checkError(error);
        return;
    }
    for (i=0; i<patt_info.no_of_remap_params; i++)
	ui_remapParameters[i] = (float) 
	    ui_xFloatFromAsciiWidget(ui_remapParameterWidgets[i]);
    ui_checkError(krui_setRemapFunc(patt_info.remap_function,
				    ui_remapParameters));

    /* check the class information */
    if(patt_info.class_distrib_active){
	if(class_readWidgets() == -1){
	    /* class information invalid; do not move */
	    return;
	}
    }

    switch (moveType) {
      case UI_JUMP: 
	temp = ui_xIntFromAsciiWidget(ui_numberOfPatternWidget);
	if ((temp > 0) AND (temp <= krui_getNoOfPatterns())) {
	    ui_noOfCurrentPattern = temp;
	    ui_rem_displayPatternNumber();
	    doMove = TRUE;
	} else
	    ui_confirmOk("pattern goto: invalid number!");
	break;
      case UI_FIRST:
	ui_noOfCurrentPattern = 1;
	ui_rem_displayPatternNumber();
	doMove = TRUE;
	break;
      case UI_LAST:
	ui_noOfCurrentPattern = krui_getNoOfPatterns();
	ui_rem_displayPatternNumber();
	doMove = TRUE;
	break;
      case UI_NEXT:
	if (++ui_noOfCurrentPattern > krui_getNoOfPatterns())
	    ui_noOfCurrentPattern = 1;
	ui_rem_displayPatternNumber();
	doMove = TRUE;
	break;
      case UI_PREVIOUS:
	if (--ui_noOfCurrentPattern < 1)
	    ui_noOfCurrentPattern = krui_getNoOfPatterns();
	ui_rem_displayPatternNumber();
	doMove = TRUE;
	break;
    }
    
    if (doMove) {
	/* new pattern: reset */

	krui_setPatternNo(ui_noOfCurrentPattern);
	ui_rem_getSubPatPanel();
	ui_rem_resetSubPat();
	ui_checkError(krui_DefShowSubPat(subIPatSize,subOPatSize,
					 subIPatPos,subOPatPos));
	ui_rem_resetCounter();

	ui_checkError(krui_showPattern(ui_patternLoadMode));
	ui_net_updateWhole(NULL, UI_GLOBAL, UI_DRAW_UNITS, UI_DRAW);
	if(patt_info.classes){
	    krui_GetPatInfo(&patt_info, &descrip);
	    sprintf(buf,"current class: %s",
		    patt_info.class_names[descrip.my_class]);
	    ui_xSetLabel(ui_controlMessageWidget, buf);
	}
    }
}


/*****************************************************************************
  FUNCTION : ui_rem_moveInSubPattProc             

  PURPOSE  : callback. Called when clicked on a cursor button of the supattern 
             panel.
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/
static void ui_rem_moveInSubPattProc(Widget w, int moveParam, caddr_t call_data)
{
    int dim, moveType;
    int i;
    pattern_set_info patt_info;
    pattern_descriptor descrip;
    char buf[3];
    krui_err error;
    int i_temp, o_temp;
    int maxNpos;      /* value not used */

    /* Get information about current pattern set */

    if((error = krui_GetPatInfo(&patt_info, &descrip)) < 0){
	ui_checkError(error);
	return;
    }

    /* extract parameters */

    dim = moveParam / 10;
    moveType = moveParam % 10;


    /* get subpattern information */

    for (i=0; i< subIPatDims; i++) {
	subIPatSize[i] = (int)ui_xIntFromAsciiWidget(subIPatSizeW[i]);
	subIPatStep[i] = (int)ui_xIntFromAsciiWidget(subIPatStepW[i]);
    }
    for (i=0; i< subOPatDims; i++) {
	subOPatSize[i] = (int)ui_xIntFromAsciiWidget(subOPatSizeW[i]);
	subOPatStep[i] = (int)ui_xIntFromAsciiWidget(subOPatStepW[i]);
    }


    /* compute the new subpattern positions */

    switch (moveType) {
      case SP_FIRST:
	i_temp = 1;
	o_temp = 1;
	break;
      case SP_LAST:
	i_temp = descrip.input_dim_sizes[dim] - subIPatSize[dim]+1;
	o_temp = descrip.output_dim_sizes[dim] - subOPatSize[dim]+1;
	break;
      case SP_NEXT:
	if(subIPatPos[dim]+subIPatStep[dim] <= 
	                      descrip.input_dim_sizes[dim]-subIPatSize[dim]+1)
	    i_temp = subIPatPos[dim] + subIPatStep[dim];
	else
	    i_temp = subIPatPos[dim];
	if(subOPatPos[dim]+subOPatStep[dim] <= 
	                      descrip.output_dim_sizes[dim]-subOPatSize[dim]+1)
	    o_temp = subOPatPos[dim] + subOPatStep[dim];
	else
	    o_temp = subOPatPos[dim];
	break;
      case SP_PREVIOUS:
	i_temp = subIPatPos[dim] - subIPatStep[dim];
	if(i_temp < 1)i_temp = 1;
	o_temp = subOPatPos[dim] - subOPatStep[dim];
	if(o_temp < 1)o_temp = 1;
	break;
    }

    /* perform the move */
    subIPatPos[dim] = i_temp;
    subOPatPos[dim] = o_temp;

    /* validate the desired move */
    ui_checkError(krui_DefTrainSubPat(subIPatSize,subOPatSize,		
				      subIPatStep,subOPatStep,&maxNpos));
    ui_checkError(krui_AlignSubPat(subIPatPos,subOPatPos,&subIPatNo));

    if((error = krui_DefShowSubPat(subIPatSize,subOPatSize,
				   subIPatPos,subOPatPos)) < 0){
	ui_checkError(error);
	return;
    }
    if((error = krui_showPattern(ui_patternLoadMode)) < 0){
	ui_checkError(error);
	return;
    }
    ui_net_updateWhole(NULL, UI_GLOBAL, UI_DRAW_UNITS, UI_DRAW);

    /* reset labels if move was successful */
    for (i=0; i< subIPatDims; i++) {
	sprintf(buf," %d",subIPatPos[i]);
	ui_xSetLabel(subIPatPosW[i],buf);
    }
    for (i=0; i< subOPatDims; i++){
	sprintf(buf," %d",subOPatPos[i]);
	ui_xSetLabel(subOPatPosW[i],buf);
    }
}


/*****************************************************************************
  FUNCTION : ui_rem_testProc

  PURPOSE  : load the next (or first) pattern and perform the number of steps
             specified in the control panel in "no of steps"
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/
void ui_rem_testProc (Widget w, XtPointer button, caddr_t call_data)
{

    static int npos = 1;
    int        maxNpos;
    int        err;
    char       buf[50];
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

    if (krui_getNoOfPatterns() == 0) {
	char buf[80];
	sprintf(buf,"No patterns present.");
	ui_printMessage(buf);
	return;
    }

    /* there are patterns; get the mapping straight */
    if((err = krui_GetPatInfo(&patt_info, &descrip)) < 0){
        ui_checkError(err);
        return;
    }
    if(patt_info.class_distrib_active){
	if(class_readWidgets() == -1){
	    /* class information invalid; do not test */
	    return;
	}
    }
    if (ui_noOfCurrentPattern > krui_getNoOfPatterns() - 1)
	ui_noOfCurrentPattern = 0; /* set before first pattern */

    /* new pattern: reset */
    ui_rem_resetCounter();

    ui_rem_getSubPatPanel();
    ui_checkError(err = krui_DefTrainSubPat(subIPatSize,subOPatSize,
				      subIPatStep,subOPatStep,&maxNpos));
    if (err != KRERR_NO_ERROR)
	return;
    ui_checkError(err = krui_AlignSubPat(subIPatPos,subOPatPos,&npos));
    if (err != KRERR_NO_ERROR)
	return;
    if(++npos > maxNpos){
	ui_checkError(err = krui_setPatternNo(++ui_noOfCurrentPattern));
	if (err != KRERR_NO_ERROR)
	    return;
	ui_checkError(err = krui_DefTrainSubPat(subIPatSize,subOPatSize,
					  subIPatStep,subOPatStep,&maxNpos));
	if (err != KRERR_NO_ERROR)
	    return;
	npos = 1;
	ui_rem_resetSubPat();
    }
    ui_checkError(err = krui_GetShapeOfSubPattern(subIPatSize,subOPatSize,
					    subIPatPos,subOPatPos,npos));
    if (err != KRERR_NO_ERROR)
	return;
    ui_checkError(err = krui_DefShowSubPat(subIPatSize,subOPatSize,
				     subIPatPos,subOPatPos));
    if (err != KRERR_NO_ERROR)
	return;
    ui_rem_setSubPatPanel();

    ui_checkError(err = krui_showPattern(ui_patternLoadMode));
    if (err != KRERR_NO_ERROR)
	return;
    ui_rem_displayPatternNumber();

    if (ui_xIntFromAsciiWidget(ui_numberOfStepsWidget) < 1)
	ui_net_updateWhole(NULL, UI_GLOBAL, 
			   UI_DRAW_UNITS, UI_DRAW);
    else
	ui_rem_stepsProc(NULL, TRUE, NULL); /* multistep */

    if(patt_info.classes){
       krui_GetPatInfo(&patt_info, &descrip);
       sprintf(buf,"current class: %s",patt_info.class_names[descrip.my_class]);
       ui_xSetLabel(ui_controlMessageWidget, buf);
    }
}


/*****************************************************************************
  FUNCTION : ui_rem_deleteAllPatternsProc

  PURPOSE  : delete all patterns in main memory. The contents of the current
             pattern file (if any) is left unchanged 
  RETURNS  : void
  NOTES    :

  UPDATE   : 24.8.1990
******************************************************************************/

void ui_rem_deleteAllPatternsProc (Widget w, XtPointer button, 
					caddr_t call_data)

{
    if (ui_confirmYes("Delete all patterns. Are you sure?")) {
	krui_deleteAllPatterns();
	ui_noOfCurrentPattern = 0;
	ui_rem_displayPatternNumber();
    }
}


/*****************************************************************************
  FUNCTION : ui_rem_modifyPatternProc

  PURPOSE  : calls the kernal in order to store the current visible pattern
             replacing the current pattern. 
  RETURNS  : void 
  NOTES    :

  UPDATE   : 18.3.1991
******************************************************************************/
void ui_rem_modifyPatternProc (Widget w, XtPointer button, caddr_t call_data)
{
    pattern_set_info   patt_info;
    pattern_descriptor descrip;
    int error;
    int temp = ui_xIntFromAsciiWidget(ui_numberOfPatternWidget);

    newPatternsLoaded = 1;

    if ((temp > 0) AND (temp <= krui_getNoOfPatterns())) {
	ui_noOfCurrentPattern = temp;
	krui_setPatternNo(ui_noOfCurrentPattern);
	ui_checkError(krui_modifyPattern());
	if (ui_noOfCurrentPattern > krui_getNoOfPatterns()) {
	    ui_noOfCurrentPattern = krui_getNoOfPatterns();
	    ui_rem_displayPatternNumber();
	} 
	if((error = krui_GetPatInfo(&patt_info, &descrip)) < 0){
	    ui_checkError(error);
	    return;
	}
	if(patt_info.classes > 0) {
	    ui_newClass(w, patt_info.class_names[descrip.my_class]);
	}
    } else
	ui_confirmOk("modify pattern: invalid pattern number!");
}


/*****************************************************************************
  FUNCTION : ui_rem_deletePatternProc

  PURPOSE  : calls the kernel in order to delete the current visible pattern.
  RETURNS  : void
  NOTES    :

  UPDATE   : 18.3.1991
******************************************************************************/
void ui_rem_deletePatternProc (Widget w, XtPointer button, caddr_t call_data)
{
    int temp = ui_xIntFromAsciiWidget(ui_numberOfPatternWidget);

    newPatternsLoaded = 1;

    if ((temp > 0) AND (temp <= krui_getNoOfPatterns())) {
	ui_noOfCurrentPattern = temp;
	krui_setPatternNo(ui_noOfCurrentPattern);
	ui_checkError(krui_deletePattern());
	if (ui_noOfCurrentPattern > krui_getNoOfPatterns()) {
	    ui_noOfCurrentPattern = krui_getNoOfPatterns();
	    ui_rem_displayPatternNumber();
	} 
	krui_setPatternNo(ui_noOfCurrentPattern);
	ui_classUpdatePanel(FALSE);
    } else
	ui_confirmOk("delete pattern: invalid pattern number!");
}


/*****************************************************************************
  FUNCTION : ui_rem_newPatternProc

  PURPOSE  : calls the kernel in order to store the current visible pattern
             as a new one. This new pattern will be appended.
  RETURNS  : void
  NOTES    :

  UPDATE   : 24.8.1990
******************************************************************************/
void ui_rem_newPatternProc (Widget w, caddr_t call_data)
{
    pattern_set_info   patt_info;
    pattern_descriptor descrip;
    int error,setNo;
    bool ask = FALSE;
    char *classname;

    if(NO_OF_PATTERN_SETS == 0){
	if((error = krui_allocNewPatternSet(&setNo)) < 0){
	    ui_checkError(error);
	    return;
	}
	ui_noOfCurrentPattern = 1;
	NO_OF_PATTERN_SETS = 1;
	CURR_PATTERN_SET = setNo;

	/* set the widgets labels */
	PATTERN_SET_FILE[CURR_PATTERN_SET] = (char *)malloc(8*sizeof(char));
	strcpy(PATTERN_SET_FILE[CURR_PATTERN_SET],"New Set");
	ui_rem_updatePattList();

	ask = TRUE;
	classname = NULL;
	if((error = krui_newPattern()) < 0){
	    ui_checkError(error);
	    ui_noOfCurrentPattern = 0;
	    NO_OF_PATTERN_SETS = 0;
	    CURR_PATTERN_SET = 0;
	    return;
	}

	if((error = krui_GetPatInfo(&patt_info, &descrip)) < 0){
	    ui_checkError(error);
	    return;
	}
 	    
    }else if(krui_getNoOfPatterns() > 0){
	if((error = krui_GetPatInfo(&patt_info, &descrip)) < 0){
	    ui_checkError(error);
	    return;
	}
	if(patt_info.in_number_of_dims>0 || patt_info.out_number_of_dims>0){
	    ui_confirmOk("NEW not allowed with variable sized patterns!");
	    return;
	}
	if(patt_info.classes > 0) {
	    ask = TRUE;
	    classname = 
	     (char *)malloc((strlen(patt_info.class_names[descrip.my_class])+1)*
			      sizeof(char));
	    sprintf(classname,"%s",patt_info.class_names[descrip.my_class]);
	}
	if((error = krui_newPattern()) < 0){
	    ui_checkError(error);
	    return;
	}
	if((error = krui_GetPatInfo(&patt_info, &descrip)) < 0){
	    ui_checkError(error);
	    return;
	}
    }

    newPatternsLoaded = 1;  /* for dlvq */

    if(ask && (ui_kernelError == 0))
	ui_newClass(w, classname);

    if(patt_info.classes == 0){
	ui_noOfCurrentPattern = krui_getPatternNo();
	ui_rem_displayPatternNumber();
    }
}



/*****************************************************************************
  FUNCTION : ui_rem_printLearnInfo

  PURPOSE  : print information about last learning cycle
  RETURNS  : void
  NOTES    : Kohonen requires additional information to be displayed during
             learning. Therefore a special section is included here.

  UPDATE   : 23.03.94 by Sven Doering
******************************************************************************/
static void ui_rem_printLearnInfo (float *err_val)
{
    char    buf[MAX_NAME_LENGTH];
    int     i,j;
    static float temp1, temp2;
    static int times = 1;
    int pats;

    if(strcmp("Kohonen",krui_getLearnFunc()) == 0){
	if(INIT_KOHONEN_PRINT == 1){
	    times = 1;
	    INIT_KOHONEN_PRINT = 0; /* The variable is set when pressing the
				       ALL and SINGLE Buttons in the control */
	    temp1 =(float)ui_xFloatFromAsciiWidget(ui_learnParameterWidgets[0]);
	    temp2 =(float)ui_xFloatFromAsciiWidget(ui_learnParameterWidgets[1]);
	}else{
	    times = ui_numberOfLearnCycles DIV 10;
	}
	pats = krui_getNoOfPatterns();
	for(j=1; j<= times; j++)
	    for(i=1; i<=pats; i++){
		/* this is a copy of the adaption process in the kernel for
		   display reasons */
		temp1 *= ui_learnParameters[2];
		temp2 *= ui_learnParameters[3];
	    }
	/* Adapt the displayed values of the first two learning parameters 
	   in the control panel */
	sprintf(buf,"%7.5f",temp1);
	strncpy(buf,buf,7);
	ui_xSetString(ui_learnParameterWidgets[0], buf);
	sprintf(buf,"%7.5f",temp2);
	strncpy(buf,buf,7);
	ui_xSetString(ui_learnParameterWidgets[1], buf);
    }

    if(ui_noErrorArrayElements != 0){
	sprintf(buf,"%5d:", ui_numberOfWorkCycles + 1);
	ui_tw_printMessage(buf);
	sprintf(buf,"    %10.5f",err_val[0]);
	ui_tw_printMessage(buf);
	sprintf(buf,"     %10.5f",(err_val[0]/krui_getTotalNoOfSubPatterns()));
	ui_tw_printMessage(buf);
	if (krui_getNoOfOutputUnits() > 0) {
	    sprintf(buf,"        %10.5f\n", 
		    (err_val[0]/krui_getNoOfOutputUnits()));
	    ui_tw_printMessage(buf);
	} else
	    ui_tw_printMessage("\n");
    }else{
	sprintf(buf," no error values available\n");
	ui_tw_printMessage(buf);
    }
}


/*****************************************************************************
  FUNCTION : ui_rem_finishLearning

  PURPOSE  : finish performing learning cycles
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

static void ui_rem_finishLearning (void)

{
    /* bring the patterns back into their old order before shuffling was done */
    if (ui_shuffleFlg) 
	ui_checkError(krui_shufflePatterns(FALSE));
    if (ui_shuffleSubPatFlg) 
	ui_checkError(krui_shuffleSubPatterns(FALSE));
    
    if (ui_displ_isSomeWhereToShowWeights())
	ui_net_completeRefresh(NULL, UI_GLOBAL);
    else {
          krui_cc_updatePosOfSpecialUnits();
          ui_net_completeRefresh(NULL, UI_GLOBAL);
	ui_net_updateWhole(NULL, UI_GLOBAL, 
			   UI_DRAW_UNITS, UI_DRAW);
	}
}


/*****************************************************************************
  FUNCTION : ui_rem_performLearn

  PURPOSE  : perform learning cycles
  RETURNS  :
  NOTES    :

  UPDATE   : 19.02.95
******************************************************************************/

static Boolean ui_rem_performLearn (int learnType)

{
    krui_err err;
    float learn_error;
    static Boolean first_cycle = TRUE;
    static struct ErrorValuesType learn_error_values,
                                  test_error_values;
    
    dlvq_numberOfLearnCycles = ui_numberOfLearnCycles; 

    if (ui_numberOfWorkCycles-- < 1) {
	ui_rem_finishLearning();
	return(TRUE);
    }

    switch (learnType) {
      case UI_LEARN_ALL:
	if (ui_shuffleFlg) 
	    ui_checkError(krui_shufflePatterns(TRUE));
	if (ui_shuffleSubPatFlg) 
	    ui_checkError(krui_shuffleSubPatterns(TRUE));
	if (ui_jogFlg) 
	{
	    if (ui_jogCorrFlg)
		ui_checkError(krui_jogCorrWeights(jog_low, jog_high, 
						  jog_correlation));
	    else
		krui_jogWeights(jog_low, jog_high);
	}

#ifdef PARGAGON_KERNEL
	if (ParallelMode) {
	  ui_checkError(err = krip_learnAllPatterns(ui_learnParameters, 
		         			    UI_NO_LEARN_PARAMS
						    +UI_NO_OF_CASCADE_PARAMS,
						    &ui_errorArray,
						    &ui_noErrorArrayElements, 
						    ui_numberOfLearnCycles, TRUE));
	  ui_numberOfWorkCycles = 0;
	  ui_rem_finishLearning();
	  return (TRUE);
	  break;
	} 
	else {
#endif
	ui_checkError(err = krui_learnAllPatterns(ui_learnParameters, 
						  UI_NO_LEARN_PARAMS
						  +UI_NO_OF_CASCADE_PARAMS,
						  &ui_errorArray,
						  &ui_noErrorArrayElements));
        if (err != KRERR_NO_ERROR)
            return (TRUE);
	break;
#ifdef PARGAGON_KERNEL
        }
#endif
      case UI_LEARN_SINGLE:

#ifdef PARGAGON_KERNEL
	if (ParallelMode) {
	  ui_confirmOk("Function not supported in parallel mode.");
	  return (TRUE);
	}
	else {
#endif
	if ((ui_noOfCurrentPattern <= krui_getNoOfPatterns()) AND
	    (ui_noOfCurrentPattern > 0)) {
	    ui_checkError(err = krui_learnSinglePattern(ui_noOfCurrentPattern,
						  ui_learnParameters,
							UI_NO_LEARN_PARAMS
							+UI_NO_OF_CASCADE_PARAMS,
						  &ui_errorArray,
						  &ui_noErrorArrayElements));
            if (err != KRERR_NO_ERROR)
                return (TRUE);
	} else
	    ui_confirmOk("Can't learn current pattern.");
#ifdef PARGAGON_KERNEL
        }
#endif
	break;
    }

    ui_displWeightsFromUpdate();
    if ((ui_numberOfWorkCycles == ui_numberOfLearnCycles) or
	(ui_numberOfWorkCycles == 0) or
	(ui_numberOfLearnCycles <= 10) or
	(((ui_numberOfWorkCycles+1) MOD (ui_numberOfLearnCycles DIV 10)) == 0)){
	ui_tw_printMessage("Train");
	ui_rem_printLearnInfo(ui_errorArray);
    }

    /* initialize error_values */
    if (first_cycle){
	test_error_values.SSE = 0.0;
	test_error_values.MSE = 0.0;
	test_error_values.SSE_div_Out = 0.0;
	first_cycle = FALSE;
    }

    if((ui_testFrequency > 0 ) && 
       ((ui_numberOfWorkCycles==ui_numberOfLearnCycles-1) || 
	(((ui_numberOfWorkCycles+1) MOD ui_testFrequency) == 0)) ) 
    {
	ui_rem_performValidation(&test_error_values);
	ui_testError = test_error_values.SSE;
    }	
    
    if(o_open){
	learn_error = ui_errorArray[0];
	learn_error_values.SSE = learn_error;
	learn_error_values.MSE = learn_error / 
	                            (float) krui_getTotalNoOfSubPatterns();
	learn_error_values.SSE_div_Out = learn_error / 
	                             (float) krui_getNoOfOutputUnits();
	o_draw(learn_error_values, test_error_values,ui_testFrequency);
	if(ui_numberOfWorkCycles == 0){
	    o_PressPossible = 1;
	}
    }
    return(FALSE);
}


/*****************************************************************************
  FUNCTION : ui_rem_performValidate

  PURPOSE  : perform validation of training run (only used as Callback-Func)
  RETURNS  :
  NOTES    :

  UPDATE   : 22.06.95
******************************************************************************/
void ui_rem_performValidate(caddr_t call_data)
{
  struct ErrorValuesType test_error_values;
  if(!strcmp(krui_getLearnFunc(),"PruningFeedForward"))
      /* necessary to get the pruning parameters straight */
      krui_setFFLearnFunc (krf_getCurrentNetworkFunc (FF_LEARN_FUNC | 
						      LEARN_FUNC));
  ui_rem_performValidation(&test_error_values);

  return;
}

/*****************************************************************************
  FUNCTION : ui_rem_performValidation

  PURPOSE  : perform validation of training run
  RETURNS  :
  NOTES    :

  UPDATE   : 26.06.95
******************************************************************************/
void ui_rem_performValidation(struct ErrorValuesType *test_error_values)
{
    int err;
    int maxNpos;



    /* switch to the validation set */

    if((err = krui_setCurrPatSet(CURR_TEST_PATTERN_SET)) < 0){
	ui_checkError(err);
	return /* (TRUE) */;
    }

    ui_rem_getSubPatPanel();
    ui_rem_resetSubPat();
    if (ui_shuffleFlg) 
	ui_checkError(krui_shufflePatterns(TRUE));
    if (ui_shuffleSubPatFlg) 
	ui_checkError(krui_shuffleSubPatterns(TRUE));
    ui_checkError(krui_DefTrainSubPat(subIPatSize,subOPatSize,
				      subIPatStep,subOPatStep,&maxNpos));
    ui_checkError(err = krui_testAllPatterns(ui_learnParameters, 
					     UI_NO_LEARN_PARAMS
					     +UI_NO_OF_CASCADE_PARAMS,
					     &ui_testArray,
					     &ui_noTestArrayElements));
    if (err != KRERR_NO_ERROR){
	ui_testFrequency = 0;
    }else{
	if((ui_numberOfWorkCycles == ui_numberOfLearnCycles) or
	   (ui_numberOfWorkCycles == 0) or
	   (ui_testFrequency == 0) or
	   ((((ui_numberOfLearnCycles+1)DIV ui_testFrequency)DIV 10)==0) or
	   ((((ui_numberOfWorkCycles+1) DIV ui_testFrequency) MOD 
	     (((ui_numberOfLearnCycles+1)DIV ui_testFrequency)DIV 10))==0)){
	    ui_tw_printMessage("Test ");
	    ui_rem_printLearnInfo(ui_testArray);
	}
    }

    test_error_values->SSE = ui_testArray[0];
    test_error_values->MSE = ui_testArray[0] /  
                                 (float) krui_getTotalNoOfSubPatterns();
    test_error_values->SSE_div_Out = ui_testArray[0] / 
	                         (float) krui_getNoOfOutputUnits();
	
    /* switch back to the training pattern set */
	
    if((err = krui_setCurrPatSet(CURR_PATTERN_SET)) < 0){
	ui_checkError(err);
	return /*(TRUE)*/;
    }
    ui_rem_getSubPatPanel ();
    ui_rem_resetSubPat();
    ui_checkError(krui_DefTrainSubPat(subIPatSize,subOPatSize,
				      subIPatStep,subOPatStep,&maxNpos));

    /* return to caller */
    return;
}


/*****************************************************************************
  FUNCTION : ui_rem_performPrunInit

  PURPOSE  : perform pruning: first training, initialization
  RETURNS  :
  NOTES    :

  UPDATE   : 
******************************************************************************/

static Boolean ui_rem_performPrunInit (int learnType)

{

    float net_error;

    /* create temporary file */
    tmp_file = tempnam ("./","SNNS_");

    /* display message */
    ui_tw_printMessage
	("  -------------------- first training --------------------\n");
	
    /* train network into minimum */ 
    net_error = ui_rem_performFFLearn (pr_trainCycles, learnType);
    if (net_error >= 0)
	/* no error occured during training */
    {
	/* store maximum error */
	if (net_error < pr_minError)
	    net_error = pr_minError;
	pr_maxError = net_error * (1 + pr_maxErrorInc / 100);
	if (pr_maxError < pr_acceptedError)
	    pr_maxError = pr_acceptedError;
	
	/* call pruning work procedure */
	ui_workProcId = XtAppAddWorkProc 
	    (ui_appContext, (XtWorkProc) ui_rem_performPrun,
	     (XtPointer) ((long)learnType));
    }

    /* remove this work procedure */
    return (TRUE);

}


/*****************************************************************************
  FUNCTION : ui_rem_performPrun

  PURPOSE  : perform pruning: prune link and retrain
  RETURNS  :
  NOTES    :

  UPDATE   : 
******************************************************************************/

static Boolean ui_rem_performPrun (int learnType)

{

    float net_error;
    krui_err err;
    char *dummy_str;

    if (pr_recreate)
    {
	/* display message */
	ui_tw_printMessage
	    ("  ---------------------- saving net ----------------------\n");
	
	/* save net */
	krui_saveNet (tmp_file, "Pruned Net");
    }

    /* display message */
    ui_tw_printMessage
	("  ----------------------- pruning ------------------------\n\n");
    
    /* delete some weight */
    switch (learnType)
    {
      case UI_LEARN_ALL:
	err = pr_callPrunFunc (PR_ALL_PATTERNS);
	break;
      case UI_LEARN_SINGLE:
	err = pr_callPrunFunc (ui_noOfCurrentPattern);
	break;
    }
    if (err != KRERR_NO_ERROR)
	/* remove this work procedure */
	return (TRUE);
	
    err = pr_calcMeanDeviation (PR_ALL_PATTERNS, &net_error);	     
    if (err != KRERR_NO_ERROR)
	return (TRUE);
    if (net_error > pr_minError)
    {
	/* update display after pruning */
	if(pr_refresh)
	    ui_net_completeRefresh(NULL, UI_GLOBAL); 

	/* display message */
	ui_tw_printMessage
	    ("  ---------------------- retraining ----------------------\n");
	
	/* retrain network */ 
	net_error = ui_rem_performFFLearn (pr_retrainCycles, learnType);
	if (net_error < 0)
	    /* remove this work procedure */
	    return (TRUE);
    }

    if (net_error > pr_maxError)
	/* error is beyond maximum */
    {
	if (pr_recreate)
	{
	    /* display message */
	    ui_tw_printMessage
		("  -------------------- reloading net --------------------\n");

	    /* reload last saved net */
	    krui_loadNet (tmp_file, &dummy_str);

	    /* delete temporary file */
	    unlink (tmp_file);
	}

	/* display message */
	ui_tw_printMessage
	    ("  -------------------- end of pruning --------------------\n");

	/* remove this work procedure */
	return (TRUE);
    }
    else
	/* this work procedure stays registered */
	return (FALSE);

}

/*****************************************************************************
  FUNCTION : ui_rem_errProc

  PURPOSE  : get Net Error
  NOTES    : Thanks to Warren Sarle ! A good reference is:
  .          Judge, G.G et al. _The Theory and Practice of Econometrics_,
  .          New York: John Wiley & Sons, 1980

  RETURNS  : void
  UPDATE   :
*****************************************************************************/
void ui_rem_errProc (Widget w, XtPointer button, caddr_t call_data)
{
    float sse=0,tss,rsq=0,adjrsq=0,mse=0,rmse=0,J_p=0,gcv=0;
    float pc=0,S_p=0,gmsep=0,shibata=0,aic=0,sbc=0;
    float *errorArray, learnParams[2]={0,0};
    int p,n,noErrorArrayElements, noLearnParams=2;
    char *learnFunc=krui_getLearnFunc();
    int maxNpos; /*Not used */
    krui_err err;

    /* number of observations */
    if(!(n=krui_getNoOfPatterns() )) {    
	ui_confirmOk("sample size is 0, giving up");
	return;
    }
    printf("--------------------- s t a t i s t i c s-------------------------------------\n");
    printf("Number of Patterns : %10i\n",n);

    /* number of parameters including intercept */ 
    if(!(p=krui_countLinks())) {
	ui_confirmOk("number of parameters is 0, giving up");
	return;
    }
    printf("Number of parameters (Links+bias)\t   : %10i\n",p);
    if( krui_setLearnFunc("Std_Backpropagation")!=KRERR_NO_ERROR) {
	ui_confirmOk("Can't change Learningfunktion to Std_Backpropagation, giving up");
	return;
    }
    if( (err = krui_DefTrainSubPat(subIPatSize,subOPatSize,		
				   subIPatStep,subOPatStep,&maxNpos))
	!=KRERR_NO_ERROR){
	ui_checkError(err);
	krui_setLearnFunc(learnFunc);
	return;
    }
    if( (err=krui_learnAllPatterns(learnParams,noLearnParams,&errorArray,
				   &noErrorArrayElements)) != KRERR_NO_ERROR) {
	ui_checkError(err);
	krui_setLearnFunc(learnFunc);
	return;
    }
    sse=errorArray[0];
    printf("sse\t\t   : %10.4f\n",sse);
    krui_setLearnFunc(learnFunc);

    /* total sum of squares corrected for the mean for the dependent
     * var(s) 
     */
    tss = krui_getVariance()*n;
    printf("tss\t\t   : %10.4f\n",tss);
    /* sigma^2: an estimate of the pure error variance, usually pbtained
     *  from replicated observations or by fitting the model with all
     *  predictors 
     */
    /* if(n!=1)
     * sigma = (sse - ((sigma*sigma)/n)) / (n-1);
     */
    /* rsq: the propotion of variance explained by the model */
    if(tss)
	rsq = 1 - sse/tss;
    printf("rsq\t\t   : %10.4f\n",rsq);

    /* mse: the mean square error (Darlington 1968) */
    if(n-p>0){
	mse = sse / (n-p);
	printf("mse\t\t   : %10.4f\n",mse);
    }else
	printf("mse\t\t   : sample size to small, mse not computed \n");
    /* rmse: the root mean square error */
    rmse = (float) sqrt((double)mse);
    printf("rmse\t\t   : %10.4f\n",rmse);

    /* adjrsq: rsq adjusted for degrees of freedom */
    if(n-p>0){
	adjrsq = 1-((float)(n-1)/(n-p))*(1-rsq);
	printf("adjrsq\t\t   : %10.4f\n",adjrsq);
    }else
	printf("adjrsq\t\t   : sample size to small, adjrsq not computed\n");

    /* J_p: the estimated mean square error of prediction assuming that
     *  the values of the regressors are fixed and that the model is
     *  correct 
     */
    J_p = (n+p) * mse / n;
    printf("J_p\t\t   : %10.4f\n",J_p);
    /* gcv: the generalized crossvalidation statistic */
    if(n-p>0){
	gcv = (sse * n) / ((n-p)*(n-p));
	printf("gcv\t\t   : %10.4f\n",gcv);
    }else
	printf("gcv\t\t   : sample size to small, gvc not computed\n");
  
    /* pc: Amemiya's prediction criterion */
    if(n-p>0){
	pc = 1-((float)(n+p)/(n-p))*(1-rsq);
	printf("pc\t\t   : %10.4f\n",pc);
    }else
	printf("pc\t\t   : sample size to small, pc not computed\n");

    /* C_p: Mallow's C_p statistic */
    /* if(sigma)
     * C_p = (sse/sigma) + 2*p-n;
     * else
     * printf("Note: sigma is 0, C_p not computed\n");
     */
    /* S_p: the S_p statistic (Hocking 1976) */
    if(n-p-1>0){
	S_p = mse / (n-p-1);
	printf("S_p\t\t   : %10.4f\n",S_p);
    }else
	printf("S_p\t\t   : sample size to small, S_p not computed\n");

    /* gmsep the estimated mean square error of prediction assuming that
     *  both independent and dependent vars are multivariate normal
     *  (Stein 1960) 
     */
    if(n-p-1>0){
	gmsep = mse*(n+1)*(n-1) / (n*(n-p-1));
	printf("gmsep\t\t   : %10.4f\n",gmsep);
    }else
	printf("gmsep\t\t   : sample size to small, gmsep not computed\n");
    
    /* shibata: Shibata 1960 */
    shibata = sse*(n+2*p)/n;
    printf("shibata\t\t   : %10.4f\n",shibata);

    /* aic: Akaikes information criterion (Akaike 1960) */
    aic = n* ((float)log((double)(sse/n))) + 2*p;
    printf("aic\t\t   : %10.4f\n",aic);

    /* sbc: Schwarz's Bayesian criterion (Schwarz 1978) */
    sbc = n* ((float)log((double)(sse/n))) + p*((float)log((double)n));
    printf("sbc\t\t   : %10.4f\n",sbc);

    /* bic: Sawa's Bayesian information criterion (Sawa 1978) */
    /* if(sse) {
     * q = sigma / (sse/n);
     * bic = n* ((float)log((double)(sse/n))) + 2*(p+2)*q-2*(q*q);
     * } else {
     * printf("Note: sse is 0, bic not computed\n");
     * }
     */
    printf("------------------------------------------------------------------------------\n");
    return;
}

#ifdef _UNUSED_FUNCTIONS_
/*****************************************************************************
  FUNCTION : ui_sGamma

  PURPOSE  : gamma function, note that this works only for x%2 == 0||0.5
*****************************************************************************/
static double ui_sGamma (double x)
{
  double ret=1;
  
  if(x-(int)x) {
    ret=0.88623;
  }
  while(x>2) {
    x--;
    ret*=x;
  }
  return ret;
}



/*****************************************************************************
  FUNCTION : ui_sFisherDis

  PURPOSE  : numerical intergration of the Fisher density function
*****************************************************************************/
static double ui_sFisherDis(double m, double n, double x, double accuracy)
{
    double f;
    double result=0,foo=1./accuracy;
    double sepp=x*accuracy*ui_sGamma(m/2.)*ui_sGamma(n/2.)/\
	(ui_sGamma((m+n)/2.)*pow(m,m/2.)*pow(n,n/2.));

    for(f=0;result<sepp;f+=foo) 
	result+=pow(f,m/2.-1)/pow(n+m*f,(m+n)/2.);

    return f;
}
#endif

/*****************************************************************************
  FUNCTION : ui_rem_performFFLearn

  PURPOSE  : perform feed forward learning
  RETURNS  : net error
  NOTES    :

  UPDATE   : 19.02.95
******************************************************************************/

static float ui_rem_performFFLearn (int cycles, int learnType)

{

    krui_err err;
    struct ErrorValuesType learn_error_values,
                           test_error_values;

    switch (learnType) {
      case UI_LEARN_ALL:
	for (ui_numberOfWorkCycles = cycles - 1;
	     ui_numberOfWorkCycles >= 0; ui_numberOfWorkCycles--)
	{
	    /* shuffle patterns */
	    if (ui_shuffleFlg) 
		ui_checkError (krui_shufflePatterns (TRUE));
	    if (ui_shuffleSubPatFlg) 
		ui_checkError (krui_shuffleSubPatterns (TRUE));

	    /* perform one learn cycle */
	    ui_checkError (err = krui_learnAllPatternsFF
			   (ui_learnParameters, UI_NO_LEARN_PARAMS,
			    &ui_errorArray, &ui_noErrorArrayElements));
	    if (err != KRERR_NO_ERROR)
		return (err);

	    /* print information periodically */
	    if ((ui_numberOfWorkCycles == cycles) ||
		(ui_numberOfWorkCycles == 0) ||(cycles <= 10) || 
		(((ui_numberOfWorkCycles+1) MOD (cycles DIV 10)) == 0))
	    {
		ui_tw_printMessage("     "); /* This dummy is necessary for 
						alignment reasons */
		ui_rem_printLearnInfo(ui_errorArray);
		ui_displWeightsFromUpdate();
	    }

	    /* print error graph */
	    if (o_open){
		learn_error_values.SSE = ui_errorArray[0];
		learn_error_values.MSE = ui_errorArray[0] / 
		                        (float) krui_getTotalNoOfSubPatterns();
		learn_error_values.SSE_div_Out = ui_errorArray[0] / 
		                             (float) krui_getNoOfOutputUnits();
		test_error_values.SSE = 0.0;
		test_error_values.MSE = 0.0;
		test_error_values.SSE_div_Out = 0.0;

		o_draw (learn_error_values, test_error_values,0);
	    }
	    /* stop learning if error is smaller than minimum */
	    if (ui_errorArray [0] <= pr_minError)
		break;
	}
	o_PressPossible = 1;
/*
	ui_checkError (err = krui_testAllPatterns
		       (ui_learnParameters, UI_NO_LEARN_PARAMS,
			&ui_errorArray, &ui_noErrorArrayElements));
	if (err != KRERR_NO_ERROR)
	    return (err);
*/
	break;
      case UI_LEARN_SINGLE:
	for (ui_numberOfWorkCycles = cycles - 1; 
	     ui_numberOfWorkCycles >= 0;
	     ui_numberOfWorkCycles--)
	{
	    /* perform one learn cycle */
	    ui_checkError (err = krui_learnSinglePatternFF
			   (ui_noOfCurrentPattern, ui_learnParameters,
			    UI_NO_LEARN_PARAMS, &ui_errorArray,
			    &ui_noErrorArrayElements));
	    if (err != KRERR_NO_ERROR)
		return (err);

	    /* print information periodically */
	    if ((ui_numberOfWorkCycles == cycles) ||
		(ui_numberOfWorkCycles == 0) ||(cycles <= 10) ||
		(((ui_numberOfWorkCycles+1) MOD (cycles DIV 10)) == 0))
	    {
		ui_rem_printLearnInfo(ui_errorArray);
		ui_displWeightsFromUpdate();
	    }

	    /* print error graph */
	    if (o_open){
		learn_error_values.SSE = ui_errorArray[0];
		learn_error_values.MSE = ui_errorArray[0] / 
		                        (float) krui_getTotalNoOfSubPatterns();
		learn_error_values.SSE_div_Out = ui_errorArray[0] / 
		                             (float) krui_getNoOfOutputUnits();
		test_error_values.SSE = 0.0;
		test_error_values.MSE = 0.0;
		test_error_values.SSE_div_Out = 0.0;

		o_draw (learn_error_values, test_error_values,0);
	    }

	    /* stop learning if error is smaller than minimum */
	    if (ui_errorArray [0] <= pr_minError)
		break;
	}
	o_PressPossible = 1;
        ui_checkError (err = krui_testSinglePattern
		       (ui_noOfCurrentPattern, ui_learnParameters,
			UI_NO_LEARN_PARAMS, &ui_errorArray,
			&ui_noErrorArrayElements));
	if (err != KRERR_NO_ERROR)
	    return (err); 
	break;
    }

    return (ui_errorArray [0]);

}


/*****************************************************************************
  FUNCTION : ui_rem_learnProc

  PURPOSE  : call the learn procedure of the kernel. The number of learn cycles
             is determined by the user and also the learn parameter.
  RETURNS  : implicite alteration of the network
  NOTES    : 

  UPDATE   : 19.02.95
*****************************************************************************/

void ui_rem_learnProc (Widget w, int learnType, caddr_t call_data)

{
    char  buf[MAX_NAME_LENGTH];
    int   i, maxNpos, error;
    pattern_set_info patt_info;
    pattern_descriptor descrip;

    o_PressPossible = 0;

    cc_readElements();
    cc_allButtonIsPressed = 1;
    pr_readElements();

    /* check now for errors and react ! */
    if (krui_getNoOfPatterns() == 0) {
	ui_confirmOk("No patterns loaded/created!");
	return;
    }

    ui_rem_getSubPatPanel ();
    ui_rem_resetSubPat();
    ui_checkError(krui_DefTrainSubPat(subIPatSize,subOPatSize,
				      subIPatStep,subOPatStep,&maxNpos));
    
    ui_testFrequency =      
	ui_xIntFromAsciiWidget(ui_testFrequencyWidget);
    ui_testFrequency = (ui_testFrequency < 1) ? (0) : ui_testFrequency;
    ui_testError = 0;     

    ui_numberOfWorkCycles = /* this value will be decreased */
	ui_xIntFromAsciiWidget(ui_numberOfCyclesWidget);
    ui_numberOfLearnCycles = /* this value stays constant */
	ui_numberOfWorkCycles;

    /* get current learning parameter */
    for (i=0; i<UI_NO_LEARN_PARAMS; i++)
	ui_learnParameters[i] = (float) 
	    ui_xFloatFromAsciiWidget(ui_learnParameterWidgets[i]);
    if(strcmp(krui_getLearnFunc(),"Dynamic_LVQ") == 0)
	ui_learnParameters[3] = 
	    (int)ui_xIntFromAsciiWidget(ui_numberOfCyclesWidget);

    /* get current class information */
    if((error = krui_GetPatInfo(&patt_info, &descrip)) < 0){
        ui_checkError(error);
        return;
    }
    if(patt_info.class_distrib_active){
	if(class_readWidgets() == -1){
	    /* class information invalid; do not train */
	    return;
	}
    }

    /* remap output values if necessary */
    for (i=0; i<patt_info.no_of_remap_params; i++)
	ui_remapParameters[i] = (float) 
	    ui_xFloatFromAsciiWidget(ui_remapParameterWidgets[i]);
    ui_checkError(krui_setRemapFunc(patt_info.remap_function,
				    ui_remapParameters));

    /* prepare print protocol */
    if (learnType == UI_LEARN_ALL)
	ui_tw_printMessage("\nLearning all patterns:\n");
    else
	ui_tw_printMessage("\nLearning current pattern:\n");

    if (krui_checkPruning ())
	sprintf (buf,"  epochs   : %d + %d for each retraining\n",
		 pr_trainCycles, pr_retrainCycles);
    else
	sprintf(buf,"  epochs   : %d\n", ui_numberOfWorkCycles);

    ui_tw_printMessage(buf);

    sprintf (buf, 
       "  parameter: %8.5f\n  #o-units : %d\n  #patterns: %d (total: %d)\n",
	     ui_learnParameters [0],krui_getNoOfOutputUnits(),
	     krui_getNoOfPatterns(),krui_getTotalNoOfSubPatterns());

    ui_tw_printMessage(buf);

    sprintf(buf,
	    "\n     epoch:         SSE            MSE            SSE/o-units\n\n");
    ui_tw_printMessage(buf);
    INIT_KOHONEN_PRINT = 1; /*Enables the print kommands for kohonen */
    
    NA_StopTest () ;
    if (ui_workProcId)
	XtRemoveWorkProc(ui_workProcId); /* kill old workProc */
    ui_workType = UI_LEARN;
    if (krui_checkPruning ())
	ui_workProcId = XtAppAddWorkProc(ui_appContext, 
					 (XtWorkProc) ui_rem_performPrunInit,
					 (XtPointer) ((long)learnType));
    else
	ui_workProcId = XtAppAddWorkProc(ui_appContext, 
					 (XtWorkProc) ui_rem_performLearn, 
					 (XtPointer) ((long)learnType));

}


/*****************************************************************************
  FUNCTION : ui_rem_stopProc

  PURPOSE  : callback. Called after click on STOP button
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

void  ui_rem_stopProc (Widget widget, caddr_t client_data, caddr_t call_data)

{ 
    if (ui_workProcId) { 
	switch (ui_workType) { 
	  case UI_STEPS: 
		ui_rem_finishSteps(); break;
	  case UI_LEARN:
	    ui_rem_finishLearning();
	    ui_tw_printMessage("Train");
	    ui_rem_printLearnInfo(ui_errorArray);
	    break;
	}
	NA_StopTest () ;
	XtRemoveWorkProc(ui_workProcId); /* kill workProc */
	ui_workProcId = 0;
	ui_numberOfWorkCycles = 0;
    }
    o_PressPossible = 1;
}


/*****************************************************************************
  FUNCTION : ui_rem_shuffleProc

  PURPOSE  : notify procedure of the toggle item, to remember the status of
             it.
  RETURNS  : changes the flag ui_shuffleFlg
  NOTES    :

  UPDATE   :
*****************************************************************************/
void ui_rem_shuffleProc (Widget w, caddr_t client, caddr_t call)
{
    ui_shuffleFlg = ui_xGetToggleState(w);
}


#ifdef PARAGON_KERNEL
/*****************************************************************************
  FUNCTION : ui_rem_SwitchParallel

  PURPOSE  : Switch from sequential to parallel mode and vice versa
            
  RETURNS  : 
  NOTES    :

  UPDATE   : 23.06.94
*****************************************************************************/

void ui_rem_SwitchParallel(Widget w, caddr_t client, caddr_t call)

{
  if (ui_xGetToggleState(w))
    krip_setParallelMode(TRUE);
  else
    krip_setParallelMode(FALSE);
}
#endif


/*****************************************************************************
  FUNCTION : ui_rem_showModeProc

  PURPOSE  : notify procedure of the panel cycle.
  RETURNS  : sets the variable ui_patternLoadMode
  NOTES    :

  UPDATE   :
*****************************************************************************/
void ui_rem_showModeProc (Widget w, int value, caddr_t call_data)
{    
    ui_patternLoadMode = (int) value;
    switch (value) {
      case OUTPUT_NOTHING:
	ui_xSetLabel(ui_showModeLabel, "none");
	break;
      case OUTPUT_OUT:
	ui_xSetLabel(ui_showModeLabel, "out");
	break;
      case OUTPUT_ACT:
	ui_xSetLabel(ui_showModeLabel, "act");
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_rem_setRandomWeights

  PURPOSE  : set weights of all links randomly
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/
void ui_rem_setRandomWeights (Widget w, int randomType, caddr_t call_data)
{    
    FlintType  low, high, corr, temp;

    low  = (FlintType) ui_xFloatFromAsciiWidget(ui_lowLimitWidget);
    high = (FlintType) ui_xFloatFromAsciiWidget(ui_highLimitWidget);
    corr = (FlintType) ui_xFloatFromAsciiWidget(ui_correlationLimitWidget);
    if (low > high) {
	temp = low;
	low  = high;
	high = temp;
    }
    corr = fabs(corr);

    switch (randomType) {
      case UI_RANDOM:
	/* krui_randomizeWeights(low, high); */	   
	break;
      case UI_JOG:
	if (ui_jogCorrFlg)
	    ui_checkError(krui_jogCorrWeights(low, high, corr));
	else
	    krui_jogWeights(low, high);
	break;
    }
    if (ui_displ_isSomeWhereToShowWeights())
	ui_net_completeRefresh(NULL, UI_GLOBAL);
}


/*****************************************************************************
  FUNCTION : ui_rem_getKernelInfo

  PURPOSE  : get some info from the kernel. Output to text window
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/
void ui_rem_getKernelInfo (Widget w, XtPointer button, caddr_t call_data)
{
    char   buf[127];
    int    no_of_sites, no_of_links, no_of_STable_entries, no_of_FTable_entries;
    int    unit_bytes, site_bytes, link_bytes, 
           NTable_bytes, STable_bytes, FTable_bytes, numOfPat,
	   ui_totNumOfPat;
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

    if(krui_GetPatInfo(&patt_info, &descrip) != KRERR_NO_ERROR){
	patt_info.class_distrib_active = FALSE;
	patt_info.remap_function = NULL;
    }

    /* check the class information */
    if(patt_info.class_distrib_active){
	if(class_readWidgets() == -1){
	    /* class information invalid; do not move */
	    ui_confirmOk("Class distribution invalid!");
	    return;
	}
    }

    if((numOfPat = krui_getNoOfPatterns()) < 0){
	numOfPat = 0;
	ui_totNumOfPat = 0;
    }else{
	ui_rem_getSubPatPanel ();
	ui_rem_resetSubPat();
	ui_totNumOfPat = krui_getTotalNoOfSubPatterns();
    }

    sprintf(buf,"\n%s :\n\n", krui_getVersion());
    ui_tw_printMessage(buf);

    krui_getNetInfo(&no_of_sites, &no_of_links,
		    &no_of_STable_entries, &no_of_FTable_entries);

    sprintf(buf,"#input  units: %6d\n#output units: %6d\n",
	    krui_getNoOfInputUnits(), krui_getNoOfOutputUnits());
    ui_tw_printMessage(buf);

    sprintf(buf,"#patterns    : %6d\n",numOfPat);
    ui_tw_printMessage(buf);
    sprintf(buf,"#subpatterns : %6d\n",ui_totNumOfPat);
    ui_tw_printMessage(buf);

    sprintf(buf,"#sites       : %6d\n#links       : %6d\n",
	    no_of_sites, no_of_links);
    ui_tw_printMessage(buf);
    sprintf(buf,"#STable entr.: %6d\n#FTable-Entr.: %6d\n\n",
	    no_of_STable_entries, no_of_FTable_entries);
    ui_tw_printMessage(buf);

    ui_tw_printMessage("sizes in bytes:\n");
    krui_getMemoryManagerInfo(&unit_bytes, &site_bytes, &link_bytes,
			      &NTable_bytes, &STable_bytes, &FTable_bytes);
    sprintf(buf,"units        : %7d\nsites        : %7d\nlinks        : %7d\n",
	    unit_bytes, site_bytes, link_bytes);
    ui_tw_printMessage(buf);
    sprintf(buf,"NTable       : %7d\nSTable       : %7d\nFTable       : %7d\n\n"
	    ,NTable_bytes, STable_bytes, FTable_bytes);
    ui_tw_printMessage(buf);
    sprintf(buf,"learning function  : %s\n", krui_getLearnFunc());
    ui_tw_printMessage(buf);
    sprintf(buf,"update function    : %s\n", krui_getUpdateFunc());
    ui_tw_printMessage(buf);
    sprintf(buf,"init function      : %s\n", krui_getInitialisationFunc());
    ui_tw_printMessage(buf);
    if(patt_info.remap_function == NULL)
	sprintf(buf,"remap function     : None\n");
    else
	sprintf(buf,"remap function     : %s\n", patt_info.remap_function);

    ui_tw_printMessage(buf);
    if(ui_filenameNET[0] != '\0'){
	sprintf(buf,"network file       : %s.net\n",ui_filenameNET);
	ui_tw_printMessage(buf);
    }else{
	sprintf(buf,"No Network loaded\n");
	ui_tw_printMessage(buf);
    }
    if(numOfPat > 0){
	sprintf(buf,"learn pattern file : %s.pat\n", 
		PATTERN_SET_FILE[CURR_PATTERN_SET]);
	ui_tw_printMessage(buf);
	sprintf(buf,"test  pattern file : %s.pat\n", 
		PATTERN_SET_FILE[CURR_TEST_PATTERN_SET]);
	ui_tw_printMessage(buf);
    }else{
	sprintf(buf,"No Patterns loaded\n");
	ui_tw_printMessage(buf);
    }

    ui_tw_printMessage("\n\n");
}


/*****************************************************************************
  FUNCTION : ui_rem_shuffleSubPatProc

  PURPOSE  : notify procedure of the toggle item, to remember the status of
             it.
  RETURNS  : changes the flag ui_shuffleSubPatFlg
  NOTES    :

  UPDATE   :
*****************************************************************************/
void ui_rem_shuffleSubPatProc (Widget w, caddr_t client, caddr_t call)
{
    ui_shuffleSubPatFlg = ui_xGetToggleState(w);
}


/*****************************************************************************
  FUNCTION : ui_rem_defSubPat

  PURPOSE  : creates the panel to handle sub patterns 
  RETURNS  : void
  NOTES    :

  UPDATE   : 16.09.1993
******************************************************************************/
void ui_rem_defSubPat (Widget button, int refreshType, caddr_t call_data)
{
    Widget             sbutton,lab1,ui_pop,above,
                       isize,istep,itot,otot,osize,ostep;
    static Widget      form,title1,title2,left,subPatDoneW,
                       input,output,ipos,opos;
    Arg                args[5];
    Position           x, y;
    Dimension          width, height;
    Cardinal           n;
    char               buf[120];
    int                i;
    krui_err           error;
    pattern_set_info   patt_info;
    pattern_descriptor descrip;
    Window             rdummy;
    int                x1, y1, border;
    unsigned int       wdummy, hdummy, bdummy, ddummy;


    /* Get information about current pattern set */

    if(NO_OF_PATTERN_SETS == 0){
	if(refreshType == 0){
	    ui_confirmOk("No Patterns present !");
	    return;
	}else{
	    /* call came from ui_rem_delPatSet which deleted last pattern */
	    /* set. Therefore destroy panel                               */
	    XtCallCallbacks(subPatDoneW,XtNcallback,
			    (caddr_t) UI_POPUP_SUBPATTERN);
	    return;
	}
    }


    if((refreshType == 1) && (SubPatPanel < 1))
	/* can't refresh, if panel was never created yet */
	return;

    if((error = krui_GetPatInfo(&patt_info, &descrip)) < 0){
	ui_checkError(error);
	return;
    }

    if(patt_info.in_number_of_dims == 0){
	if(SubPatPanel < 1){
	    ui_confirmOk("Pattern has no variable dimension !");
	    return;
	}else{
	    /* New pattern set does not allow this panel */
	    XtCallCallbacks(subPatDoneW,XtNcallback,
			    (caddr_t) UI_POPUP_SUBPATTERN);
	    return;
	}

    }

    if(refreshType == 0){ /*panel has to be created */

	if(SubPatPanel == 1)
	    /* panel was supposed to be created but exists already */
	    return;

	/* Upper left corner will be in the center of the calling button */

	ui_xGetDimensions(button, &width, &height);
	XtTranslateCoords(button, (Position) (width / 2), 
			  (Position) (height / 2),&x, &y);
    
	n = 0;
	XtSetArg(args[n], XtNx, x); n++;
	XtSetArg(args[n], XtNy, y); n++;

	/* Now create Popup */
    
	ui_popPattern =
	    XtCreatePopupShell("Sub Pattern Handling", 
				topLevelShellWidgetClass,ui_toplevel,args,n);
    }else{

	/* Recreate the panel */

	n = 0;
	XtSetArg(args[n], XtNx, &x); n++;
	XtSetArg(args[n], XtNy, &y); n++;
	XtSetArg(args[n], XtNborderWidth, &border); n++;
	XtGetValues(ui_popPattern, args, n);
	XGetGeometry(ui_display, (*ui_popPattern).core.window, &rdummy, 
		     &x1, &y1, &wdummy, &hdummy, &bdummy, &ddummy);
	XtDestroyWidget(ui_popPattern);

	n = 0;
	XtSetArg(args[n], XtNx, x-x1-bdummy-1); n++;
	XtSetArg(args[n], XtNy, y-y1-bdummy-1); n++;
    	ui_popPattern =
	    XtCreatePopupShell("Sub Pattern Handling", 
				transientShellWidgetClass,ui_toplevel,args,n);
    }
    ui_pop = 
	XtCreateManagedWidget("frame",formWidgetClass,ui_popPattern,
			      NULL,0);
    
    XtSetArg(args[n], XtNborderWidth, 1); 
    form = XtCreateManagedWidget("form", formWidgetClass, ui_pop, args, 1);
    
    sprintf(buf,"                               Input");
    title1 = ui_xCreateLabelItem(buf,form,41*ui_labelFontWidth,NULL,NULL);
    sprintf(buf,"       Output");
    title2 = ui_xCreateLabelItem(buf,form,14*ui_labelFontWidth,title1,NULL);
    
    n = 0;
    XtSetArg(args[n], XtNborderWidth,  0); n++;
    XtSetArg(args[n], XtNfromHoriz, NULL); n++;
    XtSetArg(args[n], XtNfromVert, title1); n++;
    left = XtCreateManagedWidget("left", formWidgetClass, form, args, n);
    sprintf(buf,"     ");
    title1 = ui_xCreateLabelItem(buf,left,9*ui_labelFontWidth,NULL,NULL);

    sbutton = ui_xCreateToggleItem("shuffle", left, NULL, title1, NULL);
    XtAddCallback(sbutton, XtNcallback, 
		  (XtCallbackProc) ui_rem_shuffleSubPatProc, NULL);

    subIPatDims = patt_info.in_number_of_dims;
    subOPatDims = patt_info.out_number_of_dims;

    above = sbutton;

    for (i=0; i< subIPatDims; i++) {
        sprintf(buf,"dim %d :",i+1);
	lab1 = ui_xCreateLabelItem(buf, left, 7*ui_labelFontWidth, NULL, above);

	sbutton = ui_xCreateButtonItem("first",left,lab1,above);
	XtAddCallback(sbutton, XtNcallback, 
		      (XtCallbackProc)ui_rem_moveInSubPattProc,
		      (XtPointer) ((long)(SP_FIRST + i*10)));
	
	sbutton = ui_xCreateButtonItem("prev",left, sbutton, above);
	XtAddCallback(sbutton, XtNcallback, 
		      (XtCallbackProc)ui_rem_moveInSubPattProc,
		      (XtPointer) ((long)(SP_PREVIOUS + i*10)));

	sbutton = ui_xCreateButtonItem("next",left, sbutton, above);
	XtAddCallback(sbutton, XtNcallback, 
		      (XtCallbackProc) ui_rem_moveInSubPattProc,
		      (XtPointer) ((long)(SP_NEXT + i*10)));

	sbutton = ui_xCreateButtonItem("last",left, sbutton, above);
	XtAddCallback(sbutton, XtNcallback,
		      (XtCallbackProc) ui_rem_moveInSubPattProc,
		      (XtPointer) ((long)(SP_LAST + i*10)));

	above = lab1;
    }

    n = 0;
    XtSetArg(args[n], XtNborderWidth,  1); n++;
    XtSetArg(args[n], XtNfromHoriz, left); n++;
    XtSetArg(args[n], XtNfromVert, title1); n++;
    input  = XtCreateManagedWidget("input", formWidgetClass, form, args, n);
    ipos  = ui_xCreateLabelItem("Pos ",input,4*ui_labelFontWidth,NULL,NULL);
    itot  = ui_xCreateLabelItem("Tot ",input,4*ui_labelFontWidth,ipos,NULL);
    isize = ui_xCreateLabelItem("Size",input,4*ui_labelFontWidth,itot,NULL);
    istep = ui_xCreateLabelItem("Step",input,4*ui_labelFontWidth,isize,NULL);

    above = ipos;

    for (i=0; i< subIPatDims; i++) {
        sprintf(buf," %d",subIPatPos[i]);
	subIPatPosW[i] = ui_xCreateLabelItem(buf,input,4*ui_labelFontWidth,
					     NULL,above);

        sprintf(buf," %d",descrip.input_dim_sizes[i]);
	in_dim_widgets[i] = ui_xCreateLabelItem(buf,input,5*ui_labelFontWidth,
						   subIPatPosW[i],above);
        sprintf(buf,"%d",subIPatSize[i]);
        subIPatSizeW[i] = ui_xCreateDialogItem("param",input,buf,
					       4*ui_labelFontWidth,
					       in_dim_widgets[i],above);

        sprintf(buf,"%d",subIPatStep[i]);
	subIPatStepW[i] = ui_xCreateDialogItem("param",input,buf,
					       4*ui_labelFontWidth, 
					       subIPatSizeW[i],above);

	above = subIPatPosW[i];

    }

    n = 0;
    XtSetArg(args[n], XtNborderWidth,  1); n++;
    XtSetArg(args[n], XtNfromHoriz, input); n++;
    XtSetArg(args[n], XtNfromVert, title2); n++;
    output = XtCreateManagedWidget("output",formWidgetClass,form,args,n);
    opos  = ui_xCreateLabelItem("Pos ",output,4*ui_labelFontWidth,NULL,NULL);
    otot  = ui_xCreateLabelItem("Tot ",output,4*ui_labelFontWidth,opos,NULL);
    osize = ui_xCreateLabelItem("Size",output,4*ui_labelFontWidth,otot,NULL);
    ostep = ui_xCreateLabelItem("Step",output,4*ui_labelFontWidth,osize,NULL);
    
    above = opos;

    for (i=0; i<subOPatDims; i++) {
        sprintf(buf," %d",subOPatPos[i]);
	subOPatPosW[i]  = ui_xCreateLabelItem(buf,output,4*ui_labelFontWidth,
					      NULL,above);

        sprintf(buf," %d",descrip.output_dim_sizes[i]);
        out_dim_widgets[i] = ui_xCreateLabelItem(buf,output,5*ui_labelFontWidth,
						    subOPatPosW[i],above);
        sprintf(buf,"%d",subOPatSize[i]);
        subOPatSizeW[i] = ui_xCreateDialogItem("param",output,buf,
					       4*ui_labelFontWidth,
					       out_dim_widgets[i],above);

        sprintf(buf,"%d",subOPatStep[i]);
        subOPatStepW[i] = ui_xCreateDialogItem("param",output,buf,
					       4*ui_labelFontWidth, 
					       subOPatSizeW[i],above);

	above = subOPatStepW[i];

    }

    
    subPatDoneW = ui_xCreateButtonItem("done", ui_pop, NULL, form);
    XtAddCallback(subPatDoneW, XtNcallback, (XtCallbackProc) ui_popupDone, 
		  (caddr_t) UI_POPUP_SUBPATTERN);
                  /*!*/  /* constant name is meaningless ! */

    XtAddEventHandler(form,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    ui_checkWindowPosition(ui_popPattern);
    XtPopup(ui_popPattern, XtGrabNone);
    ui_xDontResizeWidget(ui_popPattern); 
    SubPatPanel = 1;
}




/*****************************************************************************
  FUNCTION : ui_rem_usePattSet

  PURPOSE  : Switches between different pattern sets
  RETURNS  : void
  NOTES    :

  UPDATE   : 13.09.1991
******************************************************************************/
void ui_rem_usePattSet (Widget button, int setNo, caddr_t call_data)
{
    int error;


    if(CURR_PATTERN_SET == setNo)
	return;

    if((error = krui_setCurrPatSet(setNo)) < 0){
	ui_checkError(error);
	return;
    }

    CURR_PATTERN_SET = setNo;
    ui_noOfCurrentPattern = 1;
    ui_xSetLabel(ui_showPattFile, PATTERN_SET_FILE[CURR_PATTERN_SET]);
    ui_classUpdatePanel(TRUE);


    /* refresh the subpattern panel */
    ui_rem_defSubPat(button,1,(caddr_t)1);

    /* refresh the pattern panel */
    ui_rem_displayPatternNumber();
}

/*****************************************************************************
  FUNCTION : ui_rem_useTestPattSet

  PURPOSE  : Switches between different pattern sets
  RETURNS  : void
  NOTES    :

  UPDATE   : 13.09.1991
******************************************************************************/
void ui_rem_useTestPattSet (Widget button, int setNo, caddr_t call_data)
{
    int error;

    if(CURR_TEST_PATTERN_SET == setNo)
	return;

    if((error = krui_setCurrPatSet(setNo)) < 0){
	ui_checkError(error);
	return;
    }
    if((error = krui_setCurrPatSet(CURR_PATTERN_SET)) < 0){
	ui_checkError(error);
	return;
    }

    CURR_TEST_PATTERN_SET = setNo;
    ui_noOfCurrentPattern = 1;
    ui_xSetLabel(ui_showTestPattFile, PATTERN_SET_FILE[CURR_TEST_PATTERN_SET]);


    /* refresh the subpattern panel */
    ui_rem_defSubPat(button,1,(caddr_t)1);

    /* refresh the pattern panel */
    ui_rem_displayPatternNumber();
}




/*****************************************************************************
  FUNCTION : ui_rem_delPattSet

  PURPOSE  : deletes a pattern set
  RETURNS  : void
  NOTES    : DELPattFileWidget[0] and USEPattFileWidget[0] ALWAYS have 
             callbacks defined, even when the label shows "No Files" !
	     This function has to take care that nothing happens in case no 
	     files are present

  UPDATE   : 19.02.95
******************************************************************************/
void ui_rem_delPattSet (Widget button, int setNo, caddr_t call_data)
{
    Arg arg[10];
    char buf[80];
    int  i;
    Widget tempWidget;


    if(NO_OF_PATTERN_SETS == 0)
	return;

    /* check whether the pattern to be deleted is the current one */

    NO_OF_PATTERN_SETS--;
    if(CURR_PATTERN_SET == setNo){
	if(CURR_PATTERN_SET == NO_OF_PATTERN_SETS)
	    CURR_PATTERN_SET = 0;          /* make the first one current */
	else
	    CURR_PATTERN_SET = NO_OF_PATTERN_SETS;
	                                   /* make the last one current */
	if(NO_OF_PATTERN_SETS == 0){       /* it was the only one        */
	    sprintf(buf,"Training Pattern File ?");
	    ui_xSetLabel(ui_showPattFile, buf);
	    PATTERN_SET_FILE[0] = NULL;
	    XtRemoveAllCallbacks(USEPattFileWidget[0],XtNcallback);
	    XtRemoveAllCallbacks(DELPattFileWidget[0],XtNcallback);
	    tempWidget = XtParent(USEPattFileWidget[0]);
	    XtDestroyWidget(USEPattFileWidget[0]);
	    USEPattFileWidget[0] = 
		XtCreateManagedWidget("No Files", smeBSBObjectClass, 
				      tempWidget, NULL, 0);
	    XtAddCallback(USEPattFileWidget[0], XtNcallback, 
			  (XtCallbackProc)ui_rem_usePattSet,(caddr_t)0);
	    tempWidget = XtParent(DELPattFileWidget[0]);
	    XtDestroyWidget(DELPattFileWidget[0]);
	    DELPattFileWidget[0] = 
		XtCreateManagedWidget("No Files", smeBSBObjectClass, 
				      tempWidget, NULL, 0);
	    XtAddCallback(DELPattFileWidget[0], XtNcallback, 
			  (XtCallbackProc)ui_rem_delPattSet,(caddr_t)0);
	    if(SubPatPanel == 1)
		ui_popupDone(tempWidget, UI_POPUP_SUBPATTERN , (caddr_t) 0);

	}else{
	    sprintf(buf,"%s",PATTERN_SET_FILE[CURR_PATTERN_SET]);
	}
	ui_xSetLabel(ui_showPattFile, buf);
	ui_classUpdatePanel(TRUE);
    }

    if(CURR_TEST_PATTERN_SET == setNo){
	if(CURR_TEST_PATTERN_SET == NO_OF_PATTERN_SETS)
	    CURR_TEST_PATTERN_SET = 0;          /* make the first one current */
	else
	    CURR_TEST_PATTERN_SET = NO_OF_PATTERN_SETS;
	                                   /* make the last one current */
	if(NO_OF_PATTERN_SETS == 0){       /* it was the only one        */
	    sprintf(buf,"Validation Pattern File ?");
	    ui_xSetLabel(ui_showTestPattFile, buf);
	    PATTERN_SET_FILE[0] = NULL; 
	    XtRemoveAllCallbacks(USETestPattFileWidget[0],XtNcallback);
	    tempWidget = XtParent(USETestPattFileWidget[0]);
	    XtDestroyWidget(USETestPattFileWidget[0]);
	    USETestPattFileWidget[0] = 
		XtCreateManagedWidget("No Files", smeBSBObjectClass, 
				      tempWidget, NULL, 0);
	    XtAddCallback(USETestPattFileWidget[0], XtNcallback, 
			  (XtCallbackProc)ui_rem_useTestPattSet,(caddr_t)0);

	}else{
	    sprintf(buf,"%s",PATTERN_SET_FILE[CURR_TEST_PATTERN_SET]);
	}
	ui_xSetLabel(ui_showTestPattFile, buf);
    }



    /* delete the pattern set now */

    ui_checkError(krui_deletePatSet(setNo));

    if(NO_OF_PATTERN_SETS == 0)	return;

    /* kernel needs resetting of current pattern */
    if(CURR_PATTERN_SET < setNo)
	krui_setCurrPatSet(CURR_PATTERN_SET);
    else
	krui_setCurrPatSet(--CURR_PATTERN_SET);


    /* refresh the subpattern panel */
	
    ui_rem_defSubPat(button,1,(caddr_t)1);


    /* Compact the array of pattern file names and relabel the menus*/

    for(i=setNo; i<NO_OF_PATTERN_SETS; i++){
	strcpy(PATTERN_SET_FILE[i],PATTERN_SET_FILE[i+1]);
	XtSetArg(arg[0],XtNlabel,PATTERN_SET_FILE[i]);
	XtSetValues(USEPattFileWidget[i],arg,1);
 	XtSetValues(USETestPattFileWidget[i],arg,1); 
	XtSetValues(DELPattFileWidget[i],arg,1);
    }

    /* Update test_pattern Display */
    if(CURR_TEST_PATTERN_SET >= setNo)
	CURR_TEST_PATTERN_SET--;
    sprintf(buf,"%s",PATTERN_SET_FILE[CURR_TEST_PATTERN_SET]);
    ui_xSetLabel(ui_showTestPattFile, buf);

    /* destroy the widgets corresponding to the deleted pattern */

    if(NO_OF_PATTERN_SETS > 0){
	XtRemoveAllCallbacks(USEPattFileWidget[NO_OF_PATTERN_SETS],XtNcallback);
	XtRemoveAllCallbacks(USETestPattFileWidget[NO_OF_PATTERN_SETS],XtNcallback); 
	XtRemoveAllCallbacks(DELPattFileWidget[NO_OF_PATTERN_SETS],XtNcallback);
	XtDestroyWidget(USEPattFileWidget[NO_OF_PATTERN_SETS]);
	XtDestroyWidget(USETestPattFileWidget[NO_OF_PATTERN_SETS]); 
	XtDestroyWidget(DELPattFileWidget[NO_OF_PATTERN_SETS]);
	USEPattFileWidget[NO_OF_PATTERN_SETS] = NULL;
	USETestPattFileWidget[NO_OF_PATTERN_SETS] = NULL; 
	DELPattFileWidget[NO_OF_PATTERN_SETS] = NULL;
    }

}



/*****************************************************************************
  FUNCTION : ui_rem_updatePattList

  PURPOSE  : Updates the lists of available pattern sets.
             Both the list under the DEL and the USE button are updated.
  RETURNS  : void
  NOTES    : Control panel has to be created before the call to this function

  UPDATE   : 19.02.95
******************************************************************************/
void ui_rem_updatePattList (void)
{
    int i;
    int n = 1;
    Arg arg[10];

    /* check whether any pattern files are loaded */
 
    if(PATTERN_SET_FILE[CURR_PATTERN_SET] != NULL){

	for(i=0; i<NO_OF_PATTERN_SETS;i++){       /* for all available files */
	    XtSetArg(arg[0],XtNlabel,PATTERN_SET_FILE[i]);

	    /* if the widgets do not exist already, create them now */
	    /* else update only their labels                        */

	    if(USEPattFileWidget[i] == NULL){
		USEPattFileWidget[i] = 
		    XtCreateManagedWidget(PATTERN_SET_FILE[i],
					  smeBSBObjectClass,
					  REM_UFileMenu,NULL,0);
		XtAddCallback(USEPattFileWidget[i], XtNcallback, 
			      (XtCallbackProc)ui_rem_usePattSet,
			      (XtPointer) ((long)i));
	    }else{
		XtSetValues(USEPattFileWidget[i],arg,n);
	    }

	    if(USETestPattFileWidget[i] == NULL){
		USETestPattFileWidget[i] =            
		    XtCreateManagedWidget(PATTERN_SET_FILE[i],
					  smeBSBObjectClass,
					  REM_UTestFileMenu,NULL,0);
		XtAddCallback(USETestPattFileWidget[i], XtNcallback, 
			      (XtCallbackProc)ui_rem_useTestPattSet,
			      (XtPointer) ((long)i));
	    }else{
		XtSetValues(USETestPattFileWidget[i],arg,n);
	    }


	    if(DELPattFileWidget[i] == NULL){
		DELPattFileWidget[i] = 
		    XtCreateManagedWidget(PATTERN_SET_FILE[i],
					  smeBSBObjectClass,
 					  REM_DFileMenu,NULL,0);
		XtAddCallback(DELPattFileWidget[i], XtNcallback, 
			      (XtCallbackProc)ui_rem_delPattSet,
			      (XtPointer) ((long)i));
	    }else{
		XtSetValues(DELPattFileWidget[i],arg,n);
	    }
	}

    }else{

	/* if no files are loaded make a comment */
	USEPattFileWidget[0] = 
	    XtCreateManagedWidget("No Files",smeBSBObjectClass, 
				  REM_DFileMenu, NULL, 0);
	USETestPattFileWidget[0] = 
	    XtCreateManagedWidget("No Files",smeBSBObjectClass, 
				  REM_DFileMenu, NULL, 0); 
    }
}



/*****************************************************************************
  FUNCTION : ui_rem_resetSubPat

  PURPOSE  : resets the values of the subpattern panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 16.09.1993
******************************************************************************/
static void ui_rem_resetSubPat (void)
{

    int                i;

    for(i=0; i< subIPatDims; i++) subIPatPos[i] = 1;
    for(i=0; i< subOPatDims; i++) subOPatPos[i] = 1;
    
    ui_rem_setSubPatPanel();

}


/*****************************************************************************
  FUNCTION : ui_rem_setSubPatPanel

  PURPOSE  : sets all values for the subpattern panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 16.09.1993
******************************************************************************/
static void ui_rem_setSubPatPanel (void)
{

    char               buf[20];
    int                i;
    pattern_set_info   patt_info;
    pattern_descriptor descrip;
    krui_err           error;

    if(SubPatPanel == 1){
	if((error = krui_GetPatInfo(&patt_info, &descrip))< 0){
	    ui_checkError(error);
	    return;
	}
	for (i=0; i< subIPatDims; i++) {
	    sprintf(buf," %d",subIPatPos[i]);
	    ui_xSetLabel(subIPatPosW[i],buf);
	    sprintf(buf," %d",subIPatStep[i]);
	    ui_xSetLabel(subIPatStepW[i],buf);
	    sprintf(buf," %d",descrip.input_dim_sizes[i]);
	    ui_xSetLabel(in_dim_widgets[i],buf);
	} 
	for (i=0; i< subOPatDims; i++) {
	    sprintf(buf," %d",subOPatPos[i]);
	    ui_xSetLabel(subOPatPosW[i],buf);
	    sprintf(buf," %d",subOPatStep[i]);
	    ui_xSetLabel(subOPatStepW[i],buf);
	    sprintf(buf," %d",descrip.output_dim_sizes[i]);
	    ui_xSetLabel(out_dim_widgets[i],buf);
	} 
    }


}

/*****************************************************************************
  FUNCTION : ui_rem_getSubPatPanel

  PURPOSE  : read all values from the subpattern panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 16.09.1993
******************************************************************************/
void ui_rem_getSubPatPanel (void)
{
    int i;

    if(SubPatPanel == 1){
	for(i=0; i<subIPatDims; i++) {
	    subIPatStep[i] = ui_xIntFromAsciiWidget(subIPatStepW[i]);
	    subIPatSize[i] = ui_xIntFromAsciiWidget(subIPatSizeW[i]);
	}
	for(i=0; i<subOPatDims; i++) {
	    subOPatStep[i] = ui_xIntFromAsciiWidget(subOPatStepW[i]);
	    subOPatSize[i] = ui_xIntFromAsciiWidget(subOPatSizeW[i]);
	}
    }
}

/*****************************************************************************
  FUNCTION : ui_rem_displayList

  PURPOSE  : popup a list panel to select learn, update, or init function
  RETURNS  : void
  NOTES    : 

  UPDATES  :
*****************************************************************************/
void ui_rem_displayList(Widget w, struct SimpleListType *listDescriptorPtr)

{
    Widget      menu, mItem;
    int         i;
    char        *func_name;
    int         listType;


    /* The list types in question have the numbers 4 to 6 and 12 !! */
    /* Do a mapping to 0, 1, 2, 3 */
    if(listDescriptorPtr->listType<7)
	listType = listDescriptorPtr->listType - 4;
    else
	listType = listDescriptorPtr->listType - 9;

    menu = XtCreatePopupShell("menu", simpleMenuWidgetClass, 
			      listDescriptorPtr->widgetPtr, NULL, 0);
    /* alloc space for function list */
    ui_cb_list[listType] = 
	(struct ui_cb_data *)calloc(krui_getNoOfFunctions(),
				    sizeof(struct ui_cb_data));

    /* create the list items */
    i=0;
    func_name = ui_list_getFirstItem(listDescriptorPtr->listType);
    do{
        mItem = XtCreateManagedWidget(func_name,smeBSBObjectClass, menu,
				      NULL,0);
	ui_cb_list[listType][i].func_type = listDescriptorPtr->listType;
	ui_cb_list[listType][i].func_name = calloc(strlen(func_name)+1,
						   sizeof(char));
	strcpy(ui_cb_list[listType][i].func_name,func_name);
        XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_rem_setFunc, 
		      (XtPointer) ((long)(listType*100 + i)));i++;
	func_name = ui_list_getNextItem(listDescriptorPtr->listType);
    }while (func_name != NULL);
}



/*****************************************************************************
  FUNCTION : ui_rem_setFunc

  PURPOSE  : to set the Function selected by the mouse from the control panel
  RETURNS  : 
  NOTES    :

  UPDATE   : 05.04.95
******************************************************************************/
void  ui_rem_setFunc (Widget widget, int func_type, caddr_t call_data)
{

    char buf[80];
    int  type,no,i;
    float dummy[UI_NO_REMAP_PARAMS];
    krui_err error;

    for (i=0; i<UI_NO_REMAP_PARAMS; i++)
	dummy[i]=0.0;

    type = func_type/100;
    no   = func_type%100;

    /* type is 0, 1 or 2 (as array index) while UI_LIST* are 4, 5, and 6 */
    /* so add 4 in the switch */
    switch(type+4){
    case UI_LIST_LEARN_FUNC:
	sprintf(buf,"Learning func:  %s\n",ui_cb_list[type][no].func_name);
	ui_checkError(krui_setLearnFunc(ui_cb_list[type][no].func_name));
 	ui_NumberOfLearnParamsChanged();
	break;
    case UI_LIST_UPDATE_FUNC:
	sprintf(buf,"Update func:  %s\n",ui_cb_list[type][no].func_name);
	ui_checkError(krui_setUpdateFunc(ui_cb_list[type][no].func_name));
 	ui_NumberOfUpdateParamsChanged();
	break;
    case UI_LIST_INIT_FUNC:
	sprintf(buf,"Init. func:  %s\n",ui_cb_list[type][no].func_name);
	ui_checkError(
               krui_setInitialisationFunc(ui_cb_list[type][no].func_name));
 	ui_NumberOfInitParamsChanged();
	break;
    case (UI_LIST_REMAP_FUNC-5): /* REMAP has type 12 not 7 !! */
	if((error = krui_setRemapFunc(ui_cb_list[type][no].func_name,
				      dummy)) != 0){
	    ui_checkError(error);
	    sprintf(buf,"");
	}else{
	    sprintf(buf,"Remap. func:  %s\n",ui_cb_list[type][no].func_name);
	    ui_NumberOfRemapParamsChanged();
	}
	break;
    default:
	break;
    }

    ui_tw_printMessage(buf); 
    ui_xSetLabel(ui_controlMessageWidget, buf);

}


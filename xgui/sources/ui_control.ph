/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_control.ph,v $
  SHORTNAME      : control.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 20.7.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 1.10 $ 
  LAST CHANGE    : $Date: 1998/04/20 11:55:10 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_CONTROL_DEFINED_
#define _UI_CONTROL_DEFINED_


/* begin global definition section */


void  ui_xCreateControlPanel(Widget parent);
void  ui_NumberOfLearnParamsChanged(void);
void  ui_NumberOfUpdateParamsChanged(void);
void  ui_NumberOfInitParamsChanged(void);
void  ui_NumberOfRemapParamsChanged(void);



/* shell widget of control panel */
Widget    ui_controlPanel;
/* ascii and label widgets containing/displaying data */
Widget    ui_numberOfStepsWidget, ui_numberOfStepsDoneWidget;
Widget    ui_numberOfCyclesWidget, ui_numberOfPatternWidget;
Widget    ui_showModeLabel;
Widget    ui_showPattFile;
Widget    ui_showTestPattFile;
Widget    REM_UFileMenu;
Widget    REM_UTestFileMenu;  
Widget    REM_DFileMenu;
Widget    USEPattFileWidget[NO_OF_PAT_SETS+1];
Widget    USETestPattFileWidget[NO_OF_PAT_SETS+1];
Widget    DELPattFileWidget[NO_OF_PAT_SETS+1];
Widget    subIPatSizeW[MAX_NO_OF_VAR_I_DIM],subIPatStepW[MAX_NO_OF_VAR_I_DIM],
          subIPatPosW[MAX_NO_OF_VAR_I_DIM];
Widget    subOPatSizeW[MAX_NO_OF_VAR_O_DIM],subOPatStepW[MAX_NO_OF_VAR_O_DIM],
          subOPatPosW[MAX_NO_OF_VAR_O_DIM];
Widget    in_dim_widgets[MAX_NO_OF_VAR_I_DIM];
Widget    out_dim_widgets[MAX_NO_OF_VAR_O_DIM];
Widget    ui_learnParameterWidgets[UI_NO_LEARN_PARAMS];
Widget    ui_updateParameterWidgets[UI_NO_UPDATE_PARAMS];
Widget    ui_initParameterWidgets[UI_NO_INIT_PARAMS];
Widget    ui_remapParameterWidgets[UI_NO_REMAP_PARAMS];
Widget    ui_testFrequencyWidget;
Widget    jogbutton;

/* current configuration of control panel */
int       ui_noOfCurrentPattern = 0;  /* number of current pattern
                                         0 means, no patterns loaded */
int       ui_patternLoadMode    = OUTPUT_ACT;
int       SubPatPanel = -1;     /* == -1 sup pattern panel so far never created
				   ==  0 not poped up right now
				   ==  1 poped up right now */
int       ui_numberOfStepsDone  = 0;
int       subIPatSize[MAX_NO_OF_VAR_I_DIM] = {1, 1};
int       subIPatPos[MAX_NO_OF_VAR_I_DIM]  = {1, 1};
int       subIPatStep[MAX_NO_OF_VAR_I_DIM] = {1, 1};
int       subOPatStep[MAX_NO_OF_VAR_O_DIM] = {1, 1};
int       subOPatSize[MAX_NO_OF_VAR_O_DIM] = {1, 1};
int       subOPatPos[MAX_NO_OF_VAR_O_DIM]  = {1, 1};
int       subIPatNo;
int       subIPatDims;
int       subOPatDims;
float     ui_learnParameters[UI_NO_LEARN_PARAMS+UI_NO_OF_CASCADE_PARAMS] = {
			    0.2, 0.0, 0.0, 0.0, 0.0 };
float     ui_updateParameters[UI_NO_UPDATE_PARAMS] = {
			    0.0, 0.0, 0.0, 0.0, 0.0 };
float     ui_initParameters[UI_NO_INIT_PARAMS] = {
			    1.0, -1.0, 0.0, 0.0, 0.0 };
float     ui_remapParameters[UI_NO_REMAP_PARAMS] = {
			    0.0, 0.0, 0.0, 0.0, 0.0 };
Bool      ui_shuffleFlg     = FALSE;
Bool      ui_jogFlg         = FALSE;
Bool      ui_jogCorrFlg     = FALSE;
float     jog_low = -0.001, jog_high = 0.001, jog_correlation = 0.98;
Bool      ui_shuffleSubPatFlg   = FALSE;
Bool      ui_controlIsCreated    = FALSE;

int       ui_numberOfCycles     = 1;
int       ui_numberOfSteps      = 1;
int       ui_testFrequency      = 0;


/* end global definition section */


/* begin private definition section */

/* end private definition section */


#endif /* _UI_CONTROL_DEFINED_ */



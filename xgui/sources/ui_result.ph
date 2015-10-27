/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_result.ph,v $
  SHORTNAME      : result.ph
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Michael Vogt
  DATE           : 20. 5. 1992

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:35 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_RESULT_DEFINED_
#define _UI_RESULT_DEFINED_


/* begin global definition section */


#define	UI_RESULT_INPUT_YES	0
#define UI_RESULT_INPUT_NO	1
#define	UI_RESULT_OUTPUT_YES	0
#define UI_RESULT_OUTPUT_NO	1
#define UI_RESULT_MODE_CREATE	0
#define UI_RESULT_MODE_APPEND	1


void ui_popupResult (Widget);


Bool            ui_ResultIsCreated = FALSE;


/* end global definition section */


/* begin private definition section */


static void ui_performRadioButtons (Widget [], int, int);
static void ui_result_Setinput (Widget, int, caddr_t);
static void ui_result_Setoutput (Widget, int, caddr_t);
static void ui_result_Setmode (Widget, int, caddr_t);
static void ui_result_lastcall (Widget, int, caddr_t);
static void ui_result_default (Widget, int, caddr_t);
static void ui_resultDone (Widget, int, caddr_t);
static void ui_resultCancel (Widget, int, caddr_t);
static Widget ui_xCreateResultPanel (Widget);


static int      ui_result_output_state;
static int      ui_result_no_start_pat;
static int      ui_result_no_end_pat;
static int      ui_result_input_state;
static int      ui_result_mode_state;
static Widget   ui_resultInputRButtons[2];
static Widget   ui_resultOutputRButtons[2];
static Widget   ui_resultModeRButtons[2];
static Widget   ui_resultStartPattern;
static Widget   ui_resultEndPattern;


/* end private definition section */


#endif /* _UI_RESULT_DEFINED_ */



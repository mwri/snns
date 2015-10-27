/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/pr_main.ph,v $
  SHORTNAME      : pr_main.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tobias Schreiner
  DATE           : 13.3.94

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/05/20 15:00:36 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#ifndef _PR_MAIN_DEFINED_
#define  _PR_MAIN_DEFINED_

/* begin global definition section */

Widget pr_learnFuncText, pr_prunFuncText,pr_prunFuncIcon,pr_learnFuncIcon;

void pr_createWindow (void);
void pr_readElements (void);

int pr_open = FALSE;
/* end global definition section */


/* begin privat definition section */

static Widget pr_mainWidget,
              pr_maxErrorIncDialog, pr_acceptedErrorDialog,
              pr_trainCyclesDialog, pr_retrainCyclesDialog,
	      pr_minErrorDialog, pr_obsInitParamDialog,
	      pr_recreateB[2], pr_refreshB[2], pr_inputPruningB[2],
	      pr_hiddenPruningB[2];

static void pr_doneProc (void);
static void pr_setFunc (Widget w, int value, caddr_t call_data);
static void pr_setrefresh(Widget w, int value, caddr_t call_data);
static void pr_setrecreate(Widget w, int value, caddr_t call_data);
static void pr_setinputPrun(Widget w, int value, caddr_t call_data);
static void pr_sethiddenPrun(Widget w, int value, caddr_t call_data);

/* end privat definition section */


#endif /* _PR_MAIN_DEFINED_ */

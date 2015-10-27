/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/cc_main.ph,v $
  SHORTNAME      : cc_main.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Michael Schmalzl, modified by Christian Wehrfritz (PCC)
  DATE           : 23.2.93

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.13 $
  LAST CHANGE    : $Date: 1998/05/20 15:00:34 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#ifndef _CC_MAIN_DEFINED_
#define  _CC_MAIN_DEFINED_

/* begin global definition section */
void cc_readElements(void);
void cc_createWindow (void);

int cc_open = 0;
/* end global definition section */


/* begin privat definition section */

static int   cc_pruneFunc    = -1,
             cc_backfittOnOff= -1,
             cc_learningFunc = -1,
             cc_candActFunc  = -1;
/* already defined in cc_glob.ph:  int cc_modification = -1 */

static Widget cc_mainWidget;
static Widget cc_w1b,cc_w2b,cc_w3b,
              cc_w4b,cc_w5b,cc_w6b,cc_w7b,cc_pfb,
              cc_w9b,cc_w10b,cc_w11b,cc_w20b,
              cc_w21b,cc_w21c,cc_w21d,cc_w21e,cc_w21f,cc_w21g;
static Widget cc_covB[2], cc_cacheB[2], cc_pruneB[2];

static void cc_centerString(char *string, int length1, int length2);
static void cc_selectActFuncCand(Widget w, int value, caddr_t call_data);
static void cc_selectOnOff(Widget w, int value, caddr_t call_data);
static void cc_selectPruneFunc(Widget w, int value, caddr_t call_data);
static void cc_selectLearningFunc(Widget w, int value, caddr_t call_data);
static void cc_selectModification(Widget w, int value, caddr_t call_data);
static void cc_doneProc(void);
static void cc_createCandidatePannel (Widget parent);
static void cc_createOutputPannel (Widget parent);
static void cc_createGlobalPannel (Widget parent);


/* end privat definition section */


#endif /* _CC_MAIN_DEFINED_ */

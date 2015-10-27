/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_controlP.ph,v $
  SHORTNAME      : controlP.ph
  SNNS VERSION   : 4.2
  
  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.7.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 1.11 $ 
  LAST CHANGE    : $Date: 1998/04/20 11:55:18 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
*****************************************************************************/


#ifndef _UI_CONTROLP_DEFINED_
#define _UI_CONTROLP_DEFINED_


/* begin global definition section */


void ui_rem_displayPatternNumber (void);
void ui_rem_resetCounter (void);
void ui_rem_resetNet (Widget w, XtPointer button, caddr_t call_data);
void ui_rem_doInitialization (Widget w, caddr_t client_data, caddr_t call_data);
void ui_rem_deleteNet (Widget w, caddr_t client_data, caddr_t call_data);
void ui_rem_stepsProc (Widget widget, Bool multiStepPressed, caddr_t call_data);
void ui_rem_moveInPatternsProc (Widget w, int moveType, caddr_t call_data);
void ui_rem_testProc (Widget w, XtPointer button, caddr_t call_data);
void ui_rem_deleteAllPatternsProc (Widget w, XtPointer button, 
					caddr_t call_data);
void ui_rem_modifyPatternProc (Widget w, XtPointer button, caddr_t call_data);
void ui_rem_deletePatternProc (Widget w, XtPointer button, caddr_t call_data);
void ui_rem_newPatternProc (Widget w, caddr_t call_data);
void ui_rem_learnProc (Widget w, int learnType, caddr_t call_data);
void ui_rem_errProc (Widget w, XtPointer button, caddr_t call_data);
void ui_rem_stopProc (Widget widget, caddr_t client_data, caddr_t call_data);
void ui_rem_shuffleProc (Widget w, caddr_t client, caddr_t call);
void ui_rem_performValidate(caddr_t call_data);
void ui_rem_performValidation(struct ErrorValuesType * test_error_values);

#ifdef PARAGON_KERNEL
void ui_rem_SwitchParallel(Widget w, caddr_t client, caddr_t call);
#endif

void ui_rem_showModeProc (Widget w, int value, caddr_t call_data);
void ui_rem_setRandomWeights (Widget w, int randomType, caddr_t call_data);
void ui_rem_getKernelInfo (Widget w, XtPointer button, caddr_t call_data);
void ui_rem_shuffleSubPatProc (Widget w, caddr_t client, caddr_t call);
void ui_rem_defSubPat (Widget button, int randomType, caddr_t call_data);
void ui_rem_usePattSet (Widget button, int setNo, caddr_t call_data);
void ui_rem_useTestPattSet (Widget button, int setNo, caddr_t call_data);
void ui_rem_delPattSet (Widget button, int setNo, caddr_t call_data);
void ui_rem_updatePattList (void);
void ui_rem_getSubPatPanel (void);
void ui_rem_displayList(Widget w, struct SimpleListType *listDescriptorPtr);
void  ui_rem_setFunc(Widget widget,int sel, caddr_t call_data);

Widget ui_popPattern,ui_popRemList;

/* end global definition section */


/* begin private definition section */


static void ui_rem_displayCounter (void);
static void ui_rem_incCounter (void);
static void ui_rem_finishSteps (void);
static Boolean ui_rem_performStep (caddr_t client_data);
static void ui_rem_printLearnInfo (float *err_val);
static void ui_rem_finishLearning (void);
static Boolean ui_rem_performLearn (int learnType);
static Boolean ui_rem_performPrunInit (int learnType);
static Boolean ui_rem_performPrun (int learnType);
static float ui_rem_performFFLearn (int cycles, int learnType);
static void ui_rem_resetSubPat (void);
static void ui_rem_setSubPatPanel (void);
static void ui_rem_moveInSubPattProc(Widget w, int moveParam,caddr_t call_data);

#ifdef _UNUSED_FUNCTIONS_
static double ui_sGamma (double x);
static double ui_sFisherDis(double m, double n, double x, double accuracy);
#endif

/* use of work procs (update steps and learning cycles) */
static XtWorkProcId  ui_workProcId =  0;         /* id reported by X */
static int           ui_numberOfWorkCycles = 0;  /* loops left to do */
static int           ui_numberOfLearnCycles = 0; /* loops to do */
static FlagType      ui_workType = UI_STEPS;     /* UI_STEPS or UI_LEARN */
static float	     pr_maxError;                /* maximum error to stop pruning */

static float         *ui_errorArray;
static float         *ui_testArray;
static int           ui_noErrorArrayElements;
static int           ui_noTestArrayElements;
static int           INIT_KOHONEN_PRINT;

float                ui_testError = 0; 

static char *tmp_file;

#define SP_FIRST 1
#define SP_PREVIOUS 2
#define SP_NEXT 3
#define SP_LAST 4

struct ui_cb_data{
    int func_type;
    char *func_name;
};
typedef struct ui_cb_data ui_cb_data;

static ui_cb_data *ui_cb_list[4];
/* end private definition section */


#endif /* _UI_CONTROLP_DEFINED_ */


/* end of file */
/* lines: 94 */

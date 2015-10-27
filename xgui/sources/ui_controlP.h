/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_controlP.h,v $
  SHORTNAME      : controlP.h
  SNNS VERSION   : 4.2
  
  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.7.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 1.8 $ 
  LAST CHANGE    : $Date: 1998/04/20 11:55:15 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_CONTROLP_DEFINED_
#define _UI_CONTROLP_DEFINED_




extern void ui_rem_displayPatternNumber (void);
extern void ui_rem_resetCounter (void);
extern void ui_rem_resetNet (Widget w, XtPointer button, caddr_t call_data);
extern void ui_rem_doInitialization (Widget w, caddr_t client_data, caddr_t call_data);
extern void ui_rem_deleteNet (Widget w, caddr_t client_data, caddr_t call_data);
extern void  ui_rem_stepsProc (Widget widget, Bool multiStepPressed, caddr_t call_data);
extern void ui_rem_moveInPatternsProc (Widget w, int moveType, caddr_t call_data);
extern void ui_rem_testProc (Widget w, XtPointer button, caddr_t call_data);
extern void ui_rem_deleteAllPatternsProc (Widget w, XtPointer button, 
					caddr_t call_data);
extern void ui_rem_modifyPatternProc (Widget w, XtPointer button, caddr_t call_data);
extern void ui_rem_deletePatternProc (Widget w, XtPointer button, caddr_t call_data);
extern void ui_rem_newPatternProc (Widget w, caddr_t call_data);
extern void ui_rem_learnProc (Widget w, int learnType, caddr_t call_data);
extern void ui_rem_errProc (Widget w, XtPointer button, caddr_t call_data);
extern void  ui_rem_stopProc (Widget widget, caddr_t client_data, caddr_t call_data);
extern void ui_rem_shuffleProc (Widget w, caddr_t client, caddr_t call);
extern void ui_rem_performValidate(caddr_t call_data);
extern void ui_rem_performValidation(struct ErrorValuesType * test_error_values);

#ifdef PARAGON_KERNEL
extern void ui_rem_SwitchParallel(Widget w, caddr_t client, caddr_t call);
#endif

extern void ui_rem_showModeProc (Widget w, int value, caddr_t call_data);
extern void ui_rem_setRandomWeights(Widget w, int randomType,caddr_t call_data);
extern void ui_rem_getKernelInfo(Widget w, XtPointer button, caddr_t call_data);
extern void ui_rem_defSubPat(Widget button, int randomType, caddr_t call_data);
extern void ui_rem_usePattSet(Widget button, int setNo, caddr_t call_data);
extern void ui_rem_useTestPattSet(Widget button, int setNo, caddr_t call_data);
extern void ui_rem_delPattSet(Widget button, int setNo, caddr_t call_data);
extern void ui_rem_updatePattList(void);
extern void ui_rem_getSubPatPanel(void);
extern void ui_rem_displayList(Widget w, 
			       struct SimpleListType *listDescriptorPtr);
extern void ui_rem_setFunc(Widget widget,int sel, caddr_t call_data);


extern Widget ui_popPattern,ui_popRemList;

#endif /* _UI_CONTROLP_DEFINED_ */
/* end of file */

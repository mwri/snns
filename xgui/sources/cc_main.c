/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/cc_main.c,v $
  SHORTNAME      : cc_main.c
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the CASCADE-window. 
  NOTES          :

  AUTHOR         : Michael Schmalzl, modified by Christian Wehrfritz (PCC)  
  DATE           : 23.2.93

  CHANGED BY     : Michael Schmalzl
  RCS VERSION    : $Revision: 2.18 $
  LAST CHANGE    : $Date: 1998/04/22 16:08:33 $

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
#include "ui_xWidgets.h"
#include "kr_ui.h"
#include "ui_display.h"
#include "ui_main.h"
#include "ui_mainP.h"
#include "ui_confirmer.h"
#include "cc_type.h"
#include "cc_mac.h"
#include "cc_glob.h"
#include "ui_control.h"
#include "ui_key.h"
#include "ui_netUpdate.h"

#include "cc_main.ph"

/*****************************************************************************
  FUNCTION :cc_centerString

  PURPOSE  : 
  NOTES    :

  UPDATE : 24.10.96
******************************************************************************/
static void cc_centerString(char *string, int length1, int length2) 
{ 
    int i,difference,move;

    if((difference=(length2 - length1)) >=2) {
	move = difference / 2;
	for(i=length1-1;i>=0;i--) {
	    string[i+move] = string[i];
	}
	for(i=0;i<move;i++){
	    string[i] = ' ';
	}
    }
}
       

/*****************************************************************************
  FUNCTION : cc_selectActFuncCand

  PURPOSE  : 
  NOTES    :

  UPDATE   : 24.10.96
******************************************************************************/
static void cc_selectActFuncCand(Widget w, int value, caddr_t call_data)
{
    switch(value) {
    case ASYM_SIGMOID: ui_xSetLabel(cc_w10b,"Logistic");
	break;
    case SYM_SIGMOID : ui_xSetLabel(cc_w10b," LogSym ");
	break;
    case TANH        : ui_xSetLabel(cc_w10b," TanH   ");
	break;
    case RANDOM      : ui_xSetLabel(cc_w10b," Random ");
	break;
    case SINUS       : ui_xSetLabel(cc_w10b," Sinus  ");
	break;
    case EXPONENTIAL : ui_xSetLabel(cc_w10b," Gauss  ");
	break;
    }
    cc_data.CAND.actFunc = cc_candActFunc = value;
}


/*****************************************************************************
  FUNCTION :cc_selectOnOff

  PURPOSE  : 
  NOTES    :

  UPDATE   : 24.10.96
******************************************************************************/
static void cc_selectOnOff(Widget w, int value, caddr_t call_data)
{
    if((w == cc_covB[0]) || (w == cc_covB[1]))
	ui_setToggles(value, &cc_data.GLOBAL.onOff, 2, cc_covB);
    if((w == cc_cacheB[0]) || (w == cc_cacheB[1]))
	ui_setToggles(value, &cc_data.GLOBAL.fastmode, 2, cc_cacheB);
    if((w == cc_pruneB[0]) || (w == cc_pruneB[1]))
	ui_setToggles(value, &cc_data.GLOBAL.pruneOnOff, 2, cc_pruneB);
}


/*****************************************************************************
  FUNCTION : cc_selectLearningFunc

  PURPOSE  : 
  NOTES    :

  UPDATE   : 24.10.96
******************************************************************************/
static void cc_selectLearningFunc(Widget w, int value, caddr_t call_data)
{
    switch(value) {
    case QUICKPROP        : ui_xSetLabel(cc_w11b," Quickprop ");
	break;
    case BACKPROP         : ui_xSetLabel(cc_w11b," Batch-BP  ");
	break;
    case RPROP            : ui_xSetLabel(cc_w11b,"   Rprop   ");
	break;
    case BACKPROP_ONLINE  : ui_xSetLabel(cc_w11b," Backprop  ");
    }
    cc_data.GLOBAL.learningFunc = cc_learningFunc = value;
}

/*****************************************************************************
  FUNCTION : cc_selectModification

  PURPOSE  : 
  NOTES    :

  UPDATE   : 24.10.96
******************************************************************************/
static void cc_selectModification(Widget w, int value, caddr_t call_data)
{
    switch(value) {
    case CC_NO_MOD: 
	ui_xSetLabel(cc_w20b," none ");
	break;
    case CC_SDCC: 
	ui_xSetLabel(cc_w20b," SDCC ");
	break;
    case CC_LFCC: 
	ui_xSetLabel(cc_w20b," LFCC ");
	break;
    case CC_ECC: 
	ui_xSetLabel(cc_w20b," ECC  ");
	break;
    case CC_GCC: 
	ui_xSetLabel(cc_w20b," GCC  ");
	break;
    case CC_STAT: 
	ui_xSetLabel(cc_w20b,"Static");
	break;
    case CC_RLCC: 
	ui_xSetLabel(cc_w20b," RLCC ");
	break;
    }
    cc_data.GLOBAL.modification = cc_modification = value;
}

/*****************************************************************************
  FUNCTION : cc_selectPruneFunc

  PURPOSE  : 
  NOTES    :

  UPDATE   : 24.10.96
******************************************************************************/
static void cc_selectPruneFunc(Widget w, int value, caddr_t call_data)
{
    switch(value) {
    case SBC:   ui_xSetLabel(cc_pfb," SBC ");
	break;
    case AIC:   ui_xSetLabel(cc_pfb," AIC ");
	break;
    case CMSEP: ui_xSetLabel(cc_pfb,"CMSEP");
	break;
    }
    cc_data.GLOBAL.pruneFunc = cc_pruneFunc = value;
}


/*****************************************************************************
  FUNCTION : cc_readElements

  PURPOSE  : 
  NOTES    :

  UPDATE   : 24.10.96
******************************************************************************/
void cc_readElements(void)
{
    int i;

    if((cc_learningFunc == -1) || (cc_candActFunc == -1) ||
       (cc_pruneFunc == -1)) {
	cc_pruneFunc    = cc_data.GLOBAL.pruneFunc;
	cc_backfittOnOff= cc_data.GLOBAL.backfittOnOff;
	cc_learningFunc = cc_data.GLOBAL.learningFunc;
	cc_candActFunc  = cc_data.CAND.actFunc;
	cc_modification = cc_data.GLOBAL.modification;
    }

    if(cc_open) {
	cc_data.GLOBAL.pixelError         = ui_xFloatFromAsciiWidget(cc_w9b);
	cc_data.GLOBAL.learningFunc       = cc_learningFunc;
	cc_data.GLOBAL.pruneFunc          = cc_pruneFunc;
	cc_data.GLOBAL.modification       = cc_modification;
	cc_data.GLOBAL.addParameters[0]   = ui_xFloatFromAsciiWidget(cc_w21c);
	cc_data.GLOBAL.addParameters[1]   = ui_xFloatFromAsciiWidget(cc_w21d);
	cc_data.GLOBAL.addParameters[2]   = ui_xFloatFromAsciiWidget(cc_w21e);
	cc_data.GLOBAL.addParameters[3]   = ui_xFloatFromAsciiWidget(cc_w21f);
	cc_data.GLOBAL.addParameters[4]   = ui_xFloatFromAsciiWidget(cc_w21g);
	cc_data.CAND.covarianceChange     = ui_xFloatFromAsciiWidget(cc_w1b);
	cc_data.CAND.candidatePatience    = ui_xIntFromAsciiWidget(cc_w2b); 
	cc_data.CAND.maxNoOfUpdateCycles  = ui_xIntFromAsciiWidget(cc_w3b);
	cc_data.CAND.maxNoOfCandUnits     = ui_xIntFromAsciiWidget(cc_w7b); 
	cc_data.CAND.actFunc              = cc_candActFunc;
	cc_data.OUT.errorChange           = ui_xFloatFromAsciiWidget(cc_w4b);
	cc_data.OUT.outputPatience        = ui_xIntFromAsciiWidget(cc_w5b);
	cc_data.OUT.maxNoOfUpdateCycles   = ui_xIntFromAsciiWidget(cc_w6b);
    }
    ui_learnParameters[6] = cc_data.GLOBAL.pixelError;       
    ui_learnParameters[7] = (float)cc_data.GLOBAL.learningFunc;   
    ui_learnParameters[8] = (float)cc_data.GLOBAL.onOff; 
          
    ui_learnParameters[9]  = cc_data.CAND.covarianceChange;  
    ui_learnParameters[10] = cc_data.CAND.candidatePatience; 
    ui_learnParameters[11] = cc_data.CAND.maxNoOfUpdateCycles; 
    ui_learnParameters[12] = cc_data.CAND.maxNoOfCandUnits;  
    ui_learnParameters[13] = (float)cc_data.CAND.actFunc;  
         
    ui_learnParameters[14] = cc_data.OUT.errorChange;        
    ui_learnParameters[15] = cc_data.OUT.outputPatience;    
    ui_learnParameters[16] = cc_data.OUT.maxNoOfUpdateCycles;

    ui_learnParameters[17] = (float)cc_data.GLOBAL.pruneOnOff; 
    ui_learnParameters[18] = (float)cc_data.GLOBAL.backfittOnOff; 
    ui_learnParameters[19] = cc_data.GLOBAL.backfittPatience;
    ui_learnParameters[20] = cc_data.GLOBAL.pruneFunc; 
    ui_learnParameters[21] = (float)cc_data.GLOBAL.modification;

    for (i=0;i<5;i++)
	ui_learnParameters[22+i] = cc_data.GLOBAL.addParameters[i];

    ui_learnParameters[27] = (float)cc_data.GLOBAL.fastmode;

}

/*****************************************************************************
  FUNCTION : cc_doneProc

  PURPOSE  : callback function of the done-button. You are leaving CC. 
             The storage is deallocated.
  NOTES    :

  UPDATE   : 3.4.1992
******************************************************************************/
static void cc_doneProc(void)
{
    cc_readElements();
    XtDestroyWidget(cc_mainWidget);
    cc_open = 0;
}



/*****************************************************************************
  FUNCTION : cc_createCandidatePannel

  PURPOSE  : creates the candidate pannel, which is part of the CC-window
  NOTES    :

  UPDATE   : 5.10.92
******************************************************************************/
static void cc_createCandidatePannel (Widget parent)
{
    Widget       form, menu, mItem, label, top_label;
    Arg          arg[25];
    Cardinal     n;
    char         buf[20];
  
    n = 0;  
    form = XtCreateManagedWidget ("pannel", formWidgetClass, parent, arg, n);

    top_label = ui_xCreateLabelItem("       Parameters for Candidate Units",
				    form, 35*ui_labelFontWidth, NULL, NULL);
    label = ui_xCreateLabelItem("Min. covariance change: ",form,
				30*ui_labelFontWidth,NULL,top_label);
    sprintf(buf,"%.3f",cc_data.CAND.covarianceChange);
    cc_w1b = ui_xCreateDialogItem ("cc_w1b",form,buf,6*ui_labelFontWidth,
				   label, top_label);

    label = ui_xCreateLabelItem("Candidate patience: ",form,
				 30*ui_labelFontWidth,NULL,cc_w1b);
    sprintf(buf,"%d",cc_data.CAND.candidatePatience);
    cc_w2b = ui_xCreateDialogItem ("cc_w2b",form,buf,6*ui_labelFontWidth,
				   label,cc_w1b); 

    label = ui_xCreateLabelItem("Max. no. of covariance updates: ",form,
				 30*ui_labelFontWidth,NULL,cc_w2b);
    sprintf(buf,"%d",cc_data.CAND.maxNoOfUpdateCycles);
    cc_w3b = ui_xCreateDialogItem ("cc_w3b",form,buf,6*ui_labelFontWidth,
				   label,cc_w2b); 

    label = ui_xCreateLabelItem("Max. no. of candidate units: ",form,
				 30*ui_labelFontWidth,NULL,cc_w3b);
    sprintf(buf,"%d",cc_data.CAND.maxNoOfCandUnits);
    cc_w7b = ui_xCreateDialogItem ("cc_w7b",form,buf,6*ui_labelFontWidth,
				   label, cc_w3b); 

    label = ui_xCreateLabelItem("Activation function: ",form,
				  29*ui_labelFontWidth,NULL,cc_w7b);
    sprintf(buf,"%8s",cc_actFuncArray2[cc_data.CAND.actFunc]);
    cc_w10b = ui_xCreateMenuButtonItem (buf,form,label,cc_w7b);

    menu     = XtCreatePopupShell("menu",simpleMenuWidgetClass,cc_w10b,
				  NULL,ZERO);
    mItem    = XtCreateManagedWidget("Logistic", smeBSBObjectClass,
				     menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectActFuncCand,
		  ASYM_SIGMOID);
    mItem    = XtCreateManagedWidget("LogSym", smeBSBObjectClass,
				     menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectActFuncCand,
		  (caddr_t)SYM_SIGMOID);
    mItem    = XtCreateManagedWidget("TanH", smeBSBObjectClass,menu,
				     NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectActFuncCand,
		  (caddr_t)TANH);
    mItem    = XtCreateManagedWidget("Sinus", smeBSBObjectClass,menu,
				     NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectActFuncCand,
		  (caddr_t)SINUS);
    mItem    = XtCreateManagedWidget("Gauss", smeBSBObjectClass,menu,
				     NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectActFuncCand,
		  (caddr_t)EXPONENTIAL);
    mItem    = XtCreateManagedWidget("Random", smeBSBObjectClass,menu,
				     NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectActFuncCand,
		  (caddr_t)RANDOM);

}



/*****************************************************************************
  FUNCTION : cc_creatOutputPannel

  PURPOSE  : creates the output pannel, which is part of the CC-window
  NOTES    :

  UPDATE   : 5.10.92
******************************************************************************/
static void cc_createOutputPannel (Widget parent)
{
    Widget       form, label, top_label;
    Arg          arg[25];
    Cardinal     n;
    char         buf[10];
  
    n = 0;  
    form = XtCreateManagedWidget ("pannel", formWidgetClass, parent, arg, n);

    top_label  = ui_xCreateLabelItem("         Parameters for Output Units",
				     form, 35*ui_labelFontWidth, NULL, NULL);

    label = ui_xCreateLabelItem("Error change: ",form,
				 30*ui_labelFontWidth, NULL, top_label);
    sprintf(buf,"%.3f",cc_data.OUT.errorChange);
    cc_w4b = ui_xCreateDialogItem ("cc_w4b",form,buf,
				   6*ui_labelFontWidth,label, top_label); 

    label = ui_xCreateLabelItem("Output patience: ",form,
				 30*ui_labelFontWidth,NULL,cc_w4b);
    sprintf(buf,"%d",cc_data.OUT.outputPatience);
    cc_w5b = ui_xCreateDialogItem ("cc_w5b",form,buf,
				   6*ui_labelFontWidth, label, cc_w4b); 

    label = ui_xCreateLabelItem("Max. no. of epochs: ",form,
				 30*ui_labelFontWidth,NULL,cc_w5b);
    sprintf(buf,"%d",cc_data.OUT.maxNoOfUpdateCycles);
    cc_w6b = ui_xCreateDialogItem ("cc_w6b",form,buf,
				   6*ui_labelFontWidth, label, cc_w5b); 

}

/*****************************************************************************
  FUNCTION : cc_createGlobalPannel

  PURPOSE  : creates the global pannel, which is part of the CC-window
  NOTES    :

  UPDATE   : 5.10.92
******************************************************************************/
static void cc_createGlobalPannel (Widget parent)
{
    Widget       form,menu,mItem,label,top_label;
    Arg          arg[25];
    Cardinal     n;
    char         buf[30];
    int          length1;
  
    n = 0;  
    form = XtCreateManagedWidget("pannel", formWidgetClass, parent, arg, n);

    top_label  = ui_xCreateLabelItem("     General Parameters for Cascade",
				     form, 35*ui_labelFontWidth, NULL, NULL);

    label = ui_xCreateLabelItem("Max. output unit error:  ",form,
				 26*ui_labelFontWidth,NULL,top_label);
    sprintf(buf,"%.3f",cc_data.GLOBAL.pixelError);
    cc_w9b = ui_xCreateDialogItem ("cc_w9b",form,buf,10*ui_labelFontWidth,
				   label, top_label);

    label = ui_xCreateLabelItem("Learning function: ",form,26*ui_labelFontWidth,
				  NULL,cc_w9b);
    sprintf(buf,"%11s",cc_learningFuncArray[cc_data.GLOBAL.learningFunc]);
    cc_w11b  = ui_xCreateMenuButtonItem(buf,form,label,cc_w9b);

    menu     = XtCreatePopupShell("menu",simpleMenuWidgetClass,cc_w11b,
				  NULL,ZERO);

    mItem    = XtCreateManagedWidget("Batch-Backprop", smeBSBObjectClass,
				     menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectLearningFunc,
		  (caddr_t)BACKPROP); 
    mItem    = XtCreateManagedWidget("Std_Backpropagation", smeBSBObjectClass,
				     menu, NULL, ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectLearningFunc,
		  (caddr_t)BACKPROP_ONLINE);
    mItem    = XtCreateManagedWidget("Quickprop", smeBSBObjectClass,menu,
				     NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectLearningFunc,
		  (caddr_t)QUICKPROP);
    mItem    = XtCreateManagedWidget("Rprop", smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectLearningFunc,
		  (caddr_t)RPROP);

    label  = ui_xCreateLabelItem("Modification: ",form,26*ui_labelFontWidth,
				   NULL,cc_w11b);
    length1 = strlen(cc_ModificationArray[cc_data.GLOBAL.modification]);
    sprintf(buf,"%-11s",cc_ModificationArray[cc_data.GLOBAL.modification]);
    cc_centerString(buf,length1,9); 
    cc_w20b  = ui_xCreateMenuButtonItem(buf,form,label,cc_w11b);

    menu    =  XtCreatePopupShell("menu",simpleMenuWidgetClass,cc_w20b,
				  NULL,ZERO);
    mItem   =  XtCreateManagedWidget("none",smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectModification,
		  (caddr_t)CC_NO_MOD);
    mItem   =  XtCreateManagedWidget("Sibling/Descendant CC",smeBSBObjectClass,
				     menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectModification,
		  (caddr_t)CC_SDCC);
    mItem   =  XtCreateManagedWidget("Limited Fan-In Random Wired CC",
				     smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectModification,
		  (caddr_t)CC_LFCC);
    mItem   =  XtCreateManagedWidget("Random Layer CC",smeBSBObjectClass,
				     menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectModification,
		  (caddr_t)CC_RLCC);
    mItem   =  XtCreateManagedWidget("Static",smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectModification,
		  (caddr_t)CC_STAT);
    mItem   =  XtCreateManagedWidget("Exponential CC",smeBSBObjectClass,menu,
				     NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectModification,
		  (caddr_t)CC_ECC);
    mItem   =  XtCreateManagedWidget("Grouped CC",smeBSBObjectClass,menu,
				     NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectModification,
		  (caddr_t)CC_GCC);

    /* creating toggle widget for printing information to the shell */
    label = ui_xCreateLabelItem("Print covariance and error ?",form,
				  26*ui_labelFontWidth,NULL,cc_w20b);
    cc_covB[1] = ui_xCreateToggleItem("yes", form, NULL, label, cc_w20b);
    ui_xSetToggleState (cc_covB[1], cc_data.GLOBAL.onOff);
    XtAddCallback (cc_covB[1], XtNcallback, (XtCallbackProc)cc_selectOnOff,
                   (caddr_t) ON);
    cc_covB[0] = ui_xCreateToggleItem("no", form, NULL, cc_covB[1], cc_w20b);
    ui_xSetToggleState(cc_covB[0], !cc_data.GLOBAL.onOff);
    XtAddCallback(cc_covB[0], XtNcallback, (XtCallbackProc)cc_selectOnOff,
                  (caddr_t) OFF);

    /* creating toggle widget for caching activations */
    label = ui_xCreateLabelItem("Cache the unit activations ?", form,
				  26*ui_labelFontWidth, NULL, cc_covB[1]);
    cc_cacheB[1] = ui_xCreateToggleItem("yes", form, NULL, label, cc_covB[1]);
    ui_xSetToggleState (cc_cacheB[1], cc_data.GLOBAL.fastmode);
    XtAddCallback (cc_cacheB[1], XtNcallback, (XtCallbackProc)cc_selectOnOff,
                   (caddr_t) ON);
    cc_cacheB[0] = ui_xCreateToggleItem("no", form, NULL, 
					cc_cacheB[1], cc_covB[1]);
    ui_xSetToggleState(cc_cacheB[0], !cc_data.GLOBAL.fastmode);
    XtAddCallback(cc_cacheB[0], XtNcallback, (XtCallbackProc)cc_selectOnOff,
                  (caddr_t) OFF);

    /* creating toggle widget for new hidden pruning */
    label = ui_xCreateLabelItem("Prune new hidden unit ?", form,
				  26*ui_labelFontWidth, NULL, cc_cacheB[1]);
    cc_pruneB[1] = ui_xCreateToggleItem("yes", form, NULL, label, cc_cacheB[1]);
    ui_xSetToggleState (cc_pruneB[1], cc_data.GLOBAL.pruneOnOff);
    XtAddCallback (cc_pruneB[1], XtNcallback, (XtCallbackProc)cc_selectOnOff,
                   (caddr_t) ON);
    cc_pruneB[0] = ui_xCreateToggleItem("no", form, NULL, 
					cc_pruneB[1], cc_cacheB[1]);
    ui_xSetToggleState(cc_pruneB[0], !cc_data.GLOBAL.pruneOnOff);
    XtAddCallback(cc_pruneB[0], XtNcallback, (XtCallbackProc)cc_selectOnOff,
                  (caddr_t) OFF);

    label = ui_xCreateLabelItem("Minimize: ",form,26*ui_labelFontWidth,NULL,
				 cc_pruneB[1]);
    length1 = strlen(cc_pruningFuncArray[cc_data.GLOBAL.pruneFunc]);
    sprintf(buf,"%-11s",cc_pruningFuncArray[cc_data.GLOBAL.pruneFunc]);
    cc_centerString(buf,length1,9); 
    cc_pfb  = ui_xCreateMenuButtonItem(buf,form,label,cc_pruneB[1]);

    menu     = XtCreatePopupShell("menu",simpleMenuWidgetClass,cc_pfb,
				  NULL,ZERO);

    mItem= XtCreateManagedWidget("   SBC   ", smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectPruneFunc,
		  (caddr_t)SBC);
    mItem= XtCreateManagedWidget("   AIC   ", smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectPruneFunc,
		  (caddr_t)AIC);
    mItem= XtCreateManagedWidget("  CMSEP  ", smeBSBObjectClass,menu,NULL,ZERO);
    XtAddCallback(mItem,XtNcallback,(XtCallbackProc)cc_selectPruneFunc,
		  (caddr_t)CMSEP);

    label = ui_xCreateLabelItem("Additional Parameters ",form,
				24*ui_labelFontWidth, NULL, cc_pfb);
    sprintf(buf,"%.3f",cc_data.GLOBAL.addParameters[0]);
    cc_w21b =  ui_xCreateLabelItem("",form,
				   1*ui_labelFontWidth, NULL, label);
    cc_w21c = ui_xCreateDialogItem ("cc_w21c",form,buf,6*ui_labelFontWidth,
				    cc_w21b, label);  
    sprintf(buf,"%.3f",cc_data.GLOBAL.addParameters[1]);
    cc_w21d = ui_xCreateDialogItem ("cc_w21d",form,buf,6*ui_labelFontWidth,
				    cc_w21c, label);
    sprintf(buf,"%.3f",cc_data.GLOBAL.addParameters[2]);
    cc_w21e = ui_xCreateDialogItem ("cc_w21e",form,buf,6*ui_labelFontWidth,
				    cc_w21d, label);
    sprintf(buf,"%.3f",cc_data.GLOBAL.addParameters[3]);
    cc_w21f = ui_xCreateDialogItem ("cc_w21f",form,buf,6*ui_labelFontWidth,
				    cc_w21e, label);
    sprintf(buf,"%.3f",cc_data.GLOBAL.addParameters[4]);
    cc_w21g = ui_xCreateDialogItem ("cc_w21g",form,buf,6*ui_labelFontWidth,
				    cc_w21f, label);  
}


/*****************************************************************************
  FUNCTION : cc_createWindow

  PURPOSE  : creates the CC-window
  NOTES    :

  UPDATE   : 5.10.92
******************************************************************************/
void cc_createWindow (void)
{
    Widget       done,box;
    Arg          arg[25];
    Cardinal     n;
    char         buf[40];

    if(!cc_open) {
	cc_open = 1;
	sprintf (buf, "SNNS Cascade");
	n = 0;  
	cc_mainWidget = XtCreatePopupShell(buf,topLevelShellWidgetClass,
					   ui_toplevel,arg,n); 

	n = 0;  
	box = XtCreateManagedWidget("box",boxWidgetClass,cc_mainWidget,arg,n);

	cc_createGlobalPannel(box);
	cc_createCandidatePannel (box); 
	cc_createOutputPannel(box);

	done = ui_xCreateButtonItem ("done",box,NULL,NULL);
	XtAddCallback(done,XtNcallback,(XtCallbackProc)cc_doneProc,NULL); 

	XtAddEventHandler(box,KeyPressMask,FALSE,
			  (XtEventHandler)ui_key_control,(Cardinal *) 0);
	ui_checkWindowPosition(cc_mainWidget);
	XtPopup (cc_mainWidget, XtGrabNone);
	ui_xDontResizeWidget(cc_mainWidget); 
    } else {
	XRaiseWindow (XtDisplay (cc_mainWidget), XtWindow (cc_mainWidget));
    }
}  


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/pr_main.h,v $
  SHORTNAME      : pr_main.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tobias Schreiner
  DATE           : 13.3.94

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/05/20 15:00:34 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#ifndef _PR_MAIN_DEFINED_
#define  _PR_MAIN_DEFINED_

/* begin global definition section */

extern Widget pr_learnFuncText, pr_prunFuncText;

extern void pr_createWindow (void);
extern void pr_readElements (void);

extern int pr_open;

/* end global definition section */

#endif /* _PR_MAIN_DEFINED_ */

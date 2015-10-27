/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/net_analyzer.h,v $
  SHORTNAME      : net_analyzer.c
  SNNS VERSION   : 4.2

  PURPOSE        : header file for corresponding '.c' file
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $ 
  LAST CHANGE    : $Date: 1998/02/25 15:20:55 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _NET_ANALYZER_DEFINED_
#define _NET_ANALYZER_DEFINED_


extern void NA_OpenNetworkAnalyzer   (Widget w, XtPointer client_data,
                                                XtPointer call_data) ;
extern Bool NA_NetworkAnalyzerIsOpen (void) ;
extern void NA_DrawNextPoint         (void) ;
extern Bool NA_ContinueTest          (void) ;
extern void NA_StopTest              (void) ;


#endif


/*****************************************************************************
                           E N D   O F   F I L E
******************************************************************************/



/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_disp.h,v $
  SHORTNAME      : disp.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_disp.c
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991


  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3_DISP_DEFINED_
#define  _D3_DISP_DEFINED_


extern void d3_createDisplayWindow (void);
extern Widget d3_displayMainWidget;
extern void d3_eventProc (Widget w, Display *display, XEvent *event);

#endif 
             
/* end of file */
/* lines: */

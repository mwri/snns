/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_pannels.ph,v $
  SHORTNAME      : pannels.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_pannels.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_PANNELS_DEFINED_
#define _D3_PANNELS_DEFINED_


/* begin global definition section */


void d3_displayGraphic (void);  /* 3D entry point */


/* end global definition section */


/* begin private definition section */


static void d3_doneProc (void);
static void d3_toggleFreezeButton (Widget w, Widget button, caddr_t call_data);
static void d3_createButtonPannel (Widget parent);
static void d3_createControlPannel (Widget parent);

static Widget d3_controlWidget;


/* end private definition section */


#endif /* _D3_PANNELS_DEFINED_ */


/* end of file */
/* lines: 25 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_zValue.ph,v $
  SHORTNAME      : zValue.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_zValue.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:51 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_ZVALUE_DEFINED_
#define _D3_ZVALUE_DEFINED_


/* begin global definition section */


void d3_createZvaluePannel (Widget parent);


extern d3_state_type d3_state;
extern short ui_actualZvalue;
extern XtAppContext ui_appContext;

extern int d3_shortNumberWidth;


/* end global definition section */


/* begin private definition section */


static void d3_readZvalue (Widget w, Widget pannel, caddr_t call_data);
static void d3_incZvalue (Widget w, Widget pannel, caddr_t call_data);
static void d3_decZvalue (Widget w, Widget pannel, caddr_t call_data);

static Widget z_valueWidget;


/* end private definition section */


#endif /* _D3_ZVALUE_DEFINED_ */


/* end of file */
/* lines: 23 */

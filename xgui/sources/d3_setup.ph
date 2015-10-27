/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_setup.ph,v $
  SHORTNAME      : setup.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_setup.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_SETUP_DEFINED_
#define _D3_SETUP_DEFINED_


/* begin global definition section */


void d3_createSetupPannel (Widget w, Widget button, caddr_t call_data);


/* end global definition section */


/* begin private definition section */


static int d3_get_float_input (Widget w, float *f);
static int d3_get_float_rad_input (Widget w, float *f);
static void d3_closeSetupPannel (Widget w, Widget pannel, caddr_t call_data);


extern int d3_numberWidth;
extern d3_state_type d3_state;

static Widget rotXinitWidget, rotYinitWidget, rotZinitWidget;
static Widget rotXstepWidget, rotYstepWidget, rotZstepWidget;
static Widget transXinitWidget, transYinitWidget, transZinitWidget;
static Widget transXstepWidget, transYstepWidget, transZstepWidget;
static Widget scaleInitWidget, scaleStepWidget, aspectWidget;
static Widget linkWidget, posTriggerWidget, negTriggerWidget;


/* end private definition section */


#endif /* _D3_SETUP_DEFINED_ */


/* end of file */
/* lines: 25 */

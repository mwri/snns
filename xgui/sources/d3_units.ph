/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_units.ph,v $
  SHORTNAME      : units.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_units.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_UNITS_DEFINED_
#define _D3_UNITS_DEFINED_


/* begin global definition section */


void d3_createUnitPannel (Widget w, Widget button, caddr_t call_data);


extern d3_state_type d3_state;


/* end global definition section */


/* begin private definition section */


static void d3_closeUnitPannel (Widget w, Widget pannel, caddr_t call_data);
static void d3_setUnitMode (Widget w, int mode, caddr_t call_data);


static Widget activation, initact, output, bias, name, number, zvalue, nothing;
static Widget size, color, toplabel, bottlabel;

static int selected_unit_mode;
static int toggled_unit_mode;

static d3_unit_mode_type temp_unit_mode;


/* end private definition section */


#endif /* _D3_UNITS_DEFINED_ */


/* end of file */
/* lines: 24 */

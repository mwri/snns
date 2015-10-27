/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_model.ph,v $
  SHORTNAME      : model.ph
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:27 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _D3_MODEL_DEFINED_
#define _D3_MODEL_DEFINED_


/* begin global definition section */


void d3_createModelPannel (Widget w, Widget button, caddr_t call_data);


/* end global definition section */


/* begin private definition section */


static void setModelToggleState (void);
static void d3_closeModelPannel (Widget w, Widget pannel, caddr_t call_data);
static void setModelWire (void);
static void setModelSolid (void);


static Widget solidButton, wireButton;

static d3_state_type temp_state;


/* end private definition section */


#endif /* _D3_MODEL_DEFINED_ */


/* end of file */
/* lines: 16 */

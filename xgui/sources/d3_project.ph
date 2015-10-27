/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_project.ph,v $
  SHORTNAME      : project.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_project.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:38 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_PROJECT_DEFINED_
#define _D3_PROJECT_DEFINED_


/* begin global definition section */


void d3_createProjectPannel (Widget w, Widget button, caddr_t call_data);


/* end global definition section */


/* begin private definition section */


static void setCentralProjection (void);
static void d3_closeProjectPannel (Widget w, Widget pannel, caddr_t call_data);
static void setProjectToggleState (void);
static void setParallelProjection (void);

static Widget parallelButton, centralButton;
static Widget viewpointXwidget, viewpointYwidget, viewpointZwidget;

static d3_state_type temp_state;


/* end private definition section */


#endif /* _D3_PROJECT_DEFINED_ */


/* end of file */
/* lines: 23 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_setup.ph,v $
  SHORTNAME      : setup.ph
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.5.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_SETUP_DEFINED_
#define _UI_SETUP_DEFINED_


/* begin global definition section */


void ui_set_initData (struct Ui_DisplayType *);
void ui_xCreateSetupPanel (Widget, struct Ui_DisplayType *);

FlintType  ui_maxAct    =  2.0; /* this are initial values */
FlintType  ui_maxWeight =  10.0;
FlintType  ui_minWeight = -10.0;
Widget ui_showBottomLabel;
Widget ui_showTopLabel;


/* end global definition section */


/* begin private definition section */


	/* shell widget of setup panel */
/* static Widget ui_setupPanel; */
	/* widgets showing the current configuration */
/* static Widget ui_set_gridWidthWidget; */
/* static Widget ui_set_originXWidget, ui_set_originYWidget; */
/* static Widget ui_set_subnetWidget, ui_set_zWidget; */

/* static Bool   ui_setupIsCreated = FALSE; */ /* yes or no */


/* static short ui_actualZvalue = 0; */

#ifdef _UNUSED_FUNCTIONS_
static FlintType  ui_minAct    = -2.0;
#endif /* _UNUSED_FUNCTIONS_ */


/* end private definition section */


#endif /* _UI_SETUP_DEFINED_ */


/* end of file */
/* lines: 30 */

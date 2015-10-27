/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_setup.h,v $
  SHORTNAME      : setup.h
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.5.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:39 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/

#ifndef _UI_SETUP_DEFINED_
#define _UI_SETUP_DEFINED_

extern void ui_set_initData (struct Ui_DisplayType *);
extern void ui_xCreateSetupPanel (Widget, struct Ui_DisplayType *);

extern FlintType  ui_maxWeight;
extern FlintType  ui_minWeight;
extern Widget ui_showBottomLabel;
extern Widget ui_showTopLabel;
extern FlintType  ui_maxAct;

#endif /* _UI_SETUP_DEFINED_ */


/* end of file */
/* lines: 35 */

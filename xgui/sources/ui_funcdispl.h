/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_funcdispl.h,v $
  SHORTNAME      : funcdispl
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Guenter Mamier 
  DATE           : 14.01.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:51 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _UI_FUNCDISPL_DEFINED_
#define _UI_FUNCDISPL_DEFINED_



/* required functions that are declared here */
extern void                ui_functionPopup(Widget w, char *title, caddr_t call_data);
extern void                ui_drawFunction(Widget w, char *title, caddr_t call_data);
extern Widget              ui_createMyScreen(char *name, Widget parent, Dimension width, Dimension height, Widget left, Widget top);

#endif


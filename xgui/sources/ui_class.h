/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_class.h,v $
  SHORTNAME      : ui_class.h
  SNNS VERSION   : 4.2

  PURPOSE        : header file for corresponding '.c' file
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 09.01.1998

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.7 $ 
  LAST CHANGE    : $Date: 1998/02/25 15:21:09 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _UI_CLASS_DEFINED_
#define _UI_CLASS_DEFINED_

/* begin global definition section */

extern void ui_classWindow(void);
extern int class_readWidgets(void);
extern void ui_newClass(Widget parent, char *oldclass);
extern void ui_classUpdatePanel(bool force);

extern unsigned int  ui_classDist[];
extern int  ui_class_open;
extern int  ui_classes;

/* end global definition section */

#endif 

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_class.ph,v $
  SHORTNAME      : ui_class.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header file for corresponding '.c' file
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 08.01.98

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:10 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _UI_CLASS_DEFINED_
#define _UI_CLASS_DEFINED_


/* begin global definition section */
void ui_classWindow(void);
int class_readWidgets(void);
void ui_newClass(Widget parent, char *oldclass);
void ui_classUpdatePanel(bool force);

unsigned int  *ui_classDist;
int  ui_classes = 0;
int  ui_class_open = 0;

/* end global definition section */

/* begin private definition section */

unsigned int  *ui_old_classDist;

static Widget   classWidget, class_popupWidget;
static Widget   *class_LabelW, class_newLab,
                *class_distW, class_leftW0,
                class_leftW1, class_leftW2,
                class_defW, class_lastW,
                class_box, class_onOffW[2];

static void class_doneProc(Widget w, Widget kill_it, caddr_t call_data);
static void class_onOffProc(Widget w, int value, caddr_t call_data);
static void class_defProc(Widget w, int value, caddr_t call_data);
static void create_classBody(pattern_set_info patt_info);

/* end private definition section */

#endif










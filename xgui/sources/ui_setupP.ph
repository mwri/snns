/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_setupP.ph,v $
  SHORTNAME      : setupP.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :


  AUTHOR         : Tilman Sommer
  DATE           : 27.3.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:42 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_SETUPP_DEFINED_
#define _UI_SETUPP_DEFINED_


/* begin global definition section */


void  ui_set_showProc(Widget w, int value, caddr_t call_data);
void  ui_set_getSetupData(Widget widget, int toggleNumber, caddr_t call_data);
void  ui_thumbed(Widget widget, int trigger, XtPointer top_ptr);
void  ui_set_assignLayers(Widget w, caddr_t client_data, caddr_t call_data);
void  ui_set_colorProc(Widget w, int value, caddr_t call_data);
void ui_scrolled(Widget widget, caddr_t label_ptr, XtPointer value_ptr);


/* end global definition section */


/* begin private definition section */




/* end private definition section */


#endif /* _UI_SETUPP_DEFINED_ */


/* end of file */
/* lines: 36 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_move.ph,v $
  SHORTNAME      : move.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_move.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:30 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_MOVE_DEFINED_
#define _D3_MOVE_DEFINED_


/* begin global definition section */


void d3_createMovePannel (Widget parent);


/* end global definition section */


/* begin private definition section */


static void d3_transform  (Widget w, int transformation, caddr_t call_data);
static void create_box_item (int height, int width, Widget parent, Widget left, Widget top, Widget *box);
static void create_scale_button (char *label, Widget parent, int xsize, int ysize, Widget *button);
static void create_move_label (char *label, Widget parent);
static void create_move_item (char *label, Widget parent, Widget *le_widget, Widget *ge_widget);
static void create_rotatePannel (Widget rotate_widget);
static void create_transPannel (Widget trans_widget);
static void create_scalePannel (Widget scale_widget);


static int border = 10;
static int button_size = 16;


/* end private definition section */


#endif /* _D3_MOVE_DEFINED_ */


/* end of file */
/* lines: 24 */

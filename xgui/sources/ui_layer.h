/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_layer.h,v $
  SHORTNAME      : layer.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 31.8.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:07 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_LAYER_DEFINED_
#define _UI_LAYER_DEFINED_




extern void   ui_xCreateLayerPanel(Widget parent);


/* widgets of input fields and buttons */
extern Widget    ui_layerNameWidgets[];
extern Widget    ui_layerButtonWidgets[];
/* the value indicating the initial state of the buttons */
extern unsigned short ui_layerStartValue;


#endif /* _UI_LAYER_DEFINED_ */


/* end of file */
/* lines: 40 */


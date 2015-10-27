/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_layer.ph,v $
  SHORTNAME      : layer.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 31.8.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:08 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_LAYER_DEFINED_
#define _UI_LAYER_DEFINED_


/* begin global definition section */


void   ui_xCreateLayerPanel(Widget parent);


/* widgets of input fields and buttons */
Widget    ui_layerNameWidgets[MAX_NO_LAYERS];
Widget    ui_layerButtonWidgets[MAX_NO_LAYERS];
/* the value indicating the initial state of the buttons */
unsigned short ui_layerStartValue = 0;


/* end global definition section */


/* begin private definition section */


static Widget    ui_layerPanel;  /* shell widget of layer panel */


/* end private definition section */


#endif /* _UI_LAYER_DEFINED_ */


/* end of file */
/* lines: 30 */

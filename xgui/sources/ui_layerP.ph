/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_layerP.ph,v $
  SHORTNAME      : layerP.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:11 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_LAYERP_DEFINED_
#define _UI_LAYERP_DEFINED_


/* begin global definition section */


void ui_layer_initLayerNames(void);
void ui_layer_setLayerNames(void);


/* array of layer names */
char      ui_layerNames[MAX_NO_LAYERS][MAX_NAME_LENGTH];


/* end global definition section */


/* begin private definition section */


/* end private definition section */


#endif /* _UI_LAYERP_DEFINED_ */


/* end of file */
/* lines: 28 */

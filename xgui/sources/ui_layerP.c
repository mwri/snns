/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_layerP.c,v $
  SHORTNAME      : layerP.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 31.8.1990
  AUTHOR         :
  DATE           :

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:34 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>

#include "ui.h"
#include "ui_utilP.h"
#include "ui_xWidgets.h"
#include "ui_layer.h"
#include "ui_action.h"

#include "ui_layerP.ph"


/*****************************************************************************
  FUNCTION : ui_layer_initLayerNames

  PURPOSE  : assign initial layer names
  RETURNS  : void
  NOTES    :

  UPDATE   : 31.8.1990
******************************************************************************/

void ui_layer_initLayerNames(void)

{
    int i;

    for (i=0; i<MAX_NO_LAYERS; i++) {
	sprintf(&ui_layerNames[i][0],"LAYER %2d",i+1);
    }
}


/*****************************************************************************
  FUNCTION : ui_layer_setLayerNames

  PURPOSE  : copy data from widgets to the name array
  RETURNS  : void
  NOTES    :

  UPDATE   : 31.8.1990
******************************************************************************/

void ui_layer_setLayerNames(void)

{
    int   i;
    int   flagWord = 0;
    int   bit = 1;
    
    for (i=0; i<MAX_NO_LAYERS; i++) {
	ui_xStringFromAsciiWidget(ui_layerNameWidgets[i], 
				  &ui_layerNames[i][0], MAX_NAME_LENGTH);
	if (ui_xGetToggleState(ui_layerButtonWidgets[i]))
	    ui_utilSetFlag(flagWord, bit);
	bit *= 2;
    }
    ui_layerReturnValue = (unsigned short) flagWord;
}    










/* end of file */
/* lines: 89 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_layer.c,v $
  SHORTNAME      : layer.c
  SNNS VERSION   : 4.2

  PURPOSE        : popups a window with all layers. It is possible to choose layers
                   and to alter layer names as well.
  NOTES          : is called only during initialisation

  AUTHOR         : Tilman Sommer
  DATE           : 31.8.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:33 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>

#include "ui.h"

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Cardinals.h>

#include "ui_xWidgets.h"

#include "ui_layerP.h"

#include "ui_layer.ph"



/*****************************************************************************
  FUNCTION : ui_xCreateLayerPanel

  PURPOSE  : create the panel
  RETURNS  : void
  NOTES    : the wigdet will be created in relative position to eachother

  UPDATE   :
*****************************************************************************/

void ui_xCreateLayerPanel (Widget parent)

{

    /* tabs for data fields and small buttons */

    Widget      oldLabel, nameLabel, nrLabel, label;

    int    i;
    char   buf[80];
    int    flagWord = ui_layerStartValue;
    int    bit = 1;
    
    int  fontWidth = 8;

    int  labelWidth  =  9 * fontWidth;
    int  nameWidth   = 25 * fontWidth;  



/***************************************************************************/

    ui_layerPanel = 
	XtCreateManagedWidget("lPanel", formWidgetClass, parent, NULL, ZERO);


/*****************************  HEAD LINE  *********************************/

    nrLabel =
	ui_xCreateLabelItem("layer", ui_layerPanel, labelWidth,  
			    NULL, NULL);
    ui_xSetResize(nrLabel, TRUE);

    label = nrLabel;

    nameLabel =
	ui_xCreateLabelItem("name", ui_layerPanel, nameWidth,  
			    nrLabel, NULL);

/*****************************  LINES  *************************************/

    for (i=0; i<MAX_NO_LAYERS; i++) {

	oldLabel = label;
	sprintf(buf,"%2d",i+1);
	label = 
	    ui_xCreateLabelItem(buf, ui_layerPanel, labelWidth, NULL, label); 
	ui_xSetResize(label, TRUE);

	ui_layerNameWidgets[i] =
	    ui_xCreateDialogItem("layerName", ui_layerPanel, 
				 &ui_layerNames[i][0], nameWidth, 
				 nrLabel, oldLabel);

	ui_layerButtonWidgets[i] =
	    ui_xCreateToggleItem("on", ui_layerPanel, NULL, 
				 ui_layerNameWidgets[i], oldLabel);
	ui_xSetToggleState(ui_layerButtonWidgets[i], 
			   ui_utilIsSet(flagWord, bit));
	bit *= 2;
    }

/***************************************************************************/

    XawFormDoLayout(ui_layerPanel, True);
}









/* end of file */
/* lines: 132 */

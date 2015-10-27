/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_infoP.c,v $
  SHORTNAME      : infoP.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/07/08 13:22:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>

#include <stdio.h>

#include "ui.h"
#include "ui_netGraph.h"

#include "kr_ui.h"

#include <X11/Xaw/List.h>
#include "ui_utilP.h"
#include "ui_xWidgets.h"
#include "ui_textP.h"
#include "ui_controlP.h"
#include "ui_mainP.h"
#include "ui_lists.h"
#include "ui_display.h"
#include "ui_confirmer.h"
#include "ui_netUpdate.h"
#include "ui_selection.h"
#include "ui_action.h"
#include "ui_info.h"
#include "ui_layer.h"


#include "ui_infoP.ph"


/*****************************************************************************
  FUNCTION : ui_info_initLinkStruct

  PURPOSE  : init link attributs 
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

static void ui_info_initLinkStruct(void)

{
    ui_link.weight          = 0.0;
    ui_link.siteValue       = 0.0;
    ui_link.siteName[0]     = '\0';
    ui_link.siteFuncName[0] = '\0';
}


/*****************************************************************************
  FUNCTION : ui_info_initUnitStruct

  PURPOSE  : init attributs specified unit (target or source)
  RERURNS  : struct UnitAttributeType  *
  NOTES    :

  UPDATE   : 15.8.1990
******************************************************************************/

void ui_info_initUnitStruct(struct UnitAttributeType *unit)

{
    (*unit).no       = 0;
    (*unit).subnetNo = 0; /* main net */
    (*unit).layers   = 1; /* first layer only */
    (*unit).frozen   = FALSE;
    (*unit).iotype   = HIDDEN;
    (*unit).act      = 0.0;
    (*unit).i_act    = 0.0;
    (*unit).out      = 0.0;
    (*unit).bias     = 0.0;
}


/*****************************************************************************
  FUNCTION : ui_info_setLinkItems

  PURPOSE  : display data in the panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

static void ui_info_setLinkItems(void)

{
    char buf[20];

    if( ui_infoIsCreated == FALSE)
	return;

    sprintf(buf,"%9.5f",ui_link.weight);
    ui_cutTrailingZeros(buf);
    ui_xSetString(ui_linkWidgets.weight, buf);
    
    if (strlen(ui_link.siteName)) {
	sprintf(buf,"%9.5f",ui_link.siteValue);
	ui_cutTrailingZeros(buf);
	ui_xSetLabel(ui_linkWidgets.siteValue, buf);
	ui_xSetLabel(ui_linkWidgets.siteName, ui_link.siteName);
	ui_xSetLabel(ui_linkWidgets.siteFuncName, ui_link.siteFuncName);
    } else {
	ui_xSetLabel(ui_linkWidgets.siteValue, "");
	ui_xSetLabel(ui_linkWidgets.siteName, "");
	ui_xSetLabel(ui_linkWidgets.siteFuncName, "");
    }
}


/*****************************************************************************
  FUNCTION : ui_info_setUnitItems

  PURPOSE  : display values in the unit-info-panel from the network data 
             structure via the kernel
  RETURNS  : alteration of unit-info-panel
  NOTES    :

  UPDATES  :
*****************************************************************************/

void ui_info_setUnitItems(struct UnitWidgetType unitWidgets, 
     struct UnitAttributeType  unit)

{
    char         buf[MAX_NAME_LENGTH];

    /* display unit number */
    if (krui_setCurrentUnit(unit.no) != UI_SET_OK  /* error: unit not valid */)
	unit.no = 0;

    if (ui_infoIsCreated) {

	ui_xSetToggleState(unitWidgets.frozen, unit.frozen);

	sprintf(buf,"%d", unit.no);
	ui_xSetLabel(unitWidgets.no, buf);

	sprintf(buf,"%d", unit.subnetNo);
	ui_xSetLabel(unitWidgets.subnetNo, buf);

	sprintf(buf,"%s", unit.outFuncName);
	ui_xSetLabel(unitWidgets.outFuncName, buf);

	sprintf(buf,"%s", unit.actFuncName);
	ui_xSetLabel(unitWidgets.actFuncName, buf);

	sprintf(buf,"%s", unit.ftype);
	ui_xSetLabel(unitWidgets.ftype, buf);

	sprintf(buf,"%s", unit.name);
	ui_xSetString(unitWidgets.name, buf); 

	switch (unit.iotype) {
	  case HIDDEN: 	
	    ui_xSetLabel(unitWidgets.iotype, "H");
	    break;
	  case INPUT: 	
	    ui_xSetLabel(unitWidgets.iotype, "I");
	    break;
	  case OUTPUT: 	
	    ui_xSetLabel(unitWidgets.iotype, "O");
	    break;
	  case DUAL: 	
	    ui_xSetLabel(unitWidgets.iotype, "D");
	    break;
	  case SPECIAL: 	
	    ui_xSetLabel(unitWidgets.iotype, "S");
	    break;
          case SPECIAL_I:
            ui_xSetLabel(unitWidgets.iotype, "SI");
            break;
          case SPECIAL_O:       
            ui_xSetLabel(unitWidgets.iotype, "SO");
            break;
          case SPECIAL_H:       
            ui_xSetLabel(unitWidgets.iotype, "SH");
            break;
          case SPECIAL_D:       
            ui_xSetLabel(unitWidgets.iotype, "SD");
            break;
	  case UNKNOWN: 	
	    ui_xSetLabel(unitWidgets.iotype, "-");
	    break;
	}	

	sprintf(buf,"%8.5f", unit.act);
	ui_cutTrailingZeros(buf);
	ui_xSetString(unitWidgets.act, buf); 

	sprintf(buf,"%8.5f", unit.i_act);
	ui_cutTrailingZeros(buf);
	ui_xSetString(unitWidgets.i_act, buf); 

	sprintf(buf,"%8.5f", unit.out);
	ui_cutTrailingZeros(buf);
	ui_xSetString(unitWidgets.out, buf); 
	
	sprintf(buf,"%8.5f", unit.bias);
	ui_cutTrailingZeros(buf);
	ui_xSetString(unitWidgets.bias, buf);
    }
}


/*****************************************************************************
  FUNCTION : ui_info_getAllSiteData

  PURPOSE  : get all link data, if a current site exists
  RETURNS  : void
  NOTES    : a current site must exist !!!!!

  UPDATE   : 20.9.1990
******************************************************************************/

static void ui_info_getAllSiteData(void)

{
    ui_link.siteValue = (FlintType) krui_getSiteValue();

    sprintf(ui_link.siteName,"%s",     krui_getSiteName());
    sprintf(ui_link.siteFuncName,"%s", krui_getSiteFuncName());
} 


/*****************************************************************************
  FUNCTION : ui_info_getAllUnitData

  PURPOSE  : get all unit attributes from the network structure via kernel
  RETURNS  : in a parameter (unit) all attributes
  NOTES    :

  UPDATES  :
*****************************************************************************/

static void ui_info_getAllUnitData(int unitNo, struct UnitAttributeType *unit)

{
    (*unit).no          = unitNo;
    (*unit).subnetNo    = krui_getUnitSubnetNo(unitNo);
    (*unit).frozen      = krui_isUnitFrozen(unitNo);
    (*unit).layers      = krui_getUnitLayerNo(unitNo);
    (*unit).iotype      = krui_getUnitTType(unitNo);
    (*unit).act         = krui_getUnitActivation(unitNo);
    (*unit).i_act       = krui_getUnitInitialActivation(unitNo);
    (*unit).out         = krui_getUnitOutput(unitNo);
    (*unit).bias        = krui_getUnitBias(unitNo);

    if (krui_getUnitName(unitNo) != NULL)
	sprintf((*unit).name       ,"%s", krui_getUnitName(unitNo));
    else
	sprintf((*unit).name       ,"");
    if (krui_getUnitFTypeName(unitNo) != NULL)
	sprintf((*unit).ftype      ,"%s", krui_getUnitFTypeName(unitNo));
    else
	sprintf((*unit).ftype       ,"");
    sprintf((*unit).outFuncName,"%s", krui_getUnitOutFuncName(unitNo));
    sprintf((*unit).actFuncName,"%s", krui_getUnitActFuncName(unitNo));

    krui_getUnitPosition(unitNo, &(*unit).gridPos);
}


/*****************************************************************************
  FUNCTION : ui_info_getDisplayedUnitAttributes 

  PURPOSE  : get the the displayed attributes: 
             Name, Act, I_Act, Out and Bias
  RETURNS  : void
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_getDisplayedUnitAttributes(struct UnitWidgetType *unitWidgets,
	struct UnitAttributeType  *unit)

{
    if (unitWidgets == &ui_sourceWidgets) 
	*unit = ui_sourceUnit;
    else
	*unit = ui_targetUnit;

    if (!ui_infoIsCreated) 
	return;

    ui_xStringFromAsciiWidget((*unitWidgets).name, (*unit).name, 
			      MAX_NAME_LENGTH);

    if (((*unit).name == NULL) OR (strlen(((*unit).name))<1))
	return;

    ui_correctName((*unit).name); /* replace white space */
    (*unit).act   = (float) ui_xFloatFromAsciiWidget((*unitWidgets).act);
    (*unit).i_act = (float) ui_xFloatFromAsciiWidget((*unitWidgets).i_act);
    (*unit).out   = (float) ui_xFloatFromAsciiWidget((*unitWidgets).out);
    (*unit).bias  = (float) ui_xFloatFromAsciiWidget((*unitWidgets).bias);

}


/*****************************************************************************
  FUNCTION : ui_info_storeAttributes

  PURPOSE  : stores all attributes from unitAttributes to unit unitNo.
  RETURNS  : void
  NOTES    : The unitNo is not set, because it can't be set!
             The layer and the subnet number will be set as well. This must
             be corrected by the caller if desired.
	     The GUI unit creation routine will do the following assignment:
	     layer    := currentLayer (only) in currentWindow
	     subNetNo := subNetNo of currentWindow

  UPDATES  : 
*****************************************************************************/

void ui_info_storeAttributes(int unitNo, struct UnitAttributeType unit)

{
    if (strlen(unit.name) OR (krui_getUnitName(unitNo) != NULL)) {
	ui_checkError(krui_setUnitName(unitNo, unit.name));
    }
    if (strlen(unit.ftype) OR (krui_getUnitFTypeName(unitNo) != NULL)) {
	ui_checkError(krui_setUnitFType(unitNo, unit.ftype));
    }
    if (unit.iotype == UNKNOWN)
	ui_checkError(krui_setUnitTType(unitNo, HIDDEN));
    else
	ui_checkError(krui_setUnitTType(unitNo, unit.iotype));

    
    krui_setUnitSubnetNo         (unitNo, unit.subnetNo);
    krui_setUnitLayerNo          (unitNo, (int) unit.layers);
    if (NOT (strcmp(unit.actFuncName, krui_getUnitActFuncName(unitNo)) == 0))
	krui_setUnitActFunc      (unitNo, unit.actFuncName);
    if (NOT (strcmp(unit.outFuncName, krui_getUnitOutFuncName(unitNo)) == 0))
	krui_setUnitOutFunc      (unitNo, unit.outFuncName);
    krui_setUnitActivation       (unitNo, unit.act);
    krui_setUnitInitialActivation(unitNo, unit.i_act);
    krui_setUnitOutput           (unitNo, unit.out);
    krui_setUnitBias             (unitNo, unit.bias);

    if (unit.frozen)
	krui_freezeUnit(unitNo);
    else
	krui_unfreezeUnit(unitNo);
}


/*****************************************************************************
  FUNCTION : ui_info_anyUnitSelected          

  PURPOSE  : test, whether the displayed source/target unit is valid.
  RETURNS  : true, if the displayed unit is ok.
  NOTES    :

  UPDATES  :
*****************************************************************************/

static Bool ui_info_anyUnitSelected(FlagType flag)

{
    Bool  selected;
    int   unitNo;
    char  buf[10], buf2[50];

    if (flag == UI_SOURCE) {
	sprintf(buf,"source");
	unitNo = ui_sourceUnit.no;
    } else {
	sprintf(buf,"target");
	unitNo = ui_targetUnit.no;
    }
    
    if (NOT (selected = (krui_setCurrentUnit(unitNo) == UI_SET_OK))) {
	sprintf(buf2,"No %s unit selected !",buf);
	ui_printMessage(buf2);
    }
    return(selected);
}


/*****************************************************************************
  FUNCTION : ui_info_displayList

  PURPOSE  : popup the list panel
  RETURNS  : void
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_displayList(Widget w, struct SimpleListType *listDescriptorPtr, 
	caddr_t call_data)

{
    Arg		args[5];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;

    /* Upper left corner will be in the center of the calling button */

    n = 0;
    XtSetArg(args[0], XtNwidth, &width); n++;
    XtSetArg(args[1], XtNheight, &height); n++;
    XtGetValues(listDescriptorPtr->widgetPtr, args, n);
    XtTranslateCoords(listDescriptorPtr->widgetPtr, 
		      (Position) (width / 2), (Position) (height / 2),
		      &x, &y);

    ui_listPopup(listDescriptorPtr,x,y); /* absolute screen coordinates */
}


/*****************************************************************************
  FUNCTION : ui_info_setDefault

  PURPOSE  : copies the value in ui_defUnit and sets kernel defaults
  RETURNS  : void
  NOTES    :

  UPDATE   : 27.9.90
******************************************************************************/

void ui_info_setDefault(Widget w, int structure, caddr_t call_data)

{
    if (structure == UI_SOURCE) 
	ui_info_getDisplayedUnitAttributes(&ui_sourceWidgets, &ui_defUnit);
    else 
	ui_info_getDisplayedUnitAttributes(&ui_targetWidgets, &ui_defUnit);
    

    ui_checkError(krui_setUnitDefaults(ui_defUnit.act, ui_defUnit.bias,
				       ui_defUnit.iotype, ui_defUnit.subnetNo, 
				       ui_defUnit.layers, 
				       ui_defUnit.actFuncName, 
				       ui_defUnit.outFuncName));
}


/*****************************************************************************
  FUNCTION : ui_info_freezeUnit

  PURPOSE  : toggles the value frozen in the specified unit-structure
  RETURNS  : void
  NOTES    :

  UPDATE   : 26.8.1990
******************************************************************************/

void ui_info_freezeUnit(Widget w, int structure, caddr_t call_data)

{
    switch (structure) {
      case UI_SOURCE: 
	ui_sourceUnit.frozen = NOT ui_sourceUnit.frozen;
	break;
      case UI_TARGET: 
	ui_targetUnit.frozen = NOT ui_targetUnit.frozen;
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_info_assignLayer

  PURPOSE  : popup the layer assignment window.
  RETURNS  : void
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_assignLayer(Widget w, struct InfoCallLayerStruct *callLayerPtr, 
	caddr_t call_data)

{
    struct UnitAttributeType  *unitPtr;
    Arg		args[5];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;

    /* Upper left corner will be in the center of the calling button */

    n = 0;
    XtSetArg(args[0], XtNwidth, &width); n++;
    XtSetArg(args[1], XtNheight, &height); n++;
    XtGetValues(callLayerPtr->widget, args, n);
    XtTranslateCoords(callLayerPtr->widget, 
		      (Position) (width / 2), (Position) (height / 2),
		      &x, &y);

    if (callLayerPtr->unitType == UI_TARGET)
	unitPtr = &ui_targetUnit;
    else
	unitPtr = &ui_sourceUnit;

    /* set start value for the buttons */
    ui_layerStartValue = unitPtr->layers;
    ui_displayLayerPanel(x,y); /* absolute screen coordinates */
    /* assign return value */
    unitPtr->layers = ui_layerReturnValue;
    ui_sel_checkList(ui_currentDisplay, UI_GLOBAL);
}


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_info_deleteSite

  PURPOSE  : popup the list panel and delete the selected site
  RETURNS  : void
  NOTES    : 

  UPDATES  :
*****************************************************************************/

static void ui_info_deleteSite(Widget w, 
	struct SimpleListType *listDescriptorPtr, caddr_t call_data)

{
    /* do some checks */
    if (NOT ui_info_anyUnitSelected(UI_TARGET)) 
	return;
    if (krui_getUnitInputType(listDescriptorPtr->unitPtr->no) != SITES)
	return;

    ui_info_displayList(w, listDescriptorPtr, call_data);

    /* the list appears */
    /* now consider the return value */
    if (ui_list_returnIndex == XAW_LIST_NONE)
	return; /* DONE pressed without selection */
	
    if (krui_setSite(ui_list_returnName) != UI_SET_OK) {
	ui_confirmOk("This site does not exist on the target unit!");
	return;
    }
    
    (void) krui_deleteSite();
}

#endif /* _UNUSED_FUNCTIONS_ */


/*****************************************************************************
  FUNCTION : ui_info_unitUpdateProc

  PURPOSE  : forces the kernal to update unit-data
  RETURNS  : alteration of the network
  NOTES    :

  UPDATES  :
*****************************************************************************/

void ui_info_unitUpdateProc(Widget w, FlagType flag, caddr_t call_data)

{
    struct UnitAttributeType  unit;
    char       buf[UI_MAX_TW_BUFFER_SIZE];

    if (ui_info_anyUnitSelected(flag)) {
	ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
	if (flag == UI_SOURCE) 
	    unit = ui_sourceUnit;
	else 
	    unit = ui_targetUnit;
	ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL,
		    unit.no, UI_ERASE);
	if (flag == UI_SOURCE) 
	    ui_info_getDisplayedUnitAttributes(&ui_sourceWidgets, &unit);
	else 
	    ui_info_getDisplayedUnitAttributes(&ui_targetWidgets, &unit);

	ui_info_storeAttributes(unit.no, unit);
    
	/* output messages and graphic */
    
	ui_tw_printMessage("SET unit: "); 
	sprintf(buf,"%d %s [st: %s, ft: %s]\n  act: %8.5f   iact: %8.5f\n  out: %8.5f   bias: %8.5f\n",
		unit.no, unit.name, ui_utilGetIOTypeName(unit.iotype), 
		unit.ftype, unit.act, unit.i_act, unit.out, unit.bias); 
	ui_tw_printMessage(buf); 
	sprintf(buf,"  act-func: %s\n  out-func: %s\n\n", 
		unit.actFuncName, unit.outFuncName);
	ui_tw_printMessage(buf); 

	ui_net_drawUnit(ui_currentDisplay, UI_GLOBAL, unit.no, UI_DRAW);
	ui_rem_resetCounter();
	ui_sel_reshowItems(ui_currentDisplay, UI_GLOBAL);
    } else {
	if (flag == UI_SOURCE) {
	    unit = ui_sourceUnit;
	    ui_info_getDisplayedUnitAttributes(&ui_sourceWidgets, &unit);
	} else {
	    unit = ui_targetUnit;
	    ui_info_getDisplayedUnitAttributes(&ui_targetWidgets, &unit);
	}
    }
}


/*****************************************************************************
  FUNCTION : ui_info_linkUpdateProc

  PURPOSE  : forces the kernal to update the connection weight of the dis-
             played link.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATES  :
*****************************************************************************/

void ui_info_linkUpdateProc(Widget w, XtPointer button, caddr_t call_data)

{  
    char                buf[UI_MAX_TW_BUFFER_SIZE];

    if (NOT ui_info_anyUnitSelected(UI_SOURCE)) return;

    if (krui_setCurrentUnit(ui_targetUnit.no) != UI_SET_OK) {
	ui_confirmOk("Can't update link. Invalid target!");
	return;
    }
    if (ui_sourceUnit.no == 0) {
	ui_confirmOk("Can't update link. Invalid source!");
	return;
    }
    if (strlen(ui_link.siteName) > 0) { /* the displayed link was on a site */
	if (krui_setSite(ui_link.siteName) != UI_SET_OK) {
	    ui_confirmOk("Can't update link. Invalid site!");
	    return;
	}
    } else {
	if (krui_getUnitInputType(ui_targetUnit.no) == SITES) {
	    /* no site specified => DIRECT_LINK */
	    /* But a unit can't have sites AND direct links ! */
	    ui_confirmOk("Can't update link. Invalid site name!");
	    return;
	}	
    }    

    if (NOT krui_isConnected(ui_sourceUnit.no)) {
	/* no connection from source to current site of target */
	ui_confirmOk("Can't update link. No connection exists.");
	return;
    }

    /* all checks positive */

    ui_link.weight = krui_getLinkWeight();
    ui_net_drawLink(ui_currentDisplay, UI_GLOBAL,
		    ui_sourceUnit.no, ui_targetUnit.no, 
		    ui_link.weight, UI_ERASE);
    
    /* set weight */
    krui_setLinkWeight(ui_link.weight = (FlintType)
		       ui_xFloatFromAsciiWidget(ui_linkWidgets.weight)); 
    
    /* show messages */
    
    ui_printMessage("update link done.");
    if (krui_getUnitInputType(ui_targetUnit.no) == SITES)
	sprintf(buf,"SET link weight  : %d -> %d : %.8f   *** to site\n\n",
		ui_sourceUnit.no, ui_targetUnit.no, ui_link.weight);
    else
	sprintf(buf,"SET link weight  : %d -> %d : %.8f   *** direct\n\n",
		ui_sourceUnit.no, ui_targetUnit.no, ui_link.weight);

    ui_tw_printMessage(buf); 
    
    ui_net_drawLink(ui_currentDisplay, UI_GLOBAL,
		    ui_sourceUnit.no, ui_targetUnit.no, 
		    ui_link.weight, UI_DRAW);
}


/*****************************************************************************
  FUNCTION : ui_info_showSuccOfSourceUnit

  PURPOSE  : Display the successor at succPosition in the row TARGET and show
             the weight of the assigned link
  RETURNS  : alteration of unit-info-panel
  NOTES    : If no unit is found at succPosition, this routine shows dummy
             values. Give a position with at least one coordinate set as
	     0 to show, that a source unit has now successors.

  UPDATES  :
*****************************************************************************/

static void ui_info_showSuccOfSourceUnit(int succNo, FlintType weight)

{
    ui_info_initLinkStruct();

    if ((NOT ui_info_anyUnitSelected(UI_SOURCE)) OR
	((ui_targetUnit.no = succNo) == 0)) {
	/* show dummy values */
	ui_info_initUnitStruct(&ui_targetUnit);	
    } else {	
	/* SOURCE exists, get values of target with succNo */
	ui_info_getAllUnitData(ui_targetUnit.no, &ui_targetUnit);
	if (krui_getUnitInputType(ui_targetUnit.no) == SITES)
	    ui_info_getAllSiteData(); 
	ui_link.weight = weight;
    }
    ui_info_setLinkItems();
    ui_info_setUnitItems(ui_targetWidgets, ui_targetUnit);
}


/*****************************************************************************
  FUNCTION : ui_info_showPredOfTargetUnit

  PURPOSE  : Display the predecessor witgh  predNo in the row SOURCE and show
             the data of this link
  RETURNS  : alteration of unit-info-panel
  NOTES    : If no unit is found with predNo, this routine shows dummy
             values. predNo == 0 means, that no predecessor exists. 

  UPDATES  :
*****************************************************************************/

void ui_info_showPredOfTargetUnit(int predNo, FlintType weight)

{
    if ((NOT ui_info_anyUnitSelected(UI_TARGET)) OR
	((ui_sourceUnit.no = predNo) == 0)) {
	/* show dummy values */
	ui_info_initUnitStruct(&ui_sourceUnit);
    } else {	
	/* get values */
	ui_info_getAllUnitData(predNo, &ui_sourceUnit);
	ui_link.weight = weight;
    }
    ui_info_setLinkItems();
    ui_info_setUnitItems(ui_sourceWidgets, ui_sourceUnit);
}


/*****************************************************************************
  FUNCTION : ui_info_showFirstSuccOfSourceUnit

  PURPOSE  : search the first successor of the current source unit and display
             the values in the TARGET row.
  RETURNS  : alteration of unit-info-panel
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_showFirstSuccOfSourceUnit(Widget w, XtPointer button, 
	caddr_t call_data)

{
    FlintType  weight = 0.0;

    ui_targetUnit.no  = 0;

    ui_info_initLinkStruct();

    if (ui_info_anyUnitSelected(UI_SOURCE)) {
	ui_targetUnit.no = 
	    krui_getFirstSuccUnit(ui_sourceUnit.no, &weight);
	if ((ui_targetUnit.no != 0) AND 
	    (krui_getUnitInputType(ui_targetUnit.no) == SITES)) {
	    /* it is shure, there is a current site */
	    ui_info_getAllSiteData();
	}
    }				  
    ui_info_showSuccOfSourceUnit(ui_targetUnit.no, (FlintType) weight);
}


/*****************************************************************************
  FUNCTION : ui_info_showNextSuccOfSourceUnit

  PURPOSE  : search the next successor of the current source unit and display
             the values in the TARGET row.
  RETURNS  : alteration of unit-info-panel
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_showNextSuccOfSourceUnit(Widget w, XtPointer button, 
	caddr_t call_data)

{
    FlintType  weight = 0.0;
    int        unitNo;

    ui_info_initLinkStruct();

    if (ui_info_anyUnitSelected(UI_SOURCE)) {
	if (ui_info_anyUnitSelected(UI_TARGET)) {
	    if (krui_isConnected(ui_sourceUnit.no)) {
		(void) krui_setCurrentUnit(ui_sourceUnit.no);
		if ((unitNo = krui_getNextSuccUnit(&weight)) > 0)
		    ui_targetUnit.no = unitNo;
		if (krui_getUnitInputType(ui_targetUnit.no) == SITES) {
		    /* it is shure, there is a current site */
		    ui_info_getAllSiteData();
		}
		ui_info_showSuccOfSourceUnit(ui_targetUnit.no, weight);
	    } else 
		ui_info_showFirstSuccOfSourceUnit(NULL, NULL, NULL);
	}
    }
}


/*****************************************************************************
  FUNCTION : ui_info_showFirstPredOfTargetUnit

  PURPOSE  : search the first predecessor of the current target unit and 
             display the values in the SOURCE row.
  RETURNS  : alteration of unit-info-panel
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_showFirstPredOfTargetUnit(Widget w, XtPointer button, 
	caddr_t call_data)

{
    FlintType    weight = 0.0;

    ui_sourceUnit.no = 0;

    if (ui_info_anyUnitSelected(UI_TARGET)) {
	(void) krui_setCurrentUnit(ui_targetUnit.no);
	if (ui_krui_setCurrentSite(ui_targetUnit.no, ui_link.siteName)) { 
	    if (krui_getUnitInputType(ui_targetUnit.no) == SITES)
		ui_info_getAllSiteData();
	    else
		ui_info_initLinkStruct();
	}	
	ui_sourceUnit.no = krui_getFirstPredUnit(&weight);
    }	
    ui_info_showPredOfTargetUnit(ui_sourceUnit.no, weight);
}

 
/*****************************************************************************
  FUNCTION : ui_info_showNextPredOfTargetUnit

  PURPOSE  : search the next predecessor of the current target unit and 
             display the values in the SOURCE row.
  RETURNS  : alteration of unit-info-panel
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_showNextPredOfTargetUnit(Widget w, XtPointer button, 
	caddr_t call_data)

{
    FlintType   weight = 0.0;
    int         unitNo;

    if (NOT ui_info_anyUnitSelected(UI_TARGET)) 
	return;

    if (krui_getUnitInputType(ui_targetUnit.no) == SITES) {
	if (strlen(ui_link.siteName)) {
	    if (krui_setSite(ui_link.siteName) != UI_SET_OK) { 
		if (krui_setFirstSite())
		    ui_info_getAllSiteData();
	    } else /* this site exist: get all site data */
		ui_info_getAllSiteData();
	} else { /* invalid site name */
	    if (krui_setFirstSite())
		ui_info_getAllSiteData();
	}
    } else /* unit with direct links */
	ui_info_initLinkStruct();
    
    if (krui_isConnected(ui_sourceUnit.no)) {
	/* Now, all CURRENT pointers in the kernel are set correctly */
	if ((unitNo = krui_getNextPredUnit(&weight)) > 0) {
	    ui_sourceUnit.no = unitNo;
	}
	ui_info_showPredOfTargetUnit(ui_sourceUnit.no, weight);
    } else { /* not connected => illegal operation */
	ui_confirmOk("Invalid source: not connected with target!");
    }
}


/*****************************************************************************
  FUNCTION : ui_info_showFirstSiteOfTargetUnit

  PURPOSE  : display the next site and the first predecessor on it
  RETURNS  : alteration of unit-info-panel
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_showFirstSiteOfTargetUnit(Widget w, caddr_t lient_data,  
	caddr_t  call_data)

{
    ui_info_initLinkStruct();

    if (NOT ui_info_anyUnitSelected(UI_TARGET)) {
	ui_confirmOk("Invalid target!");
	return;
    }

    if (krui_getUnitInputType(ui_targetUnit.no) != SITES) {
	ui_info_showFirstPredOfTargetUnit(NULL, NULL, NULL);
	return;
    }

    if (krui_setFirstSite()) {
	ui_info_getAllSiteData();
	ui_info_showFirstPredOfTargetUnit(NULL, NULL, NULL);
    } /* else is impossible !! */
}

 
/*****************************************************************************
  FUNCTION : ui_info_showNextSiteOfTargetUnit

  PURPOSE  : display the next site and the first predecessor on it
  RETURNS  : alteration of unit-info-panel
  NOTES    : 

  UPDATES  :
*****************************************************************************/

void ui_info_showNextSiteOfTargetUnit(Widget w, caddr_t client_data, 
	caddr_t call_data)

{
    if (NOT ui_info_anyUnitSelected(UI_TARGET)) {
	ui_confirmOk("Invalid target!");
	return;
    }

    if (krui_getUnitInputType(ui_targetUnit.no) != SITES) 
	return;

    if (strlen(ui_link.siteName)) {
	if (krui_setSite(ui_link.siteName) != UI_SET_OK) {
	    krui_setFirstSite();
	    ui_info_getAllSiteData();
	    ui_info_showFirstPredOfTargetUnit(NULL, NULL, NULL);
	} else {
	    if (krui_setNextSite()) {
		ui_info_getAllSiteData();
		ui_info_showFirstPredOfTargetUnit(NULL, NULL, NULL);
	    }
	    /* else (= no more sites) leave the panel unchanged */
	}
    } else { /* invalid site in ui_link.siteName */
	krui_setFirstSite();
	ui_info_getAllSiteData();
	ui_info_showFirstPredOfTargetUnit(NULL, NULL, NULL);
    }
}


/*****************************************************************************
  FUNCTION : ui_info_showSelectedUnit

  PURPOSE  : search and display as source unit the unit at the given position.
             Because this routine is called caused by an event in a graphik
	     window, the calling argument is a position (raster).
  RETURNS  : alteration of unit-info-panel
  NOTES    : if no unit at this position will be found, this routine will
             display dummy values. 

  UPDATES  :
*****************************************************************************/

void ui_info_showSelectedUnit(int unitNo)

{
    if (krui_setCurrentUnit(unitNo) == UI_SET_OK) { /* show values */
	ui_targetUnit.no = unitNo;
	ui_info_getAllUnitData(ui_targetUnit.no, &ui_targetUnit);
    } else { /* show dummy values */
	ui_targetUnit.no = 0;
	ui_printMessage("No target unit selected !");
	ui_info_initUnitStruct(&ui_targetUnit);
    }
    ui_info_setUnitItems(ui_targetWidgets, ui_targetUnit);
    ui_info_showFirstSiteOfTargetUnit(NULL, NULL, NULL);
}


/*****************************************************************************
  FUNCTION : ui_info_showFirstUnit

  PURPOSE  : search and display the first unit found by the kernal in his
             data structure.
  RETURNS  : alteration of unit-info-panel
  NOTES    : if no unit at this position will be found, this routine will
             display dummy values. 

  UPDATES  :
*****************************************************************************/

static void ui_info_showFirstUnit(void)

{
    ui_targetUnit.no = 0;

    if ((ui_targetUnit.no = krui_getFirstUnit()) <= 0) {
	/* no unit available */
	ui_info_initLinkStruct();
	ui_info_showFirstPredOfTargetUnit(NULL, NULL, NULL);	
	ui_printMessage("The network is empty!");
    }
    ui_info_showSelectedUnit(ui_targetUnit.no);
}


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_info_makeTargetToSourceUnit

  PURPOSE  : The current target unit will become the source unit. The TARGET 
             row will show the first successor of TARGET (if any).
  RETURNS  : alteration of unit-info-panel
  NOTES    : if target is invalid, the confirmer will give a message stating
	     this.
             !!! function is not used yet

  UPDATES  :
*****************************************************************************/

/*!*/ /* not used */
static void ui_info_makeTargetToSourceUnit(void)

{
    if (krui_setCurrentUnit(ui_targetUnit.no) == UI_SET_OK)
	ui_info_showSelectedUnit(ui_targetUnit.no);
    else
	ui_printMessage("Can't make target current source!");
}

#endif /* _UNUSED_FUNCTIONS_ */


#ifdef _UNUSED_FUNCTIONS_ 

/*****************************************************************************
  FUNCTION : ui_info_makeSourceToTargetUnit

  PURPOSE  : The current source unit will become the target unit. The SOURCE 
             row will show the first predecessor of SOURCE (if any).
  RETURNS  : alteration of unit-info-panel
  NOTES    : if source is invalid, the confirmer will give a message stating
           : this.
             !!! function is not used yet

  UPDATES  :
*****************************************************************************/

/*!*/ /* not used */
static void ui_info_makeSourceToTargetUnit(void)

{
    if (krui_setCurrentUnit(ui_sourceUnit.no) == UI_SET_OK)
	ui_info_showSelectedUnit(ui_sourceUnit.no);
    else
	ui_printMessage("Can't make source current target!");
}

#endif /* _UNUSED_FUNCTIONS_ */


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_info_listAllUnits

  PURPOSE  : list all units with all attributes in the text window
  RETURNS  : alteration of the text window
  NOTES    : !!! function is not used yet

  UPDATES  :
*****************************************************************************/

/*!*/ /* not used */
static void ui_info_listAllUnits(Widget w, XtPointer button, caddr_t call_data)

{
    int                       unitNo;
    struct UnitAttributeType  unit;

    char                buf[UI_MAX_TW_BUFFER_SIZE];
    Bool                successful;

    successful = ((unitNo = krui_getFirstUnit()) > 0);

    if (NOT successful) 
	ui_printMessage("The Network is empty!");
    else {
	ui_printMessage("Listing all units ...");
	/* ui_displayText_proc(); */
	ui_tw_printMessage("\n\nlisting of all units:\n");
	ui_tw_printMessage("format: NUMBER NAME [FTYPE] ACTIVATION (OUTPUT):BIAS\n");
     	
	while (successful) {
	    
	    ui_info_getAllUnitData(unitNo, &unit);
	    
	    sprintf(buf,"   %6d %s [%s] %8.5f (%8.5f):%8.5f\n", unit.no, unit.name, 
		    unit.ftype, unit.act, unit.out, unit.bias);
	    ui_tw_printMessage(buf); 
	    
	    successful = ((unitNo = krui_getNextUnit()) > 0);
	}

	ui_tw_printMessage("\n\nend of listing\n\n\n");
	ui_printMessage("");
    }
}

#endif /* _UNUSED_FUNTIONS_ */


/*****************************************************************************
  FUNCTION : ui_info_listAllTargetUnits

  PURPOSE  : list all successors of the current SOURCE in the text window
  RETURNS  : alteration of the text window
  NOTES    :

  UPDATES  :
*****************************************************************************/

void ui_info_listAllTargetUnits(Widget w, int unitType, caddr_t call_data)

{
    int                       tarNo, sourceNo;
    struct UnitAttributeType  targetUnit;
    FlintType                 weight; 
    char      *name;
    char      buf[UI_MAX_TW_BUFFER_SIZE];
    Bool      successful;

    if (NOT ui_info_anyUnitSelected(unitType)) 
	return;

    if (unitType == UI_SOURCE) {
	sourceNo = ui_sourceUnit.no;
	name     = ui_sourceUnit.name;
    } else {
	sourceNo = ui_targetUnit.no;
	name     = ui_targetUnit.name;
    }

    if (NOT (successful = 
	     ((tarNo = krui_getFirstSuccUnit(sourceNo, &weight)) > 0))) {
	ui_printMessage("This unit has no successors!");
	return;
    }

    ui_printMessage("Listing all successors ...");
    /* ui_displayText_proc(); */
    sprintf(buf,"\n\nListing of all successors of unit %d - %s\n\n", sourceNo,name);
    ui_tw_printMessage(buf);
    ui_tw_printMessage("  weight    -> number - name                 output    activat.  bias\n");
    ui_tw_printMessage("---------------------------------------------------------------------------\n");
    while (successful) {
	ui_info_getAllUnitData(tarNo, &targetUnit);
	
	sprintf(buf,"  %9.5f -> %6d - %-20s %8.5f  %8.5f  %9.5f\n", 
		weight, targetUnit.no, targetUnit.name, targetUnit.out, 
		targetUnit.act, targetUnit.bias);
	ui_tw_printMessage(buf);
	
	ui_krui_setCurrentLink(sourceNo, tarNo, "");
	(void) krui_setCurrentUnit(sourceNo);
	successful = ((tarNo = krui_getNextSuccUnit(&weight)) > 0);
    }
    ui_tw_printMessage("\n\nend of listing\n\n\n");
    ui_printMessage("");
}


/*****************************************************************************
  FUNCTION : ui_info_listAllSourceUnits

  PURPOSE  : list all predecessors of the current TARGET in the text window
  RETURNS  : alteration of the text window
  NOTES    :

  UPDATES  :
*****************************************************************************/

void ui_info_listAllSourceUnits(Widget w, int unitType, caddr_t call_data)

{
    int                       targetNo, srcNo;
    struct UnitAttributeType  sourceUnit;
    FlintType                 weight; 
    char      *name;
    char      buf[UI_MAX_TW_BUFFER_SIZE];
    Bool      successful;
    Bool      siteIsActive;

    if (NOT ui_info_anyUnitSelected(unitType)) 
	return;

    if (unitType == UI_SOURCE) {
	targetNo = ui_sourceUnit.no;
	name     = ui_sourceUnit.name;
    } else {
	targetNo = ui_targetUnit.no;
	name     = ui_targetUnit.name;
    }
   
    (void) krui_setCurrentUnit(targetNo);
    
    ui_printMessage("Listing of all predecessors ...");
    /* ui_displayText_proc(); */
    sprintf(buf,"\n\nListing of all predecessors of unit %d - %s\n\n", targetNo, name);
    ui_tw_printMessage(buf);
    ui_tw_printMessage("  weight    <- number - name                 output    activat.  bias\n");
    ui_tw_printMessage("---------------------------------------------------------------------------\n");

    siteIsActive = krui_setFirstSite();

    do { /* loop over all sites */ 
	if (siteIsActive) {
	    sprintf(buf,"\nsite: %-20s  (func: %-20s)  value: %9.5f\n",
		    krui_getSiteName(), krui_getSiteFuncName(), 
		    krui_getSiteValue());
	    ui_tw_printMessage(buf);
	}
	successful = 
	    ((srcNo = krui_getFirstPredUnit(&weight)) > 0);
	
	while (successful) {
	    ui_info_getAllUnitData(srcNo, &sourceUnit);
	    
	    sprintf(buf,"  %9.5f <- %6d - %-20s %8.5f  %8.5f  %9.5f\n", 
		    weight, srcNo, sourceUnit.name, sourceUnit.out, 
		    sourceUnit.act, sourceUnit.bias);
	    ui_tw_printMessage(buf);
	    
	    successful = ((srcNo = krui_getNextPredUnit(&weight)) > 0);
	} /* end while */

    } while ((siteIsActive = krui_setNextSite()));

    ui_tw_printMessage("\n\nend of listing\n\n\n");
    ui_printMessage("");
}


/*****************************************************************************
  FUNCTION : ui_info_listAllSites

  PURPOSE  : list all predecessors of the current TARGET in the text window
  RETURNS  : alteration of the text window
  NOTES    :

  UPDATES  :
*****************************************************************************/

void ui_info_listAllSites(Widget w, int unitType, caddr_t call_data)

{
    int       targetNo;
    char      *name;
    char      buf[UI_MAX_TW_BUFFER_SIZE];
    Bool      siteIsActive;

    if (NOT ui_info_anyUnitSelected(unitType)) 
	return;

    if (unitType == UI_SOURCE) {
	targetNo = ui_sourceUnit.no;
	name     = ui_sourceUnit.name;
    } else {
	targetNo = ui_targetUnit.no;
	name     = ui_targetUnit.name;
    }
   
    (void) krui_setCurrentUnit(targetNo);
    
    ui_printMessage("Listing of all sites ...");
    /* ui_displayText_proc(); */
    sprintf(buf,"\n\nListing of all sites of unit %d - %s\n\n", targetNo, name);
    ui_tw_printMessage(buf);
    ui_tw_printMessage("  site name                   site function                   value\n");
    ui_tw_printMessage("---------------------------------------------------------------------------\n");

    siteIsActive = krui_setFirstSite();

    do { /* loop over all sites */ 
	if (siteIsActive) {
	    sprintf(buf,"  %-24s    %-24s    %9.5f\n",
		    krui_getSiteName(), krui_getSiteFuncName(), 
		    krui_getSiteValue());
	    ui_tw_printMessage(buf);
	}
    } while ((siteIsActive = krui_setNextSite()));
    
    ui_tw_printMessage("\n\nend of listing\n\n\n");
    ui_printMessage("");
}


/*****************************************************************************
  FUNCTION : ui_info_listAllLinksToCurrentSite

  PURPOSE  : list all predecessors of the current TARGET in the text window
  RETURNS  : alteration of the text window
  NOTES    :

  UPDATES  :
*****************************************************************************/

void ui_info_listAllLinksToCurrentSite(Widget w, int unitType, 
	caddr_t call_data)

{
    int                       srcNo;
    struct UnitAttributeType  sourceUnit;
    FlintType                 weight; 
    char      buf[UI_MAX_TW_BUFFER_SIZE];
    Bool      successful;

    if (NOT ui_info_anyUnitSelected(UI_TARGET))
	return;

    if (NOT strlen(ui_link.siteName) OR 
	(krui_setSite(ui_link.siteName) != UI_SET_OK)) {
	sprintf(buf,"Invalid site <%s>!", ui_link.siteName);
	ui_confirmOk(buf);
	return;
    }

    ui_printMessage("Listing of all links to current site ...");
    /* ui_displayText_proc(); */
    sprintf(buf,"\n\nListing of all links to site %s of unit %d - %s\n\n", 
	    ui_link.siteName, ui_targetUnit.no, ui_targetUnit.name);
    ui_tw_printMessage(buf);
    sprintf(buf,"\nsite: %-20s  (func: %-20s)  value: %9.5f\n\n",
	    krui_getSiteName(), krui_getSiteFuncName(), 
	    krui_getSiteValue());
    ui_tw_printMessage(buf);
    
    ui_tw_printMessage("  weight    <- number - name                 output    activat.  bias\n");
    ui_tw_printMessage("---------------------------------------------------------------------------\n");

    successful = 
	     ((srcNo = krui_getFirstPredUnit(&weight)) > 0);

    while (successful) {
	ui_info_getAllUnitData(srcNo, &sourceUnit);
	
	sprintf(buf,"  %9.5f <- %6d - %-20s %8.5f  %8.5f  %9.5f\n", 
		weight, srcNo, sourceUnit.name, sourceUnit.out, 
		sourceUnit.act, sourceUnit.bias);
	ui_tw_printMessage(buf);
	
	successful = ((srcNo = krui_getNextPredUnit(&weight)) > 0);
    } /* end while */

    ui_tw_printMessage("\n\nend of listing\n\n\n");
    ui_printMessage("");
}


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_info_listAllLinks

  PURPOSE  : list all links of the network in forward and backward direction
  RETURNS  : alteration of the text window
  NOTES    : !!! function is not used yet

  UPDATES  :
*****************************************************************************/

/*!*/ /* not used */
static void ui_info_listAllLinks(void)

{
    int        sourceNo, targetNo;
    FlintType  weight; 

    char      buf[UI_MAX_TW_BUFFER_SIZE];
    Bool      successful;

    successful = ((sourceNo = krui_getFirstUnit()) > 0);

    if (NOT successful) 
	ui_printMessage("The network is empty!");
    else {
	ui_printMessage("Listing all links ...");
	/* ui_displayText_proc(); */
	ui_tw_printMessage("\n\nListing of all links of the network\n\n"); 
	ui_tw_printMessage("Listing forward\n");
	ui_tw_printMessage("format: SOURCE: TARGET1:WEIGHT1  TARGET2:WEIGHT2  ...\n\n"); 

	while (successful) {

	    sprintf(buf,"%6d:",sourceNo);
	    ui_tw_printMessage(buf);

	    successful = 
		((targetNo = krui_getFirstSuccUnit(sourceNo, &weight)) > 0);
			   
	    while (successful) {
		sprintf(buf, "  %d:%8.5f", targetNo, weight);
		ui_tw_printMessage(buf);

		successful = ((targetNo = krui_getNextSuccUnit(&weight)) > 0);
	    }
	    ui_tw_printMessage("\n");
	    successful = ((sourceNo = krui_getNextUnit()) > 0);
	}
	ui_tw_printMessage("\n\n");
	ui_tw_printMessage("Listing backward\n");
	ui_tw_printMessage("format: TARGET: SOURCE1:WEIGHT1, SOURCE2:WEIGHT2, ...\n\n");
	successful = ((targetNo = krui_getFirstUnit()) > 0);

	while (successful) {
	    sprintf(buf,"%6d:",targetNo);
	    ui_tw_printMessage(buf); 

	    successful = ((sourceNo = krui_getFirstPredUnit(&weight)) > 0);
	    while (successful) {
		sprintf(buf, "  %d:%8.5f", sourceNo, weight);
		ui_tw_printMessage(buf);

		successful = ((sourceNo = krui_getNextPredUnit(&weight)) > 0);
	    }
	    ui_tw_printMessage("\n");
	    successful = ((targetNo = krui_getNextUnit()) > 0);
	}
	ui_tw_printMessage("\nend of listing.\n\n");
	ui_printMessage("");
    } /* end else : there are unit */
}

#endif /* _UNUSED_FUNCTIONS_ */


/*****************************************************************************
  FUNCTION : ui_info_makeUnitInfoPanelConsistent

  PURPOSE  : After deleting units, the unit info panel may show data of a
             unit, which was deleted. This routine shows then data of a valid
	     unit/successor.
  RETURNS  : displays values in the unit info panel.
  NOTES    : Code:
	     if TARGET and SOURCE ok, but not connected or TARGET only, 
	        then display first predecessor of target.
	     if SOURCE only, then display first successor of SOURCE.
	     if both units are invalid, then display "first" connection of
                the network (first unit, site, link)

  UPDATES  :
*****************************************************************************/

void ui_info_makeUnitInfoPanelConsistent(void)

{
    if (ui_infoIsCreated) {
	Bool SourceValid = ui_info_anyUnitSelected(UI_SOURCE);
	Bool TargetValid = ui_info_anyUnitSelected(UI_TARGET);
	Bool LinkValid   = TargetValid AND 
	    ui_krui_setCurrentSite(ui_targetUnit.no, ui_link.siteName);

	LinkValid = LinkValid AND krui_isConnected(ui_sourceUnit.no);

	if ((SourceValid AND TargetValid AND (NOT LinkValid)) OR
	    (TargetValid AND (NOT SourceValid))) {
	    ui_info_showFirstPredOfTargetUnit(NULL, NULL, NULL);
	    return;
	}
	
	if (SourceValid AND (NOT TargetValid)) {
	    ui_info_showFirstSuccOfSourceUnit(NULL, NULL, NULL);
	    return;
	}

	if (LinkValid) return; /* everything is ok. */

	/* nothing is valid */
	ui_info_showFirstUnit();
    }
}


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_info_deleteLink

  PURPOSE  : delete the displayed link
  RETURNS  : alteration of the network
  NOTES    : !!! function is not used yet

  UPDATES  :
*****************************************************************************/

/*!*/ /* not used */
static void ui_info_deleteLink(void)

{
    FlintType  weight;

    if (ui_info_anyUnitSelected(UI_SOURCE) AND
	ui_info_anyUnitSelected(UI_TARGET)) {
	/* ask whether both units are connected, set current link pointer */
	if (NOT ui_krui_setCurrentSite(ui_targetUnit.no, ui_link.siteName)) {
	    ui_confirmOk("Invalid site!");
	    return;
	}
	if (NOT krui_isConnected(ui_sourceUnit.no)) {
	    ui_confirmOk("No link exist!");
	    return;
	}

	weight = krui_getLinkWeight();
	krui_deleteLink();
	ui_net_drawLink(ui_currentDisplay, UI_GLOBAL,
			ui_sourceUnit.no, ui_targetUnit.no, 
			weight, UI_ERASE);
	ui_info_makeUnitInfoPanelConsistent();
    }
}

#endif /* _UNUSED_FUNTIONS_ */


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_info_showNextUnit

  PURPOSE  : search and display the next unit found by the kernal in the
             data structure.
  RETURNS  : alteration of unit-info-panel
  NOTES    : if no unit at this position will be found, this routine will
             display dummy values. 
              !!! function is not used yet

  UPDATES  :
*****************************************************************************/

/*!*/ /* not used */
static void ui_info_showNextUnit(void)

{
    ui_info_makeUnitInfoPanelConsistent();

    if (ui_info_anyUnitSelected(UI_SOURCE)) {

	krui_setCurrentUnit(ui_sourceUnit.no);
	ui_sourceUnit.no = krui_getNextUnit();

	if ((ui_sourceUnit.no = krui_getNextUnit()) > 0) {
	    krui_getUnitPosition(ui_sourceUnit.no, &ui_sourceUnit.gridPos);
	    ui_info_showSelectedUnit(ui_sourceUnit.no);
	} else 
	    ui_printMessage("No more units in this network !"); 
    } else 
	/* the user pressed the NEXT button, but no valid unit was displayed */
	ui_info_showFirstUnit();
}

#endif /* _UNUSED_FUNCTIONS_ */




/* end of file */
/* lines: 1622 */

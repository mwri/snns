/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_lists.c,v $
  SHORTNAME      : lists.c
  SNNS VERSION   : 4.2

  PURPOSE        : build and manage lists for choosing f-types, io-types,
                   outFunction, actFunction, site and siteFunction
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 27.8.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:35 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>

#include "ui.h"

#include <X11/Xaw/List.h>

#include "glob_typ.h"	/*  Kernel constant and type definitions  */
#include "kr_ui.h"	/*  Kernel interface functions	*/

#include "ui_textP.h"
#include "ui_utilP.h"
#include "ui_xWidgets.h"
#include "ui_infoP.h"
#include "ui_mainP.h"
#include "ui_info.h"
#include "pr_main.h"
#include "ui_control.h"

#include "ui_lists.ph"


/*****************************************************************************
  FUNCTION : ui_list_getFirstItem

  PURPOSE  : return the first list item of the given type
  RETURNS  : char *
  NOTES    :

  UPDATE   : 27.8.1990
******************************************************************************/

char *ui_list_getFirstItem(int type)

{
    char *name, *funcName;
    int  funcType;
    int  i, funcNo ,number_of_func;
    Bool found = FALSE;

    switch (type) {
      case UI_LIST_FTYPE_ACT_FUNC:
	type = UI_LIST_ACT_FUNC;
	break;
      case UI_LIST_FTYPE_OUT_FUNC:
	type = UI_LIST_OUT_FUNC;
	break;
    }	

    switch (type) {
      case UI_LIST_ACT_FUNC:
      case UI_LIST_OUT_FUNC:
      case UI_LIST_SITE_FUNC:
      case UI_LIST_LEARN_FUNC:
      case UI_LIST_UPDATE_FUNC:
      case UI_LIST_INIT_FUNC:
      case UI_LIST_PRUN_FUNC:
      case UI_LIST_REMAP_FUNC:
      case UI_LIST_FF_LEARN_FUNC:
	ui_list_noOfFunctions = 0;
	number_of_func = krui_getNoOfFunctions();
	for (i=1; (i <= number_of_func) AND (NOT found); i++) {
	    krui_getFuncInfo(i,&funcName, &funcType);
	    found = (((type & FUNC_TYPE_MASK) == 
		      (funcType & FUNC_TYPE_MASK)) &&
		     ((type & funcType) == type));
	    if (found) funcNo = i;
	}
	if (found) {
	    ui_list_noOfFunctions = 1;
	    ui_list_currentFuncNo[funcType & ~FF_LEARN_FUNC] = funcNo;
	    return(funcName); 
	} else return("*** no function present ***");
	break;
      case UI_LIST_FTYPE_NAME:
      case UI_LIST_FTYPE:
	ui_list_noOfFtypes = 0;
	if (krui_setFirstFTypeEntry()) {
	    ui_list_noOfFtypes++;
	    return(krui_getFTypeName());
	} else
	    return("*** no f-types present ***");
	break;
      case UI_LIST_IOTYPE:
	ui_list_currentIOType = 1;
	return(ui_list_iotypes[ui_list_currentIOType]);
	break;
      case UI_LIST_SITE:
	ui_list_noOfSites = 0;
	if (krui_getFirstSiteTableEntry(&name, &funcName)) {
	    ui_list_noOfSites++;
	    return(name);
	} else
	    return("*** no sites ***");
	break;
      case UI_LIST_FTYPE_SITE:
	ui_list_noOfFTypeSites = 0;
	if (krui_setFirstFTypeSite()) {
	    ui_list_noOfFTypeSites++;
	    return((char *) krui_getFTypeSiteName());
	} else
	    return("*** no sites ***");
	break;
      case UI_LIST_UNIT_SITE:
	ui_list_noOfSites = 0;
	if (krui_setFirstSite()) {
	    ui_list_noOfSites++;
	    return((char *) krui_getSiteName());
	} else
	    return("*** no sites ***");
	break;
    }
    return(NULL);
}


/*****************************************************************************
  FUNCTION : ui_list_getNextItem

  PURPOSE  : return the next list item of the given type
  RETURNS  : char *
  NOTES    :

  UPDATE   : 27.8.1990
******************************************************************************/

char *ui_list_getNextItem(int type)

{
    char *name, *funcName;
    int  funcType,func_num;
    int  i, funcNo;
    Bool found = FALSE;

    switch (type) {
      case UI_LIST_FTYPE_ACT_FUNC:
	type = UI_LIST_ACT_FUNC;
	break;
      case UI_LIST_FTYPE_OUT_FUNC:
	type = UI_LIST_OUT_FUNC;
	break;
    }	

    switch (type) {
      case UI_LIST_ACT_FUNC:
      case UI_LIST_OUT_FUNC:
      case UI_LIST_SITE_FUNC:
      case UI_LIST_LEARN_FUNC:
      case UI_LIST_UPDATE_FUNC:
      case UI_LIST_INIT_FUNC:
      case UI_LIST_PRUN_FUNC:
      case UI_LIST_REMAP_FUNC:
      case UI_LIST_FF_LEARN_FUNC:
	func_num = krui_getNoOfFunctions();
	for (i = ui_list_currentFuncNo[type & ~FF_LEARN_FUNC] + 1; 
	     (i <= func_num) AND NOT found ; i++) {
	    krui_getFuncInfo(i,&funcName, &funcType);
	    found = (((type & FUNC_TYPE_MASK) == 
		      (funcType & FUNC_TYPE_MASK)) &&
		     ((type & funcType) == type));
	    if (found) funcNo = i;
	}
	if (found) {
	    ui_list_noOfFunctions++;
	    ui_list_currentFuncNo[funcType & ~FF_LEARN_FUNC] = funcNo;
	    return(funcName); 
	} else return(NULL);
	break;
      case UI_LIST_FTYPE_NAME:
      case UI_LIST_FTYPE:
	if (krui_setNextFTypeEntry())
	    return(krui_getFTypeName());
	else
	    return(NULL);
	break;
      case UI_LIST_IOTYPE:
	if (ui_list_currentIOType < 11)
	    return(ui_list_iotypes[++ui_list_currentIOType]);
	else return(NULL);
	break;
      case UI_LIST_SITE:
	if (krui_getNextSiteTableEntry(&name, &funcName)) {
	    ui_list_noOfSites++;
	    return(name);
	} else
	    return(NULL);
	break;
      case UI_LIST_FTYPE_SITE:
	if (krui_setNextFTypeSite()) {
	    ui_list_noOfFTypeSites++;
	    return((char *) krui_getFTypeSiteName());
	} else
	    return(NULL);
	break;
      case UI_LIST_UNIT_SITE:
	if (krui_setNextSite()) {
	    ui_list_noOfSites++;
	    return((char *) krui_getSiteName());
	} else
	    return(NULL);
	break;
    }
    return(NULL);
}


/*****************************************************************************
  FUNCTION : ui_list_buildList

  PURPOSE  : buikd and display the list described in the list descriptor
  RETURNS  : void
  NOTES    :

  UPDATE   : 27.8.1990
******************************************************************************/

void ui_list_buildList(struct SimpleListType *listDescriptorPtr)

{
    char *string;


    listDescriptorPtr->noOfItems = 0;

    string = ui_list_getFirstItem(listDescriptorPtr->listType);

    while (string != NULL) {
	ui_xAddListEntry(listDescriptorPtr, string);
	string = ui_list_getNextItem(listDescriptorPtr->listType);
    }
}


/*****************************************************************************
  FUNCTION : ui_list_setValue

  PURPOSE  : callback. Called when a list item was choosed. 
  RETURNS  : void
  NOTES    :

  UPDATE   : 28.8.1990
******************************************************************************/

void ui_list_setUnitValue(Widget w, struct SimpleListType *listDescrPtr, 
	XawListReturnStruct *listStructPtr)

{
    char  buf[120];

    ui_list_returnIndex = listStructPtr->list_index;

    /* if no item selected then return */
    if (listStructPtr->list_index == XAW_LIST_NONE)
	return;

    /* an item is selected */
    /* but if no ftypes present when requesting f-types then return */
    if (((listDescrPtr->listType == UI_LIST_FTYPE) OR
	 (listDescrPtr->listType == UI_LIST_FTYPE_NAME)) AND 
	(ui_list_noOfFtypes == 0))
	return;
    
    switch (listDescrPtr->listType) {
      case UI_LIST_IOTYPE:
	listDescrPtr->unitPtr->iotype = (listStructPtr->list_index + 1);
	break;
      case UI_LIST_FTYPE:
	sprintf(listDescrPtr->unitPtr->ftype, "%s", listStructPtr->string);
	break;
      case UI_LIST_ACT_FUNC:
	sprintf(listDescrPtr->unitPtr->actFuncName, "%s", 
		listStructPtr->string);
	break;
      case UI_LIST_OUT_FUNC:
	sprintf(listDescrPtr->unitPtr->outFuncName, "%s", 
		listStructPtr->string);
	break;
      case UI_LIST_FTYPE_ACT_FUNC:
      case UI_LIST_FTYPE_OUT_FUNC:
      case UI_LIST_FTYPE_NAME:
      case UI_LIST_SITE_FUNC:
      case UI_LIST_SITE:
      case UI_LIST_UNIT_SITE:
	/* site functions are requested only during editing sites!
	   Thus this must be handled in a different way. */
	sprintf(ui_list_returnName, "%s", listStructPtr->string);
	break;
      case UI_LIST_LEARN_FUNC:
	ui_checkError(krui_setLearnFunc(listStructPtr->string));
	sprintf(buf,"Learning func:  %s\n",listStructPtr->string);
	ui_tw_printMessage(buf); 
	ui_xSetLabel(ui_controlMessageWidget, buf);
 	ui_NumberOfLearnParamsChanged();
	break;
      case UI_LIST_UPDATE_FUNC:
	ui_checkError(krui_setUpdateFunc(listStructPtr->string)); 
	sprintf(buf,"Update func  :  %s\n",listStructPtr->string);
	ui_tw_printMessage(buf);
	ui_xSetLabel(ui_controlMessageWidget, buf);
 	ui_NumberOfUpdateParamsChanged();
 	break;
      case UI_LIST_INIT_FUNC:
	ui_checkError(krui_setInitialisationFunc(listStructPtr->string));
	sprintf(buf,"Init. func   :  %s\n",listStructPtr->string);
	ui_tw_printMessage(buf);
	ui_xSetLabel(ui_controlMessageWidget, buf);
 	ui_NumberOfInitParamsChanged();
	break;
      case UI_LIST_FF_LEARN_FUNC:
	ui_checkError (krui_setFFLearnFunc (listStructPtr->string));
	sprintf (buf, "Learning function: %s", listStructPtr->string);
	ui_tw_printMessage (buf);
	ui_xSetLabel (pr_learnFuncText, buf);
	break;
      case UI_LIST_PRUN_FUNC:
	ui_checkError (krui_setPrunFunc (listStructPtr->string));
	sprintf (buf, "Pruning function: %s\n", listStructPtr->string);
	ui_tw_printMessage (buf);
	ui_xSetLabel (pr_prunFuncText, buf);
	break;
    } 
    /* unitPtr may be NULL as well ! */
    if (listDescrPtr->unitPtr == &ui_targetUnit)
    {
	ui_info_getDisplayedUnitAttributes(&ui_targetWidgets, &ui_targetUnit);
        ui_info_setUnitItems(ui_targetWidgets, ui_targetUnit);
    }
    else {
	if (listDescrPtr->unitPtr == &ui_sourceUnit)
	{
	    ui_info_getDisplayedUnitAttributes(&ui_sourceWidgets, 
					       &ui_sourceUnit);
	    ui_info_setUnitItems(ui_sourceWidgets, ui_sourceUnit);
	}
    } 

}
	



/* end of file */
/* lines: 350 */

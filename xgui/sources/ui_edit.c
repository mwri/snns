/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_edit.c,v $
  SHORTNAME      : edit.c
  SNNS VERSION   : 4.2

  PURPOSE        : callbacks for editing sites and f-types
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 25.9.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:22 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include "ui.h"

#include <X11/Xaw/List.h>

#include "kr_ui.h"

#include "ui_infoP.h"  /* ui_info_displayList() */
#include "ui_lists.h"
#include "ui_xWidgets.h"
#include "ui_confirmer.h"
#include "ui_utilP.h"
#include "ui_action.h"

#include "ui_edit.ph"


/*****************************************************************************
  FUNCTION : ui_edit_setCurrentSite

  PURPOSE  : makes this side current in the panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

void ui_edit_setCurrentSite (char *name)

{
    bool successful;
    char *siteName, *func;


    if (name != NULL) {
	successful = krui_getFirstSiteTableEntry(&siteName, &func);
	while (successful) {
	    if (strcmp(siteName,name) == 0) { /* site entry found */
		sprintf(ui_edit_siteName, "%s", name);
		ui_xSetString(ui_edit_siteNameWidget, ui_edit_siteName);
		sprintf(ui_edit_siteFuncName, "%s", func);
		ui_xSetLabel(ui_edit_siteFuncNameWidget, ui_edit_siteFuncName);
		return;
	    } else 
		successful = krui_getNextSiteTableEntry(&siteName, &func);
	}
    }
    /* no such site was found or (name == NULL) */
    ui_xSetString(ui_edit_siteNameWidget, "");
    ui_xSetLabel(ui_edit_siteFuncNameWidget, "");
}


/*****************************************************************************
  FUNCTION : ui_edit_selectFunction

  PURPOSE  : popup a list with all site functions
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

void  ui_edit_selectFunction (Widget w, int type, caddr_t call_data)

{   
    static struct SimpleListType listFunc;

    switch (type) {
      case UI_EDIT_SITE_FUNC:
	sprintf(listFunc.msg,"%s","Select site function:");
	listFunc.unitPtr  = NULL;
	listFunc.widgetPtr= w;
	listFunc.listType = UI_LIST_SITE_FUNC;
	ui_info_displayList(w, (struct SimpleListType *) &listFunc, NULL);
	sprintf(ui_edit_siteFuncName,"%s",ui_list_returnName);
	ui_xSetLabel(ui_edit_siteFuncNameWidget, ui_edit_siteFuncName);
	break;
      case UI_EDIT_ACT_FUNC:
	sprintf(listFunc.msg,"%s","Select f-type activation function:");
	listFunc.unitPtr  = NULL;
	listFunc.widgetPtr= w;
	listFunc.listType = UI_LIST_FTYPE_ACT_FUNC;
	ui_info_displayList(w, (struct SimpleListType *) &listFunc, NULL);
	sprintf(ui_edit_actFuncName,"%s",ui_list_returnName);
	ui_xSetLabel(ui_edit_actFuncNameWidget, ui_edit_actFuncName);
	break;
      case UI_EDIT_OUT_FUNC:
	sprintf(listFunc.msg,"%s","Select f-type output function:");
	listFunc.unitPtr  = NULL;
	listFunc.widgetPtr= w;
	listFunc.listType = UI_LIST_FTYPE_OUT_FUNC;
	ui_info_displayList(w, (struct SimpleListType *) &listFunc, NULL);
	sprintf(ui_edit_outFuncName,"%s",ui_list_returnName);
	ui_xSetLabel(ui_edit_outFuncNameWidget, ui_edit_outFuncName);
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_edit_setSite

  PURPOSE  : change name and function of the current selected site
  RETURNS  : void
  NOTES    : This site is highlighted.
             Callback from a SET-Button.

  UPDATE   : 20.9.90
******************************************************************************/

void  ui_edit_setSite(Widget w, struct SimpleListType *listDescriptorPtr, 
	caddr_t call)

{
    XawListReturnStruct  *listStructPtr;

    listStructPtr = XawListShowCurrent(listDescriptorPtr->listWidget);
    
    if (listStructPtr->list_index == XAW_LIST_NONE) {
	ui_confirmOk("No site in the site list selected!");
	return;
    }

    ui_xStringFromAsciiWidget(ui_edit_siteNameWidget, ui_edit_siteName, 
			      MAX_NAME_LENGTH);
    ui_correctName(ui_edit_siteName);
    ui_checkError(krui_changeSiteTableEntry(listStructPtr->string,
					    ui_edit_siteName,
					    ui_edit_siteFuncName));
    if (ui_kernelError < 0)
	ui_confirmOk("Name or function invalid!");
    else {
	free((listDescriptorPtr->listPtr)[listStructPtr->list_index]);
	(listDescriptorPtr->listPtr)[listStructPtr->list_index] =
	    XtNewString(ui_edit_siteName);
	XawListChange(listDescriptorPtr->listWidget, 
		      listDescriptorPtr->listPtr, 
		      listDescriptorPtr->noOfItems, 0, True);
    }
}


/*****************************************************************************
  FUNCTION : ui_edit_listSetSite

  PURPOSE  : callback. Called when a site in the site list was seleted.
  RETURNS  : void
  NOTES    :

  UPDATE   : 28.8.1990
******************************************************************************/

void ui_edit_listSetSite (Widget w, int editType, 
	XawListReturnStruct *listStructPtr) /* call_data of list widgets */

{
    switch (editType) {
      case UI_EDIT_FTYPE:
	break;
      case UI_EDIT_SITE:
	ui_edit_setCurrentSite(listStructPtr->string);
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_edit_newSite

  PURPOSE  : callback. Called when the creation of a new site was requested.
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

void  ui_edit_newSite (Widget w, struct SimpleListType *listDescriptorPtr, 
     caddr_t call)

{
    ui_xStringFromAsciiWidget(ui_edit_siteNameWidget, ui_edit_siteName, 
			      MAX_NAME_LENGTH);
    ui_correctName(ui_edit_siteName);
    if ((strlen(ui_edit_siteName) == 0) OR (strlen(ui_edit_siteFuncName) == 0)) {
	ui_confirmOk("Invalid name or function!");
	return;
    }

    ui_checkError(krui_createSiteTableEntry(ui_edit_siteName, 
					    ui_edit_siteFuncName));
    if (ui_kernelError >= 0) {
	if (ui_list_noOfSites++ == 0) {
	    /* first entry is "*** no sites ***" */
	    free((listDescriptorPtr->listPtr)[0]);
	    (listDescriptorPtr->listPtr)[0] = 
		XtNewString(ui_edit_siteName);
	} else
	    ui_xAddListEntry(listDescriptorPtr, ui_edit_siteName);
	XawListChange(listDescriptorPtr->listWidget, listDescriptorPtr->listPtr, 
		      listDescriptorPtr->noOfItems, 0, True);
    }
}


/*****************************************************************************
  FUNCTION : ui_edit_deleteSite

  PURPOSE  : callback. Called when a site should be deleted 
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

void  ui_edit_deleteSite (Widget w, struct SimpleListType *listDescriptorPtr, 
     caddr_t call)

{
    XawListReturnStruct   *listStructPtr;
    int                   i;

    listStructPtr = XawListShowCurrent(listDescriptorPtr->listWidget);

    if ((listStructPtr->list_index == XAW_LIST_NONE) OR
	(ui_list_noOfSites <= 0))
	return; /* no site selected */

    ui_checkError(krui_deleteSiteTableEntry(listStructPtr->string));
    
    /* remove this entry from the array */
    free((listDescriptorPtr->listPtr)[listStructPtr->list_index]);
    for (i = listStructPtr->list_index + 1; 
	 i <= listDescriptorPtr->noOfItems - 1; i++)
	(listDescriptorPtr->listPtr)[i-1] = (listDescriptorPtr->listPtr)[i];
    if (ui_list_noOfSites-- == 1) { /* last site deleted */
	(listDescriptorPtr->listPtr)[0] = XtNewString("*** no sites");
    XawListChange(listDescriptorPtr->listWidget, 
		  listDescriptorPtr->listPtr, 
		  1, 0, True);
    } else
	XawListChange(listDescriptorPtr->listWidget, 
		      listDescriptorPtr->listPtr, 
		      --listDescriptorPtr->noOfItems, 0, True);
}


/*****************************************************************************
  FUNCTION : ui_edit_displayFType

  PURPOSE  : display given ftype 
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

static void  ui_edit_displayFType(char *name, 
	struct SimpleListType *listDescriptorPtr)

{
    if (krui_setFTypeEntry(name)) {
	sprintf(ui_edit_ftypeName,   "%s", krui_getFTypeName());
	sprintf(ui_edit_actFuncName, "%s", krui_getFTypeActFuncName());
	sprintf(ui_edit_outFuncName, "%s", krui_getFTypeOutFuncName());
	ui_list_buildList(listDescriptorPtr);

	ui_xSetString(ui_edit_ftypeNameWidget , ui_edit_ftypeName);
	ui_xSetLabel(ui_edit_actFuncNameWidget, ui_edit_actFuncName);
	ui_xSetLabel(ui_edit_outFuncNameWidget, ui_edit_outFuncName);

	XawListChange(listDescriptorPtr->listWidget, 
		      listDescriptorPtr->listPtr, 	
		      listDescriptorPtr->noOfItems, 0, True);
    } else {
	ui_xSetString(ui_edit_ftypeNameWidget , "");
	ui_xSetLabel(ui_edit_actFuncNameWidget, "");
	ui_xSetLabel(ui_edit_outFuncNameWidget, "");
	
	listDescriptorPtr->noOfItems = 0;
	ui_xAddListEntry(listDescriptorPtr, "*** no sites ***");
	ui_list_noOfSites = 0;
	XawListChange(listDescriptorPtr->listWidget, 
		      listDescriptorPtr->listPtr, 	
		      listDescriptorPtr->noOfItems, 0, True);
    }
}


/*****************************************************************************
  FUNCTION : ui_edit_displayFirstFType

  PURPOSE  : display first ftype 
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void  ui_edit_displayFirstFType (struct SimpleListType *listDescriptorPtr)

{
    if (krui_setFirstFTypeEntry()) {
	sprintf(ui_edit_ftypeName,   "%s", krui_getFTypeName());
	ui_edit_displayFType(ui_edit_ftypeName, listDescriptorPtr);
    }
}


/*****************************************************************************
  FUNCTION : ui_edit_ftypeAddSite

  PURPOSE  : popup site list
             if a site was choosen add it to the list
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void  ui_edit_ftypeAddSite (Widget w, 
	struct SimpleListType *listDescriptorPtr, caddr_t call)

{ 
    static struct SimpleListType listSites;

    sprintf(listSites.msg,"%s","Select site to add:");
    listSites.unitPtr  = NULL;
    listSites.widgetPtr= w;
    listSites.listType = UI_LIST_SITE;
    ui_info_displayList(w, (struct SimpleListType *) &listSites, NULL);
    if (ui_list_returnIndex != XAW_LIST_NONE) {
	if (ui_list_noOfFTypeSites++ == 0) {
	    /* first entry is "*** no sites ***" */
	    /* replace the first dummy entry     */
	    free((listDescriptorPtr->listPtr)[0]);
	    (listDescriptorPtr->listPtr)[0] = 
		XtNewString(ui_list_returnName);
	} else
	    ui_xAddListEntry(listDescriptorPtr, ui_list_returnName);
	XawListChange(listDescriptorPtr->listWidget, listDescriptorPtr->listPtr, 
		      listDescriptorPtr->noOfItems, 0, True);
    }
}


/*****************************************************************************
  FUNCTION : ui_edit_ftypeDeleteSite

  PURPOSE  : remove highlighted site from list
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void  ui_edit_ftypeDeleteSite (Widget w, 
	struct SimpleListType *listDescriptorPtr, caddr_t call)

{
    XawListReturnStruct   *listStructPtr;
    int                   i;

    listStructPtr = XawListShowCurrent(listDescriptorPtr->listWidget);

    if ((listStructPtr->list_index == XAW_LIST_NONE) OR
	(ui_list_noOfFTypeSites <= 0))
	return; /* no site selected */

    /* remove this entry from the array */
    free((listDescriptorPtr->listPtr)[listStructPtr->list_index]);
    for (i = listStructPtr->list_index + 1; 
	 i <= listDescriptorPtr->noOfItems - 1; i++)
	(listDescriptorPtr->listPtr)[i-1] = (listDescriptorPtr->listPtr)[i];
    if (ui_list_noOfFTypeSites-- == 1) { /* last site deleted */
	(listDescriptorPtr->listPtr)[0] = XtNewString("*** no sites");
    XawListChange(listDescriptorPtr->listWidget, 
		  listDescriptorPtr->listPtr, 
		  1, 0, True);
    } else
	XawListChange(listDescriptorPtr->listWidget, 
		      listDescriptorPtr->listPtr, 
		      --listDescriptorPtr->noOfItems, 0, True);
}


/*****************************************************************************
  FUNCTION : ui_edit_chooseFType

  PURPOSE  : display a list of all existingf f-types
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void  ui_edit_chooseFType (Widget  w, 
     struct SimpleListType *listDescriptorPtr, caddr_t call)

{
    static struct SimpleListType listFTypes;

    if (krui_setFirstFTypeEntry()) {
	sprintf(listFTypes.msg,"%s","Choose f-type to edit:");
	listFTypes.unitPtr  = NULL;
	listFTypes.widgetPtr= w;
	listFTypes.listType = UI_LIST_FTYPE_NAME;
	ui_info_displayList(w, (struct SimpleListType *) &listFTypes, NULL);
	sprintf(ui_edit_ftypeName,"%s", ui_list_returnName);
	ui_edit_displayFType(ui_edit_ftypeName, listDescriptorPtr);
    }
}


/*****************************************************************************
  FUNCTION : ui_edit_setFType

  PURPOSE  : update ftype name and functions; sites remain unchanged
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void  ui_edit_setFType (Widget w, 
     struct SimpleListType *listDescriptorPtr, caddr_t call)

{
    char buf[MAX_NAME_LENGTH];

    if (NOT krui_setFTypeEntry(ui_edit_ftypeName)) {
	sprintf(buf,"Invalid f-type: %s", ui_edit_ftypeName);
	ui_confirmOk(buf);
	return;
    }

    ui_xStringFromAsciiWidget(ui_edit_ftypeNameWidget, buf, 
			      MAX_NAME_LENGTH);
    ui_correctName(buf);
    if (strcmp(buf,ui_edit_ftypeName) != 0) {
	strcpy(ui_edit_ftypeName, buf);
	ui_checkError(krui_setFTypeName(ui_edit_ftypeName));
    }
    ui_checkError(krui_setFTypeActFunc(ui_edit_actFuncName));
    ui_checkError(krui_setFTypeOutFunc(ui_edit_outFuncName));
}


/*****************************************************************************
  FUNCTION : ui_edit_newFType

  PURPOSE  : create a new f-type
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void  ui_edit_newFType (Widget w, 
     struct SimpleListType *listDescriptorPtr, caddr_t call)

{
    ui_xStringFromAsciiWidget(ui_edit_ftypeNameWidget, ui_edit_ftypeName,
			      MAX_NAME_LENGTH);
    ui_correctName(ui_edit_ftypeName);

    if (strlen(ui_edit_ftypeName) > 0) {
	ui_checkError(krui_createFTypeEntry(ui_edit_ftypeName,
					    ui_edit_actFuncName,
					    ui_edit_outFuncName,
					    ui_list_noOfFTypeSites,
					    listDescriptorPtr->listPtr));
	if (ui_kernelError < 0)
	    ui_confirmOk("Creation of this f-type failed!");
    } else
	ui_confirmOk("No f-type name given!");
}


/*****************************************************************************
  FUNCTION : ui_edit_deleteFType

  PURPOSE  : delete f-type with the given name 
  RETURNS  : void
  NOTES    :

  UPDATE   : 20.9.1990
******************************************************************************/

void  ui_edit_deleteFType(Widget w, 
     struct SimpleListType *listDescriptorPtr, caddr_t call)

{
    ui_xStringFromAsciiWidget(ui_edit_ftypeNameWidget, ui_edit_ftypeName,
			      MAX_NAME_LENGTH);
    ui_correctName(ui_edit_ftypeName);
    ui_checkError(krui_deleteFTypeEntry(ui_edit_ftypeName));
    if (ui_kernelError < 0) {
	char buf[MAX_NAME_LENGTH];
	sprintf(buf,"Can't delete f-type: %s",ui_edit_ftypeName);
	ui_confirmOk(buf);
    } else
	ui_edit_displayFirstFType(listDescriptorPtr);
}









/* end of file */
/* lines: 570 */

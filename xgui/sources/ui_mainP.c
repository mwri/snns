/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_mainP.c,v $
  SHORTNAME      : mainP.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  PURPOSE        : contains callback routines of the gui window 
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.4.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.26 $ 
  LAST CHANGE    : $Date: 1998/05/20 15:00:40 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <ctype.h>		   
#include <stdio.h>			/* For the Syntax message */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ui.h"

#include <X11/Shell.h>

#include <X11/cursorfont.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/TextSrc.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/AsciiText.h>

#include "kr_ui.h"

#include "ui_info.h"
#include "ui_setup.h"
#include "ui_layer.h"
#include "ui_layerP.h"
#include "ui_file.h"
#include "ui_fileP.h"
#include "ui_netGraph.h"
#include "ui_netUpdate.h"
#include "ui_event.h"
#include "ui_key.h"
#include "ui_confirmer.h"
#include "ui_xWidgets.h"
#include "ui_lists.h"
#include "ui_display.h"
#include "ui_control.h"
#include "ui_controlP.h"
#include "ui_edit.h"
#include "ui_xGraphic.h"
#include "ui_textP.h"
#include "ui_funcdispl.h"
#include "ui_action.h"
#include "ui_result.h"
#include "ui_main.h"
#include "ui_print.h"
#include "ui_color.h"
#include "ui_infoP.h"

#include "ui_mainP.ph"

#ifdef PARAGON_KERNEL
#include "kr_ipmp.h"
#endif


/*****************************************************************************
  FUNCTION : ui_createGC

  PURPOSE  : create the GC and load the graphic font
  RETURNS  : void
  NOTES    : window is any existing window of a widget on the screen used by
             xgui. 
  UPDATE   : 20.9.1990
******************************************************************************/

void ui_xCreateGC (Window window)

{
    int count;
    char buf[255];
    char **fontList;

    ui_fontStruct = XLoadQueryFont(ui_display, ui_dfontName);

    if (ui_fontStruct == 0) {
	printf("Can't find display font specified in command line\n");
	exit(1);
    }else{
	fontList = XListFonts(ui_display, ui_dfontName, 1, &count);
	if (count != 0) { 
	    sprintf (buf, "Display-Font %s loaded\n", *fontList);
	    ui_tw_printMessage(buf);
	}
	XFreeFontNames(fontList);
    }

    ui_gcValues.line_width = 0;
    ui_gcValues.line_style = LineSolid;
    /* Set (or get) the next two attributes, because ui_drawUnit() 
       in the file ui_netGraphics.c need this information */
    ui_gc = XCreateGC(ui_display, window, GCLineWidth | GCLineStyle,
		      &ui_gcValues);
    XSetFunction(ui_display,ui_gc, GXcopy);
    XSetFont(ui_display, ui_gc, ui_fontStruct->fid);

}


/*****************************************************************************
  FUNCTION : ui_printMessage

  PURPOSE  : display a message in the manager panel 
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

void ui_printMessage (char *msgString)

{
    ui_xSetLabel(ui_message, msgString);
    XFlush(ui_display);
}


/*****************************************************************************
  FUNCTION : ui_displayDone

  PURPOSE  : close a xgui display
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

void ui_displayDone (Widget widget, struct Ui_DisplayType *displayPtr, 
     caddr_t call_data)

{
    XtDestroyWidget(XtParent(displayPtr->frameWidget));
    displayPtr->frameWidget = NULL;
    displayPtr->widget      = NULL;
    displayPtr->drawable    = 0;
    ui_displ_freeItem(displayPtr);
}
 

/*****************************************************************************
  FUNCTION : ui_helpDone

  PURPOSE  : close a xgui help window
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

/*ARGSUSED*/
static void ui_helpDone (Widget widget, struct HelpType *helpPtr, caddr_t call_data)

{
    struct HelpType *listPtr;

    if (ui_help_listPtr == helpPtr) {
	ui_help_listPtr     = helpPtr->nextPtr;
	helpPtr->nextPtr    = ui_help_freeListPtr;
	ui_help_freeListPtr = helpPtr;
	XtDestroyWidget(helpPtr->shellWidget);
    } else {
	listPtr = ui_help_listPtr;
	while (listPtr->nextPtr != NULL AND listPtr->nextPtr != helpPtr) 
	    listPtr = listPtr->nextPtr;
	
	if (listPtr->nextPtr == helpPtr) {
	    listPtr->nextPtr    = helpPtr->nextPtr;
	    helpPtr->nextPtr    = ui_help_freeListPtr;
	    ui_help_freeListPtr = helpPtr;
	    XtDestroyWidget(helpPtr->shellWidget);
	}
    }
}


/*****************************************************************************
  FUNCTION : ui_listPopupDone

  PURPOSE  : close a list panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

static void ui_listPopupDone (Widget widget, 
	struct SimpleListType *listDescriptorPtr, caddr_t call_data)

{
    int i;

    XtDestroyWidget(ui_popList);
    ui_pop_exit = TRUE; /* signal to leave temporary event loop */
    XFlush(ui_display);

    /* keep the allocated space of the array, but free all the space
       of valid strings */
    for (i = 1; i<= listDescriptorPtr->noOfItems; i++) 
	free((listDescriptorPtr->listPtr)[i-1]);
    listDescriptorPtr->noOfItems = 0;
}


/*****************************************************************************
  FUNCTION : ui_editPopupDone

  PURPOSE  : close a edit panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

static void ui_editPopupDone (Widget widget, 
	struct SimpleListType *listDescriptorPtr, caddr_t call_data)

{
    int i;

    XtDestroyWidget(ui_popEdit);

    /* keep the allocated space of the array, but free all the space
       of valid strings */
    for (i = 1; i<= listDescriptorPtr->noOfItems; i++) 
	free((listDescriptorPtr->listPtr)[i-1]);
    listDescriptorPtr->noOfItems = 0;
}


/*****************************************************************************
  FUNCTION : ui_popupDone

  PURPOSE  : close other panels and windows
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

/*ARGSUSED*/
void ui_popupDone (Widget widget, int client_data, caddr_t call_data)

{
    char buf[80];
    int  gridSize, i;

    switch (client_data) {
      case UI_POPUP_DUMMY:
	XtDestroyWidget(ui_popDummy);
	break;
      case UI_POPUP_SETUP: 
	ui_setupIsCreated = FALSE;
	gridSize  = 
	    ui_xIntFromAsciiWidget(ui_set_gridWidthWidget);
	if (gridSize >= 1) 
	    ui_set_displayPtr->gridSize = gridSize;
	ui_set_displayPtr->origin.x  = 
	    ui_xIntFromAsciiWidget(ui_set_originXWidget);
	ui_set_displayPtr->origin.y  = 
	    ui_xIntFromAsciiWidget(ui_set_originYWidget);
	ui_set_displayPtr->subNetNo  = 
	    ui_xIntFromAsciiWidget(ui_set_subnetWidget);
	sprintf(buf,"snns-display %d - subnet: %d",
		ui_set_displayPtr->displayNo, 
		ui_set_displayPtr->subNetNo);
	ui_actualZvalue  =  ui_xIntFromAsciiWidget(ui_set_zWidget);
	XStoreName(ui_display, 
		   XtWindow(XtParent(ui_set_displayPtr->frameWidget)), 
		   buf);
	ui_net_completeRefresh(ui_set_displayPtr, UI_LOCAL);
	XtDestroyWidget(ui_popSetup);
	break;
      case UI_POPUP_FILE:   
	ui_fileIsCreated = FALSE;
        ui_filePannelPoppedUp = FALSE;
	XtDestroyWidget(ui_popFile);
	break;
      case UI_POPUP_LAYER:
	ui_layer_setLayerNames();
	XtDestroyWidget(ui_popLayer);
	ui_pop_exit = TRUE; /* signal to leave temporary loop */
	XFlush(ui_display);
	break;
      case UI_POPUP_CONTROL:
	for (i=0; i<UI_NO_LEARN_PARAMS; i++) 
	    ui_learnParameters[i] = ui_xFloatFromAsciiWidget(ui_learnParameterWidgets[i]);
	for (i=0; i<UI_NO_UPDATE_PARAMS; i++) 
	    ui_updateParameters[i] = ui_xFloatFromAsciiWidget(ui_updateParameterWidgets[i]);
	for (i=0; i<UI_NO_INIT_PARAMS; i++) 
	    ui_initParameters[i] = ui_xFloatFromAsciiWidget(ui_initParameterWidgets[i]);
	for (i=0; i<UI_NO_REMAP_PARAMS; i++) 
	    ui_remapParameters[i] = ui_xFloatFromAsciiWidget(ui_remapParameterWidgets[i]);
	ui_controlIsCreated = FALSE;
	XtDestroyWidget(ui_popControl);
	break;
      case UI_POPUP_INFO:
	ui_infoIsCreated = FALSE;
	XtDestroyWidget(ui_popInfo);
	break;
      case UI_POPUP_WEIGHTS_DONE:
	XtDestroyWidget(ui_popInit);
	ui_pop_exit = TRUE; /* signal to leave temporary loop */
	break;
      case UI_POPUP_WEIGHTS_CANCEL:
	jog_low  = (FlintType) ui_xFloatFromAsciiWidget(ui_lowLimitWidget);
	jog_high = (FlintType) ui_xFloatFromAsciiWidget(ui_highLimitWidget);
	jog_correlation = (FlintType) ui_xFloatFromAsciiWidget(ui_correlationLimitWidget);
	XtDestroyWidget(ui_popWeights);
	ui_pop_exit = TRUE; /* signal to leave temporary loop */
	XFlush(ui_display);
        break;
      case UI_POPUP_RESULT:
        XtDestroyWidget(ui_popResult);
        ui_ResultIsCreated = FALSE;
 	break;
      case UI_POPUP_SUBPATTERN:
	if(NO_OF_PATTERN_SETS != 0)ui_rem_getSubPatPanel();
	XtDestroyWidget(ui_popPattern);
	SubPatPanel = 0;
	break;
    }
    ui_printMessage("");
}


/*****************************************************************************
  FUNCTION : ui_popupWeights

  PURPOSE  : popup of random/jog weights
  RETURNS  : void
  NOTES    :

  UPDATE   : 27.5.1990
******************************************************************************/
void ui_popupWeights (Widget button, caddr_t call_data, caddr_t client_data)
{
    Widget      doneButton, form, wrap, label;
    Arg		args[5];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;
    char        buf[20];

    /* Upper left corner will be in the center of the calling button */

    ui_xGetDimensions(button, &width, &height);
    XtTranslateCoords(button, (Position) (width / 2), 
		      (Position) (height / 2), &x, &y);
    
    n = 0;
    XtSetArg(args[n], XtNx, x);	n++;
    XtSetArg(args[n], XtNy, y);	n++;
    
    /* Now create Popup */
    
    ui_popWeights = XtCreatePopupShell("jog weights", transientShellWidgetClass,
				       ui_toplevel, args, n);
    
    wrap = XtCreateManagedWidget("form", formWidgetClass, ui_popWeights,NULL,0);
    form = XtCreateManagedWidget("box", formWidgetClass, wrap,NULL,0);

    label = ui_xCreateLabelItem("low  limit:", form, 11*ui_labelFontWidth, 
				NULL, NULL);

    sprintf(buf,"%g",jog_low);
    ui_lowLimitWidget =	ui_xCreateDialogItem("lowLimit", form, buf, 
					     9*ui_labelFontWidth, label, NULL);

    label = ui_xCreateLabelItem("high limit:", form, 11*ui_labelFontWidth, 
			         ui_lowLimitWidget, NULL);

    sprintf(buf,"%g",jog_high);
    ui_highLimitWidget = ui_xCreateDialogItem("highLimit", form, buf, 
					      9*ui_labelFontWidth, label, 
					      NULL);

    label = ui_xCreateLabelItem("Only jog correlated hidden units ?",form,
				37*ui_labelFontWidth,NULL,ui_lowLimitWidget);
    button = ui_xCreateToggleItem("yes", form, NULL, label, ui_lowLimitWidget);
    ui_xSetToggleState(button, ui_jogCorrFlg);
    XtAddCallback(button, XtNcallback,(XtCallbackProc)ui_jog_toggle_correlation,
		  NULL);

    label = ui_xCreateLabelItem("                   min. correlation:", form, 
				32*ui_labelFontWidth+2, 
				NULL, button);
    sprintf(buf,"%g",jog_correlation);
    ui_correlationLimitWidget = ui_xCreateDialogItem("correlation", form, buf, 
						     9*ui_labelFontWidth, label,
						     button);

    label = ui_xCreateLabelItem("Jog weights now ?",form,37*ui_labelFontWidth,
				NULL,ui_correlationLimitWidget);
    button = ui_xCreateButtonItem("yes", form, label, 
				  ui_correlationLimitWidget);
    XtAddCallback(button, XtNcallback, (XtCallbackProc)ui_rem_setRandomWeights,
		  (XtPointer) ((long)UI_JOG));

    label = ui_xCreateLabelItem("Jog every epoch ?",form,
				37*ui_labelFontWidth,NULL,button);
    button = ui_xCreateToggleItem("yes", form, NULL, label,button);

    ui_xSetToggleState(button, ui_jogFlg);
    XtAddCallback(button, XtNcallback, (XtCallbackProc)ui_jog_on,NULL);

    doneButton = ui_xCreateButtonItem("done", wrap, NULL, form);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_popupDone, 
		  (caddr_t) UI_POPUP_WEIGHTS_CANCEL);

    ui_checkWindowPosition(ui_popWeights);
    XtPopup(ui_popWeights, XtGrabExclusive);
    ui_xDontResizeWidget(ui_popWeights); 

}

/*****************************************************************************
  FUNCTION : ui_jog_on

  PURPOSE  : set the continous weight jogging
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.12.1997
******************************************************************************/
void ui_jog_on(Widget w, caddr_t client_data, caddr_t call_data)
{
    Cardinal n = 0;
    Arg	     args[10];
    Pixel    back, fore;

    ui_jogFlg = !ui_jogFlg;

    /* read the current values from a (any) existing widget */
    XtSetArg(args[n], XtNforeground, &fore); n++;
    XtSetArg(args[n], XtNbackground, &back); n++;
    XtGetValues(ui_showModeLabel, args, n);

    n = 0;
    /* reverse button in the control panel */
    if(ui_jogFlg){
	XtSetArg(args[n], XtNbackground, fore);n++;
	XtSetArg(args[n], XtNforeground, back);n++;
    }else{
	XtSetArg(args[n], XtNbackground, back);n++; 
	XtSetArg(args[n], XtNforeground, fore);n++;
    }
    XtSetValues(jogbutton, args, n);
}



/*****************************************************************************
  FUNCTION : ui_jog_toggle_correlation

  PURPOSE  : set the correlation based weight jogging
  RETURNS  : void
  NOTES    :

  UPDATE   : 08.12.1997
******************************************************************************/
void ui_jog_toggle_correlation(Widget w, caddr_t client_data, caddr_t call_data)
{
    ui_jogCorrFlg = !ui_jogCorrFlg;
}



/*****************************************************************************
  FUNCTION : ui_editPopup

  PURPOSE  : edit popup of f-types
  RETURNS  : void
  NOTES    :

  UPDATE   : 27.8.1990
******************************************************************************/

void ui_editPopup (Widget w, struct SimpleListType *listDescriptorPtr, 
	caddr_t call_data)

{
    Widget      doneButton, abutton, form, actLabel, label, button;
    Arg		args[10];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;

    /* Upper left corner will be in the center of the calling button */

    n = 0;
    XtSetArg(args[n], XtNwidth, &width); n++;
    XtSetArg(args[n], XtNheight, &height); n++;
    XtGetValues(listDescriptorPtr->widgetPtr, args, n);
    XtTranslateCoords(listDescriptorPtr->widgetPtr,
		      (Position) (width / 2), (Position) (height / 2),
		      &x, &y);

    n = 0;
    XtSetArg(args[n], XtNx, x);	n++;
    XtSetArg(args[n], XtNy, y);	n++;

    /* Now create Popup */

    ui_popEdit = XtCreatePopupShell("edit f-types", transientShellWidgetClass, 
				    ui_toplevel, args, n);

    form = XtCreateManagedWidget("form", formWidgetClass, ui_popEdit,NULL,ZERO);
    
    label = ui_xCreateLabelItem("msg", form, 14*ui_labelFontWidth, NULL, NULL);
    ui_xSetLabel(label, "Edit f-type");
    ui_xSetResize(label, TRUE);

    ui_edit_ftypeNameWidget = ui_xCreateDialogItem("name", form, "", 
						   25*ui_labelFontWidth, 
						   label, NULL);

    button = ui_xCreateButtonItem("select", form, NULL, label);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_selectFunction, 
		  (caddr_t) UI_EDIT_ACT_FUNC);

    actLabel = ui_xCreateLabelItem("actFunc", form, 11*ui_labelFontWidth, 
				   button, label);
    ui_xSetLabel(actLabel, "act. func.:");

    ui_edit_actFuncNameWidget = ui_xCreateLabelItem("actFuncName", form, 
						    25*ui_labelFontWidth, 
						    actLabel, label);
    ui_xSetResize(ui_edit_actFuncNameWidget, TRUE);
    ui_xSetLabel(ui_edit_actFuncNameWidget,"");
	
    button = ui_xCreateButtonItem("select", form, NULL, actLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_selectFunction, 
		  (caddr_t) UI_EDIT_OUT_FUNC);

    label = ui_xCreateLabelItem("outFunc", form, 11*ui_labelFontWidth, 
				button, actLabel);
    ui_xSetLabel(label, "out. func.:");

    ui_edit_outFuncNameWidget = ui_xCreateLabelItem("outFuncName", form, 
						    25*ui_labelFontWidth, 
						    label, actLabel);
    ui_xSetResize(label, TRUE);
    ui_xSetLabel(ui_edit_outFuncNameWidget,"");

    abutton =
	ui_xCreateButtonItem("add", form, NULL, label);
    XtAddCallback(abutton, XtNcallback, (XtCallbackProc) ui_edit_ftypeAddSite, 
		  (caddr_t) listDescriptorPtr);

    button =
	ui_xCreateButtonItem("delete", form, abutton, label);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_ftypeDeleteSite, 
		  (struct SimpleListType *) listDescriptorPtr);
    
    n = 0; 
    XtSetArg(args[n], XtNfromHoriz, button ); n++;
    XtSetArg(args[n], XtNfromVert , label   ); n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop ); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop ); n++;

    listDescriptorPtr->listWidget =
	XtCreateManagedWidget("list", listWidgetClass, form, args, n);
    ui_list_buildList(listDescriptorPtr);
    ui_xSetResize(listDescriptorPtr->listWidget, TRUE);
   /* XtAddCallback(listDescriptorPtr->listWidget, XtNcallback, 
		  ui_edit_listSetSite, UI_EDIT_FTYPE);
		  */
   
    doneButton = 
	ui_xCreateButtonItem("done", form, NULL, 
			     listDescriptorPtr->listWidget);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_editPopupDone, 
		  (struct SimpleListType *) listDescriptorPtr);
 
    button =
	ui_xCreateButtonItem("choose", form, doneButton, 
			     listDescriptorPtr->listWidget);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_chooseFType, 
		  (struct SimpleListType *) listDescriptorPtr);

    button =
	ui_xCreateButtonItem("set", form, button, 
			     listDescriptorPtr->listWidget);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_setFType, 
		  (struct SimpleListType *) listDescriptorPtr);

    button =
	ui_xCreateButtonItem("new", form, button, 
			     listDescriptorPtr->listWidget);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_newFType, 
		  (struct SimpleListType *) listDescriptorPtr);

    button =
	ui_xCreateButtonItem("delete", form, button, 
			     listDescriptorPtr->listWidget);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_deleteFType, 
		  (struct SimpleListType *) listDescriptorPtr);


    ui_edit_displayFirstFType(listDescriptorPtr);

    XawFormDoLayout(form, True);
    ui_checkWindowPosition(ui_popEdit);
    XtPopup(ui_popEdit, XtGrabExclusive);
    ui_xDontResizeWidget(ui_popEdit); 
}


/*****************************************************************************
  FUNCTION : ui_editSitePopup

  PURPOSE  : edit popup (sites)
  RETURNS  : void
  NOTES    :

  UPDATE   : 27.9.1990
******************************************************************************/

void ui_editSitePopup (Widget w, struct SimpleListType *listDescriptorPtr, 
	caddr_t call_data)

{
    Widget      doneButton, form, actLabel, label, button, sbutton;
    Arg		args[10];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;
    char        *name, *func;

    /* Upper left corner will be in the center of the calling button */

    n = 0;
    XtSetArg(args[0], XtNwidth, &width); n++;
    XtSetArg(args[1], XtNheight, &height); n++;
    XtGetValues(listDescriptorPtr->widgetPtr, args, n);
    XtTranslateCoords(listDescriptorPtr->widgetPtr,
		      (Position) (width / 2), (Position) (height / 2),
		      &x, &y);

    n = 0;
    XtSetArg(args[n], XtNx, x);	n++;
    XtSetArg(args[n], XtNy, y);	n++;

    /* Now create Popup */

    ui_popEdit = 
	XtCreatePopupShell("edit sites", transientShellWidgetClass, ui_toplevel,
			   args, n);

    form = XtCreateManagedWidget("form", formWidgetClass, ui_popEdit,NULL,ZERO);

    label = ui_xCreateLabelItem("msg", form, 14*ui_labelFontWidth, NULL, NULL);
    ui_xSetResize(label, TRUE);
    ui_xSetLabel (label, "Edit site");

    ui_edit_siteNameWidget = ui_xCreateDialogItem("name", form, "", 
						  25*ui_labelFontWidth, 
						  label, NULL);

    button = ui_xCreateButtonItem("select", form, NULL, label);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_selectFunction,
		  (caddr_t) UI_EDIT_SITE_FUNC);
       
    actLabel = ui_xCreateLabelItem("SiteFunc", form, 11*ui_labelFontWidth, 
				   button, label);
    ui_xSetLabel(actLabel, "site func.:");

    ui_edit_siteFuncNameWidget = ui_xCreateLabelItem("name", form, 
						     25*ui_labelFontWidth, 
						     actLabel, label);
    ui_xSetResize(ui_edit_siteFuncNameWidget, TRUE);

    doneButton = ui_xCreateButtonItem("done", form, NULL, button);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_editPopupDone, 
		  (struct SimpleListType *) listDescriptorPtr);
 
    sbutton = ui_xCreateButtonItem("set", form, doneButton, button);
    XtAddCallback(sbutton, XtNcallback, (XtCallbackProc) ui_edit_setSite, 
		  (struct SimpleListType *) listDescriptorPtr);

    n = 0; 
    XtSetArg(args[n], XtNfromHoriz, NULL); n++;
    XtSetArg(args[n], XtNfromVert , sbutton);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;

    listDescriptorPtr->listWidget =
	XtCreateManagedWidget("list", listWidgetClass, form, args, n);
    ui_list_buildList(listDescriptorPtr);
    XtAddCallback(listDescriptorPtr->listWidget, XtNcallback, 
		  (XtCallbackProc) ui_edit_listSetSite, (caddr_t) UI_EDIT_SITE);
    ui_xSetResize(listDescriptorPtr->listWidget, TRUE);
    
    if (krui_getFirstSiteTableEntry(&name, &func)) 
	XawListHighlight(listDescriptorPtr->listWidget, 1);
    /* name is NULL, if there is no first site ! */
    ui_edit_setCurrentSite(name);

    button =
	ui_xCreateButtonItem("new", form, listDescriptorPtr->listWidget, sbutton);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_newSite, 
		  (struct SimpleListType *) listDescriptorPtr);

    button =
	ui_xCreateButtonItem("delete", form, button, sbutton);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_edit_deleteSite, 
		  (struct SimpleListType *) listDescriptorPtr);
    
    if (krui_getFirstSiteTableEntry(&name, &func)) {
	sprintf(ui_edit_siteName,"%s",name);
	sprintf(ui_edit_siteFuncName,"%s",func);
	ui_xSetLabel(ui_edit_siteFuncNameWidget, ui_edit_siteFuncName);
	ui_xSetString(ui_edit_siteNameWidget, ui_edit_siteName);
    }

    ui_checkWindowPosition(ui_popEdit);
    XtPopup(ui_popEdit, XtGrabExclusive);
    ui_xDontResizeWidget(ui_popEdit); 
}


/*****************************************************************************
  FUNCTION : ui_listPopup

  PURPOSE  : popup a list panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 27.8.1990
******************************************************************************/

void ui_listPopup (struct SimpleListType *listDescriptorPtr, Position x, 
	Position y)

{
    Widget      doneButton, form, label;
    char        buf[80], title[80];
    Arg		args[6];
    Cardinal	n;
    XEvent      event;

    /* Upper left corner will be in the center of the calling button */

    n = 0;
    XtSetArg(args[n], XtNx, x);	n++;
    XtSetArg(args[n], XtNy, y);	n++;

    /* Now create Popup */

    sprintf (title, listDescriptorPtr->msg);
    title[0] = tolower(title[0]);  
    ui_popList = 
	XtCreatePopupShell(title, transientShellWidgetClass, ui_toplevel,
                           args, n);

    form = 
	XtCreateManagedWidget("form", formWidgetClass, ui_popList, 
			      NULL, ZERO);

    label = ui_xCreateLabelItem("msg", form, 80*ui_labelFontWidth, NULL, NULL);
    ui_xSetResize(label, TRUE);
    ui_xSetLabel(label, listDescriptorPtr->msg);

    if ((listDescriptorPtr->unitPtr != NULL) AND 
	(listDescriptorPtr->unitPtr->no != ZERO)) {
	label = 
	    ui_xCreateLabelItem("msg", form, 80*ui_labelFontWidth, NULL, label);
	ui_xSetResize(label, TRUE);
	sprintf(buf,"unit #%d - %s", 
		listDescriptorPtr->unitPtr->no, 
		listDescriptorPtr->unitPtr->name);
	ui_xSetLabel(label, buf);
    }

    n = 0; 
    XtSetArg(args[n], XtNfromHoriz, NULL); n++;
    XtSetArg(args[n], XtNfromVert , label);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;

    listDescriptorPtr->listWidget =
	XtCreateManagedWidget("list", listWidgetClass, form, args, n);
    ui_list_buildList(listDescriptorPtr);
    XtAddCallback(listDescriptorPtr->listWidget, XtNcallback, 
		  (XtCallbackProc) ui_list_setUnitValue, 
		  (struct SimpleListType  *) listDescriptorPtr);
    ui_list_returnIndex = XAW_LIST_NONE;

    if ((listDescriptorPtr->unitPtr != NULL) AND 
	(listDescriptorPtr->unitPtr->no != ZERO)) {
      if( listDescriptorPtr->unitPtr->no == ui_sourceUnit.no){
        if(listDescriptorPtr->listType == UI_LIST_ACT_FUNC){
	  XtAddCallback(listDescriptorPtr->listWidget, XtNcallback, 
		        (XtCallbackProc) ui_drawFunction,"Source Act");
        }else if(listDescriptorPtr->listType == UI_LIST_OUT_FUNC){
	  XtAddCallback(listDescriptorPtr->listWidget, XtNcallback, 
		        (XtCallbackProc) ui_drawFunction,"Source Out");
        }
      }else {
        if(listDescriptorPtr->listType == UI_LIST_ACT_FUNC){
	  XtAddCallback(listDescriptorPtr->listWidget, XtNcallback, 
		        (XtCallbackProc) ui_drawFunction,"Target Act");
        }else if(listDescriptorPtr->listType == UI_LIST_OUT_FUNC){
	  XtAddCallback(listDescriptorPtr->listWidget, XtNcallback, 
		        (XtCallbackProc) ui_drawFunction,"Target Out");
        }
      }
    }

    doneButton = 
	ui_xCreateButtonItem("done", form, NULL, listDescriptorPtr->listWidget);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_listPopupDone, 
		  (struct SimpleListType  *) listDescriptorPtr);

    ui_checkWindowPosition(ui_popList);
    XtPopup(ui_popList, XtGrabExclusive);
    ui_xDontResizeWidget(ui_popList); 

    ui_pop_exit = FALSE;
    while (NOT ui_pop_exit) { /* exit after pressing the DONE button */
	XtAppNextEvent(ui_appContext, &event);
	(void) XtDispatchEvent(&event);
    }
}


/*****************************************************************************
  FUNCTION : ui_popupSetup

  PURPOSE  : popup a setup panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 27.5.1990
******************************************************************************/

static void ui_popupSetup (Widget button, struct Ui_DisplayType *displayPtr, 
	caddr_t call_data)

{
    Widget      doneButton;
    Arg		args[5];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;

    /* Upper left corner will be in the center of the calling button */

    if (NOT ui_setupIsCreated) {

	n = 0;
	XtSetArg(args[0], XtNwidth, &width); n++;
	XtSetArg(args[1], XtNheight, &height); n++;
	XtGetValues(button, args, n);
	XtTranslateCoords(button, (Position) (width / 2), 
			  (Position) (height / 2),
			  &x, &y);

	n = 0;
	XtSetArg(args[n], XtNx, x);	n++;
	XtSetArg(args[n], XtNy, y);	n++;

	/* Now create Popup */

	ui_popSetup = 
	    XtCreatePopupShell("setup", transientShellWidgetClass, 
			       ui_toplevel, args, n);

	ui_setupBox = 
	    XtCreateManagedWidget("form", formWidgetClass, ui_popSetup, 
				  NULL, ZERO);

	ui_xCreateSetupPanel(ui_setupBox, displayPtr);

	doneButton = 
	    ui_xCreateButtonItem("done", ui_setupBox, NULL, ui_setupPanel);
	XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_popupDone, 
		(caddr_t) UI_POPUP_SETUP);

	ui_setupSliderMessage = 
	    ui_xCreateLabelItem("", ui_setupBox, 30*ui_labelFontWidth, 
				doneButton, ui_setupPanel);
	ui_checkWindowPosition(ui_popSetup);
	XtPopup(ui_popSetup, XtGrabExclusive);
        ui_xDontResizeWidget(ui_popSetup); 
    }
}



/*****************************************************************************
  FUNCTION : ui_displayFilePanel

  PURPOSE  : popup the file panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 22.5.1990
******************************************************************************/

void ui_displayFilePanel (Widget w, Widget button, caddr_t call_data)

{
    Widget      doneButton;
    Arg		args[5];
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;

    if (ui_filePannelPoppedUp){
       XRaiseWindow (XtDisplay (ui_popFile), XtWindow (ui_popFile));
       return;
    }
    ui_filePannelPoppedUp = TRUE;

    /* Upper left corner will be in the center of the calling button */
    n = 0;
    XtSetArg(args[0], XtNwidth, &width); n++;
    XtSetArg(args[1], XtNheight, &height); n++;
    XtGetValues(button, args, n);

    XtTranslateCoords(button, (Position) 0, (Position) (height),
		      &x, &y);

    n = 0;
    XtSetArg(args[n], XtNx, x);	n++;
    XtSetArg(args[n], XtNy, y);	n++;

    /* Now create Popup */
    ui_popFile = XtCreatePopupShell("SNNS file browser", 
				    transientShellWidgetClass, button, args, n);
    ui_fileBox = XtCreateManagedWidget("box", boxWidgetClass, ui_popFile, 
				       NULL, ZERO);
    ui_xCreateFilePanel(ui_fileBox);

    doneButton = ui_xCreateButtonItem("done", ui_fileBox, NULL, ui_filePanel);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_popupDone, 
		  (caddr_t) UI_POPUP_FILE);

    ui_fileIsCreated = TRUE;
    ui_checkWindowPosition(ui_popFile);
    XtPopup(ui_popFile, XtGrabNone);
    XtAddEventHandler(ui_fileBox,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
}


/*****************************************************************************
  FUNCTION : ui_displayLayerPanel

  PURPOSE  : popup the layer panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 22.5.1990
******************************************************************************/

void ui_displayLayerPanel (Position x, Position y)

{
    Widget      doneButton, layerBox;
    Arg		args[5];
    Cardinal	n;
    XEvent      event;

    /* Upper left corner will be in the center of the calling button */

    n = 0;
    XtSetArg(args[n], XtNx, x);	n++;
    XtSetArg(args[n], XtNy, y);	n++;
    ui_popLayer = XtCreatePopupShell("layers", transientShellWidgetClass, 
				     ui_toplevel, args, n);
    layerBox = 	XtCreateManagedWidget("box", boxWidgetClass, ui_popLayer, 
				      NULL, ZERO);
    ui_xCreateLayerPanel(layerBox);

    doneButton = ui_xCreateButtonItem("done", layerBox, NULL, NULL);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_popupDone,
		  (caddr_t) UI_POPUP_LAYER);
    
    ui_checkWindowPosition(ui_popLayer);
    XtAddEventHandler(ui_popLayer,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    XtPopup(ui_popLayer, XtGrabExclusive);
    ui_xDontResizeWidget(ui_popLayer); 

    ui_pop_exit = FALSE;
    while (NOT ui_pop_exit) { /* exit after pressing the DONE button */
	XtAppNextEvent(ui_appContext, &event);
	(void) XtDispatchEvent(&event);
    }
}


/*****************************************************************************
  FUNCTION : ui_refresh

  PURPOSE  : Refresh of a network graphic window case of an expose event
  RETURNS  : void
  NOTES    :

  UPDATE   : 15.5.1990
******************************************************************************/
static void ui_refresh (Widget w, struct Ui_DisplayType *displayPtr, 
			XEvent *event)

{
    switch ((*event).type) {
      case Expose:
	if (event->xexpose.count == 0) {

	    ui_xGetDimensions(displayPtr->widget, 
			      &(displayPtr->width), 
			      &(displayPtr->height));
    
	    displayPtr->unitsInX = 
		displayPtr->width  / displayPtr->gridSize;
	    displayPtr->unitsInY = 
		displayPtr->height / displayPtr->gridSize;
	    ui_net_completeRefresh(displayPtr, UI_LOCAL); 
	}
	break;
      case ConfigureNotify:
	ui_xGetDimensions(displayPtr->widget, 
			  &(displayPtr->width), 
			  &(displayPtr->height));
	displayPtr->unitsInX = 
	    displayPtr->width  / displayPtr->gridSize;
	displayPtr->unitsInY = 
	    displayPtr->height / displayPtr->gridSize;
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_displayGraphic

  PURPOSE  : display a graphic window
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.4.1990
******************************************************************************/
void ui_displayGraphic (Widget button, struct Ui_DisplayType *displPtr, 
			caddr_t call_data)

{
    Widget         fbutton;
    Arg		   args[10];
    Cardinal	   n;
    char           buf[40];
    XtTranslations trans;
   
    struct Ui_DisplayType  *displayPtr;

    if (displPtr == NULL) {
	/* call from the GUI menue */
	if ((displayPtr = ui_displ_getFreeItem()) == NULL) {
	    ui_confirmOk("No more memory for displays available!");
	    return;
	}
    } else
	/* call from ui_cfg_load() */
	displayPtr = displPtr;

    sprintf(buf,"SNNS display %d - subnet: %d",
	    displayPtr->displayNo, displayPtr->subNetNo);

    n = 0;
    ui_popCanvas = XtCreatePopupShell(buf, topLevelShellWidgetClass, 
				      ui_toplevel, args, n);

    /* now create the display structure to held data */
    displayPtr->frameWidget = XtCreateManagedWidget("form", formWidgetClass, 
						    ui_popCanvas, NULL, ZERO);

    fbutton = ui_xCreateButtonItem("done", displayPtr->frameWidget, NULL, NULL);
    XtAddCallback(fbutton, XtNcallback, (XtCallbackProc) ui_displayDone, 
		  (struct Ui_DisplayType  *) displayPtr);

    fbutton = ui_xCreateButtonItem("setup", displayPtr->frameWidget, fbutton, 
				   NULL);
    XtAddCallback(fbutton, XtNcallback, (XtCallbackProc) ui_popupSetup, 
		  (struct Ui_DisplayType *) displayPtr);

    fbutton = ui_xCreateToggleItem("freeze", displayPtr->frameWidget, NULL, 
				   fbutton, NULL);
    ui_xSetToggleState(fbutton, displayPtr->frozen);
    XtAddCallback(fbutton, XtNcallback, (XtCallbackProc) ui_displ_freezeDisplay,
		  (struct Ui_DisplayType *) displayPtr);

    n = 0;
    XtSetArg(args[n], XtNwidth,  displayPtr->width); n++;
    XtSetArg(args[n], XtNheight, displayPtr->height); n++;
    XtSetArg(args[n], XtNfromVert, fbutton); n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainRight); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainBottom); n++;
    displayPtr->widget = XtCreateManagedWidget("display", boxWidgetClass, 
					       displayPtr->frameWidget, args,n);
    trans = XtParseTranslationTable("Ctrl<Btn1Down>: XawPositionSimpleMenu(popmenu) MenuPopup(popmenu)");
    
    XtAugmentTranslations(displayPtr->widget, trans);                     
    

    XtAddEventHandler(displayPtr->widget, ExposureMask, GraphicsExpose, 
		      (XtEventHandler) ui_refresh, 
		      (struct Ui_DisplayType *) displayPtr);
    XtAddEventHandler(ui_popCanvas, 
		      StructureNotifyMask | SubstructureNotifyMask, FALSE, 
		      (XtEventHandler) ui_refresh, 
		      (struct Ui_DisplayType *) displayPtr);
    XtAddEventHandler(displayPtr->widget, 
		      ButtonPressMask | ButtonMotionMask | ButtonReleaseMask 
		      | EnterWindowMask | LeaveWindowMask,
		      FALSE, (XtEventHandler) ui_mw_eventProc, 
		      (struct Ui_DisplayType *) displayPtr);
    XtAddEventHandler(displayPtr->widget, 
		      KeyPressMask, FALSE, (XtEventHandler) ui_can_KEventProc, 
		      (struct Ui_DisplayType *) displayPtr);
    XtAddEventHandler(ui_popCanvas, StructureNotifyMask,
		      FALSE, (XtEventHandler) ui_can_MapEventProc, 
		      (struct Ui_DisplayType *) displayPtr);

    ui_checkWindowPosition(ui_popCanvas);
    XtPopup(ui_popCanvas, XtGrabNone);

    displayPtr->drawable = XtWindow(displayPtr->widget);

    ui_xToggleBackingStore(TRUE, ui_display, displayPtr->drawable);

    XSelectInput(ui_display, displayPtr->drawable, 
		 ExposureMask | PointerMotionMask |
		 ButtonPressMask | ButtonMotionMask | ButtonReleaseMask |
		 KeyPressMask | EnterWindowMask | LeaveWindowMask);

    XGrabButton(ui_display, AnyButton, AnyModifier, displayPtr->drawable, TRUE,
		ButtonPressMask | ButtonMotionMask | ButtonReleaseMask,
		GrabModeAsync, GrabModeAsync, None,
		XCreateFontCursor(ui_display, XC_crosshair));
}


/*****************************************************************************
  FUNCTION : ui_textLook

  PURPOSE  : callback. Called when clicked on the LOOK button 
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.5.1990
******************************************************************************/
static void ui_textLook (Widget widget, XtPointer  client_data, 
			 XtPointer call_data)

{
    Widget           w, src;
    XawTextBlock     textBlock;
    XawTextPosition  start, end, found; /* int ! */
    char             sWord[SEARCH_WORD_MAXLEN+1];
    Cardinal         n;
    Arg              args[5];

    w = (Widget) client_data; 
    
    XawTextGetSelectionPos(w, &start, &end);

    if (start != end) {
	if ((end-start+1) > SEARCH_WORD_MAXLEN) 
	    end = start + SEARCH_WORD_MAXLEN;

	n = 0;
	XtSetArg(args[n], XtNtextSource, &src); n++;
	XtGetValues(w, args, n);
	 
	found = XawTextSourceRead(src, (int)start, &textBlock,(int)(end-start));

	textBlock.firstPos = 0;
	textBlock.length   = end - start;
	textBlock.format   = FMT8BIT;
	

	XawTextSetInsertionPoint(w, 0);
	found = XawTextSearch(w, XawsdRight, &textBlock);
	if (found > 0) {
	    strncpy(sWord, textBlock.ptr, (unsigned int) (end - start));
	    sWord[end-start] = '\0';
#ifdef DEBUG
	    sprintf(buf, "Found: <%s>", sWord);
	    ui_printMessage(buf);
#endif
	    XawTextSetInsertionPoint(w, (int) (ui_lengthOfHelp));
	    XawTextSetInsertionPoint(w, (int) (found + textBlock.length));
	    XawTextSetSelection(w, (int) found, 
				(int) (found + textBlock.length));
	    XawTextInvalidate(w, (int) found, 
			      (int) (found + textBlock.length));
	}
    }	
}


/*****************************************************************************
  FUNCTION : ui_textMore

  PURPOSE  : callback. Called when clicked on the MORE button
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.5.1990
******************************************************************************/
static void ui_textMore (Widget widget, XtPointer client_data, 
			 XtPointer call_data)

{
    Widget           w, src;
    XawTextBlock     textBlock;
    XawTextPosition  start, end, found; /* int ! */
    char             sWord[SEARCH_WORD_MAXLEN+1];
    Cardinal         n;
    Arg              args[5];

    w = (Widget) client_data; 
    
    XawTextGetSelectionPos(w, &start, &end);

    if (start != end) {
	if ((end-start+1) > SEARCH_WORD_MAXLEN) 
	    end = start + SEARCH_WORD_MAXLEN;

	n = 0;
	XtSetArg(args[n], XtNtextSource, &src); n++;
	XtGetValues(w, args, n);
	 
	found = XawTextSourceRead(src, (int) start, &textBlock, 
				  (int) (end-start));

	textBlock.firstPos = 0;
	textBlock.length   = end - start;
	textBlock.format   = FMT8BIT;
	
	/* XawTextSetInsertionPoint(w, 0); */
	found = XawTextSearch(w, XawsdRight, &textBlock);
	if (found > 0) {
	    strncpy(sWord, textBlock.ptr, (unsigned int) (end - start));
	    sWord[end-start] = '\0';
#ifdef DEBUG
  	    sprintf(buf, "Found: <%s>", sWord);
	    ui_printMessage(buf);
#endif
	    XawTextSetInsertionPoint(w, (int) (ui_lengthOfHelp));
	    XawTextSetInsertionPoint(w, (int) (found + textBlock.length));
	    XawTextSetSelection(w, (int) found, 
				(int) (found + textBlock.length));

	    XawTextInvalidate(w, (int) found, 
			      (int) (found + textBlock.length));
	}	
    }
}


/*****************************************************************************
  FUNCTION : ui_textKey

  PURPOSE  : searches in the specified ascii widget for a key string
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.5.1990
******************************************************************************/
static void ui_textKey  (Widget w, char *key)

{
    XawTextBlock     textBlock;
    XawTextPosition  found; /* int ! */

    if (strlen(key) < 1) return;

    textBlock.ptr      = key;
    textBlock.firstPos = 0;
    textBlock.length   = strlen(key);
    textBlock.format   = FMT8BIT;
	
    XawTextSetInsertionPoint(w, 0);
    found = XawTextSearch(w, XawsdRight, &textBlock);
    if (found > 0) {
#ifdef DEBUG
	sprintf(buf, "Found: <%s>", key);
	ui_printMessage(buf);
#endif
	XawTextSetInsertionPoint(w, (int) (ui_lengthOfHelp));
	XawTextSetInsertionPoint(w, (int) (found + textBlock.length));
	XawTextSetSelection(w, (int) found, 
			    (int) (found + textBlock.length));
	XawTextInvalidate(w, (int) found, 
			  (int) (found + textBlock.length));
    }	
}


/*****************************************************************************
  FUNCTION : ui_textTopics

  PURPOSE  : callback. Called when clicked on the TOPICS button
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.5.1990
******************************************************************************/
static void ui_textTopics (Widget widget, Widget client_data, caddr_t call_data)

{
    ui_textKey(client_data, "* TOPICS");
}
    

/*****************************************************************************
  FUNCTION : ui_displayControl

  PURPOSE  : display control-panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 26.9.1990
******************************************************************************/
void ui_displayControl (Widget w, caddr_t client_data, caddr_t call_data)

{
    Widget      doneButton, form;

    if (ui_controlIsCreated) {
       XRaiseWindow (XtDisplay (ui_popControl), XtWindow (ui_popControl));
       return;
    }

    ui_popControl = XtCreatePopupShell("SNNS control", topLevelShellWidgetClass,
				      ui_toplevel, NULL, ZERO);

    form = XtCreateManagedWidget("form", formWidgetClass, ui_popControl, 
				 NULL, ZERO);
 
    ui_xCreateControlPanel(form); 
    
    doneButton = ui_xCreateButtonItem("done", form, NULL, ui_controlPanel);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_popupDone, 
		  (caddr_t) UI_POPUP_CONTROL);

    ui_controlMessageWidget = ui_xCreateLabelItem("message",form, 
						 10*ui_labelFontWidth, 
						 doneButton, ui_controlPanel); 
    ui_xSetResize(ui_controlMessageWidget, TRUE);
    ui_xSetLabel(ui_controlMessageWidget, "");

    XtAddEventHandler(form,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    XawFormDoLayout(form, True);
    ui_checkWindowPosition(ui_popControl);
    XtPopup(ui_popControl, XtGrabNone);
    ui_xDontResizeWidget(ui_popControl); 
    ui_file_updateShellLabels();
}


/*****************************************************************************
  FUNCTION : ui_displayInfo

  PURPOSE  : display control-panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 26.9.1990
******************************************************************************/
void ui_displayInfo (Widget w, caddr_t client_data, caddr_t call_data)

{
    Widget      doneButton, form;

    if (ui_infoIsCreated) {
       XRaiseWindow (XtDisplay (ui_popInfo), XtWindow (ui_popInfo));
       return;
    }
	
    ui_popInfo = XtCreatePopupShell("SNNS info", topLevelShellWidgetClass, 
				    ui_toplevel, NULL, ZERO);

    form = XtCreateManagedWidget("form", formWidgetClass, ui_popInfo, 
				 NULL, ZERO);
 
    ui_xCreateInfoPanel(form); 
    
    doneButton = ui_xCreateButtonItem("done", form, NULL, ui_infoPanel);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_popupDone, 
		  (caddr_t) UI_POPUP_INFO);


    XtAddEventHandler(form,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    XawFormDoLayout(form, True);
    ui_checkWindowPosition(ui_popInfo);
    XtPopup(ui_popInfo, XtGrabNone);
    ui_xDontResizeWidget(ui_popInfo); 
    ui_file_updateShellLabels();
}


/*****************************************************************************
  FUNCTION : ui_displayText

  PURPOSE  : display a help window
  RETURNS  : void
  NOTES    : returns false if no help.hdoc found

  UPDATE   : 1.5.1990
******************************************************************************/
void ui_displayText (Widget button, caddr_t client_data, caddr_t call_data)

{
    Widget      doneButton, lookButton, moreButton, topicsButton, form;
    Arg		args[12];
    Cardinal	n;
    char        buf[20];
    char        hdoc[MAX_NAME_LENGTH];
    struct stat *help_size_buf;

    struct HelpType *listPtr;

    sprintf(hdoc, "help.hdoc");
    if (NOT ui_fileExist(hdoc,0)) {
	if (getenv("XGUILOADPATH") != NULL) 
	    sprintf(hdoc, "%s/help.hdoc", getenv("XGUILOADPATH"));
	if (NOT ui_fileExist(hdoc,0)) {
	    ui_confirmOk("Can't display help. <help.hdoc> not found.");
            ui_helpOk = FALSE;
	    return;
	}
    }

    /* getting the length of the helpfile */
    help_size_buf = (struct stat *) malloc(sizeof(struct stat));
    if (help_size_buf == NULL) {
	    ui_confirmOk("No memory available to display a new help window!");
            ui_helpOk = FALSE;
	    return;
    }
    stat (hdoc, help_size_buf);
    ui_lengthOfHelp = help_size_buf->st_size;
    free (help_size_buf);

    /* first get a free help item */
    if (ui_help_freeListPtr == NULL) { /* no free item */
	listPtr = (struct HelpType *) (malloc(sizeof(struct HelpType)));
	if (listPtr == NULL) {
	    ui_confirmOk("No memory available to display a new help window!");
            ui_helpOk = FALSE;
	    return;
	}		
	listPtr->no = ++ui_noOfHelpsUp;
    } else {
	listPtr = ui_help_freeListPtr;
	ui_help_freeListPtr = listPtr->nextPtr;
    }
    /* link it in normal list */
    listPtr->nextPtr = ui_help_listPtr;
    ui_help_listPtr  = listPtr;

    sprintf(buf,"SNNS help %d",listPtr->no);
    listPtr->shellWidget =  XtCreatePopupShell(buf, topLevelShellWidgetClass, 
					       ui_toplevel,
					       NULL, ZERO);

    form = XtCreateManagedWidget("form", formWidgetClass, listPtr->shellWidget, 
				 NULL, ZERO);

    doneButton = ui_xCreateButtonItem("done", form, NULL, NULL);
    XtAddCallback(doneButton, XtNcallback, (XtCallbackProc) ui_helpDone, 
		  (struct HelpType *) listPtr); 

    lookButton = ui_xCreateButtonItem("look", form, doneButton, NULL);

    moreButton = ui_xCreateButtonItem("more", form, lookButton, NULL);

    topicsButton = ui_xCreateButtonItem("topics", form, moreButton, NULL);

    n = 0;
    XtSetArg(args[n], XtNwidth,  575); n++;
    XtSetArg(args[n], XtNheight, 400); n++;
    XtSetArg(args[n], XtNtype, XawAsciiFile); n++;
    XtSetArg(args[n], XtNstring, hdoc); n++;
    XtSetArg(args[n], XtNscrollVertical, XawtextScrollAlways); n++;
    XtSetArg(args[n], XtNfromVert , doneButton);  n++;
    XtSetArg(args[n], XtNfromHoriz, NULL);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft);   n++;
    XtSetArg(args[n], XtNright , XtChainLeft);   n++;
    XtSetArg(args[n], XtNtop   , XtChainTop);    n++;
    XtSetArg(args[n], XtNbottom, XtChainTop);    n++;	
    listPtr->textWidget = XtCreateManagedWidget("textWin", asciiTextWidgetClass,
						form, args, n);

    XtAddCallback(lookButton,   XtNcallback, ui_textLook,   
		  (XtPointer) listPtr->textWidget);
    XtAddCallback(moreButton,   XtNcallback, ui_textMore,   
		  (XtPointer) listPtr->textWidget);
    XtAddCallback(topicsButton, XtNcallback, (XtCallbackProc) ui_textTopics, 
		  (XtPointer) listPtr->textWidget);

    XtAddEventHandler(form,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    XawFormDoLayout(form, True);
    ui_checkWindowPosition(listPtr->shellWidget);
    XtPopup(listPtr->shellWidget, XtGrabNone);
    ui_xDontResizeWidget(listPtr->shellWidget); 
    ui_helpOk = TRUE;
}


/*****************************************************************************
  FUNCTION : ui_displayHelp

  PURPOSE  : show a help window and the first position where the key string
             was found
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/
void  ui_displayHelp (char *key)

{
    if (ui_help_listPtr != NULL) {
	/* there are help windows opened */
	/* get one, which is not iconified */
	/* struct HelpType *listPtr;
	   listOtr = ui_help_listPtr;
	   while ((listPtr != NULL) AND listPtr->unmapped)
	   listPtr = listPtr->nextPtr;
	   if (listPtr == NULL) {
	   } else
	   */
	XMapRaised(ui_display, XtWindow(ui_help_listPtr->shellWidget));
	ui_textKey(ui_help_listPtr->textWidget, key);
    } else {
	ui_displayText(NULL, NULL, NULL); /* popup a window */
        if (ui_helpOk)
	     ui_textKey(ui_help_listPtr->textWidget, key);
    }
}


/*****************************************************************************
  FUNCTION : ui_guiQuit()

  PURPOSE  : leave XGUI
  RETRURNS : void
  NOTES    : erases all temp files

  UPDATE   : 15.5.1992
******************************************************************************/
void ui_guiQuit (Widget widget, XtPointer client_data, XtPointer call_data)

{
    char cmdLine[200];

    if (ui_confirmYes("Do you really want to quit?")) {
	krui_deleteNet();
        sprintf(cmdLine,"/bin/rm -f %s/%s* > /dev/null", ui_tmpDir, ui_praefix);
        system (cmdLine);

	/* free all memory used by the kernel */
	XtDestroyApplicationContext(XtWidgetToApplicationContext(widget));

#ifdef PARAGON_KERNEL
        krip_TermApp();
#endif
        exit(0);
    }
}


/*****************************************************************************
  FUNCTION : ui_guiDoQuit()

  PURPOSE  : leave XGUI
  RETRURNS : void
  NOTES    : erases all temp files

  UPDATE   : 09.01.1995
******************************************************************************/
void ui_guiDoQuit (Widget widget, XtPointer client_data, XtPointer call_data)

{
    char cmdLine[200];

    krui_deleteNet();
    sprintf(cmdLine,"/bin/rm -f %s/%s* > /dev/null", ui_tmpDir, ui_praefix);
    system (cmdLine);

    /* free all memory used by the kernel */
    XtDestroyApplicationContext(XtWidgetToApplicationContext(widget));

#ifdef PARAGON_KERNEL
    krip_TermApp();
#endif
    exit(0);
}



/*****************************************************************************
  FUNCTION : ui_loadFileFromCommandLine

  PURPOSE  : load files from command line
  RETURNS  : void
  NOTES    : load default if no cfg specified

  UPDATE   : 15.6.1992
******************************************************************************/
void ui_loadFileFromCommandLine (void)
{
    int cfg_loaded;

    if (ui_loadNetbyInit) 
       cfg_loaded = ui_file_loadNet((Widget) ZERO, (XtPointer) NULL, 
				    (caddr_t) NULL);
    if (ui_loadPatbyInit) 
       ui_file_loadPatterns((Widget) ZERO, (XtPointer) NULL, (caddr_t) NULL);

    if(cfg_loaded)
	return;

    if (ui_loadCfgbyInit) 
       ui_file_loadConfiguration((Widget) ZERO, (XtPointer)NULL, (caddr_t)NULL);
    else
        ui_file_loadDefaultConfiguration(ui_pathname);
}


/*****************************************************************************
  FUNCTION : ui_chkFileName

  PURPOSE  : check extention for a given filename
  RETURNS  : int
  NOTES    :

  UPDATE   : 15.6.1992
******************************************************************************/
static int ui_chkFileName (char *s1, char *s2)

{
    return (!strcmp(s1+strlen(s1)-4, s2));
}


/*****************************************************************************
  FUNCTION : ui_showHelpMsg

  PURPOSE  : displays a help message
  RETURNS  : void
  NOTES    :

  UPDATE   : 15.6.1992
******************************************************************************/
static void ui_showHelpMsg (char *call)

{
    fprintf(stderr, "\n\n       Stuttgart Neural Network Simulator  V4.2\n\n");
    fprintf(stderr, 
	    "  usage  %s [netfile] [patternfile] [configuration] [options]\n\n",
	    call);
    fprintf(stderr, 
	    "  where options are -font  <name>  font for the simulator\n");  
    fprintf(stderr, 
	    "                    -dfont <name>  font for the displays\n");  
    fprintf(stderr, 
        "                    -mono          black & white on color screens \n");
    fprintf(stderr, "                    -help          this help\n");  
    fprintf(stderr, "\n\n");
}


/*****************************************************************************
  FUNCTION : ui_parseCmdLine

  PURPOSE  : parse command line
  RETURNS  : void
  NOTES    : sets options and filenames

  UPDATE   : 15.6.1992
******************************************************************************/
void ui_parseCmdLine (int noParams, char *params[])

{
     int i;
     char *call;

     call = *params;
     
     strcpy (ui_dfontName, "6x12");
     ui_labelFontWidth = 7;
     hasCmdlineFont = TRUE;

     i = 0;
     while(++i != noParams) {
         if (*params[i] == '-') {
             if ((strcmp(params[i], "-dfont")) == 0) {
                 if(params[++i] == NULL){
                     fprintf(stderr,"%s: No display font specified\n",call);
                     exit(1);
                 }else{
                     strcpy (ui_dfontName, params[i]);
                 }
	     } else if ((strcmp(params[i], "-mono")) == 0) {
		 ui_monoOnColorScreen = TRUE;
	     } else if ((strcmp(params[i], "-help") == 0) 
			OR (strcmp(params[i], "-h") == 0)) {
		 ui_showHelpMsg(call);
		 exit(1);
	     } else if ((strcmp(params[i], "-font")) == 0) {
                 if(params[++i] == NULL){
                     fprintf(stderr,"%s: No label font specified\n",call);
                     exit(1);
		 }
	     }
	     else{
		 fprintf(stderr,"%s: unrecognized command line option %s\n",
			 call,params[i]);
		 exit(1);
	     }
         } else if (ui_chkFileName(params[i], ".net")) {
	     strncpy(ui_filenameNET, params[i], strlen(params[i])-4);
	     ui_loadNetbyInit = TRUE;
	 } else if (ui_chkFileName(params[i], ".pat")) {
	     strncpy(ui_filenamePAT, params[i], strlen(params[i])-4);
	     ui_loadPatbyInit = TRUE;
	 } else if (ui_chkFileName(params[i], ".cfg")) {
	     strncpy(ui_filenameCFG, params[i], strlen(params[i])-4);
	     ui_loadCfgbyInit = TRUE;
	     ui_loadCorrespondingCfg = FALSE;
	 }
	 else{
	     printf("unrecognized file type in command line\n");
	     exit(1);
	 }
     }
}
/* end of file */

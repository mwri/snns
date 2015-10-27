/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_info.c,v $
  SHORTNAME      : info.c
  SNNS VERSION   : 4.2

  PURPOSE        : creates the widget containing all information of one link
  NOTES          : is called only during initialisation

  AUTHOR         : Tilman Sommer
  DATE           : 1.5.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:26 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>

#include "ui.h"

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/List.h>

#include "ui_infoP.h"
#include "ui_xWidgets.h"
#include "ui_lists.h"
#include "ui_main.h"  /* ui_labelFontWidth */
#include "ui_mainP.h"
#include "ui_funcdispl.h"
#include "ui_netUpdate.h"
#include "ui_setup.h"

#include "ui_info.ph"


/*****************************************************************************
  FUNCTION : ui_xCreateInfoPanel

  PURPOSE  : creates a form widget for user input of unit data
  RETURNS  : void
  NOTES    : some constants are used (see below) to get a relativ positioning
             of the items. This is VERY useful after changings.

  UPDATES  :
*****************************************************************************/

void ui_xCreateInfoPanel(Widget parent)
	/* the parent widget of the new form widget */

{
    /* tabs for data fields and small buttons */

    Widget      noLabel, ioLabel, actLabel, iactLabel, outLabel, biasLabel;
    Widget      snLabel, ftypeLabel;
    Widget      srcLabel, tarLabel, linkLabel, tarFuncLabel, srcFuncLabel;
    Widget      titleLabel, button, menu;
    Widget      buttonTar, buttonSrc, mItem;

    static struct InfoCallLayerStruct  srcCallLayer, tarCallLayer;

    static struct SimpleListType  listActFuncTar, listOutFuncTar;
    static struct SimpleListType  listFTypeTar, listIOTypeTar;
    static struct SimpleListType  listActFuncSrc, listOutFuncSrc;
    static struct SimpleListType  listFTypeSrc, listIOTypeSrc;
    static struct SimpleListType  listDeleteSite, listAddSite; 

    int  titleWidth  =  6 * ui_labelFontWidth;
    int  noWidth     =  5 * ui_labelFontWidth;
    int  stWidth     =  2 * ui_labelFontWidth + 3;
    int  numberWidth =  8 * ui_labelFontWidth;
    int  weightWidth =  9 * ui_labelFontWidth;
    int  nameWidth   = 20 * ui_labelFontWidth; 


/***************************************************************************/

    ui_infoPanel = 
	XtCreateManagedWidget("iPanel", formWidgetClass, parent, NULL, ZERO);


    /* build menus: ioType, fType, siteNames */



/*****************************  HEAD LINE  *********************************/

    titleLabel =
	ui_xCreateLabelItem("title", ui_infoPanel, 
			   titleWidth,  NULL,       NULL);

    noLabel =
	ui_xCreateLabelItem("no.",     ui_infoPanel,
			    noWidth,     titleLabel, NULL);

    snLabel =
	ui_xCreateLabelItem("subn.",     ui_infoPanel,
			    noWidth,     noLabel, NULL);

    ioLabel =
	ui_xCreateLabelItem("io",    ui_infoPanel,
			    stWidth,     snLabel,    NULL);

    actLabel =
	ui_xCreateLabelItem("act.",   ui_infoPanel,
			    numberWidth, ioLabel,    NULL);

    iactLabel =
	ui_xCreateLabelItem("iact.",   ui_infoPanel,
			    numberWidth, actLabel,    NULL);

    outLabel =
	ui_xCreateLabelItem("out.",   ui_infoPanel,
			    numberWidth, iactLabel,   NULL);

    biasLabel = 
	ui_xCreateLabelItem("bias",  ui_infoPanel,
			    weightWidth, outLabel,   NULL);

    ftypeLabel =
	ui_xCreateLabelItem("name", ui_infoPanel,
			    nameWidth,   biasLabel,  NULL);


/*****************************  SOURCE  ************************************/

    srcLabel =
	ui_xCreateLabelItem("SOURCE", ui_infoPanel,titleWidth,NULL,titleLabel);

    ui_sourceWidgets.no =
	ui_xCreateLabelItem(" ", ui_infoPanel,noWidth,  titleLabel, titleLabel);

    ui_sourceWidgets.subnetNo =
	ui_xCreateLabelItem(" ", ui_infoPanel, noWidth, noLabel, titleLabel);

    ui_sourceWidgets.iotype =
	ui_xCreateLabelItem(" ", ui_infoPanel, stWidth,  snLabel, titleLabel);

    ui_sourceWidgets.act = 
	ui_xCreateDialogItem("act", ui_infoPanel, "0.5", numberWidth,  
			    ioLabel, titleLabel);

    ui_sourceWidgets.i_act = 
	ui_xCreateDialogItem("iact", ui_infoPanel, "0.0", numberWidth,  
			    actLabel, titleLabel);

    ui_sourceWidgets.out = 
	ui_xCreateDialogItem("out", ui_infoPanel, "0.5", numberWidth,  
			    iactLabel, titleLabel);

    ui_sourceWidgets.bias = 
	ui_xCreateDialogItem("bias", ui_infoPanel, "0.0", weightWidth, 
			    outLabel, titleLabel);

    ui_sourceWidgets.name =
	ui_xCreateDialogItem("name", ui_infoPanel,
			     "", nameWidth, biasLabel, titleLabel);


/*****************************  FUNCTIONS SOURCE   ********************/

    srcFuncLabel =
	ui_xCreateLabelItem("  FUNC", ui_infoPanel,
			    titleWidth, NULL,    srcLabel);

    ui_sourceWidgets.actFuncName =
	ui_xCreateLabelItem("*** no act func", ui_infoPanel, nameWidth, 
			    titleLabel, srcLabel);

    ui_sourceWidgets.outFuncName =
	ui_xCreateLabelItem("*** no out func", ui_infoPanel, nameWidth, 
			    actLabel, srcLabel);

    ui_sourceWidgets.ftype = 
	ui_xCreateLabelItem("", ui_infoPanel,
			    nameWidth, biasLabel, srcLabel);


/*****************************  BUTTONS & NAME SOURCE   ********************/

    buttonSrc =
	ui_xCreateButtonItem("first", ui_infoPanel,
			     titleLabel, srcFuncLabel);
    XtAddCallback(buttonSrc, XtNcallback, 
	(XtCallbackProc) ui_info_showFirstPredOfTargetUnit, NULL);

    XtAddCallback(buttonSrc, XtNcallback, (XtCallbackProc) ui_drawFunction,
		  "Source Out");
    XtAddCallback(buttonSrc, XtNcallback, (XtCallbackProc) ui_drawFunction,
		  "Source Act");


    button = ui_xCreateButtonItem("next", ui_infoPanel, buttonSrc,srcFuncLabel);
    XtAddCallback(button, XtNcallback, 
	(XtCallbackProc) ui_info_showNextPredOfTargetUnit, NULL);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_drawFunction,"Source Out");
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_drawFunction,"Source Act");


    ui_sourceWidgets.frozen =
	ui_xCreateToggleItem("freeze", ui_infoPanel,
			     NULL, button, srcFuncLabel);

    XtAddCallback(ui_sourceWidgets.frozen, XtNcallback, 
	(XtCallbackProc) ui_info_freezeUnit, (caddr_t) UI_SOURCE);

    button = ui_xCreateButtonItem("default", ui_infoPanel, 
				  ui_sourceWidgets.frozen, srcFuncLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_info_setDefault,
	 (caddr_t) UI_SOURCE);

    button = ui_xCreateMenuButtonItem("options", ui_infoPanel, button, 
				      srcFuncLabel);

    menu = XtCreatePopupShell("menu", simpleMenuWidgetClass, button,
			      NULL, ZERO);

    mItem = XtCreateManagedWidget("change io-type", smeBSBObjectClass, menu,
				  NULL, ZERO);
    sprintf(listIOTypeSrc.msg,"Set io-type of SOURCE unit");
    listIOTypeSrc.unitPtr  = &ui_sourceUnit;
    listIOTypeSrc.widgetPtr= button;
    listIOTypeSrc.listType = UI_LIST_IOTYPE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType  *) &listIOTypeSrc);
    mItem = XtCreateManagedWidget("change f-type", smeBSBObjectClass, menu,
				  NULL, ZERO);
    sprintf(listFTypeSrc.msg,"Set f-type of SOURCE unit");
    listFTypeSrc.unitPtr  = &ui_sourceUnit;
    listFTypeSrc.widgetPtr= button;
    listFTypeSrc.listType = UI_LIST_FTYPE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType  *) &listFTypeSrc);
    mItem = XtCreateManagedWidget("display activation function",
				  smeBSBObjectClass, menu, NULL, ZERO);
    XtAddCallback(mItem, XtNcallback,  (XtCallbackProc) ui_functionPopup,
		  "Source Act");

    mItem = XtCreateManagedWidget("change activation function", 
				  smeBSBObjectClass, menu, NULL, ZERO);
    sprintf(listActFuncSrc.msg,
	    "Set activation function of SOURCE unit (will kill f-type)");
    listActFuncSrc.unitPtr  = &ui_sourceUnit;
    listActFuncSrc.widgetPtr= button;
    listActFuncSrc.listType = UI_LIST_ACT_FUNC;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType  *) &listActFuncSrc);
    mItem = XtCreateManagedWidget("display output function", 
				  smeBSBObjectClass, menu, NULL, ZERO);
    XtAddCallback(mItem, XtNcallback,  
		  (XtCallbackProc) ui_functionPopup,"Source Out");

    mItem = XtCreateManagedWidget("change output function", 
				  smeBSBObjectClass, menu, NULL, ZERO);
    sprintf(listOutFuncSrc.msg,
	    "Set output function of SOURCE unit (will kill f-type)");
    listOutFuncSrc.unitPtr  = &ui_sourceUnit;
    listOutFuncSrc.widgetPtr= button;
    listOutFuncSrc.listType = UI_LIST_OUT_FUNC;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType  *) &listOutFuncSrc);

    mItem = XtCreateManagedWidget("assign layers", smeBSBObjectClass, menu,
				  NULL, ZERO);
    srcCallLayer.widget    = button;
    srcCallLayer.unitType  = UI_SOURCE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_assignLayer,
		  (struct InfoCallLayerStruct *) &srcCallLayer);
    mItem = XtCreateManagedWidget("list all sources", smeBSBObjectClass, menu,
				  NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, 
	(XtCallbackProc) ui_info_listAllSourceUnits, (caddr_t) UI_SOURCE);

    mItem = XtCreateManagedWidget("list all targets", smeBSBObjectClass, menu,
				  NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, 
	(XtCallbackProc) ui_info_listAllTargetUnits, (caddr_t) UI_SOURCE);

    button = ui_xCreateButtonItem("set", ui_infoPanel, button, srcFuncLabel);
    XtAddCallback(button, XtNcallback, 
	(XtCallbackProc) ui_info_unitUpdateProc, (caddr_t) UI_SOURCE);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_drawFunction,"Source Out");
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_drawFunction,"Source Act");



/*****************************  TARGET  ************************************/

    tarLabel = ui_xCreateLabelItem("TARGET", ui_infoPanel,
				   titleWidth, NULL, buttonSrc);

    ui_targetWidgets.no =
	ui_xCreateLabelItem(" ", ui_infoPanel, noWidth, titleLabel, buttonSrc);

    ui_targetWidgets.subnetNo =
	ui_xCreateLabelItem(" ", ui_infoPanel, noWidth, noLabel, buttonSrc);

    ui_targetWidgets.iotype =
	ui_xCreateLabelItem("*", ui_infoPanel, stWidth,  snLabel, buttonSrc );

    ui_targetWidgets.act = 
	ui_xCreateDialogItem("act", ui_infoPanel, "0.5", numberWidth, 
			    ioLabel, buttonSrc);

    ui_targetWidgets.i_act = 
	ui_xCreateDialogItem("iact", ui_infoPanel, "0.0", numberWidth,  
			    actLabel, buttonSrc);

    ui_targetWidgets.out = 
	ui_xCreateDialogItem("out", ui_infoPanel, "0.5", numberWidth,  
			    iactLabel, buttonSrc);

    ui_targetWidgets.bias = 
	ui_xCreateDialogItem("bias", ui_infoPanel, "0.0", weightWidth, 
			    outLabel, buttonSrc);

    ui_targetWidgets.name =
	ui_xCreateDialogItem("name", ui_infoPanel,
			    "", nameWidth, biasLabel, buttonSrc);


/*****************************  FUNCTIONS TARGET   ********************/

    tarFuncLabel =
	ui_xCreateLabelItem("  FUNC", ui_infoPanel,
			    titleWidth, NULL,  tarLabel);

    ui_targetWidgets.actFuncName =
	ui_xCreateLabelItem("*** no act func", ui_infoPanel, nameWidth, 
			    titleLabel, tarLabel);

    ui_targetWidgets.outFuncName =
	ui_xCreateLabelItem("*** no out func", ui_infoPanel, nameWidth, 
			    actLabel, tarLabel);

    ui_targetWidgets.ftype = 
	ui_xCreateLabelItem("", ui_infoPanel,
			    nameWidth, biasLabel, tarLabel);



/*****************************  BUTTONS & NAME TARGET   ********************/

    buttonTar =
	ui_xCreateButtonItem("first", ui_infoPanel,
			    titleLabel, tarFuncLabel);
    XtAddCallback(buttonTar, XtNcallback, 
	(XtCallbackProc) ui_info_showFirstSuccOfSourceUnit, NULL);

    XtAddCallback(buttonTar, XtNcallback, (XtCallbackProc) ui_drawFunction,
		  "Target Out");
    XtAddCallback(buttonTar, XtNcallback, (XtCallbackProc) ui_drawFunction,
		  "Target Act");

    button =
	ui_xCreateButtonItem("next", ui_infoPanel,
			    buttonTar, tarFuncLabel);
    XtAddCallback(button, XtNcallback, 
	(XtCallbackProc) ui_info_showNextSuccOfSourceUnit, NULL);


    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_drawFunction,
		  "Target Out");
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_drawFunction,
		  "Target Act");

    ui_targetWidgets.frozen =
	ui_xCreateToggleItem("freeze", ui_infoPanel,
			     NULL, button, tarFuncLabel);

    XtAddCallback(ui_targetWidgets.frozen, XtNcallback, 
	(XtCallbackProc) ui_info_freezeUnit, (caddr_t) UI_TARGET);



    button =
	ui_xCreateButtonItem("default", ui_infoPanel,  
			     ui_targetWidgets.frozen, tarFuncLabel);
    XtAddCallback(button, XtNcallback, (XtCallbackProc) ui_info_setDefault,
	(caddr_t) UI_TARGET);

    button =
	ui_xCreateMenuButtonItem("options", ui_infoPanel,
				button, tarFuncLabel);

    menu = 
	XtCreatePopupShell("menu", simpleMenuWidgetClass, button,
			   NULL, ZERO);

    mItem =
	XtCreateManagedWidget("change io-type", smeBSBObjectClass, menu,
			      NULL, ZERO);
    sprintf(listIOTypeTar.msg,"Set io-type of TARGET unit");
    listIOTypeTar.unitPtr  = &ui_targetUnit;
    listIOTypeTar.widgetPtr= button;
    listIOTypeTar.listType = UI_LIST_IOTYPE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc)  ui_info_displayList,
		  (struct SimpleListType *) &listIOTypeTar);
    mItem = XtCreateManagedWidget("change f-type", smeBSBObjectClass, menu,
				  NULL, ZERO);
    sprintf(listFTypeTar.msg,"Set f-type of TARGET unit");
    listFTypeTar.unitPtr  = &ui_targetUnit;
    listFTypeTar.widgetPtr= button;
    listFTypeTar.listType = UI_LIST_FTYPE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType *) &listFTypeTar);

    mItem = XtCreateManagedWidget("display activation function", 
				  smeBSBObjectClass, menu,
				  NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_functionPopup,
		  "Target Act");


    mItem = XtCreateManagedWidget("change activation function", 
				  smeBSBObjectClass, menu,
				  NULL, ZERO);
    sprintf(listActFuncTar.msg,
	    "Set activation function of TARGET unit (will kill f-type)");
    listActFuncTar.unitPtr  = &ui_targetUnit;
    listActFuncTar.widgetPtr= button;
    listActFuncTar.listType = UI_LIST_ACT_FUNC;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType *) &listActFuncTar);

    mItem = XtCreateManagedWidget("display output function", 
				  smeBSBObjectClass, menu,
				  NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_functionPopup,
		  "Target Out");

    mItem = XtCreateManagedWidget("change output function", 
				  smeBSBObjectClass, menu,
				  NULL, ZERO);
    sprintf(listOutFuncTar.msg,
	    "Set output function of TARGET unit (will kill f-type)");
    listOutFuncTar.unitPtr  = &ui_targetUnit;
    listOutFuncTar.widgetPtr= button;
    listOutFuncTar.listType = UI_LIST_OUT_FUNC;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType  *) &listOutFuncTar);

    mItem = XtCreateManagedWidget("assign layers", smeBSBObjectClass, menu,
				  NULL, ZERO);
    tarCallLayer.widget    = button;
    tarCallLayer.unitType  = UI_TARGET;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_assignLayer,
		  (struct InfoCallLayerStruct *) &tarCallLayer);
    mItem = XtCreateManagedWidget("list all sources", smeBSBObjectClass, menu,
				  NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, 
	(XtCallbackProc) ui_info_listAllSourceUnits, (caddr_t) UI_TARGET);

    mItem = XtCreateManagedWidget("list all targets", smeBSBObjectClass, menu,
				  NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, 
	(XtCallbackProc) ui_info_listAllTargetUnits, (caddr_t) UI_TARGET);

    button = ui_xCreateButtonItem("set", ui_infoPanel,    button, tarFuncLabel);
    XtAddCallback(button, XtNcallback, 
	(XtCallbackProc) ui_info_unitUpdateProc, (caddr_t) UI_TARGET);
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_drawFunction,"Target Out");
    XtAddCallback(button, XtNcallback, 
		  (XtCallbackProc) ui_drawFunction,"Target Act");
		

/*****************************  LINK    ************************************/

    linkLabel = ui_xCreateLabelItem("LINK", ui_infoPanel,
				    titleWidth, NULL, buttonTar);

    ui_linkWidgets.weight =
	ui_xCreateDialogItem("weight", ui_infoPanel, "0.01", weightWidth, 
			    linkLabel, buttonTar);

    ui_linkWidgets.siteValue =
	ui_xCreateLabelItem("siteVal", ui_infoPanel,
			    weightWidth, snLabel, buttonTar);
    ui_xSetLabel(ui_linkWidgets.siteValue, "");

    ui_linkWidgets.siteFuncName =
	ui_xCreateLabelItem("site func", ui_infoPanel, nameWidth, 
			    actLabel, buttonTar);
    ui_xSetLabel(ui_linkWidgets.siteFuncName, "");

    ui_linkWidgets.siteName =
	ui_xCreateLabelItem("site name", ui_infoPanel,
			    nameWidth, biasLabel, buttonTar);
    ui_xSetLabel(ui_linkWidgets.siteName,"");

/*****************************  LINK BUTTONS & SITE NAME  ******************/

    button =
	ui_xCreateButtonItem("first", ui_infoPanel,
			    titleLabel, linkLabel);
    XtAddCallback(button, XtNcallback, 
	(XtCallbackProc) ui_info_showFirstSiteOfTargetUnit, NULL);

    button =
	ui_xCreateButtonItem("next", ui_infoPanel,
			    button, linkLabel);
    XtAddCallback(button, XtNcallback, 
	(XtCallbackProc) ui_info_showNextSiteOfTargetUnit, NULL);

    button =
	ui_xCreateMenuButtonItem("options", ui_infoPanel,
			    button, linkLabel);

    menu = 
	XtCreatePopupShell("menu", simpleMenuWidgetClass, button,
			   NULL, ZERO);

    mItem =
	XtCreateManagedWidget("list current site of TARGET", smeBSBObjectClass, 
			      menu, NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, 
	(XtCallbackProc) ui_info_listAllLinksToCurrentSite, NULL);

    mItem =
	XtCreateManagedWidget("list all sites of TARGET", smeBSBObjectClass, 
			      menu, NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_listAllSites, 
	(caddr_t) UI_TARGET);

    mItem =
	XtCreateManagedWidget("list all links from SOURCE", smeBSBObjectClass, 
			      menu, NULL, ZERO);
    XtAddCallback(mItem, XtNcallback, 
	(XtCallbackProc) ui_info_listAllTargetUnits, (caddr_t) UI_SOURCE);

    mItem =
	XtCreateManagedWidget("delete site", smeBSBObjectClass, 
			      menu, NULL, ZERO);
    sprintf(listDeleteSite.msg,"Delete site of target unit (will kill f-type)");
    listDeleteSite.unitPtr  = &ui_targetUnit;
    listDeleteSite.widgetPtr= button;
    listDeleteSite.listType = UI_LIST_UNIT_SITE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType *) &listDeleteSite);

    mItem =
	XtCreateManagedWidget("add site", smeBSBObjectClass, 
			      menu, NULL, ZERO);
    sprintf(listAddSite.msg,"Add site to target unit (will kill f-type)");
    listAddSite.unitPtr  = &ui_targetUnit;
    listAddSite.widgetPtr= button;
    listAddSite.listType = UI_LIST_SITE;
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) ui_info_displayList,
		  (struct SimpleListType *) &listAddSite);

    button =
	ui_xCreateButtonItem("set", ui_infoPanel,
			     button, linkLabel);
    XtAddCallback(button, XtNcallback, 
	(XtCallbackProc) ui_info_linkUpdateProc, NULL);


/***************************************************************************/

    XawFormDoLayout(ui_infoPanel, True);
    ui_infoIsCreated = TRUE;
}








/* end of file */
/* lines: 578 */

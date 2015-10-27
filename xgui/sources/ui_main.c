/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_main.c,v $
  SHORTNAME      : main.c
  SNNS VERSION   : 4.2

  PURPOSE        : contains many widgets to manage xgui of the SNNS
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.4.1990

  CHANGED BY     : Kai-Uwe Herrmann 27.1.1993
  RCS VERSION    : $Revision: 2.27 $
  LAST CHANGE    : $Date: 1998/04/20 11:55:30 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include "ui.h"

#ifndef ultrix
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif

#ifdef __linux__
#include <linux/limits.h>
#endif
#include <sys/param.h>
#include <stdio.h>

#include <X11/Shell.h>
#include <X11/cursorfont.h>

#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>

#include "glob_typ.h"
#include "ui_mainP.h"
#include "ui_print.h"
#include "ui_layerP.h"
#include "ui_xWidgets.h"
#include "ui_display.h"    /* init */
#include "ui_action.h"     /* init */
#include "ui_key.h"        /* init */
#include "ui_fileP.h"      /* init */
#include "ui_color.h"      /* init */
#include "ui_selection.h"
#include "ui_info.h"
#include "ui_infoP.h"
#include "ui_displwght.h"
#include "ui_displmap.h"
#include "ui_inversion.h"
#include "ui_confirmer.h"
#include "bn_bignet.h"
#include "bn_TD_bignet.h"
#include "bn_art1.h"
#include "bn_art2.h"
#include "bn_artmap.h"
#include "bn_kohonen.h"
#include "bn_bighop.h"
#include "bn_JordElm.h"
#include "bn_assoz.h"
#include "net_analyzer.h"

#include "d3_pannels.h"
#include "d3_global.h"
#include "d3_graph.h"
#include "d3_main.h"
#include "o_graph.h"
#include "cc_main.h"
#include "ui_kohonen.h"
#include "pr_main.h"
#include "ui_class.h"

#include "snns.xbm"

#include "ui_main.ph"


#ifdef PARAGON_KERNEL
#include "kr_ipdef.h"
#include "kr_ipmp.h"
#endif


/*****************************************************************************
  FUNCTION : ui_init

  PURPOSE  : do some intialization
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/
static void ui_init (void)
{
    ui_sel_initSelectionList();    /* list of selected units */
    ui_displ_initDisplayList();    /* list of xgui displays  */
    ui_layer_initLayerNames();     /* initial layer names */
    ui_key_init();                 /* editor key automata */
    ui_action_init();              /* editor action routines */
    (void) getcwd( ui_pathname, MAXPATHLEN );

    XawSimpleMenuAddGlobalActions(ui_appContext); /* for editor menu */
}


/*****************************************************************************
  FUNCTION : ui_xCreatePanel

  PURPOSE  : create info panel
  RETURNS  : void
  NOTES    :

  UPDATE   : 27.7.1990
******************************************************************************/
static void ui_xCreatePanel (void)
{
    Widget      menu, mItem, label,button,buttontop,buttontop2,
                form,dummyLabel;
    Arg		args[10];
    Cardinal	n=0;
    XFontStruct *ui_font;

    ui_infoBox = XtCreateManagedWidget("form", formWidgetClass, ui_toplevel, 
				       NULL,ZERO);

    ui_display  = XtDisplay(ui_infoBox);
    dummyLabel = ui_xCreateLabelItem(" ", ui_infoBox, 1, NULL, NULL);
    XtSetArg(args[0], XtNfont, &ui_font); 
    XtGetValues(dummyLabel, args, 1);
    ui_labelFontWidth = (int)((ui_font->max_bounds.width + 
			       ui_font->min_bounds.width)*0.5+0.5)+1;
    if(ui_labelFontWidth < 3){
	ui_confirmOk("Can not access font structure!\n Now using font 7x13");
        ui_labelFontWidth = 7;
    }

    button = ui_xCreateButtonItem("fileP", ui_infoBox, NULL, NULL);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)ui_displayFilePanel, 
		  (Widget) button);

    button = ui_xCreateButtonItem("controlP", ui_infoBox, button, NULL);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)ui_displayControl, NULL); 

    button = ui_xCreateButtonItem("infoP", ui_infoBox, button, NULL);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)ui_displayInfo,
		  (Widget) ui_infoBox); 

    button = ui_xCreateButtonItem("displayP", ui_infoBox, button, NULL);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)ui_displayGraphic, NULL); 

    button = ui_xCreateButtonItem("3dP", ui_infoBox, button, NULL);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)d3_displayGraphic, NULL); 

    button = ui_xCreateButtonItem("graphP", ui_infoBox, button, NULL);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)o_createOszi, NULL); 

    buttontop = ui_xCreateMenuButtonItem("bignetP", ui_infoBox, button, NULL);
    menu = XtCreatePopupShell("menu",simpleMenuWidgetClass,buttontop,NULL,ZERO);
    mItem = XtCreateManagedWidget("general",smeBSBObjectClass,menu,
				  NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_createBignet, NULL);
    mItem = XtCreateManagedWidget("time_delay",smeBSBObjectClass,menu,
				  NULL,ZERO);
    XtAddCallback(mItem, XtNcallback,(XtCallbackProc) bn_create_TD_Bignet,NULL);
    mItem = XtCreateManagedWidget("art1",smeBSBObjectClass,menu, NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_createART1, NULL);
    mItem = XtCreateManagedWidget("art2",smeBSBObjectClass,menu, NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_createART2, NULL);
    mItem = XtCreateManagedWidget("artmap",smeBSBObjectClass,menu, NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_createARTMAP, NULL);
    mItem = XtCreateManagedWidget("kohonen",smeBSBObjectClass,menu, NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_createKOHONEN, NULL);
    mItem = XtCreateManagedWidget("jordan",smeBSBObjectClass,menu, NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_create_jordan, NULL);
    mItem = XtCreateManagedWidget("elman",smeBSBObjectClass,menu, NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_create_elman, NULL);
    mItem = XtCreateManagedWidget("hopfield",smeBSBObjectClass,menu, NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_createBigHop, NULL);
    mItem = XtCreateManagedWidget("assoz",smeBSBObjectClass,menu, NULL,ZERO);
    XtAddCallback(mItem, XtNcallback, (XtCallbackProc) bn_create_assoz, NULL);


    button = ui_xCreateButtonItem("prunP", ui_infoBox, NULL, buttontop);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)pr_createWindow, NULL); 

    button = ui_xCreateButtonItem("cascadeP", ui_infoBox, button, buttontop);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)cc_createWindow, NULL); 

    button = ui_xCreateButtonItem("kohonenP", ui_infoBox, button, buttontop);
    XtAddCallback(button,XtNcallback,(XtCallbackProc)kohonen_createWindow,NULL);

    button = ui_xCreateButtonItem("weightsP", ui_infoBox, button, buttontop);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)ui_displWeights, NULL);

    button = ui_xCreateButtonItem("projectP", ui_infoBox, button, buttontop);
    XtAddCallback(button,XtNcallback, (XtCallbackProc)ui_displMap, NULL);

    buttontop2 = ui_xCreateButtonItem("analyzeP", ui_infoBox, button,buttontop);
    XtAddCallback(buttontop2, XtNcallback, 
		  (XtCallbackProc) NA_OpenNetworkAnalyzer, NULL);

    buttontop = ui_xCreateButtonItem("inversionP", ui_infoBox,buttontop2,
				     buttontop);
    XtAddCallback(buttontop,XtNcallback, (XtCallbackProc) ui_inversion, NULL);

    button = ui_xCreateButtonItem("printP", ui_infoBox, NULL, buttontop);
    XtAddCallback(button,XtNcallback, (XtCallbackProc) ui_printNet, NULL);

    button = ui_xCreateButtonItem("helpP", ui_infoBox, button, buttontop);
    XtAddCallback(button,XtNcallback, (XtCallbackProc) ui_displayText, NULL);

    button = ui_xCreateButtonItem("classesP", ui_infoBox, button, buttontop);
    XtAddCallback(button,XtNcallback, (XtCallbackProc) ui_classWindow, NULL);

    button = ui_xCreateButtonItem("quitP", ui_infoBox, buttontop2, buttontop);
    XtAddCallback(button,XtNcallback, (XtCallbackProc) ui_guiQuit, 
		  (XtPointer) ui_infoBox);

    n=0;
    XtSetArg(args[n], XtNborderWidth, 1);n++; 
    XtSetArg(args[n], XtNleft  , XtChainLeft);n++;
    XtSetArg(args[n], XtNright , XtChainLeft);n++;
    XtSetArg(args[n], XtNtop   , XtChainTop);n++;
    XtSetArg(args[n], XtNbottom, XtChainTop);n++;	
    XtSetArg(args[n], XtNfromVert,button);n++;
    form = XtCreateManagedWidget("form", formWidgetClass, ui_infoBox, args, n);

    ui_message = ui_xCreateLabelItem("manMsg", form, 63*ui_labelFontWidth, 
				     NULL, NULL);

    ui_stat_posWidget = ui_xCreateLabelItem("*", form, 16*ui_labelFontWidth,
					    NULL, ui_message);
    label = ui_xCreateLabelItem("noSel", form, 20,ui_stat_posWidget,ui_message);
    ui_stat_selNoWidget = ui_xCreateLabelItem("*", form, 8*ui_labelFontWidth,
					      label,ui_message);
    label = ui_xCreateLabelItem("flags",form,20,ui_stat_selNoWidget,ui_message);
    ui_stat_flagsWidget = ui_xCreateLabelItem("*",form, 15*ui_labelFontWidth,
					      label,ui_message);

    XawFormDoLayout(ui_infoBox, True);
    
    XtAddEventHandler(ui_infoBox,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control, (Cardinal *) 0);


}


/*****************************************************************************
  FUNCTION : ui_clickEventProc

  PURPOSE  : 
  RETURNS  : void
  NOTES    :

  UPDATE   : 11.2..1992
******************************************************************************/
static void ui_clickEventProc (Widget w, Display *display, XEvent *event)
{
    switch (event->type) {
        case ButtonPress:
            if (event->xbutton.button == 1)
                 XtDestroyWidget (w);
            break;
    }
}


/*****************************************************************************
  FUNCTION : ui_showBanner

  PURPOSE  : display Title Page
  RETURNS  : void
  NOTES    :

  UPDATE   : 11.2..1992
******************************************************************************/
static void ui_showBanner (Widget parent)
{
     int n;
     Arg arg[10];
     Widget banner, frame;
     int xwsize = snns_width;
     int ywsize = snns_height;
     unsigned int xsize, ysize;

     d3_getRootSizes (&xsize, &ysize);
     n = 0;

     XtSetArg (arg[n], XtNwidth, xwsize); n++;
     XtSetArg (arg[n], XtNheight, ywsize); n++;
     XtSetArg (arg[n], XtNx, (xsize - xwsize) / 2); n++;
     XtSetArg (arg[n], XtNy, (ysize - ywsize) / 2); n++;
     banner = XtCreatePopupShell ("banner", transientShellWidgetClass, 
                                  parent, arg, (unsigned int) n);
     n = 0;
     XtSetArg (arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   snns_bits, snns_width, snns_height)); n++;
     frame = XtCreateManagedWidget("", labelWidgetClass,
                                   banner, arg, (unsigned int) n);
     XtAddEventHandler (banner, ButtonPressMask, 
                        FALSE, (XtEventHandler) ui_clickEventProc, ui_display);
     XtPopup (banner, XtGrabNone);
     ui_xDontResizeWidget(banner); 
}


/*****************************************************************************
  FUNCTION : main

  PURPOSE  : construct all widgets and windows, realize them 
  RETURNS  : void
  NOTES    :

  UPDATE   : 1.4.1990
******************************************************************************/
void main (int argc, char **argv)
{
    ui_toplevel = XtAppInitialize(&ui_appContext, "Popup", NULL, ZERO,
				  (int *) &argc, argv, fallback_resources, 
				  NULL, ZERO);

#ifdef PARAGON_KERNEL
    krip_Init();
#endif
    ui_init(); 
    ui_parseCmdLine(argc, argv);
    ui_colVar_init();
    ui_xCreatePanel();
    XtRealizeWidget(ui_toplevel);
    ui_screen   = DefaultScreen(ui_display);
    ui_xDontResizeWidget(ui_toplevel);
    ui_xCreateGC(XtWindow(ui_toplevel));
    XStoreName(ui_display, XtWindow(ui_toplevel),
	       "SNNS Manager Panel");
    ui_showBanner (ui_toplevel);
    ui_col_init();
    d3_init_main ();
    ui_defaultPrinterValues();
    ui_loadFileFromCommandLine();
    XtAppMainLoop(ui_appContext);
}
/* end of file */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_class.c,v $
  SHORTNAME      : ui_class.c
  SNNS VERSION   : 4.2

  PURPOSE        : Panel for Class learning parameters
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 07.01.98

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/09/03 09:52:23 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_main.h"
#include "ui_mainP.h"
#include "ui_utilP.h"
#include "ui_textP.h"
#include "ui_xWidgets.h"
#include "ui_control.h"
#include "ui_controlP.h"
#include "ui_confirmer.h"
#include "glob_typ.h"

#include "ui_class.ph"

/*****************************************************************************
  FUNCTION : class_readWidgets 

  PURPOSE  : get values from class Widget panel
  RETURNS  : 0 for success or -1 for error
  NOTES    :

  UPDATE   : 
******************************************************************************/
int class_readWidgets(void)
{
    int i, temp, error;
    char buf[50];

    if(!ui_class_open)
	return(0);

    for(i=0; i<ui_classes; i++){
	temp = ui_xIntFromAsciiWidget(class_distW[i]);
	if(temp < 0){
	    sprintf(buf,"Illegal negative parameter in class widget %d",i);
	    ui_tw_errorMessage(buf);
	    return(-1);
	}
	ui_classDist[i] = (unsigned int)temp;
    }
    if(ui_classes && (error = krui_setClassDistribution(ui_classDist))<0){
	ui_checkError(error);
	return(-1);
    }
    return(0);
}


/*****************************************************************************
  FUNCTION : class_doneProc

  PURPOSE  : callback function for the DONE button of the class panel and the
             new class panel (new pattern button)
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void class_doneProc(Widget w, Widget kill_it, caddr_t call_data)
{
    char new_class[30];

    if(kill_it == classWidget){
	if(class_readWidgets() == -1){
	    /* do not close the window if illegal values are entered */
	    return;
	}
	ui_class_open = 0 ;
    }else{
	ui_xStringFromAsciiWidget(class_newLab, new_class, 30);
	if(strlen(new_class) > 0){
	    krui_setClassInfo(new_class);
	    ui_classUpdatePanel(TRUE);
	}
	ui_noOfCurrentPattern = krui_getPatternNo();
	ui_rem_displayPatternNumber();
     }

    XtDestroyWidget(kill_it);

}


/*****************************************************************************
  FUNCTION : class_onOffProc

  PURPOSE  : sets the on / off toggle buttons for class usage
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void class_onOffProc(Widget w, bool value, caddr_t call_data)
{
    int  err, new;
    char buf[20];

    err = krui_useClassDistribution(value);
    if(err == 0)
	ui_setToggles(value, &new, 2, class_onOffW);
    else{
	ui_setToggles(FALSE, &new, 2, class_onOffW);
	sprintf(buf,"No class information available");
	ui_tw_errorMessage(buf);
    }
}


/*****************************************************************************
  FUNCTION : class_defProc

  PURPOSE  : sets the the distributions for the classes
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void class_defProc(Widget w, bool value, caddr_t call_data)
{
    int  i, error;
    char buf[20];
    Arg  args[3];
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

    if(value){ /* use the default distribution */
	ui_old_classDist = (unsigned int *)realloc(ui_old_classDist,
						   ui_classes*sizeof(unsigned int));
	/* first, save the old values */
	for(i=0; i<ui_classes; i++)
	    ui_old_classDist[i] = ui_classDist[i];

	/* set the defaults (physical distribution in the pattern file) */
	if((error = krui_setClassDistribution(NULL)) < 0){
	    ui_checkError(error);
	    return;
	}

    }else{ /* use the virtual distribution defined last */

	/* set the old distribution */
	if((error = krui_setClassDistribution(ui_old_classDist)) < 0){
	    ui_checkError(error);
	    return;
	}
    }
    
    /* display the new settings */
    if((error = krui_GetPatInfo(&patt_info, &descrip))< 0){
	ui_checkError(error);
	return;
    }
    for(i=0; i<patt_info.classes; i++){
	sprintf(buf, "%d", *patt_info.class_redistribution++);
	XtSetArg(args[0], XtNstring,buf);
	XtSetValues(class_distW[i], args, 1);
    }
}


/*****************************************************************************
  FUNCTION : ui_newClass

  PURPOSE  : handle window to ask for new class information
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
void ui_newClass(Widget parent, char *oldclass)
{
    Widget      label, button, wrapper, class_setupPanel;
    Position    x, y;
    Dimension   width, height;
    Arg		args[25];   
    Cardinal	n = 0;
    char        buf1[65],buf2[65],buf3[65];

    /* set Popup arguments */
    ui_xGetDimensions(parent, &width, &height);
    XtTranslateCoords(parent, (Position)(width/2), (Position)(height/2), &x,&y);
    n = 0;
    XtSetArg(args[n], XtNx, x);n++;
    XtSetArg(args[n], XtNy, y);n++;


    /* Now create Popup */
    class_popupWidget = XtCreatePopupShell("New Class", 
					   transientShellWidgetClass, 
					   ui_toplevel, args, n);
    wrapper = XtCreateManagedWidget("form", formWidgetClass, class_popupWidget, 
				    NULL,0);
    class_setupPanel = XtCreateManagedWidget("form", formWidgetClass, wrapper,
					     NULL, 0);
    if(oldclass == NULL){
	sprintf(buf1,"Class information with new pattern set?");
	sprintf(buf2,"(Leave blank for patterns without classes)");
	buf3[0] = '\0';
    }else{
	sprintf(buf1,"Please enter alphanumerical");
	sprintf(buf2,"class information below: ");
	sprintf(buf3,"%s",oldclass);
    }
    label = ui_xCreateLabelItem(buf1, class_setupPanel, 45*ui_labelFontWidth,
				NULL, NULL);
    label = ui_xCreateLabelItem(buf2, class_setupPanel, 45*ui_labelFontWidth,
				NULL, label);
    class_newLab = ui_xCreateDialogItem("", class_setupPanel,buf3, 
					12*ui_labelFontWidth, NULL, label);
    button = ui_xCreateButtonItem("done", wrapper, NULL, class_setupPanel) ;
    XtAddCallback(button, XtNcallback, (XtCallbackProc) class_doneProc, 
		  class_popupWidget); 

    ui_checkWindowPosition(class_popupWidget);
    XtPopup(class_popupWidget, XtGrabExclusive);
    ui_xDontResizeWidget(class_popupWidget); 

}

/*****************************************************************************
  FUNCTION :  ui_classUpdatePanel

  PURPOSE  : creates and updates the parameters for the class widget
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
void ui_classUpdatePanel(bool force)
{
    Dimension  width, height;
    int        n = 0, error, row = 0;
    Arg        args[10] ;
    char       buf[50];
    pattern_set_info   patt_info;
    pattern_descriptor descrip;


    if((error = krui_GetPatInfo(&patt_info, &descrip))< 0){
	ui_checkError(error);
	return;
    }
    
    if(patt_info.classes && ui_controlIsCreated){
       sprintf(buf,"current class: %s",patt_info.class_names[descrip.my_class]);
       ui_xSetLabel(ui_controlMessageWidget, buf);
    }

    if(!ui_class_open){
	return;
    }

    if((ui_classes == patt_info.classes) && !force){
	return;
    }

    for(row=0; row<ui_classes; row++){
	XtDestroyWidget(class_LabelW[row]);
	XtDestroyWidget(class_distW[row]);
    }
    XtDestroyWidget(class_leftW0);
    XtDestroyWidget(class_leftW1);
    XtDestroyWidget(class_leftW2);
    XtDestroyWidget(class_defW);
    XtDestroyWidget(class_lastW);
    XtDestroyWidget(class_onOffW[0]);
    XtDestroyWidget(class_onOffW[1]);

    ui_classes = patt_info.classes;

    /* recreate the main part of the window */
    create_classBody(patt_info);

    XtMapWidget(classWidget);

    /* fix window size */
    n = 0;
    XtSetArg(args[n], XtNwidth, &width); n++;  
    XtSetArg(args[n], XtNheight, &height); n++;
    XtGetValues(classWidget, args, n);

    n = 0 ;
    XtSetArg(args[n], XtNminWidth, width); n++;  
    XtSetArg(args[n], XtNminHeight, height); n++;
    XtSetArg(args[n], XtNmaxWidth, width); n++;  
    XtSetArg(args[n], XtNmaxHeight, height); n++;
    XtSetValues(classWidget, args, n);
}


/*****************************************************************************
  FUNCTION : ui_classWindow

  PURPOSE  : creates the window for class update parameters
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
void ui_classWindow(void)
{
    Dimension  width, height;
    int        n = 0, error;
    Arg        args[10] ;
    Widget     button, wrapper;
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

    if(ui_class_open) {
	XRaiseWindow(XtDisplay(classWidget), XtWindow(classWidget));
	return;
    }

    if((error = krui_GetPatInfo(&patt_info, &descrip))< 0){
	ui_checkError(error);
	return;
    }
    ui_classes = patt_info.classes;

    classWidget = XtCreatePopupShell("SNNS Class parameters",
				     topLevelShellWidgetClass,
				     ui_toplevel, args, n);
    wrapper = XtCreateManagedWidget("box", boxWidgetClass, classWidget, NULL,0);
    class_box = XtCreateManagedWidget("form",formWidgetClass,wrapper,NULL, 0);

    /* create the main part of the window */
    create_classBody(patt_info);

    button = ui_xCreateButtonItem("done", wrapper, NULL, class_box) ;
    XtAddCallback(button, XtNcallback, (XtCallbackProc) class_doneProc, 
		  (caddr_t)classWidget ); 

    ui_checkWindowPosition(classWidget);
    XtPopup (classWidget, XtGrabNone); 
    ui_class_open = 1;

    /* fix window size */
    n = 0;
    XtSetArg(args[n], XtNwidth, &width); n++;  
    XtSetArg(args[n], XtNheight, &height); n++;
    XtGetValues(classWidget, args, n);

    n = 0 ;
    XtSetArg(args[n], XtNminWidth, width); n++;  
    XtSetArg(args[n], XtNminHeight, height); n++;
    XtSetArg(args[n], XtNmaxWidth, width); n++;  
    XtSetArg(args[n], XtNmaxHeight, height); n++;
    XtSetValues(classWidget, args, n);

}


/*****************************************************************************
  FUNCTION : create_classBody

  PURPOSE  : creates part of the window for class parameters
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void create_classBody(pattern_set_info patt_info)
{
    int        row;
    Widget     w_top;
    char       buf[50];

    class_LabelW = (Widget *)realloc(class_LabelW, ui_classes*sizeof(Widget)); 
    class_distW = (Widget *)realloc(class_distW, ui_classes*sizeof(Widget));
    ui_classDist = (unsigned int *)realloc(ui_classDist,
					   ui_classes*sizeof(unsigned int));


    w_top = NULL;
    for(row=0; row<ui_classes; row++){
	sprintf(buf,"No. of patterns from class %s", *patt_info.class_names++);
	class_LabelW[row] = ui_xCreateLabelItem(buf, class_box,
						33*ui_labelFontWidth,
						NULL, w_top);
	ui_classDist[row] = *patt_info.class_redistribution;
	sprintf(buf, "%d", *patt_info.class_redistribution++);
	class_distW[row] = ui_xCreateDialogItem(" ", class_box, buf, 
						6*ui_labelFontWidth,
						class_LabelW[row], w_top);
	w_top = class_LabelW[row];
    }
    class_leftW0 = ui_xCreateLabelItem("usage of class information:",class_box, 
				      33 * ui_labelFontWidth, NULL, w_top);
    class_onOffW[1] = ui_xCreateToggleItem("on", class_box, NULL, class_leftW0, 
					   w_top);
    ui_xSetToggleState(class_onOffW[1], patt_info.class_distrib_active);
    XtAddCallback(class_onOffW[1], XtNcallback, (XtCallbackProc)class_onOffProc,
                  (caddr_t) TRUE);
    class_onOffW[0] = ui_xCreateToggleItem("off", class_box, NULL, 
					   class_onOffW[1], w_top);
    ui_xSetToggleState(class_onOffW[0], !patt_info.class_distrib_active);
    XtAddCallback(class_onOffW[0], XtNcallback,(XtCallbackProc)class_onOffProc, 
                  (caddr_t) FALSE);
    class_leftW1 = ui_xCreateLabelItem("set physical distribution:",class_box, 
				      33 * ui_labelFontWidth,NULL,class_leftW0);
    class_defW = ui_xCreateButtonItem("set", class_box, class_onOffW[1], 
				       class_leftW0);
    XtAddCallback(class_defW, XtNcallback,(XtCallbackProc)class_defProc, 
                  (caddr_t) TRUE);
    class_leftW2 = ui_xCreateLabelItem("set last virtual distribution:",
				       class_box, 33 * ui_labelFontWidth,
				       NULL, class_leftW1);
    class_lastW = ui_xCreateButtonItem("set", class_box, class_onOffW[1], 
				       class_leftW1);
    XtAddCallback(class_lastW, XtNcallback,(XtCallbackProc)class_defProc, 
                  (caddr_t) FALSE);
}



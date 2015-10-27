/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_funcdispl.c,v $
  SHORTNAME      : funcdispl
  SNNS VERSION   : 4.2

  PURPOSE        : contains routines for output function display
  NOTES          :
  FUNCTIONS      : -- ui_functionPopup
                      Purpose : Display the characteristic function of the units
                      Calls   : ui_xCreateButtonItem()
                                ui_xCreateDialogItem()
                                ui_createMyScreen()
                   -- ui_createMyScreen
                      Purpose : Create screen for Graphic display
                      Calls   : nothing
                   -- ui_drawFunction
                      Purpose : Draw the functions of the units
                      Calls   : krf_funcSearch()
                   -- local_kill
                      Purpose : Kill the widget for unit function display
                      Calls   : XtDestroyWidget()       
                   -- ui_setXY
                      Purpose : Set the x an y range for the function display
                      Calls   : ui_xFloatFromAsciiWidget()
 
  AUTHOR         : Guenter Mamier 
  DATE           : 14.01.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:25 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>			/* For the Syntax message */
#include <stdlib.h>

#include "ui.h"

#include <X11/Shell.h>
#include <X11/cursorfont.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/TextSrc.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/AsciiText.h>

#include "kr_typ.h"
#include "kernel.h"
#include "ext_typ.h"
#include "kr_funcs.h"
#include "kr_ui.h"
#include "ui_info.h"
#include "ui_setup.h"
#include "ui_layer.h"
#include "ui_file.h"
#include "ui_fileP.h"
#include "ui_netGraph.h"
#include "ui_event.h"
#include "ui_key.h"
#include "ui_confirmer.h"
#include "ui_xWidgets.h"
#include "ui_lists.h"
#include "ui_display.h"
#include "ui_controlP.h"
#include "ui_edit.h"
#include "ui_mainP.h"
#include "ui_main.h"
#include "ui_action.h"

#include "ui_funcdispl.ph"


/*****************************************************************************
  FUNCTION : ui_functionPopup

  PURPOSE  : Display the characteristic functions of the units
  NOTES    : 
  RETURNS  :
  UPDATE   : 14.01.92
******************************************************************************/

void ui_functionPopup(Widget w, char *title, caddr_t call_data)
{
    Widget         fbutton,sbutton,framewidget[6],mainwidget[6];
    Arg		   args[25];   
    Cardinal	   n;
    int            typus;
    char           buf[40];



    sprintf(buf,"\n");


    /* Create window and all subwidgets for the display */

    if(!strcmp(title,"Target Out")){
      FUNC_GRAPH_case = 1;
      sprintf(buf,"Target Output");
      typus = krui_getUnitTType(ui_targetUnit.no);
      if(typus < 1){
        ui_confirmOk("Unit not defined!!");
        return;
      }
    }else if(!strcmp(title,"Target Act")){
      FUNC_GRAPH_case = 2;
      sprintf(buf,"Target Activation");
      typus = krui_getUnitTType(ui_targetUnit.no);
      if(typus < 2){
        ui_confirmOk("Unit has no input!!");
        return;
      }
    }else if(!strcmp(title,"Source Out")){
      FUNC_GRAPH_case = 4;
      sprintf(buf,"Source Output");
      typus = krui_getUnitTType(ui_sourceUnit.no);
      if(typus < 1){
        ui_confirmOk("Unit not defined!!");
        return;
      }
    }else if(!strcmp(title,"Source Act")){
      FUNC_GRAPH_case = 5;
      sprintf(buf,"Source Activation");
      typus = krui_getUnitTType(ui_sourceUnit.no);
      if(typus < 2){
        ui_confirmOk("Unit has no input!!");
        return;
      }
    }
    if( FUNC_GRAPH_win_ex[ FUNC_GRAPH_case] ){
       /* don't create second window */
       ui_confirmOk("Please only one Window per Function !");
       return;
    }
    
    FUNC_GRAPH_created[FUNC_GRAPH_case] = 1; 
    FUNC_GRAPH_win_ex[FUNC_GRAPH_case] = 1;
    

    n = 0;
    mainwidget[FUNC_GRAPH_case] = 
          XtCreatePopupShell(buf,topLevelShellWidgetClass,ui_toplevel,args,n);
    n =0;
    framewidget[FUNC_GRAPH_case] = 
          XtCreateManagedWidget("form",formWidgetClass,
				mainwidget[FUNC_GRAPH_case],args,n);
    fbutton = ui_xCreateButtonItem("done",framewidget[FUNC_GRAPH_case],
				   NULL,NULL);
    sbutton = ui_xCreateButtonItem("set",framewidget[FUNC_GRAPH_case]
				   ,NULL,fbutton);
    FUNC_GRAPH_x_minW[FUNC_GRAPH_case] = 
                     ui_xCreateDialogItem("xmin",framewidget[FUNC_GRAPH_case],
					  "-2.0",40,fbutton,NULL);
    if(FUNC_GRAPH_case == 2 || FUNC_GRAPH_case == 5)
       FUNC_GRAPH_x_labW[FUNC_GRAPH_case] = ui_xCreateLabelItem("<= W_ij <=",
					 framewidget[FUNC_GRAPH_case],80,
					 FUNC_GRAPH_x_minW[FUNC_GRAPH_case],
					 NULL);
    else
       FUNC_GRAPH_x_labW[FUNC_GRAPH_case] = ui_xCreateLabelItem("<= act <=",
					 framewidget[FUNC_GRAPH_case],72,
					 FUNC_GRAPH_x_minW[FUNC_GRAPH_case],
					 NULL);
    FUNC_GRAPH_x_maxW[FUNC_GRAPH_case] = 
                     ui_xCreateDialogItem("xmax",framewidget[FUNC_GRAPH_case], 
					  "2.0",40,
					  FUNC_GRAPH_x_labW[FUNC_GRAPH_case],
					  NULL);
    FUNC_GRAPH_y_minW[FUNC_GRAPH_case] = 
                     ui_xCreateDialogItem("ymin",framewidget[FUNC_GRAPH_case], 
					  "-5.0",40,fbutton,
					  FUNC_GRAPH_x_minW[FUNC_GRAPH_case]);
    if(FUNC_GRAPH_case == 2 || FUNC_GRAPH_case == 5)
       FUNC_GRAPH_y_labW[FUNC_GRAPH_case] = ui_xCreateLabelItem("<= act <=", 
					 framewidget[FUNC_GRAPH_case],72,
					 FUNC_GRAPH_y_minW[FUNC_GRAPH_case],
					 FUNC_GRAPH_x_labW[FUNC_GRAPH_case]);
    else
       FUNC_GRAPH_y_labW[FUNC_GRAPH_case] = ui_xCreateLabelItem("<= out <=", 
					 framewidget[FUNC_GRAPH_case],72,
					 FUNC_GRAPH_y_minW[FUNC_GRAPH_case],
					 FUNC_GRAPH_x_labW[FUNC_GRAPH_case]);

    FUNC_GRAPH_y_maxW[FUNC_GRAPH_case] = 
                     ui_xCreateDialogItem("ymax",framewidget[FUNC_GRAPH_case],
					  "5.0",40,
					  FUNC_GRAPH_x_labW[FUNC_GRAPH_case],
					  FUNC_GRAPH_x_maxW[FUNC_GRAPH_case]);
    displaywidget[FUNC_GRAPH_case] = 
                     ui_createMyScreen("screen",framewidget[FUNC_GRAPH_case],
				       200,100,NULL,
				       FUNC_GRAPH_y_maxW[FUNC_GRAPH_case]);


    /* set defaults for display regions */
    
    FUNC_GRAPH_x_min[FUNC_GRAPH_case] = -2.0;
    FUNC_GRAPH_x_max[FUNC_GRAPH_case] =  2.0;
    FUNC_GRAPH_y_min[FUNC_GRAPH_case] = -5.0;
    FUNC_GRAPH_y_max[FUNC_GRAPH_case] =  5.0;
 
    /* define the routines to be called for widget events */



    XtAddCallback(fbutton,XtNcallback,(XtCallbackProc)local_kill, 
		  (XtPointer) ((long)mainwidget[FUNC_GRAPH_case]));
    XtAddCallback(fbutton,XtNcallback,(XtCallbackProc)local_kill_prot,
		  (XtPointer) ((long)FUNC_GRAPH_case));
    XtAddCallback(sbutton, XtNcallback,(XtCallbackProc) ui_setXY,NULL);
    XtAddCallback(sbutton, XtNcallback,(XtCallbackProc) ui_drawFunction,
		  (XtPointer) ((long)title));

    XtAddEventHandler(displaywidget[FUNC_GRAPH_case],
		      StructureNotifyMask | ExposureMask, 
		      GraphicsExpose,(XtEventHandler) ui_drawFunction,title);




    /* Realize the window */

    ui_checkWindowPosition(mainwidget[FUNC_GRAPH_case]);
    XtPopup(mainwidget[FUNC_GRAPH_case], XtGrabNone);




}


/*****************************************************************************
  FUNCTION : ui_createMyScreen

  PURPOSE  : Create screen for Graphic display
  NOTES    : 
  RETURNS  : The Widgetnumber 
  UPDATE   : 14.01.92
******************************************************************************/


Widget ui_createMyScreen(char *name, Widget parent, Dimension width, Dimension height, Widget left, Widget top)
{
  Cardinal n;
  Widget w;
  Arg    args[15];


    n = 0;
    XtSetArg(args[n], XtNborderWidth,  1); n++;
    XtSetArg(args[n], XtNwidth,  width); n++;
    XtSetArg(args[n], XtNheight, height); n++;
    XtSetArg(args[n], XtNfromHoriz, left); n++;
    XtSetArg(args[n], XtNfromVert, top); n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainRight); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainBottom); n++;
    XtSetArg(args[n], XtNx, 0);n++;
    XtSetArg(args[n], XtNy, 0);n++;

    w = XtCreateManagedWidget(name, widgetClass,parent,args,n);
    return(w);

}



/*****************************************************************************
  FUNCTION : ui_drawFunction

  PURPOSE  : Draw the functions of the units
  NOTES    : 
  RETURNS  :
  UPDATE   : 14.01.92
******************************************************************************/

void ui_drawFunction(Widget w, char *title, caddr_t call_data)
{
  FlintType strong;
  OutFunctionPtr  thisfunc;
  XPoint point[81];
  int n;
  char outString[40];
  float temp;
  int pointcount = 80;
  XtWidgetGeometry geo_info, geo_data;
  XtGeometryResult geo_result;
  float dx,dy;
  int pred_count=1;

  /* switch for the unit and function requested */

  if(!strcmp(title,"Target Out")){
    FUNC_GRAPH_case = 1;

    /* do nothing if window does not exist */
    if(!FUNC_GRAPH_win_ex[FUNC_GRAPH_case])return; 

    /* do nothing if function is not defined yet */
    if(strlen(ui_targetUnit.outFuncName) <= 1)return;

    /* get widget geometry */
    geo_data.request_mode = XtCWQueryOnly;
    geo_result = XtQueryGeometry(displaywidget[FUNC_GRAPH_case],
				 &geo_data,&geo_info);
    if(geo_result != XtGeometryYes){
      geo_info.height = 100;
      geo_info.width  = 200;
    }

    /* get a pointer to the current function */
    if( !krf_funcSearch(ui_targetUnit.outFuncName,1,(FunctionPtr *) &thisfunc)){
      printf("grep to the function failed\n");
      return;
    }

    /* compute function values */
    dx = FUNC_GRAPH_x_max[FUNC_GRAPH_case] - FUNC_GRAPH_x_min[FUNC_GRAPH_case];
    dy = FUNC_GRAPH_y_max[FUNC_GRAPH_case] - FUNC_GRAPH_y_min[FUNC_GRAPH_case];
    if(thisfunc == NULL){
      for(n=0; n<=pointcount; n++){
	point[n].x = (short) n*geo_info.width/pointcount;
	point[n].y = (short)(geo_info.height - 
		      geo_info.height*((FUNC_GRAPH_x_min[FUNC_GRAPH_case] + 
			     (float)(dx*n)/(float)pointcount) - 
			     FUNC_GRAPH_y_min[FUNC_GRAPH_case])/dy);
      }
    }else{
      for(n=0; n<=pointcount; n++){
	point[n].x = (short)(n*geo_info.width/pointcount);
	temp       = ((OutFunctionPtr)thisfunc)( (FlintType)(FUNC_GRAPH_x_min[FUNC_GRAPH_case]+
					   (float)(dx*n)/(float)pointcount));
	point[n].y = (short)( geo_info.height - 
			      geo_info.height*(temp - 
			      FUNC_GRAPH_y_min[FUNC_GRAPH_case])/(float)dy );
      }
    }
  }else if(!strcmp(title,"Target Act")){
    FUNC_GRAPH_case = 2;

    /* do nothing if window does not exist */
    if(!FUNC_GRAPH_win_ex[FUNC_GRAPH_case])return; 

    /* do nothing if function is not defined yet */
    if(strlen(ui_targetUnit.actFuncName) <= 1)return;

    /* get widget geometry */
    geo_data.request_mode = XtCWQueryOnly;
    geo_result = XtQueryGeometry(displaywidget[FUNC_GRAPH_case],
				 &geo_data,&geo_info);
    if(geo_result != XtGeometryYes){
      geo_info.height = 100;
      geo_info.width  = 200;
    }

    /* make target unit current unit */
    krui_setCurrentUnit(ui_targetUnit.no);

    /* get a pointer to the current function */
    if( !krf_funcSearch(ui_targetUnit.actFuncName,2,(FunctionPtr *) &thisfunc)){
      ui_confirmOk("grep to the activation function failed");
      return;
    }

    /* This is a violation of the kernel-xgui interface, but necessary to 
       initinalize some of the unit fields */
    if( strcmp(ui_targetUnit.actFuncName,"Act_TD_Logistic") == 0){
	kr_topoSort(11);
    }

    /* save current strengths */
    handle = (struct chain_el *)malloc(sizeof(struct chain_el));
    weight_chain = handle;
    krui_getFirstPredUnit(&strong);
    weight_chain->weight = strong;
    while(krui_getNextPredUnit(&strong)){
      weight_chain->next = (struct chain_el *)malloc(sizeof(struct chain_el));
      weight_chain = weight_chain->next;
      weight_chain->weight = strong;
    }
    weight_chain->next = NULL;


    /* clear strengths */
    krui_getFirstPredUnit(&strong);
    krui_setLinkWeight(0.0);
    while(krui_getNextPredUnit(&strong)){
      krui_setLinkWeight(0.0);
    }

    /* compute function values */
    dx = FUNC_GRAPH_x_max[FUNC_GRAPH_case] - FUNC_GRAPH_x_min[FUNC_GRAPH_case];
    dy = FUNC_GRAPH_y_max[FUNC_GRAPH_case] - FUNC_GRAPH_y_min[FUNC_GRAPH_case];
    if(thisfunc == NULL){
      for(n=0; n<=pointcount; n++){
	point[n].x = (short) n*geo_info.width/pointcount;
	point[n].y = (short)(geo_info.height - 
		      geo_info.height*((FUNC_GRAPH_x_min[FUNC_GRAPH_case] + 
			     (float)(dx*n)/(float)pointcount) - 
				    FUNC_GRAPH_y_min[FUNC_GRAPH_case])/dy);
      }
      krui_getFirstPredUnit(&strong);
      pred_count = 1;
      while(krui_getNextPredUnit(&strong))pred_count++;
    }else{
      for(n=0; n<=pointcount; n++){
	point[n].x = (short)(n*geo_info.width/pointcount);
        krui_getFirstPredUnit(&strong);
        pred_count = 1;
        krui_setLinkWeight( (FlintType)(FUNC_GRAPH_x_min[FUNC_GRAPH_case]
					+(float)(dx*n)/(float)pointcount));
        while(krui_getNextPredUnit(&strong)){
           krui_setLinkWeight((FlintType)(FUNC_GRAPH_x_min[FUNC_GRAPH_case]+
					  (float)(dx*n)/(float)pointcount));
	   pred_count++;
        }
	temp       = ((ActFunctionPtr) thisfunc) (kr_getUnitPtr(ui_targetUnit.no));
	point[n].y = (short)( geo_info.height - 
			      geo_info.height*
			     (temp-FUNC_GRAPH_y_min[FUNC_GRAPH_case])/
			      (float)dy);
      }
    }

    /* reset strengths */
    weight_chain = handle;
    krui_getFirstPredUnit(&strong);
    strong = weight_chain->weight;
    krui_setLinkWeight(strong);
    while((weight_chain = weight_chain->next) != NULL){
      krui_getNextPredUnit(&strong);
      strong = weight_chain->weight;
      krui_setLinkWeight(strong);
    }

  }else if(!strcmp(title,"Source Out")){
    FUNC_GRAPH_case = 4;

    /* do nothing if window does not exist */
    if(!FUNC_GRAPH_win_ex[FUNC_GRAPH_case])return; 

    /* do nothing if function is not defined yet */
    if(strlen(ui_sourceUnit.outFuncName) <= 1)return;

    /* get widget geometry */
    geo_data.request_mode = XtCWQueryOnly;
    geo_result = XtQueryGeometry(displaywidget[FUNC_GRAPH_case],&geo_data,
				 &geo_info);
    if(geo_result != XtGeometryYes){
      geo_info.height = 100;
      geo_info.width  = 200;
    }

    /* get a pointer to the current function */
    if( ! krf_funcSearch(ui_sourceUnit.outFuncName,1,(FunctionPtr *) &thisfunc)){
      printf("grep to the function failed\n");
      return;
    }

    /* compute function values */
    dx = FUNC_GRAPH_x_max[FUNC_GRAPH_case] - FUNC_GRAPH_x_min[FUNC_GRAPH_case];
    dy = FUNC_GRAPH_y_max[FUNC_GRAPH_case] - FUNC_GRAPH_y_min[FUNC_GRAPH_case];
    if(thisfunc == NULL){
      for(n=0; n<=pointcount; n++){
	point[n].x = (short) n*geo_info.width/pointcount;
	point[n].y = (short)(geo_info.height - 
			 geo_info.height*((FUNC_GRAPH_x_min[FUNC_GRAPH_case]+
			     (float)(dx*n)/(float)pointcount) - 
			      FUNC_GRAPH_y_min[FUNC_GRAPH_case])/dy);
      }
    }else{
      for(n=0; n<=pointcount; n++){
	point[n].x = (short)(n*geo_info.width/pointcount);
	temp       = thisfunc((FlintType)(FUNC_GRAPH_x_min[FUNC_GRAPH_case]+
					   (float)(dx*n)/(float)pointcount));
	point[n].y = (short)( geo_info.height - 
			      geo_info.height*
			   (temp-FUNC_GRAPH_y_min[FUNC_GRAPH_case])/(float)dy);
      }
    }
  
  }else if(!strcmp(title,"Source Act")){
    FUNC_GRAPH_case = 5;

    /* do nothing if window does not exist */
    if(!FUNC_GRAPH_win_ex[FUNC_GRAPH_case])return; 

    /* do nothing if function is not defined yet */
    if(strlen(ui_sourceUnit.actFuncName) <= 1)return;

    /* get widget geometry */
    geo_data.request_mode = XtCWQueryOnly;
    geo_result = XtQueryGeometry(displaywidget[FUNC_GRAPH_case],
				 &geo_data,&geo_info);
    if(geo_result != XtGeometryYes){
      geo_info.height = 100;
      geo_info.width  = 200;
    }
    
    /* make source unit current unit */
    krui_setCurrentUnit(ui_sourceUnit.no);

    /* get a pointer to the current function */
    if( !krf_funcSearch(ui_sourceUnit.actFuncName,2,(FunctionPtr *) &thisfunc)){
      ui_confirmOk("grep to the activation function failed");
      return;
    }

    /* This is a violation of the kernel-xgui interface, but necessary to 
       initinalize some of the unit fields */
    if( strcmp(ui_sourceUnit.actFuncName,"Act_TD_Logistic") == 0){
	kr_topoSort(11);
    }

    /* save current strengths */
    handle = (struct chain_el *)malloc(sizeof(struct chain_el));
    weight_chain = handle;
    krui_getFirstPredUnit(&strong);
    weight_chain->weight = strong;
    while(krui_getNextPredUnit(&strong)){
      weight_chain->next = (struct chain_el *)malloc(sizeof(struct chain_el));
      weight_chain = weight_chain->next;
      weight_chain->weight = strong;
    }
    weight_chain->next = NULL;


    /* clear strengths */
    krui_getFirstPredUnit(&strong);
    krui_setLinkWeight(0.0);
    while(krui_getNextPredUnit(&strong)){
      krui_setLinkWeight(0.0);
    }

    /* compute function values */
    dx = FUNC_GRAPH_x_max[FUNC_GRAPH_case] - FUNC_GRAPH_x_min[FUNC_GRAPH_case];
    dy = FUNC_GRAPH_y_max[FUNC_GRAPH_case] - FUNC_GRAPH_y_min[FUNC_GRAPH_case];
    if(thisfunc == NULL){
      for(n=0; n<=pointcount; n++){
	point[n].x = (short) n*geo_info.width/pointcount;
	point[n].y = (short)(geo_info.height - geo_info.height*
			     ((FUNC_GRAPH_x_min[FUNC_GRAPH_case] + 
			       (float)(dx*n)/(float)pointcount) - 
			       FUNC_GRAPH_y_min[FUNC_GRAPH_case])/dy);
      }
      krui_getFirstPredUnit(&strong);
      pred_count = 1;
      while(krui_getNextPredUnit(&strong))pred_count++;
    }else{
      for(n=0; n<=pointcount; n++){
	point[n].x = (short)(n*geo_info.width/pointcount);
        krui_getFirstPredUnit(&strong);
	pred_count = 1;
        krui_setLinkWeight( (FlintType)(FUNC_GRAPH_x_min[FUNC_GRAPH_case]+
					(float)(dx*n)/(float)pointcount));
        while(krui_getNextPredUnit(&strong)){
           krui_setLinkWeight((FlintType)(FUNC_GRAPH_x_min[FUNC_GRAPH_case]+
					  (float)(dx*n)/(float)pointcount));
	   pred_count++;
        }
	temp       = ((ActFunctionPtr) thisfunc) (kr_getUnitPtr(ui_sourceUnit.no));
	point[n].y = (short)(geo_info.height - 
			     geo_info.height*
			     (temp-FUNC_GRAPH_y_min[FUNC_GRAPH_case])/
			      (float)dy);
      }
    }

    /* reset strengths */
    weight_chain = handle;
    krui_getFirstPredUnit(&strong);
    strong = weight_chain->weight;
    krui_setLinkWeight(strong);
    while((weight_chain = weight_chain->next) != NULL){
      krui_getNextPredUnit(&strong);
      strong = weight_chain->weight;
      krui_setLinkWeight(strong);
    }

  }else{
    printf("Error in selection process\n");
    return;
  }


   /* create a new xlib environment, but just once */

   if(FUNC_GRAPH_created[FUNC_GRAPH_case]){
       FUNC_GRAPH_screen   = DefaultScreen(ui_display);
       FUNC_GRAPH_window[FUNC_GRAPH_case] =  XtWindow(w);
       FUNC_GRAPH_gc[FUNC_GRAPH_case] = 
	            XCreateGC(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
			      0,0);
       XSetForeground(ui_display,FUNC_GRAPH_gc[FUNC_GRAPH_case],
		      BlackPixel(ui_display,FUNC_GRAPH_screen));
       XSetBackground(ui_display,FUNC_GRAPH_gc[FUNC_GRAPH_case],
		      WhitePixel(ui_display,FUNC_GRAPH_screen));
       FUNC_GRAPH_created[FUNC_GRAPH_case] = 0;
   }

   XClearWindow(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case]);


   /* display function */

   XDrawLines(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
	      FUNC_GRAPH_gc[FUNC_GRAPH_case],point,pointcount+1,
	      CoordModeOrigin);


   /* display coordinate system */

   point[0].x = 0;
   point[0].y = geo_info.height + 
            (int)(geo_info.height*FUNC_GRAPH_y_min[FUNC_GRAPH_case]/(float)dy);
   point[1].x = geo_info.width;
   point[1].y = point[0].y; 
   XDrawLines(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
	      FUNC_GRAPH_gc[FUNC_GRAPH_case],point,2,CoordModeOrigin);
   point[0].x = -(int)
                (geo_info.width*FUNC_GRAPH_x_min[FUNC_GRAPH_case]/(float)dx);
   point[0].y = 0;
   point[1].x = point[0].x;
   point[1].y = geo_info.height;
   XDrawLines(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
	      FUNC_GRAPH_gc[FUNC_GRAPH_case],point,2,CoordModeOrigin);


   /* label the coordinates */

   n = (int)FUNC_GRAPH_x_min[FUNC_GRAPH_case];
   do{
     point[0].x = (int)(geo_info.width*
			(n-FUNC_GRAPH_x_min[FUNC_GRAPH_case])/(float)dx);
     point[1].x = point[0].x;
     point[0].y = geo_info.height +
                 (int)
		 (geo_info.height*FUNC_GRAPH_y_min[FUNC_GRAPH_case]/(float)dy);
     point[1].y = point[0].y + 5;
     XDrawLines(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
		FUNC_GRAPH_gc[FUNC_GRAPH_case],point,2,CoordModeOrigin);
     sprintf(outString,"%d",n*pred_count);
     if(n == -1 || n == 1)
        XDrawString(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
		    FUNC_GRAPH_gc[FUNC_GRAPH_case],
		    point[0].x,point[0].y+15,outString,(int) strlen(outString));
   }while(++n < FUNC_GRAPH_x_max[FUNC_GRAPH_case]);

   n = (int)FUNC_GRAPH_y_min[FUNC_GRAPH_case];
   do{
     point[0].x = -(int)
                  (geo_info.width*FUNC_GRAPH_x_min[FUNC_GRAPH_case]/(float)dx);
     point[1].x = point[0].x - 5;
     point[0].y = (int)((float)geo_info.height*
		  (float)(dy-n+FUNC_GRAPH_y_min[FUNC_GRAPH_case])/(float)dy);
     point[1].y = point[0].y;
     XDrawLines(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
		FUNC_GRAPH_gc[FUNC_GRAPH_case],point,2,
		CoordModeOrigin);
     sprintf(outString,"%d",n);
     if(n == -1 || n == 1){
        XDrawString(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
		    FUNC_GRAPH_gc[FUNC_GRAPH_case],
		    (int) point[0].x-9*strlen(outString),
		    point[0].y+4,outString,(int) strlen(outString));
     }
   }while(++n < FUNC_GRAPH_y_max[FUNC_GRAPH_case]);

   n = (int)FUNC_GRAPH_y_min[FUNC_GRAPH_case];
   do{
     point[0].x = 0;
     point[1].x = point[0].x+(int)((float)geo_info.width*0.01);
     point[2].x = geo_info.width;
     point[0].y = (int)((float)geo_info.height*(float)(dy-
			n+FUNC_GRAPH_y_min[FUNC_GRAPH_case])/(float)dy);
     point[2].y = point[1].y = point[0].y;
     XDrawLines(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
		FUNC_GRAPH_gc[FUNC_GRAPH_case],point,2,CoordModeOrigin);

     XDrawLine(ui_display,FUNC_GRAPH_window[FUNC_GRAPH_case],
	       FUNC_GRAPH_gc[FUNC_GRAPH_case],
	       point[2].x,point[0].y,
	       point[2].x-(int)((float)geo_info.width*0.01),point[1].y);
   }while(++n < FUNC_GRAPH_y_max[FUNC_GRAPH_case]);


   /* get everything on the screen */
 
   XFlush(ui_display);

}


/****************************************************************************
  FUNCTION : local_kill

  PURPOSE  : Kill the widget for unit function display
  NOTES    : 
  RETURNS  :

  UPDATE   : 14.01.92
******************************************************************************/

static void local_kill(Widget dummy, Widget w, caddr_t call_data)
{
  XtDestroyWidget(w);
}


/****************************************************************************
  FUNCTION : local_kill_prot

  PURPOSE  : Kill the widget for unit function display
  NOTES    : 
  RETURNS  :

  UPDATE   : 14.01.92
******************************************************************************/

static void local_kill_prot(Widget dummy, int thiscase, caddr_t call_data)
{
  FUNC_GRAPH_win_ex[thiscase] = 0;
}


/*****************************************************************************
  FUNCTION : ui_setXY

  PURPOSE  : Set the x an y range for the function display
  NOTES    : 
  RETURNS  :
  UPDATE   : 22.01.92
******************************************************************************/
static void ui_setXY(Widget w, caddr_t call_data)
{
    FUNC_GRAPH_x_min[FUNC_GRAPH_case] = 
          (float)ui_xFloatFromAsciiWidget(FUNC_GRAPH_x_minW[FUNC_GRAPH_case]);
    FUNC_GRAPH_x_max[FUNC_GRAPH_case] = 
          (float)ui_xFloatFromAsciiWidget(FUNC_GRAPH_x_maxW[FUNC_GRAPH_case]);
    FUNC_GRAPH_y_min[FUNC_GRAPH_case] = 
          (float)ui_xFloatFromAsciiWidget(FUNC_GRAPH_y_minW[FUNC_GRAPH_case]);
    FUNC_GRAPH_y_max[FUNC_GRAPH_case] = 
          (float)ui_xFloatFromAsciiWidget(FUNC_GRAPH_y_maxW[FUNC_GRAPH_case]);
}





















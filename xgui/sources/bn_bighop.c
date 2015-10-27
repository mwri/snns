/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_bighop.c,v $
  SHORTNAME      : bn_bighop
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the Hopfield BIGNET-window. 
  NOTES          :

  AUTHOR         : Christine Bagdi  
  DATE           : 27.5.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:43 $

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
#include "ui_utilP.h"
#include "ui_textP.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"
#include "bn_basics.h"
#include "bn_bignet.h"
#include "ui_confirmer.h"
#include "ui_control.h"

#include "bn_bighop.ph"


/*****************************************************************************
  FUNCTION : bn_createPlanePanel

  PURPOSE  : creates the plane panel, which is part of the bignet-window
  NOTES    :
  RETURNS  :
  UPDATE   : 20.12.1991
******************************************************************************/

static void bn_createPlanePannel (Widget parent)

{
    Widget pannel;
    Widget plane_edit,current_plane;
    Widget plane_enter,plane_delete,plane_insert,plane_overwrite,
	   plane_to_edit,type,pos;
    Widget plane_first,plane_last,plane_prev,plane_next;
    Widget       dummy;
    Arg          arg[25];
    Cardinal     n;
    int colom1 = 25, colom2 = 13, colom3 = 10;

    n = 0;

    pannel = XtCreateManagedWidget ("pannel", formWidgetClass, parent, arg, n);

    z1s1 = ui_xCreateLabelItem ("           Plane           ", pannel, colom1 * bn_fontWidth, NULL, NULL);
    z1s2 = ui_xCreateLabelItem ("Current Plane  ", pannel, colom2 * bn_fontWidth, z1s1, NULL);
    z1s3 = ui_xCreateLabelItem ("Edit Plane  ", pannel, colom3 * bn_fontWidth, z1s2, NULL);

    z2s1   = ui_xCreateLabelItem ("Plane: ", pannel, colom1 * bn_fontWidth, NULL, z1s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth, z2s1, z1s1);
    z2s2   = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z1s1);

    z3s1  = ui_xCreateLabelItem ("Type: ",pannel,colom1 * bn_fontWidth,NULL,z2s1);
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z3s1,z2s1);
    z3s2  = ui_xCreateFrameLabelItem ("",pannel,bn_intWidth,dummy,z2s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z3s2,z2s1);
    z3s3  = ui_xCreateFrameLabelItem (plane_type[PLANE_type],pannel,bn_intWidth,dummy,z2s1);

    z4s1  = ui_xCreateLabelItem ("No. of units in x-direction: ",pannel,colom1 * bn_fontWidth,NULL,z3s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth,z4s1,z3s1);
    z4s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z3s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 5 * bn_fontWidth,z4s2,z3s1);
    z4s3  = ui_xCreateDialogItem ("z4s3", pannel,"",bn_intWidth,dummy,z3s1);

    z5s1  = ui_xCreateLabelItem ("No. of units in y-direction: ",pannel,colom1 * bn_fontWidth,NULL,z4s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth,z5s1,z4s1);
    z5s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z4s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 5 * bn_fontWidth,z5s2,z4s1);
    z5s3  = ui_xCreateDialogItem ("z4s3", pannel, "",bn_intWidth,dummy,z4s1);

    z6s1  = ui_xCreateLabelItem ("z-coordinates of the plane: ",pannel,colom1 * bn_fontWidth,NULL,z5s1);
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z6s1,z5s1);
    z6s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z5s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z6s2,z5s1);
    z6s3  = ui_xCreateDialogItem ("z4s3",pannel,"",bn_intWidth,dummy,z5s1);

    z7s1  = ui_xCreateLabelItem ("Rel. Position: ",pannel,colom1 * bn_fontWidth,NULL,z6s1);
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z7s1,z6s1);
    z7s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z6s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z7s2,z6s1);
    z7s3  = ui_xCreateFrameLabelItem (plane_pos[PLANE_pos],pannel,bn_intWidth,dummy,z6s1);

    dummy         = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,z7s1);

    plane_edit = ui_xCreateLabelItem ("Edit Plane: ",pannel,15 * bn_fontWidth,NULL,dummy);
    plane_enter      = ui_xCreateButtonItem ("enter",pannel,plane_edit,dummy);
      XtAddCallback(plane_enter,XtNcallback,(XtCallbackProc)enter_plane_PROC,NULL);
    plane_insert     = ui_xCreateButtonItem ("insert",pannel,plane_enter,dummy);
      XtAddCallback(plane_insert,XtNcallback,(XtCallbackProc)insert_plane_PROC,NULL);
    plane_overwrite  = ui_xCreateButtonItem ("overwrite",pannel,plane_insert,dummy);

      XtAddCallback(plane_overwrite,XtNcallback,(XtCallbackProc)overwrite_plane_PROC,NULL);
    plane_delete     = ui_xCreateButtonItem ("delete",pannel,plane_overwrite,dummy);
      XtAddCallback(plane_delete,XtNcallback,(XtCallbackProc)delete_plane_PROC,NULL);

    dummy         = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,plane_edit);
    plane_to_edit = ui_xCreateButtonItem ("plane_to_edit",pannel,dummy,plane_edit);
      XtAddCallback(plane_to_edit,XtNcallback,(XtCallbackProc)current_plane_to_editor_PROC,NULL);
    type          = ui_xCreateButtonItem ("type",pannel,plane_to_edit,plane_edit);
      XtAddCallback(type,XtNcallback,(XtCallbackProc)type_PROC,NULL);
    pos           = ui_xCreateButtonItem ("pos",pannel,type,plane_edit);
      XtAddCallback(pos,XtNcallback,(XtCallbackProc)pos_PROC,NULL);

    current_plane = ui_xCreateLabelItem ("Current plane: ",pannel,15 * bn_fontWidth,NULL,dummy);
    plane_first         = ui_xCreateButtonItem ("first",pannel,current_plane,dummy);
      XtAddCallback(plane_first,XtNcallback,(XtCallbackProc)beginning_plane_PROC,NULL);
    plane_prev          = ui_xCreateButtonItem ("prev",pannel,plane_first,dummy);
      XtAddCallback(plane_prev,XtNcallback,(XtCallbackProc)backward_plane_PROC,NULL);
    plane_next          = ui_xCreateButtonItem ("next",pannel,plane_prev,dummy);
      XtAddCallback(plane_next,XtNcallback,(XtCallbackProc)forward_plane_PROC,NULL);
    plane_last          = ui_xCreateButtonItem ("last",pannel,plane_next,dummy);
      XtAddCallback(plane_last,XtNcallback,(XtCallbackProc)end_plane_PROC,NULL);
}


/*****************************************************************************
  FUNCTION : exit_PROC

  PURPOSE  : callback function of the exit-button. You are leaving bignet.
  NOTES    :
  RETURNS  :

  UPDATE   : 27.05.1993
******************************************************************************/

static void exit_PROC (void)

{
    XtDestroyWidget (hop_baseWidget);
    bighop_widget_open = 0;
}
/* chris */
/*****************************************************************************
  FUNCTION : bn_createBigHop

  PURPOSE  : creates the bignet-window.
  NOTES    : 
  RETURNS  :

  UPDATE   : 27.05.1993
******************************************************************************/

void bn_createBigHop (void)

{
    Widget       box;
    Widget       create_net,exit,cancel;
    Arg          arg[25];
    Cardinal     n;
    char         buf[40];

    if(bighop_widget_open) {
	XRaiseWindow (XtDisplay (hop_baseWidget), XtWindow (hop_baseWidget));
	return;
    }

    sprintf (buf, "SNNS BigNet (Hopfield)");
    n = 0;  

    XtSetArg(arg[n],XtNminHeight,699); n++;
    XtSetArg(arg[n],XtNminWidth,410); n++;  
    XtSetArg(arg[n],XtNmaxHeight,699); n++;
    XtSetArg(arg[n],XtNmaxWidth,410); n++;
       

    hop_baseWidget = XtCreatePopupShell (buf, topLevelShellWidgetClass, 
				     ui_toplevel, arg, n); 
    n = 0;  
    box = XtCreateManagedWidget ("box", boxWidgetClass, hop_baseWidget, arg, n);
    bn_createPlanePannel(box); 

    create_net = ui_xCreateButtonItem ("create_net",box,NULL,NULL);
    XtAddCallback(create_net,XtNcallback,
		  (XtCallbackProc)create_net_PROC,NULL);
    exit = ui_xCreateButtonItem ("done",box,create_net,NULL);
    XtAddCallback(exit,XtNcallback,(XtCallbackProc)exit_PROC,NULL);
    cancel     = ui_xCreateButtonItem ("cancel",box,exit,NULL);
    XtAddCallback(cancel,XtNcallback,(XtCallbackProc)cancel_net_PROC,NULL);

    ui_checkWindowPosition(hop_baseWidget);
    XtPopup (hop_baseWidget, XtGrabNone);
    bighop_widget_open = 1;
    if(PLANE_length != 0){
	write_current_plane();
    }

}


   

/*****************************************************************************
  FUNCTION : create_net_PROC

  PURPOSE  : creates a SNNS-net in two steps:
             1. calls up a function, which creates SNNS-units
             2. calls up a function, which connects the SNNS-units.
  NOTES    : There is a full connection of the units! ( Clique ) 
             
  RETURNS  :

  UPDATE   : 27.05.1993
******************************************************************************/

static void create_net_PROC(void)

{
 int create=1;

 BN_ERROR = 0;

 if(krui_getNoOfUnits() != 0) {
   if(! ui_confirmYes("Create will erase current network. Create?")) {
     create = 0;
   }
 }
 
 if(create && (PLANE_length != 0)) {

   krui_deleteNet();

   calculate_first_snns_unit_no_of_plane();
   create_snns_unit(); 
   make_links();
   krui_setLearnFunc("BPTT");
   krui_setUpdateFunc("Hopfield_Synchronous");
   krui_setInitialisationFunc("ClippHebb");
   ui_NumberOfLearnParamsChanged();
   ui_NumberOfUpdateParamsChanged();
   ui_NumberOfInitParamsChanged();
   bn_basics_refresh();
 }/* if */ 
}

/*****************************************************************************
  FUNCTION : calculate_first_snns_unit_no_of_plane

  PURPOSE  : calculates the SNNS unit no of the left upper corner of every
             plane.
  NOTES    : relative to this unit no, knowing the width and the height of the
             planes, all other SNNS unit numbers of every plane can be 
             calculated. That's important for the function create_snns_unit.
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void calculate_first_snns_unit_no_of_plane(void)

{
 PLANE *PLANE_element = PLANE_first_element;

 (*PLANE_element).begin = 1;  
 PLANE_element = (*PLANE_element).next;

 while(PLANE_element != NULL) {
   (*PLANE_element).begin = (*(*PLANE_element).before).width *
                                (*(*PLANE_element).before).height +
				  (*(*PLANE_element).before).begin;
   PLANE_element = (*PLANE_element).next; 
 }
}




/*****************************************************************************
  FUNCTION : create_snns_unit

  PURPOSE  : creates for every unit of a plane a SNNS  unit.

  NOTES    : Default unit with Step - activation function
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void create_snns_unit(void)

{
 struct PosType unit_pos;

 PLANE *PLANE_element = PLANE_first_element; 
 int x,y,width,height,unit_no,pos,ret;
 int y_offset=0,relative_y_max=0,absolute_y_max=0,y_begin;
 int x_max=0,old_x_begin=2,new_x_begin,x_begin;

 ret = krui_allocateUnits((*PLANE_last_element).width * 
                             (*PLANE_last_element).height +
                               (*PLANE_last_element).begin-1);
 if(ret != 0){
   ui_tw_errorMessage(krui_error(ret));
   BN_ERROR = 1;
 }

 while((! BN_ERROR) && (PLANE_element != NULL)) {

   height = (*PLANE_element).height;
   width  = (*PLANE_element).width;
   pos    = (*PLANE_element).pos;
    
   y_begin = calculate_y_begin(&absolute_y_max,&relative_y_max,&y_offset,height,pos);
   x_begin = calculate_x_begin(&new_x_begin,&old_x_begin,&x_max,width,pos);

   for(y=0;y<height;y++){
     for(x=0;x<width;x++){
       unit_no = krui_createDefaultUnit();
       if(unit_no<0) ui_checkError(unit_no);
       krui_setUnitActFunc(unit_no, "Act_Perceptron");
       ret = krui_setUnitTType(unit_no,(*PLANE_element).type+1);
       if(ret != 0){
         ui_tw_errorMessage(krui_error(ret));
         BN_ERROR = 1;
       }
      
       unit_pos.x = x_begin + x;
       unit_pos.y = y_begin + y;

       krui_setUnitPosition(unit_no,&unit_pos);
     }/*for*/
   }/*for*/

   PLANE_element = (*PLANE_element).next;
 }/*while*/
}


/*****************************************************************************
  FUNCTION : make_links

  PURPOSE  : Insert Links between all units, that they form a
             clique, i.e. every unit is connected to all other units.
  RETURNS  : alteration of the network
  NOTES    :

  UPDATE   :
*****************************************************************************/

static void make_links(void)

{
  int                     unit_ptr, mainUnit_Ptr;
  FlintType               weight;
  
  
  
  weight = 1.0;
  
  for ((mainUnit_Ptr) = krui_getFirstUnit();
       (mainUnit_Ptr) <= krui_getNoOfUnits(); (mainUnit_Ptr)++)
    
    /* main loop  self connections too  */
    
    for ((unit_ptr) =  krui_getFirstUnit();
	 (unit_ptr) <= krui_getNoOfUnits(); (unit_ptr)++) { 
      
      
      /* inner loop */
      /*if (NOT (unit_ptr == mainUnit_Ptr)) {  not the same entry !! */
      
      krui_setCurrentUnit(mainUnit_Ptr);
      if (NOT krui_isConnected(unit_ptr)) {
	krui_createLink(unit_ptr, weight);
	krui_setLinkWeight(weight); 
	
      }
    }
}



/*****************************************************************************
  FUNCTION : read_plane_elements

  PURPOSE  : reads the input datas of the plane editor (Edit Plane).

  NOTES    : 
 
  RETURNS  :  Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/

static int read_plane_elements(void)

{
 PLANE_width = ui_xIntFromAsciiWidget(z4s3);
 PLANE_height = ui_xIntFromAsciiWidget(z5s3);
 if((PLANE_width > 0) && (PLANE_height > 0)) {
   return 1;
 }
 else {
   return 0;
 }
}
  
/*****************************************************************************
  FUNCTION : write_current_plane

  PURPOSE  : writes the datas of the current_element to the control part 
             (Current Plane) of the plane editor.

  NOTES    : 
  RETURNS  : 

  UPDATE   : 20.12.1991
******************************************************************************/

static void write_current_plane(void)

 {
  char buf[10];

  sprintf(buf,"%d",PLANE_no);
  ui_xSetLabel(z2s2,bn_widget_output(buf));
  ui_xSetLabel(z3s2,plane_type[(*PLANE_current_element).type]);
  sprintf(buf,"%d",(*PLANE_current_element).width);
  ui_xSetLabel(z4s2,bn_widget_output(buf));
  sprintf(buf,"%d",(*PLANE_current_element).height);
  ui_xSetLabel(z5s2,bn_widget_output(buf));
  sprintf(buf,"%d",0);
  ui_xSetLabel(z6s2,bn_widget_output(buf));
  ui_xSetLabel(z7s2,plane_pos[(*PLANE_current_element).pos]);
}

/*****************************************************************************
  FUNCTION : clear_current_plane

  PURPOSE  : Deletes the datas standing in Current Plane.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void clear_current_plane(void)

 {
  ui_xSetLabel(z2s2,"");
  ui_xSetLabel(z3s2,"");
  ui_xSetLabel(z4s2,"");
  ui_xSetLabel(z5s2,"");
  ui_xSetLabel(z6s2,"");
  ui_xSetLabel(z7s2,"");
}

/*****************************************************************************
  FUNCTION : write_current_plane_to_editor

  PURPOSE  : writes the datas of Current Plane to Edit Plane

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void write_current_plane_to_editor(void)

 {
  char buf[10];

  PLANE_type = (*PLANE_current_element).type;
  ui_xSetLabel(z3s3,plane_type[PLANE_type]);
  sprintf(buf,"%d",(*PLANE_current_element).width);
  ui_xSetString(z4s3,bn_widget_output(buf));
  sprintf(buf,"%d",(*PLANE_current_element).height);
  ui_xSetString(z5s3,bn_widget_output(buf));
  sprintf(buf,"%d",0);
  ui_xSetString(z6s3,bn_widget_output(buf));
  PLANE_pos = (*PLANE_current_element).pos;
  ui_xSetLabel(z7s3,plane_pos[PLANE_pos]);
}

/*****************************************************************************
  FUNCTION : clear_edit_plane

  PURPOSE  : Deletes the datas standing in Edit Link.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void clear_edit_plane(void)

{
 ui_xSetString(z4s3,"");
 ui_xSetString(z5s3,"");
 ui_xSetString(z6s3,"");
} 


/*****************************************************************************
  FUNCTION : cancel_net_PROC

  PURPOSE  : callback function, which deletes all the datas in the link 
             editor and in the plane editor.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void cancel_net_PROC(void)

{
 while(PLANE_length){
  delete_current_plane();
 }
 clear_current_plane();
}

/*****************************************************************************
  FUNCTION : enter_plane_PROC
 
  PURPOSE  : callback function, which enters a plane element at the end of the 
             plane list.

  NOTES    : the plane list contains all datas entered by the plane editor.
             Every plane element is a struct (see bn_types.c)
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void enter_plane_PROC(void)

{
 if(read_plane_elements()){
   enter_plane();
   write_current_plane();
   clear_edit_plane();
 }
}

/*****************************************************************************
  FUNCTION : insert_plane_PROC
 
  PURPOSE  : callback function, which inserts the the input datas of the
             plane editor (Edit Plane) in front of Current Plane.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void insert_plane_PROC(void)

{
 if(read_plane_elements()) {
   insert_plane();
   write_current_plane();
   clear_edit_plane();
/*   if(LINK_length == 0) {
     clear_current_link();
   }
   else {
     write_current_link();
   }
*/
 }
}

/*****************************************************************************
  FUNCTION : overwrite_plane_PROC
 
  PURPOSE  : callback function, which overwrites the current plane element
             (Current Plane) with the datas of the plane editor (Edit plane).

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void overwrite_plane_PROC(void)

{
 if(read_plane_elements()) {
  overwrite_plane();
  write_current_plane();
  clear_edit_plane();
/*  if(LINK_length == 0) {
    clear_current_link();
  }
  else {
    write_current_link();
  }
*/
 }
}

/*****************************************************************************
  FUNCTION : delete_plane_PROC
 
  PURPOSE  : callback function, which deletes the Currenrt Plane out of
             the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void delete_plane_PROC(void)

{
 delete_current_plane();
 if(PLANE_length == 0) {
   clear_current_plane();
 }
 else {
   write_current_plane();
 }
/* if(LINK_length == 0) {
   clear_current_link();
 }
 else {
  write_current_link();
 }
*/
}

/*****************************************************************************
  FUNCTION : forward_plane_PROC
 
  PURPOSE  : callback function, which moves forward one list element in the
             plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void forward_plane_PROC(void)

{
 if(forward_plane()){
   write_current_plane();
 }
}

/*****************************************************************************
  FUNCTION : backward_plane_PROC
 
  PURPOSE  : callback function, which moves backward one list element in the
             plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void backward_plane_PROC(void)

{
 if(backward_plane()) {
   write_current_plane();
 }
}

/*****************************************************************************
  FUNCTION : end_plane_PROC
 
  PURPOSE  : callback function, which jumps to the end of the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void end_plane_PROC(void)

{
 if(end_plane()) {
   write_current_plane();
 }
}

/*****************************************************************************
  FUNCNCTION : beginning_plane_PROC
 
  PURPOSE  : callback function, which jumps to the beginning of the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void beginning_plane_PROC(void)

{
 if(beginning_plane()) {
   write_current_plane();
 }
}

/*****************************************************************************
  FUNCNCTION : pos_PROC
 
  PURPOSE  : callback function, which defines the position of the plane in the
             display window.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void pos_PROC(void)

{
 if(PLANE_pos == BN_RIGHT) {
   PLANE_pos = BN_BELOW;
 }
 else if(PLANE_pos == BN_BELOW) {
   PLANE_pos = BN_LEFT;
 } 
 else {
   PLANE_pos = BN_RIGHT;
 }
 ui_xSetLabel(z7s3,plane_pos[PLANE_pos]);
}

/*****************************************************************************
  FUNCNCTION : current_plane_to_editor_PROC
 
  PURPOSE  : callback function, which writes the Current Plane to Edit Plane

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void current_plane_to_editor_PROC(void)

{
 if(PLANE_length != 0) {
   write_current_plane_to_editor();
 }
 else {
   clear_edit_plane();
 }
}

/*****************************************************************************
  FUNCNCTION : type_PROC
 
  PURPOSE  : callback function, which defines the unit type of a plane.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void type_PROC(void)

{
 switch(PLANE_type){
   case BN_INPUT : {PLANE_type = BN_HIDDEN; break;}
   case BN_HIDDEN: {PLANE_type = BN_OUTPUT; break;}
   case BN_OUTPUT: {PLANE_type = BN_INPUT ; break;}
   case BN_DUAL:   {PLANE_type = BN_DUAL  ; break;}  
 }
 ui_xSetLabel(z3s3,plane_type[PLANE_type]);
}

/*****************************************************************************
  FUNCTION : enter_plane
 
  PURPOSE  : enters a plane element at the end of the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void enter_plane(void)

{
 PLANE *PLANE_element;

 PLANE_element = (PLANE *)malloc(sizeof(PLANE));
 (*PLANE_element).width      = PLANE_width;                                    
 (*PLANE_element).height      = PLANE_height;
 (*PLANE_element).pos    = PLANE_pos;
 (*PLANE_element).type   = PLANE_type;                                    
 (*PLANE_element).next   = NULL;                            
 (*PLANE_element).before = PLANE_last_element;         

 if(PLANE_last_element != NULL) {
   (*PLANE_last_element).next = PLANE_element;
 }

 if(PLANE_first_element == NULL) {
   PLANE_first_element = PLANE_element;
 }

 PLANE_last_element = PLANE_element;
 PLANE_current_element = PLANE_last_element;

 PLANE_no = ++PLANE_length;
}/* enter_plane */

/*****************************************************************************
  FUNCTION : insert_plane
 
  PURPOSE  : inserts the the input datas of the plane editor (Edit Plane) in 
             front of Current Plane.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void insert_plane(void)

{
 PLANE *PLANE_element;

 if(PLANE_length == 0) {
   enter_plane();
 }
 else {
   PLANE_element = (PLANE *)malloc(sizeof(PLANE));
   
   (*PLANE_element).width      = PLANE_width;                                    
   (*PLANE_element).height      = PLANE_height;
   (*PLANE_element).pos    = PLANE_pos;
   (*PLANE_element).type   = PLANE_type;                                    
   (*PLANE_element).next   = PLANE_current_element;                            
   (*PLANE_element).before = (*PLANE_current_element).before;

   if((*PLANE_current_element).before != 0) {
     (*(*PLANE_current_element).before).next = PLANE_element;
   }
   (*PLANE_current_element).before = PLANE_element;         

   PLANE_current_element = PLANE_element;
   
   if((*PLANE_element).before == NULL) {
     PLANE_first_element = PLANE_element;
   } 

   PLANE_length++;
/*
   LINK_current_element = LINK_first_element; 

   while(LINK_current_element) {
     if((*LINK_current_element).SOURCE.plane >= PLANE_no) {
       (*LINK_current_element).SOURCE.plane++;
     }
     if((*LINK_current_element).TARGET.plane >= PLANE_no) {
       (*LINK_current_element).TARGET.plane++;
     }
     LINK_current_element = (*LINK_current_element).next;
   }

   LINK_current_element = LINK_last_element;
*/
 }
}

/*****************************************************************************
  FUNCTION : overwrite_plane
 
  PURPOSE  : overwrites the current plane element (Current Plane) with the 
             datas of the plane editor (Edit Plane).

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void overwrite_plane(void)

{
 if(PLANE_length == 0) {
   enter_plane();
 }
 else {
   (*PLANE_current_element).width = PLANE_width;
   (*PLANE_current_element).height = PLANE_height;
   (*PLANE_current_element).pos = PLANE_pos;
   (*PLANE_current_element).type = PLANE_type;

/*   get_links_and_delete_them(PLANE_no);

   LINK_current_element = LINK_first_element;
*/
 }
}

/*****************************************************************************
  FUNCTION : delete_current_plane
 
  PURPOSE  : deletes Currenrt Plane out of the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void delete_current_plane(void)

{
 PLANE *PLANE_element;

 PLANE_element = PLANE_current_element;

 /* Delete the element "PLANE_element" out of the plane-list */

 if(PLANE_length != 0) {
   if((*PLANE_element).before != NULL) {
     (*(*PLANE_element).before).next = (*PLANE_element).next;
   }
   if((*PLANE_element).next != NULL) {
     (*(*PLANE_element).next).before = (*PLANE_element).before;
   }

   /* Delete all the links which are connected with the deleted plane */
   
/*   get_links_and_delete_them(PLANE_no);

   LINK_current_element = LINK_first_element;
*/
   /* Decrement the number of the Planes */

/*   while(LINK_current_element) {
     if((*LINK_current_element).SOURCE.plane > PLANE_no) {
       (*LINK_current_element).SOURCE.plane--;
     }
     if((*LINK_current_element).TARGET.plane > PLANE_no) {
       (*LINK_current_element).TARGET.plane--;
     }
     LINK_current_element = (*LINK_current_element).next;
   }

   LINK_current_element = LINK_last_element;
*/
   PLANE_length--;

   /* Update the three pointers "PLANE_current_element", "PLANE_first_element"
      "PLANE_last_element."  */

   if(PLANE_length == 0){
     PLANE_current_element = NULL;
     PLANE_first_element   = NULL;
     PLANE_last_element    = NULL;
     PLANE_no--;
   }
   else if((*PLANE_element).next == NULL) {
     PLANE_current_element = (*PLANE_element).before;
     PLANE_last_element    = PLANE_current_element;
     PLANE_no--;
   }
   else if((*PLANE_element).before == NULL) {
     PLANE_current_element = (*PLANE_element).next;
     PLANE_first_element   = PLANE_current_element;
   }
   else {
     PLANE_current_element = (*PLANE_element).next;
   }

   free(PLANE_element);
 }
}

/*****************************************************************************
  FUNCTION : forward_plane
 
  PURPOSE  : moves forward one list element in the plane list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/

static int forward_plane(void)

{
 if(PLANE_length != 0) {
   if((*PLANE_current_element).next != NULL) {
     PLANE_current_element = (*PLANE_current_element).next;
     PLANE_no++;
   }
   return 1;
 }
 else {
  return 0;
 }
}

/*****************************************************************************
  FUNCTION : backward_link
 
  PURPOSE  : moves backward one plane element in the link list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/

static int backward_plane(void)

{
 if(PLANE_length != 0) {
   if((*PLANE_current_element).before != NULL) {
     PLANE_current_element = (*PLANE_current_element).before;
     PLANE_no--;
   }
   return 1;
 }
 else {
   return 0;
 }
}

/*****************************************************************************
  FUNCTION : beginning_plane
 
  PURPOSE  : jumps to the beginning of the plane list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/

static int beginning_plane(void)

{
 if(PLANE_length != 0){
   PLANE_current_element = PLANE_first_element;
   PLANE_no = 1;
   return 1;
 }
 else {
  return 0;
 }
}

/*****************************************************************************
  FUNCTION : end_plane
 
  PURPOSE  : jumps to the end of the plane list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/

static int end_plane(void)

{
 if(PLANE_length != 0) {
   PLANE_current_element = PLANE_last_element;
   PLANE_no = PLANE_length;
   return 1;
 }
 else {
   return 0;
 }
}

/* end of file */





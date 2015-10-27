/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_TD_bignet.c,v $
  SHORTNAME      : bn_TD_bignet
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the BIGNET-window for TDNNs. 
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 09.02.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:37 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "kr_typ.h"
#include "kernel.h"
#include "ui.h"
#include "kr_ui.h"
#include "ui_utilP.h"
#include "ui_textP.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"
#include "ui_fileP.h"
#include "ui_display.h"
#include "bn_basics.h"
#include "bn_bignet.h"
#include "ui_confirmer.h"

#include "bn_TD_bignet.ph"



/*****************************************************************************
  FUNCTION : bn_createPlanePanel

  PURPOSE  : creates the plane panel, which is part of the bignet-window
  NOTES    :
  RETURNS  :
  UPDATE   : 09.02.1993
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

    z1s1 = ui_xCreateLabelItem ("Plane                      ", pannel, 
				colom1 * bn_fontWidth, NULL, NULL); 
    z1s2 = ui_xCreateLabelItem ("Current Plane  ", pannel, 
				colom2 * bn_fontWidth, z1s1, NULL); 
    z1s3 = ui_xCreateLabelItem ("Edit Plane  ", pannel, 
				colom3 * bn_fontWidth, z1s2, NULL); 

    z2s1   = ui_xCreateLabelItem ("                     Plane:", pannel, 
				  colom1 * bn_fontWidth, NULL, z1s1); 
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth, z2s1, z1s1); 
    z2s2   = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,z1s1);

    z3s1  = ui_xCreateLabelItem ("                      Type: ",pannel,
				 colom1 * bn_fontWidth,NULL,z2s1); 
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z3s1,z2s1); 
    z3s2  = ui_xCreateFrameLabelItem ("",pannel,td_bn_intWidth,dummy,z2s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z3s2,z2s1);
    z3s3  = ui_xCreateFrameLabelItem (td_plane_type[TD_PLANE_type],pannel,
				      td_bn_intWidth,dummy,z2s1);

    z4s1  = ui_xCreateLabelItem ("      No. of feature units:",pannel,
				 colom1 * bn_fontWidth,NULL,z3s1); 
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth,z4s1,z3s1); 
    z4s2  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,z3s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 5 * bn_fontWidth,z4s2,z3s1); 
    z4s3  = ui_xCreateDialogItem ("z4s3", pannel,"",td_bn_intWidth,dummy,z3s1);
 
    z5s1  = ui_xCreateLabelItem ("        Total delay length: ",pannel,
				 colom1 * bn_fontWidth,NULL,z4s1); 
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth,z5s1,z4s1); 
    z5s2  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,z4s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 5 * bn_fontWidth,z5s2,z4s1); 
    z5s3  = ui_xCreateDialogItem ("z4s3",pannel, "",td_bn_intWidth,dummy,z4s1);
 

    z6s1  = ui_xCreateLabelItem ("z-coordinates of the plane: ",pannel,
				 colom1 * bn_fontWidth,NULL,z5s1); 
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z6s1,z5s1); 
    z6s2  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,z5s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z6s2,z5s1); 
    z6s3  = ui_xCreateDialogItem ("z4s3",pannel,"",td_bn_intWidth,dummy,z5s1);
 
    z7s1  = ui_xCreateLabelItem ("             Rel. Position: ",pannel,
				 colom1 * bn_fontWidth,NULL,z6s1); 
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z7s1,z6s1); 
    z7s2  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,z6s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z7s2,z6s1);
    z7s3  = ui_xCreateFrameLabelItem (td_plane_pos[TD_PLANE_pos],pannel,
				      td_bn_intWidth,dummy,z6s1);

    dummy         = ui_xCreateLabelItem (" ",pannel,
					 15 * bn_fontWidth,NULL,z7s1); 
    
    plane_edit = ui_xCreateLabelItem ("Edit Plane: ",pannel,
				      15 * bn_fontWidth,NULL,dummy); 
    plane_enter = ui_xCreateButtonItem("enter",pannel,plane_edit,dummy);
    XtAddCallback(plane_enter,XtNcallback,
		  (XtCallbackProc)enter_plane_PROC,NULL);

    plane_insert = ui_xCreateButtonItem ("insert",pannel,
						plane_enter,dummy);
    XtAddCallback(plane_insert,XtNcallback,
		  (XtCallbackProc)insert_plane_PROC,NULL);

    plane_overwrite = ui_xCreateButtonItem ("overwrite",pannel,
						   plane_insert,dummy);
    XtAddCallback(plane_overwrite,XtNcallback,
		  (XtCallbackProc)overwrite_plane_PROC,NULL);

    plane_delete = ui_xCreateButtonItem ("delete",pannel,
						plane_overwrite,dummy);
    XtAddCallback(plane_delete,XtNcallback,
		  (XtCallbackProc)delete_plane_PROC,NULL);

    dummy = ui_xCreateLabelItem(" ",pannel,15 * bn_fontWidth,NULL,plane_edit); 
    plane_to_edit = ui_xCreateButtonItem ("plane_to_edit",pannel,
						 dummy,plane_edit);
    XtAddCallback(plane_to_edit,XtNcallback,
		  (XtCallbackProc)current_plane_to_editor_PROC,NULL);

    type = ui_xCreateButtonItem ("type",pannel,plane_to_edit,
					plane_edit);
    XtAddCallback(type,XtNcallback,(XtCallbackProc)type_PROC,NULL);

    pos = ui_xCreateButtonItem ("pos",pannel,type,plane_edit);
    XtAddCallback(pos,XtNcallback,(XtCallbackProc)pos_PROC,NULL);

    current_plane = ui_xCreateLabelItem ("Current plane: ",pannel,
					 15 * bn_fontWidth,NULL,dummy); 
    plane_first = ui_xCreateButtonItem ("first",pannel,current_plane,
					       dummy);
    XtAddCallback(plane_first,XtNcallback,
		  (XtCallbackProc)beginning_plane_PROC,NULL);

    plane_prev = ui_xCreateButtonItem("prev",pannel,plane_first,dummy);
    XtAddCallback(plane_prev,XtNcallback,
		  (XtCallbackProc)backward_plane_PROC,NULL);

    plane_next = ui_xCreateButtonItem ("next",pannel,plane_prev,dummy);
    XtAddCallback(plane_next,XtNcallback,
		  (XtCallbackProc)forward_plane_PROC,NULL);

    plane_last = ui_xCreateButtonItem ("last",pannel,plane_next,dummy);
    XtAddCallback(plane_last,XtNcallback,(XtCallbackProc)end_plane_PROC,NULL);
}



/*****************************************************************************
  FUNCTION : bn_createLinkPanel

  PURPOSE  : creates the link panel, which is part of the bignet-window
  NOTES    :
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void bn_createLinkPannel (Widget parent)

{
    Widget pannel;
    Widget dummy,dummy1,dummy2;
    Widget link_edit,current_link;
    Widget link_enter,link_delete,link_overwrite,link_to_edit;
    Widget link_first,link_last,link_prev,link_next;
    Arg          arg[25];
    Cardinal     n;
  
    n = 0;  

    pannel = XtCreateManagedWidget("pannel", formWidgetClass, parent, arg, n);



    dummy1 = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,NULL); 
    z8s2   = ui_xCreateLabelItem (" Current Link ", pannel,
				  14*bn_fontWidth,dummy1,NULL); 
    z8s3   = ui_xCreateLabelItem ("       Edit Link", pannel,
				  16*bn_fontWidth,z8s2,NULL); 
    dummy2 = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,dummy1); 
    z9s1   = ui_xCreateLabelItem ("Source", pannel,
				  8*bn_fontWidth,dummy2,dummy1); 
    z9s2   = ui_xCreateLabelItem ("Target", pannel,
				  8*bn_fontWidth,z9s1,dummy1); 
    z9s3   = ui_xCreateLabelItem ("  Source", pannel,
				  8*bn_fontWidth,z9s2,dummy1); 
    z9s4   = ui_xCreateLabelItem (" Target", pannel,
				  8*bn_fontWidth,z9s3,dummy1); 
    z10s1  = ui_xCreateLabelItem ("Plane", pannel,
				  15*bn_fontWidth,NULL,dummy2); 
    z10s2  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,z10s1,dummy2);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z10s2,dummy2); 
    z10s3  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,dummy2);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z10s3,dummy2); 
    z10s4  = ui_xCreateDialogItem("z10s4", pannel,"",td_bn_intWidth,
				  dummy,dummy2);
    dummy  = ui_xCreateLabelItem(" ",pannel,1 * bn_fontWidth,z10s4,dummy2); 
    z10s5  = ui_xCreateDialogItem("z10s5", pannel,"",td_bn_intWidth,
				  dummy,dummy2);
    z11s1  = ui_xCreateLabelItem(" ", pannel, 15*bn_fontWidth,NULL,z10s1); 

    z12s1  = ui_xCreateLabelItem ("     Receptive Field Coordinates", pannel,
				  30*bn_fontWidth,dummy2,z11s1); 
    z13s1  = ui_xCreateLabelItem ("1st feat. unit:", pannel,
				  15*bn_fontWidth,NULL,z12s1); 
    z13s2  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,z13s1,z12s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z13s2,z12s1); 
    z13s3  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,z12s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z13s3,z12s1); 
    z13s4  = ui_xCreateDialogItem("z13s4", pannel, "",td_bn_intWidth,
				  dummy,z12s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z13s4,z12s1); 
    z13s5  = ui_xCreateDialogItem("z13s5", pannel, "",td_bn_intWidth,dummy,
				  z12s1);
    z15s1  = ui_xCreateLabelItem ("         width:", pannel,
				  15*bn_fontWidth,NULL,z13s1); 
    z15s2  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,z15s1,z13s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z15s2,z13s1); 
    z15s3  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,z13s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z15s3,z13s1); 
    z15s4  = ui_xCreateDialogItem("z15s4", pannel, "",td_bn_intWidth,
				  dummy,z13s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z15s4,z13s1); 
    z15s5  = ui_xCreateDialogItem("z15s5", pannel, "",td_bn_intWidth,
				  dummy,z13s1);

    z16s1  = ui_xCreateLabelItem ("  delay length:", pannel,
				  15*bn_fontWidth,NULL,z15s1); 
    z16s2  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,z16s1,z15s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z16s2,z15s1); 
    z16s3  = ui_xCreateFrameLabelItem ("", pannel,td_bn_intWidth,dummy,z15s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z16s3,z15s1); 
    z16s4  = ui_xCreateDialogItem("z16s4", pannel, "",td_bn_intWidth,
				  dummy,z15s1);

    dummy  = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,z16s1); 
    link_edit = ui_xCreateLabelItem ("Edit Link: ",pannel,
				     15 * bn_fontWidth,NULL,dummy); 
    link_enter = ui_xCreateButtonItem ("enter",pannel,link_edit,dummy);
    XtAddCallback(link_enter,XtNcallback,(XtCallbackProc)enter_link_PROC,NULL);

    link_overwrite = ui_xCreateButtonItem ("overwrite",pannel,
						  link_enter,dummy);
    XtAddCallback(link_overwrite,XtNcallback,
		  (XtCallbackProc)overwrite_link_PROC,NULL);

    link_to_edit = ui_xCreateButtonItem ("link_to_edit",pannel,
						link_overwrite,dummy);
    XtAddCallback(link_to_edit,XtNcallback,
		  (XtCallbackProc)current_link_to_editor_PROC,NULL);

    link_delete = ui_xCreateButtonItem ("delete",pannel,
					       link_to_edit,dummy);
    XtAddCallback(link_delete,XtNcallback,
		  (XtCallbackProc)delete_link_PROC,NULL);

    dummy = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,link_edit); 
    current_link = ui_xCreateLabelItem ("Current Link: ",pannel,
					15*bn_fontWidth,NULL,link_edit); 
    link_first = ui_xCreateButtonItem ("first",pannel,
					      current_link,link_edit);
    XtAddCallback(link_first,XtNcallback,
		  (XtCallbackProc)beginning_link_PROC,NULL);

    link_prev = ui_xCreateButtonItem ("prev",pannel,link_first,link_edit);
    XtAddCallback(link_prev,XtNcallback,
		  (XtCallbackProc)backward_link_PROC,NULL);

    link_next = ui_xCreateButtonItem ("next",pannel,link_prev,link_edit);
    
    XtAddCallback(link_next,XtNcallback,
		  (XtCallbackProc)forward_link_PROC,NULL);
    
    link_last = ui_xCreateButtonItem ("last",pannel,link_next,link_edit);
      XtAddCallback(link_last,XtNcallback,(XtCallbackProc)end_link_PROC,NULL);

  }



/*****************************************************************************
  FUNCTION : exit_PROC

  PURPOSE  : callback function of the exit-buttom. You are leaving bignet.
  NOTES    :
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void exit_PROC (void)

{
    XtDestroyWidget (td_baseWidget);
    td_bignet_widget_open = 0;
}



/*****************************************************************************
  FUNCTION : bn_create_TD_Bignet

  PURPOSE  : creates the bignet-window.
  NOTES    :
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

void bn_create_TD_Bignet (void)

{
    Widget       box;
    Widget       create_net,exit,cancel;
    Arg          arg[25];
    Cardinal     n;
    char         buf[40];

    if(td_bignet_widget_open) {
	XRaiseWindow (XtDisplay (td_baseWidget), XtWindow (td_baseWidget));
	return;
    }


    sprintf (buf, "SNNS BigNet (Time Delay)");

    n = 0;  
    td_baseWidget = XtCreatePopupShell (buf, topLevelShellWidgetClass, 
					ui_toplevel, arg, n); 
    box = XtCreateManagedWidget ("box",boxWidgetClass,td_baseWidget,arg,n);
    bn_createPlanePannel (box); 
    bn_createLinkPannel(box);

    create_net = ui_xCreateButtonItem("create_td_net",box,NULL,NULL);
    XtAddCallback(create_net,XtNcallback,
		  (XtCallbackProc)create_net_PROC,NULL);
    exit = ui_xCreateButtonItem ("done",box,create_net,NULL);
    XtAddCallback(exit,XtNcallback,(XtCallbackProc)exit_PROC,NULL);
    cancel     = ui_xCreateButtonItem ("cancel",box,exit,NULL);
    XtAddCallback(cancel,XtNcallback,(XtCallbackProc)cancel_net_PROC,NULL);

    ui_checkWindowPosition(td_baseWidget);
    XtPopup (td_baseWidget, XtGrabNone);
    ui_xDontResizeWidget(td_baseWidget);
    td_bignet_widget_open = 1;
    if(TD_PLANE_length != 0){
	write_current_plane();
	if(TD_LINK_length != 0) {
	    write_current_link();
	}
    }

}


/*****************************************************************************
  FUNCTION : create_net_PROC

  PURPOSE  : creates a SNNS-net in two steps:
             1. calls up a function, which creates SNNS-units
             2. calls up a function, which connections the SNNS-units.
  NOTES    : There are nine different ways 
             (PLANE_TO_PLANE, PLANE_TO_CLUSTER ...) 
             two different planes can be connected.
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void create_net_PROC(void)

{
    TD_LINK  *TD_LINK_element=TD_LINK_first_element;
    TD_LINK  *TD_LINK_test=TD_LINK_first_element;
    TD_PLANE *source_plane,*target_plane;
    TD_PLANE *test_plane;
    int move,type_of_connection=1;
    int create=1;
    int j,k;
    int correct = 0;
    int overlap =0;
    char buf1[80],buf2[80];


    TD_BN_ERROR = 0;
    TD_NET = 1;

    strcpy(buf1,"Unused feature units in Plane ");
    strcpy(buf2,"Overlapping Receptive Field in Plane ");


    /* check whether all feature units are used or any
       receptive fields overlap in the target plane  */


    TD_LINK_test = TD_LINK_first_element;

    for(k=1; k<=TD_PLANE_length; k++){           /* foreach plane        */
	test_plane = get_plane(k);
	for(j=1; j<=(*test_plane).width; j++){   /* foreach feature unit */
	    overlap = 0;
	    while(TD_LINK_test != NULL){         /* foreach link set     */
		if(TD_LINK_test->SOURCE.plane == k){
		    if((j >= (*TD_LINK_test).SOURCE.CLUSTER.x) &&
		       (j < ((*TD_LINK_test).SOURCE.CLUSTER.x +
			     (*TD_LINK_test).SOURCE.CLUSTER.width))){
			correct++;
		    }
		}else if(TD_LINK_test->TARGET.plane == k){
		    if((j >= TD_LINK_test->TARGET.CLUSTER.x) &&
		       (j < (TD_LINK_test->TARGET.CLUSTER.x +
			     TD_LINK_test->TARGET.CLUSTER.width))){
			overlap++;
			if(overlap > 1){
			    /* receptive fields overlap */
			    sprintf(buf2,"%s %d",buf2,k);
			    ui_confirmOk(buf2);
			    return;
			}
		    }
		}
		TD_LINK_test = TD_LINK_test->next;
	    }
	    if((correct < 1) && (k != TD_PLANE_length)){ 
		/* not all feature units are used */
		sprintf(buf1,"%s %d",buf1,k);
		ui_confirmOk(buf1);
		return;
	    }
	    TD_LINK_test = TD_LINK_first_element;
	    correct = 0;
	}
    }


    if(krui_getNoOfUnits() != 0) {
	if(! ui_confirmYes("Create will erase current network. Create?")) {
	    create = 0;
	}
    }
 
    if(create && (TD_PLANE_length != 0)) {

	krui_deleteNet();

	calculate_first_snns_unit_no_of_plane();
	create_snns_unit(); 

	while((TD_LINK_element != NULL) && (! TD_BN_ERROR)) {

	    move               = (*TD_LINK_element).move;
	    type_of_connection = (*TD_LINK_element).type_of_connection;
	    source_plane       =  get_plane((*TD_LINK_element).SOURCE.plane);
	    target_plane       =  get_plane((*TD_LINK_element).TARGET.plane);
   
	    switch(type_of_connection) {
	      case PLANE_TO_PLANE: {
		  make_link(source_plane,0,0,(*source_plane).width,
			    (*source_plane).height,target_plane,0,0,
			    (*target_plane).width,(*target_plane).height);
		  break;
	      }
	      case PLANE_TO_CLUSTER: {
		  make_link(source_plane,0,0,(*source_plane).width,
			    (*source_plane).height,target_plane,
			    (*TD_LINK_element).TARGET.CLUSTER.x-1,
			    (*TD_LINK_element).TARGET.CLUSTER.y-1,
			    (*TD_LINK_element).TARGET.CLUSTER.width,
			    (*TD_LINK_element).TARGET.CLUSTER.height);
		  break;
	      }
	      case PLANE_TO_UNIT: {
		  make_link(source_plane,0,0,
			    (*source_plane).width,(*source_plane).height,
			    target_plane,(*TD_LINK_element).TARGET.UNIT.x-1,
			    (*TD_LINK_element).TARGET.UNIT.y-1,1,1);
		  break;
	      }  
	      case CLUSTER_TO_PLANE: {
		  make_link(source_plane,(*TD_LINK_element).SOURCE.CLUSTER.x-1,
			    (*TD_LINK_element).SOURCE.CLUSTER.y-1,
			    (*TD_LINK_element).SOURCE.CLUSTER.width,
			    (*TD_LINK_element).SOURCE.CLUSTER.height,
			    target_plane,0,0,
			    (*target_plane).width,(*target_plane).height);
		  break;
	      }
	      case CLUSTER_TO_CLUSTER: {
		  if(move) {
		      make_move_link(source_plane,
				     (*TD_LINK_element).SOURCE.CLUSTER.x-1,
				     (*TD_LINK_element).SOURCE.CLUSTER.y-1,
				     (*TD_LINK_element).SOURCE.CLUSTER.width,
				     (*TD_LINK_element).SOURCE.CLUSTER.height,
				     target_plane,
				     (*TD_LINK_element).TARGET.CLUSTER.x-1,
				     (*TD_LINK_element).TARGET.CLUSTER.y-1,
				     (*TD_LINK_element).TARGET.CLUSTER.width,
				     (*TD_LINK_element).TARGET.CLUSTER.height,
				     (*TD_LINK_element).SOURCE.MOVE.delta_x,
				     (*TD_LINK_element).SOURCE.MOVE.delta_y,
				     (*TD_LINK_element).TARGET.MOVE.delta_x,
				     (*TD_LINK_element).TARGET.MOVE.delta_y);
		  }else{
		      make_link(source_plane,
				(*TD_LINK_element).SOURCE.CLUSTER.x-1,
				(*TD_LINK_element).SOURCE.CLUSTER.y-1,
				(*TD_LINK_element).SOURCE.CLUSTER.width,
				(*TD_LINK_element).SOURCE.CLUSTER.height,
				target_plane,
				(*TD_LINK_element).TARGET.CLUSTER.x-1,
				(*TD_LINK_element).TARGET.CLUSTER.y-1,
				(*TD_LINK_element).TARGET.CLUSTER.width,
				(*TD_LINK_element).TARGET.CLUSTER.height);
		  }
		  break;
	      }
	      case CLUSTER_TO_UNIT: {
		  if(move) {
		      make_move_link(source_plane,
				     (*TD_LINK_element).SOURCE.CLUSTER.x-1,
				     (*TD_LINK_element).SOURCE.CLUSTER.y-1,
				     (*TD_LINK_element).SOURCE.CLUSTER.width,
				     (*TD_LINK_element).SOURCE.CLUSTER.height,
				     target_plane,
				     (*TD_LINK_element).TARGET.UNIT.x-1,
				     (*TD_LINK_element).TARGET.UNIT.y-1,1,1,
				     (*TD_LINK_element).SOURCE.MOVE.delta_x,
				     (*TD_LINK_element).SOURCE.MOVE.delta_y,
				     (*TD_LINK_element).TARGET.MOVE.delta_x,
				     (*TD_LINK_element).TARGET.MOVE.delta_y);
		  }else{
		      make_link(source_plane,
				(*TD_LINK_element).SOURCE.CLUSTER.x-1,
				(*TD_LINK_element).SOURCE.CLUSTER.y-1,
				(*TD_LINK_element).SOURCE.CLUSTER.width,
				(*TD_LINK_element).SOURCE.CLUSTER.height,
				target_plane,
				(*TD_LINK_element).TARGET.UNIT.x-1,
				(*TD_LINK_element).TARGET.UNIT.y-1,1,1);
		  }
		  break;
	      }
	      case UNIT_TO_PLANE: {
		  make_link(source_plane,(*TD_LINK_element).SOURCE.UNIT.x-1,
			    (*TD_LINK_element).SOURCE.UNIT.y-1,1,1,
			    target_plane,0,0,
			    (*target_plane).width,(*target_plane).height);
		  break;
	      }
	      case UNIT_TO_CLUSTER: {
		  if(move) {
		      make_move_link(source_plane,
				     (*TD_LINK_element).SOURCE.UNIT.x-1,
				     (*TD_LINK_element).SOURCE.UNIT.y-1,1,1,
				     target_plane,
				     (*TD_LINK_element).TARGET.CLUSTER.x-1,
				     (*TD_LINK_element).TARGET.CLUSTER.y-1,
				     (*TD_LINK_element).TARGET.CLUSTER.width,
				     (*TD_LINK_element).TARGET.CLUSTER.height,
				     (*TD_LINK_element).SOURCE.MOVE.delta_x,
				     (*TD_LINK_element).SOURCE.MOVE.delta_y,
				     (*TD_LINK_element).TARGET.MOVE.delta_x,
				     (*TD_LINK_element).TARGET.MOVE.delta_y);
		  }else{
		      make_link(source_plane,
				(*TD_LINK_element).SOURCE.UNIT.x-1,
				(*TD_LINK_element).SOURCE.UNIT.y-1,1,1,
				target_plane,
				(*TD_LINK_element).TARGET.CLUSTER.x-1,
				(*TD_LINK_element).TARGET.CLUSTER.y-1,
				(*TD_LINK_element).TARGET.CLUSTER.width,
				(*TD_LINK_element).TARGET.CLUSTER.height);
		  }
		  break;
	      }
	      case UNIT_TO_UNIT: {
		  if(move) {
		      make_move_link(source_plane,
				     (*TD_LINK_element).SOURCE.UNIT.x-1,
				     (*TD_LINK_element).SOURCE.UNIT.y-1,1,1,
				     target_plane,
				     (*TD_LINK_element).TARGET.UNIT.x-1,
				     (*TD_LINK_element).TARGET.UNIT.y-1,1,1,
				     (*TD_LINK_element).SOURCE.MOVE.delta_x,
				     (*TD_LINK_element).SOURCE.MOVE.delta_y,
				     (*TD_LINK_element).TARGET.MOVE.delta_x,
				     (*TD_LINK_element).TARGET.MOVE.delta_y);
		  }else{
		      make_link(source_plane,
				(*TD_LINK_element).SOURCE.UNIT.x-1,
				(*TD_LINK_element).SOURCE.UNIT.y-1,1,1,
				target_plane,
				(*TD_LINK_element).TARGET.UNIT.x-1,
				(*TD_LINK_element).TARGET.UNIT.y-1,1,1);
		  }
		  break;
	      }
	    }			/* switch */

	    TD_LINK_element = (*TD_LINK_element).next;

	}			/* while */    

	bn_basics_refresh();

    }				/* if */ 

}


/*****************************************************************************
  FUNCTION : calculate_first_snns_unit_no_of_plane

  PURPOSE  : calculates the SNNS unit no of the left upper corner of every
             plane.

  NOTES    : relative to this unit no, knowing the width and the height of the
             planes, all other SNNS unit numbers of every plane can be 
             calculated. That's important for the function create_snns_unit.
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void calculate_first_snns_unit_no_of_plane(void)

{
    TD_PLANE *TD_PLANE_element = TD_PLANE_first_element;

    (*TD_PLANE_element).begin = 1;  
    TD_PLANE_element = (*TD_PLANE_element).next;

    while(TD_PLANE_element != NULL) {
	(*TD_PLANE_element).begin = (*(*TD_PLANE_element).before).width *
	    (*(*TD_PLANE_element).before).height +
		(*(*TD_PLANE_element).before).begin;
	TD_PLANE_element = (*TD_PLANE_element).next; 
    }
}



/*****************************************************************************
  FUNCTION : create_snns_unit

  PURPOSE  : creates for every unit of a plane a SNNS default unit.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void create_snns_unit(void)

{
    struct PosType unit_pos;
    struct Unit *unit_ptr;

    TD_PLANE *TD_PLANE_element = TD_PLANE_first_element; 
    TD_LINK  *TD_LINK_Element;
    int plane_no = 1;
    int lun = 1; /* unit_number within a plane */
    int connection_type;
    int x,y,width,height,unit_no,pos,ret;
    int y_offset=0,relative_y_max=0,absolute_y_max=0,y_begin;
    int x_max=0,old_x_begin=2,new_x_begin,x_begin;
    Bool found;

    ret = krui_allocateUnits((*TD_PLANE_last_element).width * 
                             (*TD_PLANE_last_element).height +
			     (*TD_PLANE_last_element).begin-1);
    if(ret != 0){
	ui_tw_errorMessage(krui_error(ret));
	TD_BN_ERROR = 1;
    }

    while((! TD_BN_ERROR) && (TD_PLANE_element != NULL)) {

	height = (*TD_PLANE_element).height;
	width  = (*TD_PLANE_element).width;
	pos    = (*TD_PLANE_element).pos;
    
	y_begin = calculate_y_begin(&absolute_y_max,&relative_y_max,
				    &y_offset,height,pos);
	x_begin = calculate_x_begin(&new_x_begin,&old_x_begin,
				    &x_max,width,pos);

	for(y=0;y<height;y++){
	    for(x=0;x<width;x++){
		unit_no = krui_createDefaultUnit();
		if(unit_no<0) ui_checkError(unit_no);
		unit_ptr = kr_getUnitPtr(unit_no);
		ret = krui_setUnitTType(unit_no,(*TD_PLANE_element).type+1);
		if(ret != 0){
		    ui_tw_errorMessage(krui_error(ret));
		    TD_BN_ERROR = 1;
		}

		connection_type = 0;
		found = FALSE;
		TD_LINK_Element = TD_LINK_first_element;
		do {
		    if (TD_LINK_Element->TARGET.plane == plane_no) {
			connection_type=TD_LINK_Element->type_of_connection;
			found = TRUE;
		    }
		    TD_LINK_Element=TD_LINK_Element->next;
		} while ((TD_LINK_Element != NULL) && (!found));

		unit_ptr->TD.target_offset = -y*width;
		unit_ptr->TD.source_offset = 0;

		if (connection_type == CLUSTER_TO_CLUSTER) { 
		    /* recept. field */
		    unit_ptr->TD.td_connect_typ = 1;
		}else{
		    unit_ptr->TD.td_connect_typ = 0; /* fully connected */
		}
         
		unit_ptr->lun = lun;  /* set logical unitno. */
		unit_ptr->lln = plane_no;  /* set logical layerno.*/ 


		unit_pos.x = x_begin + x;
		unit_pos.y = y_begin + y;
		krui_setUnitPosition(unit_no,&unit_pos);
		lun++;

		krui_setUnitActFunc(unit_no, "Act_TD_Logistic");
	    }	
	}		
	TD_PLANE_element = (*TD_PLANE_element).next;
	plane_no++;
	lun = 1;   /* reset logical unitnumber */
    } /*while*/
}



/*****************************************************************************
  FUNCTION : make_link

  PURPOSE  : creates the links between two planes.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void make_link(TD_PLANE *TD_LINK_source_plane,
		      int TD_LINK_source_cluster_x,
		      int TD_LINK_source_cluster_y,
		      int TD_LINK_source_cluster_width,
		      int TD_LINK_source_cluster_height,
		      TD_PLANE *TD_LINK_target_plane,
		      int TD_LINK_target_cluster_x,
		      int TD_LINK_target_cluster_y,
		      int TD_LINK_target_cluster_width,
		      int TD_LINK_target_cluster_height)

{
    int i,j,k,l,ret,target_unit,source_unit;
    int source_features;
    struct Unit *t_ptr;

    source_features = TD_LINK_source_plane -> width;
    for (i = 0; i < TD_LINK_target_plane->height; i++)
	for (j = 0; j < TD_LINK_target_plane->width; j++)
	{
	    target_unit = TD_LINK_target_plane->begin 
		+ (i * TD_LINK_target_plane->width) + j;
	    ret = krui_setCurrentUnit(target_unit);
	    if(ret != 0)
	    {
		ui_tw_errorMessage(krui_error(ret));
		TD_BN_ERROR = 1;
		krui_deleteNet();
		return;
	    }
	    t_ptr = kr_getUnitPtr(target_unit);
	    t_ptr -> TD.source_offset = source_features * i;
	}
    for(i = TD_LINK_target_cluster_y;
	i < TD_LINK_target_cluster_height+TD_LINK_target_cluster_y;
	i++)
	for(j = TD_LINK_target_cluster_x;
	    j < TD_LINK_target_cluster_width+TD_LINK_target_cluster_x;
	    j++){

	    target_unit = (*TD_LINK_target_plane).begin + 
		(*TD_LINK_target_plane).width * i + j;
	    ret = krui_setCurrentUnit(target_unit);
	    if(ret != 0){
		ui_tw_errorMessage(krui_error(ret));
		TD_BN_ERROR = 1;
		krui_deleteNet();
		return;
	    }
	    for(k = TD_LINK_source_cluster_y;
		k < TD_LINK_source_cluster_height+TD_LINK_source_cluster_y;
		k++)
		for(l = TD_LINK_source_cluster_x;
		    l < TD_LINK_source_cluster_width+TD_LINK_source_cluster_x;
		    l++){
           
		    source_unit = (*TD_LINK_source_plane).begin + 
			(*TD_LINK_source_plane).width * k + l;
		    if((ret = krui_createLink(source_unit,0.0)) != 0){
			ui_tw_errorMessage(krui_error(ret));
			TD_BN_ERROR = 1;
			krui_deleteNet();
			return;
		    }
		} 
	}             
}



/*****************************************************************************
  FUNCTION : make_move_link

  PURPOSE  : while "moving" (see docu)  is possible the function make_link
             is called up.
  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void make_move_link(TD_PLANE *TD_LINK_source_plane,
			   int TD_LINK_source_cluster_x,
			   int LINK_source_cluster_y,
                           int TD_LINK_source_cluster_width,
			   int TD_LINK_source_cluster_height,
                           TD_PLANE * TD_LINK_target_plane,
			   int TD_LINK_target_cluster_x,
			   int LINK_target_cluster_y,
                           int TD_LINK_target_cluster_width,
			   int TD_LINK_target_cluster_height,
                           int TD_LINK_source_move_delta_x,
			   int TD_LINK_source_move_delta_y,
                           int TD_LINK_target_move_delta_x,
			   int TD_LINK_target_move_delta_y)

{
 int xs_pivot = TD_LINK_source_cluster_x;
 int xt_pivot = TD_LINK_target_cluster_x;

 do{
   make_link(TD_LINK_source_plane,TD_LINK_source_cluster_x,
	     TD_LINK_source_cluster_y,TD_LINK_source_cluster_width,
	     TD_LINK_source_cluster_height,TD_LINK_target_plane,
	     TD_LINK_target_cluster_x,TD_LINK_target_cluster_y,
             TD_LINK_target_cluster_width,TD_LINK_target_cluster_height);
 }while(! TD_BN_ERROR &&
	move_p(TD_LINK_source_plane,&TD_LINK_source_cluster_y,
	       &TD_LINK_source_cluster_x,TD_LINK_source_cluster_width,
	       TD_LINK_source_cluster_height,xs_pivot,
	       TD_LINK_source_move_delta_x,TD_LINK_source_move_delta_y) &&
	move_p(TD_LINK_target_plane,&TD_LINK_target_cluster_y,
	       &TD_LINK_target_cluster_x,TD_LINK_target_cluster_width,
	       TD_LINK_target_cluster_height,xt_pivot,
	       TD_LINK_target_move_delta_x,TD_LINK_target_move_delta_y));
}
    


/*****************************************************************************
  FUNCTION : move_p

  PURPOSE  : tests whether the boundaries of a plane are crossed while
             "moving" (see docu)

  NOTES    : 
  RETURNS  : 

  UPDATE   : 09.02.1993
******************************************************************************/

static int move_p(TD_PLANE *plane_no,int *y,int *x,int width,int height,
		  int x_pivot,int x_step,int y_step)

{
    if(x_step > 0 && ((*x)+width+x_step<=(*plane_no).width))
	(*x)+= x_step;
    else
	if(y_step > 0 && ((*y)+height+y_step<=(*plane_no).height))
	    {
		(*y) += y_step;
		(*x) = x_pivot;
	    }
	else
	    return 0;

    return 1;
}


/*****************************************************************************
  FUNCTION : read_link_elements

  PURPOSE  : reads the input datas of the link editor (Edit Link).

  NOTES    : 
  RETURNS  : @@@

  UPDATE   : 09.02.1993
******************************************************************************/

static int read_link_elements(void)

{
    TD_LINK_source_plane          = ui_xIntFromAsciiWidget(z10s4);
    TD_LINK_source_cluster_x      = ui_xIntFromAsciiWidget(z13s4);
    TD_LINK_source_cluster_y      = 1;
    TD_LINK_source_cluster_width  = ui_xIntFromAsciiWidget(z15s4);
    TD_LINK_source_cluster_height = ui_xIntFromAsciiWidget(z16s4);

    TD_LINK_target_plane          = ui_xIntFromAsciiWidget(z10s5);
    TD_LINK_target_cluster_x      = ui_xIntFromAsciiWidget(z13s5);
    TD_LINK_target_cluster_y      = 1;
    TD_LINK_target_cluster_width  = ui_xIntFromAsciiWidget(z15s5);
    TD_LINK_target_cluster_height = 1;


    return correct_link_input();
}


/*****************************************************************************
  FUNCTION : write_current_link

  PURPOSE  : writes the datas of the current_element to the control part 
             (Current Link) of the link editor.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void write_current_link(void)

{
    char buf[10];
  
    sprintf(buf,"%d",(*TD_LINK_current_element).SOURCE.plane);
    ui_xSetLabel(z10s2,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).SOURCE.CLUSTER.x);
    ui_xSetLabel(z13s2,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).SOURCE.CLUSTER.width);
    ui_xSetLabel(z15s2,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).SOURCE.CLUSTER.height);
    ui_xSetLabel(z16s2,td_bn_widget_output(buf));

    sprintf(buf,"%d",(*TD_LINK_current_element).TARGET.plane);
    ui_xSetLabel(z10s3,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).TARGET.CLUSTER.x);
    ui_xSetLabel(z13s3,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).TARGET.CLUSTER.width);
    ui_xSetLabel(z15s3,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).TARGET.CLUSTER.height);
    ui_xSetLabel(z16s3,td_bn_widget_output(buf));

}



/*****************************************************************************
  FUNCTION : clear_current_link

  PURPOSE  : Deletes the datas standing in Current Link.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void clear_current_link(void)

{
    ui_xSetLabel(z10s2,"");
    ui_xSetLabel(z13s2,"");
    ui_xSetLabel(z15s2,"");
    ui_xSetLabel(z16s2,"");

    ui_xSetLabel(z10s3,"");
    ui_xSetLabel(z13s3,"");
    ui_xSetLabel(z15s3,"");
    ui_xSetLabel(z16s3,"");
}



/*****************************************************************************
  FUNCTION : write_current_link_to_editor

  PURPOSE  : writes the datas of Current Link to Edit Link

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void write_current_link_to_editor(void)

{

    char buf[10];

    sprintf(buf,"%d",(*TD_LINK_current_element).SOURCE.plane);
    ui_xSetString(z10s4,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).SOURCE.CLUSTER.x);
    ui_xSetString(z13s4,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).SOURCE.CLUSTER.width);
    ui_xSetString(z15s4,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).SOURCE.CLUSTER.height);
    ui_xSetString(z16s4,td_bn_widget_output(buf));

    sprintf(buf,"%d",(*TD_LINK_current_element).TARGET.plane);
    ui_xSetString(z10s5,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).TARGET.CLUSTER.x);
    ui_xSetString(z13s5,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_LINK_current_element).TARGET.CLUSTER.width);
    ui_xSetString(z15s5,td_bn_widget_output(buf));

}



/*****************************************************************************
  FUNCTION : clear_edit_link

  PURPOSE  : Deletes the datas standing in Edit Link.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void clear_edit_link(void)

{
    ui_xSetString(z10s4,"");
    ui_xSetString(z13s4,"");
    ui_xSetString(z15s4,"");
    ui_xSetString(z16s4,"");

    ui_xSetString(z10s5,"");
    ui_xSetString(z13s5,"");
    ui_xSetString(z15s5,"");
}


/*****************************************************************************
  FUNCTION : enter_link_PROC
 
  PURPOSE  : callback function, which enters a link element at the end of the 
             link list.

  NOTES    : the link list contains all datas entered by the link editor.
             Every link element is a struct (see bn_types.c)
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void enter_link_PROC(void)

{
    if(read_link_elements()){
	enter_link();
	write_current_link();
	clear_edit_link();
    }
}



/*****************************************************************************
  FUNCTION : overwrite_link_PROC
 
  PURPOSE  : callback function, which overwrites the current link element
             (Current Link) with the datas of the link editor (Edit Link).

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void overwrite_link_PROC(void)

{
    if(read_link_elements()) {
	overwrite_link();
	write_current_link();
	clear_edit_link();
    }
}



/*****************************************************************************
  FUNCTION : delete_link_PROC
 
  PURPOSE  : callback function, which deletes the Currenrt Link out of
             the link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void delete_link_PROC(void)

{
    delete_current_link();
    if(TD_LINK_length == 0) {
	clear_current_link();
    }else{
	write_current_link();
    }
}


/*****************************************************************************
  FUNCTION : forward_link_PROC
 
  PURPOSE  : callback function, which moves forward one list element in the
             link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void forward_link_PROC(void)

{
    if(forward_link()){
	write_current_link();
    }
}



/*****************************************************************************
  FUNCTION : backward_link_PROC
 
  PURPOSE  : callback function, which moves backward one list element in the
             link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void backward_link_PROC(void)

{
    if(backward_link()) {
	write_current_link();
    }
}



/*****************************************************************************
  FUNCTION : end_link_PROC
 
  PURPOSE  : callback function, which jumps to the end of the link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void end_link_PROC(void)

{
    if(end_link()) {
	write_current_link();
    }
}



/*****************************************************************************
  FUNCNCTION : beginning_link_PROC
 
  PURPOSE  : callback function, which jumps to the beginning of the link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void beginning_link_PROC(void)

{
    if(beginning_link()) {
	write_current_link();
    }
}



/*****************************************************************************
  FUNCNCTION : current_link_to_editor_PROC
 
  PURPOSE  : callback function, which writes the Current Link to Edit Link

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void current_link_to_editor_PROC(void)

{
    if(TD_LINK_length != 0) {
	write_current_link_to_editor();
    }else{
	clear_edit_link();
    }
}



/*****************************************************************************
  FUNCNCTION : correct_link_input
 
  PURPOSE  : tests whether the input datas of Edit Link are correct and decides
             what kind of link (PLANE_TO_PLANE, PLANE_TO_LINK...) we have.

  NOTES    : 
  RETURNS  : Correct = 1 ; ERROR = 0

  UPDATE   : 09.02.1993
******************************************************************************/

static int correct_link_input(void)

{
    TD_PLANE *TD_PLANE_source_no,*TD_PLANE_target_no;

    int link_type;
    int target_plane=0,
    source_plane=0;
    int target_cluster=0,
    source_cluster=0;
    int target_unit=0,
    source_unit=0;
    int a,b,c,d;
    char buf[200];

    TD_PLANE_source_no = get_plane(TD_LINK_source_plane);
    TD_PLANE_target_no = get_plane(TD_LINK_target_plane);

    TD_LINK_move = 0;

    if((TD_PLANE_source_no == TD_PLANE_target_no) ||
       TD_PLANE_source_no == NULL ||
       TD_PLANE_target_no == NULL) {
	ui_confirmOk("Source and target plane need to be defined");
	return 0;
    }
	
    if ((TD_LINK_source_cluster_x + TD_LINK_source_cluster_width  >
	 (*TD_PLANE_source_no).width + 1) ||
	(TD_LINK_source_cluster_y + TD_LINK_source_cluster_height > 
	 (*TD_PLANE_source_no).height + 1) ||
	(TD_LINK_source_unit_x > (*TD_PLANE_source_no).width) ||
	(TD_LINK_source_unit_y > (*TD_PLANE_source_no).height)) {
	ui_confirmOk("Source receptive field exceeds bounds of source plane");
	return 0;
    }

    if ((TD_LINK_target_cluster_x + TD_LINK_target_cluster_width  >
	 (*TD_PLANE_target_no).width + 1) ||
	(TD_LINK_target_cluster_y + TD_LINK_target_cluster_height > 
	 (*TD_PLANE_target_no).height + 1) ||
	(TD_LINK_target_unit_x > (*TD_PLANE_target_no).width) ||
	(TD_LINK_target_unit_y > (*TD_PLANE_target_no).height)) {
	ui_confirmOk("Target receptive field exceeds bounds of target plane");
	return 0;
    }

    target_plane = TD_TARGET_PLANE;
    source_plane = TD_SOURCE_PLANE;

    a=b=c=d=0;

    if(((a=TD_LINK_source_cluster_x) >= 1) &&
       ((b=TD_LINK_source_cluster_y) >= 1) &&
       ((c=TD_LINK_source_cluster_width) >= 1) &&
       ((d=TD_LINK_source_cluster_height) >= 1)) {
	source_cluster = TD_SOURCE_CLUSTER;
    }

    if((! source_cluster) && a+b+c+d) {
	ui_confirmOk("Source receptive field not properly defined");
	return 0;
    }

    a=b=0;

    if(((a=TD_LINK_source_unit_x) >= 1) &&
       ((b=TD_LINK_source_unit_y) >= 1)) {
	source_unit = TD_SOURCE_UNIT;
    }

    if((! source_unit) && a+b) {
	ui_confirmOk("Source unit not properly defined");
	return 0;
    }

    a=b=c=d=0;

    if(((a=TD_LINK_target_cluster_x) >= 1) &&
       ((b=TD_LINK_target_cluster_y) >= 1) &&
       ((c=TD_LINK_target_cluster_width) >= 1) &&
       ((d=TD_LINK_target_cluster_height) >= 1)) {
	target_cluster = TD_TARGET_CLUSTER;
    }

    if((! target_cluster) && a+b+c+d) {
	ui_confirmOk("Target receptive field not properly defined");
	return 0;
    }

    a=b=0;

    if(((a=TD_LINK_target_unit_x) >= 1) &&
       ((b=TD_LINK_target_unit_y) >= 1)) {
	target_unit = TD_TARGET_UNIT;
    }

    if((! target_unit) && a+b) {
	ui_confirmOk("Target unit not properly defined");
	return 0;
    }


    link_type =  target_plane+source_plane+
	target_cluster+source_cluster+
	    target_unit+source_unit;

    if(link_type != PLANE_TO_PLANE &&
       TD_LINK_source_cluster_height !=
       (*TD_PLANE_source_no).height - (*TD_PLANE_target_no).height + 1){
	sprintf(buf, "Source delay error: should be %d", 
		(*TD_PLANE_source_no).height-(*TD_PLANE_target_no).height+1);
	ui_confirmOk(buf);
	return 0;
    }

    if(link_type == PLANE_TO_PLANE){
	TD_LINK_source_cluster_height = (*TD_PLANE_source_no).height;
    }

    if((link_type == PLANE_TO_PLANE) ||
       (link_type == PLANE_TO_CLUSTER) ||
       (link_type == PLANE_TO_UNIT) ||
       (link_type == CLUSTER_TO_PLANE) ||
       (link_type == CLUSTER_TO_CLUSTER) ||
       (link_type == CLUSTER_TO_UNIT) ||
       (link_type == UNIT_TO_PLANE) ||
       (link_type == UNIT_TO_CLUSTER) ||
       (link_type == UNIT_TO_UNIT)) {

	TD_LINK_type_of_connection = link_type;

	if((TD_LINK_source_move_delta_x || TD_LINK_source_move_delta_y ||
	    TD_LINK_target_move_delta_x || TD_LINK_target_move_delta_y) &&
	   ((link_type == CLUSTER_TO_CLUSTER) ||
	    (link_type == CLUSTER_TO_UNIT)    ||
	    (link_type == UNIT_TO_CLUSTER)    ||
	    (link_type == UNIT_TO_UNIT))) {
	    TD_LINK_move = 1;
	}
	else {
	    TD_LINK_source_move_delta_x = 
		TD_LINK_source_move_delta_y = 
		    TD_LINK_target_move_delta_x = 
			TD_LINK_target_move_delta_y = 0;
	}
    }
    else
    {
	ui_confirmOk("Internal Error: Impossible connection type, "
		     "please report your current BIGNET setting");
	return 0;
    }

    /* sucessful */
    return 1;
}


/*****************************************************************************
  FUNCTION : enter_link
 
  PURPOSE  : enters a link element at the end of the link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void enter_link(void)

{
    TD_LINK *TD_LINK_element;

    TD_LINK_element = (TD_LINK *)malloc(sizeof(TD_LINK)); 
 
    (*TD_LINK_element).SOURCE.plane          = TD_LINK_source_plane;
    (*TD_LINK_element).SOURCE.CLUSTER.x      = TD_LINK_source_cluster_x;
    (*TD_LINK_element).SOURCE.CLUSTER.y      = TD_LINK_source_cluster_y;
    (*TD_LINK_element).SOURCE.CLUSTER.width  = TD_LINK_source_cluster_width;
    (*TD_LINK_element).SOURCE.CLUSTER.height = TD_LINK_source_cluster_height;
    (*TD_LINK_element).SOURCE.UNIT.x         = TD_LINK_source_unit_x;
    (*TD_LINK_element).SOURCE.UNIT.y         = TD_LINK_source_unit_y;
    (*TD_LINK_element).SOURCE.MOVE.delta_x   = TD_LINK_source_move_delta_x;
    (*TD_LINK_element).SOURCE.MOVE.delta_y   = TD_LINK_source_move_delta_y;

    (*TD_LINK_element).TARGET.plane          = TD_LINK_target_plane;
    (*TD_LINK_element).TARGET.CLUSTER.x      = TD_LINK_target_cluster_x;
    (*TD_LINK_element).TARGET.CLUSTER.y      = TD_LINK_target_cluster_y;
    (*TD_LINK_element).TARGET.CLUSTER.width  = TD_LINK_target_cluster_width;
    (*TD_LINK_element).TARGET.CLUSTER.height = TD_LINK_target_cluster_height;
    (*TD_LINK_element).TARGET.UNIT.x         = TD_LINK_target_unit_x;
    (*TD_LINK_element).TARGET.UNIT.y         = TD_LINK_target_unit_y;
    (*TD_LINK_element).TARGET.MOVE.delta_x   = TD_LINK_target_move_delta_x;
    (*TD_LINK_element).TARGET.MOVE.delta_y   = TD_LINK_target_move_delta_y;

    (*TD_LINK_element).move                  = TD_LINK_move;
    (*TD_LINK_element).type_of_connection    = TD_LINK_type_of_connection;

    (*TD_LINK_element).next   = NULL;
    (*TD_LINK_element).before = TD_LINK_last_element;
   
    if(TD_LINK_last_element != NULL) {
	(*TD_LINK_last_element).next = TD_LINK_element;
    }

    if(TD_LINK_first_element == NULL) {
	TD_LINK_first_element = TD_LINK_element;
    }

    TD_LINK_last_element = TD_LINK_element;
    TD_LINK_current_element = TD_LINK_last_element;
   
    TD_LINK_length++;

}




/*****************************************************************************
  FUNCTION : overwrite_link
 
  PURPOSE  : overwrites the current link element (Current Link) with the 
             datas of the link editor (Edit Link).

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void overwrite_link(void)

{
    if(TD_LINK_length == 0) {
	enter_link();
    }else{
	(*TD_LINK_current_element).SOURCE.plane = TD_LINK_source_plane;
	(*TD_LINK_current_element).SOURCE.CLUSTER.x = TD_LINK_source_cluster_x;
	(*TD_LINK_current_element).SOURCE.CLUSTER.y = TD_LINK_source_cluster_y;
	(*TD_LINK_current_element).SOURCE.CLUSTER.width = 
	    TD_LINK_source_cluster_width;
	(*TD_LINK_current_element).SOURCE.CLUSTER.height = 
	    TD_LINK_source_cluster_height;
	(*TD_LINK_current_element).SOURCE.UNIT.x = TD_LINK_source_unit_x;
	(*TD_LINK_current_element).SOURCE.UNIT.y = TD_LINK_source_unit_y;
	(*TD_LINK_current_element).SOURCE.MOVE.delta_x =
	    TD_LINK_source_move_delta_x;
	(*TD_LINK_current_element).SOURCE.MOVE.delta_y = 
	    TD_LINK_source_move_delta_y;
  
	(*TD_LINK_current_element).TARGET.plane = TD_LINK_target_plane;
	(*TD_LINK_current_element).TARGET.CLUSTER.x = TD_LINK_target_cluster_x;
	(*TD_LINK_current_element).TARGET.CLUSTER.y = TD_LINK_target_cluster_y;
	(*TD_LINK_current_element).TARGET.CLUSTER.width = 
	    TD_LINK_target_cluster_width;
	(*TD_LINK_current_element).TARGET.CLUSTER.height = 
	    TD_LINK_target_cluster_height;
	(*TD_LINK_current_element).TARGET.UNIT.x = TD_LINK_target_unit_x;
	(*TD_LINK_current_element).TARGET.UNIT.y = TD_LINK_target_unit_y;
	(*TD_LINK_current_element).TARGET.MOVE.delta_x = 
	    TD_LINK_target_move_delta_x;
	(*TD_LINK_current_element).TARGET.MOVE.delta_y = 
	    TD_LINK_target_move_delta_y;

	(*TD_LINK_current_element).move = TD_LINK_move;
	(*TD_LINK_current_element).type_of_connection = 
	    TD_LINK_type_of_connection;


    }		
}		



/*****************************************************************************
  FUNCTION : delete_current_link
 
  PURPOSE  : deletes  Currenrt Link out of the link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void delete_current_link(void)

{
    TD_LINK *TD_LINK_element;

    TD_LINK_element = TD_LINK_current_element;

    /* Delete the element "TD_LINK_element" out of the plane-list */

    if(TD_LINK_length != 0) {
	if((*TD_LINK_element).before != NULL) {
	    (*(*TD_LINK_element).before).next = (*TD_LINK_element).next;
	}
	if((*TD_LINK_element).next != NULL) {
	    (*(*TD_LINK_element).next).before = (*TD_LINK_element).before;
	}

	TD_LINK_length--;

	/* Update the three pointers 
	   TD_LINK_current_element,
	   TD_LINK_first_element,
	   TD_LINK_last_element.
	   */

	if(TD_LINK_length == 0){
	    TD_LINK_current_element = NULL;
	    TD_LINK_first_element   = NULL;
	    TD_LINK_last_element    = NULL;
	}else if((*TD_LINK_element).next == NULL) {
	    TD_LINK_current_element = (*TD_LINK_element).before;
	    TD_LINK_last_element    = TD_LINK_current_element;
	}else if((*TD_LINK_element).before == NULL) {
	    TD_LINK_current_element = (*TD_LINK_element).next;
	    TD_LINK_first_element   = TD_LINK_current_element;
	}else{
	    TD_LINK_current_element = (*TD_LINK_element).next;
	}  

	free(TD_LINK_element);
    }
}




/*****************************************************************************
  FUNCTION : get_links_and_delete_them(plane_no)
 
  PURPOSE  : deletes all the links which are connected with the plane 
             "plane_no"

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void get_links_and_delete_them(int plane_no)

{
    TD_LINK *TD_LINK_element = TD_LINK_first_element;

    while(TD_LINK_element != NULL) {
	if(((*TD_LINK_element).SOURCE.plane != plane_no) &&
	   ((*TD_LINK_element).TARGET.plane != plane_no)) {
	    TD_LINK_element = (*TD_LINK_element).next;
	}else {
	    TD_LINK_current_element = TD_LINK_element;
	    TD_LINK_element = (*TD_LINK_element).next;
	    delete_current_link();
	}
    }		
}




/*****************************************************************************
  FUNCTION : forward_link
 
  PURPOSE  : moves forward one list element in the link list.

  NOTES    : 
  RETURNS  :  Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int forward_link(void)

{
    if(TD_LINK_length != 0) {
	if((*TD_LINK_current_element).next != NULL) {
	    TD_LINK_current_element = (*TD_LINK_current_element).next;
	}
	return 1;
    }
    return 0;
}




/*****************************************************************************
  FUNCTION : backward_link
 
  PURPOSE  : moves backward one list element in the link list.
  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int backward_link(void)

{
    if(TD_LINK_length != 0) {
	if((*TD_LINK_current_element).before != NULL) {
	    TD_LINK_current_element = (*TD_LINK_current_element).before;
	}
	return 1;
    }
    return 0;
}



/*****************************************************************************
  FUNCTION : beginning_link
 
  PURPOSE  : jumps to the beginning of the link list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int beginning_link(void)

{
    if(TD_LINK_length != 0){
	TD_LINK_current_element = TD_LINK_first_element;
	return 1;
    }
    return 0;
}



/*****************************************************************************
  FUNCTION : end_link
 
  PURPOSE  : jumps to the end of the link list.
  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int end_link(void)

{
    if(TD_LINK_length != 0) {
	TD_LINK_current_element = TD_LINK_last_element;
	return 1;
    }
    return 0;
}



/*****************************************************************************
  FUNCTION : read_plane_elements

  PURPOSE  : reads the input datas of the plane editor (Edit Plane).

  NOTES    : 
  RETURNS  :  Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int read_plane_elements(void)

{
    TD_PLANE_width = ui_xIntFromAsciiWidget(z4s3);
    TD_PLANE_height = ui_xIntFromAsciiWidget(z5s3);
    if((TD_PLANE_width > 0) && (TD_PLANE_height > 0)) {
	return 1;
    }else{
	return 0;
    }
}
  

/*****************************************************************************
  FUNCTION : write_current_plane

  PURPOSE  : writes the datas of the current_element to the control part 
             (Current Plane) of the plane editor.

  NOTES    : 
  RETURNS  : 

  UPDATE   : 09.02.1993
******************************************************************************/

static void write_current_plane(void)

{
    char buf[10];

    sprintf(buf,"%d",TD_PLANE_no);
    ui_xSetLabel(z2s2,td_bn_widget_output(buf));
    ui_xSetLabel(z3s2,td_plane_type[(*TD_PLANE_current_element).type]);
    sprintf(buf,"%d",(*TD_PLANE_current_element).width);
    ui_xSetLabel(z4s2,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_PLANE_current_element).height);
    ui_xSetLabel(z5s2,td_bn_widget_output(buf));
    sprintf(buf,"%d",0);
    ui_xSetLabel(z6s2,td_bn_widget_output(buf));
    ui_xSetLabel(z7s2,td_plane_pos[(*TD_PLANE_current_element).pos]);
}



/*****************************************************************************
  FUNCTION : clear_current_plane

  PURPOSE  : Deletes the datas standing in Current Plane.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
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

  UPDATE   : 09.02.1993
******************************************************************************/

static void write_current_plane_to_editor(void)

{
    char buf[10];

    TD_PLANE_type = (*TD_PLANE_current_element).type;
    ui_xSetLabel(z3s3,td_plane_type[TD_PLANE_type]);
    sprintf(buf,"%d",(*TD_PLANE_current_element).width);
    ui_xSetString(z4s3,td_bn_widget_output(buf));
    sprintf(buf,"%d",(*TD_PLANE_current_element).height);
    ui_xSetString(z5s3,td_bn_widget_output(buf));
    sprintf(buf,"%d",0);
    ui_xSetString(z6s3,td_bn_widget_output(buf));
    TD_PLANE_pos = (*TD_PLANE_current_element).pos;
    ui_xSetLabel(z7s3,td_plane_pos[TD_PLANE_pos]);
}



/*****************************************************************************
  FUNCTION : clear_edit_plane

  PURPOSE  : Deletes the datas standing in Edit Link.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
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

  UPDATE   : 09.02.1993
******************************************************************************/

static void cancel_net_PROC(void)

{
    while(TD_PLANE_length){
	delete_current_plane();
    }
    clear_current_plane();
    clear_current_link();
}



/*****************************************************************************
  FUNCTION : enter_plane_PROC
 
  PURPOSE  : callback function, which enters a plane element at the end of the 
             plane list.

  NOTES    : the plane list contains all datas entered by the plane editor.
             Every plane element is a struct (see bn_types.c)
  RETURNS  :

  UPDATE   : 09.02.1993
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

  UPDATE   : 09.02.1993
******************************************************************************/

static void insert_plane_PROC(void)

{
    if(read_plane_elements()) {
	insert_plane();
	write_current_plane();
	clear_edit_plane();
	if(TD_LINK_length == 0) {
	    clear_current_link();
	}else{
	    write_current_link();
	}
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
	if(TD_LINK_length == 0) {
	    clear_current_link();
	}else{
	    write_current_link();
	}
    }
}



/*****************************************************************************
  FUNCTION : delete_plane_PROC
 
  PURPOSE  : callback function, which deletes the Currenrt Plane out of
             the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void delete_plane_PROC(void)

{
    delete_current_plane();
    if(TD_PLANE_length == 0) {
	clear_current_plane();
    }else{
	write_current_plane();
    }
    if(TD_LINK_length == 0) {
	clear_current_link();
    }else{
	write_current_link();
    }
}



/*****************************************************************************
  FUNCTION : forward_plane_PROC
 
  PURPOSE  : callback function, which moves forward one list element in the
             plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
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

  UPDATE   : 09.02.1993
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

  UPDATE   : 09.02.1993
******************************************************************************/

static void end_plane_PROC(void)

{
    if(end_plane()) {
	write_current_plane();
    }
}



/*****************************************************************************
  FUNCTION : beginning_plane_PROC
 
  PURPOSE  : callback function, which jumps to the beginning of the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
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

  UPDATE   : 09.02.1993
******************************************************************************/

static void pos_PROC(void)

{
    if(TD_PLANE_pos == TD_BN_RIGHT) {
	TD_PLANE_pos = TD_BN_BELOW;
    }else if(TD_PLANE_pos == TD_BN_BELOW) {
	TD_PLANE_pos = TD_BN_LEFT;
    }else{
	TD_PLANE_pos = TD_BN_RIGHT;
    }
    ui_xSetLabel(z7s3,td_plane_pos[TD_PLANE_pos]);
}



/*****************************************************************************
  FUNCNCTION : current_plane_to_editor_PROC
 
  PURPOSE  : callback function, which writes the Current Plane to Edit Plane

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void current_plane_to_editor_PROC(void)

{
    if(TD_PLANE_length != 0) {
	write_current_plane_to_editor();
    }else{
	clear_edit_plane();
    }
}



/*****************************************************************************
  FUNCNCTION : type_PROC
 
  PURPOSE  : callback function, which defines the unit type of a plane.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void type_PROC(void)

{
    switch(TD_PLANE_type){
      case TD_BN_INPUT : {TD_PLANE_type = TD_BN_HIDDEN; break;}
      case TD_BN_HIDDEN: {TD_PLANE_type = TD_BN_OUTPUT; break;}
      case TD_BN_OUTPUT: {TD_PLANE_type = TD_BN_INPUT ; break;}
    }
    ui_xSetLabel(z3s3,td_plane_type[TD_PLANE_type]);
}



/*****************************************************************************
  FUNCTION : enter_plane
 
  PURPOSE  : enters a plane element at the end of the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void enter_plane(void)

{
    TD_PLANE *TD_PLANE_element;

    TD_PLANE_element = (TD_PLANE *)malloc(sizeof(TD_PLANE));
    (*TD_PLANE_element).width  = TD_PLANE_width;    
    (*TD_PLANE_element).height = TD_PLANE_height;
    (*TD_PLANE_element).pos    = TD_PLANE_pos;
    (*TD_PLANE_element).type   = TD_PLANE_type;  
    (*TD_PLANE_element).next   = NULL;                            
    (*TD_PLANE_element).before = TD_PLANE_last_element;         

    if(TD_PLANE_last_element != NULL) {
	(*TD_PLANE_last_element).next = TD_PLANE_element;
    }

    if(TD_PLANE_first_element == NULL) {
	TD_PLANE_first_element = TD_PLANE_element;
    }

    TD_PLANE_last_element = TD_PLANE_element;
    TD_PLANE_current_element = TD_PLANE_last_element;

    TD_PLANE_no = ++TD_PLANE_length;

}	



/*****************************************************************************
  FUNCTION : insert_plane
 
  PURPOSE  : inserts the the input datas of the plane editor (Edit Plane) in 
             front of Current Plane.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void insert_plane(void)

{
    TD_PLANE *TD_PLANE_element;

    if(TD_PLANE_length == 0) {
	enter_plane();
    }else{
	TD_PLANE_element = (TD_PLANE *)malloc(sizeof(TD_PLANE));
   
	(*TD_PLANE_element).width  = TD_PLANE_width;                   
	(*TD_PLANE_element).height = TD_PLANE_height;
	(*TD_PLANE_element).pos    = TD_PLANE_pos;
	(*TD_PLANE_element).type   = TD_PLANE_type;                         
	(*TD_PLANE_element).next   = TD_PLANE_current_element;          
	(*TD_PLANE_element).before = (*TD_PLANE_current_element).before;

	if((*TD_PLANE_current_element).before != 0) {
	    (*(*TD_PLANE_current_element).before).next = TD_PLANE_element;
	}
	(*TD_PLANE_current_element).before = TD_PLANE_element;         

	TD_PLANE_current_element = TD_PLANE_element;
   
	if((*TD_PLANE_element).before == NULL) {
	    TD_PLANE_first_element = TD_PLANE_element;
	} 

	TD_PLANE_length++;

	TD_LINK_current_element = TD_LINK_first_element; 

	while(TD_LINK_current_element) {
	    if((*TD_LINK_current_element).SOURCE.plane >= TD_PLANE_no) {
		(*TD_LINK_current_element).SOURCE.plane++;
	    }
	    if((*TD_LINK_current_element).TARGET.plane >= TD_PLANE_no) {
		(*TD_LINK_current_element).TARGET.plane++;
	    }
	    TD_LINK_current_element = (*TD_LINK_current_element).next;
	}

	TD_LINK_current_element = TD_LINK_last_element;

    }

}



/*****************************************************************************
  FUNCTION : overwrite_plane
 
  PURPOSE  : overwrites the current plane element (Current Plane) with the 
             datas of the plane editor (Edit Plane).

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void overwrite_plane(void)

{
    if(TD_PLANE_length == 0) {
	enter_plane();
    }else{
	(*TD_PLANE_current_element).width  = TD_PLANE_width;
	(*TD_PLANE_current_element).height = TD_PLANE_height;
	(*TD_PLANE_current_element).pos    = TD_PLANE_pos;
	(*TD_PLANE_current_element).type   = TD_PLANE_type;

	get_links_and_delete_them(TD_PLANE_no);

	TD_LINK_current_element = TD_LINK_first_element;

    }

}



/*****************************************************************************
  FUNCTION : delete_current_plane
 
  PURPOSE  : deletes Currenrt Plane out of the plane list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 09.02.1993
******************************************************************************/

static void delete_current_plane(void)

{
    TD_PLANE *TD_PLANE_element;

    TD_PLANE_element = TD_PLANE_current_element;

    /* Delete the element "TD_PLANE_element" out of the plane-list */

    if(TD_PLANE_length != 0) {
	if((*TD_PLANE_element).before != NULL) {
	    (*(*TD_PLANE_element).before).next = (*TD_PLANE_element).next;
	}
	if((*TD_PLANE_element).next != NULL) {
	    (*(*TD_PLANE_element).next).before = (*TD_PLANE_element).before;
	}

	/* Delete all the links which are connected with the deleted plane */
   
	get_links_and_delete_them(TD_PLANE_no);

	TD_LINK_current_element = TD_LINK_first_element;

	/* Decrement the number of the Planes */

	while(TD_LINK_current_element) {
	    if((*TD_LINK_current_element).SOURCE.plane > TD_PLANE_no) {
		(*TD_LINK_current_element).SOURCE.plane--;
	    }
	    if((*TD_LINK_current_element).TARGET.plane > TD_PLANE_no) {
		(*TD_LINK_current_element).TARGET.plane--;
	    }
	    TD_LINK_current_element = (*TD_LINK_current_element).next;
	}

	TD_LINK_current_element = TD_LINK_last_element;

	TD_PLANE_length--;

	/* Update the three pointers "PLANE_current_element", 
	   "PLANE_first_element" "PLANE_last_element."  */

	if(TD_PLANE_length == 0){
	    TD_PLANE_current_element = NULL;
	    TD_PLANE_first_element   = NULL;
	    TD_PLANE_last_element    = NULL;
	    TD_PLANE_no--;
	}else if((*TD_PLANE_element).next == NULL) {
	    TD_PLANE_current_element = (*TD_PLANE_element).before;
	    TD_PLANE_last_element    = TD_PLANE_current_element;
	    TD_PLANE_no--;
	}else if((*TD_PLANE_element).before == NULL) {
	    TD_PLANE_current_element = (*TD_PLANE_element).next;
	    TD_PLANE_first_element   = TD_PLANE_current_element;
	}else{
	    TD_PLANE_current_element = (*TD_PLANE_element).next;
	}

	free(TD_PLANE_element);
    }

}



/*****************************************************************************
  FUNCTION : forward_plane
 
  PURPOSE  : moves forward one list element in the plane list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int forward_plane(void)

{
    if(TD_PLANE_length != 0) {
	if((*TD_PLANE_current_element).next != NULL) {
	    TD_PLANE_current_element = (*TD_PLANE_current_element).next;
	    TD_PLANE_no++;
	}
	return 1;
    }else{
	return 0;
    }

}



/*****************************************************************************
  FUNCTION : backward_link
 
  PURPOSE  : moves backward one plane element in the link list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int backward_plane(void)

{
    if(TD_PLANE_length != 0) {
	if((*TD_PLANE_current_element).before != NULL) {
	    TD_PLANE_current_element = (*TD_PLANE_current_element).before;
	    TD_PLANE_no--;
	}
	return 1;
    }else{
	return 0;
    }
}



/*****************************************************************************
  FUNCTION : beginning_plane
 
  PURPOSE  : jumps to the beginning of the plane list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int beginning_plane(void)

{
    if(TD_PLANE_length != 0){
	TD_PLANE_current_element = TD_PLANE_first_element;
	TD_PLANE_no = 1;
	return 1;
    }else{
	return 0;
    }

}



/*****************************************************************************
  FUNCTION : end_plane
 
  PURPOSE  : jumps to the end of the plane list.

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int end_plane(void)

{
    if(TD_PLANE_length != 0) {
	TD_PLANE_current_element = TD_PLANE_last_element;
	TD_PLANE_no = TD_PLANE_length;
	return 1;
    }else{
	return 0;
    }

}



/*****************************************************************************
  FUNCTION : get_plane
 
  PURPOSE  : returns a pointer to the plane element "plane_no"

  NOTES    : 
  RETURNS  : returns a pointer to the plane element "plane_no" 

  UPDATE   : 09.02.1993
******************************************************************************/

static TD_PLANE *get_plane(int plane_no)

{
    TD_PLANE *TD_PLANE_element = TD_PLANE_first_element;
    int counter = 1;

    while((TD_PLANE_element != NULL) && (counter != plane_no)) {
	counter++;
	TD_PLANE_element = (*TD_PLANE_element).next;
    }		
    return TD_PLANE_element;

}


/*****************************************************************************
 
******************************************************************************/

/* end of file */
/* lines: 2644 */




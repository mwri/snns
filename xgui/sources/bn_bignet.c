/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_bignet.c,v $
  SHORTNAME      : bn_bignet
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the BIGNET-window. 
  NOTES          :

  AUTHOR         : Michael Schmalzl  
  DATE           : 1.4.1990

  CHANGED BY     : Sven Doering, Kai-Uwe Herrmann
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/03/20 13:34:52 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Box.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "glob_typ.h"
#include "kr_ui.h"
#include "ui_textP.h"
#include "ui_mainP.h"
#include "ui_confirmer.h"
#include "ui_utilP.h"
#include "ui_xWidgets.h"
#include "ui_fileP.h"
#include "ui_display.h"
#include "bn_basics.h"

#include "bn_bignet.ph"



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

    z1s1 = ui_xCreateLabelItem ("           Plane           ", pannel, 
				colom1 * bn_fontWidth, NULL, NULL); 
    z1s2 = ui_xCreateLabelItem ("Current Plane  ", pannel, 
				colom2 * bn_fontWidth, z1s1, NULL); 
    z1s3 = ui_xCreateLabelItem ("Edit Plane  ", pannel, colom3 * bn_fontWidth, 
				z1s2, NULL); 

    z2s1   = ui_xCreateLabelItem ("Plane: ", pannel, colom1 * bn_fontWidth, 
				  NULL, z1s1); 
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth, z2s1, z1s1); 
    z2s2   = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z1s1);

    z3s1  = ui_xCreateLabelItem ("Type: ",pannel,colom1 * bn_fontWidth,
				 NULL,z2s1); 
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z3s1,z2s1); 
    z3s2  = ui_xCreateFrameLabelItem ("",pannel,bn_intWidth,dummy,z2s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z3s2,z2s1);
    z3s3  = ui_xCreateFrameLabelItem (plane_type[PLANE_type],pannel,
				      bn_intWidth,dummy,z2s1);

    z4s1  = ui_xCreateLabelItem ("No. of units in x-direction: ",pannel,
				 colom1 * bn_fontWidth,NULL,z3s1); 
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth,z4s1,z3s1); 
    z4s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z3s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 5 * bn_fontWidth,z4s2,z3s1); 
    z4s3  = ui_xCreateDialogItem ("z4s3", pannel,"",bn_intWidth,dummy,z3s1);
 
    z5s1  = ui_xCreateLabelItem ("No. of units in y-direction: ",pannel,
				 colom1 * bn_fontWidth,NULL,z4s1); 
    dummy = ui_xCreateLabelItem (" ", pannel, 3 * bn_fontWidth,z5s1,z4s1); 
    z5s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z4s1);
    dummy = ui_xCreateLabelItem (" ", pannel, 5 * bn_fontWidth,z5s2,z4s1); 
    z5s3  = ui_xCreateDialogItem ("z4s3", pannel, "",bn_intWidth,dummy,z4s1);
 

    z6s1  = ui_xCreateLabelItem ("z-coordinates of the plane: ",pannel,
				 colom1 * bn_fontWidth,NULL,z5s1); 
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z6s1,z5s1); 
    z6s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z5s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z6s2,z5s1); 
    z6s3  = ui_xCreateDialogItem ("z4s3",pannel,"",bn_intWidth,dummy,z5s1);
 
    z7s1  = ui_xCreateLabelItem ("Rel. Position: ",pannel,
				 colom1 * bn_fontWidth,NULL,z6s1); 
    dummy = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z7s1,z6s1); 
    z7s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z6s1);
    dummy = ui_xCreateLabelItem (" ",pannel,5 * bn_fontWidth,z7s2,z6s1);
    z7s3  = ui_xCreateFrameLabelItem (plane_pos[PLANE_pos],
				      pannel,bn_intWidth,dummy,z6s1);

    dummy = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,z7s1); 
    
    plane_edit = ui_xCreateLabelItem ("Edit Plane: ",pannel,
				      15 * bn_fontWidth,NULL,dummy); 
    plane_enter = ui_xCreateButtonItem ("enter",pannel,plane_edit,dummy);
    XtAddCallback(plane_enter,XtNcallback,(XtCallbackProc)enter_plane_PROC,
		  NULL);
    plane_insert = ui_xCreateButtonItem ("insert",pannel,plane_enter,
						dummy);
    XtAddCallback(plane_insert,XtNcallback,(XtCallbackProc)insert_plane_PROC,
		  NULL);
    plane_overwrite  = ui_xCreateButtonItem ("overwrite",pannel,
						    plane_insert,dummy);
    XtAddCallback(plane_overwrite,XtNcallback,
		  (XtCallbackProc)overwrite_plane_PROC,NULL);
    plane_delete = ui_xCreateButtonItem ("delete",pannel,
						plane_overwrite,dummy);
    XtAddCallback(plane_delete,XtNcallback,(XtCallbackProc)delete_plane_PROC,
		  NULL);

    dummy = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,plane_edit); 
    plane_to_edit = ui_xCreateButtonItem ("plane_to_edit",pannel,
						 dummy,plane_edit);
    XtAddCallback(plane_to_edit,XtNcallback,
		  (XtCallbackProc)current_plane_to_editor_PROC,NULL);
    type = ui_xCreateButtonItem ("type",pannel,plane_to_edit,plane_edit);
    XtAddCallback(type,XtNcallback,(XtCallbackProc)type_PROC,NULL);
    pos           = ui_xCreateButtonItem ("pos",pannel,type,plane_edit);
    XtAddCallback(pos,XtNcallback,(XtCallbackProc)pos_PROC,NULL);

    current_plane = ui_xCreateLabelItem ("Current plane: ",pannel,
					 15 * bn_fontWidth,NULL,dummy); 
    plane_first = ui_xCreateButtonItem ("first",pannel,current_plane,
					       dummy);
    XtAddCallback(plane_first,XtNcallback,(XtCallbackProc)beginning_plane_PROC,
		  NULL);
    plane_prev = ui_xCreateButtonItem ("prev",pannel,plane_first,dummy);
    XtAddCallback(plane_prev,XtNcallback,(XtCallbackProc)backward_plane_PROC,
		  NULL);
    plane_next = ui_xCreateButtonItem ("next",pannel,plane_prev,dummy);
    XtAddCallback(plane_next,XtNcallback,(XtCallbackProc)forward_plane_PROC,
		  NULL);
    plane_last = ui_xCreateButtonItem ("last",pannel,plane_next,dummy);
    XtAddCallback(plane_last,XtNcallback,(XtCallbackProc)end_plane_PROC,NULL);
}



/*****************************************************************************
  FUNCTION : bn_createLinkPanel

  PURPOSE  : creates the link panel, which is part of the bignet-window
  NOTES    :
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void bn_createLinkPannel (Widget parent)

{
    Widget pannel;
    Widget dummy,dummy1,dummy2;
    Widget link_edit,current_link;
    Widget link_enter,link_delete,link_overwrite,link_to_edit,
	   full_connection,shortcut_connection;
    Widget link_first,link_last,link_prev,link_next;
    Arg          arg[25];
    Cardinal     n;
  
    n = 0;  

    pannel = XtCreateManagedWidget ("pannel", formWidgetClass, parent, arg, n);



    dummy1 = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,NULL); 
    z8s2   = ui_xCreateLabelItem (" Current Link ", pannel,14*bn_fontWidth,
				  dummy1,NULL); 
    z8s3   = ui_xCreateLabelItem ("       Edit Link", pannel,16*bn_fontWidth,
				  z8s2,NULL); 

    dummy2 = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,dummy1); 
    z9s1   = ui_xCreateLabelItem ("Source", pannel,8*bn_fontWidth,dummy2,
				  dummy1); 
    z9s2   = ui_xCreateLabelItem ("Target", pannel,8*bn_fontWidth,z9s1,dummy1); 
    z9s3   = ui_xCreateLabelItem ("  Source", pannel,8*bn_fontWidth,z9s2,
				  dummy1); 
    z9s4   = ui_xCreateLabelItem (" Target", pannel,8*bn_fontWidth,z9s3,dummy1); 

    z10s1  = ui_xCreateLabelItem ("Plane", pannel,15*bn_fontWidth,NULL,dummy2); 
    z10s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z10s1,dummy2);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z10s2,dummy2); 
    z10s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,dummy2);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z10s3,dummy2); 
    z10s4  = ui_xCreateDialogItem ("z10s4", pannel,"",bn_intWidth,dummy,dummy2);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z10s4,dummy2); 
    z10s5  = ui_xCreateDialogItem("z10s5", pannel, "",bn_intWidth,dummy,dummy2);

    z11s1  = ui_xCreateLabelItem ("Cluster", pannel,15*bn_fontWidth,NULL,z10s1);

    z12s1  = ui_xCreateLabelItem ("  Coordinates", pannel,15*bn_fontWidth,
				  NULL,z11s1); 

    z13s1  = ui_xCreateLabelItem ("        x:", pannel,15*bn_fontWidth,
				  NULL,z12s1); 
    z13s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z13s1,z12s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z13s2,z12s1); 
    z13s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z12s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z13s3,z12s1); 
    z13s4  = ui_xCreateDialogItem ("z13s4", pannel, "",bn_intWidth,dummy,z12s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z13s4,z12s1); 
    z13s5  = ui_xCreateDialogItem ("z13s5", pannel, "",bn_intWidth,
				   dummy,z12s1);

    z14s1  = ui_xCreateLabelItem ("        y:", pannel,15*bn_fontWidth,
				  NULL,z13s1); 
    z14s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z14s1,z13s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z14s2,z13s1); 
    z14s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z13s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z14s3,z13s1); 
    z14s4  = ui_xCreateDialogItem ("z14s4", pannel, "",bn_intWidth,dummy,z13s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z14s4,z13s1); 
    z14s5  = ui_xCreateDialogItem ("z14s5", pannel, "",bn_intWidth,dummy,z13s1);

    z15s1  = ui_xCreateLabelItem ("   width :", pannel,15*bn_fontWidth,
				  NULL,z14s1); 
    z15s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z15s1,z14s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z15s2,z14s1); 
    z15s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z14s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z15s3,z14s1); 
    z15s4  = ui_xCreateDialogItem ("z15s4", pannel, "",bn_intWidth,dummy,z14s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z15s4,z14s1); 
    z15s5  = ui_xCreateDialogItem ("z15s5", pannel, "",bn_intWidth,dummy,z14s1);

    z16s1  = ui_xCreateLabelItem ("   height:", pannel,15*bn_fontWidth,
				  NULL,z15s1); 
    z16s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z16s1,z15s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z16s2,z15s1); 
    z16s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z15s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z16s3,z15s1); 
    z16s4  = ui_xCreateDialogItem ("z16s4", pannel, "",bn_intWidth,dummy,z15s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z16s4,z15s1); 
    z16s5  = ui_xCreateDialogItem ("z16s5", pannel, "",bn_intWidth,dummy,z15s1);

    z17s1  = ui_xCreateLabelItem ("Unit", pannel,15*bn_fontWidth,NULL,z16s1); 

    z18s1  = ui_xCreateLabelItem ("  Coordinates", pannel,15*bn_fontWidth,
				  NULL,z17s1); 

    z19s1  = ui_xCreateLabelItem ("        x:", pannel,15*bn_fontWidth,
				  NULL,z18s1); 
    z19s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z19s1,z18s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z19s2,z18s1); 
    z19s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z18s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z19s3,z18s1); 
    z19s4  = ui_xCreateDialogItem ("z19s4", pannel, "",bn_intWidth,dummy,z18s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z19s4,z18s1); 
    z19s5  = ui_xCreateDialogItem ("z19s5", pannel, "",bn_intWidth,dummy,z18s1);

    z20s1  = ui_xCreateLabelItem ("        y:", pannel,15*bn_fontWidth,
				  NULL,z19s1); 
    z20s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z20s1,z19s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z20s2,z19s1); 
    z20s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z19s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z20s3,z19s1); 
    z20s4  = ui_xCreateDialogItem ("z20s4", pannel, "",bn_intWidth,dummy,z19s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z20s4,z19s1); 
    z20s5  = ui_xCreateDialogItem ("z20s5", pannel, "",bn_intWidth,dummy,z19s1);

    z21s1  = ui_xCreateLabelItem ("Move", pannel,15*bn_fontWidth,NULL,z20s1); 


    z22s1  = ui_xCreateLabelItem ("       dx:", pannel,15*bn_fontWidth,
				  NULL,z21s1); 
    z22s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z22s1,z21s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z22s2,z21s1); 
    z22s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z21s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z22s3,z21s1); 
    z22s4  = ui_xCreateDialogItem ("z22s4", pannel, "",bn_intWidth,dummy,z21s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z22s4,z21s1); 
    z22s5  = ui_xCreateDialogItem ("z22s5", pannel, "",bn_intWidth,dummy,z21s1);

    z23s1  = ui_xCreateLabelItem ("       dy:", pannel,15*bn_fontWidth,
				  NULL,z22s1); 
    z23s2  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,z23s1,z22s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z23s2,z22s1); 
    z23s3  = ui_xCreateFrameLabelItem ("", pannel,bn_intWidth,dummy,z22s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,3 * bn_fontWidth,z23s3,z22s1); 
    z23s4  = ui_xCreateDialogItem ("z23s4", pannel, "",bn_intWidth,dummy,z22s1);
    dummy  = ui_xCreateLabelItem (" ",pannel,1 * bn_fontWidth,z23s4,z22s1); 
    z23s5  = ui_xCreateDialogItem ("z23s5", pannel, "",bn_intWidth,dummy,z22s1);


    dummy  = ui_xCreateLabelItem (" ",pannel,15 * bn_fontWidth,NULL,z23s1); 

    link_edit = ui_xCreateLabelItem ("Edit Link: ",pannel,15 * bn_fontWidth,
				     NULL,dummy); 
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

    full_connection = ui_xCreateButtonItem ("full_connection",pannel,
						   dummy,link_edit);
      XtAddCallback(full_connection,XtNcallback,
		    (XtCallbackProc)full_connection_PROC,NULL);
    shortcut_connection = ui_xCreateButtonItem ("shortcut_connection",
						       pannel,full_connection,
						       link_edit);
      XtAddCallback(shortcut_connection,XtNcallback,
		    (XtCallbackProc)shortcut_connection_PROC,NULL);

    current_link  = ui_xCreateLabelItem ("Current Link: ",pannel,
					 15 * bn_fontWidth,NULL,dummy); 
    link_first    = ui_xCreateButtonItem ("first",pannel,
						 current_link,dummy);
    XtAddCallback(link_first,XtNcallback,
		  (XtCallbackProc)beginning_link_PROC,NULL);
    link_prev = ui_xCreateButtonItem ("prev",pannel,link_first,dummy);
    XtAddCallback(link_prev,XtNcallback,
		  (XtCallbackProc)backward_link_PROC,NULL);
    link_next = ui_xCreateButtonItem ("next",pannel,link_prev,dummy);
    XtAddCallback(link_next,XtNcallback,(XtCallbackProc)forward_link_PROC,NULL);
    link_last = ui_xCreateButtonItem ("last",pannel,link_next,dummy);
    XtAddCallback(link_last,XtNcallback,(XtCallbackProc)end_link_PROC,NULL);

  }


/*****************************************************************************
  FUNCTION : exit_PROC

  PURPOSE  : callback function of the exit-buttom. You are leaving bignet.
  NOTES    :
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void exit_PROC (void)
{
    XtDestroyWidget (baseWidget);
    bignet_widget_open = 0;
}

/*****************************************************************************
  FUNCTION : bn_createBignet

  PURPOSE  : creates the bignet-window.
  NOTES    :
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
void bn_createBignet (void)
{
    Widget       box;
    Widget       create_net,exit,cancel;
    Arg          arg[25];
    Cardinal     n;
    char         buf[40];

    if(bignet_widget_open) {
	XRaiseWindow (XtDisplay (baseWidget), XtWindow (baseWidget));
	return;
    }

    sprintf (buf, "SNNS BigNet (General Type)");
    n = 0;  

    baseWidget = XtCreatePopupShell (buf, topLevelShellWidgetClass, 
				     ui_toplevel, arg, n); 

    box = XtCreateManagedWidget ("box", boxWidgetClass, baseWidget, arg, n);
    bn_createPlanePannel (box); 
    bn_createLinkPannel(box);

    create_net = ui_xCreateButtonItem ("create_net",box,NULL,NULL);
    XtAddCallback(create_net,XtNcallback,(XtCallbackProc)create_net_PROC,NULL);

    exit = ui_xCreateButtonItem ("done",box,create_net,NULL);
    XtAddCallback(exit,XtNcallback,(XtCallbackProc)exit_PROC,NULL);

    cancel = ui_xCreateButtonItem ("cancel",box,exit,NULL);
    XtAddCallback(cancel,XtNcallback,(XtCallbackProc)cancel_net_PROC,NULL);

    ui_checkWindowPosition(baseWidget);
    XtPopup (baseWidget, XtGrabNone);
    ui_xDontResizeWidget(baseWidget);
    bignet_widget_open = 1;
    if(PLANE_length != 0){
	write_current_plane();
	if(LINK_length != 0) {
	    write_current_link();
	}
    }
}


/*****************************************************************************
  FUNCTION : create_net_PROC

  PURPOSE  : creates a SNNS-net in two steps:
             1. calls up a function, which creates SNNS-units
             2. calls up a function, which connections the SNNS-units.
  NOTES    : There are nine different ways (PLANE_TO_PLANE, PLANE_TO_CLUSTER ..)
             two different planes can be connected.
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void create_net_PROC(void)

{
    LINK  *LINK_element=LINK_first_element;
    PLANE *source_plane,*target_plane;
    int move,type_of_connection=1;
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

	while((LINK_element != NULL) && (! BN_ERROR)) {

	    move               = (*LINK_element).move;
	    type_of_connection = (*LINK_element).type_of_connection;
	    source_plane       =  get_plane((*LINK_element).SOURCE.plane);
	    target_plane       =  get_plane((*LINK_element).TARGET.plane);
   
	    switch(type_of_connection) {
	    case PLANE_TO_PLANE: {
		make_link(source_plane,0,0,
			  (*source_plane).width,(*source_plane).height,
			  target_plane,0,0,
			  (*target_plane).width,(*target_plane).height);
		break;
	    }
	    case PLANE_TO_CLUSTER: {
		make_link(source_plane,0,0,
			  (*source_plane).width,(*source_plane).height,
			  target_plane,(*LINK_element).TARGET.CLUSTER.x-1,
			  (*LINK_element).TARGET.CLUSTER.y-1,
			  (*LINK_element).TARGET.CLUSTER.width,
			  (*LINK_element).TARGET.CLUSTER.height);
		break;
	    }
	    case PLANE_TO_UNIT: {
		make_link(source_plane,0,0,
			  (*source_plane).width,(*source_plane).height,
			  target_plane,(*LINK_element).TARGET.UNIT.x-1,
			  (*LINK_element).TARGET.UNIT.y-1,1,1);
		break;
	    }  
	    case CLUSTER_TO_PLANE: {
		make_link(source_plane,(*LINK_element).SOURCE.CLUSTER.x-1,
			  (*LINK_element).SOURCE.CLUSTER.y-1,
			  (*LINK_element).SOURCE.CLUSTER.width,
			  (*LINK_element).SOURCE.CLUSTER.height, target_plane,
			  0, 0,(*target_plane).width,(*target_plane).height);

		break;
	    }
	    case CLUSTER_TO_CLUSTER: {
		if(move) {
		    make_move_link(source_plane,
				   (*LINK_element).SOURCE.CLUSTER.x-1,
				   (*LINK_element).SOURCE.CLUSTER.y-1,
				   (*LINK_element).SOURCE.CLUSTER.width,
				   (*LINK_element).SOURCE.CLUSTER.height,
				   target_plane,
				   (*LINK_element).TARGET.CLUSTER.x-1,
				   (*LINK_element).TARGET.CLUSTER.y-1,
				   (*LINK_element).TARGET.CLUSTER.width,
				   (*LINK_element).TARGET.CLUSTER.height,
				   (*LINK_element).SOURCE.MOVE.delta_x,
				   (*LINK_element).SOURCE.MOVE.delta_y,
				   (*LINK_element).TARGET.MOVE.delta_x,
				   (*LINK_element).TARGET.MOVE.delta_y);
		}else{
		    make_link(source_plane,
			      (*LINK_element).SOURCE.CLUSTER.x-1,
			      (*LINK_element).SOURCE.CLUSTER.y-1,
			      (*LINK_element).SOURCE.CLUSTER.width,
			      (*LINK_element).SOURCE.CLUSTER.height,
			      target_plane,(*LINK_element).TARGET.CLUSTER.x-1,
			      (*LINK_element).TARGET.CLUSTER.y-1,
			      (*LINK_element).TARGET.CLUSTER.width,
			      (*LINK_element).TARGET.CLUSTER.height);
		}
		break;
	    }
	    case CLUSTER_TO_UNIT: {
		if(move) {
		    make_move_link(source_plane,
				   (*LINK_element).SOURCE.CLUSTER.x-1,
				   (*LINK_element).SOURCE.CLUSTER.y-1,
				   (*LINK_element).SOURCE.CLUSTER.width,
				   (*LINK_element).SOURCE.CLUSTER.height,
				   target_plane,
				   (*LINK_element).TARGET.UNIT.x-1,
				   (*LINK_element).TARGET.UNIT.y-1, 1, 1,
				   (*LINK_element).SOURCE.MOVE.delta_x,
				   (*LINK_element).SOURCE.MOVE.delta_y,
				   (*LINK_element).TARGET.MOVE.delta_x,
				   (*LINK_element).TARGET.MOVE.delta_y);
		}else {
		    make_link(source_plane,
			      (*LINK_element).SOURCE.CLUSTER.x-1,
			      (*LINK_element).SOURCE.CLUSTER.y-1,
			      (*LINK_element).SOURCE.CLUSTER.width,
			      (*LINK_element).SOURCE.CLUSTER.height,
			      target_plane,(*LINK_element).TARGET.UNIT.x-1,
			      (*LINK_element).TARGET.UNIT.y-1, 1, 1);
		}
		break;
	    }
	    case UNIT_TO_PLANE: {
		make_link(source_plane,(*LINK_element).SOURCE.UNIT.x-1,
			  (*LINK_element).SOURCE.UNIT.y-1, 1, 1, target_plane,
			  0, 0, (*target_plane).width, (*target_plane).height);
		break;
	    }
	    case UNIT_TO_CLUSTER: {
		if(move) {
		    make_move_link(source_plane,
				   (*LINK_element).SOURCE.UNIT.x-1,
				   (*LINK_element).SOURCE.UNIT.y-1, 1, 1,
				   target_plane,
				   (*LINK_element).TARGET.CLUSTER.x-1,
				   (*LINK_element).TARGET.CLUSTER.y-1,
				   (*LINK_element).TARGET.CLUSTER.width,
				   (*LINK_element).TARGET.CLUSTER.height,
				   (*LINK_element).SOURCE.MOVE.delta_x,
				   (*LINK_element).SOURCE.MOVE.delta_y,
				   (*LINK_element).TARGET.MOVE.delta_x,
				   (*LINK_element).TARGET.MOVE.delta_y);
		}else{
		    make_link(source_plane,(*LINK_element).SOURCE.UNIT.x-1,
			      (*LINK_element).SOURCE.UNIT.y-1, 1, 1,
			      target_plane,(*LINK_element).TARGET.CLUSTER.x-1,
			      (*LINK_element).TARGET.CLUSTER.y-1,
			      (*LINK_element).TARGET.CLUSTER.width,
			      (*LINK_element).TARGET.CLUSTER.height);
		}
		break;
	    }
	    case UNIT_TO_UNIT: {
		if(move) {
		    make_move_link(source_plane,
				   (*LINK_element).SOURCE.UNIT.x-1,
				   (*LINK_element).SOURCE.UNIT.y-1, 1, 1,
				   target_plane,
				   (*LINK_element).TARGET.UNIT.x-1,
				   (*LINK_element).TARGET.UNIT.y-1, 1, 1,
				   (*LINK_element).SOURCE.MOVE.delta_x,
				   (*LINK_element).SOURCE.MOVE.delta_y,
				   (*LINK_element).TARGET.MOVE.delta_x,
				   (*LINK_element).TARGET.MOVE.delta_y);
		}else{
		    make_link(source_plane,(*LINK_element).SOURCE.UNIT.x-1,
			      (*LINK_element).SOURCE.UNIT.y-1, 1, 1,
			      target_plane,(*LINK_element).TARGET.UNIT.x-1,
			      (*LINK_element).TARGET.UNIT.y-1, 1, 1);
		}
		break;
	    }
	    } /* switch */
	    LINK_element = (*LINK_element).next;
	} /* while */    
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
  FUNCTION : calculate_x_begin

  PURPOSE  : calculates the x-coordinate of a plane, where the plane is
             positioned in the display window.
             plane.
  NOTES    : 
  RETURNS  : The X-Position

  UPDATE   : 20.12.1991
******************************************************************************/
int calculate_x_begin(int *new_x_begin,int *old_x_begin,int *x_max,
			     int width,int pos)
{

    if(pos == 0 /* RIGHT */) {
   
	*new_x_begin = *x_max + 2; 
	*x_max = *new_x_begin + width;
	*old_x_begin = *new_x_begin;
	return *new_x_begin;
    }else if(pos == 1 /* BELOW */) {
	if(*x_max < (*old_x_begin + width)) {
	    *x_max = *old_x_begin + width;
	}
	return *old_x_begin; 
    }else if(pos == 2 /* LEFT */) {
	*x_max = 0;
	*old_x_begin = 2;

	*new_x_begin = *x_max + 2;
	*x_max = *new_x_begin + width;
	return *new_x_begin;
    }
    return 0;
}


/*****************************************************************************
  FUNCTION : calculate_y_begin

  PURPOSE  : calculates the y-coordinate of a plane, where the plane is
             positioned in the display window.
             plane.

  NOTES    : 
  RETURNS  : The Y-Position

  UPDATE   : 20.12.1991
******************************************************************************/
int calculate_y_begin(int *absolute_y_max,int *relative_y_max,
			     int *y_offset,int height,int pos)
{

    int y_begin;

    if(pos == 0 /* RIGHT */) {
	y_begin = *y_offset + 2;         
	*relative_y_max = y_begin + height;
	if(*absolute_y_max < *relative_y_max) {
	    *absolute_y_max = *relative_y_max;
	}
    }else if(pos == 1 /* BELOW */) {
	y_begin = *relative_y_max + 2;   
	*relative_y_max = y_begin + height;
	if(*absolute_y_max < *relative_y_max) {
	    *absolute_y_max = *relative_y_max;
	}
    }else if(pos == 2 /* LEFT */) {
	*y_offset = *absolute_y_max;
	y_begin = *y_offset + 2;
	*absolute_y_max = y_begin + height;
	*relative_y_max = y_begin + height;
    }
    return y_begin;
}


/*****************************************************************************
  FUNCTION : create_snns_unit

  PURPOSE  : creates for every unit of a plane a SNNS default unit.

  NOTES    : 
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
    
	y_begin = calculate_y_begin(&absolute_y_max,&relative_y_max,
				    &y_offset,height,pos);
	x_begin = calculate_x_begin(&new_x_begin,&old_x_begin,&x_max,width,pos);

	for(y=0;y<height;y++){
	    for(x=0;x<width;x++){
		unit_no = krui_createDefaultUnit();
		if(unit_no<0) ui_checkError(unit_no);
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
  FUNCTION : make_link

  PURPOSE  : creates the links between two planes.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void make_link(PLANE *LINK_source_plane, int LINK_source_cluster_x,
		      int LINK_source_cluster_y, int LINK_source_cluster_width,
		      int LINK_source_cluster_height, PLANE *LINK_target_plane,
		      int LINK_target_cluster_x, int LINK_target_cluster_y,
		      int LINK_target_cluster_width,
		      int LINK_target_cluster_height)
{

    int i,j,k,l,ret,target_unit,source_unit;

    for(i = LINK_target_cluster_y;
	i < LINK_target_cluster_height+LINK_target_cluster_y;i++)

	for(j = LINK_target_cluster_x;
	    j < LINK_target_cluster_width+LINK_target_cluster_x;j++){

	    target_unit = (*LINK_target_plane).begin + 
		          (*LINK_target_plane).width * i + j;
	    ret = krui_setCurrentUnit(target_unit);
	    if(ret != 0){
		ui_tw_errorMessage(krui_error(ret));
		BN_ERROR = 1;
		krui_deleteNet();
		return;
	    }
	    for(k = LINK_source_cluster_y;
		k < LINK_source_cluster_height+LINK_source_cluster_y;k++)

		for(l = LINK_source_cluster_x;
		    l < LINK_source_cluster_width+LINK_source_cluster_x;l++){

		    source_unit = (*LINK_source_plane).begin + 
			(*LINK_source_plane).width * k + l;
		    if((ret = krui_createLink(source_unit,0.0)) != 0){
			ui_tw_errorMessage(krui_error(ret));
			BN_ERROR = 1;
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

  UPDATE   : 20.12.1991
******************************************************************************/
static void make_move_link(PLANE *LINK_source_plane, int LINK_source_cluster_x,
			   int LINK_source_cluster_y,
			   int LINK_source_cluster_width,
			   int LINK_source_cluster_height,
                           PLANE * LINK_target_plane, int LINK_target_cluster_x,
			   int LINK_target_cluster_y,
			   int LINK_target_cluster_width,
			   int LINK_target_cluster_height,
                           int LINK_source_move_delta_x,
			   int LINK_source_move_delta_y,
                           int LINK_target_move_delta_x,
			   int LINK_target_move_delta_y)
{
    int xs_pivot = LINK_source_cluster_x;
    int xt_pivot = LINK_target_cluster_x;

    do {
	make_link(LINK_source_plane,LINK_source_cluster_x,LINK_source_cluster_y,
		  LINK_source_cluster_width,LINK_source_cluster_height,
		  LINK_target_plane,LINK_target_cluster_x,LINK_target_cluster_y,
		  LINK_target_cluster_width,LINK_target_cluster_height);
    }while(! BN_ERROR && 
	   move_p(LINK_source_plane, &LINK_source_cluster_y,
		  &LINK_source_cluster_x, LINK_source_cluster_width,
		  LINK_source_cluster_height, xs_pivot,
		  LINK_source_move_delta_x, LINK_source_move_delta_y) &&
           move_p(LINK_target_plane, &LINK_target_cluster_y,
		  &LINK_target_cluster_x, LINK_target_cluster_width,
		  LINK_target_cluster_height, xt_pivot,
		  LINK_target_move_delta_x,LINK_target_move_delta_y));
}
    

/*****************************************************************************
  FUNCTION : move_p

  PURPOSE  : tests whether the boundaries of a plane are crossed while
             "moving" (see docu)

  NOTES    : 
  RETURNS  : 

  UPDATE   : 20.12.1991
******************************************************************************/
static int move_p(PLANE *plane_no,int *y,int *x,int width,
		  int height,int x_pivot,int x_step,int y_step)

{
    if(x_step > 0 && ((*x)+width+x_step<=(*plane_no).width))
	(*x)+= x_step;
    else if(y_step > 0 && ((*y)+height+y_step<=(*plane_no).height)){
	(*y) += y_step;
	(*x) = x_pivot;
    }else
	return 0;

    return 1;
}


/*****************************************************************************
  FUNCTION : read_link_elements

  PURPOSE  : reads the input datas of the link editor (Edit Link).

  NOTES    : 
  RETURNS  : 

  UPDATE   : 20.12.1991
******************************************************************************/
static int read_link_elements(void)

{
    LINK_source_plane = ui_xIntFromAsciiWidget(z10s4);
    LINK_source_cluster_x = ui_xIntFromAsciiWidget(z13s4);
    LINK_source_cluster_y = ui_xIntFromAsciiWidget(z14s4);
    LINK_source_cluster_width = ui_xIntFromAsciiWidget(z15s4);
    LINK_source_cluster_height = ui_xIntFromAsciiWidget(z16s4);
    LINK_source_unit_x = ui_xIntFromAsciiWidget(z19s4);
    LINK_source_unit_y = ui_xIntFromAsciiWidget(z20s4);
    LINK_source_move_delta_x = ui_xIntFromAsciiWidget(z22s4);
    LINK_source_move_delta_y = ui_xIntFromAsciiWidget(z23s4);

    LINK_target_plane = ui_xIntFromAsciiWidget(z10s5);
    LINK_target_cluster_x = ui_xIntFromAsciiWidget(z13s5);
    LINK_target_cluster_y = ui_xIntFromAsciiWidget(z14s5);
    LINK_target_cluster_width = ui_xIntFromAsciiWidget(z15s5);
    LINK_target_cluster_height = ui_xIntFromAsciiWidget(z16s5);
    LINK_target_unit_x = ui_xIntFromAsciiWidget(z19s5);
    LINK_target_unit_y = ui_xIntFromAsciiWidget(z20s5);
    LINK_target_move_delta_x = ui_xIntFromAsciiWidget(z22s5);
    LINK_target_move_delta_y = ui_xIntFromAsciiWidget(z23s5);

    return correct_link_input();
}


/*****************************************************************************
  FUNCTION : write_current_link

  PURPOSE  : writes the datas of the current_element to the control part 
             (Current Link) of the link editor.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void write_current_link(void)
{
    char buf[10];
  
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.plane);
    ui_xSetLabel(z10s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.CLUSTER.x);
    ui_xSetLabel(z13s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.CLUSTER.y);
    ui_xSetLabel(z14s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.CLUSTER.width);
    ui_xSetLabel(z15s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.CLUSTER.height);
    ui_xSetLabel(z16s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.UNIT.x);
    ui_xSetLabel(z19s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.UNIT.y);
    ui_xSetLabel(z20s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.MOVE.delta_x);
    ui_xSetLabel(z22s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.MOVE.delta_y);
    ui_xSetLabel(z23s2,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.plane);
    ui_xSetLabel(z10s3,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.CLUSTER.x);
    ui_xSetLabel(z13s3,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.CLUSTER.y);
    ui_xSetLabel(z14s3,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.CLUSTER.width);
    ui_xSetLabel(z15s3,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.CLUSTER.height);
    ui_xSetLabel(z16s3,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.UNIT.x);
    ui_xSetLabel(z19s3,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.UNIT.y);
    ui_xSetLabel(z20s3,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.MOVE.delta_x);
    ui_xSetLabel(z22s3,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.MOVE.delta_y);
    ui_xSetLabel(z23s3,bn_widget_output(buf));
}


/*****************************************************************************
  FUNCTION : clear_current_link

  PURPOSE  : Deletes the datas standing in Current Link.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void clear_current_link(void)
{

    ui_xSetLabel(z10s2,"");
    ui_xSetLabel(z13s2,"");
    ui_xSetLabel(z14s2,"");
    ui_xSetLabel(z15s2,"");
    ui_xSetLabel(z16s2,"");
    ui_xSetLabel(z19s2,"");
    ui_xSetLabel(z20s2,"");
    ui_xSetLabel(z22s2,"");
    ui_xSetLabel(z23s2,"");

    ui_xSetLabel(z10s3,"");
    ui_xSetLabel(z13s3,"");
    ui_xSetLabel(z14s3,"");
    ui_xSetLabel(z15s3,"");
    ui_xSetLabel(z16s3,"");
    ui_xSetLabel(z19s3,"");
    ui_xSetLabel(z20s3,"");
    ui_xSetLabel(z22s3,"");
    ui_xSetLabel(z23s3,"");
}

/*****************************************************************************
  FUNCTION : write_current_link_to_editor

  PURPOSE  : writes the datas of Current Link to Edit Link

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void write_current_link_to_editor(void)
{

    char buf[10];

    sprintf(buf,"%d",(*LINK_current_element).SOURCE.plane);
    ui_xSetString(z10s4,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.CLUSTER.x);
    ui_xSetString(z13s4,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.CLUSTER.y);
    ui_xSetString(z14s4,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.CLUSTER.width);
    ui_xSetString(z15s4,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.CLUSTER.height);
    ui_xSetString(z16s4,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.UNIT.x);
    ui_xSetString(z19s4,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.UNIT.y);
    ui_xSetString(z20s4,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.MOVE.delta_x);
    ui_xSetString(z22s4,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).SOURCE.MOVE.delta_y);
    ui_xSetString(z23s4,bn_widget_output(buf));

    sprintf(buf,"%d",(*LINK_current_element).TARGET.plane);
    ui_xSetString(z10s5,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).TARGET.CLUSTER.x);
    ui_xSetString(z13s5,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).TARGET.CLUSTER.y);
    ui_xSetString(z14s5,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).TARGET.CLUSTER.width);
    ui_xSetString(z15s5,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).TARGET.CLUSTER.height);
    ui_xSetString(z16s5,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).TARGET.UNIT.x);
    ui_xSetString(z19s5,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).TARGET.UNIT.y);
    ui_xSetString(z20s5,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).TARGET.MOVE.delta_x);
    ui_xSetString(z22s5,bn_widget_output(buf));
    sprintf(buf,"%d",(*LINK_current_element).TARGET.MOVE.delta_y);
    ui_xSetString(z23s5,bn_widget_output(buf));

}


/*****************************************************************************
  FUNCTION : clear_edit_link

  PURPOSE  : Deletes the datas standing in Edit Link.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void clear_edit_link(void)
{
    ui_xSetString(z10s4,"");
    ui_xSetString(z13s4,"");
    ui_xSetString(z14s4,"");
    ui_xSetString(z15s4,"");
    ui_xSetString(z16s4,"");
    ui_xSetString(z19s4,"");
    ui_xSetString(z20s4,"");
    ui_xSetString(z22s4,"");
    ui_xSetString(z23s4,"");

    ui_xSetString(z10s5,"");
    ui_xSetString(z13s5,"");
    ui_xSetString(z14s5,"");
    ui_xSetString(z15s5,"");
    ui_xSetString(z16s5,"");
    ui_xSetString(z19s5,"");
    ui_xSetString(z20s5,"");
    ui_xSetString(z22s5,"");
    ui_xSetString(z23s5,"");
}


/*****************************************************************************
  FUNCTION : enter_link_PROC
 
  PURPOSE  : callback function, which enters a link element at the end of the 
             link list.

  NOTES    : the link list contains all datas entered by the link editor.
             Every link element is a struct (see bn_types.c)
  RETURNS  :

  UPDATE   : 20.12.1991
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
  FUNCTION : full_connection_PROC
 
  PURPOSE  : callback function, which creates links between planes (see docu)

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void full_connection_PROC(void)
{
    while(LINK_length) {
	delete_current_link();
    }
    if(full_connection()) {
	write_current_link();
    }
}

/*****************************************************************************
  FUNCTION : shortcut_connection_PROC
 
  PURPOSE  : callback function, which creates links between planes (see docu)

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void shortcut_connection_PROC(void)
{
    while(LINK_length) {
	delete_current_link();
    }
    if(shortcut_connection()) {
	write_current_link();
    }
}

/*****************************************************************************
  FUNCTION : overwrite_link_PROC
 
  PURPOSE  : callback function, which overwrites the current link element
             (Current Link) with the datas of the link editor (Edit Link).

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
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

  UPDATE   : 20.12.1991
******************************************************************************/
static void delete_link_PROC(void)
{
    delete_current_link();
    if(LINK_length == 0) {
	clear_current_link();
    } else {
	write_current_link();
    }
}

/*****************************************************************************
  FUNCTION : forward_link_PROC
 
  PURPOSE  : callback function, which moves forward one list element in the
             link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
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

  UPDATE   : 20.12.1991
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

  UPDATE   : 20.12.1991
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

  UPDATE   : 20.12.1991
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

  UPDATE   : 20.12.1991
******************************************************************************/
static void current_link_to_editor_PROC(void)
{
    if(LINK_length != 0) {
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

  UPDATE   : 20.03.1998
******************************************************************************/
static int correct_link_input(void)
{
    PLANE *PLANE_source_no, *PLANE_target_no;

    int link_type;
    int target_plane=0, source_plane=0;
    int target_cluster=0, source_cluster=0;
    int target_unit=0, source_unit=0;
    int a,b,c,d;

    PLANE_source_no = get_plane(LINK_source_plane);
    PLANE_target_no = get_plane(LINK_target_plane);

    LINK_move = 0;


    /* check plane numbers */
/*    if((PLANE_source_no == PLANE_target_no) ||*/
    if((PLANE_source_no == NULL) || (PLANE_target_no == NULL))
	return 0;

    /* check source plane dimensions */
    if((LINK_source_cluster_x + LINK_source_cluster_width  
	> (*PLANE_source_no).width + 1) ||
       (LINK_source_cluster_y + LINK_source_cluster_height 
	> (*PLANE_source_no).height + 1) ||
       (LINK_source_unit_x > (*PLANE_source_no).width) ||
       (LINK_source_unit_y > (*PLANE_source_no).height))
	return 0;

    /* check target plane dimensions */
    if((LINK_target_cluster_x + LINK_target_cluster_width  
	> (*PLANE_target_no).width + 1) ||
       (LINK_target_cluster_y + LINK_target_cluster_height 
	> (*PLANE_target_no).height + 1) ||
       (LINK_target_unit_x > (*PLANE_target_no).width) ||
       (LINK_target_unit_y > (*PLANE_target_no).height)) 
	return 0;
   
    target_plane = TARGET_PLANE;
    source_plane = SOURCE_PLANE;

    a = b = c = d = 0;

    if(((a=LINK_source_cluster_x) >= 1) && ((b=LINK_source_cluster_y) >= 1) &&
       ((c=LINK_source_cluster_width) >= 1) &&
       ((d=LINK_source_cluster_height) >= 1)) {
	source_cluster = SOURCE_CLUSTER;
    }

    if((! source_cluster) && a+b+c+d) 
	return 0;

    a = b = 0;

    if(((a=LINK_source_unit_x) >= 1) && ((b=LINK_source_unit_y) >= 1)) 
	source_unit = SOURCE_UNIT;

    if((! source_unit) && a+b) 
	return 0;

    a = b = c = d = 0;

    if(((a=LINK_target_cluster_x) >= 1) && ((b=LINK_target_cluster_y) >= 1) &&
       ((c=LINK_target_cluster_width) >= 1) &&
       ((d=LINK_target_cluster_height) >= 1)) {
	target_cluster = TARGET_CLUSTER;
    }

    if((! target_cluster) && a+b+c+d) 
	return 0;

    a = b = 0;

    if(((a=LINK_target_unit_x) >= 1) && ((b=LINK_target_unit_y) >= 1))
	target_unit = TARGET_UNIT;

    if((! target_unit) && a+b) 
	return 0;

    link_type = target_plane + source_plane + target_cluster+
	source_cluster + target_unit + source_unit;

    if((link_type == PLANE_TO_PLANE) || (link_type == PLANE_TO_CLUSTER) ||
       (link_type == PLANE_TO_UNIT) ||  (link_type == CLUSTER_TO_PLANE) ||
       (link_type == CLUSTER_TO_CLUSTER) || (link_type == CLUSTER_TO_UNIT) ||
       (link_type == UNIT_TO_PLANE) ||(link_type == UNIT_TO_CLUSTER) ||
       (link_type == UNIT_TO_UNIT)) {

	LINK_type_of_connection = link_type;

	if((LINK_source_move_delta_x || LINK_source_move_delta_y ||
	    LINK_target_move_delta_x || LINK_target_move_delta_y) &&
	   ((link_type==CLUSTER_TO_CLUSTER) || (link_type==CLUSTER_TO_UNIT) ||
	    (link_type==UNIT_TO_CLUSTER)    || (link_type == UNIT_TO_UNIT))) {
	    LINK_move = 1;
	} else {
	    LINK_source_move_delta_x = 
		LINK_source_move_delta_y = 
		LINK_target_move_delta_x = 
		LINK_target_move_delta_y = 0;
	}
    }

    return 1;
}


/*****************************************************************************
  FUNCTION : enter_link
 
  PURPOSE  : enters a link element at the end of the link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void enter_link(void)
{
    LINK *LINK_element;

    LINK_element = (LINK *)malloc(sizeof(LINK)); 
 
    (*LINK_element).SOURCE.plane          = LINK_source_plane;
    (*LINK_element).SOURCE.CLUSTER.x      = LINK_source_cluster_x;
    (*LINK_element).SOURCE.CLUSTER.y      = LINK_source_cluster_y;
    (*LINK_element).SOURCE.CLUSTER.width  = LINK_source_cluster_width;
    (*LINK_element).SOURCE.CLUSTER.height = LINK_source_cluster_height;
    (*LINK_element).SOURCE.UNIT.x         = LINK_source_unit_x;
    (*LINK_element).SOURCE.UNIT.y         = LINK_source_unit_y;
    (*LINK_element).SOURCE.MOVE.delta_x   = LINK_source_move_delta_x;
    (*LINK_element).SOURCE.MOVE.delta_y   = LINK_source_move_delta_y;

    (*LINK_element).TARGET.plane          = LINK_target_plane;
    (*LINK_element).TARGET.CLUSTER.x      = LINK_target_cluster_x;
    (*LINK_element).TARGET.CLUSTER.y      = LINK_target_cluster_y;
    (*LINK_element).TARGET.CLUSTER.width  = LINK_target_cluster_width;
    (*LINK_element).TARGET.CLUSTER.height = LINK_target_cluster_height;
    (*LINK_element).TARGET.UNIT.x         = LINK_target_unit_x;
    (*LINK_element).TARGET.UNIT.y         = LINK_target_unit_y;
    (*LINK_element).TARGET.MOVE.delta_x   = LINK_target_move_delta_x;
    (*LINK_element).TARGET.MOVE.delta_y   = LINK_target_move_delta_y;

    (*LINK_element).move                  = LINK_move;
    (*LINK_element).type_of_connection    = LINK_type_of_connection;

    (*LINK_element).next   = NULL;
    (*LINK_element).before = LINK_last_element;
   
    if(LINK_last_element != NULL) {
	(*LINK_last_element).next = LINK_element;
    }

    if(LINK_first_element == NULL) {
	LINK_first_element = LINK_element;
    }

    LINK_last_element = LINK_element;
    LINK_current_element = LINK_last_element;
   
    LINK_length++;
}

/*****************************************************************************
  FUNCTION : full_connection
 
  PURPOSE  : creates links between planes (see docu).

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/
static int full_connection(void)
{
    PLANE *PLANE_element;

    if(PLANE_length >= 2) { 

	PLANE_element = (*PLANE_first_element).next;

	LINK_source_plane = 0;
	LINK_target_plane = 1;
   
	LINK_source_cluster_x =  
	    LINK_source_cluster_y = 
	    LINK_source_cluster_width = 
	    LINK_source_cluster_height = 0;
	LINK_source_unit_x = LINK_source_unit_y = 0;
	LINK_source_move_delta_x = LINK_source_move_delta_y = 0;

	LINK_target_cluster_x = 
	    LINK_target_cluster_y = 
	    LINK_target_cluster_width = 
	    LINK_target_cluster_height = 0;
	LINK_target_unit_x = LINK_target_unit_y = 0;
	LINK_target_move_delta_x = LINK_target_move_delta_y = 0;

	LINK_move = 0;
	LINK_type_of_connection = PLANE_TO_PLANE;

	while(PLANE_element != NULL) {
 
	    LINK_source_plane++;
	    LINK_target_plane++;
	    enter_link();

	    PLANE_element = (*PLANE_element).next;
	}
	return 1;
    }
    return 0;
}

/*****************************************************************************
  FUNCTION : shortcut_connection
 
  PURPOSE  : creates links between planes(see docu).

  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/
static int shortcut_connection(void)
{
    PLANE *PLANE_source_element,*PLANE_target_element;

    if(PLANE_length >= 2) { 

	PLANE_source_element = (*PLANE_first_element).next;
	PLANE_target_element = PLANE_source_element;

	LINK_source_plane = 0;
   
	LINK_source_cluster_x =  
	    LINK_source_cluster_y = 
	    LINK_source_cluster_width = 
	    LINK_source_cluster_height = 0;
	LINK_source_unit_x = LINK_source_unit_y = 0;
	LINK_source_move_delta_x = LINK_source_move_delta_y = 0;

	LINK_target_cluster_x = 
	    LINK_target_cluster_y = 
	    LINK_target_cluster_width = 
	    LINK_target_cluster_height = 0;
	LINK_target_unit_x = LINK_target_unit_y = 0;
	LINK_target_move_delta_x = LINK_target_move_delta_y = 0;

	LINK_move = 0;
	LINK_type_of_connection = PLANE_TO_PLANE;

	while(PLANE_source_element != NULL){
	    LINK_source_plane++;
	    LINK_target_plane = LINK_source_plane;
	    while(PLANE_target_element != NULL) {
		LINK_target_plane++;
		enter_link();
		PLANE_target_element = (*PLANE_target_element).next;
	    }
	    PLANE_source_element = (*PLANE_source_element).next;
	    PLANE_target_element = PLANE_source_element;
	}
	return 1;
    }
    return 0;
}

/*****************************************************************************
  FUNCTION : overwrite_link
 
  PURPOSE  : overwrites the current link element (Current Link) with the 
             datas of the link editor (Edit Link).

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void overwrite_link(void)
{
    if(LINK_length == 0) {
	enter_link();
	return;
    } 
    (*LINK_current_element).SOURCE.plane         = LINK_source_plane;
    (*LINK_current_element).SOURCE.CLUSTER.x     = LINK_source_cluster_x;
    (*LINK_current_element).SOURCE.CLUSTER.y     = LINK_source_cluster_y;
    (*LINK_current_element).SOURCE.CLUSTER.width = LINK_source_cluster_width;
    (*LINK_current_element).SOURCE.CLUSTER.height= LINK_source_cluster_height;
    (*LINK_current_element).SOURCE.UNIT.x        = LINK_source_unit_x;
    (*LINK_current_element).SOURCE.UNIT.y        = LINK_source_unit_y;
    (*LINK_current_element).SOURCE.MOVE.delta_x  = LINK_source_move_delta_x;
    (*LINK_current_element).SOURCE.MOVE.delta_y  = LINK_source_move_delta_y;
  
    (*LINK_current_element).TARGET.plane         = LINK_target_plane;
    (*LINK_current_element).TARGET.CLUSTER.x     = LINK_target_cluster_x;
    (*LINK_current_element).TARGET.CLUSTER.y     = LINK_target_cluster_y;
    (*LINK_current_element).TARGET.CLUSTER.width = LINK_target_cluster_width;
    (*LINK_current_element).TARGET.CLUSTER.height= LINK_target_cluster_height;
    (*LINK_current_element).TARGET.UNIT.x        = LINK_target_unit_x;
    (*LINK_current_element).TARGET.UNIT.y        = LINK_target_unit_y;
    (*LINK_current_element).TARGET.MOVE.delta_x  = LINK_target_move_delta_x;
    (*LINK_current_element).TARGET.MOVE.delta_y  = LINK_target_move_delta_y;

    (*LINK_current_element).move                 = LINK_move;
    (*LINK_current_element).type_of_connection   = LINK_type_of_connection;

}


/*****************************************************************************
  FUNCTION : delete_current_link
 
  PURPOSE  : deletes  Currenrt Link out of the link list.

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void delete_current_link(void)
{
    LINK *LINK_element;

    LINK_element = LINK_current_element;

    /* Delete the element "LINK_element" out of the plane-list */

    if(LINK_length != 0) {
	if((*LINK_element).before != NULL) {
	    (*(*LINK_element).before).next = (*LINK_element).next;
	}
	if((*LINK_element).next != NULL) {
	    (*(*LINK_element).next).before = (*LINK_element).before;
	}

	LINK_length--;

	if(LINK_length == 0){
	    LINK_current_element = NULL;
	    LINK_first_element   = NULL;
	    LINK_last_element    = NULL;
	}else if((*LINK_element).next == NULL) {
	    LINK_current_element = (*LINK_element).before;
	    LINK_last_element    = LINK_current_element;
	}else if((*LINK_element).before == NULL) {
	    LINK_current_element = (*LINK_element).next;
	    LINK_first_element   = LINK_current_element;
	}else{
	    LINK_current_element = (*LINK_element).next;
	}  

	free(LINK_element);
    }
}

/*****************************************************************************
  FUNCTION : get_links_and_delete_them(plane_no)
 
  PURPOSE  : deletes all the links which are connected with the plane 
             "plane_no"

  NOTES    : 
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/
static void get_links_and_delete_them(int plane_no)
{
    LINK *LINK_element = LINK_first_element;

    while(LINK_element != NULL) {
	if(((*LINK_element).SOURCE.plane != plane_no) &&
	   ((*LINK_element).TARGET.plane != plane_no)) {
	    LINK_element = (*LINK_element).next;
	} else {
	    LINK_current_element = LINK_element;
	    LINK_element = (*LINK_element).next;
	    delete_current_link();
	}
    }
}


/*****************************************************************************
  FUNCTION : forward_link
 
  PURPOSE  : moves forward one list element in the link list.

  NOTES    : 
  RETURNS  :  Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/
static int forward_link(void)
{
    if(LINK_length != 0) {
	if((*LINK_current_element).next != NULL) {
	    LINK_current_element = (*LINK_current_element).next;
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

  UPDATE   : 20.12.1991
******************************************************************************/
static int backward_link(void)
{
    if(LINK_length != 0) {
	if((*LINK_current_element).before != NULL) {
	    LINK_current_element = (*LINK_current_element).before;
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

  UPDATE   : 20.12.1991
******************************************************************************/
static int beginning_link(void)
{
    if(LINK_length != 0){
	LINK_current_element = LINK_first_element;
	return 1;
    }
    return 0;
}

/*****************************************************************************
  FUNCTION : end_link
 
  PURPOSE  : jumps to the end of the link list.
  NOTES    : 
  RETURNS  : Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/
static int end_link(void)
{
    if(LINK_length != 0) {
	LINK_current_element = LINK_last_element;
	return 1;
    }
    return 0;
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
    } else {
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
    clear_current_link();
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
	if(LINK_length == 0) {
	    clear_current_link();
	} else {
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
	if(LINK_length == 0) {
	    clear_current_link();
	} else {
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

  UPDATE   : 20.12.1991
******************************************************************************/
static void delete_plane_PROC(void)
{
    delete_current_plane();
    if(PLANE_length == 0) {
	clear_current_plane();
    } else {
	write_current_plane();
    }
    if(LINK_length == 0) {
	clear_current_link();
    } else {
	write_current_link();
    }
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
    } else if(PLANE_pos == BN_BELOW) {
	PLANE_pos = BN_LEFT;
    } else {
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
    } else {
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
    (*PLANE_element).width  = PLANE_width;
    (*PLANE_element).height = PLANE_height;
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
}

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
    } else {
	PLANE_element = (PLANE *)malloc(sizeof(PLANE));
   
	(*PLANE_element).width  = PLANE_width; 
	(*PLANE_element).height = PLANE_height;
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
    } else {
	(*PLANE_current_element).width = PLANE_width;
	(*PLANE_current_element).height = PLANE_height;
	(*PLANE_current_element).pos = PLANE_pos;
	(*PLANE_current_element).type = PLANE_type;

	get_links_and_delete_them(PLANE_no);

	LINK_current_element = LINK_first_element;

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

    if(PLANE_length == 0)
	return;

    if((*PLANE_element).before != NULL) {
	(*(*PLANE_element).before).next = (*PLANE_element).next;
    }
    if((*PLANE_element).next != NULL) {
	(*(*PLANE_element).next).before = (*PLANE_element).before;
    }

    /* Delete all the links which are connected with the deleted plane */
   
    get_links_and_delete_them(PLANE_no);

    LINK_current_element = LINK_first_element;

    /* Decrement the number of the Planes */

    while(LINK_current_element) {
	if((*LINK_current_element).SOURCE.plane > PLANE_no) {
	    (*LINK_current_element).SOURCE.plane--;
	}
	if((*LINK_current_element).TARGET.plane > PLANE_no) {
	    (*LINK_current_element).TARGET.plane--;
	}
	LINK_current_element = (*LINK_current_element).next;
    }

    LINK_current_element = LINK_last_element;

    PLANE_length--;

    if(PLANE_length == 0){
	PLANE_current_element = NULL;
	PLANE_first_element   = NULL;
	PLANE_last_element    = NULL;
	PLANE_no--;
    }else if((*PLANE_element).next == NULL) {
	PLANE_current_element = (*PLANE_element).before;
	PLANE_last_element    = PLANE_current_element;
	PLANE_no--;
    }else if((*PLANE_element).before == NULL) {
	PLANE_current_element = (*PLANE_element).next;
	PLANE_first_element   = PLANE_current_element;
    }else {
	PLANE_current_element = (*PLANE_element).next;
    }

    free(PLANE_element);
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
    } else {
	return 0;
    }
}

/*****************************************************************************
  FUNCTION : backward_plane
 
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
    } else {
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
    } else {
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
    } else {
	return 0;
    }
}

/*****************************************************************************
  FUNCTION : get_plane
 
  PURPOSE  : returns a pointer to the plane element "plane_no"

  NOTES    : 
  RETURNS  : returns a pointer to the plane element "plane_no" 

  UPDATE   : 20.12.1991
******************************************************************************/
static PLANE *get_plane(int plane_no)
{
    PLANE *PLANE_element = PLANE_first_element;
    int counter = 1;

    while((PLANE_element != NULL) &&
	  (counter != plane_no)) {
	counter++;
	PLANE_element = (*PLANE_element).next;
          
    }
    return PLANE_element;
}

/* end of file */
/* lines: 2710 */




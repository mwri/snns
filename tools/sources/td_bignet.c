/*****************************************************************************
  FILE           : td_bignet.c
  SHORTNAME      : td_bignet
  SNNS VERSION   : 4.2

  PURPOSE        : Creates TDNN Network
  NOTES          :

  AUTHOR         : Michael Vogt
  DATE           : 12/09/93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.12 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:13 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "kr_typ.h"
#include "kernel.h"
#include "glob_typ.h"
#include "kr_ui.h"

#include "td_bignet.ph"

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
    int j,k;
    int correct = 0;
    int overlap =0;
    char buf1[80],buf2[80];

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
			    fprintf(stderr,"%s %d\n",buf2,k);
			    return;
			}
		    }
		}
		TD_LINK_test = TD_LINK_test->next;
	    }
	    if((correct < 1) && (k != TD_PLANE_length)){ 
		/* not all feature units are used */
		fprintf(stderr,"%s %d\n",buf1,k);
		return;
	    }
	    TD_LINK_test = TD_LINK_first_element;
	    correct = 0;
	}
    }

    if(TD_PLANE_length != 0) {

	krui_deleteNet();

	calculate_first_snns_unit_no_of_plane();
	create_snns_unit(); 

	while(TD_LINK_element != NULL) {

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
  FUNCTION : calculate_x_begin

  PURPOSE  : calculates the x-coordinate of a plane, where the plane is
             positioned in the display window.
             plane.
  NOTES    : 
  RETURNS  : The X-Position

  UPDATE   : 09.02.1993
******************************************************************************/


static int calculate_x_begin(int *new_x_begin,int *old_x_begin,
			     int *x_max,int width,int pos)

{
    if(pos == TD_BN_RIGHT) {
   
	*new_x_begin = *x_max + 2; 
	*x_max = *new_x_begin + width;
	*old_x_begin = *new_x_begin;
	return *new_x_begin;
    }else if(pos == TD_BN_BELOW) {
	if(*x_max < (*old_x_begin + width)) {
	    *x_max = *old_x_begin + width;
	}
	return *old_x_begin; 
    }else if(pos == TD_BN_LEFT) {
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

  UPDATE   : 09.02.1993
******************************************************************************/

static int calculate_y_begin(int *absolute_y_max,int *relative_y_max,
			     int *y_offset,int height,int pos)

{
    int y_begin;

    if(pos == TD_BN_RIGHT) {
	y_begin = *y_offset + 2;         
	*relative_y_max = y_begin + height;
	if(*absolute_y_max < *relative_y_max) {
	    *absolute_y_max = *relative_y_max;
	}
    }else if(pos == TD_BN_BELOW) {
	y_begin = *relative_y_max + 2;   
	*relative_y_max = y_begin + height;
	if(*absolute_y_max < *relative_y_max) {
	    *absolute_y_max = *relative_y_max;
	}
    }else if(pos == TD_BN_LEFT) {
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
    int found;

    ret = krui_allocateUnits((*TD_PLANE_last_element).width * 
                             (*TD_PLANE_last_element).height +
			     (*TD_PLANE_last_element).begin-1);
    if(ret != 0){
	fprintf(stderr, "%s", krui_error(ret));
	exit(1);
    }

    while(TD_PLANE_element != NULL) {

	height = (*TD_PLANE_element).height;
	width  = (*TD_PLANE_element).width;
	pos    = (*TD_PLANE_element).pos;
    
	y_begin = calculate_y_begin(&absolute_y_max,&relative_y_max,
				    &y_offset,height,pos);
	x_begin = calculate_x_begin(&new_x_begin,&old_x_begin,
				    &x_max,width,pos);

	for(y=0;y<height;y++){
	    for(x=0;x<width;x++){
		unit_no = krui_createUnit("td", "Out_Identity", 
					  "Act_TD_Logistic", 0.0, 0.0);
		if(unit_no<0){
		    fprintf(stderr, "%s", krui_error(ret));
		    exit(1);
		}
		unit_ptr = kr_getUnitPtr(unit_no);
		ret = krui_setUnitTType(unit_no,(*TD_PLANE_element).type+1);
		if(ret != 0){
		    fprintf(stderr, "%s", krui_error(ret));
		    exit(1);
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
		fprintf(stderr, "%s", krui_error(ret));
		exit(1);
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
		fprintf(stderr, "%s", krui_error(ret));
		exit(1);
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
			fprintf(stderr, "%s", krui_error(ret));
			exit(1);
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
    }while(move_p(TD_LINK_source_plane,&TD_LINK_source_cluster_y,
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
    else if(y_step > 0 && ((*y)+height+y_step<=(*plane_no).height)) {
	(*y) += y_step;
	(*x) = x_pivot;
    }else
	return 0;

    return 1;
}

/*****************************************************************************
  FUNCTION : more_data
 
  PURPOSE  : read command line integer value

  NOTES    : 
  RETURNS  : 

  UPDATE   : 
******************************************************************************/

static int more_data(char **rest, char **argv[])
{
    int retval;

    retval = atoi(**argv);
    *rest = **argv;
    while (**rest != '\0' && isspace((int) **rest))
	(*rest)++;
    while (**rest != '\0' && isdigit((int) **rest))
	(*rest)++;
    (*argv)++;

    return retval;
}

/*****************************************************************************
  FUNCTION : read_link_elements

  PURPOSE  : reads the input datas of the link editor (Edit Link).

  NOTES    : 
  RETURNS  : @@@

  UPDATE   : 09.02.1993
******************************************************************************/

#define RETURN_IF_FAULT(x) { if ((x) != NULL && *(x) != '\0') return 0; }

static int read_link_elements(int *argc, char **argv[])

{
    char *rest = NULL;

    if (*argc < 7)
	return 0;
    TD_LINK_source_plane          = more_data(&rest, argv); RETURN_IF_FAULT(rest);
    TD_LINK_source_cluster_x      = more_data(&rest, argv); RETURN_IF_FAULT(rest);
    TD_LINK_source_cluster_y      = 1;
    TD_LINK_source_cluster_width  = more_data(&rest, argv); RETURN_IF_FAULT(rest);
    TD_LINK_source_cluster_height = more_data(&rest, argv); RETURN_IF_FAULT(rest);

    TD_LINK_target_plane          = more_data(&rest, argv); RETURN_IF_FAULT(rest);
    TD_LINK_target_cluster_x      = more_data(&rest, argv); RETURN_IF_FAULT(rest);
    TD_LINK_target_cluster_y      = 1;
    TD_LINK_target_cluster_width  = more_data(&rest, argv); RETURN_IF_FAULT(rest);
    TD_LINK_target_cluster_height = 1;
    (*argc) -= 7;

    return correct_link_input();
}

#undef RETURN_IF_FAULT
#undef MORE_DATA



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

    TD_PLANE_source_no = get_plane(TD_LINK_source_plane);
    TD_PLANE_target_no = get_plane(TD_LINK_target_plane);

    TD_LINK_move = 0;

    if((TD_PLANE_source_no != TD_PLANE_target_no) &&
       TD_PLANE_source_no != NULL &&
       TD_PLANE_target_no != NULL &&
       (TD_LINK_source_cluster_x + TD_LINK_source_cluster_width  <= 
	(*TD_PLANE_source_no).width + 1) &&
       (TD_LINK_source_cluster_y + TD_LINK_source_cluster_height <= 
	(*TD_PLANE_source_no).height + 1) &&
       (TD_LINK_source_unit_x <= (*TD_PLANE_source_no).width) &&
       (TD_LINK_source_unit_y <= (*TD_PLANE_source_no).height) &&
       (TD_LINK_target_cluster_x + TD_LINK_target_cluster_width  <= 
	(*TD_PLANE_target_no).width + 1) &&
       (TD_LINK_target_cluster_y + TD_LINK_target_cluster_height <= 
	(*TD_PLANE_target_no).height + 1) &&
       (TD_LINK_target_unit_x <= (*TD_PLANE_target_no).width) &&
       (TD_LINK_target_unit_y <= (*TD_PLANE_target_no).height)) {
   
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
	    return 0;
	}

	a=b=0;

	if(((a=TD_LINK_source_unit_x) >= 1) &&
	   ((b=TD_LINK_source_unit_y) >= 1)) {
	    source_unit = TD_SOURCE_UNIT;
	}

	if((! source_unit) && a+b) {
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
	    return 0;
	}

	a=b=0;

	if(((a=TD_LINK_target_unit_x) >= 1) &&
	   ((b=TD_LINK_target_unit_y) >= 1)) {
	    target_unit = TD_TARGET_UNIT;
	}

	if((! target_unit) && a+b) {
	    return 0;
	}


	if(TD_LINK_source_cluster_height !=
	   (*TD_PLANE_source_no).height - (*TD_PLANE_target_no).height + 1){
	    return 0;
	}


	link_type =  target_plane+source_plane+
	             target_cluster+source_cluster+
		     target_unit+source_unit;

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
	    return 1;
	}			/* if */
    }
    return 0;
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
  FUNCTION : read_plane_elements

  PURPOSE  : reads the input datas of the plane editor (Edit Plane).

  NOTES    : 
  RETURNS  :  Error = 0 ; Succes = 1

  UPDATE   : 09.02.1993
******************************************************************************/

static int read_plane_elements(int *argc, char **argv[])

{
    if (*argc > 0)
    {
	TD_PLANE_width = atoi(**argv);
	(*argv)++;
	(*argc)--;
    }
    else return 0;

    if (*argc > 0)
    {
	TD_PLANE_height = atoi(**argv);
	(*argv)++;
	(*argc)--;
    }
    else return 0;

    if((TD_PLANE_width > 0) && (TD_PLANE_height > 0)) {
	return 1;
    }else{
	return 0;
    }
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
    (*TD_PLANE_element).pos    = TD_BN_RIGHT;
    (*TD_PLANE_element).type   = TD_BN_HIDDEN;  
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
  FUNCTION : usage
 
  PURPOSE  : prints out usage information and exits

  NOTES    : 
  RETURNS  : 

  UPDATE   : 
******************************************************************************/
void usage(void)
{

    fprintf(stderr, "usage: td_bignet <plane definition> ... <link definition> ... [<output file>]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "<plane definition> : -p <f> <d>\n");
    fprintf(stderr, "                      <f>  : number of feature units\n");
    fprintf(stderr, "                      <d>  : total delay length\n");
    fprintf(stderr, "<link defintion>   : -l <sp> <sf> <sw> <d> <tp> <tf> <tw>\n");
    fprintf(stderr, "                      <sp> : source plane (1, 2, ...)\n");
    fprintf(stderr, "                      <sf> : 1st feature unit in source plane\n");
    fprintf(stderr, "                      <sw> : field width in source plane\n");
    fprintf(stderr, "                      <d>  : delay length in source plane\n");
    fprintf(stderr, "                      <tp> : target plane (2, 3, ...)\n");
    fprintf(stderr, "                      <tf> : 1st feature unit in target plane\n");
    fprintf(stderr, "                      <tw> : field width in target plane\n");
    fprintf(stderr, "<output file>      : name of the output file (default SNNS_TD_NET.net)\n");
    exit(1);
}

/*****************************************************************************
  FUNCTION : main
 
  PURPOSE  : main time-delay network generator function

  NOTES    : 
  RETURNS  : 

  UPDATE   : 
******************************************************************************/
void main(int argc, char *argv[])
{
    char *file_name;
    int err;
    file_name = "SNNS_TD_NET.net";

    if (argc <= 1)
	usage();

    argc--;
    argv++;
    while (argc > 0)
    {
	if (strcmp(*argv, "-p") == 0)
	{
	    argc--;
	    argv++;

	    if (!read_plane_elements(&argc, &argv)){
		fprintf(stderr,"Illegal plane definition\n");
		exit(1);
	    }
	    enter_plane();
	}
	else if (strcmp(*argv, "-l") == 0)
	{
	    argc--;
	    argv++;

	    if (!read_link_elements(&argc, &argv)){
		fprintf(stderr,"Illegal link definition\n");
		exit(1);
	    }
	    enter_link();
	}
	else
	{
	    if (argc != 1)
	    {
		fprintf(stderr,"Parameters not understood.\n");
		exit(1);
	    }
	    file_name = *argv;
	    argc--;
	    argv++;
	}
    }

    if(TD_PLANE_first_element != NULL) 
    {
	TD_PLANE_first_element -> type = TD_BN_INPUT;
    }

    if(TD_PLANE_last_element != NULL) 
    {
	TD_PLANE_last_element -> type = TD_BN_OUTPUT;
    }
    
    create_net_PROC();
    err = krui_setUpdateFunc("TimeDelay_Order");
    if (err != KRERR_NO_ERROR) {
	fprintf(stderr, "%s", krui_error(err));
	exit(1);
    }
    err = krui_setLearnFunc("TimeDelayBackprop");
    if (err != KRERR_NO_ERROR) {
	fprintf(stderr, "%s", krui_error(err));
	exit(1);
    }
    err = krui_saveNet(file_name, "SNNS_TD_NET");
    if (err != KRERR_NO_ERROR) {
	fprintf(stderr, "%s", krui_error(err));
	exit(1);
    }

    exit(0);
}

/* end of file */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/ff_bignet.c,v $
  SHORTNAME      : ff_bignet
  SNNS VERSION   : 4.2

  PURPOSE        : Creates feedforward network 
  NOTES          :

  AUTHOR         : Michael Vogt (xgui Version by Michael Schmalzl)
  DATE           : 4.7.1997

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 1.11 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:06 $

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

#include "ff_bignet.ph"



/*****************************************************************************
  FUNCTION : create_net_PROC

  PURPOSE  : creates a SNNS-net in two steps:
             1. calls up a function, which creates SNNS-units
             2. calls up a function, which connections the SNNS-units.
  NOTES    : There are nine different ways (PLANE_TO_PLANE, 
             PLANE_TO_CLUSTER ...) two different planes can be connected.
  RETURNS  :

  UPDATE   : 20.12.1991
******************************************************************************/

static void create_net_PROC(void)

{
    LINK  *LINK_element = LINK_first_element;
    PLANE *source_plane, *target_plane;
    int move,type_of_connection=1;

    if(PLANE_length == 0) 
	return;

    calculate_first_snns_unit_no_of_plane();
    create_snns_unit(); 

    while(LINK_element != NULL) {

	move               = (*LINK_element).move;
	type_of_connection = (*LINK_element).type_of_connection;
	source_plane       =  get_plane((*LINK_element).SOURCE.plane);
	target_plane       =  get_plane((*LINK_element).TARGET.plane);
   
	switch(type_of_connection) {
	case PLANE_TO_PLANE: 
	    make_link(source_plane, 0, 0, (*source_plane).width,
		      (*source_plane).height, target_plane, 0, 0,
		      (*target_plane).width,(*target_plane).height);
	    break;
	case PLANE_TO_CLUSTER: 
	    make_link(source_plane, 0, 0, (*source_plane).width,
		      (*source_plane).height, target_plane,
		      (*LINK_element).TARGET.CLUSTER.x-1,
		      (*LINK_element).TARGET.CLUSTER.y-1,
		      (*LINK_element).TARGET.CLUSTER.width,
		      (*LINK_element).TARGET.CLUSTER.height);
	    break;
	case PLANE_TO_UNIT: 
	    make_link(source_plane, 0, 0, (*source_plane).width,
		      (*source_plane).height, target_plane,
		      (*LINK_element).TARGET.UNIT.x-1,
		      (*LINK_element).TARGET.UNIT.y-1, 1, 1);
	    break;
	case CLUSTER_TO_PLANE: 
	    make_link(source_plane, (*LINK_element).SOURCE.CLUSTER.x-1,
		      (*LINK_element).SOURCE.CLUSTER.y-1,
		      (*LINK_element).SOURCE.CLUSTER.width,
		      (*LINK_element).SOURCE.CLUSTER.height,
		      target_plane, 0, 0, (*target_plane).width,
		      (*target_plane).height);
	    break;
	case CLUSTER_TO_CLUSTER: 
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
	    } else {
		make_link(source_plane, (*LINK_element).SOURCE.CLUSTER.x-1,
			  (*LINK_element).SOURCE.CLUSTER.y-1,
			  (*LINK_element).SOURCE.CLUSTER.width,
			  (*LINK_element).SOURCE.CLUSTER.height,
			  target_plane, (*LINK_element).TARGET.CLUSTER.x-1,
			  (*LINK_element).TARGET.CLUSTER.y-1,
			  (*LINK_element).TARGET.CLUSTER.width,
			  (*LINK_element).TARGET.CLUSTER.height);
	    }
	    break;
	case CLUSTER_TO_UNIT: 
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
	    } else {
		make_link(source_plane, (*LINK_element).SOURCE.CLUSTER.x-1,
			  (*LINK_element).SOURCE.CLUSTER.y-1,
			  (*LINK_element).SOURCE.CLUSTER.width,
			  (*LINK_element).SOURCE.CLUSTER.height,
			  target_plane,(*LINK_element).TARGET.UNIT.x-1,
			  (*LINK_element).TARGET.UNIT.y-1, 1, 1);
	    }
	    break;
	case UNIT_TO_PLANE: 
	    make_link(source_plane, (*LINK_element).SOURCE.UNIT.x-1,
		      (*LINK_element).SOURCE.UNIT.y-1, 1, 1, target_plane,
		      0, 0, (*target_plane).width, (*target_plane).height);
	    break;
	case UNIT_TO_CLUSTER: 
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
	    } else {
		make_link(source_plane, (*LINK_element).SOURCE.UNIT.x-1,
			  (*LINK_element).SOURCE.UNIT.y-1, 1, 1,
			  target_plane, (*LINK_element).TARGET.CLUSTER.x-1,
			  (*LINK_element).TARGET.CLUSTER.y-1,
			  (*LINK_element).TARGET.CLUSTER.width,
			  (*LINK_element).TARGET.CLUSTER.height);
	    }
	    break;
	case UNIT_TO_UNIT: 
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
	    } else {
		make_link(source_plane, (*LINK_element).SOURCE.UNIT.x-1,
			  (*LINK_element).SOURCE.UNIT.y-1, 1, 1,
			  target_plane,(*LINK_element).TARGET.UNIT.x-1,
			  (*LINK_element).TARGET.UNIT.y-1, 1, 1);
	    }
	    break;
	} /* switch */
	LINK_element = (*LINK_element).next;

    } /* while */    
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

static int calculate_x_begin(int *new_x_begin, int *old_x_begin, int *x_max,
			     int width, int pos)

{
    if(pos == BN_RIGHT) {   
	*new_x_begin = *x_max + 2; 
	*x_max = *new_x_begin + width;
	*old_x_begin = *new_x_begin;
	return *new_x_begin;
    } else if(pos == BN_BELOW) {
	if(*x_max < (*old_x_begin + width)) {
	    *x_max = *old_x_begin + width;
	}
	return *old_x_begin; 
    } else if(pos == BN_LEFT) {
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

static int calculate_y_begin(int *absolute_y_max, int *relative_y_max, 
			     int *y_offset, int height, int pos)
{
    int y_begin;

    if(pos == BN_RIGHT) {
	y_begin = *y_offset + 2;         
	*relative_y_max = y_begin + height;
	if(*absolute_y_max < *relative_y_max) {
	    *absolute_y_max = *relative_y_max;
	}
    } else if(pos == BN_BELOW) {
	y_begin = *relative_y_max + 2;   
	*relative_y_max = y_begin + height;
	if(*absolute_y_max < *relative_y_max) {
	    *absolute_y_max = *relative_y_max;
	}
    } else if(pos == BN_LEFT) {
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
	fprintf(stderr, "%s\n", krui_error(ret));
	exit(1);
    }

    while(PLANE_element != NULL) {

	height = (*PLANE_element).height;
	width  = (*PLANE_element).width;
	pos    = (*PLANE_element).pos;
    
	y_begin = calculate_y_begin(&absolute_y_max, &relative_y_max,
				    &y_offset, height, pos);
	x_begin = calculate_x_begin(&new_x_begin, &old_x_begin,
				    &x_max, width, pos);

	for(y=0;y<height;y++){
	    for(x=0;x<width;x++){
		unit_no = krui_createUnit("unit", (*PLANE_element).out_func, 
					  (*PLANE_element).act_func, 0.0, 0.0);
		if(unit_no<0){
		    fprintf(stderr, "%s\n", krui_error(unit_no));
		    exit(1);
		}

		ret = krui_setUnitTType(unit_no,(*PLANE_element).type+1);
		if(ret != 0){
		    fprintf(stderr, "%s\n", krui_error(ret));
		    exit(1);
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
		      int LINK_source_cluster_y,int LINK_source_cluster_width,
		      int LINK_source_cluster_height, PLANE *LINK_target_plane,
		      int LINK_target_cluster_x, int LINK_target_cluster_y,
                      int LINK_target_cluster_width,
		      int LINK_target_cluster_height)

{
    int i,j,k,l,ret,target_unit,source_unit;

    for(i = LINK_target_cluster_y; 
	i < LINK_target_cluster_height+LINK_target_cluster_y; i++)
	for(j = LINK_target_cluster_x;
	    j < LINK_target_cluster_width+LINK_target_cluster_x; j++)
	{
	    target_unit = (*LINK_target_plane).begin + 
		(*LINK_target_plane).width * i + j;
	    ret = krui_setCurrentUnit(target_unit);
	    if(ret != 0){
		fprintf(stderr, "%s\n", krui_error(ret));
		exit(1);
	    }
	    for(k = LINK_source_cluster_y;
		k < LINK_source_cluster_height+LINK_source_cluster_y; k++)
		for(l = LINK_source_cluster_x;
		    l < LINK_source_cluster_width+LINK_source_cluster_x; l++)
		{
		    source_unit = (*LINK_source_plane).begin + 
			(*LINK_source_plane).width * k + l;
		    if((ret = krui_createLink(source_unit,0.0)) != 0){
			fprintf(stderr, "%s\n", krui_error(ret));
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
    int xs_pivot=LINK_source_cluster_x;
    int xt_pivot=LINK_target_cluster_x;

    do {
	make_link(LINK_source_plane,LINK_source_cluster_x,LINK_source_cluster_y,
		  LINK_source_cluster_width,LINK_source_cluster_height,
		  LINK_target_plane,LINK_target_cluster_x,LINK_target_cluster_y,
		  LINK_target_cluster_width,LINK_target_cluster_height);
    } while(move_p(LINK_source_plane, &LINK_source_cluster_y, 
		   &LINK_source_cluster_x, LINK_source_cluster_width,
		   LINK_source_cluster_height, xs_pivot, 
		   LINK_source_move_delta_x, LINK_source_move_delta_y)  &&
	    move_p(LINK_target_plane, &LINK_target_cluster_y,
		   &LINK_target_cluster_x, LINK_target_cluster_width,
		   LINK_target_cluster_height, xt_pivot, 
		   LINK_target_move_delta_x, LINK_target_move_delta_y));
}
    

/*****************************************************************************
  FUNCTION : move_p

  PURPOSE  : tests whether the boundaries of a plane are crossed while
             "moving" (see docu)

  NOTES    : 
  RETURNS  : 

  UPDATE   : 20.12.1991
******************************************************************************/

static int move_p(PLANE *plane_no, int *y, int *x, int width, int height,
		  int x_pivot, int x_step, int y_step)

{
    if(x_step > 0 && ((*x)+width+x_step<=(*plane_no).width))
	(*x)+= x_step;
    else
	if(y_step > 0 && ((*y)+height+y_step<=(*plane_no).height)){
	    (*y) += y_step;
	    (*x) = x_pivot;
	}else
	    return 0;

    return 1;
}


/*****************************************************************************
  FUNCTION : more_data
 
  PURPOSE  : read command line integer value or "+"

  NOTES    : 
  RETURNS  : 

  UPDATE   : 
******************************************************************************/

static int more_data(char **rest, char **argv[], int *argc)
{
    int retval = 0;

    if (*argc > 0 && strcmp(**argv, "+") != 0)
    {
	retval = atoi(**argv);
	*rest = **argv;
	while (**rest != '\0' && isspace((int) **rest))
	    (*rest)++;
	while (**rest != '\0' && isdigit((int) **rest))
	    (*rest)++;
	(*argc)--;
	(*argv)++;
    }

    return retval;
}

/*****************************************************************************
  FUNCTION : read_link_elements

  PURPOSE  : reads the input datas of the link editor (Edit Link).

  NOTES    : 
  RETURNS  : @@@

  UPDATE   : 20.12.1991
******************************************************************************/

#define RETURN_IF_FAULT(x) { if ((x) != NULL && *(x) != '\0') return 0; }

static int read_link_elements(int *argc, char **argv[])

{
    /* the + character may be used to fill the remaining parameters
       of one link definition column (compare bignet for xgui).
       e.g. argv values like '1 + 2 +' result in a full connection 
       between plane 1 and plane 2 */
    char *rest = NULL;

    if (*argc <= 0)
	return 0;
    LINK_source_plane          = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);

    LINK_source_cluster_x      = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);

    LINK_source_cluster_y      = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);

    LINK_source_cluster_width  = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);

    LINK_source_cluster_height = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);

    LINK_source_unit_x         = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);

    LINK_source_unit_y         = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);

    LINK_source_move_delta_x   = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);

    LINK_source_move_delta_y   = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    if (*argc > 0 && strcmp(**argv, "+") == 0)
    {
	(*argc)--;
	(*argv)++;
    }

    if (*argc <= 0)
	return 0;
    LINK_target_plane          = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    LINK_target_cluster_x      = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    LINK_target_cluster_y      = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    LINK_target_cluster_width  = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    LINK_target_cluster_height = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    LINK_target_unit_x         = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    LINK_target_unit_y         = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    LINK_target_move_delta_x   = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    LINK_target_move_delta_y   = more_data(&rest, argv, argc); 
    RETURN_IF_FAULT(rest);
    
    if (*argc > 0 && strcmp(**argv, "+") == 0)
    {
	(*argc)--;
	(*argv)++;
    }

    return correct_link_input();
}

#undef RETURN_IF_FAULT

/*****************************************************************************
  FUNCNCTION : correct_link_input
 
  PURPOSE  : tests whether the input datas of Edit Link are correct and decides
             what kind of link (PLANE_TO_PLANE, PLANE_TO_LINK...) we have.

  NOTES    : 
  RETURNS  : Correct = 1 ; ERROR = 0

  UPDATE   : 20.12.1991
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
}/* enter_link */



/*****************************************************************************
  FUNCTION : read_plane_elements

  PURPOSE  : reads the input datas of the plane editor (Edit Plane).

  NOTES    : 
  RETURNS  :  Error = 0 ; Succes = 1

  UPDATE   : 20.12.1991
******************************************************************************/

static int read_plane_elements(int *argc, char **argv[])
{

    if (*argc > 0)
    {
	PLANE_width = atoi(**argv);
	(*argv)++;
	(*argc)--;
    }
    else return 0;

    if (*argc > 0)
    {
	PLANE_height = atoi(**argv);
	(*argv)++;
	(*argc)--;
    }
    else return 0;

    PLANE_type = BN_HIDDEN;
    PLANE_type_was_set = 0;
    PLANE_act_func = DEFAULT_ACT_FUNC;
    PLANE_out_func = DEFAULT_OUT_FUNC;
    
    if (*argc > 0 && krui_isFunction(**argv, ACT_FUNC))
    {
	PLANE_act_func = **argv;
	(*argv)++;
	(*argc)--;
	if (*argc > 0 && krui_isFunction(**argv, OUT_FUNC))
	{
	    PLANE_out_func =  **argv;
	    (*argv)++;
	    (*argc)--;
	    
	    if (*argc > 0)
	    {
		if (strcasecmp(**argv, "input") == 0)
		{
		    PLANE_type = BN_INPUT;
		    PLANE_type_was_set = 1;
		    (*argv)++;
		    (*argc)--;
		}
		else if (strcasecmp(**argv, "hidden") == 0)
		{
		    PLANE_type = BN_HIDDEN;
		    PLANE_type_was_set = 1;
		    (*argv)++;
		    (*argc)--;
		}
		else if (strcasecmp(**argv, "output") == 0)
		{
		    PLANE_type = BN_OUTPUT;
		    PLANE_type_was_set = 1;
		    (*argv)++;
		    (*argc)--;
		}
	    }
	}
    }

    return (PLANE_width > 0 && PLANE_height > 0) ? 1 : 0;
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
    (*PLANE_element).pos    = BN_RIGHT;
    (*PLANE_element).type   = PLANE_type;
    (*PLANE_element).type_was_set = PLANE_type_was_set;
    (*PLANE_element).act_func = PLANE_act_func;
    (*PLANE_element).out_func = PLANE_out_func;
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


/*****************************************************************************
  FUNCTION : usage
 
  PURPOSE  : gives usage information 

  NOTES    : 
  RETURNS  : 

  UPDATE   : 
******************************************************************************/
void usage(void)
{

    fprintf(stderr, "usage: ff_bignet <plane definition> ... <link definition> ... [<output file>]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "<plane definition> : -p <x> <y> [<act> [<out> [<type>]]]\n");
    fprintf(stderr, "                      <x>   : number of units in x-direction\n");
    fprintf(stderr, "                      <y>   : number of units in y-direction\n");
    fprintf(stderr, "                      <act> : opt. activation function\n");
    fprintf(stderr, "                              e.g.: Act_Logistic\n");
    fprintf(stderr, "                      <out> : opt. activation function, <act> must be given too\n");
    fprintf(stderr, "                              e.g.: Out_Identity\n");
    fprintf(stderr, "                      <type>: opt. layer type, <act> and <out> must be given too\n");
    fprintf(stderr, "                              valid types: input, hidden, or output\n\n");
    fprintf(stderr, "<link defintion>   : -l <sp> ... [+] <tp> ... [+]\n");
    fprintf(stderr, "                      Link parameters must be given in the following order.\n");
    fprintf(stderr, "                      All remaing Parameters within one section may be filled\n");
    fprintf(stderr, "                      with 0 by giving the '+' character (0 means: not used)\n\n");
    fprintf(stderr, "                      Source section:\n");
    fprintf(stderr, "                      <sp>  : source plane (1, 2, ...)\n");
    fprintf(stderr, "                      <scx> : x position of source cluster\n");
    fprintf(stderr, "                      <scy> : y position of source cluster\n");
    fprintf(stderr, "                      <scw> : width of source cluster\n");
    fprintf(stderr, "                      <sch> : height of source cluster\n");
    fprintf(stderr, "                      <sux> : x position of a distinct source unit\n");
    fprintf(stderr, "                      <suy> : y position of a distinct source unit\n");
    fprintf(stderr, "                      <smx> : delta x for multiple source fields\n");
    fprintf(stderr, "                      <smy> : delta y for multiple source fields\n\n");
    fprintf(stderr, "                      Target section:\n");
    fprintf(stderr, "                      <tp>  : target plane (1, 2, ...)\n");
    fprintf(stderr, "                      <tcx> : x position of target cluster\n");
    fprintf(stderr, "                      <tcy> : y position of target cluster\n");
    fprintf(stderr, "                      <tcw> : width of target cluster\n");
    fprintf(stderr, "                      <tch> : height of target cluster\n");
    fprintf(stderr, "                      <tux> : x position of a distinct target unit\n");
    fprintf(stderr, "                      <tuy> : y position of a distinct target unit\n");
    fprintf(stderr, "                      <tmx> : delta x for multiple target fields\n");
    fprintf(stderr, "                      <tmy> : delta y for multiple target fields\n\n");
    fprintf(stderr, "<output file>      : name of the output file (default SNNS_FF_NET.net)\n");
    exit(1);
}

/*****************************************************************************
  FUNCTION : main
 
  PURPOSE  : 

  NOTES    : 
  RETURNS  : 

  UPDATE   : 
******************************************************************************/

void main(int argc, char *argv[])
{
    char *file_name;
    int err;

    file_name = "SNNS_FF_NET.net";

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
		fprintf(stderr, "Illegal plane definition\n");
		exit(1);
	    }

	    enter_plane();
	}
	else if (strcmp(*argv, "-l") == 0)
	{
	    argc--;
	    argv++;

	    if (!read_link_elements(&argc, &argv)){
		fprintf(stderr, "Illegal link definition\n");
		exit(1);
	    }

	    enter_link();
	}
	else
	{
	    if (argc != 1)
	    {
		fprintf(stderr, "Parameters not understood. Invalid function or type name?\n");
		exit(1);
	    }
	    file_name = *argv;
	    argc--;
	    argv++;
	}
    }

    if(PLANE_first_element != NULL && !PLANE_first_element->type_was_set) 
    {
	PLANE_first_element -> type = BN_INPUT;
    }

    if(PLANE_last_element != NULL && !PLANE_last_element->type_was_set) 
    {
	PLANE_last_element -> type = BN_OUTPUT;
    }
    
    create_net_PROC();

    err = krui_setUpdateFunc("Topological_Order");
    if (err != KRERR_NO_ERROR){
	fprintf(stderr, "%s\n", krui_error(err));
	exit(1);
    }
    err = krui_setLearnFunc("Std_Backpropagation");
    if (err != KRERR_NO_ERROR){
	fprintf(stderr, "%s\n", krui_error(err));
	exit(1);
    }
    err = krui_saveNet(file_name, "SNNS_FF_NET");
    if (err != KRERR_NO_ERROR){
	fprintf(stderr, "%s\n", krui_error(err));
	exit(1);
    }

    exit(0);
}

/* end of file */

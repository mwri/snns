/*****************************************************************************
  FILE           : td_bignet.ph
  SHORTNAME      : td_bignet
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Michael Vogt
  DATE           : 09.02.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.7 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:14 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_TD_BIGNET_DEFINED_
#define  _BN_TD_BIGNET_DEFINED_


/* begin private definition section */

#define TD_BN_RIGHT  0
#define TD_BN_BELOW  1
#define TD_BN_LEFT   2

#define TD_BN_INPUT 0
#define TD_BN_OUTPUT 1
#define TD_BN_HIDDEN 2

#define TD_SOURCE_PLANE 1
#define TD_SOURCE_CLUSTER 3
#define TD_SOURCE_UNIT 5

#define TD_TARGET_PLANE 8
#define TD_TARGET_CLUSTER 17
#define TD_TARGET_UNIT 33

#define PLANE_TO_PLANE 9
#define PLANE_TO_CLUSTER 26
#define PLANE_TO_UNIT 42
#define CLUSTER_TO_PLANE 12
#define CLUSTER_TO_CLUSTER 29
#define CLUSTER_TO_UNIT 45
#define UNIT_TO_PLANE 14
#define UNIT_TO_CLUSTER 31
#define UNIT_TO_UNIT 47

typedef struct plane {
  int width;
  int height;
  int pos;
  int type;
  int begin;
  struct plane *next;
  struct plane *before;
} TD_PLANE;

static TD_PLANE *TD_PLANE_current_element = NULL;
static TD_PLANE *TD_PLANE_first_element = NULL;
static TD_PLANE *TD_PLANE_last_element = NULL;

static int   TD_PLANE_length = 0;
static int   TD_PLANE_no = 0;

static int TD_PLANE_width,TD_PLANE_height;

typedef struct link 
{
    struct {
	int plane;
	struct {
	    int x;
	    int y;
	    int width;
	    int height;
	} CLUSTER;
	struct {
	    int x;
	    int y;
	} UNIT;
	struct {
	    int delta_x;
	    int delta_y;
	} MOVE;
    } SOURCE;
    struct {
	int plane;
	struct {
	    int x;
	    int y;
	    int width;
	    int height;
	} CLUSTER;
	struct {
	    int x;
	    int y;
	} UNIT;
	struct {
	    int delta_x;
	    int delta_y;
	} MOVE;
    } TARGET;
    int type_of_connection;
    int move;
    struct link *before;
    struct link *next;
} TD_LINK;

static TD_LINK *TD_LINK_current_element = NULL;
static TD_LINK *TD_LINK_first_element = NULL;
static TD_LINK *TD_LINK_last_element = NULL;

static int TD_LINK_length=0;

static int TD_LINK_move,TD_LINK_type_of_connection;

static int TD_LINK_source_move_delta_x,TD_LINK_source_move_delta_y,
           TD_LINK_target_move_delta_x,TD_LINK_target_move_delta_y;

static int TD_LINK_source_plane,TD_LINK_source_cluster_x,
	   TD_LINK_source_cluster_y,
           TD_LINK_source_cluster_width,TD_LINK_source_cluster_height,
           TD_LINK_source_unit_x,TD_LINK_source_unit_y;

static int TD_LINK_target_plane,TD_LINK_target_cluster_x,
	   TD_LINK_target_cluster_y,
           TD_LINK_target_cluster_width,TD_LINK_target_cluster_height,
           TD_LINK_target_unit_x,TD_LINK_target_unit_y;

static void create_net_PROC(void);
static void calculate_first_snns_unit_no_of_plane(void);
static int calculate_x_begin(int *new_x_begin,int *old_x_begin,int *x_max,
			     int width,int pos);
static int calculate_y_begin(int *absolute_y_max,int *relative_y_max,
			     int *y_offset,int height,int pos);
static void create_snns_unit(void);
static void make_link(TD_PLANE *TD_LINK_source_plane,
		      int TD_LINK_source_cluster_x,
		      int TD_LINK_source_cluster_y,
		      int TD_LINK_source_cluster_width,
		      int TD_LINK_source_cluster_height,
		      TD_PLANE *TD_LINK_target_plane,
		      int TD_LINK_target_cluster_x,
		      int TD_LINK_target_cluster_y,
		      int TD_LINK_target_cluster_width,
		      int TD_LINK_target_cluster_height);
static void make_move_link(TD_PLANE *TD_LINK_source_plane,
			   int TD_LINK_source_cluster_x,
			   int TD_LINK_source_cluster_y,
			   int TD_LINK_source_cluster_width,
			   int TD_LINK_source_cluster_height,
			   TD_PLANE * TD_LINK_target_plane,
			   int TD_LINK_target_cluster_x,
			   int TD_LINK_target_cluster_y,
			   int TD_LINK_target_cluster_width,
			   int TD_LINK_target_cluster_height,
			   int TD_LINK_source_move_delta_x,
			   int TD_LINK_source_move_delta_y,
			   int TD_LINK_target_move_delta_x,
			   int TD_LINK_target_move_delta_y);
static int move_p(TD_PLANE *plane_no,int *y,int *x,int width,int height,
		  int x_pivot,int x_step,int y_step);

static int read_link_elements(int *argc, char **argv[]);
static int correct_link_input(void);
static void enter_link(void);
static int read_plane_elements(int *argc, char **argv[]);
static void enter_plane(void);
static TD_PLANE *get_plane(int plane_no);

/* end private definition section */

#endif 
/* end of file */

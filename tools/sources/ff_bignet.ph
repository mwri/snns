/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/ff_bignet.ph,v $
  SHORTNAME      : ff_bignet
  SNNS VERSION   : 4.2

  PURPOSE        : Creates feedforward network 
  NOTES          :

  AUTHOR         : Michael Vogt (xgui Version by Michael Schmalzl)
  DATE           : 4.7.1997

  CHANGED BY     : Sven Doering, Kai-Uwe Herrmann
  RCS VERSION    : $Revision: 1.6 $
  LAST CHANGE    : $Date: 1998/03/04 12:20:52 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_BIGNET_DEFINED_
#define  _BN_BIGNET_DEFINED_

/* begin private definition section */

#define BN_RIGHT  0
#define BN_BELOW  1
#define BN_LEFT   2

#define BN_INPUT 0
#define BN_OUTPUT 1
#define BN_HIDDEN 2

#define SOURCE_PLANE 1
#define SOURCE_CLUSTER 3
#define SOURCE_UNIT 5

#define TARGET_PLANE 8
#define TARGET_CLUSTER 17
#define TARGET_UNIT 33

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
    int type_was_set;
    int begin;
    char *act_func;
    char *out_func;
    struct plane *next;
    struct plane *before;
} PLANE;


static PLANE *PLANE_current_element = NULL;
static PLANE *PLANE_first_element = NULL;
static PLANE *PLANE_last_element = NULL;

static int   PLANE_length = 0;
static int   PLANE_no = 0;

static char *DEFAULT_ACT_FUNC = "Act_Logistic";
static char *DEFAULT_OUT_FUNC = "Out_Identity";

static int PLANE_width, PLANE_height;
static int PLANE_type;
static int PLANE_type_was_set;
static char *PLANE_act_func;
static char *PLANE_out_func;

typedef struct link {
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
} LINK;

static LINK *LINK_current_element = NULL;
static LINK *LINK_first_element = NULL;
static LINK *LINK_last_element = NULL;

static int LINK_length = 0;

static int LINK_move, LINK_type_of_connection;

static int LINK_source_move_delta_x, LINK_source_move_delta_y,
    LINK_target_move_delta_x, LINK_target_move_delta_y;

static int LINK_source_plane, LINK_source_cluster_x,
    LINK_source_cluster_y, LINK_source_cluster_width,
    LINK_source_cluster_height, LINK_source_unit_x,LINK_source_unit_y;

static int LINK_target_plane, LINK_target_cluster_x,
    LINK_target_cluster_y, LINK_target_cluster_width,
    LINK_target_cluster_height, LINK_target_unit_x, LINK_target_unit_y;


static void create_net_PROC(void);

static void calculate_first_snns_unit_no_of_plane(void);

static int calculate_x_begin(int *new_x_begin,int *old_x_begin,int
			     *x_max,int width,int pos);

static int calculate_y_begin(int *absolute_y_max,int
			     *relative_y_max,int *y_offset,int
			     height,int pos);

static void create_snns_unit(void);

static void make_link(PLANE *LINK_source_plane, 
		      int LINK_source_cluster_x, 
		      int LINK_source_cluster_y,
		      int LINK_source_cluster_width, 
		      int LINK_source_cluster_height, 
		      PLANE *LINK_target_plane, 
		      int LINK_target_cluster_x, 
		      int LINK_target_cluster_y, 
		      int LINK_target_cluster_width, 
		      int LINK_target_cluster_height);

static void make_move_link(PLANE *LINK_source_plane, 
			   int LINK_source_cluster_x, 
			   int LINK_source_cluster_y, 
			   int LINK_source_cluster_width, 
			   int LINK_source_cluster_height, 
			   PLANE *LINK_target_plane, 
			   int LINK_target_cluster_x, 
			   int LINK_target_cluster_y, 
			   int LINK_target_cluster_width, 
			   int LINK_target_cluster_height, 
			   int LINK_source_move_delta_x, 
			   int LINK_source_move_delta_y, 
			   int LINK_target_move_delta_x, 
			   int LINK_target_move_delta_y);

static int move_p(PLANE *plane_no, int *y, int *x, int width, int
		  height, int x_pivot, int x_step, int y_step);


static int read_link_elements(int *argc, char **argv[]);
static int correct_link_input(void);
static void enter_link(void);
static int read_plane_elements(int *argc, char **argv[]);
static void enter_plane(void);
static PLANE *get_plane(int plane_no);

/* end private definition section */

#endif 
/* end of file */
/* lines: 255 */

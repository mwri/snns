/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_TD_bignet.ph,v $
  SHORTNAME      : bn_TD_bignet
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 09.02.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:27 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_TD_BIGNET_DEFINED_
#define  _BN_TD_BIGNET_DEFINED_



/* begin global definition section */

void bn_create_TD_Bignet (void);

/* end global definition section */






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

#define td_bn_widget_output(string) ((strcmp("0",string)==0) ? ("") : (string))

static int TD_BN_ERROR=0;

typedef struct plane {
  int width;
  int height;
  int pos;
  int type;
  int begin;
  struct plane *next;
  struct plane *before;
} TD_PLANE;

static char *td_plane_pos[] = {"right","below","left"};
static char *td_plane_type[] = {"input","output","hidden"};

static TD_PLANE *TD_PLANE_current_element = NULL;
static TD_PLANE *TD_PLANE_first_element = NULL;
static TD_PLANE *TD_PLANE_last_element = NULL;

static int   TD_PLANE_length = 0;
static int   TD_PLANE_no = 0;
static int   TD_PLANE_pos = TD_BN_RIGHT;
static int   TD_PLANE_type = TD_BN_INPUT;

static int TD_PLANE_width,TD_PLANE_height;

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

#define bn_fontWidth 8

static int td_bignet_widget_open = 0;

static int td_bn_intWidth = 6 * bn_fontWidth;


static Widget td_baseWidget;

static Widget z1s1,z1s2,z1s3,z2s1,z3s1,z4s1,z5s1,z6s1,z7s1,z2s2,z3s2,z3s3,
              z4s2,z4s3,z5s2,z5s3,z6s2,z6s3,z7s2,z7s3,z8s2,z8s3,z9s1,z9s2,
              z9s3,z9s4,z10s1,z11s1,z12s1,z13s1,z15s1,z16s1,z10s2,z10s3,z10s4,
              z10s5,z13s2,z13s3,z13s4,z13s5,z15s2,z15s3,z15s4,z15s5,z16s2,
              z16s3,z16s4;


static int TD_NET=0;



/* bn_bignet */
static void bn_createPlanePannel (Widget parent);
static void bn_createLinkPannel (Widget parent);
static void exit_PROC (void);

/* bn_create_net */
static void create_net_PROC(void);
static void calculate_first_snns_unit_no_of_plane(void);
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

/* bn_link */

static int read_link_elements(void);
static void write_current_link(void);
static void clear_current_link(void);
static void write_current_link_to_editor(void);
static void clear_edit_link(void);
static void enter_link_PROC(void);
static void overwrite_link_PROC(void);
static void delete_link_PROC(void);
static void forward_link_PROC(void);
static void backward_link_PROC(void);
static void end_link_PROC(void);
static void beginning_link_PROC(void);
static void current_link_to_editor_PROC(void);
static int correct_link_input(void);
static void enter_link(void);
static void overwrite_link(void);
static void delete_current_link(void);
static void get_links_and_delete_them(int plane_no);
static int forward_link(void);
static int backward_link(void);
static int beginning_link(void);
static int end_link(void);

/* bn_plane */

static int read_plane_elements(void);
static void write_current_plane(void);
static void clear_current_plane(void);
static void write_current_plane_to_editor(void);
static void clear_edit_plane(void);
static void cancel_net_PROC(void);
static void enter_plane_PROC(void);
static void insert_plane_PROC(void);
static void overwrite_plane_PROC(void);
static void delete_plane_PROC(void);
static void forward_plane_PROC(void);
static void backward_plane_PROC(void);
static void end_plane_PROC(void);
static void beginning_plane_PROC(void);
static void pos_PROC(void);
static void current_plane_to_editor_PROC(void);
static void type_PROC(void);
static void enter_plane(void);
static void insert_plane(void);
static void overwrite_plane(void);
static void delete_current_plane(void);
static int forward_plane(void);
static int backward_plane(void);
static int beginning_plane(void);
static int end_plane(void);
static TD_PLANE *get_plane(int plane_no);

/* end private definition section */

#endif 
/* end of file */
/* lines: 255 */


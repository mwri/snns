/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_bighop.ph,v $
  SHORTNAME      : bn_bighop
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Christine Bagdi 
  DATE           : 27.5.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:44 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_BIGHOP_DEFINED_
#define  _BN_BIGHOP_DEFINED_



/* begin global definition section */

void bn_createBigHop (void);

/* end global definition section */






/* begin private definition section */

#define BN_RIGHT  0
#define BN_BELOW  1
#define BN_LEFT   2

#define BN_INPUT 0
#define BN_OUTPUT 1
#define BN_HIDDEN 2
#define BN_DUAL 3

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

#define bn_widget_output(string) ((strcmp("0",string)==0) ? ("") : (string))

#define UI_GENERAL 0


static int BN_ERROR=0;

typedef struct plane {
  int width;
  int height;
  int pos;
  int type;
  int begin;
  struct plane *next;
  struct plane *before;
} PLANE;

static char *plane_pos[] = {"right","below","left"};
static char *plane_type[] = {"input","output","hidden","dual"};

static PLANE *PLANE_current_element = NULL;
static PLANE *PLANE_first_element = NULL;
static PLANE *PLANE_last_element = NULL;

static int   PLANE_length = 0;
static int   PLANE_no = 0;
static int   PLANE_pos = BN_RIGHT;
static int   PLANE_type = BN_DUAL;

static int PLANE_width,PLANE_height;


#define bn_fontWidth 8

static int bighop_widget_open = 0;

static int bn_intWidth = 6 * bn_fontWidth;


static Widget hop_baseWidget;
static Widget z1s1,z1s2,z1s3,z2s1,z3s1,z4s1,z5s1,z6s1,z7s1;

static Widget z2s2,z3s2,z3s3,z4s2,z4s3,
       z5s2,z5s3,z6s2,z6s3,z7s2,z7s3;





/* bn_bignet */
static void bn_createPlanePannel (Widget parent);

/* bn_create_net */
static void create_net_PROC(void);
static void calculate_first_snns_unit_no_of_plane(void);
static void create_snns_unit(void);

static void make_links(void);

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

/* end private definition section */

#endif 
/* end of file */
/* lines: 255 */

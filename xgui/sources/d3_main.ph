/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_main.ph,v $
  SHORTNAME      : main.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_main.c
  NOTES          : all functions will be exported 

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:24 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_MAIN_DEFINED_
#define _D3_MAIN_DEFINED_


/* begin global definition section */


#define CENTER_VECTOR 8


void d3_getColorValue (int mode, int unitNo, float *value);
void d3_drawNet (void);
void d3_recenter_window (int width, int height);
void d3_resetDisplay (void);
void d3_clear_xyTranslationTable (void);
void d3_init_main (void);


/* end global definition section */


/* begin private definition section */


static void get_unit_pos_vector (int unit, vector v);
static void unit_transformation (cube c, matrix m);
static void get_net_extrema (vector min, vector max);
static void insert_center_vector (cube c, int unit);
static bool get_size_vector (vector v, int unitNo);
static char *get_label_string (int label, int unitNo);
static void d3_labelUnit (cube c, int unitNo, int vert, int label, bool toplabel_flag);
static int get_vert_index (cube c, vector corner);
static void get_label_vert_indices (vector rot_vec, int *top_index, int *bott_index);
static void calc_transformed_cube (matrix unit_trans_mat, matrix world_center_mat,
                                   matrix unit_scale_mat, matrix unit_activ_mat,
                                   matrix world_scale_mat, matrix world_trans_mat, 
                                   vector viewpoint, cube transformed_cube);
static void draw_units (void);
static void d3_labelLink (vector v1, vector v2, float *weight);
static void draw_links (void);


static float grid_size = 37.0;

static float trans_x, trans_y;
static unsigned long linkColor = 25;
static int d3_currentDisplay = 0;


/* end private definition section */


#endif /* _D3_MAIN_DEFINED_ */


/* end of file */
/* lines: 29 */

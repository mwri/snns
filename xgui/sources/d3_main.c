/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_main.c,v $
  SHORTNAME      : main.c
  SNNS VERSION   : 4.2
 
  PURPOSE        : contains the main routine to draw the net
  NOTES          :
 
  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991
 
  CHANGED BY     : Niels Mache
  RCS VERSION    : $Revision: 2.13 $
  LAST CHANGE    : $Date: 1998/03/13 16:31:48 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>

 
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <X11/Xlib.h>

#include "ui.h"

#include "glob_typ.h"	/*  Kernel constant and type definitions  */
#include "d3_global.h"
#include "d3_draw.h"
#include "d3_anageo.h"
#include "d3_graph.h"
#include "d3_fonts.h"
#include "d3_point.h"

#include "kr_ui.h"	/*  Kernel interface functions	*/

#include "d3_zgraph.h"
#include "ui_setup.h"
#include "ui_color.h"

#include "d3_main.ph"


/*****************************************************************************
  FUNCTION : d3_getColorValue

  PURPOSE  : get the value of an unit for a selected mode
  RETURNS  : -1.0 <= value <= 1.0
  NOTES    :

  UPDATE   :
******************************************************************************/
void d3_getColorValue (int mode, int unitNo, float *value)

{
    switch (mode) {
         case activation_on :  *value = krui_getUnitActivation (unitNo);
                               break;
         case init_act_on   :  *value = krui_getUnitInitialActivation (unitNo);
                               break;
         case output_on     :  *value = krui_getUnitOutput (unitNo);
                               break;
         case bias_on       :  *value = krui_getUnitBias (unitNo);
                               break;
         default            :  *value = krui_getUnitActivation (unitNo);
                               break;
    }
    if (*value > 1.0)
        *value = 1.0;
    if (*value < -1.0)
        *value = -1.0;
}


/*****************************************************************************
  FUNCTION : get_unit_pos_vector

  PURPOSE  : calculates the 3d vector form the 2d representation
  RETURNS  : the vector
  NOTES    : this is the only interface to get a 3d vector

  UPDATE   :
******************************************************************************/
static void get_unit_pos_vector (int unit, vector v)

{
    struct PosType unit_pos;
    int  x, y;

    krui_getUnitPosition (unit, &unit_pos);
    krui_xyTransTable( OP_TRANSTABLE_GET, &x, &y, unit_pos.z );

    v[0]= (unit_pos.x - x) * grid_size;
    v[1] = (unit_pos.y - y) * grid_size;

    v[2] = unit_pos.z * grid_size;
    v[3] = 1.0;
}


/*****************************************************************************
  FUNCTION : unit_transformation

  PURPOSE  : multipies a matrix with an unit cube
  RETURNS  : the transformed cube
  NOTES    :

  UPDATE   :
******************************************************************************/
static void unit_transformation (cube c, matrix m)

{
    int i;

    for (i=0; i<ANZ_VECS; i++)
      d3_multMatrixVector (c[i], m, d3_e_cube[i]);
}


/*****************************************************************************
  FUNCTION : get_net_extrema

  PURPOSE  : calculates the dimensions of a net
  RETURNS  : two vectors containing the minimum and the maximum of the net
  NOTES    :

  UPDATE   :
******************************************************************************/
static void get_net_extrema (vector min, vector max)

{
    vector p;
    int act_unit, i;

    act_unit = krui_getFirstUnit ();
    if (act_unit != 0)
      {
        get_unit_pos_vector (act_unit, min);
        get_unit_pos_vector (act_unit, max);
        act_unit = krui_getNextUnit ();
        while (act_unit != 0)
        {
             get_unit_pos_vector (act_unit, p);
             for (i=0; i<3; i++)
               {
                 if (p[i] < min[i])
                   min[i] = p[i];
                 if (p[i] > max[i])
                   max[i] = p[i];
 	       }
             act_unit = krui_getNextUnit ();
	  }
      }
 }


/*****************************************************************************
  FUNCTION : insert_center_vector

  PURPOSE  : stores the center of a cube
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void insert_center_vector (cube c, int unit)

{
    struct PositionVector pv;

    pv.x = c[CENTER_VECTOR][0];
    pv.y = c[CENTER_VECTOR][1];
    pv.z = c[CENTER_VECTOR][2];
    krui_setUnitCenters (unit, d3_currentDisplay, &pv);
}


/*****************************************************************************
  FUNCTION : get_size_vector

  PURPOSE  : get the value of an unit to calculate the size
  RETURNS  : a vector containing the size
  NOTES    : only positive values are returned

  UPDATE   :
******************************************************************************/
static bool get_size_vector (vector v, int unitNo)

{
     float value;

     switch (d3_state.unit_mode.size)
       {
         case activation_on :  value = krui_getUnitActivation (unitNo);
                               break;
         case init_act_on   :  value = krui_getUnitInitialActivation(unitNo);
                               break;
         case output_on     :  value = krui_getUnitOutput(unitNo);
                               break;
         case bias_on       :  value = krui_getUnitBias(unitNo);
                               break;
         case nothing_on    :  value = 1.0;
                               break;
         default            :  value = 1.0;
       }
     if (value > 1.0)
       value = 1.0;
     v[0] = v[1] = v[2] = value;
     return (value >= 0.1);
}


/*****************************************************************************
  FUNCTION : get_label_string

  PURPOSE  : gets the label for an unit
  RETURNS  : the string
  NOTES    :

  UPDATE   :
******************************************************************************/
static char *get_label_string (int label, int unitNo)

{
    char *fmt_string = "%.3f", *str;
    struct PosType pos;

    str = malloc (255);
    switch (label) {
         case activation_on :  sprintf (str, fmt_string, 
                                        krui_getUnitActivation (unitNo));
                               break;
         case init_act_on   :  sprintf (str, fmt_string,
                                        krui_getUnitInitialActivation (unitNo));
                               break;
         case output_on     :  sprintf (str, fmt_string, 
                                        krui_getUnitOutput (unitNo));
                               break;
         case bias_on       :  sprintf (str, fmt_string, 
                                        krui_getUnitBias (unitNo));
                               break;
         case number_on     :  sprintf (str, "%d", unitNo);
                               break;
         case zvalue_on     :  krui_getUnitPosition (unitNo, &pos);
                               sprintf (str, "%d", pos.z);
                               break;
         case name_on       :  sprintf (str, "%s", krui_getUnitName(unitNo));  
                               if (str == NULL)
                                   sprintf (str, " ");
                               break;
         default            :  sprintf (str, "%s", "nothing");
                               break;
    }
    return (str);

}


/*****************************************************************************
  FUNCTION : d3_labelUnit

  PURPOSE  : draws a string at the specified vertex of a cube
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void d3_labelUnit (cube c, int unitNo, int vert, int label, 
			  bool toplabel_flag)

{
    int x, y;
    float z;
    char *label_string;
    
    x = c[vert][0];
    y = c[vert][1];
    z = c[vert][2];
    if (toplabel_flag)
        y = y - d3_fontYsize;
    label_string = get_label_string (label, unitNo);
    d3_draw_string (x, y, z, label_string);     
}


/*****************************************************************************
  FUNCTION : get_vert_index 

  PURPOSE  : calculates the neatest vertex for a given vector
  RETURNS  : the index in the vertex structure 
  NOTES    :

  UPDATE   :
******************************************************************************/
static int get_vert_index (cube c, vector corner)

{
    cube vert_diff;
    float length, vert_len;
    int index, i, j;

    for (i=0; i<8; i++) {
        for (j=0; j<3; j++) {
            vert_diff[i][j] = corner[j] - c[i][j];
	}
    }
    length = vert_diff[0][0]*vert_diff[0][0] + vert_diff[0][1]*vert_diff[0][1] +
             vert_diff[0][2]*vert_diff[0][2]; 
    index = 0;
    for (i=1; i<8; i++) {
        vert_len = vert_diff[i][0]*vert_diff[i][0] + 
	           vert_diff[i][1]*vert_diff[i][1] +
                   vert_diff[i][2]*vert_diff[i][2]; 
        if (vert_len < length) {
             length = vert_len;
             index = i;
	}
    }
    return (index);         
}


/*****************************************************************************
  FUNCTION : get_label_vert_indices

  PURPOSE  : gets the vertex indices of the upper right and the lower right
             vertex for a given rotation
  RETURNS  : the two indices
  NOTES    :

  UPDATE   :
******************************************************************************/
static void get_label_vert_indices (vector rot_vec, int *top_index, 
				    int *bott_index)

{
    static vector upper_corner = {0.0, -0.5, -0.5, 1.0};
    static vector lower_corner = {0.0, 0.5, -0.5, 1.0};
    cube c;

    d3_rotateCube (c, rot_vec, d3_e_cube);
    *top_index = get_vert_index (c, upper_corner);
    *bott_index = get_vert_index (c, lower_corner);
}


/*****************************************************************************
  FUNCTION : calc_transformed_cube

  PURPOSE  : multplies all matices for the viewing pipeline
  RETURNS  : the transformed cube
  NOTES    :

  UPDATE   :
******************************************************************************/
static void calc_transformed_cube (matrix unit_trans_mat, 
				   matrix world_center_mat,
                                   matrix unit_scale_mat, 
				   matrix unit_activ_mat,
                                   matrix world_scale_mat, 
				   matrix world_trans_mat, 
                                   vector viewpoint, cube transformed_cube)

{
    matrix trans_mat, scale_mat, unit_mat, world_mat, global_mat;
    cube c1, c2;
    int i,j;

    for(i=0;i<4;i++)
	for(j=0;j<ANZ_VECS;j++)
	    c1[j][i] = c2[j][i] = transformed_cube[j][i] = 0.0;

 
    d3_multMatrix (trans_mat, world_center_mat, unit_trans_mat);
    d3_multMatrix (scale_mat, unit_activ_mat, unit_scale_mat);
    d3_multMatrix (unit_mat, trans_mat, scale_mat);
    d3_multMatrix (world_mat, world_trans_mat, world_scale_mat);
    d3_multMatrix (global_mat, world_mat, unit_mat);

    unit_transformation (c1, global_mat);

    d3_rotateCube (c2, d3_state.rot_vec, c1);

    d3_shiftCube (transformed_cube, c2, trans_x, trans_y);
    if (d3_state.projection_mode == central)
       d3_projection (transformed_cube, viewpoint, transformed_cube);
}


/*****************************************************************************
  FUNCTION : draw_units

  PURPOSE  : draw all units of the net
  RETURNS  : void
  NOTES    : the centers of the the units are stored

  UPDATE   :
******************************************************************************/
static void draw_units (void)

{
    cube transformed_cube;
    vector min, max, center, p, unit_scale, activ_vec, vp, lp;
    matrix world_center_mat, world_scale_mat, world_trans_mat;
    matrix unit_scale_mat, unit_trans_mat, unit_activ_mat;
    int act_unit, i, top_vert_index, bott_vert_index, do_draw;

    d3_shiftVector (vp, d3_state.viewpoint, trans_x, trans_y);
    d3_shiftVector (lp, d3_state.light.position, trans_x, trans_y);

    get_net_extrema (min, max);
    for (i=0; i<3; i++)
      center[i] = -(max[i] - min[i]) / 2.0 - min[i];
    center[3] = 1.0;
    d3_transMatrix (world_center_mat, center);
    unit_scale[0] = unit_scale[1] = unit_scale[2] = grid_size * 
           d3_state.unit_aspect;
    d3_scaleMatrix (unit_scale_mat, unit_scale);
    d3_scaleMatrix (world_scale_mat, d3_state.scale_vec);
    d3_transMatrix (world_trans_mat, d3_state.trans_vec);

    get_label_vert_indices(d3_state.rot_vec, &top_vert_index, &bott_vert_index);

    act_unit = krui_getFirstUnit ();
    while (act_unit != 0) {
         get_unit_pos_vector (act_unit, p);
         d3_transMatrix (unit_trans_mat, p);
         do_draw = get_size_vector (activ_vec, act_unit);
         d3_scaleMatrix (unit_activ_mat, activ_vec);
         calc_transformed_cube (unit_trans_mat, world_center_mat, 
                                unit_scale_mat,
                                unit_activ_mat, world_scale_mat,
                                world_trans_mat, vp, transformed_cube);
         insert_center_vector (transformed_cube, act_unit);
         if (do_draw) {
              if (d3_state.model_mode == wire_frame)
                  d3_draw_wireframeCube (transformed_cube);
              else {
                  
                  d3_draw_solidCube (transformed_cube, vp, lp, act_unit);
	      }
              if (d3_state.unit_mode.top_label != nothing_on)
                  d3_labelUnit (transformed_cube, act_unit, top_vert_index, 
                                d3_state.unit_mode.top_label, TRUE);
              if (d3_state.unit_mode.bottom_label != nothing_on)
                  d3_labelUnit (transformed_cube, act_unit, bott_vert_index, 
                               d3_state.unit_mode.bottom_label, FALSE);
	 }
         act_unit = krui_getNextUnit ();
    }
}


/*****************************************************************************
  FUNCTION : d3_labelLink

  PURPOSE  : draws the weight of a link 
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void d3_labelLink (vector v1, vector v2, float *weight)

{
    char string[40];
    int dx, dy;
    float dz;

    dx = floor (v1[0] + (v2[0] - v1[0]) / 2);
    dy = floor (v1[1] + (v2[1] - v1[1]) / 2);
    dz = v1[2] + (v2[2] - v1[2]) / 2;
    sprintf (string, "%.3f", *weight);
    d3_setBlackColor ();
    d3_draw_string (dx, dy, dz, string);      
    d3_setColor (linkColor);
}


/*****************************************************************************
  FUNCTION : draw_links

  PURPOSE  : draws all links of the net
  RETURNS  : void
  NOTES    :

  UPDATE   : 8.3.93  Link Trigger
******************************************************************************/
static void draw_links (void)

{
    int act_unit, pred_unit;
    FlintType str;    /* old style: FlintType *str */
    struct PositionVector *start_vec, *end_vec;
    vector v1, v2;

    if (d3_state.model_mode == solid) {
         d3_setColor (linkColor);
         if (d3_state.color_mode == mono_mode) {
              d3_setBlackColor ();    
              d3_intens = 0.0;
	 }
    } else {
         d3_setBlackColor ();    
         d3_intens = 0.0;
    }

    act_unit = krui_getFirstUnit ();
    while (act_unit != 0) {
         pred_unit = krui_getFirstPredUnit (&str); 
         while (pred_unit != 0) {
              krui_getUnitCenters (act_unit, d3_currentDisplay, &start_vec);
              krui_getUnitCenters (pred_unit, d3_currentDisplay, &end_vec);
              v1[0] = start_vec->x;
              v1[1] = start_vec->y;
              v1[2] = start_vec->z;
              v1[3] = 0.0;
              v2[0] = end_vec->x;
              v2[1] = end_vec->y;
              v2[2] = end_vec->z;
              v2[3] = 0.0;

              if ((str >= d3_state.pos_link_trigger) 
                   OR (str <= -d3_state.neg_link_trigger)) { 
                  if (d3_state.model_mode == wire_frame)
                      d3_draw_wireframeLine (v1, v2);
                  else {
                      if ((d3_state.link_mode == links_color) && 
                          (d3_state.color_mode == rgb_mode))
                          d3_setLinkColor (&str);
                      if (d3_state.color_mode == mono_mode)
                          d3_setBlackColor (); 
                      d3_draw_solidLine (v1, v2);
        	  }
                  if (d3_state.link_mode == links_label)
                      d3_labelLink (v1, v2, &str);
	      }
              pred_unit = krui_getNextPredUnit (&str);
         }
         act_unit = krui_getNextUnit ();
    }
    if (d3_state.model_mode == solid)
        d3_flushPixels ();
}


/*****************************************************************************
  FUNCTION : d3_drawNet

  PURPOSE  : draws the whole net
  RETURNS  : void
  NOTES    : this routine is called by every redraw request

  UPDATE   :
******************************************************************************/
void d3_drawNet (void)

{
    if (d3_freeze)
        return;
    if (d3_displayIsReady) {
        if (d3_state.model_mode == solid)
            d3_clearZbuffer ();
        else
            d3_setBlackColor ();
        d3_clearDisplay ();   /* old style: d3_clearDispaly */
        draw_units ();
        if (d3_state.link_mode != links_off)
            draw_links ();
    }

}


/*****************************************************************************
  FUNCTION : d3_recenter_window

  PURPOSE  : calculate the new dimensions if the window is resized by the user 
  RETURNS  : new dimension values
  NOTES    : called by the X-Event Handler  

  UPDATE   :
******************************************************************************/
void d3_recenter_window (int width, int height)

{
   if ((width != d3_displayXsize) || (height != d3_displayYsize)) {
      d3_displayXsize = width;
      d3_displayYsize = height;
      trans_x = width / 2;
      trans_y = height / 2;
      d3_setClipWindow (0, 0, d3_displayXsize, d3_displayYsize);
   }
}


/*****************************************************************************
  FUNCTION : d3_resetDisplay

  PURPOSE  : set all translations, scalings and rotations to zero
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
void d3_resetDisplay (void)

{
    int i;

    if (d3_freeze)
        return;
    for (i=0; i<3; i++)
      {
         d3_state.scale_vec [i] = 1.0; 
         d3_state.trans_vec [i] = 0.0; 
         d3_state.rot_vec [i] = 0.0; 
      }
    d3_drawNet ();
}


/*****************************************************************************
  FUNCTION : d3_clear_xyTranslationTable

  PURPOSE  : clears the Translation Table for the 2d -> 3d mapping
  RETURNS  : the cleared Table
  NOTES    :

  UPDATE   :
******************************************************************************/
void d3_clear_xyTranslationTable (void)

{
    krui_xyTransTable( OP_TRANSTABLE_CLEAR, NULL, NULL, 0 );
}


/*****************************************************************************
  FUNCTION : d3_init_main

  PURPOSE  : this is the initialization for the Translation Table, the Fonts,
             the color, and the dimensions
  RETURNS  : void
  NOTES    : only called one time by ui_init

  UPDATE   :
******************************************************************************/
void d3_init_main (void)
{
    trans_x = d3_displayXsize / 2.0;
    trans_y = d3_displayYsize / 2.0;

    d3_setClipWindow (0, 0, d3_displayXsize, d3_displayYsize);

    if (ui_col_monochromeMode)
        d3_state.color_mode = mono_mode;
    else
        d3_state.color_mode = rgb_mode;
    
    d3_select_font (fnt8x14);
    d3_get_font_size (&d3_fontXsize, &d3_fontYsize);
}
/* end of file */


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_draw.ph,v $
  SHORTNAME      : draw.c
  SNNS VERSION   : 4.2

  PURPOSE        : header for draw.c
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:03 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3_DRAW_DEFINED_
#define _D3_DRAW_DEFINED_

/* begin global definition section */

/* void d3_draw_wireframeCube (float (*c)[4]); */
void d3_draw_wireframeCube (cube c);
void d3_draw_wireframeLine (float *v1, float *v2);

void d3_draw_solidLine (vector v1, vector v2);
void d3_draw_solidCube (cube c, vector vp, vector lp, int unit_no);

int d3_cube_lines [ANZ_LINES][ANZ_PKT_LINE] = {
    {0, 1},
    {0, 3},
    {1, 2},
    {3, 2},
    {4, 5},
    {4, 7},
    {5, 6},
    {7, 6},
    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7}
};

/* indices for the surface */

int d3_vertex_index[ANZ_FACETS][ANZ_PKT_FACETS] = {
    {0, 1, 2, 3},
    {0, 4, 5, 1},
    {1, 5, 6, 2},
    {7, 6, 5, 4},
    {2, 6, 7, 3},
    {0, 3, 7, 4}
};


/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif 

/* end of file */
/* lines: */

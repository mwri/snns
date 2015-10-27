/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_draw.h,v $
  SHORTNAME      : draw.c
  SNNS VERSION   : 4.2

  PURPOSE        : header for draw.c
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:02 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3_DRAW_DEFINED_
#define _D3_DRAW_DEFINED_

extern void d3_draw_wireframeCube (cube c);
extern void d3_draw_wireframeLine (vector v1, vector v2);

extern void d3_draw_solidLine (float *v1, float *v2);
extern void d3_draw_solidCube (float (*c)[4], float *vp, float *lp, int unit_no);

extern int **d3_cube_lines;

/* indices for the surface */

extern int **d3_vertex_index;


#endif 

/* end of file */
/* lines: */

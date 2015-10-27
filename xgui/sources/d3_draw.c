/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_draw.c,v $
  SHORTNAME      : draw.c
  SNNS VERSION   : 4.2

  PURPOSE        : routines for cube and line drawing
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "glob_typ.h"	/*  Kernel constant and type definitions  */
#include "kr_ui.h"	/*  Kernel interface functions	*/


#include "d3_global.h"
#include "d3_anageo.h"
#include "d3_shade.h"
#include "d3_zgraph.h"
#include "d3_graph.h"
#include "d3_point.h"
#include "d3_main.h"

#include "d3_draw.ph"

/*****************************************************************************
  FUNCTION : d3_draw_wireframeLine

  PURPOSE  : draws a link for the wireframe model
  RETURNS  : 
  NOTES    :

******************************************************************************/


void d3_draw_wireframeLine (float *v1, float *v2)
{
    int xp1, yp1, xp2, yp2;

    xp1 = floor (v1[0] + 0.5);
    yp1 = floor (v1[1] + 0.5);
    xp2 = floor (v2[0] + 0.5);
    yp2 = floor (v2[1] + 0.5);
    d3_drawLine (xp1, yp1, xp2, yp2);
}




/*****************************************************************************
  FUNCTION : d3_draw_wireframeCube

  PURPOSE  : draws a unit for the wireframe model
  RETURNS  : 
  NOTES    :

******************************************************************************/



/* void d3_draw_wireframeCube (float (*c)[4]) */
void d3_draw_wireframeCube (cube c)
{
    int i, xp1, yp1, xp2, yp2;

    for (i=0; i<ANZ_LINES; i++) {
         xp1 = c [d3_cube_lines [i][0]][0];
         yp1 = c [d3_cube_lines [i][0]][1];
         xp2 = c [d3_cube_lines [i][1]][0];
         yp2 = c [d3_cube_lines [i][1]][1];
         d3_drawLine (xp1, yp1, xp2, yp2);
    }
}





/*****************************************************************************
  FUNCTION : d3_draw_solidLine

  PURPOSE  : draws a link for the solid model
  RETURNS  : 
  NOTES    :

******************************************************************************/


void d3_draw_solidLine (vector v1, vector v2)
{
    int xp, yp, i;
    float my, my_step, dx, dy, zp;
    vector v, w;


    dx = fabs (v2[0] - v1[0]);
    dy = fabs (v2[1] - v1[1]);
    if (dx > dy)
        my_step = (dx==0)? 1.0 : 1.0/dx;
    else
        my_step = (dy==0)? 1.0 : 1.0/dy;
    for (i=0; i<4; i++)
        w[i] = v2[i] - v1[i];
    my = 0.0;
    while (fabs (my) <= 1.0) {
        for (i=0; i<4; i++)
            v[i] = v1[i] + my * w[i];
        xp = (int) (v[0]+0.5);
        yp = (int) (v[1]+0.5);
        if ((xp >= d3_clipWindow.x0) && (yp >= d3_clipWindow.y0) 
          && (yp < d3_clipWindow.y1) && (xp < d3_clipWindow.x1)) {
            d3_readZbuffer (xp, yp, &zp);
            if (v[2] < zp) {
                d3_putPixel (xp, yp);
                d3_writeZbuffer (xp, yp, v[2]);
	    }
        }
        my += my_step;
    }
}




/*****************************************************************************
  FUNCTION : d3_draw_solidCube

  PURPOSE  : draws a unit for the solid model
  RETURNS  : 
  NOTES    :
  UPDATE   : 3.3.93  shading for activation

******************************************************************************/



void d3_draw_solidCube (cube c, vector vp, vector lp, int unit_no)
{
    int pi, vi;
    d3_polygon_type p;
    unsigned long color;
    vector normal;
    float col_val;
    static long shades[6] = {0, -2, 2, 0, -2, 2};

    for (pi=0; pi<6; pi++) {
        d3_normalVector (normal, c[d3_vertex_index[pi][0]],
                                 c[d3_vertex_index[pi][1]],
                                 c[d3_vertex_index[pi][2]]);

        if (d3_state.unit_mode.color == nothing_on) {
            d3_shadeIntens (&d3_intens, c[d3_vertex_index[pi][0]], normal, lp);
            color = d3_intens_to_grayval (d3_intens);
        } else {
            d3_getColorValue (d3_state.unit_mode.color, unit_no, &col_val);
            color = (long) d3_value_to_color (col_val) + 3 + shades[pi];
        }
        d3_setColor (color);
        p.n = 4;
        for (vi=0; vi<4; vi++) {
            p.vert[vi][0] = c[d3_vertex_index[pi][vi]] [0];
            p.vert[vi][1] = c[d3_vertex_index[pi][vi]] [1];
            p.vert[vi][2] = c[d3_vertex_index[pi][vi]] [2];
        }
        d3_drawPoly (&p);
        d3_flushPixels ();
    }
}


/* end of file */
/* lines:*/







/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_graph.c,v $
  SHORTNAME      : graph.c
  SNNS VERSION   : 4.2

  PURPOSE        : routines for polygons and colors
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/13 16:31:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif
#include <math.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>

#include "ui.h"

#include "ui_mainP.h"
#include "ui_event.h"
#include "ui_color.h"
#include "d3_global.h"
#include "d3_zgraph.h"
#include "d3_point.h"

#include "d3_graph.ph"




/*****************************************************************************
  FUNCTION : incrementalize_y

  PURPOSE  : increment y
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void incrementalize_y (register float *p1, register float *p2, register float *p, register float *dp, int y, register int mask)
{
    float dy, frac;

    dy = *(*(vector *)p2+1) - *(*(vector *)p1+1);
    if (dy==0.0) dy = 1.0;
    frac = y + 0.5 - *(*(vector *)p1+1);

    for (; mask!=0; mask>>=1, p1++, p2++, p++, dp++)
        if (mask&1) {
            *dp = (*p2-*p1) / dy;
            *p = *p1 + *dp * frac;
        }
}



/*****************************************************************************
  FUNCTION : incrementalize_x

  PURPOSE  : increment x
  RETURNS  : 
  NOTES    :

******************************************************************************/



static void incrementalize_x (register float *p1, register float *p2, register float *p, register float *dp, int x, register int mask)
{
    float dx, frac;

    dx = **(vector *)p2 - **(vector *)p1;
    if (dx==0.0) dx = 1.0;
    frac = x + 0.5 - **(vector *)p1;

    for (; mask!=0; mask>>=1, p1++, p2++, p++, dp++)
        if (mask&1) {
            *dp = (*p2-*p1) / dx;
            *p = *p1 + *dp * frac;
        }
}



/*****************************************************************************
  FUNCTION : increment

  PURPOSE  : increment a masked value
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void increment (register float *p, register float *dp, register int mask)
{
    for (; mask!=0; mask>>=1, p++, dp++)
        if (mask&1)
            *p += *dp;
}




/*****************************************************************************
  FUNCTION : put_pixel

  PURPOSE  : draw a z-buffered pixel
  RETURNS  : 
  NOTES    :

******************************************************************************/



static void put_pixel (int px, int py, vector point)
{
    float zp;

    d3_readZbuffer (px, py, &zp);
    if (point[2] < zp) {
        d3_putColPixel (px, py);
        d3_writeZbuffer (px, py, point[2]);
    }
}




/*****************************************************************************
  FUNCTION : scanline

  PURPOSE  : scan a horizontal line in a polygon
  RETURNS  : 
  NOTES    :

******************************************************************************/



static void scanline (int y, vector l, vector r, int mask)
{
    int x, lx, rx;
    vector p, dp;

    mask &= ~POLY_MASK(0);
    lx = ceil ((*l) - 0.5);
    if (lx<d3_clipWindow.x0) lx = d3_clipWindow.x0;
    rx = floor ((*r) - 0.5);
    if (rx>d3_clipWindow.x1) rx = d3_clipWindow.x1;
    if (lx>rx) return;
    incrementalize_x (l, r, p, dp, lx, mask);
    for (x=lx; x<=rx; x++) {
        put_pixel (x, y, p);
        increment (p, dp, mask);
    }
}




/*****************************************************************************
  FUNCTION : d3_drawPoly

  PURPOSE  : draw a zbuffered polygon
  RETURNS  : 
  NOTES    :

******************************************************************************/



void d3_drawPoly (register d3_polygon_type *p)
{
    register int i, li, ri, y, ly, ry, top, rem, mask;
    float ymin;
    vector l, r, dl, dr;

    p->mask = POLY_MASK(0) | POLY_MASK(1) | POLY_MASK(2);

    ymin = MAXFLOAT;
    for (i=0; i<p->n; i++) {            /* find top vertex */
        if (p->vert[i][1] < ymin) {
            ymin = (p->vert[i])[1];
            top = i;
        }
    }
    li = ri = top;                     /* left and right vertex indices */
    rem = p->n;                        /* number of vertices remaining */
    y = ceil (ymin - 0.5);             /* current scanline */
    ly = ry = y-1;                     /* lower end of left & right edges */
    mask = p->mask & ~POLY_MASK(1);   /* stop interpolating screen y */

    while (rem > 0) {
         while (ly<=y && rem>0) {
             rem--;
             i = li-1;
             if (i<0) i = p->n-1;
             incrementalize_y (p->vert[li], p->vert[i], l, dl, y, mask);
             ly = floor (p->vert[i][1] + 0.5);
             li = i;
         }
         while (ry<=y && rem>0) {
             rem--;
             i = ri+1;
             if (i>=p->n) i = 0;
             incrementalize_y (p->vert[ri], p->vert[i], r, dr, y, mask);
             ry = floor (p->vert[i][1] + 0.5);
             ri = i;
         }


         while (y<ly && y<ry) {
             if (y>=d3_clipWindow.y0 && y<=d3_clipWindow.y1)
               if (*l <= *r)
                 scanline (y, l, r, mask);
               else
                 scanline (y, r, l, mask);
             y++;
             increment (l, dl, mask);
             increment (r, dr, mask);
         }
    }
}





/*****************************************************************************
  FUNCTION : d3_setClipWindow

  PURPOSE  : sets the clipping window for the polygon routines
  RETURNS  : 
  NOTES    :

******************************************************************************/



void d3_setClipWindow (int x0, int y0, int x1, int y1)
{
    d3_clipWindow.x0 = x0;
    d3_clipWindow.x1 = x1;
    d3_clipWindow.y0 = y0;
    d3_clipWindow.y1 = y1;
}




/*****************************************************************************
  FUNCTION : d3_drawLine

  PURPOSE  : draw a line
  RETURNS  : 
  NOTES    :

******************************************************************************/




void d3_drawLine (int x1, int y1, int x2, int y2)
{
    XDrawLine(d3_display, d3_window, d3_gc, x1, y1, x2 ,y2);
}




/*****************************************************************************
  FUNCTION : d3_intens_to_grayval

  PURPOSE  : converts the polygon light intensity in a palette gray value
  RETURNS  : palette index
  NOTES    :

******************************************************************************/



int d3_intens_to_grayval (float intens)
{
    return (floor ((1.0 - intens) * (float) MAXCOLSTEPS));
}





/*****************************************************************************
  FUNCTION : d3_value_to_color

  PURPOSE  : converts the polygon light intensity in a palette color value
  RETURNS  : palette index
  NOTES    : 
  UPDATE   : 3.3.93  shading for activation

******************************************************************************/


int d3_value_to_color (float value)
{
    return ((int) ((1.0 + value) * ((float) MAXCOLSTEPS - 3.0)));
}




/*****************************************************************************
  FUNCTION : d3_setColor

  PURPOSE  : sets the drawing color
  RETURNS  : 
  NOTES    :

******************************************************************************/


void d3_setColor (long unsigned int color)
{
    XSetForeground(d3_display, d3_gc, 
                   ui_col_rangePixels[color]); 
}



/*****************************************************************************
  FUNCTION : d3_setBlackColor

  PURPOSE  : sets the drawing color to black
  RETURNS  : 
  NOTES    :

******************************************************************************/



void d3_setBlackColor (void)
{
    XSetForeground(d3_display, d3_gc, 
                   BlackPixel (d3_display, d3_screen)); 
}



/*****************************************************************************
  FUNCTION : d3_setLinkColor

  PURPOSE  : calculates the link color from a value
  RETURNS  : 
  NOTES    :

******************************************************************************/



void d3_setLinkColor (float *weight)
{
    int procent_value;
    float quotient;

    if (*weight >= 0.0) {
	if (d3_state.link_scale != 0.0)
	{
	    quotient = fabs((*weight) / d3_state.link_scale);
	    if (quotient < 1.0)
		procent_value = (int)(100.0 * quotient);
	    else
		procent_value = 100;
	}
	else
	    procent_value = 100;
        XSetForeground (d3_display, d3_gc, ui_col_rangePixels[MAXCOLSTEPS + 
			procent_value * MAXCOLSTEPS / 100]);
     } else {
         if (d3_state.link_scale != 0.0)
	 {
	    quotient = fabs((*weight) / d3_state.link_scale);
	    if (quotient < 1.0)
		procent_value = (int)(100.0 * quotient);
	    else
		procent_value = 100;
	 }
	 else
	    procent_value = 100;
	 XSetForeground (d3_display, d3_gc, ui_col_rangePixels[MAXCOLSTEPS - 
			 procent_value * MAXCOLSTEPS / 100]);
     }
}	    




/*****************************************************************************
  FUNCTION : d3_clearDisplay

  PURPOSE  : clears the display window
  RETURNS  : 
  NOTES    :

******************************************************************************/


void d3_clearDisplay (void)
{
    XClearWindow (d3_display, d3_window);
}



/*****************************************************************************
  FUNCTION : d3_getRootSizes

  PURPOSE  : gets the dimensions of the root window
  RETURNS  : width and height
  NOTES    :

******************************************************************************/


void d3_getRootSizes (unsigned int *xsize, unsigned int *ysize)
{
    Window root;
    int x, y;
    unsigned int width, height, border, depth;
    Status status;

    status = XGetGeometry (ui_display, DefaultRootWindow (ui_display), &root, 
                           &x, &y, &width, &height, &border, &depth);
    *xsize = width;
    *ysize = height;
}




/* end of file */
/* lines: 512 */

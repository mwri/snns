/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_point.c,v $
  SHORTNAME      : point.c
  SNNS VERSION   : 4.2

  PURPOSE        : low level pixel IO
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:00 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "glob_typ.h"
#include "d3_global.h"
#include "d3_dither.h"


#undef BUFFERED_IO


#include "d3_point.ph"


#ifdef BUFFERED_IO


/*****************************************************************************
  FUNCTION : d3_putPixel

  PURPOSE  : draw a pixel
  RETURNS  : void
  NOTES    : the pixels are buffered in the points array

  UPDATE   :
******************************************************************************/

void d3_putPixel (int xp, int yp)

{
    points[point_count].x = (short) xp;
    points[point_count].y = (short) yp;
    point_count++;
    if (point_count == POINT_BLOCK_SIZE) {
        XDrawPoints (d3_display, d3_window, d3_gc, points, 
                     point_count, 0);
        point_count = 0;
    }
}


/*****************************************************************************
  FUNCTION : d3_flushPixels

  PURPOSE  : draw the remaining pixels
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_flushPixels (void)

{
    XDrawPoints (d3_display, d3_window, d3_gc, points, 
                 point_count, 0);
    point_count = 0;
}


#else /* BUFFERED_IO */


/*****************************************************************************
  FUNCTION : d3_putPixel

  PURPOSE  : draw a pixel
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_putPixel (int xp, int yp)

{
    XDrawPoint (d3_display, d3_window, d3_gc, xp, yp);
}


/*****************************************************************************
  FUNCTION : d3_putColPixel

  PURPOSE  : draw a color pixel
  RETURNS  : void
  NOTES    : dither if monchrome mode

  UPDATE   :
******************************************************************************/

void d3_putColPixel (int xp, int yp)

{
    unsigned long color;

    if (d3_state.color_mode == mono_mode) {
        if (dither (xp, yp, d3_intens))
            color = WhitePixel (d3_display, d3_screen);
        else
            color = BlackPixel (d3_display, d3_screen);
        XSetForeground(d3_display, d3_gc, color); 
    }
    XDrawPoint (d3_display, d3_window, d3_gc, xp, yp);
}


/*****************************************************************************
  FUNCTION : d3_flushPixels

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_flushPixels (void)

{
}


#endif /* BUFFERED_IO */



/* end of file */
/* lines: 178 */

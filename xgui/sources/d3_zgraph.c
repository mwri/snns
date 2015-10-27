/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_zgraph.c,v $
  SHORTNAME      : zgraph.c
  SNNS VERSION   : 4.2

  PURPOSE        : z-buffer for the hidden line detection
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/13 16:31:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#else
#include <math.h>
#endif
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "glob_typ.h"

#include "ui.h"
#include "d3_global.h"
#include "d3_graph.h"

#include "d3_zgraph.ph"


/*****************************************************************************
  FUNCTION : d3_initZbuffer

  PURPOSE  : init and clear z-buffer
  RETURNS  : int
  NOTES    :

  UPDATE   :
******************************************************************************/

int d3_initZbuffer (void)

{
   d3_getRootSizes ((unsigned int *) &zbuffer_xsize, (unsigned int *) &zbuffer_ysize);

   if (!zbuffer_isInit) {
        zbuffer = (float *) malloc (zbuffer_xsize * zbuffer_ysize * sizeof (float));
        if (zbuffer == NULL) {
            fprintf (stderr, "Error allocating zbuffer");
            return (FALSE);
	}
        zbuffer_isInit = TRUE;
    }
    d3_clearZbuffer ();
    return (TRUE);
}


/*****************************************************************************
  FUNCTION : d3_clearZbuffer

  PURPOSE  : clear z-buffer
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_clearZbuffer (void)

{
  float  *zbuf_ptr, *limit;

  limit = zbuffer + zbuffer_ysize * zbuffer_ysize;
  for (zbuf_ptr = zbuffer; zbuf_ptr < limit; zbuf_ptr++)
    *zbuf_ptr = MAXFLOAT;
}


/*****************************************************************************
  FUNCTION : d3_readZbuffer

  PURPOSE  : read a value out of the z-buffer
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_readZbuffer (int x, int y, float *z)

{
    if ((x >= 0) && (x < zbuffer_xsize) && (y >= 0) && (y < zbuffer_ysize))
         *z = *(zbuffer + y*zbuffer_ysize + x);
    else {
         *z = 0.0;
    }
}


/*****************************************************************************
  FUNCTION : d3_writeZbuffer

  PURPOSE  : write a value into the z-buffer
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_writeZbuffer (int x, int y, float z)

{
    if ((x >= 0) && (x < zbuffer_xsize) && (y >= 0) && (y < zbuffer_ysize))
        *(zbuffer + y*zbuffer_ysize + x) = z;
}


/*****************************************************************************
  FUNCTION : d3_freeZbuffer

  PURPOSE  : free the memory if the z-buffer is not longer used
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

void d3_freeZbuffer (void)

{
    if (zbuffer_isInit) {
        free (zbuffer);
        zbuffer_isInit = FALSE;
    }
}

/* end of file */
/* lines: 163 */

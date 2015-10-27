/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_dither.c,v $
  SHORTNAME      : dither.c
  SNNS VERSION   : 4.2

  PURPOSE        : dithering of graylevels on a monochrome display
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:49 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include "d3_dither.ph"

/*****************************************************************************
  FUNCTION : dither

  PURPOSE  : calculate the dither value
  RETURNS  : 0 if black or 1 if white
  NOTES    :

******************************************************************************/


int dither (int x, int y, float level)
{
    return (level * size*size > dither_matrix[(x % size) + size * (y % size)]);
}


/* end of file */
/* lines: 71 */


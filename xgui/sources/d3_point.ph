/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_point.ph,v $
  SHORTNAME      : point.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_point.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:35 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_POINT_DEFINED_
#define _D3_POINT_DEFINED_


/* begin global definition section */


#define POINT_BLOCK_SIZE  1024


void d3_putPixel (int, int);
void d3_putColPixel (int, int);
void d3_flushPixels (void);


/* end global definition section */


/* begin private definition section */


#ifdef BUFFERED_IO
static XPoint points[POINT_BLOCK_SIZE];
static int point_count = 0; 
#endif /* BUFFERED_IO */


/* end private definition section */


#endif /* _D3_POINT_DEFINED_ */


/* end of file */
/* lines: 26 */

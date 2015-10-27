/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_graph.h,v $
  SHORTNAME      : graph.h
  SNNS VERSION   : 4.2

  PURPOSE        : header for graph.c
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:13 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3_GRAPH_DEFINED_
#define  _D3_GRAPH_DEFINED_

extern void d3_drawPoly (register d3_polygon_type *p);
extern void d3_drawLine (int x1, int y1, int x2, int y2);

extern void d3_setClipWindow (int x0, int y0, int x1, int y1);

extern int d3_intens_to_grayval (float intens);
extern int d3_value_to_color (float value);


extern void d3_setColor (long unsigned int color);
extern void d3_setBlackColor (void);
extern void d3_setLinkColor (float *weight);

extern void d3_clearDisplay (void);
extern void d3_getRootSizes (unsigned int *xsize, unsigned int *ysize);


#endif
/* end of file */
/* lines: */

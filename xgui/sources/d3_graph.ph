/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_graph.ph,v $
  SHORTNAME      : graph.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for graph.c
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:14 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3_GRAPH_DEFINED_
#define  _D3_GRAPH_DEFINED_
/* begin global definition section */

void d3_drawPoly (register d3_polygon_type *p);
void d3_drawLine (int x1, int y1, int x2, int y2);

void d3_setClipWindow (int x0, int y0, int x1, int y1);

int d3_intens_to_grayval (float intens);
int d3_value_to_color (float value);


void d3_setColor (long unsigned int color);
void d3_setBlackColor (void);
void d3_setLinkColor (float *weight);

void d3_clearDisplay (void);
void d3_getRootSizes (unsigned int *xsize, unsigned int *ysize);


/* end global definition section */

/* begin private definition section */

#define MAXCOLSTEPS 15
#define POLY_MASK(elem)  (1 << (&(*(*poly_dummy+elem)) - (float *)poly_dummy))

static void put_pixel (int px, int py, vector point);

typedef struct {                 /* WINDOW */
     int x0, y0;                 /* xmin and ymin */
     int x1, y1;                 /* xmax and ymax */
} windowType;

static vector *poly_dummy;


/* end private definition section */

#endif 


/* end of file */
/* lines: */

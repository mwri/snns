/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_zgraph.ph,v $
  SHORTNAME      : zgraph.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_zgraph.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:53 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_ZGRAPH_DEFINED_
#define _D3_ZGRAPH_DEFINED_


/* begin global definition section */


int d3_initZbuffer (void);
void d3_clearZbuffer (void);
void d3_readZbuffer (int, int, float *);
void d3_writeZbuffer (int, int, float);
void d3_freeZbuffer (void);


extern int d3_displayXsize;
extern int d3_displayYsize;
extern bool d3_displayIsReady;


/* end global definition section */

/* begin private definition section */


static float *zbuffer;
static bool zbuffer_isInit = FALSE;
static int zbuffer_xsize;
static int zbuffer_ysize;


/* end private definition section */


#endif /* _D3_ZGRAPH_DEFINED_ */


/* end of file */
/* lines: 30 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_anageo.h,v $
  SHORTNAME      : anageo.h
  SNNS VERSION   : 4.2

  PURPOSE        : private header for d3_anageo.c
  NOTES          : all functions will be exported

  AUTHOR   	 : Ralf Huebner
  DATE     	 : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:54 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3_ANAGEO_DEFINED_
#define _D3_ANAGEO_DEFINED_

extern void d3_transMatrix (matrix, vector);
extern void d3_scaleMatrix (matrix, vector);
extern void d3_multMatrix (matrix, matrix, matrix);
extern void d3_multMatrixVector (vector, matrix, vector);
extern void d3_normalVector (vector, vector, vector, vector);
extern void d3_rotateCube (cube, vector, cube);
extern void d3_shiftCube (cube, cube, float, float);
extern void d3_shiftVector (vector, vector, float, float);
extern void d3_projection (cube, vector, cube);
extern void d3_rotateMatrix (matrix, vector);

#endif
/* end of file */


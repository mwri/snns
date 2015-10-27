/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_anageo.ph,v $
  SHORTNAME      : anageo.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header for d3_anageo.c
  NOTES          : all functions will be exported

  AUTHOR   	 : Ralf Huebner
  DATE     	 : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:55 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3_ANAGEO_DEFINED_
#define _D3_ANAGEO_DEFINED_
/* begin global definition section */

void d3_transMatrix (matrix, vector);
void d3_scaleMatrix (matrix, vector);
void d3_multMatrix (matrix, matrix, matrix);
void d3_multMatrixVector (vector, matrix, vector);
void d3_normalVector (vector, vector, vector, vector);
void d3_rotateCube (cube, vector, cube);
void d3_shiftCube (cube, cube, float, float);
void d3_shiftVector (vector, vector, float, float);
void d3_projection (cube, vector, cube);
void d3_rotateMatrix (matrix, vector);


/* end global definition section */

/* begin private definition section */

static void e_matrix (matrix m);
static void d3_rotateXmatrix (matrix, float);
static void d3_rotateYmatrix (matrix, float);
static void d3_rotateZmatrix (matrix, float);

#ifdef _UNUSED_UNCTIONS_

static void d3_showVector (vector);

#endif /* _UNUSED_FUNCTIONS_ */

/* end private definition section */

#endif
/* end of file */
/* lines:  */

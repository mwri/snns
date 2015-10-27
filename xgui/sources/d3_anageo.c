/*****************************************************************************
  File           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_anageo.c,v $
  SHORTNAME      : anageo.c 
  SNNS VERSION   : 4.2

  PURPOSE        : matrix operations and cube operations
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:47 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "glob_typ.h"
#include "d3_global.h"

#include "d3_anageo.ph"



#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : d3_showVector

  PURPOSE  : writes a vector to stdout
  RETURNS  : 
  NOTES    :

******************************************************************************/

static void d3_showVector (vector v)

{
    fprintf (stdout, "%10.5f   %10.5f   %10.5f   %10.5f\n",
             v[0], v[1], v[2], v[3]);
}

#endif /* _UNUSED_UNCTIONS_ */




/*****************************************************************************
  FUNCTION : e_matrix

  PURPOSE  : creates a unit matrix
  RETURNS  : unit matrix
  NOTES    :

******************************************************************************/

static void e_matrix (matrix m)
{
    int i, j;

    for (i=0; i<4; i++)
      {
        for (j=0; j<4; j++)
          {
            m[i][j] = 0.0;
            if (i==j)
              m[i][j] = 1.0;
          }
      }
}



/*****************************************************************************
  FUNCTION : d3_transMatrix

  PURPOSE  : creates a translation matrix
  RETURNS  : translation matrix
  NOTES    :

******************************************************************************/

void d3_transMatrix (matrix m, vector v)
{
    e_matrix (m);
    m[0][3] = v[0];
    m[1][3] = v[1];
    m[2][3] = v[2];
}




/*****************************************************************************
  FUNCTION : d3_scaleMatrix

  PURPOSE  : creates a scale matrix
  RETURNS  : scale matrix 
  NOTES    :

******************************************************************************/

void d3_scaleMatrix (matrix m, vector v)
{
    e_matrix (m);
    m[0][0] = v[0];
    m[1][1] = v[1];
    m[2][2] = v[2];
}


/*****************************************************************************
  FUNCTION : d3_rotateXmatrix

  PURPOSE  : creates a rotation matrix arround the X - axis
  RETURNS  : rotation matrix 
  NOTES    :

*****************************************************************************/

static void d3_rotateXmatrix (matrix m, float phi)

{
    float sinus, cosinus;

    e_matrix (m);
    sinus = sin (phi);
    cosinus = cos (phi);
    m[1][1] = cosinus;
    m[2][1] = sinus;
    m[1][2] = -sinus;
    m[2][2] = cosinus;
}




/*****************************************************************************
  FUNCTION : d3_rotateYmatrix

  PURPOSE  : creates a rotation matrix arround the Y - axis
  RETURNS  : rotation matrix
  NOTES    :

*****************************************************************************/

static void d3_rotateYmatrix (matrix m, float phi)
{
    float sinus, cosinus;

    e_matrix (m);
    sinus = sin (phi);
    cosinus = cos (phi);
    m[0][0] = cosinus;
    m[2][0] = sinus;
    m[0][2] = -sinus;
    m[2][2] = cosinus;
}





/*****************************************************************************
  FUNCTION : d3_rotateZmatrix

  PURPOSE  : creates a rotation matrix arround the Z - axis
  RETURNS  : rotation matrix
  NOTES    :

******************************************************************************/
static void d3_rotateZmatrix (matrix m, float phi)
{
    float sinus, cosinus;

    e_matrix (m);
    sinus = sin (phi);
    cosinus = cos (phi);
    m[0][0] = cosinus;
    m[1][0] = sinus;
    m[0][1] = -sinus;
    m[1][1] = cosinus;
}




/*****************************************************************************
  FUNCTION : d3_multMatrix

  PURPOSE  : multplies two matrices
  RETURNS  : the new matrix
  NOTES    : C = A * B

******************************************************************************/
void d3_multMatrix (matrix c, matrix a, matrix b)
{
    int i, j, k;
    float s;

    for (i=0; i<4; i++)
      for (j=0; j<4; j++)
        {
          s = 0;
          for (k=0; k<4; k++)
            s += a[i][k] * b[k][j];
          c[i][j] = s;
        };
}
        
 

/*****************************************************************************
  FUNCTION : d3_multMatrixVector

  PURPOSE  : multplies a matrix with a vector
  RETURNS  : the new vector
  NOTES    :

******************************************************************************/
void d3_multMatrixVector (vector u, matrix m, vector v)
{
    int i;

    for (i=0; i<4; i++)
      u[i] = m[i][0] * v[0] + m[i][1] * v[1] + m[i][2] * v[2] + m[i][3];
} 





/*****************************************************************************
  FUNCTION : d3_normalVector

  PURPOSE  : calculates the normal for a polygon
  RETURNS  : the normal
  NOTES    : only the first 3 vectors

******************************************************************************/
void d3_normalVector (vector nv, vector v1, vector v2, vector v3)
{
    int i;
    vector v, w;

    for (i=0; i<4; i++) {
        v[i] = v2[i] - v1[i];
        w[i] = v3[i] - v1[i];
    }
    nv[0] = v[1]*w[2] - v[2]*w[1];
    nv[1] = v[2]*w[0] - v[0]*w[2];
    nv[2] = v[0]*w[1] - v[1]*w[0];
    nv[3] = sqrt (nv[0]*nv[0] + nv[1]*nv[1] + nv[2]*nv[2]);
}




/*****************************************************************************
  FUNCTION : d3_rotateMatrix

  PURPOSE  : creates a rotation matrix arround all axes
  RETURNS  : the rotation matrix
  NOTES    :

******************************************************************************/
void d3_rotateMatrix (matrix m, vector v)
{
    matrix rot_x, rot_y, rot_z, temp;

    d3_rotateXmatrix (rot_x,(float) v[0]);
    d3_rotateYmatrix (rot_y,(float) v[1]);
    d3_rotateZmatrix (rot_z,(float) v[2]);

    d3_multMatrix (temp, rot_y, rot_x);
    d3_multMatrix (m, rot_z, temp);
}



/*****************************************************************************
  FUNCTION : d3_rotateCube

  PURPOSE  : rotates a cube arround all axes
  RETURNS  : the rotated cube
  NOTES    :

******************************************************************************/
void d3_rotateCube (cube c1, vector r, cube c2)
{
    float a, b, c, d, e, f, g, h, j;
    vector si, co;
    int i;

    for (i=0; i<3; i++)
      {
         si[i] = sin (r[i]);
         co[i] = cos (r[i]);
      }
    a = co[1] * co[2];
    b = co[1] * si[2];
    c = -si[1];
    d = si[0] * si[1] * co[2] - co[0] * si[2];
    e = si[0] * si[1] * si[2] + co[0] * co[2];
    f = si[0] * co[1];
    g = co[0] * si[1] * co[2] + si[0] * si[2];
    h = co[0] * si[1] * si[2] - si[0] * co[2];
    j = co[0] * co[1];
    for (i=0; i<ANZ_VECS; i++)
      {
        c1[i][0] = c2[i][0] * a + c2[i][1] * b + c2[i][2] * c; 
        c1[i][1] = c2[i][0] * d + c2[i][1] * e + c2[i][2] * f; 
        c1[i][2] = c2[i][0] * g + c2[i][1] * h + c2[i][2] * j;
      } 
}




/*****************************************************************************
  FUNCTION : d3_shiftCube

  PURPOSE  : shifts a cube in the upper positive quadrant of the kordsys
  RETURNS  : the new cube
  NOTES    :

******************************************************************************/
void d3_shiftCube (cube c1, cube c2, float x, float y)
{
    int i;

    for (i=0; i<ANZ_VECS; i++) {
        c1[i][0] = x + c2[i][0];
        c1[i][1] = y + c2[i][1];
        c1[i][2] = c2[i][2];
        c1[i][3] = c2[i][3];
    }
}




/*****************************************************************************
  FUNCTION : d3_shiftVector

  PURPOSE  : shifts a vector in the upper positive quadrant of the kordsys
  RETURNS  : the new cube
  NOTES    :

******************************************************************************/
void d3_shiftVector (vector v1, vector v2, float x, float y)
{
    v1[0] = x + v2[0];
    v1[1] = y + v2[1];
    v1[2] = v2[2];
    v1[3] = v2[3];
}




/*****************************************************************************
  FUNCTION : d3_projection

  PURPOSE  : central projection of a cube
  RETURNS  : the new cube 
  NOTES    :

******************************************************************************/

void d3_projection (cube c1, vector observer, cube c)
{
    float zwis;
    int i;
        
    for (i=0; i<ANZ_VECS; i++) {
        zwis = c[i][2] - observer [2];
        c1[i][0] = observer [0] - observer [2] * (c[i][0] - observer [0]) / zwis;
        c1[i][1] = observer [1] - observer [2] * (c[i][1] - observer [1]) / zwis;
    }     
}


/* end of file */
/* lines: */








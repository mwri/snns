/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_shade.c,v $
  SHORTNAME      : shade.c
  SNNS VERSION   : 4.2

  PURPOSE        : illumination of an unit
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:03 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "glob_typ.h"

#include <math.h>

#include "d3_global.h"
#include "d3_anageo.h"

#include "d3_shade.ph"


/*****************************************************************************
  FUNCTION : d3_shadeIntens

  PURPOSE  : calculate the light intensity for a given polygon
  RETURNS  : -1.0 <= intensity <= 1.0
  NOTES    : 

  UPDATE   :
******************************************************************************/

void d3_shadeIntens (float *intens, float *v1, float *normal, float *lp)

{
     vector lv;
     float cos_a;
     int i;

     for (i=0; i<3; i++)
         lv[i] = v1[i] + lp[i];
     lv[3] = sqrt (lv[0]*lv[0] + lv[1]*lv[1] + lv[2]*lv[2]);

     cos_a = (normal[0] * lv[0] + normal[1] * lv[1] + normal[2] * lv[2]) /
             (normal[3] * lv[3]);

     *intens = d3_state.light.Ia * d3_state.light.Ka;
     if (cos_a < 0.0)
         *intens -= d3_state.light.Ip * d3_state.light.Kd * cos_a;
     if (*intens > 1.0)
         *intens = 1.0;
     if (*intens < -1.0)
         *intens = -1.0;
}



/* end of file */
/* lines: 68 */

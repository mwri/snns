/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_fonts.h,v $
  SHORTNAME      : fonts.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:08 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3FONTS_DEFINED_
#define _D3FONTS_DEFINED_

extern void d3_select_font (int selected_font);
extern void d3_get_font_size (int *x, int *y);
extern void d3_draw_string (int xpos, int ypos, float zpos, char *s);

#endif 
/* end of file */
/* lines: 27 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_fonts.ph,v $
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

/* begin global definition section */

void d3_select_font (int selected_font);
void d3_get_font_size (int *x, int *y);
void d3_draw_string (int xpos, int ypos, float zpos, char *s);

/* end global definition section */

/* begin private definition section */

static void draw_char (int xpos, int ypos, unsigned char ch);
static void draw_zbuffered_char (int xpos, int ypos, float zpos, unsigned char ch);

static int width, height;
static char *font;

/* end private definition section */

#endif 
/* end of file */
/* lines: 27 */

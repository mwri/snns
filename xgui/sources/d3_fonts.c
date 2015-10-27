/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_fonts.c,v $
  SHORTNAME      : fonts.c
  SNNS VERSION   : 4.2

  PURPOSE        : font selection and character drawing
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:51 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "glob_typ.h"
#include "d3_global.h"
#include "d3_fonts.ph"
#include "d3_point.h"
#include "d3_zgraph.h"
#include "d3_graph.h"

#include "d3_font5x7.h"
#include "d3_font5x8.h"
#include "d3_font8x14.h"


/*****************************************************************************
  FUNCTION : d3_select_font

  PURPOSE  : select a font
  RETURNS  : 
  NOTES    : set width and height of the font

******************************************************************************/


void d3_select_font (int selected_font)
{
    switch (selected_font) {
       case fnt5x7:  font = font5x7; 
                     width = font5x7_width;
                     height = font5x7_height;
                     break;
       case fnt5x8:  font = font5x8; 
                     width = font5x8_width;
                     height = font5x8_height;
                     break;
       default:      font = font8x14; 
                     width = font8x14_width;
                     height = font8x14_height;
                     break;
    }

}





/*****************************************************************************
  FUNCTION : d3_get_font_size

  PURPOSE  : gets the width and height of the current font
  RETURNS  : width and height
  NOTES    :

******************************************************************************/


void d3_get_font_size (int *x, int *y)
{
    *x = width;
    *y = height;
}



/*****************************************************************************
  FUNCTION : draw_char

  PURPOSE  : draws the charcter c at (xpos, ypos)
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void draw_char (int xpos, int ypos, unsigned char ch)
{
     int xsize, ysize, index, dx, dy;
     unsigned char b;


     xsize = width - 1;
     ysize = height - 1;
     index = height * ch;

     for (dy=0; dy<=ysize; dy++) {
         b = font[index];
         for (dx=0; dx<=xsize; dx++) {
             if ((b >> (7-dx)) & 0x01)
                 d3_putPixel (xpos+dx, ypos+dy);
         }
         index++;
     }
}



/*****************************************************************************
  FUNCTION : draw_zbuffered_char

  PURPOSE  : draws the z-bufferd charcter c at (xpos, ypos, zpos)
  RETURNS  : 
  NOTES    :

******************************************************************************/



static void draw_zbuffered_char (int xpos, int ypos, float zpos, unsigned char ch)
{
     int xsize, ysize, index, dx, dy, xp, yp;
     float zp;
     unsigned char b;


     xsize = width - 1;
     ysize = height - 1;
     index = height * ch;

     for (dy=0; dy<=ysize; dy++) {
         b = font[index];
         for (dx=0; dx<=xsize; dx++) {
             if ((b >> (7-dx)) & 0x01) {
                 xp = xpos + dx;
                 yp = ypos + dy;
                 if ((xp >= d3_clipWindow.x0) && (yp >= d3_clipWindow.y0) 
                     && (yp < d3_clipWindow.y1) && (xp < d3_clipWindow.x1)) {
                     d3_readZbuffer (xp, yp, &zp);
                     if (zpos < zp) {
                         d3_putPixel(xp, yp);
                         d3_writeZbuffer (xp, yp, zpos);
		     }
                 }
	     }
         }
         index++;
     }
}



/*****************************************************************************
  FUNCTION : d3_draw_string

  PURPOSE  : draws a string
  RETURNS  : 
  NOTES    : check wether z buffering is active

******************************************************************************/



void d3_draw_string (int xpos, int ypos, float zpos, char *s)
{
    d3_setBlackColor ();
    if (d3_state.model_mode == solid)
        for (; *s!='\0'; s++) {
           draw_zbuffered_char (xpos, ypos, zpos, *s);
           xpos += width + 1;
	}
    else
        for (; *s!='\0'; s++) {
           draw_char (xpos, ypos, *s);
           xpos += width + 1;
	}
    
}



/* end of file */
/* lines: 212 */



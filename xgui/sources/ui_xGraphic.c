/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_xGraphic.c,v $
  SHORTNAME      : xGraph.c
  SNNS VERSION   : 4.2

  PURPOSE        : Interface to graphic primitives of X
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.5.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:47 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <math.h>

#include "ui.h"
#include "ui_utilP.h"
#include "ui_setup.h"
#include "ui_action.h"
#include "ui_mainP.h"

#include "ui_xGraphic.ph"


/*****************************************************************************
  FUNCTION : ui_xDrawPoint

  PURPOSE  : displays an arrow in the canvas
  RETURNS  : void
  NOTES    : coordinates will be given in canvas pixel coordinates
             the arrow ends at pixPos2

  UPDATE   :
*****************************************************************************/

void ui_xDrawPoint(Display *displ, Drawable d, GC gc, struct PosType pixPos1)

{
    XDrawPoint(displ, d, gc, pixPos1.x, pixPos1.y);
}


/*****************************************************************************
  FUNCTION : ui_xDrawLine

  PURPOSE  : displays an arrow in the canvas
  RETURNS  : void
  NOTES    : coordinates will be given in canvas pixel coordinates
             the arrow ends at pixPos2

  UPDATE   :
*****************************************************************************/

void ui_xDrawLine(Display *displ, Drawable d, GC gc, struct PosType pixPos1, struct PosType pixPos2)

{
    XDrawLine(displ, d, gc, 
	      pixPos1.x, pixPos1.y, pixPos2.x, pixPos2.y);
}


/*****************************************************************************
  FUNCTION : ui_xDrawTriangle

  PURPOSE  : displays an arrow in the canvas
  RETURNS  : void
  NOTES    : coordinates will be given in canvas pixel coordinates
             the arrow ends at pixPos2

  UPDATE   :
*****************************************************************************/

void ui_xDrawTriangle(Display *displ, Drawable d, GC gc, struct PosType pixPos1, struct PosType pixPos2)

{
    float    angleCos, angleSin;
    float    vectorX, vectorY;
    float    vectorLength;
    int      i, vtx;


    struct PosType   vlist_triangle[3];
    int              npts[1];

    npts[0] = 3;
    
    /* the coordinates of an arrow pointing in direction of the x axis */
    /* arrow length is 4 pixels, arrow width is 4 pixels */
    /* 0,0 is the endpoint of the arrow */
    vlist_triangle[0].x =   0;
    vlist_triangle[0].y =   0;
    vlist_triangle[1].x = - 4;
    vlist_triangle[1].y =  -2;
    vlist_triangle[2].x = - 4;
    vlist_triangle[2].y =   2;
    
    /* do some calculations needed to perform the graphic rotation */
    vectorX      = pixPos2.x - pixPos1.x;
    vectorY      = pixPos2.y - pixPos1.y;
    vectorLength = sqrt((vectorX * vectorX) + (vectorY * vectorY));
    
    angleCos = 1.0 * vectorX / vectorLength;
    angleSin = sqrt(1.0 - (angleCos * angleCos));
    if (vectorY < 0) angleSin = - angleSin;
    
    for (i=1; i<3; i++) { /* graphic transformation: rotation angle */
	/* Don't transform the center-point (0,0) of the arrow */
	/* store new x coordinate in vtx temporarly */
	vtx =
	    angleCos * vlist_triangle[i].x
		- angleSin * vlist_triangle[i].y;
	vlist_triangle[i].y = 
	    angleCos * vlist_triangle[i].y 
		+ angleSin * vlist_triangle[i].x;
	vlist_triangle[i].x = 
	    vtx; /* now update x coordinate too */
    }
    
    
    /* draw arrow */
    /* pw_polygon_2(ui_pw,
       pixPos2.x, pixPos2.y, 
       nbds, npts, vlist_triangle, 
       raster_op, NULL, 0,0); */
 /*   for (i=0; i<3; i++)
	XDrawLine(displ, d, gc, 
		  (vlist_triangle[i].x + pixPos2.x),
		  (vlist_triangle[i].y + pixPos2.y), 
		  (vlist_triangle[(i+1) MOD 3].x + pixPos2.x),
		  (vlist_triangle[(i+1) MOD 3].y + pixPos2.y));
 */
    for (i=1; i<3; i++)
	XDrawLine(displ, d, gc, 
		  (vlist_triangle[0].x + pixPos2.x),
		  (vlist_triangle[0].y + pixPos2.y), 
		  (vlist_triangle[i].x + pixPos2.x),
		  (vlist_triangle[i].y + pixPos2.y));
}


/*****************************************************************************
  FUNCTION : ui_xDrawBox

  PURPOSE  : draws a box in canvas, 1 pixel thick
  RETURNS  : void
  NOTES    : uses SUNVIEWS pw_vector

  UPDATE   :
*****************************************************************************/

void ui_xDrawBox(Display *displ, Drawable d, GC gc, struct PosType pixPos1, struct PosType pixPos2)

{
    int   xt1, yt1, xt2, yt2; 

    ui_utilNormalizeRect(&pixPos1,&pixPos2);
    xt1 = pixPos1.x;
    yt1 = pixPos1.y; 
    xt2 = pixPos2.x;
    yt2 = pixPos2.y;


    /* (xt1,yt1) is the upper left, (xt2,yt2) is the bottom right coordinate */

    XDrawRectangle(displ,d,gc, xt1,yt1, (unsigned int) xt2-xt1, (unsigned int) yt2-yt1);

}


/*****************************************************************************
  FUNCTION : ui_xDrawCrossBox

  PURPOSE  : draws a box with a cross inside on a canvas, 1 pixel thick
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

void ui_xDrawCrossBox(Display *displ, Drawable d, GC gc, struct PosType pixPos1, struct PosType pixPos2)

{
    int             xt1, yt1, xt2, yt2; 
    struct PosType  pos1,pos2;

    ui_utilNormalizeRect(&pixPos1,&pixPos2);
    pos1.x = pixPos1.x + 1;
    pos1.y = pixPos1.y + 1;
    pos2.x = pixPos2.x - 1;
    pos2.y = pixPos2.y - 1;

    /* draw the Box */
    ui_xDrawBox(displ, d, gc, pixPos1, pixPos2);
    ui_xDrawBox(displ, d, gc, pos1   , pos2);

    /* draw the Cross */    
    xt1 = pixPos1.x;
    yt1 = pixPos1.y; 
    xt2 = pixPos2.x;
    yt2 = pixPos2.y;

    /* (xt1,yt1) is the upper left, (xt2,yt2) is the bottom right coordinate */

    if ((xt2-xt1 >= 2) AND (yt2-yt1 >= 2)) {
	/* big enough to draw a cross */
	XDrawLine(displ, d, gc, xt1, yt1, xt2, yt2);
	XDrawLine(displ, d, gc, xt2, yt1, xt1, yt2);
    }
#ifdef DEBUG
    XFlush(ui_display);
#endif
}


/*****************************************************************************
  FUNCTION : ui_xDeleteRect

  PURPOSE  : fills a rectangular whith black or white pixel
  RETURNS  : void
  NOTES    : uses pw_writebackground

  UPDATE   :
*****************************************************************************/

void ui_xDeleteRect(Display *displ, Drawable d, GC gc, struct PosType pixPos1, struct PosType pixPos2)

{
    Dimension         width, height;

    ui_utilNormalizeRect(&pixPos1, &pixPos2);

    width  = (Dimension) pixPos2.x - pixPos1.x + 1; 
    height = (Dimension) pixPos2.y - pixPos1.y + 1;

    XFillRectangle(displ, d, gc, pixPos1.x, pixPos1.y, (unsigned int) width, (unsigned int) height);
}


/*****************************************************************************
  FUNCTION : ui_xToggleBackingStore

  PURPOSE  : toggels the the Backing Store Attibute in a X Window
  RETURNS  : void
  NOTES    : 

  UPDATE   : 23.04.1992
******************************************************************************/

void ui_xToggleBackingStore (Boolean toggle, Display *display, Window window)

{
    XSetWindowAttributes wa;

#ifndef UI_NO_BACKING_STORE
    if (toggle == TRUE) {
        wa.backing_store = Always;
        wa.save_under    = TRUE;
    } else {
        wa.backing_store = NotUseful;
        wa.save_under    = FALSE;
    }
    XChangeWindowAttributes(display, window,
                            CWBackingStore | CWSaveUnder, &wa);
#endif       
}






/* end of file */
/* lines: 270 */

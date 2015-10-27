/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_xGraphic.ph,v $
  SHORTNAME      : xGraph.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.51990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:53 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_XGRAPHIC_DEFINED_
#define _UI_XGRAPHIC_DEFINED_


/* begin global definition section */


void   ui_xDeleteRect(Display *displ, Drawable d, GC gc, 
		struct PosType pixPos1, struct PosType pixPos2); 
void   ui_xDrawBox(Display *displ, Drawable d, GC gc, struct PosType pixPos1, 
		struct PosType pixPos2); 
void   ui_xDrawCrossBox(Display *displ, Drawable d, GC gc, 
		struct PosType pixPos1, struct PosType pixPos2);
void   ui_xDrawLine(Display *displ, Drawable d, GC gc, struct PosType pixPos1, 
		struct PosType pixPos2); 
void   ui_xDrawTriangle(Display *displ, Drawable d, GC gc, 
		struct PosType pixPos1, struct PosType pixPos2);
void   ui_xDrawPoint(Display *displ, Drawable d, GC gc, struct PosType pixPos1);
void   ui_xToggleBackingStore (Boolean toggle, Display *display, Window window);


/* end global definition section */


/* begin private definition section */


/* end private definition section */


#endif /* _UI_XGRAPHIC_DEFINED_ */


/* end of file */
/* lines: 23 */

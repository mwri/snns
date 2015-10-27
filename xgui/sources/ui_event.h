/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_event.h,v $
  SHORTNAME      : event.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 5.6.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:43 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_EVENT_DEFINED_
#define _UI_EVENT_DEFINED_

extern void ui_mw_eventProc (Widget, struct Ui_DisplayType *, XEvent *);
extern void ui_can_MapEventProc (Widget,  struct Ui_DisplayType *, XEvent *);

extern unsigned long   *ui_col_mainCells[];
extern struct PosType  ui_pixPosMouse, ui_gridPosMouse; /* current mouse 
			  				   position */


#endif /* _UI_EVENT_DEFINED_ */

/* end of file */
/* lines: 38 */

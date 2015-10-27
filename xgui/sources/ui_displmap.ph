/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_displmap.ph,v $
  SHORTNAME      : ui_displmap
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Christian Wehrfritz
  DATE           : 20.05.94

  CHANGED BY     : Guenter Mamier 
  RCS VERSION    : $Revision: 1.10 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:34 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _UI_DISPLMAP_DEFINED_
#define _UI_DISPLMAP_DEFINED_

/* begin global definition section */
void         ui_displMap(Widget w, caddr_t call_data);
void         ui_displMapFromUpdate(void);

/* end global definition section */

/* begin private definition section */

/* required functions that are declared here */
static void       ui_displMapBW(void);
static void       ui_displMapCOL(void);
static void       ui_map_close(Widget dummy, Widget w, caddr_t call_data);
static void       ui_map_setup(Widget button, caddr_t call_data);
static void       ui_map_scale(Widget w);
static void       ui_map_event(Widget w, Display *display, XEvent *event);
static void       ui_map_close_setup(Widget dummy,Widget w,caddr_t call_data);
static void       ui_map_zoom_in(Widget w);
static void       ui_map_zoom_out(Widget w);
static void       ui_map_grid(int type);
static void       ui_map_geometry(int zoom_fact);

/* local  variables for Xlib */
static GC         MAP_GRAPH_gc[21];        /* local grapic context   */
static Window     MAP_GRAPH_win;           /* w. where func is drawn */
static int        MAP_GRAPH_screen;        /* screen for drawwindow  */


/* global variables for this file */
static Widget     MAP_GRAPH_displaywidget,
                  MAP_GRAPH_mainwidget;
static int        MAP_WINDOW_created;
static int        MAP_squaresizeX = 6;
static int        MAP_squaresizeY = 6;
static int        MAP_BW_squaresizeX = 16;
static int        MAP_BW_squaresizeY = 16;
static int        MAP_windowsizeX = 400;
static int        MAP_windowsizeY = 400;
static int        MAP_viewsizeX   = 400;
static int        MAP_viewsizeY   = 400;
static int        MAP_gridWidth;
static int        MAP_xUnit=0,MAP_yUnit=0,MAP_outputUnit=0;
static float      MAP_minWght=0;
static float      MAP_maxWght=1;
static float      MAP_scaleX=0.02;
static float      MAP_scaleY=0.02;
static float      MAP_xMin=0,MAP_xMax=1,MAP_yMin=0,MAP_yMax=1;
static Widget     out_min,out_max,out_dia,scale,scale2,view;
static Widget     x_dia,y_dia,x_min,x_max,y_min,y_max;

/* end private definition section */
#endif

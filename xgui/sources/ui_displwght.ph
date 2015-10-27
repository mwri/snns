/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_displwght.ph,v $
  SHORTNAME      : ui_displweights
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Guenter Mamier 
  DATE           : 14.01.92 

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/04/20 11:55:24 $

    Copyright (c) 1990-1998  SNNS Group, IPVR, Univ. Stuttgart, Germany

******************************************************************************/

#ifndef _UI_DISPLWGHT_DEFINED_
#define _UI_DISPLWGHT_DEFINED_

/* begin global definition section */
void         ui_displWeights(Widget w, caddr_t call_data);
void         ui_displWeightsFromUpdate(void);

/* end global definition section */

/* begin private definition section */

/* required functions that are declared here */
static void       ui_displWeightsBW(void);
static void       ui_displWeightsCOL(void);
static void       ui_wght_close(Widget dummy, Widget w, caddr_t call_data);
static void       ui_wght_info(Widget w, Display *display, XEvent *event);
static void       ui_wght_setup(Widget button, caddr_t call_data);
static void       ui_wght_scale(Widget w);
static void       ui_wght_event(Widget w, Display *display, XEvent *event);
static void       ui_wght_close_setup(Widget dummy,Widget w,caddr_t call_data);
static void       ui_wght_zoom_in(Widget w);
static void       ui_wght_zoom_out(Widget w);
static void       ui_wght_grid(int type);
static void       ui_wght_geometry(int zoom_fact);


/* global variables for this file */
static GC         dwght_gc;            /* local grapic context   */
static Window     dwght_win;           /* window where func is drawn */
static Pixmap     dwght_pix;           /* pixmap where func is drawn */
static int        dwght_depth;         /* color depth of pixmap */
static int        dwght_screen;        /* screen for window  */
static Widget     WEIGHT_GRAPH_displaywidget,
                  WEIGHT_GRAPH_mainwidget;
static int        WEIGHT_WINDOW_created;
static int        WEIGHT_squaresize = 0;
static int        HINTON_squaresize = 16;
static int        WEIGHT_windowsize = 400;
static int        WEIGHT_viewsize   = 400;
static int        WEIGHT_gridWidth;
static int        maxunits = 0;
static int        old_maxunits = 0;
static float      minWght;
static float      maxWght;
static Widget     bottomLabel,topLabel,grid_dia,scale,scale2,view;

/* end private definition section */

#endif




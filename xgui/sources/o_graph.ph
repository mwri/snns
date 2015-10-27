/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/o_graph.ph,v $
  SHORTNAME      : o_graph.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for o_graph.c
  NOTES          :

  AUTHOR         : Markus Heuttel and Michael Schmalzl
  DATE           : 3.4.1992

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:59 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _O_GRAPH_DEFINED_
#define _O_GRAPH_DEFINED_


/* begin global definition section */


#define MAX_CURVE_NO 50 

/* definition of output type */
#define GRAPH_SSE         1
#define GRAPH_MSE         2
#define GRAPH_SSE_DIV_OUT 3


void o_createOszi (void);
void o_InitCurve (void);
void o_draw (struct ErrorValuesType, struct ErrorValuesType, int drawTest);


typedef struct {
          double ScreenEinheit;
          double Einheit;
        } SCALE_VALUE_TYPE;

typedef struct {
          unsigned long  x;
          double y;
	} MPoint;


int           o_open=0; 	/* flag, which indicates whether graf 
				is open or not */
int           o_PressPossible=1; /* flag : while drawing, pressing a 
				button has no effect */

long unsigned o_LearnStepCount=0; /* counts the learn steps */

short   o_CurveLengths[MAX_CURVE_NO]; /* array with the lengths of the 
				curves Curve1 and Curve2 */
int     o_CurveNo; 		/* the number of visible curves on the graf */


/* end global definition section */


/* begin private definition section */


static void o_ClearCurves (void);
static void o_ClearPixmap (Display *,Drawable,GC,uint,int,int,int,int,int);
static void o_DoneProc (void);
static void o_ResizeOszi (int, int);
static void o_ScaleY (Display *, Drawable, GC);
static void o_ScaleX (Display *, Drawable, GC);
static void o_XBackProc (void);
static void o_XForwardProc (void);
static void o_XScaleCurve (void);
static void o_YBackProc (void);
static void o_YForwardProc (void);
static void o_YScaleCurve (void);
static void o_clearProc (void);
static void o_printProc (void);
static void o_gridProc (void);
static void o_eventProc (Widget, Display *, XEvent *);
static void o_init (void);
static Widget o_xCreateScreenItem (char *, Widget, Dimension, Dimension, Widget,
				   Widget);
static void o_set_err_scale(Widget w, int type, caddr_t call_data);


static double  o_AbsoluteScale; 	/* scale factor for the error (y-value)*/
static XPoint *o_Curve1[MAX_CURVE_NO]; /* contains the pixel positons of 
					  the curves */
static MPoint *o_Curve2[MAX_CURVE_NO]; /* contains the coordinates 
					  (o_LearnStepCounter,learn_error) of 
					  the curves */
static Widget o_DisplayWidget;
static int    o_DrawAllowed; 	/* flag, which indicates whether drawing 
				   is allowed or not */
static int    o_LearningStepsOfOnePixel; /* One learning step corresponds with 
					    at least one pixel. For example: 
					    if you want see 1000 learning steps
					    and oszi has a width of 100 then
					    o_LearningStepsOfOnePixel = 10. 
					    If o_PixelsOfOneLearningStep==1 then
					    o_LearningStepsOfOnePixel==1. 
					    To distinguish the variables 
					    o_LearningStepsOfOnePixel is set 
					    to 0 */ 
static int    o_PixelsOfOneLearningStep; /* One Pixel corresponds with at least 
					    one learningstep. For example: if
					    the graph has a width of 100 Pixels
					    and you want see only 10 learning
					    steps, 
					    then o_PixelsOfOneLearningStep=10.
					    The minimum value of
					    o_PixelsOfOneLearningStep is 1 */
static short  o_MaxCurveLengths;     /* the maximum length of the current curve 
					This value can change by resizing */
static int          o_MaxYDrawPos;
static int          o_OsziFrameXPos, o_OsziFrameYPos, o_OsziFrameWidth;
static int          o_OsziFrameHeight;
static int          o_OsziXPos, o_OsziYPos, o_OsziHeight, o_OsziWidth;
static Pixmap       o_Pixmap;
static int          o_PixmapWidth, o_PixmapHeight;
static int          o_RightOffset;
static int          o_SpaceAbove=20, 
                    o_SpaceBelow=30, 
                    o_SpaceLeft=60, 
                    o_SpaceRight=30;
static int          o_WindowWidth;
static int          o_WindowHeight;
static int          o_XCounter=9;       /* indicates the position in the 
					   array o_XScaleValues */
static int          o_YCounter=3; 	/* indicates the position in the 
					   array o_YScaleValues */
static SCALE_VALUE_TYPE o_XScaleValues[15]={{50,50000}, {50,25000}, {50,10000},
				{50,5000}, {50,2500}, {50,1000}, {50,500},
				{50,250}, {50,100}, {50,50}, {50,25},
                                {50,10}, {50,5}, {50,2}, {50,1}}; 
static SCALE_VALUE_TYPE o_YScaleValues[15]={{20,0.01}, {20,0.02}, {20,0.05}, 
				{20,0.1}, {20,0.2}, {20,0.5}, {20,1},
				{20,2}, {20,5}, {20,10}, {20,20},
                                {20,50}, {20,100}, {20,200}, {20,500}};
static short        o_depth;
static Display      *o_display;
static Widget       o_displayMainWidget;
static Widget       grid_oszi;
static Widget       o_printW;
static int          oszi_drawgrid;
static Widget       o_scaleWidget;
static XFontStruct  *o_fontStruct;
static Window       o_window;
static XColor       fg, fg2;
static GC           o_gc[3];
static int          label_gc=0;
static int          train_gc=1;
static int          test_gc=2;
static int          o_screen;
static int          o_graph_error_scale;
static char         o_printfile[256];
/* end private definition section */


#endif /* _O_GRAPH_DEFINED_ */


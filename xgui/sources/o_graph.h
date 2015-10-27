/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/o_graph.h,v $
  SHORTNAME      : o_graph.h
  SNNS VERSION   : 4.2

  PURPOSE        : header for o_graph.c
  NOTES          :

  AUTHOR         : Markus Heuttel and Michael Schmalzl
  DATE           : 3.4.1992

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _O_GRAPH_DEFINED_
#define _O_GRAPH_DEFINED_



#define MAX_CURVE_NO 50 


extern void o_createOszi (void);
extern void o_InitCurve (void);
extern void o_draw (struct ErrorValuesType net_error,
		    struct ErrorValuesType test_net_error,
		    int drawTest);


extern Display *ui_display;
extern Widget   ui_toplevel;


extern SCALE_VALUE_TYPE;

extern MPoint;


extern int           o_open; 	/* flag, which indicates whether graf 
				is open or not */
extern int           o_PressPossible; /* flag : while drawing, pressing a 
				button has no effect */

extern long unsigned o_LearnStepCount; /* counts the learn steps */

extern short   o_CurveLengths[]; /* array with the lengths of the 
				curves Curve1 and Curve2 */
extern int     o_CurveNo; 		/* the number of visible curves on the graf */






#endif /* _O_GRAPH_DEFINED_ */




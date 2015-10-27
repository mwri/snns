/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_funcdispl.ph,v $
  SHORTNAME      : funcdispl
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Guenter Mamier 
  DATE           : 14.01.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:52 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _UI_FUNCDISPL_DEFINED_
#define _UI_FUNCDISPL_DEFINED_


/* begin global definition section */

/* required functions that are declared here */
void                ui_functionPopup(Widget w, char *title, caddr_t call_data);
void                ui_drawFunction(Widget w, char *title, caddr_t call_data);
Widget              ui_createMyScreen(char *name, Widget parent, Dimension width, Dimension height, Widget left, Widget top);

/* end global definition section */

/* begin private definition section */


typedef FlintType (*ActFunctionPtr) (struct Unit *);
typedef FlintType (*OutFunctionPtr) (FlintType);
	/* Old Style : typedef FlintType (*FunctionPtr) (...); */

typedef struct chain_el{
    FlintType weight;
    struct chain_el *next;
}chain_el;
static struct chain_el *weight_chain;
static struct chain_el *handle;


static Widget displaywidget[6];

/* global variables for this file */
static int FUNC_GRAPH_win_ex[6];
static int FUNC_GRAPH_created[6];
static int FUNC_GRAPH_case;
static Widget FUNC_GRAPH_x_minW[6],FUNC_GRAPH_x_labW[6],FUNC_GRAPH_x_maxW[6];
static Widget FUNC_GRAPH_y_minW[6],FUNC_GRAPH_y_labW[6],FUNC_GRAPH_y_maxW[6];
static float  FUNC_GRAPH_x_min[6],FUNC_GRAPH_x_max[6];
static float  FUNC_GRAPH_y_min[6],FUNC_GRAPH_y_max[6];
static GC                    FUNC_GRAPH_gc[6];     /* local grapic context          */
static Window                FUNC_GRAPH_window[6]; /* w. where func is drawn        */
static int                   FUNC_GRAPH_screen;    /* screen for FUNC_GRAPH_window  */

static void                ui_setXY(Widget w, caddr_t call_data);
static void                local_kill(Widget dummy, Widget w, caddr_t call_data);
static void                local_kill_prot(Widget dummy, int thiscase, caddr_t call_data);

/* end private definition section */


#endif
















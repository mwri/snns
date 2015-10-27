/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_inversion.ph,v $
  SHORTNAME      : inversion.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Guenter Mamier 
  DATE           : 29.01.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:01 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _UI_INVERSION_DEFINED_
#define _UI_INVERSION_DEFINED_

/* begin global definition section */

void ui_inversion(Widget button, caddr_t call_data);
int INVERS_CREATED = 0;               /* TRUE if display exists      */
Widget ui_InvRootWidget;              /* root widget for inversion   */

/* end global definition section */


/* begin private definition section */

/* required functions that are declared here */
static void ui_start_inversion(Widget button, caddr_t call_data),
            ui_stop_inversion(Widget button, caddr_t call_data),
            ui_closeDisplay(Widget w, caddr_t call_data),
            ui_invNew(Widget w, caddr_t call_data),
            ui_drawInput(void),
            ui_invSetup(Widget button, caddr_t call_data),
            ui_invEvent(Widget w, Display *display, XEvent *event),
            ui_invHelp(Widget button, Boolean fromMain, caddr_t call_data),
            ui_inv_helpDone(Widget button, caddr_t call_data),
            ui_inv_popupDone(Widget button, caddr_t call_data);
static Boolean ui_do_inversion(Widget button, caddr_t call_data);
static int  ui_set_IO_units(void);

/* global variables for this file */

static struct UnitList *inputs,              /* handle for input units      */
                       *outputs;             /* handle for output units     */

static Widget          ui_invpop,            /* setup widget               */
                       inPatW,               /* dialog widget for input pat*/
                       etaW,                 /*   "      "    for eta      */
                       delta_maxW,           /*   "      "    for delta    */
                       ratioW;               /*   "      "    for 2nd appr.*/

static int      INV_NEW        = 1;          /* TRUE for restart            */
static int      INV_RUNNING    = 0;          /* TRUE while inv loops        */
static int      INV_cycle      = 0;          /* counts the inv. cycles      */
static int      INV_units      = 0;          /* counts faulty output units  */
static double   INV_error;                   /* counts net inv. error       */
static float    INPUT_PAT      = 0.0;        /* stores UnitList.i_act       */
static float    RATIO          = 0.5;        /* stores 2nd aprox. ratio     */  
static float    INV_delta_max  = 0.1;        /* maximum output unit error   */
static float    INV_eta        = 2.0;        /* learn parameter             */
static XtWorkProcId  ui_workProcId = (XtWorkProcId)NULL; 
						   /* work id of inversion   */
static FlagType      ui_workType;                  /* not used here          */

/* variables for Xlib */
static int                    inv_screen;    /* X screen                   */
static Display                *inv_display;  /* actual "setenv DISPLAY"    */

static struct Ui_DisplayType *displayPtr;

/* end private definition section */





#endif

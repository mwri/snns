/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/net_analyzer.ph,v $
  SHORTNAME      : net_analyzer.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header file for corresponding '.c' file
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:56 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _NET_ANALYZER_DEFINED_
#define _NET_ANALYZER_DEFINED_

/* begin global definition section */

void NA_OpenNetworkAnalyzer   (Widget    w, 
                                      XtPointer client_data,
                                      XtPointer call_data) ;
Bool NA_NetworkAnalyzerIsOpen (void) ;
void NA_DrawNextPoint         (void) ;
void NA_StopTest              (void) ;
Bool NA_ContinueTest          (void) ;


/* end global definition section */

/* begin private definition section */

#define NA_GRAPH_XY       1
#define NA_GRAPH_TY       2
#define NA_GRAPH_TE       3

#define NA_DRAW_WINDOW    1
#define NA_DRAW_PIXMAP    2

#define INFO_BORDER      25

/* types for the network-analyzer tool
*/


typedef struct
{
  unsigned int x, y, w, h ;
} Rectangle ;


typedef struct
{
  bool  on, line, grid, record ;
} Toggles ;


typedef struct
{
  float  min, max, factor ;
  int    unit_no, grid ;
  bool   out ;
} AxisAttributes ;


typedef struct
{
  int  time, i ;
  bool no_data ;
  bool wrapped ;
  float x[MAX_ARRAY_SIZE],
        y[MAX_ARRAY_SIZE] ;
  bool  lineflag[MAX_ARRAY_SIZE] ;
} GraphData ;


typedef struct 
{
  int             graphtype ;
  AxisAttributes  hor     ; 
  AxisAttributes  ver     ;
  int             error   ;
  int             error_unit ;
  bool            ave     ;
  int             period  ;
  int             m_test  ;
  char            rec_filename[ 80 ] ;
  FILE           *rec_file       ;

  Rectangle       window, pixmap, graph ;
  Toggles         toggles ;
  GraphData       gd ;
} NaStateType ;


static NaStateType  na, se ;
static int          na_Open      =    0 ;
static bool         na_FirstCall = TRUE ;

static Pixmap       na_Pixmap       ;
static Display     *na_Display      ;
static XFontStruct *na_FontStruct   ;
static Window       na_Window       ;
static GC           na_GC           ;
static int          na_Screen       ;
static short        na_Depth        ;
static char         info_line[1024] ;
static Dimension    na_Width, na_Height ;

static Widget NA_CreateButtonItem (char   *name, Widget parent, 
                                   Widget  left, Widget top   ) ;
static Widget NA_CreateToggleItem (char   *name,  Widget parent,
                                   Widget  group, Widget left, Widget top) ;


/* ---------  network analyzer  ------------ */

static Widget w_NetworkAnalyzer, w_Window, w_Screen, w_Buttons ;
static Widget b_Clear, b_Setup, b_Done, t_MTest ;
static Widget t_On, t_Line, t_Grid, t_Record, b_DCtrl ;

static int          no_of_m_test ;

static void NA_OpenRecordFile    (void) ;
static void NA_CloseRecordFile   (void) ;
static void NA_WriteToRecordFile (char *str) ; 

static void NA_DrawCS        (void) ;
static void NA_DrawInfo      (void) ;
static void NA_DrawPoint     (int i2, int mode) ;
static void NA_DrawGraph     (void) ;
static void NA_DrawGrid      (void) ;
static void NA_ClearGraph    (void) ;
static void NA_RedrawPixmap  (void) ;
static void NA_ShowPixmap    (void) ;
static void NA_ResizeWindow  (void) ;
static void NA_EventProc     (Widget w, Display *display, XEvent *event) ;
static void NA_CallbackProc  (Widget w, XtPointer client_data,
                                        XtPointer call_data) ;

static void NA_CreateNetworkAnalyzerWidget (void) ;
static void NA_ResetNetworkAnalyzer        (void) ;



/* ----------- setup functions ------------ */

static Widget w_Setup , w_box ;
static Widget t_XY    , t_TY    , t_TE  ;  
static Widget t_Lin   , t_Sqr   , t_Su, t_Ave ;
static Widget b_Cancel, b_DoneS ;
static Widget w_HorMin, w_HorMax, w_HorUnit, w_HorGrid ;
static Widget w_VerMin, w_VerMax, w_VerUnit, w_VerGrid ;
static Widget t_HorOut, t_HorAct, t_VerOut , t_VerAct  ;
static Widget w_Period, w_MSteps, w_RecFile, w_Su      ;

static void NA_SetupGetValues    (void) ;
static void NA_SetupSetValues    (void) ;
static void NA_SetupCallbackProc (Widget w, 
                                  XtPointer client_data,
                                  XtPointer call_data) ;
static void NA_CreateSetupWidget (int graphtype) ;
static void NA_OpenSetup         (Widget w,
                                  XtPointer client_data,
                                  XtPointer call_data) ;


/* ----------- display control ------------ */

static Widget w_DisplayControl ;
static Widget b_HorHalf , b_HorDouble ;
static Widget b_HorLeft , b_HorLLeft  ;
static Widget b_HorRight, b_HorRRight ;
static Widget b_VerHalf , b_VerDouble ;
static Widget b_VerUp   , b_VerUUp    ;
static Widget b_VerDown , b_VerDDown  ;
static Widget b_DoneDC ;

static int    na_DisplayControlOpen ;


static void NA_DControlCallbackProc (Widget w, 
                                     XtPointer client_data,
                                     XtPointer call_data) ;
static void NA_OpenDisplayControl   (Widget w,
                                     XtPointer client_data,
                                     XtPointer call_data) ;

/* end private definition section */

#endif


/*****************************************************************************
                           E N D   O F   F I L E
******************************************************************************/








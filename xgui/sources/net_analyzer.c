/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/net_analyzer.c,v $
  SHORTNAME      : net_analyzer.c 
  SNNS VERSION   : 4.2

  PURPOSE        : a tool for analyzing neural networks
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : Michael Vogt, Guenter Mamier, Christine Bagdi, Sven Doering
  RCS VERSION    : $Revision: 2.18 $ 
  LAST CHANGE    : $Date: 1998/03/13 16:31:51 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Grip.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Scrollbar.h>

#include "kr_typ.h"
#include "kr_const.h"
#include "kr_def.h"
#include "kr_mac.h"
#include "kernel.h"

#include "ui.h"
#include "ui_xWidgets.h"
#include "ui_textP.h"
#include "ui_main.h"
#include "ui_mainP.h"
#include "kr_ui.h"
#include "ui_confirmer.h"
#include "ui_control.h"
#include "ui_controlP.h"
#include "ui_key.h"
 
#include "on2.xbm"
#include "line.xbm"
#include "grid.xbm"
#include "clear.xbm"
#include "m_test.xbm"
#include "record.xbm"
#include "setup.xbm"
#include "done2.xbm"

#include "xy.xbm"
#include "ty.xbm"
#include "te.xbm"
#include "e_lin.xbm"
#include "e_sqr.xbm"
#include "e_su.xbm"
#include "ave.xbm"
#include "act.xbm"
#include "out.xbm"
#include "cancel.xbm"

#include "dctrl.xbm"
#include "half.xbm"
#include "double.xbm"
#include "left.xbm"
#include "lleft.xbm"
#include "right.xbm"
#include "rright.xbm"
#include "up2.xbm"
#include "uup.xbm"
#include "down2.xbm"
#include "ddown.xbm"

#include "net_analyzer.ph"



/*****************************************************************************
  FUNCTION : NA_CreateButtonItem

  PURPOSE  : creates a button item
  RETURNS  : button widget
  NOTES    :

  UPDATE   : 
******************************************************************************/

static Widget NA_CreateButtonItem (char   *name,
                                 Widget  parent, Widget left, Widget top)

{
  Cardinal  n ;
  Arg       arg[15] ;


  n=0 ;

  if (strcmp (name, "clear") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), clear_bits, clear_width,
              clear_height)) ; n++ ;
  }

  if (strcmp (name, "m_test") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), m_test_bits, m_test_width,
              m_test_height)) ; n++ ;
  }

  if (strcmp (name, "setup") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), setup_bits, setup_width,
              setup_height)) ; n++ ;
  }

  if (strcmp (name, "done2") == 0)
  {
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), done2_bits, done2_width, 
              done2_height)) ; n++ ;
  }

  if (strcmp (name, "cancel") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), cancel_bits, cancel_width,
              cancel_height)) ; n++ ;
  }

  if (strcmp (name, "dctrl") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), dctrl_bits, dctrl_width,
              dctrl_height)) ; n++ ;
  }

  if (strcmp (name, "half") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), half_bits, half_width,
              half_height)) ; n++ ;
  }

  if (strcmp (name, "double") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), double_bits, double_width,
              double_height)) ; n++ ;
  }

  if (strcmp (name, "left") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), left_bits, left_width,
              left_height)) ; n++ ;
  }

  if (strcmp (name, "lleft") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), lleft_bits, lleft_width,
              lleft_height)) ; n++ ;
  }

  if (strcmp (name, "right") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), right_bits, right_width,
              right_height)) ; n++ ;
  }

  if (strcmp (name, "rright") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), rright_bits, rright_width,
              rright_height)) ; n++ ;
  }

  if (strcmp (name, "up2") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), up2_bits, up2_width,
              up2_height)) ; n++ ;
  }

  if (strcmp (name, "uup") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), uup_bits, uup_width,
              uup_height)) ; n++ ;
  }

  if (strcmp (name, "down2") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), down2_bits, down2_width,
              down2_height)) ; n++ ;
  }

  if (strcmp (name, "ddown") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), ddown_bits, ddown_width,
              ddown_height)) ; n++ ;
  }

  XtSetArg (arg[n], XtNborderWidth   , 0          ) ; n++ ;
  XtSetArg (arg[n], XtNinternalHeight, 1          ) ; n++ ;
  XtSetArg (arg[n], XtNinternalWidth , 1          ) ; n++ ;
  XtSetArg (arg[n], XtNfromVert      , top        ) ; n++ ;
  XtSetArg (arg[n], XtNfromHoriz     , left       ) ; n++ ;
  XtSetArg (arg[n], XtNleft          , XtChainLeft) ; n++ ; 
  XtSetArg (arg[n], XtNright         , XtChainLeft) ; n++ ;
  XtSetArg (arg[n], XtNtop           , XtChainTop ) ; n++ ; 
  XtSetArg (arg[n], XtNbottom        , XtChainTop ) ; n++ ;

  return (XtCreateManagedWidget (name, commandWidgetClass, parent, arg, n)) ;
}



/*****************************************************************************
  FUNCTION : NA_CreateToggleItem

  PURPOSE  : creates a toggle item
  RETURNS  : toggle widget 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static Widget NA_CreateToggleItem (char *name, Widget parent, Widget group, 
                                   Widget left, Widget top)

{ 
  Cardinal  n ; 
  Arg       arg[15] ;


  n=0 ;

  if (strcmp (name, "on2") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display, 
              XDefaultRootWindow (ui_display), on2_bits, on2_width, 
              on2_height)) ; n++ ; 
  }

  if (strcmp (name, "line") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display, 
              XDefaultRootWindow (ui_display), line_bits, line_width, 
              line_height)) ; n++ ; 
  }

  if (strcmp (name, "grid") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), grid_bits, grid_width, 
              grid_height)) ; n++ ; 
  }

  if (strcmp (name, "m_test") == 0)
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), m_test_bits, m_test_width,
              m_test_height)) ; n++ ;
  }

  if (strcmp (name, "record") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), record_bits, record_width, 
              record_height)) ; n++ ; 
  }

  if (strcmp (name, "xy") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), xy_bits, xy_width, 
              xy_height)) ; n++ ; 
  }

  if (strcmp (name, "ty") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), ty_bits, ty_width, 
              ty_height)) ; n++ ; 
  }

  if (strcmp (name, "te") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), te_bits, te_width, 
              te_height)) ; n++ ; 
  }

  if (strcmp (name, "e_lin") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), e_lin_bits, e_lin_width, 
              e_lin_height)) ; n++ ; 
  }

  if (strcmp (name, "e_sqr") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), e_sqr_bits, e_sqr_width, 
              e_sqr_height)) ; n++ ; 
  }

  if (strcmp (name, "e_su") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), e_su_bits, e_su_width, 
              e_su_height)) ; n++ ; 
  }

  if (strcmp (name, "ave") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), ave_bits, ave_width, 
              ave_height)) ; n++ ; 
  }

  if (strcmp (name, "act") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), act_bits, act_width, 
              act_height)) ; n++ ; 
  }

  if (strcmp (name, "out") == 0) 
  { 
    XtSetArg (arg[n], XtNbitmap, (Pixmap) XCreateBitmapFromData (ui_display,
              XDefaultRootWindow (ui_display), out_bits, out_width, 
              out_height)) ; n++ ; 
  }


  XtSetArg (arg[n], XtNborderWidth   , 0          ) ; n++ ; 
  XtSetArg (arg[n], XtNinternalHeight, 1          ) ; n++ ; 
  XtSetArg (arg[n], XtNinternalWidth , 1          ) ; n++ ; 
  XtSetArg (arg[n], XtNfromVert      , top        ) ; n++ ; 
  XtSetArg (arg[n], XtNfromHoriz     , left       ) ; n++ ; 
  XtSetArg (arg[n], XtNleft          , XtChainLeft) ; n++ ; 
  XtSetArg (arg[n], XtNright         , XtChainLeft) ; n++ ;
  XtSetArg (arg[n], XtNtop           , XtChainTop ) ; n++ ; 
  XtSetArg (arg[n], XtNbottom        , XtChainTop ) ; n++ ; 
  XtSetArg (arg[n], XtNradioGroup    , group      ) ; n++ ;

  return (XtCreateManagedWidget (name, toggleWidgetClass, parent, arg, n)) ;
}



/*****************************************************************************
  FUNCTION : NA_ResetNetworkAnalyzer

  PURPOSE  : initializes global variables
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_ResetNetworkAnalyzer (void) 

{
  na.window.x       =   0 ;
  na.window.y       =   0 ;
  na.window.w       = 300 ;
  na.window.h       = 150 ;

  na.pixmap.x       =   0 ;
  na.pixmap.y       =   0 ;
  na.pixmap.w       = 300 ;
  na.pixmap.h       = 150 ;

  na.graphtype      = NA_GRAPH_TE  ;
  na.error          = NA_ERROR_SQR ;
  na.error_unit     = 0 ;
  na.ave            = FALSE ;
  na.m_test         = 10 ;
  na.period         = 1  ;

  na.hor.min        =   1.0 ;
  na.hor.max        = 100.0 ;
  na.hor.factor     = (float) na.graph.w / (float) (na.hor.max-na.hor.min) ;
  na.hor.unit_no    = 0  ; 
  na.hor.grid       = 10 ;
  na.hor.out        = TRUE ;

  na.ver.min        = 0.0 ;
  na.ver.max        = 1.0 ;
  na.ver.factor     = (float) na.graph.h / (float) (na.ver.max-na.ver.min) ;
  na.ver.unit_no    = 0  ; 
  na.ver.grid       = 10 ;
  na.ver.out        = TRUE ;

  na.toggles.on     = FALSE ;
  na.toggles.line   = TRUE  ;     
  na.toggles.grid   = FALSE ;     
  na.toggles.record = FALSE ;     
 
  na.gd.time        = 1  ; 
  na.gd.i           = 0  ;
  na.gd.no_data     = TRUE  ;
  na.gd.wrapped     = FALSE ;
  na.rec_file       = (FILE *) NULL ;

  strcpy (na.rec_filename, "./SNNS_REC") ;

  strcpy (info_line, "\n") ;
}


/*****************************************************************************
  FUNCTION : NA_NetworkAnalyzerIsOpen 

  PURPOSE  : returns true if the network analyzer widget is open, else false
  NOTES    :

  UPDATE   : 
******************************************************************************/

Bool NA_NetworkAnalyzerIsOpen (void)

{
  return (na_Open == 1) ;
}


/*****************************************************************************
  FUNCTION : NA_OpenRecordFile

  PURPOSE  : opens the record file
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_OpenRecordFile (void)

{
  char buf[80] ;


  sprintf (buf, "%s.rec", na.rec_filename) ;

  if ((na.rec_file = fopen (buf, "a")) == (FILE *) NULL)
  {
    ui_confirmOk ("can't open file") ;
    fclose (na.rec_file) ; 
    ui_xSetToggleState (t_Record, FALSE) ; 
    na.toggles.record = FALSE ;
  }
  else
  {
    NA_WriteToRecordFile (info_line) ;
  }
}



/*****************************************************************************
  FUNCTION : NA_CloseRecordFile

  PURPOSE  : closes the record file
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_CloseRecordFile (void)

{
  fclose (na.rec_file) ; 
}



/*****************************************************************************
  FUNCTION : NA_WriteToRecordFile

  PURPOSE  : writes a string into the record file
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_WriteToRecordFile (char *str) 

{
  if (na.toggles.record) fprintf (na.rec_file, str) ;
}



/*############################################################################

                    D R A W I N G     F U N C T I O N S 

############################################################################*/


/*****************************************************************************
  FUNCTION : NA_DrawCS 

  PURPOSE  : draws the coordinate system 
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_DrawCS (void) 

{
  char  buf[40]   ;
  int   i, y, x   ;
  int   dist, len ;
  float f ;

  
  XDrawRectangle (na_Display, na_Pixmap, na_GC, na.graph.x, na.graph.y,
                  na.graph.w, na.graph.h) ;


  /* --------------------  draw horizontal labels  ------------------------- */

  dist = 100 ;

  for (i = 0 ; i < na.hor.grid ; i++)
  {
    x   = (int) ((float) (i * na.graph.w)  / (float) na.hor.grid) ;
    len = 3 ;

    if ((dist > 40) && (x < na.graph.w - 35)) 
    {
      f = (float) i / (float) na.hor.grid * (na.hor.max - na.hor.min) 
          + na.hor.min ;

      if (f < 10000.0) sprintf (buf, "%6.5f", f) ;
      else             sprintf (buf, "%6d"  , (int) f) ;

      XDrawString (na_Display, na_Pixmap, na_GC, na.graph.x - 18 + x, 
                   na.graph.y + na.graph.h + 14, buf, 6) ;
      len += 2 ;
      dist = 0 ;
    }

    XDrawLine (na_Display, na_Pixmap, na_GC, na.graph.x + x, 
               na.graph.y + na.graph.h + len,
               na.graph.x + x, na.graph.y + na.graph.h ) ;

    dist += (int) ((float) na.graph.w / (float) na.hor.grid) ;
  }

  if (na.hor.max < 10000.0) sprintf (buf, "%6.5f", na.hor.max) ;
  else                      sprintf (buf, "%6d"  , (int) na.hor.max) ;

  XDrawString (na_Display, na_Pixmap, na_GC, na.graph.x + na.graph.w - 18, 
               na.graph.y + na.graph.h + 14, buf, 6) ;

  XDrawLine   (na_Display, na_Pixmap, na_GC, 
               na.graph.x + na.graph.w, na.graph.y + na.graph.h, 
               na.graph.x + na.graph.w, na.graph.y + na.graph.h + 5) ;



  /* ---------------------  draw vertical labels  ------------------------- */

  dist = 100 ;

  for (i = 0 ; i < na.ver.grid ; i++)
  {
    y   = (int) ((float) (i * na.graph.h)  / (float) na.ver.grid) ;
    len = 3 ;

    if ((dist > 10) && (y < na.graph.h - 10 ))
    {
      f = (float) i / (float) na.ver.grid * (na.ver.max - na.ver.min) 
          + na.ver.min ;

      if (f < 10000.0) sprintf (buf, "%6.5f", f) ;
      else             sprintf (buf, "%6d"  , (int) f) ;

      XDrawString (na_Display, na_Pixmap, na_GC, 4, 
                   na.graph.y + na.graph.h + 4 - y, buf, 6) ;
      len += 2 ;
      dist = 0 ;
    }

    XDrawLine (na_Display, na_Pixmap, na_GC, 
               na.graph.x - len, na.graph.y + na.graph.h - y,
               na.graph.x      , na.graph.y + na.graph.h - y) ;

    dist += (int) ((float) na.graph.h / (float) na.ver.grid) ; 
  }

  if (na.ver.max < 10000.0) sprintf (buf, "%6.5f", na.ver.max) ;
  else                      sprintf (buf, "%6d"  , (int) na.ver.max) ;

  XDrawString (na_Display, na_Pixmap, na_GC, 4, na.graph.y + 4, buf, 6) ;

  XDrawLine   (na_Display, na_Pixmap, na_GC, na.graph.x - 5, na.graph.y,
               na.graph.x, na.graph.y) ;
}



/*****************************************************************************
  FUNCTION : NA_DrawInfo

  PURPOSE  : displays information about the axis 
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_DrawInfo (void)

{ 
  int          info_x, info_y ;
  char         buf1[256], buf2[256], info[1024] ;

  
  info_x = 0 ;
  info_y = na.pixmap.h - INFO_BORDER ;

  XDrawLine (na_Display, na_Pixmap, na_GC, 0, info_y, na.pixmap.w, info_y) ;

  if (na.graphtype == NA_GRAPH_XY) 
  {
    if (na.hor.out) 
      sprintf (buf1, "horizontal : output of unit no. %d", 
               na.hor.unit_no) ;
    else        
      sprintf (buf1, "horizontal : activation of unit no. %d", 
               na.hor.unit_no) ;
  }
  else
    sprintf (buf1, "horizontal : time") ;

  XDrawString (na_Display, na_Pixmap, na_GC, info_x + 7, info_y + 11, 
               buf1, strlen(buf1)) ;

  if (na.graphtype == NA_GRAPH_TE)
  {
    switch (na.error)
    {
      case NA_ERROR_LIN : 
        if (na.ave)
          strcpy  (buf2, "vertical   : error (lin ave)") ; 
        else
          strcpy  (buf2, "vertical   : error (lin)") ; break ;
      case NA_ERROR_SQR : 
        if (na.ave)
          strcpy  (buf2, "vertical   : error (sqr ave)") ; 
        else
          strcpy  (buf2, "vertical   : error (sqr)") ; break ;
      case NA_ERROR_SU  : 
        sprintf   (buf2, "vertical   : error of output unit no. %d",
                 na.error_unit) ; 
        break ;
    } 
  }
  else
  {
    if (na.ver.out) 
      sprintf (buf2, "vertical   : output of unit no. %d", 
               na.ver.unit_no) ;
    else           
      sprintf (buf2, "vertical   : activation of unit no. %d", 
               na.ver.unit_no) ;
  }

  XDrawString (na_Display, na_Pixmap, na_GC, info_x + 7, info_y + 21, 
               buf2, strlen(buf2)) ;

  sprintf (info, "# %s # %s #\n", buf1, buf2) ;

  if (strcmp (info_line, info) != 0)
  {
    strcpy (info_line, info) ;
    if (na.toggles.record) NA_WriteToRecordFile (info_line) ;
    NA_ClearGraph () ;
  }
}



/*****************************************************************************
  FUNCTION : NA_DrawPoint

  PURPOSE  : draws a single point into the network analyzer pixmap
             if mode = NA_DRAW_WINDOW then the point will be drawn into the 
             window, too
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_DrawPoint (int i2, int mode)

{
  int i1, x1, y1, x2, y2 ;


  x2 = na.graph.x + (int) ((na.gd.x[i2] - na.hor.min) * na.hor.factor) ;
  y2 = na.graph.y + 
       na.graph.h - (int) ((na.gd.y[i2] - na.ver.min) * na.ver.factor) ;

  if (na.gd.lineflag[i2])
  {
    i1 = (i2 - 1 + MAX_ARRAY_SIZE) % MAX_ARRAY_SIZE ;

    x1 = na.graph.x + (int) ((na.gd.x[i1] - na.hor.min) * na.hor.factor) ;
    y1 = na.graph.y + 
         na.graph.h - (int) ((na.gd.y[i1] - na.ver.min) * na.ver.factor) ;

    XDrawLine  (na_Display, na_Pixmap, na_GC, x1, y1, x2, y2) ;

    if (mode == NA_DRAW_WINDOW)
      XDrawLine  (na_Display, na_Window, na_GC, x1, y1, x2, y2) ;
  }
  else
  {
    XDrawPoint (na_Display, na_Pixmap, na_GC, x2, y2) ;

    if (mode == NA_DRAW_WINDOW)
      XDrawPoint (na_Display, na_Window, na_GC, x2, y2) ;
  }

  XDrawArc (na_Display, na_Pixmap, na_GC, x2-1, y2-1, 3, 3, 0, 359*64) ;

  if (mode == NA_DRAW_WINDOW)
    XDrawArc (na_Display, na_Window, na_GC, x2-1, y2-1, 3, 3, 0, 359*64) ;

}



/*****************************************************************************
  FUNCTION : NA_DrawGraph

  PURPOSE  : draws the graph  
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_DrawGraph (void) 

{
  int  j , i, no_of_points ;


  if (na.gd.wrapped)
  {
    no_of_points = MAX_ARRAY_SIZE ;
    i = na.gd.i ;
  }
  else
  {
    no_of_points = na.gd.i ;
    i = 0 ;
  }

  for (j = 0 ; j < no_of_points ; j ++) 
  {
    NA_DrawPoint (i, NA_DRAW_PIXMAP) ;
    i = (i + 1) % MAX_ARRAY_SIZE ;
  }
}



/*****************************************************************************
  FUNCTION : NA_DrawGrid

  PURPOSE  : draws the grid (only in the window !)
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_DrawGrid (void) 

{
  int  x, y, i, j ;


  for  (i = 1 ; i < na.hor.grid ; i++)
  {
    x = (int) ((float) (i * na.graph.w) / (float) na.hor.grid) ;

    for (j = 0 ; j <= na.graph.h ; j += 3) 
    {
      XDrawPoint (na_Display, na_Window, na_GC, 
                  na.graph.x + x, na.graph.y + na.graph.h - j) ;
    }
  }

  for (i = 1 ; i < na.ver.grid ; i++)
  {
    y = (int) ((float) (i * na.graph.h) / (float) na.ver.grid) ;

    for (j = 0 ; j <= na.graph.w ; j += 3)
    {
      XDrawPoint (na_Display, na_Window, na_GC, 
                  na.graph.x + j, na.graph.y + na.graph.h - y) ;
    }
  }
}



/*****************************************************************************
  FUNCTION : NA_ClearGraph

  PURPOSE  : clears the graph in the window and in the pixmap
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_ClearGraph (void)

{
  NA_StopTest () ;

  XSetForeground (na_Display, na_GC, WhitePixel (na_Display, na_Screen)) ;
  XFillRectangle (na_Display, na_Pixmap, na_GC, na.graph.x + 1, na.graph.y + 1,
                  na.graph.w - 1, na.graph.h - 1);
  XSetForeground (na_Display, na_GC, BlackPixel (na_Display, na_Screen)) ;

  XClearArea (na_Display, na_Window, na.graph.x + 1, na.graph.y + 1, 
              na.graph.w - 1, na.graph.h - 1, 0) ;

  na.gd.time        = 1 ;
  na.gd.i           = 0     ;
  na.gd.no_data     = TRUE  ;
  na.gd.wrapped     = FALSE ;
  na.gd.lineflag[0] = FALSE ;

  if (na.toggles.grid) NA_DrawGrid () ;
}



/*****************************************************************************
  FUNCTION : NA_RedrawPixmap

  PURPOSE  : redraws the pixmap, but NOT the window 
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_RedrawPixmap (void)

{
  XRectangle  clip_recs[1] ;


  clip_recs[0].x      = (short) 0 ;
  clip_recs[0].y      = (short) 0 ;
  clip_recs[0].width  = (unsigned short) na.pixmap.w ;
  clip_recs[0].height = (unsigned short) na.pixmap.h ;

  XSetClipRectangles (na_Display, na_GC, 0, 0, clip_recs, 1, Unsorted) ;   

  XSetForeground (na_Display, na_GC, WhitePixel (na_Display, na_Screen)) ;
  XFillRectangle (na_Display, na_Pixmap, na_GC, 0, 0, 
                  na.pixmap.w, na.pixmap.h) ;
  XSetForeground (na_Display, na_GC, BlackPixel (na_Display, na_Screen)) ;

  XDrawRectangle (na_Display, na_Pixmap, na_GC, 0, 0,
                  na.pixmap.w  - 1, na.pixmap.h - 1) ;

  NA_DrawCS    () ;
  NA_DrawInfo  () ;

  clip_recs[0].x      = (short)          na.graph.x ;
  clip_recs[0].y      = (short)          na.graph.y ;
  clip_recs[0].width  = (unsigned short) na.graph.w ;
  clip_recs[0].height = (unsigned short) na.graph.h ;

  XSetClipRectangles (na_Display, na_GC, 0, 0, clip_recs, 1, Unsorted) ;   

  NA_DrawGraph () ;
}



/*****************************************************************************
  FUNCTION : NA_ShowPixmap

  PURPOSE  : copies the pixmap into the window 
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_ShowPixmap (void)

{
  XRectangle  clip_recs[1] ;


  clip_recs[0].x      = (short) 0 ;
  clip_recs[0].y      = (short) 0 ;
  clip_recs[0].width  = (unsigned short) na.pixmap.w ;
  clip_recs[0].height = (unsigned short) na.pixmap.h ;

  XSetClipRectangles (na_Display, na_GC, 0, 0, clip_recs, 1, Unsorted) ;   

  XClearArea (na_Display, na_Window, 0, 0, na.pixmap.w, na.pixmap.h, 0) ;

  XCopyArea  (na_Display, na_Pixmap, na_Window, na_GC, 0, 0,
              na.pixmap.w, na.pixmap.h, 0, 0) ;

  clip_recs[0].x      = (short)          na.graph.x ;
  clip_recs[0].y      = (short)          na.graph.y ;
  clip_recs[0].width  = (unsigned short) na.graph.w ;
  clip_recs[0].height = (unsigned short) na.graph.h ;

  XSetClipRectangles (na_Display, na_GC, 0, 0, clip_recs, 1, Unsorted) ;   

  if (na.toggles.grid) NA_DrawGrid () ;
}



/*****************************************************************************
  FUNCTION : NA_DrawNextPoint

  PURPOSE  : gets the next point and updates the graph
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

void NA_DrawNextPoint (void)

{
  char         buf[80]  ;
  struct Unit *unit_ptr ;


  if (! NA_NetworkAnalyzerIsOpen()) return ;
  if (! na.toggles.on)              return ;


  /* ---------------------------  check units  ----------------------------- */

  if (na.graphtype == NA_GRAPH_XY)
  {
    if ((na.hor.unit_no < 1) || (na.hor.unit_no > krui_getNoOfUnits()))
    {
      ui_confirmOk ("x : illegal unit no.")   ;
      na.toggles.on = FALSE ;
      ui_xSetToggleState (t_On, FALSE) ;
      return ;
    }
  }

  if (na.graphtype != NA_GRAPH_TE)
  {
    if ((na.ver.unit_no < 1) || (na.ver.unit_no > krui_getNoOfUnits()))
    {
      ui_confirmOk ("y : illegal unit no.") ; 
      na.toggles.on = FALSE ;
      ui_xSetToggleState (t_On, FALSE) ;
      return ;
    }
  }
  else  
  {
    if (na.error == NA_ERROR_SU)
    {
      if ((unit_ptr = kr_getUnitPtr (na.error_unit)) == NULL) 
      {
        ui_confirmOk ("unit j : illegal output unit no.") ;
        na.toggles.on = FALSE ;
        ui_xSetToggleState (t_On, FALSE) ;
        return ;
      }

      if (! IS_OUTPUT_UNIT (unit_ptr))
      {
        ui_confirmOk ("unit j : illegal output unit no.") ;
        na.toggles.on = FALSE ;
        ui_xSetToggleState (t_On, FALSE) ;
        return ;
      }
    }
  }


  /* -------------------------  get values  -------------------------- */

  if (na.graphtype == NA_GRAPH_XY)
  {
    if (na.hor.out) 
      na.gd.x[na.gd.i] = (float) krui_getUnitOutput     (na.hor.unit_no) ;
    else            
      na.gd.x[na.gd.i] = (float) krui_getUnitActivation (na.hor.unit_no) ;

  }
  else
  {
    na.gd.x[na.gd.i] = (float) na.gd.time ;
    na.gd.time++ ;
  }

  if (na.graphtype == NA_GRAPH_TE)
  {
    na.gd.y[na.gd.i] = krui_NA_Error (ui_noOfCurrentPattern - 1
				      , na.error_unit, na.error, na.ave);
  }
  else
  {
    if (na.ver.out) 
      na.gd.y[na.gd.i] = (float) krui_getUnitOutput     (na.ver.unit_no) ;
    else            
      na.gd.y[na.gd.i] = (float) krui_getUnitActivation (na.ver.unit_no) ;
  }

  if ((na.toggles.line) && (!na.gd.no_data)) 
    na.gd.lineflag[na.gd.i] = True ;

  NA_DrawPoint (na.gd.i, NA_DRAW_WINDOW) ;

  if (na.toggles.record)
  { 
    sprintf (buf, "%-10f %-10f\n", na.gd.x[na.gd.i], na.gd.y[na.gd.i]) ;
    NA_WriteToRecordFile (buf) ;
  }

  na.gd.i = (na.gd.i + 1) % MAX_ARRAY_SIZE ;

  na.gd.no_data           = FALSE ;
  na.gd.lineflag[na.gd.i] = FALSE ;  

  if (na.gd.i == 0) na.gd.wrapped = TRUE ;
}



/*############################################################################

           N E T W O R K - A N A L Y Z E R       F U N C T I O N S 

############################################################################*/


/*****************************************************************************
  FUNCTION : NA_ResizeWindow

  PURPOSE  : resizes the network analyzer widget
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_ResizeWindow () 

{
  Cardinal   n ;
  Arg        args[10] ;

  int        s_hor, s_ver, b_hor, b_ver ;
  Dimension  w_width, w_height, b_width ;


  n = 0 ;
  XtSetArg    (args[n], XtNwidth  , &w_width ) ; n++ ;
  XtSetArg    (args[n], XtNheight , &w_height) ; n++ ;
  XtGetValues (w_Window, args, n) ;

  na.window.w = (unsigned int) w_width  ;
  na.window.h = (unsigned int) w_height ;

  n = 0 ;
  XtSetArg    (args[n], XtNhorizDistance, &s_hor) ; n++ ;
  XtSetArg    (args[n], XtNvertDistance , &s_ver) ; n++ ;
  XtGetValues (w_Screen, args, n) ;

  n = 0 ;
  XtSetArg    (args[n], XtNwidth         , &b_width) ; n++ ;
  XtSetArg    (args[n], XtNhorizDistance , &b_hor  ) ; n++ ;
  XtSetArg    (args[n], XtNvertDistance  , &b_ver  ) ; n++ ;
  XtGetValues (w_Buttons, args, n) ;

  na.pixmap.w = na.window.w - 1 * s_hor - (int) b_width - 2 * b_hor ;
  na.pixmap.h = na.window.h - 2 * s_ver ;

  na.graph.x = 50  ;
  na.graph.y = 10  ;
  na.graph.w = na.pixmap.w - na.graph.x - 20 ;
  na.graph.h = na.pixmap.h - na.graph.y - INFO_BORDER - 20 ;

  na_Pixmap = XCreatePixmap (na_Display, na_Window, na.pixmap.w, na.pixmap.h,
                             (unsigned int) na_Depth) ;

  na.hor.factor = (float) (na.graph.w) / (float) (na.hor.max - na.hor.min) ;
  na.ver.factor = (float) (na.graph.h) / (float) (na.ver.max - na.ver.min) ;

  XtResizeWidget (w_Screen, na.pixmap.w, na.pixmap.h, 0) ;
  XtMoveWidget   (w_Buttons, (Position) ((int) na.pixmap.w + s_hor + b_hor),
                  (Position) b_ver) ;

  NA_RedrawPixmap () ;
  NA_ShowPixmap   () ;
}



/*****************************************************************************
  FUNCTION : NA_EventProc

  PURPOSE  : event handler 
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void NA_EventProc (Widget w, Display *display, XEvent *event)

{
  switch (event->type)
  {
    case Expose: 
    {
      if (event->xexpose.count == 0) NA_ShowPixmap () ;
      break ;
    }

    case ConfigureNotify:
    {
      XFreePixmap (na_Display, na_Pixmap) ;
      NA_ResizeWindow () ;
      break ;
    }    
  }
}



/*****************************************************************************
  FUNCTION : NA_ContinueTest 

  PURPOSE  : used to synchronize the execution of multiple test operations
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

Bool NA_ContinueTest (void)

{
  if (! NA_NetworkAnalyzerIsOpen ()) return (FALSE) ;

  if (--no_of_m_test > 0) return (TRUE) ; 

  NA_StopTest () ; 
  return (FALSE) ;
}



/*****************************************************************************
  FUNCTION : NA_StopTest 

  PURPOSE  : used to stop a m-test operation
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

void NA_StopTest (void)

{
  if (NA_NetworkAnalyzerIsOpen ())
  {
    no_of_m_test  = 0 ;  
    ui_xSetToggleState (t_MTest, FALSE) ;
  }
}



/*****************************************************************************
  FUNCTION : NA_CallbackProc

  PURPOSE  : network analyzer callback function 
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_CallbackProc (Widget w, XtPointer client_data, 
                                       XtPointer call_data)

{
  if      (w == t_On) 
    na.toggles.on = ui_xGetToggleState (t_On) ;    

  else if (w == t_Line) 
    na.toggles.line = ui_xGetToggleState (t_Line) ;    

  else if (w == t_Grid)
  {
    if ((na.toggles.grid = ui_xGetToggleState (t_Grid))) 
      NA_DrawGrid   () ;
    else           
      NA_ShowPixmap () ;
  }

  else if (w == b_Clear) NA_ClearGraph () ;

  else if (w == t_MTest)
  {
    if (ui_controlIsCreated)
    {
      if (ui_xGetToggleState (t_MTest))
      {
        no_of_m_test = na.m_test ;
        ui_rem_testProc (NULL, NULL, NULL) ;
	ui_xSetToggleState (t_MTest, FALSE) ;
      }
      else
        ui_xSetToggleState (t_MTest, TRUE) ;
    }
    else{
	ui_confirmOk ("control panel not open") ;
	ui_xSetToggleState (t_MTest, FALSE) ;
    }
  }

  else if (w == t_Record)
  {
    if ((na.toggles.record = ui_xGetToggleState (t_Record))) 
      NA_OpenRecordFile  () ;
    else  
      NA_CloseRecordFile () ; 
  }

  else if (w == b_Done)
  {
    if (na.toggles.record) 
      ui_confirmOk ("Rec-file not closed !") ; 
    else 
    { 
      NA_StopTest () ;

      if (na_DisplayControlOpen != 0) 
      {
        XtDestroyWidget (w_DisplayControl) ;
        na_DisplayControlOpen = 0 ;
      }

      XFreePixmap (na_Display, na_Pixmap) ;
      XtDestroyWidget (w_NetworkAnalyzer) ; 
      na_Open = 0 ;
      na.toggles.on = FALSE ; 
    } 
  }

} 



/*****************************************************************************
  FUNCTION : NA_CreateNetworkAnalyzerWidget

  PURPOSE  : creates the network analyzer widget
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_CreateNetworkAnalyzerWidget (void)

{ 
  Cardinal   n ; 
  Arg        args[15] ; 
  Widget     dummy ;


  n = 0;  

  w_NetworkAnalyzer = 
    XtCreatePopupShell ("SNNS Network Analyzer", topLevelShellWidgetClass,
                        ui_toplevel, args, n); 

  n = 0 ;
  XtSetArg (args[n], XtNheight   , (Dimension) na.window.h) ; n++ ;
  XtSetArg (args[n], XtNwidth    , (Dimension) na.window.w) ; n++ ;
  XtSetArg (args[n], XtNresizable, True                   ) ; n++ ;

  w_Window = 
  XtCreateManagedWidget ("form", formWidgetClass, w_NetworkAnalyzer, NULL, 0) ;

  n = 0 ;
  XtSetArg (args[n], XtNheight      , (Dimension) na.pixmap.h ) ; n++ ; 
  XtSetArg (args[n], XtNwidth       , (Dimension) na.pixmap.w ) ; n++ ;
  XtSetArg (args[n], XtNleft        , XtChainLeft             ) ; n++ ; 
  XtSetArg (args[n], XtNright       , XtChainLeft             ) ; n++ ; 
  XtSetArg (args[n], XtNtop         , XtChainTop              ) ; n++ ;
  XtSetArg (args[n], XtNbottom      , XtChainTop              ) ; n++ ;
  XtSetArg (args[n], XtNfromVert    , NULL                    ) ; n++ ; 
  XtSetArg (args[n], XtNresizable   , True                    ) ; n++ ;
  XtSetArg (args[n], XtNborderWidth , 0                       ) ; n++ ;

  w_Screen = 
    XtCreateManagedWidget("screen", formWidgetClass, w_Window, args, n) ;

  n = 0 ;
  XtSetArg (args[n], XtNfromVert    , NULL        ) ; n++ ; 
  XtSetArg (args[n], XtNfromHoriz   , w_Screen    ) ; n++ ; 
  XtSetArg (args[n], XtNleft        , XtChainLeft ) ; n++ ; 
  XtSetArg (args[n], XtNright       , XtChainLeft ) ; n++ ; 
  XtSetArg (args[n], XtNtop         , XtChainTop  ) ; n++ ;
  XtSetArg (args[n], XtNbottom      , XtChainTop  ) ; n++ ;
  XtSetArg (args[n], XtNresizable   , False       ) ; n++ ;
  XtSetArg (args[n], XtNborderWidth , 0           ) ; n++ ;
  XtSetArg (args[n], XtNvertDistance, 0           ) ; n++ ;

  w_Buttons = 
    XtCreateManagedWidget ("form", formWidgetClass, w_Window, args, n) ;

  t_On     = NA_CreateToggleItem ("on2"   , w_Buttons, NULL , NULL, NULL    ) ;
  t_Line   = NA_CreateToggleItem ("line"  , w_Buttons, NULL , NULL, t_On    ) ;
  t_Grid   = NA_CreateToggleItem ("grid"  , w_Buttons, NULL , NULL, t_Line  ) ;
  b_Clear  = NA_CreateButtonItem ("clear" , w_Buttons, NULL ,       t_Grid  ) ;
  dummy    = ui_xCreateLabelItem (" "     , w_Buttons, 6 * 8, NULL, b_Clear ) ;
  t_MTest  = NA_CreateToggleItem ("m_test", w_Buttons, NULL , NULL, dummy   ) ;
  dummy    = ui_xCreateLabelItem (" "     , w_Buttons, 6 * 8, NULL, t_MTest ) ;
  t_Record = NA_CreateToggleItem ("record", w_Buttons, NULL , NULL, dummy   ) ;
  dummy    = ui_xCreateLabelItem (" "     , w_Buttons, 6 * 8, NULL, t_Record) ;
  b_DCtrl  = NA_CreateButtonItem ("dctrl" , w_Buttons, NULL ,       dummy   ) ;
  b_Setup  = NA_CreateButtonItem ("setup" , w_Buttons, NULL ,       b_DCtrl ) ;
  b_Done   = NA_CreateButtonItem ("done2" , w_Buttons, NULL ,       b_Setup ) ;

  ui_xSetToggleState (t_On    , na.toggles.on    ) ; 
  ui_xSetToggleState (t_Line  , na.toggles.line  ) ; 
  ui_xSetToggleState (t_Grid  , na.toggles.grid  ) ; 
  ui_xSetToggleState (t_MTest , FALSE) ;
  ui_xSetToggleState (t_Record, na.toggles.record) ;

  XtAddCallback (t_On    , XtNcallback, (XtCallbackProc)NA_CallbackProc, NULL);
  XtAddCallback (t_Line  , XtNcallback, (XtCallbackProc)NA_CallbackProc, NULL);
  XtAddCallback (t_Grid  , XtNcallback, (XtCallbackProc)NA_CallbackProc, NULL);
  XtAddCallback (b_Clear , XtNcallback, (XtCallbackProc)NA_CallbackProc, NULL);
  XtAddCallback (t_MTest , XtNcallback, (XtCallbackProc)NA_CallbackProc, NULL);
  XtAddCallback (t_Record, XtNcallback, (XtCallbackProc)NA_CallbackProc, NULL);
  XtAddCallback (b_Setup , XtNcallback, (XtCallbackProc)NA_OpenSetup   , NULL);
  XtAddCallback (b_Done  , XtNcallback, (XtCallbackProc)NA_CallbackProc, NULL);
  XtAddCallback (b_DCtrl , XtNcallback, (XtCallbackProc)NA_OpenDisplayControl, 
                 NULL);


  XtAddEventHandler (w_Screen, ExposureMask, FALSE, 
                     (XtEventHandler) NA_EventProc, NULL) ;

  XtAddEventHandler (w_Window, StructureNotifyMask, FALSE, 
                     (XtEventHandler) NA_EventProc, NULL) ;
  XtAddEventHandler(w_Window,KeyPressMask,FALSE,
		    (XtEventHandler)ui_key_control,(Cardinal *) 0);
   
  ui_checkWindowPosition (w_NetworkAnalyzer);
  XtPopup (w_NetworkAnalyzer, XtGrabNone);

  if (na_FirstCall)
  {
    n = 0 ;
    XtSetArg    (args[n], XtNwidth , &na_Width ) ; n++ ;  
    XtSetArg    (args[n], XtNheight, &na_Height) ; n++ ;
    XtGetValues (w_NetworkAnalyzer, args, n) ;
  }

  n = 0 ;
  XtSetArg    (args[n], XtNminWidth , na_Width ) ; n++ ;  
  XtSetArg    (args[n], XtNminHeight, na_Height) ; n++ ;
  XtSetValues (w_NetworkAnalyzer, args, n) ;

  na_Display    = XtDisplay      (w_Screen) ; 
  na_Window     = XtWindow       (w_Screen) ;
  na_FontStruct = XLoadQueryFont (na_Display, "6x12") ;
  na_GC         = XCreateGC      (na_Display, na_Window, ZERO, NULL) ;
  na_Screen     = DefaultScreen  (na_Display) ;
  na_Depth      = DisplayPlanes  (na_Display, na_Screen) ;

  XSetFont (na_Display, na_GC, (*na_FontStruct).fid) ;

  XSetBackground (na_Display, na_GC, WhitePixel (na_Display, na_Screen)) ;
  XSetForeground (na_Display, na_GC, BlackPixel (na_Display, na_Screen)) ; 

  XSetGraphicsExposures (na_Display, na_GC, 0) ;

  NA_ResizeWindow () ;
  na_Open = 1 ;
}



/*****************************************************************************
  FUNCTION : NA_OpenNetworkAnalyzer

  PURPOSE  : opens the network analyzer widget
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

void NA_OpenNetworkAnalyzer (Widget w, 
                             XtPointer client_data, 
                             XtPointer call_data)

{
  if (na_Open == 0)
  {
    no_of_m_test = 0 ;

    if (na_FirstCall) NA_ResetNetworkAnalyzer () ;
 
    NA_CreateNetworkAnalyzerWidget () ;
    na_FirstCall = FALSE ;
    na_DisplayControlOpen = 0 ;
   } 
   else
       XRaiseWindow(XtDisplay(w_NetworkAnalyzer), XtWindow(w_NetworkAnalyzer));
}



/*############################################################################

                      S E T U P      F U N C T I O N S 

############################################################################*/


/*****************************************************************************
  FUNCTION : NA_SetupGetValues

  PURPOSE  : gets data from setup
  RETURNS  : void 
  NOTES    :

  UPDATE   :
******************************************************************************/

static void NA_SetupGetValues (void)

{ 
  se.hor.grid  = ui_xIntFromAsciiWidget   (w_HorGrid) ;
  se.ver.min   = ui_xFloatFromAsciiWidget (w_VerMin ) ;  
  se.ver.max   = ui_xFloatFromAsciiWidget (w_VerMax ) ;  
  se.ver.grid  = ui_xIntFromAsciiWidget   (w_VerGrid) ;  
  se.m_test    = ui_xIntFromAsciiWidget   (w_MSteps ) ; 
  se.hor.min   = ui_xFloatFromAsciiWidget (w_HorMin)  ;
  se.hor.max   = ui_xFloatFromAsciiWidget (w_HorMax)  ;  

  if (se.graphtype == NA_GRAPH_XY)
  {
    se.hor.unit_no = ui_xIntFromAsciiWidget   (w_HorUnit) ;  
    se.hor.out     = ui_xGetToggleState       (t_HorOut)  ;
  }
  else
    se.period      = ui_xIntFromAsciiWidget   (w_Period)  ;

  if (se.graphtype == NA_GRAPH_TE)
  {
    if      (ui_xGetToggleState (t_Lin)) se.error = NA_ERROR_LIN ;
    else if (ui_xGetToggleState (t_Sqr)) se.error = NA_ERROR_SQR ;
    else                                 
    { 
      se.error      = NA_ERROR_SU  ;
      se.error_unit = ui_xIntFromAsciiWidget (w_Su) ;
    }

    se.ave = ui_xGetToggleState (t_Ave) ;
  }
  else 
  {
    se.ver.unit_no = ui_xIntFromAsciiWidget (w_VerUnit) ;  
    se.ver.out     = ui_xGetToggleState     (t_VerOut ) ;
  }

  if (!na.toggles.record) 
    ui_xStringFromAsciiWidget (w_RecFile, se.rec_filename, 30) ; 
}



/*****************************************************************************
  FUNCTION : NA_SetupCheckValues

  PURPOSE  : checks the new values
  RETURNS  : void 
  NOTES    :

  UPDATE   :
******************************************************************************/

static Bool NA_SetupCheckValues (void)

{
  struct Unit *unit_ptr ;


  if (se.hor.grid < 0)           se.hor.grid = 0 ;
  if (se.hor.grid > na.graph.w)  se.hor.grid = na.graph.w ;

  if (se.ver.grid < 0)           se.ver.grid = 0 ;
  if (se.ver.grid > na.graph.h)  se.ver.grid = na.graph.h ;

  if (se.m_test  < 1) se.m_test  = 1 ;
  if (se.period  < 1) se.period  = 1 ;

  if (se.hor.min >= se.hor.max) 
  {
    ui_confirmOk (" x : min has to be less than max") ;
    return (False) ;
  }

  if (se.ver.min >= se.ver.max)
  {
    ui_confirmOk (" y : min has to be less than max") ;
    return (False) ;
  }

  if (se.graphtype == NA_GRAPH_XY)
  {
    if ((se.hor.unit_no < 1) || (se.hor.unit_no > krui_getNoOfUnits()))
    {
      ui_confirmOk ("x : illegal unit no.") ; 
      return (False) ;
    }
  }

  if (se.graphtype != NA_GRAPH_TE)
  {
    if ((se.ver.unit_no < 1) || (se.ver.unit_no > krui_getNoOfUnits()))
    {
      ui_confirmOk ("y : illegal unit no.") ; 
      return (False) ;
    }
  }
  else  
  {
    if (se.error == NA_ERROR_SU)
    {
      if ((unit_ptr = kr_getUnitPtr (se.error_unit)) == NULL) 
      {
        ui_confirmOk ("unit j : illegal output unit no.") ;
        return (False) ;
      }

      if (! IS_OUTPUT_UNIT (unit_ptr))
      {
        ui_confirmOk ("unit j : illegal output unit no.") ;
        return (False) ;
      }
    }
  }

  return (True) ;
}



/*****************************************************************************
  FUNCTION : NA_SetupSetValues

  PURPOSE  : sets the new values 
  RETURNS  : void 
  NOTES    :

  UPDATE   :
******************************************************************************/

static void NA_SetupSetValues (void)

{
  if (se.graphtype != na.graphtype) NA_ClearGraph () ;

  na.graphtype     = se.graphtype  ;
  na.error         = se.error      ;
  na.error_unit    = se.error_unit ;
  na.ave           = se.ave        ;
  na.m_test        = se.m_test     ;
  na.period        = se.period     ;
  na.gd.time       = se.period     ;

  na.hor.min       = se.hor.min     ;
  na.hor.max       = se.hor.max     ;
  na.hor.unit_no   = se.hor.unit_no ;
  na.hor.grid      = se.hor.grid    ;
  na.hor.out       = se.hor.out     ;

  na.ver.min       = se.ver.min     ;
  na.ver.max       = se.ver.max     ;
  na.ver.unit_no   = se.ver.unit_no ;
  na.ver.grid      = se.ver.grid    ;
  na.ver.out       = se.ver.out     ;

  strcpy (na.rec_filename, se.rec_filename) ;

  na.hor.factor    = (float) (na.graph.w) / (na.hor.max - na.hor.min) ;
  na.ver.factor    = (float) (na.graph.h) / (na.ver.max - na.ver.min) ;

}



/*****************************************************************************
  FUNCTION : NA_SetupCallbackProc

  PURPOSE  : setup callback function  
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_SetupCallbackProc (Widget w, 
                                  XtPointer client_data, 
                                  XtPointer call_data)

{
  if      (w == t_XY)
  {
    if (ui_xGetToggleState (t_XY))
    {
      ui_xSetToggleState   (t_TY, False) ; 
      ui_xSetToggleState   (t_TE, False) ;
      NA_SetupGetValues    ()            ;
/*      NA_CreateSetupWidget (NA_GRAPH_XY) ; */
      XtDestroyWidget (w_Setup) ;
      XtDestroyWidget (w_box)   ;
      na.graphtype = NA_GRAPH_XY;
      NA_OpenSetup (w, client_data, call_data);
    }
    else 
      ui_xSetToggleState (t_XY, True) ;
  }

  else if (w == t_TY)
  {  
    if (ui_xGetToggleState (t_TY))
    {
      ui_xSetToggleState   (t_XY, False) ; 
      ui_xSetToggleState   (t_TE, False) ; 
      NA_SetupGetValues    ()            ;
/*      NA_CreateSetupWidget (NA_GRAPH_TY) ; */
      XtDestroyWidget (w_Setup) ;
      XtDestroyWidget (w_box)   ;
      na.graphtype =  NA_GRAPH_TY;
      NA_OpenSetup (w, client_data, call_data);
    }
    else 
      ui_xSetToggleState (t_TY, True) ;
  }

  else if (w == t_TE)
  {  
    if (ui_xGetToggleState (t_TE))
    {
      ui_xSetToggleState   (t_XY, False) ; 
      ui_xSetToggleState   (t_TY, False) ;
      NA_SetupGetValues    ()            ;
/*      NA_CreateSetupWidget (NA_GRAPH_TE) ; */
      XtDestroyWidget (w_Setup) ;
      XtDestroyWidget (w_box)   ;
      na.graphtype =  NA_GRAPH_TE;
      NA_OpenSetup (w, client_data, call_data);
    }
    else 
      ui_xSetToggleState (t_TE, True) ;
  }

  else if (w == t_HorOut)
  {
    if (ui_xGetToggleState (t_HorOut)) ui_xSetToggleState (t_HorAct, False) ;
    else                               ui_xSetToggleState (t_HorOut, True ) ;
  }

  else if (w == t_HorAct)
  {
    if (ui_xGetToggleState (t_HorAct)) ui_xSetToggleState (t_HorOut, False) ;
    else                               ui_xSetToggleState (t_HorAct, True ) ;
  }

  else if (w == t_VerOut)
  {
    if (ui_xGetToggleState (t_VerOut)) ui_xSetToggleState (t_VerAct, False) ;
    else                               ui_xSetToggleState (t_VerOut, True ) ;
  }

  else if (w == t_VerAct)
  {
    if (ui_xGetToggleState (t_VerAct)) ui_xSetToggleState (t_VerOut, False) ;
    else                               ui_xSetToggleState (t_VerAct, True ) ;
  }

  else if (w == t_Lin)
  {  
    if (ui_xGetToggleState (t_Lin))
    {
      ui_xSetToggleState (t_Sqr, False) ; 
      ui_xSetToggleState (t_Su , False) ; 
    }
    else                               
      ui_xSetToggleState (t_Lin, True ) ;
  }

  else if (w == t_Sqr)
  {  
    if (ui_xGetToggleState (t_Sqr)) 
    {
      ui_xSetToggleState (t_Lin, False) ; 
      ui_xSetToggleState (t_Su , False) ; 
    }
    else                               
      ui_xSetToggleState (t_Sqr, True ) ;
  }

  else if (w == t_Su)
  {  
    if (ui_xGetToggleState (t_Su)) 
    {
      ui_xSetToggleState (t_Lin, False) ; 
      ui_xSetToggleState (t_Sqr, False) ; 
    }
    else                               
      ui_xSetToggleState (t_Su , True ) ;
  }

  else if (w == b_Cancel) XtDestroyWidget (w_Setup) ;

  else if (w == b_DoneS)
  { 
    NA_SetupGetValues () ;
    
    if (NA_SetupCheckValues ())
    {
      NA_SetupSetValues () ;
      NA_RedrawPixmap   () ;
      NA_ShowPixmap     () ;
      XtDestroyWidget   (w_Setup) ;
    }
  }

}



/*****************************************************************************
  FUNCTION : NA_CreateSetupWidget
 
  PURPOSE  : creates the setup widget 
  RETURNS  : void 
  NOTES    :

  UPDATE   :
******************************************************************************/

static void NA_CreateSetupWidget (int graphtype)

{
  Widget     w_OldSetup, w_form[10] ;
  Widget     w_top, w_left, dummy ;

  Cardinal   n, f ;
  Arg        args[15] ;
  Position   setup_x, setup_y ;
  Dimension  width, max_width ;

  char       buf[80] ;


  w_OldSetup = w_Setup ;

  if (w_OldSetup == NULL)
  {
    XtTranslateCoords (b_Setup, 20, 8, &setup_x, &setup_y) ;

    n = 0;
    XtSetArg (args[n], XtNx        , setup_x      ) ; n++ ;
    XtSetArg (args[n], XtNy        , setup_y      ) ; n++ ;

    w_Setup = XtCreatePopupShell    ("Network Analyzer Setup", 
                                     transientShellWidgetClass, 
                                     ui_toplevel, args, n) ;

  }
  else
  {
    XtUnmapWidget   (w_Setup) ;
    XtDestroyWidget (w_box)   ;
  }

  se.graphtype = graphtype ;
  
  n = 0 ;

  w_box   = XtCreateManagedWidget ("box", boxWidgetClass, w_Setup, args, n) ;


  f = 0 ;
  n = 0 ;

  XtSetArg (args[n], XtNwidth , 10) ; n++ ; 
  XtSetArg (args[n], XtNheight, 10) ; n++ ;

 
  /* ---------------------------  setup part 1  ---------------------------- */

  w_form[f] = XtCreateManagedWidget ("form", formWidgetClass, w_box, args, n) ;

  dummy = ui_xCreateLabelItem ("graph :", w_form[f], 10 * 8, NULL , NULL) ;
  t_XY  = NA_CreateToggleItem ("xy"     , w_form[f], NULL  , dummy, NULL) ;
  dummy = ui_xCreateLabelItem ("  "     , w_form[f],  2 * 8, t_XY , NULL) ;
  t_TY  = NA_CreateToggleItem ("ty"     , w_form[f], NULL  , dummy, NULL) ;
  dummy = ui_xCreateLabelItem ("  "     , w_form[f],  2 * 8, t_TY , NULL) ;
  t_TE  = NA_CreateToggleItem ("te"     , w_form[f], NULL  , dummy, NULL) ;

  ui_xSetToggleState (t_XY, se.graphtype == NA_GRAPH_XY) ; 
  ui_xSetToggleState (t_TY, se.graphtype == NA_GRAPH_TY) ;
  ui_xSetToggleState (t_TE, se.graphtype == NA_GRAPH_TE) ; 

  XtAddCallback 
    (t_XY, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;
  XtAddCallback 
    (t_TY, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;
  XtAddCallback 
    (t_TE, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;

  f++ ;


  /* ---------------------------  setup part 2  ---------------------------- */

  w_form[f] = XtCreateManagedWidget ("form", formWidgetClass, w_box, args, n) ;

  strcpy (buf, "axis    min     max    unit   grid") ;

  w_top  = ui_xCreateLabelItem (buf, w_form[f], 33 * 8 - 6, NULL, NULL ) ;

  if (se.graphtype == NA_GRAPH_XY) strcpy (buf, " x ") ;
  else                             strcpy (buf, " t ") ;

  w_left = ui_xCreateLabelItem (buf, w_form[f], 5 * 8, NULL, w_top) ;

  sprintf (buf, "%5.3f", se.hor.min) ;
  w_HorMin  = w_left =
    ui_xCreateDialogItem (" ", w_form[f], buf,  6 * 8, w_left, w_top) ;

  sprintf (buf, "%5.3f", se.hor.max) ;
  w_HorMax  = w_left =
    ui_xCreateDialogItem (" ", w_form[f], buf,  6 * 8, w_left, w_top) ;

  if (se.graphtype == NA_GRAPH_XY)
  {
    sprintf (buf, "%d", se.hor.unit_no) ;
    w_HorUnit = w_left =
      ui_xCreateDialogItem (" ", w_form[f], buf,  6 * 8, w_left, w_top) ;
  }
  else
    w_left =
      ui_xCreateLabelItem  (" ", w_form[f],   2 + 6 * 8, w_left, w_top) ;

  sprintf (buf, "%d", se.hor.grid) ;
  w_HorGrid  = w_top =
      ui_xCreateDialogItem (" ", w_form[f], buf,  6 * 8, w_left, w_top) ;

  if (se.graphtype == NA_GRAPH_TE) strcpy (buf, " e ") ;
  else                             strcpy (buf, " y ") ;

  w_left  =
      ui_xCreateLabelItem  (buf, w_form[f],       5 * 8, NULL  , w_top) ;

  sprintf (buf, "%5.3f", se.ver.min) ;
  w_VerMin  = w_left =
      ui_xCreateDialogItem (" ", w_form[f], buf,  6 * 8, w_left, w_top) ;

  sprintf (buf, "%5.3f", se.ver.max) ;
  w_VerMax  = w_left =
      ui_xCreateDialogItem (" ", w_form[f], buf,  6 * 8, w_left, w_top) ;

  if (se.graphtype != NA_GRAPH_TE)
  {
    sprintf (buf, "%d", se.ver.unit_no) ;
    w_VerUnit  = w_left =
      ui_xCreateDialogItem (" ", w_form[f], buf,  6 * 8, w_left, w_top) ;
  }
  else
    w_left =
      ui_xCreateLabelItem  (" ", w_form[f],   6 * 8 + 2, w_left, w_top) ;

  sprintf (buf, "%d", se.ver.grid) ;
  w_VerGrid  = w_left =
      ui_xCreateDialogItem (" ", w_form[f], buf,  6 * 8, w_left, w_top) ;

  f++ ;


  /* ---------------------------  setup part 3  ---------------------------- */

  w_form[f] = XtCreateManagedWidget ("form", formWidgetClass, w_box, args, n) ;
 
  w_top = NULL ;

  if (se.graphtype == NA_GRAPH_XY)
  {
    dummy    = ui_xCreateLabelItem ("x unit :", w_form[f], 10*8, NULL, w_top) ;
    t_HorOut = NA_CreateToggleItem ("out", w_form[f], NULL, dummy    , w_top) ;
    dummy    = ui_xCreateLabelItem ("  " , w_form[f], 2*8 , t_HorOut , w_top) ;
    t_HorAct = NA_CreateToggleItem ("act", w_form[f], NULL, dummy    , w_top) ;

    ui_xSetToggleState (t_HorOut,  se.hor.out) ;
    ui_xSetToggleState (t_HorAct, !se.hor.out) ;

    XtAddCallback 
      (t_HorOut, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;
    XtAddCallback 
      (t_HorAct, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;

    w_top = dummy ;
  }

  if (se.graphtype != NA_GRAPH_TE) 
  {
    dummy    = ui_xCreateLabelItem ("y unit :", w_form[f], 10*8, NULL, w_top) ;
    t_VerOut = NA_CreateToggleItem ("out", w_form[f], NULL, dummy    , w_top) ;
    dummy    = ui_xCreateLabelItem ("  " , w_form[f], 2*8 , t_VerOut , w_top) ;
    t_VerAct = NA_CreateToggleItem ("act", w_form[f], NULL, dummy    , w_top) ;

    ui_xSetToggleState (t_VerOut,  se.ver.out) ;
    ui_xSetToggleState (t_VerAct, !se.ver.out) ;

    XtAddCallback 
      (t_VerOut, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;
    XtAddCallback 
      (t_VerAct, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;
  }
  else
  {
    dummy = ui_xCreateLabelItem ("error  :", w_form[f], 8 * 8, NULL , w_top) ;
    t_Lin = NA_CreateToggleItem ("e_lin"   , w_form[f], NULL , dummy, w_top) ;
    dummy = ui_xCreateLabelItem ("  "      , w_form[f], 1 * 8, t_Lin, w_top) ;
    t_Sqr = NA_CreateToggleItem ("e_sqr"   , w_form[f], NULL , dummy, w_top) ;
    dummy = ui_xCreateLabelItem ("  "      , w_form[f], 1 * 8, t_Sqr, w_top) ;
    t_Su  = NA_CreateToggleItem ("e_su"    , w_form[f], NULL , dummy, w_top) ;

    w_top = t_Lin ;
    sprintf (buf, "%d", se.error_unit) ;

    dummy = ui_xCreateLabelItem ("unit j :", w_form[f], 8 * 8, NULL , w_top) ;
    w_Su  = ui_xCreateDialogItem(" ",   w_form[f], buf, 6 * 8, dummy, w_top) ;
    dummy = ui_xCreateLabelItem ("  "      , w_form[f], 9*8+2, w_Su , w_top) ;
    t_Ave = NA_CreateToggleItem ("ave"     , w_form[f], NULL , dummy, w_top) ;

    ui_xSetToggleState (t_Lin, (se.error == NA_ERROR_LIN)) ;
    ui_xSetToggleState (t_Sqr, (se.error == NA_ERROR_SQR)) ;
    ui_xSetToggleState (t_Su , (se.error == NA_ERROR_SU )) ;
    ui_xSetToggleState (t_Ave,  se.ave) ;

    XtAddCallback 
      (t_Lin, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;
    XtAddCallback 
      (t_Sqr, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;
    XtAddCallback 
      (t_Su , XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;
  }

  f++ ;


  /* ---------------------------  setup part 4  ---------------------------- */

  w_form[f] = XtCreateManagedWidget ("form", formWidgetClass, w_box, args, n) ;

  w_left = w_top =
    ui_xCreateLabelItem  ("m-test :", w_form[f], 8 * 8, NULL  , NULL) ;

  sprintf (buf, "%d", se.m_test) ;
  w_MSteps = ui_xCreateDialogItem (" ", w_form[f], buf, 6 * 8, w_left, NULL) ;


  if (se.graphtype != NA_GRAPH_XY)
  {
    w_left =                              
      ui_xCreateLabelItem ("time   :", w_form[f], 8 * 8, NULL , w_top) ;

    sprintf (buf, "%d", se.period) ;
      w_Period =
        ui_xCreateDialogItem (" ", w_form[f], buf, 6 * 8, w_left   , w_top) ;
  }

  f++ ;


  /* ---------------------------  setup part 5  ---------------------------- */

  w_form[f] = XtCreateManagedWidget ("form", formWidgetClass, w_box, args, n) ;

  dummy =
      ui_xCreateLabelItem  ("record file :", w_form[f], 14 * 8, NULL, NULL) ;

  if (na.toggles.record)
    dummy = 
      ui_xCreateLabelItem  (se.rec_filename, w_form[f], 32 * 8, NULL, dummy) ;
  else
    w_RecFile =
      ui_xCreateDialogItem (" ", w_form[f], se.rec_filename, 32 * 8, NULL, 
                            dummy) ;
 
  f++ ;


  /* ---------------------------  setup part 6  ---------------------------- */

  if (f > 0) w_top = w_form[f-1] ;
  else       w_top = NULL ;
 
  b_DoneS  = NA_CreateButtonItem ("done2" , w_box, NULL   , w_top) ;
  b_Cancel = NA_CreateButtonItem ("cancel", w_box, b_DoneS, w_top) ;

  XtAddCallback 
    (b_DoneS , XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;

  XtAddCallback 
    (b_Cancel, XtNcallback, (XtCallbackProc) NA_SetupCallbackProc, NULL) ;


  /* ---------------  popup new setup, destroy old setup  ------------------ */

  if (w_OldSetup == NULL){
      ui_checkWindowPosition (w_Setup);
      XtPopup (w_Setup, XtGrabExclusive) ;
  }

  XtMapWidget (w_Setup) ;


  /* ------------------------  resize widgets  ----------------------------- */

  max_width = 0 ;

  for (n = 0 ; n < f ; n++)
  {
    XtSetArg    (args[0]  , XtNwidth, &width) ;
    XtGetValues (w_form[n], args    ,  1    ) ;

    if (width > max_width) max_width = width ;
  }

  for (n = 0 ; n < f ; n++)
  {
    XtSetArg    (args[0]  , XtNwidth,  width) ;
    XtSetValues (w_form[n], args    ,  1    ) ;
  }

/*
  n = 0 ;
  XtSetArg    (args[n], XtNwidth    , &width ) ; n++ ;
  XtSetArg    (args[n], XtNheight   , &height) ; n++ ;
  XtGetValues (w_Setup, args        ,  n     ) ;

  n = 0 ;
  XtSetArg    (args[n], XtNminWidth ,  width ) ; n++ ;
  XtSetArg    (args[n], XtNminHeight,  height) ; n++ ;
  XtSetArg    (args[n], XtNmaxWidth ,  width ) ; n++ ;
  XtSetArg    (args[n], XtNmaxHeight,  height) ; n++ ;
  XtSetValues (w_Setup, args        ,  n     ) ;
*/
}



/*****************************************************************************
  FUNCTION : NA_OpenSetup

  PURPOSE  : gets the actual parameters and opens the setup widget
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_OpenSetup (Widget w, 
                          XtPointer client_data,
                          XtPointer call_data)

{
  se.hor.min     = na.hor.min     ;
  se.hor.max     = na.hor.max     ;
  se.hor.factor  = na.hor.factor  ;
  se.hor.unit_no = na.hor.unit_no ;
  se.hor.grid    = na.hor.grid    ;
  se.hor.out     = na.hor.out     ;

  se.ver.min     = na.ver.min     ;
  se.ver.max     = na.ver.max     ;
  se.ver.factor  = na.ver.factor  ;
  se.ver.unit_no = na.ver.unit_no ;
  se.ver.grid    = na.ver.grid    ;
  se.ver.out     = na.ver.out     ;

  se.error       = na.error       ;
  se.error_unit  = na.error_unit  ;
  se.ave         = na.ave         ;
  se.period      = na.gd.time     ;
  se.m_test      = na.m_test      ;

  strcpy (se.rec_filename, na.rec_filename) ;

  w_Setup = NULL ;

  NA_CreateSetupWidget (na.graphtype) ;
}



/*****************************************************************************
  FUNCTION : NA_DControlCallbackProc

  PURPOSE  : display control callback function  
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_DControlCallbackProc (Widget w, 
                                     XtPointer client_data, 
                                     XtPointer call_data)

{
  float new_value ;


  if      (w == b_HorHalf)
  {  
    new_value = na.hor.min + (na.hor.max - na.hor.min) / 2 ;     
    if (na.hor.min < new_value) na.hor.max = new_value ;
  }

  else if (w == b_HorDouble)
  {
    new_value = na.hor.min + (na.hor.max - na.hor.min) * 2 ;     
    if (na.hor.min < new_value) na.hor.max = new_value ;
  } 

  else if (w == b_HorLeft)
  {
    new_value  = (na.hor.max - na.hor.min) / (float) na.hor.grid ;
    na.hor.min = na.hor.min - new_value ;
    na.hor.max = na.hor.max - new_value ;
  } 

  else if (w == b_HorLLeft)
  {
    new_value  = na.hor.max - na.hor.min ;
    na.hor.min = na.hor.min - new_value ;
    na.hor.max = na.hor.max - new_value ;
  } 

  else if (w == b_HorRight)
  {
    new_value  = (na.hor.max - na.hor.min) / (float) na.hor.grid ;
    na.hor.min = na.hor.min + new_value ;
    na.hor.max = na.hor.max + new_value ;
  } 

  else if (w == b_HorRRight)
  {
    new_value  = na.hor.max - na.hor.min ;
    na.hor.min = na.hor.min + new_value ;
    na.hor.max = na.hor.max + new_value ;
  } 

  else if (w == b_VerHalf)
  {
    new_value = na.ver.min + (na.ver.max - na.ver.min) / 2 ;     
    if (na.ver.min < new_value) na.ver.max = new_value ;
  } 

  else if (w == b_VerDouble)
  {
    new_value = na.ver.min + (na.ver.max - na.ver.min) * 2 ;     
    if (na.ver.min < new_value) na.ver.max = new_value ;
  } 

  else if (w == b_VerUp)
  {
    new_value  = (na.ver.max - na.ver.min) / (float) na.ver.grid ;
    na.ver.min = na.ver.min + new_value ;
    na.ver.max = na.ver.max + new_value ;
  } 

  else if (w == b_VerUUp)
  {
    new_value  = na.ver.max - na.ver.min ;
    na.ver.min = na.ver.min + new_value ;
    na.ver.max = na.ver.max + new_value ;
  } 

  else if (w == b_VerDown)
  {
    new_value  = (na.ver.max - na.ver.min) / (float) na.ver.grid ;
    na.ver.min = na.ver.min - new_value ;
    na.ver.max = na.ver.max - new_value ;
  } 

  else if (w == b_VerDDown)
  {
    new_value  = na.ver.max - na.ver.min ;
    na.ver.min = na.ver.min - new_value ;
    na.ver.max = na.ver.max - new_value ;
  } 

  else if (w == b_DoneDC)
  {
    XtDestroyWidget (w_DisplayControl) ;
    na_DisplayControlOpen = 0 ; 
    return ;
  } 

  na.hor.factor = (float) na.graph.w / (float) (na.hor.max - na.hor.min) ;
  na.ver.factor = (float) na.graph.h / (float) (na.ver.max - na.ver.min) ;

  NA_RedrawPixmap () ;
  NA_ShowPixmap   () ;
}


/*****************************************************************************
  FUNCTION : NA_OpenDisplayControl

  PURPOSE  : open network analyzer display control window 
  RETURNS  : void 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void NA_OpenDisplayControl (Widget w, 
                                   XtPointer client_data, 
                                   XtPointer call_data)

{
  Widget     w_box, w_form, w_dummy, w_top ;
  Dimension  width, height ;
  Cardinal   n  ;
  Arg        args[15] ;


  if (na_DisplayControlOpen != 0) return ;

  w_DisplayControl = 
    XtCreatePopupShell ("NA Display Control", 
                        topLevelShellWidgetClass, ui_toplevel, NULL, 0) ;

  w_box  =
    XtCreateManagedWidget ("box" , boxWidgetClass, w_DisplayControl, NULL, 0) ;

  w_form = XtCreateManagedWidget ("form", formWidgetClass, w_box, NULL, 0) ;
  w_top  = NULL ;

  w_dummy     = ui_xCreateLabelItem ("hor.: ", w_form, 6 * 8, NULL, w_top) ;
  b_HorHalf   = NA_CreateButtonItem ("half"  , w_form, w_dummy    , w_top) ;
  w_dummy     = ui_xCreateLabelItem (" ", w_form, 2*8, b_HorHalf  , w_top) ;
  b_HorLeft   = NA_CreateButtonItem ("left"  , w_form, w_dummy    , w_top) ;
  b_HorRight  = NA_CreateButtonItem ("right" , w_form, b_HorLeft , w_top) ;
  
  w_top = w_dummy ;
  w_dummy     = ui_xCreateLabelItem ("      ", w_form, 6 * 8, NULL, w_top) ;
  b_HorDouble = NA_CreateButtonItem ("double", w_form, w_dummy    , w_top) ;
  w_dummy     = ui_xCreateLabelItem (" ", w_form, 2*8, b_HorDouble, w_top) ;
  b_HorLLeft  = NA_CreateButtonItem ("lleft" , w_form, w_dummy    , w_top) ;
  b_HorRRight = NA_CreateButtonItem ("rright", w_form, b_HorLLeft , w_top) ;

  w_form = XtCreateManagedWidget ("form", formWidgetClass, w_box, NULL, 0) ;
  w_top  = NULL ;

  w_dummy     = ui_xCreateLabelItem ("ver.: ", w_form, 6 * 8, NULL, w_top) ;
  b_VerHalf   = NA_CreateButtonItem ("half"  , w_form, w_dummy    , w_top) ;
  w_dummy     = ui_xCreateLabelItem (" ", w_form, 2*8, b_VerHalf  , w_top) ;
  b_VerDown   = NA_CreateButtonItem ("down2" , w_form, w_dummy    , w_top) ;
  b_VerUp     = NA_CreateButtonItem ("up2"   , w_form, b_VerDown  , w_top) ;
  
  w_top = w_dummy ;
  w_dummy     = ui_xCreateLabelItem ("      ", w_form, 6 * 8, NULL, w_top) ;
  b_VerDouble = NA_CreateButtonItem ("double", w_form, w_dummy    , w_top) ;
  w_dummy     = ui_xCreateLabelItem (" ", w_form, 2*8, b_VerDouble, w_top) ;
  b_VerDDown  = NA_CreateButtonItem ("ddown" , w_form, w_dummy    , w_top) ;
  b_VerUUp    = NA_CreateButtonItem ("uup"   , w_form, b_VerDDown , w_top) ;
  
  b_DoneDC    = NA_CreateButtonItem ("done2" , w_box, NULL, w_form) ;

  XtAddCallback (b_HorHalf, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_HorDouble, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_HorLeft, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_HorLLeft, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_HorRight, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_HorRRight, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_VerHalf, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_VerDouble, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_VerUp, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_VerUUp, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_VerDown, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_VerDDown, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;
  XtAddCallback (b_DoneDC, XtNcallback, 
                 (XtCallbackProc) NA_DControlCallbackProc, NULL) ;

  ui_checkWindowPosition (w_DisplayControl);
  XtPopup (w_DisplayControl, XtGrabNone);

  n = 0 ;
  XtSetArg    (args[n], XtNwidth    , &width ) ; n++ ;
  XtSetArg    (args[n], XtNheight   , &height) ; n++ ;
  XtGetValues (w_DisplayControl, args, n) ;

  n = 0 ;
  XtSetArg    (args[n], XtNminWidth ,  width ) ; n++ ;
  XtSetArg    (args[n], XtNminHeight,  height) ; n++ ;
  XtSetArg    (args[n], XtNmaxWidth ,  width ) ; n++ ;
  XtSetArg    (args[n], XtNmaxHeight,  height) ; n++ ;
  XtSetValues (w_DisplayControl, args, n) ;

  na_DisplayControlOpen = 1 ;
}



/*****************************************************************************
                           E N D   O F   F I L E
******************************************************************************/




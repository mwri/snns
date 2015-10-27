/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_xUtils.c,v $
  SHORTNAME      : xUtils.c
  SNNS VERSION   : 4.2

  PURPOSE        : command and toggle buttons for the pannels
  NOTES          : analog to ui_xWidgets.c

  AUTHORS        : Tilman Sommer and Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:05 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include "ui.h"

#include <X11/Xlib.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>

#include "done.xbm"
#include "done2.xbm"
#include "control.xbm"
#include "testL.xbm"
#include "setup.xbm"
#include "units.xbm"
#include "links.xbm"
#include "light.xbm"
#include "clear.xbm"
#include "project.xbm"
#include "options.xbm"
#include "display.xbm"
#include "model.xbm"
#include "parallel.xbm"
#include "central.xbm"
#include "on.xbm"
#include "off.xbm"
#include "ok.xbm"
#include "up.xbm"
#include "down.xbm"
#include "prev.xbm"
#include "next.xbm"
#include "activation.xbm"
#include "initact.xbm"
#include "output.xbm"
#include "bias.xbm"
#include "name.xbm"
#include "number.xbm"
#include "zvalue.xbm"
#include "nothing.xbm"
#include "size.xbm"
#include "color.xbm"
#include "color2.xbm"
#include "label.xbm"
#include "toplabel.xbm"
#include "bottlabel.xbm"
#include "wire.xbm"
#include "solid.xbm"
#include "open.xbm"
#include "close.xbm"
#include "plus.xbm"
#include "minus.xbm"
#include "freeze.xbm"
#include "reset.xbm"
#include "small.xbm"
#include "big.xbm"

#include "glob_typ.h"
#include "ui_xWidgets.h"

#include "d3_global.h"

#include "d3_xUtils.ph"


/*****************************************************************************
  FUNCTION : d3_xCreateButtonItem

  PURPOSE  : creates a command button
  RETURNS  : the command widget
  NOTES    :

  UPDATE   :
******************************************************************************/

Widget d3_xCreateButtonItem (char *name, Widget parent, Widget left, Widget top)

{
    Cardinal n;
    Widget   w;
    Arg	     arg[15];
    
    n = 0;
    if (strcmp (name, "done") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   done_bits, done_width, done_height)); n++;
      } 
     if (strcmp (name, "done2") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   done2_bits, done2_width, done2_height)); n++;
      } 
    if (strcmp (name, "control") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   control_bits, control_width, control_height)); n++;
      } 
    if (strcmp (name, "testL") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   testL_bits, testL_width, testL_height)); n++;
      }
    if (strcmp (name, "setup") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   setup_bits, setup_width, setup_height)); n++;
      } 
    if (strcmp (name, "project") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   project_bits, project_width, project_height)); n++;
      } 
    if (strcmp (name, "links") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   links_bits, links_width, links_height)); n++;
      } 
    if (strcmp (name, "model") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   model_bits, model_width, model_height)); n++;
      } 
    if (strcmp (name, "light") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   light_bits, light_width, light_height)); n++;
      } 
    if (strcmp (name, "units") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   units_bits, units_width, units_height)); n++;
      } 
    if (strcmp (name, "options") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   options_bits, options_width, options_height)); n++;
      } 
    if (strcmp (name, "freeze") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   freeze_bits, freeze_width, freeze_height)); n++;
      } 
    if (strcmp (name, "display") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   display_bits, display_width, display_height)); n++;
      } 
    if (strcmp (name, "clear") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   clear_bits, clear_width, clear_height)); n++;
      } 
    if (strcmp (name, "ok") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   ok_bits, ok_width, ok_height)); n++;
      } 
    if (strcmp (name, "up") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   up_bits, up_width, up_height)); n++;
      } 
    if (strcmp (name, "down") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   down_bits, down_width, down_height)); n++;
      } 
    if (strcmp (name, "open") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   open_bits, open_width, open_height)); n++;
      } 
    if (strcmp (name, "close") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   close_bits, close_width, close_height)); n++;
      } 
    if (strcmp (name, "prev") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   prev_bits, prev_width, prev_height)); n++;
      } 
    if (strcmp (name, "next") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   next_bits, next_width, next_height)); n++;
      } 
    if (strcmp (name, "plus") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   plus_bits, plus_width, plus_height)); n++;
      } 
    if (strcmp (name, "minus") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   minus_bits, minus_width, minus_height)); n++;
      } 
    if (strcmp (name, "reset") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   reset_bits, reset_width, reset_height)); n++;
      } 
/*
    if (strcmp (name, "") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   _bits, _width, _height)); n++;
      } 
*/

    XtSetArg(arg[n], XtNborderWidth, 0); n++;
    XtSetArg(arg[n], XtNinternalHeight, 1); n++;
    XtSetArg(arg[n], XtNinternalWidth , 1); n++;

    XtSetArg(arg[n], XtNfromVert , top);  n++;
    XtSetArg(arg[n], XtNfromHoriz, left);  n++;
    XtSetArg(arg[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(arg[n], XtNright , XtChainLeft); n++;
    XtSetArg(arg[n], XtNtop   , XtChainTop); n++;
    XtSetArg(arg[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, commandWidgetClass, parent, arg, n);
    return(w);

}


/*****************************************************************************
  FUNCTION : d3_xCreateToggleItem

  PURPOSE  : creates a toggle button
  RETURNS  : the toggle widget
  NOTES    :

  UPDATE   :
******************************************************************************/

Widget  d3_xCreateToggleItem(char *name, Widget parent, Widget group, Widget left, Widget top)

{ 
    Cardinal n;
    Widget   w;
    Arg	     arg[15];

    n = 0;

    if (strcmp (name, "parallel") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   parallel_bits, parallel_width, parallel_height)); n++;
      } 
    if (strcmp (name, "central") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   central_bits, central_width, central_height)); n++;
      } 
    if (strcmp (name, "on") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   on_bits, on_width, on_height)); n++;
      } 
    if (strcmp (name, "off") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   off_bits, off_width, off_height)); n++;
      } 

    if (strcmp (name, "activation") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   activation_bits, activation_width, activation_height)); n++;
      }
    if (strcmp (name, "initact") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   initact_bits, initact_width, initact_height)); n++;
      } 
    if (strcmp (name, "output") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   output_bits, output_width, output_height)); n++;
      } 
    if (strcmp (name, "bias") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   bias_bits, bias_width, bias_height)); n++;
      } 
    if (strcmp (name, "name") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   name_bits, name_width, name_height)); n++;
      } 
    if (strcmp (name, "number") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   number_bits, number_width, number_height)); n++;
      } 
    if (strcmp (name, "zvalue") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   zvalue_bits, zvalue_width, zvalue_height)); n++;
      } 
    if (strcmp (name, "nothing") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   nothing_bits, nothing_width, nothing_height)); n++;
      } 
    if (strcmp (name, "size") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   size_bits, size_width, size_height)); n++;
      } 
    if (strcmp (name, "color") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   color_bits, color_width, color_height)); n++;
      } 
    if (strcmp (name, "color2") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   color2_bits, color2_width, color2_height)); n++;
      } 
    if (strcmp (name, "label") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   label_bits, label_width, label_height)); n++;
      } 
    if (strcmp (name, "toplabel") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   toplabel_bits, toplabel_width, toplabel_height)); n++;
      } 
    if (strcmp (name, "bottlabel") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   bottlabel_bits, bottlabel_width, bottlabel_height)); n++;
      } 
    if (strcmp (name, "freeze") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   freeze_bits, freeze_width, freeze_height)); n++;
      } 
    if (strcmp (name, "wire") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   wire_bits, wire_width, wire_height)); n++;
      } 
    if (strcmp (name, "solid") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   solid_bits, solid_width, solid_height)); n++;
      } 
    if (strcmp (name, "small") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   small_bits, small_width, small_height)); n++;
      } 
    if (strcmp (name, "big") == 0)
      {  
        XtSetArg(arg[n], XtNbitmap, 
                  (Pixmap) XCreateBitmapFromData (ui_display,
                   XDefaultRootWindow (ui_display), 
                   big_bits, big_width, big_height)); n++;
      } 

    XtSetArg(arg[n], XtNborderWidth, 0 /*!*/); n++;
    XtSetArg(arg[n], XtNradioGroup, group); n++;
    XtSetArg(arg[n], XtNinternalHeight, 1); n++;
    XtSetArg(arg[n], XtNinternalWidth , 1); n++;
    XtSetArg(arg[n], XtNfromVert , top);  n++;
    XtSetArg(arg[n], XtNfromHoriz, left);  n++;
    XtSetArg(arg[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(arg[n], XtNright , XtChainLeft); n++;
    XtSetArg(arg[n], XtNtop   , XtChainTop); n++;
    XtSetArg(arg[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, toggleWidgetClass, parent, arg, n);
    return(w);
}





/* end of file */
/* lines: 508 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_JordElm.c,v $
  SHORTNAME      : bn_JordElm.c
  SNNS VERSION   : 4.2

  PURPOSE        : Bignet panel for Jordan and Elman networks
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.13 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:36 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"
#include "ui_control.h"
#include "ui_confirmer.h"
#include "bn_basics.h"

#include "bn_JordElm.ph"



/*****************************************************************************
  FUNCTION : createUnitLayer

  PURPOSE  : creates a layer of no_of_units units 
  RETURNS  : the number of the first unit of the created layer or a kernel
             error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
static int createUnitLayer (int  no_of_units, 
  			    int  io_type,
		    	    char *act_func_name,
			    char *out_func_name,
			    char *unit_name)

{
    krui_err     error_code = KRERR_NO_ERROR ;
    int          i, unit_no, first_unit ;
    char         name[10] ;

  
    if (no_of_units < 1) return (KRERR_NO_UNITS) ;

    for (i = 1 ; i <= no_of_units ; i++){
	sprintf (name, "%s%d", unit_name, i) ;

	unit_no = krui_createUnit (name, out_func_name, act_func_name, 0, 0) ; 
	if (unit_no < 0) IF_ERROR_RETURN (unit_no) ;

	if (i == 1) first_unit = unit_no ;

	error_code = krui_setUnitTType (unit_no, io_type) ;

	IF_ERROR_RETURN (error_code) ;
    }

    return (first_unit) ;
}



/*****************************************************************************
  FUNCTION : connectFull

  PURPOSE  : connects every unit of one set (no_of_source_units units, 
             beginning with from_unit) to every unit of a second set 
             no_of_target_units units, beginning with to_unit)
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
static krui_err connectFull (int from_unit, int no_of_source_units,
			     int to_unit,   int no_of_target_units,
			     FlintType weight) 
{
    krui_err  error_code = KRERR_NO_ERROR ;
    int       i, j ;

  
    for (i = to_unit ; i < to_unit + no_of_target_units ; i++){
	error_code = krui_setCurrentUnit (i) ;
	IF_ERROR_RETURN (error_code) ;

	for (j = from_unit ; j < from_unit + no_of_source_units ; j++){
	    error_code = krui_createLink (j, weight) ;
	    IF_ERROR_RETURN (error_code) ;
	} 
    }

    return (error_code) ;
}



/*****************************************************************************
  FUNCTION : connectOneToOne

  PURPOSE  : connects every unit of one set (no_of_units units, beginning 
             with from_unit) to the corresponding unit in the second set
             (no_of_units units, beginning with to_unit)
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   :
******************************************************************************/
static krui_err connectOneToOne (int from_unit, int to_unit,
			         int no_of_units, FlintType weight) 
{
    krui_err  error_code = KRERR_NO_ERROR ;
    int       i ;


    for (i = 0 ; i < no_of_units ; i++){
	error_code = krui_setCurrentUnit (to_unit + i) ;
	IF_ERROR_RETURN (error_code) ;

	error_code = krui_createLink (from_unit + i, weight) ;
	IF_ERROR_RETURN (error_code) ;
    }

    return (error_code) ;
}



/*****************************************************************************
  FUNCTION : connectSelfRec

  PURPOSE  : connects every unit of a set (no_of_units units, beginning with
             unit_no) to itself
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
static krui_err connectSelfRec (int unit_no, int no_of_units, FlintType weight) 
{
    krui_err  error_code = KRERR_NO_ERROR ;
    int       i ;


    for (i = unit_no ; i < unit_no + no_of_units ; i++)
    {
	error_code = krui_setCurrentUnit (i) ;
	IF_ERROR_RETURN (error_code) ;
 
	error_code = krui_createLink (i, weight) ;
	IF_ERROR_RETURN (error_code) ;
    }
    return (error_code) ;
}



/*****************************************************************************
  FUNCTION : setPosRow

  PURPOSE  : sets the positions of a set of units (no_of_units units,
             beginning with unit_no) starting at (x, y) with max. r units
             per row
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void setPosRow (int unit_no, int no_of_units, int x, int y, int r)
{
    struct PosType  unit_pos ;
    int             col, row ;
    int             i ;
 

    col = x ;
    row = y ;

    for (i = unit_no ; i < unit_no + no_of_units ; i++){
	unit_pos.x = col ;
	unit_pos.y = row ;
	krui_setUnitPosition (i, &unit_pos) ;
	col++ ;
	if ((col - x) == r){
	    col = x ;
	    row++   ;
	}
    }
}



/*****************************************************************************
  FUNCTION : fixWindowSize 

  PURPOSE  : fixes the window size 
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void fixWindowSize (Widget window)
{
    Dimension  width, height ;
    int        n ;
    Arg        args[10] ;


    if (bn_elman_open != 0)
	XtMapWidget (baseWidgetElman) ;
    if (bn_jordan_open != 0) 
	XtMapWidget (baseWidgetJordan) ;
    n = 0 ;
    XtSetArg    (args[n], XtNwidth , &width ) ; n++ ;  
    XtSetArg    (args[n], XtNheight, &height) ; n++ ;
    XtGetValues (window , args, n) ;

    n = 0 ;
    XtSetArg    (args[n], XtNminWidth , width ) ; n++ ;  
    XtSetArg    (args[n], XtNminHeight, height) ; n++ ;
    XtSetArg    (args[n], XtNmaxWidth , width ) ; n++ ;  
    XtSetArg    (args[n], XtNmaxHeight, height) ; n++ ;

    XtSetValues (window , args, n) ;
}

 

/*****************************************************************************
  FUNCTION : bn_jordan_createNet

  PURPOSE  : generates the JORDAN network 
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   :
******************************************************************************/
static krui_err bn_jordan_createNet (int IUnits, int HUnits, int OUnits,
                                     int ICols , int HCols , int OCols )
{
  krui_err  error_code = KRERR_NO_ERROR;
  int       i_unit, h_unit, o_unit, s_unit ;
  int       idy, hdy, maxdy ;

 
  /* ---------------------------   create units  ----------------------------*/

  i_unit = createUnitLayer(IUnits,INPUT,"Act_Identity","Out_Identity","inp");
  if (i_unit < 0) IF_ERROR_RETURN (i_unit) ;

  h_unit = createUnitLayer(HUnits,HIDDEN,"Act_Logistic","Out_Identity","hid") ;
  if (h_unit < 0) IF_ERROR_RETURN (h_unit) ;

  o_unit = createUnitLayer(OUnits,OUTPUT,"Act_Logistic","Out_Identity","out") ;
  if (o_unit < 0) IF_ERROR_RETURN (o_unit) ;

  s_unit =createUnitLayer(OUnits,SPECIAL_H,"Act_Identity","Out_Identity","con");
  if (s_unit < 0) IF_ERROR_RETURN (s_unit) ;


  /* ---------------------------   create links  ----------------------------*/

  error_code = connectFull     (i_unit, IUnits, h_unit, HUnits, 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = connectFull     (h_unit, HUnits, o_unit, OUnits, 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = connectFull     (s_unit, OUnits, h_unit, HUnits, 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = connectOneToOne (o_unit, s_unit, OUnits, 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = connectSelfRec  (s_unit, OUnits, 0.0) ; 
  IF_ERROR_RETURN (error_code) ;


  /* -------------  calculate y-coordinate of state layer  ------------------*/

  idy = (int) ((IUnits + ICols - 1) / ICols) ;
  hdy = (int) ((HUnits + HCols - 1) / HCols) ;

  if (idy > hdy) maxdy = idy ;
  else           maxdy = hdy ;


  /*  --------------------------  set positions  ----------------------------*/

  setPosRow (i_unit, IUnits, 1                  , 1        , ICols) ;
  setPosRow (h_unit, HUnits, 5+ICols+OCols      , 1        , HCols) ;
  setPosRow (o_unit, OUnits, 9+ICols+HCols+OCols, 1        , OCols) ;
  setPosRow (s_unit, OUnits, 3+ICols            , 1 + maxdy, OCols) ;


  /*  -----------------------  set default functions  -----------------------*/

  error_code = krui_setLearnFunc ("JE_BP") ;
  IF_ERROR_RETURN (error_code) ;

  error_code = krui_setInitialisationFunc ("JE_Weights") ;
  IF_ERROR_RETURN (error_code) ;

  ui_NumberOfLearnParamsChanged();
  ui_NumberOfUpdateParamsChanged();
  ui_NumberOfInitParamsChanged();

  error_code = krui_setUpdateFunc ("JE_Order") ;

  return (error_code) ;
} 



/*****************************************************************************
  FUNCTION : bn_jordan_donePROC

  PURPOSE  : callback function for DONE button in 'Bignet (Jordan)' 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void bn_jordan_donePROC (void)
{
   XtDestroyWidget (baseWidgetJordan) ;
   bn_jordan_open = 0 ;
}



/*****************************************************************************
  FUNCTION : bn_jordan_createPROC

  PURPOSE  : callback function for CREATE NET button in 'BigNet (Jordan)'
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void bn_jordan_createPROC (void)
{
  int  units[3], cols[3] ;
  int  i ;
  char buf[80] ;


  if (bn_basics_check_existingNetwork()) 
  {
    /* -------------------------  get values  ------------------------------ */

    for (i=0; i<3; i++)
    {
      units[i] = ui_xIntFromAsciiWidget (jordanUnitWidget[i]) ;
      cols[i]  = ui_xIntFromAsciiWidget (jordanColWidget[i])  ;
    } /*for*/


    /* ------------------------  check values  ----------------------------- */

    for (i=0; i<3; i++)
    {
      if (units[i] < 1)
      {
        sprintf (buf,"Number of units has to be greater than 0 in line %d!",
                 i + 1) ;
        ui_confirmOk (buf);
        return ;
      } 

      if (cols[i] < 1)
      {
        sprintf (buf,"Number of cols has to be greater than 0 in line %d!",
                 i + 1);
        ui_confirmOk (buf) ;
        return ;
      } 
    }


    /* -------------------------  create net  ------------------------------ */

    if (bn_jordan_createNet (units[0], units[1], units[2], cols[0] , 
                             cols[1] , cols[2] ) != KRERR_NO_ERROR) 
      ui_confirmOk ("create net :  internal error") ;
     
    bn_basics_refresh () ;
  } 

  return ;
}




/*****************************************************************************
  FUNCTION : bn_create_jordan

  PURPOSE  : creates the 'BigNet (Jordan)' window
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

void bn_create_jordan(void) 

{
    Widget      jordan_panel, jordan_form, button, w_left, w_top ;
    int         i;
    char        Name[4][20];

  
    strcpy (Name[0], "Input  Layer: ") ;
    strcpy (Name[1], "Hidden Layer: ") ;
    strcpy (Name[2], "Output Layer: ") ;


    if(bn_jordan_open) {
	XRaiseWindow (XtDisplay (baseWidgetJordan), 
		      XtWindow (baseWidgetJordan));
	return;
    }
    baseWidgetJordan   = 
	XtCreatePopupShell ("SNNS BigNet (Jordan)", topLevelShellWidgetClass,
			    ui_toplevel, NULL, 0) ;

    jordan_form  =
	XtCreateManagedWidget ("box" , boxWidgetClass , baseWidgetJordan , 
			       NULL, 0) ;

    jordan_panel =
	XtCreateManagedWidget ("form", formWidgetClass, jordan_form, NULL, 0) ;
  

    /* ----------------------------  headline  -----------------------------*/
     
    w_left = w_top =
	ui_xCreateLabelItem (" "           , jordan_panel, 12*8, NULL  , NULL);
    w_left = 
	ui_xCreateLabelItem ("No. of Units", jordan_panel, 12*8, w_left, NULL);
    w_left = 
	ui_xCreateLabelItem ("No. of Col." , jordan_panel, 12*8, w_left, NULL);


    /* --------------------------  dialog rows  ----------------------------*/

    for (i = 0 ; i < 3 ; i++)
    {
	w_left =
	    ui_xCreateLabelItem  (Name[i], jordan_panel, 12*8, NULL  , w_top) ;
  
	w_left = 
	    ui_xCreateLabelItem  (" "    , jordan_panel,  2*8, w_left, w_top) ;
 
	w_left = jordanUnitWidget[i] = 
	    ui_xCreateDialogItem (" ", jordan_panel, "" , 6*8, w_left, w_top) ;
 
	w_left =
	    ui_xCreateLabelItem  (" "    , jordan_panel,  5*8, w_left, w_top) ;
 
	w_left = jordanColWidget[i]  = 
	    ui_xCreateDialogItem (" ", jordan_panel, "1", 6*8, w_left, w_top) ;
  
	w_top  = w_left ;
    } 
       

    /* -----------------------------  buttons  -----------------------------*/

    button = ui_xCreateButtonItem ("create_net", jordan_form, NULL  , w_top) ;
    XtAddCallback(button,XtNcallback,(XtCallbackProc)bn_jordan_createPROC,NULL);

    button = ui_xCreateButtonItem ("done"      , jordan_form, button, w_top) ;
    XtAddCallback(button, XtNcallback,(XtCallbackProc)bn_jordan_donePROC, NULL);
 
    ui_checkWindowPosition(baseWidgetJordan) ;
    XtPopup (baseWidgetJordan, XtGrabNone) ;

    bn_jordan_open = 1 ;

    fixWindowSize (baseWidgetJordan) ;

  
}




/*****************************************************************************
  FUNCTION : bn_elman_createNet

  PURPOSE  : generates the ELMAN network 
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/

static krui_err bn_elman_createNet (void)

{
  krui_err  error_code = KRERR_NO_ERROR;
  int       i_unit, o_unit ;
  int       h_unit[MAX_NO_OF_LAYERS], c_unit[MAX_NO_OF_LAYERS] ;
  int       i, no_of_context_layers ;
  int       no_of_hidden_layers, o_layer ;
  int       dx, cx, cy ;
  char      name[10] ;


  no_of_hidden_layers = no_of_layers - 2 ;
  o_layer             = no_of_layers - 1 ;   /* no. of the output layer */


  /* ---------------------------   create units  ----------------------------*/

  i_unit = createUnitLayer (layer[0], INPUT, "Act_Identity", 
                            "Out_Identity", "inp") ;
  if (i_unit < 0) IF_ERROR_RETURN (i_unit) ;

  strcpy (name, "hid") ;
  for (i = 1 ; i <= no_of_hidden_layers ; i++)
  {
    if (no_of_hidden_layers > 1) sprintf (name, "hid%d", i) ;  
    h_unit[i] = createUnitLayer(layer[i], HIDDEN, "Act_Logistic", 
                                "Out_Identity", name) ;
    if (h_unit[i] < 0) IF_ERROR_RETURN (h_unit[i]) ;
  }

  o_unit = createUnitLayer (layer[o_layer], OUTPUT,  "Act_Logistic", 
                            "Out_Identity", "out") ;
  if (o_unit < 0) IF_ERROR_RETURN (o_unit) ;

  if (out_context) 
  {
    no_of_context_layers = no_of_hidden_layers + 1 ;
    h_unit[no_of_context_layers] = o_unit ;
  }
  else 
    no_of_context_layers = no_of_hidden_layers ;

  strcpy (name, "con") ;
  for (i = 1 ; i <= no_of_context_layers ; i++)
  {
    if (no_of_context_layers > 1) sprintf (name, "con%d", i) ;  
    c_unit[i] = createUnitLayer (layer[i], SPECIAL_H, "Act_Identity", 
                                 "Out_Identity", name ) ;
    if (c_unit[i] < 0) IF_ERROR_RETURN (c_unit[i]) ;
  }


  /* ---------------------------   create links  ----------------------------*/

  error_code = connectFull (i_unit, layer[0], h_unit[1], layer[1], 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  for (i = 1 ; i < no_of_hidden_layers ; i++)
  {
    error_code = 
      connectFull (h_unit[i], layer[i], h_unit[i+1], layer[i+1], 0) ;
    IF_ERROR_RETURN (error_code) ;
  }

  error_code = connectFull (h_unit[i], layer[i], o_unit, layer[o_layer], 0) ;
  IF_ERROR_RETURN (error_code) ;
  
  for (i = 1 ; i <= no_of_context_layers ; i++)
  {
    error_code = connectFull (c_unit[i], layer[i],  h_unit[i], layer[i], 0) ;
    IF_ERROR_RETURN (error_code) ;

    error_code = connectOneToOne (h_unit[i], c_unit[i], layer[i], 0) ;
    IF_ERROR_RETURN (error_code) ;

    error_code = connectSelfRec (c_unit[i], layer[i], 0) ;
    IF_ERROR_RETURN (error_code) ;
  }
  

  /* --------------------------   set positions  ----------------------------*/

  cx = 1 ; /* position of the upper left */
  cy = 1 ; /* corner of a context layer  */

  for (i = 0 ; i < no_of_layers ; i++)
  {
    if ((int) (layer[i] / columns[i]) > cy - 1)
      cy = 1 + (int) (layer[i] / columns[i]) ;
  }

  cy += 1 ;

  dx  = 1 ; /* x position of the upper left corner of an input, */
            /* hidden or output layer                           */

  setPosRow (i_unit, layer[0], 1, 1, columns[0]) ;

  cx = dx + columns[0] + 2 ;
  dx = cx + columns[1] + 2 ;

  for (i = 1 ; i <= no_of_context_layers ; i++)
  {
    setPosRow (h_unit[i], layer[i],  dx, 1 , columns[i]) ;
    setPosRow (c_unit[i], layer[i],  cx, cy, columns[i]) ;

    cx = dx + columns[i]   + 2 ;
    dx = cx + columns[i+1] + 2 ;
  }

  if (! out_context)
    setPosRow (o_unit, layer[o_layer], dx, 1, columns[o_layer]) ;


  /* ----------------------   set default functions  ------------------------*/

  error_code = krui_setLearnFunc ("JE_BP") ;
  IF_ERROR_RETURN (error_code) ;

  error_code = krui_setInitialisationFunc ("JE_Weights") ;
  IF_ERROR_RETURN (error_code) ;

  error_code = krui_setUpdateFunc ("JE_Order") ;

  ui_NumberOfLearnParamsChanged();
  ui_NumberOfUpdateParamsChanged();
  ui_NumberOfInitParamsChanged();
  return (error_code) ;
}



/*****************************************************************************
  FUNCTION : bn_elman_getFromWidget 

  PURPOSE  : get values from 'BigNet (Elman)' window
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void bn_elman_getFromWidget (void)

{
  int i ;

  for (i = 0 ; i < no_of_layers ; i++)
  {
    layer[i]   = ui_xIntFromAsciiWidget (elmanUnitWidget[i]) ;
    columns[i] = ui_xIntFromAsciiWidget (elmanColWidget[i] ) ;
  }

  out_context = ui_xGetToggleState (t_yes) ;
}



/*****************************************************************************
  FUNCTION : bn_elman_donePROC

  PURPOSE  : callback function for DONE button in 'BigNet (Elman)'
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void bn_elman_donePROC (void)

{
   XtDestroyWidget (baseWidgetElman) ;
   bn_elman_open = 0 ;
}



/*****************************************************************************
  FUNCTION : bn_elman_createPROC

  PURPOSE  : callback function for CREATE NET button in 'BigNet (Elman)'
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void bn_elman_createPROC (void)

{
  int  i       ;
  char buf[80] ;


  if (bn_basics_check_existingNetwork())
  {
    bn_elman_getFromWidget () ;

    for (i = 0 ; i < no_of_layers ; i++)
    {

      /* -------------------------  check values  -------------------------- */

      if (layer[i] < 1)
      {
        sprintf (buf, "Number of units has to be greater than 0 in line %d!",
                 i + 1) ;
        ui_confirmOk (buf) ;
        return ;
      } 

      if (columns[i] < 1)
      {
        sprintf (buf, "Number of columns has to be greater than 0 in line %d!",
                 i + 1) ;
        ui_confirmOk (buf) ;
        return ;
      } 
    }
   
   if (bn_elman_createNet () != KRERR_NO_ERROR)
     ui_confirmOk ("create net :  internal error") ;

   bn_basics_refresh  () ;
  } 

  return ;
}



/*****************************************************************************
  FUNCTION : bn_elman_create_dialogrow

  PURPOSE  : creates a dialog row in the 'BigNet (Elman)' window
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void bn_elman_create_dialogrow (int i, char *str) 

{
  Widget w_top, w_left ;
  char   buf[256] ;


  if (i == 0) w_top = w_headline ;
  else        w_top = elmanLabelWidget [i - 1] ;

  sprintf (buf, "  %3d      %s", i+1, str) ;

  w_left = elmanLabelWidget[i] =
    ui_xCreateLabelItem (buf, elman_form[0], 19 * 8, NULL, w_top) ;

  if (layer[i] == 0) strcpy  (buf, "") ;
  else               sprintf (buf, "%d", layer[i]) ;

  w_left = elmanUnitWidget[i] = 
    ui_xCreateDialogItem (" ", elman_form[0], buf, 6 * 8, w_left, w_top) ;

  w_left = elmanSpaceWidget[i] =
    ui_xCreateLabelItem  (" ", elman_form[0], 5 * 8, w_left, w_top) ;

  if (columns[i] == 0) strcpy  (buf, "") ;
  else                 sprintf (buf, "%d", columns[i]) ;

  w_left = w_top = elmanColWidget[i]  = 
    ui_xCreateDialogItem (" ", elman_form[0], buf, 6 * 8, w_left, w_top) ;

}



/*****************************************************************************
  FUNCTION : bn_elman_yesPROC

  PURPOSE  : callback function for YES button in 'BigNet (Elman)'
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void bn_elman_yesPROC (void)

{
  if (! ui_xGetToggleState (t_yes)) ui_xSetToggleState (t_yes, TRUE ) ;
  else                              ui_xSetToggleState (t_no , FALSE) ;
}



/*****************************************************************************
  FUNCTION : bn_elman_noPROC

  PURPOSE  : callback function for NO button in 'BigNet (Elman)'
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void bn_elman_noPROC (void)

{
  if (! ui_xGetToggleState (t_no)) ui_xSetToggleState (t_no , TRUE ) ;
  else                             ui_xSetToggleState (t_yes, FALSE) ;
}



/*****************************************************************************
  FUNCTION : bn_elman_insertPROC

  PURPOSE  : callback function for INSERT button in 'BigNet (Elman)'
             inserts a new hidden layer
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/

static void bn_elman_insertPROC (void)

{
  if (no_of_layers < MAX_NO_OF_LAYERS)
  {
    bn_elman_getFromWidget () ;

    layer[no_of_layers]     = layer[no_of_layers - 1] ;
    layer[no_of_layers - 1] = 0 ;

    columns[no_of_layers]     = columns[no_of_layers - 1] ;
    columns[no_of_layers - 1] = 1 ;


    /* ----------    delete output layer     ---------- */

    XtDestroyWidget (elmanLabelWidget[no_of_layers - 1]) ;
    XtDestroyWidget (elmanUnitWidget [no_of_layers - 1]) ;
    XtDestroyWidget (elmanSpaceWidget[no_of_layers - 1]) ;
    XtDestroyWidget (elmanColWidget  [no_of_layers - 1]) ;


    /* -------  insert hidden and output layer  ------- */

    no_of_layers++ ;
    bn_elman_create_dialogrow (no_of_layers - 2, "hidden") ;
    bn_elman_create_dialogrow (no_of_layers - 1, "output") ;

    fixWindowSize (baseWidgetElman) ;
  }
  else
    ui_confirmOk ("No more hidden layers are supported !") ;
}



/*****************************************************************************
  FUNCTION : bn_elman_deletePROC

  PURPOSE  : callback function for DELETE button in 'BigNet (Elman)'
             deletes the last hidden layer
  NOTES    :

  UPDATE   :
******************************************************************************/

static void bn_elman_deletePROC (void)

{
  if (no_of_layers > 3)
  {
    bn_elman_getFromWidget () ;

    no_of_layers-- ;

    layer[no_of_layers - 1]   = layer[no_of_layers] ;
    layer[no_of_layers]       = 0 ;

    columns[no_of_layers - 1] = columns[no_of_layers] ;
    columns[no_of_layers]     = 1 ;


    /* ----------  delete last hidden layer  ---------- */

    XtDestroyWidget (elmanLabelWidget[no_of_layers - 1]) ;
    XtDestroyWidget (elmanUnitWidget [no_of_layers - 1]) ;
    XtDestroyWidget (elmanSpaceWidget[no_of_layers - 1]) ;
    XtDestroyWidget (elmanColWidget  [no_of_layers - 1]) ;


    /* ----------    delete output layer     ---------- */

    XtDestroyWidget (elmanLabelWidget[no_of_layers]) ;
    XtDestroyWidget (elmanUnitWidget [no_of_layers]) ;
    XtDestroyWidget (elmanSpaceWidget[no_of_layers]) ;
    XtDestroyWidget (elmanColWidget  [no_of_layers]) ;


    /* ----------    insert output layer     ---------- */

    bn_elman_create_dialogrow (no_of_layers - 1, "output") ;

    fixWindowSize (baseWidgetElman) ;
  }
  else
    ui_confirmOk ("You can not delete the last hidden layer !") ; 
}



/*****************************************************************************
  FUNCTION : bn_create_elman

  PURPOSE  : creates the 'BigNet (Elman)' window
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/

void bn_create_elman (void)

{
  Widget      button, w_left, w_top ;
  int         i;


  if(bn_elman_open) {
      XRaiseWindow (XtDisplay (baseWidgetElman), XtWindow (baseWidgetElman));
      return;
  }

    no_of_layers = 3 ;                  /* 1 input, 1 hidden, 1 output layer */
 
    for (i = 0 ; i < MAX_NO_OF_LAYERS ; i++)
    {
      layer[i]   = 0 ;
      columns[i] = 1 ;
    }

    baseWidgetElman    = XtCreatePopupShell ("SNNS BigNet (Elman)",
                                        topLevelShellWidgetClass,
                                        ui_toplevel, NULL, 0);

    elman_box     = XtCreateManagedWidget ("box" , boxWidgetClass , 
                                           baseWidgetElman, NULL , 0) ;

    elman_form[0] = XtCreateManagedWidget ("form", formWidgetClass, 
                                           elman_box, NULL , 0) ;


    /* -----------------------------  headline  ----------------------------*/

    w_left = w_top = w_headline =
      ui_xCreateLabelItem ("Layer No.  Type    No. of Units  No. of Col.  ",
                           elman_form[0], (9 + 8 + 24) * 8, NULL  , NULL) ;


    /* ---------------------------  dialog rows  ---------------------------*/

    for (i = 0 ; i < no_of_layers ; i++)
    {
      if      (i == 0)              bn_elman_create_dialogrow (i, "input ") ;
      else if (i == no_of_layers-1) bn_elman_create_dialogrow (i, "output") ;
      else                          bn_elman_create_dialogrow (i, "hidden") ;
    } 


    /* -----------------  output context :  yes / no  ----------------------*/

    elman_form[1] = XtCreateManagedWidget ("form", formWidgetClass, elman_box,
					   NULL , 0) ;

    w_left = ui_xCreateLabelItem ("Output Context :", elman_form[1], 
                                  20 * 8, NULL, NULL) ;

    t_yes =
      ui_xCreateToggleItem ("yes", elman_form[1], NULL, w_left, NULL) ;
      XtAddCallback (t_yes, XtNcallback,
                     (XtCallbackProc) bn_elman_yesPROC, NULL) ; 

    w_left = ui_xCreateLabelItem (" ", elman_form[1], 7 * 8, t_yes, 
                                  NULL) ;

    t_no =
      ui_xCreateToggleItem ("no", elman_form[1], NULL, w_left, NULL) ;
      XtAddCallback (t_no, XtNcallback,
                     (XtCallbackProc) bn_elman_noPROC, NULL) ; 

    w_left = ui_xCreateLabelItem (" ", elman_form[1], 3 * 8 + 4, t_no, 
                                  NULL) ;

    ui_xSetToggleState (t_yes, FALSE) ;
    ui_xSetToggleState (t_no , TRUE ) ;


    /* --------------  hidden layers :  insert / delete  -------------------*/

    elman_form[2] = XtCreateManagedWidget ("form", formWidgetClass, elman_box,
					   NULL , 0) ;

    w_left = ui_xCreateLabelItem ("Hidden Layers  :", elman_form[2], 
                                  18 * 8 + 5, NULL, NULL) ;

    button = ui_xCreateButtonItem ("insert", elman_form[2], w_left, NULL) ;
    XtAddCallback (button, XtNcallback,
		   (XtCallbackProc) bn_elman_insertPROC, NULL) ; 

    w_left = ui_xCreateLabelItem (" ", elman_form[2], 4 * 8 + 5, button, 
                                  NULL) ;

    button = ui_xCreateButtonItem ("delete", elman_form[2], w_left, NULL) ;
    XtAddCallback (button, XtNcallback,
                     (XtCallbackProc) bn_elman_deletePROC, NULL) ; 

    w_left = ui_xCreateLabelItem (" ", elman_form[2], 2 * 8 + 4, button, 
                                  NULL) ;


    /* -----------------------  done / create  -----------------------------*/

    button = ui_xCreateButtonItem ("create_net", elman_box, NULL  , 
                                   elman_form[2]) ;
      XtAddCallback (button, XtNcallback,
                     (XtCallbackProc) bn_elman_createPROC, NULL) ; 
 
    button = ui_xCreateButtonItem ("done"      , elman_box, button,
                                   elman_form[2]) ;
      XtAddCallback (button, XtNcallback,
                     (XtCallbackProc) bn_elman_donePROC  , NULL) ; 

    ui_checkWindowPosition (baseWidgetElman) ;
    XtPopup (baseWidgetElman, XtGrabNone) ; 

    fixWindowSize (baseWidgetElman) ;

    bn_elman_open = 1 ;
}






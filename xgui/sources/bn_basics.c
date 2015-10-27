/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_basics.c,v $
  SHORTNAME      : bn_basics
  SNNS VERSION   : 4.2

  PURPOSE        : Provides general procedures for the BigNet Tool
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : Ralf Huebner
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:42 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"
#include "ui_fileP.h"
#include "ui_display.h"
#include "ui_event.h"
#include "ui_selection.h"
#include "ui_netUpdate.h"
#include "ui_status.h"
#include "ui_confirmer.h"

#include "bn_basics.ph"




/*****************************************************************************
  FUNCTION : bn_basics_createART

  PURPOSE  : create ART1 panel
  NOTES    : calling function has to allocate memory for unitWidget and
             rowWidget arrays.

  UPDATE   : 20.1.1993
******************************************************************************/
void bn_basics_createART(int model, Widget *baseWidget,int *already_open,
                         Widget unitWidget[], Widget rowWidget[],
                         XtCallbackProc   CreateCallbackProc,
                         XtCallbackProc   DoneCallbackProc )
{
    Widget      artpanel, artform, button, dummy1x, dummy1y;
    Widget      dummy2x, dummy3, dummy4, dummy5;
    Arg		args[10];
    Cardinal	n = 0;
    char        buf[40];
    int         colwidth = 12;
    int         Width, Height;
    int         NoOfDialogRows;
    int         i;
    char        Name[4][20];
    char        ModelName[20];
    
    switch (model) {
    case ART1_MODEL:
	sprintf(ModelName,"ART 1");
	Width = 314;
	Height = 103;
	NoOfDialogRows = 2;
	strcpy(Name[0],"F1 layer: ");
	strcpy(Name[1],"F2 layer: ");
	break;
    case ART2_MODEL:
	sprintf(ModelName,"ART 2");
	Width = 314;
	Height = 124;
	NoOfDialogRows = 2;
	strcpy(Name[0],"F1 layer: ");
	strcpy(Name[1],"F2 layer: ");
	break;
    case ARTMAP_MODEL:
	sprintf(ModelName,"ARTMAP");
	Width = 314;
	Height = 149;
	NoOfDialogRows = 4;
	strcpy(Name[0],"F1a layer: ");
	strcpy(Name[1],"F2a layer: ");
	strcpy(Name[2],"F1b layer: ");
	strcpy(Name[3],"F2b layer: ");
	break;
    default:
	*baseWidget = NULL;
	*already_open = 0;
	return;
    } /*switch*/

    if(*already_open) {
	XRaiseWindow (XtDisplay (*baseWidget), XtWindow (*baseWidget));
	return;
    }

    sprintf(buf,"SNNS BigNet (%s)", ModelName);
       

    *baseWidget = 
	XtCreatePopupShell(buf, topLevelShellWidgetClass, ui_toplevel,
			   args, n);

    artform = XtCreateManagedWidget("box", boxWidgetClass, *baseWidget, args,n);
    artpanel = XtCreateManagedWidget("form", formWidgetClass, artform, NULL, 0);

       
    /* Headings */
    dummy1y = dummy1x =
	ui_xCreateLabelItem (" ",artpanel, colwidth*8, NULL, NULL);
    dummy2x = ui_xCreateLabelItem ("No. of units", artpanel, colwidth*8, 
				   dummy1x, NULL);
    dummy3 = ui_xCreateLabelItem ("No. of rows", artpanel, colwidth*8, 
				  dummy2x, NULL);

    /* dialog rows */
    for (i=0; i<NoOfDialogRows; i++) {

	dummy3 = 
            ui_xCreateLabelItem (Name[i], artpanel, colwidth*8, NULL, dummy1y);
	dummy4 = ui_xCreateLabelItem (" ", artpanel, 2*8, dummy1x,dummy1y);
	unitWidget[i] = 
            ui_xCreateDialogItem (" ", artpanel, "",  6*8, dummy4, dummy1y);
	dummy5 = ui_xCreateLabelItem (" ", artpanel, 2*8, dummy2x, dummy1y);
	rowWidget[i]  = dummy1y = 
            ui_xCreateDialogItem (" ", artpanel, "",  6*8, dummy5, dummy1y);
         
    } /*for*/
       
    dummy3 = dummy5;

    /* buttons have to be defined by calling procedure */
    button = ui_xCreateButtonItem ("create_net", artform, NULL, dummy3);
    XtAddCallback (button, XtNcallback, CreateCallbackProc, NULL); 
    button = ui_xCreateButtonItem ("done", artform, button, dummy3);
    XtAddCallback (button, XtNcallback,DoneCallbackProc, NULL); 

    ui_checkWindowPosition(*baseWidget);
    XtPopup (*baseWidget, XtGrabNone); 
    ui_xDontResizeWidget (*baseWidget);

    *already_open = 1;

}




/*****************************************************************************
  FUNCTION : bn_basics_refresh();

  PURPOSE  : refresh Dispaly
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/
void bn_basics_refresh (void)
{

   ui_sel_resetList();
   ui_net_completeRefresh(ui_currentDisplay,2);
   ui_stat_displayStatus(ui_gridPosMouse);
   strcpy(ui_filenameNET,"SNNS_NET");
   strcpy(ui_filenameSLNET,"SNNS_NET");
   ui_file_updateShellLabels();

}



/*****************************************************************************
  FUNCTION : bn_basics_check_existingNetwork();

  PURPOSE  : check if network exists and if so, as user if he wants to erase
             it by creating the new one
  NOTES    : returns TRUE, if no current network exists or user pressed YES.
             returns FALSE, if current network exists and user pressed NO.

  UPDATE   : 20.1.1993
******************************************************************************/
int bn_basics_check_existingNetwork (void)
{

  if (krui_getNoOfUnits()) { /* There is an existing network */

     if ( ! ui_confirmYes ("Create will erase current network. Create?")) {
        return 0;
     } else {
        krui_deleteNet();
        return 1;
     } /*if*/

  } else { /* There is no existing network */

      return 1;

  } /*if*/

} 



/*****************************************************************************
  FUNCTION : bn_basics_getValues();

  PURPOSE  : read out user values from dialog widgets
  NOTES    : calling function has to allocate memory for unit and row array

  UPDATE   : 20.1.1993
******************************************************************************/

void bn_basics_getValues (int NoOfLayers, int units[], int rows[],
                          Widget unitWidget[], Widget rowWidget[])

{
   int i;

   for (i=0; i<NoOfLayers; i++) {
      units[i] = ui_xIntFromAsciiWidget(unitWidget[i]);
      rows[i]  = ui_xIntFromAsciiWidget(rowWidget[i]);
   } /*for*/

   return;

}


/*****************************************************************************
  FUNCTION : bn_basics_checkValues();

  PURPOSE  : check the values, the user entered
  NOTES    : returns TRUE, if values OK
             returns FALSE, if illegal values have been entered.
             calling function has to allocate memory for unit and row array

  UPDATE   : 20.1.1993
******************************************************************************/

int bn_basics_checkValues (int NoOfLayers, int units[], int rows[])

{
   int  i;
   char buf[80];

   for (i=0; i<NoOfLayers; i++) {

     if (units[i] < 1) {
        sprintf (buf,"Number of units has to be greater than 0 in line %d!",i+1);
        ui_confirmOk (buf);
        return 0;
     } /*if*/

     if (rows[i] < 1) {
        sprintf (buf,"Number of rows has to be greater than 0 in line %d!",i+1);
        ui_confirmOk (buf);
        return 0;
     } /*if*/

     if (units[i] < rows[i]) {
        sprintf(buf,
                "Number of rows must not be greater\nthan number of units in line %d!",
                i+1);
        ui_confirmOk (buf);
        return 0;
     } /*if*/

   } /*for*/

   return 1;

} 

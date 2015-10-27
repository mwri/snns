/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_status.c,v $
  SHORTNAME      : status.c
  SNNS VERSION   : 4.2

  PURPOSE        : display and refresh of status information in graphic windows
  NOTES          :


  AUTHOR         : Tilman Sommer
  DATE           : 27.8.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>

#include "ui.h"
#include "ui_utilP.h"
#include "ui_xWidgets.h"
#include "ui_key.h"
#include "ui_action.h"
#include "ui_selection.h"
#include "ui_main.h"

#include "ui_status.ph"



/*****************************************************************************
  FUNCTION : ui_stat_displayStatus

  PURPOSE  : display status information.
  RETURNS  : alteration of the status panel
  NOTES    : Except the current raster position, which is taken from the
             parameter, all information is got out from global variables.

  UPDATE   :
*****************************************************************************/

void ui_stat_displayStatus(struct PosType gridPos)

{
    char   string[40];

    if (ui_stat_oldFlags != ui_safetyFlg) {
	if (ui_safetyFlg)
	    sprintf(string,": safe");
	else
	    sprintf(string,": -   ");
	ui_xSetLabel(ui_stat_flagsWidget,string);
	ui_stat_oldFlags = ui_safetyFlg;
    }

    if (ui_stat_oldNumberOfSelectedItems != ui_sel_numberOfSelectedItems) {
	sprintf(string,": %-4d", ui_sel_numberOfSelectedItems);
	ui_xSetLabel(ui_stat_selNoWidget,string);
	ui_stat_oldNumberOfSelectedItems = ui_sel_numberOfSelectedItems;
    }

    if (ui_utilAreDifferentPositions(gridPos, ui_stat_oldPos)) {
	sprintf(string,"x:%-6d  y:%-6d",gridPos.x, gridPos.y);
	ui_xSetLabel(ui_stat_posWidget,string);
	ui_stat_oldPos = gridPos;
    }
}









/* end of file */
/* lines: 77 */

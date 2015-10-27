/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_status.ph,v $
  SHORTNAME      : status.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_STATUS_DEFINED_
#define _UI_STATUS_DEFINED_


/* begin global definition section */


void ui_stat_displayStatus(struct PosType gridPos);


/* end global definition section */


/* begin private definition section */


static struct PosType ui_stat_oldPos; /* current displayed position */
static int            ui_stat_oldNumberOfSelectedItems = 0; /* current number ... */
static FlagType       ui_stat_oldFlags = 0;                 /* current flags */


/* end private definition section */


#endif /* _UI_STATUS_DEFINED_ */


/* end of file */
/* lines: 28 */

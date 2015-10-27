/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_netUpdate.ph,v $
  SHORTNAME      : netUpdat.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.5.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:29 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_NETUPDATE_DEFINED_
#define _UI_NETUPDATE_DEFINED_


/* begin global definition section */


void ui_net_drawLink (struct Ui_DisplayType *, FlagType, int, int, 
        FlintType, Bool);
void ui_net_drawUnit (struct Ui_DisplayType *, FlagType, int, Bool);
void ui_net_drawAllLinksToUnit (struct Ui_DisplayType *, FlagType, int, Bool);
void ui_net_drawAllLinksFromUnit (struct Ui_DisplayType *, FlagType, 
        int, FlagType);
void ui_net_updateWhole (struct Ui_DisplayType *, FlagType, int, Bool);
void ui_net_completeRefresh (struct Ui_DisplayType *, FlagType);

/* end global definition section */


/* begin private definition section */


static void ui_net_doClearWindow (struct Ui_DisplayType *);
static void ui_net_clearWindow (struct Ui_DisplayType *, FlagType);


/* end private definition section */


#endif /* _UI_NETUPDATE_DEFINED_ */


/* end of file */
/* lines: 30 */

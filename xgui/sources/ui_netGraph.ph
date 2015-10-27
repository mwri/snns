/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_netGraph.ph,v $
  SHORTNAME      : netGraph.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.5.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:26 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_NETGRAPH_DEFINED_
#define _UI_NETGRAPH_DEFINED_


/* begin global definition section */


void ui_drawGrowingThing (struct Ui_DisplayType *, struct PosType, int);
void ui_drawUnit (struct Ui_DisplayType *, int, FlagType);
void ui_drawLink (struct Ui_DisplayType *, int, int, FlintType, FlagType);


/* dimension of a unit */
int ui_unitWidth  = 16;
int ui_unitHeight = 16;


/* end global definition section */


/* begin private definition section */


static struct PosType ui_arrowPoint (struct PosType, struct PosType);
static double ui_angle (struct PosType source, struct PosType target) ;


/* end private definition section */


#endif /* _UI_NETGRAPH_DEFINED_ */


/* end of file */
/* lines: 30 */

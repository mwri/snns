/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_kohonen.ph,v $
  SHORTNAME      : ui_kohonen.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Kohonen Function Prototypes, private
  NOTES          :

  AUTHOR         : Marcus Ritt
  DATE           : July 13 1993

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:06 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#ifndef _KO_MAIN_DEFINED_
#define  _KO_MAIN_DEFINED_

/* begin global definition section */

void kohonen_createWindow (void);

Widget ui_ExtraToggle;
Widget ui_LayerWidget;
int noOfCurrentLayer = 0;
int kohonen_open = 0;

/* end global definition section */


/* begin privat definition section */

#define kohonen_fontWidth 8

static Widget kohonen_mainWidget;
static void   kohonen_rem_moveLayerProc(Widget w, int moveType, 
					caddr_t call_data);
static void   kohonen_tree(void);
static void   kohonen_doneProc(void);

/* end privat definition section */

#endif /* _KO_MAIN_DEFINED_ */


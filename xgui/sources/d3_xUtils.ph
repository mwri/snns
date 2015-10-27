/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_xUtils.ph,v $
  SHORTNAME      : xUtils.ph
  SNNS VERSION   : 4.2

  PURPOSE        : header for d3_xUtils.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:48 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _D3_XUTILS_DEFINED_
#define _D3_XUTILS_DEFINED_


/* begin global definition section */


Widget d3_xCreateButtonItem (char *name, Widget parent, Widget left, Widget top);
Widget  d3_xCreateToggleItem(char *name, Widget parent, Widget group, Widget left, Widget top);


extern Display *ui_display;


/* end global definition section */


/* begin private definition section */


/* end private definition section */


#endif /* _D3_XUTILS_DEFINED_ */


/* end of file */
/* lines: 23 */

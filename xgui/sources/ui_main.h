/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_main.h,v $
  SHORTNAME      : main.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.4.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:16 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_MAIN_DEFINED_
#define _UI_MAIN_DEFINED_


extern void main (int, char **);

#ifndef MAXPATHLEN
#define MAXPATHLEN  512
#endif

	/* path name (see file panel). Initial value by getwd() */
extern char         ui_pathname[];

extern XtAppContext ui_appContext;
extern int ui_labelFontWidth;

	/* label widgets of status info */
extern Widget       ui_stat_posWidget;
extern Widget       ui_stat_selNoWidget;
extern Widget       ui_stat_flagsWidget;

#endif /* _UI_MAIN_DEFINED_ */


/* end of file */
/* lines: 44 */

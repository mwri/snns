/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_lists.h,v $
  SHORTNAME      : lists.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 28.8.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/04/20 11:55:27 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_LISTS_DEFINED_
#define _UI_LISTS_DEFINED_


extern void ui_list_buildList (struct SimpleListType *);
extern void ui_list_setUnitValue (Widget, struct SimpleListType *, 
				  XawListReturnStruct *);
extern char *ui_list_getFirstItem (int type);
extern char *ui_list_getNextItem (int type);

extern char  *ui_list_iotypes[];

	/* number of list items */
extern int   ui_list_noOfSites;
extern int   ui_list_noOfFTypeSites;

	/* return values of the list panel */
extern char   ui_list_returnName[];
extern int    ui_list_returnIndex;

#endif /* _UI_LISTS_DEFINED_ */
/* end of file */


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_lists.ph,v $
  SHORTNAME      : lists.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 28.8.1990

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:14 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_LISTS_DEFINED_
#define _UI_LISTS_DEFINED_


/* begin global definition section */


void ui_list_buildList (struct SimpleListType *);
void ui_list_setUnitValue (Widget, struct SimpleListType *, 
	XawListReturnStruct *);
char *ui_list_getFirstItem (int type);
char *ui_list_getNextItem (int type);


char  *ui_list_iotypes[] = {
    "unknown",
    "input",
    "output",
    "hidden",
    "dual",
    "special",
    "special_i",
    "special_o",
    "special_h",
    "special_d",
    "special_X",
    "non-special_X"
    };

	/* number of list items */
int   ui_list_noOfSites      = 0;
int   ui_list_noOfFTypeSites = 0;

	/* return values of the list panel */
char   ui_list_returnName[MAX_NAME_LENGTH];
int    ui_list_returnIndex = XAW_LIST_NONE;

	/* message widget in the control panel */
/* Widget   ui_controlMessageWidget;*/



/* end global definition section */


/* begin private definition section */




	/* used to store the last funtion number reported by the kernel
	UI_LIST_ACT_FUNC, UI_LIST_OUT_FUNC, UI_LIST_SITE_FUNC, 
	UI_LIST_LEARN_FUNC, UI_LIST_UPDATE_FUNC, UI_LIST_INIT_FUNC */
static int   ui_list_currentFuncNo[11] = { 0,0,0,0,0,0,0,0,0,0,0 };

	/* number of list items */
static int   ui_list_currentIOType = 0;
static int   ui_list_noOfFtypes     = 0;
static int   ui_list_noOfFunctions  = 0;

/* end private definition section */


#endif /* _UI_LISTS_DEFINED_ */


/* end of file */
/* lines: 28 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_edit.ph,v $
  SHORTNAME      : edit.ph
  SNNS VERSION   : 4.2

  PURPOSE        : callbacks for editing sites and f-types
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 25.9.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:41 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_EDIT_DEFINED_
#define _UI_EDIT_DEFINED_


/* begin global definition section */


void  ui_edit_selectFunction (Widget, int, caddr_t);
void  ui_edit_setSite (Widget, struct SimpleListType *, caddr_t);
void  ui_edit_listSetSite (Widget, int, XawListReturnStruct *);
void  ui_edit_setCurrentSite (char *);
void  ui_edit_newSite (Widget, struct SimpleListType *, caddr_t);
void  ui_edit_deleteSite (Widget, struct SimpleListType *, caddr_t);
void  ui_edit_displayFirstFType (struct SimpleListType *);
void  ui_edit_ftypeAddSite (Widget, struct SimpleListType *, caddr_t);
void  ui_edit_ftypeDeleteSite (Widget, struct SimpleListType *, caddr_t);
void  ui_edit_chooseFType (Widget, struct SimpleListType *, caddr_t);
void  ui_edit_setFType (Widget, struct SimpleListType *, caddr_t);
void  ui_edit_newFType (Widget, struct SimpleListType *, caddr_t);
void  ui_edit_deleteFType (Widget, struct SimpleListType *, caddr_t);


	/* widgets in the edit panels */
Widget ui_edit_siteFuncNameWidget;
Widget ui_edit_siteNameWidget;
Widget ui_edit_actFuncNameWidget;
Widget ui_edit_outFuncNameWidget;
Widget ui_edit_ftypeNameWidget;

	/* the following values are available after */
	/* the call of ui_list_buildList */
int   ui_list_noOfFtypes;     /* no of ftypes reported by the kernel */

	/* names */
char    ui_edit_siteName[MAX_NAME_LENGTH];
char    ui_edit_siteFuncName[MAX_NAME_LENGTH];


/* end global definition section */


/* begin private definition section */


static void  ui_edit_displayFType(char *, struct SimpleListType *);


	/*  names */
static char    ui_edit_actFuncName[MAX_NAME_LENGTH];
static char    ui_edit_ftypeName[MAX_NAME_LENGTH];
static char    ui_edit_outFuncName[MAX_NAME_LENGTH];


/* end private definition section */


#endif /* _UI_EDIT_DEFINED_ */


/* end of file */
/* lines: 40 */

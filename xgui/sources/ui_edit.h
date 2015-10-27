/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_edit.h,v $
  SHORTNAME      : edit.h
  SNNS VERSION   : 4.2

  PURPOSE        : callbacks for editing sites and f-types
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 25.9.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_EDIT_DEFINED_
#define _UI_EDIT_DEFINED_




extern void  ui_edit_selectFunction (Widget, int, caddr_t);
extern void  ui_edit_setSite (Widget, struct SimpleListType *, caddr_t);
extern void  ui_edit_listSetSite (Widget, int, XawListReturnStruct *);
extern void  ui_edit_setCurrentSite (char *);
extern void  ui_edit_newSite (Widget, struct SimpleListType *, caddr_t);
extern void  ui_edit_deleteSite (Widget, struct SimpleListType *, caddr_t);
extern void  ui_edit_displayFirstFType (struct SimpleListType *);
extern void  ui_edit_ftypeAddSite (Widget, struct SimpleListType *, caddr_t);
extern void  ui_edit_ftypeDeleteSite(Widget, struct SimpleListType *, caddr_t);
extern void  ui_edit_chooseFType (Widget, struct SimpleListType *, caddr_t);
extern void  ui_edit_setFType (Widget, struct SimpleListType *, caddr_t);
extern void  ui_edit_newFType (Widget, struct SimpleListType *, caddr_t);
extern void  ui_edit_deleteFType (Widget, struct SimpleListType *, caddr_t);


	/* widgets in the edit panels */
extern Widget ui_edit_siteFuncNameWidget;
extern Widget ui_edit_siteNameWidget;
extern Widget ui_edit_actFuncNameWidget;
extern Widget ui_edit_outFuncNameWidget;
extern Widget ui_edit_ftypeNameWidget;


	/* the following values are available after */
	/* the call of ui_list_buildList */
extern int   ui_list_noOfFtypes;     /* no of ftypes reported by the kernel */

	/* names */
extern char    ui_edit_siteName[];
extern char    ui_edit_siteFuncName[];



#endif /* _UI_EDIT_DEFINED_ */


/* end of file */
/* lines: 63 */

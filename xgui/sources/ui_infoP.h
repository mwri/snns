/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_infoP.h,v $
  SHORTNAME      : infoP.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 27.6.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_INFOP_DEFINED_
#define _UI_INFOP_DEFINED_




extern void ui_info_unitUpdateProc (Widget, FlagType, caddr_t);
extern void ui_info_linkUpdateProc (Widget, XtPointer, caddr_t);
extern void ui_info_freezeUnit (Widget, int, caddr_t);
extern void ui_info_assignLayer(Widget, struct InfoCallLayerStruct *, caddr_t);
extern void ui_info_setDefault (Widget, int, caddr_t);
extern void ui_info_displayList (Widget, struct SimpleListType *, caddr_t);
extern void ui_info_showFirstSiteOfTargetUnit (Widget, caddr_t, caddr_t);
extern void ui_info_showNextSiteOfTargetUnit (Widget, caddr_t, caddr_t);
extern void ui_info_showFirstSuccOfSourceUnit (Widget, XtPointer, caddr_t);
extern void ui_info_showNextSuccOfSourceUnit (Widget, XtPointer, caddr_t);
extern void ui_info_showFirstPredOfTargetUnit (Widget, XtPointer, caddr_t);
extern void ui_info_showNextPredOfTargetUnit (Widget, XtPointer, caddr_t);
extern void ui_info_showSelectedUnit (int);
extern void ui_info_listAllTargetUnits (Widget, int, caddr_t);
extern void ui_info_listAllSourceUnits (Widget, int, caddr_t);
extern void ui_info_listAllSites (Widget, int, caddr_t);
extern void ui_info_listAllLinksToCurrentSite (Widget, int, caddr_t);
extern void ui_info_setUnitItems (struct UnitWidgetType, 
	struct UnitAttributeType);
extern void ui_info_storeAttributes (int, struct UnitAttributeType);
extern void ui_info_initUnitStruct(struct UnitAttributeType *unit);
extern void ui_info_getDisplayedUnitAttributes(struct UnitWidgetType *,
        struct UnitAttributeType *);
extern void ui_info_showPredOfTargetUnit(int, FlintType);
extern void ui_info_makeUnitInfoPanelConsistent(void);


	/* widgets of all data fields in the info panel */
extern struct UnitWidgetType ui_targetWidgets, ui_sourceWidgets;
extern struct LinkWidgetType ui_linkWidgets;

extern Bool   ui_infoIsCreated; /* yes or no */

#endif /* _UI_INFOP_DEFINED_ */


/* end of file */
/* lines: 63 */


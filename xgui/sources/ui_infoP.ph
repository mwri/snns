/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_infoP.ph,v $
  SHORTNAME      : infoP.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 27.6.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_INFOP_DEFINED_
#define _UI_INFOP_DEFINED_


/* begin global definition section */


void ui_info_unitUpdateProc (Widget, FlagType, caddr_t);
void ui_info_linkUpdateProc (Widget, XtPointer, caddr_t);
void ui_info_freezeUnit (Widget, int, caddr_t);
void ui_info_assignLayer (Widget, struct InfoCallLayerStruct *, caddr_t);
void ui_info_setDefault (Widget, int, caddr_t);
void ui_info_displayList (Widget, struct SimpleListType *, caddr_t);
void ui_info_showFirstSiteOfTargetUnit (Widget, caddr_t, caddr_t);
void ui_info_showNextSiteOfTargetUnit (Widget, caddr_t, caddr_t);
void ui_info_showFirstSuccOfSourceUnit (Widget, XtPointer, caddr_t);
void ui_info_showNextSuccOfSourceUnit (Widget, XtPointer, caddr_t);
void ui_info_showFirstPredOfTargetUnit (Widget, XtPointer, caddr_t);
void ui_info_showNextPredOfTargetUnit (Widget, XtPointer, caddr_t);
void ui_info_showSelectedUnit (int);
void ui_info_listAllTargetUnits (Widget, int, caddr_t);
void ui_info_listAllSourceUnits (Widget, int, caddr_t);
void ui_info_listAllSites (Widget, int, caddr_t);
void ui_info_listAllLinksToCurrentSite (Widget, int, caddr_t);
void ui_info_setUnitItems (struct UnitWidgetType, struct UnitAttributeType);
void ui_info_storeAttributes (int, struct UnitAttributeType);
void ui_info_initUnitStruct(struct UnitAttributeType *unit);
void ui_info_getDisplayedUnitAttributes(struct UnitWidgetType *,
        struct UnitAttributeType *);
void ui_info_showPredOfTargetUnit(int, FlintType);
void ui_info_makeUnitInfoPanelConsistent(void);


	/* widgets of all data fields in the info panel */
struct UnitWidgetType ui_targetWidgets, ui_sourceWidgets;
struct LinkWidgetType ui_linkWidgets;

Bool   ui_infoIsCreated; /* yes or no */

/* end global definition section */


/* begin private definition section */


static Bool ui_info_anyUnitSelected (FlagType);
static void ui_info_getAllUnitData (int, struct UnitAttributeType *);
static void ui_info_showSuccOfSourceUnit (int, FlintType);
static void ui_info_getAllSiteData (void);
static void ui_info_initLinkStruct (void);
static void ui_info_setLinkItems (void);
static void ui_info_showFirstUnit (void);


	/* attributs of source/target/link in the info panel */
static struct UnitAttributeType    ui_defUnit;

/* static struct LinkAttributeType    ui_link;
*/

#ifdef _UNUSED_FUNCTIONS_
static void ui_info_deleteLink (void);
static void ui_info_deleteSite (Widget, struct SimpleListType *, caddr_t);
static void ui_info_showNextUnit (void);
static void ui_info_makeTargetToSourceUnit (void);
static void ui_info_makeSourceToTargetUnit (void);
static void ui_info_listAllUnits (Widget, XtPointer, caddr_t);
static void ui_info_listAllLinks (void);
#endif /* _UNUSED_FUNCTIONS_ */


/* end private definition section */


#endif /* _UI_INFOP_DEFINED_ */


/* end of file */
/* lines: 68 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_selection.ph,v $
  SHORTNAME      : select.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 29.5.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:37 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_SELECTION_DEFINED_
#define _UI_SELECTION_DEFINED_


/* begin global definition section */


void ui_sel_msgNumber (void);
void ui_sel_initSelectionList (void);
struct SelectionType *ui_sel_lookForItem (int unitNo);
struct SelectionType *ui_sel_lookForSelectedItem (struct SelectionType *);
void ui_sel_drawBoxes (struct Ui_DisplayType *, FlagType, struct PosType);
void ui_sel_selectOne (struct Ui_DisplayType *, FlagType, int, struct PosType);
void ui_sel_unselectOne (struct Ui_DisplayType *, FlagType, int, 
			struct PosType);
void ui_sel_selectRect (struct Ui_DisplayType *, FlagType, struct PosType, 
			struct PosType);
void ui_sel_unselectRect (struct Ui_DisplayType *, FlagType, struct PosType, 
			struct PosType);
void ui_sel_reshowItems (struct Ui_DisplayType *, FlagType);
void ui_sel_checkList (struct Ui_DisplayType *, FlagType);
void ui_sel_unselectAll (struct Ui_DisplayType *, FlagType);
void ui_sel_resetList (void);


/* handling of the selection list */
struct SelectionType  *ui_sel_listPtr;        /* pointer to first item */
int            ui_sel_numberOfSelectedItems;  /* number of selected items */


/* end global definition section */


/* begin private definition section */


static struct SelectionType *ui_sel_createItem (void);
static struct SelectionType *ui_sel_getFreeItem (void);
static void ui_sel_freeItem (struct SelectionType *);
static void ui_sel_doActionSelect (struct Ui_DisplayType *, FlagType, 
			struct SelectionType *, int, struct PosType);
static void ui_sel_showSelectedItem (struct Ui_DisplayType *, FlagType, int, 
			struct PosType);


static struct SelectionType *ui_sel_freeListPtr;/* pointer to first free item*/
static int            ui_sel_numberOfItems;     /* number of created items */
static int ui_selectionSize = UI_UNIT_SIZE;


/* end private definition section */



#endif /* _UI_SELECTION_DEFINED_ */


/* end of file */
/* lines: 46 */

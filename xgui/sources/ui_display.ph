/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_display.ph,v $
  SHORTNAME      : display.ph
  SNNS VERSION   : 4.2

  PURPOSE  	 : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR   	 : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _UI_DISPLAY_DEFINED_
#define _UI_DISPLAY_DEFINED_


/* begin global definition section */


struct Ui_DisplayType *ui_currentDisplay;  /* current display */
struct Ui_DisplayType *ui_displ_listPtr;   /* pointer to first item */

int      ui_displ_numberOfItems;       /* number of created items */


void             ui_displ_initDisplayList(void);
void             ui_displ_freeItem(struct Ui_DisplayType *displayPtr);
	/* free it */
void             ui_displ_resetList(void);     /* free al items */
struct Ui_DisplayType  *ui_displ_getFreeItem(void);   /* create */
Bool             ui_displ_isSomeWhereToShowWeights(void);
Bool             ui_displ_isSomeWhereToShowValues(void);
void             ui_displ_freezeDisplay(Widget w, 
                                struct Ui_DisplayType *displayPtr, 
                                caddr_t call_data);


/* end global definition section */


/* begin private definition section */


static struct Ui_DisplayType *ui_displ_freeListPtr;  
	/* pointer to first free item */


static struct Ui_DisplayType *ui_displ_createItem(void);


#ifdef _UNUSED_FUNCTIONS_
static struct Ui_DisplayType  *ui_displ_getDisplayPtrViaFrameWidget(Widget);
	/* get display via frame widget */
static struct Ui_DisplayType  *ui_displ_getDisplayPtrViaDrawable(Widget);
	/* get display via widget */
#endif /* _UNUSED_FUNCTIONS_ */


/* end private definition section */


#endif /* _UI_DISPLAY_DEFINED_ */



/* end of file */
/* lines: 34 */

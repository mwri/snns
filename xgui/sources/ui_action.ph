/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_action.ph,v $
  SHORTNAME      : action.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:06 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_ACTION_DEFINED_
#define _UI_ACTION_DEFINED_


/* begin global definition section */


void  ui_action_init (void);
void  ui_action_unitsMovev (void);
void  ui_action_unitsCopy (void);
void  ui_action_unitInsert (int);
void  ui_action_linksMakeBidirectional (void);
void  ui_action_linksMakeReverse (void);
void  ui_action_linksCopy (void);
void  ui_action_linksCopyEnvironment (void);
void  ui_action_linksMakeClique (int);
void  ui_action_linksMakeFromSourceUnit (int);
void  ui_action_linksMakeToTargetUnit (int);
void  ui_action_linksSet (void);
void  ui_action_unitsDelete (void);
void  ui_action_setUnitAttribute (FlagType);
void  ui_action_sitesCopy (void);
void  ui_action_graphicsDirection (void);
void  ui_action_graphicsGrid (void);
void  ui_action_UnitZcoordinate (void);
void  ui_action_UnitZsetReference (void);
void  ui_action_unitsMove (void);
void  ui_action_sitesMake (int);
void  ui_action_setUnitLayers (void);
void  ui_action_unitSetDefaultType (void);


int   ui_kernelError; /* stores the error reported by kernel functions */

short ui_actualZvalue;

struct LinkAttributeType   ui_link; /* attributes of the link shown in the 
	   				info panel */

unsigned short ui_layerReturnValue; /* return value of the layer panel */

/* end global definition section */


/* begin private definition section */


#define  UI_GENERAL  0  /* unit number 0: no special unit */


static Bool  ui_action_checkNewPositions (int);
static Bool  ui_action_createLink (int, int, char [], FlintType);
static Bool  ui_action_deleteLink (int, int, char []);
static Bool  ui_action_getSite (int);
static Bool  ui_action_isNewUnit (int);


	/*********************************************************************
                              NAMES of local VARIABLES
  
  	selPtr           ptr to step through the XGUI's selection list
  	mainSelPtr       if a outer and a inner step loop required, mainSelPtr
       		         holds the ptr of the outer loop


                              FURTHER REMARKS

  	Sometimes the operation has to erase first all selection markers 
	in order to produce a "nice" picture. This is done with the routine:
                        ui_sel_reshowItems()
  	This routine performs everytime a XOR raster operation when drawing 
	the markers, so the next time the routine will be called, the 
	markes occur again.

	**********************************************************************/

static struct SimpleListType ui_action_listSite; /* list of all sites */
static char ui_action_siteName[MAX_NAME_LENGTH];
                             /* site name choosed in the site list */


/* end private definition section */


#endif /* _UI_ACTION_DEFINED_ */



/* end of file */
/* lines: 52 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_action.h,v $
  SHORTNAME      : action.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:06 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_ACTION_DEFINED_
#define _UI_ACTION_DEFINED_

extern void  ui_action_init (void);
extern void  ui_action_unitsMovev (void);
extern void  ui_action_unitsCopy (void);
extern void  ui_action_unitInsert (int);
extern void  ui_action_linksMakeBidirectional (void);
extern void  ui_action_linksMakeReverse (void);
extern void  ui_action_linksCopy (void);
extern void  ui_action_linksCopyEnvironment (void);
extern void  ui_action_linksMakeClique (int);
extern void  ui_action_linksMakeFromSourceUnit (int);
extern void  ui_action_linksMakeToTargetUnit (int);
extern void  ui_action_linksSet (void);
extern void  ui_action_unitsDelete (void);
extern void  ui_action_setUnitAttribute (FlagType);
extern void  ui_action_sitesCopy (void);
extern void  ui_action_graphicsDirection (void);
extern void  ui_action_graphicsGrid (void);
extern void  ui_action_UnitZcoordinate (void);
extern void  ui_action_UnitZsetReference (void);
extern void  ui_action_unitsMove (void);
extern void  ui_action_sitesMake (int);
extern void  ui_action_setUnitLayers (void);
extern void  ui_action_unitSetDefaultType (void);


extern int   ui_kernelError; 
		/* used to store the error reported by kernel functions */

extern short ui_actualZvalue;

extern struct LinkAttributeType    ui_link;
                  /* attributes of the link shown in the info panel */

extern unsigned short ui_layerReturnValue;
                  /* return value of the layer panel */

#endif /* _UI_ACTION_DEFINED_ */

/* end of file */
/* lines: 64 */


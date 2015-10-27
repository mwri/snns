/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_utilP.ph,v $
  SHORTNAME      : ui_utilP.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer 
  DATE           : 18.5.1990

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _UI_UTILP_DEFINED_
#define _UI_UTILP_DEFINED_

/* begin global definition section */

Bool ui_isUnitVisibleInAnyDisplay(int unitNo);


/* kernel handling */
void             ui_checkError(krui_err error);
void             ui_krui_setCurrentLink(int sourceNo, int targetNo, char *siteName);
void             ui_correctName(char *string);
void             ui_cutTrailingZeros(char *string);
Bool             ui_krui_sitesExistInNetwork(void);
Bool             ui_krui_setCurrentSite(int targetNo, char *siteName);

struct PosType   ui_utilGridToPix(struct Ui_DisplayType *displayPtr, struct PosType gridPos);
struct PosType   ui_utilPixToGrid(struct Ui_DisplayType *displayPtr, struct PosType pixPos);
/*struct PosType   ui_utilPixToAlignPix();*/
struct PosType   ui_utilPixUpperLeft(struct Ui_DisplayType *displayPtr, struct PosType gridPos);
struct PosType   ui_utilPixLowerRight(struct Ui_DisplayType *displayPtr, struct PosType gridPos, short int extent);

void             ui_utilChangeFlag(FlagType *flag_word, FlagType flag);

int              ui_utilSgnInt(int x);
void             ui_utilMax(short int *x1Ptr, short int *x2Ptr);
void             ui_utilNormalizeRect(struct PosType *pixPos1Ptr, struct PosType *pixPos2Ptr);
/*void             ui_utilNormalizeCoord();*/


/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif 


/* end of file */
/* lines: 51 */

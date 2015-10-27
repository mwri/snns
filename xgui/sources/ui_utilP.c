/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_utilP.c,v $
  SHORTNAME      : ui_utilP.ph
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Tilman Sommer 
  DATE           : 18.5.1990

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:47 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>

#include "ui.h"

#include <X11/Xaw/Text.h>
#include <X11/Xaw/TextSrc.h>
#include <X11/Xaw/AsciiText.h>

#include "ui_display.h"
#include "ui_mainP.h"
#include "ui_fileP.h"
#include "ui_netGraph.h"
#include "ui_textP.h"
#include "ui_action.h"

#include "kr_ui.h"

#include "ui_utilP.ph"


/*****************************************************************************
  FUNCTION : ui_isUnitVisibleInAnyDisplay

  PURPOSE  : checks, whether the specified unit ist visible in any display
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

Bool ui_isUnitVisibleInAnyDisplay(int unitNo)
{
    struct Ui_DisplayType *dPtr;
    
    dPtr = ui_displ_listPtr;
    while (dPtr != NULL) {
	if ((krui_getUnitSubnetNo(unitNo) == dPtr->subNetNo) AND
	    (krui_getUnitLayerNo(unitNo) BIT_AND dPtr->layers))
	    return(TRUE);
	dPtr = dPtr->nextPtr;
    }
    return(FALSE);
}


/*****************************************************************************
  FUNCTION : ui_checkError

  PURPOSE  : checks the error value, coming from a kernel function call.
             If an error has occured, ui gets the message from the kernel
	     and writes it to the text window.
  NOTES    :

  UPDATE   : 24.8.1990
******************************************************************************/

void
ui_checkError(krui_err error)
{
    if ((ui_kernelError = error) != 0)
	ui_tw_errorMessage(krui_error(error));
}




/*****************************************************************************
  FUNCTION : ui_krui_sitesExistInNetwork

  PURPOSE  : returns TRUE, if there is at least one site in the network
  NOTES    : Only the site table is checked. If there is no unit with sites,
             but there is at least one site table entry, this function still
             returns TRUE!
  UPDATE   : 26.8.1990
******************************************************************************/

Bool
ui_krui_sitesExistInNetwork(void)
{
    char *name, *funcName;

    return(krui_getFirstSiteTableEntry(&name, &funcName));
}


/*****************************************************************************
  FUNCTION : ui_krui_setCurrentSite

  PURPOSE  : sets the current site
  NOTES    : The unit must exist (no error checking) !!
             If the unit has no sites, only the current unit is set and TRUE will
	     be returned.
  UPDATE   : 26.8.1990
******************************************************************************/

Bool
ui_krui_setCurrentSite(int targetNo, char *siteName)
{
    (void) krui_setCurrentUnit(targetNo);    /* target */
    if (krui_getUnitInputType(targetNo) != SITES)
	return(TRUE);

    if ((siteName != NULL) AND strlen(siteName)) 
	return((krui_setSite(siteName) == UI_SET_OK));
    else
	return(FALSE); /* invalid site name */
}



/*****************************************************************************
  FUNCTION : ui_krui_setCurrentLink

  PURPOSE  : sets the current link (kernel)
  NOTES    : This routine was written, because the handling of the current
             link is hard sometimes, specially while doing the loops
	     getFirstSuccUnit..getNextSuccUnit and
	     getFirstPredUnit..getNextPredUnit
	     The link and all units must exist (no error checking) !!

  UPDATE   : 26.8.1990
******************************************************************************/

void
ui_krui_setCurrentLink(int sourceNo, int targetNo, char *siteName)
{
    (void) krui_setCurrentUnit(targetNo);    /* target */
    (void) ui_krui_setCurrentSite(targetNo, siteName);
    (void) krui_isConnected(sourceNo);       /* sets current link */
}


/*****************************************************************************
  FUNCTION : ui_correctName

  PURPOSE  : replace all ' ', '\t' und '\n' with underscores ('_') 
  NOTES    :

  UPDATE   : 27.9.1990
******************************************************************************/

void ui_correctName(char *string)
{
    int j;
    if (string == NULL) return;
    /* replace white space with underscores */
    for (j=0; string[j] != '\0'; j++) {
	if ((string[j] == ' ') OR
	    (string[j] == '\t') OR
	    (string[j] == '\n'))
	    string[j] = '_';
    }
}

/*****************************************************************************
  FUNCTION : ui_cutTrailingZeros

  PURPOSE  : replace all trailing '0' (except after '.') 
             with null character ('\0') 
  NOTES    :

  UPDATE   : 27.9.1990
******************************************************************************/

void ui_cutTrailingZeros(char *string)
{
    int j;
    if ((string == NULL) OR (strlen(string) < 2)) return;
    for (j = strlen(string)-1 ; ((j > 0) AND (string[j] != '.')) ; j--) {
	if ((string[j] == '0') AND (string[j-1] != '.'))
	    string[j] = '\0';
    }
}



/*****************************************************************************
  FUNCTION :                     ui_utilGridToPix

  PURPOSE  : calculates the center pixel coordinates of a unit grid position.
  RETURNS  : pixel coordinate
  NOTES    : see above
*****************************************************************************/

struct PosType
ui_utilGridToPix(struct Ui_DisplayType *displayPtr, struct PosType gridPos)
{
    struct PosType  pixPos;

    pixPos.x = displayPtr->gridSize * (gridPos.x - displayPtr->origin.x) + 
	displayPtr->gridSize/2;
    pixPos.y = displayPtr->gridSize * (gridPos.y - displayPtr->origin.y) + 
	displayPtr->gridSize/2;

    return (pixPos);
}



/*****************************************************************************
  FUNCTION :                      ui_utilPixToGrid

  PURPOSE  : transforms a pixel coordinate to a grid position 
  RETURNS  : the corresponding grid position
  NOTES    : to be consistent use always this routine to do the translation
*****************************************************************************/

struct PosType
ui_utilPixToGrid(struct Ui_DisplayType *displayPtr, struct PosType pixPos)
{
    struct PosType  gridPos;

    gridPos.x =  displayPtr->origin.x + (pixPos.x / displayPtr->gridSize);
    gridPos.y =  displayPtr->origin.y + (pixPos.y / displayPtr->gridSize);
    gridPos.z =  ui_actualZvalue;
/*
    gridPos.z =  0;
*/
    return (gridPos);
}



/*****************************************************************************
  FUNCTION :                      ui_utilChangeFlag

  PURPOSE  : XOR operation of certain bits in a flag word
  RETURNS  : alteration of certain bits in a flag word
*****************************************************************************/


void 
ui_utilChangeFlag(FlagType *flag_word, FlagType flag)
{
    FlagType aux = *flag_word;

    if (ui_utilIsSet(aux, flag))
	ui_utilResetFlag(aux,flag);
    else
	ui_utilSetFlag(aux,flag);
    *flag_word = aux;
}



/*****************************************************************************
  FUNCTION :                        ui_utilSgnInt

  PURPOSE  : calculates the signum function for integer arguments
  RETURNS  : sgn(x)  
*****************************************************************************/


int
ui_utilSgnInt(int x)
{
    if (x > 0) return(1);
    else
	if (x < 0) return(-1);
	else return(0);
}



/*****************************************************************************
  FUNCTION :                         ui_utilMax

  PURPOSE  : The varaibles x1 and x2 will be swapped, if x1 is not the maximum
  RETURNS  : --- (side-effect)
  NOTES    : Both arguments must be pointers to integers!
*****************************************************************************/

void
ui_utilMax(short int *x1Ptr, short int *x2Ptr)
{
    short h;

    if (*x1Ptr < *x2Ptr) { /* x1 is not the maximum, so swap */
	h = *x1Ptr;  *x1Ptr = *x2Ptr;   *x2Ptr = h; 
    } 
}




/*****************************************************************************
  FUNCTION :                 ui_utilNormalizeRect

  PURPOSE  : Two pairs of coordinates are given describing a rectangle.
             They will be reassigned, that (x1,y1) is the upper left and
	     (x2,y2) the lower right corner. 
  RETURNS  : call by reference --> (x1Ptr,y1Ptr,x2Ptr,y2Ptr)
  NOTES    : see ui_utilnormalize_coord()
*****************************************************************************/

void
ui_utilNormalizeRect(struct PosType *pixPos1Ptr, struct PosType *pixPos2Ptr)
{
    ui_utilMax(&(pixPos2Ptr->x), &(pixPos1Ptr->x));
    ui_utilMax(&(pixPos2Ptr->y), &(pixPos1Ptr->y));
}



/*****************************************************************************
  FUNCTION :                       ui_utilPixUpperLeft

  PURPOSE  : calculates the pixel coordinates of the upper left corner of a
             unit rectangle.
  RETURNS  : coordinate in pixel
  NOTES    : unit position are stored as raster (grid) positions
*****************************************************************************/


struct PosType
ui_utilPixUpperLeft(struct Ui_DisplayType *displayPtr, struct PosType gridPos)
{
    struct PosType  pixPos;

    pixPos    = ui_utilGridToPix(displayPtr, gridPos);
    pixPos.x -= ui_unitWidth/2;
    pixPos.y -= ui_unitHeight/2;

    return(pixPos);
}



/*****************************************************************************
  FUNCTION :                  ui_utilPixLowerRight

  PURPOSE  : calculates the pixel coordinates of the lower right corner of a
             unit rectangle.
  RETURNS  : pixel coordinate
  NOTES    : see above
*****************************************************************************/

struct PosType
ui_utilPixLowerRight(struct Ui_DisplayType *displayPtr, struct PosType gridPos, short int extent)
{
    struct PosType  pixPos;


    pixPos    = ui_utilGridToPix(displayPtr, gridPos);
    pixPos.x += ui_unitWidth  / 2 - 1 + extent;
    pixPos.y += ui_unitHeight / 2 - 1 + extent;

    return (pixPos);
}










/* end of file */
/* lines: 407 */

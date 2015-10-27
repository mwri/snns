/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_print.c,v $
  SHORTNAME      : print.c
  SNNS VERSION   : 4.2

  PURPOSE        : contains routines to print a network on a postscript printer
  NOTES          : not fully implemented

  AUTHORS        : Ralf Huebner (C - Code), Tilman Sommer (Postscript Code)
  DATE           : 25.5.1992

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.16 $
  LAST CHANGE    : $Date: 1998/03/13 16:31:56 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>		    
#include <time.h>		    

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Toggle.h>

#include "ui.h"
#include "kr_ui.h"

#include "ui_setup.h"
#include "ui_xWidgets.h"
#include "ui_display.h"
#include "ui_mainP.h"
#include "ui_confirmer.h"
#include "ui_fileP.h"
#include "ui_color.h"
#include "ui_key.h"
#include "ui_netGraph.h"

#include "ui_print.ph"


/*****************************************************************************
  FUNCTION : ui_defaultPrinterValues

  PURPOSE  : sets the default printer values
  RETURNS  : void
  NOTES    : change the setup here

  UPDATE   :
*****************************************************************************/

void ui_defaultPrinterValues (void)

{
    ui_prVal.dest            = UI_DEST_FILE;                
    ui_prVal.format          = UI_FORMAT_POSTSCRIPT; 
    ui_prVal.paper           = UI_PAPER_DINA4;   
    ui_prVal.autoScale       = UI_AUTOSCALE_ON;  
    ui_prVal.clip            = UI_CLIP_ON;             
    ui_prVal.color           = UI_COLOR_OFF;             
    ui_prVal.orient          = UI_ORIENTATION_LANDSCAPE;
    ui_prVal.autoUni         = UI_AUTOUNI_ON;  
    ui_prVal.size            = UI_UNITSIZE_VALUE;   
    ui_prVal.shape           = UI_SHAPE_CIRCLE;  
    ui_prVal.text            = UI_TEXT_SOLID;  
    ui_prVal.border          = UI_BORDER_ON;
    ui_prVal.resolution      = 300; 
    ui_prVal.displayToPrint  = 1;
    ui_prVal.borderHoriz     = 10.0;         
    ui_prVal.borderVert      = 10.0;       
    ui_prVal.scaleValX       = 1.0;       
    ui_prVal.scaleValY       = 1.0;       
    ui_prVal.unitGray        = 0.5;
    strcpy (ui_prVal.fileNameStr, "outfile.ps");
    strcpy (ui_prVal.cmdLineStr, "rlp $1 -raw -pr=lw4");
}

#ifdef _UNUSED_FUNCTIONS_
/*****************************************************************************
  FUNCTION : ui_readPrintHeader

  PURPOSE  : reads the postscript header into memory
  RETURNS  : void
  NOTES    : this will be changed in the final version

  UPDATE   :
*****************************************************************************/
static void ui_readPrintHeader (void)
{

#ifndef HEADER_COMPILE
    if ((filePtr = fopen ("psheader05.ps", "r")) == NULL) {
	ui_confirmOk("Error! Can't find Header File\n");
        return;
    }
    fseek (filePtr, 0L, 2);
    headerSize = ftell (filePtr);
    rewind (filePtr);
    if ((psHeader = malloc ((unsigned int) headerSize+1)) == NULL) {
	ui_confirmOk("Error! Can't allocate Memory\n");
        return;
    }
    fread (psHeader, (unsigned int) headerSize, 1, filePtr);
    fclose (filePtr);
    *(psHeader+headerSize) = '\0';
#endif
}
#endif

/*****************************************************************************
  FUNCTION : ui_valueToRGB

  PURPOSE  : converts a value to a RGB color tripple
  RETURNS  : void
  NOTES    :

  UPDATE   :
*****************************************************************************/

static void ui_valueToRGB (float value, float *red, float *green, float *blue)

{
    *blue = (1.0 - fabs(value)) / 2.0;
    *red = *green = 0.0;
    if (value > 0) 
       *green = value;
    else
       *red = fabs(value);
}


/*****************************************************************************
  FUNCTION : ui_printUnit

  PURPOSE  : prints a single unit assoziated with a display
  RETURNS  : void
  NOTES    : this routine is taken from the module ui_netGraph.c

  UPDATE   :
*****************************************************************************/

static void ui_printUnit (struct Ui_DisplayType *displayPtr, int unitNo)

{   
    FlintType          value;
    struct PosType     gridPos;
    int                unitSize;
    char               topString[50];
    char               valueString[50];
    float              red, green, blue;

    if (NOT ui_isUnitVisibleInDisplay(displayPtr, unitNo))
  	return;

    if ((displayPtr->setup).unitScaleFactor < 0.00001) 
	(displayPtr->setup).unitScaleFactor = 1.0;

    switch ((displayPtr->setup).showValue) {
      case UI_ACTIVATION:
	value = krui_getUnitActivation(unitNo);
	break;
      case UI_INITIAL_ACTIVATION:
	value = krui_getUnitInitialActivation(unitNo);
	break;
      case UI_OUTPUT: 
	value = krui_getUnitOutput(unitNo);
	break;
      case UI_BIAS:
	value = krui_getUnitBias(unitNo);
        break;
    }
    
    value /= (displayPtr->setup).unitScaleFactor;
    if (value > 1.0) value = 1.0;
    if (value < -1.0) value = -1.0;

    if (ui_prVal.color == UI_COLOR_ON)
        ui_valueToRGB (value, &red, &green, &blue);
    else
        red = green = blue = ui_prVal.unitGray;

    if (value > 0.001)
       unitSize = (int) (value * ms);
    else
       unitSize = 0;

    if (ui_prVal.size == UI_UNITSIZE_FIXED)
       unitSize = ms;

    krui_getUnitPosition(unitNo, &gridPos);
	
    if ((displayPtr->setup).showTitleFlg) {
       switch ((displayPtr->setup).showTitle) {
           case UI_NUMBER : sprintf(topString,"%d",unitNo);
                            break;
	   case UI_ZVALUE : sprintf(topString,"%d",gridPos.z);
                            break;
           case UI_NAME   : if ((krui_getUnitName(unitNo) == NULL) OR
                              (strlen(krui_getUnitName(unitNo)) == 0))
                                sprintf(topString,"%d",unitNo);
	                    else
 	                        sprintf(topString,"%s", krui_getUnitName(unitNo));
                            break;
           default        : sprintf(topString,"%d",unitNo);
       }
    }

    sprintf(valueString,"%4.3f", value);

    fprintf (psOutfile, "  (%s) (%s)  %4.3f %4.3f %4.3f  %d %d  %d  du\n",
            topString, valueString, red, green, blue, 
            gridPos.x, gridPos.y, unitSize); 

}


/*****************************************************************************
  FUNCTION : ui_printUnits

  PURPOSE  : prints all units assoziated with a display
  RETURNS  : void
  NOTES    : 

  UPDATE   : 
*****************************************************************************/

static void ui_printUnits (struct Ui_DisplayType *displayPtr)
 
{
    Bool   successful;
    int    unitNo;

    successful = ((unitNo = krui_getFirstUnit()) > 0);
    while (successful) {
        ui_printUnit (displayPtr, unitNo);
        successful = ((unitNo = krui_getNextUnit()) > 0);
    }
}


/*****************************************************************************
  FUNCTION : ui_getNetworkSize

  PURPOSE  : gets the size of a network
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_getNetworkSize (int *xmin, int *ymin, int *xmax, int *ymax)

{
    struct PosType gridPos;
    Bool   successful;
    int    unitNo;

    *xmin = 100000;
    *ymin = 100000;
    *xmax = -100000;
    *ymax = -100000;
    successful = ((unitNo = krui_getFirstUnit()) > 0);
    while (successful) {
        krui_getUnitPosition(unitNo, &gridPos);
        if (gridPos.x < *xmin) *xmin = gridPos.x;
        if (gridPos.y < *ymin) *ymin = gridPos.y;
        if (gridPos.x > *xmax) *xmax = gridPos.x;
        if (gridPos.y > *ymax) *ymax = gridPos.y;
        successful = ((unitNo = krui_getNextUnit()) > 0);
    }
}
    

/*****************************************************************************
  FUNCTION : ui_printLink

  PURPOSE  : prints a single link assoziated with a display
  RETURNS  : void
  NOTES    : this routine is taken from the module ui_netGraph.c
*****************************************************************************/

static void ui_printLink (struct Ui_DisplayType *displayPtr, int sourceNo, 
	int targetNo, FlintType weight)

{
    struct PosType   sourceGridPos, targetGridPos;
    Bool             triggered;
    FlintType        weight_true;
    float            red, green, blue;

    if (NOT (displayPtr->setup).showLinkFlg)
	return;
    
    if (krui_getUnitSubnetNo(sourceNo) != krui_getUnitSubnetNo(targetNo))
	return;

    /* same subnet ... 

    if (NOT (ui_isUnitVisibleInDisplay(displayPtr, sourceNo) AND
	     ui_isUnitVisibleInDisplay(displayPtr, targetNo)))
  	return;
   
       one visible layer containing both units was found */

    weight_true = weight;
   
    if (weight >= 0.0) 
	triggered = (weight >= (displayPtr->setup).linkPosTrigger);
    else
	triggered = (weight <= (displayPtr->setup).linkNegTrigger);

    if (NOT triggered) return;

    krui_getUnitPosition(sourceNo, &sourceGridPos);
    krui_getUnitPosition(targetNo, &targetGridPos);

    if (ui_utilAreEqualPositions(sourceGridPos, targetGridPos))
        return;

    weight /= (displayPtr->setup).linkScaleFactor;
    if (weight > 1.0) weight = 1.0;
    if (weight < -1.0) weight = -1.0;

    if (ui_prVal.color == UI_COLOR_ON)
        ui_valueToRGB (weight, &red, &green, &blue);
    else
        red = green = blue = 0.0;

    fprintf (psOutfile, "  (%4.3f)  %d %d  %d %d  %4.3f %4.3f %4.3f  co\n",
             weight_true, sourceGridPos.x, sourceGridPos.y, 
             targetGridPos.x, targetGridPos.y, red, green, blue);

}


/*****************************************************************************
  FUNCTION : ui_printLinks

  PURPOSE  : prints all links assoziated with a display
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_printLinks (struct Ui_DisplayType *displayPtr)
 
{
    Bool       successful;
    int        unitNo, predUnit;
    FlintType  weight;

    successful = ((unitNo = krui_getFirstUnit()) > 0);
    while (successful) {

        (void) krui_setCurrentUnit(unitNo);

        if (krui_getUnitInputType(unitNo) == SITES) {
  	    for (successful = krui_setFirstSite();
	         successful; successful = krui_setNextSite()) {
	        for(predUnit = krui_getFirstPredUnit(&weight);
		    predUnit > 0;
		    predUnit = krui_getNextPredUnit(&weight)) {
	            ui_printLink(displayPtr, predUnit, unitNo, weight);
	        }
	     }
        } else { /* direct links */
	     for(predUnit = krui_getFirstPredUnit(&weight);
	         predUnit > 0;
	         predUnit = krui_getNextPredUnit(&weight)) {
	         ui_printLink(displayPtr, predUnit, unitNo, weight);
	     }
        }
        successful = ((unitNo = krui_getNextUnit()) > 0);
    }
}


/*****************************************************************************
  FUNCTION : ui_psBoolean

  PURPOSE  : converts a boolean in a string
  RETURNS  : "true" or "false"
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static char *ui_psBoolean (int toggle)

{
    char *s;

    s = malloc(6);
    if (toggle)
       strcpy(s, "true");
    else
       strcpy(s, "false");
    return(s);
}


/*****************************************************************************
  FUNCTION : ui_closeNetworkPannel

  PURPOSE  : reads the new values into the ps variables
  RETURNS  : void
  NOTES    : callback from the network setup pannel

  UPDATE   :
*****************************************************************************/

static void ui_closeNetworkPannel (Widget w, Widget pannel, caddr_t call_data)

{
    ulx = ui_xIntFromAsciiWidget(ulxPos);
    uly = ui_xIntFromAsciiWidget(ulyPos);
    lrx = ui_xIntFromAsciiWidget(lrxPos);
    lry = ui_xIntFromAsciiWidget(lryPos);
    FixedUnitSize = ui_psBoolean(!ui_prVal.size);
    ShapeCircle = ui_psBoolean(ui_prVal.shape);
    TransText = ui_psBoolean(ui_prVal.text);
    ShowBorder = ui_psBoolean(!ui_prVal.border);
    ClipOnBorder = ui_psBoolean(!ui_prVal.clip);
    ug = ui_xFloatFromAsciiWidget(fillIntens);
    ui_prVal.displayToPrint = ui_xIntFromAsciiWidget(assDisplay);

    XtDestroyWidget (pannel);
}


/*****************************************************************************
  FUNCTION : ui_transferSettings

  PURPOSE  : reads the initial values into the ps variables
  RETURNS  : void
  NOTES    : nessecary if the network pannel is not opened

  UPDATE   :
*****************************************************************************/

static void ui_transferSettings (void)

{
    ulx = networkXmin;
    uly = networkYmin;
    lrx = networkXmax;
    lry = networkYmax;
    FixedUnitSize = ui_psBoolean(!ui_prVal.size);
    ShapeCircle = ui_psBoolean(ui_prVal.shape);
    TransText = ui_psBoolean(ui_prVal.text);
    ShowBorder = ui_psBoolean(!ui_prVal.border);
    ClipOnBorder = ui_psBoolean(!ui_prVal.clip);
    ug = ui_prVal.unitGray;
}


/*****************************************************************************
  FUNCTION : swap

  PURPOSE  : 
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void swap (float *a, float *b)

{
    float h;

    h = *a;
    *a = *b;
    *b = h;
}   


/*****************************************************************************
  FUNCTION : ui_setupParams

  PURPOSE  : reads the new values into the ps variables
  RETURNS  : void
  NOTES    : called by the printer setup

  UPDATE   :
*****************************************************************************/

static void ui_setupParams (struct Ui_DisplayType *displayPtr)

{
    float frameWidth; 
    float paperWidth, paperHeight, borderVertical, borderHorizontal; /* in points */
    int colIndex;
    time_t t;
    float mm2points = 2.834646;

    pw = paperFormats[ui_prVal.paper][0];
    ph = paperFormats[ui_prVal.paper][1];
    LandscapeMode = ui_psBoolean(ui_prVal.orient);

    bv = ui_xFloatFromAsciiWidget(borderVert);
    bh = ui_xFloatFromAsciiWidget(borderHoriz);

    AutoScale = ui_psBoolean(!ui_prVal.autoScale);
    AutoUni = ui_psBoolean(!ui_prVal.autoUni);
    
    ShowTop = ui_psBoolean((displayPtr->setup).showTitleFlg);
    ShowBottom = ui_psBoolean((displayPtr->setup).showValueFlg);
    Direction = ui_psBoolean((displayPtr->setup).showDirectionFlg);
    ShowWeight = ui_psBoolean((displayPtr->setup).showWeightFlg);
    rs = displayPtr->gridSize;
    ms = ui_unitWidth;   

    ImageWidth = (lrx - ulx) * rs + ms;
    ImageHeight = (lry - uly) * rs + ms;
    if ((displayPtr->setup).showTitleFlg)
        ImageHeight += ufs + 1;
    if ((displayPtr->setup).showValueFlg)
        ImageHeight += ufs + 1;

    paperWidth = pw * mm2points;
    paperHeight = ph * mm2points;
    borderVertical = bv * mm2points;
    borderHorizontal = bh * mm2points;
    frameWidth = blw * mm2points;

    if (ui_prVal.border == UI_BORDER_OFF)
       frameWidth = 0;
   
    if (ui_prVal.orient == UI_ORIENTATION_LANDSCAPE) {
       swap(&ph, &pw);
     }

    pw -= 2 * bh;
    ph -= 2 * bv;

    isx = isy = 1.0;
    if (ui_prVal.autoScale == UI_AUTOSCALE_ON) {
        isx = (pw * mm2points) / ImageWidth;
        isy = (ph * mm2points) / ImageHeight;
        if (ui_prVal.autoUni == UI_AUTOUNI_ON) {
            isx = min(isx, isy);
            isy = isx;
        }
    } else {
        isx = ui_xFloatFromAsciiWidget(psxScale);
        isy = ui_xFloatFromAsciiWidget(psyScale);
    }

    if (ui_prVal.border == UI_BORDER_OFF) {
        if (ui_prVal.orient == UI_ORIENTATION_PORTAIT) {
            bb_xmin = (paperWidth - ImageWidth * isx) / 2;
            bb_ymin = (paperHeight - ImageHeight * isy) / 2;
            bb_xmax = (paperWidth + ImageWidth * isx) / 2;
            bb_ymax = (paperHeight + ImageHeight * isy) / 2;
	} else {
            bb_xmin = (paperWidth - ImageHeight * isy) / 2;
            bb_ymin = (paperHeight - ImageWidth * isx) / 2;
            bb_xmax = (paperWidth + ImageHeight * isy) / 2;
            bb_ymax = (paperHeight + ImageWidth * isx) / 2;
	}
    pw += 2 * bh;
    ph += 2 * bv;
        bv = bb_xmin / mm2points;
        bh = bb_ymin / mm2points;
    pw -= 2 * bh;
    ph -= 2 * bv;
    } else {
        bb_xmin = (ui_prVal.orient ? borderVertical : borderHorizontal) - frameWidth;
        bb_ymin = (ui_prVal.orient ? borderHorizontal : borderVertical) - frameWidth;
        bb_xmax = paperWidth + bb_xmin + 2 * frameWidth;
        bb_ymax = paperHeight + bb_ymin + 2 * frameWidth;

    }

    if (ui_prVal.color == UI_COLOR_ON) {
        colIndex = (displayPtr->setup).textColorIndex;
        TextRed = ui_editColorRGB[colIndex][0]; 
        TextGreen = ui_editColorRGB[colIndex][1]; 
        TextBlue = ui_editColorRGB[colIndex][2]; 
        colIndex = (displayPtr->setup).backgroundColorIndex;
        BackgroundRed = ui_editColorRGB[colIndex][0]; 
        BackgroundGreen = ui_editColorRGB[colIndex][1]; 
        BackgroundBlue = ui_editColorRGB[colIndex][2];
    } else {
        TextRed = TextGreen = TextBlue = 0.0;
        BackgroundRed = BackgroundGreen = BackgroundBlue = 1.0;
    } 
    
    time(&t);
    timeStr = ctime(&t);
}


/*****************************************************************************
  FUNCTION : ui_printProlog

  PURPOSE  : writes the postscript prolog
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_printProlog (void)

{
    fprintf (psOutfile, "%%!PS-Adobe-2.0\n");
    fprintf (psOutfile, "%%%%Copyright (c) 1992-1998 SNNS Group\n");
    fprintf (psOutfile, "%%%%Title: SNNS network: %s\n", ui_filenameNET);
    fprintf (psOutfile, "%%%%Creator: SNNS Version 4.2\n");
    fprintf (psOutfile, "%%%%CreationDate: %s", timeStr);
    fprintf (psOutfile, "%%%%Pages: 0\n");
/*
    fprintf (psOutfile, "%%%%BoundingBox: %4.1f %4.1f  %4.1f %4.1f\n",
                         epsX, epsY, epsWidth, epsHeight);
*/
    fprintf (psOutfile, "%%%%BoundingBox: %d %d  %d %d\n",
                         (int) (bb_xmin - 0.5), (int) (bb_ymin - 0.5), 
                         (int) (bb_xmax + 0.5), (int) (bb_ymax + 0.5));
    fprintf (psOutfile, "%%%%DocumentFonts: %s\n", fo);
    fprintf (psOutfile, "%%%%EndComments\n\n");

    fprintf (psOutfile, "%%%%BeginProcSet: SNNSGeneralSet 1.0 0\n");
    fprintf (psOutfile, "%%Fixed stuff - DON'T CHANGE THIS!\n");
    fprintf (psOutfile, "/inch { 72 mul } bind def\n");
    fprintf (psOutfile, "/cm { 28.34646 mul } bind def\n");
    fprintf (psOutfile, "/mm { 2.834646 mul } bind def\n");
    fprintf (psOutfile, "/lw 72 300 div def\n");
    fprintf (psOutfile, "/min { 2 copy gt { exch } if pop } bind def\n");
    fprintf (psOutfile, "/max { 2 copy lt { exch } if pop } bind def\n");
    fprintf (psOutfile, "/sgn {dup 0 eq {pop 0}{0 gt {1}{-1} ifelse} ifelse} bind def\n");
    fprintf (psOutfile, "%%%%EndProcSet\n");
    fprintf (psOutfile, "\n");
}


/*****************************************************************************
  FUNCTION : ui_printParams

  PURPOSE  : writes the postscript variables
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_printParams (void)

{
    fprintf (psOutfile, "%%%%BeginSetup\n");
    fprintf (psOutfile, "%%General Settings\n\n");
    fprintf (psOutfile, "/tt %s def\n", TransText);
    fprintf (psOutfile, "/sb %s def\n", ShowBorder);
    fprintf (psOutfile, "/cab %s def\n", ClipOnBorder);
    fprintf (psOutfile, "/fo /%s def\n", fo);
    fprintf (psOutfile, "/rs %d def\n", rs);
    fprintf (psOutfile, "/ph %.2f mm def\n", ph);
    fprintf (psOutfile, "/pw %.2f mm def\n", pw);
    fprintf (psOutfile, "/bh %.2f mm def\n", bh);
    fprintf (psOutfile, "/bv %.2f mm def\n", bv);
    fprintf (psOutfile, "/blw %.2f mm def\n", blw);
    fprintf (psOutfile, "/LandscapeMode %s def\n\n", LandscapeMode);

    fprintf (psOutfile, "/cabb true def\n");
/*
    fprintf (psOutfile, "/bbllx %.1f def\n", epsX);
    fprintf (psOutfile, "/bblly %.1f def\n", epsY);
    fprintf (psOutfile, "/bburx %.1f def\n", epsWidth);
    fprintf (psOutfile, "/bbury %.1f def\n\n", epsHeight);
*/
    fprintf (psOutfile, "/bbllx %.1f def\n", bb_xmin);
    fprintf (psOutfile, "/bblly %.1f def\n", bb_ymin);
    fprintf (psOutfile, "/bburx %.1f def\n", bb_xmax);
    fprintf (psOutfile, "/bbury %.1f def\n\n", bb_ymax);

    fprintf (psOutfile, "/ulx %d def\n", ulx);
    fprintf (psOutfile, "/uly %d def\n", uly);
    fprintf (psOutfile, "/lrx %d def\n", lrx);
    fprintf (psOutfile, "/lry %d def\n\n", lry);

    fprintf (psOutfile, "/ter %.2f def\n", TextRed);
    fprintf (psOutfile, "/teg %.2f def\n", TextGreen);
    fprintf (psOutfile, "/teb %.2f def\n", TextBlue);
    fprintf (psOutfile, "/bgr %.2f def\n", BackgroundRed);
    fprintf (psOutfile, "/bgg %.2f def\n", BackgroundGreen);
    fprintf (psOutfile, "/bgb %.2f def\n\n", BackgroundBlue);

    fprintf (psOutfile, "/ms %d def\n", ms);
    fprintf (psOutfile, "/sc %s def\n", ShapeCircle);
    fprintf (psOutfile, "/ufs %d def\n", ufs);
    fprintf (psOutfile, "/sht %s def\n", ShowTop);
    fprintf (psOutfile, "/shb %s def\n", ShowBottom);
    fprintf (psOutfile, "/ulw %.2f def\n", ulw);

    fprintf (psOutfile, "/di %s def\n", Direction);
    fprintf (psOutfile, "/sw %s def\n", ShowWeight);
    fprintf (psOutfile, "/wfs %d def\n", wfs);

    fprintf (psOutfile, "/iw %.2f def\n", ImageWidth);
    fprintf (psOutfile, "/ih %.2f def\n", ImageHeight);
    fprintf (psOutfile, "/isx %.2f def\n", isx);
    fprintf (psOutfile, "/isy %.2f def\n", isy);
    fprintf (psOutfile, "%%%%EndSetup\n\n");
}


/*****************************************************************************
  FUNCTION : ui_printProc

  PURPOSE  : the main print procedure
  RETURNS  : ui_printProc
  NOTES    : callback for the print button in the printer pannel

  UPDATE   :
*****************************************************************************/

static void ui_printProc (void)

{
    int        ret;
    char       *commandLine, *s;
    char       buf[40];

    if ((ui_prVal.displayToPrint > ui_displ_numberOfItems) OR
        (ui_displ_listPtr == NULL)) {
        sprintf (buf, "Error! Display %d is not open\n", ui_prVal.displayToPrint);
	ui_confirmOk(buf);
        return;
    }
    if (ui_prVal.displayToPrint < 1) {
        sprintf (buf, "Error! Display %d doesn't exits \n", ui_prVal.displayToPrint);
	ui_confirmOk(buf);
        return;
    }
         
    displayPrintPtr = ui_displ_listPtr;
    while ((displayPrintPtr->nextPtr != NULL) AND 
           (displayPrintPtr->displayNo != ui_prVal.displayToPrint))
        displayPrintPtr = displayPrintPtr->nextPtr;   
#ifndef HEADER_COMPILE
    ui_readPrintHeader ();
#endif
    if ((fileName = malloc (UI_STR_LEN)) == NULL) {
        ui_confirmOk("Error! Can't allocate Memory\n");
        return;
    }
    if (ui_prVal.dest == UI_DEST_PRINTER) {
       ui_xStringFromAsciiWidget(cmdLine, ui_prVal.cmdLineStr, UI_STR_LEN);
       fileName = tempnam(ui_tmpDir, ui_praefix);
       if (fileName == NULL) {
	    ui_confirmOk("Error! Can't print\n");
            return;
       }
       if ((commandLine = malloc (UI_STR_LEN)) == NULL) {
           ui_confirmOk("Error! Can't allocate Memory\n");
           return;
       }
       *commandLine = '\0';
       for (s=ui_prVal.cmdLineStr; *s!='\0'; s++) {
           if ((*s == '$') AND (*(s+1) == '1')) {
               strcat (commandLine, fileName);
               s++;
	    } else {
               strncat (commandLine, s, 1);
	    }
       }
    } else {
       ui_xStringFromAsciiWidget(cmdLine, fileName, UI_STR_LEN);
    }

    if ((psOutfile = fopen (fileName, "w")) == NULL) {
	ui_confirmOk("Error! Can't open outfile\n");
        return;
    }
    ui_setupParams(displayPrintPtr);
    ui_printProlog();
    ui_printParams();
    fprintf (psOutfile, "%s", psHeader);
    fprintf (psOutfile, "\n%%get font\n");
    fprintf (psOutfile, " /fs wfs def\n");
    fprintf (psOutfile, " fo findfont fs scalefont setfont\n\n");
    ui_printLinks(displayPrintPtr);
    fprintf (psOutfile, "\n%%get font\n");
    fprintf (psOutfile, " /fs ufs def\n");
    fprintf (psOutfile, " fo findfont fs scalefont setfont\n\n");
    ui_printUnits(displayPrintPtr);
    fprintf (psOutfile, "\n showpage\n");
    fprintf (psOutfile, "%%%%EndPage\n\n");
    fprintf (psOutfile, "%%%%Trailer\n\n");
    
    fclose (psOutfile);
    free (fileName);
    
    if (ui_prVal.dest == UI_DEST_PRINTER) {
        ret = system(commandLine);
        if (ret != 0)
	    ui_confirmOk("Error! Can't print\n");
        free (commandLine);
    }
}


/*****************************************************************************
  FUNCTION : ui_closePrintPannel

  PURPOSE  : closes the printer pannel
  RETURNS  : void
  NOTES    : callback for the done button in the printer pannel

  UPDATE   :
*****************************************************************************/

static void ui_closePrintPannel (Widget w, Widget pannel, caddr_t call_data)

{
    ui_printPannelIsOpen = FALSE;
    XtDestroyWidget (pannel);
}



/*****************************************************************************
  FUNCTION : ui_setCmdLine

  PURPOSE  : toggles the first widget in the printer pannel between
             command fine and file name
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setCmdLine (int toggle)

{
    char str1[15], str2[UI_STR_LEN];
    Arg  arg[2];

    if (toggle == UI_DEST_PRINTER) {
        sprintf (str1, "%s", "Command Line");
        sprintf (str2, "%s", ui_prVal.cmdLineStr);
    } else {
        sprintf (str1, "%s", "File Name");
        sprintf (str2, "%s", ui_prVal.fileNameStr);
    }
    ui_prVal.dest = toggle;
    XtSetArg(arg[0], XtNlabel, str1);
    XtSetValues(cmdLabel, arg, 1);
    XtSetArg(arg[0], XtNstring, str2);
    XtSetValues(cmdLine, arg, 1);
}


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_setFormatProc
  
  PURPOSE  : toggles the format radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setFormatProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.format, noOfFmtToggles, formatToggle);
}

#endif /* _UNUSED_FUNCTIONS_ */


/*****************************************************************************
  FUNCTION : ui_setDestProc

  PURPOSE  : toggles the destination radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setDestProc (Widget w, int value, caddr_t call_data)

{
    ui_setCmdLine(value);
    ui_setToggles(value, &ui_prVal.dest, noOfDestToggles, destToggle);
}


/*****************************************************************************
  FUNCTION : ui_setPaperProc

  PURPOSE  : toggles the paper radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setPaperProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.paper, noOfPaperToggles, paperToggle);
    pw = paperFormats[value][0];
    ph = paperFormats[value][1];
}


/*****************************************************************************
  FUNCTION : ui_setOrientProc

  PURPOSE  : toggles the orientation radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setOrientProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.orient, noOfOrientToggles, orientToggle);
}


/*****************************************************************************
  FUNCTION : ui_setAutoScaleProc

  PURPOSE  : toggles the autoscale radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setAutoScaleProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.autoScale, noOfAsToggles, autoScaleToggle);
}


/*****************************************************************************
  FUNCTION : ui_setAutoUniProc

  PURPOSE  : toggles the autouni radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setAutoUniProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.autoUni, noOfAuToggles, autoUniToggle);
}


/*****************************************************************************
  FUNCTION : ui_setSizeProc

  PURPOSE  : toggles the size radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setSizeProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.size, noOfSizeToggles, sizeToggle);
}


/*****************************************************************************
  FUNCTION : ui_setShapeProc

  PURPOSE  : toggles the shape radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setShapeProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.shape, noOfShapeToggles, shapeToggle);
}



/*****************************************************************************
  FUNCTION : ui_setTextProc

  PURPOSE  : toggles the text radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setTextProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.text, noOfTextToggles, textToggle);
}


/*****************************************************************************
  FUNCTION : ui_setBorderProc

  PURPOSE  : toggles the border radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setBorderProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.border, noOfBorderToggles, borderToggle);
}


#ifdef _UNUSED_FUNCTIONS_

/*****************************************************************************
  FUNCTION : ui_setClipProc

  PURPOSE  : toggles the clip radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setClipProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.clip, noOfClipToggles, clipToggle);
}

#endif /* _UNUSED_FUNCTIONS_ */


/*****************************************************************************
  FUNCTION : ui_setColorProc

  PURPOSE  : toggles the color radio button group
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setColorProc (Widget w, int value, caddr_t call_data)

{
    ui_setToggles(value, &ui_prVal.color, noOfColorToggles, colorToggle);
}


/*****************************************************************************
  FUNCTION : ui_printSetupProc

  PURPOSE  : creates the network setup pannel
  RETURNS  : void
  NOTES    : callback for the networkbutton in the printer pannel

  UPDATE   :
*****************************************************************************/

static void ui_printSetupProc (Widget w, Widget button, caddr_t call_data)

{
    int n;
    Arg arg[10];
    char buf[20];
    Position xPos, yPos;
    Dimension width, height;
    Widget networkPannel, pannel, border, done, label;
    Widget label1, label2, label3, label4;

    n = 0;
    XtSetArg (arg[n], XtNwidth, &width); n++;
    XtSetArg (arg[n], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);

    XtTranslateCoords (button, (Position) (width / 2), (Position) (height / 2),
                       &xPos, &yPos);

    n = 0;
    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;
    networkPannel = XtCreatePopupShell ("network setup", transientShellWidgetClass,
                                      button, arg, (unsigned int) n);

    border = XtCreateManagedWidget("border", formWidgetClass,
                                   networkPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget("pannel", formWidgetClass, border, NULL, ZERO);

    done = ui_xCreateButtonItem ("done", border, NULL, pannel);
    XtAddCallback (done, XtNcallback, (XtCallbackProc) ui_closeNetworkPannel,
                   (caddr_t) networkPannel);
    label1 = ui_xCreateLabelItem ("x-min", pannel, ui_fontWidth * 6,
                                   NULL, NULL);
    sprintf (buf, "%d", networkXmin);
    ulxPos = ui_xCreateDialogItem ("", pannel, buf, ui_fontWidth * 7, 
                                   label1, NULL);
    label2 = ui_xCreateLabelItem ("y-min", pannel, ui_fontWidth * 6,
                                   ulxPos, NULL);
    sprintf (buf, "%d", networkYmin);
    ulyPos = ui_xCreateDialogItem ("", pannel, buf, ui_fontWidth * 7, 
                                   label2, NULL);
    label3 = ui_xCreateLabelItem ("x-max", pannel, ui_fontWidth * 6,
                                   NULL, label1);
    sprintf (buf, "%d", networkXmax);
    lrxPos = ui_xCreateDialogItem ("", pannel, buf, ui_fontWidth * 7, 
                                   label3, label1);
    label4 = ui_xCreateLabelItem ("y-max", pannel, ui_fontWidth * 6,
                                   lrxPos, label1);
    sprintf (buf, "%d", networkYmax);
    lryPos = ui_xCreateDialogItem ("", pannel, buf, ui_fontWidth * 7, 
                                   label4, label1);

    label = ui_xCreateLabelItem ("Unitsize", pannel, ui_fontWidth * 10,
                                   NULL, label3); 
    sizeToggle[0] = ui_xCreateToggleItem ("fixed", pannel, NULL,
                                    label, label3);
    XtAddCallback (sizeToggle[0], XtNcallback, (XtCallbackProc) ui_setSizeProc, (caddr_t) UI_UNITSIZE_FIXED);
    sizeToggle[1] = ui_xCreateToggleItem ("value2", pannel, NULL,
                                    sizeToggle[0], label3);
    XtAddCallback (sizeToggle[1], XtNcallback, (XtCallbackProc) ui_setSizeProc, (caddr_t) UI_UNITSIZE_VALUE);

    label = ui_xCreateLabelItem ("Shape", pannel, ui_fontWidth * 10,
                                   NULL, sizeToggle[0]); 
    shapeToggle[0] = ui_xCreateToggleItem ("rectangle", pannel, NULL,
                                    label, sizeToggle[0]);
    XtAddCallback (shapeToggle[0], XtNcallback, (XtCallbackProc) ui_setShapeProc, (caddr_t) UI_SHAPE_RECT);
    shapeToggle[1] = ui_xCreateToggleItem ("circle", pannel, NULL,
                                    shapeToggle[0], sizeToggle[0]);
    XtAddCallback (shapeToggle[1], XtNcallback, (XtCallbackProc) ui_setShapeProc, (caddr_t) UI_SHAPE_CIRCLE);

    label = ui_xCreateLabelItem ("Text", pannel, ui_fontWidth * 10,
                                   NULL, shapeToggle[0]); 
    textToggle[0] = ui_xCreateToggleItem ("solid2", pannel, NULL,
                                    label, shapeToggle[0]);
    XtAddCallback (textToggle[0], XtNcallback, (XtCallbackProc) ui_setTextProc, (caddr_t) UI_TEXT_SOLID);
    textToggle[1] = ui_xCreateToggleItem ("trans", pannel, NULL,
                                    textToggle[0], shapeToggle[0]);
    XtAddCallback (textToggle[1], XtNcallback, (XtCallbackProc) ui_setTextProc, (caddr_t) UI_TEXT_TRANS);

    label = ui_xCreateLabelItem ("Border", pannel, ui_fontWidth * 10,
                                   NULL, textToggle[0]); 
    borderToggle[0] = ui_xCreateToggleItem ("on", pannel, NULL,
                                    label, textToggle[0]);
    XtAddCallback (borderToggle[0], XtNcallback, (XtCallbackProc) ui_setBorderProc, (caddr_t) UI_BORDER_ON);
    borderToggle[1] = ui_xCreateToggleItem ("off", pannel, NULL,
                                    borderToggle[0], textToggle[0]);
    XtAddCallback (borderToggle[1], XtNcallback, (XtCallbackProc) ui_setBorderProc, (caddr_t) UI_BORDER_OFF);

    label = ui_xCreateLabelItem ("Color", pannel, ui_fontWidth * 10,
                                   NULL, borderToggle[0]); 
    colorToggle[0] = ui_xCreateToggleItem ("on", pannel, NULL,
                                    label, borderToggle[0]);
    XtAddCallback (colorToggle[0], XtNcallback, (XtCallbackProc)ui_setColorProc,
		   (caddr_t) UI_COLOR_ON);
    colorToggle[1] = ui_xCreateToggleItem ("off", pannel, NULL,
                                    colorToggle[0], borderToggle[0]);
    XtAddCallback (colorToggle[1], XtNcallback, (XtCallbackProc)ui_setColorProc,
		   (caddr_t) UI_COLOR_OFF);

    label = ui_xCreateLabelItem ("Fill Intens.", pannel, ui_fontWidth * 10,
                                  NULL, colorToggle[0]); 
    sprintf (buf, "%.2f", ui_prVal.unitGray);
    fillIntens = ui_xCreateDialogItem ("", pannel, buf, ui_fontWidth * 7, 
                                   label, colorToggle[0]);

    label = ui_xCreateLabelItem ("Display", pannel, ui_fontWidth * 10,
                                  NULL, fillIntens); 
    sprintf (buf, "%d", ui_prVal.displayToPrint);
    assDisplay = ui_xCreateDialogItem ("", pannel, buf, ui_fontWidth * 7, 
                                   label, fillIntens);

    ui_checkWindowPosition(networkPannel);
    XtPopup (networkPannel, XtGrabExclusive);
    ui_xDontResizeWidget(networkPannel); 

    ui_setToggles(ui_prVal.border, &ui_prVal.border, noOfBorderToggles, 
		  borderToggle);
    ui_setToggles(ui_prVal.color, &ui_prVal.color,noOfColorToggles,colorToggle);
    ui_setToggles(ui_prVal.size, &ui_prVal.size, noOfSizeToggles, sizeToggle);
    ui_setToggles(ui_prVal.shape, &ui_prVal.shape,noOfShapeToggles,shapeToggle);
    ui_setToggles(ui_prVal.text, &ui_prVal.text, noOfTextToggles, textToggle);

}


/*****************************************************************************
  FUNCTION : u_printNet

  PURPOSE  : creates the printer setup pannel
  RETURNS  : void
  NOTES    : callback for the print item in the the gui menu

  UPDATE   :
*****************************************************************************/

void ui_printNet (void)

{
    Widget       border, pannel, done, network, print, label;
    Widget       label1, label2, label3;
    Arg          arg[25];
    Cardinal     n;
    char         buf[UI_STR_LEN];
    int          labelWidth = 11;

    if (ui_printPannelIsOpen){
       XRaiseWindow (XtDisplay (ui_printPannel), XtWindow (ui_printPannel));
       return;
    }

    ui_printPannelIsOpen = TRUE;

    if (!ui_printDefaultsAreThere) {
         ui_defaultPrinterValues();
         ui_printDefaultsAreThere = TRUE;
    }

    ui_getNetworkSize (&networkXmin, &networkYmin, &networkXmax, &networkYmax);
    ui_transferSettings();

    sprintf(buf, "SNNS printer setup");
    n = 0;
    ui_printPannel = XtCreatePopupShell(buf, topLevelShellWidgetClass, 
					ui_toplevel, arg, n);
    border = XtCreateManagedWidget("border", boxWidgetClass,
                                   ui_printPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget("pannel", formWidgetClass, 
                                   border, NULL, ZERO);

    print = ui_xCreateButtonItem ("print", border, NULL, NULL);
    network = ui_xCreateButtonItem ("network", border, print, NULL);
    done = ui_xCreateButtonItem ("done", border, network, NULL);

    XtAddCallback (done, XtNcallback, (XtCallbackProc) ui_closePrintPannel,
                   (caddr_t) ui_printPannel);

    XtAddCallback (print, XtNcallback, (XtCallbackProc) ui_printProc, NULL);
    XtAddCallback (network, XtNcallback, (XtCallbackProc) ui_printSetupProc, 
		   (caddr_t) network);

    cmdLabel = ui_xCreateLabelItem ("Command", pannel, ui_fontWidth *labelWidth,
                                   NULL, NULL); 

    cmdLine = ui_xCreateDialogItem ("cmd_line", pannel, ui_prVal.cmdLineStr, 
				    ui_fontWidth * 40,    cmdLabel, NULL);

    label = ui_xCreateLabelItem ("Destination", pannel, ui_fontWidth*labelWidth,
                                   NULL, cmdLine); 
    destToggle[0] = ui_xCreateToggleItem ("printer", pannel, NULL,
                                    label, cmdLine);
    XtAddCallback (destToggle[0], XtNcallback, (XtCallbackProc) ui_setDestProc,
		   (caddr_t) UI_DEST_PRINTER);
    destToggle[1] = ui_xCreateToggleItem ("file", pannel, NULL,
                                    destToggle[0], cmdLine);
    XtAddCallback (destToggle[1], XtNcallback, (XtCallbackProc) ui_setDestProc,
		   (caddr_t) UI_DEST_FILE);

    label = ui_xCreateLabelItem ("Paper", pannel, ui_fontWidth * labelWidth,
                                   NULL, destToggle[0]); 
    paperToggle[0] = ui_xCreateToggleItem ("dina4", pannel, NULL,
                                    label, destToggle[0]);
    XtAddCallback (paperToggle[0], XtNcallback, (XtCallbackProc)ui_setPaperProc,
		   (caddr_t) UI_PAPER_DINA4);
    paperToggle[1] = ui_xCreateToggleItem ("dina3", pannel, NULL,
                                    paperToggle[0], destToggle[0]);
    XtAddCallback (paperToggle[1], XtNcallback, (XtCallbackProc)ui_setPaperProc,
		   (caddr_t) UI_PAPER_DINA3);
    paperToggle[2] = ui_xCreateToggleItem ("usletter", pannel, NULL,
                                    paperToggle[1], destToggle[0]);
    XtAddCallback (paperToggle[2], XtNcallback, (XtCallbackProc)ui_setPaperProc,
		   (caddr_t) UI_PAPER_USLETTER);

    label = ui_xCreateLabelItem("Orientation", pannel, ui_fontWidth*labelWidth,
                                   NULL, paperToggle[0]); 
    orientToggle[0] = ui_xCreateToggleItem ("portrait", pannel, NULL,
                                    label, paperToggle[0]);
    XtAddCallback (orientToggle[0],XtNcallback,(XtCallbackProc)ui_setOrientProc,
		   (caddr_t) UI_ORIENTATION_PORTAIT);
    orientToggle[1] = ui_xCreateToggleItem ("landscape", pannel, NULL,
                                    orientToggle[0], paperToggle[0]);
    XtAddCallback(orientToggle[1], XtNcallback,(XtCallbackProc)ui_setOrientProc,
		  (caddr_t) UI_ORIENTATION_LANDSCAPE);

    label = ui_xCreateLabelItem ("Border (mm)", pannel, ui_fontWidth*labelWidth,
                                   NULL, orientToggle[0]);
    label1 = ui_xCreateLabelItem ("horiz", pannel, ui_fontWidth * 7,
                                   label, orientToggle[0]);
    sprintf (buf, "%.2f", ui_prVal.borderHoriz); 
    borderHoriz = ui_xCreateDialogItem ("horiz", pannel, buf, ui_fontWidth * 7, 
                                   label1, orientToggle[0]);
    label2 = ui_xCreateLabelItem ("vert", pannel, ui_fontWidth * 7,
                                   borderHoriz, orientToggle[0]);
    sprintf (buf, "%.2f", ui_prVal.borderVert); 
    borderVert = ui_xCreateDialogItem ("vert", pannel, buf, ui_fontWidth * 7, 
                                   label2, orientToggle[0]);

    label = ui_xCreateLabelItem ("X-Scale", pannel, ui_fontWidth * labelWidth,
                                   NULL, label1);
    sprintf (buf, "%.2f", ui_prVal.scaleValX); 
    psxScale = ui_xCreateDialogItem ("xscale", pannel, buf, ui_fontWidth * 7, 
                                   label, label1);

    label3 = ui_xCreateLabelItem ("   Y-Scale", pannel, ui_fontWidth * 10,
                                   psxScale, label1);
    sprintf (buf, "%.2f", ui_prVal.scaleValY); 
    psyScale = ui_xCreateDialogItem ("yscale", pannel, buf, ui_fontWidth * 7, 
                                   label3, label1);

    label = ui_xCreateLabelItem ("AutoScale", pannel, ui_fontWidth * labelWidth, 
                                   NULL, psxScale); 
    autoScaleToggle[0] = ui_xCreateToggleItem ("on", pannel, NULL,
                                    label, psxScale);
    XtAddCallback(autoScaleToggle[0], XtNcallback, 
		  (XtCallbackProc)ui_setAutoScaleProc,(caddr_t)UI_AUTOSCALE_ON);
    autoScaleToggle[1] = ui_xCreateToggleItem ("off", pannel, NULL,
                                    autoScaleToggle[0], psxScale);
    XtAddCallback(autoScaleToggle[1], XtNcallback, 
		 (XtCallbackProc)ui_setAutoScaleProc,(caddr_t)UI_AUTOSCALE_OFF);

    label1 = ui_xCreateLabelItem ("   Aspect", pannel, ui_fontWidth * 8,
                                   autoScaleToggle[1], psxScale); 
    autoUniToggle[0] = ui_xCreateToggleItem ("on", pannel, NULL,
                                    label1, psxScale);
    XtAddCallback (autoUniToggle[0], XtNcallback, 
		   (XtCallbackProc) ui_setAutoUniProc, (caddr_t) UI_AUTOUNI_ON);
    autoUniToggle[1] = ui_xCreateToggleItem ("off", pannel, NULL,
                                    autoUniToggle[0], psxScale);
    XtAddCallback (autoUniToggle[1], XtNcallback, 
		   (XtCallbackProc) ui_setAutoUniProc, (caddr_t)UI_AUTOUNI_OFF);


    XtAddEventHandler(pannel,KeyPressMask,FALSE,
		      (XtEventHandler)ui_key_control,(Cardinal *) 0);
    ui_checkWindowPosition(ui_printPannel);
    XtPopup (ui_printPannel, XtGrabNone);
    ui_xDontResizeWidget(ui_printPannel); 

    ui_setCmdLine(ui_prVal.dest);
    ui_setToggles(ui_prVal.dest, &ui_prVal.dest, noOfDestToggles, destToggle);
    ui_setToggles(ui_prVal.paper,&ui_prVal.paper,noOfPaperToggles,paperToggle); 
    ui_setToggles(ui_prVal.autoScale, &ui_prVal.autoScale, noOfAsToggles, 
		  autoScaleToggle);
    ui_setToggles(ui_prVal.autoUni, &ui_prVal.autoUni, noOfAuToggles, 
		  autoUniToggle);
    ui_setToggles(ui_prVal.orient, &ui_prVal.orient, noOfOrientToggles, 
		  orientToggle);
}


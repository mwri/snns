/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_netGraph.c,v $
  SHORTNAME      : netGraph.c
  SNNS VERSION   : 4.2

  PURPOSE        : routines to draw all graphic elements of the net
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 18.5.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:39 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <math.h>

#include "ui.h"
#include "ui_utilP.h"
#include "ui_setup.h"
#include "ui_xGraphic.h"
#include "ui_color.h"
#include "ui_mainP.h"
#include "kr_typ.h"
#include "kr_const.h"
#include "kr_def.h"
#include "kernel.h"
#include "kr_mac.h"
#include "glob_typ.h"

#include "kr_ui.h"

#include "ui_display.h"

#include "ui_netGraph.ph"


#if !defined( M_PI )
#  define M_PI  3.14159265358979323846
#endif


/*****************************************************************************
  FUNCTION : ui_drawGrowingThing

  PURPOSE  : draws an growing box according the procent_value (can be 
             either activation or output of a unit)
  RETURNS  : alteration of the graphic
  NOTES    : The size of the box represents the absolut-value of the box.
             The box is filled black --> positive value
	     The box is filled white --> negative value

  UPDATE   :
*****************************************************************************/

void ui_drawGrowingThing (struct Ui_DisplayType *displayPtr, 
	struct PosType gridPos, int procent_value)
     			/* procent_value: [-100,+100] range */
{
    struct PosType   pixUpperLeftPos, pixLowerRightPos;
    int              xSize, ySize;
    Bool             positivValue = TRUE;
    Bool             performDraw  = FALSE;
    int              pVal = procent_value;
    
    if (displayPtr->frozen) {
	positivValue = FALSE;
	pVal = 100;
    } else {
	positivValue  = (pVal >= 0);
	pVal          = abs(pVal);
    }

    pixUpperLeftPos   = ui_utilGridToPix(displayPtr, gridPos);
    
    xSize             = (ui_unitWidth  * pVal + 50 /* round */) / 100;
    ySize             = (ui_unitHeight * pVal + 50 /* round */) / 100;
    
    if (xSize > 0) {
	pixUpperLeftPos.x  -= xSize / 2;
	pixUpperLeftPos.y  -= ySize / 2;
	pixLowerRightPos.x  = pixUpperLeftPos.x + xSize - 1;
	pixLowerRightPos.y  = pixUpperLeftPos.y + ySize - 1;
	performDraw = TRUE;
    }
    
 /* growing bar:

    pixUpperLeftPos   = ui_utilGridToPix(displayPtr, gridPos);
    pixLowerRightPos  = pixUpperLeftPos;
    
    xSize             = ui_unitWidth;
    ySize             = ui_unitHeight * pVal / 100;
    
    if (ySize > 0) {
    pixUpperLeftPos.x  -= xSize / 2;
    pixUpperLeftPos.y  -= (ySize - (ui_unitHeight / 2));
    pixLowerRightPos.x += xSize / 2;
    pixLowerRightPos.y += ui_unitHeight / 2;
    performDraw = TRUE;
    }
	
 */
    if (performDraw) 
	if (positivValue) 
	    ui_xDeleteRect(ui_display, displayPtr->drawable, ui_gc,
			   pixUpperLeftPos, pixLowerRightPos);
	else 
	    ui_xDrawBox(ui_display, displayPtr->drawable, ui_gc,  
			pixUpperLeftPos, pixLowerRightPos);

#ifdef DEBUG
    XFlush(ui_display);
#endif
}


/*****************************************************************************
  FUNCTION : ui_drawUnit

  PURPOSE  : draws a unit whith activation or output in different formats
  RETURNS  : alteration of the graphic
  NOTES    : format options are:
             - UI_DRAW or UI_ERASE (operation)
	     - show activation or output
	     - show value numerically
	     - show unit number
	     - show as growing bar or growing box

  UPDATE   :
*****************************************************************************/

void ui_drawUnit (struct Ui_DisplayType *displayPtr, int unitNo, 
		FlagType operation)

{   
    char               buf[50];
    FlintType          value;
    int                procent_value;
    struct PosType     gridPos, pixPos;

    int                xUpperLeft, yUpperLeft;

    int                direction_hint, font_ascent, font_descent;
    XCharStruct        overall;

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

    if (value >= 0.0) {
	procent_value = 
	    abs((int)(100.0 * value / (displayPtr->setup).unitScaleFactor));
	if (procent_value > 100) procent_value = 100;
	/* triggered     = (value >= (displayPtr->setup).unitPosTrigger); */
    } else {
	procent_value = 
	    -abs((int)(100.0 * value / (displayPtr->setup).unitScaleFactor));
	if (procent_value < -100) procent_value = -100;
	/* triggered     = (-value >= (displayPtr->setup).unitNegTrigger); */
    }
    
    /* if (triggered) { */
    krui_getUnitPosition(unitNo, &gridPos);
    gridPos.x = gridPos.x;
    gridPos.y = gridPos.y;
    gridPos.z = gridPos.z;
    pixPos = ui_utilPixUpperLeft(displayPtr, gridPos);
    xUpperLeft = pixPos.x; 
    yUpperLeft = pixPos.y;
    
    /* set background */
    XSetBackground(ui_display, ui_gc, 
		   ui_backgroundColor);

    if (ui_col_monochromeMode AND (operation != UI_ERASE_BOTTOM)) { 
	XSetFunction(ui_display, ui_gc, GXcopy);
	XSetForeground(ui_display, ui_gc, 
		       ui_backgroundColor);
	ui_xDeleteRect(ui_display, displayPtr->drawable, ui_gc,
		       pixPos, ui_utilPixLowerRight(displayPtr, gridPos,0));
    } 
    
    if (operation == UI_ERASE) {
	/* XDrawImageString() don't uses the FUNCTION value in a gc.
	   Thus invert the colours */
	XSetForeground(ui_display, ui_gc, 
		       ui_backgroundColor);
	XSetFunction(ui_display, ui_gc, GXcopyInverted);
    } else { /* DRAW */
	XSetForeground(ui_display, ui_gc, ui_textColor);
	XSetFunction(ui_display, ui_gc, GXcopy);
    }

    if (operation != UI_ERASE_BOTTOM) {
	/* print number first !! Because the space behind a character is 
	   some pixels deeper than the bottom line of numbers (0,1,2,3...) in
	   this character set. Therefore they would erase a piece of a unit
	   with a high value! */

	if ((displayPtr->setup).showTitleFlg) {
            switch ((displayPtr->setup).showTitle) {
                case UI_NUMBER : sprintf(buf,"%d",unitNo);
                                 break;
	        case UI_ZVALUE : sprintf(buf,"%d",gridPos.z);
                                 break;
        	case UI_NAME   : if ((krui_getUnitName(unitNo) == NULL) OR
                                   (strlen(krui_getUnitName(unitNo)) == 0))
                                     sprintf(buf,"%d",unitNo);
	                         else
 	                             sprintf(buf,"%s", krui_getUnitName(unitNo));
                                 break;
	    	case UI_WINNER : if (krui_getUnitValueA(unitNo)!=0)
	      			     sprintf(buf,"%d",(int)krui_getUnitValueA(unitNo));
		                 else 
                                     *buf=0;
                                 break;

                default        : sprintf(buf,"%d",unitNo);
            }
	    XDrawImageString(ui_display, displayPtr->drawable, ui_gc,
			     xUpperLeft, yUpperLeft-1, buf, (int) strlen(buf));
/*
	    if (((displayPtr->setup).showTitle == UI_NUMBER) OR 
		(krui_getUnitName(unitNo) == NULL) OR
		(strlen(krui_getUnitName(unitNo)) == 0)) {
		sprintf(buf,"%d",unitNo);
		XDrawImageString(ui_display, displayPtr->drawable, ui_gc,
				 xUpperLeft, yUpperLeft-1, buf, strlen(buf));
	    } else {
                if ((displayPtr->setup).showTitle == UI_ZVALUE) {
                    sprintf (buf, "%d", gridPos.z);
        	    XDrawImageString(ui_display, displayPtr->drawable, ui_gc,
				     xUpperLeft, yUpperLeft-1, buf, strlen(buf));
		} else {
	            sprintf(buf,"%s", krui_getUnitName(unitNo));
		    XDrawImageString(ui_display, displayPtr->drawable, ui_gc,
			             xUpperLeft, yUpperLeft-1, buf, strlen(buf));
	        }
	    }
*/
	}
	
#ifdef DEBUG
	XFlush(ui_display);
#endif
    }

    /* now the value */
    
    if ((displayPtr->setup).showValueFlg) {
	sprintf(buf,"%4.3f", value);
	XTextExtents(ui_fontStruct, buf, (int) strlen(buf),
		     &direction_hint, &font_ascent, &font_descent, 
		     &overall);
	XDrawImageString(ui_display, displayPtr->drawable, ui_gc,
			 xUpperLeft, 
			 yUpperLeft + ui_unitHeight + font_ascent + 1, 
			 buf, (int) strlen(buf));
    }
    
#ifdef DEBUG
    XFlush(ui_display);
#endif

    if (operation != UI_ERASE_BOTTOM) {

	/* now draw the unit itsself */
	if (operation == UI_DRAW) {
	    if (NOT ui_col_monochromeMode) {
		XSetForeground(ui_display, ui_gc, 
			       ui_col_rangePixels[ui_col_steps + 
						  procent_value * ui_col_steps 
						  DIV 100]);
		ui_xDeleteRect(ui_display, displayPtr->drawable, ui_gc,
			       pixPos, ui_utilPixLowerRight(displayPtr, gridPos,0));	    
	    } else { 
		XSetFunction(ui_display, ui_gc, GXcopy);
		XSetForeground(ui_display, ui_gc, 
			       ui_textColor);
		ui_drawGrowingThing(displayPtr, gridPos, procent_value);
	    }
	} else { /* ERASE */
	    if (NOT ui_col_monochromeMode) {
		XSetFunction(ui_display, ui_gc, GXcopy);
		XSetForeground(ui_display, ui_gc, 
			       ui_backgroundColor);
		ui_xDeleteRect(ui_display, displayPtr->drawable, ui_gc,
			       pixPos, ui_utilPixLowerRight(displayPtr, gridPos,0));
	    }
	}
	/* if operation == UI_ERASE: there is nothing to draw. area is 
	   already erased by ui_xDeleteRect() */
	
#ifdef DEBUG
	XFlush(ui_display);
#endif
    }
}


/*****************************************************************************
  FUNCTION : ui_arrowPoint

  PURPOSE  : calculates the start-/end-point of a Link
  RETURNS  : the calculated position
  NOTES    : call this routine with exchanged source- and target positions
             to calculate the end-point
	     !!! The algorithm used allows only quadratic units i.e. 
	         ui_unitHeight == ui_unitWidth !!!

  UPDATE   :
*****************************************************************************/

static struct PosType ui_arrowPoint (struct PosType sourcePixPos, 
			struct PosType targetPixPos)

{
    struct PosType  arrowPoint;
    int               deltaX, deltaY;
    int               halfSize = ui_unitWidth / 2 + 1;
                      /* keep a distance from 1 round a 100% unit */

    deltaX   = targetPixPos.x - sourcePixPos.x;
    deltaY   = targetPixPos.y - sourcePixPos.y;
    
    if (abs(deltaX) >= abs(deltaY)) {
	deltaY = deltaY * halfSize / abs(deltaX);
	if (deltaX >= 0) deltaX = halfSize;
	else deltaX = - halfSize;
    } else {
	deltaX = deltaX * halfSize / abs(deltaY);
	if (deltaY >= 0) deltaY = ui_utilSgnInt(deltaY) * halfSize;
	else deltaY = - halfSize;
    }
    
    arrowPoint.x = sourcePixPos.x + deltaX;
    arrowPoint.y = sourcePixPos.y + deltaY;

    return(arrowPoint);
}


/*****************************************************************************
  FUNCTION : ui_angle

  PURPOSE  : returns the angle between the line from source to target and a
             horizontal line from source 
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static double ui_angle (struct PosType source, struct PosType target)

{
  double dx, dy, l, angle ;


  dx = target.x - source.x ;
  dy = target.y - source.y ;
        
  l = sqrt (dx * dx + dy * dy) ;
    
  angle = asin (fabs(dy) / l) * 180.0 / M_PI ;

  if (dx >= 0)
  {
    if (dy >= 0) angle = 360.0 - angle ; 
  }
  else
  {
    if (dy >= 0) angle = 180.0 + angle ;
    else         angle = 180.0 - angle ;
  }

  return (angle) ;
}



/*****************************************************************************
  FUNCTION : ui_drawLink

  PURPOSE  : draws an arrow (or line) from unitPos to targetPos
  RETURNS  : alteration of the network
  NOTES    : The routine checks first
               - have the units the same subNetNo
	       - are both units visible (layers on and visible in a window)
             the format can be:
             - as line or as arrow (i.e. with direction indicator)
	     - with or without weight in numerical form

  UPDATE   :
*****************************************************************************/

void ui_drawLink (struct Ui_DisplayType *displayPtr, int sourceNo, 
			int targetNo, FlintType weight, FlagType operation)
     				 /*  operation:  UI_DRAW or UI_ERASE */
{
    struct PosType   sourcePixPos, targetPixPos, textPixPos;
    struct PosType   sourceGridPos, targetGridPos;
    struct PosType   arrowStart, arrowEnd;
    char             buf[10];
    Bool             triggered;
    int              procent_value;
    FlintType        quotient;
    struct           PosType arc, delta ;
    int              arc_r, arc_dist = 20 ;
    double           angle1, angle2 ;
    double           len ;

    if (NOT (displayPtr->setup).showLinkFlg)
	return;
    
    if (krui_getUnitSubnetNo(sourceNo) != krui_getUnitSubnetNo(targetNo))
	return;

    /* same subnet ... */
    
    if (NOT (ui_isUnitVisibleInDisplay(displayPtr, sourceNo) AND
	     ui_isUnitVisibleInDisplay(displayPtr, targetNo)))
	/* one or both units are not visible */
  	return;
    
    /* one visible layer containing both units was found */
   
    if (weight >= 0.0) 
	triggered = (weight >= (displayPtr->setup).linkPosTrigger);
    else
	triggered = (weight <= (displayPtr->setup).linkNegTrigger);

    if (NOT triggered) return;

    krui_getUnitPosition(sourceNo, &sourceGridPos);
    krui_getUnitPosition(targetNo, &targetGridPos);

    if ((ui_utilAreEqualPositions(sourceGridPos, targetGridPos)) &&
        (sourceNo != targetNo))

        return;

    /****************************************************/
    /* Now everything is checked, get data and draw ... */

    sourcePixPos = ui_utilGridToPix(displayPtr, sourceGridPos);
    targetPixPos = ui_utilGridToPix(displayPtr, targetGridPos);
 
    textPixPos.x  = (targetPixPos.x + sourcePixPos.x)/2 - 10;
    textPixPos.y  = (targetPixPos.y + sourcePixPos.y)/2 +  3;
    
    XSetFunction(ui_display, ui_gc, GXcopy);
     /* set background */
    XSetBackground(ui_display, ui_gc, 
		   ui_backgroundColor);

    if (operation == UI_DRAW) {
	if (NOT ui_col_monochromeMode) {
	    if (weight >= 0.0) {
	      if ((displayPtr->setup).linkScaleFactor != 0.0)
	      {
		quotient = fabs(weight / (displayPtr->setup).linkScaleFactor);
		if (quotient < 1.0)
		    procent_value = (int)(100.0 * quotient);
		else
		    procent_value = 100;
	      }
	      else
		  procent_value = 100;
	      XSetForeground(ui_display, ui_gc, 
			       ui_col_rangePixels[ui_col_steps + 
						  procent_value * ui_col_steps 
						  DIV 100]);
	    } else {
	      if ((displayPtr->setup).linkScaleFactor != 0.0)
	      {
		quotient = fabs(weight / (displayPtr->setup).linkScaleFactor);
		if (quotient < 1.0)
		    procent_value = (int)(100.0 * quotient);
		else
		    procent_value = 100;
	      }
	      else
		  procent_value = 100;
	      XSetForeground(ui_display, ui_gc, 
			       ui_col_rangePixels[ui_col_steps - 
						  procent_value * ui_col_steps 
						  DIV 100]);
	    }	    
	} else
	    XSetForeground(ui_display, ui_gc, 
			   ui_textColor);
    } else { /* ERASE */
	XSetForeground(ui_display, ui_gc, 
		       ui_backgroundColor);
    }
	
    if (sourceNo != targetNo)
    {  
      arrowStart = ui_arrowPoint (sourcePixPos, targetPixPos ) ;
      arrowEnd   = ui_arrowPoint (targetPixPos, sourcePixPos ) ;

      if (krui_areConnected (targetNo, sourceNo)) 
      {
        /* draw link as an arc */

        delta.x = arrowEnd.x - arrowStart.x ;
        delta.y = arrowEnd.y - arrowStart.y ;

        len   = sqrt ((double) (delta.x * delta.x + delta.y * delta.y)) ;
        arc_r = ((delta.x * delta.x + delta.y * delta.y)  
                 + 4 * arc_dist * arc_dist) / (8 * arc_dist) ; 

        if (len < 4.0 * arc_dist) arc_dist = len / 4 ;

        arc.x = arrowStart.x + delta.x/2 - (arc_r - arc_dist) * delta.y/len ;
        arc.y = arrowStart.y + delta.y/2 + (arc_r - arc_dist) * delta.x/len ;

        textPixPos.x = arrowStart.x + delta.x/2 + arc_dist * delta.y/len -17 ;
        textPixPos.y = arrowStart.y + delta.y/2 - arc_dist * delta.x/len + 5 ;

        angle1 = ui_angle (arc, arrowStart) ;
        angle2 = ui_angle (arc, arrowEnd  ) ;
        if (angle1 < angle2) angle1 += 360.0 ;

        XDrawArc (ui_display, displayPtr->drawable, ui_gc, 
                  arc.x - arc_r, arc.y - arc_r, 2 * arc_r, 2 * arc_r, 
                  (int) (angle2 * 64.0),
                  (int) ((angle1  - angle2) * 64.0)) ;

        arrowStart.x = arrowEnd.x + (arrowEnd.y - arc.y) ;
        arrowStart.y = arrowEnd.y - (arrowEnd.x - arc.x) ;

      }
      else
      {
        /* draw link as a line */

        ui_xDrawLine(ui_display, displayPtr->drawable, ui_gc, 
                     arrowStart, arrowEnd) ;
      }
    }
    else
    {
      /* self recurrent link */

      XDrawArc (ui_display, displayPtr->drawable, ui_gc,
                sourcePixPos.x - ui_unitWidth, sourcePixPos.y,
                ui_unitWidth, ui_unitHeight,
                90 * 64, 270*64) ;

      textPixPos.x = sourcePixPos.x - ui_unitWidth    - 32 ;
      textPixPos.y = sourcePixPos.y + ui_unitHeight/2 +  2 ;

      arrowStart.x = sourcePixPos.x - ui_unitWidth/2 - 4 ;
      arrowStart.y = sourcePixPos.y + 1 ;
 
      arrowEnd.x   = arrowStart.x + 3 ;
      arrowEnd.y   = arrowStart.y     ;
    }

#ifdef DEBUG
   XFlush(ui_display);
#endif

    if ((displayPtr->setup).showDirectionFlg) {
	ui_xDrawTriangle(ui_display, displayPtr->drawable, ui_gc, 
			 arrowStart, arrowEnd);
#ifdef DEBUG
	XFlush(ui_display);
#endif
    }

    if ((displayPtr->setup).showWeightFlg) {
	/* Show Link weight if toggle set by the user */ 
	if (operation == UI_DRAW)
	    XSetForeground(ui_display, ui_gc, 
			   ui_textColor);
	sprintf(buf,"%5.3f",weight);
	/* if (operation == UI_DRAW)  */
	    XDrawImageString(ui_display, displayPtr->drawable, ui_gc, 
			     textPixPos.x, textPixPos.y, buf, (int) strlen(buf));
	/* else
	    XDrawString(ui_display, displayPtr->drawable, ui_gc, 
			textPixPos.x, textPixPos.y, buf, strlen(buf)); */
    }
#ifdef DEBUG
    XFlush(ui_display);
#endif

}







/* end of file */
/* lines: 518 */

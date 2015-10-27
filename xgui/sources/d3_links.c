/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_links.c,v $
  SHORTNAME      : links.c
  SNNS VERSION   : 4.2

  PURPOSE        : controls the link pannel
  NOTES          :

  AUTHOR         : Ralf Huebner 
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:54 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <memory.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Toggle.h>
#include "glob_typ.h"
#include "ui.h"
#include "ui_xWidgets.h"
#include "d3_global.h"
#include "d3_xUtils.h"
#include "d3_fonts.h"
#include "d3_main.h"
#include "d3_links.ph"




/*****************************************************************************
  FUNCTION : setLinkToggleState

  PURPOSE  : sets the toggles in the link pannel
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void setLinkToggleState (void)
{
    Boolean onFlag, offFlag, labelFlag, colorFlag;

    onFlag = offFlag = colorFlag = labelFlag = FALSE;
    switch (temp_state.link_mode) {
        case links_off   : offFlag = TRUE;
                           break;
        case links_on    : onFlag = TRUE;
                           break;
        case links_label : labelFlag = TRUE;
                           break;
        case links_color : colorFlag = TRUE;
                           break;
    }
    ui_xSetToggleState (linksOnButton, onFlag);    
    ui_xSetToggleState (linksOffButton, offFlag);    
    ui_xSetToggleState (linksColorButton, colorFlag);    
    ui_xSetToggleState (linksLabelButton, labelFlag);    

}


/*****************************************************************************
  FUNCTION : setLinksOn

  PURPOSE  : sets mode to draw links
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void setLinksOn (void)
{
    temp_state.link_mode = links_on;
    setLinkToggleState (); 
}





/*****************************************************************************
  FUNCTION : setLinksOff

  PURPOSE  : sets mode not to draw links
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void setLinksOff (void)
{
    temp_state.link_mode = links_off;
    setLinkToggleState ();
}





/*****************************************************************************
  FUNCTION : setLinksColor 

  PURPOSE  : set mode to show the weight by color
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void setLinksColor (void)
{
    if (temp_state.color_mode != rgb_mode) {
        ui_xSetToggleState (linksColorButton, FALSE);    
        return;
    }
    temp_state.link_mode = links_color;
    setLinkToggleState ();
}



/*****************************************************************************
  FUNCTION : setLinksLabel 

  PURPOSE  : set mode to show the weight by label
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void setLinksLabel (void)
{
    temp_state.link_mode = links_label;
    setLinkToggleState ();
}




/*****************************************************************************
  FUNCTION : d3_closeLinkPannel

  PURPOSE  : close the link pannel
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void d3_closeLinkPannel (Widget w, Widget pannel, caddr_t call_data)
{
    XtDestroyWidget (pannel);
    if (memcmp (&d3_state, &temp_state, 
                sizeof (d3_state_type)) != 0) {
         memcpy (&d3_state, &temp_state, 
                 sizeof (d3_state_type)); 
         d3_drawNet ();
    }
}





/*****************************************************************************
  FUNCTION : setFontToggleState

  PURPOSE  : sets the link toggles
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void setFontToggleState (void)
{
    switch (temp_state.font)
      {
        case fnt5x8  : ui_xSetToggleState (fontSmallButton, TRUE);
                       ui_xSetToggleState (fontBigButton, FALSE);
                       break;
        case fnt8x14 : ui_xSetToggleState (fontSmallButton, FALSE);
                       ui_xSetToggleState (fontBigButton, TRUE);
                       break;
      }
}




/*****************************************************************************
  FUNCTION : setFontSmall

  PURPOSE  : activates the small font
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void setFontSmall (void)
{
    temp_state.font = fnt5x8;
    d3_select_font (fnt5x8);
    d3_get_font_size (&d3_fontXsize, &d3_fontYsize);
    setFontToggleState (); 
}



/*****************************************************************************
  FUNCTION : setFontBig

  PURPOSE  : activates the large font
  RETURNS  : 
  NOTES    :

******************************************************************************/


static void setFontBig (void)
{
    temp_state.font = fnt8x14;
    d3_select_font (fnt8x14);
    d3_get_font_size (&d3_fontXsize, &d3_fontYsize);
    setFontToggleState (); 
}




/*****************************************************************************
  FUNCTION : d3_createLinkPannel

  PURPOSE  : creates the link pannel
  RETURNS  : 
  NOTES    :

******************************************************************************/



void d3_createLinkPannel (Widget w, Widget button, caddr_t call_data)
{
    int n;
    Arg arg[10];
    Position xPos, yPos;
    Dimension width, height;
    Widget linkPannel, border, pannel, doneButton, linkLabel, fontLabel, dummy;


    n = 0;
    XtSetArg (arg[n], XtNwidth, &width);   n++;
    XtSetArg (arg[n], XtNheight, &height); n++;
    XtGetValues (button, arg, (unsigned int) n);
    XtTranslateCoords (button, (Position) (width / 2), (Position) (height / 2), 
                       &xPos, &yPos);

    n = 0;
    XtSetArg(arg[n], XtNx, xPos); n++;
    XtSetArg(arg[n], XtNy, yPos); n++;
    linkPannel = XtCreatePopupShell ("links", transientShellWidgetClass, 
                                      button, arg, (unsigned int) n);

    border = XtCreateManagedWidget("border", boxWidgetClass, 
                                   linkPannel, NULL, ZERO);
    pannel = XtCreateManagedWidget("pannel", formWidgetClass, 
                                    border, NULL, ZERO);

    linkLabel = ui_xCreateLabelItem ("  links", pannel,
                                     7 * d3_fontWidth, NULL, NULL);
    linksOffButton = d3_xCreateToggleItem ("off", pannel, NULL, NULL, linkLabel);
    linksOnButton = d3_xCreateToggleItem ("on", pannel, NULL, 
                                     linksOffButton, linkLabel);
    linksColorButton = d3_xCreateToggleItem ("color2", pannel, NULL, 
                                     NULL, linksOffButton);
    linksLabelButton = d3_xCreateToggleItem ("label", pannel, NULL, 
                                     NULL, linksColorButton);
    dummy = ui_xCreateLabelItem (" ", pannel,
                                     d3_fontWidth, NULL, linksLabelButton);

    fontLabel = ui_xCreateLabelItem ("  fonts", pannel,
                                     7 * d3_fontWidth, NULL, dummy);
    fontSmallButton = d3_xCreateToggleItem ("small", pannel, NULL, 
                                      NULL, fontLabel);
    fontBigButton = d3_xCreateToggleItem ("big", pannel, NULL, 
                                     fontSmallButton, fontLabel);

    doneButton = d3_xCreateButtonItem ("done", border, NULL, NULL);


    XtAddCallback (linksOffButton, XtNcallback, (XtCallbackProc) setLinksOff, NULL);
    XtAddCallback (linksOnButton, XtNcallback, (XtCallbackProc) setLinksOn, NULL);
    XtAddCallback (linksLabelButton, XtNcallback, (XtCallbackProc) setLinksLabel, NULL);
    XtAddCallback (linksColorButton, XtNcallback, (XtCallbackProc) setLinksColor, NULL);

    XtAddCallback (fontSmallButton, XtNcallback, (XtCallbackProc) setFontSmall, NULL);
    XtAddCallback (fontBigButton, XtNcallback, (XtCallbackProc) setFontBig, NULL);

    XtAddCallback (doneButton, XtNcallback, (XtCallbackProc) d3_closeLinkPannel, 
                   (Widget) linkPannel);

    memcpy (&temp_state, &d3_state, sizeof (d3_state_type)); 

    setLinkToggleState ();
    setFontToggleState ();

    ui_checkWindowPosition(linkPannel);
    XtPopup (linkPannel, XtGrabExclusive);

}



/* end of file */
/* lines: 359 */

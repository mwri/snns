/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_color.c,v $
  SHORTNAME      : color.c
  SNNS VERSION   : 4.2

  PURPOSE        : handling colors
  NOTES          :

  AUTHOR         : Tilman Sommer 
  DATE           : 18.2.1991

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/04/20 11:55:01 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

#include "ui.h"
#include "ui_textP.h"
#include "ui_mainP.h"

#include "ui_color.ph"


/*****************************************************************************
  FUNCTION : ui_col_setColor

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   : 18.2.1991
******************************************************************************/

static void ui_col_setColor (XColor *color, int value, int intensity, Bool print)

{
    switch (value) {
      case UI_COL_SEA: 
	color->blue  = 65535;
	color->red   = 65535;
	color->green = 0;
	if (print) ui_tw_printMessage("seablue\n");
	break;
      case UI_COL_BLUE: 
	color->blue  = 65535;
	color->red   = 0;
	color->green = 0;
	if (print) ui_tw_printMessage("blue\n");
	break;
      case UI_COL_YELLOW: 
	color->blue  = 0;
	color->red   = 65535;
	color->green = 65535;
	if (print) ui_tw_printMessage("yellow\n");
	break;
      case UI_COL_BLACK: 
	color->blue  = 0;
	color->red   = 0;
	color->green = 0;
	if (print) ui_tw_printMessage("black\n");
	break;
      case UI_COL_WHITE: 
	color->blue  = 65535;
	color->red   = 65535;
	color->green = 65535;
	if (print) ui_tw_printMessage("white\n");
	break;
      case UI_COL_RED: 
	color->blue  = 0;
	color->red   = 65535;
	color->green = 0;
	if (print) ui_tw_printMessage("red\n");
	break;
      case UI_COL_GREEN: 
	color->blue  = 0;
	color->red   = 0;
	color->green = 65535;
	if (print) ui_tw_printMessage("green\n");
	break;
    }

    switch (intensity) {
      case UI_COL_DARK: 
	color->blue  = color->blue DIV 2;
	color->red   = color->red DIV 2;
	color->green = color->green DIV 2;
	break;
      case UI_COL_BRIGHT: 
	  if (color->blue < 32768)
	      color->blue  += (65535 - color->blue ) DIV 3;
	  if (color->red  < 32768)
	      color->red   += (65535 - color->red  ) DIV 3;
	  if (color->green < 32768)
	      color->green += (65535 - color->green) DIV 3;
	break;
    }
}


/*****************************************************************************
  FUNCTION : ui_col_setRangeColors

  PURPOSE  :
  RETURNS  : void
  NOTES    :

  UPDATE   : 18.2.1991
******************************************************************************/

static void ui_col_setRangeColors (int min, int mid, int max)

{
    ui_tw_printMessage("positive values: ");
    ui_col_setColor(&ui_col_max , max , UI_COL_NORM, TRUE);
    ui_tw_printMessage("negative values: ");
    ui_col_setColor(&ui_col_min , min , UI_COL_NORM, TRUE);
    ui_tw_printMessage("zero     value : ");
    switch (mid) {
      case UI_COL_BLACK:
	ui_col_setColor(&ui_col_mid , mid , UI_COL_BRIGHT, TRUE);
	break;
      case UI_COL_BLUE:
	ui_col_setColor(&ui_col_mid , mid , UI_COL_DARK, TRUE);
	break;
      default:
	ui_col_setColor(&ui_col_mid , mid , UI_COL_NORM, TRUE);
    }
}


/*****************************************************************************
  FUNCTION : ui_col_colorSegment

  PURPOSE  : the 'distance' between the colors mid and end is divided into
             ui_col_steps segments. This functions calculates the color of
	     a given segment between start and end
  RETURNS  : void
  NOTES    :

  UPDATE   : 18.2.1991
******************************************************************************/
static void ui_col_colorSegment(XColor start, XColor end, 
				int segment, XColor *color)
{
    /* use a little bit linear algebra */

    color->red = 
	(segment ) * (end.red   - start.red  ) DIV ui_col_steps 
	    + start.red;
    color->green = 
	(segment  ) * (end.green - start.green) DIV ui_col_steps 
	    + start.green;
    color->blue = 
	(segment  ) * (end.blue  - start.blue ) DIV ui_col_steps 
	    + start.blue;
}



/*****************************************************************************
  FUNCTION : ui_convertColorString

  PURPOSE  : converts a string TTBBSS from ui_config.c into the display colors
  RETURNS  : void
  NOTES    : emulates the color palettes before this version

  UPDATE   : 25.5.1992
******************************************************************************/
void ui_convertColorString (char *colorString, int *textColor, 
			int *backgroundColor, int *selectionColor)
{
    int paletteNo, res, len;
    Bool inversePal = FALSE;
    static int emulation[3][3] = {{UI_BLACK, UI_WHITE, UI_YELLOW}, 
				  {UI_WHITE, UI_BLACK, UI_YELLOW},
				  {UI_BLACK, UI_CYAN, UI_YELLOW}};

    
    len = strlen(colorString);
    res = sscanf(colorString, "%d", &paletteNo);
    if ((!res) OR (paletteNo < 0) OR ((len > 1) AND (len < 6)) 
      OR ((len == 1) AND (paletteNo >= 3)) OR (len > 6)) {
        /* wrong string, take defaults */
        *textColor = ui_initalTextColorIndex;  
        *backgroundColor = ui_initalBackgroundColorIndex;
        *selectionColor = ui_initalSelectionColorIndex;   
    } else {
        if ((len == 1) AND (paletteNo < 3)) {
           *textColor = emulation[paletteNo][0];
           *backgroundColor = emulation[paletteNo][1];
           *selectionColor = emulation[paletteNo][2];
           inversePal = (paletteNo == 1);
        } else {
           sscanf (colorString, "%02d%02d%02d", textColor,
                   backgroundColor, selectionColor);
        }
    }
    if (ui_col_monochromeMode AND NOT inversePal) {
         *textColor = UI_BLACK;
         *backgroundColor = UI_WHITE;
    }   
}


/*****************************************************************************
  FUNCTION : ui_init_monochromePalette

  PURPOSE  : creates the monochrome Palette
  RETURNS  : kernel error code
  NOTES    : 

  UPDATE   : 23.10.1997
******************************************************************************/
static void ui_init_monochromePalette(void)
{
    XColor color;

    color.red = color.blue = color.green = 0;
    if (NOT (XAllocColor(ui_display, ui_colormap, &color))) {
	Cardinal  n=0;
	Arg       args[3];
	ui_colormap = XCopyColormapAndFree(ui_display, ui_colormap);
	XtSetArg(args[n], XtNcolormap, ui_colormap); n++;
	XtSetArg(args[n], XtNbackground, WhitePixel(ui_display,ui_screen)); n++;
	XtSetArg(args[n], XtNforeground, BlackPixel(ui_display,ui_screen)); n++;
	XtSetValues(ui_toplevel, args, n);
	XSetWindowColormap(ui_display,XtWindow(ui_toplevel),ui_colormap);
	if(NOT (XAllocColor(ui_display, ui_colormap, &color))){
	    fprintf(stderr,"Can't allocate color in private color map!\n");
	    exit(1);
	}
    }
    ui_editColor[UI_BLACK] = color.pixel;

    color.red = color.blue = color.green = 65535;
    if (NOT (XAllocColor(ui_display, ui_colormap, &color))) {
	Cardinal  n=0;
	Arg       args[3];
	ui_colormap = XCopyColormapAndFree(ui_display, ui_colormap);
	XtSetArg(args[n], XtNcolormap, ui_colormap); n++;
	XtSetArg(args[n], XtNbackground, WhitePixel(ui_display,ui_screen)); n++;
	XtSetArg(args[n], XtNforeground, BlackPixel(ui_display,ui_screen)); n++;
	XtSetValues(ui_toplevel, args, n);
	XSetWindowColormap(ui_display,XtWindow(ui_toplevel),ui_colormap);
	if(NOT (XAllocColor(ui_display, ui_colormap, &color))){
	    fprintf(stderr, "Can't allocate color in private color map!\n");
	    exit(1);
	}
    }
    ui_editColor[UI_WHITE] = color.pixel;

}


/*****************************************************************************
  FUNCTION : hsv_to_rgb

  PURPOSE  : 
  RETURNS  : void
  NOTES    : 

  UPDATE   :
******************************************************************************/
static void hsv_to_rgb (float h, float s, float v, int *r, int *g, int *b)
{
        int i;
        float f, p, q, t;

        if (s == 0.0)
        {
                *r = *g = *b = v * RGBMAX;
        }
        else
        {
                if (h >= 360.0)
                        h = 0.0;

                h /= 60.0;
                i = (int) h;
                f = h - (float) i;
                p = v * ( 1.0 - s );
                q = v * ( 1.0 - s * f );
                t = v * ( 1.0 - s * ( 1.0 - f ) );

                switch (i)
                {
                    case 0:
                        *r = RGBMAX *  v;
                        *g = RGBMAX *  t;
                        *b = RGBMAX *  p;
                        break;
                    case 1:
                        *r = RGBMAX *  q;
                        *g = RGBMAX *  v;
                        *b = RGBMAX *  p;
                        break;
                    case 2:
                        *r = RGBMAX *  p;
                        *g = RGBMAX *  v;
                        *b = RGBMAX *  t;
                        break;
                    case 3:
                        *r = RGBMAX *  p;
                        *g = RGBMAX *  q;
                        *b = RGBMAX *  v;
                        break;
                    case 4:
                        *r = RGBMAX *  t;
                        *g = RGBMAX *  p;
                        *b = RGBMAX *  v;
                        break;
                    case 5:
                        *r = RGBMAX *  v;
                        *g = RGBMAX *  p;
                        *b = RGBMAX *  q;
                        break;
                }
        }
}


/*****************************************************************************
  FUNCTION : setRGBcolor

  PURPOSE  : 
  RETURNS  : void
  NOTES    : 

  UPDATE   :
******************************************************************************/
static void setRGBcolor (int ind, float r, float g, float b)
{
    XColor color;

    color.red = (int) (RGBMAX * r);
    color.green = (int) (RGBMAX * g);
    color.blue = (int) (RGBMAX * b);
    if(NOT (XAllocColor(ui_display, ui_colormap, &color))){
	Cardinal  n=0;
	Arg       args[3];
	ui_colormap = XCopyColormapAndFree(ui_display, ui_colormap);
	XtSetArg(args[n], XtNcolormap, ui_colormap); n++;
	XtSetArg(args[n], XtNbackground, WhitePixel(ui_display,ui_screen)); n++;
	XtSetArg(args[n], XtNforeground, BlackPixel(ui_display,ui_screen)); n++;
	XtSetValues(ui_toplevel, args, n);
	XSetWindowColormap(ui_display,XtWindow(ui_toplevel),ui_colormap);
	if(NOT (XAllocColor(ui_display, ui_colormap, &color))){
	    fprintf(stderr, "Can't allocate color in private color map!\n");
	    exit(1);
	}
    }
    ui_editColor[ind] = color.pixel;
    ui_editColorRGB[ind][0] = ((float) color.red) / 65535.0; 
    ui_editColorRGB[ind][1] = ((float) color.green) / 65535.0; 
    ui_editColorRGB[ind][2] = ((float) color.blue) / 65535.0;
}


/*****************************************************************************
  FUNCTION : setHSVcolor

  PURPOSE  : 
  RETURNS  : void
  NOTES    : 

  UPDATE   :
******************************************************************************/
static void setHSVcolor (int ind, float h, float s, float v)
{
    int r, g, b;

    hsv_to_rgb(h, s, v, &r, &g, &b);
    setRGBcolor(ind, ((float)r)/RGBMAX, ((float)g)/RGBMAX, ((float)b)/RGBMAX);
}


/*****************************************************************************
  FUNCTION : setPureColors

  PURPOSE  : set the basic, pure colors
  RETURNS  : error_code
  NOTES    : 

  UPDATE   : 22.10.97
******************************************************************************/
static void setPureColors (int ind)
{
    setRGBcolor(ind+0, 0.0, 0.0, 0.0);
    setRGBcolor(ind+7, 1.0, 1.0, 1.0);
    setRGBcolor(ind+1, 1.0, 0.0, 0.0);
    setRGBcolor(ind+2, 0.0, 1.0, 0.0);
    setRGBcolor(ind+3, 0.0, 0.0, 1.0);
    setRGBcolor(ind+4, 1.0, 1.0, 0.0);
    setRGBcolor(ind+5, 1.0, 0.0, 1.0);
    setRGBcolor(ind+6, 0.0, 1.0, 1.0);
}


/*****************************************************************************
  FUNCTION : ui_init_colorPalette2

  PURPOSE  : 
  RETURNS  : void
  NOTES    :

  UPDATE   :
******************************************************************************/
static void ui_init_colorPalette2 (void)
{
    int i;
    float s, v, hStep, gStep, fhs, fgs;

    ui_colorEditPossible = TRUE;

    hStep = 360.0 / 8.0;
    gStep = 1.0 / 8.0;

    setPureColors(56);

    s = v = 0.5;
    for (i=0; i<8; i++){
	fhs = (float)i*hStep;
	fgs = (float)i*gStep;
        setHSVcolor(   i, fhs, 1.0, 0.4);
        setHSVcolor(8 +i, fhs, 1.0, 0.6);
        setHSVcolor(16+i, fhs, 1.0, 0.8);
        setHSVcolor(24+i, fhs, 1.0, 1.0);
        setHSVcolor(32+i, fhs, 0.5, 1.0);
        setHSVcolor(40+i, fhs, 0.25, 1.0);
        setRGBcolor(48+i, fgs, fgs, fgs);
    }
}


/*****************************************************************************
  FUNCTION : ui_initEditCols

  PURPOSE  : create the Palette
  RETURNS  : void
  NOTES    : the monochrome and the color palette must have the same index
             for black and white (the index is defined in ui.h)

  UPDATE   : 25.5.1992
******************************************************************************/
static void ui_initEditCols (void)
{
    if (ui_col_monochromeMode)
        ui_init_monochromePalette();
    else
        ui_init_colorPalette2();
}


/*****************************************************************************
  FUNCTION : ui_colVar_init

  PURPOSE  : init the ui_col_monochromeMode Variable;
  RETURNS  : void
  NOTES    : called from ui_main()

  UPDATE   : 20.5.1992
******************************************************************************/
void ui_colVar_init (void)
{
    Display *dpy;
    char vc[30], buf[255];

    dpy  = XtDisplay(ui_toplevel);
    ui_col_visual = XDefaultVisual(dpy, DefaultScreen(dpy));
    ui_colormap = XDefaultColormap(dpy, DefaultScreen(dpy));


#ifdef DEBUG_COLOR
    switch (ui_col_visual->class) {
      case StaticGray: 
	sprintf(vc,"StaticGray");
	break;
      case GrayScale: 
	sprintf(vc,"GrayScale");
	break;
      case StaticColor: 
	sprintf(vc,"StaticColor");
	break;
      case PseudoColor: 
	sprintf(vc,"PseudoColor");
	break;
      case TrueColor: 
	sprintf(vc,"TrueColor");
	break;
      case DirectColor: 
	sprintf(vc,"DirectColor");
	break;
    }
    sprintf(buf,"\nVisual Class: %s    bits per rgb: %d    map entries: %d\n",
	    vc, ui_col_visual->bits_per_rgb,
	    ui_col_visual->map_entries); 
    ui_tw_printMessage(buf);
#endif

    if ((ui_col_visual->class == GrayScale) OR 
	(ui_col_visual->class == StaticGray))
      ui_col_colorDisplay = FALSE;
    else
      ui_col_colorDisplay = TRUE;

    if(ui_monoOnColorScreen)
        ui_col_colorDisplay = FALSE;
    else
        ui_col_monochromeMode = !ui_col_colorDisplay;
}


/*****************************************************************************
  FUNCTION : ui_col_init

  PURPOSE  : init color module
  RETURNS  : void
  NOTES    : called from ui_main()

  UPDATE   : 18.2.1991
******************************************************************************/
void ui_col_init (void)
{

    int  i;

    /* initialize  palette */
    ui_initEditCols();

    if (! ui_col_colorDisplay) {
	ui_tw_printMessage("XGUI running in monochrome mode!\n");
	ui_col_monochromeMode = TRUE;
    } else {
	ui_tw_printMessage("Color/Grayscale screen detected!\n");
	ui_tw_printMessage("XGUI running in color mode!\n");
	ui_col_monochromeMode = FALSE;

	/* initialize the array of all colors used to draw units/links */

	ui_col_setRangeColors(UI_COL_RED, UI_COL_BLUE, UI_COL_GREEN);

	for (i=0; i <= 2 * ui_col_steps; i++) {
	    XColor color;
	    
	    if (i<ui_col_steps) /* range: [min,0] */
		ui_col_colorSegment(ui_col_mid, ui_col_min, 
				    ui_col_steps - i, &color);
	    else /* range: [0,max] */ {
		if (i == ui_col_steps)
		    color = ui_col_mid;
		else
		    ui_col_colorSegment(ui_col_mid, ui_col_max,
					i - ui_col_steps, &color);
	    }
	    if (NOT (XAllocColor(ui_display, ui_colormap, &color))){
		Cardinal  n=0;
		Arg       args[3];
		ui_colormap = XCopyColormapAndFree(ui_display, ui_colormap);
		XtSetArg(args[n], XtNcolormap, ui_colormap); n++;
		XtSetArg(args[n], XtNbackground, 
			 WhitePixel(ui_display,ui_screen)); n++;
		XtSetArg(args[n], 
			 XtNforeground, BlackPixel(ui_display,ui_screen)); n++;
		XtSetValues(ui_toplevel, args, n);
		XSetWindowColormap(ui_display,XtWindow(ui_toplevel),
				   ui_colormap);
		if(NOT (XAllocColor(ui_display, ui_colormap, &color))){
		    fprintf(stderr,
			    "Can't allocate color in private color map!\n");
		    exit(1);
		}
	    }

	    ui_col_rangePixels[i] = color.pixel;
#ifdef DEBUG_COLOR
	    printf("%2d  %5d  %5d  %5d\n",i,color.red,color.green,color.blue);
#endif
	}
    } 
}
/* end of file */

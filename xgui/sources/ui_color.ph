/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_color.ph,v $
  SHORTNAME      : color.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer, Universitaet Stuttgart
  DATE           : 18.2.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/04/20 11:55:06 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _UI_COLOR_DEFINED_
#define _UI_COLOR_DEFINED_


/* begin global definition section */
#define RGBMAX 65535
#define SNNS_COLORS 96


void ui_convertColorString (char *, int *, int *, int *);
void ui_colVar_init (void);
void ui_col_init (void);

unsigned long   ui_col_rangePixels[2 * UI_MAXCOLORSTEPS + 1];
	/* UI_MAXCOLORSTEPS segments for positive values,
   	UI_MAXCOLORSTEPS segments for negative values, 1 segment  for zero */

	/* array to hold all colors for selection, text and background */
unsigned long ui_editColor[UI_MAX_EDIT_COLS];
float ui_editColorRGB[UI_MAX_EDIT_COLS][3];
Colormap ui_colormap;

	/* default values */
int ui_initalBackgroundColorIndex = UI_WHITE;  /* index to ui_editColor */
int ui_initalSelectionColorIndex  = UI_YELLOW;  /* index to ui_editColor */
int ui_initalTextColorIndex       = UI_BLACK;   /* index to ui_editColor */

Bool ui_monoOnColorScreen = FALSE;
Bool ui_col_monochromeMode = TRUE;
Bool ui_col_colorDisplay;

int     ui_col_steps = UI_MAXCOLORSTEPS;
/* end global definition section */


/* begin private definition section */
static void hsv_to_rgb (float, float, float, int *, int *, int *);
static void setRGBcolor (int, float, float, float);
static void setHSVcolor (int, float, float, float);
static void setPureColors (int);
static void ui_col_colorSegment (XColor, XColor, int, XColor *);
static void ui_init_monochromePalette (void);
static void ui_col_setColor (XColor *, int, int, Bool);
static void ui_col_setRangeColors (int, int, int);
static void ui_init_colorPalette2 (void);
static void ui_initEditCols (void);

static XColor  ui_col_max,ui_col_min,ui_col_mid;
static Visual  *ui_col_visual;
static Bool ui_colorEditPossible;


/* end private definition section */


#endif /* _UI_COLOR_DEFINED_ */


/* end of file */
/* lines: 22 */

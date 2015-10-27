/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_color.h,v $
  SHORTNAME      : color.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer, Universitaet Stuttgart
  DATE           : 18.2.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/04/20 11:55:04 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _UI_COLOR_DEFINED_
#define _UI_COLOR_DEFINED_


#define RGBMAX 65535
#define SNNS_COLORS 96


extern void ui_convertColorString (char *, int *, int *, int *);
extern void ui_colVar_init (void);
extern void ui_col_init (void);

extern unsigned long   ui_col_rangePixels[];

	/* array to hold all colors for selection, text and background */
extern unsigned long ui_editColor[];
extern float ui_editColorRGB[][3];
extern Colormap ui_colormap;


	/* default values */
extern int ui_initalBackgroundColorIndex;  /* index to ui_editColor */
extern int ui_initalSelectionColorIndex;  /* index to ui_editColor */
extern int ui_initalTextColorIndex;   /* index to ui_editColor */

extern Bool ui_monoOnColorScreen;
extern Bool ui_col_monochromeMode;
extern Bool ui_col_colorDisplay;

extern int  ui_col_steps;

#endif /* _UI_COLOR_DEFINED_ */
/* end of file */

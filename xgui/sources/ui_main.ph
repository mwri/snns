/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_main.ph,v $
  SHORTNAME      : main.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.4.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/02/27 17:50:49 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_MAIN_DEFINED_
#define _UI_MAIN_DEFINED_


/* begin global definition section */


void main (int, char **);


#ifndef MAXPATHLEN
#define MAXPATHLEN  512
#endif

	/* path name (see file panel). Initial value by getwd() */
char         ui_pathname[MAXPATHLEN] = "\0";


XtAppContext ui_appContext;
int ui_labelFontWidth;

	/* label widgets of status info */
Widget       ui_stat_posWidget;
Widget       ui_stat_selNoWidget;
Widget       ui_stat_flagsWidget;


/* end global definition section */


/* begin private definition section */


static void ui_init (void);
static void ui_xCreatePanel (void);
static void ui_showBanner (Widget);
static void ui_clickEventProc (Widget, Display *, XEvent *);


static String fallback_resources[] = {
    "*background: white",
    "*borderColor: black",
    "*font: 7x13bold",
    "*foreground: black",
    "*help.label: HELP",
    "*display.label: DISPLAY",
    "*view3d.label: VIEW3d",
    "*text.label: TEXT",
    "*control.label: CONTROL",
    "*file.label: FILE",
    "*quit.label: QUIT",
    "*sPanel*trigger.orientation:   horizontal",
    "*allowShellResize: True",
    "*dummyMsg.label: Still not implemented!",
    "*manMsg.label: SNNS 4.2 (c) 1990-98 SNNS-Group at IPVR and WSI",
    NULL,
};
static Widget       ui_infoBox; /* frame widget (form) of info item */


/* end private definition section */


#endif /* _UI_MAIN_DEFINED_ */


/* end of file */
/* lines: 41 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_print.h,v $
  SHORTNAME      : print.h
  SNNS VERSION   : 4.2

  PURPOSE        : header for ui_print.c
  NOTES          : all functions will be exported

  AUTHOR         : Ralf Huebner
  DATE           : 11.5.1992

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:30 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_PRINT_DEFINED_
#define _UI_PRINT_DEFINED_




#ifndef ZERO
#define ZERO 0
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

#define UI_STR_LEN MAX_NAME_LENGTH

#define HEADER_COMPILE

	/* number of radio buttons in a group */
#define noOfFmtToggles     2
#define noOfPaperToggles   3
#define noOfDestToggles    2
#define noOfAsToggles      2
#define noOfClipToggles    2
#define noOfColorToggles   2
#define noOfOrientToggles  2
#define noOfAuToggles      2
#define noOfBorderToggles  2
#define noOfSizeToggles    2
#define noOfShapeToggles   2
#define noOfTextToggles    2


extern void ui_defaultPrinterValues (void);
extern void ui_printNet (void);

extern Ui_printerValType ui_prVal;  /* the structure that holds the values */
extern char *ui_tmpDir;     /* the directory for the temporary printer files */
extern char *ui_praefix;    /* the prefix for the temporary printer files */
extern FILE *psOutfile;             /* file id of the postscript output file */


#endif /* _UI_PRINT_DEFINED_ */


/* end of file */
/* lines: 64 */


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_confirmer.ph,v $
  SHORTNAME      : confirm.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 6.7.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:22 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_CONFIRMER_DEFINED_
#define _UI_CONFIRMER_DEFINED_


/* begin global definition section */

void  ui_confirmOk (char *);
int   ui_confirmYes (char *);

/* end global definition section */


/* begin private definition section */


static void  ui_confirm (char *, int);
static void  ui_cf_yesNoOk (Widget, int);


static int     ui_cf_answer;      /* which button was pressed to exit ? */
static Widget  ui_cf_popup;       /* shell widget containing the confirmer */
static Bool    ui_cf_exit = TRUE; /* to control the confirmer loop */


/* end private definition section */


#endif /* _UI_CONFIRMER_DEFINED_ */



/* end of file */
/* lines: 32 */

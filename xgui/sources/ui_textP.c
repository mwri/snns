/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_textP.c,v $
  SHORTNAME      : textP.c
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 1.2.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:10:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <string.h>

#include "ui.h"
#include "ui_confirmer.h"

#include "ui_fileP.h"
#include "ui_textP.ph"


/*****************************************************************************
  FUNCTION : ui_tw_errorMessage()

  PURPOSE  : prints error messages into a confirmer
  RETURNS  : void
  NOTES    :

  UPDATE   : 10.12.1991
******************************************************************************/

void ui_tw_errorMessage(char *message)

{
    char error[512];
    char *cptr, *temp;
    int max_len = 40;
    int best_len, str_len, i;

    strcpy (error, message);
    str_len = strlen(error);
    if (str_len == 0)
        return;
    best_len = str_len DIV (((str_len - 1) DIV max_len) + 1);
    cptr = error;
    i = 0;
    while (*cptr != '\0') {
       cptr++;
       i++;
       if (i == best_len) {
           temp = cptr;
           while (*temp != ' ')
               temp--;
           *temp = '\n';
           i = (int) (cptr - temp);
       }
    }
    ui_confirmOk (error);
    if (ui_textFilePtr != NULL) {
	fprintf(ui_textFilePtr, "%s\n", message);
	fflush(ui_textFilePtr);
    }
}


/*****************************************************************************
  FUNCTION : ui_tw_printMessage()

  PURPOSE  :
  RETURNS  :
  NOTES    :

  UPDATE   : 1.2.1990
******************************************************************************/

void ui_tw_printMessage(char *message)

{
    printf(message);
    if (ui_textFilePtr != NULL) {
	fprintf(ui_textFilePtr, message);
	fflush(ui_textFilePtr);
    }
}









/* end of file */
/* lines: 103 */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/error.ph,v $
  SHORTNAME      : error
  SNNS VERSION   : 4.2

  PURPOSE        : Functions to print out error messages and warnings
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.5 $
  LAST CHANGE    : $Date: 1998/05/01 11:21:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _ERROR_DEFINED_
#define _ERROR_DEFINED_

/* begin global definition section */

/* some often-used strings: */
#define WRN_FMATE "Found more arguments than expected; ignored"
#define WRN_NFAE  "No function arguments expected; ignored"

#define ERR_ALOAE "At least one argument expected"
#define ERR_NOAMM "Number of arguments must match an integer multiple of 4"
#define ERR_ATOFE "Argument TRUE or FALSE expected"
#define ERR_MPF   "Missing pattern filename"
#define ERR_MEM   "Memory allocation failed"


void inc_sourcelines(void);
int get_sourceline(void);

void chck_type(Data_type expected, Data_type two);

void err_prt(char *text);
void kernel_error(krui_err err);
void warn_prt(char *text);
void yyzerror(char *text);
void yyyerror(char *text);
void signal_init(void);
void signal_suspend(void);
void signal_resume(void);

/* end global definition section */


/* begin private definition section */

#define MAX_WARNINGS 10           /* max. number of warnings to be displayed */

static int sourceline_no = 1;     /* scanner position in source file */

/* end private definition section */

#endif


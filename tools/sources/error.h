/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/error.h,v $
  SHORTNAME      : error
  SNNS VERSION   : 4.2

  PURPOSE        : Functions to print out error messages and warnings
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.5 $
  LAST CHANGE    : $Date: 1998/05/01 11:21:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


/* some often-used error strings: */
#define WRN_FMATE "Found more arguments than expected; ignored"
#define WRN_NFAE  "No function arguments expected; ignored"

#define ERR_ALOAE "At least one argument expected"
#define ERR_NOAMM "Number of arguments must match an integer multiple of 4"
#define ERR_ATOFE "Argument TRUE or FALSE expected"
#define ERR_MPF   "Missing pattern filename"
#define ERR_MEM   "Memory allocation failed"


extern void inc_sourcelines(void);
extern int get_sourceline(void);

extern void chck_type(Data_type expected, Data_type two);

extern void err_prt(char *text);
extern void kernel_error(krui_err err);
extern void warn_prt(char *text);
extern void yyzerror(char *text);
extern void yyyerror(char *text);
extern void signal_init(void);
extern void signal_suspend(void);
extern void signal_resume(void);


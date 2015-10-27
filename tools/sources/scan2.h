/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/scan2.h,v $
  SHORTNAME      : scan2
  SNNS VERSION   : 4.2

  PURPOSE        : Scanner 2 for SNNS batch interpreter 
                   scans the output of a shell command

		   For translation with flex lexical analyzer generator

  NOTES          : Use flex option '-P yyy' to change symbol name prefixes
                   in order to avoid name conflicts with the other scanners.

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  SCCS VERSION   : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:47 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


/*****************************************************************************
  NOTE: This file is added for consistency with the "Ansi-Richtlinien fuer
        SNNS" since flex does not support header files.

******************************************************************************/


extern FILE *yyyin;               /* scanner input stream */
extern Data_type yyylex(void);    /* scanning function */
extern Val_type yyylval;          /* token value structure */


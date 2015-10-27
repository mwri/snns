/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/scan1.h,v $
  SHORTNAME      : scan1
  SNNS VERSION   : 4.2

  PURPOSE        : Scanner 1 for SNNS batch interpreter 

                   For translation with flex lexical analyzer generator

  NOTES          : Use flex option '-P yyz' to change symbol name prefixes.
                   See grammar file gram1.y for the reason why.

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  SCCS VERSION   : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


/*****************************************************************************
  NOTE: This file is added for consistency with the "Ansi-Richtlinien fuer
        SNNS" since flex does not support header files.

******************************************************************************/

extern int yyzparse (void);           /* parser function */
extern FILE *yyzin;                   /* scanner input stream */
extern int yyzdebug;                  /* parser debug mode; 0 means off */



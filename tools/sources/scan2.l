/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/scan2.l,v $
  SHORTNAME      : scan2
  SNNS VERSION   : 4.1

  PURPOSE        : Second Scanner for SNNS batch interpreter 
                   scans the output of a shell command

		   For translation with flex lexical analyzer generator

  NOTES          : Use flex option '-P yyy' to change symbol name prefixes
                   in order to avoid name conflicts with the other scanners.

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  IDENTIFICATION : $State: Exp $ $Locker:  $
  SCCS VERSION   : $Revision: 1.5 $
  LAST CHANGE    : $Date: 1998/03/03 16:00:04 $

             Copyright (c) 1995 SNNS Group, IPVR, Univ. Stuttgart, FRG

******************************************************************************/

%{  /* C definitions */
#include <config.h>

#include <math.h>
#include <string.h> 

#include "symtab.h"
#include "glob_typ.h"     /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"

#define yyywrap() 1       /* terminate scanner at EOF of input */

/* change the type of the scanning function: */
#undef YY_DECL
#define YY_DECL Data_type yyylex(void) 

Val_type yyylval;         /* token type (usually declared by parser) */

%}

DIGIT  [0-9]
REAL   [+-]?[0-9]*"."?[0-9]+([eE][+-]?[0-9]+)?
STRING [^ \t\n]+

%%  /* rules */

[+-]?{DIGIT}+    { yyylval.int_val = atoi(yytext);
	      return INT; }

{REAL}      { yyylval.real_val = atof(yytext);
	      return REAL; }

TRUE        { yyylval.bool_val = TRUE; 
       	      return BOOL; }

FALSE       { yyylval.bool_val = FALSE; 
       	      return BOOL; }

{STRING}    { yyylval.string_val = strdup(yytext); 
              return STRING; }

[ \t\n]+        /* eat up whitespace */



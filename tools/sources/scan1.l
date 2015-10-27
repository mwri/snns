/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/scan1.l,v $
  SHORTNAME      : scan1
  SNNS VERSION   : 4.1

  PURPOSE        : Scanner 1 for SNNS batch interpreter 

                   For translation with flex lexical analyzer generator

  NOTES          : Use flex option '-P yyz' to change symbol name prefixes.
                   See grammar file gram1.y for the reason why.

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  IDENTIFICATION : $State: Exp $ $Locker:  $
  SCCS VERSION   : $Revision: 1.2 $
  LAST CHANGE    : $Date: 1998/03/03 16:00:03 $

             Copyright (c) 1995 SNNS Group, IPVR, Univ. Stuttgart, FRG

******************************************************************************/

%{  /* C definitions */
#include <config.h>

#include <stdlib.h>
#include <math.h>
#include <string.h> 

#include "symtab.h"
#include "ictab.h"        /* required by the following include file */
#include "backpatch.h"    /* required by the following include file */
#include "gram1.tab.h"    /* declarations of grammar file */
#include "glob_typ.h"     /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"
#include "batchman.h"     /* for input_is_file */

#define yyzwrap() 1       /* terminate scanner at EOF of input */

%}

DIGIT  [0-9]+
REAL   [0-9]*"."?[0-9]+([eE][+-]?[0-9]+)?
IDENT  [_a-zA-Z][_a-zA-Z0-9]*
STRING [^"\n\t]*

%x string string_is_ok

%%  /* rules */

#.*$        /* forget anything written between # and end of line */

 /* Constants (strings, reals and integers) are inserted in symbol table
    to avoid type checking in grammar. Type checking and conversion is 
    done by the intermediate code functions (add, :=, +, ...). */

{DIGIT}     { /* insert integers into symbol table: */
              Val_type tmp;
	      tmp.int_val = atoi(yytext);
	      yyzlval.t.stp = st_insert(yytext); 
	      st_set_val_type(yyzlval.t.stp, INT, tmp); 
	      st_set_ro(yyzlval.t.stp);
	      return Identifier;
	    }

{REAL}      { /* insert reals into symbol table: */
              Val_type tmp;
	      tmp.real_val = atof(yytext);
	      yyzlval.t.stp = st_insert(yytext); 
	      st_set_val_type(yyzlval.t.stp, REAL, tmp); 
	      st_set_ro(yyzlval.t.stp);
	      return Identifier;
	    }

TRUE        { /* lookup built-in variable TRUE in symbol table: */
       	      yyzlval.t.stp = st_lookup("TRUE"); 
       	      return Identifier;
       	    }

FALSE       { /* lookup built-in variable FALSE in symbol table: */
       	      yyzlval.t.stp = st_lookup("FALSE"); 
       	      return Identifier;
       	    }


for         return FOR; 
to          return TO;
do          return DO;
endfor      return ENDFOR;
if          return IF;
then        return THEN;
else        return ELSE;
endif       return ENDIF;
while       return WHILE;
endwhile    return ENDWHILE;
repeat      return REPEAT;
until       return UNTIL;
break       return BREAK;
continue    return CONTINUE;

not|!       return NOT;
sqrt        return SQRT;
ln          return LN;
log         return LOG;
"**"|"^"    return EXP;
div         return DIV;
mod|"%"     return MOD;
and|"&&"    return AND;
or|"||"     return OR;
:=|=        return Assignment;
==          return EQOP;
>=|=>       return GEQOP;
"<="|=<     return LEQOP;
"<>"|!=     return NEQOP;


 /* insert valid strings into symbol table: */
\"          BEGIN(string);

<string>{STRING}     {
              Val_type tmp;
	      BEGIN(string_is_ok);
	      tmp.string_val = strdup(yytext);
	      if (tmp.string_val == NULL) err_prt(ERR_MEM);
	      yyzlval.t.stp = st_insert(yytext);
	      st_set_val_type(yyzlval.t.stp, STRING, tmp); 
	      st_set_ro(yyzlval.t.stp);
	      return Identifier;
	    } 

<string>\"  yyzerror("empty string");

<string_is_ok>\"     BEGIN(INITIAL);

<string,string_is_ok>.  {
              static char s[50] = "invalid character in string: ";
	      strcat (s, yytext);
	      yyzerror(s);
	    }


{IDENT}     { /* insert Identifier in symbol table, if not already there, 
                 and return its position: */
              yyzlval.t.stp = st_insert(yytext); 
              return Identifier; 
	    }


 /* return valid single character tokens unchanged: */
,           return (int)*yytext; 
"("         return (int)*yytext; 
")"         return (int)*yytext; 
"+"         return (int)*yytext; 
"-"         return (int)*yytext; 
"*"         return (int)*yytext; 
"/"         return (int)*yytext; 
"<"         return (int)*yytext; 
">"         return (int)*yytext; 

;           return Delimiter;    /* command delimiter */
\n          { inc_sourcelines(); /* count linenumbers */ 
              if (! input_is_file) printf("batchman> ");
              return Delimiter;  /* \n is a command delimiter as well as ';' */
            }

[\t ]+      /* eat up whitespace */

.           { static char s[50] = "don't know what to do with the ";
	      strcat (s, yytext);
	      yyzerror(s);
	    }

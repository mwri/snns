/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/gram1.y,v $
  SHORTNAME      : gram1
  SNNS VERSION   : 4.2

  PURPOSE        : Grammar rules and semantic actions (intermediate code 
                   generation) for SNNS batch interpreter

                   For translation with bison parser generator 

  NOTES          : Use bison option '-p yyz' to change symbol name prefixes.
                   This is to avoid name conflicts with the SNNS-kernel 
		   pattern parse functions.
		   This grammar expects the error reporting routine to be 
		   called 'yyzerror' and the scanner function to be called
		   'yyzlex'. 

		   In actions, C comments are BELOW the corresponding 
		   statement!

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  IDENTIFICATION : $State: Exp $ $Locker:  $
  RCS VERSION    : $Revision: 1.7 $  
  LAST CHANGE    : $Date: 1998/03/02 17:58:00 $

    Copyright (c) 1995 SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

/* Prefix 'yyz' taken from Rush's 'Moving Pictures' album (1981).
   Used without permission.
*/


%{  /* C declarations */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>        /* for malloc & free in bison.simple */
#include <math.h>
#include <string.h>        /* for strcat in bison.simple */

#include "symtab.h"        /* for symbol table functions */
#include "ictab.h"         /* for intermediate code (icode_...) functions */
#include "icopjmp.h"       /* for operators and jump function pointers */
#include "backpatch.h"     /* for backpatch operations */
#include "arglist.h"       /* for argument list handling */
#include "glob_typ.h"      /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"         /* for yyzerror() */


#define YYERROR_VERBOSE 1  /* verbose error reporting from parser */

extern int yyzlex(void);   /* see 1st comment in this file */

%}

/* bison declarations */

%union { struct {        /* bison doesn't know struct as a token type */
  St_ptr_type stp;       /* symbol table pointer */
                         /* 2 backpatch target lists: */
  bp_list *brk;          /* #1 contains positions of break statements */
  bp_list *cont;         /* #2 contains positions of continue statements */
  Ic_ptr_type tmp;       /* instruction # temp buffer for local backpatching */
  arglist_type *arglist; /* pointer to an argument list */
} t; }                   /* prefer a short identifier here (see above) */ 
 
/* specify multiple character token's names and their type: */
%token <t> FOR TO DO ENDFOR IF THEN ELSE ENDIF WHILE ENDWHILE
%token <t> REPEAT UNTIL BREAK CONTINUE 
%token <t> Delimiter Identifier Assignment

/* specify operator precedence (top to bottom corresponds low to high p.): */
%left OR AND
%left NEQOP EQOP
%left GEQOP '>' LEQOP '<'
%left '-' '+'
%left MOD DIV '/' '*'
%left EXP
%right SQRT LN LOG
%right NOT
%right SIGN

%type <t> simpleExpr expr paramlist fullList stmtList statement ifexpr

%%  /* grammar and semantic actions */

/* WARNING: Do not change a single bit in this area
   unless you are shure that you know what you do!
*/

batchprog:
   stmtList             { bp_backpatch($1.brk, get_ic_pos()+1);
                          /* breaks in the outermost block cause exit */ }
;

stmtList:
 statement              { $$.cont = $<t>1.cont; 
			  $$.brk = $<t>1.brk; }

 | stmtList Delimiter
     statement          { $$.cont = bp_merge($<t>1.cont, $<t>3.cont); 
			  $$.brk = bp_merge($<t>1.brk, $<t>3.brk);
			  /* concatenate break and continue lists */ }
;

statement:
  /* e */               { $$.cont = BP_NULL; 
			  $$.brk = BP_NULL;
			  /* there were no cont or break statements */ }

 | Identifier '(' paramlist ')' 
                        { Val_type dmy;
			  Data_type data;
			  st_get_val_type($<t>1.stp, &data, &dmy);
			  /* check wether it is really a known function */
			  if (data != FCT)
			    yyzerror("Function name invalid"); 
			  icode_jacket($<t>1.stp, $<t>3.arglist);
			  /* call jacket fct. with fct. name and pointer
			     to it's argument pointer list */
			  new_arglist();
			  /* prepare list for next fct.-call statement */
			  $$.cont = BP_NULL; 
			  $$.brk = BP_NULL;
			  /* there were no cont or break statements */ }

 | Identifier Assignment expr        
                        { icode_op(assign, $1.stp, $3.stp, 0);
			  $$.cont = BP_NULL; 
			  $$.brk = BP_NULL;
			  /* there were no cont or break statements */ } 

 | FOR Identifier Assignment expr    
                        { icode_op(assign, $<t>2.stp, $<t>4.stp, 0);
			  /* assign result of expr to Identifier */ }
     TO expr            { $$.stp = st_insert(newtmp());
			  icode_op(less_eq, $<t>$.stp, $<t>2.stp, $<t>7.stp);
			  /* instruction to evaluate loop condition */
			  $$.tmp = get_ic_pos();
			  /* store position of less_eq */
			  icode_jmp(jmp_false, 0, $<t>$.stp); 
			  /* exit FOR loop if condition is false */
			  $<t>$.brk = bp_makelist(get_ic_pos());
			  /* store position of jmp_false */ } 
     DO stmtList ENDFOR { icode_op(add,$<t>2.stp,$<t>2.stp, st_lookup("%ONE"));
			  /* increment Identifier (loop counter) by one */
			  icode_jmp(jmp, $<t>8.tmp, 0);
			  /* jump back to condition evaluation */
			  $<t>$.brk = bp_merge($<t>8.brk, $<t>10.brk);
			  /* merge jumps that exit the FOR loop... */
			  bp_backpatch($<t>$.brk, get_ic_pos()+1);
                          /* and bp them. */
			  bp_backpatch($<t>10.cont, get_ic_pos()-1);
			  /* bp possible cont-stmts from stmtList:
			     let them jump to ENDFOR (increment) */
                          $$.cont = BP_NULL; 
			  $$.brk = BP_NULL;
			  /* there were no unresolved cont or break stmts */ }

 | WHILE                { $<t>$.tmp = get_ic_pos()+1; } 
                          /* store position of following expr */
     expr               { icode_jmp(jmp_false, 0, $<t>3.stp); 
			  /* exit WHILE loop if expr is false */
                          $<t>$.brk = bp_makelist(get_ic_pos());
			  /* store position of jmp_false */ } 
     DO stmtList ENDWHILE
                        { icode_jmp(jmp, $<t>2.tmp, 0);
			  /* jump back to expr */
			  $<t>$.brk = bp_merge($<t>4.brk, $<t>6.brk);
			  /* merge jumps that exit the WHILE loop... */
			  bp_backpatch($<t>$.brk, get_ic_pos()+1);
                          /* and bp them. */
			  bp_backpatch($<t>6.cont, $<t>2.tmp);
			  /* bp possible cont-stmts from stmtList */
                          $$.cont = BP_NULL;
			  $$.brk = BP_NULL;
			  /* there were no unresolved cont or break stmts */ }

 | REPEAT               { $<t>$.tmp = get_ic_pos()+1;
                          /* store position of following stmtList */ }
     stmtList UNTIL     { $<t>$.tmp = get_ic_pos()+1;
                          /* store position of following expr */ }
     expr               { icode_jmp(jmp_false, $<t>2.tmp, $<t>6.stp);
			  /* jump back to begin of stmtList */
			  bp_backpatch($<t>3.brk, get_ic_pos()+1);
			  /* bp possible break-stmts from stmtList */
			  bp_backpatch($<t>3.cont, $<t>5.tmp);
			  /* bp possible cont-stmts from stmtList */
                          $$.cont = BP_NULL;
			  $$.brk = BP_NULL;
			  /* there were no unresolved cont or break stmts */ }

 | ifexpr THEN stmtList ELSE 
                        { icode_jmp(jmp_true, 0, $<t>1.stp); 
			  /* jump over the else block if expr is true */
                          $<t>$.brk = bp_makelist(get_ic_pos());
			  /* store position of jmp_true */ 
                          $<t>$.tmp = get_ic_pos()+1;
			  /* store position of else block */ }
     stmtList ENDIF     { bp_backpatch($<t>1.brk, $<t>5.tmp);
			  /* backpatch jump over the if block */
			  bp_backpatch($<t>5.brk, get_ic_pos()+1);
			  /* backpatch jump over the else block */
			  $$.cont = bp_merge($<t>3.cont, $<t>6.cont);
			  $$.brk = bp_merge($<t>3.brk, $<t>6.brk);
			  /* merge possible continue and break lists 
			     from both if and else blocks and return them */ }

 | ifexpr THEN stmtList ENDIF
                        { bp_backpatch($<t>1.brk, get_ic_pos()+1);
			  /* backpatch the jump after expr */
			  $$.cont = $<t>3.cont; 
			  /* pass possible continues... */
			  $$.brk = $<t>3.brk;
			  /* and breaks to the surrounding loop */ }

 | BREAK                { icode_jmp(jmp, 0, 0); 
			  $$.cont = BP_NULL;
                          $$.brk = bp_makelist(get_ic_pos());
                          /* store position of jump */ }

 | CONTINUE             { icode_jmp(jmp, 0, 0); 
                          $$.cont = bp_makelist(get_ic_pos()); 
                          /* store position of jump */
			  $$.brk = BP_NULL; }
;

ifexpr:  /* need this to avoid a fatal reduce/reduce conflict, since bison 
            sees even identical mid-rule actions as different ones */
 IF expr                { icode_jmp(jmp_false, 0, $<t>2.stp); 
			  /* jump over the if block if expr is false */
                          $<t>$.brk = bp_makelist(get_ic_pos());
			  /* store position of jmp_false */ 
			  $<t>$.tmp = $<t>2.stp;
			  $<t>$.stp = $<t>2.stp;
			  /* pass result of expr to the else alternative */ }
;

expr:
 simpleExpr               { $$.stp = $1.stp; } 

 | '-' expr  %prec SIGN   { $$.stp = st_insert(newtmp()); 
			    icode_op(sub,$$.stp,st_lookup("%ZERO"),$2.stp);
			    /* negate the value of expr by subtracting it 
			       from the zero built-in constant */ }

 | '+' expr  %prec SIGN   { $$.stp = $2.stp ;
			    /* if one wants to use a meaningless + sign */}

 | NOT expr               { $$.stp = st_insert(newtmp()); 
			    icode_op(not,$$.stp,$2.stp,0); }

 | SQRT expr              { $$.stp = st_insert(newtmp()); 
			    icode_op(sq_rt,$$.stp,$2.stp,0); }

 | LN expr                { $$.stp = st_insert(newtmp()); 
			    icode_op(ln,$$.stp,$2.stp,0); }

 | LOG expr               { $$.stp = st_insert(newtmp()); 
			    icode_op(lg,$$.stp,$2.stp,0); }

 /* proceeding of the following productions is always similar:
    1. get a new temporary variable
    2. insert the appropriate instruction in the IC list
       store it's result in the new temporary variable */

 | expr '+' expr          { $$.stp = st_insert(newtmp()); 
			    icode_op(add,$$.stp,$1.stp,$3.stp); }

 | expr '-' expr          { $$.stp = st_insert(newtmp()); 
			    icode_op(sub,$$.stp,$1.stp,$3.stp); }

 | expr '*' expr          { $$.stp = st_insert(newtmp()); 
			    icode_op(mult,$$.stp,$1.stp,$3.stp); }

 | expr '/' expr          { $$.stp = st_insert(newtmp()); 
			    icode_op(dvde,$$.stp,$1.stp,$3.stp); }

 | expr DIV expr          { $$.stp = st_insert(newtmp()); 
			    icode_op(intdiv,$$.stp,$1.stp,$3.stp); }

 | expr MOD expr          { $$.stp = st_insert(newtmp()); 
			    icode_op(mod,$$.stp,$1.stp,$3.stp); }

 | expr EXP expr          { $$.stp = st_insert(newtmp()); 
			    icode_op(bmraise,$$.stp,$1.stp,$3.stp); }

 | expr '<' expr          { $$.stp = st_insert(newtmp());
			    icode_op(less,$$.stp,$1.stp,$3.stp); }

 | expr '>' expr          { $$.stp = st_insert(newtmp());
			    icode_op(greater,$$.stp,$1.stp,$3.stp); }

 | expr EQOP expr         { $$.stp = st_insert(newtmp());
			    icode_op(eq,$$.stp,$1.stp,$3.stp); }

 | expr NEQOP expr        { $$.stp = st_insert(newtmp());
			    icode_op(not_eq,$$.stp,$1.stp,$3.stp); }

 | expr LEQOP expr        { $$.stp = st_insert(newtmp());
			    icode_op(less_eq,$$.stp,$1.stp,$3.stp); }

 | expr GEQOP expr        { $$.stp = st_insert(newtmp());
			    icode_op(great_eq,$$.stp,$1.stp,$3.stp); }

 | expr AND expr          { $$.stp = st_insert(newtmp());
			    icode_op(and,$$.stp,$1.stp,$3.stp); }

 | expr OR expr           { $$.stp = st_insert(newtmp());
			    icode_op(or,$$.stp,$1.stp,$3.stp); }
;

simpleExpr:
 Identifier               { $$.stp = $1.stp; } 
 | '(' expr ')'           { $$.stp = $2.stp; } 
;

paramlist:
 /* e */                  { $<t>$.arglist = ARG_NULL; }
 | fullList               { $<t>$.arglist = $<t>1.arglist; }
;

fullList:
 expr                     { $<t>$.arglist = add_to_arglist($<t>1.stp); }
 | fullList ',' expr      { $<t>$.arglist = add_to_arglist($<t>3.stp);
			    /* ST pointers to the arguments of a function
			       are stored in the argument pointer list */ }
;



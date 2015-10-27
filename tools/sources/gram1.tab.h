/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FOR = 258,
     TO = 259,
     DO = 260,
     ENDFOR = 261,
     IF = 262,
     THEN = 263,
     ELSE = 264,
     ENDIF = 265,
     WHILE = 266,
     ENDWHILE = 267,
     REPEAT = 268,
     UNTIL = 269,
     BREAK = 270,
     CONTINUE = 271,
     Delimiter = 272,
     Identifier = 273,
     Assignment = 274,
     AND = 275,
     OR = 276,
     EQOP = 277,
     NEQOP = 278,
     LEQOP = 279,
     GEQOP = 280,
     DIV = 281,
     MOD = 282,
     EXP = 283,
     LOG = 284,
     LN = 285,
     SQRT = 286,
     NOT = 287,
     SIGN = 288
   };
#endif
/* Tokens.  */
#define FOR 258
#define TO 259
#define DO 260
#define ENDFOR 261
#define IF 262
#define THEN 263
#define ELSE 264
#define ENDIF 265
#define WHILE 266
#define ENDWHILE 267
#define REPEAT 268
#define UNTIL 269
#define BREAK 270
#define CONTINUE 271
#define Delimiter 272
#define Identifier 273
#define Assignment 274
#define AND 275
#define OR 276
#define EQOP 277
#define NEQOP 278
#define LEQOP 279
#define GEQOP 280
#define DIV 281
#define MOD 282
#define EXP 283
#define LOG 284
#define LN 285
#define SQRT 286
#define NOT 287
#define SIGN 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 64 "gram1.y"
{ struct {        /* bison doesn't know struct as a token type */
  St_ptr_type stp;       /* symbol table pointer */
                         /* 2 backpatch target lists: */
  bp_list *brk;          /* #1 contains positions of break statements */
  bp_list *cont;         /* #2 contains positions of continue statements */
  Ic_ptr_type tmp;       /* instruction # temp buffer for local backpatching */
  arglist_type *arglist; /* pointer to an argument list */
} t; }
/* Line 1489 of yacc.c.  */
#line 124 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yyzlval;


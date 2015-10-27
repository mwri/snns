/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse yyzparse
#define yylex   yyzlex
#define yyerror yyzerror
#define yylval  yyzlval
#define yychar  yyzchar
#define yydebug yyzdebug
#define yynerrs yyznerrs


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




/* Copy the first part of user declarations.  */
#line 39 "gram1.y"
  /* C declarations */
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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 187 of yacc.c.  */
#line 202 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 215 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  27
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   156

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  50
/* YYNRULES -- Number of states.  */
#define YYNSTATES  100

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,    41,    31,    29,    42,    28,     2,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      25,     2,    24,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    26,
      27,    32,    33,    34,    35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,    11,    12,    17,    21,    22,
      23,    35,    36,    37,    45,    46,    47,    54,    55,    63,
      68,    70,    72,    75,    77,    80,    83,    86,    89,    92,
      95,    99,   103,   107,   111,   115,   119,   123,   127,   131,
     135,   139,   143,   147,   151,   155,   157,   161,   162,   164,
     166
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      44,     0,    -1,    45,    -1,    46,    -1,    45,    17,    46,
      -1,    -1,    18,    40,    57,    41,    -1,    18,    19,    55,
      -1,    -1,    -1,     3,    18,    19,    55,    47,     4,    55,
      48,     5,    45,     6,    -1,    -1,    -1,    11,    49,    55,
      50,     5,    45,    12,    -1,    -1,    -1,    13,    51,    45,
      14,    52,    55,    -1,    -1,    54,     8,    45,     9,    53,
      45,    10,    -1,    54,     8,    45,    10,    -1,    15,    -1,
      16,    -1,     7,    55,    -1,    56,    -1,    28,    55,    -1,
      29,    55,    -1,    38,    55,    -1,    37,    55,    -1,    36,
      55,    -1,    35,    55,    -1,    55,    29,    55,    -1,    55,
      28,    55,    -1,    55,    31,    55,    -1,    55,    30,    55,
      -1,    55,    32,    55,    -1,    55,    33,    55,    -1,    55,
      34,    55,    -1,    55,    25,    55,    -1,    55,    24,    55,
      -1,    55,    22,    55,    -1,    55,    23,    55,    -1,    55,
      26,    55,    -1,    55,    27,    55,    -1,    55,    20,    55,
      -1,    55,    21,    55,    -1,    18,    -1,    40,    55,    41,
      -1,    -1,    58,    -1,    55,    -1,    58,    42,    55,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    98,    98,   103,   106,   113,   117,   133,   140,   142,
     139,   166,   168,   166,   185,   187,   185,   200,   199,   215,
     223,   228,   236,   246,   248,   253,   256,   259,   262,   265,
     273,   276,   279,   282,   285,   288,   291,   294,   297,   300,
     303,   306,   309,   312,   315,   320,   321,   325,   326,   330,
     331
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FOR", "TO", "DO", "ENDFOR", "IF",
  "THEN", "ELSE", "ENDIF", "WHILE", "ENDWHILE", "REPEAT", "UNTIL", "BREAK",
  "CONTINUE", "Delimiter", "Identifier", "Assignment", "AND", "OR", "EQOP",
  "NEQOP", "'>'", "'<'", "LEQOP", "GEQOP", "'-'", "'+'", "'/'", "'*'",
  "DIV", "MOD", "EXP", "LOG", "LN", "SQRT", "NOT", "SIGN", "'('", "')'",
  "','", "$accept", "batchprog", "stmtList", "statement", "@1", "@2", "@3",
  "@4", "@5", "@6", "@7", "ifexpr", "expr", "simpleExpr", "paramlist",
  "fullList", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,    62,    60,   279,   280,    45,    43,
      47,    42,   281,   282,   283,   284,   285,   286,   287,   288,
      40,    41,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    43,    44,    45,    45,    46,    46,    46,    47,    48,
      46,    49,    50,    46,    51,    52,    46,    53,    46,    46,
      46,    46,    54,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    56,    56,    57,    57,    58,
      58
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     3,     0,     4,     3,     0,     0,
      11,     0,     0,     7,     0,     0,     6,     0,     7,     4,
       1,     1,     2,     1,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     0,     1,     1,
       3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     0,    11,    14,    20,    21,     0,     0,     2,
       3,     0,     0,    45,     0,     0,     0,     0,     0,     0,
       0,    22,    23,     0,     5,     0,    47,     1,     5,     5,
       0,    24,    25,    29,    28,    27,    26,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    12,     0,     7,    49,     0,    48,     4,
       0,     8,    46,    43,    44,    39,    40,    38,    37,    41,
      42,    31,    30,    33,    32,    34,    35,    36,     0,    15,
       6,     0,    17,    19,     0,     5,     0,    50,     5,     0,
       0,    16,     0,     9,    13,    18,     0,     5,     0,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     8,     9,    10,    84,    96,    23,    78,    24,    86,
      88,    11,    21,    22,    57,    58
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -15
static const yytype_int8 yypact[] =
{
      99,    -4,    21,   -15,   -15,   -15,   -15,     0,    47,    34,
     -15,    44,    35,   -15,    21,    21,    21,    21,    21,    21,
      21,    98,   -15,    21,    99,    21,    21,   -15,    99,    99,
      21,   -15,   -15,   -15,   -15,   -15,   -15,    67,    21,    21,
      21,    21,    21,    21,    21,    21,    21,    21,    21,    21,
      21,    21,    21,    98,     6,    98,    98,    14,    11,   -15,
      -2,    98,   -15,   111,   111,   122,   122,    50,    50,    50,
      50,    12,    12,    26,    26,    26,    26,   -15,    57,   -15,
     -15,    21,   -15,   -15,    59,    99,    21,    98,    99,    21,
       5,    98,    31,    98,   -15,   -15,    60,    99,     4,   -15
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -15,   -15,   -11,    36,   -15,   -15,   -15,   -15,   -15,   -15,
     -15,   -15,   -14,   -15,   -15,   -15
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      31,    32,    33,    34,    35,    36,    37,    82,    83,    53,
      99,    55,    56,    54,    12,    28,    61,    94,    60,    25,
      79,    28,    28,    28,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    13,
      26,    95,    48,    49,    50,    51,    52,    27,    28,    14,
      15,    28,    29,    81,    30,    80,    16,    17,    18,    19,
      52,    20,    85,    89,    59,    97,     0,    87,     0,     0,
       0,     0,    91,     0,    90,    93,     0,    92,    46,    47,
      48,    49,    50,    51,    52,     0,    98,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     1,     0,     0,     0,     2,     0,    62,     0,
       3,     0,     4,     0,     5,     6,     0,     7,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52
};

static const yytype_int8 yycheck[] =
{
      14,    15,    16,    17,    18,    19,    20,     9,    10,    23,
       6,    25,    26,    24,    18,    17,    30,    12,    29,    19,
      14,    17,    17,    17,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    18,
      40,    10,    30,    31,    32,    33,    34,     0,    17,    28,
      29,    17,     8,    42,    19,    41,    35,    36,    37,    38,
      34,    40,     5,     4,    28,     5,    -1,    81,    -1,    -1,
      -1,    -1,    86,    -1,    85,    89,    -1,    88,    28,    29,
      30,    31,    32,    33,    34,    -1,    97,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,     3,    -1,    -1,    -1,     7,    -1,    41,    -1,
      11,    -1,    13,    -1,    15,    16,    -1,    18,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     7,    11,    13,    15,    16,    18,    44,    45,
      46,    54,    18,    18,    28,    29,    35,    36,    37,    38,
      40,    55,    56,    49,    51,    19,    40,     0,    17,     8,
      19,    55,    55,    55,    55,    55,    55,    55,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    55,    45,    55,    55,    57,    58,    46,
      45,    55,    41,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    50,    14,
      41,    42,     9,    10,    47,     5,    52,    55,    53,     4,
      45,    55,    45,    55,    12,    10,    48,     5,    45,     6
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 98 "gram1.y"
    { bp_backpatch((yyvsp[(1) - (1)].t).brk, get_ic_pos()+1);
                          /* breaks in the outermost block cause exit */ }
    break;

  case 3:
#line 103 "gram1.y"
    { (yyval.t).cont = (yyvsp[(1) - (1)].t).cont; 
			  (yyval.t).brk = (yyvsp[(1) - (1)].t).brk; }
    break;

  case 4:
#line 107 "gram1.y"
    { (yyval.t).cont = bp_merge((yyvsp[(1) - (3)].t).cont, (yyvsp[(3) - (3)].t).cont); 
			  (yyval.t).brk = bp_merge((yyvsp[(1) - (3)].t).brk, (yyvsp[(3) - (3)].t).brk);
			  /* concatenate break and continue lists */ }
    break;

  case 5:
#line 113 "gram1.y"
    { (yyval.t).cont = BP_NULL; 
			  (yyval.t).brk = BP_NULL;
			  /* there were no cont or break statements */ }
    break;

  case 6:
#line 118 "gram1.y"
    { Val_type dmy;
			  Data_type data;
			  st_get_val_type((yyvsp[(1) - (4)].t).stp, &data, &dmy);
			  /* check wether it is really a known function */
			  if (data != FCT)
			    yyzerror("Function name invalid"); 
			  icode_jacket((yyvsp[(1) - (4)].t).stp, (yyvsp[(3) - (4)].t).arglist);
			  /* call jacket fct. with fct. name and pointer
			     to it's argument pointer list */
			  new_arglist();
			  /* prepare list for next fct.-call statement */
			  (yyval.t).cont = BP_NULL; 
			  (yyval.t).brk = BP_NULL;
			  /* there were no cont or break statements */ }
    break;

  case 7:
#line 134 "gram1.y"
    { icode_op(assign, (yyvsp[(1) - (3)].t).stp, (yyvsp[(3) - (3)].t).stp, 0);
			  (yyval.t).cont = BP_NULL; 
			  (yyval.t).brk = BP_NULL;
			  /* there were no cont or break statements */ }
    break;

  case 8:
#line 140 "gram1.y"
    { icode_op(assign, (yyvsp[(2) - (4)].t).stp, (yyvsp[(4) - (4)].t).stp, 0);
			  /* assign result of expr to Identifier */ }
    break;

  case 9:
#line 142 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			  icode_op(less_eq, (yyval.t).stp, (yyvsp[(2) - (7)].t).stp, (yyvsp[(7) - (7)].t).stp);
			  /* instruction to evaluate loop condition */
			  (yyval.t).tmp = get_ic_pos();
			  /* store position of less_eq */
			  icode_jmp(jmp_false, 0, (yyval.t).stp); 
			  /* exit FOR loop if condition is false */
			  (yyval.t).brk = bp_makelist(get_ic_pos());
			  /* store position of jmp_false */ }
    break;

  case 10:
#line 151 "gram1.y"
    { icode_op(add,(yyvsp[(2) - (11)].t).stp,(yyvsp[(2) - (11)].t).stp, st_lookup("%ONE"));
			  /* increment Identifier (loop counter) by one */
			  icode_jmp(jmp, (yyvsp[(8) - (11)].t).tmp, 0);
			  /* jump back to condition evaluation */
			  (yyval.t).brk = bp_merge((yyvsp[(8) - (11)].t).brk, (yyvsp[(10) - (11)].t).brk);
			  /* merge jumps that exit the FOR loop... */
			  bp_backpatch((yyval.t).brk, get_ic_pos()+1);
                          /* and bp them. */
			  bp_backpatch((yyvsp[(10) - (11)].t).cont, get_ic_pos()-1);
			  /* bp possible cont-stmts from stmtList:
			     let them jump to ENDFOR (increment) */
                          (yyval.t).cont = BP_NULL; 
			  (yyval.t).brk = BP_NULL;
			  /* there were no unresolved cont or break stmts */ }
    break;

  case 11:
#line 166 "gram1.y"
    { (yyval.t).tmp = get_ic_pos()+1; }
    break;

  case 12:
#line 168 "gram1.y"
    { icode_jmp(jmp_false, 0, (yyvsp[(3) - (3)].t).stp); 
			  /* exit WHILE loop if expr is false */
                          (yyval.t).brk = bp_makelist(get_ic_pos());
			  /* store position of jmp_false */ }
    break;

  case 13:
#line 173 "gram1.y"
    { icode_jmp(jmp, (yyvsp[(2) - (7)].t).tmp, 0);
			  /* jump back to expr */
			  (yyval.t).brk = bp_merge((yyvsp[(4) - (7)].t).brk, (yyvsp[(6) - (7)].t).brk);
			  /* merge jumps that exit the WHILE loop... */
			  bp_backpatch((yyval.t).brk, get_ic_pos()+1);
                          /* and bp them. */
			  bp_backpatch((yyvsp[(6) - (7)].t).cont, (yyvsp[(2) - (7)].t).tmp);
			  /* bp possible cont-stmts from stmtList */
                          (yyval.t).cont = BP_NULL;
			  (yyval.t).brk = BP_NULL;
			  /* there were no unresolved cont or break stmts */ }
    break;

  case 14:
#line 185 "gram1.y"
    { (yyval.t).tmp = get_ic_pos()+1;
                          /* store position of following stmtList */ }
    break;

  case 15:
#line 187 "gram1.y"
    { (yyval.t).tmp = get_ic_pos()+1;
                          /* store position of following expr */ }
    break;

  case 16:
#line 189 "gram1.y"
    { icode_jmp(jmp_false, (yyvsp[(2) - (6)].t).tmp, (yyvsp[(6) - (6)].t).stp);
			  /* jump back to begin of stmtList */
			  bp_backpatch((yyvsp[(3) - (6)].t).brk, get_ic_pos()+1);
			  /* bp possible break-stmts from stmtList */
			  bp_backpatch((yyvsp[(3) - (6)].t).cont, (yyvsp[(5) - (6)].t).tmp);
			  /* bp possible cont-stmts from stmtList */
                          (yyval.t).cont = BP_NULL;
			  (yyval.t).brk = BP_NULL;
			  /* there were no unresolved cont or break stmts */ }
    break;

  case 17:
#line 200 "gram1.y"
    { icode_jmp(jmp_true, 0, (yyvsp[(1) - (4)].t).stp); 
			  /* jump over the else block if expr is true */
                          (yyval.t).brk = bp_makelist(get_ic_pos());
			  /* store position of jmp_true */ 
                          (yyval.t).tmp = get_ic_pos()+1;
			  /* store position of else block */ }
    break;

  case 18:
#line 206 "gram1.y"
    { bp_backpatch((yyvsp[(1) - (7)].t).brk, (yyvsp[(5) - (7)].t).tmp);
			  /* backpatch jump over the if block */
			  bp_backpatch((yyvsp[(5) - (7)].t).brk, get_ic_pos()+1);
			  /* backpatch jump over the else block */
			  (yyval.t).cont = bp_merge((yyvsp[(3) - (7)].t).cont, (yyvsp[(6) - (7)].t).cont);
			  (yyval.t).brk = bp_merge((yyvsp[(3) - (7)].t).brk, (yyvsp[(6) - (7)].t).brk);
			  /* merge possible continue and break lists 
			     from both if and else blocks and return them */ }
    break;

  case 19:
#line 216 "gram1.y"
    { bp_backpatch((yyvsp[(1) - (4)].t).brk, get_ic_pos()+1);
			  /* backpatch the jump after expr */
			  (yyval.t).cont = (yyvsp[(3) - (4)].t).cont; 
			  /* pass possible continues... */
			  (yyval.t).brk = (yyvsp[(3) - (4)].t).brk;
			  /* and breaks to the surrounding loop */ }
    break;

  case 20:
#line 223 "gram1.y"
    { icode_jmp(jmp, 0, 0); 
			  (yyval.t).cont = BP_NULL;
                          (yyval.t).brk = bp_makelist(get_ic_pos());
                          /* store position of jump */ }
    break;

  case 21:
#line 228 "gram1.y"
    { icode_jmp(jmp, 0, 0); 
                          (yyval.t).cont = bp_makelist(get_ic_pos()); 
                          /* store position of jump */
			  (yyval.t).brk = BP_NULL; }
    break;

  case 22:
#line 236 "gram1.y"
    { icode_jmp(jmp_false, 0, (yyvsp[(2) - (2)].t).stp); 
			  /* jump over the if block if expr is false */
                          (yyval.t).brk = bp_makelist(get_ic_pos());
			  /* store position of jmp_false */ 
			  (yyval.t).tmp = (yyvsp[(2) - (2)].t).stp;
			  (yyval.t).stp = (yyvsp[(2) - (2)].t).stp;
			  /* pass result of expr to the else alternative */ }
    break;

  case 23:
#line 246 "gram1.y"
    { (yyval.t).stp = (yyvsp[(1) - (1)].t).stp; }
    break;

  case 24:
#line 248 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(sub,(yyval.t).stp,st_lookup("%ZERO"),(yyvsp[(2) - (2)].t).stp);
			    /* negate the value of expr by subtracting it 
			       from the zero built-in constant */ }
    break;

  case 25:
#line 253 "gram1.y"
    { (yyval.t).stp = (yyvsp[(2) - (2)].t).stp ;
			    /* if one wants to use a meaningless + sign */}
    break;

  case 26:
#line 256 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(not,(yyval.t).stp,(yyvsp[(2) - (2)].t).stp,0); }
    break;

  case 27:
#line 259 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(sq_rt,(yyval.t).stp,(yyvsp[(2) - (2)].t).stp,0); }
    break;

  case 28:
#line 262 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(ln,(yyval.t).stp,(yyvsp[(2) - (2)].t).stp,0); }
    break;

  case 29:
#line 265 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(lg,(yyval.t).stp,(yyvsp[(2) - (2)].t).stp,0); }
    break;

  case 30:
#line 273 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(add,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 31:
#line 276 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(sub,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 32:
#line 279 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(mult,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 33:
#line 282 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(dvde,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 34:
#line 285 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(intdiv,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 35:
#line 288 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(mod,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 36:
#line 291 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp()); 
			    icode_op(bmraise,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 37:
#line 294 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			    icode_op(less,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 38:
#line 297 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			    icode_op(greater,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 39:
#line 300 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			    icode_op(eq,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 40:
#line 303 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			    icode_op(not_eq,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 41:
#line 306 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			    icode_op(less_eq,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 42:
#line 309 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			    icode_op(great_eq,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 43:
#line 312 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			    icode_op(and,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 44:
#line 315 "gram1.y"
    { (yyval.t).stp = st_insert(newtmp());
			    icode_op(or,(yyval.t).stp,(yyvsp[(1) - (3)].t).stp,(yyvsp[(3) - (3)].t).stp); }
    break;

  case 45:
#line 320 "gram1.y"
    { (yyval.t).stp = (yyvsp[(1) - (1)].t).stp; }
    break;

  case 46:
#line 321 "gram1.y"
    { (yyval.t).stp = (yyvsp[(2) - (3)].t).stp; }
    break;

  case 47:
#line 325 "gram1.y"
    { (yyval.t).arglist = ARG_NULL; }
    break;

  case 48:
#line 326 "gram1.y"
    { (yyval.t).arglist = (yyvsp[(1) - (1)].t).arglist; }
    break;

  case 49:
#line 330 "gram1.y"
    { (yyval.t).arglist = add_to_arglist((yyvsp[(1) - (1)].t).stp); }
    break;

  case 50:
#line 331 "gram1.y"
    { (yyval.t).arglist = add_to_arglist((yyvsp[(3) - (3)].t).stp);
			    /* ST pointers to the arguments of a function
			       are stored in the argument pointer list */ }
    break;


/* Line 1267 of yacc.c.  */
#line 1868 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}




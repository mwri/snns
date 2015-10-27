/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/symtab.ph,v $
  SHORTNAME      : symtab
  SNNS VERSION   : 4.2

  PURPOSE        : Symbol table (ST) for SNNS batch interpreter
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.4 $
  LAST CHANGE    : $Date: 1998/02/25 15:35:00 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _SYMTAB_DEFINED_
#define _SYMTAB_DEFINED_

/* begin global definition section */

/* the Symbol Table (ST):


          | symbol | symbol | symbol | symbol
    Index | name   | status | type   | value
    ------+--------+--------+--------+--------
      0   | entry  | entry  | entry  | entry  
      1   | ...    |        |        |

      |       |        |        |        |________ Val_type
      |       |        |        |_________________ Data_type
      |       |        |__________________________ Bool_type
      |       |___________________________________ char *
      |___________________________________________ St_ptr_type


   types used by the ST:
*/
typedef int Int_type;       /* God made the integers ... */
typedef enum { UNKNOWN, INT, REAL, BOOL, FCT, STRING } Data_type;
#undef FALSE
#undef TRUE
typedef enum { FALSE = 0, TRUE } Bool_type;
#define FALSE 0
#define TRUE 1
typedef double Real_type;
typedef char *String_type;  /* ... all else is the work of man */

typedef int St_ptr_type;    /* ST index; points to ST entries */

#define ST_NULL -1          /* ST empty value */

/*****************************************************************************
  the argument pointer list:
  linked list of ST pointers which point to 
  the arguments for a function call in a user program:
******************************************************************************/
struct arglist_type { 
                      St_ptr_type arg_ptr;        /* points to ST entry */
		      struct arglist_type *next;  /* link field */
		    };
typedef struct arglist_type arglist_type;

#define ARG_NULL (arglist_type *) 0


/* function type for jacket function: */
typedef void (*Jacket_fct_ptr)(arglist_type *); 

/* type of value of a ST member: */
typedef union { Int_type int_val;
                Bool_type bool_val;
                Real_type real_val;
		String_type string_val;
		Jacket_fct_ptr fct_val;
              } Val_type;    


void st_init(void);

St_ptr_type st_insert(const char *name);
St_ptr_type st_lookup(const char *name);

void st_get_val_type(St_ptr_type pos, Data_type *type, Val_type *val);
void st_set_val_type(St_ptr_type pos, Data_type type, Val_type val);

void st_set_ro(St_ptr_type pos);
Bool_type st_get_ro(St_ptr_type pos);

char *newtmp(void);

/* end global definition section */

/* begin private definition section */

#define ST_SIZE_ADD 100       /* number of ST entries allocated at first and
			         increment size of ST memory reallocation */
#define TMP_STR     "tmp"     /* suffix of temporary variable names */

#define D( x ) 


/*****************************************************************************
  the symbol table data type:

******************************************************************************/
typedef struct { char *name;
		 Bool_type read_only;
		 Data_type type;
		 Val_type val;
     	       } St_type;

static St_type *st = NULL;              /* the symbol table */
static St_ptr_type st_pos = 0;          /* the pointer to the entries */
static size_t st_size = ST_SIZE_ADD;    /* and the current size */

static void st_out(void);

/* end private definition section */

#endif

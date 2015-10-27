/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/ictab.ph,v $
  SHORTNAME      : ictab
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS batch interpreter 
                   functions to set up the Intermediate Code TABle
		   and to execute the code (interpreter loop)

  NOTES          : abbreviations: IC = Intermediate Code
                                  ST = Symbol Table

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:34 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _ICTAB_DEFINED_
#define _ICTAB_DEFINED_


/* begin global definition section */

typedef int Ic_ptr_type;

typedef void (*Op_fct_ptr)(St_ptr_type, St_ptr_type, St_ptr_type);
typedef void (*Jmp_fct_ptr)(Ic_ptr_type, St_ptr_type);

int get_xref(void);

void ictab_init(void);

void icode_jacket(St_ptr_type fct_name, arglist_type *arglist);
void icode_op(Op_fct_ptr Op_fct, St_ptr_type res, St_ptr_type op1, 
	      St_ptr_type op2);
void icode_jmp(Jmp_fct_ptr Jmp_fct, Ic_ptr_type jmp_pos, St_ptr_type arg);

void set_ic_pc(Ic_ptr_type pos);
Ic_ptr_type get_ic_pc(void);
Ic_ptr_type get_ic_pos(void);

void backpatch(Ic_ptr_type from_ic_pos, Ic_ptr_type to_ic_pos);

void run(void);

/* end global definition section */


/* begin private definition section */

/*****************************************************************************
  type definitions for the intermediate code table

******************************************************************************/

typedef struct{
    Jacket_fct_ptr Jacket_fct;       /* function pointer */
    arglist_type *arglist;       /* pointer to argument pointer list */
} Jacket_fct_type;                 /* structure for a SNNS-kernel call fct. */

typedef struct{
    Op_fct_ptr Op_fct;           /* function pointer */
    St_ptr_type res, op1, op2;   /* ST pointers to the arguments */
} Op_fct_type;                   /* structure for a 3-address instruction */

typedef struct{
    Jmp_fct_ptr Jmp_fct;         /* function pointer */
    Ic_ptr_type jmp_pos;         /* IC list pointer to the jump target */
    St_ptr_type arg;             /* ST pointer to the jump condition */
} Jmp_fct_type;                  /* structure for a jump instruction */

typedef union{
    Jacket_fct_type Jacket_fct;
    Op_fct_type Op_fct;
    Jmp_fct_type Jmp_fct;
} Ic_fct_type;                   /* union to collect the 3 possible IC
				    function types (see above) */

typedef enum{JACKET_FCT, OP_FCT, JMP_FCT} fct_type; /* function types */

/* the IC list structure consists of the function type field and union
   with the function pointer and the corresponding argument(s): */
typedef struct{
    fct_type fct;                /* function type field */
    Ic_fct_type Ic_fct;          /* function pointer */
} Ic_type;                       /* type for interm. code table */


#define ICTAB_SIZE_ADD 100    /* number of ictab entries allocated at first 
				 and increment size of 
				 ictab memory reallocation */
#define D( x ) 

static void ic_xref(Ic_ptr_type ic_pos);
static void inc_ic_pos(void);

static size_t ictab_size = ICTAB_SIZE_ADD; 
                                 /* current size of ictab */
static Ic_type *ic_list = NULL;  /* IC code table */
static Ic_ptr_type ic_pos = 0;   /* instruction number in IC list */
static Ic_ptr_type ic_pc;        /* runtime instruction counter */


/*****************************************************************************
  cross reference table: 
  source code line number <==> IC instruction 
  purpose: supply line numbers to warnings and error messages 
******************************************************************************/
static int *lines_tab = NULL;

/* end private definition section */

#endif

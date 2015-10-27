/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/ictab.c,v $
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
  RCS VERSION    : $Revision: 1.5 $
  LAST CHANGE    : $Date: 1998/03/13 16:50:59 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>

#include "symtab.h"
#include "ictab.ph"
#include "glob_typ.h"    /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"       /* for ic_xref() */
#include "memory.h"      /* for re_malloc */


/*****************************************************************************
  FUNCTION : ic_xref

  PURPOSE  : fills in the cross reference table
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void ic_xref(Ic_ptr_type ic_pos)
{
  lines_tab[ic_pos] = get_sourceline();
}


/*****************************************************************************
  FUNCTION : get_xref

  PURPOSE  : reads the cross reference table
  RETURNS  : the source line number
  NOTES    :

  UPDATE   : 
******************************************************************************/
int get_xref(void)
{
  return lines_tab[get_ic_pc()];
}



/*****************************************************************************
  FUNCTION : inc_ic_pos

  PURPOSE  : increments the instruction number and inserts it in the
             source/intermediate code cross reference list in error.c
	     increases ictab and cross reference table if ictab is full
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void inc_ic_pos(void)
{
  size_t new_ictab_size;

D( printf("ic_pos: %d\n",ic_pos); )

  if (ic_pos == ictab_size-1)
  {  
D( printf("re_malloc ictab\n"); )

    /* increase the size of ictab: */
    new_ictab_size = ictab_size + ICTAB_SIZE_ADD;

    /* reallocate memory with self-made realloc-function: */
    ic_list = re_malloc(ic_list,
		   (size_t) (ictab_size * sizeof (Ic_type)),
		   (size_t) (new_ictab_size * sizeof (Ic_type)));

    lines_tab = re_malloc(lines_tab,
		   (size_t) (ictab_size * sizeof (int)),
		   (size_t) (new_ictab_size * sizeof (int)));

    /* store the new ictab size for the next re_malloc:*/
    ictab_size = new_ictab_size;
  }

  /* enter a line-number cross reference: */
  ic_xref(ic_pos);

  ic_pos++;
}


/*****************************************************************************
  FUNCTION : ictab_init

  PURPOSE  : initializes the intermediate code table
             and the cross reference table
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void ictab_init(void)
{
  ic_list = malloc (ictab_size * sizeof(Ic_type));
  lines_tab = malloc (ictab_size * sizeof(int));
  if ((ic_list == NULL) || (lines_tab == NULL)) err_prt(ERR_MEM);
}



/*****************************************************************************
  FUNCTION : icode_jacket

  PURPOSE  : adds a jacket function to the IC list
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void icode_jacket(St_ptr_type fct_name, arglist_type *arglist)
{
  Val_type val;
  Data_type data;
  
  /* enter function type: */
  ic_list[ic_pos].fct = JACKET_FCT;
  /* get function pointer: */
  st_get_val_type(fct_name, &data, &val);
  /* enter function pointer: */
  ic_list[ic_pos].Ic_fct.Jacket_fct.Jacket_fct = val.fct_val;
  /* enter pointer to argument pointer list: */
  ic_list[ic_pos].Ic_fct.Jacket_fct.arglist = arglist;

  inc_ic_pos();
}


/*****************************************************************************
  FUNCTION : icode_op

  PURPOSE  : adds a 3-address command (arithmetic, logical or assignment) 
             to the IC list
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void icode_op(Op_fct_ptr Op_fct, St_ptr_type res, St_ptr_type op1, 
	      St_ptr_type op2)
{ 
  /* enter function type: */
  ic_list[ic_pos].fct = OP_FCT;
  /* enter function name: */
  ic_list[ic_pos].Ic_fct.Op_fct.Op_fct = Op_fct; 
  /* enter function arguments (all are symbol table pointers): */
  ic_list[ic_pos].Ic_fct.Op_fct.res = res;
  ic_list[ic_pos].Ic_fct.Op_fct.op1 = op1;
  ic_list[ic_pos].Ic_fct.Op_fct.op2 = op2;

  inc_ic_pos();
}


/*****************************************************************************
  FUNCTION : icode_jmp

  PURPOSE  : adds a(n) (un-)conditional jump to the IC list
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void icode_jmp(Jmp_fct_ptr Jmp_fct, Ic_ptr_type jmp_pos, St_ptr_type arg)
{
  /* enter function type: */
  ic_list[ic_pos].fct = JMP_FCT;
  /* enter jump function name: */
  ic_list[ic_pos].Ic_fct.Jmp_fct.Jmp_fct = Jmp_fct;
  /* enter jump target address: */
  ic_list[ic_pos].Ic_fct.Jmp_fct.jmp_pos = jmp_pos;
  /* enter ST pointer to variable containing jump condition: */
  ic_list[ic_pos].Ic_fct.Jmp_fct.arg = arg;

  inc_ic_pos();
}


/*****************************************************************************
  FUNCTION : set_ic_pc

  PURPOSE  : sets runtime instruction counter to pos
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void set_ic_pc(Ic_ptr_type pos)
{
  ic_pc = pos;
}


/*****************************************************************************
  FUNCTION : get_ic_pc

  PURPOSE  : gets runtime instruction counter
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
Ic_ptr_type get_ic_pc(void)
{
  return ic_pc;
}


/*****************************************************************************
  FUNCTION : get_ic_pos

  PURPOSE  : gets number of least inserted IC instruction
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
Ic_ptr_type get_ic_pos(void)
{
  return ic_pos-1;
}


/*****************************************************************************
  FUNCTION : backpatch

  PURPOSE  : performs a low-level backpatch for a single jump instruction
             by inserting to_ic_pos as jump target in the jump instruction
	     at IC address from_ic_pos
  RETURNS  : 
  NOTES    : low-level interface function for backpatch.c

  UPDATE   : 
******************************************************************************/
void backpatch(Ic_ptr_type from_ic_pos, Ic_ptr_type to_ic_pos)
{
  ic_list[from_ic_pos].Ic_fct.Jmp_fct.jmp_pos = to_ic_pos;
}


/*****************************************************************************
  FUNCTION : run

  PURPOSE  : main intermediate code interpreter loop
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void run(void)
{ 
  ic_pc = 0;    /* set runtime instruction counter to 1st instruction */

  D( printf("run start; last instruction: %d\nexecuting IC instructions:\n", 
	    ic_pos-1);)

  while (ic_pc < ic_pos)
  {
    D( printf("%d  ", ic_pc); )

    switch(ic_list[ic_pc].fct)  /* switch function type */
    {
      /* execute IC function pointers with appropriate arguments: */
      case JACKET_FCT: 
      ic_list[ic_pc].Ic_fct.Jacket_fct.Jacket_fct(
      ic_list[ic_pc].Ic_fct.Jacket_fct.arglist);
      break;

      case OP_FCT: 
      ic_list[ic_pc].Ic_fct.Op_fct.Op_fct( 
      ic_list[ic_pc].Ic_fct.Op_fct.res,
      ic_list[ic_pc].Ic_fct.Op_fct.op1,
      ic_list[ic_pc].Ic_fct.Op_fct.op2);
      break;

      case JMP_FCT: 
      ic_list[ic_pc].Ic_fct.Jmp_fct.Jmp_fct( 
      ic_list[ic_pc].Ic_fct.Jmp_fct.jmp_pos,
      ic_list[ic_pc].Ic_fct.Jmp_fct.arg);
    }
    /* increment instruction counter: */
    ic_pc++;
  }

  D( printf("\nrun stop\n");)
}

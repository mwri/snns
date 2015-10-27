/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/icopjmp.ph,v $
  SHORTNAME      : icopjmp
  SNNS VERSION   : 4.2

  PURPOSE        : Intermediate code (IC) functions for SNNS batch interpreter:
                   assignment, arithmetic and logical operators, jumps

                   Pointers to these functions are inserted in the 
                   intermediate code table by the grammar
		   and are executed during intermediate code 
		   interpretation (run())
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.4 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _ICOPJMP_DEFINED_
#define _ICOPJMP_DEFINED_

/* begin global definition section */

void assign(St_ptr_type, St_ptr_type, St_ptr_type);

void sq_rt(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
void ln(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
void lg(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);

void add(St_ptr_type, St_ptr_type, St_ptr_type);
void sub(St_ptr_type, St_ptr_type, St_ptr_type);
void mult(St_ptr_type, St_ptr_type, St_ptr_type);
void dvde(St_ptr_type, St_ptr_type, St_ptr_type);
void intdiv(St_ptr_type, St_ptr_type, St_ptr_type);
void mod(St_ptr_type, St_ptr_type, St_ptr_type);
void bmraise(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);

void not(St_ptr_type, St_ptr_type, St_ptr_type);
void and(St_ptr_type, St_ptr_type, St_ptr_type);
void or(St_ptr_type, St_ptr_type, St_ptr_type);

void less(St_ptr_type, St_ptr_type, St_ptr_type);
void greater(St_ptr_type, St_ptr_type, St_ptr_type);
void less_eq(St_ptr_type, St_ptr_type, St_ptr_type);
void great_eq(St_ptr_type, St_ptr_type, St_ptr_type);
void eq(St_ptr_type, St_ptr_type, St_ptr_type);
void not_eq(St_ptr_type, St_ptr_type, St_ptr_type);

void jmp(Ic_ptr_type jmp_pos, St_ptr_type arg_ptr);
void jmp_true(Ic_ptr_type jmp_pos, St_ptr_type arg_ptr);
void jmp_false(Ic_ptr_type jmp_pos, St_ptr_type arg_ptr);

/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif

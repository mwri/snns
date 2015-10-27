/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/icopjmp.h,v $
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
  LAST CHANGE    : $Date: 1998/02/25 15:34:31 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


extern void assign(St_ptr_type res, St_ptr_type arg, St_ptr_type dmy);

extern void sq_rt(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void ln(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void lg(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);

extern void add(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void sub(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void mult(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void dvde(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void intdiv(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void mod(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void bmraise(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);

extern void not(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void and(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void or(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);

extern void less(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void greater(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void less_eq(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void great_eq(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void eq(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);
extern void not_eq(St_ptr_type res, St_ptr_type op1, St_ptr_type op2);

extern void jmp(Ic_ptr_type jmp_pos, St_ptr_type arg_ptr);
extern void jmp_true(Ic_ptr_type jmp_pos, St_ptr_type arg_ptr);
extern void jmp_false(Ic_ptr_type jmp_pos, St_ptr_type arg_ptr);

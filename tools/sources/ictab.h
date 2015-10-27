/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/ictab.h,v $
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


typedef int Ic_ptr_type;

typedef void (*Op_fct_ptr)(St_ptr_type, St_ptr_type, St_ptr_type);
typedef void (*Jmp_fct_ptr)(Ic_ptr_type, St_ptr_type);

extern int get_xref(void);

extern void ictab_init(void);

extern void icode_jacket(St_ptr_type fct_name, arglist_type *arglist);
extern void icode_op(Op_fct_ptr Op_fct, St_ptr_type res, St_ptr_type op1, 
	      St_ptr_type op2);
extern void icode_jmp(Jmp_fct_ptr Jmp_fct, Ic_ptr_type jmp_pos, 
		      St_ptr_type arg);

extern void set_ic_pc(Ic_ptr_type pos);
extern Ic_ptr_type get_ic_pc(void);
extern Ic_ptr_type get_ic_pos(void);

extern void backpatch(Ic_ptr_type from_ic_pos, Ic_ptr_type to_ic_pos);

extern void run(void);


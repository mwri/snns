/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/arglist.h,v $
  SHORTNAME      : arglist
  SNNS VERSION   : 4.2

  PURPOSE        : Argument list handling functions for the SNNS batch
                   interpreter:
                   The argument list contains pointers to the symbol table 
		   positions of a function's arguments

  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:11 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


extern void new_arglist(void);
extern arglist_type *add_to_arglist(St_ptr_type ident);


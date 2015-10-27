/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/arglist.ph,v $
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

#ifndef _ARGLIST_DEFINED_
#define _ARGLIST_DEFINED_

/* begin global definition section */

void new_arglist(void);
arglist_type *add_to_arglist(St_ptr_type ident);

/* end global definition section */

/* begin private definition section */

/* define argument list pointer and initialize it to NULL: */
static arglist_type *arg_list = ARG_NULL;

/* end private definition section */

#endif


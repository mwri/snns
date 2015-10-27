/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/backpatch.ph,v $
  SHORTNAME      : backpatch
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS batch interpreter 
                   Functions to perform backpatching of jump targets
		   in intermediate code

  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:14 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _BACKPATCH_DEFINED_
#define _BACKPATCH_DEFINED_

/* begin global definition section */

struct bp_list{
  Ic_ptr_type ic_pc;
  struct bp_list *next;
};

typedef struct bp_list bp_list;

#define BP_NULL (bp_list *) 0

bp_list *bp_makelist(Ic_ptr_type elem);
bp_list *bp_merge(bp_list *list1, bp_list *list2);

void bp_backpatch(bp_list *list, Ic_ptr_type label);

/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/backpatch.h,v $
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
  LAST CHANGE    : $Date: 1998/02/25 15:34:13 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

struct bp_list{
  Ic_ptr_type ic_pc;
  struct bp_list *next;
};

typedef struct bp_list bp_list;

#define BP_NULL (bp_list *) 0

extern bp_list *bp_makelist(Ic_ptr_type elem);
extern bp_list *bp_merge(bp_list *list1, bp_list *list2);

extern void bp_backpatch(bp_list *list, Ic_ptr_type label);


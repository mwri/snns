/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/backpatch.c,v $
  SHORTNAME      : backpatch
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS batch interpreter 
                   Functions to perform backpatching of jump targets
		   in intermediate code

  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.4 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:05 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>

#include "symtab.h"        /* required by the following include file */
#include "ictab.h"
#include "backpatch.ph"
#include "glob_typ.h"      /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"

#define D( x ) 


/*****************************************************************************
  FUNCTION : bp_makelist

  PURPOSE  : creates a list with one element elem in it
             elem is the instruction number of the jump instruction which 
	     is to be backpatched
  RETURNS  : a pointer to that list
  NOTES    : 

  UPDATE   : 
******************************************************************************/
bp_list *bp_makelist(Ic_ptr_type elem)
{
  bp_list *tmp;

  /* allocate memory for the element: */
  tmp = (bp_list *)malloc(sizeof(bp_list));
  if (tmp == BP_NULL) err_prt(ERR_MEM);

  /* fill in element and link field: */
  tmp->ic_pc = elem;
  tmp->next = BP_NULL;

  return tmp;
}


/*****************************************************************************
  FUNCTION : bp_merge

  PURPOSE  : merges list1 and list2
  RETURNS  : a pointer to the resulting list 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
bp_list *bp_merge(bp_list *list1, bp_list *list2)
{
  bp_list *tmp = list1;

  /* one of 'em empty? */
  if (list1 == BP_NULL) return list2;
  if (list2 == BP_NULL) return list1;

  /* no, so glue 'em together: */
  while (tmp->next != BP_NULL) tmp = tmp->next;
  tmp->next = list2;

  return list1;
}


/*****************************************************************************
  FUNCTION : bp_backpatch

  PURPOSE  : enters label as jump target for all jump instructions
             pointed to by list 
  RETURNS  : 
  NOTES    : deallocates memory for list afterwards
             warning: list is not nil'ed!
  UPDATE   : 
******************************************************************************/
void bp_backpatch(bp_list *list, Ic_ptr_type label)
{
  bp_list *tmp;

  while (list != BP_NULL)
  {
    D( printf("backpatching from list %p jump at #%d with ic address %d\n", 
	   list, list->ic_pc, label); )

    /* backpatch a single instruction: */
    backpatch(list->ic_pc, label);

    /* and sell the memory: */
    tmp = list;
    list = list->next;
    free(tmp);
  }
}


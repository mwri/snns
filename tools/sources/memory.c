/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/memory.c,v $
  SHORTNAME      : memory
  SNNS VERSION   : 4.2

  PURPOSE        : implements the realloc function
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.7 $
  LAST CHANGE    : $Date: 1998/03/13 16:51:01 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "memory.ph"
#include "symtab.h"
#include "ictab.h"
#include "glob_typ.h"    /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"


/*****************************************************************************
  FUNCTION : re_malloc

  PURPOSE  : increases or decreases the size of an malloc'ed memory structure
             1. parameter is pointer to old mem block
	     2. parameter is size of old mem block
	     3. parameter is size of new mem block
  RETURNS  : pointer to new mem block
  NOTES    : frees the old mem block

  UPDATE   : 
******************************************************************************/
void *re_malloc(void *old_ptr, size_t old_size, size_t new_size)
{
  void *new_ptr;

  /* allocate mem block of new size: */
  new_ptr = malloc(new_size);
  if (new_ptr == NULL) err_prt(ERR_MEM);

  /* copy old mem block to new mem block: */
  memcpy(new_ptr, old_ptr, (new_size<old_size) ? new_size : old_size);

  /* free old mem block: */
  free(old_ptr);

  return new_ptr;
}


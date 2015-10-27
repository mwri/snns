/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/memory.ph,v $
  SHORTNAME      : memory
  SNNS VERSION   : 4.2

  PURPOSE        : implements the realloc function
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _MEMORY_DEFINED_
#define _MEMORY_DEFINED_

/* begin global definition section */

void *re_malloc(void *old_ptr, size_t old_size, size_t new_size);

/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif

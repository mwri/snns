/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/arglist.c,v $
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
  RCS VERSION    : $Revision: 1.4 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:05 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include "symtab.h"      /* for arglist_type, St_ptr_type */
#include "arglist.ph"
#include "ictab.h"       /* for error.h */
#include "glob_typ.h"    /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"



/*****************************************************************************
  FUNCTION : new_arglist

  PURPOSE  : initializes a new argument list
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void new_arglist(void)
{
  arg_list = ARG_NULL;
}


/*****************************************************************************
  FUNCTION : add_to_arglist

  PURPOSE  : adds a new element to the end of the argument list
             element is a symbol table pointer
  RETURNS  : the argument list
  NOTES    : list is constructed 'backwards' because the grammar that
             fills in the list works from left to right

  UPDATE   : 
******************************************************************************/
arglist_type *add_to_arglist(St_ptr_type ident)
{
  arglist_type *new = arg_list;

  /* if list is empty, let it point to the newly malloc'ed element: */
  if (arg_list == ARG_NULL)
  {
    arg_list = (arglist_type *) malloc(sizeof(arglist_type));
    if (arg_list == ARG_NULL) err_prt(ERR_MEM);

    arg_list->arg_ptr = ident;
    arg_list->next = ARG_NULL;
  }
  else
  /* list is not empty: */
  {
    /* advance new pointer to it's end: */
    while (new->next != ARG_NULL) new = new->next;

    /* add new element to the end of the list: */
    new->next = (arglist_type *) malloc(sizeof(arglist_type));
    if (new->next == ARG_NULL) err_prt(ERR_MEM);

    /* let new point to the new element: */
    new = new->next;

    /* and fill it's fields: */
    new->arg_ptr = ident;
    new->next = ARG_NULL;
  }

  return arg_list;
}



/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_lists.c,v $
  SHORTNAME      : lists.c
  SNNS VERSION   : 4.2

  PURPOSE        : create a linear list
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.199

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/13 16:31:47 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "glob_typ.h"

#include "d3_global.h"
#include "d3_lists.h"
#include "d3_lists.ph"


/*****************************************************************************
  FUNCTION : d3_insertUnit

  PURPOSE  : creates a new list entry
  RETURNS  : a pointer to the new list
  NOTES    :

******************************************************************************/

void d3_insertUnit (d3_unitPtrType **list, int num)
{
    d3_unitPtrType *new_elem;

    new_elem = (d3_unitPtrType *) malloc (sizeof (d3_unitPtrType));
    if (new_elem == NULL)
      {
         fprintf (stdout, "D3-Error: Not Enough Memory\n\n");
         return;
      }
    new_elem->unitNo = num;
    new_elem->next = *list;
    *list = new_elem;
}



/*****************************************************************************
  FUNCTION : d3_displayUnitList

  PURPOSE  : writes a list to stdout
  RETURNS  : 
  NOTES    :

******************************************************************************/
void d3_displayUnitList (d3_unitPtrType  *list)
{
    while (list != NULL)
      {
        fprintf (stdout, "%7d", list->unitNo);
        list = list->next;
      }
    fprintf (stdout, "\n");
}

/* end of file */
/* lines: 83 */

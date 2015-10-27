/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/mkout.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : create snns output patten
  NOTES          :

  AUTHOR         : Ralf Huebner 
  DATE           : 2.4.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:09 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RAWNUM 8

void main(int argc, char *argv[])

{
   int i, units, active;

   if ((argc==1) || (int) strcmp(*++argv, "-h")==0 ) {
      fprintf (stderr, "create snns output patten\n\n");
      fprintf (stderr, "  usage: mkout <#units> <active_unit>\n\n");
      exit(0);
   }
   units = atoi(*argv);
   active = atoi(*++argv);
   for (i=0; i<units; i++) {
      if ( (i % RAWNUM == 0) && (i != 0) )
          printf("\n");
      if (i+1==active)
          printf ("1.000 ");
      else
          printf ("0.000 ");
   }
   printf ("\n");
}
   

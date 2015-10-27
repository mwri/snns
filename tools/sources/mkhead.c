/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/mkhead.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : write snns pattern file header
  NOTES          :

  AUTHOR         : Ralf Huebner 
  DATE           : 2.4.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:20 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


void main(int argc, char *argv[])

{
   time_t t;

   if ((argc != 4) || (int) strcmp(*++argv, "-h")==0 ) {
      fprintf (stderr, "write snns pattern file header\n\n");
      fprintf (stderr, "  usage: mkhead <#pats> <#in_units> <#out_units>\n\n");
      exit(0);
   }
   time(&t);
   printf ("SNNS pattern definition file V1.4\n");
   printf ("generated at %s\n\n", ctime( &t));
   printf ("No. of patterns     : %s\n", *argv);
   printf ("No. of input units  : %s\n", *++argv);
   printf ("No. of output units : %s\n", *++argv);
   printf ("\n");
}
   

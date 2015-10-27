/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/mkpat.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : convert 8 bit raw file to snns pattern entry
  NOTES          :

  AUTHOR         : Ralf Huebner 
  DATE           : 2.4.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/03/13 16:51:01 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAWNUM 8

void main(int argc, char *argv[])

{
   int i;
   float out_val;
   unsigned xsize, ysize;
   unsigned char *buffer, *ptr, *limit;

   if ((argc!=3) || (int) strcmp(*++argv, "-h")==0 ) {
      fprintf (stderr, "convert 8 bit raw file to snns pattern entry\n\n");
      fprintf (stderr, "  usage: mkpat <xsize> <ysize>\n\n");
      exit(0);
   }
   xsize = atol(*argv);
   ysize = atol(*++argv);

   if ( (buffer=(unsigned char *)malloc(xsize*ysize) ) == NULL) {
       fprintf (stderr, "ERROR: Can`t allocate memory\n");
       exit(-1);
   }
   if ((unsigned char *)fread(buffer, xsize*ysize, 1, stdin) == NULL) {
       fprintf (stderr, "ERROR: Can`t read from stdin\n");
       exit(-1);
   }
   
   i = 0;
   limit = buffer + xsize*ysize;
   
   for(ptr=buffer; ptr<limit; ptr++) {
      out_val = ((float) *ptr) / 255.0;
      if ( ((i % xsize == 0) || (i % RAWNUM == 0)) && (i != 0) )
          printf("\n");
      i++;
      printf ("%.3f ", out_val);
   }
  printf ("\n");
  free(buffer);
}
   

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/feedback-gennet.c,v $
  SHORTNAME      : feedback-gennet.c
  SNNS VERSION   : 4.2

  PURPOSE        : Generate a SNNS network file for fully recurrent networks
  NOTES          :

  AUTHOR         : Martin Reczko 
  DATE           : 1.3.93
  VERSION        : 1.0                                         

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:05 $


          This part is Copyright 1992,93 by Martin Reczko, DKFZ Heidelberg

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

/*
  Generate a SNNS network file for fully recurrent networks.
  The network has the following structure:
    - input layer with no intra layer connections
    - fully recurrent hidden layer
    - output layer: connections from each hidden unit to each output unit AND
     OPTIONALLY fully recurrent intra layer connections in the output layer AND
     OPTIONALLY feedback connections from each output unit to each hidden unit
  The activation function of the output units can be set to
  sigmoidal or linear. All weights are initialized with 0.0 .
  Other initializations should be performed by the init functions in SNNS.
*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *in,*out;
float amplitude;

double drand48()
{
 return 0.0;
}

void main ()
{
 int i,j;
 int nin,nhid,nout,nconnections;
 char netname[1024];
 char out2out,linout,out2hid;
 int last_output_source;

 printf("Enter # input units:");
 scanf("%d",&nin);
 printf("Enter # hidden units:");
 scanf("%d",&nhid);
 printf("Enter # output units:");
 scanf("%d",&nout);
 printf("INTRA layer connections in the output layer (y/n):");
 scanf("\n%c",&out2out);
 printf("feedback connections from output to hidden units (y/n):"),
 scanf("\n%c",&out2hid);
 printf("Linear output activation function (y/n):");
 scanf("\n%c",&linout);
 printf("Enter name of network file:");
 scanf("\n%s",netname);

 printf("working...");
 if (out2out=='y') {
   last_output_source=nout;
   if (out2hid=='y')
     nconnections = nhid *(nin+nhid+nout);
   else
     nconnections = nhid *(nin+nhid);
    nconnections += nout *(nhid+nout);
 } else {
   last_output_source=0;
   if (out2hid=='y')
     nconnections = nhid *(nin+nhid+nout);
   else
     nconnections = nhid *(nin+nhid);
   nconnections += nout * nhid;
 }
 if ((out = fopen(netname, "w")) ==  NULL)
   {  
     fprintf (stderr, "error:  can't create file %s\n", netname) ;
     exit(0);
   } 

 fprintf(out,"SNNS network definition file V1.4-3D\n");
 fprintf(out,"generated at Sat Jan 23 16:55:30 1992\n");
 fprintf(out,"\n");
 fprintf(out,"network name : rec-gennet\n");
 fprintf(out,"source files :\n");
 fprintf(out,"no. of units : %d\n",nin+nhid+nout);
 fprintf(out,"no. of connections : %d\n",nconnections);
 fprintf(out,"no. of unit types : 0\n");
 fprintf(out,"no. of site types : 0\n");
 fprintf(out,"\n");
 fprintf(out,"learning function : QPTT\n");
 fprintf(out,"update function   : BPTT_Order\n");
 fprintf(out,"\n");
 fprintf(out,"unit default section :\n");
 fprintf(out,"\n");
 fprintf(out,"---------|----------|----|--------|-------|--------------|-------------\n");
 fprintf(out," 0.00000 |  0.00000 | h  |      0 |     1 | Act_Logistic | Out_Identity \n");
 fprintf(out,"---------|----------|----|--------|-------|--------------|-------------\n");
 fprintf(out,"\n");
 fprintf(out,"unit definition section :\n");
 fprintf(out,"\n");
 fprintf(out,"----|----------|----------|----------|----------|----|----------|--------------|--------------|-------\n");

 for (i=1;i<=nin;i++) fprintf(out,
"%4d|          |          |  0.00000 |  0.00000 | i  | %2d, 1, 2 |              |              | \n",i,i);
 for (i=1;i<=nhid;i++) fprintf(out,
"%4d|          |          |  0.00000 |  0.00000 | h  | %2d, 2, 2 |              |              | \n",i+nin,i);
 for (i=1;i<=nout;i++) if (linout=='y') fprintf(out,
"%4d|          |          |  0.00000 |  0.00000 | o  | %2d, 3, 2 | Act_Identity |              | \n",i+nin+nhid,i); else fprintf(out,
"%4d|          |          |  0.00000 |  0.00000 | o  | %2d, 3, 2 |              |              | \n",i+nin+nhid,i);
 fprintf(out,
"----|----------|----------|----------|----------|----|----------|--------------|--------------|-------\n\n");
 fprintf(out,"connection definition section :\n\n");
 fprintf(out,"target | site | source:weight\n");
 fprintf(out,
"-------|------|----------------------------------------------------------------------------------------------------------------\n");
  if (out2hid=='y')
  for (j=1;j<=nhid;j++){
   for (i=1;i<=(nin+nhid+nout);i++) {
     if (i==1)
       fprintf(out,"  %4d |      |",j+nin);
     else
       if ( ( (i-1) % 8) == 0) fprintf(out,"\n               ");
     fprintf(out," %3d:%8.5f",i,drand48());
     if (i < (nin+nhid+nout)) fprintf(out,",");
   }
   fprintf(out,"\n");
 }
 else
 for (j=1;j<=nhid;j++){
   for (i=1;i<=(nin+nhid);i++) {
     if (i==1)
       fprintf(out,"  %4d |      |",j+nin);
     else
       if ( ( (i-1) % 8) == 0) fprintf(out,"\n               ");
     fprintf(out," %3d:%8.5f",i,drand48());
     if (i < (nin+nhid)) fprintf(out,",");
   }
   fprintf(out,"\n");
 }

 for (j=1;j<=nout;j++){
   for (i=1;i<=(nhid+last_output_source);i++) {
     if (i==1)
       fprintf(out,"  %4d |      |",j+nin+nhid);
     else
       if ( ( (i-1) % 8) == 0) fprintf(out,"\n               ");
     fprintf(out," %3d:%8.5f",i+nin,drand48());
     if (i < (nhid+last_output_source)) fprintf(out,",");
   }
   fprintf(out,"\n");
 }
 fprintf(out,
"-------|------|----------------------------------------------------------------------------------------------------------------\n");
 fclose(out);
 printf("\ngenerated %s\n",netname);
}


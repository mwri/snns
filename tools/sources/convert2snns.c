/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/convert2snns.c,v $
  SHORTNAME      : convert2snns.c
  SNNS VERSION   : 4.2

  PURPOSE        : Network Generator for n-Component Kohonen Networks
  NOTES          :

  AUTHOR         : Marc Seemann & Marcus Ritt 
  DATE           : 24.2.92
  VERSION        : 2.0
  UPDATE         : 13.7.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:04 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*  SNNS-Kernel constants and data type definitions  */
#include "glob_typ.h"
/*  SNNS-Kernel User-Interface Function Prototypes   */
#include "kr_ui.h"

/*########################################################################
    PROTOTYPE - Section
#########################################################################*/

void    errChk (int);
void    quit   (void);
void    control(FILE *);        
void    create_network(char *);
void    create_patterns(char *,int);

/*########################################################################
    global definitions  
#########################################################################*/

/* Learning function name */
#define KOHONEN_LEARN_FUNC_NAME     "Kohonen"

/* Update function name */
#define KOHONEN_UPDATE_FUNC_NAME    "Kohonen_Order"

/* Init functionname */
#define KOHONEN_INIT_FUNC_NAME      "Kohonen_Weights_v3.2"

char   *control_file,pattern_file[80],weight_file[80],
       string[80],name[50];
int    no_of_exemplars,X,Y,
       ret, i, j, unit_no,
       IUnits, OUnits, HUnits;

float  initialize_params[5];
struct PosType    unit_pos;

char keywords[][20] = { "xsize", "ysize", "components", "patterns", "weightfile", "patternfile", "decrease", "height", "radius" };
#define NONE -1
#define XSIZE 0
#define YSIZE 1
#define COMPONENTS 2
#define PATTERNS 3
#define WEIGHTFILE 4
#define PATTERNFILE 5
#define DECREASE 6
#define HEIGHT 7
#define RADIUS 8
#define NKEYW 8           /* Number of Keywords */
  
/*########################################################################
    FUNCTION - Section
#########################################################################*/

/*****************************************************************************
  FUNCTION : errChk

  PURPOSE  : Check whether an error occured during a process
  NOTES    :

  UPDATE   : june 15 1993
******************************************************************************/

void    errChk(int errNo )
{
  if (errNo != 0)
    {
    printf( "%s\n", krui_error( errNo ));
    exit( 1 );
    }
} /* end of errChk */

/*****************************************************************************
  FUNCTION : control

  PURPOSE  : get information from the control file
  NOTES    : 

  UPDATE   : july 13 1993
******************************************************************************/


void control(FILE *fp)
{

int   components=0, xsize=0, ysize=0, adapt_radius=0;
int   no_of_pus;
float adapt_height=0, mul=0;

char key[255];
short point;

  while (!feof(fp)) {
    fscanf(fp,"%s",key);
    for(point=0;key[point];point++) key[point]=tolower(key[point]);
    for(point=NKEYW;point>=0;point--) if (!strcmp(keywords[point],key)) break;

    switch(point) {
      case NONE       : break;
      case XSIZE      : fscanf(fp,"%d",&xsize); break;
      case YSIZE      : fscanf(fp,"%d",&ysize); break;
      case COMPONENTS : fscanf(fp,"%d",&components); break; 
      case PATTERNS   : fscanf(fp,"%d",&no_of_exemplars); break;
      case WEIGHTFILE : fscanf(fp,"%s",weight_file); break;
      case PATTERNFILE: fscanf(fp,"%s",pattern_file); break;
      case HEIGHT     :
      case DECREASE   :
      case RADIUS     : break;
    }
    fscanf(fp,"%[^\n]s",key);       /* ignores comments at end of line */
  }

  printf("\nThe File %s contains the following Datas:\n",control_file);
  printf("\nPattern File: %s ",pattern_file);
  printf("\nWeight File: %s ",weight_file);
  printf("\nX-Size: %d",xsize);
  printf("\nY-Size: %d",ysize);
  printf("\nComponents: %d",components);
  printf("\nNo. of Patterns: %d",no_of_exemplars);
  printf("\nAdaptation Height: %f",adapt_height);
  printf("\nAdaptation Radius: %d",adapt_radius);
  printf("\nDecrease Factor: %f",mul);

  no_of_pus=xsize*ysize;
  printf("\nNetwork consists of %d PUs",no_of_pus);
  fclose(fp);

  IUnits = components;
  OUnits = 0;
  X = xsize;
  Y = ysize;

  printf( "\nNo. of Components (No. of Input Units): " );
  printf( "%i", IUnits );
  printf( "\nNo. of Output Units(0 for don`t care): " );
  printf( "%i",OUnits );
  printf( "\nDimension of the Feature Map [X Y](No. of Hidden Units=X*Y): " );
  printf( "%i %i", X, Y );
  HUnits = X*Y;

} /* end of control */

/*****************************************************************************
  FUNCTION : create_network 

  PURPOSE  : create the Kohonen Network with datas from control file 
  NOTES    : If no weight file is specified the weights will be set to zero

  UPDATE   : june 15 1993
******************************************************************************/

void create_network(char *weight_file)

{
 FILE *fp;
 float val;

/*  Allocate units (the user may or may not use this function, 
      there is no need to do this)  */

       ret = krui_allocateUnits( OUnits + HUnits + IUnits );
       errChk( ret );
       printf( "\n\nCreate Units now\n" );

/*  Create standard (input) Units  */

       unit_pos.x = 1;
       for (i = 1; i <= IUnits; i++)
	 {
	   unit_no = krui_createDefaultUnit();
	   if (unit_no < 0)  errChk( unit_no );
	   ret = krui_setUnitTType( unit_no, INPUT );
	   errChk( ret );

	   unit_pos.y = (IUnits<Y)?i+(Y-IUnits)/2:i;
	   krui_setUnitPosition( unit_no, &unit_pos );
	 }

/*  Create standard (hidden) Units  */
  
       for (i = 1; i <= Y; i++)
	 for (j = 1; j <= X; j++)
	   {
	     unit_pos.x = 4+j;
	     unit_no = krui_createDefaultUnit();
	     if (unit_no < 0)  errChk( unit_no );
	     ret = krui_setUnitTType( unit_no, HIDDEN );
	     errChk( ret );

	     unit_pos.y = i;
	     krui_setUnitPosition( unit_no, &unit_pos );
	   }

/*  Create standard (output) Units  */

       unit_pos.x = 4+X+3;
       if (OUnits) for (i = 1; i <= OUnits; i++)
	 {
	   unit_no = krui_createDefaultUnit();
	   if (unit_no < 0)  errChk( unit_no );
	   ret = krui_setUnitTType( unit_no, OUTPUT );
	   errChk( ret );

	   unit_pos.y = (OUnits<Y)?i+(Y-OUnits)/2:i;
	   krui_setUnitPosition( unit_no, &unit_pos );
	 }

/* Make Connections now */
/* Make connections between hidden units and output units first !  */

       for (i = IUnits + HUnits + 1; i <= IUnits + HUnits + OUnits; i++)
	 {  /*  Make output unit to current unit  */
	   ret = krui_setCurrentUnit( i );
	   errChk( ret );

	   for (j = IUnits + 1; j <= IUnits + HUnits; j++)
	     {  /*  connect current (output) unit with hidden unit. 
                    REMEMBER: The hidden unit #j is the predecessor of
                        the (output) unit #i (it is a backward connection) */
	       ret = krui_createLink( j, 0 );
	       errChk( ret );
	     }
	 }

/* Make connections between input units and hidden units
   and set link weight with datas from output_file   */

       printf("\nSet link weights now\n");
       if((fp=fopen(weight_file,"r"))!=NULL)
        for (i = IUnits + 1; i <= IUnits + HUnits; i++)
	 {  /*  Make hidden unit to current unit  */
	   ret = krui_setCurrentUnit( i );
	   errChk( ret );

	   for (j = 1; j <= IUnits; j++)
	     { /*  (backward) connect current (hidden) unit with input unit  */
	       fscanf(fp,"%s",string);
               val = atof(string);
               ret = krui_createLink( j,val);
	       errChk( ret );
	     }
	 }
        else{ /* set all link weights to zero */
	  for (i = IUnits + 1; i <= IUnits + HUnits; i++)
	    {  /*  Make hidden unit to current unit  */
	      ret = krui_setCurrentUnit( i );
	      errChk( ret );
	      
	      for (j = 1; j <= IUnits; j++)
		{/* (backward) connect current (hidden) unit with input unit */
		  ret = krui_createLink( j,0);
		  errChk( ret );
		}
	    } 
	  
	  printf("\nWeight file %s could not be opened!\n",weight_file);
	  printf("All weights have been set to zero!\n");
	}
       fclose(fp);

 
        /*  set the update function  */
       ret = krui_setUpdateFunc (KOHONEN_UPDATE_FUNC_NAME);
       errChk( ret );
        /* set the learning function */
       ret = krui_setLearnFunc (KOHONEN_LEARN_FUNC_NAME);
       errChk( ret );
        /* set the init function */
       ret = krui_setInitialisationFunc (KOHONEN_INIT_FUNC_NAME);
       errChk( ret );


       printf("\nEnter Filename of the Network to save: ");
       scanf("%s", name);
       strcat(name,".net");
       printf("Save Network\n");
      
/*  save the network  */

       ret = krui_saveNet( name, NULL );
       errChk( ret );
       printf( "\nCreate Patterns now\n" );

} /* end of create_network */

/*****************************************************************************
  FUNCTION : create_patterns

  PURPOSE  : if a pattern file is specified patterns will be created and
             converted into the SNNS .pat format
  NOTES    : 

  UPDATE   : june 15 1993
******************************************************************************/


void create_patterns(char *pattern_file,int no_of_exemplars)
{
    FILE *fp;
    float val;
    int setNo;

    if (!strlen(pattern_file) || !no_of_exemplars) 
	return; /* shortcut of no patterns */

    /*  create patterns with datas from pattern_file */
       
    if((fp=fopen(pattern_file,"r"))!=NULL){
	ret = krui_allocNewPatternSet(&setNo);
	errChk( ret );
	for (j = 1; j <= no_of_exemplars; j++){
	    for (i = 1; i <= IUnits; i++){
		ret = krui_setCurrentUnit(i);
		errChk( ret );
		fscanf(fp,"%s",string);
		val=atof(string);
		krui_setUnitActivation(i,val);
	    }
	    ret = krui_newPattern();
	    errChk( ret );
	}
    } else {
	printf("\n Pattern file %s could not be opened!\n",pattern_file);
	exit(1);
    }
    fclose(fp);
 
    printf("\nFilename of patterns to save: ");
    scanf("%s", name);
    strcat(name,".pat");
    printf("Save Patterns\n");

    /*  save the patterns  */

    ret = krui_saveNewPatterns( name, setNo );
    errChk( ret );
} /* end of create_patterns */

/*****************************************************************************
  FUNCTION : quit 

  PURPOSE  : before exiting the Network is deleted
  NOTES    : 

  UPDATE   : june 15 1993
******************************************************************************/

void quit(void)
{
       krui_deleteNet();
       exit( 0 );

} /* end of quit */

/*****************************************************************************
  FUNCTION : main

  PURPOSE  : Network Generator for n-Component Kohonen Networks
             using a controlfile 
  NOTES    : 

  UPDATE   : june 15 1993
******************************************************************************/

void main(int argc,char **argv)
{
FILE *fp;

  printf( "\nNetwork Generator for n-Component Kohonen Networks\n" );
  control_file=argv[1];
  if(argc==2)               
    if((fp=fopen(control_file,"r"))!=NULL){
       control(fp);  /* get information from control file */
       if ((IUnits <= 0) || (OUnits < 0) || (HUnits <= 0))
	 exit( 1 );
       create_network(weight_file);
       create_patterns(pattern_file,no_of_exemplars); /* if any */
       quit();
     }
    else
      printf("\nControl file %s could not be opened!",argv[1]);
else
  printf("\nUsage: convert2snns <control file>");
  printf("\n");

} /* main */


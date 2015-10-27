/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/pat_sel_simple.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Pattern Selector
  NOTES          :

  AUTHOR         : Stefan Broeckel, Tobias Soyez 
  DATE           : 30.07.92
  VERSION        : 1.0

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.1 $
  LAST CHANGE    : $Date: 1998/05/15 13:24:51 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


/******************************************************************************/
/* included headers                                                           */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/******************************************************************************/
/* constants                                                                  */
/******************************************************************************/

#define NO_ERROR      0
#define BAD_PAT_FILE  1
#define WRITE_ERR     2
#define UNEXP_EOF     3


/******************************************************************************/
/* global variables                                                           */
/******************************************************************************/

FILE  *in_no_file   ;
FILE  *in_pat_file  ;
FILE  *out_pat_file ;


static char *patHeader[] =
{  
  "SNNS pattern definition file %s\n",
  "generated at",
  "No. of patterns     : %d\n",
  "No. of input units  : %d\n",
  "No. of output units : %d\n"
} ;



/******************************************************************************/
/* function skip_comment                                                      */
/******************************************************************************/

void skip_comment (void)
  
  {
    register int  c ;
    int           q ;

    q    = 1 ;

    while (q != 3)
    {
      c = getc (in_pat_file) ;

      switch (c)
      {
        case ' '  : break ;  
        case '\t'  : break ;  
        case '\n' : if (q == 2) q = 1 ; break ;
        case '#'  : if (q == 1) q = 2 ; break ;
        case EOF  :             q = 3 ; break ;
        default   : if (q == 1) q = 3 ; break ;
      }
    }

    ungetc (c, in_pat_file) ;
  }

 

/******************************************************************************/
/* function read_file_header                                                  */
/******************************************************************************/

int read_file_header (no_of_patterns, no_of_input_units, no_of_output_units,
                      version)

  int  *no_of_patterns     ;
  int  *no_of_input_units  ;
  int  *no_of_output_units ;
  char *version            ;

  {
    char  dstring [255] ;


    skip_comment() ;

    if (fscanf(in_pat_file, patHeader[0], version) == 1)
    { 
      if (fscanf(in_pat_file, patHeader[1]) != 0)    return (BAD_PAT_FILE) ;
      if (fgets (dstring, 254, in_pat_file) == NULL) return (BAD_PAT_FILE) ;
      skip_comment() ;
      if (fscanf(in_pat_file, patHeader[2], no_of_patterns)     != 1)
         return (BAD_PAT_FILE) ;
      if (fscanf(in_pat_file, patHeader[3], no_of_input_units)  != 1)
         return (BAD_PAT_FILE) ;
      if (fscanf(in_pat_file, patHeader[4], no_of_output_units) != 1)
         return (BAD_PAT_FILE) ;
    }
    else
    {
      sprintf (version, "%s", "V1.4-3D") ;
      if (fscanf (in_pat_file, "%d%d%d", no_of_patterns, no_of_input_units,
          no_of_output_units) != 3) return (BAD_PAT_FILE) ;
    }

    return (NO_ERROR) ;
  }



/******************************************************************************/
/* function write_file_header                                                 */
/******************************************************************************/

int write_file_header (no_of_patterns, no_of_input_units, no_of_output_units,
                       version)

  int  no_of_patterns     ;
  int  no_of_input_units  ;
  int  no_of_output_units ;
  char *version           ;


  {
    time_t  clock ;


    fprintf(out_pat_file, patHeader[0], version) ;
    fprintf(out_pat_file, patHeader[1]) ;
    (void) time (&clock) ;
    fprintf(out_pat_file, " %s\n", ctime (&clock)) ;
    fprintf(out_pat_file, patHeader[2], no_of_patterns    ) ;
    fprintf(out_pat_file, patHeader[3], no_of_input_units ) ;
    fprintf(out_pat_file, patHeader[4], no_of_output_units) ;
    fprintf(out_pat_file, "\n") ;
    return (NO_ERROR) ;
  }



/******************************************************************************/
/* main program                                                               */
/******************************************************************************/
	  
void main (int argc, char *argv[])
{ 
  float  *output_pattern    ;
  float  *input_pattern     ;
  char   version[255]       ;
  int    no_of_patterns     ;
  int    no_of_input_units  ;
  int    no_of_output_units ;
  int    no_of_sel_pat      ;
  int    pat_no             ;
  int    act_pat            ;
  int    error, i, j        ;
  

  error = NO_ERROR ;

  if (argc != 4)
  {  
    fprintf (stderr, "usage: %s <no_file> <in_pat_file> <out_pat_file>\n",
             argv[0]); 
    return ;
  }

  
  if ((in_no_file   = fopen(argv[1], "r")) == (FILE *) NULL) 
  { 
    fprintf(stderr, "error: can't read file %s\n", argv[1]) ;
    return ;
  }
  
  if ((in_pat_file  = fopen(argv[2], "r")) == (FILE *) NULL)
  { 
    fprintf(stderr, "error: can't read file %s\n", argv[2]) ;
    fclose (in_no_file) ;
    return ;
  }

  if ((out_pat_file = fopen(argv[3], "r")) != (FILE *) NULL)
  {
    fclose(out_pat_file) ;
    fprintf (stderr, "overwrite %s (y/n) ? ", argv[3]) ;
    if (getc(stdin) != 'y') return ; 
  }

  if ((out_pat_file = fopen(argv[3], "w")) == (FILE *) NULL)
  {
    fprintf(stderr, "error: can't create file %s\n", argv[3]) ;
    fclose (in_no_file)  ;
    fclose (in_pat_file) ;
    return ;
  }

  
  act_pat       = 1 ; 
  no_of_sel_pat = 0 ;

  /* number of entries in the number file */
  while (fscanf(in_no_file, "%d", &pat_no) == 1) no_of_sel_pat++ ;
  fseek (in_no_file, 0, 0) ;

  if (read_file_header  (&no_of_patterns, &no_of_input_units,
      &no_of_output_units, version) != 0) error = BAD_PAT_FILE ;
   
  if (write_file_header (no_of_sel_pat, no_of_input_units,
      no_of_output_units, version) != 0)  error = WRITE_ERR ; 

  input_pattern  = (float *) malloc(no_of_input_units  * sizeof(float)) ;
  output_pattern = (float *) malloc(no_of_output_units * sizeof(float)) ;

  /* select patterns */
  for (i = 1 ; (i <= no_of_sel_pat) && (error == NO_ERROR) ; i++)
  { 
    if (fscanf (in_no_file, "%d", &pat_no) == EOF) error = UNEXP_EOF ;
  
    /* search pattern */
    while ((act_pat != pat_no) && (error == NO_ERROR))
    {  
      for (j = 0 ; j < no_of_input_units ; j++)
      {
        skip_comment () ;
        if (fscanf (in_pat_file, "%*f") == EOF) error = UNEXP_EOF ;
      }  
      for (j = 0 ; j < no_of_output_units ; j++)
      {
        skip_comment () ;
        if (fscanf (in_pat_file, "%*f") == EOF) error = UNEXP_EOF ;
      } 
      act_pat++ ;       
    }
	
    /* read pattern */
    if (error == NO_ERROR)
    {
      for (j = 0 ; j < no_of_input_units ; j++)
      {
        skip_comment () ;
        if (fscanf (in_pat_file, "%f", &input_pattern[j]) == EOF)
            error = UNEXP_EOF ;
      }  
      for (j = 0 ; j < no_of_output_units ; j++)
      {
        skip_comment () ;
        if (fscanf (in_pat_file, "%f", &output_pattern[j]) == EOF)
            error = UNEXP_EOF ;
      }
      
      act_pat++ ;

      /* write pattern */
      if (error == NO_ERROR)
      {
        fprintf (out_pat_file, "#%d\n", i) ;

        for (j = 0 ; j < no_of_input_units ; j++)
        {
          if      (input_pattern[j] == 0.0) fprintf (out_pat_file, "0 ") ;
          else if (input_pattern[j] == 1.0) fprintf (out_pat_file, "1 ") ;
          else    fprintf (out_pat_file, "%f ", input_pattern[j]) ;

          if ((j % 10) == 9) fprintf (out_pat_file, "\n") ;
        } 
        fprintf (out_pat_file, "\n") ;
 
        for (j = 0 ; j < no_of_output_units ; j++)
        {
          if      (output_pattern[j] == 0.0) fprintf (out_pat_file, "0 ") ;
          else if (output_pattern[j] == 1.0) fprintf (out_pat_file, "1 ") ;
          else    fprintf (out_pat_file, "%f ", output_pattern[j]) ;

          if ((j % 10) == 9) fprintf (out_pat_file, "\n") ;
        } 
        fprintf (out_pat_file, "\n") ;
      }
    }
  }

  switch (error)
  {
    case BAD_PAT_FILE : fprintf(stderr, "error: bad pattern file\n")       ;
                        break ;
    case WRITE_ERR    : fprintf(stderr, "error: write error\n")            ;
                        break ;
    case UNEXP_EOF    : fprintf(stderr, "error: unexpected end of file\n") ;
                        break ;
    default           : break ;
  }

  free (input_pattern)  ;
  free (output_pattern) ;

  fclose (in_no_file)   ;
  fclose (in_pat_file)  ;
  fclose (out_pat_file) ;
}


/******************************************************************************/
/* end of file                                                                */
/******************************************************************************/







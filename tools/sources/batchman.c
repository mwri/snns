/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/batchman.c,v $
  SHORTNAME      : batchman
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS batch interpreter main module
                   initialization, command line argument handling, main()
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.8 $
  LAST CHANGE    : $Date: 1998/03/03 16:13:18 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>                   /* for getopt() */
#endif

#include "batchman.ph"
#include "symtab.h"                   /* for st_init() */
#include "ictab.h"                    /* for run() */
#include "scan1.h"                    /* scanner 1 declarations */
#include "glob_typ.h"         /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"

/*****************************************************************************
  FUNCTION : print_help

  PURPOSE  : prints helpful information to stdout
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void print_help(void)
{
  printf("This is batchman, the SNNS Batch Interpreter.\n");
  printf("Type Ctrl-D in interactive mode to start the interpreter.\n");
  printf("Options:\n");
  printf("-f <file> : read input from <file>\n");
  printf("-l <file> : (logfile) write output to <file>\n");
  printf("-p        : parse-only; just check the syntax, do not execute\n");
  printf("-q        : quiet mode; neither warnings nor errors are printed\n");
  printf("-s        : suppress warnings\n");
  printf("-h        : print this help\n");
  printf("\n");
  exit(0);
}


/*****************************************************************************
  FUNCTION : read_cmdline

  PURPOSE  : reads the command line
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void read_cmdline(int argc, char **argv)
{
  int option;
  extern char *optarg;
  extern int opterr, optind; 

  opterr = 0;      /* don't let getopt print errors */

  input_is_file = 0;
  message_flag = 0;
  warn_flag = 0;

  while ((option = getopt(argc, argv, "f:l:pqsh")) != -1)
  switch(option)
  {
    case 'f': input_file_name = optarg; input_is_file = 1; break;
    case 'l': log_file_name = optarg; break;
    case 'p': parse_only_flag = 1; break;
    case 'q': message_flag = 1; break;
    case 's': warn_flag = 1; break;
    case 'h': print_help(); break;
    case '?': fprintf(stderr,"*** Batchman error: illegal option\n");
              exit(1);
  }
  
  if (optind != argc) 
  {
    fprintf(stderr, 
	    "*** Batchman error: unexpected command line argument(s)\n");
    exit(1);
  }
}


/*****************************************************************************
  FUNCTION : open_files

  PURPOSE  : opens input and log files, if names are given on command line
  RETURNS  :
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void open_files(void)
{
  /* open log file, if a name is given: */
 if (log_file_name == NULL) log_file = stdout;
 else 
 if (!(log_file = fopen(log_file_name, "w")))
 {
   fprintf(stderr,
	   "*** Batchman error: opening file %s failed\n", log_file_name);
   exit(1);
 }

 /* use stdin as input, if no filename is given: */
 if (input_file_name == NULL)
 {
   printf("SNNS Batch Interpreter V1.0. Type batchman -h for help.\n");
   printf("No input file specified, reading input from stdin.\n");
   printf("batchman> ");
 }
 else
 /* redirect scanner input to input file: */
 if (!(yyzin = fopen(input_file_name, "r")))
 {
   fprintf(stderr,
	   "*** Batchman error: opening file %s failed\n", input_file_name);
   exit(1);
 }
}


/*****************************************************************************
  FUNCTION : main

  PURPOSE  : reads the command line, starts parsing and runs 
             the interpreter loop
  RETURNS  : exit value 0 on success, 1 or more on errors
  NOTES    : 

  UPDATE   : 
******************************************************************************/
int main(int argc, char **argv)
{
  yyzdebug = 0;                /* set parser debug mode to off */

  read_cmdline(argc, argv);     /* have a look at the command line */
  open_files();                 /* open some files */
  
  st_init();                    /* Initialize symbol table ... */
  ictab_init();                 /* Initialize intermediate code table ... */

  signal_init();                /* Initialize the signal handler */

  yyzparse();                   /* do the parse ... */
  if (!parse_only_flag) run();  /* and run the interpreter loop. */

  return 0;
}


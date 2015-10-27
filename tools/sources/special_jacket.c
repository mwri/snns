/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/special_jacket.c,v $
  SHORTNAME      : special_jacket
  SNNS VERSION   : 4.2

  PURPOSE        : Intermediate Code (IC) functions for the SNNS batch 
                   interpreter:
                   Special jacket functions
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.7 $
  LAST CHANGE    : $Date: 1998/05/01 11:21:47 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>       /* for WEXITSTATUS */

#include "symtab.h"         /* for arglist_type */
#include "special_jacket.ph"
#include "batchman.h"       /* for file pointer to log file */
#include "glob_typ.h"       /* SNNS-Kernel: Global Datatypes and Constants */
#include "ictab.h"          /* File required by following includefile */
#include "error.h"
#include "scan2.h"

extern void yyyrestart(FILE *);

/*****************************************************************************
  FUNCTION : print

  PURPOSE  : prints the values of its arguments;
             if none given, a carriage return
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void print(arglist_type *arglist)
{
  Val_type val;
  Data_type type;

  while (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    switch(type)
    {
      case INT: fprintf(log_file,"%d", val.int_val); break;
      case REAL: fprintf(log_file,"%g", val.real_val); break;
      case BOOL: fprintf(log_file,"%s", (val.bool_val?"TRUE":"FALSE")); break;
      case STRING: fprintf(log_file,"%s", val.string_val); break;
      case UNKNOWN: fprintf(log_file,"<undef>"); break;
      default: err_prt("Cannot print this");
    }
    arglist = arglist->next;
  }
  fprintf(log_file,"\n");
  fflush(log_file);
}


/*****************************************************************************
  FUNCTION : execute

  PURPOSE  : executes a shell command by passing the string it gets as
             argument to the /bin/sh shell

	     The standard output of the shell command is read in and
	     scanned. It is expected to consist of one line of constants,
	     separated by blanks. The constants may be of one of the 
	     batchman types. Their values are stored in the variables 
	     supplied as further arguments to the execute() command.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void execute(arglist_type *arglist)
{
  Val_type val;
  Data_type type, tok_typ;
  FILE *stream;
  int tmp;

  if (arglist == ARG_NULL)
  err_prt("Shell command line string missing");
  
  /* get first parameter, should be a string: */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);

  /* stop signal handling during external job execution */
  /* resume after all results have been read */
  signal_suspend();

  /* execute the shell process ... */
  stream = popen(val.string_val, "r");

  /* ... see whether popen worked ... */
  if (stream == NULL) err_prt("Execution of shell command failed");

  /* ... and redirect the result scanner input to it's output stream: */
  yyyin = stream;
  yyyrestart(yyyin);

  /* advance argument list pointer: */
  arglist = arglist->next;

  /* scan the command's output line as long as there are symbols on it
     and as long as there are variables in execute()'s argument list: */
  while (((tok_typ = yyylex()) != 0) && (arglist != ARG_NULL))
  /* scanner returns 0 on end of line */
  {
    if (st_get_ro(arglist->arg_ptr))
    err_prt("Constants are not allowed as parameters here");

    switch(tok_typ)
    {
      case INT:
      /* insert value in ST for each output symbol: */
      st_set_val_type(arglist->arg_ptr, INT, yyylval);
      break;
      case REAL:
      st_set_val_type(arglist->arg_ptr, REAL, yyylval);
      break;
      case BOOL:
      st_set_val_type(arglist->arg_ptr, BOOL, yyylval);
      break;
      case STRING:
      st_set_val_type(arglist->arg_ptr, STRING, yyylval);
      break;
      default: err_prt("Error parsing shell command's output");
    }
  arglist = arglist->next;
  }
  
  /* eat up remaining output from shell command (needs to be done for
     subsequent external calls) */
  while (tok_typ != 0)
      tok_typ = yyylex();

  /* close the shell output stream and update built-in variable: */
  tmp = pclose(stream);
  val.int_val = WEXITSTATUS(tmp);

  type = INT;
  st_set_val_type(st_lookup("EXIT_CODE"), type, val);

  /* resume signal handling */
  signal_resume();
}


/*****************************************************************************
  FUNCTION : xit

  PURPOSE  : exits the batch interpreter and returns it's parameter
             as exit status
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void xit(arglist_type *arglist)
{
  Val_type val;
  Data_type type;

  if (arglist != ARG_NULL) /* a parameter is given */
  {
    /* get first parameter, the exit code: */
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(INT, type);

    /* check range of argument: */
    if ((val.int_val>127) || (val.int_val<-128))
    warn_prt("Exit status out of range");

    /* don't expect any further parameters: */
    arglist = arglist->next;
    if (arglist != ARG_NULL)
    warn_prt(WRN_FMATE);
  }
  else  /* no parameter is given */
  /* preset exit status with 0: */
  val.int_val = 0;

  if(!message_flag)
  {
    fprintf(log_file,"Exiting with status %d\n", val.int_val);
    fflush(log_file);
  }

  exit (val.int_val);
}



/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/error.c,v $
  SHORTNAME      : error
  SNNS VERSION   : 4.2

  PURPOSE        : Functions to print out error messages and warnings
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.9 $
  LAST CHANGE    : $Date: 1998/05/01 11:21:43 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <stdio.h>

#ifndef NO_SIGNAL_HANDLING
#include <signal.h>
#endif /* not NO_SIGNAL_HANDLING */

#include "symtab.h"
#include "ictab.h"
#include "glob_typ.h"  /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.ph"
#include "batchman.h"  /* for warn_flag, message_flag */
#include "kr_ui.h"     /* SNNS-Kernel: User Interface Function Prototypes */

#ifndef NO_SIGNAL_HANDLING
static sigset_t stored_signal_set;
static int      signal_set_valid = 0;
#endif

/*****************************************************************************
  FUNCTION : inc_sourcelines

  PURPOSE  : increments the source line number counter
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void inc_sourcelines(void)
{
  sourceline_no++;
}


/*****************************************************************************
  FUNCTION : get_sourceline

  PURPOSE  : returns the source line number 
  RETURNS  : the source line number
  NOTES    : for errors that do occur during parsing or scanning;
             line number translation is not necessary here
  UPDATE   : 
******************************************************************************/
int get_sourceline(void)
{
  return sourceline_no;
}


/*****************************************************************************
  FUNCTION : chck_type

  PURPOSE  : checks if argument two is of expected type
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void chck_type(Data_type expected, Data_type two)
{
  if (expected != two)
  switch(expected)
  {
    case STRING: err_prt("String argument expected"); break;
    case REAL: err_prt("Real value argument expected"); break;
    case INT: err_prt("Integer value argument expected"); break;
    case BOOL: err_prt("Boolean value argument expected"); break;
    default:;
  }
}


/*****************************************************************************
  FUNCTION : err_prt

  PURPOSE  : prints an error message and exits batchman
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void err_prt(char *text)
{
  fprintf(stderr, "*** Batchman error at line %d:\n    %s\n", 
	  get_xref(), text);
  exit(1);
}


/*****************************************************************************
  FUNCTION : kernel_error

  PURPOSE  : prints SNNS-kernel errors in Batchman error format
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void kernel_error(krui_err err)
{
  if(err != KRERR_NO_ERROR)
  err_prt(krui_error(err));
}


/*****************************************************************************
  FUNCTION : warn_prt

  PURPOSE  : prints a warning
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void warn_prt(char *text)
{
  static int warn_cnt = 0;
  static int no_more_warnings = 0;

  if ((!no_more_warnings) && (!message_flag) && (!warn_flag))
  if (warn_cnt++ < MAX_WARNINGS)  /* do not print too many warnings */
  fprintf(stderr, "-> Batchman warning at line %d:\n   %s\n", 
	  get_xref(), text);
  else 
  {
    fprintf(stderr, "-> Further warnings suppressed.\n");
    no_more_warnings = 1;
  }
}


/*****************************************************************************
  FUNCTION : yyzerror

  PURPOSE  : error reporting routine required by main input parser 
             prints parse errors
  RETURNS  :
  NOTES    :

  UPDATE   : 
******************************************************************************/
void yyzerror(char *text)
{
  if (!message_flag)
  fprintf(stderr, "*** Batchman parser error at line %d:\n    %s\n", 
	  get_sourceline(), text);
  exit(1);
}


/*****************************************************************************
  FUNCTION : yyyerror

  PURPOSE  : error reporting routine required by shell output parser 
             prints parse errors
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void yyyerror(char *text)
{
  yyzerror(text);
}


/*****************************************************************************
  FUNCTION : user_exit_batchman

  PURPOSE  : handle non-fatal signals once by setting the internal 
             variable SIGNAL
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
RETSIGTYPE user_exit_batchman(int msg)
{
#ifndef NO_SIGNAL_HANDLING
    Data_type type;
    Val_type val;
    char *signame;

    /* update built-in variable: */
    val.int_val = msg;
    type = INT;
    st_set_val_type(st_lookup("SIGNAL"), type, val);

    switch (msg)
    {
      case SIGHUP:
	signame = "SIGHUP";
	break;
      case SIGINT:
	signame = "SIGINT";
	break;
      case SIGQUIT:
	signame = "SIGQUIT";
	break;
      case SIGALRM:
	signame = "SIGALRM";
	break;
      case SIGTERM:
	signame = "SIGTERM";
	break;
#ifdef SIGPWR
      case SIGPWR:
	signame = "SIGPWR";
	break;
#endif
      default:
	signame = "UNKNOWN";
	break;
    }

    fprintf(stderr,
	    "\n"
	    "Signal %s caught by batchman. Built in variable SIGNAL was set to %d.\n"
	    "Next reception of this signal will call the OS specific default handler.\n",
	    signame, msg);

    signal(msg, SIG_DFL);
#endif /* not NO_SIGNAL_HANDLING */
}

/*****************************************************************************
  FUNCTION : signal_init

  PURPOSE  : install some signal handlers which will set the value of the 
             intenal variable SIGNAL, so the user may react on a SIGINT by 
	     saving networks and cleaning up things.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void signal_init(void)
{
#ifndef NO_SIGNAL_HANDLING
    signal(SIGHUP,  user_exit_batchman);  
    signal(SIGINT,  user_exit_batchman);
    signal(SIGQUIT, user_exit_batchman);
    signal(SIGALRM, user_exit_batchman);
    signal(SIGTERM, user_exit_batchman);
#ifdef SIGPWR
    signal(SIGPWR,  user_exit_batchman);
#endif


#if 0
    /* don't know what to do with these fatal signals, so they
       are not handled yet. -> room for further improvemnts */

    signal(SIGILL,  emergency_exit_batchman);
    signal(SIGTRAP, emergency_exit_batchman);
    signal(SIGIOT,  emergency_exit_batchman);
#ifdef SIGEMT
    signal(SIGEMT,  emergency_exit_batchman);
#endif
    signal(SIGFPE,  emergency_exit_batchman);
#ifdef SIGBUS
    signal(SIGBUS,  emergency_exit_batchman);
#endif
    signal(SIGSEGV, emergency_exit_batchman);

#endif /* 0 */
#endif /* not NO_SIGNAL_HANDLING */
}

/*****************************************************************************
  FUNCTION : signal_suspend

  PURPOSE : suspend the delivery of some signals during critical
  program sections (e.g. calls to external programs)

  RETURNS : 
  NOTES :

  UPDATE   : 
******************************************************************************/
void signal_suspend(void)
{
#ifndef NO_SIGNAL_HANDLING
    sigset_t blocked_signal_set;
    
    if (signal_set_valid)
	/* already suspended, do nothing */
	return;

    sigemptyset(&blocked_signal_set);
    sigaddset(&blocked_signal_set, SIGHUP);
    sigaddset(&blocked_signal_set, SIGINT);
    sigaddset(&blocked_signal_set, SIGQUIT);
    sigaddset(&blocked_signal_set, SIGALRM);
    sigaddset(&blocked_signal_set, SIGTERM);
#ifdef SIGPWR
    sigaddset(&blocked_signal_set, SIGPWR);
#endif

    if (sigprocmask(SIG_BLOCK, &blocked_signal_set, &stored_signal_set)==0)
    {
	signal_set_valid = 1;
    }
#endif /* not NO_SIGNAL_HANDLING */
}

/*****************************************************************************
  FUNCTION : signal_resume

  PURPOSE : resume the delivery of signals after critical
  program sections (e.g. calls to external programs)

  RETURNS : 
  NOTES :

  UPDATE   : 
******************************************************************************/
void signal_resume(void)
{
#ifndef NO_SIGNAL_HANDLING
    if (!signal_set_valid)
	/* ignore if no valid signal set has been stored */
	return;

    sigprocmask(SIG_SETMASK, &stored_signal_set, NULL);
    signal_set_valid = 0;
#endif /* not NO_SIGNAL_HANDLING */
}

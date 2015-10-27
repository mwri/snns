/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/batchman.ph,v $
  SHORTNAME      : batchman
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS batch interpreter main module
                   initialization, command line argument handling, main()
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:17 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _BATCHMAN_DEFINED_
#define _BATCHMAN_DEFINED_

/* begin global definition section */

FILE *log_file;                  /* file pointer of log file */

int input_is_file;               /* global flag to notify the scanner
			            not to print batchman> every line */

int warn_flag,                   /* flag not to print warnings if set */
    message_flag;                /* flag not to print messages if set */

/* end global definition section */


/* begin private definition section */

static void print_help(void);
static void read_cmdline(int argc, char **argv);
static void open_files(void);

static char *input_file_name = NULL;  /* file name of user program */
static char *log_file_name = NULL;    /* file name of log file */

static int parse_only_flag = 0;       /* store parse-only mode */

/* end private definition section */

#endif

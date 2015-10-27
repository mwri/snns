/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/batchman.h,v $
  SHORTNAME      : batchman
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS batch interpreter main module
                   initialization, command line argument handling, main()
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:16 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

extern FILE *log_file;           /* file pointer of log file */

extern int input_is_file;        /* global flag to notify the scanner
	                            not to print batchman> every line */

extern int 
    warn_flag,                   /* flag not to print warnings if set */
    message_flag;                /* flag not to print messages if set */


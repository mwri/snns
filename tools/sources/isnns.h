/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/isnns.h,v $
  SHORTNAME      : isnns
  SNNS VERSION   : 4.2

  PURPOSE        : interactive control of SNNS
  NOTES          :

  AUTHOR         : original SNNS Version for V 3.0 by Michael Vogt
  DATE           : 06.08.93

  CHANGED BY     : Michael Vogt
  SCCS VERSION   : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:36 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _ISNNS_DEFINED_
#define  _ISNNS_DEFINED_

/* begin global definition section */

/* end global definition section */

/* begin private definition section */

#define C_UNKNOWN -1
#define C_LOAD 1
#define C_PROPAGATE 2
#define C_BACKPROP 3
#define C_LEARN 4
#define C_QUIT 5
#define C_HELP 6
#define C_SAVE 7

typedef struct
{
    char *c_string;
    int c;
} c_entry;

c_entry c_table[] = {
    { "load", C_LOAD },
    { "prop", C_PROPAGATE },
    { "train", C_BACKPROP },
    { "learn", C_LEARN },
    { "quit", C_QUIT },
    { "help", C_HELP },
    { "save", C_SAVE }	
};

static int c_table_no = 7;

/* end private definition section */

#endif 



/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/ic_snns.ph,v $
  SHORTNAME      : ic_snns
  SNNS VERSION   : 4.2

  PURPOSE        : Intermediate Code (IC) functions for the SNNS batch 
                   interpreter:
                   function calls to the SNNS-Kernel function interface

  NOTES          : Abbreviations: ST: symbol table
                                  IC: intermediate code

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.13 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:08 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _IC_SNNS_DEFINED_
#define _IC_SNNS_DEFINED_


/* begin global definition section */

void setInitFunc(arglist_type *arglist);
void setLearnFunc(arglist_type *arglist);
void setUpdateFunc(arglist_type *arglist);
void setPruningFunc(arglist_type *arglist);
void setRemapFunc(arglist_type *arglist);
void setSubPattern(arglist_type *arglist);
void setShuffle(arglist_type *arglist);
void setSubShuffle(arglist_type *arglist);
void setClassDistrib(arglist_type *arglist);
void setParallelMode(arglist_type *arglist);
void setCascadeParams(arglist_type *arglist);

void initNet(arglist_type *arglist);
void resetNet(arglist_type *arglist);
void loadNet(arglist_type *arglist);
void saveNet(arglist_type *arglist);
void saveResult(arglist_type *arglist);
void trainNet(arglist_type *arglist);
void testNet(arglist_type *arglist);
void pruneNet(arglist_type *arglist);
void pruneTrainNet(arglist_type *arglist);
void pruneNetNow(arglist_type *arglist);
void delCandUnits(arglist_type *arglist);
void setActFunc(arglist_type *arglist);
 
void loadPattern(arglist_type *arglist);
void setPattern(arglist_type *arglist);
void delPattern(arglist_type *arglist);

void setSeed(arglist_type *arglist);
void jogWeights(arglist_type *arglist);
void jogCorrWeights(arglist_type *arglist);

/* end global definition section */


/* begin private definition section */

/*****************************************************************************
  variables for storing parameters for SNNS-Kernel calls:
******************************************************************************/

static int
    noOfInitParams = 0,        /* number of init parameters */
    noOfLearnInP = 1,          /* number of learn parameters */
    noOfUpdateParam = 0,       /* number of update parameters */
    noOfRemapParam = 0;        /* number of pattern remap parameters */

static int 
    spIsize[MAX_NO_OF_VAR_DIM],/* parameters for subpattern definition */
    spIstep[MAX_NO_OF_VAR_DIM],
    spOsize[MAX_NO_OF_VAR_DIM],
    spOstep[MAX_NO_OF_VAR_DIM];

static float 
    init_param_array[NO_OF_INIT_PARAMS],
                               /* parameters for the init function */
    learn_param_array[NO_OF_LEARN_PARAMS],
                               /* parameters for the learning function */
    update_param_array[NO_OF_UPDATE_PARAMS],
                               /* parameters for the update function */
    remap_param_array[NO_OF_REMAP_PARAMS];
                               /* parameters for the remap function */
static unsigned int *distrib_array = NULL;     /* class distribution values */

static char 
    init_fct[FUNCTION_NAME_MAX_LEN],     /* init function name */
    learn_fct[FUNCTION_NAME_MAX_LEN],    /* learn function name */
    update_fct[FUNCTION_NAME_MAX_LEN],   /* update function name */
    remap_fct[FUNCTION_NAME_MAX_LEN],    /* remap function name */
    pruning_fct[FUNCTION_NAME_MAX_LEN],  /* pruning function name */
    sublearn_fct[FUNCTION_NAME_MAX_LEN]; /* subord. learn. func. name */

static Bool_type
    init_net_flag = FALSE,     /* init_param_array init'ed or not */
    init_learn_flag = FALSE,   /* learn_param_array init'ed or not */
    init_update_flag = FALSE,  /* update_param_array init'ed or not */
    init_remap_flag = FALSE,   /* remap_param_array init'ed or not */
    init_subPat_flag = FALSE;  /* subPattern arrays init'ed or not */

static char 
    *netname;                  /* SNNS name of the current network */


/*****************************************************************************
  pruning parameters

  NOTE: prune_f.h supplies its internal variables instead of the
        initialization #defines (probably the hell knows why)
	so initialization is performed in setPruningFunc()
******************************************************************************/
static float
    max_error_incr,
    accepted_error,
    min_error_to_stop,
    init_matrix_value;

static int
    first_train_cyc,
    retrain_cyc;

static Bool_type
    recreate,
    input_pruning,
    hidden_pruning;


/*****************************************************************************
  the pattern table:
  table to associate pattern file names with SNNS-kernel pattern numbers: 
  an array of ST pointers indexed by SNNS patset numbers
******************************************************************************/
static St_ptr_type 
    pat_tab[NO_OF_PAT_SETS];

static int 
    pat_sets_loaded = 0,       /* number of patternsets in memory */
    curr_patSet = 0;           /* current pattern set */


void enter_patName(St_ptr_type name);
int lookup_patName(St_ptr_type pat);
St_ptr_type lookup_patNumber(int pat);
void del_patName(int pat_no);
void print_parameters(const float *array, int number);
void defSubpattern(void);
float trainFFNet(int cycles);

/*char *tempnam(const char *dir, const char *pfx);*/

/* end private definition section */

#endif

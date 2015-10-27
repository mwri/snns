/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/ic_snns.c,v $
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
  RCS VERSION    : $Revision: 1.30 $
  LAST CHANGE    : $Date: 1998/07/08 11:13:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

/* Pointers to the IC SNNS functions defined below are inserted in the 
   IC Table and executed during intermediate code interpretation (run())
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>    /* for unlink() */
#endif
#include <time.h>

#include "symtab.h"    /* for arglist_type */
#include "glob_typ.h"  /* SNNS-Kernel: Global Datatypes and Constants */
#include "ext_typ.h"   /* SNNS MasPar Kernel: Datatypes and Constants for
                          Internal Usage (for FUNCTION_NAME_MAX_LEN) */
#include "cc_mac.h"    /* macros for cascade correlation */

#include "ic_snns.ph"
#include "batchman.h"  /* for file pointer to log file */
#include "ictab.h"     /* File required by following includefile */
#include "error.h"     /* errors and warnings */

#include "kr_ui.h"     /* SNNS-Kernel: User Interface Function Prototypes */
#include "kr_typ.h"    /* SNNS-Kernel: Datatypes and Constants for Internal 
			  Usage. File required by following includefile */
#include "kr_newpattern.h" /* SNNS-Kernel: new pattern handling functions */
#include "prun_f.h"    /* SNNS-Kernel: pruning functions and constants */



/*****************************************************************************
  Functions for pattern table handling
  Purpose: enable xgui-lookalike patternset handling

******************************************************************************/

/*****************************************************************************
  FUNCTION : enter_patName

  PURPOSE  : enters pattern name (as ST pointer) in the pattern table 
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void enter_patName(St_ptr_type name)
{
  pat_tab[pat_sets_loaded++] = name;
}


/*****************************************************************************
  FUNCTION : lookup_patName

  PURPOSE  : looks up the filename of a pattern in the pattern table 
  RETURNS  : SNNS patset number, -1 if not found
  NOTES    :

  UPDATE   : 
******************************************************************************/
int lookup_patName(St_ptr_type pat)
{
  Val_type val, tmp_val;
  Data_type type;
  int i;

  /* search for pattern name in patterntable: */
  for (i=0; i<pat_sets_loaded; i++)
  {
    /* compare name searched with pattern table entries: */
    st_get_val_type(pat, &type, &val);
    st_get_val_type(pat_tab[i], &type, &tmp_val);
    if (strcmp(val.string_val, tmp_val.string_val) == 0)
    return i;
  }
  err_prt("Pattern file not yet loaded");
  return -1;
}


/*****************************************************************************
  FUNCTION : lookup_patNumber

  PURPOSE  : looks up the SNNS pattern number of a pattern in the 
             pattern table 
  RETURNS  : ST entry of pattern name
  NOTES    :

  UPDATE   : 
******************************************************************************/
St_ptr_type lookup_patNumber(int pat)
{
  return pat_tab[pat];
}


/*****************************************************************************
  FUNCTION : del_patName

  PURPOSE  : deletes a pattern set entry from the pattern table
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void del_patName(int pat_no)
{
  int i;

  for (i=pat_no; i<NO_OF_PAT_SETS - 1; i++)
  pat_tab[i] = pat_tab[i+1];

  if (curr_patSet == pat_no)
  curr_patSet = 0;
  else
  if (curr_patSet > pat_no) 
  curr_patSet--;

  pat_sets_loaded--;
}


/*****************************************************************************
  other internal functions

******************************************************************************/

/*****************************************************************************
  FUNCTION : print_parameters

  PURPOSE  : prints number elements from array to the logfile
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void print_parameters(const float *array, int number)
{
  int j;

  if (!number) return;

  fprintf(log_file, "#  Parameters are: ");
  for (j=0; j<number; j++) fprintf(log_file, "%g ", array[j]);
  fprintf(log_file, "\n");
  fflush(log_file);
}


/*****************************************************************************
  FUNCTION : defSubpattern

  PURPOSE  : defines subpattern shifting scheme
             writes the 4 subpattern parameter arrays in the kernel
	     arrays are initialized if not previously set by the
	     user with the setSubPattern() function
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void defSubpattern(void)
{
  krui_err kr_err;
  int i;

  /* initialize the 4 subpattern parameter arrays, if not already done: */
  if(! init_subPat_flag)
  {  
    for(i=0; i<MAX_NO_OF_VAR_DIM; i++)
    {
      spIsize[i] = 0; spIstep[i] = 0;
      spOsize[i] = 0; spOstep[i] = 0;
    }
    init_subPat_flag = TRUE;
  }

  kr_err = krui_DefTrainSubPat(spIsize, spOsize, spIstep, spOstep, NULL);
  kernel_error(kr_err);
}


/*****************************************************************************
  FUNCTION : trainFFNet

  PURPOSE  : trains a net using a subordinate learning function
  RETURNS  : summed squared error
  NOTES    : function taken from snnsbat.c with minor changes

  UPDATE   : 
******************************************************************************/
float trainFFNet(int cycles)
{
  krui_err kr_err;
  float *return_values;
  int NoOfOutParams;

  int cycle = 0;

  defSubpattern();

  while ((cycle < cycles) && 
	 ((cycle && (return_values [0] > min_error_to_stop)) || !cycle))
  {
    kr_err = krui_learnAllPatternsFF(learn_param_array, noOfLearnInP,
				     &return_values, &NoOfOutParams);
    kernel_error(kr_err);
    
    cycle++;
  }

  return (return_values[0]);
}



/*****************************************************************************
  Functions that can be called from the user program
  'arguments' refers to function arguments in the user program,
  here pointed to by the argument pointer list

******************************************************************************/

/*****************************************************************************
  FUNCTION : setInitFunc

  PURPOSE  : sets the initialization function and its parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : init function name is mandatory

  UPDATE   : 
******************************************************************************/
void setInitFunc(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  int i;

  if (arglist == ARG_NULL)
  err_prt(ERR_ALOAE);
  
  /* get first parameter, should be an valid init function name: */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);
  if (! krui_isFunction(val.string_val, INIT_FUNC))
  err_prt("Unknown init function name");
  strcpy(init_fct, val.string_val);

  /* advance argument list pointer: */
  arglist = arglist->next;

  /* initialize init parameter array:  */
  for(i=0; i<NO_OF_INIT_PARAMS; i++) {
      if(i == 0)
	  init_param_array[i] = 1.0;
      else if(i == 1)
	  init_param_array[i] = -1.0;
      else
	  init_param_array[i] = 0.0;
  }


  /* overwrite parameter array with supplied parameters: */
  i = 0;
  while ( (i < NO_OF_INIT_PARAMS) && arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	init_param_array[i] = val.real_val;
    else if(type == INT)
	init_param_array[i] = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    arglist = arglist->next;
    i++;
  }

  /* set number of actual init parameters: */
  noOfInitParams = i;

  kr_err = krui_setInitialisationFunc(init_fct);
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file, "#  Init function is now %s\n", init_fct);
    print_parameters(init_param_array, noOfInitParams);
    fflush(log_file);
  }

  if (arglist != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* to enable to warn the user, set an init flag: */
  init_net_flag = TRUE;
}


/*****************************************************************************
  FUNCTION : setLearnFunc

  PURPOSE  : sets the learning function and its parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : learning function name is mandatory

  UPDATE   : 
******************************************************************************/
void setLearnFunc(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  int i;

  if (arglist == ARG_NULL)
  err_prt(ERR_ALOAE);
  
  /* get first parameter, should be a learning function name: */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);
  if (! krui_isFunction(val.string_val, LEARN_FUNC))
  err_prt("Unknown learning function name");
  strcpy(learn_fct, val.string_val);

  /* advance argument list pointer: */
  arglist = arglist->next;

  /* initialize learn parameter array: */
  learn_param_array[0] = 0.2;
  for(i=1; i<NO_OF_LEARN_PARAMS; i++) learn_param_array[i] = 0.0;

  /* overwrite parameter array with supplied parameters: */
  i = 0;
  while ((i<NO_OF_LEARN_PARAMS) && arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	learn_param_array[i] = val.real_val;
    else if(type == INT)
	learn_param_array[i] = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    arglist = arglist->next;
    i++;
  }

  /* set number of actual learn parameters; at least 1: */
  noOfLearnInP = (i==0) ? 1 : i;

  kr_err = krui_setLearnFunc(learn_fct);
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Learning function is now %s\n", learn_fct);
    print_parameters(learn_param_array, noOfLearnInP);
    fflush(log_file);
  }

  if (arglist != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* the learn_param_array[] should be initialized at least once
     before calling the trainNet-function;
     remember here that this is now done: */
  init_learn_flag = TRUE;
}


/*****************************************************************************
  FUNCTION : setUpdateFunc

  PURPOSE  : sets the update function and its parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : update function name is mandatory

  UPDATE   : 
******************************************************************************/
void setUpdateFunc(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  int i;

  if (arglist == ARG_NULL)
  err_prt(ERR_ALOAE);
  
  /* get first parameter, should be an update function name: */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);
  if (! krui_isFunction(val.string_val, UPDATE_FUNC))
  err_prt("Unknown update function name");
  strcpy(update_fct, val.string_val);

  /* advance argument list pointer: */
  arglist = arglist->next;

  /* initialize update parameter array:  */
  for(i=0; i<NO_OF_UPDATE_PARAMS; i++) update_param_array[i] = 0.0;

  /* overwrite parameter array with supplied parameters: */
  i = 0;
  while ((i < NO_OF_UPDATE_PARAMS) && arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	update_param_array[i] = val.real_val;
    else if(type == INT)
	update_param_array[i] = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    arglist = arglist->next;
    i++;
  }
  
  /* set number of actual update parameters: */
  noOfUpdateParam = i;

  kr_err = krui_setUpdateFunc(update_fct);
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Update function is now %s\n", update_fct);
    print_parameters(update_param_array, noOfUpdateParam);
    fflush(log_file);
  }

  if (arglist != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* remember, update_param_array is now initialized: */
  init_update_flag = TRUE;
}


/*****************************************************************************
  FUNCTION : setPruningFunc

  PURPOSE  : sets the pruning functions and their parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : pruning and subordinate learning function names are mandatory

  UPDATE   : 
******************************************************************************/
void setPruningFunc(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  /* initialize pruning parameters: */
  max_error_incr = pr_maxErrorInc,
  accepted_error = pr_acceptedError,
/*  min_error_to_stop = pr_minError,*/
  init_matrix_value = pr_obs_initParameter;
  
  first_train_cyc = pr_trainCycles,
  retrain_cyc = pr_retrainCycles;
  
  recreate = pr_recreate,
  input_pruning = pr_inputPruning,
  hidden_pruning = pr_hiddenPruning;

  /* get mandatory parameter: pruning function */
  if (arglist == ARG_NULL)
  err_prt("Missing pruning function name");

  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);
  if (! krui_isFunction(val.string_val, PRUNING_FUNC))
  err_prt("Unknown pruning function name");
  strcpy(pruning_fct, val.string_val);
  arglist = arglist->next;

  kr_err = krui_setPrunFunc(pruning_fct);
  kernel_error(kr_err);

  /* get mandatory parameter: subordinate learning function */
  if (arglist == ARG_NULL)
  err_prt("Missing subordinate learning function name");

  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);
  if (! krui_isFunction(val.string_val, LEARN_FUNC))
  err_prt("Unknown learning function name");
  strcpy(sublearn_fct, val.string_val);
  arglist = arglist->next;

  kr_err = krui_setFFLearnFunc(sublearn_fct);
  kernel_error(kr_err);

  if(!message_flag)
  {  
    fprintf(log_file, "#  Pruning function is now %s\n", pruning_fct);
    fprintf(log_file, "#  Subordinate learning function is now %s\n", 
	    sublearn_fct);
    if (arglist != ARG_NULL) fprintf(log_file, "#  Parameters are: ");
    fflush(log_file);
  }

  /* get further parameters, if supplied, and overwrite the 
     corresponding defaults: */

  /* get parameter: maximum error increase */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	max_error_incr = val.real_val;
    else if(type == INT)
	max_error_incr = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%g ", max_error_incr);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: accepted error */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	accepted_error = val.real_val;
    else if(type == INT)
	accepted_error = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%g ", accepted_error);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: recreate flag */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(BOOL, type);
    recreate = val.bool_val;
    if(!message_flag) {
	fprintf(log_file, "%s ", val.bool_val ? "TRUE" : "FALSE");
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: first train cycles */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == INT)
        first_train_cyc = val.int_val;
    else if(type == REAL)
        first_train_cyc = (int)val.real_val;
    else
        err_prt("Integer value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%d ", first_train_cyc);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: retrain cycles */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == INT)
        retrain_cyc = val.int_val;
    else if(type == REAL)
        retrain_cyc = (int)val.real_val;
    else
        err_prt("Integer value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%d ", retrain_cyc);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: min error to stop*/
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	min_error_to_stop = val.real_val;
    else if(type == INT)
	min_error_to_stop = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%g ", min_error_to_stop);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: matrix init value */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	init_matrix_value = val.real_val;
    else if(type == INT)
	init_matrix_value = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%g ", init_matrix_value);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: input pruning flag */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(BOOL, type);
    input_pruning = val.bool_val;
    if(!message_flag) {
	fprintf(log_file, "%s ", val.bool_val ? "TRUE" : "FALSE");
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: hidden pruning flag */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(BOOL, type);
    hidden_pruning = val.bool_val;
    if(!message_flag) {
	fprintf(log_file, "%s ", val.bool_val ? "TRUE" : "FALSE");
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  if (!message_flag)
  {
    fprintf(log_file, "\n");
    fflush(log_file);
  }

  /* update pruning parameters (may have been changed): */
  pr_obs_setInitParameter((double)init_matrix_value);
  pr_setInputPruning((int)input_pruning);
  pr_setHiddenPruning((int)hidden_pruning);

  if (arglist != ARG_NULL)
  warn_prt(WRN_FMATE);
}


/*****************************************************************************
  FUNCTION : setRemapFunc

  PURPOSE  : sets the pattern remapping function and its parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : remap function name is mandatory

  UPDATE   : 
******************************************************************************/
void setRemapFunc(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  int i;

  if (arglist == ARG_NULL)
  err_prt(ERR_ALOAE);
  
  /* get first parameter, should be an remap function name: */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);
  if (! krui_isFunction(val.string_val, REMAP_FUNC))
  err_prt("Unknown remap function name");
  strcpy(remap_fct, val.string_val);

  /* advance argument list pointer: */
  arglist = arglist->next;

  /* initialize remap parameter array:  */
  for(i=0; i<NO_OF_REMAP_PARAMS; i++) remap_param_array[i] = 0.0;

  /* overwrite parameter array with supplied parameters: */
  i = 0;
  while ((i < NO_OF_REMAP_PARAMS) && arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	remap_param_array[i] = val.real_val;
    else if(type == INT)
	remap_param_array[i] = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    arglist = arglist->next;
    i++;
  }
  
  /* set number of actual remap parameters: */
  noOfRemapParam = i;

  kr_err = krui_setRemapFunc(remap_fct,remap_param_array);
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Remap function is now %s\n", remap_fct);
    print_parameters(remap_param_array, noOfRemapParam);
    fflush(log_file);
  }

  if (arglist != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* remember, remap_param_array is now initialized: */
  init_remap_flag = TRUE;
}


/*****************************************************************************
  FUNCTION : setSubPattern

  PURPOSE  : sets the subpattern handling parameters

  RETURNS  :
  NOTES    : kernel call is performed in defSubpattern()

  UPDATE   : 
******************************************************************************/
void setSubPattern(arglist_type *arglist)
{
  Val_type val;
  Data_type type;
  int i;

  if (arglist == ARG_NULL)
  err_prt("At least four arguments expected");

  /* initialize the 4 subpattern parameter arrays: */
  for(i=0; i<MAX_NO_OF_VAR_DIM; i++)
  {
    spIsize[i] = 0; spIstep[i] = 0;
    spOsize[i] = 0; spOstep[i] = 0;
  }

  if(!message_flag)
  {  
    fprintf(log_file, "#  Subpattern shifting scheme (re)defined\n");
    fprintf(log_file, "#  Parameters are: ");
    fflush(log_file);
  }

  /* overwrite parameter arrays with supplied parameters: */
  i = 0;
  while ( (i < MAX_NO_OF_VAR_DIM) && arglist != ARG_NULL)
  {
    /* get subpatternIsize: */
    if (arglist == ARG_NULL)    /* no more arguments in list? */
    err_prt(ERR_NOAMM);         /* print an error */
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == INT)
        spIsize[i] = val.int_val;
    else if(type == REAL)
        spIsize[i] = (int)val.real_val;
    else
        err_prt("Integer value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%d ", spIsize[i]);
	fflush(log_file);
    }
    arglist = arglist->next;    /* advance argument list pointer */

    /* get subpatternIstep: */
    if (arglist == ARG_NULL)
    err_prt(ERR_NOAMM);
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == INT)
        spIstep[i] = val.int_val;
    else if(type == REAL)
        spIstep[i] = (int)val.real_val;
    else
        err_prt("Integer value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%d ", spIstep[i]);
	fflush(log_file);
    }
    arglist = arglist->next;

    /* get subpatternOsize: */
    if (arglist == ARG_NULL)
    err_prt(ERR_NOAMM);
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == INT)
        spOsize[i] = val.int_val;
    else if(type == REAL)
        spOsize[i] = (int)val.real_val;
    else
        err_prt("Integer value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%d ", spOsize[i]);
	fflush(log_file);
    }
    arglist = arglist->next;

    /* and, last not least, get subpatternOstep: */
    if (arglist == ARG_NULL)
    err_prt(ERR_NOAMM);
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == INT)
        spOstep[i] = val.int_val;
    else if(type == REAL)
        spOstep[i] = (int)val.real_val;
    else
        err_prt("Integer value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%d ", spOstep[i]);
	fflush(log_file);
    }
    arglist = arglist->next;

    i++;

    /* oops, so far so good; but this was only one dimension! */
  }

  if (arglist != ARG_NULL)
  err_prt(ERR_NOAMM);

  /* notify defSubpattern() that the user program has set the params: */
  init_subPat_flag = TRUE;

  /* kernel call is performed in defSubpattern() */

  if(!message_flag)
  {
    fprintf(log_file, "\n");
    fflush(log_file);
  }
}


/*****************************************************************************
  FUNCTION : setShuffle

  PURPOSE  : sets the pattern shuffling mode for ordinary patterns
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void setShuffle(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  if (arglist == ARG_NULL)
  err_prt(ERR_ATOFE);

  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(BOOL, type);

  kr_err = krui_shufflePatterns((bool)val.bool_val);
  /* formal type cast; SNNS and Batchman boolean types are compatible */
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Pattern shuffling %sabled\n", 
	    (val.bool_val) ? "en" : "dis");
    fflush(log_file);
  }

  if (arglist->next != ARG_NULL)
  warn_prt(WRN_FMATE);
}


/*****************************************************************************
  FUNCTION : setSubShuffle

  PURPOSE  : sets the pattern shuffling mode for subpatterns
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void setSubShuffle(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  if (arglist == ARG_NULL)
  err_prt(ERR_ATOFE);

  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(BOOL, type);

  kr_err = krui_shuffleSubPatterns((bool)val.bool_val);
  /* formal type cast; SNNS and Batchman boolean types are compatible */
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Subpattern shuffling %sabled\n", 
	    (val.bool_val) ? "en" : "dis");
    fflush(log_file);
  }

  if (arglist->next != ARG_NULL)
  warn_prt(WRN_FMATE);
}


/*****************************************************************************
  FUNCTION : setClassDistrib

  PURPOSE  : sets the distribution of the classes in the pattern file
             to the values given as parameters. 
  RETURNS  :
  NOTES    : first parameter (bool) switches redistribution on or off
             Also sets the value of PAT

  UPDATE   : 
******************************************************************************/
void setClassDistrib(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  int i,j;
  bool onOff;
  pattern_set_info   patt_info;
  pattern_descriptor descrip;
  char s_onOff[5];
  float *outarray;

  if (arglist == ARG_NULL)
  err_prt(ERR_ALOAE);
  
  /* get first parameter*/
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(BOOL, type);
  onOff = (bool)val.bool_val;
  kr_err = krui_useClassDistribution(onOff);
  kernel_error(kr_err);

  /* advance argument list pointer: */
  arglist = arglist->next;

  /* get pattern information */
  kr_err = krui_GetPatInfo(&patt_info, &descrip);
  kernel_error(kr_err);

  /* initialize class distribution array:  */
  distrib_array = (unsigned int *)realloc(distrib_array, 
					  patt_info.classes*sizeof(unsigned int));
  for(i=0; i<patt_info.classes; i++) 
      distrib_array[i] = *patt_info.class_redistribution++;

  /* overwrite distribution with supplied values: */
  i = 0;
  while ((i < patt_info.classes) && arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	distrib_array[i] = (int) val.real_val;
    else if(type == INT)
	distrib_array[i] = val.int_val;
    else
	err_prt("Real or INT value argument expected");
    arglist = arglist->next;
    i++;
  }
  
  kr_err = krui_setClassDistribution(distrib_array);
  kernel_error(kr_err);

  if(!message_flag)
  {
    outarray = (float *)malloc(i*sizeof(float));
    for(j=0;j<i;j++)outarray[j] = (float)distrib_array[j];
    sprintf(s_onOff,"%s",(onOff)?" ON":" OFF");
    fprintf(log_file,"#  Class distribution is now %s\n", s_onOff);
    print_parameters( outarray, i);
    fflush(log_file);
  }

  if (arglist != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* update built-in variable: */
  val.int_val = krui_getNoOfPatterns();
  type = INT;
  st_set_val_type(st_lookup("PAT"), type, val);
}


/*****************************************************************************
  FUNCTION : setParallelMode

  PURPOSE  : switches to the parallel kernel if argument is TRUE and back
             if argument is FALSE
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void setParallelMode(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  if (arglist == ARG_NULL)
  err_prt(ERR_ATOFE);

  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(BOOL, type);

  kr_err = krui_setSpecialNetworkType(
	    (val.bool_val==TRUE) ? NET_TYPE_FF1 : NET_TYPE_GENERAL);

  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Parallel Mode %sabled\n", 
	    (val.bool_val) ? "en" : "dis");
    fflush(log_file);
  }

  if (arglist->next != ARG_NULL)
  warn_prt(WRN_FMATE);
}


/*****************************************************************************
  FUNCTION : setCascadeParams

  PURPOSE  : sets the parameters of the cascade correlation algorithms
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void setCascadeParams(arglist_type *arglist)
{
  Val_type val;
  Data_type type;
  int i;

  /* initialization of learn_param_array: */

  /* XGUI Cascade subwindow 'Global Parameters' entries: */
  learn_param_array[6] = MAX_PIXEL_ERROR;
  learn_param_array[7] = QUICKPROP;       /* QUICKPROP = Quickprop
					     BACKPROP = Backprop
					     RPROP = Rprop */
  learn_param_array[8] = ON;
  learn_param_array[17] = OFF;
  learn_param_array[20] = SBC;            /* SBC AIC CMSEP */


  /* XGUI Cascade subwindow 'Candidate Parameters' entries: */
  learn_param_array[9] = MIN_COVARIANCE_CHANGE;
  learn_param_array[10] = SPECIAL_PATIENCE;
  learn_param_array[11] = MAX_NO_OF_COVARIANCE_UPDATE_CYCLES;
  learn_param_array[12] = MAX_SPECIAL_UNIT_NO;
  learn_param_array[13] = SYM_SIGMOID;    /* ASYM_SIGMOID = Act_Logistic
					     SYM_SIGMOID=Act_LogSym
					     TANH = Act_TanH
					     RANDOM = Act_Random */


  /* XGUI Cascade subwindow 'Output Parameters' entries: */
  learn_param_array[14] = MIN_ERROR_CHANGE;
  learn_param_array[15] = OUT_PATIENCE;
  learn_param_array[16] = MAX_NO_OF_ERROR_UPDATE_CYCLES;


  /* unused: */
  learn_param_array[18] = 0;
  learn_param_array[19] = 0;


  /* reading parameters, if supplied:*/

  if ((!message_flag) && (arglist != ARG_NULL))
  {  
    fprintf(log_file, "#  Cascade Correlation\n");
    fprintf(log_file, "#  Parameters are: ");
    fflush(log_file);
  }

  /* XGUI Cascade subwindow 'Global Parameters' entries: */

  /* get parameter: max outp. unit error */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	learn_param_array[6] = val.real_val;
    else if(type == INT)
	learn_param_array[6] = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%g ", learn_param_array[6]);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: learning function */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(STRING, type);
    if (strcmp(val.string_val, "Quickprop") == 0)
    learn_param_array[7] = QUICKPROP;
    else
    if (strcmp(val.string_val, "Backprop") == 0)
    learn_param_array[7] = BACKPROP;
    else
    if (strcmp(val.string_val, "Rprop") == 0)
    learn_param_array[7] = RPROP;
    else 
    err_prt("Invalid learning function name");
    
    if(!message_flag) {
	fprintf(log_file, "%s ", val.string_val);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: print covar. and error */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(BOOL, type);
    learn_param_array[8] = (val.bool_val) ? ON : OFF;
    if(!message_flag) { 
	fprintf(log_file, "%s ", val.bool_val ? "TRUE" : "FALSE");
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: prune new hidden unit */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(BOOL, type);
    learn_param_array[17] = (val.bool_val) ? ON : OFF;
    if(!message_flag) { 
	fprintf(log_file, "%s ", val.bool_val ? "TRUE" : "FALSE");
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: minimize */   /* SBC AIC CMSEP */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(STRING, type);
    if (strcmp(val.string_val, "SBC") == 0)
    learn_param_array[20] = SBC;
    else
    if (strcmp(val.string_val, "AIC") == 0)
    learn_param_array[20] = AIC;
    else
    if (strcmp(val.string_val, "CMSEP") == 0)
    learn_param_array[20] = CMSEP;
    else 
    err_prt("Invalid minimization function name");
    
    if(!message_flag) {
	fprintf(log_file, "%s ", val.string_val);
	fflush(log_file);
    }

    arglist = arglist->next;
  }

  /* XGUI Cascade subwindow 'Candidate Parameters' entries: */

  /* get parameter: min. covar. change */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	learn_param_array[9] = val.real_val;
    else if(type == INT)
	learn_param_array[9] = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%g ", learn_param_array[9]);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: cand. patience */
  /* get parameter: max no of covar. updates */
  /* get parameter: max no of candidate units */
  for(i=10; i<=12; i++){
      if (arglist != ARG_NULL){
	  st_get_val_type(arglist->arg_ptr, &type, &val);
	  if(type == INT)
	      learn_param_array[i] = val.int_val;
	  else if(type == REAL)
	      learn_param_array[i] = (int)val.real_val;
	  else
	      err_prt("Integer value argument expected");
	  if(!message_flag) {
	      fprintf(log_file, "%d ", learn_param_array[i]);
	      fflush(log_file);
	  }
	  arglist = arglist->next;
      }
  }

  /* get parameter: activation function */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(STRING, type);
    if (strcmp(val.string_val, "Act_Logistic") == 0)
    learn_param_array[13] = ASYM_SIGMOID;
    else
    if (strcmp(val.string_val, "Act_LogSym") == 0)
    learn_param_array[13] = SYM_SIGMOID;
    else
    if (strcmp(val.string_val, "Act_TanH") == 0)
    learn_param_array[13] = TANH;
    else
    if (strcmp(val.string_val, "Act_Random") == 0)
    learn_param_array[13] = RANDOM;
    else
    err_prt("Invalid activation function name");
    
    if(!message_flag) {
	fprintf(log_file, "%s ", val.string_val);
	fflush(log_file);
    }

    arglist = arglist->next;
  }


  /* XGUI Cascade subwindow 'Output Parameters' entries: */

  /* get parameter: error change */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == REAL)
	learn_param_array[14] = val.real_val;
    else if(type == INT)
	learn_param_array[14] = (double)val.int_val;
    else
	err_prt("Real or INT value argument expected");
    if(!message_flag) {
	fprintf(log_file, "%g ", learn_param_array[14]);
	fflush(log_file);
    }
    arglist = arglist->next;
  }

  /* get parameter: output patience */
  /* get parameter: max no of epochs */
  for(i=15; i<=16; i++){
      if (arglist != ARG_NULL){
	  st_get_val_type(arglist->arg_ptr, &type, &val);
	  if(type == INT)
	      learn_param_array[i] = val.int_val;
	  else if(type == REAL)
	      learn_param_array[i] = (int)val.real_val;
	  else
	      err_prt("Integer value argument expected");
	  if(!message_flag) {
	      fprintf(log_file, "%d ", learn_param_array[i]);
	      fflush(log_file);
	  }
	  arglist = arglist->next;
      }
  }

  if (!message_flag)
  {
    fprintf(log_file, "\n");
    fflush(log_file);
  }

  if (arglist != ARG_NULL)
  warn_prt(WRN_FMATE);
}


/*****************************************************************************
  FUNCTION : initNet

  PURPOSE  : initializes the network
  RETURNS  : 
  NOTES    : resets the value of CYCLES

  UPDATE   : 
******************************************************************************/
void initNet(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  int i;

  if (! init_net_flag) 
  warn_prt("Init function and params not specified; using defaults");

  if (arglist != ARG_NULL)
  warn_prt(WRN_NFAE);

  /* initialize init parameter array:  */
  if (! init_net_flag)
  {
    for(i=0; i<NO_OF_INIT_PARAMS; i++)
    { 
      if(i == 0)
	init_param_array[i] = 1.0;
      else if(i == 1)
	init_param_array[i] = -1.0;
      else
	init_param_array[i] = 0.0;
    }
    init_net_flag = TRUE;
  }

  kr_err = krui_initializeNet(init_param_array, noOfInitParams);  
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Net initialized\n");
    fflush(log_file);
  }

  /* update built-in variable: */
  val.int_val = 0;
  type = INT;
  st_set_val_type(st_lookup("CYCLES"), type, val);
}


/*****************************************************************************
  FUNCTION : resetNet

  PURPOSE  : resets the network to its initial configuration
  RETURNS  : 
  NOTES    : resets the value of CYCLES

  UPDATE   : 
******************************************************************************/
void resetNet(arglist_type *arglist)
{
  Val_type val;
  Data_type type;

  krui_resetNet();  

  if(!message_flag)
  {
    fprintf(log_file,"#  Net has been reset\n");
    fflush(log_file);
  }

  /* update built-in variable: */
  val.int_val = 0;
  type = INT;
  st_set_val_type(st_lookup("CYCLES"), type, val);
}


/*****************************************************************************
  FUNCTION : loadNet

  PURPOSE  : loads a network file
             Parameter: network file
  RETURNS  : 
  NOTES    : resets the value of CYCLES

  UPDATE   : 
******************************************************************************/
void loadNet(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  if (arglist == ARG_NULL)
  err_prt("Missing network filename");

  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);

  kr_err = krui_loadNet(val.string_val, &netname);
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Net %s loaded\n", val.string_val);
    fflush(log_file);
  }

  if (arglist->next != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* update built-in variable: */
  val.int_val = 0;
  type = INT;
  st_set_val_type(st_lookup("CYCLES"), type, val);

  init_net_flag = FALSE; /* maybe the user wants a new init function? */
}


/*****************************************************************************
  FUNCTION : saveNet

  PURPOSE  : saves a network
             Parameter: network file name
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void saveNet(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  if (arglist == ARG_NULL)
  err_prt("Missing network filename");

  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);

  kr_err = krui_saveNet(val.string_val, netname);
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Network file %s written\n", val.string_val);
    fflush(log_file);
  }

  if (arglist->next != ARG_NULL)
  warn_prt(WRN_FMATE);
}


/*****************************************************************************
  FUNCTION : saveResult

  PURPOSE  : saves a result file
             1st parameter: result file name
	     optional parameters: start pattern, end pattern,
	                          include input, include output
				  file mode: create | append
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void saveResult(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  int i;
  char *filename;
  bool create = TRUE,
       incl_inp = TRUE,
       incl_out = FALSE;
  int  start_pat = 1, 
       end_pat;

  end_pat = krui_getNoOfPatterns();

  if (arglist == ARG_NULL)
  err_prt("Missing result filename");

  /* get parameters: filename */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);
  filename = val.string_val;
  arglist = arglist->next;

  /* start- & endpattern numbers: */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == INT)
        start_pat = val.int_val;
    else if(type == REAL)
        start_pat = (int)val.real_val;
    else
        err_prt("Integer value argument expected");
    arglist = arglist->next;
  }

  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    if(type == INT)
        end_pat = val.int_val;
    else if(type == REAL)
        end_pat = (int)val.real_val;
    else
        err_prt("Integer value argument expected");
    arglist = arglist->next;
  }

  /* include-input, include-output flags: */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(BOOL, type);
    incl_inp = val.bool_val;
    arglist = arglist->next;
  }

  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(BOOL, type);
    incl_out = val.bool_val;
    arglist = arglist->next;
  }

  /* file mode: create | append: */
  if (arglist != ARG_NULL)
  {
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(STRING, type);
    if (strcmp(val.string_val, "append") == 0)
    create = FALSE;
    else
    if (strcmp(val.string_val, "create") != 0)
    warn_prt
    ("Either \"create\" or \"append\" as file mode expected; using create");
    arglist = arglist->next;
  }

  /* define subpattern shifting scheme: */
  defSubpattern();

  /* initialize update parameter array if not already done: */
  if (! init_update_flag)
  {
    for(i=0; i<NO_OF_UPDATE_PARAMS; i++) update_param_array[i] = 0.0;
    init_update_flag = TRUE;
  }

  kr_err = krui_saveResultParam(filename, create,
				start_pat, end_pat,
				incl_inp, incl_out,
				update_param_array,
				noOfUpdateParam);
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Result file %s written\n", filename);
    fflush(log_file);
  }

  if (arglist != ARG_NULL)
  warn_prt(WRN_FMATE);
}


/*****************************************************************************
  FUNCTION : trainNet

  PURPOSE  : trains the network one cycle with the current patternset
             Parameters: none
  RETURNS  : 
  NOTES    : sets the values of SSE, MSE, SSEPU, CYCLES

  UPDATE   : 
******************************************************************************/
void trainNet(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  float *return_values;
  int NoOfOutParams;
  int i;

  if (arglist != ARG_NULL)
  warn_prt(WRN_NFAE);

  /* initialize learn parameter array if not already done: */
  if (! init_learn_flag)
  {
    learn_param_array[0] = 0.2;
    for(i=1; i<NO_OF_LEARN_PARAMS; i++) learn_param_array[i] = 0.0;
    init_learn_flag = TRUE;
  }

  /* define subpattern shifting scheme: */
  defSubpattern();

  kr_err = 
  krui_learnAllPatterns(learn_param_array, 
			NO_OF_LEARN_PARAMS,
			&return_values, &NoOfOutParams);
  kernel_error(kr_err);

  /* update built-in variables: errors and training cycles*/
  val.real_val = return_values[0];
  type = REAL;
  st_set_val_type(st_lookup("SSE"), type, val);
  
  val.real_val = return_values[0] / krui_getTotalNoOfSubPatterns();
  type = REAL;
  st_set_val_type(st_lookup("MSE"), type, val);
  
  /* set built-in variable SSEPU only if output units are present: */
  if (krui_getNoOfOutputUnits() != 0){
      val.real_val = return_values[0] / krui_getNoOfOutputUnits();
      type = REAL;
  }else
    type = UNKNOWN;

  st_set_val_type(st_lookup("SSEPU"), type, val);

  /* increment the value of CYCLES: */
  st_get_val_type(st_lookup("CYCLES"), &type, &val);
  val.int_val++;
  st_set_val_type(st_lookup("CYCLES"), type, val);
}


/*****************************************************************************
  FUNCTION : testNet

  PURPOSE  : tests the network with the current patternset
             Parameters: none
  RETURNS  : 
  NOTES    : sets the values of SSE, MSE, SSEPU

  UPDATE   : 
******************************************************************************/
void testNet(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  float *return_values;
  int NoOfOutParams;

  if (arglist != ARG_NULL)
  warn_prt(WRN_NFAE);

  /* define subpattern shifting scheme: */
  defSubpattern();
    
  kr_err = 
  krui_testAllPatterns(learn_param_array, 
		       NO_OF_LEARN_PARAMS,
			&return_values, &NoOfOutParams);
  kernel_error(kr_err);

  /* update built-in variables: errors and training cycles*/
  val.real_val = return_values[0];
  type = REAL;
  st_set_val_type(st_lookup("SSE"), type, val);
  
  val.real_val = return_values[0] / krui_getTotalNoOfSubPatterns();
  type = REAL;
  st_set_val_type(st_lookup("MSE"), type, val);
  
  /* set built-in variable SSEPU only if output units are present: */
  if (krui_getNoOfOutputUnits() != 0){
      val.real_val = return_values[0] / krui_getNoOfOutputUnits();
      type = REAL;
  }else
    type = UNKNOWN;

  st_set_val_type(st_lookup("SSEPU"), type, val);
}



/*****************************************************************************
  FUNCTION : pruneNet

  PURPOSE  : prune the net using the pruning parameters
  RETURNS  : 
  NOTES    : function taken from snnsbat.c with minor changes

  UPDATE   : 
******************************************************************************/
void pruneNet(arglist_type *arglist)
{
  krui_err kr_err;
  char *tmp_file1;
  float first_error,
        max_error,
        net_error;

  first_error = trainFFNet(first_train_cyc);
  max_error = first_error * (1 + max_error_incr / 100);
  if (max_error < accepted_error) max_error = accepted_error;

  if (recreate)
  if ((tmp_file1 = tempnam("./", "batch")) == NULL)
  err_prt("Cannot create temporary file");
    
  do
  {
    if (recreate)
    {
      kr_err = krui_saveNet(tmp_file1, netname);
      kernel_error(kr_err);
    }

    kr_err = pr_callPrunFunc(PR_ALL_PATTERNS);
    kernel_error(kr_err);

    kr_err = pr_calcMeanDeviation (PR_ALL_PATTERNS, &net_error);
    kernel_error(kr_err);

    if (net_error > min_error_to_stop) 
    net_error = trainFFNet(retrain_cyc);
  }
  while (net_error <= max_error);

  if (recreate)
  {
    krui_loadNet(tmp_file1, &netname);
    kernel_error(kr_err);
  }
  unlink(tmp_file1);
}

/*****************************************************************************
  FUNCTION : pruneTrainNet
  PURPOSE  : pruning: Train one Cycle with pruning-function
  RETURNS  : 
  NOTES    : function taken pruneNet with minor changes
             sets the values of SSE, MSE, SSEPU, CYCLES
  UPDATE   : 
******************************************************************************/
void pruneTrainNet(arglist_type *arglist)
{
  Val_type val;
  Data_type type;
  float net_error;


  net_error = trainFFNet(1);

  val.real_val = net_error;
  type = REAL;
  st_set_val_type(st_lookup("SSE"), type, val);
  
  val.real_val = net_error / krui_getNoOfPatterns();
  type = REAL;
  st_set_val_type(st_lookup("MSE"), type, val);
  
  /* set built-in variable SSEPU only if output units are present: */
  if (krui_getNoOfOutputUnits() != 0)
    {
      val.real_val = net_error / krui_getNoOfOutputUnits();
      type = REAL;
    }
  else
    type = UNKNOWN;

  st_set_val_type(st_lookup("SSEPU"), type, val);

  /* increment the value of CYCLES: */
  st_get_val_type(st_lookup("CYCLES"), &type, &val);
  val.int_val++;
  st_set_val_type(st_lookup("CYCLES"), type, val);
}

/*****************************************************************************
  FUNCTION : pruneNetNow

  PURPOSE  : pruning: prune Net and calc error of new net
  RETURNS  : 
  NOTES    : function taken pruneNet with minor changes
             sets the values of SSE, MSE, SSEPU
  UPDATE   : 
******************************************************************************/
void pruneNetNow(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;
  float net_error;

  kr_err = pr_callPrunFunc(PR_ALL_PATTERNS);
  kernel_error(kr_err);

  kr_err = pr_calcMeanDeviation (PR_ALL_PATTERNS, &net_error);
  kernel_error(kr_err);

  val.real_val = net_error;
  type = REAL;
  st_set_val_type(st_lookup("SSE"), type, val);
  
  val.real_val = net_error / krui_getNoOfPatterns();
  type = REAL;
  st_set_val_type(st_lookup("MSE"), type, val);
  
  /* set built-in variable SSEPU only if output units are present: */
  if (krui_getNoOfOutputUnits() != 0)
    {
      val.real_val = net_error / krui_getNoOfOutputUnits();
      type = REAL;
    }
  else
    type = UNKNOWN;

  st_set_val_type(st_lookup("SSEPU"), type, val);

}



/*****************************************************************************
  FUNCTION : setActFunc

  PURPOSE  : sets the Activation Funtion for all Units or Units with
            a define type
  RETURNS  : 
  NOTES    : call: setActFunc(<Type>, <new Activation Funktion Name>)
            with: Type=0 for all Units and
                  Type=1... (see kernel/sources/glob_typ.h section
                             Topological Unit Types near line 261)

  UPDATE   : 31.03.1998 by Thomas Rausch
******************************************************************************/
void setActFunc(arglist_type *arglist)
{
    Val_type val;
    Data_type type;
    int i, unit_type;

    /* reading parameters: */

    /* get parameter 1: Unittype for changing activation function */
    if (arglist != ARG_NULL) {
	st_get_val_type(arglist->arg_ptr, &type, &val);
	if(type == INT)
	    unit_type = val.int_val;
	else if(type == REAL)
	    unit_type = (int)val.real_val;
	else
	    err_prt("Integer value argument expected");
	arglist = arglist->next;
    }

    /* get parameter 2: New Activation Function Name */
    if (arglist == ARG_NULL) {
	err_prt("Not enough parameters!\n");
	return;
    }

    /* check for valid function name */
    st_get_val_type(arglist->arg_ptr, &type, &val);
    chck_type(STRING, type);
    if( !krui_isFunction(val.string_val, ACT_FUNC) ) {
	if (message_flag) {
	    fprintf(log_file, "#  Unknown activation function name %s", val.string_val);
	    fflush(log_file);
	}
	err_prt("Unknown activation function name");
	return;
    }

    /* set the new function */
    strcpy(init_fct, val.string_val);
    for (i=1; i<=krui_getNoOfUnits(); i++) {
	if ((krui_getUnitTType(i)==unit_type) || (unit_type==0))
	    krui_setUnitActFunc(i, init_fct);
    }

    /* make a note in the log file */
    if (message_flag) {
	if (unit_type==0)
	    fprintf(log_file,"#  Activation function for all Units is now %s\n", init_fct);
	else {
	    fprintf(log_file, "#  Activation function for all Units with Type ");
	    
	    switch (unit_type) {
	      case INPUT:
		fprintf(log_file, "INPUT");
		break;
	      case OUTPUT:
		fprintf(log_file, "OUTPUT");
		break;
	      case HIDDEN:
		fprintf(log_file, "HIDDEN");
		break;
	      case DUAL:
		fprintf(log_file, "DUAL");
		break;
	      case SPECIAL:
		fprintf(log_file, "SPECIAL");
		break;
	      case SPECIAL_I:
		fprintf(log_file, "SPECIAL_I");
		break;
	      case SPECIAL_O:
		fprintf(log_file, "SPECIAL_O");
		break;
	      case SPECIAL_H:
		fprintf(log_file, "SPECIAL_H");
		break;
	      case SPECIAL_D:
		fprintf(log_file, "SPECIAL_D");
		break;
	      case SPECIAL_X:
		fprintf(log_file, "SPECIAL_X");
		break;
	      case N_SPECIAL_X:
		fprintf(log_file, "N_SPECIAL_X");
		break;
	      default:
		fprintf(log_file, "%i", unit_type);
	    };
	    fprintf(log_file, " is now %s\n", init_fct);
	}
	fflush(log_file);
    }

    /* is arg list empty? */
    if (arglist->next != ARG_NULL)
	warn_prt(WRN_FMATE);
}
 


/*****************************************************************************
  FUNCTION : delCandUnits

  PURPOSE  : Cascade Correlation: deletes the candidate units
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void delCandUnits(arglist_type *arglist)
{
  if(!message_flag)
  {
    fprintf(log_file,"#  Old call to delCandUnits ignored\n");
    fflush(log_file);
  }
}



/*****************************************************************************
  FUNCTION : loadPattern

  PURPOSE  : loads a pattern file
             Parameter: pattern file name
  RETURNS  : 
  NOTES    : sets the value of PAT

  UPDATE   : 
******************************************************************************/
void loadPattern(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  if (arglist == ARG_NULL)
  err_prt(ERR_MPF);

  /* get pattern name: */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);

  kr_err = krui_loadNewPatterns(val.string_val, &curr_patSet);
  kernel_error(kr_err);
  enter_patName(arglist->arg_ptr);

  if(!message_flag)
  {
    fprintf(log_file,"#  Patternset %s loaded; %d patternset(s) in memory\n", 
	    val.string_val, pat_sets_loaded);
    fflush(log_file);
  }

  if (arglist->next != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* update built-in variable: */
  val.int_val = krui_getNoOfPatterns();
  type = INT;
  st_set_val_type(st_lookup("PAT"), type, val);
}


/*****************************************************************************
  FUNCTION : setPattern

  PURPOSE  : makes the named patternset the current
             Parameter: pattern file name
  RETURNS  : 
  NOTES    : sets the value of PAT

  UPDATE   : 
******************************************************************************/
void setPattern(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  if (arglist == ARG_NULL)
  err_prt("Missing pattern filename");

  /* get pattern name: */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);

  curr_patSet = lookup_patName(arglist->arg_ptr);
  kr_err = kr_npui_setCurrPatSet(curr_patSet);
  kernel_error(kr_err);

  if(!message_flag)
  {
    fprintf(log_file,"#  Patternset is now %s\n", val.string_val);
    fflush(log_file);
  }

  if (arglist->next != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* update built-in variable: */
  val.int_val = krui_getNoOfPatterns();
  type = INT;
  st_set_val_type(st_lookup("PAT"), type, val);
}


/*****************************************************************************
  FUNCTION : delPattern

  PURPOSE  : deletes the named patternset
             Parameter: pattern file name
  RETURNS  : 
  NOTES    : resets the value of PAT

  UPDATE   : 
******************************************************************************/
void delPattern(arglist_type *arglist)
{
  krui_err kr_err;
  Val_type val;
  Data_type type;

  if (arglist == ARG_NULL)
  err_prt(ERR_MPF);

  /* get pattern name: */
  st_get_val_type(arglist->arg_ptr, &type, &val);
  chck_type(STRING, type);

  kr_err = kr_npui_deletePatSet(lookup_patName(arglist->arg_ptr));
  kernel_error(kr_err);

  del_patName(lookup_patName(arglist->arg_ptr));

  if(!message_flag)
  {
    fprintf(log_file,"#  Patternset %s deleted; %d patternset(s) in memory\n", 
	    val.string_val, pat_sets_loaded);
    fflush(log_file);
  }

  if (pat_sets_loaded != 0){
      /* if current patset is deleted, make patset 0 the current one: */
      kr_err = kr_npui_setCurrPatSet(curr_patSet);
      kernel_error(kr_err);
      
      /* lookup name of patset and display it: */
      st_get_val_type(lookup_patNumber(curr_patSet), &type, &val);
      
      if(!message_flag)
	  {
	      fprintf(log_file, "#  Patternset is now %s\n", val.string_val);
	      fflush(log_file);
	  }
      val.int_val = krui_getNoOfPatterns();
  }
  else{
      val.int_val = 0;
      if(!message_flag)
	  {
	      fprintf(log_file, "#  No Patternset actually loaded.\n");
	      fflush(log_file);
	  }
  }

  if (arglist->next != ARG_NULL)
  warn_prt(WRN_FMATE);

  /* update built-in variable: */
  type = INT;
  st_set_val_type(st_lookup("PAT"), type, val);
}

/*****************************************************************************
  FUNCTION : setSeed

  PURPOSE  : sets the seed for the pseudo random generator
             Parameter: (optional) seed_value 
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void setSeed(arglist_type *arglist)
{
  Val_type val;
  Data_type type;
  long int seed;

  if (arglist == ARG_NULL)
     seed = time(NULL);
  else{
      /* get seed value */
      st_get_val_type(arglist->arg_ptr, &type, &val);
      if(type == INT)
	  seed = val.int_val;
      else if(type == REAL)
	  seed = (int)val.real_val;
      else
	  err_prt("Integer value argument expected");
      if (arglist->next != ARG_NULL)
	  warn_prt(WRN_FMATE);
  }

  krui_setSeedNo(seed);

  return;
}

/*****************************************************************************
  FUNCTION : jogWeights

  PURPOSE  : adds (multiplies) noise to the link weights
             Parameter: minus, plus noise boundary
  RETURNS  : 
  NOTES    :

  UPDATE :
******************************************************************************/
void jogWeights(arglist_type *arglist)
{

    Val_type val; 
    Data_type type; 

    float jog_minus, jog_plus;

    jog_minus = -0.001;
    jog_plus = 0.001;

    if (arglist != ARG_NULL)
    {
	st_get_val_type(arglist->arg_ptr, &type, &val);
	chck_type(REAL, type);
	jog_minus = val.real_val;
	arglist = arglist->next;
    }

    if (arglist != ARG_NULL)
    {
	st_get_val_type(arglist->arg_ptr, &type, &val);
	chck_type(REAL, type);
	jog_plus = val.real_val;
	arglist = arglist->next;
    }

    krui_jogWeights(jog_minus, jog_plus);

    if(!message_flag)
    {
	fprintf(log_file,"#  weights jogged\n");
	fflush(log_file);
    }

    if (arglist != ARG_NULL)
	warn_prt(WRN_FMATE);
}

/*****************************************************************************
  FUNCTION : jogCorrWeights

  PURPOSE  : adds (multiplies) noise to the link weights of correlated hiddens
             Parameter: minus, plus noise boundary
  RETURNS  : 
  NOTES    :

  UPDATE :
******************************************************************************/
void jogCorrWeights(arglist_type *arglist)
{

    krui_err kr_err; 
    Val_type val; 
    Data_type type; 

    float jog_minus, jog_plus, jog_correlation;

    jog_minus = -0.001;
    jog_plus = 0.001;
    jog_correlation = 0.98;

    if (arglist != ARG_NULL)
    {
	st_get_val_type(arglist->arg_ptr, &type, &val);
	chck_type(REAL, type);
	jog_minus = val.real_val;
	arglist = arglist->next;
    }

    if (arglist != ARG_NULL)
    {
	st_get_val_type(arglist->arg_ptr, &type, &val);
	chck_type(REAL, type);
	jog_plus = val.real_val;
	arglist = arglist->next;
    }

    if (arglist != ARG_NULL)
    {
	st_get_val_type(arglist->arg_ptr, &type, &val);
	chck_type(REAL, type);
	jog_correlation = val.real_val;
	arglist = arglist->next;
    }

    kr_err = krui_jogCorrWeights(jog_minus, jog_plus, jog_correlation);
    kernel_error(kr_err);

    if(!message_flag)
    {
	fprintf(log_file,"#  weights of correlated hiddens jogged\n");
	fflush(log_file);
    }

    if (arglist != ARG_NULL)
	warn_prt(WRN_FMATE);
}

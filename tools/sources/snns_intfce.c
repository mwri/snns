/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/snns_intfce.c,v $
  SHORTNAME      : snns_intfce
  SNNS VERSION   : 4.2

  PURPOSE        : Interface between Batchman's jacket function calls (via 
                   the symbol table) and SNNS-kernel calls

  NOTES          : This file is provided for future extensions of Batchman
                   and has *no semantic purpose* in this version. The 
                   intention is to set up a clearly defined entry point 
                   between the batch language processing and the SNNS-kernel. 
                   Instead of adressing the kernel, XGUI callback
                   functions can be executed from here.

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.12 $
  LAST CHANGE    : $Date: 1998/07/08 11:13:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include "symtab.h"          /* for arglist_type */
#include "snns_intfce.ph"
#include "ic_snns.h"         /* implementation module of SNNS functions */


/*****************************************************************************
  FUNCTION : setInitFunc

  PURPOSE  : sets the initialization function and its parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : init function name is mandatory

  UPDATE   : 
******************************************************************************/
void int_setInitFunc(arglist_type *arglist)
{
  setInitFunc(arglist);
}


/*****************************************************************************
  FUNCTION : setLearnFunc

  PURPOSE  : sets the learning function and its parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : learning function name is mandatory

  UPDATE   : 
******************************************************************************/
void int_setLearnFunc(arglist_type *arglist)
{
  setLearnFunc(arglist);
}


/*****************************************************************************
  FUNCTION : setUpdateFunc

  PURPOSE  : sets the update function and its parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : update function name is mandatory

  UPDATE   : 
******************************************************************************/
void int_setUpdateFunc(arglist_type *arglist)
{
  setUpdateFunc(arglist);
}


/*****************************************************************************
  FUNCTION : setPruningFunc

  PURPOSE  : sets the pruning functions and their parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : pruning and subordinate learning function names are mandatory

  UPDATE   : 
******************************************************************************/
void int_setPruningFunc(arglist_type *arglist)
{
  setPruningFunc(arglist);
}


/*****************************************************************************
  FUNCTION : setRemapFunc

  PURPOSE  : sets the pattern remapping functions and their parameters,
             if arguments are supplied
  RETURNS  :
  NOTES    : remap function name is mandatory

  UPDATE   : 
******************************************************************************/
void int_setRemapFunc(arglist_type *arglist)
{
  setRemapFunc(arglist);
}


/*****************************************************************************
  FUNCTION : setSubPattern

  PURPOSE  : sets the subpattern handling parameters

  RETURNS  :
  NOTES    : kernel call is performed in defSubpattern()

  UPDATE   : 
******************************************************************************/
void int_setSubPattern(arglist_type *arglist)
{
  setSubPattern(arglist);
}


/*****************************************************************************
  FUNCTION : setShuffle

  PURPOSE  : sets the pattern shuffling mode for ordinary patterns
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void int_setShuffle(arglist_type *arglist)
{
  setShuffle(arglist);
}


/*****************************************************************************
  FUNCTION : setSubShuffle

  PURPOSE  : sets the pattern shuffling mode for subpatterns
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void int_setSubShuffle(arglist_type *arglist)
{
  setSubShuffle(arglist);
}


/*****************************************************************************
  FUNCTION : setClassDistrib

  PURPOSE  : sets the distribution of the classes in the pattern file
             to the values given as parameters. 
  RETURNS  :
  NOTES    : first parameter (bool) switches redistribution on or off
             also sets the value of PAT

  UPDATE   : 
******************************************************************************/
void int_setClassDistrib(arglist_type *arglist)
{
  setClassDistrib(arglist);
}


/*****************************************************************************
  FUNCTION : setParallelMode

  PURPOSE  : switches to the parallel kernel if argument is TRUE and back
             if argument is FALSE
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void int_setParallelMode(arglist_type *arglist)
{
  setParallelMode(arglist);
}


/*****************************************************************************
  FUNCTION : setCascadeParams

  PURPOSE  : sets the parameters of the cascade correlation algorithms
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void int_setCascadeParams(arglist_type *arglist)
{
  setCascadeParams(arglist);
}



/*****************************************************************************
  FUNCTION : initNet

  PURPOSE  : initializes the network
  RETURNS  : 
  NOTES    : resets the value of CYCLES

  UPDATE   : 
******************************************************************************/
void int_initNet(arglist_type *arglist)
{
  initNet(arglist);
}


/*****************************************************************************
  FUNCTION : resetNet

  PURPOSE  : resets the network to its initial configuration
  RETURNS  : 
  NOTES    : resets the value of CYCLES

  UPDATE   : 
******************************************************************************/
void int_resetNet(arglist_type *arglist)
{
  resetNet(arglist);
}


/*****************************************************************************
  FUNCTION : loadNet

  PURPOSE  : loads a network file
             Parameter: network file
  RETURNS  : 
  NOTES    : resets the value of CYCLES

  UPDATE   : 
******************************************************************************/
void int_loadNet(arglist_type *arglist)
{
  loadNet(arglist);
}


/*****************************************************************************
  FUNCTION : saveNet

  PURPOSE  : saves a network
             Parameter: network file name
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void int_saveNet(arglist_type *arglist)
{
  saveNet(arglist);
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
void int_saveResult(arglist_type *arglist)
{
  saveResult(arglist);
}


/*****************************************************************************
  FUNCTION : trainNet

  PURPOSE  : trains the network one cycle with the current patternset
             Parameters: none
  RETURNS  : 
  NOTES    : sets the values of SSE, MSE, SSEPU, CYCLES

  UPDATE   : 
******************************************************************************/
void int_trainNet(arglist_type *arglist)
{
  trainNet(arglist);
}


/*****************************************************************************
  FUNCTION : testNet

  PURPOSE  : tests the network with the current patternset
             Parameters: none
  RETURNS  : 
  NOTES    : sets the values of SSE, MSE, SSEPU

  UPDATE   : 
******************************************************************************/
void int_testNet(arglist_type *arglist)
{
  testNet(arglist);
}


/*****************************************************************************
  FUNCTION : pruneNet

  PURPOSE  : prune the net using the pruning parameters
  RETURNS  : 
  NOTES    : function taken from snnsbat.c with minor changes

  UPDATE   : 
******************************************************************************/
void int_pruneNet(arglist_type *arglist)
{
  pruneNet(arglist);
}

/*****************************************************************************
  FUNCTION : pruneTrainNet

  PURPOSE  : pruning: Train one Cycle with pruning-function
  RETURNS  : 
  NOTES    : function taken pruneNet with minor changes
             sets the values of SSE, MSE, SSEPU, CYCLES
  UPDATE   : 
******************************************************************************/
void int_pruneTrainNet(arglist_type *arglist)
{
  pruneTrainNet(arglist);
}

/*****************************************************************************
  FUNCTION : pruneNetNow

  PURPOSE  : pruning: prune Net and calc error of new net
  RETURNS  : 
  NOTES    : function taken pruneNet with minor changes
             sets the values of SSE, MSE, SSEPU
  UPDATE   : 
******************************************************************************/
void int_pruneNetNow(arglist_type *arglist)
{
    pruneNetNow(arglist);
}

/*****************************************************************************
  FUNCTION : setActFunc

  PURPOSE  : sets the Activation Funtion for _all_ Neurons
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void int_setActFunc(arglist_type *arglist)
{
    setActFunc(arglist);
}


/*****************************************************************************
  FUNCTION : delCandUnits

  PURPOSE  : Cascade Correlation: deletes the candidate units
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
void int_delCandUnits(arglist_type *arglist)
{
  delCandUnits(arglist);
}



/*****************************************************************************
  FUNCTION : loadPattern

  PURPOSE  : loads a pattern file
             Parameter: pattern file name
  RETURNS  : 
  NOTES    : sets the value of PAT

  UPDATE   : 
******************************************************************************/
void int_loadPattern(arglist_type *arglist)
{
  loadPattern(arglist);
}


/*****************************************************************************
  FUNCTION : setPattern

  PURPOSE  : makes the named patternset the current
             Parameter: pattern file name
  RETURNS  : 
  NOTES    : sets the value of PAT

  UPDATE   : 
******************************************************************************/
void int_setPattern(arglist_type *arglist)
{
  setPattern(arglist);
}


/*****************************************************************************
  FUNCTION : delPattern

  PURPOSE  : deletes the named patternset
             Parameter: pattern file name
  RETURNS  : 
  NOTES    : resets the value of PAT

  UPDATE   : 
******************************************************************************/
void int_delPattern(arglist_type *arglist)
{
  delPattern(arglist);
}

/*****************************************************************************
  FUNCTION : setSeed

  PURPOSE  : sets the seed for the pseudo random generator
             Parameter: (optional) seed_value 
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void int_setSeed(arglist_type *arglist)
{
    setSeed(arglist);
}


/*****************************************************************************
  FUNCTION : jogWeights

  PURPOSE  : adds (multiplies) noise to the link weights
             Parameter: minus, plus noise boundary
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void int_jogWeights(arglist_type *arglist)
{
    jogWeights(arglist);
}


/*****************************************************************************
  FUNCTION : jogCorrWeights

  PURPOSE  : adds (multiplies) noise to the link weights of correlated hiddens
             Parameter: minus, plus noise boundary
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void int_jogCorrWeights(arglist_type *arglist)
{
    jogCorrWeights(arglist);
}



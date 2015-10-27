/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/snns_intfce.ph,v $
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
  RCS VERSION    : $Revision: 1.10 $
  LAST CHANGE    : $Date: 1998/04/20 11:48:14 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _SNNS_INTFCE_DEFINED_
#define _SNNS_INTFCE_DEFINED_

/* begin global definition section */

void int_setInitFunc(arglist_type *arglist);
void int_setLearnFunc(arglist_type *arglist);
void int_setUpdateFunc(arglist_type *arglist);
void int_setPruningFunc(arglist_type *arglist);
void int_setRemapFunc(arglist_type *arglist);
void int_setSubPattern(arglist_type *arglist);
void int_setShuffle(arglist_type *arglist);
void int_setSubShuffle(arglist_type *arglist);
void int_setClassDistrib(arglist_type *arglist);
void int_setParallelMode(arglist_type *arglist);
void int_setCascadeParams(arglist_type *arglist);

void int_initNet(arglist_type *arglist);
void int_resetNet(arglist_type *arglist);
void int_loadNet(arglist_type *arglist);
void int_saveNet(arglist_type *arglist);
void int_saveResult(arglist_type *arglist);
void int_trainNet(arglist_type *arglist);
void int_testNet(arglist_type *arglist);
void int_pruneNet(arglist_type *arglist);
void int_pruneTrainNet(arglist_type *arglist);
void int_pruneNetNow(arglist_type *arglist);
void int_delCandUnits(arglist_type *arglist);
void int_setActFunc(arglist_type *arglist);

void int_loadPattern(arglist_type *arglist);
void int_setPattern(arglist_type *arglist);
void int_delPattern(arglist_type *arglist);

void int_setSeed(arglist_type *arglist);
void int_jogWeights(arglist_type *arglist);
void int_jogCorrWeights(arglist_type *arglist);

/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif

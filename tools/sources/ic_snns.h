/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/ic_snns.h,v $
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
  RCS VERSION    : $Revision: 1.10 $
  LAST CHANGE    : $Date: 1998/04/20 11:48:09 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


extern void setInitFunc(arglist_type *arglist);
extern void setLearnFunc(arglist_type *arglist);
extern void setUpdateFunc(arglist_type *arglist);
extern void setPruningFunc(arglist_type *arglist);
extern void setRemapFunc(arglist_type *arglist);
extern void setSubPattern(arglist_type *arglist);
extern void setShuffle(arglist_type *arglist);
extern void setSubShuffle(arglist_type *arglist);
extern void setClassDistrib(arglist_type *arglist);
extern void setParallelMode(arglist_type *arglist);
extern void setCascadeParams(arglist_type *arglist);

extern void initNet(arglist_type *arglist);
extern void resetNet(arglist_type *arglist);
extern void loadNet(arglist_type *arglist);
extern void saveNet(arglist_type *arglist);
extern void saveResult(arglist_type *arglist);
extern void trainNet(arglist_type *arglist);
extern void testNet(arglist_type *arglist);
extern void pruneNet(arglist_type *arglist);
extern void pruneTrainNet(arglist_type *arglist);
extern void pruneNetNow(arglist_type *arglist);
extern void delCandUnits(arglist_type *arglist);
extern void setActFunc(arglist_type *arglist);
 
extern void loadPattern(arglist_type *arglist);
extern void setPattern(arglist_type *arglist);
extern void delPattern(arglist_type *arglist);

extern void setSeed(arglist_type *arglist);
extern void jogWeights(arglist_type *arglist);
extern void jogCorrWeights(arglist_type *arglist);


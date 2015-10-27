/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_bignet.h,v $
  SHORTNAME      : bn_bignet
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Michael Schmalzl  
  DATE           : 1.4.1990

  CHANGED BY     : Sven Doering, Kai-Uwe Herrmann
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_BIGNET_DEFINED_
#define  _BN_BIGNET_DEFINED_

extern void bn_createBignet (void);
extern int calculate_x_begin(int *new_x_begin, int *old_x_begin,
			     int *x_max, int width, int pos);
extern int calculate_y_begin(int *absolute_y_max, int *relative_y_max,
			     int *y_offset, int height, int pos);

extern Widget ff_baseWidget;
#endif 
/* end of file */
/* lines:  */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_dither.ph,v $
  SHORTNAME      : dither.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header
  NOTES          :

  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:00 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _D3_DITHER_DEFINED_
#define  _D3_DITHER _DEFINED_

/* begin global definition section */

int dither (int x, int y, float level);

/* end global definition section */

/* begin private definition section */
#define DITHER8x8

#ifdef DITHER8x8

static int size = 8;

static int dither_matrix[] = {
       0,  48,  12,  60,   3,  51,  15,  63,
      32,  16,  44,  28,  35,  19,  47,  31,
       8,  56,   4,  52,  11,  59,   7,  55,
      40,  24,  36,  20,  43,  27,  39,  23,
       2,  50,  14,  62,   1,  49,  13,  61,
      34,  18,  46,  30,  33,  17,  45,  29,
      10,  58,   6,  54,   9,  57,   5,  53,
      42,  26,  38,  22,  41,  25,  37,  21
};

#else

static int size = 4;

static int dither_matrix[] = {
       0,  12,   3,  15,
       8,   4,  11,   7,
       2,  14,   1,  13,
      10,   6,   9,   5
};

#endif

/* end private definition section */

#endif

/* end of file */
/* lines: 26 */

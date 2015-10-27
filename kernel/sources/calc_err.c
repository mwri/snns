/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/calc_err.c,v $
  SHORTNAME      : calc_err
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Tool: Calculates the error of a table-lookup function
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 06.12.91

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:21 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <math.h>


#define MINUS_INFINITE_BORDER  -100.0
#define PLUS_INFINITE_BORDER	100.0
#define CALC_STEPS 200000



/* ***************************************************************** */

/*  Sigmoid Function
*/
double	f( x )
double	x;
{
  return( 1.0 / (1.0 + exp( -x )));
}

/*  Sigmoid Function
    using table lookup and linear approximation method
*/
double	 LogisticTbl( x )
double	x;
{
#include "sigmoid.tbl"

  register int	index;


  index = (int) (x * SCALE_FACTOR) + INDEX_OFFSET;

  if (index < 0)
    {  /*  x is less then MIN_APPROX_X:
	   approx. func value to MINUS_INFINITE_FUNC_VALUE  */
    if (index <= MIN_INDEX)
      { /*  printf( "x below -infinite : %g\n", x );  */
	return( MINUS_INFINITE_FUNC_VALUE );
      }
    /*	printf( "x below MIN_APPROX_X : %g\n", x );  */
    return( m[0] * x + b[0] );
    }

  if (index > NO_OF_APPROX)
    {  /*  x is greater then MAX_APPROX_X:
	   approx. func value to PLUS_INFINITE_FUNC_VALUE  */
    if (index >= MAX_INDEX)
      { /*  printf( "x above +infinite : %g\n", x );  */
	return( PLUS_INFINITE_FUNC_VALUE );
      }
    /*	printf( "x above MAX_APPROX_X : %g\n", x );  */
    return( m[ NO_OF_APPROX ] * x + b[ NO_OF_APPROX ] );
    }

  return( m[ index ] * x + b[ index ] );
}


/* ***************************************************************** */




int  main()
{
  double  x, step, y1, y2, devit;
  int	  i, nl_cycle;


  devit = 0.0;
  step = (PLUS_INFINITE_BORDER - MINUS_INFINITE_BORDER) / (double) CALC_STEPS;

  x = MINUS_INFINITE_BORDER;
  y1 = fabs( f(x) - LogisticTbl( x ) );
  x += step;

  for (i = 0; i < CALC_STEPS - 1; i++)
    {
    y2 = fabs( f(x) - LogisticTbl( x ) );
    devit += 0.5 * (y1 + y2) * step;
    y1 = y2;
    x += step;
    }

  fprintf( stdout, "Error in [%+g,%+g]: %.10g\n",
	   MINUS_INFINITE_BORDER, PLUS_INFINITE_BORDER, devit );
}

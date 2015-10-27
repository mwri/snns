/*     $State: Exp $ $Locker:  $  */

/**********************************************************************
FILE   : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/make_tbl.c,v $
PURPOSE: SNNS-Kernel Tool: Calculates the lookup tables for table lookup
	 transfer functions
NOTES  : 
AUTHOR : Niels Mache
DATE   : 06.12.91
VERSION : $Revision: 2.5 $  $Date: 1998/03/03 14:08:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

**********************************************************************/
#include <config.h>

#include <stdio.h>
#include <math.h>


#define NEWLINE_INSERT	10
#define PRECISION  7



/*  user defined parameters
*/
#define NO_OF_APPROX 1000
#define MIN_APPROX_X -12.0
#define MAX_APPROX_X  12.0
#define MINUS_INFINITE_BORDER  -13.0
#define PLUS_INFINITE_BORDER	13.0
#define MINUS_INFINITE_FUNC_VALUE  0.0
#define PLUS_INFINITE_FUNC_VALUE   1.0


#define ERR_MINUS_INFINITE_BORDER  -100.0
#define ERR_PLUS_INFINITE_BORDER    100.0
#define ERROR 7.425010618e-05
/*
#define INDEX_OFFSET (NO_OF_APPROX / 2)
#define SCALE_FACTOR (NO_OF_APPROX / (MAX_APPROX_X - MIN_APPROX_X))
#define MIN_INDEX ((int) (MINUS_ININITE_BORDER * SCALE_FACTOR) + INDEX_OFFSET)
#define MAX_INDEX ((int) (PLUS_ININITE_BORDER * SCALE_FACTOR) + INDEX_OFFSET)
*/

/*  Sigmoid Function
*/
double	f( x )
double	x;
{
  return( 1.0 / (1.0 + exp( -x )));
}


void  calc_m_and_b( x1, x2, m, b )
double	x1, x2;
double	*m, *b;
{
  double  y1, y2;

  y1 = f( x1 );
  y2 = f( x2 );

  *m = (y2 - y1) / (x2 - x1);
  *b = y1 - *m * x1;
}


int  main()
{
  double  x, m, b, step, scale;
  int  i, nl_cycle;
  int  index_offset, min_index, max_index;


  index_offset = NO_OF_APPROX / 2;
  step = (MAX_APPROX_X - MIN_APPROX_X) / (double) NO_OF_APPROX;
  scale = (double) NO_OF_APPROX / (MAX_APPROX_X - MIN_APPROX_X);
  min_index = (int) (MINUS_INFINITE_BORDER * scale) + index_offset;
  max_index = (int) (PLUS_INFINITE_BORDER * scale) + index_offset;

  fprintf( stdout, "/*\nNo. of Approx.: %d\nMin. x: %g\nMax. x: %g\n",
	   NO_OF_APPROX, MIN_APPROX_X, MAX_APPROX_X );
  fprintf( stdout, "Minus inf. border: %g\nPlus inf. border : %g\n",
	   MINUS_INFINITE_BORDER, PLUS_INFINITE_BORDER );
  fprintf( stdout, "Error in [%+g,%+g]: %g\n*/\n\n",
	   ERR_MINUS_INFINITE_BORDER, ERR_PLUS_INFINITE_BORDER, ERROR );

  fprintf( stdout, "#define NO_OF_APPROX %d\n#define INDEX_OFFSET %d\n",
	   NO_OF_APPROX, index_offset );
  fprintf( stdout, "#define MINUS_INFINITE_FUNC_VALUE %#.10g\n#define PLUS_INFINITE_FUNC_VALUE %#.10g\n",
	   MINUS_INFINITE_FUNC_VALUE, PLUS_INFINITE_FUNC_VALUE );
  fprintf( stdout, "#define SCALE_FACTOR %#.10g\n#define MIN_INDEX %d\n#define MAX_INDEX %d\n\n",
	   scale, min_index, max_index );


  fprintf( stdout, "static float m[] = {\n" );

  calc_m_and_b( MINUS_INFINITE_BORDER, MIN_APPROX_X, &m, &b );
  fprintf( stdout, "%.8g,  /*  minus infinite border  */\n", m );

  x = MIN_APPROX_X;
  nl_cycle = 0;
  for (i = 0; i < NO_OF_APPROX - 1; i++)
    {
    calc_m_and_b( x, x + step, &m, &b );

    if ((++nl_cycle % NEWLINE_INSERT) == 0)
      fprintf( stdout, "%.8g,\n", m );
    else
      fprintf( stdout, "%.8g, ", m );

    x += step;
    }

  calc_m_and_b( MAX_APPROX_X, PLUS_INFINITE_BORDER, &m, &b );
  if ((nl_cycle % NEWLINE_INSERT) == 0)
    fprintf( stdout, "%.8g  };  /*  plus infinite border  */\n\n", m );
  else
    fprintf( stdout, "\n%.8g  };  /*  plus infinite border  */\n\n", m );


  fprintf( stdout, "static float b[] = {\n" );

  calc_m_and_b( MINUS_INFINITE_BORDER, MIN_APPROX_X, &m, &b );
  fprintf( stdout, "%.8g,  /*  minus infinite border  */\n", b );

  x = MIN_APPROX_X;
  nl_cycle = 0;
  for (i = 0; i < NO_OF_APPROX - 1; i++)
    {
    calc_m_and_b( x, x + step, &m, &b );

    if ((++nl_cycle % NEWLINE_INSERT) == 0)
      fprintf( stdout, "%.8g,\n", b );
    else
      fprintf( stdout, "%.8g, ", b );

    x += step;
    }

  calc_m_and_b( MAX_APPROX_X, PLUS_INFINITE_BORDER, &m, &b );
  if ((nl_cycle % NEWLINE_INSERT) == 0)
    fprintf( stdout, "%.8g  };  /*  plus infinite border  */\n", b );
  else
    fprintf( stdout, "\n%.8g  };  /*  plus infinite border  */\n", b );

  exit( 0 );
}

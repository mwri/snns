/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/netperf.c,v $
  SHORTNAME      : netperf
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Performance Test Program
  NOTES          : 

  AUTHOR         : Niels Mache
  DATE           : 30.07.90

  CHANGED BY     : Niels Mache
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:23 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#ifndef  __BORLANDC__
#include <sys/param.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/utsname.h>
#endif


/*  SNNS-Kernel constants and data type definitions  */
#include "glob_typ.h"
/*  SNNS-Kernel User-Interface Function Prototypes   */
#include "kr_ui.h"



static void  errChk( err_code )
int  err_code;
{
  if (err_code != KRERR_NO_ERROR)  {
    fprintf( stderr, "%s\n", krui_error( err_code ));
    exit( 1 );
  }
}


int main()
{
  int	ret_code, N, i, j, no_of_sites, no_of_links, no_of_units,
	no_of_patterns, dummy, step, NoOfReturnVals, no_of_input_params,
	no_of_output_params, choose_me,
      	SubPatternISize_param[5],
        SubPatternOSize_param[5],
        SubPatternIStep_param[5],
        SubPatternOStep_param[5];

  double  cpu_time, perf;
  char	*netname, file_name[80];
  float learn_parameters[5];
  float *return_values;
#ifdef  __BORLANDC__
  clock_t  init_time;
#else
  struct tms  tp;
  long  HZ_value;
  long init_time;
#endif


#ifndef  __BORLANDC__
  /*  get HZ value  */
  if ((getenv( "HZ" ) == NULL) ||
      (sscanf( getenv( "HZ" ), "%ld", &HZ_value ) != 1))  {
#ifndef HZ
    fprintf( stderr, "****  WARNING: No HZ defined!\n" );
    HZ_value = 60;
#else
    HZ_value = HZ;
#endif
  }
#endif


  fprintf( stdout, "\n%s\n", krui_getVersion() );
  fprintf( stdout, "-----  Benchmark Test  -----\n\n" );

  fprintf( stdout,"Filename of the network file: ");
  scanf("%s", file_name);

  fprintf( stdout, "Loading the network ...\n\n" );
  ret_code = krui_loadNet( file_name, &netname );
  errChk( ret_code );
  krui_getNetInfo( &no_of_sites, &no_of_links, &dummy, &dummy );
  no_of_units = krui_getNoOfUnits();
  fprintf( stdout, "Network name: %s\n", netname );
  fprintf( stdout, "No. of units       : %d\n", no_of_units );
  fprintf( stdout, "No. of input units : %d\n", krui_getNoOfTTypeUnits( INPUT ) );
  fprintf( stdout, "No. of output units: %d\n", krui_getNoOfTTypeUnits( OUTPUT ) );
  fprintf( stdout, "No. of sites: %d\n", no_of_sites );
  fprintf( stdout, "No. of links: %d\n\n", no_of_links );
  fprintf( stdout, "Learning function: %s\n", krui_getLearnFunc() );
  fprintf( stdout, "Update function  : %s\n", krui_getUpdateFunc() );

  fprintf( stdout, "\nFilename of the pattern file: " );
  scanf( "%s", file_name );
  fprintf( stdout, "Loading the patterns ...\n\n" );

#ifdef  OLDFORMAT
  ret_code = krui_loadPatterns( file_name );
#else
  ret_code = krui_loadNewPatterns( file_name, &dummy );
#endif
  errChk( ret_code );
  no_of_patterns = krui_getNoOfPatterns();
  fprintf( stdout, "No. of patterns: %d\n", no_of_patterns );


  errChk(krui_DefTrainSubPat(SubPatternISize_param,SubPatternOSize_param,
		             SubPatternIStep_param,SubPatternOStep_param,
		             NULL));

  fprintf( stdout,"\nDo you want to benchmark\nPropagation [1] or\nBackpropagation [2] ?\nInput: ");
  scanf( "%d", &choose_me );


#if defined(VECTOR_KERNEL)
  fprintf( stdout,"\nVectorize Network? (y/n): " );
  scanf("%s", file_name);

  if (file_name[0] == 'y')  {
    fprintf( stdout,"Vectorize Network...\n" );
    errChk( krui_setSpecialNetworkType(KERNEL_MODE_VECTOR) );
    fprintf( stdout,"Network vectorized.\n" );
  }
#endif
/*
#if defined(VECTOR_KERNEL)
  fprintf( stdout,"Devectorize Network...\n" );
  errChk( krui_setSpecialNetworkType(KERNEL_MODE_STD) );
  fprintf( stdout,"Network devectorized.\n" );
#endif
*/

  fprintf( stdout, "\nChoose no. of cycles: " );
  scanf("%d", &N);
  if (N <= 0)  {
     fprintf( stdout, "\nInvalid no. of cycles !\n" );
     exit( 1 );
  }
  if (choose_me == 1)  {
    fprintf( stdout, "\nBegin propagation ...\n" );

#ifdef  __BORLANDC__
    init_time = clock();
#else
    (void) times( &tp );
    init_time = tp.tms_utime;
#endif

    for (i = 0; i < N; i++)  {
      for (j=1; j <= no_of_patterns; j++)  {
/*
	krui_setPatternNo(j);
        errChk( ret_code );
        krui_showPattern(OUTPUT_NOTHING);
        errChk( ret_code );
*/
        ret_code = krui_updateNet( learn_parameters, 0 );
        errChk( ret_code );
      }
    }
  }
  else  {
    /*  determine the no. of parameters of the current learning function
    */
    (void) krui_getFuncParamInfo( krui_getLearnFunc(), LEARN_FUNC,
	  			&no_of_input_params, &no_of_output_params );
    fprintf( stdout, "\nThe learning function '%s' needs %d input parameters:\n",
	  krui_getLearnFunc(), no_of_input_params );
    for (i = 0; i < no_of_input_params; i++)  {
       fprintf( stdout, "Parameter [%d]: ", i + 1 );
       scanf( "%f", &learn_parameters[ i ] );
    }

    step = ((N - 1) / 10) + 1;
    fprintf( stdout, "\nBegin learning ...\n" );

#ifdef  __BORLANDC__
    init_time = clock();
#else
    (void) times( &tp );
    init_time = tp.tms_utime;
#endif

    for (i = 0; i < N; i++)  {
/*  REMEMBER:  return_values[ 0 ] returns the current net error
               learn_parameters[ 0 ] contains the learning parameter (backpropagation)
*/
      ret_code = krui_learnAllPatterns( learn_parameters, no_of_input_params, &return_values, &NoOfReturnVals );
      errChk( ret_code );

      /*  print the return values of the learning function  */
      if ((i % step == 0) || i == (N - 1))  {
        fprintf( stdout, "\nCycle: %d\nLearning function value(s): ", i + 1 );
        for (j = 0; j < NoOfReturnVals; j++ )
	  fprintf( stdout, "[%d]: %f  ", j + 1, return_values[ j ] );
        fprintf( stdout, "\n" );
      }
    }
  }


#ifdef  __BORLANDC__
  cpu_time = (double) (clock() - init_time) / (double) CLK_TCK;
#else
  (void) times( &tp );
  cpu_time = (double) (tp.tms_utime - init_time) / (double) HZ_value;
#endif

  fprintf( stdout, "\n\nNo. of units updated: %.0f\n", (double) no_of_units * (double) N * (double) no_of_patterns );
  fprintf( stdout, "No. of sites updated: %.0f\n", (double) no_of_sites * (double) N * (double) no_of_patterns);
  fprintf( stdout, "No. of links updated: %.0f\n", (double) no_of_links * (double) N * (double) no_of_patterns);
  fprintf( stdout, "CPU Time used: %.2f seconds\n", cpu_time );

  perf = (double) no_of_links * (double) N * (double) no_of_patterns / cpu_time;
  if (choose_me == 1)  {
    fprintf( stdout, "\nNo. of connection per second (CPS): %.4e\n", perf );
  }
  else  {
    fprintf( stdout, "\nNo. of connection updates per second (CUPS or WUPS): %.4e\n", perf );
  }

  /*  before exiting: delete network  */
  krui_deleteNet();

  return( 0 );
}

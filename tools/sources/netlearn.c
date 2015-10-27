/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/netlearn.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Backpropagation Test Program
  NOTES          :

  AUTHOR         : Niels Mache 
  DATE           : 19.10.90

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:10 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*  SNNS-Kernel constants and data type definitions  */
#include "glob_typ.h"
/*  SNNS-Kernel User-Interface Function Prototypes   */
#include "kr_ui.h"


static void  errChk( err_code )
int  err_code;
{
  if (err_code != KRERR_NO_ERROR)  {
    printf( "%s\n", krui_error( err_code ));
    exit( 1 );
  }
}

int main()
{
  int   ret_code, N, i, j, no_of_sites, no_of_links, no_of_units,
	no_of_patterns, dummy, NoOfReturnVals, no_of_input_params,
        no_of_output_params, step, set_no;
  char	*netname, file_name[80];
  bool	shuffle;
  float learn_parameters[5], parameterInArray[5], sum_error;
  float *return_values;


  printf( "\n%s\n", krui_getVersion() );
  printf( "----  Network Learning -----\n\n" );

  printf("Filename of the network file: ");
  scanf("%s", file_name);

  printf( "Loading the network ...\n\n" );
  ret_code = krui_loadNet( file_name, &netname );
  errChk( ret_code );
  krui_getNetInfo( &no_of_sites, &no_of_links, &dummy, &dummy );
  no_of_units = krui_getNoOfUnits();
  printf( "Network name: %s\n", netname );
  printf( "No. of units       : %d\n", no_of_units );
  printf( "No. of input units : %d\n", krui_getNoOfTTypeUnits( INPUT ) );
  printf( "No. of output units: %d\n", krui_getNoOfTTypeUnits( OUTPUT ) );
  printf( "No. of sites: %d\n", no_of_sites );
  printf( "No. of links: %d\n\n", no_of_links );
  printf( "Learning function: %s\n", krui_getLearnFunc() );
  printf( "Update function  : %s\n", krui_getUpdateFunc() );

  printf( "\nFilename of the pattern file: " );
  scanf( "%s", file_name );
  printf( "Loading the patterns ...\n\n" );
  ret_code = krui_loadNewPatterns( file_name, &set_no );
  errChk( ret_code );
  no_of_patterns = krui_getNoOfPatterns();
  printf( "No. of patterns: %d\n", no_of_patterns );

  /*  determine the no. of parameters of the current learning function
  */
  (void) krui_getFuncParamInfo( krui_getLearnFunc(), LEARN_FUNC,
				&no_of_input_params, &no_of_output_params );
  printf( "\nThe learning function '%s' needs %d input parameters:\n",
	  krui_getLearnFunc(), no_of_input_params );
  for (i = 0; i < no_of_input_params; i++)  {
     printf( "Parameter [%d]: ", i + 1 );
     scanf( "%f", &learn_parameters[ i ] );
  }

  printf( "\nChoose no. of cycles: " );
  scanf("%d", &N);
  if (N <= 0)  {
     printf( "\nInvalid no. of cycles !\n" );
     exit( 1 );
  }

  printf( "\nShuffle patterns ? (Y/N): " );
  scanf( "%s", file_name );
  shuffle = toupper( file_name[0] ) == 'Y';
  if (shuffle)
    printf( "\nShuffleing of patterns enabled\n" );
  else
    printf( "\nShuffleing of patterns disabled\n" );

  printf( "\nBegin learning ...\n" );

  step = ((N - 1) / 20) + 1;
  for (i = 0; i < N; i++)  {
    if (shuffle)
      {  /*  shuffle patterns every cycle  */
      ret_code = krui_shufflePatterns( TRUE );
      errChk( ret_code );
    }

/*  REMEMBER:  return_values[ 0 ] returns the current net error
               learn_parameters[ 0 ] contains the learning parameter
               learn_parameters[ 1 ] contains the max. devitation to learn
*/
    ret_code = krui_learnAllPatterns( learn_parameters, 1, &return_values, &NoOfReturnVals );
    errChk( ret_code );

    /*	print the return values of the learning function  */
    if ((i % step == 0) || i == (N - 1))  {
      printf( "\nCycle: %d\nLearning function value(s): ", i + 1);
      for (j = 0; j < NoOfReturnVals; j++ )
        printf( "[%d]: %f  ", j + 1, return_values[ j ] );
      printf( "\n" );
    }
  }

  printf( "\nEnd learning ...\n" );
  printf( "\nCalculating the last error of the network ...\n" );

  sum_error = 0.0;
  parameterInArray[0] = 0.0;  /*  set the max. devitation to 0.0  */

/*   ====================== Attention: ==================================
   Formerly the network propagated the patterns here. With the new pattern 
   handling of version 3.2 this can not longer be done without major rewriting 
   of the function krui_testNet. Since this function is not used within SNNS,
   we decided, that it is not worth the effort and dropped the function.
   ==> Anybody who needs it here has to rewrite it himself. Sorry.
*/
/*
  for (i = 1; i <= no_of_patterns; i++)  {
    ret_code = krui_testNet( i,
                             updateParameterArray, 0,
			     parameterInArray, 1,
			     &return_values, &NoOfReturnVals );
    errChk( ret_code );
    sum_error += return_values[0];
  }
  printf( "The error is: %g\n", sum_error );
*/

  /*  save the network	*/
  printf( "\n\nFilename of the network to save: " );
  scanf( "%s", file_name );
  printf( "Saving the network ...\n" );
  ret_code = krui_saveNet( file_name, netname );
  errChk( ret_code );

  /*  before exiting: delete network  */
  krui_deleteNet();

  return( 0 );
}

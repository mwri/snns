/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/isnns.c,v $
  SHORTNAME      : isnns
  SNNS VERSION   : 4.2

  PURPOSE        : interactive control of SNNS
  NOTES          :

  AUTHOR         : original SNNS Version for V 3.0 by Michael Vogt
  DATE           : 06.08.93

  CHANGED BY     : Michael Vogt
  SCCS VERSION   : $Revision: 1.8 $
  LAST CHANGE    : $Date: 1998/03/19 12:25:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "glob_typ.h"   /* SNNS-Kernel constants and data type definitions  */
#include "kr_ui.h"      /* SNNS-Kernel User-Interface Function Prototypes   */

#include "isnns.h"

int pat_set;
int dummyarray[MAX_NO_OF_VAR_DIM];

/*****************************************************************************
  FUNCTION : errChk

  PURPOSE  : Check for errors
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/

static void  errChk(int errNo)
{
    if(errNo != KRERR_NO_ERROR)
    {
	fprintf(stderr, "isnns: %s\n", krui_error(errNo));
	exit(1);
    }
}

/*****************************************************************************
  FUNCTION : read_activation

  PURPOSE  : reads a number of float values from stream into an array
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void read_activation(FILE *stream, int n, float *field)
{
    int i;

    for (i=0; i<n; i++)
    {
	if (fscanf(stream, "%g", &field[i]) != 1)
	{
	    fprintf(stderr, "isnns: not enough values\n");
	    exit(1);
	}  
    }
}

/*****************************************************************************
  FUNCTION : write_activation

  PURPOSE  : writes a number of float values from an array into stream
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void write_activation(FILE *stream, int n, float *field)
{
    int i;

    for (i=0; i<n; i++)
	fprintf(stream, "%g ", field[i]);
}

/*****************************************************************************
  FUNCTION : set_unit_activation

  PURPOSE  : sets the activation of some units from array
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void set_unit_activation(float *pat, int start, int end)
{
    int i, j;

    j = 0;
    for (i=start; i<=end; i++, j++)
    {
	(void) krui_setUnitActivation(i, (FlintType) pat[j]);
    }
}

/*****************************************************************************
  FUNCTION : get_unit_activation

  PURPOSE  : reads the output of some units into an array
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void get_unit_activation(float *pat, int start, int end)
{
    int i, j;

    j = 0;
    for (i=start; i<=end; i++, j++)
    {
	pat[j] = (float) krui_getUnitOutput(i);
    }
}

/*****************************************************************************
  FUNCTION : reset_pattern_handling

  PURPOSE  : resets the pattern handling for a new loaded network
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void reset_pattern_handling(void)
{
    if (pat_set != -1)
    {
	errChk(krui_deletePatSet(pat_set));
	pat_set = -1;
    }
}

/*****************************************************************************
  FUNCTION : define_pattern

  PURPOSE  : defines the actual activation of the units as the only one pattern
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void define_pattern(void)
{
    if (pat_set == -1)
	errChk(krui_allocNewPatternSet(&pat_set));
    else
    {
	errChk(krui_setPatternNo(1));
	errChk(krui_deletePattern());
    }

    errChk(krui_newPattern());

    /* since the pattern has no variable dimensions the following */
    /* functions may be called with dummyarray arguments with don't */
    /* care contents */ 
    errChk(krui_DefShowSubPat(dummyarray, dummyarray, dummyarray, dummyarray));
    errChk(krui_DefTrainSubPat(dummyarray, dummyarray, dummyarray,
			       dummyarray, NULL));
}

/*****************************************************************************
  FUNCTION : propagate

  PURPOSE  : propagates the only one pattern through the network
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void propagate(void)
{
    float params[10];

    errChk(krui_setPatternNo(1));
    errChk(krui_showPattern(OUTPUT_NOTHING));
    errChk(krui_updateNet(params, 0));
}

/*****************************************************************************
  FUNCTION : log_pattern

  PURPOSE  : logs the actual pattern into file

  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/

void log_pattern(FILE *patout, int pattern, float *inpat, int unit_input, 
		 float *outpat, int unit_output)
{
    int i;

    fprintf(patout, "# %d\n", pattern);
    for (i=0; i<unit_input; i++)
	fprintf(patout, "%g ", *inpat++);
    fprintf(patout, "\n");
    for (i=0; i<unit_output; i++)
	fprintf(patout, "%g ", *outpat++);
    fprintf(patout, "\n\n");
    fflush(patout);
}

/*****************************************************************************
  FUNCTION : learn_one_pattern

  PURPOSE  : propagates the only one pattern through the network and performs
             one step of the learning function
  RETURNS  : 
  NOTES    : the parameter l is transfered as the only one parameter into
             the learning function

  UPDATE   : 
******************************************************************************/

static float learn_one_pattern(float l)
{
    float params[25];
    float *result;
    int   result_no;
    int i;

    for (i=0; i<25; i++)
	params[i] = 0.0;

    params[0] = l;

    errChk(krui_learnSinglePattern(1, params, 1, &result, &result_no));

    return *result;
}

/*****************************************************************************
  FUNCTION : lookup_command

  PURPOSE  : searches the command string in the command table
  RETURNS  : the symbolic command or C_UNKNOWN
  NOTES    :

  UPDATE   : 
******************************************************************************/

static int lookup_command(char *command)
{
    int i;

    for (i=0; i<c_table_no; i++)
    {
	if (strcmp(c_table[i].c_string, command) == 0)
	    return c_table[i].c;
    }

    return C_UNKNOWN;
}


/*****************************************************************************
  FUNCTION : main

  PURPOSE  : perform interactive processing with the SNNS kernel
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

int main(int argc, char  *argv[] )
{
    char command[80];
    char *netname;
    char netfilename[200];
    int stop;
    FILE *infile;
    FILE *outfile;
    FILE *patout;
    int i;
    int ret, error, verb, pattern;
    float *inpat;
    float *outpat;
    float learn_rate;
    time_t clock;
    int isnnscmd;

    int unit_n;
    int unit_input;
    int unit_output;
    int unit_input_start;
    int unit_output_start;
    int unit_input_end;
    int unit_output_end;
    int cu;

    infile = stdin;
    outfile = stdout;
    stop = 0;
    verb = 0;
    pattern = 0;

    pat_set = -1;

    if (argc>1)
    {
	patout = fopen(argv[1], "w");
	if (patout == (FILE *) NULL)
	{
	    fprintf(stderr, "isnns: can't open pattern file %s for writing\n",
		    argv[1]);
	    exit(1);
	}
	pattern = 1;
    }

    error = 0;

    do
    {
	fprintf(outfile, "%s", error ? "notok> " : "ok> ");
	fflush(outfile);
	error = 0;

	if (fscanf(infile, "%s", command) != 1)
	    isnnscmd = C_QUIT;
	else
	    isnnscmd = lookup_command(command);

	switch(isnnscmd)
	{
	  case C_LOAD:
	    fscanf(infile, "%s", netfilename);
	    ret = krui_loadNet(netfilename, &netname);
	    errChk(ret);
	    if (verb)
	    {
		fprintf(outfile,"Network '%s' loaded from file '%s'\n", 
			netname, netfilename);
	    }
	    unit_n = krui_getNoOfUnits();
	    unit_input = 0;
	    unit_output = 0;
	    cu = krui_getFirstUnit();
	    for (i=0; i<unit_n; i++)
	    {
		switch(krui_getUnitTType(cu))
		{
		  case INPUT:
		    if (unit_input == 0)
		    {
			unit_input_start = cu;
		    }
		    unit_input++;
		    unit_input_end = cu;
		    break;
		  case OUTPUT:
		    if (unit_output == 0)
		    {
			unit_output_start = cu;
		    }
		    unit_output++;
		    unit_output_end = cu;
		    break;
		  default:
		    break;
		}
		cu = krui_getNextUnit();
	    }

	    inpat = (float *) malloc(unit_input * sizeof(float));
	    outpat = (float *) malloc(unit_output * sizeof(float));

	    if (unit_input != unit_input_end - unit_input_start + 1 ||
		unit_output != unit_output_end - unit_output_start + 1)
	    {
		fprintf(stderr, "unit_n: %d\n", unit_n);
		fprintf(stderr, "unit_input: %d\n", unit_input);
		fprintf(stderr, "unit_input_start: %d\n", unit_input_start);
		fprintf(stderr, "unit_input_end: %d\n", unit_input_end);
		fprintf(stderr, "unit_output: %d\n", unit_output);
		fprintf(stderr, "unit_output_start: %d\n", unit_output_start);
		fprintf(stderr, "unit_output_end: %d\n", unit_output_end);
		fprintf(stderr, "isnns: inconsitent unit structure\n");
		exit(1);
	    }

	    fprintf(outfile, "%d %d\n", unit_input, unit_output);

	    reset_pattern_handling();
	    if (pattern)
	    {
		fprintf(patout, 
			"SNNS pattern definition file V3.0\n");
		clock = time((time_t *) NULL);
		fprintf(patout, "generated at %s\n\n", (char *) ctime(&clock));
		fprintf(patout, 
			"No. of patterns : ?????\n");
		fprintf(patout, 
			"No. of input units : %d\n", unit_input);
		fprintf(patout, 
			"No. of output units : %d\n\n", unit_output);
	    }
	    break;

	  case C_SAVE:
	    fscanf(infile, "%s", netfilename);
	    ret = krui_saveNet(netfilename, netname);
	    errChk(ret);
	    break;

	  case C_PROPAGATE:
	    read_activation(infile, unit_input, inpat);
	    set_unit_activation(inpat, unit_input_start, unit_input_end);
	    define_pattern();
	    propagate();
	    get_unit_activation(outpat, unit_output_start, unit_output_end);
	    write_activation(outfile, unit_output, outpat);
	    fprintf(outfile, "\n");
	    break;

	  case C_BACKPROP:
	    if (fscanf(infile, "%g", &learn_rate) != 1)
	    {
		fprintf(stderr, "isnns: illegal learning rate\n");
		error = 1;
		break;
	    }
	    read_activation(infile, unit_output, outpat);
	    set_unit_activation(inpat, unit_input_start, unit_input_end);
	    set_unit_activation(outpat, unit_output_start, unit_output_end);
	    define_pattern();
	    if (pattern)
	    {
		log_pattern(patout, pattern, inpat, unit_input, 
			    outpat, unit_output);
		pattern++;
	    }
	    fprintf(outfile, "%g \n", learn_one_pattern(learn_rate));
	    break;

	  case C_LEARN:
	    if (fscanf(infile, "%g", &learn_rate) != 1)
	    {
		fprintf(stderr, "isnns: illegal learning rate\n");
		error = 1;
		break;
	    }
	    read_activation(infile, unit_input, inpat);
	    read_activation(infile, unit_output, outpat);
	    set_unit_activation(inpat, unit_input_start, unit_input_end);
	    set_unit_activation(outpat, unit_output_start, unit_output_end);
	    define_pattern();
	    if (pattern)
	    {
		log_pattern(patout, pattern, inpat, unit_input, 
			    outpat, unit_output);
		pattern++;
	    }
	    fprintf(outfile, "%g \n", learn_one_pattern(learn_rate));
	    break;

	  case C_QUIT:
	    stop = 1;
	    break;

	  case C_UNKNOWN:
	    fprintf(stderr, "isnns: unrecognized command %s\n", command);
	    error = 1;
	    /* no break! fall through to C_HELP */

	  case C_HELP:
	    fprintf(stderr, "available commands:       \n");
	    fprintf(stderr, "load <net file name>      \n");
	    fprintf(stderr, "\t-> load this network\n");
	    fprintf(stderr, "save <net file name>      \n");
	    fprintf(stderr, "\t-> save this network\n");
	    fprintf(stderr, "prop <i_1> ... <i_n>\n");
	    fprintf(stderr, "\t-> propagate the given input pattern.\n");
	    fprintf(stderr, "\t-> returns the network output pattern.\n");
	    fprintf(stderr, "train <lr> <o_1> ... <o_m>\n");
	    fprintf(stderr, 
		    "\t-> performs one supervised training step with CURRENT\n");
	    fprintf(stderr, 
		    "\t-> input pattern and the GIVEN output pattern.\n");
	    fprintf(stderr, "\t-> <lr> is the learning rate.\n");
	    fprintf(stderr, "\t-> returns the error for this training step\n");

	    fprintf(stderr, "learn <lr> <i_1> ... <i_n> <o_1> ... <o_m> \n");
	    fprintf(stderr, 
		    "\t-> performs one supervised training step with GIVEN\n");
	    fprintf(stderr, 
		    "\t-> input pattern and the GIVEN output pattern.\n");
	    fprintf(stderr, "\t-> <lr> is the learning rate.\n");
	    fprintf(stderr, "\t-> returns the error for this training step\n");
	    fprintf(stderr, "quit\n");
	    fprintf(stderr, "\t-> quits isnns\n");
	    fprintf(stderr, "help\n");
	    fprintf(stderr, "\t-> this help\n");
	    break;
	  default:
	    /* never reached because of lookup_command */
	    break;
	}
    } while (!stop);

    fprintf(outfile, "%s", error ? "notok>\n" : "ok>\n");
    fclose(outfile);
    fclose(infile);

    exit(error);
}

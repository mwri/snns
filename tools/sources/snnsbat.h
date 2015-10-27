/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/snnsbat.h,v $
  SNNS VERSION   : 4.2

  PURPOSE        : snnsbat function prototypes 
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 16.03.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.11 $ 
  LAST CHANGE    : $Date: 1998/03/02 17:58:03 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _SNNSBAT_DEFINED_
#define  _SNNSBAT_DEFINED_

/* begin global definition section */

/* end global definition section */

/* begin private definition section */

/*****************/
/*    defines    */
/*****************/

#define SAVE_PERIOD 1800  		/* save temp file every 1800 sec.    */
#define NO_OF_CYCLES_DISPLAYED  100 	/*  display not more than 100 cycles */

#ifndef PATH_MAX	            	/*  longest path name  		     */
#define PATH_MAX 512
#endif

#define ABNORMAL  TRUE
#define NORMAL	  FALSE

#define IsKey(x,y) (strcmp(x,y) == 0)

#define PR_ALL_PATTERNS -1

/*****************/
/*    globals    */
/*****************/


static FILE     *file_out = NULL,
                *file_in = NULL;
static char     *tmp_file1, *tmp_file2, *pr_tmp_file, *netname,
	     	log_file[PATH_MAX+1];
static long    	starttime, HZ_value;
static struct tms  tp;
static int  	no_of_sites, no_of_links, no_of_units, cycle,
            	no_of_patterns, no_of_return_values, tmp_switch,
	        no_of_cycles,
		no_of_pr_retrain_cycles,
            	no_of_learn_param, no_of_init_param, no_of_update_param,
                no_of_var_dim[2],
            	start_pattern, end_pattern,
	    	shuffle, shuffleSubPat,
            	input_included, output_included,
            	SubPatternISize_param[5],
            	SubPatternOSize_param[5],
            	SubPatternIStep_param[5],
            	SubPatternOStep_param[5],
            	exec_loop = 0,
                learn_pat_set,test_pat_set;
static float 	*return_values,
	        learn_param[NO_OF_LEARN_PARAMS],
            	init_param[NO_OF_INIT_PARAMS],
                update_param[NO_OF_UPDATE_PARAMS],
            	best_net_error,
		max_pr_error_increase,
                pr_accepted_error,
                pr_obs_init_parameter;
static char 	config_file[PATH_MAX+1],
            	net_file[PATH_MAX+1],
            	trained_net_file[PATH_MAX+1],
            	learn_pat_file[PATH_MAX+1],
            	test_pat_file[PATH_MAX+1],
            	res_file[PATH_MAX+1],
            	init_func[80],
            	init_param_string[PATH_MAX+1],
                update_param_string[PATH_MAX+1],
            	learn_param_string[PATH_MAX+1],
            	SubPatternISize_string[PATH_MAX+1],
            	SubPatternOSize_string[PATH_MAX+1],
            	SubPatternIStep_string[PATH_MAX+1],
            	SubPatternOStep_string[PATH_MAX+1];
static time_t  	lasttime;
static int  	InitParamFlag = 0,
            	LearnParamFlag = 0,
                UpdateParamFlag = 0,
            	NoOfInitParamFlag = 0,
            	NoOfLearnParamFlag = 0,
                NoOfUpdateParamFlag = 0,
            	NoOfVarDimFlag = 0,
                SubPatternISizeFlag =0,
                SubPatternOSizeFlag =0,
                SubPatternIStepFlag =0,
                SubPatternOStepFlag =0,
            	NetworkFileFlag = 0,
            	TrainedNetworkFileFlag = 0,
            	LearnPatternFileFlag = 0,
            	TestPatternFileFlag = 0,
            	ResultFileFlag = 0,
            	InitFunctionFlag = 0,
            	MaxLearnCyclesFlag = 0,
		MaxErrorToStopFlag = 0,
            	ResultMinMaxPatternFlag = 0,
            	ShuffleFlag = 0,
            	ShuffleSubPatFlag = 0,
            	ResultIncludeInputFlag = 0,
            	ResultIncludeOutputFlag = 0,
            	PatternWarning = 0,
            	SyntaxError = 0,
            	SyntaxWarning = 0;
           

/*****************/
/*  prototypes   */
/*****************/

static bool  skipComments(register FILE *stream);
static void  skipRestOfLine(register FILE  *stream);
static void  errChk(int errNo);
static void  writeLearnFuncValues(void);
static void  writeInfo(FILE *stream);
static void  writeStatistics(bool status);
static void  writeCaughtMsg(int);
RETSIGTYPE   emergency_exit(int);
RETSIGTYPE   exit_batchlearn(int);
static void  catchSignals(void);
static int   IsOld(char *stream);
static int   getYesNoSwitch(FILE  *file_in);
static void  do_initial_log(void);
static void configError(int err_code, char* err_string);
static bool scanParameter(void *field, char  *string, int number, bool isInt);
static int parseFile(int op_code);
static void timeCheck (void);
static void trainNet(void);
static float trainFFNet (int no_of_cycles_ff);
static void pruneNet ();
static void computeResultfile(void);

/* end private definition section */

#endif 



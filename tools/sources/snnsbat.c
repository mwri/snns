/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/snnsbat.c,v $
  SHORTNAME      : snnsbat
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Batchlearning Program
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 24.07.92

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.25 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:12 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#include <time.h>
#include <string.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/param.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/utsname.h>

#ifndef SIGIOT
#define SIGIOT SIGABRT
#endif

#include "glob_typ.h"   /* SNNS-Kernel constants and data type definitions  */
#include "kr_ui.h"      /* SNNS-Kernel User-Interface Function Prototypes   */
#include "kr_typ.h"
#include "kr_newpattern.h"
#include "prun_f.h"

#include "snnsbat.h"


#ifdef NeXT
char *tempnam(dir,pfx)
char *dir, *pfx;
{
char *filename,*start,*t;
int n,count;

filename = (char*)malloc(MAXPATHLEN);
if((dir == NULL)&&(pfx == NULL))
    tmpnam(filename);
if((dir == NULL)&&(pfx != NULL))
    {
    t=tmpnam(NULL);
    start = rindex(t,'/');
    *start = '\0';
    start++;
    sprintf(filename,"%s/%s%s",t,pfx,start);
    }
if(dir != NULL)
    {
    t=tmpnam(NULL);
    start = rindex(t,'/');
    *start = '\0';
    start++;
    if(pfx != NULL)
        {
        if(dir[strlen(dir)-1] != '/')
            sprintf(filename,"%s/%s%s",dir,pfx,start);
        else
            sprintf(filename,"%s%s%s",dir,pfx,start);
        }
    else
        {
        if(dir[strlen(dir)-1] != '/')
            sprintf(filename,"%s/%s",dir,start);
        else
            sprintf(filename,"%s%s",dir,start);
        }

    }
return(filename);
}
#endif /* NeXT */

/*****************************************************************************
  FUNCTION : skipComments

  PURPOSE  : to skip comments
  NOTES    :

  AUTHOR   : Niels Mache
  RETURNS  : FALSE, if EOF is detected
  UPDATE   : 
******************************************************************************/
static	bool skipComments(register FILE *stream)
{
    register int   c;

    while(TRUE){
	do{ c = getc(stream);}
	while(isspace(c));

	if(c != '#')break;

	do{ c = getc(stream);}while(c != EOF && c != '\n');
    }

    if(c == EOF)  
	return(FALSE);

    ungetc(c,stream);
    return( TRUE );
}



/*****************************************************************************
  FUNCTION : skipRestOfLine

  PURPOSE  : to skip the rest of a line
  NOTES    :

  AUTHOR   : Michael Vogt
  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void skipRestOfLine(register FILE  *stream)
{
    register int   c;

    do
    {
	c = getc(stream);
    } while (c != EOF && c != '\n');
}



/*****************************************************************************
  FUNCTION : errChk

  PURPOSE  : Check for errors
  NOTES    :

  AUTHOR   : Niels Mache
  RETURNS  : Error code
  UPDATE   : 
******************************************************************************/
static void  errChk(int errNo)
{
    if(errNo != KRERR_NO_ERROR){
	fprintf(stderr, "Batchlearning: %s\n", krui_error(errNo));
	fflush(stderr);
	if(file_out != NULL){
	    fprintf(file_out,"Batchlearning: %s\n",krui_error(errNo));
	    fflush(file_out);
	}
	exit(1);
    }
}



/*****************************************************************************
  FUNCTION : writeLearnFuncValues

  PURPOSE  : write the learning function values to the log file
  NOTES    :

  AUTHOR   : Niels Mache
  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void  writeLearnFuncValues()
{

    if(return_values != NULL){
	fprintf(file_out,"  %5d:    %10.5f", cycle + 1,return_values[0]);
	fprintf(file_out,"     %10.5f",
		(return_values[0]/krui_getNoOfPatterns()));
	if (krui_getNoOfOutputUnits() > 0)
	    fprintf(file_out,"        %10.5f\n", 
		    (return_values[0]/krui_getNoOfOutputUnits()));
	else
	    fprintf(file_out,"\n");
	fflush(file_out);
    }
}



/*****************************************************************************
  FUNCTION : writeInfo

  PURPOSE  : write system information
  NOTES    :

  AUTHOR   : Niels Mache
  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void  writeInfo(FILE *stream)
{
    time_t	   clock;

    if(stream == NULL)
	return;

    clock = 1;
    (void) time(&clock);
    
    fprintf(stream, "\n%s Batchlearning terminated at %s",
	    krui_getVersion(), ctime(&clock));
    fprintf(stream,"Node: %s\n",getenv("HOST") ? getenv("HOST") : "unknown");
    fflush(stream);
}



/*****************************************************************************
  FUNCTION : writeStatistics

  PURPOSE  : write Statistics
  NOTES    :

  AUTHOR   : Niels Mache
  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void  writeStatistics(bool status)
{
    time_t  clock;
    float cpu_time;


    if(file_out != NULL){
	clock = 1;
	(void)time(&clock);
	(void)times(&tp);
	cpu_time = (float)tp.tms_utime / (float)HZ_value;

	writeInfo(file_out);
	fprintf(file_out, 
		"********************************************************\n");
	fprintf(file_out,"\n----   STATISTICS  ----\n");
	fprintf(file_out,"No. of learned cycles: %d\n",cycle);
	if(status)  
	    writeLearnFuncValues();
	fprintf(file_out,"No. of units updated : %d\n", 
		no_of_units * cycle * no_of_patterns);
	fprintf(file_out,"No. of sites updated : %d\n", 
		no_of_sites * cycle * no_of_patterns);
	fprintf(file_out,"No. of links updated : %d\n", 
		no_of_links * cycle * no_of_patterns);
	fprintf(file_out,"CPU Time used: %.2f seconds\n",cpu_time);
	fprintf(file_out,"User time: %ld seconds\n",clock-starttime);
	fprintf(file_out,"No. of connection updates per second (CUPS): %e\n\n",
		((double)no_of_links * (double)cycle * 
		 (double)no_of_patterns) / ((double)cpu_time));
	fflush(stdout);
    }
}




/*****************************************************************************
  FUNCTION : writeCaughtMsg

  PURPOSE  : log the number of a caught signal
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void  writeCaughtMsg(int msg)
{
  fprintf( stdout, "\nSignal %d caught, %s Batchlearning terminated.\n",
	  msg, krui_getVersion() );
  writeInfo( stdout );

  if(file_out != NULL){
    fprintf(file_out, 
	    "\n#####  Signal %d caught, %s Batchlearning terminated.\n",
	    msg, krui_getVersion() );
    writeInfo( file_out );
  }
}



/*****************************************************************************
  FUNCTION : emergency_exit

  PURPOSE  : exit after signal
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
RETSIGTYPE emergency_exit(int msg)
{
    writeCaughtMsg(msg);

    fprintf(stdout,"\nFatal Error in SNNS-Kernel. Networkfile is lost.\n");

    if(file_out != NULL) {
	fprintf( stdout, 
		"\n#####  Fatal Error in SNNS-Kernel. Networkfile is lost.\n");
	writeStatistics(ABNORMAL);
	fprintf(stdout, "Logfile '%s' written.\n",log_file);
	fclose(file_out);
    }

    exit(-3);
}




/*****************************************************************************
  FUNCTION : exit_batchlearn

  PURPOSE  : handle signals
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
RETSIGTYPE exit_batchlearn(int msg)
{
    char *tmp_file;


    writeCaughtMsg(msg);

    if(krui_getNoOfUnits() > 0){ /*  save network  */
	if(tmp_switch & 1)
	    tmp_file = tmp_file1;
	else
	    tmp_file = tmp_file2;

	(void) krui_saveNet(tmp_file,netname);
	fprintf( stdout, "Networkfile '%s' saved.\n",tmp_file);
	if(file_out != NULL)
	    fprintf(file_out,"Networkfile '%s' saved.\n",tmp_file);
    }

    if(file_out != NULL){
	writeStatistics(ABNORMAL);
	fprintf(stdout,"Logfile '%s' written.\n",log_file);
	fclose(file_out);
    }

    exit(-2);
}



/*****************************************************************************
  FUNCTION : catchSignals

  PURPOSE  : catch these signals
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void  catchSignals()
{
  signal( SIGHUP,  exit_batchlearn );  
  signal( SIGINT,  exit_batchlearn );
  signal( SIGQUIT, exit_batchlearn );
  signal( SIGALRM, exit_batchlearn );
  signal( SIGTERM, exit_batchlearn );

#ifdef SIGPWR
  signal( SIGPWR,  exit_batchlearn );
#endif

  signal( SIGILL,  emergency_exit );
  signal( SIGTRAP, emergency_exit );
  signal( SIGIOT,  emergency_exit );
#ifdef SIGEMT
  signal( SIGEMT,  emergency_exit );
#endif
  signal( SIGFPE,  emergency_exit );
#ifdef SIGBUS
  signal( SIGBUS,  emergency_exit );
#endif
  signal( SIGSEGV, emergency_exit );
}



/*****************************************************************************
  FUNCTION : IsOld

  PURPOSE  : test an input string against the string <OLD>
  RETURNS  : 1 if string matches
  NOTES    :

  UPDATE   : 
******************************************************************************/
static int IsOld(char *stream)
{
    char temp[80];
    int  i=0,j=0;

    strcpy(temp,"<OLD>");
    while(stream[i] == ' ' || stream[i] == '\t')i++; 
    for(j=0; j<5; j++)temp[j] = toupper(stream[i+j]);

    if(strcmp(temp,"<OLD>") == 0)return(1);

    return(0);

}


/*****************************************************************************
  FUNCTION : getYesNoSwitch

  PURPOSE  : read a YES/NO switch
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static int  getYesNoSwitch(FILE  *file_in)
{
    int  i, ret;
    char do_shuffleing[80];


    skipComments(file_in);
    ret = fscanf(file_in,"%s",do_shuffleing);
    if(ret != 1)return(-2);

    if(IsOld(do_shuffleing))
	return(-1);

    for(i=0; i<(size_t)strlen(do_shuffleing); i++)
	do_shuffleing[i] = toupper(do_shuffleing[i]);

    if(strcmp(do_shuffleing,"YES") == 0)return(1);
    if(strcmp(do_shuffleing,"NO") != 0)return(-2);

    return(0);
}



/*****************************************************************************
  FUNCTION : do_initial_log 

  PURPOSE  : log the initial settings
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void do_initial_log()
{

    int i, ret;


    /* log network info */

    krui_getNetInfo(&no_of_sites,&no_of_links,&ret,&ret);
    no_of_units = krui_getNoOfUnits();
    fprintf( file_out,"Network name             : %s\n", netname );
    fprintf( file_out,"No. of units             : %d\n", no_of_units );
    fprintf( file_out,"No. of sites             : %d\n", no_of_sites );
    fprintf( file_out,"No. of links             : %d\n", no_of_links );
    fprintf( file_out,"\nLearning Function        : %s\n",krui_getLearnFunc());
    fprintf( file_out,"\nUpdate Function          : %s\n",krui_getUpdateFunc());
    if (krui_checkPruning ())
    {
	fprintf( file_out,"\nPruning Function         : %s\n",krui_getPrunFunc());
	fprintf( file_out,"\nSubordinate Function     : %s\n",krui_getFFLearnFunc());
    }
    for(i=0; i<no_of_learn_param; i++)
	fprintf(file_out,"Learning Parameter #%d    : %f\n",i+1,
		learn_param[i]);
    fprintf(file_out,"\n");


    /* log variable Dimensions */

    if(NoOfVarDimFlag != 0){
	fprintf(file_out,"\nUsing variable sub-patterns\n");
	fprintf(file_out,"Input Dimensions         : ");
	for(i=0; i<no_of_var_dim[0]; i++)
	    fprintf(file_out,"%d ",SubPatternISize_param[i]);
	fprintf(file_out,"\nOutput Dimensions        : ");
	for(i=0; i<no_of_var_dim[1]; i++)
	    fprintf(file_out,"%d ",SubPatternOSize_param[i]);
	fprintf(file_out,"\nInput Step Size          : ");
	for(i=0; i<no_of_var_dim[0]; i++)
	    fprintf(file_out,"%d ",SubPatternIStep_param[i]);
	fprintf(file_out,"\nOutput Step Size         : ");
	for(i=0; i<no_of_var_dim[1]; i++)
	    fprintf(file_out,"%d ",SubPatternOStep_param[i]);
	fprintf(file_out,"\n");
    }


    /* log test pattern file */

    if(test_pat_file[0] != '\0')
	fprintf(file_out,"\nTest pattern file : '%s'\n",test_pat_file);


    /* log initialization settings */

    if(init_func[0] == '\0'){
	fprintf( file_out, "\nNo Initialization \n");
    }else{
	fprintf( file_out,"\nInit Function: %s\n",init_func);
	for(i=0; i<no_of_init_param; i++)
	    fprintf(file_out,"Init Parameter #%d       : %f\n",i+1,
		    init_param[i]);
    }
    fprintf(file_out,"\n");

    /* log update settings  */

    for(i=0; i<no_of_update_param; i++)
	fprintf(file_out,"Update Parameter #%d       : %f\n",i+1,
		update_param[i]);
    
    fprintf(file_out,"\n");

 
    /* log result file settings */

    if(res_file[0] == '\0'){
	fprintf(file_out,"No Result File \n");
    }else{
	fprintf(file_out,"\nResult File              : '%s'\n",res_file);
	fprintf(file_out,"Result File Start Pattern: %d\n",start_pattern);
	fprintf(file_out,"Result File End Pattern  : %d\n",end_pattern);
	if(input_included)
	    fprintf(file_out,"Result File Input Pattern included\n");
	if(output_included)
	    fprintf(file_out,"Result File Output Pattern included\n");
	fprintf(file_out, "\n" );
    }
    fprintf(file_out, 
	    "\n********************************************************\n");

    fflush(file_out);
}




/*****************************************************************************
  FUNCTION : configError

  PURPOSE  : report errors in configuration file
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void configError(int err_code, char* err_string)
{
    switch(err_code){
    case 1:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"Configuration file must first disclose type\n" );
	SyntaxError += 1;
	exit(1);
    case 2:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"This program handles only filetypes%s and%s not %s\n",
		" SNNSBATCH_1"," SNNSBATCH_2",err_string);
	SyntaxError += 1;
	exit(1);
    case 3:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"Wrong number of learn params given !!\n");
	SyntaxError += 1;
	break;
    case 4:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"Wrong number of %s params given !!\n",err_string);
	SyntaxError += 1;
	break;
    case 5:
	fprintf(stderr,"\n\nWARNING !!\n");
	fprintf(stderr,"Found unknown key in configuration file!!\n");
	fprintf(stderr,"\nIgnoring unknown key '%s'!!\n",	err_string);
	SyntaxWarning += 1;
	break;
    case 6:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"No pattern file for training specified in loop %d!!\n",
		exec_loop+1);
	SyntaxError += 1;
	break;
    case 7:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"Invalid argument for key %s\n",err_string);
	SyntaxError += 1;
	break;
    case 8:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"%s Value <OLD> not allowed in first loop\n",
		err_string);
	SyntaxError += 1;
	break;
    case 9:
	fprintf(stderr,"\n\nWARNING !!\n");
	fprintf(stderr,"Value <OLD> for key LearnPatternFile: in loop %d ",
		exec_loop+1);
	fprintf(stderr,"\nuses loaded TEST pattern file of previous loop\n\n");
	SyntaxWarning += 1;
	break;
    case 10:
	fprintf(stderr,"\n\nWARNING !!\n");
	fprintf(stderr,"Result file parameters specified without file name");
	fprintf(stderr," in loop %d!!\n",exec_loop);
	fprintf(stderr,"Result file will not be generated !!\n\n");
	SyntaxWarning += 1;
	break;
    case 11:
	fprintf(stderr,"\n\nWARNING !!\n");
	fprintf(stderr,"Init function parameters specified without function ");
	fprintf(stderr,"name in loop %d!!\n",exec_loop);
	fprintf(stderr,"Net will not be initialized in loop %d!!\n",
		exec_loop);
	SyntaxWarning += 1;
	break;
    case 12:
	fprintf(stderr,"\n\nWARNING !!\n");
	if(strcmp(err_string,"LEARN")==0){
	    fprintf(stderr,"Not more than %d Learning parameters allowed.\n",
		    NO_OF_LEARN_PARAMS);
	    fprintf(stderr,"Only %d  are used !!\n",NO_OF_LEARN_PARAMS);
	}else if (strcmp(err_string,"UPDATE")==0){
	    fprintf(stderr,"Not more than %d Update parameters allowed.\n",
		    NO_OF_UPDATE_PARAMS);
	    fprintf(stderr,"Only %d are used !!\n",NO_OF_UPDATE_PARAMS);

	}else{
	    fprintf(stderr,"Not more than %d Init parameters allowed.\n",
		    NO_OF_INIT_PARAMS);
	    fprintf(stderr,"Only %d are used !!\n",NO_OF_INIT_PARAMS);
	}
	SyntaxWarning += 1;
	break;
    case 13:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"No more memory available\n");
	exit(1);
    case 14:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"Loop %d: New network can not use old Patterns\n",
		exec_loop);
	SyntaxError += 1;
	break;
    case 15:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"Loop %d: Subpattern: No Size specified\n",exec_loop);
	SyntaxError += 1;
	break;
    case 16:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"Loop %d: Subpattern: No Step specified\n",exec_loop);
	SyntaxError += 1;
	break;
    case 17:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"Subpattern: Need two %s Dimension values\n",
		err_string);
	SyntaxError += 1;
	break;
    case 18:
	fprintf(stderr,"\n\nERROR !!\n");
	fprintf(stderr,"No number of variable pattern sizes given for ");
	fprintf(stderr,"subpatterns in loop %d\n",exec_loop);
	SyntaxError += 1;
	break;
    default:
	fprintf(stderr,"\n\nDon't known what to make of unknown parsing ");
	fprintf(stderr,"error %d\nAborting scan\n",err_code);
	exit(1);
    }
}





/*****************************************************************************
  FUNCTION : scanParameter

  PURPOSE  : Read in a number of values in one line
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static bool scanParameter(void *field, char  *string, int number, bool isInt)
{
    register int i, ret;
    char dummy[80];
    char copy[80];

    strcpy(copy, string);
    for(i=0; i<number; i++){
	if(isInt)
	    ret = sscanf(copy,"%d %[^\n]", ((int *)field)+i, dummy);
	else
	    ret = sscanf(copy,"%f %[^\n]", ((float *)field)+i, dummy);
	strcpy(copy, dummy);
	if(((ret!=2) && (i<number-1)) || ((ret==2) && (i==number-1))){
	    return FALSE;
	}
    }
    return TRUE;
}



/*****************************************************************************
  FUNCTION : parseFile

  PURPOSE  : read the configuration file for snnsbat
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static int parseFile(int op_code)
{

    int   ret,dum_ret;
    char  key[80],type[80],arg_string[80];
    float result_param[2];


    /* do configuration file scan */

    skipComments(file_in);

    if(exec_loop == 0){
	ret = fscanf(file_in,"%s",key);
	if(ret != 1 || !IsKey(key,"Type:")){
	    /* no file type given */
	    configError(1,"");
	}
	ret = fscanf(file_in,"%s",type);
	if((strcmp(type,"SNNSBATCH_1") && strcmp(type,"SNNSBATCH_2"))){
	    /* wrong file type given */
	    configError(2,type);
	}
	skipComments(file_in);
    }

    while((ret = fscanf(file_in,"%s",key)) != EOF){

	if(IsKey(key,"NoOfLearnParam:")){
	    NoOfLearnParamFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(ret != 1)
		configError(7,"NoOfLearnParam:");
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"NoOfLearnParam:");
	    }else{
		ret = sscanf(arg_string,"%d",&no_of_learn_param);
		if(no_of_learn_param > NO_OF_LEARN_PARAMS){
		    if(!op_code)configError(12,"LEARN");
		    no_of_learn_param = NO_OF_LEARN_PARAMS;
		}
	    }
	    if(learn_param_string[0] != '\0'){
		if (!scanParameter(learn_param, learn_param_string, 
				   no_of_learn_param,FALSE))
		    configError(3,"");
		learn_param_string[0] = '\0';
	    }

	}else if(IsKey(key,"LearnParam:")){
	    LearnParamFlag = 1;
	    if(!no_of_learn_param){ /* value not read yet */
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"LearnParam:");
		}else
		    ret = sscanf(arg_string,"%[^\n]",learn_param_string);
		if(ret != 1)
		    configError(7,"LearnParam:");
	    }else{
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"LearnParam:");
		}else{
		    if (!scanParameter(learn_param, arg_string, 
				       no_of_learn_param,FALSE))
			configError(3,"");
		}
		learn_param_string[0] = '\0';
	    }

	}else if(IsKey(key,"NoOfInitParam:")){
	    NoOfInitParamFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(ret != 1)
		configError(7,"NoOfInitParam:");
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"NoOfInitParam:");
	    }else{
		ret = sscanf(arg_string,"%d",&no_of_init_param);
		if(no_of_init_param > NO_OF_INIT_PARAMS){
		    if(!op_code)configError(12,"INIT");
		    no_of_init_param = NO_OF_INIT_PARAMS;
		}
	    }
	    if(init_param_string[0] != '\0'){
		if (!scanParameter(init_param, init_param_string,
				   no_of_init_param,FALSE))
		    configError(4,"init");
		init_param_string[0] = '\0';
	    }

	}else if(IsKey(key,"InitParam:")){
	    InitParamFlag = 1;
	    if(!no_of_init_param){ /* value not read yet */
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"InitParam:");
		}else
		    ret = sscanf(arg_string,"%[^\n]",init_param_string);
		if(ret != 1)
		    configError(7,"InitParam:");
	    }else{
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"InitParam:");
		}else{
		    if (!scanParameter(init_param, arg_string, 
				       no_of_init_param,FALSE))
			configError(4,"init");
		}
		init_param_string[0] = '\0';
	    }

	}else if(IsKey(key,"NoOfUpdateParam:")){
	    NoOfUpdateParamFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(ret != 1)
		configError(7,"NoOfUpdateParam:");
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"NoOfUpdateParam:");
	    }else{
		ret = sscanf(arg_string,"%d",&no_of_update_param);
 		if(no_of_update_param > NO_OF_UPDATE_PARAMS){
		    if(!op_code)configError(12,"UPDATE");
		    no_of_update_param = NO_OF_UPDATE_PARAMS;
		}
	    }
	    if(update_param_string[0] != '\0'){
		if (!scanParameter(update_param, update_param_string,
				   no_of_update_param,FALSE))
		    configError(4,"update");
		update_param_string[0] = '\0';
	    }

	}else if(IsKey(key,"UpdateParam:")){
	    UpdateParamFlag = 1;
	    if(!no_of_update_param){ /* value not read yet */
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"UpdateParam:");
		}else
		    ret = sscanf(arg_string,"%[^\n]",update_param_string);
		if(ret != 1)
		    configError(7,"UpdateParam:");
	    }else{
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"UpdateParam:");
		}else{
		    if (!scanParameter(update_param, arg_string, 
				       no_of_update_param,FALSE))
			configError(4,"update");
		}
		update_param_string[0] = '\0';
	    }

	}else if(IsKey(key,"SubPatternISize:")){
	    SubPatternISizeFlag = 1;
	    if(NoOfVarDimFlag == 0){ /* value not read yet */
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"SubPatternISize:");
		}else
		    ret = sscanf(arg_string,"%[^\n]",SubPatternISize_string);
		if(ret != 1)
		    configError(7,"SubPatternISize:");
	    }else{
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"SubPatternISize:");
		}else{
		    if (!scanParameter(SubPatternISize_param, arg_string, 
				       no_of_var_dim[0],TRUE))
			configError(4,"SubPatternISize:");
		}
		SubPatternISize_string[0] = '\0';
	    }

	}else if(IsKey(key,"SubPatternOSize:")){
	    SubPatternOSizeFlag = 1;
	    if(NoOfVarDimFlag == 0){ /* value not read yet */
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"SubPatternOSize:");
		}else
		    ret = sscanf(arg_string,"%[^\n]",SubPatternOSize_string);
		if(ret != 1)
		    configError(7,"SubPatternOSize:");
	    }else{
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"SubPatternOSize:");
		}else{
		    if (!scanParameter(SubPatternOSize_param, arg_string, 
				       no_of_var_dim[1],TRUE))
			configError(4,"SubPatternOSize");
		}
		SubPatternOSize_string[0] = '\0';
	    }

	}else if(IsKey(key,"SubPatternIStep:")){
	    SubPatternIStepFlag = 1;
	    if(NoOfVarDimFlag == 0){ /* value not read yet */
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"SubPatternIStep:");
		}else
		    ret = sscanf(arg_string,"%[^\n]",SubPatternIStep_string);
		if(ret != 1)
		    configError(7,"SubPatternIStep:");
	    }else{
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"SubPatternIStep:");
		}else{
		    if (!scanParameter(SubPatternIStep_param, arg_string, 
				       no_of_var_dim[0],TRUE))
			configError(4,"SubPatternIStep");
		}
		SubPatternIStep_string[0] = '\0';
	    }

	}else if(IsKey(key,"SubPatternOStep:")){
	    SubPatternOStepFlag = 1;
	    if(NoOfVarDimFlag == 0){ /* value not read yet */
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"SubPatternOStep:");
		}else
		    ret = sscanf(arg_string,"%[^\n]",SubPatternOStep_string);
		if(ret != 1)
		    configError(7,"SubPatternOStep:");
	    }else{
		ret = fscanf(file_in,"%[^\n]",arg_string);
		if(IsOld(arg_string)){
		    if(exec_loop == 0)configError(8,"SubPatternOStep:");
		}else{
		    if (!scanParameter(SubPatternOStep_param, arg_string, 
				       no_of_var_dim[1],TRUE))
			configError(4,"SubPatternOStep");
		}
		SubPatternOStep_string[0] = '\0';
	    }

	}else if(IsKey(key,"NoOfVarDim:")){
	    NoOfVarDimFlag = 1;
	    ret = fscanf(file_in,"%[^\n]",arg_string);
	    if(ret != 1)
		configError(7,"NoOfVarDim:");
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"NoOfVarDim:");
	    }else{
		ret = sscanf(arg_string,"%d %d",&no_of_var_dim[0],
			                        &no_of_var_dim[1]);
		if(ret != 2)configError(17,"variable");
	    }
	    if(SubPatternISize_string[0] != '\0'){
		if(!scanParameter(SubPatternISize_param,SubPatternISize_string,
				   no_of_var_dim[0],TRUE))
		    configError(4,"SubPatternISize");
		SubPatternISize_string[0] = '\0';
	    }
	    if(SubPatternOSize_string[0] != '\0'){
		if(!scanParameter(SubPatternOSize_param,SubPatternOSize_string,
				   no_of_var_dim[1],TRUE))
		    configError(4,"SubPatternOSize");
		SubPatternOSize_string[0] = '\0';
	    }
	    if(SubPatternIStep_string[0] != '\0'){
		if(!scanParameter(SubPatternIStep_param,SubPatternIStep_string,
				   no_of_var_dim[0],TRUE))
		    configError(4,"SubPatternIStep");
		SubPatternIStep_string[0] = '\0';
	    }
	    if(SubPatternOStep_string[0] != '\0'){
		if(!scanParameter(SubPatternOStep_param,SubPatternOStep_string,
				   no_of_var_dim[1],TRUE))
		    configError(4,"SubPatternOStep");
		SubPatternOStep_string[0] = '\0';
	    }

	}else if(IsKey(key,"NetworkFile:")){
	    NetworkFileFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		NetworkFileFlag = -1;
		if(exec_loop == 0)configError(8,"NetworkFile:");
	    }else
		strcpy(net_file, arg_string);

	}else if(IsKey(key,"TrainedNetworkFile:")){
	    TrainedNetworkFileFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		TrainedNetworkFileFlag = -1;
		if(exec_loop == 0)configError(8,"TrainedNetworkFile:");
	    }else
		strcpy(trained_net_file, arg_string);

	}else if(IsKey(key,"LearnPatternFile:")){
	    LearnPatternFileFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		LearnPatternFileFlag = -1;
		if(exec_loop == 0)configError(8,"LearnPatternFile:");
		if(PatternWarning && !op_code)configError(9,"");
	    }else
		strcpy(learn_pat_file, arg_string);

	}else if(IsKey(key,"TestPatternFile:")){
	    TestPatternFileFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"TestPatternFile:");
	    }else
		strcpy(test_pat_file, arg_string);

	}else if(IsKey(key,"ResultFile:")){
	    ResultFileFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"ResultFile:");
	    }else
		strcpy(res_file, arg_string);

	}else if(IsKey(key,"InitFunction:")){
	    InitFunctionFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"InitFunction:");
	    }else
		strcpy(init_func, arg_string);

	}else if(IsKey(key,"MaxLearnCycles:")){
	    MaxLearnCyclesFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"MaxLearnCycles:");
	    }else
	        ret = sscanf(arg_string,"%d",&no_of_cycles);
	    if(ret != 1)
		configError(7,"MaxLearnCycles:");

	}else if(IsKey(key,"PruningMaxRetrainCycles:")){
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"PruningMaxRetrainCycles:");
	    }else
	        ret = sscanf(arg_string,"%d",&no_of_pr_retrain_cycles);
	    if(ret != 1)
		configError(7,"PruningMaxRetrainCycles:");

	}else if(IsKey(key,"MaxErrorToStop:")){
	    MaxErrorToStopFlag = 1;
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"MaxErrorToStop:");
	    }else
	        ret = sscanf(arg_string,"%f",&best_net_error);
	    if(ret != 1)
		configError(7,"MaxErrorToStop:");

	}else if(IsKey(key,"PruningMaxErrorIncrease:")){
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"PruningMaxErrorIncrease:");
	    }else
	        ret = sscanf(arg_string,"%g",&max_pr_error_increase);
	    if(ret != 1)
		configError(7,"PruningMaxErrorIncrease:");

	}else if(IsKey(key,"PruningAcceptedError:")){
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"PruningAcceptedError:");
	    }else
	        ret = sscanf(arg_string,"%g",&pr_accepted_error);
	    if(ret != 1)
		configError(7,"PruningAcceptedError:");

	}else if(IsKey(key,"PruningRecreate:")){
	    dum_ret = getYesNoSwitch(file_in);
	    if(dum_ret == -1){
		if(exec_loop == 0)configError(8,"PruningRecreate:");
	    }else
		pr_recreate = dum_ret;
	    if(pr_recreate < -1)
		configError(7,"PruningRecreate:");

	}else if(IsKey(key,"PruningOBSInitParam:")){
	    ret = fscanf(file_in,"%s",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"PruningOBSInitParam:");
	    }else{
	        ret = sscanf(arg_string,"%g",&pr_obs_init_parameter);
		pr_obs_setInitParameter (pr_obs_init_parameter);
	    }
	    if(ret != 1)
		configError(7,"PruningOBSInitParam:");

	}else if(IsKey(key,"PruningInputPruning:")){
	    dum_ret = getYesNoSwitch(file_in);
	    if(dum_ret == -1){
		if(exec_loop == 0)configError(8,"PruningInputPruning:");
	    }else
		pr_setInputPruning (dum_ret);

	}else if(IsKey(key,"PruningHiddenPruning:")){
	    dum_ret = getYesNoSwitch(file_in);
	    if(dum_ret == -1){
		if(exec_loop == 0)configError(8,"PruningHiddenPruning:");
	    }else
		pr_setHiddenPruning (dum_ret);

	}else if(IsKey(key,"Shuffle:")){
	    ShuffleFlag = 1;
	    dum_ret = getYesNoSwitch(file_in);
	    if(dum_ret == -1){
		if(exec_loop == 0)configError(8,"Shuffle:");
	    }else
		shuffle = dum_ret;
	    if(shuffle < -1)
		configError(7,"Shuffle:");

	}else if(IsKey(key,"ShuffleSubPat:")){
	    ShuffleSubPatFlag = 1;
	    dum_ret = getYesNoSwitch(file_in);
	    if(dum_ret == -1){
		if(exec_loop == 0)configError(8,"ShuffleSubPat:");
	    }else
		shuffleSubPat = dum_ret;
	    if(shuffleSubPat < -1)
		configError(7,"ShuffleSubPat:");

	}else if(IsKey(key,"ResultMinMaxPattern:")){
	    ResultMinMaxPatternFlag = 1;
	    ret = fscanf(file_in,"%[^\n]",arg_string);
	    if(IsOld(arg_string)){
		if(exec_loop == 0)configError(8,"ResultMinMaxPattern:");
	    }else{
		if(scanParameter(result_param,arg_string,2,FALSE)){
		    start_pattern = (int)result_param[0];
		    end_pattern = (int)result_param[1];
		}else{
		    configError(7,"ResultMinMaxPattern:");
		}
	    }
	}else if(IsKey(key,"ResultIncludeInput:")){
	    ResultIncludeInputFlag = 1;
	    dum_ret = getYesNoSwitch(file_in);
	    if(dum_ret == -1){
		if(exec_loop == 0)configError(8,"ResultIncludeInput:");
	    }else
		input_included = dum_ret;
	    if(input_included < -1)
		configError(7,"ResultIncludeInput:");

	}else if(IsKey(key,"ResultIncludeOutput:")){
	    ResultIncludeOutputFlag = 1;
	    dum_ret = getYesNoSwitch(file_in);
	    if(dum_ret == -1){
		if(exec_loop == 0)configError(8,"ResultIncludeOutput:");
	    }else
		output_included = dum_ret;
	    if(output_included < -1)
		configError(7,"ResultIncludeOutput:");

	}else if(IsKey(key,"PerformActions:")){
	    if( (no_of_var_dim[1] > 0 && !SubPatternOStepFlag) ||
		(no_of_var_dim[0] > 0 && !SubPatternIStepFlag))
		configError(16," ");
	    if( (no_of_var_dim[1] > 0 && !SubPatternOSizeFlag) ||
		(no_of_var_dim[0] > 0 && !SubPatternISizeFlag))
		configError(15," ");
	    if(!NoOfVarDimFlag && (SubPatternISizeFlag || SubPatternOSizeFlag ||
				   SubPatternIStepFlag || SubPatternOStepFlag))
		configError(18," ");

	    if(!LearnPatternFileFlag && no_of_cycles)
		configError(6,"");
	    PatternWarning = 0;
	    if(TestPatternFileFlag)
		PatternWarning = 1;

	    exec_loop += 1;
	    if(op_code /* == read */){
		return(0);
	    }else{

		if(LearnPatternFileFlag == -1 && NetworkFileFlag != -1)
		    configError(14,"");

		if(!ResultFileFlag && (ResultMinMaxPatternFlag ||
		   ResultIncludeInputFlag || ResultIncludeOutputFlag))
		    configError(10,"");

		if(!InitFunctionFlag &&
	           (InitParamFlag || NoOfInitParamFlag))
		    configError(11,"");

		/* reset keywords for correct syntax check */
		update_param_string[0]    = '\0';
		init_param_string[0]      = '\0';
		learn_param_string[0]     = '\0';
		SubPatternISize_string[0] = '\0';
		SubPatternOSize_string[0] = '\0';
		SubPatternIStep_string[0] = '\0';
		SubPatternISizeFlag       = 0;
		SubPatternOSizeFlag       = 0;
		SubPatternIStepFlag       = 0;
		SubPatternOStepFlag       = 0;
		NoOfVarDimFlag            = 0;
		no_of_update_param        = 0;
		no_of_init_param          = 0;
		no_of_learn_param         = 0;
		net_file[0]               = '\0';
		trained_net_file[0]       = '\0';
		learn_pat_file[0]         = '\0';
		test_pat_file[0]          = '\0';
		res_file[0]               = '\0';
		init_func[0]              = '\0';
		no_of_cycles              = 0;
		best_net_error            = 0.0;
		LearnPatternFileFlag      = 0;
		TestPatternFileFlag       = 0;
                ResultFileFlag            = 0;
                ResultMinMaxPatternFlag   = 0;
                ResultIncludeInputFlag    = 0;
                ResultIncludeOutputFlag   = 0;
		UpdateParamFlag           = 0;
		NoOfUpdateParamFlag       = 0;
                InitFunctionFlag          = 0;
                InitParamFlag             = 0;
                NoOfInitParamFlag         = 0;
	    }
	}else{
	    /* Found unknown key in configuration file */
	    skipRestOfLine(file_in);
	    configError(5,key);
	}
	skipComments(file_in);

    }/* end while */

    exec_loop += 1;
    if(!(op_code) /* == parse */){

	if(PatternWarning && IsOld(learn_pat_file))
	    configError(9,"");

	if( (no_of_var_dim[1] > 0 && !SubPatternOStepFlag) ||
	   (no_of_var_dim[0] > 0 && !SubPatternIStepFlag))
	    configError(16," ");
	if( (no_of_var_dim[1] > 0 && !SubPatternOSizeFlag) ||
	   (no_of_var_dim[0] > 0 && !SubPatternISizeFlag))
	    configError(15," ");
	if(!NoOfVarDimFlag && (SubPatternISizeFlag || SubPatternOSizeFlag ||
			       SubPatternIStepFlag || SubPatternOStepFlag))
	    configError(18," ");

	rewind(file_in);

	if(!ResultFileFlag && (ResultMinMaxPatternFlag ||
	   ResultIncludeInputFlag || ResultIncludeOutputFlag))
	    configError(10,"");

	if(!InitFunctionFlag && (InitParamFlag || NoOfInitParamFlag))
	    configError(11,"");


	/* reset keywords for correct subsequent read */

	update_param_string[0]= '\0';
	init_param_string[0]  = '\0';
	learn_param_string[0] = '\0';
	no_of_update_param    = 0;
	no_of_init_param      = 0;
	no_of_learn_param     = 0;
	net_file[0]           = '\0';
	trained_net_file[0]   = '\0';
	learn_pat_file[0]     = '\0';
	test_pat_file[0]      = '\0';
	res_file[0]           = '\0';
	init_func[0]          = '\0';
	no_of_cycles          = 0;
	best_net_error        = 0.0;
    }
    return(1);
}


/*****************************************************************************
  FUNCTION : saveTmp

  PURPOSE  : save network
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void saveTmp ()

{

    int ret;

    if(++tmp_switch & 1)
    {
	ret = krui_saveNet(tmp_file1,netname);
	fprintf (file_out,
		 "\n#####  Saved to 1st Temporary network file.  #####\n\n");
	fflush (file_out);
    }	
    else
    {
	ret = krui_saveNet(tmp_file2,netname);
	fprintf (file_out,
		 "\n#####  Saved to 2nd Temporary network file.  #####\n\n");
	fflush (file_out);
    }	
    errChk(ret);
    
    if(tmp_switch == 1){
	fprintf(file_out,
		"\n#####  Temporary network file '%s' created.  #####\n\n",
		tmp_file1);
	fflush(file_out);
    }
    if(tmp_switch == 2){
	fprintf( file_out,
		"\n#####  Temporary network file '%s' created.  #####\n\n",
		tmp_file2);
	fflush(file_out);
    }
    lasttime = tp.tms_utime;
    
}


/*****************************************************************************
  FUNCTION : timeCheck

  PURPOSE  : save network if necessary
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void timeCheck ()

{

    /* get user cpu time */
    (void) times(&tp);
    
    if(((tp.tms_utime-lasttime) / HZ_value) >= SAVE_PERIOD)
	/*  save temporary network  */
	saveTmp ();

}


/*****************************************************************************
  FUNCTION : trainNet

  PURPOSE  : perform network training
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void trainNet()
{

    int ret,step;


    fprintf(file_out,
	    "\n  epoch:         SSE            MSE            SSE/o-units\n\n");
    
    step = ((no_of_cycles-1) / NO_OF_CYCLES_DISPLAYED)+1;
    while((cycle < no_of_cycles) && 
	  ((cycle && (return_values[0] > best_net_error)) || !cycle)){
	if(shuffle){		/*  shuffle patterns every cycle  */
	    ret = krui_shufflePatterns(TRUE);
	    errChk(ret);
	}
	if(shuffleSubPat){	/*  shuffle subpatterns every cycle  */
	    ret = krui_shuffleSubPatterns(TRUE);
	    errChk(ret);
	}

	/**************************************************************
	*  REMEMBER:  return_values[0] returns the current net error  *
	*  learn_param[0] contains the 1st learning parameter         *
	**************************************************************/
	ret = krui_learnAllPatterns(learn_param, NO_OF_LEARN_PARAMS, 
				    &return_values, &no_of_return_values );
	errChk(ret);

	/* print the return values of the learning function  */
	if ((cycle % step == 0) || cycle == no_of_cycles)
	    writeLearnFuncValues();

	timeCheck ();

	cycle++;
    }


    /* print the return values of the learning function  */
    if (cycle < no_of_cycles)
	writeLearnFuncValues();


}



/*****************************************************************************
  FUNCTION : trainFFNet

  PURPOSE  : perform network training with subordinate learn function
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static float trainFFNet (int no_of_cycles_ff)

{

    int ret, step;


    errChk (krui_DefTrainSubPat 
	    (SubPatternISize_param, SubPatternOSize_param,
	     SubPatternIStep_param, SubPatternOStep_param, NULL));

    cycle = 0;
    step = ((no_of_cycles_ff-1) / NO_OF_CYCLES_DISPLAYED)+1;
    while ((cycle < no_of_cycles_ff) && 
	   ((cycle && (return_values [0] > best_net_error)) || !cycle))
    {
	if (shuffle)
	    /*  shuffle patterns every cycle  */
	{
	    ret = krui_shufflePatterns (TRUE);
	    errChk (ret);
	}

	if (shuffleSubPat)
	    /*  shuffle subpatterns every cycle  */
	{
	    ret = krui_shuffleSubPatterns (TRUE);
	    errChk (ret);
	}

	/* call subordinate learn function */
	ret = krui_learnAllPatternsFF (learn_param, NO_OF_LEARN_PARAMS, 
				       &return_values, &no_of_return_values);
	errChk (ret);

	/* print the return values of the learning function  */
	if ((cycle % step == 0) || cycle == no_of_cycles)
	    writeLearnFuncValues ();

	timeCheck ();
	
	cycle++;
    }


    /* print the return values of the learning function  */
    if (cycle < no_of_cycles)
	writeLearnFuncValues ();

    return (return_values [0]);

}


/*****************************************************************************
  FUNCTION : pruneNet

  PURPOSE  : perform network training with subordinate learn function
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/

static void pruneNet ()

{

    float first_error, max_error, net_error;
    krui_err err;


    /* return if no training/pruning should take place */

    if(no_of_cycles == 0)
	return;


    /* display message */
    fprintf (file_out, "*** first training ***\n");

    /* train network into minimum */ 
    first_error = trainFFNet (no_of_cycles);
    if (first_error < 0)
	return;

    /* store maximum error */
    max_error = first_error * (1 + max_pr_error_increase / 100);
    if (max_error < pr_accepted_error)
	max_error = pr_accepted_error;

    if (pr_recreate)
	/* create temporary file */
	pr_tmp_file = tempnam ("./","SNNS_");

    do
	/* prune links until error is beyond maximum error */
    {
	if (pr_recreate)
	{
	    /* display message */
	    fprintf (file_out, "*** saving net ***\n");

	    /* save net */
	    krui_saveNet (pr_tmp_file, netname);
	}

	/* display message */
	fprintf (file_out, "*** pruning ***\n");

	/* delete some weight */
	err = pr_callPrunFunc (PR_ALL_PATTERNS);
	errChk (err);

	timeCheck ();

	/* calculate net error */
	err = pr_calcMeanDeviation (PR_ALL_PATTERNS, &net_error);	     
	errChk (err);
	if (net_error > best_net_error)
	{
	    /* display message */
	    fprintf (file_out, "*** retraining ***\n");

	    /* retrain network */
	    net_error = trainFFNet (no_of_pr_retrain_cycles);
	}
    }
    while (net_error <= max_error);

    if (pr_recreate)
    {
	/* display message */
	fprintf (file_out, "*** reloading net ***\n");

	/* reload last saved net */
	krui_loadNet (pr_tmp_file, &netname);

	/* delete temporary file */
	unlink (pr_tmp_file);
    }

    /* display message */
    fprintf (file_out, "*** end of pruning ***\n");
    fflush(file_out);

    return;

}


/*****************************************************************************
  FUNCTION : computeResultfile

  PURPOSE  : perform result file computation
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void computeResultfile()
{

    int ret;


    if(test_pat_file[0] == '\0' && learn_pat_file[0] == '\0'){
	fprintf(stderr,"No pattern file present !!\n");
	exit(0);
    }

    /* load test pattern file */

    if(test_pat_file[0] != '\0' && strcmp(test_pat_file,learn_pat_file) != 0){ 
	if(exec_loop > 1){
	    ret = krui_deletePatSet(test_pat_set);
	errChk(ret);
	}
	ret = krui_loadNewPatterns(test_pat_file,&test_pat_set);
	errChk(ret);
	fprintf(file_out,"\n\nTest Pattern File '%s' loaded.\n",test_pat_file);
	no_of_patterns = krui_getNoOfPatterns();
	fprintf(file_out,"No. of test patterns: %d\n\n",no_of_patterns);
    }

    /* create result file */

    if(!start_pattern && !end_pattern){
	start_pattern = 1;
	end_pattern = no_of_patterns;
    }
    errChk(krui_DefTrainSubPat(SubPatternISize_param,SubPatternOSize_param,
			       SubPatternIStep_param,SubPatternOStep_param,
			       NULL));
    ret = krui_saveResultParam(res_file,TRUE,start_pattern,end_pattern,
			       input_included,output_included,update_param, 
			       NO_OF_UPDATE_PARAMS);
    errChk(ret);
    fprintf(file_out,"\nResult file saved.\n");
    fflush(file_out);
}




/*****************************************************************************
  FUNCTION : main

  PURPOSE  : perform batch processing with the SNNS kernel
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int main(int argc, char  *argv[] )
{
    int	        ret, i,
                pError,
                notLast=1;
    time_t	clock;



    /* set configuration and log file names */

    switch (argc)  {
      case 1:
	strcpy( config_file, "snnsbat.cfg" );
	strcpy( log_file, "snnsbat.log" );
	break;
      case 2:
	strcpy( config_file, argv[1] );
	strcpy( log_file, "snnsbat.log" );
	break;
      case 3:
	strcpy( config_file, argv[1] );
	strcpy( log_file, argv[2] );
	break;
      default:
	fprintf( stderr, "Usage: %s [config_file [log_file]]\n", argv[0] );
	exit( 1 );
    }


    /* get HZ value */

    if((getenv("HZ") == NULL) || (sscanf(getenv("HZ"),"%ld",&HZ_value)!=1)){
#ifndef HZ
	fprintf( stderr, "****  WARNING: no HZ defined!\n" );
	HZ_value = 100;
#else
	HZ_value = HZ;
#endif
    }


    /*  open configuration file  */

    if((file_in = fopen(config_file,"r")) == NULL){
	fprintf(stderr,"Can't open configuration file '%s'\n",config_file);
	exit(1);
    }


    /* parse configuration files for errors */

    fprintf(stdout,"Parsing the configuration file .... ");fflush(stdout);
    pError = parseFile(0);
    exec_loop = 0;
    if(SyntaxError){
	fprintf(stderr,"\n\nFound %d errors in the configuraton file\n",
		SyntaxError);
	exit(1);
    }
    if(SyntaxWarning)
	fprintf(stderr,"\n\nFound %d problems in the configuraton file\n",
		SyntaxWarning);
    else
	fprintf(stdout,"Correct\n");


    /* open Log-file */

    if((file_out = fopen(log_file,"w")) == NULL){
	fprintf(stderr,"Can't open LOG file\n");
	exit(1);
    }


    /* begin loggin */

    fprintf(stdout,"\n%s\n", krui_getVersion() );
    fprintf(stdout,"-----  SNNS batch execution run  -----\n\n" );
    fprintf(file_out,"%s Batchlearning Program\n",krui_getVersion());
    fprintf(file_out,"Configuration file: '%s'\n",config_file);
    fprintf(file_out,"Log file          : '%s'\n\n",log_file);


    /*  create names for temporary files */

    tmp_file1 = tempnam("./","SNNS_");
    tmp_file2 = tempnam("./","SNNS_");


    /* perform the batch program */

    while(notLast){


	/*  clear all flags */

	InitParamFlag = 0;
	UpdateParamFlag = 0;
	LearnParamFlag = 0;
	NoOfInitParamFlag = 0;
	NoOfUpdateParamFlag = 0;
	NoOfLearnParamFlag = 0;
	NoOfVarDimFlag = 0;
	NetworkFileFlag = 0;
	TrainedNetworkFileFlag = 0;
	LearnPatternFileFlag = 0;
	TestPatternFileFlag = 0;
	ResultFileFlag = 0;
	InitFunctionFlag = 0;
	MaxLearnCyclesFlag = 0;
	ResultMinMaxPatternFlag = 0;
	ShuffleFlag = 0;
	ShuffleSubPatFlag = 0;
	ResultIncludeInputFlag = 0;
	ResultIncludeOutputFlag = 0;


	/* read parameters for next execution loop */

        pError = parseFile(1);
	if(pError)notLast = 0;


	/* clear unused  keyword values */
    
	if(!InitParamFlag){	
	    init_param_string[0]  = '\0';
	    for(i=0; i<NO_OF_INIT_PARAMS; i++) 
	       init_param[i] = 0.0;
	}
	if(!UpdateParamFlag){	
	    update_param_string[0]  = '\0';
	    for(i=0; i<NO_OF_UPDATE_PARAMS; i++) 
		update_param[i] = 0.0;
	}

	if(!LearnParamFlag){	
	    learn_param_string[0] = '\0';
	    for(i=0; i<NO_OF_LEARN_PARAMS; i++) 
	       learn_param[i] = 0.0;
	}
	if(!NoOfInitParamFlag)
	    no_of_init_param      = 0;
	if(!NoOfUpdateParamFlag)
	    no_of_update_param    = 0;
	if(!NoOfLearnParamFlag)
	    no_of_learn_param     = 0;
	if(!NoOfVarDimFlag)
	    no_of_var_dim[0] =  
		no_of_var_dim[1]  = 0;
	if(!NetworkFileFlag)
	    net_file[0]           = '\0';
	if(!TrainedNetworkFileFlag)
	    trained_net_file[0]   = '\0';
	if(!LearnPatternFileFlag)
	    learn_pat_file[0]     = '\0';
	if(!TestPatternFileFlag)
	    test_pat_file[0]      = '\0';
	if(!ResultFileFlag)
	    res_file[0]           = '\0';
	if(!InitFunctionFlag)	
	    init_func[0]          = '\0';
	if(!MaxLearnCyclesFlag)
	    no_of_cycles          = 0;
	if(!MaxErrorToStopFlag)
	    best_net_error        = 0.0;
	if(!ResultMinMaxPatternFlag){
	    start_pattern         = 0;
	    end_pattern           = 0;
	}
	if(!ShuffleFlag)
	    shuffle               = 0;
	if(!ShuffleSubPatFlag)
	    shuffleSubPat         = 0;
	if(!ResultIncludeInputFlag)
	    input_included        = 0;
	if(!ResultIncludeOutputFlag)	
	    output_included       = 1;

	tmp_switch            = 0;
	cycle                 = 0;


	/* dump header */

	fprintf( file_out,"\n\n\n########");
	fprintf( file_out," SNNS batch execution run. Loop %d ", exec_loop );
	fprintf( file_out,"########\n");


	/* load the network */

	if(NetworkFileFlag != -1){
	    ret = krui_loadNet(net_file,&netname);
	    errChk(ret);
	    fprintf(file_out,"Networkfile '%s' loaded.\n",net_file);
	}

    
	/* load learn pattern file */

	if(LearnPatternFileFlag > 0){
	    if(exec_loop > 1){
		ret = krui_deletePatSet(learn_pat_set);
	    errChk(ret);
	    }
	    ret = krui_loadNewPatterns(learn_pat_file,&learn_pat_set);
	    errChk(ret);
	    fprintf(file_out,"Patternfile '%s' loaded.\n",learn_pat_file);
	    no_of_patterns = krui_getNoOfPatterns();
	    fprintf(file_out,"No. of patterns: %d\n\n",no_of_patterns);
	    fprintf(file_out,"No. of cycles: %d\n\n", no_of_cycles);
	    fprintf(file_out,
		    "Max. network error to stop: %f\n\n",best_net_error);
	    if(shuffle)
		fprintf(file_out,"Patterns are shuffled\n");
	    else
		fprintf(file_out,"Patterns are in order\n");
	    if(shuffleSubPat)
		fprintf(file_out,"Subpatterns are shuffled\n");

	    fprintf(file_out,"\n");
	    errChk(krui_DefTrainSubPat(SubPatternISize_param
				       ,SubPatternOSize_param
				       ,SubPatternIStep_param
				       ,SubPatternOStep_param
				       ,NULL));

	}


	/* log the initial settings */

	do_initial_log();


	/*  init cpu time  */

	(void)times(&tp);
	lasttime = tp.tms_utime;


	/* start timer */

	clock = 1;
	(void)time(&clock);
	starttime = clock;
	if(no_of_cycles)
	    fprintf(file_out,"\n%s Batchlearning started at %s\n",
		    krui_getVersion(),ctime(&clock));
	else
	    fprintf(file_out,"\n%s Result file computation started at %s\n",
		    krui_getVersion(),ctime(&clock));

	fflush(file_out);

	/*  catch signals  */

	catchSignals();


	/* initialize net */

	if(init_func[0] != '\0'){
	    ret = krui_setInitialisationFunc(init_func);
	    errChk(ret);
	    ret = krui_initializeNet(init_param,no_of_init_param);
	    errChk(ret);
	    fprintf(file_out, "\nNetwork initialized with\n%s ",init_func);
	    for(i=0; i<no_of_init_param; i++)
		fprintf(file_out,"%4.2f ",init_param[i]);
	    fprintf(file_out,"\n");
	}


	/* train the net */

	if(LearnPatternFileFlag)
	    if (krui_checkPruning ())
		pruneNet ();
	    else
		trainNet();


	/* perform result file computation */

	if(ResultFileFlag)
	    computeResultfile();


	/*  save the network */

	if(trained_net_file[0] == '\0'){
	    fprintf(file_out, "\nTrained Network was not saved \n");
	}else{
	    ret = krui_saveNet(trained_net_file,netname);
	    errChk(ret);
	    fprintf(file_out,"\nNetwork saved to %s.\n",trained_net_file);
	}


	/* remove temporary files */

	if(tmp_switch >= 1){
	    if(unlink(tmp_file1) == 0){
		fprintf(file_out, 
		      "\n#####  Temporary network file '%s' removed.  #####\n",
		       tmp_file1);
		fflush(file_out);
	    }
	    if(tmp_switch > 1){
		if(unlink(tmp_file2) == 0){
		    fprintf(file_out, 
			"#####  Temporary network file '%s' removed.  #####\n",
			    tmp_file2);
		    fflush(file_out);
		}
	    }
	}


	/* write statistics */

	if(LearnPatternFileFlag > 0)
	    writeStatistics(NORMAL);
	

    } /* while(notLast) */

    fclose(file_out);
    fclose(file_in);

    fprintf(stdout,"\n%s Batchlearning has terminated successfully.\n",
	    krui_getVersion());
    fprintf(stdout,"Logfile '%s' written.\n",log_file);
    fflush(file_out);
    return(0);
}

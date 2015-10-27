/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/analyze.c,v $
  SHORTNAME      : analyze.c
  SNNS VERSION   : 4.2

  PURPOSE        : Network Analyzation Tool
  NOTES          :

  AUTHOR         : Stefan Broeckel, Tobias Soyez 
  DATE           : 30.07.92

  CHANGED BY     : Michael Vogt
  RCS VERSION    : $Revision: 2.17 $
  LAST CHANGE    : $Date: 1998/03/17 15:49:54 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* constants                                                                 */
/*****************************************************************************/

#define  WRONG   1
#define  RIGHT   2
#define  UNKNOWN 4
#define  SPECIFIC 8

#define  ON  1
#define  OFF 2

#define  R402040  1
#define  WTA      2
#define  band     3


/*****************************************************************************/
/* type definitions                                                          */
/*****************************************************************************/

typedef struct 
{
  int  no_of_patterns           ;
  int  no_of_input_units        ;
  int  no_of_output_units       ;
  int  startpattern             ;
  int  endpattern               ;
  int  input_pattern_included   ;
  int  teaching_output_included ;
  int  sub_pattern_present      ;
} FileHeaderInfo ;


/*****************************************************************************/
/* global variables                                                          */
/*****************************************************************************/

FILE  *in_file  ;
FILE  *out_file ;



/*****************************************************************************/
/* function Error                                                            */
/*****************************************************************************/
float Error (float *output, float *teaching_output, int no_of_units)
{   
    int    i ;
    float  e, diff ;

    e = 0.0 ;
    for (i = 0 ; i < no_of_units ; i++){
	diff = teaching_output[i] - output[i]  ;
	e    = e + diff * diff                 ;
    }
    
    return e;
}

/*****************************************************************************/
/* function ClassNo                                                          */
/*****************************************************************************/
int ClassNo (float *teaching_output, int no_of_units)
{
    int i;
    float t_max;
    int t_pos;

    t_max = teaching_output[0];
    t_pos = 0;

    for (i = 1 ; i < no_of_units ; i++) {
	if (teaching_output[i] > t_max){  
	    t_max = teaching_output[i] ;
	    t_pos = i ; 
	}
    }

    if (t_max > 0.0)
	return t_pos;
    else
	return -1;
}

/*****************************************************************************/
/* The function F_402040 returns :                                           */
/*                                                                           */
/*    RIGHT   : the output of exactly one unit is >= high               and  */
/*              this unit has the greatest teaching output              and  */
/*              the output of all the other units is <= low                  */
/*                                                                           */
/*    WRONG   : the output of exactly one unit is >= high               and  */
/*              this unit has NOT the greatest teaching output          and  */
/*              the output of all the other units is <= low                  */
/*                                                                           */
/*    UNKNOWN : in any other case                                            */
/*                                                                           */
/* high, low  : parameters of F_402040                                       */
/*              default values: low = 0.4, high = 0.6                        */
/*****************************************************************************/
int F_402040 (float *output, float *teaching_output, int no_of_units, 
	      float high, float low, int *winner)
{
    int    o_pos, t_pos, on, off, i;
    float  t_max;

    on    = 0 ;
    off   = 0 ;
    o_pos = 0 ;
    t_pos = 0 ;
    t_max = teaching_output[t_pos] ;

    for (i = 0 ; i < no_of_units ; i++){
	if(output[i] >= high) { 
	    on++; 
	    o_pos = i; 
	}else if (output[i] <= low)    
	    off++ ;
    
	if (teaching_output[i] > t_max){
	    t_max = teaching_output[i] ;
	    t_pos = i ; 
	} 
    }
 
    if ((on + off == no_of_units) && (on == 1)) {  
	*winner = o_pos;
	if (t_max > 0.0 && o_pos == t_pos)
	    return (RIGHT) ;
	else 
	    return (WRONG) ;
    }else{
	*winner = -1;
	return (UNKNOWN) ;
    }
}



/*****************************************************************************/
/* The function F_WTA returns :                                              */
/*                                                                           */
/*    RIGHT   :  there is exactly one unit j with maximal output a       and */
/*               a > high                                                and */
/*               the output of all the other units is < a - low          and */
/*               the unit j has the greatest teaching output                 */
/*                                                                           */
/*    WRONG   :  there is exactly one unit j with maximal output a       and */
/*               a > high                                                and */
/*               the output of all the other units is < a - low          and */
/*               the unit j has NOT the greatest teaching output             */
/*                                                                           */
/*    UNKNOWN :  in any other case                                           */
/*                                                                           */
/* high, low  : parameters of F_WTA                                          */
/*              default values: low = 0.0, high = 0.0                        */
/*****************************************************************************/
int F_WTA (float *output, float *teaching_output, int no_of_units, int high, 
	   int low, int *winner)
{
    int    i, o_pos, t_pos, no_of_max ;
    float  min, max, max2 , t_max     ;


    o_pos = t_pos = 0;
    t_max = teaching_output[0] ;
    max = min = output[0];
    no_of_max = 1;

    for(i=1; i<no_of_units; i++){
	if(output[i] > max){
	    max       = output[i] ; 
	    o_pos     = i;
	    no_of_max = 1;
	}else if (output[i] == max) 
	    no_of_max++;
	else if (output[i] <  min) 
	    min = output[i] ;

	if (teaching_output[i] > t_max){
	    t_max = teaching_output[i] ;
	    t_pos = i ; 
	} 
    }
   
    max2 = min  ;
    for (i = 1 ; i < no_of_units ; i++) 
	if ((output[i] > max2) && (output[i] < max)) 
	    max2 = output[i] ;

    if ((no_of_max == 1) && (output[o_pos] > high) && (max2  < max - low)){  
	*winner = o_pos;
	if (t_max > 0.0 && o_pos == t_pos)
	    return (RIGHT) ; 
	else 
	    return (WRONG) ;
    }else{
	*winner = -1;
	return (UNKNOWN) ;
    }
}



/*****************************************************************************/
/* The function F_band returns :                                             */
/*                                                                           */
/*    RIGHT   : for all units :                                              */
/*              the output is >= the teaching output - low              and  */
/*              the output is <= the teaching output + high                  */
/*                                                                           */
/*    WRONG   : for all units :                                              */
/*              the output is < the teaching output - low               or   */
/*              the output is > the teaching output + high                   */
/*                                                                           */
/*    UNKNOWN : in any other case                                            */
/*                                                                           */
/* high, low  : parameters of F_band                                         */
/*              default values: low = 0.1, high = 0.1                        */
/*****************************************************************************/
int F_band (float *output, float *teaching_output, int no_of_units, float high,
	    float low)
{
    int found_right = 0;
    int found_wrong = 0;
    int i;

    for (i = 0 ; i < no_of_units ; i++){   
	if ((output[i] <= (teaching_output[i] + high)) &&
	    (output[i] >= (teaching_output[i] - low))) {   
	    if (found_wrong)
		return (UNKNOWN);
	    else
		found_right = 1;
	}else{
	    if (found_right)
		return (UNKNOWN);
	    else
		found_wrong = 1;
	}
    }
    if(found_right)
	return (RIGHT);
    else
	return (WRONG);
}



/*****************************************************************************/
/*  function get_options                                                     */
/*****************************************************************************/
int get_options (int argc, char *argv[], int *function, int *sel_cond, 
		 int *output_text, float *high, float *low, int *statistics, 
		 int *class_statistics, int *confm, int *spec_teach, 
		 int *spec_out)

{
    int error, c, hl_flag;
    extern char  *optarg ;

    *function    = R402040;
    *sel_cond    = 0;
    *output_text = OFF;
    *statistics  = OFF;
    *class_statistics = OFF;
    *confm = OFF;
    in_file = (FILE *) NULL ;
    out_file = (FILE *) NULL ;
    hl_flag = 0;
    error = 0 ;
   
    while ((c = getopt (argc, argv, "awruvcme:i:o:h:l:sS:")) != -1)
	switch (c){  
        case 'l' :
	    sscanf (optarg, "%f", low) ;
	    hl_flag = hl_flag | 1 ;
	    break ;
        case 'h' :
	    sscanf (optarg, "%f", high) ;
	    hl_flag = hl_flag | 2 ;
	    break ;
        case 'e' :
	    if(strcmp (optarg, "402040") == 0) 
		*function = R402040 ;
	    else if(strcmp (optarg, "WTA") == 0)
		*function = WTA    ;
	    else if(strcmp (optarg, "band") == 0)
		*function = band    ;
	    else error++ ; 
	    break ;
        case 'r' :
	    *sel_cond = *sel_cond | RIGHT   ;
	    break ;
        case 'w' :
	    *sel_cond = *sel_cond | WRONG   ;
	    break ;
        case 'u' :
	    *sel_cond = *sel_cond | UNKNOWN ;
	    break ;
        case 'S' :
	    *sel_cond = *sel_cond | SPECIFIC ;
	    if (sscanf(optarg, "%d %d", spec_teach, spec_out) != 2)
		error++;
	    break ;
        case 'a' :
	    *sel_cond = WRONG | RIGHT | UNKNOWN ;
	    break ;
        case 'v' :
	    *output_text = ON ;
	    break ;
        case 's' :
	    *statistics  = ON ;
	    break ;
        case 'c' :                  
	    *class_statistics = ON ;
	    break ;
        case 'm' :                  
	    *confm = ON ;
	    break ;
        case 'i' :
	    if ((in_file = fopen(optarg, "r")) == (FILE *) NULL) {  
		fprintf (stderr, "error:  can't read file %s \n", optarg) ;
		error++ ;
	    } 
	    break ;
        case 'o' :
	    if ((out_file = fopen(optarg, "w")) == (FILE *) NULL){  
		fprintf (stderr, "error:  can't create file %s\n", optarg) ;
		error++ ;
	    } 
	    break ;
        default  : error++ ;
	}

    if (*sel_cond == 0) 
	*sel_cond = WRONG  ;

    if ((hl_flag & 1) == 0){ 
	switch (*function){
	case R402040 : *low  = 0.4 ; break ;
	case WTA     : *low  = 0.0 ; break ;
	case band    : *low  = 0.1 ; break ;
	}
    }

    if ((hl_flag & 2) == 0){
	switch (*function){
	case R402040 : *high = 0.6 ; break ;
	case WTA     : *high = 0.0 ; break ;
	case band    : *high = 0.1 ; break ;
	}
    }
       
    if (in_file  == (FILE *) NULL) 
	in_file  = stdin  ;
    if (out_file == (FILE *) NULL) 
	out_file = stdout ;

    return (error) ;  
}



/*****************************************************************************/
/* function read_file_header                                                 */
/*                                                                           */
/* reads from the input file :                                               */
/*     no. of patterns                                                       */
/*     no. of input units                                                    */
/*     no. of output units                                                   */
/*     startpattern                                                          */
/*     endpattern                                                            */
/*****************************************************************************/
int read_file_header (FileHeaderInfo  *file_header_info)
{
    char  str1[80], str2[80], str3[80] ;

    fscanf (in_file, "%s %s %s", str1, str2, str3) ;
    if((strcmp(str1, "SNNS") != 0) || (strcmp(str2, "result") != 0) ||  
       (strcmp(str3, "file") != 0)){
	fprintf(stderr, "error:  no SNNS result file\n") ;
	return 1;
    }

    fscanf (in_file, "%*s %*s %*s %*s %*s %*s %*s %*s") ;
    fscanf (in_file, "%*s %*s %*s %*s     %d",
	    &(file_header_info->no_of_patterns));
    fscanf (in_file, "%*s %*s %*s %*s %*s %d", 
	    &(file_header_info->no_of_input_units));
    fscanf (in_file, "%*s %*s %*s %*s %*s %d", 
	    &(file_header_info->no_of_output_units));
    fscanf (in_file, "%*s %*s             %d", 
	    &(file_header_info->startpattern));
    fscanf (in_file, "%*s %*s             %d", &(file_header_info->endpattern));
          
    file_header_info -> sub_pattern_present = 
	(file_header_info->endpattern) - (file_header_info->startpattern) + 1 
     	     != (file_header_info -> no_of_patterns);
       
    fscanf (in_file, "%s", str1) ;

    if (strcmp (str1, "input") == 0){   
	fscanf (in_file, "%s %s %s", str2, str3, str1) ;
	if((strcmp(str2, "patterns") == 0) && (strcmp(str3, "included") == 0)) 
	    file_header_info->input_pattern_included = 1 ;   
	else 
	    file_header_info->input_pattern_included = 0 ;
    }
    else 
	file_header_info->input_pattern_included = 0 ;

    if(strcmp(str1, "teaching") == 0){ 
	fscanf(in_file, "%s %s %s", str2, str3, str1) ; 
	if((strcmp(str2, "output") == 0) && (strcmp(str3, "included") == 0)) 
	    file_header_info->teaching_output_included = 1 ;
	else{
	    file_header_info->teaching_output_included = 0 ;
	    fprintf(stderr, "error:  missing teaching_output \n");
	    return 1;
	}   
    }else{
        file_header_info->teaching_output_included = 0 ;
	fprintf (stderr, "error:  missing teaching_output \n") ;
	return 1;
    }   

    return 0;
}



/*****************************************************************************/
/* main program                                                              */
/*****************************************************************************/
int main (int argc, char *argv[])
{
    int   pat_no, i, j, result, function, sel_cond, output_text;
    int   winner, spec_teach, spec_out;
    int   statistics, class_statistics, confm, class_no, right, wrong, unknown;
    int   *class_stat_wrong, *class_stat_right,  *class_stat_unknown ;
    int   **confusion;
    float low, high, error;
    float *output, *teaching_output;
    FileHeaderInfo  file_header_info;

    if (get_options (argc, argv, &function, &sel_cond, &output_text,
		     &high, &low, &statistics, &class_statistics, 
		     &confm, &spec_teach, &spec_out) != 0) {
	fprintf(stderr, "usage: %s [options]        \n", argv[0]) ;
	fprintf(stderr, "analyzes result files which are generated by SNNS\n");
	fprintf(stderr, "options are:\n");
	fprintf(stderr, "\t-w               : report wrong classified ");
	fprintf(stderr, "patterns (default)\n") ;
	fprintf(stderr, 
	        "\t-r               : report right classified patterns\n") ;
	fprintf(stderr,"\t-u               : report unclassified patterns\n") ;
	fprintf(stderr,"\t-a               : same as -w -r -u\n") ;
	fprintf(stderr,"\t-S \"t c\"         : report confusion from class t ");
	fprintf(stderr,"to c (-1 = noclass)\n") ;
	fprintf(stderr,"\t-s               : show statistic information\n") ;
	fprintf(stderr,"\t-c               : show class statistic information\n");
	fprintf(stderr,"\t-m               : show confusion matrix\n") ; 
	fprintf(stderr,"\t-v               : verbous mode\n");
	fprintf(stderr,"\t-e <function>    : select error function \n") ;
	fprintf(stderr,"\t                   <function> = [402040 | WTA | band]\n") ;
	fprintf(stderr,"\t                   default = 402040\n") ;
	fprintf(stderr,"\t-l <float>       : lower bound level (see documentation) \n") ;
	fprintf(stderr,"\t                   default: 0.4 for 402040\n") ;
	fprintf(stderr,"\t                   default: 0.0 for WTA\n") ;
	fprintf(stderr,"\t                   default: 0.1 for band\n") ;
	fprintf(stderr,"\t-h <float>       : upper bound level (see documentation) \n") ;
	fprintf(stderr,"\t                   default: 0.6 for 402040\n") ;
	fprintf(stderr,"\t                   default: 0.0 for WTA\n") ;
	fprintf(stderr,"\t                   default: 0.1 for band\n") ;
	fprintf(stderr,"\t-i <input file>  : input result file (default stdin)\n");
	fprintf(stderr,"\t-o <output file> : output file (default stdout)\n") ;
	return 1;
    }

    if (read_file_header (&file_header_info) != 0) {
	fprintf (stderr, "error:  invalid file header\n") ;
	return 1;
    }

    output = (float *) malloc (file_header_info.no_of_output_units 
			       * sizeof(float)) ;
    teaching_output = (float *) malloc (file_header_info.no_of_output_units 
					* sizeof(float)) ;
    class_stat_wrong = (int *) malloc (file_header_info.no_of_output_units 
				       * sizeof(int)) ; 
    class_stat_right = (int *) malloc (file_header_info.no_of_output_units 
				       * sizeof(int)) ;
    class_stat_unknown = (int *) malloc(file_header_info.no_of_output_units 
					* sizeof(int)) ;
    confusion = (int **) malloc((file_header_info.no_of_output_units + 1)
				* sizeof(int *));

    for (i=0; i<= file_header_info.no_of_output_units; i++)	{
	confusion[i] = (int *)malloc((file_header_info.no_of_output_units+1)
				     * sizeof(int));
	for (j=0; j<= file_header_info.no_of_output_units; j++)
	    confusion[i][j] = 0;
    }
	  
    wrong   = 0;
    right   = 0;
    unknown = 0;
    error   = 0.0;
      
    for (i = 0; i < file_header_info.no_of_output_units; i++){ 
	class_stat_wrong[i] = 0;   
	class_stat_right[i] = 0;   
	class_stat_unknown[i] = 0;   
    }

    for (pat_no = 0 ; pat_no < file_header_info.no_of_patterns ; pat_no++){ 
	if (file_header_info.input_pattern_included != 0){
	    for (i = 1 ; i <= file_header_info.no_of_input_units ; i++)
		fscanf (in_file, "%*f") ;
	}

	for (i = 0 ; i < file_header_info.no_of_output_units ; i++)
	    fscanf (in_file, "%f", &teaching_output[i]) ;

	for (i = 0 ; i < file_header_info.no_of_output_units ; i++)
	    fscanf (in_file, "%f", &output[i]) ;
        
	switch (function){
	case R402040 :
	    result = F_402040 (output, teaching_output, 
			       file_header_info.no_of_output_units, 
			       high, low, &winner);
	    break ;
	case WTA :
	    result = F_WTA(output, teaching_output, 
			   file_header_info.no_of_output_units, 
			   high, low, &winner);
	    break ;
	case band :
	    result = F_band(output, teaching_output, 
			    file_header_info.no_of_output_units, high, low);
	    break ;
	}

	if(file_header_info.no_of_output_units > 1)
	    class_no = ClassNo(teaching_output, 
			       file_header_info.no_of_output_units);
	else
	    class_no = abs((int)teaching_output[0]);

	if (class_no >= 0){
	    switch (result){
	    case WRONG:  
		wrong++; 
		class_stat_wrong[class_no]++;
		break;  
	    case RIGHT:  
		right++;
		class_stat_right[class_no]++;
		break;
	    case UNKNOWN:  
		unknown++; 
		class_stat_unknown[class_no]++;
		break;
	    }
	}
	if (function == R402040 || function == WTA){
	    if (class_no >= 0)
		if (result != UNKNOWN)
		    confusion[class_no][winner]++;
		else
		    confusion[class_no]
			[file_header_info.no_of_output_units]++;
	    else
		if (result != UNKNOWN)
		    confusion[file_header_info.no_of_output_units]
			[winner]++;
		else
		    confusion[file_header_info.no_of_output_units]
			[file_header_info.no_of_output_units]++;
	}else{
	    confm = OFF;
	}

	if (statistics == OFF){
	    if (output_text == OFF){
		if((sel_cond & WRONG) == result) 
		    fprintf(out_file,"%d\n",file_header_info.sub_pattern_present
			    ? pat_no+1 : pat_no+file_header_info.startpattern);
		else if ((sel_cond & RIGHT) == result) 
		    fprintf(out_file,"%d\n",file_header_info.sub_pattern_present
			    ? pat_no+1 : pat_no+file_header_info.startpattern);
		else if ((sel_cond & UNKNOWN) == result)
		    fprintf(out_file,"%d\n",file_header_info.sub_pattern_present
			    ? pat_no+1 : pat_no+file_header_info.startpattern);
		if ((sel_cond & SPECIFIC) 
		    && spec_teach == class_no && spec_out == winner)
		    fprintf(out_file,"%d\n",file_header_info.sub_pattern_present
			    ? pat_no+1 : pat_no+file_header_info.startpattern);

	    } else {
		if      ((sel_cond & WRONG)   == result) 
		    fprintf(out_file, "wrong   : %d\n", 
			    file_header_info.sub_pattern_present 
			    ? pat_no+1 : pat_no+file_header_info.startpattern);
		else if ((sel_cond & RIGHT)   == result) 
		    fprintf(out_file, "right   : %d\n", 
			    file_header_info.sub_pattern_present 
			    ? pat_no+1 : pat_no+file_header_info.startpattern);
		else if ((sel_cond & UNKNOWN) == result) 
		    fprintf(out_file, "unknown : %d\n", 
			    file_header_info.sub_pattern_present 
			    ? pat_no+1 : pat_no+file_header_info.startpattern);
		if ((sel_cond & SPECIFIC) 
		    && spec_teach == class_no && spec_out == winner)
		    fprintf(out_file, "specific : %d\n", 
			    file_header_info.sub_pattern_present 
			    ? pat_no+1 : pat_no+file_header_info.startpattern);
	    }
	}
	else 
	    error = error + Error (output, teaching_output, 
				   file_header_info.no_of_output_units) ;
	
	if (pat_no < file_header_info.no_of_patterns - 1) 
	    fscanf (in_file, "%*s") ;    
    }

    free (output)          ;
    free (teaching_output) ;

    if (statistics == ON){ 
	fprintf (out_file, "STATISTICS ( %d patterns )\n", 
		 file_header_info.no_of_patterns) ;
	fprintf (out_file, "wrong   : %5.2f %%  ( %d pattern(s) )\n", 
		 100.0*wrong  /file_header_info.no_of_patterns, wrong) ;
	fprintf (out_file, "right   : %5.2f %%  ( %d pattern(s) )\n",
		 100.0*right  /file_header_info.no_of_patterns, right) ;
	fprintf (out_file, "unknown : %5.2f %%  ( %d pattern(s) )\n",
		 100.0*unknown/file_header_info.no_of_patterns, unknown) ;
	fprintf (out_file, "error   : %f\n", error)  ;
    } 
    if (class_statistics == ON){
	fprintf (out_file,"\n\n");
	for (i = 0; i < file_header_info.no_of_output_units; i++){
	    fprintf(out_file,"1ST ORDER STATISTICS FOR CLASS NO. : %d\n",i);
	    fprintf(out_file,"wrong   : %5.2f %%  ( %d pattern(s) )\n",
		    100.0*class_stat_wrong[i]  /(class_stat_wrong[i]+
						 class_stat_right[i]+
						 class_stat_unknown[i]), 
		    class_stat_wrong[i]) ;
	    fprintf (out_file,"right   : %5.2f %%  ( %d pattern(s) )\n",
		     100.0*class_stat_right[i]  /(class_stat_wrong[i]+
						  class_stat_right[i]+
						  class_stat_unknown[i]), 
		     class_stat_right[i]) ;
	    fprintf (out_file,"unknown : %5.2f %%  ( %d pattern(s) )\n",
		     100.0*class_stat_unknown[i]/(class_stat_wrong[i]+
						  class_stat_right[i]+
						  class_stat_unknown[i]), 
		     class_stat_unknown[i]) ;
	    fprintf (out_file,"\n");
	}
    }
    if (confm == ON){
	fprintf(out_file,"\nCONFUSION MATRIX (rows: teaching input, ");
	fprintf(out_file,"colums: classification)\n\n");
	fprintf(out_file, " class ");
	for (i = 0; i < file_header_info.no_of_output_units; i++)
	    fprintf(out_file, "|%5d  ", i);
	fprintf(out_file, "| unknown\n");
	for (i = 0; i < file_header_info.no_of_output_units; i++)
	    fprintf(out_file, "-------+");
	fprintf(out_file, "-------+--------\n");
	for (i = 0; i <= file_header_info.no_of_output_units; i++){
	    if (i < file_header_info.no_of_output_units)
		fprintf(out_file,"%5d  ", i);
	    else
		fprintf(out_file,"noclass");
	    for (j=0; j<= file_header_info.no_of_output_units; j++)
		fprintf(out_file,"|%6d ", confusion[i][j]);
	    fprintf(out_file,"\n");
	}
    }
    return 0;
}

/*****************************************************************************/
/* end of file                                                               */
/*****************************************************************************/


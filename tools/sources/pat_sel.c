/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/pat_sel.c,v $
  SHORTNAME      : pat_sel
  SNNS VERSION   : 4.2

  PURPOSE        : Selects a number of patterns from a given pattern file and
                   writes them out to a new file.
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 11.05.98
  VERSION        : 2.0

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.13 $
  LAST CHANGE    : $Date: 1998/05/20 15:19:37 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include "kr_typ.h"
#include "kernel.h"
#include "glob_typ.h"
#include "kr_ui.h"

/*****************************************************************************
  FUNCTION : mysort

  PURPOSE  : comparison function for qsort
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int mysort(const void *a, const void *b)
{
    if( (int *)a < (int *)b )
        return -1;
    else if( (int *)a > (int *)b )
        return 1;
    else
        return 0;
}


/*****************************************************************************
  FUNCTION :  main

  PURPOSE  : main (only) routine of this module
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void main (int argc, char *argv[])
{ 

    int   pat_set, tot_pat_num;
    int   error, i, j, dummy, act_num;
    int   *pat_no;
    int   virt_was_on = 0, no_of_sel_pat = 0;
    FILE  *in_no_file;
    FILE  *out_pat_file;
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

  
    /* check for proper calling sequence */
    if (argc != 4){  
	fprintf(stderr, "usage: %s <no_file> <in_pat_file> <out_pat_file>\n",
		argv[0]); 
	exit;
    }

    /* is number file readable ? */
    if ((in_no_file = fopen(argv[1], "r")) == NULL) { 
	fprintf(stderr, "error: can't read file %s\n", argv[1]) ;
	exit;
    }
  
    /* load input pattern file */
    if(krui_loadNewPatterns(argv[2],&pat_set) != 0 /*KRERR_NO_ERROR*/){
	fprintf(stderr, "error: can't read file %s\n", argv[2]) ;
	fclose (in_no_file) ;
	return ;
    }
    tot_pat_num = krui_getNoOfPatterns();

    /*check for virtual patterns */
    if((error = krui_GetPatInfo(&patt_info, &descrip))< 0){
	printf("found error %d when testing pattern set %d\n",error);
        exit;
    }
    if(patt_info.class_distrib_active){
	/* we do not want virtual patterns here */
	virt_was_on = 1;
	error = krui_useClassDistribution(FALSE);
    }

    /* check output pattern file */
    if ((out_pat_file = fopen(argv[3], "r")) != NULL) {
	fclose(out_pat_file);
	fprintf (stderr, "overwrite existing file %s (y/n) ? ", argv[3]) ;
	if (getc(stdin) != 'y') exit; 
    }
    if ((out_pat_file = fopen(argv[3], "w")) == NULL){
	fprintf(stderr, "error: can't create output file %s\n", argv[3]) ;
	exit;
    }
    fclose(out_pat_file);
  

    /* check number of entries in the number file */
    while (fscanf(in_no_file, "%d", &dummy) == 1) no_of_sel_pat++ ;
    fseek (in_no_file, 0, 0) ;
    pat_no = (int *)calloc(no_of_sel_pat, sizeof(int));

    /* read and sort numbers of patterns to be selected */
    for(i=0; i<no_of_sel_pat; i++){
	fscanf(in_no_file, "%d", &pat_no[i]);
    }
    fflush(stdout);
    fclose(in_no_file);
    qsort(pat_no, no_of_sel_pat, sizeof(int), mysort);

    /* delete all pattern not mentioned in the list from the pattern set */
    act_num = tot_pat_num;
    for(i=no_of_sel_pat-1; i>=0; i--){

	/* ignore pattern numbers that are to big */
	if( pat_no[i] > tot_pat_num) continue; 

	while(act_num > pat_no[i]){
	    krui_setPatternNo(act_num);
	    krui_deletePattern();
	    act_num--;
	}
	act_num--;
    }

    /* if virtual patterns were used in the original file, we want to have 
       them in the output file too */
    if(virt_was_on){
	error = krui_useClassDistribution(TRUE);
    }

    /* save all the left over patterns in the output file */
    if((error=krui_saveNewPatterns(argv[3], pat_set)) != 0 /*KRERR_NO_ERROR*/){
	fprintf(stderr, "error %d during writing file %s\n", error, argv[3]) ;
    }

}


/******************************************************************************/
/* end of file                                                                */
/******************************************************************************/







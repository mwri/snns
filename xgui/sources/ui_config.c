/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_config.c,v $  
  SHORTNAME      : config.c
  SNNS VERSION   : 4.2

  PURPOSE        : handling of configuration files
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 28.9.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.11 $ 
  LAST CHANGE    : $Date: 1998/05/20 15:00:37 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"
#include "ui_display.h"
#include "ui_fileP.h"
#include "ui_mainP.h"
#include "ui_infoP.h"
#include "ui_confirmer.h"
#include "ui_xWidgets.h"
#include "ui_utilP.h"
#include "ui_color.h"
#include "ui_class.h"
#include "ui_control.h"
#include "ui_print.h"
#include "ui_main.h"
#include "cc_main.h"
#include "pr_main.h"
#include "ui_layerP.h"
#include "cc_type.h"
#include "cc_glob.h"

#include "d3_global.h"
#include "d3_main.h"
#include "d3_fonts.h"
#include "d3_pannels.h"
#include "d3_disp.h"
#include "d3_zgraph.h"
#include "kr_ui.h"

#include "ui_config.ph"


/*****************************************************************************
  FUNCTION : ui_cfg_save

  PURPOSE  : save configuration in specified file
  RETURNS  : int
  NOTES    : In the first line, format determines the format of the file. This
             number will be checked be future routines, to be able to read 
	     all previous formats for compatibility reasons.
  UPDATE   : 
******************************************************************************/
int ui_cfg_save (FILE *filePtr)

{
    int    err;
    int    i, count = 0;
    int    in, out;
    struct Ui_DisplayType *listPtr;
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

    err = fprintf(filePtr,"SNNS xgui configuration file.  format: %d\n\n",
		  CURRENT_FORMAT);
    retchk(err);

    err = fprintf(filePtr,"filenames:\n");
    retchk(err);
    
    err = fprintf(filePtr,"%s#\n",ui_pathname);
    retchk(err);
    err = fprintf(filePtr,"%s#\n",ui_filenameNET);
    retchk(err);
    err = fprintf(filePtr,"%s#\n",ui_filenamePAT);
    retchk(err);
    err = fprintf(filePtr,"%s#\n",ui_filenameRES);
    retchk(err);
    err = fprintf(filePtr,"%s#\n",ui_filenameCFG);
    retchk(err);
    err = fprintf(filePtr,"%s#\n",ui_filenameTXT);
    retchk(err);

    err = fprintf(filePtr,"\n");
    retchk(err);

    err = fprintf(filePtr,"Layers : %d\n", MAX_NO_LAYERS); 
    retchk(err);
    for (i=1; i <= MAX_NO_LAYERS; i++) {
	err = fprintf(filePtr,"%s#\n",&ui_layerNames[i-1][0]);
	retchk(err);
    }
    err = fprintf(filePtr,"\n");
    retchk(err);

    krui_getFuncParamInfo(krui_getLearnFunc(), LEARN_FUNC, &in, &out);
    if(strcmp(krui_getLearnFunc(),"CC") == 0)
	err = fprintf(filePtr,"Learn parameters: %d\n", 
		      UI_NO_LEARN_PARAMS+UI_NO_OF_CASCADE_PARAMS);
    else
	err = fprintf(filePtr,"Learn parameters: %d\n", in);
    retchk(err);
    for (i=0; i<in; i++) {
	if (ui_controlIsCreated) {
	    err = fprintf(filePtr,"param %d: %9.5f\n",i+1,
			 ui_xFloatFromAsciiWidget(ui_learnParameterWidgets[i]));
	}else{
	    err = fprintf(filePtr,"param %d: %9.5f\n",i+1,
			  ui_learnParameters[i]);
	}
	retchk(err);
    }
    cc_readElements();
    for (i=in; i<UI_NO_LEARN_PARAMS+UI_NO_OF_CASCADE_PARAMS; i++) {
	err = fprintf(filePtr,"param %d: %9.5f\n",i+1, ui_learnParameters[i]);
	retchk(err);
    }
    err = fprintf(filePtr,"\n");
    retchk(err);

    krui_getFuncParamInfo(krui_getUpdateFunc(), UPDATE_FUNC, &in, &out);
    err = fprintf(filePtr,"Update parameters: %d\n", in);
    retchk(err);
    for (i=0; i<in; i++) {
	if (ui_controlIsCreated) 
	    err = fprintf(filePtr,"param %d: %9.5f\n",i+1,
			ui_xFloatFromAsciiWidget(ui_updateParameterWidgets[i]));
	else
	    err = fprintf(filePtr,"param %d: %9.5f\n",i+1,
			  ui_updateParameters[i]);
	retchk(err);
    }
    err = fprintf(filePtr,"\n");
    retchk(err);

    krui_getFuncParamInfo(krui_getInitialisationFunc(), INIT_FUNC, &in, &out);
    err = fprintf(filePtr,"Initialisation parameters: %d\n", in);
    retchk(err);
    for (i=0; i<in; i++) {
	if (ui_controlIsCreated) 
	    err = fprintf(filePtr,"param %d: %9.5f\n",i+1,
			 ui_xFloatFromAsciiWidget(ui_initParameterWidgets[i]));
	else
	    err = fprintf(filePtr,"param %d: %9.5f\n",i+1,
			  ui_initParameters[i]);
	retchk(err);
    }

    krui_GetPatInfo(&patt_info, &descrip);
    err = fprintf(filePtr,"Remap parameters: %d\n",
		  patt_info.no_of_remap_params);
    retchk(err);
    for (i=0; i<patt_info.no_of_remap_params; i++) {
	if (ui_controlIsCreated) 
	    err = fprintf(filePtr,"param %d: %9.5f\n",i+1,
			 ui_xFloatFromAsciiWidget(ui_remapParameterWidgets[i]));
	else
	    err = fprintf(filePtr,"param %d: %9.5f\n",i+1,
			  ui_remapParameters[i]);
	retchk(err);
    }

    err = fprintf(filePtr,"\n");
    retchk(err);

    listPtr = ui_displ_listPtr;
    while (listPtr != NULL) {
	count++;
	listPtr = listPtr->nextPtr;
    }
    err = fprintf(filePtr,"Displays: %d\n\n",count); /* no of displays */
    retchk(err);

    listPtr = ui_displ_listPtr;
    while (listPtr != NULL) {
	err = fprintf(filePtr,"display no             : %d\n",listPtr->displayNo);
	retchk(err);
	err = fprintf(filePtr,"position x             : %d\n",(int) listPtr->x);
	retchk(err);
	err = fprintf(filePtr,"position y             : %d\n",(int) listPtr->y);
	retchk(err);
	err = fprintf(filePtr,"width                  : %d\n",(int) listPtr->width);
	retchk(err);
	err = fprintf(filePtr,"height                 : %d\n",(int) listPtr->height);
	retchk(err);
	err = fprintf(filePtr,"origin x               : %d\n",(int) listPtr->origin.x);
	retchk(err);
	err = fprintf(filePtr,"origin y               : %d\n",(int) listPtr->origin.y);
	retchk(err);
	err = fprintf(filePtr,"gridSize               : %d\n",listPtr->gridSize);
	retchk(err);
	err = fprintf(filePtr,"frozen                 : %d\n",listPtr->frozen);
	retchk(err);
	err = fprintf(filePtr,"raster                 : %d\n",listPtr->raster);
	retchk(err);
	err = fprintf(filePtr,"subNetNo               : %d\n",listPtr->subNetNo);
	retchk(err);
	err = fprintf(filePtr,"flags                  : %d\n",listPtr->flags);
	retchk(err);
	err = fprintf(filePtr,"layers                 : %d\n",(int) listPtr->layers);
	retchk(err);
	err = fprintf(filePtr,"update type            : %d\n",listPtr->updateType);
	retchk(err);
	err = fprintf(filePtr,"setup show title       : %d\n",listPtr->setup.showTitleFlg);
	retchk(err);
	err = fprintf(filePtr,"setup title            : %d\n",listPtr->setup.showTitle);
	retchk(err);
	err = fprintf(filePtr,"setup show value       : %d\n",listPtr->setup.showValueFlg);
	retchk(err);
	err = fprintf(filePtr,"setup value            : %d\n",listPtr->setup.showValue);
	retchk(err);
	err = fprintf(filePtr,"setup show link        : %d\n",listPtr->setup.showLinkFlg);
	retchk(err);
	err = fprintf(filePtr,"setup show direction   : %d\n",listPtr->setup.showDirectionFlg);
	retchk(err);
	err = fprintf(filePtr,"setup show weight      : %d\n",listPtr->setup.showWeightFlg);
	retchk(err);
	err = fprintf(filePtr,"setup link trigger pos : %9.5f\n",(float) listPtr->setup.linkPosTrigger);
	retchk(err);
	err = fprintf(filePtr,"setup link trigger neg : %9.5f\n",(float) listPtr->setup.linkNegTrigger);
	retchk(err);
	err = fprintf(filePtr,"setup unit scale factor: %8.5f\n",(float) listPtr->setup.unitScaleFactor);
	retchk(err);
	err = fprintf(filePtr,"setup link scale factor: %8.5f\n",(float) listPtr->setup.linkScaleFactor);
	retchk(err);
	err = fprintf(filePtr,"setup site name        : %s#\n", listPtr->setup.siteName);
	retchk(err);
	err = fprintf(filePtr,"setup display colors   : %02d%02d%02d\n",
                                                  listPtr->setup.textColorIndex,
                                                  listPtr->setup.backgroundColorIndex,
                                                  listPtr->setup.selectionColorIndex);
	retchk(err);

	err = fprintf(filePtr,"\n");
	retchk(err);
	listPtr = listPtr->nextPtr;
    }

    if (d3_controlIsReady) {
        err = fprintf (filePtr, "%s\n", d3_configStr);
        retchk(err);
        err = fprintf(filePtr,"\n");
        retchk(err);
    
        err = fprintf (filePtr, 
                  "initial rotation               : %9.4f %9.4f %9.4f\n", 
                  deg(d3_state.rot_vec[0]), deg(d3_state.rot_vec[1]), 
                  deg(d3_state.rot_vec[2]));
        retchk(err);
        err = fprintf (filePtr, 
              "initial translation            : %9.4f %9.4f %9.4f\n", 
              d3_state.trans_vec[0], d3_state.trans_vec[1], d3_state.trans_vec[2]);
        retchk(err);
        err = fprintf (filePtr, 
              "initial scale                  : %9.4f %9.4f %9.4f\n", 
              d3_state.scale_vec[0], d3_state.scale_vec[1], d3_state.scale_vec[2]);
        retchk(err);

        err = fprintf (filePtr, 
              "rotation step                  : %9.4f %9.4f %9.4f\n", 
              deg(d3_state.rot_step[0]), deg(d3_state.rot_step[1]), 
              deg(d3_state.rot_step[2]));
        retchk(err);
        err = fprintf (filePtr, 
              "translation step               : %9.4f %9.4f %9.4f\n", 
              d3_state.trans_step[0], d3_state.trans_step[1], 
              d3_state.trans_step[2]);
        retchk(err);
        err = fprintf (filePtr, 
              "scale step                     : %9.4f %9.4f %9.4f\n", 
              d3_state.scale_step[0], d3_state.scale_step[1], 
              d3_state.scale_step[2]);
        retchk(err);

        err = fprintf (filePtr, "projection                     : %d\n", 
              d3_state.projection_mode);
        retchk(err);
        err = fprintf (filePtr, 
              "viewpoint                      : %9.4f %9.4f %9.4f\n",
              d3_state.viewpoint[0], d3_state.viewpoint[1], d3_state.viewpoint[2]);
        retchk(err);

        err = fprintf (filePtr, "font                          : %d\n", 
               d3_state.font);
        retchk(err);
        err = fprintf (filePtr, "model mode                     : %d\n", 
               d3_state.model_mode);
        retchk(err);
        err = fprintf (filePtr, "link mode                      : %d\n", 
              d3_state.link_mode);
        retchk(err);
        err = fprintf (filePtr, "link scale                     : %9.4f\n", 
              d3_state.link_scale);
        retchk(err);
        err = fprintf (filePtr, "unit aspect                    : %9.4f\n", 
              d3_state.unit_aspect);
            retchk(err);
   
        err = fprintf (filePtr, "show unit size                 : %d\n", 
              d3_state.unit_mode.size);
        retchk(err);
        err = fprintf (filePtr, "show unit color                : %d\n", 
              d3_state.unit_mode.color);
        retchk(err);
        err = fprintf (filePtr, "show unit top label            : %d\n", 
              d3_state.unit_mode.top_label);
        retchk(err);
        err = fprintf (filePtr, "show unit bottom label         : %d\n", 
              d3_state.unit_mode.bottom_label);
        retchk(err);
        err = fprintf (filePtr, 
             "light position                 : %9.4f %9.4f %9.4f\n",
              d3_state.light.position[0], d3_state.light.position[1], 
              d3_state.light.position[2]);
        retchk(err);
        err = fprintf (filePtr, "ambient light intensity        : %9.4f\n",
              d3_state.light.Ia);
        retchk(err);
        err = fprintf (filePtr, "ambient reflection coefficient : %9.4f\n",
              d3_state.light.Ka);
        retchk(err);
        err = fprintf (filePtr, "point light intensity          : %9.4f\n",
              d3_state.light.Ip);
        retchk(err);
        err = fprintf (filePtr, "diffuse refection coefficient  : %9.4f\n",
              d3_state.light.Kd);
        retchk(err);

        err = fprintf(filePtr,"\n");
        retchk(err);
   }  

   err = fprintf (filePtr, "%s\n\n", ui_printerConfigStr);
   retchk(err);
    
   err = fprintf (filePtr, "destination               : %d\n", ui_prVal.dest); 
   retchk(err);
   err = fprintf (filePtr, "format                    : %d\n", ui_prVal.format); 
   retchk(err);
   err = fprintf (filePtr, "paper                     : %d\n", ui_prVal.paper); 
   retchk(err);
   err = fprintf (filePtr, "auto scale                : %d\n", ui_prVal.autoScale); 
   retchk(err);
   err = fprintf (filePtr, "clip on border            : %d\n", ui_prVal.clip); 
   retchk(err);
   err = fprintf (filePtr, "color                     : %d\n", ui_prVal.color); 
   retchk(err);
   err = fprintf (filePtr, "orientation               : %d\n", ui_prVal.orient); 
   retchk(err);
   err = fprintf (filePtr, "scale uniform             : %d\n", ui_prVal.autoUni); 
   retchk(err);
   err = fprintf (filePtr, "unitsize by value         : %d\n", ui_prVal.size); 
   retchk(err);
   err = fprintf (filePtr, "unitshape                 : %d\n", ui_prVal.shape); 
   retchk(err);
   err = fprintf (filePtr, "transparent text          : %d\n", ui_prVal.text); 
   retchk(err);
   err = fprintf (filePtr, "border                    : %d\n", ui_prVal.border); 
   retchk(err);
   err = fprintf (filePtr, "resolution                : %d\n", ui_prVal.resolution); 
   retchk(err);
   err = fprintf (filePtr, "display to print          : %d\n", ui_prVal.displayToPrint); 
   retchk(err);
   err = fprintf (filePtr, "horizontal border         : %.2f\n", ui_prVal.borderHoriz); 
   retchk(err);
   err = fprintf (filePtr, "vertical border           : %.2f\n", ui_prVal.borderVert); 
   retchk(err);
   err = fprintf (filePtr, "image x scale             : %.2f\n", ui_prVal.scaleValX); 
   retchk(err);
   err = fprintf (filePtr, "image y scale             : %.2f\n", ui_prVal.scaleValY); 
   retchk(err);
   err = fprintf (filePtr, "unit gray value           : %.2f\n", ui_prVal.unitGray); 
   retchk(err);
   err = fprintf (filePtr, "file name                 : %s\n", ui_prVal.fileNameStr); 
   retchk(err);
   err = fprintf (filePtr, "command line              : %s\n", ui_prVal.cmdLineStr); 
   retchk(err);


   err = fprintf (filePtr, "\n%s\n\n", ui_classConfigStr);
   retchk(err);
   if(ui_class_open)class_readWidgets();
   err = fprintf (filePtr, "%d %d ",ui_classes,patt_info.class_distrib_active);
   retchk(err);
   for(i=0; i<ui_classes; i++){
       err = fprintf(filePtr,"%d ", *patt_info.class_redistribution++);
   }
   err = fprintf(filePtr,"\n");
   retchk(err);
  
   err = fprintf (filePtr, "\n%s\n\n", ui_windowConfigStr);
   retchk(err);
   err = fprintf (filePtr, "%d %d %d %d %d \n", ui_controlIsCreated, 
		  ui_infoIsCreated, ui_class_open, pr_open, cc_open); 
   retchk(err);

   err = fprintf(filePtr,"\n");
   retchk(err);

   return(TRUE); /* success */
}


/*****************************************************************************
  FUNCTION : ui_cfg_load

  PURPOSE  : load a configuration
  RETURNS  : int
  NOTES    : future load routines have to check the value "format" to be able
             to read all previous formats as well.
  UPDATE   : 
******************************************************************************/
int ui_cfg_load (FILE *filePtr)

{
    char   header[200];
    char   colorString[7];
    int    i, err, noParams, dummy, count, format, lines = 1;
    int    number, onOff, *dist, one, two, three, four, five;
    float  dummyf, f1, f2, f3;
    char   ch[1], rest_string[256], classline[256];
    pattern_set_info   patt_info;
    pattern_descriptor descrip;

    struct Ui_DisplayType *listPtr;

    ch[0] = '\0';
 
    ui_configHas3dSection = FALSE; 
    err = fscanf(filePtr,"%*[^:]: %d\n\n", &format);
    if ((format < 1) OR (format > CURRENT_FORMAT)) 
	return(-1); /* error in line 1: wrong format */
    chkretline(1);
    lines += 2;

    if (format >= 2) { /* read filenames */
	err = fscanf(filePtr,"filenames:\n");
	chkretline01;
	lines++;   
	err = fscanf(filePtr,"%[^#]#\n",ui_pathname );
	chkretline01;
	lines++;
	err = fscanf(filePtr,"%[^#]#\n",ui_filenameNET);
	chkretline01;
	lines++;
	err = fscanf(filePtr,"%[^#]#\n",ui_filenamePAT);
	chkretline01;
	lines++;
        if (format >= 6) {
    	    err = fscanf(filePtr,"%[^#]#\n",ui_filenameRES);
	    chkretline01;
	    lines++;
	}
	err = fscanf(filePtr,"%[^#]#\n",ui_filenameCFG);
	chkretline01;
	lines++;
	err = fscanf(filePtr,"%[^#]#\n",ui_filenameTXT);
	chkretline01;
	lines++;

	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;   
    }
    
    err = fscanf(filePtr,"%*[^:]: %d\n", &count); 
    chkretline(1);
    lines++;
    for (i=1; i <= count; i++) {
	err = fscanf(filePtr,"%[^#]#\n",&ui_layerNames[i-1][0]);
	chkretline01;
	lines++;
    }
    err = fscanf(filePtr,"\n");
    chkretline(0);
    lines++;

    if (format >= 4) {
	err = fscanf(filePtr,"%*[^:]: %d\n", &noParams);
	chkretline(1);
	lines++;
	for (i=0; (i<UI_NO_LEARN_PARAMS+UI_NO_OF_CASCADE_PARAMS) && (i<noParams); i++){
	    err = fscanf(filePtr,"%*[^:]: %f\n", &ui_learnParameters[i]);
	    chkretline(1);
	    lines++;
	    if (ui_controlIsCreated && (i < 5)) {
		char buf[20];
		sprintf(buf,"%6.4f",ui_learnParameters[i]);
		ui_cutTrailingZeros(buf);
		ui_xSetString(ui_learnParameterWidgets[i], buf);
	    }
	}
	cc_data.GLOBAL.pixelError = ui_learnParameters[6];
	cc_data.GLOBAL.learningFunc = (int)ui_learnParameters[7];
	cc_data.GLOBAL.onOff = (int)ui_learnParameters[8];
	cc_data.CAND.covarianceChange = ui_learnParameters[9] ;
	cc_data.CAND.candidatePatience = (int)ui_learnParameters[10];
	cc_data.CAND.maxNoOfUpdateCycles = (int)ui_learnParameters[11];
	cc_data.CAND.maxNoOfCandUnits = (int)ui_learnParameters[12];
	cc_data.CAND.actFunc = (int)ui_learnParameters[13];
	cc_data.OUT.errorChange = ui_learnParameters[14];
	cc_data.OUT.outputPatience = (int)ui_learnParameters[15];
	cc_data.OUT.maxNoOfUpdateCycles = (int)ui_learnParameters[16];
	cc_data.GLOBAL.pruneOnOff = (int)ui_learnParameters[17];
	cc_data.GLOBAL.backfittOnOff = (int)ui_learnParameters[18];
	cc_data.GLOBAL.backfittPatience = (int)ui_learnParameters[19];
	cc_data.GLOBAL.pruneFunc = (int)ui_learnParameters[20];
	cc_data.GLOBAL.modification = (int)ui_learnParameters[21];
	for (i=0;i<5;i++)
	    cc_data.GLOBAL.addParameters[i] = ui_learnParameters[22+i];
	cc_data.GLOBAL.fastmode = (int)ui_learnParameters[27];

	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;
	
	err = fscanf(filePtr,"%*[^:]: %d\n", &noParams);
	chkretline(1);
	lines++;
	for (i=0; (i<UI_NO_UPDATE_PARAMS) AND (i<noParams); i++) {
	    err = fscanf(filePtr,"%*[^:]: %f\n", &ui_updateParameters[i]);
	    chkretline(1);
	    lines++;
	    if (ui_controlIsCreated) {
		char buf[20];
		sprintf(buf,"%6.4f",ui_updateParameters[i]);
		ui_cutTrailingZeros(buf);
		ui_xSetString(ui_updateParameterWidgets[i], buf);
	    }
	}
	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;
	
	err = fscanf(filePtr,"%*[^:]: %d\n", &noParams);
	chkretline(1);
	lines++;
	for (i=0; (i<UI_NO_INIT_PARAMS) AND (i<noParams); i++) {
	    err = fscanf(filePtr,"%*[^:]: %f\n", &ui_initParameters[i]);
	    chkretline(1);
	    lines++;
	    if (ui_controlIsCreated) {
		char buf[20];
		sprintf(buf,"%6.4f",ui_initParameters[i]);
		ui_cutTrailingZeros(buf);
		ui_xSetString(ui_initParameterWidgets[i], buf);
	    }
	}
	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;
    }

    if (format >= 7) {
	err = fscanf(filePtr,"%*[^:]: %d\n", &noParams);
	chkretline(1);
	lines++;
	for (i=0; (i<UI_NO_REMAP_PARAMS) AND (i<noParams); i++) {
	    err = fscanf(filePtr,"%*[^:]: %f\n", &ui_remapParameters[i]);
	    chkretline(1);
	    lines++;
	    if (ui_controlIsCreated) {
		char buf[20];
		sprintf(buf,"%6.4f",ui_remapParameters[i]);
		ui_cutTrailingZeros(buf);
		ui_xSetString(ui_remapParameterWidgets[i], buf);
	    }
	}
	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;
    }

    err = fscanf(filePtr,"%*[^:]: %d\n\n",&count);
    chkretline(1);
    lines += 2;
 
    ui_displ_resetList();
    for (i=1; i<=count; i++) {
	if ((listPtr = ui_displ_getFreeItem()) == NULL) {
	    ui_confirmOk("No more memory for displays available!");
	    return 1;
	}
		
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->displayNo));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&dummy);
	chkfreeretline(1);
	lines++;
	listPtr->x = (Position) dummy;
	err = fscanf(filePtr,"%*[^:]: %d\n",&dummy);
	chkfreeretline(1);
	lines++;
	listPtr->y = (Position) dummy;
	err = fscanf(filePtr,"%*[^:]: %d\n",&dummy);
	chkfreeretline(1);
	lines++;
	listPtr->width = (Dimension) dummy;
	err = fscanf(filePtr,"%*[^:]: %d\n",&dummy);
	chkfreeretline(1);
	lines++;
	listPtr->height = (Dimension) dummy;
	err = fscanf(filePtr,"%*[^:]: %d\n",&dummy);
	chkfreeretline(1);
	lines++;
	listPtr->origin.x = (short) dummy;
	err = fscanf(filePtr,"%*[^:]: %d\n",&dummy);
	chkfreeretline(1);
	lines++;
	listPtr->origin.y = (short) dummy;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->gridSize));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->frozen));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->raster));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->subNetNo));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->flags));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&dummy);
	chkfreeretline(1);
	lines++;
	listPtr->layers = (unsigned short) dummy;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->updateType));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->setup.showTitleFlg));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->setup.showTitle));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->setup.showValueFlg));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->setup.showValue));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->setup.showLinkFlg));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->setup.showDirectionFlg));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %d\n",&(listPtr->setup.showWeightFlg));
	chkfreeretline(1);
	lines++;
	err = fscanf(filePtr,"%*[^:]: %f\n",&dummyf);
	chkfreeretline(1);
	lines++;
	listPtr->setup.linkPosTrigger = dummyf;
	err = fscanf(filePtr,"%*[^:]: %f\n",&dummyf);
	chkfreeretline(1);
	lines++;
	listPtr->setup.linkNegTrigger = dummyf;
	err = fscanf(filePtr,"%*[^:]: %f\n",&dummyf);
	chkfreeretline(1);
	lines++;
	listPtr->setup.unitScaleFactor= dummyf;
	if (format >= 3) {
	    err = fscanf(filePtr,"%*[^:]: %f\n",&dummyf);
	    chkfreeretline(1);
	    lines++;
	    listPtr->setup.linkScaleFactor= dummyf;
	    err = fscanf(filePtr,"%*[^:]: %[^#]#\n",listPtr->setup.siteName);
	    chkretline01;
	    lines++;
	    err = fscanf(filePtr,"%*[^:]: %s\n", colorString);
	    chkfreeretline(1);
	    lines++;
            ui_convertColorString(colorString, &(listPtr->setup.textColorIndex),
                                  &(listPtr->setup.backgroundColorIndex),
                                  &(listPtr->setup.selectionColorIndex));
	}
	err = fscanf(filePtr,"\n");


	lines++;

	/* popup display */
	ui_displayGraphic(NULL, listPtr, NULL);
    }

    if (format < 5) 
	return(TRUE);

    err = fscanf(filePtr,"%s\n", header);
    if (strcmp (header, d3_configStr) == 0) {
	ui_configHas3dSection = TRUE; 
	chkretline(1);
	lines++;
	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;

	err = fscanf(filePtr, "%*[^:]: %f %f %f\n", &f1, &f2, &f3); 
	chkretline(3);
	lines++;
	d3_state.rot_vec[0] = rad(f1);
	d3_state.rot_vec[1] = rad(f2);
	d3_state.rot_vec[2] = rad(f3);
	err = fscanf(filePtr, "%*[^:]: %f %f %f\n", 
		     &d3_state.trans_vec[0], &d3_state.trans_vec[1], 
		     &d3_state.trans_vec[2]);
	chkretline(3);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f %f %f\n", 
		     &d3_state.scale_vec[0], &d3_state.scale_vec[1], 
		     &d3_state.scale_vec[2]);
	chkretline(3);
	lines++;

	err = fscanf(filePtr, "%*[^:]: %f %f %f\n", &f1, &f2, &f3); 
	chkretline(3);
	lines++;
	d3_state.rot_step[0] = rad(f1);
	d3_state.rot_step[1] = rad(f2);
	d3_state.rot_step[2] = rad(f3);
	err = fscanf(filePtr, "%*[^:]: %f %f %f\n", &d3_state.trans_step[0], 
		     &d3_state.trans_step[1], &d3_state.trans_step[2]);
	chkretline(3);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f %f %f\n", &d3_state.scale_step[0], 
		     &d3_state.scale_step[1], &d3_state.scale_step[2]);
	chkretline(3);
	lines++;

	err = fscanf(filePtr, "%*[^:]: %d\n", &d3_state.projection_mode); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f %f %f\n", &d3_state.viewpoint[0], 
		     &d3_state.viewpoint[1], &d3_state.viewpoint[2]);
	chkretline(3);
	lines++;

	err = fscanf(filePtr, "%*[^:]: %d\n", &d3_state.font); 
	chkretline(1);
	lines++;

	err = fscanf(filePtr, "%*[^:]: %d\n", &d3_state.model_mode); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &d3_state.link_mode); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &d3_state.link_scale); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &d3_state.unit_aspect); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &d3_state.unit_mode.size); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &d3_state.unit_mode.color); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &d3_state.unit_mode.top_label); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &d3_state.unit_mode.bottom_label);
	chkretline(1);
	lines++;

	err = fscanf(filePtr, "%*[^:]: %f %f %f\n", &d3_state.light.position[0],
		     &d3_state.light.position[1], &d3_state.light.position[2]);
	chkretline(3);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &d3_state.light.Ia); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &d3_state.light.Ka); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &d3_state.light.Ip); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &d3_state.light.Kd); 
	chkretline(1);
	lines++;

	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;
    
	d3_select_font (d3_state.font);
	if (d3_state.model_mode == solid) 
	    d3_initZbuffer ();
	d3_displayGraphic ();
	d3_createDisplayWindow ();
    }

    if (format < 6) 
	return(TRUE);
        
    if (ui_configHas3dSection)
	err = fscanf(filePtr,"%s\n", header);
    if (strcmp (header, ui_printerConfigStr) == 0) { 
	chkretline(1);
	lines++;
	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;

	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.dest); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.format); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.paper); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.autoScale); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.clip); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.color); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.orient); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.autoUni); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.size); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.shape); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.text); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.border); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.resolution); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %d\n", &ui_prVal.displayToPrint); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &ui_prVal.borderHoriz); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &ui_prVal. borderVert); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &ui_prVal.scaleValX); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &ui_prVal.scaleValY); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %f\n", &ui_prVal.unitGray); 
	chkretline(1);
	lines++;
	err = fscanf(filePtr, "%*[^:]: %s\n", ui_prVal.fileNameStr); 
	chkretline(1);
	lines++;

	err = fscanf(filePtr, "%*[^:]: ");
	ui_prVal.cmdLineStr[0] = '\0';
	while ((*ch = getc(filePtr)) != '\n')
	    strncat (ui_prVal.cmdLineStr, ch, 1);
	lines++;
	err = fscanf(filePtr,"%s\n", header);
    }

    if (strcmp (header, ui_classConfigStr) == 0) { 
	chkretline(1);
	lines++;
	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;

	err = (fgets(classline, 256, filePtr) == NULL)?0:1;
	chkretline(1);
	sscanf(classline,"%d %d %[^\n]",&number, &onOff, rest_string);
	if( (krui_GetPatInfo(&patt_info, &descrip) != 0) || 
	    (number != patt_info.classes)){
	    ui_confirmOk("Class information does not match pattern file;\nclass information ignored");
	}else{
            dist = (int *)malloc(patt_info.classes * sizeof(int));
	    for(i=0; i<patt_info.classes; i++){
		sscanf(rest_string,"%d %[^\n]",&dist[i],classline);
		strcpy(rest_string,classline);
	    }
	    krui_useClassDistribution(onOff);
	    krui_setClassDistribution(dist);
	}
	lines++;
	err = fscanf(filePtr,"\n%s\n", header);
    }

    if (strcmp (header, ui_windowConfigStr) == 0) { 
	chkretline(1);
	lines++;
	err = fscanf(filePtr,"\n");
	chkretline(0);
	lines++;

	err = fscanf(filePtr, "%d %d %d %d %d", &one, &two, &three, 
		     &four, &five); 
	if(one) ui_displayControl(NULL, NULL, NULL);
	if(two) ui_displayInfo(NULL, NULL, NULL);
	if(three) ui_classWindow();
	if(four) pr_createWindow();
	if(five) cc_createWindow();
	chkretline(5);
	lines++;
    }
    return(TRUE);

}
/* end of file */

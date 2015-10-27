/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_config.ph,v $
  SHORTNAME      : config.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer, Universitaet Stuttgart
  DATE           : 1.9.1990

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/05/20 15:00:38 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _UI_CONFIG_DEFINED_
#define _UI_CONFIG_DEFINED_


/* begin global definition section */


int  ui_cfg_save (FILE *);
int  ui_cfg_load (FILE *);

/* end global definition section */


/* begin private definition section */

#define retchk(code)     if (code == EOF) return(-1)
#define chkretline(code) if (err != code) return(-lines);
#define chkretline01     if ((err != 0) AND (err != 1)) return(-lines);
#define chkfreeretline(code) if (err != code) { ui_displ_freeItem(listPtr); return(-lines); }
#define CURRENT_FORMAT  7

#define chkretline(code) if (err != code) return(-lines);


static Bool ui_configHas3dSection; 
static char *d3_configStr = "3D-Configuration";
static char *ui_printerConfigStr = "Printer-Configuration";
static char *ui_windowConfigStr = "Window-Configuration";
static char *ui_classConfigStr = "Class-Configuration";

/* end private definition section */

#endif /* _UI_CONFIG_DEFINED_ */
/* end of file */

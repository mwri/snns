/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_file.h,v $
  SHORTNAME      : file.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer
  DATE           : 22.5.1990

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:21:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/


#ifndef _UI_FILE_DEFINED_
#define _UI_FILE_DEFINED_

extern void ui_xCreateFilePanel (Widget);

extern Widget    ui_filePanel;

	/* strings storing the data of all input fields */
extern Bool      ui_fileIsCreated; /* file panel created yes or no */


#endif /* _UI_FILE_DEFINED_ */

/* end of file */
/* lines: 35 */


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_maspar.ph,v $
  SHORTNAME      : maspar.ph
  SNNS VERSION   : 4.2

  PURPOSE        :
  NOTES          :

  AUTHOR         :
  DATE           :

  CHANGED BY     :
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:23 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifdef MASPAR_KERNEL


#ifndef _UI_MASPAR_DEFINED_
#define _UI_MASPAR_DEFINED_


/* begin global definition section */


void ui_masparPannel (Widget, Widget, caddr_t);


/* end global definition section */


/* begin private definition section */


static void connect_MasPar (void);
static void disconnect_MasPar (void);
static void standard_Net (void);
static void feedforward_Net (void);
static void closeMasparPannel (Widget, Widget, caddr_t);


static Widget connect, disconnect, standard, feedforward;
static int err;


/* end private definition section */


#endif /* _UI_MASPAR_DEFINED_ */


#endif /* MASPAR_KERNEL */


/* end of file */
/* lines: 9 */

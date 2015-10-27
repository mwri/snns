/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_JordElm.ph,v $
  SHORTNAME      : bn_JordElm.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header file for corresponding '.c' file
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:24 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _BN_JORDANELMAN_DEFINED_
#define _BN_JORDANELMAN_DEFINED_


/* --------------------  global definition section  ------------------------ */

void bn_create_jordan (void);
void bn_create_elman  (void);



/* -------------------  private definition section  ------------------------ */

#define MAX_NO_OF_LAYERS        10
#define IF_ERROR_RETURN(error_code)  if (error_code != KRERR_NO_ERROR) \
                                     return (error_code)


static int      bn_jordan_open = 0 ;
static Widget   baseWidgetJordan ;
static Widget   jordanUnitWidget[3], jordanColWidget[3] ;

static void     bn_jordan_donePROC   (void) ;
static void     bn_jordan_createPROC (void) ;
static krui_err bn_jordan_createNet  (int IUnits, int HUnits, int OUnits,
                                      int ICols , int HCols , int OCols ) ;



static int      bn_elman_open = 0 ;
static Widget   baseWidgetElman ;
static int      no_of_layers  = 3 ;
static int      layer[MAX_NO_OF_LAYERS]   ;
static int      columns[MAX_NO_OF_LAYERS] ;
static Bool     out_context ;

static Widget   elmanUnitWidget[MAX_NO_OF_LAYERS],
                elmanColWidget[MAX_NO_OF_LAYERS],
                elmanLabelWidget[MAX_NO_OF_LAYERS],
                elmanSpaceWidget[MAX_NO_OF_LAYERS],
                elman_box, elman_form[3], w_headline,
                t_yes, t_no;

static void     bn_elman_noPROC           (void) ;
static void     bn_elman_yesPROC          (void) ;
static void     bn_elman_donePROC         (void) ;
static void     bn_elman_createPROC       (void) ;
static void     bn_elman_insertPROC       (void) ;
static void     bn_elman_deletePROC       (void) ;
static void     bn_elman_create_dialogrow (int i, char *str) ;
static void     bn_elman_getFromWidget    (void) ;
static krui_err bn_elman_createNet        (void) ;


static int      createUnitLayer (int  no_of_units, 
                                 int  io_type,
                                 char *act_func_name,
                                 char *out_func_name,
                                 char *unit_name) ;


static krui_err connectFull     (int       from_unit,
                                 int       no_of_source_units,
                                 int       to_unit,
                                 int       no_of_target_units,
                                 FlintType weight) ;


static krui_err connectOneToOne (int       from_unit,
                                 int       to_unit,
                                 int       no_of_units,
                                 FlintType weight) ;


static krui_err connectSelfRec  (int       unit_no,
                                 int       no_of_units,
                                 FlintType weight) ;


static void     fixWindowSize   (Widget window) ;

#endif


/*****************************************************************************
                        E N D     O F     F I L E
******************************************************************************/










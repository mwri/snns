/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_artmap.ph,v $
  SHORTNAME      : bn_artmap
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:35 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_ARTMAP_DEFINED_
#define  _BN_ARTMAP_DEFINED_

/* begin global definition section */

void bn_createARTMAP (void);

/* end global definition section */


/* begin private definition section */

/* Definition of the Output Function */
#define OUTFUNC    "Out_Identity"


/* Definition of the Activationfunctions */
#define ACTF_INPa "Act_Identity"
#define ACTF_CMPa "Act_at_least_2"
#define ACTF_RECa "Act_Identity"
#define ACTF_DELa "Act_at_least_2"
#define ACTF_Da   "Act_at_least_1"
#define ACTF_RSTa "Act_at_least_1"
#define ACTF_G1a  "Act_at_least_2"
#define ACTF_RIa  "Act_Product"
#define ACTF_RCa  "Act_Identity"
#define ACTF_RGa  "Act_less_than_0"
#define ACTF_CLa  "Act_at_least_1"
#define ACTF_NCa  "Act_ARTMAP_NCa"
#define ACTF_RHOa "Act_Identity"
#define ACTF_G2a  "Act_at_most_0"


#define ACTF_INPb "Act_Identity"
#define ACTF_CMPb "Act_at_least_2"
#define ACTF_RECb "Act_Identity"
#define ACTF_DELb "Act_at_least_2"
#define ACTF_Db   "Act_at_least_1"
#define ACTF_RSTb "Act_at_least_1"
#define ACTF_G1b  "Act_at_least_2"
#define ACTF_RIb  "Act_Product"
#define ACTF_RCb  "Act_Identity"
#define ACTF_RGb  "Act_less_than_0"
#define ACTF_CLb  "Act_at_least_1"
#define ACTF_NCb  "Act_ARTMAP_NCb"
#define ACTF_RHOb "Act_Identity"
#define ACTF_G2b  "Act_at_most_0"

#define ACTF_MAP  "Act_at_least_2"
#define ACTF_D1   "Act_Identity"
#define ACTF_RB   "Act_Product"
#define ACTF_RM   "Act_Identity"
#define ACTF_RG   "Act_less_than_0"
#define ACTF_G    "Act_exactly_1"
#define ACTF_CL   "Act_at_least_2"
#define ACTF_NC   "Act_at_least_1"
#define ACTF_RHO  "Act_Identity"
#define ACTF_QU   "Act_Product"
#define ACTF_DRHO "Act_ARTMAP_DRho"



/* Site names
*/
#define SITE_NAME_RSTa_SELF    "ArtMap_Site_Rsta_Self"
#define SITE_NAME_RSTa_SIGNAL  "ArtMap_Site_Rsta_Signal"
#define SITE_NAME_REC_G1a      "ArtMap_Site_Rec_G1a"
#define SITE_NAME_INP_G1a      "ArtMap_Site_Inp_G1a"
#define SITE_NAME_INP_RIa      "ArtMap_Site_Inp_RIa"
#define SITE_NAME_RHO_RIa      "ArtMap_Site_Rho_RIa"

#define SITE_NAME_RSTb_SELF    "ArtMap_Site_Rstb_Self"
#define SITE_NAME_RSTb_SIGNAL  "ArtMap_Site_Rstb_Signal"
#define SITE_NAME_REC_G1b      "ArtMap_Site_Rec_G1b"
#define SITE_NAME_INP_G1b      "ArtMap_Site_Inp_G1b"
#define SITE_NAME_INP_RIb      "ArtMap_Site_Inp_RIb"
#define SITE_NAME_RHO_RIb      "ArtMap_Site_Rho_RIb"

#define SITE_NAME_ARTa_G       "ArtMap_Site_ARTa_G"
#define SITE_NAME_ARTb_G       "ArtMap_Site_ARTb_G"
#define SITE_NAME_B_RB         "ArtMap_Site_B_RB"
#define SITE_NAME_RHO_RB       "ArtMap_Site_Rho_RB"
#define SITE_NAME_CMPa_QU      "ArtMap_Site_Cmpa_QU"
#define SITE_NAME_INPa_QU      "ArtMap_Site_Inpa_QU"

/* Site function names
*/
#define SITE_FUNC_RSTa_SELF    "Site_WeightedSum"
#define SITE_FUNC_RSTa_SIGNAL  "Site_at_least_2"
#define SITE_FUNC_REC_G1a      "Site_at_most_0"
#define SITE_FUNC_INP_G1a      "Site_at_least_1"
#define SITE_FUNC_INP_RIa      "Site_WeightedSum"
#define SITE_FUNC_RHO_RIa      "Site_WeightedSum"

#define SITE_FUNC_RSTb_SELF    "Site_WeightedSum"
#define SITE_FUNC_RSTb_SIGNAL  "Site_at_least_2"
#define SITE_FUNC_REC_G1b      "Site_at_most_0"
#define SITE_FUNC_INP_G1b      "Site_at_least_1"
#define SITE_FUNC_INP_RIb      "Site_WeightedSum"
#define SITE_FUNC_RHO_RIb      "Site_WeightedSum"

#define SITE_FUNC_ARTa_G       "Site_at_least_1"
#define SITE_FUNC_ARTb_G       "Site_at_least_1"
#define SITE_FUNC_B_RB         "Site_WeightedSum"
#define SITE_FUNC_RHO_RB       "Site_WeightedSum"
#define SITE_FUNC_CMPa_QU      "Site_WeightedSum"
#define SITE_FUNC_INPa_QU      "Site_Reciprocal"

/* Learning function name */
#define LEARN_FUNC_NAME     "ARTMAP"

/* Update function name */
#define UPDATE_FUNC_NAME    "ARTMAP_Stable"



/* Macros
*/


/* returns 0 if x<=0 and 1 if x>0
*/
#define POS_SIGN(x)            (((x) > 0) ? 1 : 0)


#define NO_OF_SPECIAL_ab_UNITS   8   /* d1a to d3a (d1b to d3b) excluded */
#define NO_OF_SPECIAL_UNITS      10   /* including d1 unit */

/* Absolute Unit-Nummer berechnet aus ihrer relativen Nummer im Layer */
#define INPa_UNIT(y)            y
#define CMPa_UNIT(y)            INPa_UNIT(f1aUnits) + y
#define RECa_UNIT(y)            CMPa_UNIT(f1aUnits) + y
#define DELa_UNIT(y)            RECa_UNIT(f2aUnits) + y
#define RSTa_UNIT(y)            DELa_UNIT(f2aUnits) + 3 + y

#define MAP_UNIT(y)             RSTa_UNIT(f2aUnits) + NO_OF_SPECIAL_ab_UNITS + y

#define INPb_UNIT(y)            MAP_UNIT(f2bUnits) + NO_OF_SPECIAL_UNITS + y
#define CMPb_UNIT(y)            INPb_UNIT(f1bUnits) + y
#define RECb_UNIT(y)            CMPb_UNIT(f1bUnits) + y
#define DELb_UNIT(y)            RECb_UNIT(f2bUnits) + y
#define RSTb_UNIT(y)            DELb_UNIT(f2bUnits) + 3 + y

#define Da_UNIT(y)              DELa_UNIT(f2aUnits) + y
#define Db_UNIT(y)              DELb_UNIT(f2bUnits) + y
#define D1_UNIT                 MAP_UNIT(f2bUnits) + 1

#define SPECIAL_UNITS_a         RSTa_UNIT(f2aUnits) + 1
#define SPECIAL_UNITS_b         RSTb_UNIT(f2bUnits) + 1
#define SPECIAL_UNITS           D1_UNIT + 1

#define G1a_UNIT                 SPECIAL_UNITS_a
#define G1b_UNIT                 SPECIAL_UNITS_b
#define G_UNIT                   SPECIAL_UNITS

#define RIa_UNIT                 SPECIAL_UNITS_a + 1
#define RIb_UNIT                 SPECIAL_UNITS_b + 1
#define RB_UNIT                  SPECIAL_UNITS + 1

#define RCa_UNIT                 SPECIAL_UNITS_a + 2
#define RCb_UNIT                 SPECIAL_UNITS_b + 2
#define RM_UNIT                  SPECIAL_UNITS + 2

#define RGa_UNIT                 SPECIAL_UNITS_a + 3
#define RGb_UNIT                 SPECIAL_UNITS_b + 3
#define RG_UNIT                  SPECIAL_UNITS + 3

#define CLa_UNIT                 SPECIAL_UNITS_a + 4
#define CLb_UNIT                 SPECIAL_UNITS_b + 4
#define CL_UNIT                  SPECIAL_UNITS + 4

#define NCa_UNIT                 SPECIAL_UNITS_a + 5
#define NCb_UNIT                 SPECIAL_UNITS_b + 5
#define NC_UNIT                  SPECIAL_UNITS + 5

#define RHOa_UNIT                SPECIAL_UNITS_a + 6
#define RHOb_UNIT                SPECIAL_UNITS_b + 6
#define RHO_UNIT                 SPECIAL_UNITS + 6

#define G2a_UNIT                 SPECIAL_UNITS_a + 7
#define G2b_UNIT                 SPECIAL_UNITS_b + 7

#define QU_UNIT                  SPECIAL_UNITS + 7
#define DRHO_UNIT                SPECIAL_UNITS + 8


/* Calculates the number of columns when no of units and no. of rows is given
*/
#define NO_OF_COLS(units,rows)  ((int) (units/rows) + POS_SIGN(units % rows))

#define MAX(x,y)                ((x) > (y) ? (x) : (y))

/* No of columns for F1a, F2a, F1b, F2b */
#define F1a_COLS                 NO_OF_COLS(f1aUnits, f1aRows)
#define F2a_COLS                 NO_OF_COLS(f2aUnits, f2aRows)
#define F1b_COLS                 NO_OF_COLS(f1bUnits, f1bRows)
#define F2b_COLS                 NO_OF_COLS(f2bUnits, f2bRows)

/* Unit Positions */
#define OFFSET_X                 0
#define OFFSET_Y                 0
#define INPa_X                   OFFSET_X + 1
#define INPa_Y                   OFFSET_Y + 8
#define CMPa_X                   INPa_X + MAX(F1a_COLS,F1b_COLS) + 1
#define CMPa_Y                   INPa_Y
#define RECa_X                   CMPa_X + MAX(F1a_COLS,F1b_COLS) + 3
#define RECa_Y                   CMPa_Y
#define DELa_X                   RECa_X + MAX(F2a_COLS,F2b_COLS) + 1
#define DELa_Y                   RECa_Y
#define RSTa_X                   DELa_X + MAX(F2a_COLS,F2b_COLS) + 1
#define RSTa_Y                   DELa_Y
#define D1a_X                    DELa_X
#define D1a_Y                    DELa_Y + MAX(f1aRows,f2aRows) + 1
#define D2a_X                    (RSTa_X + DELa_X) / 2
#define D2a_Y                    D1a_Y
#define D3a_X                    RSTa_X
#define D3a_Y                    D1a_Y
#define G1a_X                    CMPa_X
#define G1a_Y                    CMPa_Y + MAX(f1aRows,f2aRows) + 1
#define RIa_X                    INPa_X
#define RIa_Y                    INPa_Y - 2
#define RCa_X                    CMPa_X
#define RCa_Y                    RIa_Y
#define RGa_X                    RCa_X
#define RGa_Y                    RCa_Y - 2
#define CLa_X                    RSTa_X + MAX(F2a_COLS,F2b_COLS) + 1
#define CLa_Y                    RSTa_Y + MAX(f1aRows, f2aRows) + 1
#define NCa_X                    CLa_X
#define NCa_Y                    CLa_Y - 2
#define RHOa_X                   RIa_X
#define RHOa_Y                   RIa_Y - 2
#define G2a_X                    RECa_X
#define G2a_Y                    G1a_Y

#define MAP_X                    RSTa_X + MAX(F2a_COLS, F2b_COLS) + 9
#define MAP_Y                    RSTa_Y + (MAX(f1aRows,f2aRows) + 6) / 2
#define D1_X                     MAP_X - 2
#define D1_Y                     RSTb_Y + f2bRows - 1
#define RB_X                     MAP_X - 2
#define RB_Y                     MAP_Y - 2
#define RM_X                     MAP_X
#define RM_Y                     RB_Y
#define RG_X                     RM_X
#define RG_Y                     RGa_Y
#define CL_X                     MAP_X - 6
#define CL_Y                     (CLa_Y + CLb_Y) / 2
#define NC_X                     CL_X + 2
#define NC_Y                     CL_Y
#define G_X                      MAP_X
#define G_Y                      MAP_Y + f2bRows + 1
#define RHO_X                    RB_X
#define RHO_Y                    RB_Y - 2
#define QU_X                     RECa_X
#define QU_Y                     RCa_Y
#define DRHO_X                   QU_X
#define DRHO_Y                   RGa_Y

#define INPb_X                   INPa_X
#define INPb_Y                   INPa_Y + MAX(f1aRows,f2aRows) + 5
#define CMPb_X                   CMPa_X
#define CMPb_Y                   INPb_Y
#define RECb_X                   RECa_X
#define RECb_Y                   CMPb_Y
#define DELb_X                   DELa_X
#define DELb_Y                   RECb_Y
#define RSTb_X                   RSTa_X
#define RSTb_Y                   DELb_Y
#define D1b_X                    D1a_X
#define D1b_Y                    DELb_Y - 2
#define D2b_X                    D2a_X
#define D2b_Y                    D1b_Y
#define D3b_X                    D3a_X
#define D3b_Y                    D2b_Y
#define G1b_X                    G1a_X
#define G1b_Y                    CMPb_Y - 2
#define RIb_X                    RIa_X
#define RIb_Y                    INPb_Y + MAX(f1bRows,f2bRows) + 1
#define RCb_X                    RCa_X
#define RCb_Y                    RIb_Y
#define RGb_X                    RGa_X
#define RGb_Y                    RCb_Y + 2
#define CLb_X                    CLa_X
#define CLb_Y                    D3b_Y
#define NCb_X                    NCa_X
#define NCb_Y                    CLb_Y + 2
#define RHOb_X                   RHOa_X
#define RHOb_Y                   RIb_Y + 2
#define G2b_X                    RECb_X
#define G2b_Y                    G1b_Y

#define STRING_LENGTH            30


/* checks whether value is not equal to KRERR_NO_ERROR and if so, returns
*/
#define CHECK_RETURN(ret_code)  if (ret_code != KRERR_NO_ERROR) \
                                   return (ret_code)






static int      bn_artmap_open = 0;
static Widget   baseWidget;
static Widget   artmapUnitWidget[4], artmapRowWidget[4];



static krui_err bn_artmap_make_layer (
                            int    units,
                            int    rows,
                            int    cols,
                            int    Xbegin,
                            int    Ybegin,
                            char  *name_prefix,
                            int    unit_ttype,
                            char  *actfuncname,
                            char  *outfuncname,
                            int    NoOfSites,
                            char **SiteNames,
                            char **SiteFuncNames);                        
static krui_err bn_artmap_createNet (int f1aUnits, int f1aRows, int f2aUnits, int f2aRows, 
                                     int f1bUnits, int f1bRows, int f2bUnits, int f2bRows);
static void bn_artmap_donePROC (void);
static void bn_artmap_createPROC (void);

/* end private definition section */

#endif

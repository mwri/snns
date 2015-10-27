/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_artmap.c,v $
  SHORTNAME      : bn_artmap
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the ARTMAP-window and ARTMAP networks. 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"
#include "ui_fileP.h"
#include "bn_basics.h"
#include "ui_control.h"
#include "ui_confirmer.h"

#include "bn_artmap.ph"




/*****************************************************************************
  FUNCTION : bn_artmap_make_layer

  PURPOSE  : make a complete layer
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/
static krui_err bn_artmap_make_layer (int  units, int  rows, int  cols, 
				      int  Xbegin, int  Ybegin,
				      char *name_prefix, int  unit_ttype, 
				      char *actfuncname, char *outfuncname,
				      int  NoOfSites, char **SiteNames,
				      char **SiteFuncNames)

{
   krui_err             ret_code = KRERR_NO_ERROR;

   int                  row;
   int                  col;
   int                  count;
   int                  last_col;
   int                  unit_no;
   struct PosType       unit_pos;
   char                 *site_name, *site_func_name;
   int                  site_no;
   char                 name[10];


   /* Create Site Table Entries */
   site_no = 1;
   while (site_no <= NoOfSites) {
      site_name= *(SiteNames+(site_no-1));
      site_func_name= *(SiteFuncNames+(site_no-1));

      if ((site_name != NULL) && (site_func_name != NULL)) {
         ret_code = krui_createSiteTableEntry(site_name, site_func_name);
         CHECK_RETURN (ret_code);
      } /*if*/

      site_no++;
   }  /*while*/

   /* Create Units */
   for (row = 1, count = 0; row <= rows; row++) {

      if ((units % rows) > 0) {
         last_col = POS_SIGN((units-count) % (rows-row+1));
      } else {
         last_col = 1;
      } /*if*/

      for (col = 1; col <= (cols-1+last_col); col++) {

         count++;

         unit_no = krui_createDefaultUnit();
         if (unit_no < 0)  CHECK_RETURN (unit_no);

         ret_code = krui_setUnitTType (unit_no, unit_ttype);
         CHECK_RETURN( ret_code );

         ret_code = krui_setUnitActFunc(unit_no, actfuncname);
         CHECK_RETURN (ret_code);

         ret_code = krui_setUnitOutFunc(unit_no, outfuncname);
         CHECK_RETURN (ret_code);

         if (units > 1) {
            sprintf (name,"%s%d",name_prefix,count);
         } else {
            sprintf (name,"%s",name_prefix);
         } /*if*/

         ret_code = krui_setUnitName (unit_no, name);
         CHECK_RETURN (ret_code);

         unit_pos.x = Xbegin + col - 1;
         unit_pos.y = Ybegin + row - 1;
         krui_setUnitPosition( unit_no, &unit_pos );

         ret_code = krui_setCurrentUnit (unit_no);
         CHECK_RETURN (ret_code);

         /* Add Sites */
         site_no = 1;
         while (site_no <= NoOfSites) {
             site_name= *(SiteNames+(site_no-1));

             if (site_name != NULL) {
                ret_code = krui_addSite (site_name);
                CHECK_RETURN (ret_code);
             } /*if*/

             site_no++;
         } /*while*/

      } /*for*/

   } /*for*/

   return (ret_code);
} 



/*****************************************************************************
  FUNCTION : bn_artmap_createNet

  PURPOSE  : Generation of the ARTMAP network using SNNS kernel functions.
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/
static krui_err bn_artmap_createNet(int f1aUnits, int f1aRows, int f2aUnits, 
				    int f2aRows, int f1bUnits, int f1bRows, 
				    int f2bUnits, int f2bRows)

{
   krui_err          ret_code   = KRERR_NO_ERROR;

   int               NoOfSites;

   char              **SiteName, **SiteFunc;

   int               i, j;

   /* Allocate memory for the to pointer Arrays for site names
      and sitefunc names
   */
   SiteName      = (char **) malloc (2*sizeof(char *));
   SiteFunc      = (char **) malloc (2*sizeof(char *));

   if ((SiteName == NULL) || (SiteFunc == NULL)) {
      return (KRERR_INSUFFICIENT_MEM);
   } /*if*/
   /* Allocate memory to save Site names and Sitefunc names */
   *SiteName     = (char *)malloc (STRING_LENGTH);
   *(SiteName+1) = (char *)malloc (STRING_LENGTH);
   *SiteFunc     = (char *)malloc (STRING_LENGTH);
   *(SiteFunc+1) = (char *)malloc (STRING_LENGTH);

   if ((*SiteName == NULL) || (*(SiteName+1) == NULL) ||
       (*SiteFunc == NULL) || (*(SiteFunc+1) == NULL)
      )
   {
      return (KRERR_INSUFFICIENT_MEM);
   } /*if*/


   /********* create ARTa units ******************/

   /* create Input Layer for ARTa */
   ret_code = bn_artmap_make_layer (f1aUnits, f1aRows, F1a_COLS, INPa_X, INPa_Y, "inpa",
                                    INPUT, ACTF_INPa, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Comparison Layer for ARTa */
   ret_code = bn_artmap_make_layer (f1aUnits, f1aRows, F1a_COLS, CMPa_X, CMPa_Y, "cmpa",
                                    HIDDEN, ACTF_CMPa, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Recognition Layer for ARTa */
   ret_code = bn_artmap_make_layer (f2aUnits, f2aRows, F2a_COLS, RECa_X, RECa_Y, "reca",
                                    SPECIAL, ACTF_RECa, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Delay Layer for ARTa */
   ret_code = bn_artmap_make_layer (f2aUnits, f2aRows, F2a_COLS, DELa_X, DELa_Y, "dela",
                                    HIDDEN, ACTF_DELa, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d1a */
   ret_code = bn_artmap_make_layer (1, 1, 1, D1a_X, D1a_Y, "d1a", HIDDEN, ACTF_Da,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d2a */
   ret_code = bn_artmap_make_layer (1, 1, 1, D2a_X, D2a_Y, "d2a", HIDDEN, ACTF_Da,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d3a */
   ret_code = bn_artmap_make_layer (1, 1, 1, D3a_X, D3a_Y, "d3a", HIDDEN, ACTF_Da,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create local reset layer */
   NoOfSites = 2;
   strcpy(*SiteName,     SITE_NAME_RSTa_SELF);
   strcpy(*(SiteName+1), SITE_NAME_RSTa_SIGNAL);
   strcpy(*SiteFunc,     SITE_FUNC_RSTa_SELF);
   strcpy(*(SiteFunc+1), SITE_FUNC_RSTa_SIGNAL);

   ret_code = bn_artmap_make_layer (f2aUnits, f2aRows, F2a_COLS, RSTa_X, RSTa_Y, "rsta",
                                    HIDDEN, ACTF_RSTa, OUTFUNC, NoOfSites, SiteName,
                                    SiteFunc);
   CHECK_RETURN (ret_code);


   /* create g1a */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_REC_G1a);
   strcpy(*(SiteName+1), SITE_NAME_INP_G1a);
   strcpy(*SiteFunc    , SITE_FUNC_REC_G1a);
   strcpy(*(SiteFunc+1), SITE_FUNC_INP_G1a);

   ret_code = bn_artmap_make_layer (1, 1, 1, G1a_X, G1a_Y, "g1a", HIDDEN, ACTF_G1a,
                                    OUTFUNC, NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create ria */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_INP_RIa);
   strcpy(*(SiteName+1), SITE_NAME_RHO_RIa);
   strcpy(*SiteFunc    , SITE_FUNC_INP_RIa);
   strcpy(*(SiteFunc+1), SITE_FUNC_RHO_RIa);

   ret_code = bn_artmap_make_layer (1, 1, 1, RIa_X, RIa_Y, "ria", HIDDEN, ACTF_RIa,
                                    OUTFUNC, NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rca */
   ret_code = bn_artmap_make_layer (1, 1, 1, RCa_X, RCa_Y, "rca", HIDDEN, ACTF_RCa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);


   /* create rga */
   ret_code = bn_artmap_make_layer (1, 1, 1, RGa_X, RGa_Y, "rga", HIDDEN, ACTF_RGa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create cla */
   ret_code = bn_artmap_make_layer (1, 1, 1, CLa_X, CLa_Y, "cla", HIDDEN, ACTF_CLa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create nca */
   ret_code = bn_artmap_make_layer (1, 1, 1, NCa_X, NCa_Y, "nca", HIDDEN, ACTF_NCa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create rhoa */
   ret_code = bn_artmap_make_layer (1, 1, 1, RHOa_X, RHOa_Y, "rhoa", HIDDEN, ACTF_RHOa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create g2a */
   ret_code = bn_artmap_make_layer (1, 1, 1, G2a_X, G2a_Y, "g2a", HIDDEN, ACTF_G2a,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);



   /********* create MAP-Field units ******************/

   /* create map layer */
   ret_code = bn_artmap_make_layer (f2bUnits, f2bRows, F2b_COLS, MAP_X, MAP_Y, "map", HIDDEN,
                                    ACTF_MAP, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d1 unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, D1_X, D1_Y, "d1", HIDDEN, ACTF_D1, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create G unit */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_ARTa_G);
   strcpy(*(SiteName+1), SITE_NAME_ARTb_G);
   strcpy(*SiteFunc    , SITE_FUNC_ARTa_G);
   strcpy(*(SiteFunc+1), SITE_FUNC_ARTb_G);

   ret_code = bn_artmap_make_layer (1, 1, 1, G_X, G_Y, "G", HIDDEN, ACTF_G, OUTFUNC,
                                    NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rb unit */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_B_RB);
   strcpy(*(SiteName+1), SITE_NAME_RHO_RB);
   strcpy(*SiteFunc    , SITE_FUNC_B_RB);
   strcpy(*(SiteFunc+1), SITE_FUNC_RHO_RB);

   ret_code = bn_artmap_make_layer (1, 1, 1, RB_X, RB_Y, "rb", HIDDEN, ACTF_RB, OUTFUNC,
                                    NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rm unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, RM_X, RM_Y, "rm", HIDDEN, ACTF_RM, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create rg unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, RG_X, RG_Y, "rg", HIDDEN, ACTF_RG, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create cl unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, CL_X, CL_Y, "cl", HIDDEN, ACTF_CL, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create nc unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, NC_X, NC_Y, "nc", HIDDEN, ACTF_NC, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create rho unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, RHO_X, RHO_Y, "rho", HIDDEN, ACTF_RHO, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create qu unit */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_INPa_QU);
   strcpy(*(SiteName+1), SITE_NAME_CMPa_QU);
   strcpy(*SiteFunc    , SITE_FUNC_INPa_QU);
   strcpy(*(SiteFunc+1), SITE_FUNC_CMPa_QU);

   ret_code = bn_artmap_make_layer (1, 1, 1, QU_X, QU_Y, "qu", HIDDEN, ACTF_QU, OUTFUNC,
                                    NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create drho unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, DRHO_X, DRHO_Y, "drho", HIDDEN, ACTF_DRHO, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);



   /********* create ARTb units ******************/


   /* create Input Layer for ARTb */
   ret_code = bn_artmap_make_layer (f1bUnits, f1bRows, F1b_COLS, INPb_X, INPb_Y, "inpb",
                                    INPUT, ACTF_INPb, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Comparison Layer for ARTb */
   ret_code = bn_artmap_make_layer (f1bUnits, f1bRows, F1b_COLS, CMPb_X, CMPb_Y, "cmpb",
                                    HIDDEN, ACTF_CMPb, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Recognition Layer for ARTb */
   ret_code = bn_artmap_make_layer (f2bUnits, f2bRows, F2b_COLS, RECb_X, RECb_Y, "recb",
                                    SPECIAL, ACTF_RECb, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Delay Layer for ARTa */
   ret_code = bn_artmap_make_layer (f2bUnits, f2bRows, F2b_COLS, DELb_X, DELb_Y, "delb",
                                    HIDDEN, ACTF_DELb, OUTFUNC, 0, NULL, NULL);

   /* create d1b */
   ret_code = bn_artmap_make_layer (1, 1, 1, D1b_X, D1b_Y, "d1b", HIDDEN, ACTF_Db,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d2b */
   ret_code = bn_artmap_make_layer (1, 1, 1, D2b_X, D2b_Y, "d2b", HIDDEN, ACTF_Db,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d3b */
   ret_code = bn_artmap_make_layer (1, 1, 1, D3b_X, D3b_Y, "d3b", HIDDEN, ACTF_Db,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create local reset layer */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_RSTb_SELF);
   strcpy(*(SiteName+1), SITE_NAME_RSTb_SIGNAL);
   strcpy(*SiteFunc    , SITE_FUNC_RSTb_SELF);
   strcpy(*(SiteFunc+1), SITE_FUNC_RSTb_SIGNAL);

   ret_code = bn_artmap_make_layer (f2bUnits, f2bRows, F2b_COLS, RSTb_X, RSTb_Y, "rstb",
                                    HIDDEN, ACTF_RSTb, OUTFUNC, NoOfSites, SiteName,
                                    SiteFunc);
   CHECK_RETURN (ret_code);


   /* create g1b */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_REC_G1b);
   strcpy(*(SiteName+1), SITE_NAME_INP_G1b);
   strcpy(*SiteFunc    , SITE_FUNC_REC_G1b);
   strcpy(*(SiteFunc+1), SITE_FUNC_INP_G1b);

   ret_code = bn_artmap_make_layer (1, 1, 1, G1b_X, G1b_Y, "g1b", HIDDEN, ACTF_G1b,
                                    OUTFUNC, NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rib */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_INP_RIb);
   strcpy(*(SiteName+1), SITE_NAME_RHO_RIb);
   strcpy(*SiteFunc    , SITE_FUNC_INP_RIb);
   strcpy(*(SiteFunc+1), SITE_FUNC_RHO_RIb);

   ret_code = bn_artmap_make_layer (1, 1, 1, RIb_X, RIb_Y, "rib", HIDDEN, ACTF_RIb,
                                    OUTFUNC, NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rcb */
   ret_code = bn_artmap_make_layer (1, 1, 1, RCb_X, RCb_Y, "rcb", HIDDEN, ACTF_RCb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);


   /* create rgb */
   ret_code = bn_artmap_make_layer (1, 1, 1, RGb_X, RGb_Y, "rgb", HIDDEN, ACTF_RGb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create clb */
   ret_code = bn_artmap_make_layer (1, 1, 1, CLb_X, CLb_Y, "clb", HIDDEN, ACTF_CLb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create ncb */
   ret_code = bn_artmap_make_layer (1, 1, 1, NCb_X, NCb_Y, "ncb", HIDDEN, ACTF_NCb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create rhob */
   ret_code = bn_artmap_make_layer (1, 1, 1, RHOb_X, RHOb_Y, "rhob", HIDDEN, ACTF_RHOb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create g2b */
   ret_code = bn_artmap_make_layer (1, 1, 1, G2b_X, G2b_Y, "g2b", HIDDEN, ACTF_G2b,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);



   /* Make Connections now */


   /********* Create Links **************************************/


   /********* Links to ARTa units ***********************/

   /* TO cmpa units */
   for (i=1; i<=f1aUnits; i++) {

      ret_code = krui_setCurrentUnit (CMPa_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM inpa units */
      ret_code = krui_createLink (INPa_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

      /* FROM g1a unit */
      ret_code = krui_createLink (G1a_UNIT, 0.0);
      CHECK_RETURN (ret_code);

      /* FROM dela units */
      for (j=1; j<=f2aUnits; j++) {
         ret_code = krui_createLink (DELa_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

   } /*for*/


   /* TO reca units */
   for (i=1; i<=f2aUnits; i++) {

      ret_code = krui_setCurrentUnit (RECa_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM cmpa units */
      for (j=1; j<=f1aUnits; j++) {
         ret_code = krui_createLink (CMPa_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM rga unit */
      ret_code = krui_createLink (RGa_UNIT, 0.0);
      CHECK_RETURN (ret_code);

      /* FROM rsta units */
      ret_code = krui_createLink (RSTa_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM g2a unit */
      ret_code = krui_createLink (G2a_UNIT, 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO dela units */
   for (i=1; i<=f2aUnits; i++) {

      ret_code = krui_setCurrentUnit (DELa_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM reca units */
      ret_code = krui_createLink (RECa_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO rsta units */
   for (i=1; i<=f2aUnits; i++) {

      ret_code = krui_setCurrentUnit (RSTa_UNIT(i));
      CHECK_RETURN (ret_code);

      /* first Site */
      ret_code = krui_setSite (SITE_NAME_RSTa_SELF);
      CHECK_RETURN (ret_code);

      /* FROM itself */
      ret_code = krui_createLink (RSTa_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* second Site */
      ret_code = krui_setSite (SITE_NAME_RSTa_SIGNAL);
      CHECK_RETURN (ret_code);

      /* FROM dela units */
      ret_code = krui_createLink (DELa_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM rga unit */
      ret_code = krui_createLink (RGa_UNIT, 0.0);
      CHECK_RETURN (ret_code);


   } /*for*/


   /* TO d1a unit */
   ret_code = krui_setCurrentUnit (Da_UNIT(1));
   CHECK_RETURN (ret_code);

   /* FROM dela units */
   for (j=1; j<=f2aUnits; j++) {
      ret_code = krui_createLink (DELa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO d2a unit */
   ret_code = krui_setCurrentUnit (Da_UNIT(2));
   CHECK_RETURN (ret_code);

   /* FROM d1a unit */
   ret_code = krui_createLink (Da_UNIT(1), 0.0);
   CHECK_RETURN (ret_code);


   /* TO d3a unit */
   ret_code = krui_setCurrentUnit (Da_UNIT(3));
   CHECK_RETURN (ret_code);

   /* FROM d2a unit */
   ret_code = krui_createLink (Da_UNIT(2), 0.0);
   CHECK_RETURN (ret_code);


   /* TO g1a unit */
   ret_code = krui_setCurrentUnit (G1a_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_REC_G1a);
   CHECK_RETURN (ret_code);

   /* FROM reca units */
   for (j=1; j<=f2aUnits; j++) {
      ret_code = krui_createLink (RECa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_INP_G1a);
   CHECK_RETURN (ret_code);

   /* FROM inpa units */
   for (j=1; j<=f1aUnits; j++) {
      ret_code = krui_createLink (INPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO ria unit */
   ret_code = krui_setCurrentUnit (RIa_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_INP_RIa);
   CHECK_RETURN (ret_code);

   /* FROM inpa units */
   for (j=1; j<=f1aUnits; j++) {
      ret_code = krui_createLink (INPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_RHO_RIa);
   CHECK_RETURN (ret_code);

   /* from rhoa unit */
   ret_code = krui_createLink (RHOa_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO rca unit */
   ret_code = krui_setCurrentUnit (RCa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM cmpa units */
   for (j=1; j<=f1aUnits; j++) {
      ret_code = krui_createLink (CMPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rga unit */
   ret_code = krui_setCurrentUnit (RGa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM ria unit */
   ret_code = krui_createLink (RIa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rca unit */
   ret_code = krui_createLink (RCa_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO cla unit */
   ret_code = krui_setCurrentUnit (CLa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM d3a unit */
   ret_code = krui_createLink (Da_UNIT(3), 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rga unit */
   ret_code = krui_createLink (RGa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM g2a unit */
   ret_code = krui_createLink (G2a_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO nca unit */
   ret_code = krui_setCurrentUnit (NCa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM rsta units */
   for (j=1; j<=f2aUnits; j++) {
      ret_code = krui_createLink (RSTa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* TO rhoa unit */
   ret_code = krui_setCurrentUnit (RHOa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM itself */
   ret_code = krui_createLink (RHOa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM drho unit */
   ret_code = krui_createLink (DRHO_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO g2a unit */
   ret_code = krui_setCurrentUnit (G2a_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM inpa units */
   for (j = 1; j <= f1aUnits; j++) {
      ret_code = krui_createLink (INPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /********* Links to MAP Fiels units ***********************/

   /* TO map units */
   for (i=1; i<=f2bUnits; i++) {

      ret_code = krui_setCurrentUnit (MAP_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM dela units */
      for (j=1; j<=f2aUnits; j++) {
         ret_code = krui_createLink (DELa_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM delb unit */
      ret_code = krui_createLink (DELb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM G unit */
      ret_code = krui_createLink (G_UNIT, 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO d1 unit */
   ret_code = krui_setCurrentUnit (D1_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM delb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (DELb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO G unit */
   ret_code = krui_setCurrentUnit (G_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_ARTa_G);
   CHECK_RETURN (ret_code);

   /* FROM reca units */
   for (j=1; j<=f2aUnits; j++) {
      ret_code = krui_createLink (RECa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_ARTb_G);
   CHECK_RETURN (ret_code);

   /* FROM recb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (RECb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rb unit */
   ret_code = krui_setCurrentUnit (RB_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_B_RB);
   CHECK_RETURN (ret_code);

   /* FROM d1 unit */
   ret_code = krui_createLink (D1_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_RHO_RB);
   CHECK_RETURN (ret_code);

   /* from rho unit */
   ret_code = krui_createLink (RHO_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO rm unit */
   ret_code = krui_setCurrentUnit (RM_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM map units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (MAP_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rg unit */
   ret_code = krui_setCurrentUnit (RG_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM rb unit */
   ret_code = krui_createLink (RB_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rm unit */
   ret_code = krui_createLink (RM_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO cl unit */
   ret_code = krui_setCurrentUnit (CL_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM cla unit */
   ret_code = krui_createLink (CLa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM clb unit */
   ret_code = krui_createLink (CLb_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rg unit */
   ret_code = krui_createLink (RG_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO nc unit */
   ret_code = krui_setCurrentUnit (NC_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM nca unit */
   ret_code = krui_createLink (NCa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM ncb unit */
   ret_code = krui_createLink (NCb_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* TO rho unit */
   ret_code = krui_setCurrentUnit (RHO_UNIT);
   CHECK_RETURN (ret_code);

   /* from itself */
   ret_code = krui_createLink (RHO_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO qu unit */
   ret_code = krui_setCurrentUnit (QU_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_INPa_QU);
   CHECK_RETURN (ret_code);

   /* FROM inpa units */
   for (j=1; j <= f1aUnits; j++) {
      ret_code = krui_createLink (INPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_CMPa_QU);
   CHECK_RETURN (ret_code);

   /* from cmpa units */
   for (j=1; j<=f1aUnits; j++) {
      ret_code = krui_createLink (CMPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO drho unit */
   ret_code = krui_setCurrentUnit (DRHO_UNIT);
   CHECK_RETURN (ret_code);

   /* from itself */
   ret_code = krui_createLink (DRHO_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from rg unit */
   ret_code = krui_createLink (RG_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from qu unit */
   ret_code = krui_createLink (QU_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from rhoa unit */
   ret_code = krui_createLink (RHOa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from rga unit */
   ret_code = krui_createLink (RGa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from clb unit */
   ret_code = krui_createLink (CLb_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /********* Links to ARTb units ***********************/

   /* TO cmpb units */
   for (i=1; i<=f1bUnits; i++) {

      ret_code = krui_setCurrentUnit (CMPb_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM inpb units */
      ret_code = krui_createLink (INPb_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

      /* FROM g1b unit */
      ret_code = krui_createLink (G1b_UNIT, 0.0);
      CHECK_RETURN (ret_code);

      /* FROM delb units */
      for (j=1; j<=f2bUnits; j++) {
         ret_code = krui_createLink (DELb_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

   } /*for*/


   /* TO recb units */
   for (i=1; i<=f2bUnits; i++) {

      ret_code = krui_setCurrentUnit (RECb_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM cmpb units */
      for (j=1; j<=f1bUnits; j++) {
         ret_code = krui_createLink (CMPb_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM rgb unit */
      ret_code = krui_createLink (RGb_UNIT, 0.0);
      CHECK_RETURN (ret_code);

      /* FROM rstb units */
      ret_code = krui_createLink (RSTb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM g2b unit */
      ret_code = krui_createLink (G2b_UNIT, 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO delb units */
   for (i=1; i<=f2bUnits; i++) {

      ret_code = krui_setCurrentUnit (DELb_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM recb units */
      ret_code = krui_createLink (RECb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM map units */
      ret_code = krui_createLink (MAP_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO rstb units */
   for (i=1; i<=f2bUnits; i++) {

      ret_code = krui_setCurrentUnit (RSTb_UNIT(i));
      CHECK_RETURN (ret_code);

      /* first Site */
      ret_code = krui_setSite (SITE_NAME_RSTb_SELF);
      CHECK_RETURN (ret_code);

      /* FROM itself */
      ret_code = krui_createLink (RSTb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* second Site */
      ret_code = krui_setSite (SITE_NAME_RSTb_SIGNAL);
      CHECK_RETURN (ret_code);

      /* FROM delb units */
      ret_code = krui_createLink (DELb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM rgb unit */
      ret_code = krui_createLink (RGb_UNIT, 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO d1b unit */
   ret_code = krui_setCurrentUnit (Db_UNIT(1));
   CHECK_RETURN (ret_code);

   /* FROM delb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (DELb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO d2b unit */
   ret_code = krui_setCurrentUnit (Db_UNIT(2));
   CHECK_RETURN (ret_code);

   /* FROM d1b unit */
   ret_code = krui_createLink (Db_UNIT(1), 0.0);
   CHECK_RETURN (ret_code);


   /* TO d3b unit */
   ret_code = krui_setCurrentUnit (Db_UNIT(3));
   CHECK_RETURN (ret_code);

   /* FROM d2b unit */
   ret_code = krui_createLink (Db_UNIT(2), 0.0);
   CHECK_RETURN (ret_code);


   /* TO g1b unit */
   ret_code = krui_setCurrentUnit (G1b_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_REC_G1b);
   CHECK_RETURN (ret_code);

   /* FROM recb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (RECb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_INP_G1b);
   CHECK_RETURN (ret_code);

   /* FROM inpb units */
   for (j=1; j<=f1bUnits; j++) {
      ret_code = krui_createLink (INPb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rib unit */
   ret_code = krui_setCurrentUnit (RIb_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_INP_RIb);
   CHECK_RETURN (ret_code);

   /* FROM inpb units */
   for (j=1; j<=f1bUnits; j++) {
      ret_code = krui_createLink (INPb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_RHO_RIb);
   CHECK_RETURN (ret_code);

   /* from rhob unit */
   ret_code = krui_createLink (RHOb_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO rcb unit */
   ret_code = krui_setCurrentUnit (RCb_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM cmpb units */
   for (j=1; j<=f1bUnits; j++) {
      ret_code = krui_createLink (CMPb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rgb unit */
   ret_code = krui_setCurrentUnit (RGb_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM rib unit */
   ret_code = krui_createLink (RIb_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rcb unit */
   ret_code = krui_createLink (RCb_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO clb unit */
   ret_code = krui_setCurrentUnit (CLb_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM d3b unit */
   ret_code = krui_createLink (Db_UNIT(3), 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rgb unit */
   ret_code = krui_createLink (RGb_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM g2b unit */
   ret_code = krui_createLink (G2b_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO ncb unit */
   ret_code = krui_setCurrentUnit (NCb_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM rstb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (RSTb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rhob unit */
   ret_code = krui_setCurrentUnit (RHOb_UNIT);
   CHECK_RETURN (ret_code);

   /* from itself */
   ret_code = krui_createLink (RHOb_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO g2b unit */
   ret_code = krui_setCurrentUnit (G2b_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM inpb units */
   for (j = 1; j <= f1bUnits; j++) {
      ret_code = krui_createLink (INPb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


  /*  set the update function  */
  ret_code = krui_setUpdateFunc (UPDATE_FUNC_NAME);
  CHECK_RETURN( ret_code );

  /* set the learning function */
  ret_code = krui_setLearnFunc (LEARN_FUNC_NAME);

   ui_NumberOfLearnParamsChanged();
   ui_NumberOfUpdateParamsChanged();
   return (ret_code);
}




/*****************************************************************************
  FUNCTION : bn_artmap_donePROC

  PURPOSE  : Callback function for DONE button in artmap
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/

static void bn_artmap_donePROC (void)

{

   XtDestroyWidget (baseWidget);
   bn_artmap_open = 0;

}



/*****************************************************************************
  FUNCTION : bn_art1_createPROC

  PURPOSE  : Callback function for CREATE NET button in art1
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/

static void bn_artmap_createPROC (void)

{
  int units[4], rows[4];
  int NoOfLayers = 4;

  if (bn_basics_check_existingNetwork()) {

     bn_basics_getValues (NoOfLayers, units, rows, artmapUnitWidget, artmapRowWidget);

     if (bn_basics_checkValues(NoOfLayers, units, rows)) {
        bn_artmap_createNet (units[0], rows[0], units[1], rows[1],
                             units[2], rows[2], units[3], rows[3]);

        bn_basics_refresh ();
        ui_confirmOk ("Network created!");
     } /*if*/

  } /*if*/

  return;

}



/*****************************************************************************
  FUNCTION : bn_createARTMAP

  PURPOSE  : create ARTMAP panel
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/

void bn_createARTMAP(void)

{
  bn_basics_createART (ARTMAP_MODEL, &baseWidget, &bn_artmap_open,
                       artmapUnitWidget, artmapRowWidget,
                       (XtCallbackProc) bn_artmap_createPROC,
                       (XtCallbackProc) bn_artmap_donePROC);


}


/******************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/linknets.c,v $
  SHORTNAME      : snnslinknets.c
  SNNS VERSION   : 4.2

  PURPOSE        : Combination of multiple SNNS feedforward networks to one
                   combined net

  AUTHOR         : Michael Vogt
  DATE           : 09.05.97
  LAST UPDATE    : 09.05.97

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.11 $
  LAST CHANGE    : $Date: 1998/05/25 16:01:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
  
  used files     : glob_typ.h, kr_ui.h      from kernel/sources
                   libkernel.a              from kernel/bin/<architecture>
                   functions.h, templates.h from actual directory
******************************************************************************/
#include <config.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#include <time.h>
#include <memory.h>
#include "glob_typ.h"
#include "kr_ui.h"
#include "functions.h"
#include "snns2clib.h"

#include "kr_typ.h"
#include "kernel.h"  

#undef debug

#define MAX_NO_OF_NETWORKS 20
#define KR_ERROR_RETURN(x) { fprintf(stderr, "%s\n", krui_error(x)); return ERR; }

/* Macros for calculating the minimum or maximum of two values */
#define MAX(a, b) ( (a > b) ? (a) : (b) )
#define MIN(a, b) ( (a < b) ? (a) : (b) )

/* Status (Error) Codes : OK = 0 (NO Error), ERR = 1, ...  */
typedef enum { OK=0, ERR, CANT_ADD, CANT_LOAD, MEM_ERR,
		   WRONG_PARAM, WRONG_ACT_FUNC, CANT_OPEN,
		   ILLEGAL_CYCLES, NO_CPN, NO_TDNN, NOT_SUPPORTED} Status;

/* Recordtype for Layers */
typedef struct {
  int   number;			/* of the Layer (not used yet) */
  pList members;		/* Numbers of all member-units */
  pList sources;		/* numbers of all sources of the member-units */
  int   type;			/* INPUT , OUTPUT ...               */
  int   ActFunc;		/* No in the ActivationFunctionList */
  char  *name;			/* Name of the Layer */
} tLayer, *pLayer;

/* Recordtype for Units */
typedef struct {
    int    number;		/* of the Unit                         */
    pList  sources;		/* numbers of the source-Units         */
    float  *weights;	        /* Link-Weights to the Source-Units    */
    int    ActFunc;		/* No in the ActivationFunctionList    */
    int    type;		/* INPUT , OUTPUT ...                  */
    char   *name;		/* Name of the unit, given by the user */
    float  Bias;		/* Bias of the unit                    */
    pLayer layer;		/* Pointer to the layer containing the unit */
    float  act;                 /* Initial Activation of the Unit      */
    char   *ActFuncName;	/* Name of ActivationFunction          */
    char   *OutFuncName;	/* Name of OutputFunction              */
    int    xpos;                /* unit position */
    int    ypos;                /* unit position */
    int    zpos;                /* unit position */

    int    kernel_nr;           /* kernel internal unit number */
} tUnit, *pUnit;

#define DEFAULT_INPUT_UNIT_INITIALIZER \
{ 0, NULL, NULL, 0, INPUT, "input", 0.0, NULL, 0.0, "Act_Logistic", "Out_Identity", 0, 0, 0, 0 }

#define DEFAULT_OUTPUT_UNIT_INITIALIZER \
{ 0, NULL, NULL, 0, OUTPUT, "output", 0.0, NULL, 0.0, "Act_Logistic", "Out_Identity", 0, 0, 0, 0 }

#define IsInputUnit(x) ((x).type == INPUT || (x).type == SPECIAL_I)
#define IsOutputUnit(x) ((x).type == OUTPUT || (x).type == SPECIAL_O)
#define IsSpecialIHOUnit(x) ((x).type == SPECIAL_I || (x).type == SPECIAL_H || (x).type == SPECIAL_O)

typedef struct {
    int maxx;
    int maxy;
    int minx;
    int miny;
    int no_of_inputs;
    int no_of_outputs;
} tNet, *pNet;

static char *Act_Identity_String = "Act_IdentityPlusBias";

/*
 *  Output-Functions for debug-informations
 */

void printLayer(pLayer layer)
{
  int i;
  printf("\nLayer %d", layer->number);
  printf("\nmembers: ");
  for (i = 0; i < NoOf(layer->members); i++) {
    printf("%d ", element(layer->members, i) );
  }
  printf("\nsources: ");
  for (i = 0; i < NoOf(layer->sources); i++) {
    printf("%d ", element(layer->sources, i) );
  }
  printf("\n");
}


void printUnit(pUnit unit)
{
  int i;
  printf("\nUnit %d", unit->number);
  printf("\nsources: ");
  for (i = 0; i < NoOf(unit->sources); i++) {
    printf("%9d ", element(unit->sources, i) );
  }
  printf("\nweights: ");
  for (i = 0; i < NoOf(unit->sources); i++) {
    printf("%9f ", unit->weights[i] );
  }
  printf("\n");
}  


/*****************************************************************************
  FUNCTION : getpUnit

  PURPOSE  : Return the array element for the unit with a given number
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
pUnit getpUnit(pUnit Units, int NoOfUnits, int number)
{
    int i;

    for (i=0; i<NoOfUnits && Units[i].number != number; i++)
	;

    if (i>=NoOfUnits)
	return NULL;
    else
	return &Units[i];    
}


/*****************************************************************************
  FUNCTION : checkErr

  PURPOSE  : writes an error message if needed and also stops program if 
             errCode stands for a fatal error
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void checkErr(int errCode)
{
  switch(errCode) 
      {
      case OK          : ;
	break;
      case ERR         : printf("unspecified Error\n");
	break;
      case CANT_ADD    :
      case MEM_ERR     : printf("not enough memory\n");
	break;
      case CANT_LOAD   : printf("can't load file\n");
	break;
      case WRONG_PARAM : printf("wrong parameters\n");
	break;
      case CANT_OPEN   : printf("can't open file\n");
	break;
      case NO_CPN      : printf("net is not a CounterPropagation network\n");
	break;
      case NO_TDNN     : printf("net is not a Time Delay Neural Network\n");
	break;
      case ILLEGAL_CYCLES : printf("net contains illegal cycles\n");
	break;
      case WRONG_ACT_FUNC : ;
	break;
      case NOT_SUPPORTED  : printf("not supported network type\n");
	break;
      default             : printf("unknown error code : %d\n", errCode);
      }
}


/*****************************************************************************
  FUNCTION : is_BPTT_net

  PURPOSE  : checks, if the current Net is a BPTT, BBPTT or QPTT Network by 
             testing the learning functions.
  RETURNS  : TRUE  if net is a kind of BPTT network, FALSE otherwise
  NOTES    :

  UPDATE   : 
******************************************************************************/
bool is_BPTT_net(void)
{
  return (   (0 == strcmp("BPTT", krui_getLearnFunc() ))
	  || (0 == strcmp("BBPTT", krui_getLearnFunc() ))
	  || (0 == strcmp("QPTT", krui_getLearnFunc() )) );
}


/*****************************************************************************
  FUNCTION : checkLearnFunc

  PURPOSE  : checks, if the Learning Function is supported by linknets
  RETURNS  : Error Code : OK / NOT_SUPPORTED
  NOTES    :

  UPDATE   : 
******************************************************************************/
int checkLearnFunc(void)
{
  static char *NotSupportedLearnFuncs[] = {
    "ART1", "ART2", "ARTMAP", "BackPercolation", "Hebbian", "RM_delta",
    "Kohonen", NULL
  };

  char *LearnFunc = krui_getLearnFunc();     /* learning function of the network */
  char **string   = NotSupportedLearnFuncs;  /* current function name to test */

  while(*string) {
    if (!strcmp(*string, LearnFunc)) {	     /* e.g. the same function-name */
      return(NOT_SUPPORTED);
    }
    string++;
  }
  
  return(OK);
}


/*****************************************************************************
  FUNCTION : checkActFunc

  PURPOSE  : checks, if an activation Function with the name actName is present
  RETURNS  : number in the function table or -1 if not present
  NOTES    :

  UPDATE   : 
******************************************************************************/
int checkActFunc(char *actName)
{
  int i=0;
  
  while (**(ACT_FUNC_NAMES + i) ) {
    if (!strcmp(ACT_FUNC_NAMES[i], actName) ) return (i);
    i++;
  }
  fprintf(stderr, "Can't find the function <%s>\n", actName);
  return(-1);
}


/*****************************************************************************
  FUNCTION : initLayer

  PURPOSE  :  initialize the given layer e.g :
              - initializes the two lists members and sources
              - insert the type and the Number of the activation-function of the
                given Unit in the predefined places.
              - inserts the sources of the given Unit into the list sources

  RETURNS  : error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
int initLayer(pLayer layer, pUnit unit)
{
  layer->members = newList();		   /* a list for member unit */
  if (!layer->members) return(MEM_ERR);

  layer->sources = newList();		   /* a list for all predecessor */
  if (!layer->sources) return(MEM_ERR);	   /* units of all members       */
  
  addList(layer->members, unit->number);   /* prototype unit is first member */
  if (copyList(layer->sources, unit->sources) ) return(MEM_ERR);

  layer->ActFunc = unit->ActFunc;
  layer->type    = unit->type;

  unit->layer = layer;

  return(OK);
}


/*****************************************************************************
  FUNCTION : matchLayer

  PURPOSE  : checks if the unit could be in the same layer as the other units 
             which are in the layer yet
  RETURNS  : TRUE the unit matches with the other units, FALSE otherwise
  NOTES    :

  UPDATE   : 
******************************************************************************/
int matchLayer(pLayer layer, pUnit unit)
{
  static int is_BPTT = 0, first_time = 1;

  /* a special flag is set to avoid unneeded function calls */
  if (first_time) {
    is_BPTT = is_BPTT_net();
    first_time = 0;
  }

  /* input neurons are all treated the same way */
  if ( (unit->type == INPUT) && (layer->type == INPUT) ) return (TRUE);

  /* unit should match the attributes of the Layer */
  if (unit->type != layer->type) return(FALSE);
  if (unit->ActFunc != layer->ActFunc) return(FALSE);

  /* BPTT-nets have no topological order */
  if (is_BPTT) return (TRUE);

  /* unit must not be a member of the source units */ 
  if (isMember(layer->sources, unit->number) ) return (FALSE);

  /* Neue Version von Matthias Oderdorfer */
  return ( CompareSources(unit->sources, layer->sources) );
 
  /* alte Version */
  /* a member of the layer must not be a source element of the unit */
  /* return( !haveIntersection(unit->sources, layer->members) ); */
}


/*****************************************************************************
  FUNCTION : searchLayer

  PURPOSE  : searches a layer with matches the unit (or an empty Layer) and 
            inserts the unit in the layer.
  RETURNS  : MEM_ERR : not enough Memory / OK: no problems
  NOTES    :

  UPDATE   : 
******************************************************************************/
int searchLayer(pUnit unit, pLayer globalLayers)
{
  pLayer layer;

  layer = globalLayers;
  while(TRUE) {
    if (layer->members == NULL) {	   /* empty layer found */
      return(initLayer(layer, unit));	   /* give possible Errors to caller */
    }
    else if (matchLayer(layer, unit) ) {   /* matching layer found */
      if (addList(layer->members, unit->number)) {
	return(MEM_ERR);
      }
      unit->layer = layer; 
      return(mergeList(layer->sources, unit->sources)); /* returns Status */
    }
    layer++;
  }
}


/*****************************************************************************
  FUNCTION : divideNet

  PURPOSE  : parts a net into groups and prepares the Net for sorting the
             layers.
  RETURNS  : MEM_ERR : not enough Memory / OK: no problems
  NOTES    :

  UPDATE   : 
******************************************************************************/
int divideNet(pUnit globalUnits, pLayer globalLayers, int netnumber)
{
  int       unitNo, sourceNo;	      /* number of the unit and source unit */
  pUnit     unit;       	      /* unit and prototype unit */
  FlintType dummy, weight;	      /* link weights */
  int       error;		      /* error code    */
  char      *string;		      /* free variable */
  int       pos;		      /* free variable */
  struct PosType unitpos;


  /* loading all Units and group them into Layers  */
  unitNo = krui_getFirstUnit(); 
  unit   = globalUnits;
  
  while (unitNo) {
    unit->number  = unitNo;

    /* copy the entries from SNNS to the own format */
    unit->act     = krui_getUnitActivation(unitNo);
    unit->type    = krui_getUnitTType(unitNo);
    unit->Bias    = krui_getUnitBias(unitNo);

    unit->ActFuncName = strdup(krui_getUnitActFuncName(unitNo));
    unit->OutFuncName = strdup(krui_getUnitOutFuncName(unitNo));
    krui_getUnitPosition(unitNo, &unitpos);
    unit->xpos = unitpos.x;
    unit->ypos = unitpos.y;
    unit->zpos = unitpos.z;

    /* units always have a name (at least its old number) */
    string = krui_getUnitName(unitNo);
    if (NULL == string) {
      unit->name = malloc(12 * sizeof(char));
      if(! unit->name) {
	return (MEM_ERR);
      }
      sprintf(unit->name, "N%dU%d", netnumber, unit->number);
    }
    else {
      unit->name = malloc(MAX(1,strlen(string)+1));
      if(! unit->name) {
	return (MEM_ERR);
      }
      strcpy(unit->name, string);
    }

    unit->ActFunc = checkActFunc(krui_getUnitActFuncName(unitNo) );
    if (unit->ActFunc < 0) return(WRONG_ACT_FUNC);

    /* insert all Source units in the list */
    unit->sources = newList();
    if (!unit->sources) return (MEM_ERR);
    sourceNo = krui_getFirstPredUnit(&dummy);
    while (sourceNo) {
      /* only special-hidden-neurons may have links to itself */
      if ( (unit->type != SPECIAL_H) && !is_BPTT_net() ) {
        if (unit->number == sourceNo) return(ILLEGAL_CYCLES);
      }
      if(addList(unit->sources, sourceNo)) return (MEM_ERR);
      sourceNo = krui_getNextPredUnit(&dummy);
    }
    /* now the weights can be written in the right order */
    /* One more Element is allocated, because the array might have size 0 */ 
    unit->weights = (float *)malloc(NoOf(unit->sources) * 
				    sizeof(float) + sizeof(float));
    if (!unit->weights) return(MEM_ERR);
    sourceNo = krui_getFirstPredUnit(&weight);
    while (sourceNo) {
      pos = searchList(unit->sources, sourceNo);
      unit->weights[pos] = weight;
      sourceNo = krui_getNextPredUnit(&weight);
    }
#ifdef debug
    printUnit(unit);
#endif
    error = searchLayer(unit, globalLayers);
		if (error) return(error);

    unit++;
    unitNo = krui_getNextUnit();
  }

  return(OK);
}


/*****************************************************************************
  FUNCTION : checkOrder

  PURPOSE  : checks the order between layer x and layer y
  RETURNS  : -1       : layer x before layer y
              0       : indifferent
              1       : layer x after layer y
              ILLEGAL_CYCLES : error occured (no order given)

  NOTES    : SIDE-EFFECT : the entry SuccDelay is set up, if one layer 
             follows immediatly another. (needed by TDNN-networks)


  UPDATE   : 
******************************************************************************/
signed char checkOrder(pLayer globalLayers, int x, int y)
{
  /* preference of the unit type : low value means early update */
  static char pref[12] = {0, 0, 2, 0, 1, 3, 3, 3, 3, 3, 3, 3};

  signed char order = 0;

  if (pref[globalLayers[x].type] < pref[globalLayers[y].type]) {
    order = (signed char)-1;	/* e.g. layer x before layer y */
  }
  else if (pref[globalLayers[x].type] > pref[globalLayers[y].type]) {
    order = 1;			/* e.g. layer x after layer y */ 
  }

	/* BPTT-Networks may contain any cycles so they must not be checked */
	if (is_BPTT_net() ) {
		return(order);
	}

  if (haveIntersection(globalLayers[x].sources, globalLayers[y].members) ) {	        
    if (order == -1) {
      if (SPECIAL_H != globalLayers[y].type) return (ILLEGAL_CYCLES);
    }
    else {
      order = 1;
    }
  }
  if (haveIntersection(globalLayers[x].members, globalLayers[y].sources) ) {		
    if (order == 1) {
      if (SPECIAL_H != globalLayers[x].type) return (ILLEGAL_CYCLES);
    }
    else {
      order = (signed char)-1;
    }
  }

  return(order);
}


/*****************************************************************************
  FUNCTION : sortNet

  PURPOSE  : calculates the order between the global layers an returns it in 
             the array order. So the first number in the array is the first 
             layer to be updated and so on.
  RETURNS  : MEM_ERR : not enough Memory / OK: no problems
  NOTES    :

  UPDATE   : 
******************************************************************************/
int sortNet(pLayer globalLayers, int NoOfLayers, int *order)
{
  char **matrix;		/* precedence matrix        */
  char  *mask;			/* already chosen layers    */
  int    i, j, x, y, ord, isSource = TRUE;
  char   precedence;

  /* reserve memory for all the arrays and matrices */
  matrix = (char **)malloc(NoOfLayers * sizeof(char *) );
  if (!matrix) return (MEM_ERR);
  
  for (i = 0; i < NoOfLayers; i++) {
    matrix[i] = (char *)calloc(NoOfLayers, sizeof(char) );
    if (!matrix[i]) return (MEM_ERR);
  }
  
  mask  = (char *)calloc(NoOfLayers, sizeof(char) );
  if (!mask) {
		free(matrix);
		return (MEM_ERR);
	}

  /* build the precedence matrix of the Layer-Graph */
  for (y = 0; y < NoOfLayers; y++) {
    for (x = y + 1; x < NoOfLayers; x++) {
      precedence = checkOrder(globalLayers, x, y);
      if (precedence == ILLEGAL_CYCLES) return(ILLEGAL_CYCLES);

      matrix[x][y] = precedence;
      matrix[y][x] = -precedence;

    } /* for x */

  } /* for y */

#ifdef debug
  printf("\nPrecedence Matrix is:\n");
  for (y = 0; y < NoOfLayers; y++) {
    for (x = 0; x < NoOfLayers; x++) {
      printf("%3d ", matrix[y][x]);
    }
    printf("\n");
  }
#endif

  /* put the Layers in the right order **/
  for (ord = 0; ord < NoOfLayers; ord++) {

    for (i = 0; i < NoOfLayers; i++) {
      if (mask[i]) continue;	/* Layer already chosen */
      
      isSource = TRUE;
      for (j = 0; j < NoOfLayers; j++) {
        /* exists a layer wich must be updated before ? */
        if (matrix[i][j] == 1) {
          isSource = FALSE;
          break;
        }
      }

      if (isSource) {
        order[ord] = i;		/* the number of the Layer becomes ord */
        mask[i] = 1;		/* must not test this Layer again      */

        for (j = 0; j < NoOfLayers; j++) {
          matrix[j][i] = 0;	/* clear depencies for other Layers */
        }

        break;			/* find next Layer */
      }

    } /* for i */

    if (!isSource) {
      return(ILLEGAL_CYCLES);
    }

  } /* for ord */

  for (i = 0; i < NoOfLayers; i++) {
    free(matrix[i]);
  }
  free(matrix);
  free(mask);

#ifdef debug
  printf("\nLayers sorted in following order :\n");
  for (i = 0; i < NoOfLayers; i++) {
    printf(" %d", order[i]);
  }
  printf("\n");
#endif

  return(OK);
}


/*****************************************************************************
  FUNCTION : add_input_sources

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void add_input_sources(pUnit Units, int NoOfUnits, int sources)
{
    int i;

    for (i=0; i<NoOfUnits; i++)
    {
	if (IsInputUnit(Units[i]))
	{
	    Units[i].sources = newList();
	    Units[i].weights = (float *) malloc(sources * sizeof(float));
	}
    }    
}


/*****************************************************************************
  FUNCTION : adjust_unit_numbers

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int adjust_unit_numbers(pUnit Units, int NoOfUnits, int offset)
{
    int new_offset = offset;
    int i, j;

    for (i=0; i<NoOfUnits; i++)
    {
	Units[i].number += offset;
	new_offset = MAX(new_offset, Units[i].number);
	if (Units[i].sources)
	{
	    for (j=0; j<Units[i].sources->no; j++)
	    {
		Units[i].sources->values[j] += offset;
	    }
	}
    }

    return new_offset;
}


/*****************************************************************************
  FUNCTION : move_networks_down

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void move_networks_down(pUnit *UnitsP, int *NoOfUnitsP, int n, int offset)
{
    int i, j;

    for (i=0; i<n; i++)
    {
	for (j=0; j<NoOfUnitsP[i]; j++)
	    UnitsP[i][j].ypos += offset;
    }
}


/*****************************************************************************
  FUNCTION : adjust_network_positions

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int adjust_network_positions(pUnit *UnitsP, int *NoOfUnitsP, pNet NetinfoP, 
			     int n, int offset, int *height)
{
    int maxwidth = 0;
    int i;
    int j;
    int yoffset=0;

    for (i=0; i<n; i++)
    {
	for (j=0; j<NoOfUnitsP[i]; j++)
	{
	    UnitsP[i][j].xpos += offset - NetinfoP[i].minx;
	    UnitsP[i][j].ypos += yoffset - NetinfoP[i].miny;
	}
	yoffset += 2 + NetinfoP[i].maxy - NetinfoP[i].miny;
    }

    for (i=0; i<n; i++)
	maxwidth = MAX(maxwidth, 1 + NetinfoP[i].maxx - NetinfoP[i].minx);

    *height = yoffset-1;
    return maxwidth + offset;
}


/*****************************************************************************
  FUNCTION : make_new_link

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void make_new_link(tUnit from, tUnit to, float weight)
{
    addList(to.sources, from.number);
    to.weights[searchList(to.sources, from.number)] = weight;
}


/*****************************************************************************
  FUNCTION : connect_input_units

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void connect_input_units(pUnit NewUnits, int nnew,
			 pUnit OldUnits, int nold, int full)
{
    int opos;
    int npos=0;

    for (opos=0; opos<nold; opos++)
	if (IsInputUnit(OldUnits[opos]))
	{
	    if (full)
	    {
		for (npos=0; npos<nnew; npos++)
		    make_new_link(NewUnits[npos], OldUnits[opos], 0.0);
		OldUnits[opos].type = HIDDEN;
	    }
	    else
	    {
		if (npos >= nnew)
		{
		    fprintf(stderr, "%s: internal error\n", 
			    "connect_input_units");
		    exit(2);
		}
		make_new_link(NewUnits[npos], OldUnits[opos], 1.0);
		OldUnits[opos].type = SPECIAL_H;
		OldUnits[opos].Bias = 0.0;
		OldUnits[opos].ActFuncName = Act_Identity_String;
		npos++;
	    }
	}
}


/*****************************************************************************
  FUNCTION : connect_output_units

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void connect_output_units(pUnit NewUnits, int nnew, pUnit OldUnits, int nold)
{
    int opos;
    int npos;

    for (opos=0; opos<nold; opos++)
	if (IsOutputUnit(OldUnits[opos]))
	{
	    for (npos=0; npos<nnew; npos++)
		make_new_link(OldUnits[opos], NewUnits[npos], 0.0);
	    if (IsSpecialIHOUnit(OldUnits[opos]))
		OldUnits[opos].type = SPECIAL_H;
	    else
		OldUnits[opos].type = HIDDEN;
	}
}


/*****************************************************************************
  FUNCTION : connect_inter_units

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void connect_inter_units(pUnit OldOutputUnits, int nout,
			 pUnit OldInputUnits, int nin)
{
    int opos;
    int npos;

    for (opos=0; opos<nout; opos++)
	if (IsOutputUnit(OldOutputUnits[opos]))
	    for (npos=0; npos<nin; npos++)
		if (IsInputUnit(OldInputUnits[npos]))
		    make_new_link(OldOutputUnits[opos], 
				  OldInputUnits[npos], 0.0);
}


/*****************************************************************************
  FUNCTION : multi_connect_inter_units

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void multi_connect_inter_units(pUnit *InputUnits, int *InputNoOfUnits, 
			       int ninputs,
			       pUnit *OutputUnits, int *OutputNoOfUnits, 
			       int noutputs, int nconnects)
{
    int inet=0;
    int ipos=0;
    int onet=0;
    int opos=0;

    while (nconnects>0)
    {
	while (!IsOutputUnit(InputUnits[inet][ipos]))
	{
	    ipos++;
	    if (ipos >= InputNoOfUnits[inet])
	    {
		ipos = 0;
		inet++;
		if (inet >= ninputs)
		{
		    fprintf(stderr, "%s: internal error\n",
			    "multi_connect_inter_unit");
		    exit(2);
		}
	    }
	}
	while (!IsInputUnit(OutputUnits[onet][opos]))
	{
	    opos++;
	    if (opos >= OutputNoOfUnits[onet])
	    {
		opos = 0;
		onet++;
		if (onet >= noutputs)
		{
		    fprintf(stderr, "%s: internal error\n",
			    "multi_connect_inter_units");
		    exit(2);
		}
	    }
	}
	make_new_link(InputUnits[inet][ipos],
		      OutputUnits[onet][opos],
		      1.0);

	if (IsSpecialIHOUnit(InputUnits[inet][ipos]))
	    InputUnits[inet][ipos].type = SPECIAL_H;
	else
	    InputUnits[inet][ipos].type = HIDDEN;

	OutputUnits[onet][opos].type = SPECIAL_H;
	OutputUnits[onet][opos].Bias = 0.0;
	OutputUnits[onet][opos].ActFuncName = Act_Identity_String;
	nconnects--;
    }
}


/*****************************************************************************
  FUNCTION : substitute_output_units

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void substitute_output_units(pUnit Units, int n, int newtype)
{
    int newspecialtype;

    switch (newtype)
    {
      case INPUT:
	newspecialtype = SPECIAL_I;
	break;
      case HIDDEN:
	newspecialtype = SPECIAL_H;
	break;
      case OUTPUT:
	newspecialtype = SPECIAL_O;
	break;
      default:
	newspecialtype = newtype;
	break;
    }

    while (--n>=0)
    {
	if (IsOutputUnit(Units[n]))
	{
	    if (IsSpecialIHOUnit(Units[n]))
		Units[n].type = newspecialtype;
	    else
		Units[n].type = newtype;
	}
    }
}


/*****************************************************************************
  FUNCTION : substitute_input_units

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void substitute_input_units(pUnit Units, int n, int newtype)
{
    int newspecialtype;

    switch (newtype)
    {
      case INPUT:
	newspecialtype = SPECIAL_I;
	break;
      case HIDDEN:
	newspecialtype = SPECIAL_H;
	break;
      case OUTPUT:
	newspecialtype = SPECIAL_O;
	break;
      default:
	newspecialtype = newtype;
	break;
    }

    while (--n>=0)
    {
	if (IsInputUnit(Units[n]))
	{
	    if (IsSpecialIHOUnit(Units[n]))
		Units[n].type = newspecialtype;
	    else
		Units[n].type = newtype;
	}
    }
}


/*****************************************************************************
  FUNCTION : usage

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void usage(char *name)
{
    fprintf(stderr, "usage : %s -innets <netfile> ... [ -outnets <netfile> ... ] -o <output network file> [ options ]\n",
	    name);
    fprintf(stderr, "options:    -inunits        use copies of input units\n");
    fprintf(stderr, "            -inconnect <n>  fully connect with <n> input units\n");
    fprintf(stderr, "            -direct         connect input with output one-to-one\n");
    fprintf(stderr, "            -outconnect <n> fully connect to <n> output units\n");
    fprintf(stderr, "\n            -inunits and -inconnect may not be used together\n");
    fprintf(stderr, "\n!!!! PLEASE REFER TO THE MANUAL FOR DETAILED DESCRIPTION!!!!\n"); 
    exit(1);
}


/*****************************************************************************
  FUNCTION : create_and_save_network

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int create_and_save_network(char *NetFileName, pUnit Units, int NoOfUnits)
{
    int i, j;
    krui_err krerr;
    pUnit suP;

    struct PosType unitpos;
    
    krui_deleteNet();
    for (i=0; i<NoOfUnits; i++)
    {
	Units[i].kernel_nr = krui_createUnit(Units[i].name, 
					     Units[i].OutFuncName,
					     Units[i].ActFuncName,
					     Units[i].act,
					     Units[i].Bias);
	if (Units[i].kernel_nr <= 0)
	    KR_ERROR_RETURN(Units[i].kernel_nr);
	if ((krerr=krui_setUnitTType(Units[i].kernel_nr, Units[i].type)) != KRERR_NO_ERROR)
	    KR_ERROR_RETURN(krerr);
	unitpos.x = Units[i].xpos;
	unitpos.y = Units[i].ypos;
	unitpos.z = Units[i].zpos;
	krui_setUnitPosition(Units[i].kernel_nr, &unitpos);
    }

    for (i=0; i<NoOfUnits; i++)
    {
	if (Units[i].sources)
	{
	    if ((krerr=krui_setCurrentUnit(Units[i].kernel_nr)) != KRERR_NO_ERROR)
		KR_ERROR_RETURN(krerr);
	    for (j=0; j<Units[i].sources->no; j++)
	    {
		suP = getpUnit(Units, NoOfUnits, Units[i].sources->values[j]);
		if (!suP)
		    return ERR;
		if ((krerr=krui_createLink(suP->kernel_nr, 
					   Units[i].weights[j])) != KRERR_NO_ERROR)
		    KR_ERROR_RETURN(krerr);
	    }
	}
    }

    if ((krerr=krui_saveNet(NetFileName, "snnslinknet")) != KRERR_NO_ERROR)
	KR_ERROR_RETURN(krerr);

    return OK;
}


/*****************************************************************************
  FUNCTION : load_and_analyze_network

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int load_and_analyze_network(int netnumber, char *NetFileName, pUnit *UnitsP, 
			     pLayer *LayersP, int *NoOfUnitsP, int *NoOfLayersP,
			     int **orderP, pNet NetinfoP)
{
    krui_err err;			     /* error code of SNNS - krui */

    char *netname;
    int error;
    int nr;

#ifdef debug
    printf("loading net %s \n", NetFileName);
#endif

    /* load Net */
    err = krui_loadNet(NetFileName, &netname);
    if (err) {
	fprintf(stderr, "%s\n", krui_error(err) );
	return(CANT_LOAD);
    }

    error = checkLearnFunc();
    if (error) {
	checkErr(error);
	return(error);
    }

    *NoOfUnitsP = krui_getNoOfUnits();

    *UnitsP  =  (pUnit)calloc((*NoOfUnitsP + 1), sizeof(tUnit) );  /* because of sentinels */
    if (! *UnitsP) {
	checkErr(MEM_ERR);
	return(MEM_ERR);
    }

    *LayersP = (pLayer)calloc((*NoOfUnitsP + 1), sizeof(tLayer) ); /* because of sentinels */
    if (! *LayersP) {
	free(*UnitsP);
	checkErr(MEM_ERR);
	return(MEM_ERR);
    }

    for (nr = 0; nr <= *NoOfUnitsP; nr++) {
	(*LayersP)[nr].number  = nr;
    }

#ifdef debug
    printf("dividing net into layers ...\n");
#endif

    /* part Net into groups */
    error =  divideNet(*UnitsP, *LayersP, netnumber);
    if (error) {
	checkErr(error);
	return(error);
    }

    /* count the Non-empty Layers */
    for (nr = 0; (*LayersP)[nr].members != NULL; nr++);
    *NoOfLayersP = nr;
    *orderP = (int *)malloc(*NoOfLayersP * sizeof(int) );


    /* count the real number of units  */
    /* unused units have the number 0 the total Number can't exeed the   */
    /* Number of Units given by the SNNS-Interface-Function              */
    for(nr = 0; ((*UnitsP)[nr].number != 0) && (nr < *NoOfUnitsP); nr++) {
    }
    *NoOfUnitsP = nr;
  
#ifdef debug
    for(nr = 0; nr < *NoOfLayersP; printLayer(*LayersP + nr++) );
#endif

    /* topological sort of the layers */
#ifdef debug
    printf("sorting layers ...\n");
#endif

    error = sortNet(*LayersP, *NoOfLayersP, *orderP);
    if (error) {
	checkErr(error);
	return(error);
    }
  
    /* determine overall network info */
    NetinfoP->maxx = (*UnitsP)[0].xpos;
    NetinfoP->maxy = (*UnitsP)[0].ypos;
    NetinfoP->minx = NetinfoP->maxx;
    NetinfoP->miny = NetinfoP->maxy;
    NetinfoP->no_of_inputs = 0;
    NetinfoP->no_of_outputs = 0;
    for (nr=0; nr< *NoOfUnitsP; nr++)
    {
	NetinfoP->maxx = MAX((*UnitsP)[nr].xpos, NetinfoP->maxx);
	NetinfoP->maxy = MAX((*UnitsP)[nr].ypos, NetinfoP->maxy);
	NetinfoP->minx = MIN((*UnitsP)[nr].xpos, NetinfoP->minx);
	NetinfoP->miny = MIN((*UnitsP)[nr].ypos, NetinfoP->miny);
	if (IsInputUnit((*UnitsP)[nr]))
	    NetinfoP->no_of_inputs++;
	if (IsOutputUnit((*UnitsP)[nr]))
	    NetinfoP->no_of_outputs++;
    }
    return OK;
}


/*****************************************************************************
  FUNCTION : main

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int main(int argc, char **argv)
{
    int      ninputs = 0;
    char     *InputNetFileNames[MAX_NO_OF_NETWORKS];	     /* input files */
    pUnit    InputUnits[MAX_NO_OF_NETWORKS];    /* all input unit variables */
    pLayer   InputLayers[MAX_NO_OF_NETWORKS];  /* all input layer variables */
    tNet     InputNetinfo[MAX_NO_OF_NETWORKS];

    pUnit    NewInputUnits;

    int      noutputs = 0;
    char     *OutputNetFileNames[MAX_NO_OF_NETWORKS];       /* output files */
    pUnit    OutputUnits[MAX_NO_OF_NETWORKS];  /* all output unit variables */
    pLayer   OutputLayers[MAX_NO_OF_NETWORKS];/* all output layer variables */
    tNet     OutputNetinfo[MAX_NO_OF_NETWORKS];

    pUnit    NewOutputUnits;

    pUnit    FinalUnits;
    int      FinalNoOfUnits;

    int      InputNoOfUnits[MAX_NO_OF_NETWORKS], 
	     InputNoOfLayers[MAX_NO_OF_NETWORKS]; 
    int      *Inputorder[MAX_NO_OF_NETWORKS];    /* array with the order of 
						    the sorted layers */

    int      OutputNoOfUnits[MAX_NO_OF_NETWORKS], 
	     OutputNoOfLayers[MAX_NO_OF_NETWORKS];	 
    int      *Outputorder[MAX_NO_OF_NETWORKS];	 /* array with the order of 
						    the sorted layers */

    char     *outputnetworkfile;
    int      inunits = 0;
    int      inconnect = 0;
    int      interconnect = 1;
    int      outconnect = 0;

    int      total_no_of_in_inputs = 0;
    int      total_no_of_in_outputs = 0;
    int      total_no_of_out_inputs = 0;
    int      total_no_of_out_outputs = 0;

    int      equal_in_input_sizes = 1;
    int      equal_in_output_sizes = 1;
    int      equal_out_input_sizes = 1;
    int      equal_out_output_sizes = 1;

    int      total_no_of_units = 0;
    int      no_of_new_input_units = 0;
    int      no_of_new_output_units = 0;

    int i, j;
    int offset;
    int posoffset;
    int yoffseti = 0;
    int yoffseto = 0;
    int height = 0;

    static tUnit default_input_unit = DEFAULT_INPUT_UNIT_INITIALIZER;
    static tUnit default_output_unit = DEFAULT_OUTPUT_UNIT_INITIALIZER;

    char unitname[20];

    /* process options */

    i = 0;
    if (++i>=argc) usage(argv[0]);

    if (strcmp(argv[i], "-innets"))
	usage(argv[0]);

    if (++i>=argc) usage(argv[0]);
    while (i<argc && argv[i][0] != '-')
    {
	InputNetFileNames[ninputs] = strdup(argv[i]);
	ninputs++;
	if (ninputs >= MAX_NO_OF_NETWORKS)
	{
	    fprintf(stderr, "to many input networks\n");
	    exit(1);
	}
	i++;
    }
    if (ninputs <= 0)
	usage(argv[0]);

    if (i>=argc)usage(argv[0]);

    if (strcmp(argv[i], "-outnets") == 0)
    {
	if (++i>=argc) usage(argv[0]);
	while (i<argc && argv[i][0] != '-')
	{
	    OutputNetFileNames[noutputs] = strdup(argv[i]);
	    noutputs++;
	    if (noutputs >= MAX_NO_OF_NETWORKS)
	    {
		fprintf(stderr, "to many output networks\n");
		exit(1);
	    }
	    i++;
	}
    }

    if (strcmp(argv[i], "-o"))
	usage(argv[0]);

    if (++i>=argc) usage(argv[0]);


    outputnetworkfile = strdup(argv[i]);
    i++;


    while (i<argc)
    {
	if (strcmp(argv[i], "-inunits") == 0)
	    inunits = 1;
	else if (strcmp(argv[i], "-inconnect") == 0)
	{
	    if (++i>=argc) usage(argv[0]);
	    inconnect = atoi(argv[i]);
	}
	else if (strcmp(argv[i], "-direct") == 0)
	    interconnect = 0;
	else if (strcmp(argv[i], "-outconnect") == 0)
	{
	    if (++i>=argc) usage(argv[0]);
	    outconnect = atoi(argv[i]);
	}
	else
	    usage(argv[0]);
	i++;
    }
    if (inunits && inconnect)
	usage(argv[0]);

    if (!interconnect && noutputs==0)
    {
	fprintf(stderr, "no output networks defined, option -direct ignored\n");
	interconnect = 1;
    }

    for (i=0; i<ninputs; i++)
    {
	if (load_and_analyze_network(i+1, InputNetFileNames[i], 
				     &InputUnits[i], &InputLayers[i], 
				     &InputNoOfUnits[i], &InputNoOfLayers[i], 
				     &Inputorder[i], &InputNetinfo[i]) != OK)
	    exit(1);

	total_no_of_in_inputs += InputNetinfo[i].no_of_inputs;
	total_no_of_in_outputs += InputNetinfo[i].no_of_outputs;
	total_no_of_units += InputNoOfUnits[i];

	if (i>0)
	{
	    if (InputNetinfo[i].no_of_inputs !=
		InputNetinfo[i-1].no_of_inputs)
		equal_in_input_sizes = 0;
	    if (InputNetinfo[i].no_of_outputs !=
		InputNetinfo[i-1].no_of_outputs)
		equal_in_output_sizes = 0;
	}
    }

    for (i=0; i<noutputs; i++)
    {
	if (load_and_analyze_network(i+1+ninputs, OutputNetFileNames[i], 
				     &OutputUnits[i], &OutputLayers[i], 
				     &OutputNoOfUnits[i], &OutputNoOfLayers[i], 
				     &Outputorder[i], &OutputNetinfo[i]) != OK)
	    exit(1);

	total_no_of_out_inputs += OutputNetinfo[i].no_of_inputs;
	total_no_of_out_outputs += OutputNetinfo[i].no_of_outputs;
	total_no_of_units += OutputNoOfUnits[i];

	if (i>0)
	{
	    if (OutputNetinfo[i].no_of_inputs !=
		OutputNetinfo[i-1].no_of_inputs)
		equal_out_input_sizes = 0;
	    if (OutputNetinfo[i].no_of_outputs !=
		OutputNetinfo[i-1].no_of_outputs)
		equal_out_output_sizes = 0;
	}

    }

    /* consitency check */
    if (inunits && !equal_in_input_sizes)
    {
	fprintf(stderr, "illegal use of option -inunits\n"
		"input networks are of different input sizes\n");
	usage(argv[0]);
    }
    if (noutputs>0 && !interconnect && 
	total_no_of_in_outputs != total_no_of_out_inputs)
    {
	fprintf(stderr, "illegal use of option -direct\n"
		"overall sum of input network output sizes differs from overall sum of output network input sizes\n");
	usage(argv[0]);
    }

    /* determine numbers of additional units */
    if (inunits)
	no_of_new_input_units = InputNetinfo[0].no_of_inputs;
    else if (inconnect)
	no_of_new_input_units = inconnect;

    if (outconnect)
	no_of_new_output_units = outconnect;

    /* reassign unit numbers */
    offset = no_of_new_input_units + 1;
    for (i=0; i<ninputs; i++)
	offset = adjust_unit_numbers(InputUnits[i],InputNoOfUnits[i],offset)+1;
    for (i=0; i<noutputs; i++)
       offset = adjust_unit_numbers(OutputUnits[i],OutputNoOfUnits[i],offset)+1;

    /* create new units */
    NewInputUnits  = (pUnit) calloc(no_of_new_input_units + 1, sizeof(tUnit) );
    NewOutputUnits  = (pUnit) calloc(no_of_new_output_units + 1, sizeof(tUnit));
    FinalUnits = (pUnit) calloc(offset + no_of_new_output_units + 1, 
				sizeof(tUnit) );
    
    if (! NewOutputUnits || ! NewInputUnits || ! FinalUnits) {
	checkErr(MEM_ERR);
	exit(1);
    }

    /* move unit positions for graphical output */
    posoffset = 1 + adjust_network_positions(InputUnits, InputNoOfUnits, 
					 InputNetinfo, ninputs, 
					 no_of_new_input_units > 0 ? 
					 2 : 0, &yoffseti);
    if (noutputs > 0)
	posoffset = 1 + adjust_network_positions(OutputUnits, OutputNoOfUnits, 
						 OutputNetinfo, noutputs, 
						 posoffset, &yoffseto);
    
    height = MAX(yoffseti, yoffseto);
    if (noutputs > 0)
    {
	if (yoffseti < yoffseto)
	    move_networks_down(InputUnits, InputNoOfUnits, ninputs,
			       (yoffseto - yoffseti)/2);
	else
	    move_networks_down(OutputUnits, OutputNoOfUnits, noutputs,
			       (yoffseti - yoffseto)/2);
    }

    /* create new input and output units */
    for (i=0; i<no_of_new_input_units; i++)
    {
	NewInputUnits[i] = default_input_unit;
	NewInputUnits[i].sources = newList();
	NewInputUnits[i].number = i;
	NewInputUnits[i].xpos = 0;
	NewInputUnits[i].ypos = i + (no_of_new_input_units < height ? 
				     (height-no_of_new_input_units)/2 : 0);
	sprintf(unitname, "in%d", i+1);
	NewInputUnits[i].name = strdup(unitname);
    }
    for (i=0; i<no_of_new_output_units; i++)
    {
	NewOutputUnits[i] = default_output_unit;
	NewOutputUnits[i].sources = newList();
	NewOutputUnits[i].weights = 
	    (float *) malloc(MAX(total_no_of_out_outputs, 
				 total_no_of_in_outputs) * sizeof(float));
	NewOutputUnits[i].number = i+offset;
	NewOutputUnits[i].xpos = posoffset;
	NewOutputUnits[i].ypos = i + (no_of_new_output_units < height ? 
				      (height-no_of_new_output_units)/2 : 0);
	sprintf(unitname, "out%d", i+1);
	NewOutputUnits[i].name = strdup(unitname);
    }

    /* modify former input units to receive input from other source units */
    for (i=0; i<ninputs; i++)
	add_input_sources(InputUnits[i], InputNoOfUnits[i], 
			  no_of_new_input_units);
    for (i=0; i<noutputs; i++)
	add_input_sources(OutputUnits[i], OutputNoOfUnits[i], 
			  total_no_of_in_outputs);

    /* connect new input unit structure if necessary */
    for (i=0; i<ninputs; i++)
    {
	if (inunits)
	{
	    connect_input_units(NewInputUnits, no_of_new_input_units,
				InputUnits[i], InputNoOfUnits[i], 0);
	}
	else if (inconnect)
	{
	    connect_input_units(NewInputUnits, no_of_new_input_units,
				InputUnits[i], InputNoOfUnits[i], 1);
	}
    }

    /* connect new output unit structure if necessay */
    if (outconnect)
    {
	if (noutputs > 0)
	    for (i=0; i<noutputs; i++)
	    {
		connect_output_units(NewOutputUnits, no_of_new_output_units,
				     OutputUnits[i], OutputNoOfUnits[i]);
	    }
	else
	    for (i=0; i<ninputs; i++)
	    {
		connect_output_units(NewOutputUnits, no_of_new_output_units,
				     InputUnits[i], InputNoOfUnits[i]);
	    }
    }

    /* interconnect networks */
    if (interconnect && noutputs>0)
    {
	for (i=0; i<ninputs; i++)
	    for (j=0; j<noutputs; j++)
		connect_inter_units(InputUnits[i], InputNoOfUnits[i],
				    OutputUnits[j], OutputNoOfUnits[j]);
	for (i=0; i<ninputs; i++)
	    substitute_output_units(InputUnits[i], InputNoOfUnits[i],
				    HIDDEN);
	for (i=0; i<noutputs; i++)
	    substitute_input_units(OutputUnits[i], OutputNoOfUnits[i],
				   HIDDEN);
    }
    else if (noutputs>0)
	multi_connect_inter_units(InputUnits, InputNoOfUnits, ninputs,
				  OutputUnits, OutputNoOfUnits, noutputs,
				  total_no_of_in_outputs);
			
    /* combine all networks to one single unit array */
    FinalNoOfUnits = 0;
    for (i=0; i<no_of_new_input_units; i++)
	FinalUnits[FinalNoOfUnits++] = NewInputUnits[i];
    for (i=0; i<ninputs; i++)
	for (j=0; j<InputNoOfUnits[i]; j++)
	    FinalUnits[FinalNoOfUnits++] = InputUnits[i][j];
    for (i=0; i<noutputs; i++)
	for (j=0; j<OutputNoOfUnits[i]; j++)
	    FinalUnits[FinalNoOfUnits++] = OutputUnits[i][j];
    for (i=0; i<no_of_new_output_units; i++)
	FinalUnits[FinalNoOfUnits++] = NewOutputUnits[i];

#ifdef debug
    for (i=0; i<FinalNoOfUnits; i++)
	printUnit(&FinalUnits[i]);
#endif

    /* save created network */
    if(create_and_save_network(outputnetworkfile, FinalUnits,
			       FinalNoOfUnits) != OK)
	exit(1);

    exit(0);
}


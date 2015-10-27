/******************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/snns2c.c,v $
  SHORTNAME      : snns2c.c
  SNNS VERSION   : 4.2

  PURPOSE        : Converter for SNNS-net -Files to  executable C-Source
                   only for Feed-Forward- and Elman-nets.

                   Loads the net with the functions given by the
                   kernel-interface of the SNNS-kernel. 
                   Groups similar units in layers and sorts the layers 
                   topologically.
                   Saves the Net as a C function.
  
  Updates : TDNN (01.11.94 - 04.01.95)
            DLVQ (05.01.95 - 10.01.95)
	    CPN  (11.01.95 - 30.01.95)
            generated HeaderFiles         (31.01.95)
	    update function with pointers (01.02.95)
            BBTT (02.02.95 - 20.02.95)
	    releasing all memory (23.02.95)

  AUTHOR         : Bernward Kett
  DATE           : 31.08.94
  LAST UPDATE    : 23.02.95 (Bernward Kett)

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.19 $
  LAST CHANGE    : $Date: 1998/04/22 16:48:12 $

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
#include "templates.h"
#include "snns2clib.h"

#include "kr_typ.h"
#include "kernel.h"  /* should be removed, when the kr_ui.h is updated */

#undef debug
/* 
#define debug
*/

/* Macros for calculating the minimum or maximum of two values */
#define MAX(a, b) ( (a > b) ? (a) : (b) )
#define MIN(a, b) ( (a < b) ? (a) : (b) )

/* Macro for releasing memory of units and layers */
#define FREE_ALL freeUnits(Units); freeLayers(Layers); free(TDNN_prot);

/* Status (Error) Codes : OK = 0 (NO Error), ERR = 1, ...  */
typedef enum { OK, ERR, CANT_ADD, CANT_LOAD, MEM_ERR,
		   WRONG_PARAM, WRONG_ACT_FUNC, CANT_OPEN,
		   ILLEGAL_CYCLES, NO_CPN, NO_TDNN, NOT_SUPPORTED} Status;

/* Recordtype for Layers */
typedef struct {
  int   number;			/* of the Layer (not used yet) */
  pList members;		/* Numbers of all member-units */
  pList sources;		/* numbers of all sources of the member-units */
  int   type;			  /* INPUT , OUTPUT ...               */
  int   ActFunc;		/* No in the ActivationFunctionList */
  char  *name;			/* Name of the Layer */
  /* Special entries for TDNN */
  int   TotalDelay;		/* Total Delay Number of the Layer */
  int   delay;			/* Delay of the receptive Field  */
  int   SuccDelay;		/* Delay of the following Layer */
  char  *readCounter;		/* Name of the DelayPointer      */ 
  char  *writeCounter;		/* Name of the WriteDelayPointer */
} tLayer, *pLayer;

/* Recordtype for Units */
typedef struct {
  int    number;		/* of the Unit                         */
  pList  members;		/* Units with the same Prototype (TDNN)*/
  pList  sources;		/* numbers of the source-Units         */
  float  *weights;	        /* Link-Weights to the Source-Units    */
  int    ActFunc;		/* No in the ActivationFunctionList    */
  int    type;			/* INPUT , OUTPUT ...                  */
  char   *name;			/* Name of the unit, given by the user */
  float  Bias;			/* Bias of the unit                    */
  pLayer layer;			/* Pointer to the layer wich contains the unit */
  /* Special entries for BPTT and Elman/Jordan */
  float  act;                   /* Initial Activation of the Unit      */
  /* Special entries for CPN */
  float *dest;      /* weights from Hidden to Output written in the hidden units */
  int   NoOfDest;   /* Numbers of the weights to the Output */
  /* Special entries for TDNN */
  int    index;			/* Index in the global Array           */
  int   FeatureWidth;		/* Number of Prototype Source units    */
  int   DelayLength;		/* Delay Length of the receptive Field */
  int   **TDNNsources;		/* special Format for TDNNs            */
  float **TDNNweights;		/* special Format for TDNNs            */
} tUnit, *pUnit;

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


void printTDNNunit(pUnit unit)
{
  int i, j;
  printf("\nUnit %d", unit->number);
  printf("\nmembers : ");
  for (i = 0; i < NoOf(unit->members); i++) {
    printf("%d ", element(unit->members, i) );
  }
  printf("\nsources: \n");
  for (i = 0; i < unit->FeatureWidth; i++) {
    for (j = 0; j < NoOf(unit->sources) / unit->FeatureWidth; j++) {
      printf("%8d ", unit->TDNNsources[i][j]);
    }
    printf("\n");
  }
  printf("weights: \n");
  for (i = 0; i < unit->FeatureWidth; i++) {
    for (j = 0; j < NoOf(unit->sources) / unit->FeatureWidth; j++) {
      printf("%8f ", unit->TDNNweights[i][j]);
    }
    printf("\n");
  }
}

/******************************************************************************
  void toAlphaNum(char *string)
  -----------------------------------------------------------------------------
  replaces all characters in string wich are not alphanumerical with '_'.
  Furtheron it deletes all characters before first '/'.
  If the first Character is a digit, it is replaced by 'X'
  ******************************************************************************/
void toAlphaNum(char *string)
{
  char *pointer;

  pointer = string - 1;		/* because of increment in while-condition */
  while( *(++pointer) != '\0') { /* to the end of the String                */
    while (*pointer == '/') {	/* only the tail of Path-names             */
      strcpy(string, pointer + 1);
      pointer = string;
    }
    if (!isalnum(*pointer) ) *pointer = '_'; /* no special characters */
  }
  if (*string == '\0') strcpy (string, "NOTHING"); /* no empty strings      */
  if (isdigit(*string) ) *string = 'X';	/* no leading digit      */
}


/******************************************************************************
  void checkErr(int errCode)
  -----------------------------------------------------------------------------
  writes an error message if needed and also stops program if errCode
  stands for a fatal error
  -> errCode : code of the actual status
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
  bool is_TDNN_net(void)
  ----------------------------------------------------------------------------
  checks, if the current Net is a Time Delay Neural Network by testing
  the learning functions.
  <- (func) : TRUE  Net is a TDNN Network
  FALSE otherwise
  *****************************************************************************/
bool is_TDNN_net(void)
{
  return (0 == strcmp("TimeDelayBackprop", krui_getLearnFunc() ) );
}


/*****************************************************************************
  bool is_DLVQ_net(void)
  ----------------------------------------------------------------------------
  checks, if the current Net is a Dynamic LVQ Network by testing
  the learning functions.
  <- (func) : TRUE  Net is a TDNN Network
  FALSE otherwise
  *****************************************************************************/
bool is_DLVQ_net(void)
{
  return (0 == strcmp("Dynamic_LVQ", krui_getLearnFunc() ));
}


/*****************************************************************************
  bool is_CPN_net(void)
  ----------------------------------------------------------------------------
  checks, if the current Net is a CounterPropagation Network by testing
  the learning functions.
  <- (func) : TRUE  Net is a TDNN Network
  FALSE otherwise
  *****************************************************************************/
bool is_CPN_net(void)
{
  return (0 == strcmp("Counterpropagation", krui_getLearnFunc() ));
}


/*****************************************************************************
  bool is_BPTT_net(void)
  ----------------------------------------------------------------------------
  checks, if the current Net is a BPTT, BBPTT or QPTT Network by testing
  the learning functions.
  <- (func) : TRUE  Net is a kind of BPTT Network
  FALSE otherwise
  *****************************************************************************/
bool is_BPTT_net(void)
{
  return (   (0 == strcmp("BPTT", krui_getLearnFunc() ))
	  || (0 == strcmp("BBPTT", krui_getLearnFunc() ))
	  || (0 == strcmp("QPTT", krui_getLearnFunc() )) );
}


/*****************************************************************************
  int checkLearnFunc(void)
  ----------------------------------------------------------------------------
  checks, if the Learning Function is supported by the snns2c
  <- Error Code : OK / NOT_SUPPORTED
  *****************************************************************************/
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
  bool is_Prototype_Unit(int UnitNr)
  ----------------------------------------------------------------------------
  checks, if the current unit is a prototype unit, e.g. if the links are 
  defined in this unit.
  
  Note : Function is only needed for TDNN
  
  <- (func) : TRUE  unit is a prototype unit
  FALSE otherwise
  *****************************************************************************/
bool is_Prototype_Unit(int UnitNr)
{
  struct Unit *unit_ptr;

  if ((unit_ptr = kr_getUnitPtr( UnitNr ) ) == NULL) {
    return (FALSE);
  }
  else {
    return ( unit_ptr->TD.target_offset == 0);
  }  
}


/****************************************************************************
  int get_Prototype_Unit(int(unitNr)
  ---------------------------------------------------------------------------
  returns the number of the prototype Unit and 0 if an error occurs
  
  Note : Function is only needed for TDNN
  
  <- (func) : Number of the Prototype Unit
              (0 = Unit not found)
  *****************************************************************************/
int get_Prototype_Unit(int UnitNr)
{
  struct Unit *unit_ptr;

  if ((unit_ptr = kr_getUnitPtr( UnitNr ) ) == NULL) {
    return (0);
  }
  else {
    return (UnitNr + unit_ptr->TD.target_offset);
  }
}


/****************************************************************************
  pUnit searchUnit(int UnitNr, pUnit global Units, pUnit lastUnit)
  ---------------------------------------------------------------------------
  returns a pointer to the Unit wich has the current number UnitNr
  
  Note : Function is only needed for TDNN
  
  -> UnitNr      : Number of the UNit in the original SNNS network
  globalUnits : Array of all existing snns2c-units
  <- index       : index of the unit, found in the Array 
  (func)      : pointer to the unit or NULL if the unit isn't found
*****************************************************************************/
pUnit searchUnit(int UnitNr, pUnit globalUnits, int *index)
{
  pUnit unit;

  *index = 0;
  for (unit = globalUnits; unit->number > 0; unit++, (*index)++) {
    if (unit->number == UnitNr) return (unit);
  }

  return(NULL);
}


/*****************************************************************************
  int checkActFunc(char *actName)
  ----------------------------------------------------------------------------
  checks, if an activation Function with the name actName is present
  -> actName : Name of the activation function
  <- (func)  : number in the function table or -1 if not present
*****************************************************************************/
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
  int initLayer(pLayer layer, pUnit unit)
  ----------------------------------------------------------------------------
  initialize the given layer e.g :
  - initializes the two lists members and sources
  - insert the type and the Number of the activation-function of the
  given Unit in the predefined places.
  - inserts the sources of the given Unit into the list sources
  
  -> layer : pointer to the new Layer
  unit  : pointer to the first (e.g.prototype-) unit
  <- (func) : Status (Errorcodes)
  *****************************************************************************/
int initLayer(pLayer layer, pUnit unit)
{
  layer->members = newList();		   /* a list for member unit */
  if (!layer->members) return(MEM_ERR);

  layer->sources = newList();		   /* a list for all predecessor */
  if (!layer->sources) return(MEM_ERR);	   /* units of all members       */
  
  addList(layer->members, unit->number);   /* prototype unit is the first member */
  if (copyList(layer->sources, unit->sources) ) return(MEM_ERR);

  layer->ActFunc = unit->ActFunc;
  layer->type    = unit->type;

  unit->layer = layer;

  return(OK);
}


/******************************************************************************
  int matchLayer(pLayer layer, pUnit unit)
  -----------------------------------------------------------------------------
  checks if the unit could be in the same layer as the other units wich
  are in the layer yet
  -> layer : pointer to the (existing) layer
  unit  : pointer to the unit wich is to be prooved
  <- (func) TRUE the unit matches with the other units
  FALSE otherwise
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


/******************************************************************************
  int searchLayer(pUnit unit, pLayer globalLayers)
  -----------------------------------------------------------------------------
  searches a Layer with matches the unit (or an empty Layer) and inserts the
  unit in the layer.
  -> unit         : actual unit wich searches a friendly layer
  globalLayers : array of all Layers
  <- (func) MEM_ERR : not enough Memory
  OK      : no problems
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


/******************************************************************************
  int divideNet(pUnit globalUnits, pLayer globalLayers, int *TDNN_prot)
  -----------------------------------------------------------------------------
  parts a net into groups and prepares the Net for sorting the
  layers.
  
  -> globalUnits  : all the Units are going to be written in this array
  globalLayers : all the different Layers will be written in this array
  <- globalUnits  : (the values in the array)
  globalLayers : (the values in the array)
  TDNN_prot    : The numbers of the prototype units of each unit
  <- (func) MEM_ERR : not enough Memory
  OK      : no problems
  ******************************************************************************/
int divideNet(pUnit globalUnits, pLayer globalLayers, int *TDNN_prot)
{
  int       unitNo, sourceNo;		   /* number of the unit and source unit */
  int       prototypeNo;                   /* number of the prototype unit */
  pUnit     unit, prot_unit;		   /* unit and prototype unit */
  FlintType dummy, weight;		   /* link weights */
  bool      isTDNN;			   /* flag for TDNN networks */
  int       error;			   /* error code    */
  char      *string;			   /* free variable */
  int       pos;			   /* free variable */

  /* Testing if the network is a TDNN-network */
  isTDNN = is_TDNN_net();

  /* --------------------------------------------
   * loading all Units and group them into Layers 
   */
  unitNo = krui_getFirstUnit(); 
  unit   = globalUnits;
  
  while (unitNo) {
    unit->members = newList();
    if (!unit->members) return (MEM_ERR);
    
    /* TDNN-Units wich are not Prototype-Units are
     * treated in a special way ...
     */
    if (isTDNN && !is_Prototype_Unit(unitNo) ) {
      prototypeNo = get_Prototype_Unit(unitNo);
      TDNN_prot[unitNo] = prototypeNo;	         /* fix the Number of the prototype Unit */
      prot_unit = searchUnit(prototypeNo, globalUnits, &pos);
      if (!prot_unit) return(ERR);		 /* prototype unit not found */
      addList(prot_unit->members, unitNo);
      unitNo = krui_getNextUnit();
      continue;					 /* nothing else to do */ 
    }
    
    /* 
     * TDNN Prototype-Units are treated as normal units
     */
    unit->number  = unitNo;
    addList(unit->members, unitNo);	/* the Prototype is also part of the member list */
    TDNN_prot[unitNo] = unitNo;		/* the prototype unit is his own prototype */

    /* copy the entries from SNNS to the own format */
    unit->act     = krui_getUnitActivation(unitNo);
    unit->type    = krui_getUnitTType(unitNo);
    unit->Bias    = krui_getUnitBias(unitNo);

    /* units always have a name (at least its old number) */
    string = krui_getUnitName(unitNo);
    if (NULL == string) {
      unit->name = malloc(12 * sizeof(char));
      if(! unit->name) {
	return (MEM_ERR);
      }
      sprintf(unit->name, "Old: %d", unit->number);
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
    unit->weights = (float *)malloc(NoOf(unit->sources) * sizeof(float) + sizeof(float));
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


/****************************************************************************
  int prepareCpnUnits(pUnit globalUnits, pLayer global Layers, int NoOfLayers)
  ---------------------------------------------------------------------------
  calculates the entries in the Units, wich are necessary for CPN
  -> globalUnits  : all units of the net
     globalLayers : all layers of the net
  <- (func)  MEM_ERR : Not enough memory
  ERR     : Error in structure of the Network
  OK      : No problems found
  ****************************************************************************/
int prepareCpnUnits(pUnit globalUnits, pLayer globalLayers)
{
  pLayer HiddenLayer, OutputLayer;       /* pointer to the hidden and Output layer */
  int    memberNr, sourceNr, dummy;      /* loop-variables                         */
  
  /* CPN-Nets need a fixed architecture : */
  if (   (globalLayers[0].type != INPUT) 
      || (globalLayers[1].type != HIDDEN)
      || (globalLayers[2].type != OUTPUT)
      || (globalLayers[3].members != NULL)  /* there must not exist a forth layer */
      ) {
    return(NO_CPN);
  }
  
  /* better to read */
  HiddenLayer = globalLayers + 1; 
  OutputLayer = HiddenLayer + 1; 
  
  
  for (memberNr = 0; memberNr < NoOf(HiddenLayer->members); memberNr++) {
    pUnit unit = searchUnit(element(HiddenLayer->members, memberNr), globalUnits, &dummy);
    if (unit == NULL) return (NO_TDNN);
    
    if (NULL == (unit->dest = malloc(NoOf(OutputLayer->members))) ) {
      return(MEM_ERR);
    }
  }
  
  for (memberNr = 0; memberNr < NoOf(OutputLayer->members); memberNr++) {
    pUnit dest = searchUnit(element(OutputLayer->members, memberNr), globalUnits, &dummy);
    if (dest == NULL) return (NO_CPN);
    
    /* Hiddenlayer and Outputlayer must be fully connected */ 
    if (NoOf(dest->sources) != NoOf(HiddenLayer->members)) {
      return(NO_CPN);
    }
    
    /* copy the weights to the hidden units */
    for(sourceNr = 0; sourceNr < NoOf(dest->sources); sourceNr++) {
      pUnit source = searchUnit(element(dest->sources,sourceNr), globalUnits, &dummy);
      if (source == NULL) return (NO_CPN);
      printUnit(source);
      source->dest[memberNr] = dest->weights[sourceNr];
      source->NoOfDest = NoOf(OutputLayer->members);
    }
  }
  
  return(OK);
}


/****************************************************************************
  int prepareTDNNunits(pUnit globalUnits, int *TDNN_prot)
  ---------------------------------------------------------------------------
  calculates the entries in the Units, wich are necessary for TDNNs
  -> globalUnits  : all units of the net
  TDNN_prot    : Array with the numbers of the needed prototypes
  <- (func)  MEM_ERR : Not enough memory
  NO_TDNN : Error in structure of the Network
  MEM_ERR : not enough memory
  OK      : No problems found
  ****************************************************************************/
int prepareTDNNunits(pUnit globalUnits, int *TDNN_prot)
{
  int   sourceNo;		  /* Unit Number of the current source unit         */
  int   protNo;			  /* Unit Number of the current prototype Unit      */
  pUnit unit;			    /* Current examined Unit                          */
  pList ProtSources;  /* List with the prototype source units           */
  int   *countArray;	/* Counters for the entries in the raws           */
  int   raw;			    /* current raw in the weight matrix of a unit     */
  int   listNo;			  /* Number of a list entry                         */
  int   i, delay;

  countArray = malloc(sizeof(int)); /* necessary because of the reallocs */
	if(! countArray) {
		return MEM_ERR;
	}
 
  for(unit = globalUnits; unit->number != 0; unit++) { /* e.g. All Units */
    
    ProtSources = newList();	/* An own list for each unit */
		if(! ProtSources) {
			free(countArray);
			return MEM_ERR;
		}

    /* finding all Prototype Units in the source Units */
    for (listNo = 0; listNo < NoOf(unit->sources); listNo++) {

      sourceNo = element(unit->sources, listNo);

      if (sourceNo == TDNN_prot[sourceNo]) { /* e.g. his own prototype */
        addList(ProtSources, sourceNo);
      }

    }
    
    if (NoOf(unit->sources) == 0) { /* Must be an input unit           */
      killList(ProtSources);	/* e.g. has no sources and weights */
      unit->FeatureWidth = 0;
      unit->DelayLength = 0;
      continue;			/* so nothing to do                */
    }

    /* A simple check (delay * width = Number of Sources) */
    if ( (NoOf(unit->sources) % NoOf(ProtSources)) != 0 ) {
      printf("Unit %d has wrong count of Source Units", unit->number);
      return(NO_TDNN);
    }

    unit->FeatureWidth = NoOf(ProtSources);
    unit->DelayLength  = NoOf(unit->sources) / unit->FeatureWidth;

    /* get Memory for the new data structures            */
    /* width := NoOf(ProtSources); Delay Length := delay */
    countArray        = (int *)realloc(countArray, NoOf(ProtSources) * sizeof(int) );
    unit->TDNNsources = (int **)malloc(NoOf(ProtSources) * sizeof (int *) );
    unit->TDNNweights = (float **)malloc(NoOf(ProtSources) * sizeof (float *) );
  
    delay = unit->DelayLength;
    for(i = 0; i < NoOf(ProtSources); i++) {
      unit->TDNNsources[i] = (int *)malloc(delay * sizeof(int));
      unit->TDNNweights[i] = (float *)malloc(delay * sizeof(float));
      if(unit->TDNNweights[i] == NULL) return (MEM_ERR);
    }

    /** write New Datas **/
    /* At first one Prototype Unit for each raw */ 
    for(raw = 0; raw < NoOf(ProtSources); raw++) {
      sourceNo = element(ProtSources, raw);
      unit->TDNNsources[raw][0] = sourceNo;
      countArray[raw] = 1;
    }

    /* Then writing the other weights and sources */
    for(i = 0; i < NoOf(unit->sources); i++) {
      sourceNo = element(unit->sources, i);
      protNo   = TDNN_prot[sourceNo];

      /* Searching the raw with the Prototype unit */
      for(raw = 0; raw < NoOf(ProtSources); raw++) {
	if(unit->TDNNsources[raw][0] == protNo) break; /* Prototype Unit found */
      }
      if(unit->TDNNsources[raw][0] != protNo) {
	printf("Prototype Unit (%d) of (%d) not found", protNo, sourceNo);
	free(countArray);
	return(NO_TDNN);
      }

      /* writing SourceNumber and weight */
      if (sourceNo == protNo ) {	     /* Prototype Unit */
	unit->TDNNweights[raw][0] = unit->weights[i];
      }	
      else {
	unit->TDNNsources[raw][countArray[raw]] = sourceNo;
	unit->TDNNweights[raw][countArray[raw]] = unit->weights[i];
	countArray[raw]++;		     /* one more entry */
      }
    }

    killList(ProtSources);

  }

  free(countArray);
  return(OK);
}

/****************************************************************************
  char checkOrder(pLayer globalLayers, int x, int y)
  ---------------------------------------------------------------------------
  checks the order between layer x and layer y
  
  SIDE-EFFECT : the entry SuccDelay is set up, if one layer follows
  immediatly another. (needed by TDNN-networks)
  
  -> globalLayers : all layers of the net
  x, y         : numbers of layer x and y
  <- (func)    -1       : layer x before layer y
  0       : indifferent
  1       : layer x after layer y
  ILLEGAL_CYCLES : error occured (no order given)
  ****************************************************************************/
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
    globalLayers[y].SuccDelay = globalLayers[x].delay; /* Side-Effect */
    if (order == -1) {
      if (SPECIAL_H != globalLayers[y].type) return (ILLEGAL_CYCLES);
    }
    else {
      order = 1;
    }
  }
  if (haveIntersection(globalLayers[x].members, globalLayers[y].sources) ) {		
    globalLayers[x].SuccDelay = globalLayers[y].delay; /* Side-Effect */
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
  int sortNet(pLayer globalLayers, int NoOfLayers, int *order)
  ----------------------------------------------------------------------------
  calculates the order between the globalLayers an returns it in the array
  order. So the first number in the array is the first Layer to be updated
  and so on.
  -> globalLayers : Array of all layers in the net
  NoOfLayers   : Number of all layers in the net
  <- order        : the order for updating the layers
  (func)    OK : no errors occured
  MEM_ERR : not enough memory for calculations
  *****************************************************************************/ 
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

  /** build the precedence matrix of the Layer-Graph */
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

  /** put the Layers in the right order **/
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
  void writeList(pList list, FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes a List to a OutputFile in the Form :
  { <element1>, <element2>, ... }
  -> list     : pointer to the list wich is to be written
  fOutFile : appreciated output stream
  *****************************************************************************/
void writeList(pList list, FILE *fOutFile)
{
  int e;			/* (like element) Element counter for the list */

  fprintf(fOutFile, "{");
  if (0 == NoOf(list) ) fprintf(fOutFile, "0 /*NO MEMBERS */");
  for(e = 0; e < NoOf(list); e++) {
    fprintf(fOutFile, "%d", element(list, e) );
    if (e < NoOf(list) - 1) fprintf(fOutFile, ", ");
  }
  fprintf(fOutFile, "}");
}


/*****************************************************************************
  void writeLayer(pLayer layer,  FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes the LayerName and his members as an array of integers to the
  given OutputStream
  -> layer     : layer wich is to be written
  LayerName : a C-identifier for the struct
  fOutFile  : appreciated output stream 
  *****************************************************************************/
void writeLayer(pLayer layer, FILE *fOutFile)
{  
	int i;

  fprintf(fOutFile, "\n  static pUnit %s[%d] = ", 
          layer->name, NoOf(layer->members) );

  /* write the Members of the layer */
	fprintf(fOutFile, "{");
  for(i = 0; i < NoOf(layer->members); i++) {
		/* write the members as pointer to the member units */
    fprintf(fOutFile, "Units + %d", element(layer->members,i) );
    if (i < NoOf(layer->members) - 1) fprintf(fOutFile, ", ");
  }

  fprintf(fOutFile, "}; /* members */\n");
}

/*****************************************************************************
  void writeUnitNew(pUnit unit, FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes a Unit ( activation, bias, sources, Linkweights) as a struct to the
  Outfile
  -> unit   : unit wich is to be written
  fOutFile  : appreciated output stream 
  Autor     : Matthias Oberdorfer
  *****************************************************************************/
void writeUnitNew(pUnit unit, FILE *fOutFile, int count_links)
 {

  static int first_time = 1, is_Bptt = 0; /* to avoid unneeded procedure calls */
 
 	/* initialisation of is_Bptt */
 	if (first_time) {
 		is_Bptt = is_BPTT_net();
 		first_time = 0;
 	}
 
  /* write Number and Name of the unit for identification,
   * if a user is reading the code 
   */
  fprintf(fOutFile, "    { /* unit %d (%s) */\n", unit->number, unit->name);
 
  /* write Activation, Bias and number of sources */
  if (is_Bptt) {
    fprintf(fOutFile, "      {%f, 0.0}, %f, %d,\n",
 	    unit->act, unit->Bias, NoOf(unit->sources));
   }
   else {
     fprintf(fOutFile, "      0.0, %f, %d,\n",unit->Bias, NoOf(unit->sources));
   }
 
   /* write the Sources of the unit */
   fprintf(fOutFile, "       &Sources[%d] , \n", count_links);
   /* write the weights of the units */
   fprintf(fOutFile, "       &Weights[%d] , \n", count_links);
   
   fprintf(fOutFile, "      }");
 }

/*****************************************************************************
  void writeUnit(pUnit unit, FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes a Unit ( activation, bias, sources, Linkweights) as a struct to the
  Outfile
  -> unit      : unit wich is to be written
  fOutFile  : appreciated output stream 
  *****************************************************************************/
void writeUnit(pUnit unit, FILE *fOutFile)
 {
  int i;
  static int first_time = 1, is_Bptt = 0; /* to avoid unneeded procedure calls */

	/* initialisation of is_Bptt */
	if (first_time) {
		is_Bptt = is_BPTT_net();
		first_time = 0;
	}

  /* write Number and Name of the unit for identification, if a user is reading the code */
  fprintf(fOutFile, "    { /* unit %d (%s) */\n", unit->number, unit->name);

  /* write Activation, Bias and number of sources */
  if (is_Bptt) {
    fprintf(fOutFile, "      {%f, 0.0}, %f, %d,\n",
	    unit->act, unit->Bias, NoOf(unit->sources));
  }
  else {
    fprintf(fOutFile, "      0.0, %f, %d,\n",unit->Bias, NoOf(unit->sources));
  }

  /* write the Sources of the unit */
  fprintf(fOutFile, "      {");
  /* There should be at least one element in the list */
  if (NoOf(unit->sources) ==  0) fprintf(fOutFile, "NULL /* no source units */");
  for(i = 0; i < NoOf(unit->sources); i++) {
    /* write the sources as pointer to the source units */
    fprintf(fOutFile, "Units + %d", element(unit->sources,i) );
    if (i < NoOf(unit->sources) - 1) fprintf(fOutFile, ", ");
  }
  fprintf(fOutFile, "},\n");

  /* write the weights of the units */
  fprintf(fOutFile, "      {");
  /* ANSI-C needs at least one entry in an array */
  if (0 == NoOf(unit->sources) ) fprintf(fOutFile, "0.0 /* NO MEMBERS */");

  for(i = 0; i < NoOf(unit->sources); i++) {
    fprintf(fOutFile, "%f", unit->weights[i]);
    if (i < NoOf(unit->sources) - 1) fprintf(fOutFile, ", ");
  }

  fprintf(fOutFile, "}\n    }");
}


/*****************************************************************************
  void writeCpnUnit(pUnit unit, FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes a CPN Unit ( activation, bias, sources, Linkweights, dest) as a struct to the
  Outfile
  -> unit      : unit wich is to be written
  fOutFile  : appreciated output stream 
  *****************************************************************************/
void writeCpnUnit(pUnit unit, FILE *fOutFile)
{
  int i;

  /* output-units are dummy-units */
  if (unit->type == OUTPUT) {
    fflush(fOutFile);
    fprintf(fOutFile, CpnDummyUnit);
    fflush(fOutFile);
    return;
  }

  /* write Number and Name of the unit for identification, if a user is reading the code */
  fprintf(fOutFile, "    { /* unit %d (%s) */\n", unit->number, unit->name);

  /* write Activation, Bias and number of sources */
  fprintf(fOutFile, "      0.0, %f, %d,\n",unit->Bias, NoOf(unit->sources));
 
  /* write the Sources of the unit */
  fprintf(fOutFile, "      {");
  /* There should be at least one element in the list  */
  if ((NoOf(unit->sources) ==  0) || (unit->type == OUTPUT) ){
    fprintf(fOutFile, "NULL /* no source units */");
  }
  for(i = 0; i < NoOf(unit->sources); i++) {
		/* write the sources as pointer to the source units */
    fprintf(fOutFile, "Units + %d", element(unit->sources,i) );
    if (i < NoOf(unit->sources) - 1) fprintf(fOutFile, ", ");
  }
	fprintf(fOutFile, "},\n");
	
  /* write the weights of the units */
  fprintf(fOutFile, "      {");
  /* ANSI-C needs at least one entry in an array */
  if (0 == NoOf(unit->sources) ) fprintf(fOutFile, "0.0 /* NO MEMBERS */");

  for(i = 0; i < NoOf(unit->sources); i++) {
    fprintf(fOutFile, "%f", unit->weights[i]);
    if (i < NoOf(unit->sources) - 1) fprintf(fOutFile, ", ");
  }
  fprintf(fOutFile, "},\n");

  /* write the weights from hidden to the output */
  if (unit->type == INPUT) {             /* inputs haven't such weights */
    fprintf(fOutFile, "      {0.0}\n");
  }
  else {
    fprintf(fOutFile, "      {");       
    for(i = 0; i < unit->NoOfDest; i++) {
      fprintf(fOutFile, "%f", unit->dest[i]);
      if (i < unit->NoOfDest - 1) fprintf(fOutFile, ", ");
    }
    fprintf(fOutFile, "}\n");
  }
  fprintf(fOutFile, "    }");
}


/*****************************************************************************
  void writeTdnnUnit(pUnit unit, FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes a TdnnUnit ( activation, bias, sources, Linkweights) 
  as a struct to the Outfile
  -> unit      : unit wich is to be written
  Units     : pointer to the global UnitArray
  fOutFile  : appreciated output stream 
  *****************************************************************************/
void writeTdnnUnit(pUnit unit, pUnit Units, FILE *fOutFile)
{
  int feature, delay, sourceIndex, pos;

  /* write Number and Name of the unit for identification by a user */
  fprintf(fOutFile, "    { /* unit %d (%s) */\n", unit->index, unit->name);

  /* write Activation, Bias and number of sources */
  fprintf(fOutFile, "      {");
  for (delay = 1; delay < unit->NoOf(members); delay++) fprintf(fOutFile, "0.0, ");  
  fprintf(fOutFile, "0.0}, %f, %d,",unit->Bias, unit->FeatureWidth);
 
  /* write the Sources of the Unit */
  fprintf(fOutFile, "\n      {");

  /* ANSI-C needs at least one entry in an array */
  if (0 == NoOf(unit->sources) ) fprintf(fOutFile, "NULL /* NO SOURCES */");

  for(feature = 0; feature < unit->FeatureWidth; feature++) {
    sourceIndex = searchUnit(unit->TDNNsources[feature][0], Units, &pos)->index;
    fprintf(fOutFile, "Units + %d", sourceIndex);
    if (feature < unit->FeatureWidth - 1) fprintf(fOutFile, ", ");
  }
  fprintf(fOutFile, "},\n");


  /* write the weights to the source units */
  fprintf(fOutFile, "      {");

  /* ANSI-C needs at least one entry in an array */
  if (0 == NoOf(unit->sources) ) fprintf(fOutFile, " {0.0} /* NO WEIGHTS */");
  
  for(feature = 0; feature < unit->FeatureWidth; feature++) {
    fprintf(fOutFile, "\n         {");

    for(delay = 0; delay < unit->DelayLength; delay++) {
      fprintf(fOutFile, "%f", unit->TDNNweights[feature][delay]);
      if (delay < unit->DelayLength - 1) fprintf(fOutFile, ", ");
    }
    
    fprintf(fOutFile, "}");
    if (feature < unit->FeatureWidth - 1) fprintf(fOutFile, ", ");
  }
  fprintf(fOutFile, "\n      }\n    }");

  fflush(fOutFile);
}



/*****************************************************************************
  void writeAllUnitsOld(pUnit Units, int NoOfUnits, FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes all Units of the Net as an array of structs to the OutputFile
  -> pUnits    : array of all units of the net
  NoOfUnits : #units in the net
  fOutFile  : appreciated output stream
  *****************************************************************************/
void writeAllUnitsOld(pUnit Units, int NoOfUnits, FILE *fOutFile)
{
  int nr;

  fprintf(fOutFile, "  /* unit definition section (see also UnitType) */\n");

  /* Writing declaration of the Unit-Array */
  /* the 0-Element is left free because the comiler was easier 
   * to implement this way. The only exeptions are the TDNNs 
   * because here the Units are rewritten completely */
  if (is_TDNN_net()) {
    fprintf(fOutFile, 
	    "  static UnitType Units[%d] = \n  {\n", NoOfUnits
	    );
  }
  else if (is_CPN_net() ){
    fflush(fOutFile);
    fprintf(fOutFile, 
	    "  static UnitType Units[%d] = \n  {\n %s,\n",
	    NoOfUnits + 1, CpnDummyUnit
	    );
    fflush(fOutFile);
  }
  else if (is_BPTT_net() ) {
    fprintf(fOutFile, 
	    "  static UnitType Units[%d] = \n  {\n    %s,\n",
	    NoOfUnits + 1,
	    "{ {0.0, 0.0}, 0.0, 0, {NULL /* NO SOURCES */}, {0.0 /* NO MEMBERS*/} }"
	    );
  }
  else {
    fprintf(fOutFile, 
	    "  static UnitType Units[%d] = \n  {\n    %s,\n",
	    NoOfUnits + 1,
	    "{ 0.0, 0.0, 0, {NULL /* NO SOURCES */}, {0.0 /* NO MEMBERS*/} }"
	    );
  }

  /* Because of the special requirements of each network-type
   * the Unit types are slightly modified for each update-function.
   * so they need a special output-template */
  if (is_TDNN_net() ) {
    for(nr = 0; nr < NoOfUnits; nr++) {
     writeTdnnUnit(Units + nr, Units, fOutFile);
      if (nr < NoOfUnits -1) fprintf(fOutFile, ",\n");
      else                   fprintf(fOutFile, "\n");
    }
  }
  else if(is_CPN_net() ) {
    for(nr = 0; nr < NoOfUnits; nr++) {
      if (nr != 0) {
	fprintf(fOutFile, ",\n");
      }
      writeCpnUnit(Units + nr, fOutFile);
    }	
    fprintf(fOutFile, "\n");
  }
  /* Here also BPTT-units are included, because they are very similar to
     the other types */
  else {
    for(nr = 0; nr < NoOfUnits; nr++) {
      writeUnit(Units + nr, fOutFile);
      if (nr < NoOfUnits -1) fprintf(fOutFile, ",\n");
      else                   fprintf(fOutFile, "\n");
    }
  }
  fprintf(fOutFile, "\n  };\n\n");
}

/*****************************************************************************
  void writeAllUnits(pUnit Units, int NoOfUnits, FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes all Units of the Net as an array of structs to the OutputFile
  -> pUnits    : array of all units of the net
  NoOfUnits : #units in the net
  fOutFile  : appreciated output stream
  Autor     : Matthias Oberdorfer (changed original)
  *****************************************************************************/
void writeAllUnits(pUnit Units, int NoOfUnits, FILE *fOutFile)
{
  int nr;
  int count_links;  /* to give number to links */

  fprintf(fOutFile, "  /* unit definition section (see also UnitType) */\n");

  /* Writing declaration of the Unit-Array */
  /* the 0-Element is left free because the comiler was easier 
   * to implement this way. The only exeptions are the TDNNs 
   * because here the Units are rewritten completely */
  if (is_TDNN_net()) {
    fprintf(fOutFile, 
	    "  static UnitType Units[%d] = \n  {\n", NoOfUnits
	    );
  }
  else if (is_CPN_net() ){
    fflush(fOutFile);
    fprintf(fOutFile, 
	    "  static UnitType Units[%d] = \n  {\n %s,\n",
	    NoOfUnits + 1, CpnDummyUnit
	    );
    fflush(fOutFile);
  }
  else if (is_BPTT_net() ) {
    fprintf(fOutFile, 
	    "  static UnitType Units[%d] = \n  {\n    %s,\n",
	    NoOfUnits + 1,
	    "{ {0.0, 0.0}, 0.0, 0, {NULL /* NO SOURCES */}, {0.0 /* NO MEMBERS*/} }"
	    );
  }
  else {

    fprintf(fOutFile, 
	    "  static UnitType Units[%d] = \n  {\n    %s,\n",
	    NoOfUnits + 1,
	    "{ 0.0, 0.0, 0, NULL , NULL }"
 /* old  "{ 0.0, 0.0, 0, {NULL * NO SOURCES *}, {0.0 * NO MEMBERS*} }" */
	    );
  }

  /* Because of the special requirements of each network-type
   * the Unit types are slightly modified for each update-function.
   * so they need a special output-template */
  if (is_TDNN_net() ) {
    for(nr = 0; nr < NoOfUnits; nr++) {
     writeTdnnUnit(Units + nr, Units, fOutFile);
      if (nr < NoOfUnits -1) fprintf(fOutFile, ",\n");
      else                   fprintf(fOutFile, "\n");
    }
  }
  else if(is_CPN_net() ) {
    for(nr = 0; nr < NoOfUnits; nr++) {
      if (nr != 0) {
	fprintf(fOutFile, ",\n");
      }
      writeCpnUnit(Units + nr, fOutFile);
    }	
    fprintf(fOutFile, "\n");
  }
  /* Here also BPTT-units are included, because they are very similar to
     the other types */
   else {        
    count_links = 0; /* no links yet */

    for(nr = 0; nr < NoOfUnits; nr++) {
      writeUnitNew(Units + nr, fOutFile, count_links);
      count_links += NoOf((Units+nr)->sources);
   /* old  writeUnit(Units + nr, fOutFile); */
      if (nr < NoOfUnits -1) fprintf(fOutFile, ",\n");
      else                   fprintf(fOutFile, "\n");
    }
  }
  fprintf(fOutFile, "\n  };\n\n");
}

  /*****************************************************************************
  void writeForwardDeclarationAllUnits(pUnit Units, int NoOfUnits, FILE *fOutFile)
  ----------------------------------------------------------------------------
  writes for all Units of the Net an forward decl. array of structs to the OutputFile
  -> pUnits    : array of all units of the net
  NoOfUnits : #units in the net
  fOutFile  : appreciated output stream
  Autor     : Matthias Oberdorfer
  *****************************************************************************/
 void writeForwardDeclarationAllUnits(pUnit Units, int NoOfUnits, FILE *fOutFile)
 {
 
   fprintf(fOutFile, "  /* Forward Declaration for all unit types */\n");
 
   /* Writing declaration of the Unit-Array */
   /* the 0-Element is left free because the comiler was easier 
    * to implement this way. The only exeptions are the TDNNs 
    * because here the Units are rewritten completely */
   if (is_TDNN_net()) {
     fprintf(fOutFile, 
 	    "  static UnitType Units[%d];\n", NoOfUnits
 	    );
   }
   else if (is_CPN_net() ){
     fflush(fOutFile);
     fprintf(fOutFile, 
 	    "  static UnitType Units[%d];\n",
 	    NoOfUnits + 1
 	    );
     fflush(fOutFile);
   }
   else if (is_BPTT_net() ) {
     fprintf(fOutFile, 
 	    "  static UnitType Units[%d];\n",
 	    NoOfUnits + 1
 	    );
   }
   else {
     fprintf(fOutFile, 
 	    "  static UnitType Units[%d];\n",
 	    NoOfUnits + 1
 	    );
   }
 }

 /***************************************************************************
 int writeSource(pUnit unit, FILE *fOutFile)
 ----------------------------------------------------------------------------
 help-function for writeAllSources
 Autor : Matthias Oberdorfer
 ****************************************************************************/ 
 int writeSource(pUnit unit, FILE *fOutFile)
   {
     int i;
     /* There should be at least one element in the list */
     for(i = 0; i < NoOf(unit->sources); i++)
      {
       /* write the sources as pointer to the source units */
       fprintf(fOutFile, "Units + %d, ", element(unit->sources,i) );
       if(!((i+1) % 10) )
        fprintf(fOutFile, "\n");  
      }
     return i;
   }
   
 /*****************************************************************************
   void writeAllSources(pUnit Units, int NoOfUnits, FILE *fOutFile)
   ----------------------------------------------------------------------------
   writes all Units of the Net as an array of structs to the OutputFile
   -> pUnits    : array of all units of the net
   NoOfUnits : #units in the net
   fOutFile  : appreciated output stream
   Autor     : Matthias Oberdorfer
   *****************************************************************************/

 void writeAllSources(pUnit Units, int NoOfUnits, FILE *fOutFile)
 {
   int nr;
 
   fprintf(fOutFile, "  /* Sources definition section */\n");
   fprintf(fOutFile, "  static pUnit Sources[] =  {\n");
 
   for(nr = 0; nr < NoOfUnits; nr++)
   {
       if(writeSource(Units + nr, fOutFile))
         fprintf(fOutFile, "\n");
   }
   fprintf(fOutFile, "\n  };\n\n");
 }

 /***************************************************************************
 int writeWeigths(pUnit unit, FILE *fOutFile)
 ---------------------------------------------------------------------------
 help-function for writeAllWeigths
 Autor: Matthias Oberdorfer
 ****************************************************************************/

 int writeWeigths(pUnit unit, FILE *fOutFile)
   {
     int i;
   /* ANSI-C needs at least one entry in an array */
   for(i = 0; i < NoOf(unit->sources); i++)
     {
      fprintf(fOutFile, "%f, ", unit->weights[i]);
      if(!((i+1) % 10) )
        fprintf(fOutFile, "\n");  
     }
    return i;
  }
 
 /*****************************************************************************
   void writeAllWeigths(pUnit Units, int NoOfUnits, FILE *fOutFile)
   ----------------------------------------------------------------------------
   writes all Units of the Net as an array of structs to the OutputFile
   -> pUnits    : array of all units of the net
   NoOfUnits : #units in the net
   fOutFile  : appreciated output stream
   Autor     : Matthias Oberdorfer
   *****************************************************************************/

 void writeAllWeights(pUnit Units, int NoOfUnits, FILE *fOutFile)
 {
   int nr;

   fprintf(fOutFile, "  /* Weigths definition section */\n");
   fprintf(fOutFile, "  static float Weights[] =  {\n");
 
   for(nr = 0; nr < NoOfUnits; nr++)
   {
      if( writeWeigths(Units + nr, fOutFile))
         fprintf(fOutFile, "\n");
   }
   fprintf(fOutFile, "\n  };\n\n");
 }
 
/***************************************************************************
  int NameLayers(pLayer globalLayers, int NoOfLayers)
  --------------------------------------------------------------------------
  name the Layers of the Net
  -> globalLayers : all layers of the net
  NoOfLayers   : #layers of the net
  <- (func) OK      : no errors
  MEM_ERR : not enough memory
  ***************************************************************************/
int NameLayers(pLayer globalLayers, int NoOfLayers)
{
#define NAME_LENGTH 11

  int  nr;
  int  hcounter = 0, ocounter = 0, scounter =0;
  pLayer layer = NULL;

  for(nr = 0; nr < NoOfLayers; nr++) {
    layer = (globalLayers + nr);
    layer->name = (char *)malloc(NAME_LENGTH * sizeof(char) );
    if (!(layer->name)) return(MEM_ERR);

    switch( (globalLayers + nr)->type) {
    case INPUT  : sprintf(layer->name, "Input"); 
      break;
      
    case OUTPUT : sprintf(layer->name, "Output%d", ++ocounter);
      break;
      
    case HIDDEN : sprintf(layer->name, "Hidden%d", ++hcounter); 
      break;
      
    default     : sprintf(layer->name, "Special%d", ++scounter); 
    }

  }

  return(OK);
}


/*****************************************************************************
  int writeTdnnNet(pLayer globalLayers, pUnit globalUnits, 
  int NoOfLayers, int NoOfUnits,
  int *order, char *OutFile)
  ----------------------------------------------------------------------------
  writes a Net wich is discribed by globalLayers and globalUnits in the File
  named <OutFile>.
  The order for updating the Layers is discribed in order and in the single
  Layers there is a set of unit-numbers, wich will be updated when its the turn
  of the layer.
  -> globalLayers : all Layers of the Net
  -> globalUnits  : all Units of the Net
  -> NoOfLayers   : #Layers in the Net
  -> NoOfUnits    : #Units in the Net
  -> order        : in wich order the Layers have to be updated
  -> OutFile      : Name of the Output-File
  <- (func) OK        : no errors occured
  MEM_ERR   : not enough memory
  CANT_OPEN : could not open the output-file
******************************************************************************/
int writeTdnnNet(pLayer globalLayers, pUnit globalUnits, 
                 int NoOfLayers, int NoOfUnits,
                 int *order, char *OutFile, char *ProcName)
{
  pLayer layer, source;
  pUnit  unit;
  int    nr, layerNo, unitNo, sourceNo, pos, maxSource = 0, 
  maxFeature = 0, maxDelay = 0, maxTotalDelay = 0;
  FILE   *fOutFile, *fHeaderFile;
  pList  OutList, FunctionList;
  time_t   timer;
  char   HeaderFile[50];

  /* calculate time for the date in the header of the output file */
  time(&timer);
  
  /* Preparations for the Header-File */
  strcpy(HeaderFile, OutFile);
  HeaderFile[strlen(HeaderFile) - 1] = 'h';
  fHeaderFile = fopen(HeaderFile, "w");   
  if (!fHeaderFile) return(CANT_OPEN);
 
  /* prepare Output Stream */
  fOutFile = fopen(OutFile, "w");   
  if (!fOutFile) return(CANT_OPEN);
  
  /* maxSource is needed for writing the Unit-array */
  for(unit = globalUnits; unit < globalUnits + NoOfUnits; unit++) {
    maxSource  = MAX(maxSource, NoOf(unit->sources) );
    /* Calculating the Maximum Feature-Width is harmless for non-TDNNs */
    maxFeature = MAX(maxFeature, unit->FeatureWidth);
  }

  /* TDNN needs the Maximum Receptive Field */
  for(layer = globalLayers; layer < globalLayers + NoOfLayers; layer++) {
    maxDelay      = MAX(maxDelay, layer->delay);
    maxTotalDelay = MAX(maxTotalDelay, layer->TotalDelay);
  }

  /* Net-Output may consist of several (output) Layers     */
  /* so there must be an extra merge-list : OutList        */
  /* FunctionList will contain a set of all used functions */
  OutList = newList();
  if (!OutList) return (MEM_ERR);

  FunctionList = newList();
  if (!FunctionList) return (MEM_ERR);

  for(nr = 0; nr < NoOfLayers; nr ++) 
   {
    layer = globalLayers + nr;
    if( layer->type == OUTPUT) 
     {
      if (mergeList(OutList, layer->members) ) return (MEM_ERR);
     }
    else
    if (addList(FunctionList, layer->ActFunc) ) return(MEM_ERR);
   }

  /** find Names for the Layers **/
  if (NameLayers(globalLayers, NoOfLayers+1)) return(MEM_ERR);


  /* ------------------------------------------------------------
     Beginning of the output-functions
     ------------------------------------------------------------ */

  /** write Header-File **/
  fprintf(fHeaderFile, TdnnHeaderFileTemplate,
	  HeaderFile, ctime(&timer), ProcName, 
	  ProcName, NoOf(globalLayers[order[0]].members),
	  NoOf(OutList), globalLayers[order[0]].TotalDelay,
	  ProcName);
  fclose(fHeaderFile);

  /** write the Programm Header and Act-Functions  **/
  fprintf(fOutFile, ProgHeader, OutFile, ctime(&timer) );
  for(nr = 0; nr < NoOf(FunctionList); nr ++) {
    fprintf(fOutFile, "%s\n", ACT_FUNCTIONS[element(FunctionList, nr)]);
  }
  /* to avoid unneeded include-files */
  fprintf(fOutFile, "#ifndef NULL\n");
  fprintf(fOutFile, "#define NULL (void *)0\n");
  fprintf(fOutFile, "#endif\n");

  /* write Procedure-Header */
  fprintf(fOutFile, ProcHeader, ProcName);

  /* if more than one net is included in the application, 
     the types must be defined local to the procedure. So 
     it's done generally */
  fprintf(fOutFile, TdnnTypeTemplate, maxTotalDelay, 
				      maxFeature, maxFeature, maxDelay);


  /* The Delays of the TDNN are organized as a ring-buffer 
     e.g they needs a variable for the current number */
  for (layerNo = 0; layerNo < NoOfLayers; layerNo++) {
    layer = (globalLayers + layerNo);
    
    /* one variable for the current read Position */
    layer->readCounter = malloc( (strlen(layer->name) + 11) * sizeof (char) );
    if (NULL == layer->readCounter) return(MEM_ERR);
    sprintf(layer->readCounter, "%sReadCounter", layer->name);
    
    /* and one for the current write position */
    layer->writeCounter = malloc( (strlen(layer->name) + 12) * sizeof (char) );
    if (NULL == layer->writeCounter) return(MEM_ERR);
    sprintf(layer->writeCounter, "%sWriteCounter", layer->name);
    
    /* write buffer-variables and their initialisation */
    fprintf(fOutFile, "  static int %s = %d, %s = %d; \n", 
	    layer->readCounter,  layer->TotalDelay - layer->SuccDelay,
	    layer->writeCounter, layer->TotalDelay - 1);
  }
  /* a counter for the pattern is also needed */
  fprintf(fOutFile, "  static int Pattern_counter = 0;\n");
	/* a universal variable for units */
	fprintf(fOutFile, "  pUnit unit;\n");

  /* write the Units, their weights and biases */
  writeAllUnits(globalUnits, NoOfUnits, fOutFile);

  /** write  Layers e.g. Member of the layers **/
  fprintf(fOutFile, 
          "\n  /* layer definition section (names & member units) */\n");
  for (nr = 0; nr < NoOfLayers; nr++) {
    writeLayer( (globalLayers + nr), fOutFile);
  }
  /* the Output list may be treated as a layer */
  fprintf(fOutFile, "\n  static int Output[%d] = ", NoOf(OutList) );
  writeList(OutList, fOutFile);
  fprintf(fOutFile, ";\n\n");

  /** last not least the Update-Function **/
  layer = globalLayers + order[0];     /* first in order e.g. Input-Layer */
  
  fprintf(fOutFile, TdnnFirstTemplate);
  fprintf(fOutFile, TdnnInputTemplate, NoOf(layer->members) );

  for (nr = 1; nr < NoOfLayers; nr++) {
    layerNo = order[nr];	/* update Layers in the right order */
    layer   = globalLayers + layerNo; /* current Layer                    */

      unitNo = element(layer->members, 0); /* Number of the first member unit */
      unit = (globalUnits + unitNo); 
      sourceNo = element(unit->sources, 0); /* Number of the first source unit */
      unit = searchUnit(sourceNo, globalUnits, &pos);
      source = unit->layer;	/* layer of the first source unit */

      fprintf(fOutFile, TdnnTemplate, 
	      NoOf(layer->members),
	      layer->name,
	      source->readCounter, layer->delay, source->TotalDelay,
	      source->readCounter, source->readCounter, layer->delay,
	      source->readCounter,
	      source->readCounter, source->TotalDelay,
	      source->readCounter,
	      source->readCounter, layer->delay, source->TotalDelay,
	      source->readCounter, source->TotalDelay,
	      layer->writeCounter,  ACT_FUNC_NAMES[layer->ActFunc]);
  }

  fprintf(fOutFile, TdnnOutputTemplate, NoOf(OutList) );
  /* Updating the counters of the ring-buffers */
  for (layer = globalLayers; layer < globalLayers + NoOfLayers; layer++) {
    fprintf(fOutFile, "  %s = (++%s) %% %d;\n", 
	    layer->readCounter, layer->readCounter, layer->TotalDelay);
    fprintf(fOutFile, "  %s = (++%s) %% %d;\n", 
	    layer->writeCounter, layer->writeCounter, layer->TotalDelay);
  }
  
  /** the procedure should also have an end **/
  fprintf(fOutFile, TdnnExitTemplate, globalLayers[order[0]].TotalDelay);
  fprintf(fOutFile, "}\n");
  
  /** that's all folks, or in German: "Ende gut, alles gut" **/
	killList(OutList);
	killList(FunctionList);
  return(OK);
}


/*****************************************************************************
  int writeNet(pLayer globalLayers, pUnit globalUnits, 
  int NoOfLayers, int NoOfUnits,
  int *order, char *OutFile)
  ----------------------------------------------------------------------------
  writes a Net wich is discribed by globalLayers and globalUnits in the File
  named <OutFile>.
  The order for updating the Layers is discribed in order and in the single
  Layers there is a set of unit-numbers, wich will be updated when its the turn
  of the layer.
  -> globalLayers : all Layers of the Net
  -> globalUnits  : all Units of the Net
  -> NoOfLayers   : #Layers in the Net
  -> NoOfUnits    : #Units in the Net
  -> order        : in wich order the Layers have to be updated
  -> OutFile      : Name of the Output-File
  <- (func) OK    : no errors occured
  MEM_ERR   : not enough memory
  CANT_OPEN : could not open the output-file
******************************************************************************/
int writeNet(pLayer globalLayers, pUnit globalUnits, 
             int NoOfLayers, int NoOfUnits,
             int *order, char *OutFile, char *ProcName)
{
  pLayer layer;
  pUnit  unit;
  int    nr, layerNo, maxSource = 0, maxFeature = 0;
  FILE   *fOutFile, *fHeaderFile;
  pList  OutList, FunctionList;
  time_t timer;
  char   HeaderFile[50];

  /* calculate time for the date in the header of the output file */
  time(&timer);
  
  /* Preparations for the Header-File */
  strcpy(HeaderFile, OutFile);
  HeaderFile[strlen(HeaderFile) - 1] = 'h';
  fHeaderFile = fopen(HeaderFile, "w");   
  if (!fHeaderFile) return(CANT_OPEN);
 
  /* prepare Output Stream */
  fOutFile = fopen(OutFile, "w");   
  if (!fOutFile) return(CANT_OPEN);
  
  /* maxSource is needed for writing the Unit-array */
  for(unit = globalUnits; unit < globalUnits + NoOfUnits; unit++) {
    maxSource  = MAX(maxSource, NoOf(unit->sources) );
    /* Calculating the Maximum Feature-Width is harmless for non-TDNNs */
    maxFeature = MAX(maxFeature, unit->FeatureWidth);
  }

  /* Net-Output may consist of several (output) Layers     */
  /* so there must be an extra merge-list : OutList        */
  /* FunctionList will contain a set of all used functions */
  OutList = newList();
  if (!OutList) return (MEM_ERR);
  FunctionList = newList();
  if (!FunctionList) return (MEM_ERR);
  for(nr = 0; nr < NoOfLayers; nr ++) {
    layer = globalLayers + nr;
    if( layer->type == OUTPUT) {
      if (mergeList(OutList, layer->members) ) return (MEM_ERR);
    }
    if (addList(FunctionList, layer->ActFunc) ) return(MEM_ERR);
  }

  /** find Names for the Layers **/
  if (NameLayers(globalLayers, NoOfLayers)) return(MEM_ERR);


  /* ------------------------------------------------------------
     Beginning of the output-functions
     ------------------------------------------------------------ */

  /** write Header-File **/
  fprintf(fHeaderFile, HeaderFileTemplate,
	  HeaderFile, ctime(&timer), ProcName,
	  ProcName, NoOf(globalLayers[order[0]].members),
	  NoOf(OutList), ProcName);
  fclose(fHeaderFile);

  /** write the Programm Header and Act-Functions **/
  fprintf(fOutFile, ProgHeader, OutFile, ctime(&timer) );
  for(nr = 0; nr < NoOf(FunctionList); nr ++) {
    fprintf(fOutFile, "%s\n", ACT_FUNCTIONS[element(FunctionList, nr)]);
  }
  /* to avoid unneeded include-files */
  fprintf(fOutFile, "#ifndef NULL\n");
  fprintf(fOutFile, "#define NULL (void *)0\n");
  fprintf(fOutFile, "#endif\n");

  
  /* if more than one net is included in the application, 
     the types must be defined local to the procedure 
     so it's done generally */
  if (is_CPN_net() ) {
    fprintf(fOutFile, CpnTypeTemplate, maxSource, maxSource, 
			       NoOf(globalLayers[2].members));
  }
  else if (is_BPTT_net() ) {
    fprintf(fOutFile, BpttTypeTemplate, maxSource, maxSource);
  }  
  else {
    fprintf(fOutFile, TypeTemplate);
  }

  /* write the Units, their weights and biases */
   writeForwardDeclarationAllUnits(globalUnits, NoOfUnits, fOutFile);

   if( ! is_CPN_net() && !is_BPTT_net())
    {

     /* write the Units, their weights and biases */
     writeAllSources(globalUnits, NoOfUnits, fOutFile);
 
     /* write the Units, their weights and biases */
     writeAllWeights(globalUnits, NoOfUnits, fOutFile);
    } 
   if( !is_BPTT_net() )
   {
    /* write the Units, their weights and biases */
    writeAllUnits(globalUnits, NoOfUnits, fOutFile);
   }
  else
  {
    /* write the Units, their weights and biases */
    writeAllUnitsOld(globalUnits, NoOfUnits, fOutFile);
   }
 
   /* write Procedure-Header */
   fprintf(fOutFile, ProcHeader, ProcName);

  /* DLVQ needs the biggest Scalar Product */
  if(is_DLVQ_net() ) {
    fprintf(fOutFile, "  float maxSum = -1.0;\n");
    fprintf(fOutFile, "  pUnit unit;\n\n");  /* variable needed for update */
  }
  /* CounterPropagation needs a winner Unit */
  else if(is_CPN_net() ) {
   fprintf(fOutFile, "  float maxSum = -1.0e30;\n"); /* biggest Scalar Product        */
   fprintf(fOutFile, "  pUnit winner, unit;\n\n");   /* winner unit and variable unit */
  }
  else {
    fprintf(fOutFile, "  pUnit unit;\n\n");  /* variable needed for update */
  }
  
 
  /** write  Layers e.g. Member of the layers **/
  fprintf(fOutFile, 
          "\n  /* layer definition section (names & member units) */\n");
  if (is_DLVQ_net() || is_CPN_net() ) {
    /* DLVQ and CPN don't need the output layer */
    for (nr = 0; nr < NoOfLayers; nr++) {
      if (globalLayers[nr].type != OUTPUT) {
				writeLayer( (globalLayers + nr), fOutFile);
      }
    }
    fprintf(fOutFile, "\n");
  }
  else {     /* not (DLVQ or CPN) */
    for (nr = 0; nr < NoOfLayers; nr++) {
      writeLayer( (globalLayers + nr), fOutFile);
    }
    /* the Output list may be treated as a layer */
    fprintf(fOutFile, "\n  static int Output[%d] = ", NoOf(OutList) );
    writeList(OutList, fOutFile);
    fprintf(fOutFile, ";\n\n");
  }

  /* BPTT-nets may be inialised by a flag */
  if (is_BPTT_net() ) {
    fprintf(fOutFile, BpttFirstTemplate, NoOfUnits);
  }

  /** last not least the Update-Function **/
  layer = globalLayers + order[0];     /* first in order e.g. Input-Layer */
  
  if (is_BPTT_net() ) {
    fprintf(fOutFile, BpttInputTemplate, NoOf(layer->members) );
  }
  else {
    fprintf(fOutFile, InputTemplate, NoOf(layer->members) );
  }

  for (nr = 1; nr < NoOfLayers; nr++) {
    layerNo = order[nr];	/* update Layers in the right order */
    layer   = globalLayers + layerNo; /* current Layer                    */
    
    if (is_DLVQ_net() ) {
      if (layer->type == HIDDEN) {
	fprintf(fOutFile, DlvqTemplate, NoOf(layer->members),
		layer->name);
      }
      /* Output Layer not needed (see DlvqOutputTemplate) */
    }
    else if (is_CPN_net() ) {
      if (layer->type == HIDDEN) {
				fprintf(fOutFile, CpnTemplate, NoOf(layer->members),
								layer->name);
      }
      /* Output Layer not needed (see CpnOutputTemplate) */
    }
		else if (is_BPTT_net() ) {
      fprintf(fOutFile, BpttTemplate, NoOf(layer->members),
              layer->name,  ACT_FUNC_NAMES[layer->ActFunc]);
		}			
    else if (layer->ActFunc >= ActRbfNumber) {
      fprintf(fOutFile, RbfTemplate, NoOf(layer->members),
              layer->name,  ACT_FUNC_NAMES[layer->ActFunc]);
    }
    else {
      fprintf(fOutFile, NormalTemplate, NoOf(layer->members),
              layer->name,  ACT_FUNC_NAMES[layer->ActFunc]);
    }
  }

  if (is_DLVQ_net() ) {
    fprintf(fOutFile, DlvqOutputTemplate);
  }
  else if (is_CPN_net() ) {
    fprintf(fOutFile, CpnOutputTemplate, NoOf(globalLayers[2].members));
  }
  else if (is_BPTT_net() ) {
    fprintf(fOutFile, BpttOutputTemplate, NoOf(OutList) );
    fprintf(fOutFile, BpttExitTemplate);
  }
  else {
    fprintf(fOutFile, OutputTemplate, NoOf(OutList) );
  }
  
  /** the procedure should also have an end **/
  fprintf(fOutFile, "  return(OK);\n");
  fprintf(fOutFile, "}\n");
  
  /** that's all folks, or in German: "Ende gut, alles gut" **/
	killList(OutList);
	killList(FunctionList);
  return(OK);
}


/*****************************************************************************
  void freeUnits(pUnit Units)
  ----------------------------------------------------------------------------
	releases memory of the Unit-array
	-> Units : Arrray which contains all Units of the network.
             The last unit must be empty (all entries 0 or NULL)
*****************************************************************************/
void freeUnits(pUnit Units)
{
	pUnit unit = Units;

	while(unit->number) {																	 /* e.g. non-empty unit */
		if (unit->members) killList(unit->members);			 /* make sure, that there is */
		if (unit->sources) killList(unit->sources);			 /* memory to release        */
		if (unit->weights) free(unit->weights);
		if (unit->name) free(unit->name);
		if (unit->dest) free(unit->dest);
		
		if(unit->FeatureWidth) {
			int i;
			for(i = 0; i < unit->FeatureWidth; i++) {
				if(unit->TDNNsources[i]) free(unit->TDNNsources[i]);
				if(unit->TDNNweights[i]) free(unit->TDNNweights[i]);
			}
			if(unit->TDNNsources) free(unit->TDNNsources);
			if(unit->TDNNweights) free(unit->TDNNweights);
		}
		unit++;
	}

	free(Units);
}


/*****************************************************************************
  void freeLayers(pLayer Layers)
  ----------------------------------------------------------------------------
	releases memory of the Layer-array
	-> Units : Arrray which contains all Layer of the network.
             The last Layer must be empty (all entries 0 or NULL)
*****************************************************************************/
void freeLayers(pLayer Layers)
{
	pLayer layer = Layers;

	while(layer->members) {														 /* e.g. non-empty layer */
		if (layer->members) free(layer->members);				 /* make sure, that there is */
		if (layer->sources) free(layer->sources);				 /* memory to release        */
		if (layer->name) free(layer->name);
		if (layer->readCounter) free(layer->readCounter);
		if (layer->writeCounter) free(layer->writeCounter);

		layer++;
	}

	free(Layers);
}


/*****************************************************************************
  int main(int argc, char **argv)
  ----------------------------------------------------------------------------
  -> arg c   : must be 2, 3, or 4 e.g. 1 to 3 commandline-parameter
     argv[1] : the root of the filename of the network 
               (e.g. without ".net")
  <- (func)  status
*****************************************************************************/
int main(int argc, char **argv)
{
  krui_err err;				     /* error code of SNNS - krui */
  char     *netname;			     /* internal name of the SNNS-network */
  char     NetFileName[200];		     /* input file */
  char     CFileName[200];		     /* output file */
  pUnit    Units, unit = NULL;		     /* all Units and unit variable */
  pLayer   Layers, layer;		     /* all Layers and layer variable */
  int      NoOfUnits, NoOfLayers;	     /* Number of units and layers */
  int      *order;			     /* array with the order of the sorted layers */
  int      nr, pos;			     /* help variables */
  int      error;			     /* error code */
  pList    HelpList;			     /* needed for exchange */
  char     ProcName[50];		     /* function name in the output */
                                   
  /* Array with the numbers of the prototype units */
  int      *TDNN_prot; 

  /* check Params */
  if ((argc < 2) || (argc > 4)) {
    fprintf(stderr, "usage : %s <netfile> [<output-file> [<function-name>] ]\n",
						argv[0]);
    return(WRONG_PARAM);
  }
  strcpy(NetFileName, argv[1]);
  if (argc >= 3) {               /* C-File-Name mentioned in the command-line */ 
    strcpy(CFileName, argv[2]);
  }
  else {
    strcpy(CFileName, argv[1]);  /* taking Netfile Name */
    /* if the ending is not ".net" surprising Names may occur */
    strcpy(CFileName + strlen(CFileName) - 3, "c\0"); 
  }
  if (argc ==4) {               /* Function-Name mentioned in the command-line */
    strcpy(ProcName, argv[3]);
    toAlphaNum(ProcName);       /* Function Name must not contain special chars */
  } 
  else {
    /* define procedure-name for propagate-function */
    strncpy(ProcName, CFileName, strlen(CFileName) - 2);
    ProcName[strlen(CFileName) -2] = '\0';
    toAlphaNum(ProcName);       /* Function Name must not contain special chars */
  }

  /* Write a Message (what to do) on screen */
  printf(HeadingTemplate, NetFileName, CFileName, ProcName);

  printf("loading net... \n");

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

  NoOfUnits = krui_getNoOfUnits();

  Units  =  (pUnit)calloc((NoOfUnits + 1), sizeof(tUnit) );  /* because of sentinels */
	if (! Units) {
		checkErr(MEM_ERR);
		return(MEM_ERR);
	}

  Layers = (pLayer)calloc((NoOfUnits + 1), sizeof(tLayer) ); /* because of sentinels */
	if (! Layers) {
		free(Units);
		checkErr(MEM_ERR);
		return(MEM_ERR);
	}

  /* TDNN_prot needs one more Element, because the enumeration 
     of the units starts with one */
  TDNN_prot = (int *)malloc((NoOfUnits+1) * sizeof(int) );
  if (! TDNN_prot) {
		free(Units);
		free(Layers);
		checkErr(MEM_ERR);
		return(MEM_ERR);
	}

  for (nr = 0; nr <= NoOfUnits; nr++) {
    Layers[nr].number  = nr;
  }

  printf("dividing net into layers ...\n");

  /* part Net into groups */
  error =  divideNet(Units, Layers, TDNN_prot);
	if (error) {
		checkErr(error);
		FREE_ALL;
		return(error);
	}

  /* count the Non-empty Layers */
  for (nr = 0; Layers[nr].members != NULL; nr++);
  NoOfLayers = nr;
  order = (int *)malloc(NoOfLayers * sizeof(int) );


  /* count the real number of units (e.g. the prototype units in TDNN) */
  /* unused units have the number 0 the total Number can't exeed the   */
  /* Number of Units given by the SNNS-Interface-Function              */
  for(nr = 0; (Units[nr].number != 0) && (nr < NoOfUnits); nr++) {
    Units[nr].index = nr;
  }
  NoOfUnits = nr;
  
#ifdef debug
  for(nr = 0; nr < NoOfLayers; printLayer(Layers + nr++) );
#endif

  /* the TDNN_units must have a special format */
  if ( is_TDNN_net() ) {
    prepareTDNNunits(Units, TDNN_prot);
    
    /* some values are passed to the layers for TDNNs */
    for(layer = Layers; layer < Layers + NoOfLayers; layer++) {
      unit = searchUnit(element(layer->members, 0), Units, &pos);
      layer->delay      = unit->DelayLength;
      layer->TotalDelay = NoOf(unit->members);
      layer->SuccDelay  = 0;         /* only initalisation (see function checkOrder )*/
    }
  }

  /* topological sort of the layers */
  printf("sorting layers ...\n");
  error = sortNet(Layers, NoOfLayers, order);
  if (error) {
    checkErr(error);
    FREE_ALL;
    return(error);
  }

  if ( is_TDNN_net() ) {
    /* Update the entries in the member list of the layer */
    /* now the snns2c-indices are valid                   */

    for(layer = Layers; layer < Layers + NoOfLayers; layer++) {
      HelpList = newList();
      for(nr = 0; nr < NoOf(layer->members); nr++) {
	searchUnit(element(layer->members, nr), Units, &pos);
	addList(HelpList, pos);
      }
      killList(layer->members);
      layer->members = HelpList;
    }
  }
  else if( is_CPN_net() ) {
    /* copy the weights from hidden to output 
       units into hidden units */
    error = prepareCpnUnits(Units, Layers);
    if (error) {
      checkErr(error);
      FREE_ALL;
      return(error);
    }
  }

  printf("writing net ...\n");

  /* write the net as a C-source */
  if (is_TDNN_net() ) {
    error = writeTdnnNet(Layers, Units, NoOfLayers, NoOfUnits, order, CFileName, ProcName); 
    if (error) {
      checkErr(error);
      FREE_ALL;
      return(error);
    }
  }
  else {
    error = writeNet(Layers, Units, NoOfLayers, NoOfUnits, order, CFileName, ProcName);
    if (error) {
      checkErr(error);
      FREE_ALL;
      return(error);
    }
  }
  
  FREE_ALL;
  return(0);
}

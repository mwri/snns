/******************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/templates.h,v $
  SHORTNAME      : templates.h
  SNNS VERSION   : 4.2

  PURPOSE        : Templates for snns2c.c
  
  AUTHOR         : Bernward Kett
  DATE           : 31.08.94

  CHANGED BY     : Bernward Kett
  RCS VERSION    : $Revision: 1.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:35:03 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
  
  used files     : glob_typ.h, kr_ui.h      from kernel/sources
                   libkernel.a              from kernel/bin/<architecture>
                   functions.h, templates.h from actual directory
******************************************************************************/

#define HeadingTemplate ""\
"============================================================\n"\
"   snns2c by Bernward Kett (1995)\n"\
"============================================================\n"\
"   converts %s to %s \n"\
"   Function-Name %s \n"\
"============================================================\n"

#define HeaderFileTemplate ""\
"/*********************************************************\n"\
"  %s\n"\
"  --------------------------------------------------------\n"\
"  generated at %s  by snns2c ( Bernward Kett 1995 ) \n"\
"*********************************************************/\n\n"\
"extern int %s(float *in, float *out, int init);\n\n"\
"static struct {\n"\
"  int NoOfInput;    /* Number of Input Units  */\n"\
"  int NoOfOutput;   /* Number of Output Units */\n"\
"  int(* propFunc)(float *, float*, int);\n"\
"} %sREC = {%d,%d,%s};\n"

#define ProgHeader ""\
"/*********************************************************\n"\
"  %s\n"\
"  --------------------------------------------------------\n"\
"  generated at %s  by snns2c ( Bernward Kett 1995 ) \n"\
"*********************************************************/\n\n"\
"#include <math.h>\n\n"
 

#define ProcHeader ""\
"\n\nint %s(float *in, float *out, int init)\n"\
"{\n  int member, source;\n"\
"  float sum;\n"\
"  enum{OK, Error, Not_Valid};\n"\

#define TypeTemplate "\n"\
"typedef struct UT {\n"\
"          float act;         /* Activation       */\n"\
"          float Bias;        /* Bias of the Unit */\n"\
"          int   NoOfSources; /* Number of predecessor units */\n"\
"   struct UT   **sources; /* predecessor units */\n"\
"          float *weights; /* weights from predecessor units */\n"\
"        } UnitType, *pUnit;\n\n"

#define InputTemplate ""\
"  for(member = 0; member < %d; member++) {\n"\
"    Input[member]->act = in[member];\n  }\n\n"

#define OutputTemplate ""\
"  for(member = 0; member < %d; member++) {\n"\
"    out[member] = Units[Output[member]].act;\n  }\n\n"

#define NormalTemplate ""\
"  for (member = 0; member < %d; member++) {\n"\
"    unit = %s[member];\n"\
"    sum = 0.0;\n"\
"    for (source = 0; source < unit->NoOfSources; source++) {\n"\
"      sum += unit->sources[source]->act\n"\
"             * unit->weights[source];\n"\
"    }\n"\
"    unit->act = %s(sum, unit->Bias);\n"\
"  };\n\n"

#define RbfTemplate ""\
"  for (member = 0; member < %d; member++) {\n"\
"    unit = %s[member];\n"\
"    sum = 0.0;\n"\
"    for (source = 0; source < unit->NoOfSources; source++) {\n"\
"      static float diff;\n"\
"      diff = unit->sources[source]->act\n"\
"             - unit->weights[source];\n"\
"      sum += diff * diff;\n"\
"    }\n"\
"    unit->act = %s(sum, unit->Bias);\n"\
"  };\n\n"

#define DlvqTemplate ""\
"  for (member = 0; member < %d; member++) {\n"\
"    unit = %s[member];\n"\
"    sum = 0.0;\n"\
"    for (source = 0; source < unit->NoOfSources; source++) {\n"\
"      sum += unit->sources[source]->act\n"\
"             * unit->weights[source];\n"\
"    }\n"\
"    if (sum > maxSum) {\n"\
"      maxSum = sum;\n"\
"      out[0] = unit->Bias;\n"\
"    }\n"\
"  };\n\n"

/* not needed, because the output is already calculated */
#define DlvqOutputTemplate " "


/* --------------------------------------------------------------- */
/* ------ Backpropagation through time (BPTT) - Section  --------- */
/* --------------------------------------------------------------- */

#define BpttInputTemplate ""\
"  for(member = 0; member < %d; member++) {\n"\
"    Input[member]->act[old] = in[member];\n  }\n\n"

#define BpttTypeTemplate "\n"\
"typedef struct UT {\n"\
"          float act[2];      /* Activation       */\n"\
"          float Bias;        /* Bias of the Unit */\n"\
"          int   NoOfSources; /* Number of predecessor units */\n"\
"   struct UT   *sources[%d]; /* predecessor units */\n"\
"          float weights[%d]; /* weights from predecessor units */\n"\
"        } UnitType, *pUnit;\n\n"

#define BpttTemplate ""\
"  for (member = 0; member < %d; member++) {\n"\
"    unit = %s[member];\n"\
"    sum = 0.0;\n"\
"    for (source = 0; source < unit->NoOfSources; source++) {\n"\
"      sum += unit->sources[source]->act[old]\n"\
"             * unit->weights[source];\n"\
"    }\n"\
"    unit->act[new] = %s(sum, unit->Bias);\n"\
"  };\n\n"

#define BpttOutputTemplate ""\
"  for(member = 0; member < %d; member++) {\n"\
"    out[member] = Units[Output[member]].act[new];\n  }\n\n"

#define BpttExitTemplate ""\
"  old  = new; new = !new; \n"

#define BpttFirstTemplate ""\
"  static int new = 1, old = 0; /* variables for the current activations */\n"\
"  if (init) {\n"\
"    for(unit = 0; unit < Units + %d; unit++) unit->act[old] = 0.0;\n"\
"  }\n\n"


/* --------------------------------------------------------------- */
/* ------------------ CounterPropagation (CPN) - Section --------- */
/* --------------------------------------------------------------- */

#define CpnTypeTemplate "\n"\
"typedef struct UT {\n"\
"          float act;         /* Activation       */\n"\
"          float Bias;        /* Bias of the Unit */\n"\
"          int   NoOfSources; /* Number of predecessor units */\n"\
"   struct UT   *sources[%d]; /* predecessor units */\n"\
"          float weights[%d]; /* weights from predecessor units */\n"\
"          float dest[%d];    /* weights to output units */\n"\
"        } UnitType, *pUnit;\n\n"

#define CpnTemplate ""\
"  for (member = 0; member < %d; member++) {\n"\
"    unit = %s[member];\n"\
"    sum = 0.0;\n"\
"    for (source = 0; source < unit->NoOfSources; source++) {\n"\
"      sum += unit->sources[source]->act\n"\
"             * unit->weights[source];\n"\
"    }\n"\
"    if (sum > maxSum) {\n"\
"      maxSum = sum;\n"\
"      winner = unit;\n"\
"    }\n"\
"  };\n"\

#define CpnOutputTemplate ""\
"  for (member = 0; member < %d; member++) {\n"\
"    out[member] = winner->dest[member];\n"\
"  }\n\n"

#define CpnDummyUnit ""\
"    {0.0, 0.0, 0, {NULL}, {0.0}, {0.0} /* dummy unit */}"

/* --------------------------------------------------------------- */
/* ------------------- TDNN section ------------------------------ */
/* --------------------------------------------------------------- */

#define TdnnHeaderFileTemplate ""\
"/*********************************************************\n"\
"  %s\n"\
"  --------------------------------------------------------\n"\
"  generated at %s  by snns2c ( Bernward Kett 1995 ) \n"\
"*********************************************************/\n\n"\
"extern int %s(float *in, float *out, int init);\n\n"\
"static struct {\n"\
"  int NoOfInput;    /* Number of Input Units  */\n"\
"  int NoOfOutput;   /* Number of Output Units */\n"\
"  int MinDelay;     /* Timestep needed for first valid output */\n"\
"  int(* propFunc)(float *, float*, int);\n"\
"} %sREC = {%d,%d,%d,%s};\n"


#define TdnnFirstTemplate ""\
"  if (init) Pattern_counter = 0;\n"

#define TdnnExitTemplate ""\
"  if (++Pattern_counter < %d) return(Not_Valid);\n"\
"  else return(OK);\n" 

#define TdnnTypeTemplate "\n"\
"typedef struct UT {\n"\
"          float act[%d];         /* Activations of the Delays   */\n"\
"          float Bias;            /* Bias of the Unit            */\n"\
"          int   NoOfSources;     /* Number of predecessor units */\n"\
"   struct UT   *sources[%d];     /* predecessor units           */\n"\
"          float weights[%d][%d]; /* weights from pred. units and their delays */\n"\
"        } UnitType, *pUnit;\n\n"

#define TdnnInputTemplate ""\
"  for(member = 0; member < %d; member++) {\n"\
"    Input[member]->act[InputWriteCounter] = in[member];\n  }\n\n"

#define TdnnOutputTemplate ""\
"  for(member = 0; member < %d; member++) {\n"\
"    out[member] = Units[Output[member]].act[Output1ReadCounter];\n  }\n\n"

/*************************************************************************
  The Template TdnnTemplate
  ------------------------------------------------------------------------
  At first a pseudo-Code which explains the function of the Template:
  It is only necessary to compute the newest Activation of the Feature
  Units (e.g. Delay 0)

  For all Units U in a layer L:   
    For all Source Feature Units SFU of the Unit U:
      For all delays in DelayLength:
         sum = sum + (delayed Activation of the Feature Unit)
                      * ( weight to the delayed Activation)
      end
    end
    Activation (Unit U, delay 0 ) = ActivationFunc(sum, bias of Unit U) 
  end

  After updating all units the activation of the  units become older, 
  e.g. the delay of each Activation is increased. To avoid this work
  there is a pointer for each Layer which shows which activation
  is the newest one.
  ------------------------------------------------------------------------
  Explanation of the needed Parameters:

  Shortcuts : DP  : DelayPointer of a Layer
              DL  : DelayLength of a Receptive Field
              TDL : Total Delay Length of a Feature Unit

  DP      is a variable Name
  Actfunc is the Name of a Actifationfunction
  TD, TDL are integer Values

  Parameters in order of Occurence

  - Number of Layer Members
  - Layer Name
  - DP(source), DL(target), TDL(source)
  - DP(source), DP(source), DL(target)
  - DP(source)
  - DP(source), TDL(source)
  - DP(source)
  - DP(source), DL(target), TDL(source)
  - DP(source), TDL(source)
  - DP(target), ActFunc(target)

  => 19 params
***************************************************************************/
#define TdnnTemplate ""\
"  for (member = 0; member < %d; member++) { \n"\
"    unit = %s[member]; \n"\
"      sum = 0.0; \n"\
"      for(source = 0; source < unit->NoOfSources; source++) { \n"\
"        static int delay; \n"\
"        if (%s + %d < %d) { \n"\
"          for(delay = %s; delay < %s + %d; delay++) { \n"\
"            sum += unit->sources[source]->act[delay] \n"\
"                   * unit->weights[source][delay-%s]; \n"\
"          } \n"\
"        } \n"\
"        else { \n"\
"          for(delay = %s; delay < %d; delay++) { \n"\
"            sum += unit->sources[source]->act[delay] \n"\
"                   * unit->weights[source][delay-%s]; \n"\
"          } \n"\
"          for(delay = 0; delay < %s + %d - %d; delay++) { \n"\
"            sum += unit->sources[source]->act[delay] \n"\
"                   * unit->weights[source][delay- %s + %d]; \n"\
"          } \n"\
"        } /* if */  \n"\
"     }  /* for source */ \n"\
"     unit->act[%s] = %s(sum, unit->Bias); \n"\
"  };  /* for member */ \n\n"  

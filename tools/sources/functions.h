/******************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/functions.h,v $
  SHORTNAME      : functions.h
  SNNS VERSION   : 4.2

  PURPOSE        : Function-Table for snns2c.c
  
  AUTHOR         : Bernward Kett
  DATE           : 31.08.94

  CHANGED BY     : Bernward Kett
  RCS VERSION    : $Revision: 1.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:26 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
  
  used files     : glob_typ.h, kr_ui.h      from kernel/sources
                   libkernel.a              from kernel/bin/<architecture>
                   functions.h, templates.h from actual directory
******************************************************************************/
  
/* Number of the first Radial-Basis-Function is needed because */
/* they need different arguments                               */
#define ActRbfNumber 14

char *ACT_FUNC_NAMES[]= { "Act_Logistic",
                          "Act_Elliott", 
                          "Act_BSB", 
                          "Act_TanH",
                          "Act_TanHPlusBias",
                          "Act_TanH_Xdiv2", 
                          "Act_Perceptron",
                          "Act_Signum",
                          "Act_Signum0",
                          "Act_StepFunc",
                          "Act_Identity",
                          "Act_IdentityPlusBias",
                          "Act_TD_Logistic",
	      		  "Act_TD_Elliott",
                          "Act_RBF_Gaussian",
                          "Act_RBF_MultiQuadratic",
                          "Act_RBF_ThinPlateSpline",
	       		  "",
                         };


char *ACT_FUNCTIONS[] = {
"#define Act_Logistic(sum, bias)  ( (sum+bias<10000.0) ? ( 1.0/(1.0 + exp(-sum-bias) ) ) : 0.0 )",
"#define Act_Elliott(sum, bias) ( (sum+bias>0) ? (sum+bias)/(1.0+sum+bias) : (sum+bias)/(1.0-sum-bias) )",
"#define Act_BSB(sum, bias)          ( sum * bias )",
"#define Act_TanH(sum, bias)         ( tanh(sum + bias) )",
"#define Act_TanHPlusBias(sum, bias) ( tanh(sum + bias) )",
"#define Act_TanH_Xdiv2(sum, bias)   ( tanh( (sum + bias) / 2) )",
"#define Act_Perceptron(sum, bias)   ( (sum >= bias) ? 1.0 : 0.0)",
"#define Act_Signum(sum, bias)       ( (sum > 0.0) ? 1.0 : -1.0 )",
"#define Act_Signum0(sum, bias)      ( (sum == 0.0) ? 0.0 : ActSignum(sum, bias) )",
"#define Act_StepFunc(sum, bias)     ( (sum > 0.0) ? 1.0 : 0.0)",
"#define Act_Identity(sum, bias)     ( sum )",
"#define Act_IdentityPlusBias(sum, bias) (sum + bias)",
"#define Act_TD_Logistic(sum, bias)     ( 1.0/(1 + exp(-sum-bias) ) )",
"#define Act_TD_Elliott(sum, bias) ( (sum+bias>0) ? (sum+bias)/(1.0+sum+bias) : (sum+bias)/(1.0-sum-bias) )", 
"#define Act_RBF_Gaussian(sum2, bias)    (exp(-sum2 * bias) )",
"#define Act_RBF_MultiQuadratic(sum2, bias) (sqrt(sum2 + bias) )",
"#define Act_RBF_ThinPlateSpline(sum2, bias) (bias*bias*sum2*(0.5*log(sum2) + log(bias)))",
};


/* not used (yet) */
typedef float (*tActFunc)(float, float);

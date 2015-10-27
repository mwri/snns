/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/icopjmp.c,v $
  SHORTNAME      : icopjmp
  SNNS VERSION   : 4.2

  PURPOSE        : Intermediate code (IC) functions for SNNS batch interpreter:
                   assignment, arithmetic and logical operators, jumps

                   Pointers to these functions are inserted in the 
                   intermediate code table by the grammar
		   and are executed during intermediate code 
		   interpretation (run())
  NOTES          : 

  AUTHOR         : Jens Wieland
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.6 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:13 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "symtab.h"
#include "ictab.h"
#include "icopjmp.ph"
#include "glob_typ.h"    /* SNNS-Kernel: Global Datatypes and Constants */
#include "error.h"

#define D( x ) 


/*****************************************************************************
  FUNCTION : assign

  PURPOSE  : assigns res the value of arg
  RETURNS  : 
  NOTES    : argument dmy is not used, value ignored

  UPDATE   : 
******************************************************************************/
void assign(St_ptr_type res, St_ptr_type arg, St_ptr_type dmy)
{
  Data_type arg_type;
  Val_type arg_val;

  /* don't write to built-in variables: */
  if (st_get_ro(res)) 
  err_prt("Assignment to constant or built-in variable");

  /* assign value and type: */
  st_get_val_type(arg, &arg_type, &arg_val);
  if (arg_type == UNKNOWN)
  err_prt("Right side of assignment is undefined");

  st_set_val_type(res, arg_type, arg_val);
}


/*****************************************************************************
  unary arithmetical operators 

******************************************************************************/

/*****************************************************************************
  FUNCTION : sq_rt

  PURPOSE  : square root
             argument res is assigned the square root of op1
  RETURNS  : 
  NOTES    : argument op2 is not used, value ignored

  UPDATE   : 
******************************************************************************/
void sq_rt(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type;
  Val_type res_val, op1_val;

  st_get_val_type(op1, &op1_type, &op1_val);

  /* check wether operand is numeric: */
  if (! ((op1_type == INT) || (op1_type == REAL )))
  err_prt("Invalid operand type for 'sqrt'");

  /* check wether operand is positive: */
  if (((op1_type == REAL) ? op1_val.real_val : op1_val.int_val) < 0 )
  err_prt("Argument for 'sqrt' has negative value");

  /* calculate square root: */
  res_type = REAL;
  res_val.real_val = 
  sqrt((double)((op1_type == REAL) ? op1_val.real_val : op1_val.int_val));

  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : ln

  PURPOSE  : natural logarithm
             argument res is assigned the natural logarithm of op1
  RETURNS  : 
  NOTES    : argument op2 is not used, value ignored

  UPDATE   : 
******************************************************************************/
void ln(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type;
  Val_type res_val, op1_val;

  st_get_val_type(op1, &op1_type, &op1_val);

  /* check wether operand is numeric: */
  if (! ((op1_type == INT) || (op1_type == REAL )))
  err_prt("Invalid operand type for 'ln'");

  /* check wether operand is greater than 0: */
  if (((op1_type == REAL) ? op1_val.real_val : op1_val.int_val) <= 0 )
  err_prt("Argument for 'ln' is zero or negative");

  /* calculate ln: */
  res_type = REAL;
  res_val.real_val = 
  log((double)((op1_type == REAL) ? op1_val.real_val : op1_val.int_val));

  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : lg

  PURPOSE  : base-10 logarithm
             argument res is assigned the base-10 logarithm of op1
  RETURNS  : 
  NOTES    : argument op2 is not used, value ignored

  UPDATE   : 
******************************************************************************/
void lg(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type;
  Val_type res_val, op1_val;

  st_get_val_type(op1, &op1_type, &op1_val);

  /* check wether operand is numeric: */
  if (! ((op1_type == INT) || (op1_type == REAL )))
  err_prt("Invalid operand type for 'log'");

  /* check wether operand is greater than 0: */
  if (((op1_type == REAL) ? op1_val.real_val : op1_val.int_val) <= 0 )
  err_prt("Argument for 'log' is zero or negative");

  /* calculate log: */
  res_type = REAL;
  res_val.real_val = 
  log10((double)((op1_type == REAL) ? op1_val.real_val : op1_val.int_val));

  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  binary arithmetical operators 

******************************************************************************/

/*****************************************************************************
  FUNCTION : add

  PURPOSE  : integer, real and string add
             argument res is assigned the result of op1 + op2
	     strings are concatenated; if one operand is of string type and
	     the other is not, the latter is converted to string type
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void add(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* First wipe old memory entry if needed -- if this is not done, heavy
     string manipulation in the interpreter causes uncontrolable memory use! */
  st_get_val_type(res, &res_type, &res_val);
  if (res_type == STRING) {
    if (res_val.string_val != NULL) {
	free (res_val.string_val);
	res_val.string_val = NULL;
	st_set_val_type(res, res_type, res_val);
    }
  }

 /* check whether operands are numeric or strings: */
  if (!( ((op1_type == INT) || (op1_type == REAL ) || (op1_type == STRING ))
      && ((op2_type == INT) || (op2_type == REAL ) || (op2_type == STRING ))) )
  err_prt("Invalid operand type(s) for '+'");

  /* add operands according to their types: */
  if ((op1_type == INT) && (op2_type == INT ))
  {
    /* integer add: */
    res_type = INT;
    res_val.int_val = op1_val.int_val + op2_val.int_val;
    D( printf("int add: %d + %d = %d\n",
	      op1_val.int_val, op2_val.int_val, res_val.int_val); )
  }
  else
  if ( ((op1_type == INT) || (op1_type == REAL ))
      && ((op2_type == INT) || (op2_type == REAL )) ) 
  {
    /* real add, if one or both of the operands are real: */
    res_type = REAL;
    res_val.real_val = 
    ((op1_type == REAL) ? op1_val.real_val : op1_val.int_val)
    + ((op2_type == REAL) ? op2_val.real_val : op2_val.int_val);
    D( printf("real add:  %f\n",res_val.real_val); )
  }
  else
  /* at least one operand seems to be a string */
  {
    char *s1, *s2; /* s1 must have space for the strcat operation below */

    switch (op1_type){
      case INT: 
	  s1 = malloc (20); 
	  sprintf(s1, "%d", op1_val.int_val); 
	  break;
      case REAL: 
	  s1 = malloc (20);
	  sprintf(s1, "%f", op1_val.real_val); 
	  break;
      case STRING: 
	  s1 = strdup(op1_val.string_val); 
	  break;
      default: 
	  break;   /* gcc, be quiet with -Wall */
    }
    switch (op2_type){
      case INT: 
	  s2 = malloc (20);
	  sprintf(s2, "%d", op2_val.int_val); 
	  break;
      case REAL: 
	  s2 = malloc (20);
	  sprintf(s2, "%f", op2_val.real_val); 
	  break;
      case STRING: 
	  s2 = strdup(op2_val.string_val); 
	  break;
      default: 
	  break;
    }

    /* set the result type */
    res_type = STRING;

    /* set the result value */
    res_val.string_val = (char *) malloc(strlen(s1) + strlen(s2) + 1);
    if (res_val.string_val == NULL) err_prt(ERR_MEM);

    /* concatenate the strings by first copying s1 and then
       concatenating s2 behind that; copy the result to the ST: */
    strcpy(res_val.string_val, s1);
    strcat(res_val.string_val, s2);
       
    /* Remember to FREE the allocated memory... */
    free (s1); free (s2);
  
    D( printf("str add: %s\n", res_val.string_val); )
  }

  /* set type and value of result according to add operation: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : sub

  PURPOSE  : integer and real subtraction
             argument res is assigned the result of op1 - op2
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void sub(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are numeric: */
  if (!( ((op1_type == INT) || (op1_type == REAL ))
      && ((op2_type == INT) || (op2_type == REAL ))) )
  err_prt("Invalid operand type(s) for '-'");


  /* subtract operands according to their types: */
  if ((op1_type == REAL) || (op2_type == REAL ))
  {
    /* real subtract, if one or both of the operands are real: */
    res_type = REAL;
    res_val.real_val = 
    ((op1_type == REAL) ? op1_val.real_val : op1_val.int_val)
    - ((op2_type == REAL) ? op2_val.real_val : op2_val.int_val);
  }
  else
  {
    /* integer subtract: */
    res_type = INT;
    res_val.int_val = op1_val.int_val - op2_val.int_val;
  }
  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : mult

  PURPOSE  : integer and real multiply
             argument res is assigned the result of op1 * op2
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void mult(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are numeric: */
  if (!( ((op1_type == INT) || (op1_type == REAL ))
      && ((op2_type == INT) || (op2_type == REAL ))) )
  err_prt("Invalid operand type(s) for '*'");

  /* multiply operands according to their types: */
  if ((op1_type == REAL) || (op2_type == REAL ))
  {
    /* real multiply, if one or both of the operands are real: */
    res_type = REAL;
    res_val.real_val = 
    ((op1_type == REAL) ? op1_val.real_val : op1_val.int_val)
    * ((op2_type == REAL) ? op2_val.real_val : op2_val.int_val);
  }
  else
  {
    /* integer multiply: */
    res_type = INT;
    res_val.int_val = op1_val.int_val * op2_val.int_val;
  }
  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : dvde

  PURPOSE  : real division
             argument res is assigned the result of op1 / op2
	     integer type arguments are treated like reals
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void dvde(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are numeric: */
  if (!( ((op1_type == INT) || (op1_type == REAL ))
      && ((op2_type == INT) || (op2_type == REAL ))) )
  err_prt("Invalid operand type(s) for '/'");

  /* check wether the divisor is zero: */
  if ((Real_type)((op2_type == REAL) ? op2_val.real_val : op2_val.int_val)
      == 0.0)
  err_prt("Division by zero attempted within '/'");
  
  /* divide operands: */
  res_type = REAL;
  res_val.real_val = 
  ((op1_type == REAL) ? op1_val.real_val : op1_val.int_val)
  / ((op2_type == REAL) ? op2_val.real_val : op2_val.int_val);

  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : intdiv

  PURPOSE  : integer division
             argument res is assigned the result of op1 / op2
	     the value of res is truncated to its integer part
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void intdiv(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are integers: */
  if (! ((op1_type == INT) && (op2_type == INT)))
  err_prt("Invalid operand type(s) for 'div'");

  /* perform integer division: */
  if (op2_val.int_val == 0)
  err_prt("Division by zero attempted within 'div'");
  res_type = INT;
  res_val.int_val = op1_val.int_val / op2_val.int_val;

  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : mod

  PURPOSE  : integer modulo operation
             argument res is assigned the result of op1 mod op2
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void mod(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are integers: */
  if (! ((op1_type == INT) && (op2_type == INT)))
  err_prt("Invalid operand type(s) for 'mod'");

  /* perform modulo operation: */
  if (op2_val.int_val == 0)
  err_prt("Division by zero attempted within 'mod'");
  res_type = INT;
  res_val.int_val = op1_val.int_val % op2_val.int_val;

  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : bmraise

  PURPOSE  : power of
             argument res is assigned the result of op1 raised to the
	     power of op2
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void bmraise(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are numeric: */
  if (!( ((op1_type == INT) || (op1_type == REAL ))
      && ((op2_type == INT) || (op2_type == REAL ))) )
  err_prt("Invalid operand type(s) for '**' resp. '^'");

  /* check wether the operands are valid: */
  if ((((op1_type == REAL) ? op1_val.real_val : op1_val.int_val) < 0)
      && (op2_type == REAL) )
  err_prt(
   "Arg 2 of '**' resp. '^' must be of integral type if arg 1 is negative");
  
  /* raise op1 to the power of op2: */
  res_type = REAL;
  res_val.real_val = 
  pow((double)((op1_type == REAL) ? op1_val.real_val : op1_val.int_val),
  (double)((op2_type == REAL) ? op2_val.real_val : op2_val.int_val));

  /* set type and value of result: */
  st_set_val_type(res, res_type, res_val);
}



/*****************************************************************************
  logical operators

******************************************************************************/

/*****************************************************************************
  FUNCTION : not

  PURPOSE  : boolean not
  RETURNS  : 
  NOTES    : argument op2 is not used, value ignored

  UPDATE   : 
******************************************************************************/
void not(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type;
  Val_type res_val, op1_val;

  st_get_val_type(op1, &op1_type, &op1_val);

  /* check wether operand is of boolean type: */
  if (op1_type == BOOL)
  res_val.bool_val = ! op1_val.bool_val;
  else 
  err_prt("Invalid operand type for 'not'");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}

/*****************************************************************************
  FUNCTION : and

  PURPOSE  : boolean and
             TRUE and TRUE evaluates to TRUE, all else to FALSE
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void and(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are of boolean type: */
  if ((op1_type == BOOL) && (op2_type == BOOL )) 
  res_val.bool_val = op1_val.bool_val && op2_val.bool_val;
  else 
  err_prt("Invalid operand type(s) for 'and'");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : or

  PURPOSE  : boolean or
             FALSE or FALSE evaluates to FALSE, all else to TRUE  
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void or(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are of boolean type: */
  if ((op1_type == BOOL) && (op2_type == BOOL )) 
  res_val.bool_val = op1_val.bool_val || op2_val.bool_val;
  else 
  err_prt("Invalid operand type(s) for 'or'");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : less

  PURPOSE  : integer, real and boolean less
             FALSE < TRUE evaluates to TRUE, all else to FALSE
             integers and reals can be compared
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void less(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are of compatible type: */
  if ((op1_type == BOOL) && (op2_type == BOOL )) 
  res_val.bool_val = op1_val.bool_val < op2_val.bool_val;
  else
  if ( ((op1_type == REAL) || (op1_type == INT)) 
      && ((op2_type == REAL ) || (op2_type == INT )) )
  res_val.bool_val = 
  ( ((op1_type == REAL) ? op1_val.real_val : op1_val.int_val) 
   < ((op2_type == REAL) ? op2_val.real_val : op2_val.int_val) ) 
  ? TRUE : FALSE;
  /* ...in words: compare the appropriate operands */
  else 
  err_prt("Invalid operand type(s) for '<'");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : greater

  PURPOSE  : integer, real and boolean greater
             TRUE > FALSE evaluates to TRUE, all else to FALSE
             integers and reals can be compared
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void greater(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are of compatible type: */
  if ((op1_type == BOOL) && (op2_type == BOOL )) 
  res_val.bool_val = op1_val.bool_val > op2_val.bool_val;
  else
  if ( ((op1_type == REAL) || (op1_type == INT)) 
      && ((op2_type == REAL ) || (op2_type == INT )) )
  res_val.bool_val = 
  ( ((op1_type == REAL) ? op1_val.real_val : op1_val.int_val) 
   > ((op2_type == REAL) ? op2_val.real_val : op2_val.int_val) ) 
  ? TRUE : FALSE;
  /* ...in words: compare the appropriate operands */
  else 
  err_prt("Invalid operand type(s) for '>'");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : less_eq

  PURPOSE  : integer, real and boolean less than or equal
             TRUE <= FALSE evaluates to FALSE, all else to TRUE
             integers and reals can be compared
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void less_eq(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are of compatible type: */
  if ((op1_type == BOOL) && (op2_type == BOOL )) 
  res_val.bool_val = op1_val.bool_val <= op2_val.bool_val;
  else
  if ( ((op1_type == REAL) || (op1_type == INT)) 
      && ((op2_type == REAL ) || (op2_type == INT )) )
  res_val.bool_val = 
  ( ((op1_type == REAL) ? op1_val.real_val : op1_val.int_val) 
   <= ((op2_type == REAL) ? op2_val.real_val : op2_val.int_val) ) 
  ? TRUE : FALSE;
  /* ...in words: compare the appropriate operands */
  else 
  err_prt("Invalid operand type(s) for '<='");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : great_eq

  PURPOSE  : integer, real and boolean greater than or equal
             FALSE >= TRUE evaluates to FALSE, all else to TRUE
             integers and reals can be compared
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void great_eq(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are of compatible type: */
  if ((op1_type == BOOL) && (op2_type == BOOL )) 
  res_val.bool_val = op1_val.bool_val >= op2_val.bool_val;
  else
  if ( ((op1_type == REAL) || (op1_type == INT)) 
      && ((op2_type == REAL ) || (op2_type == INT )) )
  res_val.bool_val = 
  ( ((op1_type == REAL) ? op1_val.real_val : op1_val.int_val) 
   >= ((op2_type == REAL) ? op2_val.real_val : op2_val.int_val) ) 
  ? TRUE : FALSE;
  /* ...in words: compare the appropriate operands */
  else 
  err_prt("Invalid operand type(s) for '>='");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : eq

  PURPOSE  : integer, real, string and boolean compare
             compares 2 arguments of same type for equality
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void eq(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are of same type: */
  if ((op1_type == BOOL) && (op2_type == BOOL)) 
  res_val.bool_val = op1_val.bool_val == op2_val.bool_val;
  else
  if ((op1_type == REAL) && (op2_type == REAL)) 
  res_val.bool_val = (op1_val.real_val == op2_val.real_val) ? TRUE : FALSE;
  else 
  if ((op1_type == INT) && (op2_type == INT))
  res_val.bool_val = (op1_val.int_val == op2_val.int_val) ? TRUE : FALSE;
  else 
  if ((op1_type == STRING) && (op2_type == STRING))
  res_val.bool_val =
  (!strcmp(op1_val.string_val, op2_val.string_val)) ? TRUE : FALSE;
  else 
  err_prt("Invalid operand type(s) for '=='");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}


/*****************************************************************************
  FUNCTION : not_eq

  PURPOSE  : integer, real and boolean compare
             compares 2 arguments of same type for unequality
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void not_eq(St_ptr_type res, St_ptr_type op1, St_ptr_type op2)
{
  Data_type res_type, op1_type, op2_type;
  Val_type res_val, op1_val, op2_val;

  st_get_val_type(op1, &op1_type, &op1_val);
  st_get_val_type(op2, &op2_type, &op2_val);

  /* check wether operands are of same type (boolean, int or real): */
  if ((op1_type == BOOL) && (op2_type == BOOL )) 
  res_val.bool_val = op1_val.bool_val != op2_val.bool_val;
  else
  if ((op1_type == REAL) && (op2_type == REAL )) 
  res_val.bool_val = (op1_val.real_val != op2_val.real_val) ? TRUE : FALSE;
  else 
  if ((op1_type == INT) && (op2_type == INT ))
  res_val.bool_val = (op1_val.int_val != op2_val.int_val) ? TRUE : FALSE;
  else 
  err_prt("Invalid operand type(s) for '!=' resp. '<>'");

  res_type = BOOL;
  st_set_val_type(res, res_type, res_val);
}

/*****************************************************************************
  jump-functions 

******************************************************************************/

/*****************************************************************************
  FUNCTION : jmp

  PURPOSE  : unconditional jump to jmp_pos
  RETURNS  : 
  NOTES    : arg is unused, it's value ignored

  UPDATE   : 
******************************************************************************/
void jmp(Ic_ptr_type jmp_pos, St_ptr_type arg)
/* jump to jmp_pos-1 because interpreter loop adds 1 to ic_pc immediately 
   after execution of the jump command
*/
{
  if (jmp_pos == 0) /* bp_backpatch could not insert a target */
  warn_prt("Continue found outside of a block; ignored");
  else
  set_ic_pc(jmp_pos-1);
}


/*****************************************************************************
  FUNCTION : jmp_true

  PURPOSE  : jump to jmp_pos if arg is TRUE
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void jmp_true(Ic_ptr_type jmp_pos, St_ptr_type arg)
/* jmp_pos-1 because interpreter loop adds 1 to ic_pc immediately 
   after execution of the jump command
*/
{
  Data_type arg_type;
  Val_type arg_val;

  st_get_val_type(arg, &arg_type, &arg_val);
  if (arg_type != BOOL)
  err_prt("Boolean type expression expected");
  if (arg_val.bool_val == TRUE) set_ic_pc(jmp_pos-1);
}


/*****************************************************************************
  FUNCTION : jmp_false

  PURPOSE  : jump to jmp_pos if arg is FALSE
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void jmp_false(Ic_ptr_type jmp_pos, St_ptr_type arg)
/* jmp_pos-1 because interpreter loop adds 1 to ic_pc immediately 
   after execution of the jump command
*/
{
  Data_type arg_type;
  Val_type arg_val;

  st_get_val_type(arg, &arg_type, &arg_val);
  if (arg_type != BOOL)
  err_prt("Boolean type expression expected");
  if(arg_val.bool_val == FALSE) set_ic_pc(jmp_pos-1);
}

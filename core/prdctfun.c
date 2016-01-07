   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  01/06/16             */
   /*                                                     */
   /*              PREDICATE FUNCTIONS MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for several predicate          */
/*   functions including not, and, or, eq, neq, <=, >=, <,   */
/*   >, =, <>, symbolp, stringp, lexemep, numberp, integerp, */
/*   floatp, oddp, evenp, multifieldp, sequencep, and        */
/*   pointerp.                                               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Support for long long integers.                */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW and       */
/*            MAC_MCW).                                      */
/*                                                           */
/*************************************************************/

#define _PRDCTFUN_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "setup.h"

#include "envrnmnt.h"
#include "exprnpsr.h"
#include "argacces.h"
#include "multifld.h"
#include "router.h"

#include "prdctfun.h"

/**************************************************/
/* PredicateFunctionDefinitions: Defines standard */
/*   math and predicate functions.                */
/**************************************************/
void PredicateFunctionDefinitions(
  void *theEnv)
  {
#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"not", 'b', PTIEF NotFunction, "NotFunction", "11");
   EnvDefineFunction2(theEnv,"and", 'b', PTIEF AndFunction, "AndFunction", "2*");
   EnvDefineFunction2(theEnv,"or", 'b', PTIEF OrFunction, "OrFunction", "2*");

   EnvDefineFunction2(theEnv,"eq", 'b', PTIEF EqFunction, "EqFunction", "2*");
   EnvDefineFunction2(theEnv,"neq", 'b', PTIEF NeqFunction, "NeqFunction", "2*");

   EnvDefineFunction2(theEnv,"<=", 'b', PTIEF LessThanOrEqualFunction, "LessThanOrEqualFunction", "2*n");
   EnvDefineFunction2(theEnv,">=", 'b', PTIEF GreaterThanOrEqualFunction, "GreaterThanOrEqualFunction", "2*n");
   EnvDefineFunction2(theEnv,"<", 'b', PTIEF LessThanFunction, "LessThanFunction", "2*n");
   EnvDefineFunction2(theEnv,">", 'b', PTIEF GreaterThanFunction, "GreaterThanFunction", "2*n");
   EnvDefineFunction2(theEnv,"=", 'b', PTIEF NumericEqualFunction, "NumericEqualFunction", "2*n");
   EnvDefineFunction2(theEnv,"<>", 'b', PTIEF NumericNotEqualFunction, "NumericNotEqualFunction", "2*n");
   EnvDefineFunction2(theEnv,"!=", 'b', PTIEF NumericNotEqualFunction, "NumericNotEqualFunction", "2*n");

   EnvDefineFunction2(theEnv,"symbolp", 'b', PTIEF SymbolpFunction, "SymbolpFunction", "11");
   EnvDefineFunction2(theEnv,"wordp", 'b', PTIEF SymbolpFunction, "SymbolpFunction", "11");
   EnvDefineFunction2(theEnv,"stringp", 'b', PTIEF StringpFunction, "StringpFunction", "11");
   EnvDefineFunction2(theEnv,"lexemep", 'b', PTIEF LexemepFunction, "LexemepFunction", "11");
   EnvDefineFunction2(theEnv,"numberp", 'b', PTIEF NumberpFunction, "NumberpFunction", "11");
   EnvDefineFunction2(theEnv,"integerp", 'b', PTIEF IntegerpFunction, "IntegerpFunction", "11");
   EnvDefineFunction2(theEnv,"floatp", 'b', PTIEF FloatpFunction, "FloatpFunction", "11");
   EnvDefineFunction2(theEnv,"oddp", 'b', PTIEF OddpFunction, "OddpFunction", "11i");
   EnvDefineFunction2(theEnv,"evenp", 'b', PTIEF EvenpFunction, "EvenpFunction", "11i");
   EnvDefineFunction2(theEnv,"multifieldp",'b', PTIEF MultifieldpFunction, "MultifieldpFunction", "11");
   EnvDefineFunction2(theEnv,"sequencep",'b', PTIEF MultifieldpFunction, "MultifieldpFunction", "11");
   EnvDefineFunction2(theEnv,"pointerp", 'b', PTIEF PointerpFunction, "PointerpFunction", "11");
#else
#if MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

/************************************/
/* EqFunction: H/L access routine   */
/*   for the eq function.           */
/************************************/
bool EqFunction(
  void *theEnv)
  {
   DATA_OBJECT item, nextItem;
   int numArgs, i;
   struct expr *theExpression;

   /*====================================*/
   /* Determine the number of arguments. */
   /*====================================*/

   numArgs = EnvRtnArgCount(theEnv);
   if (numArgs == 0) return(false);

   /*==============================================*/
   /* Get the value of the first argument against  */
   /* which subsequent arguments will be compared. */
   /*==============================================*/

   theExpression = GetFirstArgument();
   EvaluateExpression(theEnv,theExpression,&item);

   /*=====================================*/
   /* Compare all arguments to the first. */
   /* If any are the same, return false.  */
   /*=====================================*/

   theExpression = GetNextArgument(theExpression);
   for (i = 2 ; i <= numArgs ; i++)
     {
      EvaluateExpression(theEnv,theExpression,&nextItem);

      if (GetType(nextItem) != GetType(item))
        { return(false); }

      if (GetType(nextItem) == MULTIFIELD)
        {
         if (MultifieldDOsEqual(&nextItem,&item) == false)
           { return(false); }
        }
      else if (nextItem.value != item.value)
        { return(false); }

      theExpression = GetNextArgument(theExpression);
     }

   /*=====================================*/
   /* All of the arguments were different */
   /* from the first. Return true.        */
   /*=====================================*/

   return(true);
  }

/*************************************/
/* NeqFunction: H/L access routine   */
/*   for the neq function.           */
/*************************************/
bool NeqFunction(
  void *theEnv)
  {
   DATA_OBJECT item, nextItem;
   int numArgs, i;
   struct expr *theExpression;

   /*====================================*/
   /* Determine the number of arguments. */
   /*====================================*/

   numArgs = EnvRtnArgCount(theEnv);
   if (numArgs == 0) return(false);

   /*==============================================*/
   /* Get the value of the first argument against  */
   /* which subsequent arguments will be compared. */
   /*==============================================*/

   theExpression = GetFirstArgument();
   EvaluateExpression(theEnv,theExpression,&item);

   /*=====================================*/
   /* Compare all arguments to the first. */
   /* If any are different, return false. */
   /*=====================================*/

   for (i = 2, theExpression = GetNextArgument(theExpression);
        i <= numArgs;
        i++, theExpression = GetNextArgument(theExpression))
     {
      EvaluateExpression(theEnv,theExpression,&nextItem);
      if (GetType(nextItem) != GetType(item))
        { continue; }
      else if (nextItem.type == MULTIFIELD)
        {
         if (MultifieldDOsEqual(&nextItem,&item) == true)
           { return(false); }
        }
      else if (nextItem.value == item.value)
        { return(false); }
     }

   /*=====================================*/
   /* All of the arguments were identical */
   /* to the first. Return true.          */
   /*=====================================*/

   return(true);
  }

/*****************************************/
/* StringpFunction: H/L access routine   */
/*   for the stringp function.           */
/*****************************************/
bool StringpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;

   if (EnvArgCountCheck(theEnv,"stringp",EXACTLY,1) == -1) return(false);

   EnvRtnUnknown(theEnv,1,&item);

   if (GetType(item) == STRING)
     { return(true); }
   else
     { return(false); }
  }

/*****************************************/
/* SymbolpFunction: H/L access routine   */
/*   for the symbolp function.           */
/*****************************************/
bool SymbolpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;

   if (EnvArgCountCheck(theEnv,"symbolp",EXACTLY,1) == -1) return(false);

   EnvRtnUnknown(theEnv,1,&item);

   if (GetType(item) == SYMBOL)
     { return(true); }
   else
     { return(false); }
  }

/*****************************************/
/* LexemepFunction: H/L access routine   */
/*   for the lexemep function.           */
/*****************************************/
bool LexemepFunction(
  void *theEnv)
  {
   DATA_OBJECT item;

   if (EnvArgCountCheck(theEnv,"lexemep",EXACTLY,1) == -1) return(false);

   EnvRtnUnknown(theEnv,1,&item);

   if ((GetType(item) == SYMBOL) || (GetType(item) == STRING))
     { return(true); }
   else
     { return(false); }
  }

/*****************************************/
/* NumberpFunction: H/L access routine   */
/*   for the numberp function.           */
/*****************************************/
bool NumberpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;

   if (EnvArgCountCheck(theEnv,"numberp",EXACTLY,1) == -1) return(false);

   EnvRtnUnknown(theEnv,1,&item);

   if ((GetType(item) == FLOAT) || (GetType(item) == INTEGER))
     { return(true); }
   else
     { return(false); }
  }

/****************************************/
/* FloatpFunction: H/L access routine   */
/*   for the floatp function.           */
/****************************************/
bool FloatpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;

   if (EnvArgCountCheck(theEnv,"floatp",EXACTLY,1) == -1) return(false);

   EnvRtnUnknown(theEnv,1,&item);

   if (GetType(item) == FLOAT)
     { return(true); }
   else
     { return(false); }
  }

/******************************************/
/* IntegerpFunction: H/L access routine   */
/*   for the integerp function.           */
/******************************************/
bool IntegerpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;

   if (EnvArgCountCheck(theEnv,"integerp",EXACTLY,1) == -1) return(false);

   EnvRtnUnknown(theEnv,1,&item);

   if (GetType(item) != INTEGER) return(false);

   return(true);
  }

/*********************************************/
/* MultifieldpFunction: H/L access routine   */
/*   for the multifieldp function.           */
/*********************************************/
bool MultifieldpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;

   if (EnvArgCountCheck(theEnv,"multifieldp",EXACTLY,1) == -1) return(false);

   EnvRtnUnknown(theEnv,1,&item);

   if (GetType(item) != MULTIFIELD) return(false);

   return(true);
  }

/******************************************/
/* PointerpFunction: H/L access routine   */
/*   for the pointerp function.           */
/******************************************/
bool PointerpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;

   if (EnvArgCountCheck(theEnv,"pointerp",EXACTLY,1) == -1) return(false);

   EnvRtnUnknown(theEnv,1,&item);

   if (GetType(item) != EXTERNAL_ADDRESS) return(false);

   return(true);
  }

/*************************************/
/* NotFunction: H/L access routine   */
/*   for the not function.           */
/*************************************/
bool NotFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT result;

   theArgument = GetFirstArgument();
   if (theArgument == NULL) { return(false); }

   if (EvaluateExpression(theEnv,theArgument,&result)) return(false);

   if ((result.value == EnvFalseSymbol(theEnv)) && (result.type == SYMBOL))
     { return(true); }
   
   return(false);
  }

/*************************************/
/* AndFunction: H/L access routine   */
/*   for the and function.           */
/*************************************/
bool AndFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT result;

   for (theArgument = GetFirstArgument();
        theArgument != NULL;
        theArgument = GetNextArgument(theArgument))
     {
      if (EvaluateExpression(theEnv,theArgument,&result)) return(false);
      if ((result.value == EnvFalseSymbol(theEnv)) && (result.type == SYMBOL))
        { return(false); }
     }

   return(true);
  }

/************************************/
/* OrFunction: H/L access routine   */
/*   for the or function.           */
/************************************/
bool OrFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT result;

   for (theArgument = GetFirstArgument();
        theArgument != NULL;
        theArgument = GetNextArgument(theArgument))
     {
      if (EvaluateExpression(theEnv,theArgument,&result)) return(false);

      if ((result.value != EnvFalseSymbol(theEnv)) || (result.type != SYMBOL))
        { return(true); }
     }

   return(false);
  }

/*****************************************/
/* LessThanOrEqualFunction: H/L access   */
/*   routine for the <= function.        */
/*****************************************/
bool LessThanOrEqualFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT rv1, rv2;
   int pos = 1;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   theArgument = GetFirstArgument();
   if (theArgument == NULL) { return(true); }
   if (! GetNumericArgument(theEnv,theArgument,"<=",&rv1,false,pos)) return(false);
   pos++;

   /*====================================================*/
   /* Compare each of the subsequent arguments to its    */
   /* predecessor. If any is greater, then return false. */
   /*====================================================*/

   for (theArgument = GetNextArgument(theArgument);
        theArgument != NULL;
        theArgument = GetNextArgument(theArgument), pos++)
     {
      if (! GetNumericArgument(theEnv,theArgument,"<=",&rv2,false,pos)) return(false);
      if (rv1.type == INTEGER)
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToLong(rv1.value) > ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if ((double) ValueToLong(rv1.value) > ValueToDouble(rv2.value))
              { return(false); }
           }
        }
      else
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToDouble(rv1.value) > (double) ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if (ValueToDouble(rv1.value) > ValueToDouble(rv2.value))
              { return(false); }
           }
        }

      rv1.type = rv2.type;
      rv1.value = rv2.value;
     }

   /*======================================*/
   /* Each argument was less than or equal */
   /* to it predecessor. Return true.      */
   /*======================================*/

   return(true);
  }

/********************************************/
/* GreaterThanOrEqualFunction: H/L access   */
/*   routine for the >= function.           */
/********************************************/
bool GreaterThanOrEqualFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT rv1, rv2;
   int pos = 1;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   theArgument = GetFirstArgument();
   if (theArgument == NULL) { return(true); }
   if (! GetNumericArgument(theEnv,theArgument,">=",&rv1,false,pos)) return(false);
   pos++;

   /*===================================================*/
   /* Compare each of the subsequent arguments to its   */
   /* predecessor. If any is lesser, then return false. */
   /*===================================================*/

   for (theArgument = GetNextArgument(theArgument);
        theArgument != NULL;
        theArgument = GetNextArgument(theArgument), pos++)
     {
      if (! GetNumericArgument(theEnv,theArgument,">=",&rv2,false,pos)) return(false);
      if (rv1.type == INTEGER)
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToLong(rv1.value) < ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if ((double) ValueToLong(rv1.value) < ValueToDouble(rv2.value))
              { return(false); }
           }
        }
      else
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToDouble(rv1.value) < (double) ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if (ValueToDouble(rv1.value) < ValueToDouble(rv2.value))
              { return(false); }
           }
        }

      rv1.type = rv2.type;
      rv1.value = rv2.value;
     }

   /*=========================================*/
   /* Each argument was greater than or equal */
   /* to its predecessor. Return true.        */
   /*=========================================*/

   return(true);
  }

/**********************************/
/* LessThanFunction: H/L access   */
/*   routine for the < function.  */
/**********************************/
bool LessThanFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT rv1, rv2;
   int pos = 1;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   theArgument = GetFirstArgument();
   if (theArgument == NULL) { return(true); }
   if (! GetNumericArgument(theEnv,theArgument,"<",&rv1,false,pos)) return(false);
   pos++;

   /*==========================================*/
   /* Compare each of the subsequent arguments */
   /* to its predecessor. If any is greater or */
   /* equal, then return false.                */
   /*==========================================*/

   for (theArgument = GetNextArgument(theArgument);
        theArgument != NULL;
        theArgument = GetNextArgument(theArgument), pos++)
     {
      if (! GetNumericArgument(theEnv,theArgument,"<",&rv2,false,pos)) return(false);
      if (rv1.type == INTEGER)
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToLong(rv1.value) >= ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if ((double) ValueToLong(rv1.value) >= ValueToDouble(rv2.value))
              { return(false); }
           }
        }
      else
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToDouble(rv1.value) >= (double) ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if (ValueToDouble(rv1.value) >= ValueToDouble(rv2.value))
              { return(false); }
           }
        }

      rv1.type = rv2.type;
      rv1.value = rv2.value;
     }

   /*=================================*/
   /* Each argument was less than its */
   /* predecessor. Return true.       */
   /*=================================*/

   return(true);
  }

/*************************************/
/* GreaterThanFunction: H/L access   */
/*   routine for the > function.     */
/*************************************/
bool GreaterThanFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT rv1, rv2;
   int pos = 1;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   theArgument = GetFirstArgument();
   if (theArgument == NULL) { return(true); }
   if (! GetNumericArgument(theEnv,theArgument,">",&rv1,false,pos)) return(false);
   pos++;

   /*==========================================*/
   /* Compare each of the subsequent arguments */
   /* to its predecessor. If any is lesser or  */
   /* equal, then return false.                */
   /*==========================================*/

   for (theArgument = GetNextArgument(theArgument);
        theArgument != NULL;
        theArgument = GetNextArgument(theArgument), pos++)
     {
      if (! GetNumericArgument(theEnv,theArgument,">",&rv2,false,pos)) return(false);
      if (rv1.type == INTEGER)
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToLong(rv1.value) <= ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if ((double) ValueToLong(rv1.value) <= ValueToDouble(rv2.value))
              { return(false); }
           }
        }
      else
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToDouble(rv1.value) <= (double) ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if (ValueToDouble(rv1.value) <= ValueToDouble(rv2.value))
              { return(false); }
           }
        }

      rv1.type = rv2.type;
      rv1.value = rv2.value;
     }

   /*================================*/
   /* Each argument was greater than */
   /* its predecessor. Return true.  */
   /*================================*/

   return(true);
  }

/**************************************/
/* NumericEqualFunction: H/L access   */
/*   routine for the = function.      */
/**************************************/
bool NumericEqualFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT rv1, rv2;
   int pos = 1;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   theArgument = GetFirstArgument();

   if (theArgument == NULL) { return(true); }
   if (! GetNumericArgument(theEnv,theArgument,"=",&rv1,false,pos)) return(false);
   pos++;

   /*=================================================*/
   /* Compare each of the subsequent arguments to the */
   /* first. If any is unequal, then return false.    */
   /*=================================================*/

   for (theArgument = GetNextArgument(theArgument);
        theArgument != NULL;
        theArgument = GetNextArgument(theArgument), pos++)
     {
      if (! GetNumericArgument(theEnv,theArgument,"=",&rv2,false,pos)) return(false);
      if (rv1.type == INTEGER)
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToLong(rv1.value) != ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if ((double) ValueToLong(rv1.value) != ValueToDouble(rv2.value))
              { return(false); }
           }
        }
      else
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToDouble(rv1.value) != (double) ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if (ValueToDouble(rv1.value) != ValueToDouble(rv2.value))
              { return(false); }
           }
        }
     }

   /*=================================*/
   /* All arguments were equal to the */
   /* first argument. Return true.    */
   /*=================================*/

   return(true);
  }

/*****************************************/
/* NumericNotEqualFunction: H/L access   */
/*   routine for the <> function.        */
/*****************************************/
bool NumericNotEqualFunction(
  void *theEnv)
  {
   EXPRESSION *theArgument;
   DATA_OBJECT rv1, rv2;
   int pos = 1;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   theArgument = GetFirstArgument();
   if (theArgument == NULL) { return(true); }
   if (! GetNumericArgument(theEnv,theArgument,"<>",&rv1,false,pos)) return(false);
   pos++;

   /*=================================================*/
   /* Compare each of the subsequent arguments to the */
   /* first. If any is equal, then return false.      */
   /*=================================================*/

   for (theArgument = GetNextArgument(theArgument);
        theArgument != NULL;
        theArgument = GetNextArgument(theArgument), pos++)
     {
      if (! GetNumericArgument(theEnv,theArgument,"<>",&rv2,false,pos)) return(false);
      if (rv1.type == INTEGER)
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToLong(rv1.value) == ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if ((double) ValueToLong(rv1.value) == ValueToDouble(rv2.value))
              { return(false); }
           }
        }
      else
        {
         if (rv2.type == INTEGER)
           {
            if (ValueToDouble(rv1.value) == (double) ValueToLong(rv2.value))
              { return(false); }
           }
         else
           {
            if (ValueToDouble(rv1.value) == ValueToDouble(rv2.value))
              { return(false); }
           }
        }
     }

   /*===================================*/
   /* All arguments were unequal to the */
   /* first argument. Return true.      */
   /*===================================*/

   return(true);
  }

/**************************************/
/* OddpFunction: H/L access routine   */
/*   for the oddp function.           */
/**************************************/
bool OddpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;
   long long num, halfnum;

   if (EnvArgCountCheck(theEnv,"oddp",EXACTLY,1) == -1) return(false);
   if (EnvArgTypeCheck(theEnv,"oddp",1,INTEGER,&item) == false) return(false);

   num = DOToLong(item);

   halfnum = (num / 2) * 2;
   if (num == halfnum) return(false);

   return(true);
  }

/***************************************/
/* EvenpFunction: H/L access routine   */
/*   for the evenp function.           */
/***************************************/
bool EvenpFunction(
  void *theEnv)
  {
   DATA_OBJECT item;
   long long num, halfnum;

   if (EnvArgCountCheck(theEnv,"evenp",EXACTLY,1) == -1) return(false);
   if (EnvArgTypeCheck(theEnv,"evenp",1,INTEGER,&item) == false) return(false);

   num = DOToLong(item);

   halfnum = (num / 2) * 2;
   if (num != halfnum) return(false);

   return(true);
  }




   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  07/30/16            */
   /*                                                     */
   /*          EXPRESSION OPERATIONS HEADER FILE          */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides utility routines for manipulating and   */
/*   examining expressions.                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Add NegateExpression function.                 */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*      6.40: Removed LOCALE definition.                     */
/*                                                           */
/*            Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*            Removed use of void pointers for specific      */
/*            data structures.                               */
/*                                                           */
/*************************************************************/

#ifndef _H_exprnops

#pragma once

#define _H_exprnops

//#include <stdbool.h> // TBD Needed?

#include "expressn.h"

   bool                           ConstantExpression(struct expr *);
   void                           PrintExpression(Environment *,const char *,struct expr *);
   long                           ExpressionSize(struct expr *);
   int                            CountArguments(struct expr *);
   struct expr                   *CopyExpression(Environment *,struct expr *);
   bool                           ExpressionContainsVariables(struct expr *,bool);
   bool                           IdenticalExpression(struct expr *,struct expr *);
   struct expr                   *GenConstant(Environment *,unsigned short,void *);
#if ! RUN_TIME
   bool                           CheckArgumentAgainstRestriction(Environment *,struct expr *,int);
   bool                           CheckArgumentAgainstRestriction2(Environment *,struct expr *,unsigned);
#endif
   bool                           ConstantType(int);
   struct expr                   *CombineExpressions(Environment *,struct expr *,struct expr *);
   struct expr                   *AppendExpressions(struct expr *,struct expr *);
   struct expr                   *NegateExpression(Environment *,struct expr *);

#endif /* _H_exprnops */



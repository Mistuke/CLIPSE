   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  01/06/16             */
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
/*************************************************************/

#ifndef _H_exprnops

#define _H_exprnops

#include <stdbool.h>

#ifndef _H_expressn
#include "expressn.h"
#endif

   bool                           ConstantExpression(struct expr *);
   void                           PrintExpression(void *,const char *,struct expr *);
   long                           ExpressionSize(struct expr *);
   int                            CountArguments(struct expr *);
   struct expr                   *CopyExpression(void *,struct expr *);
   bool                           ExpressionContainsVariables(struct expr *,bool);
   bool                           IdenticalExpression(struct expr *,struct expr *);
   struct expr                   *GenConstant(void *,unsigned short,void *);
#if ! RUN_TIME
   bool                           CheckArgumentAgainstRestriction(void *,struct expr *,int);
#endif
   bool                           ConstantType(int);
   struct expr                   *CombineExpressions(void *,struct expr *,struct expr *);
   struct expr                   *AppendExpressions(struct expr *,struct expr *);
   struct expr                   *NegateExpression(void *,struct expr *);

#endif /* _H_exprnops */



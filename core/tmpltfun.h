   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.50  07/30/16            */
   /*                                                     */
   /*          DEFTEMPLATE FUNCTION HEADER FILE           */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
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
/*      6.24: Added deftemplate-slot-names,                  */
/*            deftemplate-slot-default-value,                */
/*            deftemplate-slot-cardinality,                  */
/*            deftemplate-slot-allowed-values,               */
/*            deftemplate-slot-range,                        */
/*            deftemplate-slot-types,                        */
/*            deftemplate-slot-multip,                       */
/*            deftemplate-slot-singlep,                      */
/*            deftemplate-slot-existp, and                   */
/*            deftemplate-slot-defaultp functions.           */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Support for deftemplate slot facets.           */
/*                                                           */
/*            Added deftemplate-slot-facet-existp and        */
/*            deftemplate-slot-facet-value functions.        */
/*                                                           */
/*            Support for long long integers.                */
/*                                                           */
/*            Used gensprintf instead of sprintf.            */
/*                                                           */
/*            Support for modify callback function.          */
/*                                                           */
/*            Added additional argument to function          */
/*            CheckDeftemplateAndSlotArguments to specify    */
/*            the expected number of arguments.              */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*            Added code to prevent a clear command from     */
/*            being executed during fact assertions via      */
/*            Increment/DecrementClearReadyLocks API.        */
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
/*      6.50: Fact ?var:slot references in defrule actions.  */
/*                                                           */
/*************************************************************/

#ifndef _H_tmpltfun

#pragma once

#define _H_tmpltfun

#include "expressn.h"
#include "factmngr.h"
#include "scanner.h"
#include "symbol.h"
#include "tmpltdef.h"

   bool                           UpdateModifyDuplicate(Environment *,struct expr *,const char *,void *);
   struct expr                   *ModifyParse(Environment *,struct expr *,const char *);
   struct expr                   *DuplicateParse(Environment *,struct expr *,const char *);
   void                           DeftemplateFunctions(Environment *);
   void                           ModifyCommand(UDFContext *,CLIPSValue *);
   void                           DuplicateCommand(UDFContext *,CLIPSValue *);
   void                           DeftemplateSlotNamesFunction(UDFContext *,CLIPSValue *);
   void                           EnvDeftemplateSlotNames(Environment *,Deftemplate *,DATA_OBJECT *);
   void                           DeftemplateSlotDefaultValueFunction(UDFContext *,CLIPSValue *);
   bool                           EnvDeftemplateSlotDefaultValue(Environment *,Deftemplate *,const char *,DATA_OBJECT *);
   void                           DeftemplateSlotCardinalityFunction(UDFContext *,CLIPSValue *);
   void                           EnvDeftemplateSlotCardinality(Environment *,Deftemplate *,const char *,DATA_OBJECT *);
   void                           DeftemplateSlotAllowedValuesFunction(UDFContext *,CLIPSValue *);
   void                           EnvDeftemplateSlotAllowedValues(Environment *,Deftemplate *,const char *,DATA_OBJECT *);
   void                           DeftemplateSlotRangeFunction(UDFContext *,CLIPSValue *);
   void                           EnvDeftemplateSlotRange(Environment *,Deftemplate *,const char *,DATA_OBJECT *);
   void                           DeftemplateSlotTypesFunction(UDFContext *,CLIPSValue *);
   void                           EnvDeftemplateSlotTypes(Environment *,Deftemplate *,const char *,DATA_OBJECT *);
   void                           DeftemplateSlotMultiPFunction(UDFContext *,CLIPSValue *);
   bool                           EnvDeftemplateSlotMultiP(Environment *,Deftemplate *,const char *);
   void                           DeftemplateSlotSinglePFunction(UDFContext *,CLIPSValue *);
   bool                           EnvDeftemplateSlotSingleP(Environment *,Deftemplate *,const char *);
   void                           DeftemplateSlotExistPFunction(UDFContext *,CLIPSValue *);
   bool                           EnvDeftemplateSlotExistP(Environment *,Deftemplate *,const char *);
   void                           DeftemplateSlotDefaultPFunction(UDFContext *,CLIPSValue *);
   int                            EnvDeftemplateSlotDefaultP(Environment *,Deftemplate *,const char *);
   void                           DeftemplateSlotFacetExistPFunction(UDFContext *,CLIPSValue *);
   bool                           EnvDeftemplateSlotFacetExistP(Environment *,Deftemplate *,const char *,const char *);
   void                           DeftemplateSlotFacetValueFunction(UDFContext *,CLIPSValue *);
   bool                           EnvDeftemplateSlotFacetValue(Environment *,Deftemplate *,const char *,const char *,DATA_OBJECT *);
   SYMBOL_HN                     *FindTemplateForFactAddress(SYMBOL_HN *,struct lhsParseNode *);

#endif /* _H_tmpltfun */





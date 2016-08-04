   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  07/30/16            */
   /*                                                     */
   /*               DEFINSTANCES HEADER FILE              */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            GetConstructNameAndComment API change.         */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*            Changed find construct functionality so that   */
/*            imported modules are search when locating a    */
/*            named construct.                               */
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

#ifndef _H_defins

#pragma once

#define _H_defins

#if DEFINSTANCES_CONSTRUCT

struct definstances;

#include "conscomp.h"
#include "constrct.h"
#include "cstrccom.h"
#include "moduldef.h"
#include "object.h"

typedef struct definstancesModule
  {
   struct defmoduleItemHeader header;
  } DEFINSTANCES_MODULE;

typedef struct definstances
  {
   struct constructHeader header;
   unsigned busy;
   EXPRESSION *mkinstance;
  } Definstances;

#define DEFINSTANCES_DATA 22

struct definstancesData
  { 
   struct construct *DefinstancesConstruct;
   int DefinstancesModuleIndex;
#if CONSTRUCT_COMPILER && (! RUN_TIME)
   struct CodeGeneratorItem *DefinstancesCodeItem;
#endif
  };

#define DefinstancesData(theEnv) ((struct definstancesData *) GetEnvironmentData(theEnv,DEFINSTANCES_DATA))

   const char                    *EnvDefinstancesModule(Environment *,Definstances *);
   const char                    *EnvDefinstancesModuleName(Environment *,Definstances *);
   Definstances                  *EnvFindDefinstances(Environment *,const char *);
   Definstances                  *EnvFindDefinstancesInModule(Environment *,const char *);
   void                           EnvGetDefinstancesList(Environment *,DATA_OBJECT *,Defmodule *);
   const char                    *EnvGetDefinstancesName(Environment *,Definstances *);
   SYMBOL_HN                     *EnvGetDefinstancesNamePointer(Environment *,Definstances *);
   const char                    *EnvGetDefinstancesPPForm(Environment *,Definstances *);
   Definstances                  *EnvGetNextDefinstances(Environment *,Definstances *);
   bool                           EnvIsDefinstancesDeletable(Environment *,Definstances *);
   void                           EnvSetDefinstancesPPForm(Environment *,Definstances *,const char *);
   bool                           EnvUndefinstances(Environment *,Definstances *);
   void                           GetDefinstancesListFunction(UDFContext *,CLIPSValue *);
   void                           GetDefinstancesModuleCommand(UDFContext *,CLIPSValue *);
   void                           SetupDefinstances(Environment *);
   void                           UndefinstancesCommand(UDFContext *,CLIPSValue *);
#if DEBUGGING_FUNCTIONS
   void                           PPDefinstancesCommand(UDFContext *,CLIPSValue *);
   void                           ListDefinstancesCommand(UDFContext *,CLIPSValue *);
   void                           EnvListDefinstances(Environment *,const char *,Defmodule *);
#endif

#endif /* DEFINSTANCES_CONSTRUCT */

#endif /* _H_defins */





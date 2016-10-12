   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  08/25/16            */
   /*                                                     */
   /*              CLASS COMMANDS HEADER FILE             */
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
/*      6.30: Borland C (IBM_TBC) and Metrowerks CodeWarrior */
/*            (MAC_MCW, IBM_MCW) are no longer supported.    */
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
/*            ALLOW_ENVIRONMENT_GLOBALS no longer supported. */
/*                                                           */
/*            UDF redesign.                                  */
/*                                                           */
/*************************************************************/

#ifndef _H_classcom

#pragma once

#define _H_classcom

#define CONVENIENCE_MODE  0
#define CONSERVATION_MODE 1

#include "cstrccom.h"
#include "moduldef.h"
#include "object.h"
#include "symbol.h"

   const char             *EnvGetDefclassName(Environment *,Defclass *);
   const char             *EnvGetDefclassPPForm(Environment *,Defclass *);
   struct defmoduleItemHeader
                          *EnvGetDefclassModule(Environment *,Defclass *);
   const char             *EnvDefclassModule(Environment *,Defclass *);
   CLIPSLexeme            *GetDefclassNamePointer(Defclass *);
   void                    SetNextDefclass(Defclass *,Defclass *);
   void                    EnvSetDefclassPPForm(Environment *,Defclass *,char *);

   Defclass               *EnvFindDefclass(Environment *,const char *);
   Defclass               *EnvFindDefclassInModule(Environment *,const char *);
   Defclass               *LookupDefclassByMdlOrScope(Environment *,const char *);
   Defclass               *LookupDefclassInScope(Environment *,const char *);
   Defclass               *LookupDefclassAnywhere(Environment *,Defmodule *,const char *);
   bool                    DefclassInScope(Environment *,Defclass *,Defmodule *);
   Defclass               *EnvGetNextDefclass(Environment *,Defclass *);
   bool                    EnvIsDefclassDeletable(Environment *,Defclass *);

   void                    UndefclassCommand(Environment *,UDFContext *,UDFValue *);
   unsigned short          EnvSetClassDefaultsMode(Environment *,unsigned short);
   unsigned short          EnvGetClassDefaultsMode(Environment *);
   void                    GetClassDefaultsModeCommand(Environment *,UDFContext *,UDFValue *);
   void                    SetClassDefaultsModeCommand(Environment *,UDFContext *,UDFValue *);

#if DEBUGGING_FUNCTIONS
   void                    PPDefclassCommand(Environment *,UDFContext *,UDFValue *);
   void                    ListDefclassesCommand(Environment *,UDFContext *,UDFValue *);
   void                    EnvListDefclasses(Environment *,const char *,Defmodule *);
   bool                    EnvGetDefclassWatchInstances(Environment *,Defclass *);
   void                    EnvSetDefclassWatchInstances(Environment *,bool,Defclass *);
   bool                    EnvGetDefclassWatchSlots(Environment *,Defclass *);
   void                    EnvSetDefclassWatchSlots(Environment *,bool,Defclass *);
   bool                    DefclassWatchAccess(Environment *,int,bool,EXPRESSION *);
   bool                    DefclassWatchPrint(Environment *,const char *,int,EXPRESSION *);
#endif

   void                    GetDefclassListFunction(Environment *,UDFContext *,UDFValue *);
   void                    EnvGetDefclassList(Environment *,UDFValue *,Defmodule *);
   bool                    EnvUndefclass(Environment *,Defclass *);
   bool                    HasSuperclass(Defclass *,Defclass *);

   CLIPSLexeme            *CheckClassAndSlot(UDFContext *,const char *,Defclass **);

#if (! BLOAD_ONLY) && (! RUN_TIME)
   void                    SaveDefclasses(Environment *,Defmodule *,const char *);
#endif

#endif /* _H_classcom */

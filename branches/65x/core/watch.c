   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  08/25/16             */
   /*                                                     */
   /*                    WATCH MODULE                     */
   /*******************************************************/


/*************************************************************/
/* Purpose: Support functions for the watch and unwatch      */
/*   commands.                                               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian Dantes                                         */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added EnvSetWatchItem function.                */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*      6.40: Added Env prefix to GetEvaluationError and     */
/*            SetEvaluationError functions.                  */
/*                                                           */
/*            Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*            Changed return values for router functions.    */
/*                                                           */
/*            Removed use of void pointers for specific      */
/*            data structures.                               */
/*                                                           */
/*            ALLOW_ENVIRONMENT_GLOBALS no longer supported. */
/*                                                           */
/*            UDF redesign.                                  */
/*                                                           */
/*************************************************************/

#include "setup.h"

#if DEBUGGING_FUNCTIONS

#include <stdio.h>
#include <string.h>

#include "argacces.h"
#include "constant.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "memalloc.h"
#include "router.h"

#include "watch.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static WatchItemRecord        *ValidWatchItem(Environment *,const char *,bool *);
   static bool                    RecognizeWatchRouters(Environment *,const char *,void *);
   static void                    CaptureWatchPrints(Environment *,const char *,const char *,void *);
   static void                    DeallocateWatchData(Environment *);

/**********************************************/
/* InitializeWatchData: Allocates environment */
/*    data for watch items.                   */
/**********************************************/
void InitializeWatchData(
  Environment *theEnv)
  {
   AllocateEnvironmentData(theEnv,WATCH_DATA,sizeof(struct watchData),DeallocateWatchData);
  }

/************************************************/
/* DeallocateWatchData: Deallocates environment */
/*    data for watch items.                     */
/************************************************/
static void DeallocateWatchData(
  Environment *theEnv)
  {
   WatchItemRecord *tmpPtr, *nextPtr;

   tmpPtr = WatchData(theEnv)->ListOfWatchItems;
   while (tmpPtr != NULL)
     {
      nextPtr = tmpPtr->next;
      rtn_struct(theEnv,watchItemRecord,tmpPtr);
      tmpPtr = nextPtr;
     }
  }

/*************************************************************/
/* AddWatchItem: Adds an item to the list of watchable items */
/*   that can be set using the watch and unwatch commands.   */
/*   Returns false if the item is already in the list,       */
/*   otherwise returns true.                                 */
/*************************************************************/
bool AddWatchItem(
  Environment *theEnv,
  const char *name,
  int code,
  bool *flag,
  int priority,
  bool (*accessFunc)(Environment *,int,bool,struct expr *),
  bool (*printFunc)(Environment *,const char *,int,struct expr *))
  {
   WatchItemRecord *newPtr, *currentPtr, *lastPtr;

   /*================================================================*/
   /* Find the insertion point in the watchable items list to place  */
   /* the new item. If the item is already in the list return false. */
   /*================================================================*/

   for (currentPtr = WatchData(theEnv)->ListOfWatchItems, lastPtr = NULL;
        currentPtr != NULL;
        currentPtr = currentPtr->next)
     {
      if (strcmp(currentPtr->name,name) == 0) return false;
      if (priority < currentPtr->priority) lastPtr = currentPtr;
     }

   /*============================*/
   /* Create the new watch item. */
   /*============================*/

   newPtr = get_struct(theEnv,watchItemRecord);
   newPtr->name = name;
   newPtr->flag = flag;
   newPtr->code = code;
   newPtr->priority = priority;
   newPtr->accessFunc = accessFunc;
   newPtr->printFunc = printFunc;

   /*=================================================*/
   /* Insert the new item in the list of watch items. */
   /*=================================================*/

   if (lastPtr == NULL)
     {
      newPtr->next = WatchData(theEnv)->ListOfWatchItems;
      WatchData(theEnv)->ListOfWatchItems = newPtr;
     }
   else
     {
      newPtr->next = lastPtr->next;
      lastPtr->next = newPtr;
     }

   /*==================================================*/
   /* Return true to indicate the item has been added. */
   /*==================================================*/

   return true;
  }

/**************************************************/
/* Watch: C access routine for the watch command. */
/**************************************************/
bool Watch(
  Environment *theEnv,
  WatchItem item)
  {
   switch (item)
     {
      case ALL:
        return SetWatchItem(theEnv,"all",true,NULL);

      case FACTS:
        return SetWatchItem(theEnv,"facts",true,NULL);
        
      case INSTANCES:
        return SetWatchItem(theEnv,"instances",true,NULL);
        
      case SLOTS:
        return SetWatchItem(theEnv,"slots",true,NULL);
        
      case RULES:
        return SetWatchItem(theEnv,"rules",true,NULL);
        
      case ACTIVATIONS:
        return SetWatchItem(theEnv,"activations",true,NULL);
        
      case MESSAGES:
        return SetWatchItem(theEnv,"messages",true,NULL);
        
      case MESSAGE_HANDLERS:
        return SetWatchItem(theEnv,"message-handlers",true,NULL);
        
      case GENERIC_FUNCTIONS:
        return SetWatchItem(theEnv,"generic-functions",true,NULL);
        
      case METHODS:
        return SetWatchItem(theEnv,"methods",true,NULL);
        
      case DEFFUNCTIONS:
        return SetWatchItem(theEnv,"deffunctions",true,NULL);
        
      case COMPILATIONS:
        return SetWatchItem(theEnv,"compilations",true,NULL);
        
      case STATISTICS:
        return SetWatchItem(theEnv,"statistics",true,NULL);
        
      case GLOBALS:
        return SetWatchItem(theEnv,"globals",true,NULL);
        
      case FOCUS:
        return SetWatchItem(theEnv,"focus",true,NULL);
     }
     
   return false;
  }

/****************************************************/
/* Unwatch: C access routine for the watch command. */
/****************************************************/
bool Unwatch(
  Environment *theEnv,
  WatchItem item)
  {
   switch (item)
     {
      case ALL:
        return SetWatchItem(theEnv,"all",false,NULL);

      case FACTS:
        return SetWatchItem(theEnv,"facts",false,NULL);
        
      case INSTANCES:
        return SetWatchItem(theEnv,"instances",false,NULL);
        
      case SLOTS:
        return SetWatchItem(theEnv,"slots",false,NULL);
        
      case RULES:
        return SetWatchItem(theEnv,"rules",false,NULL);
        
      case ACTIVATIONS:
        return SetWatchItem(theEnv,"activations",false,NULL);
        
      case MESSAGES:
        return SetWatchItem(theEnv,"messages",false,NULL);
        
      case MESSAGE_HANDLERS:
        return SetWatchItem(theEnv,"message-handlers",false,NULL);
        
      case GENERIC_FUNCTIONS:
        return SetWatchItem(theEnv,"generic-functions",false,NULL);
        
      case METHODS:
        return SetWatchItem(theEnv,"methods",false,NULL);
        
      case DEFFUNCTIONS:
        return SetWatchItem(theEnv,"deffunctions",false,NULL);
        
      case COMPILATIONS:
        return SetWatchItem(theEnv,"compilations",false,NULL);
        
      case STATISTICS:
        return SetWatchItem(theEnv,"statistics",false,NULL);
        
      case GLOBALS:
        return SetWatchItem(theEnv,"globals",false,NULL);
        
      case FOCUS:
        return SetWatchItem(theEnv,"focus",false,NULL);
     }
     
   return false;
  }

/******************************************/
/* GetWatchState: Returns the watch state */
/*   for the specified watch item.        */
/******************************************/
bool GetWatchState(
  Environment *theEnv,
  WatchItem item)
  {
   switch (item)
     {
      case ALL:
        return false;

      case FACTS:
        return (GetWatchItem(theEnv,"facts") == 1);
        
      case INSTANCES:
        return (GetWatchItem(theEnv,"instances") == 1);
        
      case SLOTS:
        return (GetWatchItem(theEnv,"slots") == 1);
        
      case RULES:
        return (GetWatchItem(theEnv,"rules") == 1);
        
      case ACTIVATIONS:
        return (GetWatchItem(theEnv,"activations") == 1);
        
      case MESSAGES:
        return (GetWatchItem(theEnv,"messages") == 1);
        
      case MESSAGE_HANDLERS:
        return (GetWatchItem(theEnv,"message-handlers") == 1);
        
      case GENERIC_FUNCTIONS:
        return (GetWatchItem(theEnv,"generic-functions") == 1);
        
      case METHODS:
        return (GetWatchItem(theEnv,"methods") == 1);
        
      case DEFFUNCTIONS:
        return (GetWatchItem(theEnv,"deffunctions") == 1);
        
      case COMPILATIONS:
        return (GetWatchItem(theEnv,"compilations") == 1);
        
      case STATISTICS:
        return (GetWatchItem(theEnv,"statistics") == 1);
        
      case GLOBALS:
        return (GetWatchItem(theEnv,"globals") == 1);
        
      case FOCUS:
        return (GetWatchItem(theEnv,"focus") == 1);
     }
     
   return false;
  }

/******************************************/
/* SetWatchState: Returns the watch state */
/*   for the specified watch item.        */
/******************************************/
void SetWatchState(
  Environment *theEnv,
  WatchItem item,
  bool newState)
  {
   switch (item)
     {
      case ALL:
        SetWatchItem(theEnv,"all",newState,NULL);
        return;

      case FACTS:
        SetWatchItem(theEnv,"facts",newState,NULL);
        return;
        
      case INSTANCES:
        SetWatchItem(theEnv,"instances",newState,NULL);
        return;
        
      case SLOTS:
        SetWatchItem(theEnv,"slots",newState,NULL);
        return;
        
      case RULES:
        SetWatchItem(theEnv,"rules",newState,NULL);
        return;
        
      case ACTIVATIONS:
        SetWatchItem(theEnv,"activations",newState,NULL);
        return;
        
      case MESSAGES:
        SetWatchItem(theEnv,"messages",newState,NULL);
        return;
        
      case MESSAGE_HANDLERS:
        SetWatchItem(theEnv,"message-handlers",newState,NULL);
        return;
        
      case GENERIC_FUNCTIONS:
        SetWatchItem(theEnv,"generic-functions",newState,NULL);
        return;
        
      case METHODS:
        SetWatchItem(theEnv,"methods",newState,NULL);
        return;
        
      case DEFFUNCTIONS:
        SetWatchItem(theEnv,"deffunctions",newState,NULL);
        return;
        
      case COMPILATIONS:
        SetWatchItem(theEnv,"compilations",newState,NULL);
        return;
        
      case STATISTICS:
        SetWatchItem(theEnv,"statistics",newState,NULL);
        return;
        
      case GLOBALS:
        SetWatchItem(theEnv,"globals",newState,NULL);
        return;
        
      case FOCUS:
        SetWatchItem(theEnv,"focus",newState,NULL);
        return;
     }
  }

/********************************************************/
/* WatchString: C access routine for the watch command. */
/********************************************************/
bool WatchString(
  Environment *theEnv,
  const char *itemName)
  {
   return SetWatchItem(theEnv,itemName,true,NULL);
  }

/************************************************************/
/* UnwatchString: C access routine for the unwatch command. */
/************************************************************/
bool UnwatchString(
  Environment *theEnv,
  const char *itemName)
  {
   return SetWatchItem(theEnv,itemName,false,NULL);
  }

/********************************************************************/
/* SetWatchItem: Sets the state of a specified watch item to either */
/*   on or off. Returns true if the item was set, otherwise false.  */
/********************************************************************/
bool SetWatchItem(
  Environment *theEnv,
  const char *itemName,
  bool newState,
  struct expr *argExprs)
  {
   WatchItemRecord *wPtr;

   /*===================================================*/
   /* If the name of the watch item to set is all, then */
   /* all watch items are set to the new state and true */
   /* is returned.                                      */
   /*===================================================*/

   if (strcmp(itemName,"all") == 0)
     {
      for (wPtr = WatchData(theEnv)->ListOfWatchItems; wPtr != NULL; wPtr = wPtr->next)
        {
         /*==============================================*/
         /* If no specific arguments are specified, then */
         /* set the global flag for the watch item.      */
         /*==============================================*/

         if (argExprs == NULL) *(wPtr->flag) = newState;

         /*=======================================*/
         /* Set flags for individual watch items. */
         /*=======================================*/

         if ((wPtr->accessFunc == NULL) ? false :
             ((*wPtr->accessFunc)(theEnv,wPtr->code,newState,argExprs) == false))
           {
            SetEvaluationError(theEnv,true);
            return false;
           }
        }
      return true;
     }

   /*=================================================*/
   /* Search for the watch item to be set in the list */
   /* of watch items. If found, set the watch item to */
   /* its new state and return true.                  */
   /*=================================================*/

   for (wPtr = WatchData(theEnv)->ListOfWatchItems; wPtr != NULL; wPtr = wPtr->next)
     {
      if (strcmp(itemName,wPtr->name) == 0)
        {
         /*==============================================*/
         /* If no specific arguments are specified, then */
         /* set the global flag for the watch item.      */
         /*==============================================*/

         if (argExprs == NULL) *(wPtr->flag) = newState;

         /*=======================================*/
         /* Set flags for individual watch items. */
         /*=======================================*/

         if ((wPtr->accessFunc == NULL) ? false :
             ((*wPtr->accessFunc)(theEnv,wPtr->code,newState,argExprs) == false))
           {
            SetEvaluationError(theEnv,true);
            return false;
           }

         return true;
        }
     }

   /*=================================================*/
   /* If the specified item was not found in the list */
   /* of watchable items then return false.           */
   /*=================================================*/

   return false;
  }

/****************************************************************/
/* GetWatchItem: Gets the current state of the specified watch  */
/*   item. Returns the state of the watch item (0 for off and 1 */
/*   for on) if the watch item is found in the list of watch    */
/*   items, otherwise -1 is returned.                           */
/****************************************************************/
int GetWatchItem(
  Environment *theEnv,
  const char *itemName)
  {
   WatchItemRecord *wPtr;

   for (wPtr = WatchData(theEnv)->ListOfWatchItems; wPtr != NULL; wPtr = wPtr->next)
     {
      if (strcmp(itemName,wPtr->name) == 0)
        {
         if (*(wPtr->flag))
           { return 1; }
         else
           { return 0; }
        }
     }

   return -1;
  }

/***************************************************************/
/* ValidWatchItem: Returns true if the specified name is found */
/*   in the list of watch items, otherwise returns false.      */
/***************************************************************/
static WatchItemRecord *ValidWatchItem(
  Environment *theEnv,
  const char *itemName,
  bool *recognized)
  {
   WatchItemRecord *wPtr;

   *recognized = true;
   if (strcmp(itemName,"all") == 0)
     return NULL;

   for (wPtr = WatchData(theEnv)->ListOfWatchItems; wPtr != NULL; wPtr = wPtr->next)
     { if (strcmp(itemName,wPtr->name) == 0) return(wPtr); }

   *recognized = false;
   return NULL;
  }

/*************************************************************/
/* GetNthWatchName: Returns the name associated with the nth */
/*   item in the list of watchable items. If the nth item    */
/*   does not exist, then NULL is returned.                  */
/*************************************************************/
const char *GetNthWatchName(
  Environment *theEnv,
  int whichItem)
  {
   int i;
   WatchItemRecord *wPtr;

   for (wPtr = WatchData(theEnv)->ListOfWatchItems, i = 1;
        wPtr != NULL;
        wPtr = wPtr->next, i++)
     { if (i == whichItem) return(wPtr->name); }

   return NULL;
  }

/***************************************************************/
/* GetNthWatchValue: Returns the current state associated with */
/*   the nth item in the list of watchable items. If the nth   */
/*   item does not exist, then -1 is returned.                 */
/***************************************************************/
int GetNthWatchValue(
  Environment *theEnv,
  int whichItem)
  {
   int i;
   WatchItemRecord *wPtr;

   for (wPtr = WatchData(theEnv)->ListOfWatchItems, i = 1;
        wPtr != NULL;
        wPtr = wPtr->next, i++)
     { if (i == whichItem) return((int) *(wPtr->flag)); }

   return(-1);
  }

/**************************************/
/* WatchCommand: H/L access routine   */
/*   for the watch command.           */
/**************************************/
void WatchCommand(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   UDFValue theValue;
   const char *argument;
   bool recognized;
   WatchItemRecord *wPtr;

   /*========================================*/
   /* Determine which item is to be watched. */
   /*========================================*/

   if (! UDFFirstArgument(context,SYMBOL_BIT,&theValue)) return;

   argument = theValue.lexemeValue->contents;
   wPtr = ValidWatchItem(theEnv,argument,&recognized);
   if (recognized == false)
     {
      SetEvaluationError(theEnv,true);
      UDFInvalidArgumentMessage(context,"watchable symbol");
      return;
     }

   /*=================================================*/
   /* Check to make sure extra arguments are allowed. */
   /*=================================================*/

   if (GetNextArgument(GetFirstArgument()) != NULL)
     {
      if ((wPtr == NULL) ? true : (wPtr->accessFunc == NULL))
        {
         SetEvaluationError(theEnv,true);
         ExpectedCountError(theEnv,"watch",EXACTLY,1);
         return;
        }
     }

   /*=====================*/
   /* Set the watch item. */
   /*=====================*/

   SetWatchItem(theEnv,argument,true,GetNextArgument(GetFirstArgument()));
  }

/****************************************/
/* UnwatchCommand: H/L access routine   */
/*   for the unwatch command.           */
/****************************************/
void UnwatchCommand(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   UDFValue theValue;
   const char *argument;
   bool recognized;
   WatchItemRecord *wPtr;

   /*==========================================*/
   /* Determine which item is to be unwatched. */
   /*==========================================*/

   if (! UDFFirstArgument(context,SYMBOL_BIT,&theValue)) return;

   argument = theValue.lexemeValue->contents;
   wPtr = ValidWatchItem(theEnv,argument,&recognized);
   if (recognized == false)
     {
      SetEvaluationError(theEnv,true);
      UDFInvalidArgumentMessage(context,"watchable symbol");
      return;
     }

   /*=================================================*/
   /* Check to make sure extra arguments are allowed. */
   /*=================================================*/

   if (GetNextArgument(GetFirstArgument()) != NULL)
     {
      if ((wPtr == NULL) ? true : (wPtr->accessFunc == NULL))
        {
         SetEvaluationError(theEnv,true);
         ExpectedCountError(theEnv,"unwatch",EXACTLY,1);
         return;
        }
     }

   /*=====================*/
   /* Set the watch item. */
   /*=====================*/

   SetWatchItem(theEnv,argument,false,GetNextArgument(GetFirstArgument()));
  }

/************************************************/
/* ListWatchItemsCommand: H/L access routines   */
/*   for the list-watch-items command.          */
/************************************************/
void ListWatchItemsCommand(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   WatchItemRecord *wPtr;
   UDFValue theValue;
   bool recognized;

   /*=======================*/
   /* List the watch items. */
   /*=======================*/

   if (GetFirstArgument() == NULL)
     {
      for (wPtr = WatchData(theEnv)->ListOfWatchItems; wPtr != NULL; wPtr = wPtr->next)
        {
         PrintString(theEnv,WDISPLAY,wPtr->name);
         if (*(wPtr->flag)) PrintString(theEnv,WDISPLAY," = on\n");
         else PrintString(theEnv,WDISPLAY," = off\n");
        }
      return;
     }

   /*=======================================*/
   /* Determine which item is to be listed. */
   /*=======================================*/

   if (! UDFFirstArgument(context,SYMBOL_BIT,&theValue)) return;
   wPtr = ValidWatchItem(theEnv,theValue.lexemeValue->contents,&recognized);
   if ((recognized == false) || (wPtr == NULL))
     {
      SetEvaluationError(theEnv,true);
      ExpectedTypeError1(theEnv,"list-watch-items",1,"watchable symbol");
      return;
     }

   /*=================================================*/
   /* Check to make sure extra arguments are allowed. */
   /*=================================================*/

   if ((wPtr->printFunc == NULL) &&
       (GetNextArgument(GetFirstArgument()) != NULL))
     {
      SetEvaluationError(theEnv,true);
      ExpectedCountError(theEnv,"list-watch-items",EXACTLY,1);
      return;
     }

   /*====================================*/
   /* List the status of the watch item. */
   /*====================================*/

   PrintString(theEnv,WDISPLAY,wPtr->name);
   if (*(wPtr->flag)) PrintString(theEnv,WDISPLAY," = on\n");
   else PrintString(theEnv,WDISPLAY," = off\n");

   /*============================================*/
   /* List the status of individual watch items. */
   /*============================================*/

   if (wPtr->printFunc != NULL)
     {
      if ((*wPtr->printFunc)(theEnv,WDISPLAY,wPtr->code,
                             GetNextArgument(GetFirstArgument())) == false)
        { SetEvaluationError(theEnv,true); }
     }
  }

/*******************************************/
/* GetWatchItemCommand: H/L access routine */
/*   for the get-watch-item command.       */
/*******************************************/
void GetWatchItemCommand(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   UDFValue theValue;
   const char *argument;
   bool recognized;

   /*========================================*/
   /* Determine which item is to be watched. */
   /*========================================*/

   if (! UDFFirstArgument(context,SYMBOL_BIT,&theValue))
     { return; }

   argument = theValue.lexemeValue->contents;
   ValidWatchItem(theEnv,argument,&recognized);
   if (recognized == false)
     {
      SetEvaluationError(theEnv,true);
      ExpectedTypeError1(theEnv,"get-watch-item",1,"watchable symbol");
      returnValue->lexemeValue = FalseSymbol(theEnv);
      return;
     }

   /*===========================*/
   /* Get the watch item value. */
   /*===========================*/

   if (GetWatchItem(theEnv,argument) == 1)
     { returnValue->lexemeValue = TrueSymbol(theEnv); }
   else
     { returnValue->lexemeValue = FalseSymbol(theEnv); }
  }

/*************************************************************/
/* WatchFunctionDefinitions: Initializes the watch commands. */
/*************************************************************/
void WatchFunctionDefinitions(
  Environment *theEnv)
  {
#if ! RUN_TIME
   AddUDF(theEnv,"watch","v",1,UNBOUNDED,"*;y",WatchCommand,"WatchCommand",NULL);
   AddUDF(theEnv,"unwatch","v",1,UNBOUNDED,"*;y",UnwatchCommand,"UnwatchCommand",NULL);
   AddUDF(theEnv,"get-watch-item","b",1,1,"y",GetWatchItemCommand,"GetWatchItemCommand",NULL);
   AddUDF(theEnv,"list-watch-items","v",0,UNBOUNDED,"*;y",ListWatchItemsCommand,"ListWatchItemsCommand",NULL);
#endif

   AddRouter(theEnv,WTRACE,1000,RecognizeWatchRouters,CaptureWatchPrints,NULL,NULL,NULL,NULL);
   DeactivateRouter(theEnv,WTRACE);
  }

/**************************************************/
/* RecognizeWatchRouters: Looks for WTRACE prints */
/**************************************************/
static bool RecognizeWatchRouters(
  Environment *theEnv,
  const char *logName,
  void *context)
  {
#if MAC_XCD
#pragma unused(theEnv)
#endif

   if (strcmp(logName,WTRACE) == 0) return true;

   return false;
  }

/**************************************************/
/* CaptureWatchPrints: Suppresses WTRACE messages */
/**************************************************/
static void CaptureWatchPrints(
  Environment *theEnv,
  const char *logName,
  const char *str,
  void *context)
  {
#if MAC_XCD
#pragma unused(logName)
#pragma unused(str)
#pragma unused(theEnv)
#endif
  }

#endif /* DEBUGGING_FUNCTIONS */


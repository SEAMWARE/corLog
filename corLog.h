#ifndef CORLOG_CORLOG_H_
#define CORLOG_CORLOG_H_

// 
// FILE            kTrace.h
// 
// AUTHOR          Ken Zangelin
// 
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corLog/corLogInit.h"                 // corLogInit
#include "corLog/corLogOut.h"                  // corLogOut
#include "corLog/corLogGlobals.h"              // global variables
#include "corLog/corLogTraceLevelReset.h"      // corLogTraceLevelReset
#include "corLog/corLogTraceLevelSet.h"        // corLogTraceLevelSet
#include "corLog/corLogTraceLevelSetOne.h"     // corLogTraceLevelSetOne
#include "corLog/corLogTraceLevelGet.h"        // corLogTraceLevelGet



// -----------------------------------------------------------------------------
//
// corLog Macros
//
// COR_D  - debug messages,   depending on the variable 'corLogDebug' - these are temporary, to be removed before PR
// COR_T  - trace messages,   depending on the trace level 'tLevel'
// COR_V  - verbose messages, depending on the variable 'corLogVerbose'
// COR_I  - info messages,    depending on the variable 'corLogInfo'
// COR_F  - FIXME messages,   depending on the variable 'corLogFixme'
// COR_W  - warnings
// COR_E  - errors
// COR_X  - fatal errors, including exit
// COR_RE  - error + return a value
// COR_RVE - error + return from a void function
//
//
// The macros test the level INLINE and call corLogOut only when the line is to be written.
//
// corLogOut makes the same test, but a call that ends in its early return still costs the call:
// a variadic one, whose argument set-up and register spill is paid in full - and the ARGUMENTS
// are evaluated before it, strlen()s included. Traces sit in per-node loops (the JSON parser,
// corTreeFree), where that was 7.6% of all instructions of a batch create with tracing OFF.
// Behind the test, a trace that is off costs a load and a branch, and its arguments are never
// evaluated.
//
// Expressions, not do-while: a macro used as an expression keeps compiling.
//
static inline bool corLogTraceOn(unsigned int level)
{
  unsigned int index = level / 32;

  if ((corLogInitDone == false) || (index >= sizeof(corLogTraceLevels) / sizeof(corLogTraceLevels[0])))
    return false;

  return (corLogTraceLevels[index] & (1U << (level % 32))) != 0;
}

#define COR_LOG_IF(on, type, aux, ...)  ((on) ? corLogOut(__FILE__, __LINE__, __FUNCTION__, type, aux, __VA_ARGS__) : (void) 0)

#define COR_F(...)           COR_LOG_IF(corLogFixme   == true,  'F', -1,     __VA_ARGS__)
#define COR_D(...)           COR_LOG_IF(corLogDebug   == true,  'D', -1,     __VA_ARGS__)
//
// COR_T exists in a build that defines COR_T_ON (a debug build) and nowhere else. Without it a trace
// is compiled away - no code, no string, no load and branch - but it stays an expression whose
// arguments are type-checked and count as used: a variable only a trace reads compiles in a
// release build (-Werror), and a broken trace fails the release build too, not only the debug one.
//
// Every object compiled with COR_T_ON carries the string "COR_T_ON:traces-compiled-in" (here and in
// corBase/corLibLog.h), so a binary or an archive says whether it holds debug code: a release build
// must not, and coraine's makefile fails a release link that does.
//
#ifdef COR_T_ON
#define COR_T(tLevel, ...)   COR_LOG_IF(corLogTraceOn(tLevel),  'T', tLevel, __VA_ARGS__)
static const char corLogTracesCompiledIn[] __attribute__((used)) = "COR_T_ON:traces-compiled-in";
#else
#define COR_T(tLevel, ...)   COR_LOG_IF(0,                      'T', tLevel, __VA_ARGS__)
#endif
#define COR_V(...)           COR_LOG_IF(corLogVerbose == true,  'V', -1,     __VA_ARGS__)
#define COR_I(...)           COR_LOG_IF(corLogInfo    == true,  'I', -1,     __VA_ARGS__)
#define COR_W(...)           corLogOut(__FILE__, __LINE__, __FUNCTION__, 'W', -1,     __VA_ARGS__)
#define COR_E(...)           corLogOut(__FILE__, __LINE__, __FUNCTION__, 'E', -1,     __VA_ARGS__)
#define COR_X(eCode, ...)    corLogOut(__FILE__, __LINE__, __FUNCTION__, 'X', eCode,  __VA_ARGS__)
#define COR_RE(retVal, ...)  do { corLogOut(__FILE__, __LINE__, __FUNCTION__, 'E', -1,     __VA_ARGS__); return retVal; } while (0)
#define COR_RVE(...)         do { corLogOut(__FILE__, __LINE__, __FUNCTION__, 'E', -1,     __VA_ARGS__); return;        } while (0)

#endif  // CORLOG_CORLOG_H_

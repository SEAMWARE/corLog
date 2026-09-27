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
#define COR_F(...)           corLogOut(__FILE__, __LINE__, __FUNCTION__, 'F', -1,     __VA_ARGS__)
#define COR_D(...)           corLogOut(__FILE__, __LINE__, __FUNCTION__, 'D', -1,     __VA_ARGS__)
#define COR_T(tLevel, ...)   corLogOut(__FILE__, __LINE__, __FUNCTION__, 'T', tLevel, __VA_ARGS__)
#define COR_V(...)           corLogOut(__FILE__, __LINE__, __FUNCTION__, 'V', -1,     __VA_ARGS__)
#define COR_I(...)           corLogOut(__FILE__, __LINE__, __FUNCTION__, 'I', -1,     __VA_ARGS__)
#define COR_W(...)           corLogOut(__FILE__, __LINE__, __FUNCTION__, 'W', -1,     __VA_ARGS__)
#define COR_E(...)           corLogOut(__FILE__, __LINE__, __FUNCTION__, 'E', -1,     __VA_ARGS__)
#define COR_X(eCode, ...)    corLogOut(__FILE__, __LINE__, __FUNCTION__, 'X', eCode,  __VA_ARGS__)
#define COR_RE(retVal, ...)  do { corLogOut(__FILE__, __LINE__, __FUNCTION__, 'E', -1,     __VA_ARGS__); return retVal; } while (0)
#define COR_RVE(...)         do { corLogOut(__FILE__, __LINE__, __FUNCTION__, 'E', -1,     __VA_ARGS__); return;        } while (0)

#endif  // CORLOG_CORLOG_H_

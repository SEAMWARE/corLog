#ifndef CORLOG_CORLOGINIT_H_
#define CORLOG_CORLOGINIT_H_

// 
// FILE            corLogInit.h
// 
// AUTHOR          Ken Zangelin
// 
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdbool.h>                          // bool



// -----------------------------------------------------------------------------
//
// corLogInit -
//
extern int corLogInit(const char* progName, const char* logDir, bool logToScreen, const char* logLevel, const char* traceLevels, bool verbose, bool debug, bool fixme);

#endif  // CORLOG_CORLOGINIT_H_

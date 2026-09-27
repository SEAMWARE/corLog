#ifndef CORLOG_CORLOGGLOBALS_H_
#define CORLOG_CORLOGGLOBALS_H_

// 
// FILE            corLogGlobals.h
// 
// AUTHOR          Ken Zangelin
// 
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <time.h>                             // struct timespec
#include <pthread.h>                          // pthread_mutex_t
#include <stdbool.h>                          // bool



// -----------------------------------------------------------------------------
//
// Global variables for the corLog library
//
extern int                 corLogFd;
extern bool                corLogToScreen;
extern struct timespec     corLogStartTime;
extern pthread_mutex_t     corLogMutex;
extern bool                corLogInfo;
extern bool                corLogMsg;
extern bool                corLogVerbose;
extern unsigned int        corLogTraceLevels[100];  // 3200 trace levels
extern bool                corLogDebug;
extern bool                corLogFixme;
extern bool                corLogInitDone;

#endif  // CORLOG_CORLOGGLOBALS_H_

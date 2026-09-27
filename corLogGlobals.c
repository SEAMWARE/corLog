//
// FILE            corLogGlobals.c
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

#include "corLog/corLogGlobals.h"             // Own interface



// -----------------------------------------------------------------------------
//
// Global variables for the corLog library
//
int                 corLogFd       = -1;
bool                corLogToScreen = false;
struct timespec     corLogStartTime;
pthread_mutex_t     corLogMutex    = PTHREAD_MUTEX_INITIALIZER;
bool                corLogInfo;
bool                corLogVerbose;
unsigned int        corLogTraceLevels[100];  // 3200 trace levels
bool                corLogDebug;
bool                corLogFixme;
bool                corLogInitDone = false;

//
// FILE            corLogInit.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdio.h>                            // rename
#include <unistd.h>                           // access
#include <sys/types.h>                        // open
#include <sys/stat.h>                         // open
#include <fcntl.h>                            // O_RDWR, O_TRUNC, O_CREAT
#include <string.h>                           // strerror
#include <errno.h>                            // errno
#include <pthread.h>                          // pthread_mutex_init
#include <stdbool.h>                          // bool

#include "corBase/corMacros.h"                // COR_VEC_SIZE
#include "corBase/corTime.h"                  // corTimeGet

#include "corLog/corLogGlobals.h"             // corLogFd, corLogToStdout, corLogSem, corLogStartTime
#include "corLog/corLogTraceLevelSet.h"       // corLogTraceLevelSet
#include "corLog/corLogInit.h"                // Own interface



// -----------------------------------------------------------------------------
//
// corLogInit -
//
int corLogInit(const char* progName, const char* logDir, bool logToScreen, const char* logLevel, const char* traceLevels, bool verbose, bool fixme)
{
  //
  // logDir == NULL means traces only to stdout
  //
  if ((logDir != NULL) && (logDir[0] != 0))
  {
    char path[512];
    snprintf(path, sizeof(path) - 1, "%s/%s.log", logDir, progName);

    // If the file already exists, move it to .old
    if (access(path, F_OK) == 0)
    {
      char oldPath[520];
      snprintf(oldPath, sizeof(oldPath) - 1, "%s.old", path);
      rename(path, oldPath);
    }

    //
    // O_APPEND: every write lands at the end of the file as it is NOW - so a file truncated under
    // the process (logrotate's copytruncate) is written from its new start, not from the old offset
    // with a hole of zero bytes in front.
    //
    corLogFd = open(path, O_WRONLY | O_APPEND | O_TRUNC | O_CREAT, 0664);
    if (corLogFd < 0)
    {
      fprintf(stderr, "Unable to open '%s' for writing: %s\n", path, strerror(errno));
      return -1;
    }
  }
  else
    corLogFd = 1;  // stdout

  // kTrace mutex - to avoid mix of different simultaneous trace outputs
  // (already statically initialized via PTHREAD_MUTEX_INITIALIZER, but reinit for safety)
  pthread_mutex_init(&corLogMutex, NULL);

  // Record the start-time, for future diffs for the timestamps
  corTimeGet(&corLogStartTime);

  // Print logs to screen?
  corLogToScreen = logToScreen;

  // Enable FIXME messages?
  corLogFixme = fixme;

  // Log Levels
  int level = -1;
  if (logLevel != NULL)
  {
    const char* levelStrings[] = { "CERO", "ERR", "WARN", "INFO", "VERBOSE", "TRACE" };
    int         levels[]       = {   0,      1,     2,       3,       4,        5   };

    for (unsigned int ix = 1; ix < COR_VEC_SIZE(levelStrings); ix++)
    {
      if (strcmp(logLevel, levelStrings[ix]) == 0)
      {
        level = levels[ix];
        break;
      }
    }
    
    if (level == -1)
    {
      fprintf(stderr, "%s: invalid log level '%s'\n", progName, logLevel);
      return -1;
    }
  }

  // If traceLevels has been set, the logLevel might be bumped up
  if (traceLevels != NULL)
    level = 5;

  corLogInfo    = (level >= 3)? true : false;
  corLogVerbose = ((level >= 4) || (verbose == true))? true : false;

  if (traceLevels != NULL)
    corLogTraceLevelSet(traceLevels, true);

  corLogInitDone = true;
  return 0;
}

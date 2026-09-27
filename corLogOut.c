//
// FILE            corLogOut.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdio.h>                            // snprintf, vsnprintf
#include <stdlib.h>                           // malloc, free
#include <string.h>                           // strlen, strncpy
#include <stdarg.h>                           // va_start, va_end
#include <sys/uio.h>                          // writev
#include <pthread.h>                          // pthread_mutex_lock, pthread_mutex_unlock
#include <time.h>                             // struct timespec
#include <stdlib.h>                           // exit
#include <stdbool.h>                          // bool

#include "corBase/corTime.h"                  // corTimeGet, corTimeDiff
#include "corLog/corLogGlobals.h"             // corLogFt, corLogSem, ...
#include "corLog/corLogTraceLevelCheck.h"     // corLogTraceLevelCheck
#include "corLog/corLogOut.h"                 // Own interface



// -----------------------------------------------------------------------------
//
// corLogOut -
//
void corLogOut
(
  const char*  fileName,
  int          lineNo,
  const char*  functionName,
  char         type,
  int          aux,
  const char*  format,
  ...
)
{
  va_list ap;

  //
  // Should the trace message be printed?
  // If not, just return
  //
  if (corLogInitDone == false)                                    return;
  if ((type == 'T')  && (corLogTraceLevelCheck(aux) == false))    return;
  if ((type == 'D')  && (corLogDebug                == false))    return;
  if ((type == 'V')  && (corLogVerbose              == false))    return;
  if ((type == 'F')  && (corLogFixme                == false))    return;
  if ((type == 'I')  && (corLogInfo                 == false))    return;

  //
  // Log Line:
  // timestamp : logType : file[line]: function: msg
  //
  char  timestamp[16];
  int   timestampLen;
  char* separator = (char*) ": ";
  char  logType[2];
  char  lineNoString[8];
  int   lineNoLen;
  char  msg[1024];
  char* msgP = msg;
  int   msgLen;

  //
  // __FILE__ unfortunetely comes as abs path. We want just the file name
  //
  char* file = strrchr((char*) fileName, '/');
  if (file == NULL)
    file = (char*) fileName;
  else
    file = &file[1];


  logType[0] = type;
  logType[1] = 0;
  lineNoLen  = snprintf(lineNoString, sizeof(lineNoString), "%d", lineNo);

  va_start(ap, format);
  msgLen = vsnprintf(msgP, sizeof(msg), format, ap);
  va_end(ap);

  if (msgLen >= (int) sizeof(msg))  // Message was truncated - need more room
  {
    char* newMsgP = malloc(msgLen + 1);
    if (newMsgP != NULL)
    {
      va_list ap2;
      va_start(ap2, format);
      msgP   = newMsgP;
      msgLen = vsnprintf(msgP, msgLen + 1, format, ap2);
      va_end(ap2);
    }
    // If malloc fails, we just use the truncated message in msg[]
  }

  struct timespec now;
  struct timespec diff;

  corTimeGet(&now);
  corTimeDiff(&corLogStartTime, &now, &diff, NULL);
  timestampLen = snprintf(timestamp, sizeof(timestamp) - 1, "%06d.%03d", (int) diff.tv_sec, (int) (diff.tv_nsec / 1000000));

  int           comps    = 13;
  struct iovec  comp[15] = {
    { logType, 1 },
    { separator, 2},
    { timestamp, timestampLen },
    { separator, 2},
    { (char*) file, strlen(file)},
    { "[", 1},
    { lineNoString, lineNoLen},
    { "]", 1},
    { separator, 2},
    { (char*) functionName, strlen(functionName)},
    { separator, 2},
    { msgP, msgLen },
    { "\n", 1}
  };

  char level[8];
  if (type == 'T')
  {
    int  levelLen;

    levelLen = snprintf(level, sizeof(level) - 1, "%d", aux);
    comps += 2;
    comp[12].iov_base = " (T-";
    comp[12].iov_len  = 4;
    comp[13].iov_base = level;
    comp[13].iov_len  = levelLen;
    comp[14].iov_base = ")\n";
    comp[14].iov_len  = 2;
  }

  if (pthread_mutex_lock(&corLogMutex) != 0)
  {
    fprintf(stderr, "Error locking corLog mutex\n");
    if (msgP != msg)
      free(msgP);
    return;
  }

  // Write to log file
  int nb = writev(corLogFd, comp, comps);
  if (nb <= 0)
    fprintf(stderr, "Error writing trace line to log file\n");

  // Write to stdout?
  if ((corLogFd != 1) && (corLogToScreen == true))
  {
    nb = writev(1, comp, comps);
    if (nb <= 0)
      fprintf(stderr, "Error writing trace line to stdout\n");
  }

  pthread_mutex_unlock(&corLogMutex);

  if (msgP != msg)  // the buffer has been allocated
    free(msgP);

  if (type == 'X')
    exit(aux);
}

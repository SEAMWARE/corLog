//
// FILE            corLogTraceLevelGet.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2026 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdio.h>                            // snprintf

#include "corLog/corLogGlobals.h"             // corLogTraceLevels
#include "corLog/corLogTraceLevelGet.h"       // Own interface



// -----------------------------------------------------------------------------
//
// corLogTraceIsSet - check if a trace level is set
//
static inline int corLogTraceIsSet(int level)
{
  return (corLogTraceLevels[level / 32] & (1U << (level % 32))) != 0;
}



// -----------------------------------------------------------------------------
//
// corLogTraceLevelGet -
//
// Returns the current trace levels as a string (e.g. "5,10-45,101")
// The returned string is static - do not free it.
//
const char* corLogTraceLevelGet(void)
{
  static char  levelString[4096];
  char*        p          = levelString;
  int          remaining  = sizeof(levelString) - 1;
  int          rangeStart = -1;
  int          first      = 1;

  levelString[0] = 0;

  for (int level = 0; level < 3200; level++)
  {
    int isSet = corLogTraceIsSet(level);

    if (isSet && rangeStart == -1)
    {
      // Start of a new range
      rangeStart = level;
    }
    else if (!isSet && rangeStart != -1)
    {
      // End of a range
      int rangeEnd = level - 1;
      int written;

      if (!first)
      {
        written = snprintf(p, remaining, ",");
        p += written;
        remaining -= written;
      }

      if (rangeStart == rangeEnd)
        written = snprintf(p, remaining, "%d", rangeStart);
      else
        written = snprintf(p, remaining, "%d-%d", rangeStart, rangeEnd);

      p += written;
      remaining -= written;
      first = 0;
      rangeStart = -1;

      if (remaining <= 0)
        break;
    }
  }

  // Handle case where last level(s) are set
  if (rangeStart != -1)
  {
    int rangeEnd = 3199;
    int written;

    // Find actual end of range
    for (int level = 3199; level >= rangeStart; level--)
    {
      if (corLogTraceIsSet(level))
      {
        rangeEnd = level;
        break;
      }
    }

    if (!first)
    {
      written = snprintf(p, remaining, ",");
      p += written;
      remaining -= written;
    }

    if (rangeStart == rangeEnd)
      snprintf(p, remaining, "%d", rangeStart);
    else
      snprintf(p, remaining, "%d-%d", rangeStart, rangeEnd);
  }

  return levelString;
}

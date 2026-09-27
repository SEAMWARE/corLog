//
// FILE            corLogTraceLevelSet.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <string.h>                           // strncpy, strchr, memset
#include <stdlib.h>                           // atoi
#include <stdbool.h>                          // bool

#include "corLog/corLogGlobals.h"             // corLogTraceLevels
#include "corLog/corLogTraceLevelSetOne.h"    // corLogTraceLevelSetOne
#include "corLog/corLogTraceLevelSet.h"       // Own interface



// -----------------------------------------------------------------------------
//
// corLogTraceLevelSet -
//
void corLogTraceLevelSet(const char* levelString, bool replace)
{
  char  levels[512];
  char* levelsP = levels;

  if (replace == true)
    memset(corLogTraceLevels, 0, sizeof(corLogTraceLevels));

  if ((levelString == NULL) || (*levelString == 0))
    return;

  strncpy(levels, levelString, sizeof(levels) - 1);

  while (1)
  {
    char* start = levelsP;

    // Find comma
    char* commaP = strchr(start, ',');
    if (commaP != NULL)
    {
      *commaP = 0;
      levelsP = &commaP[1];  // For the next loop
    }

    // Is it a number of a range?
    int   from    = 0;
    int   to      = 0;
    char* hyphenP = strchr(start, '-');

    if (hyphenP == NULL)
    {
      from = atoi(start);
      to   = from;
    }
    else
    {
      *hyphenP = 0;
      from     = atoi(start);
      to       = atoi(&hyphenP[1]);
    }

    for (int ix = from; ix <= to; ix++)
    {
      corLogTraceLevelSetOne(ix);
    }

    if (commaP == NULL)
      break;
  }
}

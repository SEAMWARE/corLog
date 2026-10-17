//
// FILE            corLogTraceLevelReset.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2026 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <string.h>                           // strncpy, strchr, strrchr, strlen
#include <stdlib.h>                           // atoi

#include "corLog/corLogTraceLevelResetOne.h"  // corLogTraceLevelResetOne
#include "corLog/corLogTraceLevelReset.h"     // Own interface



// -----------------------------------------------------------------------------
//
// corLogTraceLevelReset -
//
void corLogTraceLevelReset(const char* levelString)
{
  char  levels[512];
  char* levelsP = levels;

  if ((levelString == NULL) || (*levelString == 0))
    return;

  //
  // Copied whole, or cut after its last complete item: strncpy alone leaves the buffer unterminated for a string
  // of 511 bytes or more, and a cut inside a number names another level ("...,403" cut to "...,4")
  //
  strncpy(levels, levelString, sizeof(levels) - 1);
  levels[sizeof(levels) - 1] = 0;

  if (strlen(levelString) >= sizeof(levels))
  {
    char* lastComma = strrchr(levels, ',');

    if (lastComma == NULL)
      return;                                 // one item longer than the buffer: no level in it

    *lastComma = 0;
  }

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
      corLogTraceLevelResetOne(ix);
    }

    if (commaP == NULL)
      break;
  }
}

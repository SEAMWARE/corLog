//
// FILE            corLogTraceLevelCheck.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdbool.h>                          // bool
#include "kbase/kMacros.h"                    // K_VEC_SIZE

#include "corLog/corLogGlobals.h"             // corLogTraceLevels
#include "corLog/corLogTraceLevelCheck.h"     // Own interface



// -----------------------------------------------------------------------------
//
// corLogTraceLevelCheck -
//
bool corLogTraceLevelCheck(unsigned int level)
{
  unsigned int index = level / 32;

  if (index >= K_VEC_SIZE(corLogTraceLevels))
    return false;

  level = level % 32;

  unsigned int mask = 1 << level;

  if ((mask & corLogTraceLevels[index]) == 0)
    return false;

  return true;
}

//
// FILE            corLogTraceLevelSetOne.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "kbase/kMacros.h"                    // K_VEC_SIZE

#include "corLog/corLogGlobals.h"             // corLogTraceLevels
#include "corLog/corLogTraceLevelSetOne.h"    // Own interface



// -----------------------------------------------------------------------------
//
// corLogTraceLevelSetOne -
//
void corLogTraceLevelSetOne(int level)
{
  unsigned int index = level / 32;

  if (index >= K_VEC_SIZE(corLogTraceLevels))
    return;

  level = level % 64;
  unsigned int mask = 1 << level;

  corLogTraceLevels[index] |= mask;
}

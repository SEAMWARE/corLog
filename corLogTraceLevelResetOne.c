//
// FILE            corLogTraceLevelResetOne.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2026 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corBase/corMacros.h"                // COR_VEC_SIZE

#include "corLog/corLogGlobals.h"             // corLogTraceLevels
#include "corLog/corLogTraceLevelResetOne.h"  // Own interface



// -----------------------------------------------------------------------------
//
// corLogTraceLevelResetOne -
//
void corLogTraceLevelResetOne(int level)
{
  unsigned int index = level / 32;

  if (index >= COR_VEC_SIZE(corLogTraceLevels))
    return;

  level = level % 32;
  unsigned int mask = 1U << level;

  corLogTraceLevels[index] &= ~mask;
}

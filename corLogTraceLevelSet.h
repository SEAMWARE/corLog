#ifndef CORLOG_CORLOGTRACELEVELSET_H_
#define CORLOG_CORLOGTRACELEVELSET_H_

//
// FILE            corLogTraceLevelSet.h
//
// AUTHOR          Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdbool.h>                          // bool



// -----------------------------------------------------------------------------
//
// corLogTraceLevelSet -
//
// If 'replace' is true, all existing trace levels are cleared before setting new ones.
// If 'replace' is false, new trace levels are added to existing ones.
//
extern void corLogTraceLevelSet(const char* levelString, bool replace);

#endif  // CORLOG_CORLOGTRACELEVELSET_H_

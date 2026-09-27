#ifndef CORLOG_CORLOGTRACELEVELGET_H_
#define CORLOG_CORLOGTRACELEVELGET_H_

//
// FILE            corLogTraceLevelGet.h
//
// AUTHOR          Ken Zangelin
//
// Copyright 2026 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//



// -----------------------------------------------------------------------------
//
// corLogTraceLevelGet -
//
// Returns the current trace levels as a string (e.g. "5,10-45,101")
// The returned string is static - do not free it.
//
extern const char* corLogTraceLevelGet(void);

#endif  // CORLOG_CORLOGTRACELEVELGET_H_

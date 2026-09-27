#ifndef CORLOG_CORLOGOUT_H_
#define CORLOG_CORLOGOUT_H_

// 
// FILE            corLogOut.h
// 
// AUTHOR          Ken Zangelin
// 
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//



// -----------------------------------------------------------------------------
//
// corLogOut -
//
extern void corLogOut
(
  const char*  fileName,
  int          lineNo,
  const char*  functionName,
  char         type,
  int          aux,
  const char*  format,
  ...
);

#endif  // CORLOG_CORLOGOUT_H_

//
// FILE            corLogTest.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <unistd.h>                        // usleep

#include "corLog/corLog.h"                 // K-Trace library



// -----------------------------------------------------------------------------
//
// main -
//
int main(int argC, char* argV[])
{
  corLogInit("corLogTest", "/tmp", true, "DEBUG", (argC > 1)? argV[1] : NULL, true, true, true);

  usleep(5000);
  COR_T(5, "This is a trace level message");
  COR_T(19, "This is a trace level message");
  COR_T(3100, "This is a trace level message");
  usleep(5000);
  COR_D("This is a %s message", "debug");
  usleep(5000);
  COR_V("This is a %s message", "verbose");
  usleep(5000);
  COR_W("This is a %s", "warning");
  usleep(5000);
  COR_E("This is an %s", "error");
  usleep(5000);
  COR_X(5, "exiting with error code %d", 5);
}

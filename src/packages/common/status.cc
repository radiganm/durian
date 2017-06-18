/* status.cc
 * Mac Radigan
 * Copyright 2016 Mac Radigan
 * All Rights Reserved
 */

#include "packages/common/status.h"

#include <stdio.h>

  void ck(const char* msg, int status, int value)
  {
    if(status != value)
    {
      perror(msg);
    }
  }

/* *EOF* */

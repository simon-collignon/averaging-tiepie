/**
 * CheckStatus.c
 *
 * This file is part of the LibTiePie programming examples.
 *
 * Find more information on http://www.tiepie.com/LibTiePie .
 */

#include "CheckStatus.h"
#include <stdio.h>
#include <libtiepie.h>
#include "Utils.h" // for NEWLINE

void checkLastStatus(const char* file, unsigned int line)
{
  LibTiePieStatus_t status = LibGetLastStatus();

  if(status < LIBTIEPIESTATUS_SUCCESS)
    fprintf(stderr, "%s:%u Error: %s" NEWLINE, file, line, LibGetLastStatusStr());
  else if(status > LIBTIEPIESTATUS_SUCCESS)
    fprintf(stderr, "%s:%u Warning: %s" NEWLINE, file, line, LibGetLastStatusStr());
}

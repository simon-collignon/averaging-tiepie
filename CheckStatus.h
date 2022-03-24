/**
 * CheckStatus.h
 *
 * This file is part of the LibTiePie programming examples.
 *
 * Find more information on http://www.tiepie.com/LibTiePie .
 */

#ifndef _CHECKSTATUS_H_
#define _CHECKSTATUS_H_

#define CHECK_LAST_STATUS() checkLastStatus(__FILE__, __LINE__);

void checkLastStatus(const char* file, unsigned int line);

#endif

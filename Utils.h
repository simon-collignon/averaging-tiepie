/**
 * Utils.h
 *
 * This file is part of the LibTiePie programming examples.
 *
 * Find more information on http://www.tiepie.com/LibTiePie .
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#  define OS_WINDOWS
#  define NEWLINE "\r\n"
#else // POSIX
#  define NEWLINE "\n"
#endif

void sleepMiliSeconds(unsigned int ms);
void waitForKeyStroke();

#endif

/*
// Full copyright information is available in the file ../docs/CREDITS
*/
/* common utils */

#ifndef cutil_h
#define cutil_h

#include "config.h"

int touch P((char *));
#ifndef HAVE_STRDUP
char * strdup P((char *));
#endif

#endif

/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <struct.h>
#include "common.h"
#include "main.h"
#include "db.h"
#include "interface.h"
#include "file.h"

#undef PLAYERFILE
#define PLAYERFILE ".players"


static char *myname;
char *playerFile;

int main(int argc, char *argv[])
{
	myname = argv[0];

	if(argc > 2) {
		fprintf(stderr, "Usage:  %s [playerfile]\n", myname);
		exit(1);
	}
#ifndef SYSV
	signal(SIGWINCH, getTTYinfo);
#endif
	if(argc == 2)
		playerFile = argv[1];
	else
		playerFile = PLAYERFILE;

	getTTYinfo();
	if(ReadIt(playerFile)) exit(1);
	Interface();
	exit(0);
}

void err(char *s, ...)
{
	va_list ap;
	char txt[60];

	va_start(ap, s);
	vsprintf(txt, s, ap);
	fprintf(stderr, "%s: %s\n", myname, txt);
	va_end(ap);
}

void err_sys(char *s, ...)
{
	va_list ap;
	char txt[60];

	va_start(ap, s);
	vsprintf(txt, s, ap);
	fprintf(stderr, "%s: %s: %s\n", myname, txt, strerror(errno));
	va_end(ap);
}

void GoAway(int type)
{
	if(type)
		if(!Verify("quit?  There are unsaved changes."))
			return;

	printf("\npped version %s by H. Kehoe\n\n", VERSSTR);
	exit(0);
}


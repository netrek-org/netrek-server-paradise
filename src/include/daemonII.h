/*--------------------------------------------------------------------------
NETREK II -- Paradise

Permission to use, copy, modify, and distribute this software and its
documentation, or any derivative works thereof, for any NON-COMMERCIAL
purpose and without fee is hereby granted, provided that this copyright
notice appear in all copies.  No representations are made about the
suitability of this software for any purpose.  This software is provided
"as is" without express or implied warranty.

    Xtrek Copyright 1986                            Chris Guthrie
    Netrek (Xtrek II) Copyright 1989                Kevin P. Smith
                                                    Scott Silvey
    Paradise II (Netrek II) Copyright 1993          Larry Denys
                                                    Kurt Olsen
                                                    Brandon Gillespie
--------------------------------------------------------------------------*/

#include "config.h"
#include <setjmp.h>
#include "struct.h"

/*-------------------------------NUMBER DEFINES-----------------------------*/
#define TOURNEXTENSION 15	/* Tmode gone for 15 seconds 8/26/91 TC */

#define PLAYERFUSE	1
#define TORPFUSE	1
#define MISSILEFUSE	1
#define PLASMAFUSE      1
#define PHASERFUSE	1
#define CLOAKFUSE       2
#define TEAMFUSE	5
#define PLFIGHTFUSE	5
#define TERRAINFUSE     1
#define BEAMFUSE	8	/* scott 8/25/90 -- was 10 */
#define PLANETFUSE	SECONDS(15)	/* every 15 seconds */
#define MINUTEFUSE      MINUTES(1)	/* 1 minute, surrender funct etc.
					   4/15/92 TC */
#define SYNCFUSE	MINUTES(5)
#define CHECKLOADFUSE   MINUTES(15)	/* change 1/26/91 -- was 600 */
#define HOSEFUSE	MINUTES(20)	/* 20 min., was 15 minutes 6/29/92 TC */
#define HOSEFUSE2	MINUTES(5)	/* 5 min., was  3 minutes 6/29/92 TC */

#define GHOSTTIME	SECONDS(30)	/* 30 secs */
#define OUTFITTIME	SECONDS(6 * AUTOQUIT)	/* 6 * AQ secs */

#define HUNTERKILLER	(-1)
#define TERMINATOR  	(-2)	/* Terminator */
#define STERMINATOR	(-3)	/* sticky Terminator */
/*--------------------------------------------------------------------------*/



/*---------------------------------MACROS-----------------------------------*/

#define FUSE(X) ((ticks % (X)) == 0)

#define NotTmode(X) (!(status->tourn) && ((X - tourntimestamp)/10 > TOURNEXTENSION))

/*--------------------------------------------------------------------------*/

void rescue P((int, int, int));
void saveplayer P((struct player *));
void ghostmess P((struct player *));


/*----------------------------MODULE VARIABLES------------------------------*/

#ifndef DAEMONII		/* only declare if not in daemonII.c */
extern int tcount[MAXTEAM + 1];
extern int ticks;
extern int tm_robots[MAXTEAM + 1];	/* To limit the number of robots */
extern int tourntimestamp;	/* ticks since last Tmode 8/2/91 TC */
extern char *teamVerbage[9];
extern jmp_buf env;
extern int dietime;
extern  plfd, glfd;
#endif

/*--------------------------------------------------------------------------*/

/*----------END OF FILE--------*/

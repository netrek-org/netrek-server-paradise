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

#include "defs.h"
#include "shmem.h"
#include "getship.h"
#include "data.h"

#define STEP 10
char   *myname;

main(argc, argv)
    int     argc;
    char  **argv;
{
    int     i;
    int     player;
    char    buf[1000];
    int     c, seconds, part;

    myname = argv[0];

    if (argc > 3)
	Usage();

    if (argc == 2) {
	i = sscanf(argv[1], "%d", &seconds);
	if (i != 1)
	    Usage();
	if (seconds < 0)
	    Usage();
    }
    else {
	seconds = 10;
    }
    openmem(0);

    part = seconds % STEP;
    if (part)
	sleep(part);

    for (seconds -= part; seconds > 0; seconds -= STEP) {
	sprintf(buf, "%s->ALL  ** Attention: The galaxy will be reset in %d seconds.", SERVNAME, seconds);
	pmessage(buf, 0, MALL);
	sleep(STEP);
    }

    sprintf(buf, "%s->ALL  ** Manual galaxy reset **", SERVNAME);
    pmessage(buf, 0, MALL);

    zap();

    /* *newgalaxy = 1;*/
    status2->newgalaxy = 1;
    exit(0);
}

pmessage(str, recip, group)
    char   *str;
    int     recip;
    int     group;
{
    struct message *cur;
    if (++(mctl->mc_current) >= MAXMESSAGE)
	mctl->mc_current = 0;
    cur = &messages[mctl->mc_current];
    cur->m_no = mctl->mc_current;
    cur->m_flags = group;
    cur->m_time = 0;
    cur->m_from = 255;		/* change 3/30/91 TC */
    cur->m_recpt = recip;
    (void) sprintf(cur->m_data, "%s", str);
    cur->m_flags |= MVALID;
}


Usage()
{
    int x;
    char message[][255] = {
	"\n\t'%s [n]'\n\n",
        "Where n is the seconds until the galaxy is reset (default: 10)\n"
        "\nThis tool resets the galaxy.\n",
        ""
    };

    fprintf(stderr, "-- NetrekII (Paradise), %s --\n", PARAVERS);
    for (x=0; *message[x] != '\0'; x++)
        fprintf(stderr, message[x], myname);

    exit(1);
}

zap()
{
    int     player;

    for (player = 0; player < MAXPLAYER; player++) {
	if (players[player].p_status == PFREE)
	    continue;
	players[player].p_whydead = KPROVIDENCE;
	players[player].p_whodead = 0;
	players[player].p_status = PEXPLODE;
	players[player].p_explode = 10;
	players[player].p_ntorp = 0;
	players[player].p_nplasmatorp = 0;
    }
}

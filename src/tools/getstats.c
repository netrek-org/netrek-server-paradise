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

struct stats {
    int     st_genocides;	/* number of genocides participated in */
    float   st_tmaxkills;	/* max kills ever */
    float   st_di;		/* total destruction inflicted for all time */
    int     st_tkills;		/* Kills in tournament play */
    int     st_tlosses;		/* Losses in tournament play */
    int     st_tarmsbomb;	/* Tournament armies bombed */
    int     st_tresbomb;	/* resources bombed off */
    int     st_tdooshes;	/* armies killed while being carried */
    int     st_tplanets;	/* Tournament planets conquered */
    int     st_tticks;		/* Tournament ticks */
    /* SB/WB/JS stats are entirely separate */
    int     st_sbkills;		/* Kills as starbase */
    int     st_sblosses;	/* Losses as starbase */
    int     st_sbticks;		/* Time as starbase */
    float   st_sbmaxkills;	/* Max kills as starbase */
    int     st_wbkills;		/* Kills as warbase */
    int     st_wblosses;	/* Losses as warbase */
    int     st_wbticks;		/* Time as warbase */
    float   st_wbmaxkills;	/* Max kills as warbase */
    int     st_jsplanets;	/* planets assisted with in JS */
    int     st_jsticks;		/* ticks played as a JS */
    long    st_lastlogin;	/* Last time this player was played */
    int     st_flags;		/* Misc option flags */
    char    st_keymap[96];	/* keymap for this player */
    int     st_rank;		/* Ranking of the player */
    int     st_royal;		/* royaly, specialty, rank */
};

struct statentry {
    char    name[16];		/* player's name */
    char    password[16];	/* player's password */
    struct stats stats;		/* player's stats */
};

int
main(int argc, char **argv)
{
    FILE   *f;
    struct statentry s;

    f = fopen(argv[1], "r");
    if (f == NULL) {
	printf("Cannot open players file\n");
	exit(1);
    }
    while (fread(&s, sizeof(struct statentry), 1, f) == 1) {
	printf("\nPlayer: %s\n", s.name);
	printf("Genocides: %d\n", s.stats.st_genocides);
	printf("Maxkills: %f\n", s.stats.st_tmaxkills);
	printf("DI: %f\n", s.stats.st_di);
	printf("Kills: %d\n", s.stats.st_tkills);
	printf("Losses: %d\n", s.stats.st_tlosses);
	printf("Armies bombed: %d\n", s.stats.st_tarmsbomb);
	printf("Resources bombed: %d\n", s.stats.st_tresbomb);
	printf("Dooshes: %d\n", s.stats.st_tdooshes);
	printf("Planets: %d\n", s.stats.st_tplanets);
	printf("Time: %f\n", (float) s.stats.st_tticks / 36000.0);
	printf("Rank: %d\n", s.stats.st_rank);
	printf("Royalty: %d\n", s.stats.st_royal);

	printf("SB kills: %d\n", s.stats.st_sbkills);
	printf("SB losses: %d\n", s.stats.st_sblosses);
	printf("SB time: %f\n", (float) s.stats.st_sbticks / 36000.0);
	printf("SB maxkills: %f\n", s.stats.st_sbmaxkills);

	printf("WB kills: %d\n", s.stats.st_wbkills);
	printf("WB losses: %d\n", s.stats.st_wblosses);
	printf("WB time: %f\n", (float) s.stats.st_wbticks / 36000.0);
	printf("WB maxkills: %f\n", s.stats.st_wbmaxkills);

	printf("JS planets: %f\n", s.stats.st_jsplanets);
	printf("JS time: %f\n", (float) s.stats.st_jsticks / 36000.0);
    }
    fclose(f);
}

/*------------------------------------------------------------------
  Copyright 1989		Kevin P. Smith
				Scott Silvey

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies.

  NETREK II -- Paradise

  Permission to use, copy, modify, and distribute this software and
  its documentation, or any derivative works thereof,  for any 
  NON-COMMERCIAL purpose and without fee is hereby granted, provided
  that this copyright notice appear in all copies.  No
  representations are made about the suitability of this software for
  any purpose.  This software is provided "as is" without express or
  implied warranty.

	Xtrek Copyright 1986			Chris Guthrie
	Netrek (Xtrek II) Copyright 1989	Kevin P. Smith
						Scott Silvey
	Paradise II (Netrek II) Copyright 1993	Larry Denys
						Kurt Olsen
						Brandon Gillespie
		                Copyright 2000  Bob Glamm

--------------------------------------------------------------------*/

#include <errno.h>
#include <sys/stat.h>

#define PLAYER_EDITOR	/* this gets us a non-status2 version of PLAYERFILE */
#include "config.h"
#include "proto.h"
#include "tool-util.h"
#include "data.h"
#include "shmem.h"

#ifdef sparc
extern char *sys_errlist[];
#define strerror(EN)	sys_errlist[(EN)]
#endif

#define LINESPERPAGE 38

static struct statentry *database;
static struct statentry **playertab;
static int     topn, motd = 0;

void header P((void));

void 
printUsage(char *me)
{
    int x;
    char message[][255] = {
        "\nHonor Roll of players in the current database.\n",
        "\n\t'%s n [options]'\n\n",
        "Where n is the number of scores to print (top n)\n",
        "\nOptions:\n",
        "\t-f file   Use \"file\" as player file (default: $NETREKDIR/etc/db.players)\n",
        "\t-m        Format output for use in server MOTD\n\n",
        ""
    };

    fprintf(stderr, "--- Netrek II (Paradise), %s ---\n", PARAVERS);
    for (x=0; *message[x] != '\0'; x++)
        fprintf(stderr, message[x], me);

    exit(1);
}

int 
cmp_func(const void *aa, const void *bb)
{
    const struct statentry **a = (const struct statentry **)aa;
    const struct statentry **b = (const struct statentry **)bb;
    float     di_diff = (*a)->stats.st_di - (*b)->stats.st_di;
    int     rk_diff = (*a)->stats.st_rank - (*b)->stats.st_rank;

    /* rank takes precedent over DI */
    if (rk_diff < 0)
	return 1;
    else if (rk_diff > 0)
	return -1;

    if (di_diff < 0)
	return 1;
    else if (di_diff > 0)
	return -1;

    return strcmp((*a)->name, (*b)->name);
}

#if 0
static char *rankh[] = {
    "Recruits:",
    "Specialists:",
    "Cadets:",
    "Midshipmen:",
    "Ensigns, Junior Grade:",
    "Ensigns:",
    "Lieutenants, Junior Grade:",
    "Lieutenants:",
    "Lieutenant Commanders:",
    "Commanders:",
    "Captains:",
    "Fleet Captains:",
    "Commodores:",
    "Moffs:",
    "Grand Moffs:",
    "Rear Admirals:",
    "Admirals:",
    "Grand Admirals:",
};
#endif

int 
main(int argc, char *argv[])
{
    int     i, nplayers, j, count = 0;
    FILE   *fp;
    struct stat fstats;
    char   *fn;
    struct stats *s;

    if (argc < 2)
	printUsage(argv[0]);

    if (!(topn = atoi(argv[1])))
	printUsage(argv[0]);

    /* initialize ranks/royals variables first */
    fn = build_path(RANKS_FILE);
    init_data(fn);

    fn = build_path(PLAYERFILE);

    for (i = 2; i < argc; i++) {
	if (!strcmp(argv[i], "-f")) {
	    if (i + 1 == argc)
		printUsage(argv[0]);
	    fn = argv[++i];
	}
	else if (!strcmp(argv[i], "-m")) {
	    motd = 1;
	}
    }

    if (!(fp = fopen(fn, "r"))) {
	fprintf(stderr, "Couldn't open file %s: %s\n", fn, strerror(errno));
	exit(1);
    }

    if (fstat(fileno(fp), &fstats) < 0) {
	fprintf(stderr, "Couldn't fstat file %s: %s\n", fn, strerror(errno));
	exit(1);
    }

    nplayers = fstats.st_size / sizeof(*database);
    database = malloc(sizeof(*database) * nplayers);

    i = fread(database, sizeof(*database), nplayers, fp);
    if (i != nplayers) {
	fprintf(stderr, "failed to read all player records from file %s (%d of %d)\n", fn, i, nplayers);
	nplayers = i;
    }

    fclose(fp);

    if (topn < 0 || topn > nplayers)
	topn = nplayers;

    /* Make an array of pointers to the database. */
    playertab = malloc(sizeof(playertab) * nplayers);

    for (i = 0; i < nplayers; i++)
	playertab[i] = &(database[i]);

    /* sort the pointers */
    qsort(playertab, nplayers, sizeof(playertab), cmp_func);

    header();
    count = 1;

    j = 18;
    for (i = 0; i < topn; i++) {
	s = &(playertab[i]->stats);
	if (j > s->st_rank) {
	    j = s->st_rank;
	    if (motd) {
		count += 2;
		if (count >= LINESPERPAGE) {
		    header();
		    count = 3;
		}
	    }
	    printf("\n%s\n", ranks[j].name);
	}
	if (motd && (++count > LINESPERPAGE)) {
	    header();
	    printf("\n");
	    count = 3;
	}
	printf("%4d. %15s %8.2f %6d %6d %6d  %4d  %5d %5d %7.2f\n", i + 1,
	       playertab[i]->name, s->st_di, s->st_tkills, s->st_tlosses,
	       s->st_tarmsbomb, s->st_tresbomb, s->st_tplanets,
	       s->st_tdooshes, (float) (s->st_tticks / 36000.0));
    }
    exit(1);
}

void
header(void)
{
    if (motd)
	printf("\t@@b\n");
    printf("TOP %-4d         Name     DI     Wins  Losses Armies Rsrcs  Plnts  Dshs  Hours\n", topn);
    printf("-------------------------------------------------------------------------------");
}

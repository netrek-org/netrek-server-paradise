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


char binary[] = "@(#)snake";

#define NEED_TIME 

#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <setjmp.h>
#include <ctype.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "shmem.h"

extern void (*r_signal()) ();
extern int findrslot();
extern int config();
struct player *perfs[2];
void     printsnakeUsage();
int     num_perfs = 2;
int     debug = 0;
int     target = -1;
int     berserk = 0;
int     patrol = 0;
int     noSmush = 0;
int     plan_guard = 0;		/* KAO */
int     planet1, planet2;	/* KAO */
int     team1 = 0, team2 = 0;
int     length = MAXTORP;	/* how many pairs of torps in the snake */

struct itimerval udt;		/* derived from frequency RF */
int     lastm;
int     tno = 0;

int 
choose_team(team)
{
    if (tno < 0)
	tno = team;
    if (!team1)
	team1 = 1 << team;
    else if (!team2)
	team2 = 1 << team;
    return 1;
}

int main(argc, argv)
    int     argc;
    char  **argv;
{
    register int i;
    int     snakemove(), exitSnake();
    int     pno;
    char    tlet;
    int     usage = 0;
    double  frequency = 10;	/* default of 10 updates per second */

    argv0 = argv[0];
    tno = -1;

    srand48(getpid() + time((time_t *) 0));
    memset(perfs, 0, sizeof(perfs));

    for (; argc > 1 && argv[1][0] == '-'; argc--, argv++) {
	switch (argv[1][1]) {
	case 'p':
	    patrol++;
	    break;
	case 'n':
	    noSmush = 1;
	    break;
	case 's':
	    noSmush = -1;
	    break;
	case 'g':		/* KAO */
	    plan_guard++;	/* KAO */
	    argv++;		/* KAO */
	    argc--;
	    planet1 = atoi(argv[1]);	/* KAO */
	    argc--;
	    argv++;		/* KAO */
	    planet2 = atoi(argv[1]);	/* KAO */
	    break;		/* KAO */
	case 'b':
	    berserk++;
	    break;
	case 'd':
	    debug++;
	    break;
	case 't':{		/* target */
		char    c;
		c = argv[1][2];
		target = -1;
		if (c == '\0') {
		    fprintf(stderr, "Must specify target.  e.g. -t3.\n");
		    exit(1);
		}
		if ((c >= '0') && (c <= '9'))
		    target = c - '0';
		else if ((c >= 'a') && (c <= 'z'))
		    target = c - 'a' + 10;
		else {
		    fprintf(stderr, "Must specify target.  e.g. -t3.\n");
		    exit(1);
		}
	    }
	    break;

	case 'T':		/* team */
	    tlet = argv[1][2];
	    if (isupper(tlet))
		tlet = tolower(tlet);
	    switch (tlet) {
	    case 'f':
		choose_team(0);
		break;
	    case 'r':
		choose_team(1);
		break;
	    case 'k':
		choose_team(2);
		break;
	    case 'o':
		choose_team(3);
		break;
	    case 'i':
		tno = 4;
		break;
	    default:
		fprintf(stderr, "Unknown team type.  Usage -Tx where x is [frkoi]\n");
		exit(1);
	    }			/* end switch argv */
	    break;

	case 'l':
	    length = atoi(argv[1] + 2);
	    if (length < 1) {
		length = 1;
	    }
	    else if (length > MAXTORP) {
		length = MAXTORP;
	    }
	    break;
	case 'f':
	    frequency = atof(argv[1] + 2);
	    if (frequency < 0) {
		frequency = 1;
	    }
	    break;
	default:
	    fprintf(stderr, "Unknown option '%c'\n", argv[1][1]);
	    usage++;
	    exit(1);
	}			/* end switch argv[1][1] */
    }				/* end for */

    if (usage) {
	printsnakeUsage();
	exit(1);
    }

    /* if -T wasn't specified default to FED */
    if (tno < 0)
	tno = lrand48() % 4;

    /* XX -- teams imply patrol */
    if (team1 && team2)
	patrol++;

#if 0
    /* debug */
    if (patrol) {
	printf("snake (%s): patrolling %s,%s\n", teamshort[1 << tno],
	       teamshort[team1],
	       teamshort[team2]);
    }
    fflush(stdout);
#endif

/*   readsysdefaults();*/

    (void) r_signal(SIGHUP, exitSnake);
    (void) r_signal(SIGINT, exitSnake);
    (void) r_signal(SIGBUS, exitSnake);
    (void) r_signal(SIGSEGV, exitSnake);
    openmem(0, 0);

    lastm = mctl->mc_current;

    for (i = 0; i < 2; i++) {	/* two players per snake */
	pno = findrslot();
	if (pno < 0) {
	    fprintf(stderr, "snake: no room in game\n");
	    if (i > 0)
		perfs[0]->p_status = PFREE;
	    exit(1);
	}
	me = &players[pno];

	perfs[i] = me;

	me->p_no = pno;
	myship = &me->p_ship;
	mystats = &me->p_stats;

	strcpy(pseudo, "The Snake");
	strcpy(login, "SnkeChrmr");

	strcpy(me->p_name, pseudo);
	me->p_name[sizeof(me->p_name) - 1] = '\0';

	(void) strncpy(me->p_login, login, sizeof(me->p_login));
	me->p_login[sizeof(me->p_login) - 1] = '\0';
	(void) strncpy(me->p_monitor, "Server", sizeof(me->p_monitor));
	me->p_monitor[sizeof(me->p_monitor) - 1] = '\0';
	/* enter(tno, 0, pno, class, -1); */

	me->p_team = (tno < 4) ? (1 << tno) : 0;
	config();

	me->p_pos = -1;
	me->p_flags |= PFROBOT;	/* Mark as a robot */
	me->p_flags |= PFSNAKE;	/* Mark as snake */
	if (berserk)
	    me->p_hostile = FED | ROM | ORI | KLI;
    }

    r_signal(SIGALRM, snakemove);

    {
	double  period = 1 / frequency;
	udt.it_interval.tv_sec = period;	/* get the whole part */
	period -= udt.it_interval.tv_sec;	/* get the fractional part */
	udt.it_interval.tv_usec = 1e6 * period;
	udt.it_value.tv_sec = 1;
	udt.it_value.tv_usec = 0;
    }

    if (setitimer(ITIMER_REAL, &udt, 0) < 0) {
	perror("setitimer");
	for (i = 0; i < num_perfs; i++) {
	    if (perfs[i])
		perfs[i]->p_status = PFREE;
	}
	exit(1);
    }
#ifndef SVR4
    /* allows robots to be forked by the daemon -- Evil ultrix bullshit */
    sigsetmask(0);
#endif				/* SVR4 */

    /* NOTE: snakes do not become alive. */

    while (1) {
	pause();
    }
}

int findtestslot()
{
    register int i;

    for (i = MAXPLAYER - configvals->ntesters; i < MAXPLAYER; i++) {
	if (players[i].p_status == PFREE) {	/* We have a free slot */
	    players[i].p_status = POUTFIT;	/* possible race code */
	    break;
	}
    }
    if (i == MAXPLAYER) {
	return -1;		/* no room in tester slots */
    }
    memset(&players[i].p_stats, 0, sizeof(struct stats));
    players[i].p_stats.st_tticks = 1;
    return (i);
}

int findrslot()
{
    register int i;

    /* look for tester slot first */
    i = findtestslot();
    if (i > -1)
	return i;

    for (i = 0; i < MAXPLAYER; i++) {
	if (players[i].p_status == PFREE) {	/* We have a free slot */
	    players[i].p_status = POUTFIT;	/* possible race code */
	    break;
	}
    }
    if ((i == MAXPLAYER) || (i == -1)) {
	if (debug) {
	    fprintf(stderr, "No more room in game\n");
	}
	return -1;
    }
    memset(&players[i].p_stats, 0, sizeof(struct stats));
    players[i].p_stats.st_tticks = 1;
    return (i);
}

void 
warning(mess)
    char   *mess;
{
    if (debug)
	fprintf(stderr, "warning: %s\n", mess);
}

int config()
{
    /* mostly not used */
    myship->s_phaser.cost = 0;
    myship->s_torp.cost = 0;
    myship->s_cloakcost = 0;
/*    myship->s_torp.fuse = MAX_SHORT;*/
    myship->s_torp.fuse = 32767;
    myship->s_torp.damage = 10;
    myship->s_plasma.damage = 50;
    myship->s_plasma.cost = 0;
    myship->s_plasma.aux = 0;
    myship->s_plasma.speed = 10;
/*    myship->s_plasma.fuse = MAX_SHORT;*/
    myship->s_torp.fuse = 32767;
    myship->s_wpncoolrate = 100;
    myship->s_egncoolrate = 100;
    return 1;
}

/*---------------------[ prints the usage of snake ]---------------------*/

void printsnakeUsage()
{
    printf("Usage: snake [options]\n");
    printf("Options:\n\
  -u -- this usage message\n\
  -p -- patrol\n\
  -s -- noSmush (?)\n\
  -b -- berserk\n\
  -d -- debug\n\
  -t -- target <player number>\n\
  -T -- team [frkoi]\n\
  -l -- length (in torps)\n\
  -f -- frequency\n\
  -g -- guardian:  -g <planet1> <planet2> (must be 2 planets listed,\n\
                      by number).\n");
}

/*--------------------------[ printsnakeUsage ]--------------------------*/

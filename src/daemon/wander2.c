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

#define NEED_TIME 

#include "config.h"

#include <malloc.h>
#include <sys/types.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/file.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <pwd.h>
#include <ctype.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

extern void (*r_signal()) ();

extern struct planet *planets;

#define COS(x) ((x) >= 0.0 ? Cosine[(int)(x)] : Cosine[(int)(-(x))])
#define SIN(x) ((x) >= 0.0 ? Sine[(int)(x)] : -Sine[(int)(-(x))])

#define PUPDATE 999999

int     pl_home[4];
int     pl_core[4][10];
int     pl_dist[4][10];
double  increment = 0.016;
double  incrementrecip = 62.5;
float  *Cosine, *Sine;

double  dpre;
double  fpre;
double  pi = 3.1415926;

int     planeti, planetj;

/* call only once */


void
pinit()
{
    double  dx, dy;
    int     i, j;

    int     pre;

    void     pmove();

    pre = 3.5 / increment;
    dpre = (double) pre;

    Cosine = (float *) calloc(sizeof(float), pre);
    Sine = (float *) calloc(sizeof(float), pre);
    for (i = 0; i < pre; i++) {
	Cosine[i] = cos((double) i * increment);
	Sine[i] = sin((double) i * increment);
    }

/*    openmem();*/

    pl_home[0] = 0;
    pl_core[0][0] = 5;
    pl_core[0][1] = 7;
    pl_core[0][2] = 8;
    pl_core[0][3] = 9;
    pl_home[1] = 10;
    pl_core[1][0] = 12;
    pl_core[1][1] = 15;
    pl_core[1][2] = 16;
    pl_core[1][3] = 19;
    pl_home[2] = 20;
    pl_core[2][0] = 24;
    pl_core[2][1] = 26;
    pl_core[2][2] = 29;
    pl_core[2][3] = 25;
    pl_home[3] = 30;
    pl_core[3][0] = 34;
    pl_core[3][1] = 37;
    pl_core[3][2] = 38;
    pl_core[3][3] = 39;

    for (i = 0; i < 4; i++) {
	for (j = 0; j < 4; j++) {
	    dx = (double) (planets[pl_core[i][j]].pl_x - planets[pl_home[i]].pl_x);
	    dy = (double) (planets[pl_home[i]].pl_y - planets[pl_core[i][j]].pl_y);
	    pl_dist[i][j] = isqrt(dx * dx + dy * dy);
/*	    pl_dist[i][j] = 12000;*/
	}
    }

    planeti = 0;
    planetj = 0;

#if 0
    {
	static struct itimerval udt;
	r_signal(SIGALRM, pmove);

	udt.it_interval.tv_sec = 0;
	udt.it_interval.tv_usec = PUPDATE;
	udt.it_value.tv_sec = 0;
	udt.it_value.tv_usec = PUPDATE;
	(void) setitimer(ITIMER_REAL, &udt, (struct itimerval *) 0);

	while (1)
	    pause();
    }
#endif
}

/* call once per second */
void
pmove()
{
    int     i, j;
    double  dir;
    double  dx, dy;

/*    for (i = 0; i < 4; i++) {
	for (j = 0; j < 4; j++) { */
    i = planeti;
    j = planetj;
    planetj = (planetj + 1) % 4;
    if (planetj == 0)
	planeti = (planeti + 1) % 4;

    dir = atan2((double) (planets[pl_core[i][j]].pl_y - planets[pl_home[i]].pl_y),
	 (double) (planets[pl_core[i][j]].pl_x - planets[pl_home[i]].pl_x));
    if (dir > pi)
	dir = dir - 2.0 * pi;
    if (dir >= 0.0)
	dir = (dir * incrementrecip + 1.5);
    else
	dir = (dir * incrementrecip + 0.5);


    planets[pl_core[i][j]].pl_x =
	planets[pl_home[i]].pl_x +
	(int) (pl_dist[i][j] * (dx = COS(dir)));
    planets[pl_core[i][j]].pl_y =
	planets[pl_home[i]].pl_y +
	(int) (pl_dist[i][j] * (dy = SIN(dir)));
/*	    dir = atan2((double) (planets[pl_core[i][j]].pl_y
				  - planets[pl_home[i]].pl_y),
			(double) (planets[pl_core[i][j]].pl_x
				  - planets[pl_home[i]].pl_x));*/

/*	    planets[pl_core[i][j]].pl_flags |= PLREDRAW;*/
}

/*
    }
}
*/

/*
usage(string)
char *string;
{
    printf("Usage: %s [-dnnn]\n", string);
    printf("  -dnnn  delay nnn 1/10 seconds between frames\n");
}

openmem()
{
    extern int errno;
    int	shmemKey = PKEY;
    int	shmid;
    struct memory	*sharedMemory;

    errno = 0;
    shmid = shmget(shmemKey, 0, 0);
    if (shmid < 0) {
	if (errno != ENOENT) {
	    fprintf(stderr, "shmget\n");
	    exit(1);
	}
	shmid = shmget(shmemKey, 0, 0);
	if (shmid < 0) {
	    fprintf(stderr, "Daemon not running\n");
	    exit (1);
	}
    }
    sharedMemory = (struct memory *) shmat(shmid, 0, 0);
    if (sharedMemory == (struct memory *) -1) {
	fprintf(stderr, "shared memory\n");
	exit (1);
    }
    players = sharedMemory->players;
    torps = sharedMemory->torps;
    plasmatorps = sharedMemory->plasmatorps;
    planets = sharedMemory->planets;
    phasers = sharedMemory->phasers;
    mctl = sharedMemory->mctl;
    messages = sharedMemory->messages;
    teams = sharedMemory->teams;
    status = sharedMemory->status;
}
*/

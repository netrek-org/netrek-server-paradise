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
#include <math.h>

#include "defs.h"
#include "struct.h"
#include "data.h"
#include "shmem.h"
#include "planets.h"
#include "imath.h"

#define SYSWIDTH	(GWIDTH/5.9)	/* width of a system */

#define SYSTEMS		7	/* number of planetary systems */

/*atmosphere chances form a cascade win rand()%100*/
#define PATMOS1		40	/* chance for normal atmosphere */
#define PATMOS2		70	/* chance for thin atmosphere */
#define PATMOS3		90	/* chance for slightly toxic stmos */
#define PPOISON		100	/* chance for poison atmos */

/*defines that deal with planets resources and types*/
#define NMETAL		10	/* number of metal deposits */
#define NDILYTH		8	/* number of dilythium deposits */
#define NARABLE		12	/* number of arable land planets */
 /* defines that deal with star placement */

#define GW	((float)GWIDTH)	/* size of galaxy in floating point */
#define	STARBORD	((GW/5.2)*1.3)
#define TEAMBORD	((GW/5.2)/1.1)
#define STARMIN		(GW/5.0)/* min dist between stars */
#define STARMIN2	(STARMIN*STARMIN)	/* min star dist squared */
#define STARMAX		GW
#define STARMAX2	(GW*GW)
#define TEAMMIN		(GW/2.8)/* min dist between team stars */
#define TEAMMIN2	(TEAMMIN*TEAMMIN)
#define TEAMMAX		(GW/1.4)/* max dist between team stars */
#define TEAMMAX2	(TEAMMAX*TEAMMAX)

 /* defines that deal with systems and their planets */
#define SYSADD		2	/* number possible above min number */
#define SYSBORD		(4000.0 + (float)GWIDTH/200)	/* min distance from
							   border wall */
#define INDBORD		(GW * 0.1)
#define SYSMIN		(6000.0 + (float)GWIDTH/200)	/* min distance between
							   objects */
#define SYSMIN2		(SYSMIN*SYSMIN)	/* square of sysmin distance */
#define SYSPLMIN	5	/* min number of planets for system */
#define SYSPLADD	0	/* number of possible extra planets */
#define MINARMY 8		/* min numer of armies on a planet */
#define MAXARMY 15		/* max number of armies on a planet */

 /* other defines */
#define HOMEARMIES 30		/* number of armies on home planets */
#define COLONYARMIES 10		/* number of armies for colony planet */


 /* defines dealing with growth timers */
#define PLGFUEL		configvals->plgrow.fuel	/* time for growth of fuel
						   depot */
#define PLGAGRI		configvals->plgrow.agri	/* time for growth of agri */
#define PLGREPAIR	configvals->plgrow.repair	/* time for growth of
							   repair */
#define PLGSHIP		configvals->plgrow.shipyard	/* time for growth of
							   shipyard */


#if 0
/*-------------------------------GENRESOURCES----------------------------*/
/*  This function goes through the planets structure and determines what
kind of atmosphere and what kind of surface the planets have.  It generates
the stars that will be used as system centers ans then places atmospheres
on the other planets.  It then distributes the resources on the planet
surfaces.  */

static void 
genresources()
{
    int     i;			/* looping vars */
    int     t;			/* temp var */

    for (i = 0; i < SYSTEMS; i++)	/* first planets are stars */
	planets[i].pl_flags |= PLSTAR;	/* or in star flag */
    for (i = SYSTEMS; i < NUMPLANETS; i++) {	/* generate atmospheres */
	t = lrand48() % 100;	/* random # 0-99 */
	if (t < PATMOS1)	/* is it atmosphere type 1 */
	    planets[i].pl_flags |= PLATYPE1;
	else if (t < PATMOS2)	/* is it atmosphere type 2 */
	    planets[i].pl_flags |= PLATYPE2;
	else if (t < PATMOS3)	/* is it atmosphere type 3 */
	    planets[i].pl_flags |= PLATYPE3;
	else if (t < PPOISON)	/* is it poison atmosphere */
	    planets[i].pl_flags |= PLPOISON;
    }
    for (i = 0; i < NMETAL; i++) {	/* place the metal deposits */
	t = lrand48() % (NUMPLANETS - SYSTEMS) + SYSTEMS;	/* random planet */
	planets[t].pl_flags |= PLMETAL;	/* OR in the metal flag */
	if (!configvals->resource_bombing)
	    planets[t].pl_flags |= PLREPAIR;
    }
    for (i = 0; i < NDILYTH; i++) {	/* place the metal deposits */
	t = lrand48() % (NUMPLANETS - SYSTEMS) + SYSTEMS;	/* random planet */
	planets[t].pl_flags |= PLDILYTH;	/* OR in the dilyth flag */
	planets[t].pl_flags &= ~(PLATMASK | PLARABLE);	/* zero off previous
							   atmos */
	planets[t].pl_flags |= PLPOISON;	/* dilyth poisons atmosphere */
	if (!configvals->resource_bombing)
	    planets[t].pl_flags |= PLFUEL;
    }
    for (i = 0; i < NARABLE; i++) {	/* place the metal deposits */
	t = lrand48() % (NUMPLANETS - SYSTEMS) + SYSTEMS;	/* random planet */
	planets[t].pl_flags |= PLARABLE | PLATYPE1;	/* OR in the arable flag */
	if (!configvals->resource_bombing)
	    planets[t].pl_flags |= PLAGRI;
    }
}
#endif


#if 0

/*--------------------------------PLACESTARS------------------------------*/
/*  This function places each system's star.  The stars are expected to be
in the first SYSTEMS number of planets.  The coordinates of the stars are
placed in the space grid.  */

static int 
placestars()
{
    int     i, j;		/* looping vars */
    double  x=0, y=0;		/* to hold star coordinates */
    int     done;		/* flag to indicate done */
    double  dx, dy;		/* delta x and y's */
    int     attempts;
    double  min, max, dist, bord, nbwidth;
    double  xoff, yoff;

    for (i = 0; i < SYSTEMS; i++) {	/* star for each system */
	if (i < 4) {
	    min = TEAMMIN2;
	    max = TEAMMAX2;
	    bord = TEAMBORD;
	}
	else {
	    min = STARMIN2;
	    max = STARMAX2;
	    bord = STARBORD;
	}
	nbwidth = GW - 2 * bord;
	x = drand48() * nbwidth + bord;	/* pick intial coords */
	y = drand48() * nbwidth + bord;
	xoff = 3574.0 - bord;
	yoff = 1034.0 - bord;
	attempts = 0;
	do {			/* do until location found */
	    attempts++;
	    done = 0;		/* not done yet */
	    x = bord + fmod(x + xoff, nbwidth);	/* offset coords a little */
	    y = bord + fmod(y + yoff, nbwidth);	/* every loop */
#if 0
	    if ((x > GW - bord) || (x < bord)
		|| (y < bord) || (y > GW - bord))
		continue;	/* too close to border? */
#endif
	    done = 1;		/* assume valid cord found */
	    for (j = 0; j < i; j++) {	/* go through previous stars */
		dx = fabs(x - (double) planets[j].pl_x);
		dy = fabs(y - (double) planets[j].pl_y);
		dist = dx * dx + dy * dy;
		if (dist < min || dist > max)	/* if too close or too far
						   then */
		    done = 0;	/* we must get another coord */
	    }
	} while (!done && attempts < 1000);	/* do until location found */

	if (!done)
	    return 0;

	planets[i].pl_owner = NOBODY;	/* no team owns a star */
	planets[i].pl_flags |= PLSTAR;	/* mark planet as a star */
	move_planet(i, (int) x, (int) y, 0);
	planets[i].pl_system = i + 1;	/* mark the sytem number */
	planets[i].pl_hinfo = ALLTEAM;	/* all teams know its a star */
	for (j = 0; j < MAXTEAM + 1; j++) {	/* go put in info for teams */
	    planets[i].pl_tinfo[j].owner = NOBODY;	/* nobody owns it */
	    planets[i].pl_tinfo[j].armies = 0;
	    planets[i].pl_tinfo[j].flags = planets[i].pl_flags;
	}
    }
    return 1;
}

#endif


/*-----------------------------PLACESYSTEMS------------------------------*/
/*  This function places the planets in each star's system.  The function
will return the index of the first planet that was not placed in a system.
The coordinates of the planets are placed in the space grid.  */

static int 
placesystems()
{
    int     i, j, k;		/* looping vars */
    double  x=0, y=0;		/* to hold star coordinates */
    int     done;		/* flag to indicate done */
    double  dx, dy;		/* delta x and y's */
    int     n;			/* number of planet to place */
    int     np;			/* number of planets in system */
    int     attempts;

    n = SYSTEMS;		/* first planet to place */
    for (i = 0; i < SYSTEMS; i++) {	/* planets for each system */
	np = SYSPLMIN + lrand48() % (SYSPLADD + 1);	/* how many planets */
	for (k = 0; k < np; k++) {	/* go place the planets */
	    attempts = 0;
	    do {		/* do until location found */
		attempts++;
		done = 0;	/* not done yet */
		dx = (drand48() * SYSWIDTH - SYSWIDTH / 2.0);
		dy = (drand48() * SYSWIDTH - SYSWIDTH / 2.0);
		if (dx * dx + dy * dy > (SYSWIDTH / 2.0) * (SYSWIDTH / 2.0))
		    continue;	/* might orbit its way out of the galaxy */
		x = planets[i].pl_x + dx;
		y = planets[i].pl_y + dy;
		if ((x > GW - SYSBORD) || (x < SYSBORD)
		    || (y < SYSBORD) || (y > GW - SYSBORD))
		    continue;	/* too close to border? */

		done = 1;	/* assume valid coord found */
		for (j = 0; j < n; j++) {	/* go through previous
						   planets */
		    dx = fabs(x - (double) planets[j].pl_x);
		    dy = fabs(y - (double) planets[j].pl_y);
		    if (dx * dx + dy * dy < SYSMIN2) {	/* if too close to
							   another star */
			done = 0;	/* we must get another coord */
		    }
		    if( ihypot( (int)dx, (int)dy) < 3000 )
			done = 0;
		}
	    } while (!done && attempts < 200);	/* do until location found */

	    if (!done)
		return 0;	/* universe too crowded, try again */

	    move_planet(n, (int) x, (int) y, 0);
	    planets[n].pl_system = i + 1;	/* mark the sytem number */
	    planets[n].pl_armies = MINARMY + lrand48() % (MAXARMY - MINARMY);
	    n++;		/* go to next planet */
	}
    }
    return (n);			/* return index of next planet */
}




/*-----------------------------PLACEINDEP------------------------------*/
/*  This function places idependent planets that are not in a system.
They can appear anywhere in the galaxy as long as they are not too close
to another planet.  The coords are put in the space grid.  */

static int 
placeindep(n)
    int     n;
 /* number of planet to start with */
{
    int     i, j;		/* looping vars */
    double  x, y;		/* to hold star coordinates */
    int     done;		/* flag to indicate done */
    double  dx, dy;		/* delta x and y's */
    int     attempts;

    for (i = n; i < (NUMPLANETS - (WORMPAIRS*2)); i++) {       
       /* go through rest of planets */
	x = drand48() * (GW - 2 * INDBORD) + INDBORD;	/* pick intial coords */
	y = drand48() * (GW - 2 * INDBORD) + INDBORD;
	attempts = 0;
	do {			/* do until location found */
	    attempts++;
	    done = 0;		/* not done yet */
	    x = INDBORD + fmod(x + (3574.0 - INDBORD), GW - 2 * INDBORD);	/* offset coords a
										   little */
	    y = INDBORD + fmod(y + (1034.0 - INDBORD), GW - 2 * INDBORD);	/* every loop */
#if 0
	    if ((x > GW - INDBORD) || (x < INDBORD)
		|| (y < INDBORD) || (y > GW - INDBORD))
		continue;	/* too close to border? */
#endif
	    done = 1;		/* assume valid coord */
	    for (j = 0; j < n; j++) {	/* go through previous planets */
		dx = fabs(x - (double) planets[j].pl_x);
		dy = fabs(y - (double) planets[j].pl_y);
		if (dx * dx + dy * dy < SYSMIN2) {	/* if planet to close */
		    done = 0;	/* we must get another coord */
		}
	    }
	} while (!done && attempts < 200);	/* do until location found */

	if (!done)
	    return 0;

	move_planet(n, (int) x, (int) y, 0);
	planets[n].pl_system = 0;	/* mark the no sytem */
	planets[n].pl_armies = MINARMY + lrand48() % (MAXARMY - MINARMY);
	n++;			/* go to next planet */
    }
    for (i = n; i < NUMPLANETS; i++) /* now place wormholes */ {
	x = drand48() * GW;  /* pick intial coords */
	y = drand48() * GW;
	attempts = 0;
	do {			/* do until location found */
	    attempts++;
	    done = 0;		/* not done yet */
	    x = fmod(x + 3574.0, GW);	/* offset coords a little */
	    y = fmod(y + 1034.0, GW);	/* every loop */
#if 0
	    if ((x > GW) || (y > GW))
		continue;	/* too close to border? */
#endif
	    done = 1;		/* assume valid coord */
	    for (j = 0; j < n; j++) {	/* go through previous planets */
		dx = fabs(x - (double) planets[j].pl_x);
		dy = fabs(y - (double) planets[j].pl_y);
		if (dx * dx + dy * dy < SYSMIN2) {	/* if planet to close */
		    done = 0;	/* we must get another coord */
		}
	    }
	} while (!done && attempts < 200);	/* do until location found */

	if (!done)
	    return 0;

	move_planet(n, (int) x, (int) y, 0);
	planets[n].pl_system = 0;	/* mark the no system */
	planets[n].pl_flags |= PLWHOLE; /* mark the planet as a wormhole */
	/* the armies in a wormhole is the other wormhole's x coord */
	/* the radius is the other wormhole's y coord*/
	if (NUMPLANETS%2) {
	   if (!(n%2)) {
	      planets[n].pl_armies = planets[n-1].pl_x;
	      planets[n].pl_radius = planets[n-1].pl_y;
	      planets[n-1].pl_armies = planets[n].pl_x;
	      planets[n-1].pl_radius = planets[n].pl_y;
	   }
	} else {
	   if (n%2) {
	      planets[n].pl_armies = planets[n-1].pl_x;
	      planets[n].pl_radius = planets[n-1].pl_y;
	      planets[n-1].pl_armies = planets[n].pl_x;
	      planets[n-1].pl_radius = planets[n].pl_y;
	   }
	}
	planets[i].pl_owner = NOBODY;	/* no team owns a star */
	planets[i].pl_hinfo = ALLTEAM;	/* all teams know its a star */
	for (j = 0; j < MAXTEAM + 1; j++) {	/* go put in info for teams */
	    planets[i].pl_tinfo[j].owner = NOBODY;	/* nobody owns it */
	    planets[i].pl_tinfo[j].armies = 0;
	    planets[i].pl_tinfo[j].flags = planets[i].pl_flags;
	}
	n++;			/* go to next planet */       
    }
    return 1;
}




/*---------------------------------PLACERACES------------------------------*/
/*  This function places the races in the galaxy.  Each race is placed in
a different system.  The race is given a home world with an Agri and Ship-
yard on it and HOMEARMIES.  They are also given a conoly planet with
dilythium deposits and COLONYARMIES on it.  */

static void 
placeraces()
{
    int     i, j, k;		/* looping vars */
    int     p;			/* to hold planet for race */

    for (i = 0; i < 4; i++) {	/* go through races */
	/* find home planet */
	p = lrand48() % NUMPLANETS;	/* pick random planet */
	while ((planets[p].pl_system != i + 1)
	       || (PL_TYPE(planets[p]) == PLSTAR)
	       || (planets[p].pl_owner != NOBODY))
	    p = (p + 1) % NUMPLANETS;	/* go on to next planet */
	planets[p].pl_flags &= ~PLSURMASK;	/* make sure no dilithium */
	planets[p].pl_flags |= (PLMETAL | PLARABLE);	/* metal and arable */
	planets[p].pl_flags |= PLATYPE1;	/* good atmosphere */
	planets[p].pl_flags |= (PLAGRI | PLSHIPYARD | PLREPAIR);
	planets[p].pl_tagri = PLGAGRI;	/* set timers for resources */
	planets[p].pl_tshiprepair = PLGSHIP;
	planets[p].pl_owner = 1 << i;	/* make race the owner */
#if 0				/* home planets do not have traditional names */
	strcpy(planets[p].pl_name, homenames[1 << i]);	/* set name and length */
	planets[p].pl_namelen = strlen(homenames[1 << i]);
#endif
	planets[p].pl_armies = HOMEARMIES;	/* set the armies */
	planets[p].pl_hinfo = 1 << i;	/* race has info on planet */
	planets[p].pl_tinfo[1 << i].owner = 1 << i;	/* know about owner */
	planets[p].pl_tinfo[1 << i].armies = planets[p].pl_armies;
	planets[p].pl_tinfo[1 << i].flags = planets[p].pl_flags;
	/* find colony planet */
	p = lrand48() % NUMPLANETS;	/* pick random planet */
	while ((planets[p].pl_system != i + 1)
	       || (PL_TYPE(planets[p]) == PLSTAR)
	       || (planets[p].pl_owner != NOBODY))
	    p = (p + 1) % NUMPLANETS;	/* go on to next planet */
	planets[p].pl_flags |= PLFUEL;	/* make fuel depot */
	planets[p].pl_tfuel = PLGFUEL;	/* set timer for fuel depot */
	planets[p].pl_flags &= ~PLATMASK;	/* take off previous atmos */
	planets[p].pl_flags |= PLPOISON;	/* poison atmosphere */
	planets[p].pl_flags |= PLDILYTH;	/* dilythium deposits */
	planets[p].pl_owner = 1 << i;	/* make race the owner */
	planets[p].pl_armies = COLONYARMIES;	/* set the armies */
	planets[p].pl_hinfo = 1 << i;	/* race knows about */
	planets[p].pl_tinfo[1 << i].owner = 1 << i;	/* know about owner */
	planets[p].pl_tinfo[1 << i].armies = planets[p].pl_armies;
	planets[p].pl_tinfo[1 << i].flags = planets[p].pl_flags;
	for (j = 0; j < NUMPLANETS; j++) {
	    if ((planets[j].pl_system == i + 1) && (PL_TYPE(planets[j]) != PLSTAR)) {
#ifdef LEAGUE_SUPPORT
		for (k = (status2->league ? 0 : i);
		     k < (status2->league ? 4 : i + 1);
		     k++)
#else
		  k=i;
#endif
		  {
		    planets[j].pl_owner = 1 << i;
		    planets[j].pl_hinfo =
#ifdef LEAGUE_SUPPORT
		      status2->league ? (1 << 4) - 1 :
#endif
			(1 << i);
		    planets[j].pl_tinfo[1 << k].owner = 1 << i;
		    planets[j].pl_tinfo[1 << k].armies = planets[j].pl_armies;
		    planets[j].pl_tinfo[1 << k].flags = planets[j].pl_flags;
		}
	    }
	}
    }
}

/* 
 * Generate a complete galaxy, deepspace style.  We use a 125k^2 grid with
 * lots of planets for lots of fun.  We're assuming a no-warp environment.
 */

void 
gen_galaxy_7()
{
    int     t;

    GWIDTH = 125000;
    NUMPLANETS = 60-WORMPAIRS*2;
    configvals->warpdrive = 0;

    while (1) {
	initplanets();		/* initialize planet structures */

	/* place the resources */
	zero_plflags(planets, NUMPLANETS);
	randomize_atmospheres(planets+SYSTEMS, NUMPLANETS-SYSTEMS,
			      PATMOS1,PATMOS2,PATMOS3,PPOISON);
	randomize_resources(planets+SYSTEMS, NUMPLANETS-SYSTEMS,
			    NMETAL, NDILYTH, NARABLE);

	/* place system centers */
	t = place_stars(planets, 4,
			(int)TEAMBORD, (int)TEAMMIN, (int)TEAMMAX,
			(struct planet*)0, 0)
	  && place_stars(planets+4, SYSTEMS-4,
			 (int)STARBORD, (int)STARMIN, (int) STARMAX,
			 planets, 4);

	if (!t)
	    continue;
	t = placesystems();	/* place planets in systems */
	if (!t)
	    continue;
	t = placeindep(t);	/* place independent planets */
	if (t)
	    break;		/* success */
    }
    if (configvals->justify_galaxy)
       justify_galaxy(SYSTEMS);
    placeraces();		/* place home planets for races */

}

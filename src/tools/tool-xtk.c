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
#include "data.h"
#include "shmem.h"
#include "getship.h"
#include "tool-util.h"

char    teamlet[] = {'I', 'F', 'R', 'X', 'K', 'X', 'X', 'X', 'O'};
char   *names[] = {"Neutral", "Fed", "Rom", "", "Kli", "", "", "", "Ori"};

void Usage(char *myname);
void pmessage();

int main(argc, argv)
    int     argc;
    char  **argv;
{
    int     i;
    int     pno;
    char    buf[1000];
    char   *myname;
    int     c;
    struct player	*victim;

    myname = *argv;

    if (argc < 2)
	Usage(myname);

    if (argv[1][1] != 0)
	Usage(myname);
    pno = letter_to_pnum(argv[1][0]);
    if (pno < 0 || pno >= MAXPLAYER) {
	fprintf(stderr, "player number out of bounds (%d, %d)\n",
		pno, MAXPLAYER);
	exit(1);
    }

    openmem(0);

    victim = &players[pno];

    if (victim->p_status != PALIVE) {
	if (argc > 2 && strcmp(argv[2], "F") == 0) {
            memset(victim, 0, sizeof(struct player));
/*	    bzero(victim, sizeof(struct player));	/* confusion 8/5/91 TC */
	    /* victim->p_status = PFREE; */
	    exit(0);
	}
	printf("Slot is not alive.\n");
	exit(1);
    }
    if (argc <= 2) {
	sprintf(buf, "%s (%2s) was utterly obliterated.",
		victim->p_name, twoletters(victim));
/*		victim->p_name, victim->p_mapchars); */
	victim->p_ship.s_type = STARBASE;
	victim->p_whydead = KPROVIDENCE;
	victim->p_explode = 10;
	victim->p_status = 3;
	victim->p_whodead = 0;
	pmessage(buf, 0, MALL);
	exit(0);
    }
    switch (*argv[2]) {
    case 'e':
	sprintf(buf, "%s (%2s) has been ejected from the game.",
		victim->p_name, twoletters(victim));
	victim->p_whydead = KQUIT;
	victim->p_explode = 10;
	victim->p_status = 3;
	victim->p_whodead = 0;
	pmessage(buf, 0, MALL);
	break;
    case 's':
	{
	    int     i;
	    for (i = 0; i < NUM_TYPES; i++) {
		if (argv[2][1] == shipvals[i].s_letter)
		    break;
	    }
	    if (i >= NUM_TYPES) {
		fprintf(stderr, "Unknown ship type %c.\n", argv[2][1]);
		exit(1);
	    }
	    else {
		get_ship_for_player(victim, i);
	    }
	}

	victim->p_damage = 0;
	victim->p_shield = victim->p_ship.s_maxshield;
	victim->p_wtemp = 0;
	victim->p_etemp = 0;
	victim->p_fuel = victim->p_ship.s_maxfuel;
	break;
#if 0
    case 't':
	switch (argv[2][1]) {
	case 'f':
	    victim->p_x = planets[0].pl_x;
	    victim->p_y = planets[0].pl_y;
	    break;
	case 'r':
	    victim->p_x = planets[10].pl_x;
	    victim->p_y = planets[10].pl_y;
	    break;
	case 'k':
	    victim->p_x = planets[20].pl_x;
	    victim->p_y = planets[20].pl_y;
	    break;
	case 'o':
	    victim->p_x = planets[30].pl_x;
	    victim->p_y = planets[30].pl_y;
	    break;
	case 'c':
	    victim->p_x = GWIDTH / 2;
	    victim->p_y = GWIDTH / 2;
	    break;
	default:
	    printf("Valid teleports: frkoc.\n");
	    exit(1);
	}
	break;
#endif
#if 0
    case 'S':			/* super ship */
	victim->p_ship.s_maxshield = 750;
	victim->p_shield = 750;
	victim->p_ship.s_maxdamage = 750;
	victim->p_ship.s_maxegntemp = 5000;
	break;
#endif
    case 'T':{
	    int     team;
	    team = letter_to_team(argv[2][1]);
	    if (team < 0) {
		fprintf(stderr, "Invalid team letter `%c', choose one of [frkoi]\n", argv[2][1]);
		exit(1);
	    }
	    victim->p_team = team;
	    victim->p_hostile &= ~team;
	    victim->p_swar &= ~team;
	    sprintf(buf, "%2s has been changed to a %s.",
		    twoletters(victim), names[team]);
	    pmessage(buf, 0, MALL);
#if 0
	    sprintf(buf, "%c%c", teamlet[victim->p_team],
		    shipnos[pno]);
	    strncpy(victim->p_mapchars, buf, 2);
#endif
	}
	break;
    case 'D':			/* demote */
	--victim->p_stats.st_rank;
	sprintf(buf, "%2s was (temporarily) demoted for \
		rank normalization purposes.", twoletters(victim));
	pmessage(buf, 0, MALL);
	break;
    case 'P':
	++victim->p_stats.st_rank;
	break;
    case 'k':
	victim->p_kills += 1.0;
	break;
    case 'h':
	victim->p_shield = 0;
	victim->p_damage = victim->p_ship.s_maxdamage / 2;
	break;
    case 'a':
	victim->p_armies += 6;
	break;
    case 'C':
	teams[victim->p_team].s_surrender = 6;
	break;
    case 'L':
	victim->p_stats.st_sblosses--;
	break;
    case 'R':
	if (victim->p_flags & PFROBOT) {
	    victim->p_ship.s_type = STARBASE;
	    victim->p_whydead = KPROVIDENCE;
	    victim->p_explode = 10;
	    victim->p_status = 3;
	    victim->p_whodead = 0;
	}
	break;
#if 0
    case 'p':			/* puck? */
	victim->p_ship.s_tractstr = 1;
	victim->p_ship.s_torpdamage = -1;
	victim->p_ship.s_plasmadamage = -1;
	victim->p_ship.s_phaserdamage = -1;
	victim->p_hostile = 0;
	victim->p_swar = 0;
	victim->p_team = 0;	/* indep */
	victim->p_ship.s_type = STARBASE;
	victim->p_ship.s_mass = 200;
	victim->p_ship.s_repair = 30000;

	victim->p_ship.s_maxspeed = 0;
#endif
    default:
	Usage(myname);
    }				/* end switch */
    return 0;
}

void Usage(char *myname)
{
    printf("-- NetrekII (Paradise), %s --\n", PARAVERS);
    printf("\
\n\t%'s [0-9a-j] <mode><mode option>'\n\
\n\
Where <mode> is one of :\n\
      e(ject from game)             (simulates self-destruct)\n\
      s(hip class change)[abcdosA]  (A = ATT)\n\
      T(eam change)[frko]           (no team == independent)\n\
      D(emote)                      (-1 to rank)\n\
      P(romote)                     (+1 to rank)\n\
      F(ree slot)                   (bypasses 6 minute ghostbuster timeout)\n\
      k(ills increment)             (+1 kill)\n\
      h(arm)                        (no shields, 50%% damage)\n\
      a(rmies increment)            (+6 armies)\n\
      C(lock, surrender -- set it)  (to 6 minutes (debugging aid))\n\
      L(oss adjust, SB (-1))        (in case you toast an SB accidentally)\n\
      R(obot obliterate)            (like obliterate, but only for robots)\n\
      (no mode == obliterate)\n\
", myname);
    exit(1);
}


#if 0
/* stuff copied from other files: */

char   *
twoletters(pl)
    struct player *pl;
/* calculate the two letters that form the players designation (e.g. R4) */
{
#define RINGSIZE MAXPLAYER+3
    static char buf[RINGSIZE][3];	/* ring of buffers so that this */
    static int idx;		/* proc can be called several times before
				   the results are used */
    if (idx >= RINGSIZE)
	idx = 0;
    buf[idx][0] = teams[pl->p_team].letter;
    buf[idx][1] = shipnos[pl->p_no];
    buf[idx][2] = 0;
    return buf[idx++];
}
#endif


/*------------------------------PMESSAGE----------------------------------*/
/*  This function sens a message to the message board.  It places the message
in the next position of the array of messages.  The message will ahve a
header attached to the front of it.  */

void 
pmessage(str, recip, group)
    char   *str;		/* the message */
    int     recip;		/* who is the recipient */
    int     group;		/* group sent to and other flags */
{
    struct message *cur;	/* to pnt to where to put message */
    int     mesgnum;		/* to hold index into array of messgs */

    if ((mesgnum = ++(mctl->mc_current)) >= MAXMESSAGE) {
	mctl->mc_current = 0;	/* move to next index in array and */
	mesgnum = 0;		/* warp around if necessart */
    }
    cur = &messages[mesgnum];	/* get addr of message struct */
    cur->m_no = mesgnum;	/* set the message number */
    cur->m_flags = group;	/* set the group and flags */
    cur->m_recpt = recip;	/* set the recipient */
    cur->m_from = 255;		/* message is from God */
    (void) sprintf(cur->m_data, "%s %s", "XTKILL:", str);
    cur->m_flags |= MVALID;	/* mark message as valid */
}

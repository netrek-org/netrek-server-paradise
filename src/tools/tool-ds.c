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

#include <stdlib.h>
#include "config.h"
#include "proto.h"
#include "tool-util.h"
#include "data.h"
#include "shmem.h"
#include "structdesc.h"

extern struct field_desc *ship_fields;

/* ----------------[ prototypes because I like main first ]---------------- */
void dump_ship_sysdef P((void));
void dump_ship_Ccode P((void));
void dump_ships_to_table P((void));
void describe_ship P((int ship));
void usage P((char *name));

/* --[ rather than duplicate it 3 times make the macro from hell (shrug) ]-- */
#define Print_value(place) { \
    switch (ship_fields[place].type) { \
        case FT_CHAR: \
            printf("%c", *(char *) temp); \
            break; \
        case FT_SHORT: \
            printf("%d", *(short *) temp); \
            break; \
        case FT_INT: \
            printf("%d", *(int *) temp); \
            break; \
        case FT_LONG: \
            printf("%ld", *(long *) temp); \
            break; \
        case FT_FLOAT: \
            printf("%g", *(float *) temp); \
            break; \
        case FT_STRING: \
            printf("%s", (char *) temp); \
            break; \
        case FT_LONGFLAGS: \
            { \
                int     zz = 0; \
                char  **names = (char **) ship_fields[place].aux; \
                long    flag = *(long *) temp; \
                int     first = 1; \
                for (zz = 0; names[zz]; zz++) { \
                    if (flag & (1 << zz)) { \
                        printf("%s%s", first ? "" : ",", names[zz]); \
                        first = 0; \
                    } \
                } \
            } \
            break; \
        default: \
            printf("unknown type"); \
            break; \
    } \
}

static char *shipTYPES[] = {
    "SCOUT",
    "DESTROYER",
    "CRUISER",
    "BATTLESHIP",
    "ASSAULT",
    "STARBASE",
    "ATT",
    "JUMPSHIP",
    "FRIGATE",
    "WARBASE",
    "LIGHTCRUISER",
    "CARRIER"
};

static struct nflags_desc_ {
    int     flag;
    char   *meaning;
} nflags_desc[] = {
    {SFNUNDOCKABLE,     "can not dock with another ship"},
    {SFNCANORBIT,       "can orbit hostile worlds"},
    {SFNCANWARP,        "has warp engines"},
    {SFNCANFUEL,        "can transfer fuel to docked ships"},
    {SFNCANREPAIR,      "can speed repair of docked ships"},
    {SFNCANREFIT,       "can let docked ships refit"},
    {SFNARMYNEEDKILL,   "needs kills to carry armies"},
    {SFNHASPHASERS,     "is armed with phasers"},
    {SFNPLASMASTYLE,    "360 arc of fire for plasmas"},
    {SFNPLASMAARMED,    "is armed with plasmas by default"},
    {SFNHASMISSILE,     "is armed with missiles by default"},
    {SFNHASFIGHTERS,    "has a fighter bay"},
    {0, 0}
};

/* ==============================[ Functions ]============================== */

int
main(int argc, char **argv)
{
    int     i, droutine = 0;
    char *name;

    name = *argv++;
    argc--;

    if (argc != 1)
        usage(name);

    while (*argv) {
        if (**argv == '-')
            ++*argv;
        else
            break;
        switch (**argv) {
            case 's':   /* sysdef */
                droutine = 1;
                break;
            case 'c':   /* C Code */
                droutine = 2;
                break;
            case 't':   /* table */
                droutine = 3;
                break;
            case 'v':   /* verbose */
                droutine = 4;
                break;
            default:
                printf("!  %s: Unknown option '-%c'\n", name, **argv);
                usage(name);
        }
    }

    /* start up a daemon if we need to */
    openmem(1, 0);

    /* do this with two switches because we don't want to fire up the daemon
       if we don't need to */
    switch (droutine) {
        case 1:                   /* Sysdef */
            dump_ship_sysdef();
            break;
        case 2:                   /* C Code */
            dump_ship_Ccode();
            break;
        case 3:                   /* Table */
            dump_ships_to_table();
            break;
        case 4: {                  /* Verbose */
            ++*argv;
            if (!**argv) {
                for (i = 0; i < NUM_TYPES; i++)
                    describe_ship(i);
            } else {
                describe_ship(atoi(*argv));
            }
            break;   /* for old times sake */
        } /* case 4 (Braces on this one because it looks nice, thats all */
    } /* switch */

    exit(0);
}

/* ------------------[ Print ship stats in sysdef format ]------------------ */
void
dump_ship_sysdef(void)
{
    int     j, i;

    for (i = 0; i < NUM_TYPES; i++) {
        struct ship *shp = &shipvals[i];
        printf("SHIP=%d\n", i);
        for (j = 0; ship_fields[j].name; j++) {
            void   *temp = ship_fields[j].offset + (char *) shp;

            printf("%c%c %-24s",
                   shp->s_desig1, shp->s_desig2, ship_fields[j].name);
            Print_value(j);
            printf("\n");
        }
        printf("end\n");
    }
}

/* ----------------[ Print ship stats in a C syntax format ]---------------- */
void
dump_ship_Ccode(void)
{
    int     j, i;

    for (i = 0; i < NUM_TYPES; i++) {
        struct ship *shp = &shipvals[i];
        printf("  /* comprehensive definition of %s */\n", shipTYPES[i]);
        for (j = 0; ship_fields[j].name; j++) {
            void   *temp = ship_fields[j].offset + (char *) shp;

            if (ship_fields[j].type == FT_STRING) {
                printf("  strcpy(shipvals[%s].s_%s, \"%s\")", shipTYPES[i],
                   ship_fields[j].name, (char *) temp);
            } else {
                printf("  shipvals[%s].s_%s = ",
                       shipTYPES[i], ship_fields[j].name);
                Print_value(j);
            }
            printf(";\n");
        }
        printf("\n");
    }
}

/* -----------------[ Print ship stats in a table format ]----------------- */
void
dump_ships_to_table(void)
{
    int j, i;

    /* we have to find the max element of the ship fields, this is the
       only way I know of so far (BG) */

    printf("Ship Statistics:\n");
    for (i=0; ship_fields[i].name; i++) {
        printf("%-13s ", ship_fields[i].name);
        for (j=0; j < NUM_TYPES; j++) {
            struct ship *shp = &shipvals[j];
            void   *temp = (ship_fields[i].offset + (char *) shp);

            /* we do this one differently so don't use Print_value() */
            switch (ship_fields[i].type) {
              case FT_CHAR:
                printf("%6c ", *(char *) temp);
                break;
              case FT_SHORT:
                printf("%6d ", *(short *) temp);
                break;
              case FT_INT:
                printf("%6d ", *(int *) temp);
                break;
              case FT_LONG:
                printf("%6ld ", *(long *) temp);
                break;
              case FT_FLOAT:
                printf("%6g ", *(float *) temp);
                break;
              case FT_STRING:
                printf("%6s ", (char *) temp);
                break;
              default:
                break;
            }
        }
        printf("\n");
    }
}


/* -------------------------[ Verbose description ]------------------------- */
void 
describe_ship(int s_no)
{
    struct ship *sp = &shipvals[s_no];
    int     i;

    printf("The %s\n", sp->s_name);
    for (i = 0; nflags_desc[i].flag; i++) {
        if ((sp->s_nflags & nflags_desc[i].flag) != nflags_desc[i].flag)
            continue;
        printf("\t%s\n", nflags_desc[i].meaning);
    }
}


/* ----------------------------[ Prints usage ]---------------------------- */
void
usage(char *name)
{
    int x;

    char errmsg[][255] = {
        "\n\t'%s <format option>'\n\n",
        "This tool will dump all ship values, configurable to 3 formats:\n",
        "\t-s     -- .sysdef Format\n",
        "\t-c     -- C code Format\n",
        "\t-t     -- Table Format (best printed with 8 point font)\n",
        "\t-v#    -- Verbose Format (optional ship number)\n",
        ""
    };

    printf("-- NetrekII (Paradise), %s --\n", PARAVERS);
    for (x=0; *errmsg[x] != NULL; x++)
        printf(errmsg[x], name);

    exit(1);
}

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

/*
// overhaul by Brandon Gillespie Sept 17 1994
*/

#include "config.h"

#include "defs.h"
#include "data.h"
#include "struct.h"
#include "shmem.h"

#define CNORMAL   "\33[37;0m"
#define CBOLD     "\33[1m"
#define CRED      "\33[31m"
#define CGREEN    "\33[32m"
#define CYELLOW   "\33[33m"
#define CBLUE     "\33[34m"
#define CMAGENTA  "\33[35m"
#define CCYAN     "\33[36m"

/* prototypes */
void       printmes(char mstr[80], int mflags, int mrecpt, int mfrom);
void       usage(char *me);
static int contains(char *str1, char *str2);

#define PRINT(__x) if (1) {\
        if (displaykills == 1) \
            printf("\n%s", __x); \
        else if (mflags != 169) /* MKILLA) */ \
            printf("\n%s", __x); \
        printf(CNORMAL); \
    }
/*
// Global Variables, bad, but this is an easier way to do options
// which are only set once
*/

int     displaykills = 1;
int     docolor = 0;
char   *myname;

int
main(int argc, char **argv)
{
    int     i;
    int     oldmctl;

    myname = *argv++;
    argc--;

    while (argc) {
        if (argv[0][0] == '-') {
            switch (argv[0][1]) {
                case 'k':
                    displaykills = 0;
                    break;
                case 'c':
                    docolor = 1;
                    break;
                default:
                    usage(argv[0]);
                    break;
            }
        }
        argv++;
        argc--;
    }

    openmem(0);

    oldmctl = mctl->mc_current;

    for (i = 0; i <= oldmctl; i++) {
        printmes(messages[i].m_data,
                 messages[i].m_flags,
                 messages[oldmctl].m_recpt,
                 messages[oldmctl].m_from);
    }

    fflush(stdout);

    for (;;) {
        sleep(1);
        while (oldmctl != mctl->mc_current) {
            oldmctl++;
            if (oldmctl == 50)
                oldmctl = 0;
            printmes(messages[oldmctl].m_data,
                     messages[oldmctl].m_flags,
                     messages[oldmctl].m_recpt,
                     messages[oldmctl].m_from);
            fflush(stdout);
        }
    }
}

static int
contains(char *str1, char *str2)
{
    char   *s;
    int     length;

    length = strlen(str2);
    for (s = str1; *s != 0; s++) {
    if (*s == *str2 && strncmp(s, str2, length) == 0)
        return (1);
    }
    return (0);
}

void
printmes(char mstr[80], int mflags, int mrecpt, int mfrom)
{
    if (docolor) {
        switch (mflags) {
            case 17: /* MGOD: */
                printf(CBOLD);
                printf(CGREEN);
                break;
            case 329: /* MJOIN: */
            case 297: /* MLEAVE: */
            case 169: /* MKILLA: */
            case 101: /* MTAKE: */
            case 197: /* MBOMB: */
            case 133: /* MDEST: */
            case 69: /* M?: */
                break;
            default:
                printf(CBOLD);
        }
    }
    PRINT(mstr);
}

void
usage(char *me)
{
    int x;
    char message[][255] = {
        "Netrek II message board watching tool.\n",
        "\t'%s [options]'\n",
        "\nOptions:\n",
        "\t-k    do not remove kills from listing\n",
        "\t-c    colorized text (need higher than a vt100 term)\n",
        "\t-h    this usage description\n",
        "\nNo options will list every message, any other option will be interpreted as\n",
        "a filter, which is a Terrance Chang mod, and I dont know|care what it is for.\n",
        ""
    };

    fprintf(stderr, "-- NetrekII (Paradise), %s --\n", PARAVERS);
    for (x=0; *message[x] != '\0'; x++)
        fprintf(stderr, message[x], me);

    exit(1);
}

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
#include "struct.h"
#include "data.h"

char   *argv0;

int     oldalert = PFGREEN;	/* Avoid changing more than we have to */
int     remap[9] = {0, 1, 2, -1, 3, -1, -1, -1, 4};
int     selfdest;
int     lastm;
int     delay;			/* delay for decaring war */
int     rdelay;			/* delay for refitting */
int     mustexit = 0;
int     keeppeace = 0;
char   *shipnos = "0123456789abcdefghijklmnopqrstuvwxyz";
int     sock = -1;
int     xtrekPort = 2592;
int     shipPick;
int     tmpPick = (int)CRUISER;	/* just to assign something :) */
int     teamPick;
int     repCount = 0;
char    namePick[16];
char    passPick[16];
int     inputMask = -1;
int     nextSocket;
char   *host;
int     noressurect = 0;
int     userVersion = 0, userUdpVersion = 0;
int     timerDelay = 200000;	/* delay between sending stuff to client */
char    testdata[KEY_SIZE];
int     RSA_Client;
char    RSA_client_type[256];	/* LAB 4/1/93 */
int     testtime = -1;
int     chaos = 0;
int     topgun = 0;		/* added 12/9/90 TC */
int     hourratio = 1;		/* Fix thing to make guests advance fast */
int     blk_flag = 0;		/* added 1/19/93 KAO */
int     udpSock = (-1);		/* UDP - the almighty socket */
int     commMode = 0;		/* UDP - initial mode is TCP only */
int     blk_metaserver = 0;	/* 1 if this call is from meta-server */
char   *galaxyValid;		/* 0 if galaxy invalid */

#ifdef FEATURE			/* plus defaults, to protect client */
int F_feature_packets = 1;	/* allow feature packets */
int F_allowViewBox = 1;		/* allow view box */
int F_allowShowAllTractorPressor = 1;	/* allow all tracts/presses */
int F_allowContinuousMouse = 1;	/* allow continuous mouse */
int F_UseNewMacro = 1;		/* allow new macros */
int F_UseSmartMacro = 1;	/* Allow smart macros */
int F_multiline_enabled = 1;	/* Allow multiline macros */
int F_why_dead = 1;		/* Allow why_dead reporting */
int F_cloakerMaxWarp = 1;	/* Allow cloaker to go maxwarp */
int F_gen_distress = 0;		/* No RCDs (not yet implemented in server) */
int F_allow_beeplite = 0;	/* No RCDs ==> no allowed beeplite */
unsigned char F_beeplite_flags;	/* flags for beeplite */
int F_terrain = 0;		/* Client isn't capable of terrain by default */
unsigned char F_terrain_major;	/* Major terrain version client can handle */
unsigned char F_terrain_minor;	/* Minor terrain version client can handle */
int F_gz_motd = 0;		/* Client can't handle GZipped MOTD packets */
unsigned char F_gz_motd_major;	/* Major gzipped format client can handle */
unsigned char F_gz_motd_minor;	/* Minor gzipped format client can handle */
#endif

double  oldmax = 0.0;

extern double Sin[], Cos[];	/* Initialized in sintab.c */

char    pseudo[PSEUDOSIZE];
char    login[PSEUDOSIZE];

#ifdef RANKS2
struct rank ranks[NUMRANKS] =
{
    /* genos   DI     batl   strat  spec      name  */
    {    0,    0,     0.00,  0.0,   0.0,   "Recruit"},
    {    1,    10,    0.30,  0.3,   0.0,   "2nd Mate"},
    {    2,    25,    0.40,  0.6,   0.0,   "1st Mate"},
    {    3,    45,    0.50,  0.9,   0.0,   "Bosun"},
    {    4,    70,    0.70,  1.2,   0.0,   "Ensign"},
    {    5,    100,   0.90,  1.5,   0.0,   "2nd Lieutenant"},
    {    6,    140,   1.10,  2.0,   0.0,   "1st Lieutenant"},
    {    8,    190,   1.30,  2.5,   0.0,   "Lt. Cmdr."},
    {    10,   250,   1.50,  3.0,   0.5,   "Commander"},
    {    15,   300,   1.80,  3.5,   0.7,   "Battle Cmdr."},
    {    18,   350,   2.00,  4.0,   1.0,   "Captain"},
    {    25,   400,   2.10,  4.3,   2.5,   "Fleet Capt."},
    {    50,   500,   2.15,  4.8,   3.0,   "Commodore"},
    {    75,   700,   2.20,  5.3,   3.3,   "Rear Adml."},
    {    100,  900,   2.25,  5.7,   3.6,   "Admiral"},
    {    200,  1200,  2.30,  6.0,   3.8,   "Grand Adml."},
    {    300,  1700,  2.35,  6.1,   4.0,   "Moff"},
    {    500,  2500,  2.40,  6.2,   4.2,   "Grand Moff"}
};
#ifdef CASSIUS_ROYALTY
struct royalty royal[NUMROYALRANKS] = {
    {"none"},
    {"Wesley"},
    {"Footslog"},
    {"COMMODORE"},
    {"Dread"},
    {"Agent"},
    {"Gorilla"},
    {"Lord"},
    {"Emperor"},
    {"Q"}
};
#else
struct royalty royal[NUMROYALRANKS] = {
    {"none"},
    {"Wesley"},
    {"Agent"},
    {"Emperor"},
    {"Q"}
};
#endif /* Royalty */
#else
struct rank ranks[NUMRANKS] =
{
    /* gen     DI     batl   strat  spec    name  */
    {0, 0, 0.00, 0.0, 0.0, "Recruit"},
    {1, 10, 0.30, 0.3, 0.0, "Specialist"},
    {2, 25, 0.40, 0.6, 0.0, "Cadet"},
    {3, 45, 0.50, 0.9, 0.0, "Midshipman"},
    {4, 70, 0.70, 1.2, 0.0, "Ensn., J.G."},
    {5, 100, 0.90, 1.5, 0.0, "Ensign"},
    {6, 140, 1.10, 2.0, 0.0, "Lt., J.G."},
    {8, 190, 1.30, 2.5, 0.0, "Lieutenant"},
    {10, 250, 1.50, 3.0, 0.5, "Lt. Cmdr."},
    {15, 300, 1.80, 3.5, 0.7, "Commander"},
    {18, 350, 2.00, 4.0, 1.0, "Captain"},
    {25, 400, 2.10, 4.3, 2.5, "Fleet Capt."},
    {50, 500, 2.15, 4.8, 3.0, "Commodore"},
    {75, 700, 2.20, 5.3, 3.3, "Moff"},
    {100, 900, 2.25, 5.7, 3.6, "Grand Moff"},
    {300, 1200, 2.30, 6.0, 3.8, "Rear Adml."},
    {700, 1700, 2.35, 6.1, 4.0, "Admiral"},
    {1000, 2500, 2.40, 6.2, 4.2, "Grand Adml."}
};

struct royalty royal[NUMROYALRANKS] = {
    {"none"},
    {"Wesley"},
    {"Centurion"},
    {"Praetor"},
    {"Q"}
};
#endif

/* ping stuff */
int     ping = 0;		/* to ping or not to ping, client's decision */
long    packets_sent = 0;	/* # all packets sent to client */
long    packets_received = 0;	/* # all packets received from client */
int     ping_ghostbust = 0;	/* ghostbust timer */


char    _PARAVERS[] = "@(#)version 3.0 patch 0 DEV";
char    *PARAVERS = &(_PARAVERS[4]);
char    MCONTROL[] = "CONTROL->";
char    UMPIRE[] = "Umpire ";
/* for sending messages from GOD/Server to others */
char    MSERVA[] = "SRV->ALL";
char    SERVNAME[] = "SRV";

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

#ifndef srv_config_h
#define srv_config_h

/*
// This file is used to define various different options in the server,
// which are not included in the sysdefs (.sysdef/sysdefaults.c), for
// optimization purposes.
//
// Simply adjust the comments for options you wish/dont wish to use.
*/

/*
// SHORT_PACKETS
*/

#define SHORT_PACKETS

/*
// give the loosing team an advantage on army supplies
*/

#if 0
#define LOOSING_ADVANTAGE 2.0
#endif

/*
// Give JS real statistics credit for assists.
*/

#define JS_TPLANET_CREDIT


/*
// Give WB double stats for bombing.
*/

#define WB_BOMBING_CREDIT

/*
// Change those annoying control characters to spaces
// untested!
*/

#define REMOVE_CTL

/*
// the robots accumulate stats
*/

#define ROBOTSTATS

/*
// If you wish your daemon to support league-style play, uncomment this.
*/

#define LEAGUE_SUPPORT

/*
// Do you wish to have god controls in message.o?
*/

#define GOD_CONTROLS

/*
// Cluechecking.
//
// Cluechecker #1 is Robs
//  ** supports clue checking with words from the MOTD with MOTD_SUPPORT
// Cluechecker #2 is Brandon's (hacked from Robs)
//  ** will mail a form letter to people with MAIL_CLUELETTER
*/

#undef CLUECHECK1
#undef MOTD_SUPPORT
#undef CLUECHECK2
#undef MAIL_CLUELETTER

/*
// Long ranged Scanners.  This boosts each ship's scanrange up to 1000
// with the exception of the scout (8000) and SB (10000).
*/

#undef LONG_SCANRANGE

/*
// Beefy Bases boosts their heaftyness a touch.
//
// Default set to on Sat Nov  5 15:50:52 MST 1994 by PLC vote
*/

#define BEEFY_BASES

/*
// RANKS2 defines a pippin structure to the ranks and royalty
//
// see data.c for the actual listing
*/

#define RANKS2

/* CASSIUS_ROYALTY defines the structure to use cassius's royalty */

#undef CASSIUS_ROYALTY

/*
// Allow Iggy during TMODE
*/

#undef IGGY_IN_T

/*
// Repair while in warp
*/

#define REPAIR_IN_WARP

/*
// bombing resources takes more time
//
// Default set to on Sat Nov  5 15:50:52 MST 1994 by PLC vote
*/

#define SLOW_BOMB

/*
// If we are using RSA exemption,
// where does the file live?
*/
#define RSA_EXEMPTION_FILE	"etc/rsa-exemption"

/*
// For the old, unbalanced ship values
//
// Default set to off Mon June 5 20:00 MDT 1995 by PLC vote
*/
#undef OLDSHIPVALS

/*
// For the new det distance
//
*/
#define SHIPDET

/*
// For FEATURE_PACKETS to determine client capabilities
*/

#define FEATURE

#undef BUTTORP_PENALTY

/*
 * more detail into logfiles
 */
#define LOG_LONG_INFO

/*
 * RC_DISTRESS stuff
 */
#define	RC_DISTRESS

#endif

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
#ifdef FEATURE
#ifdef HPUX
#include <sys/types.h>
#include <netinet/in.h>
#endif
#include "defs.h"
#include "data.h"
#include "struct.h"
#include "packets.h"

void sendClientPacket();

#ifdef UNIXWARE		/* can you BELIEVE it doesn't have strcmpi?? */
#if 0		/* just  use strcmp for now */
int strcmpi( char *a, char *b ){

  char ta, tb;

  while( (*a) && (*b) ){
    ta = ((*a)>='a') && ((*b) <= 'z'))?(*a)&223:(*a);
    tb = ((*b)>='a') && ((*b) <= 'z'))?(*b)&223:(*b);
    if( ta < tb )
      return( -1 );
    if( ta > tb )
      return( 1 );
    a++;
    b++;
  }
  if( !(*a) && (*b) )
    return( 1 );
  if( !(*b) && (*a) )
    return( -1 );
  return( 0 );
}
#endif
#endif

/*
 * Feature.c
 *
 * March, 1994.    Joe Rumsey, Tedd Hadley
 *
 * most of the functions needed to handle SP_FEATURE/CP_FEATURE
 * packets.  fill in the features list below for your client, and
 * add a call to reportFeatures just before the RSA response is sent.
 * handleFeature should just call checkFeature, which will search the
 * list and set the appropriate variable.  features unknown to the
 * server are set to the desired value for client features, and off
 * for server/client features.
 *
 * feature packets look like:
struct feature_cpacket {
   char                 type;
   char                 feature_type;
   char                 arg1,
                        arg2;
   int                  value;
   char                 name[80];
};

 *  type is CP_FEATURE, which is 60.  feature_spacket is identical.
 *
 *  Code lifted July 1995 by Bob Glamm - enabled into server code.
 *
 */

void    handleFeature(struct feature_cpacket *);
void    sendFeature (char *name, int feature_type,
		       int value, int arg1, int arg2);

/* not the actual packets: this holds a list of features to report for */
/* this client. */
struct feature {
    char   *name;
    int    *var;		/* holds allowed/enabled status */
    char    feature_type;	/* 'S' or 'C' for server/client */
    int     value;		/* desired status */
    char   *arg1, *arg2;	/* where to copy args, if non-null */
} features[] = {
    /*
       also sent seperately, put here for checking later. should be ok that
       it's sent twice.
    */
    {"FEATURE_PACKETS", &F_feature_packets, 'S', 1, 0, 0},

    {"VIEW_BOX", &F_allowViewBox, 'C', 1, 0, 0},
    {"SHOW_ALL_TRACTORS", &F_allowShowAllTractorPressor, 'S', 1, 0, 0},
#ifdef CONTINUOUS_MOUSE
    {"CONTINUOUS_MOUSE", &F_allowContinuousMouse, 'C', 1, 0, 0},
#endif
    {"NEWMACRO", &F_UseNewMacro, 'C', 1, 0, 0},
    /* {"SMARTMACRO",&F_UseSmartMacro, 'C', 1, 0, 0}, */
    {"MULTIMACROS", &F_multiline_enabled, 'S', 1, 0, 0},
    {"WHY_DEAD", &F_why_dead, 'S', 1, 0, 0},
    {"CLOAK_MAXWARP", &F_cloakerMaxWarp, 'S', 1, 0, 0},
/*{"DEAD_WARP", &F_dead_warp, 'S', 1, 0, 0},*/
#ifdef RC_DISTRESS
    {"RC_DISTRESS", &F_gen_distress, 'S', 1, 0, 0},
#ifdef BEEPLITE
    {"BEEPLITE", &F_allow_beeplite, 'C', 1, (char*)&F_beeplite_flags, 0},
#endif
#endif
/* terrain features */
    {"TERRAIN", &F_terrain, 'S', 1, (char*)&F_terrain_major, (char*)&F_terrain_minor},
/* Gzipped MOTD */
    {"GZ_MOTD", &F_gz_motd, 'S', 1, (char*)&F_gz_motd_major, (char*)&F_gz_motd_minor},
    {0, 0, 0, 0, 0, 0}
};

void handleFeature(packet)
  struct feature_cpacket *packet;
{
  int feature = 0;
/*
  int value = ntohl( packet->value );
*/

#ifdef FEATURE_DIAG
  pmessage( "Whoohoo!  Getting a feature packet.", 0, MALL, MSERVA );
#endif
  while( features[feature].name ){
    if( !strcmp( features[feature].name, packet->name ) ){
#ifdef FEATURE_DIAG
      {
        char buf[80]; 
        sprintf( buf, "BEANS!  Matched feature %s", packet->name );
        pmessage( buf, 0, MALL, MSERVA );
      }
#endif
      /* A match was found. */
      if( features[feature].var )
        *features[feature].var = packet->value;
      if( features[feature].arg1 )
        *features[feature].arg1 = packet->arg1;
      if( features[feature].arg2 )
        *features[feature].arg2 = packet->arg2;

      /*
       * tell the client that the server handles all of the above
       * server features.
       */
      if( features[feature].feature_type == 'S' )	{
#ifdef FEATURE_DIAG
        pmessage( "Sending FEATURE_PACKETS right back at ya!", 0, MALL, MSERVA);
#endif
        sendFeature( features[feature].name,
		     features[feature].feature_type,
                     features[feature].value,
                     (features[feature].arg1 ? *features[feature].arg1 : 0),
                     (features[feature].arg2 ? *features[feature].arg2 : 0) );
      }
    }
    feature++;
  }
}
    
void
sendFeature(name, feature_type, value, arg1, arg2)
    char   *name;
    int     feature_type;
    int     value;
    int     arg1, arg2;
{
    struct feature_cpacket packet;
#ifdef FEATURE_DIAG
    char buf[80];
  
    sprintf( buf, "Sending packet %d, name %s", SP_FEATURE, name );
    pmessage( buf, 0, MALL, MSERVA );
#endif

    strncpy(packet.name, name, sizeof(packet.name));
    packet.type = SP_FEATURE;
    packet.name[sizeof(packet.name) - 1] = 0;
    packet.feature_type = feature_type;
    packet.value = htonl(value);
    packet.arg1 = arg1;
    packet.arg2 = arg2;
    sendClientPacket((struct player_spacket *) & packet);
}
#endif

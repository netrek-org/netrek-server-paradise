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






/*-----------------------------NUMBER DEFINES------------------------------*/
#define EXPDIST 350		/* At this range a torp will explode */
#define DAMDIST 2000		/* At this range a torp does damage */
#define PLASDAMDIST 2500	/* At this range, a plasma does damage */
#define FSTRIKEDIST	3200	/* a fighter will fire a torp at this radius */
#define F_ESCORT  2500		/* Fighter escort range */
#define F_ARC    16		/* 1/2 the forward arc a fighter is allowed
				   to fire */
/*--------------------------------------------------------------------------*/






/*-----------------------------FUNCTION PROTOTYPES-------------------------*/
void     explode();
void     pexplode();
void     udphaser();
int     weap_near_object();
int     near();
int     pnear();
void     udtorps();
int     torp_track_target();
void     udplasmatorps();
int     plasma_track_target();
unsigned char get_bearing();
int     f_land();
int     fighter_track_target();
/*-------------------------------------------------------------------------*/






/*-------END OF FILE--------*/

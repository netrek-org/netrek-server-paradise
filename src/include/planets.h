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

#include "defs.h"		/* for GWIDTH */
#include "grid.h"		/* for GWIDTH */


/*--------------------------------NUMBER DEFINES-------------------------*/

/*-----------------------------------------------------------------------*/



/*------------------------PLANETS FUNCTION PROTOTYPES----------------------*/
void    gen_planets();		/* generate planets positions */
void    popplanets();		/* update planets pop */
void    growplanets();		/* grow planets facilities */
void    plfight();		/* do fighting for planets */
void    save_planets();		/* save the planets to '.planets' file */
/*-------------------------------------------------------------------------*/

void check_revolt();
void moveplanets();
void initplanets();

/*--------------------------PLANET UTILITY FUNCTIONS-----------------------*/
#ifdef __STDC__
extern int place_stars(struct planet *first, int count, int border, 
		       int minpad, int maxpad, struct planet *othercheck, 
		       int ocount);
extern void zero_plflags(struct planet *first, int count);
extern void randomize_atmospheres(struct planet *first, int count, 
				  int p1, int p2, int p3, int p4);
extern void randomize_resources(struct planet *first, int count, 
				int nm, int nd, int na);
extern void justify_galaxy(int numsystems);
#else
extern int place_stars();
extern void zero_plflags();
extern void randomize_atmospheres();
extern void randomize_resources();
extern void justify_galaxy();
#endif
void pl_neworbit();
/*-------------------------------------------------------------------------*/


/*----------END OF FILE-------*/

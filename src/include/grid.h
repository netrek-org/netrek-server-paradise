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

#ifndef grid_h_
#define grid_h_

/* specifying an x,y of -1,-1 removes the item from the space grid */

/* the current coordinates of the object in question are not relevant */

void    move_planet( /* pno, x, y, isold */ );
void    move_player( /* pno, x, y, isold */ );

void    move_torp( /* tno, x, y, isold */ );
void    move_missile( /* dno, x, y, isold */ );

#endif

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


#ifndef plutil_h_
#define plutil_h_

/* is the player in warp? */
int     in_warp();
int     idx_to_mask();
int     mask_to_idx();
int     base_undock();
void    explode_everyone(int, int);
void    evaporate();
void    scout_planet(int,int);
int random_round(double);

/* remove the player from his base.
   Returns 1 if the player was really docked, 0 otherwise */
int     undock_player( /* struct player * */ );

/* make sure the player has the proper coordinates */
void    enforce_dock_position( /* struct player * */ );

/* dock a player onto a base at a certain port */
void
        dock_to( /* struct player *, int base_num, int port_id */ );

#endif

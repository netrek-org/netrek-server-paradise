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



/*----------------------------FUNCTION PROTOTYPES--------------------------*/
void    warmessage();		/* print war message */
void    peacemessage();		/* print peace message */
int     realNumShips();		/* number of ships on a team */
int     tournamentMode();	/* are we in tourn mode */
void    pmessage( /* char *str, int recip, int group, char addr */ );
void    parse_godmessages();	/* detect and log messages to god */
void    PlaceLostArmies();	/* places armies from a lost/GB'd player */
void    god2player(char *, int);
/*--------------------------------------------------------------------------*/






/*--------END OF FILE------*/

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

/*-------------------------------MODULE TYPES------------------------------*/

enum field_type {
    FT_CHAR, FT_BYTE, FT_SHORT, FT_INT, FT_LONG, FT_FLOAT, FT_STRING,
    FT_LONGFLAGS
};

struct field_desc {
    char   *name;
    enum field_type type;
    int     offset;
    void   *aux;
};

/*-------------------------------------------------------------------------*/

extern struct field_desc ship_fields[];

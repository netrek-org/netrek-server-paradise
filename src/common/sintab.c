/*------------------------------------------------------------------
  Copyright 1989		Kevin P. Smith
				Scott Silvey

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies.

  NETREK II -- Paradise

  Permission to use, copy, modify, and distribute this software and
  its documentation, or any derivative works thereof,  for any 
  NON-COMMERCIAL purpose and without fee is hereby granted, provided
  that this copyright notice appear in all copies.  No
  representations are made about the suitability of this software for
  any purpose.  This software is provided "as is" without express or
  implied warranty.

	Xtrek Copyright 1986			Chris Guthrie
	Netrek (Xtrek II) Copyright 1989	Kevin P. Smith
						Scott Silvey
	Paradise II (Netrek II) Copyright 1993	Larry Denys
						Kurt Olsen
						Brandon Gillespie
		                Copyright 2000  Bob Glamm

--------------------------------------------------------------------*/

/*---------------------------------DATA------------------------------------*/
/*  The sine table so we do not have to calculate this on the fly.  */

double  Sin[] = {
    -1.0000,
    -0.9997,
    -0.9988,
    -0.9973,
    -0.9952,
    -0.9925,
    -0.9892,
    -0.9853,
    -0.9808,
    -0.9757,
    -0.9701,
    -0.9638,
    -0.9570,
    -0.9496,
    -0.9416,
    -0.9330,
    -0.9239,
    -0.9143,
    -0.9040,
    -0.8933,
    -0.8820,
    -0.8701,
    -0.8578,
    -0.8449,
    -0.8315,
    -0.8176,
    -0.8033,
    -0.7884,
    -0.7731,
    -0.7573,
    -0.7410,
    -0.7243,
    -0.7072,
    -0.6896,
    -0.6716,
    -0.6533,
    -0.6345,
    -0.6153,
    -0.5958,
    -0.5759,
    -0.5557,
    -0.5351,
    -0.5142,
    -0.4930,
    -0.4715,
    -0.4497,
    -0.4277,
    -0.4054,
    -0.3828,
    -0.3600,
    -0.3370,
    -0.3138,
    -0.2904,
    -0.2668,
    -0.2431,
    -0.2192,
    -0.1952,
    -0.1711,
    -0.1469,
    -0.1225,
    -0.0982,
    -0.0737,
    -0.0492,
    -0.0247,
    -0.0001,
    0.0244,
    0.0489,
    0.0734,
    0.0979,
    0.1223,
    0.1466,
    0.1708,
    0.1949,
    0.2190,
    0.2428,
    0.2666,
    0.2901,
    0.3135,
    0.3367,
    0.3598,
    0.3825,
    0.4051,
    0.4274,
    0.4495,
    0.4713,
    0.4928,
    0.5140,
    0.5349,
    0.5554,
    0.5757,
    0.5956,
    0.6151,
    0.6343,
    0.6531,
    0.6714,
    0.6894,
    0.7070,
    0.7241,
    0.7408,
    0.7571,
    0.7729,
    0.7882,
    0.8031,
    0.8175,
    0.8314,
    0.8448,
    0.8576,
    0.8700,
    0.8818,
    0.8931,
    0.9039,
    0.9141,
    0.9238,
    0.9329,
    0.9415,
    0.9495,
    0.9569,
    0.9637,
    0.9700,
    0.9757,
    0.9808,
    0.9852,
    0.9891,
    0.9925,
    0.9952,
    0.9973,
    0.9988,
    0.9997,
    1.0000,
    0.9997,
    0.9988,
    0.9973,
    0.9952,
    0.9925,
    0.9892,
    0.9853,
    0.9808,
    0.9757,
    0.9700,
    0.9638,
    0.9569,
    0.9495,
    0.9415,
    0.9330,
    0.9239,
    0.9142,
    0.9040,
    0.8932,
    0.8819,
    0.8701,
    0.8577,
    0.8449,
    0.8315,
    0.8176,
    0.8032,
    0.7884,
    0.7730,
    0.7572,
    0.7410,
    0.7243,
    0.7071,
    0.6896,
    0.6716,
    0.6532,
    0.6344,
    0.6153,
    0.5957,
    0.5758,
    0.5556,
    0.5350,
    0.5141,
    0.4929,
    0.4714,
    0.4496,
    0.4276,
    0.4053,
    0.3827,
    0.3599,
    0.3369,
    0.3137,
    0.2903,
    0.2667,
    0.2430,
    0.2191,
    0.1951,
    0.1710,
    0.1468,
    0.1225,
    0.0981,
    0.0736,
    0.0491,
    0.0246,
    0.0000,
    -0.0245,
    -0.0490,
    -0.0735,
    -0.0980,
    -0.1224,
    -0.1467,
    -0.1709,
    -0.1950,
    -0.2190,
    -0.2429,
    -0.2667,
    -0.2902,
    -0.3136,
    -0.3368,
    -0.3598,
    -0.3826,
    -0.4052,
    -0.4275,
    -0.4496,
    -0.4713,
    -0.4928,
    -0.5140,
    -0.5349,
    -0.5555,
    -0.5758,
    -0.5956,
    -0.6152,
    -0.6343,
    -0.6531,
    -0.6715,
    -0.6895,
    -0.7071,
    -0.7242,
    -0.7409,
    -0.7572,
    -0.7730,
    -0.7883,
    -0.8032,
    -0.8175,
    -0.8314,
    -0.8448,
    -0.8577,
    -0.8700,
    -0.8819,
    -0.8932,
    -0.9040,
    -0.9142,
    -0.9238,
    -0.9330,
    -0.9415,
    -0.9495,
    -0.9569,
    -0.9638,
    -0.9700,
    -0.9757,
    -0.9808,
    -0.9853,
    -0.9892,
    -0.9925,
    -0.9952,
    -0.9973,
    -0.9988,
    -0.9997
};
double  Cos[] = {
    0.0000,
    0.0245,
    0.0491,
    0.0736,
    0.0980,
    0.1224,
    0.1467,
    0.1710,
    0.1951,
    0.2191,
    0.2430,
    0.2667,
    0.2903,
    0.3137,
    0.3369,
    0.3599,
    0.3827,
    0.4052,
    0.4275,
    0.4496,
    0.4714,
    0.4929,
    0.5141,
    0.5350,
    0.5556,
    0.5758,
    0.5957,
    0.6152,
    0.6344,
    0.6532,
    0.6715,
    0.6895,
    0.7071,
    0.7242,
    0.7409,
    0.7572,
    0.7730,
    0.7883,
    0.8032,
    0.8176,
    0.8315,
    0.8448,
    0.8577,
    0.8701,
    0.8819,
    0.8932,
    0.9040,
    0.9142,
    0.9239,
    0.9330,
    0.9415,
    0.9495,
    0.9569,
    0.9638,
    0.9700,
    0.9757,
    0.9808,
    0.9853,
    0.9892,
    0.9925,
    0.9952,
    0.9973,
    0.9988,
    0.9997,
    1.0000,
    0.9997,
    0.9988,
    0.9973,
    0.9952,
    0.9925,
    0.9892,
    0.9853,
    0.9808,
    0.9757,
    0.9700,
    0.9638,
    0.9570,
    0.9495,
    0.9416,
    0.9330,
    0.9239,
    0.9142,
    0.9040,
    0.8933,
    0.8819,
    0.8701,
    0.8578,
    0.8449,
    0.8315,
    0.8176,
    0.8032,
    0.7884,
    0.7731,
    0.7573,
    0.7410,
    0.7243,
    0.7072,
    0.6896,
    0.6716,
    0.6532,
    0.6344,
    0.6153,
    0.5958,
    0.5759,
    0.5556,
    0.5351,
    0.5142,
    0.4930,
    0.4715,
    0.4497,
    0.4276,
    0.4053,
    0.3828,
    0.3600,
    0.3370,
    0.3138,
    0.2904,
    0.2668,
    0.2431,
    0.2192,
    0.1952,
    0.1710,
    0.1468,
    0.1225,
    0.0981,
    0.0737,
    0.0492,
    0.0246,
    0.0001,
    -0.0244,
    -0.0490,
    -0.0735,
    -0.0979,
    -0.1223,
    -0.1466,
    -0.1709,
    -0.1950,
    -0.2190,
    -0.2429,
    -0.2666,
    -0.2902,
    -0.3136,
    -0.3368,
    -0.3598,
    -0.3826,
    -0.4051,
    -0.4275,
    -0.4495,
    -0.4713,
    -0.4928,
    -0.5140,
    -0.5349,
    -0.5555,
    -0.5757,
    -0.5956,
    -0.6151,
    -0.6343,
    -0.6531,
    -0.6715,
    -0.6895,
    -0.7070,
    -0.7242,
    -0.7409,
    -0.7571,
    -0.7729,
    -0.7883,
    -0.8031,
    -0.8175,
    -0.8314,
    -0.8448,
    -0.8577,
    -0.8700,
    -0.8819,
    -0.8932,
    -0.9039,
    -0.9142,
    -0.9238,
    -0.9329,
    -0.9415,
    -0.9495,
    -0.9569,
    -0.9637,
    -0.9700,
    -0.9757,
    -0.9808,
    -0.9853,
    -0.9892,
    -0.9925,
    -0.9952,
    -0.9973,
    -0.9988,
    -0.9997,
    -1.0000,
    -0.9997,
    -0.9988,
    -0.9973,
    -0.9952,
    -0.9925,
    -0.9892,
    -0.9853,
    -0.9808,
    -0.9757,
    -0.9701,
    -0.9638,
    -0.9570,
    -0.9496,
    -0.9416,
    -0.9330,
    -0.9239,
    -0.9143,
    -0.9041,
    -0.8933,
    -0.8820,
    -0.8702,
    -0.8578,
    -0.8449,
    -0.8316,
    -0.8177,
    -0.8033,
    -0.7884,
    -0.7731,
    -0.7573,
    -0.7411,
    -0.7244,
    -0.7072,
    -0.6897,
    -0.6717,
    -0.6533,
    -0.6345,
    -0.6154,
    -0.5958,
    -0.5759,
    -0.5557,
    -0.5351,
    -0.5142,
    -0.4930,
    -0.4715,
    -0.4498,
    -0.4277,
    -0.4054,
    -0.3828,
    -0.3601,
    -0.3371,
    -0.3138,
    -0.2905,
    -0.2669,
    -0.2432,
    -0.2193,
    -0.1953,
    -0.1711,
    -0.1469,
    -0.1226,
    -0.0982,
    -0.0737,
    -0.0493,
    -0.0247
};

/*-------------------------------------------------------------------------*/






/*-------END OF FILE--------*/

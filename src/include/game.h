/*
// Full copyright information is available in the file ../docs/CREDITS
*/

#ifndef game_h
#define game_h

/* conquer.c */
void udsurrend();
void checksurrender();
void checkwin(int);

/* weapons.c */
void udmissiles();

/* tourny.c */
void udtourny();
void tlog_conquerline(char *);
void tlog_refit();
void tlog_plkill();
void tlog_plquit();
void tlog_plankill();
void tlog_plandest();
void tlog_plantake();
void tlog_planaban();
void tlog_jsassist();
void tlog_beamup();
void tlog_beamdown();
void tlog_Bbeamup();
void tlog_Bbeamdown();
void tlog_bomb();
void tlog_bres();
void tlog_pop();
void tlog_res();
void tlog_revolt();
void tlog_all();
void scan_for_unexpected_tourny_events();

/* planets.c */
void save_planets();

/* shipvals.c */
void getshipdefaults();

/* stats.c */
void credit_armiesbombed();

/* galaxies */
void gen_galaxy_1(void);
void gen_galaxy_2(void);
void gen_galaxy_3(void);
void gen_galaxy_4(void);
void gen_galaxy_5(void);
void gen_galaxy_6(void);
void gen_galaxy_7(void);
void gen_galaxy_8(void);

#endif

#ifndef PROTO_H
#define PROTO_H

#include "config.h"

#include "defs.h"
#include "struct.h"

/* *************************************************************************
   from common/
   ************************************************************************* */

/* common/cutil.c */
/* r_signal() takes a signal number and a handler of type void f(), returns
   old handler.  Handler _has_ to have no args in the parens. */
/* The signal handlers should be the only prototypes without the P(). */
void (*r_signal 
         P( (int, void (*)()) ) 
     ) ();
#ifndef HAVE_STDUP
char *strdup P((char *));
#endif

/* common/data.c */
void init_data P((char *));

/* common/detonate.c */
void detothers P((void));

/* common/enter.c */
#ifdef USED
int find_start_planet P((int, int));
#endif
int findrslot P((void));
int enter P((int, int, int, int, int));

/* common/getship.c */
void getship P((struct ship *, int));
void get_ship_for_player P((struct player *, int));

/* common/grid.c */
void move_planet P((int, int, int, int));
void move_player P((int, int, int, int));
void move_torp P((int, int, int, int));
void move_missile P((int, int, int, int));

/* common/imath.c  - this needs to go IMHO as everyone has FPUs these days ;)*/
int isqrt P((int));
int ihypot P((int, int));

/* common/interface.c */
void set_speed P((int, int));
void set_course P((unsigned char));
void shield_up P((void));
void shield_down P((void));
#ifdef USED
void shield_tog P((void));
#endif
void bomb_planet P((void));
void beam_up P((void));
void beam_down P((void));
void repair P((void));
void repair_off P((void));
#ifdef USED
void repeat_message P((void));
void cloak P((void));
#endif
void cloak_on P((void));
void cloak_off P((void));
void lock_planet P((int));
void lock_player P((int));
void tractor_player P((int));
void pressor_player P((int));
void declare_war P((int));
void switch_special_weapon P((void));
void do_refit P((int));
int allowed_ship P((int, int, int, int));
int numPlanets P((int));

/* common/orbit.c */
void orbit P((void));
void newdock P((int));

/* common/parse-ranks.c */
void parse_ranks P((char *));

/* common/path.c */
char *build_path P((char *));

/* common/phaser.c */
void phaser P((unsigned char));

/* common/plutil.c */
int idx_to_mask P((int));
int mask_to_idx P((int));
#ifdef USED
int in_warp P((struct player *));
#endif
int undock_player P((struct player *));
int base_undock P((struct player *, int));
void enforce_dock_position P((struct player *));
void dock_to P((struct player *, int, int));
void scout_planet P((int, int));
void evaporate P((struct player *));
void explode_everyone P((int, int));
int random_round P((double));
char *twoletters P((struct player *));

/* common/shmem.c */
void startdaemon P((int, int));
void openmem P((int, int));
void blast_shmem P((void));

/* common/sintab.c - redo this from client's sintab */

/* common/smessage.c */
void pmessage P((char *, int, int, char *));
void pmessage2 P((char *, int, int, char *, unsigned char));

/* common/torp.c */
void ntorp P((unsigned char, int));

/* common/util.c */
unsigned char getcourse P((int, int, int, int));
int angdist P((unsigned char, unsigned char));
int temporally_spaced P((struct timeval *, int));
int check_fire_warp P((void));
int check_fire_warpprep P((void));
int check_fire_docked P((void));

/* *************************************************************************
   from daemon/
   ************************************************************************* */

/* daemon/conquer.c */
void udsurrend P((void));
void endgame_effects P((int, int, int));
void checkwin P((int));

/* daemon/daemonII.c */
void starttimer P((void));
void stoptimer P((void));
void ghostmess P((struct player *));
void saveplayer P((struct player *));
void rescue P((int, int, int));

/* daemon/dutil.c */
void killmess P((struct player *, struct player *));
void cause_kaboom P((struct player *));
int get_explode_views P((short));
int inflict_damage P((struct player *, struct player *, struct player *,
                      int, int));
int enemy_admiral P((int));

/* daemon/misc.c */
void warmessage P((void));
void peacemessage P((void));
int realNumShips P((void));
int tournamentMode P((void));
void pmessage P((char *, int, int, char *));
void god2player P((char *, int));
void parse_godmessages P((void));

/* daemon/planets.c */
void initplanets P((void));
void growplanets P((void));
void check_revolt P((void));
void gen_planets P((void));
void moveplanets P((void));
void popplanets P((void));
void plfight P((void));
void save_planets P((void));

/* daemon/player.c */
void loserstats P((int));
void killerstats P((int, struct player *));
void checkmaxkills P((int));
void beam P((void));
void udcloak P((void));
void udplayers P((void));

/* daemon/pl_gen/pl_gen*.c */
int place_stars P((struct planet *, int, int, int, int, struct planet *, int));
void zero_pflags P((struct planet *, int));
void randomize_atmospheres P((struct planet *, int, int, int, int, int));
void randomize_resources P((struct planet *, int, int, int, int));
void justify_galaxy P((int));

#ifndef LOADABLE_PLGEN
void gen_galaxy_1 P((void));
void gen_galaxy_2 P((void));
void gen_galaxy_3 P((void));
void gen_galaxy_4 P((void));
void gen_galaxy_5 P((void));
void gen_galaxy_6 P((void));
void gen_galaxy_7 P((void));
void gen_galaxy_8 P((void));
#endif

/* daemon/shipvals.c */
void getshipdefaults P((void));

/* daemon/stats.c */
void credit_armiesbombed P((struct player *, int, struct planet *));

/* daemon/sysdefaults.c */
void readsysdefaults P((void));
int update_sys_defaults P((void));

/* daemon/terrain.c */
void generate_terrain P((void));
void doTerrainEffects P((void));

/* daemon/tourny.c */
void tlog_plkill P((struct player *, struct player *, struct player *));
void tlog_plquit P((struct player *));
void tlog_plankill P((struct player *, struct player *, struct player *));
void tlog_plandest P((struct planet *, struct player *));
void tlog_plantake P((struct planet *, struct player *));
void tlog_planaban P((struct planet *, struct player *));
#ifdef USED
void tlog_jsassist P((struct player *));
#endif
void tlog_beamup P((struct planet *, struct player *));
void tlog_beamdown P((struct planet *, struct player *));
void tlog_Bbeamup P((struct player *, struct player *));
void tlog_Bbeamdown P((struct player *, struct player *));
void tlog_bomb P((struct planet *, struct player *, int));
void tlog_bres P((struct planet *, struct player *, char *));
void tlog_pop P((struct planet *, int));
void tlog_res P((struct planet *, char *));
void tlog_revolt P((struct planet *));
void scan_for_unexpected_tourny_events P((void));
void tlog_conquerline P((char *));
void udtourny P((void));
void starttourn P((void));
void endtourn P((void));

/* daemon/wander2.c (unused as of now, I believe) */
#ifdef USED
void pinit P((void));
void pmove P((void));
#endif

/* daemon/weapons.c */
void udphaser P((void));
#ifdef USED
int outofbounds P((int, int));
#endif
void udtorps P((void));
void udmissiles P((void));
void udplasmatorps P((void));

/* *************************************************************************
   from listen/
   ************************************************************************* */

/* listen/listen.c */
/* most (if not all of these) should be static and declared in listen.c, 
   as probably only the listen binary uses them */
#ifdef USED
char *lasterr P((void));
#endif

/* *************************************************************************
   from ntserv/
   ************************************************************************* */

/* ntserv/cluecheck.c */
void init_motdbuf P((char *));
void countdown_clue P((void));
int accept_cluecheck P((char *));

/* ntserv/death.c */
void compute_ratings P((struct player *, struct rating *));
void death P((void));

/* ntserv/fatudp.c */
void reset_fat_list P((void));
void updateFat P((struct player_spacket *));
int fatten P((void));
void fatMerge P((void));

/* ntserv/feature.c */
void handleFeature P((struct feature_cpacket *));
void sendFeature P((char *, int, int, int, int));

/* ntserv/findslot.c */
int findslot P((int, enum HomeAway));

/* ntserv/gameconf.c */
void updateGameparams P((void));

/* ntserv/getentry.c */
void detourneyqueue P((void));
void getEntry P((int *, int *));
int realNumShips P((int)); /* duplicated in daemon code? */

/* ntserv/getname.c */
void getname P((void));
void savestats P((void));

/* ntserv/input.c */
void setflag P((void));
void input P((void));
int reconnect P((void));

/* ntserv/main.c */
void stop_interruptor P((void));
void start_interruptor P((void));
void exitGame P((void));
void sendMotd P((void));

/* ntserv/message.c */
int parse_command_mess P((char *, unsigned char ));

/* ntserv/missile.c */
void fire_missile_dir P((unsigned char));

/* ntserv/packets.c */
int size_of_cpacket P((void *));
int size_of_spacket P((unsigned char *));

/* ntserv/parsexbm.c */
int ParseXbmFile P((FILE *, int *, int *, char **));

/* ntserv/ping.c */
void pingResponse P((struct ping_cpacket *packet));
void sendClientPing P((void));

/* ntserv/plasma.c */
void nplasmatorp P((unsigned char, int));

/* ntserv/redraw.c */
void intrupt P((void));
void auto_features P((void));

/* ntserv/smessage.c */
void pmessage P((char *, int, int, char *));
void pmessage2 P((char *, int, int, char *, unsigned char));

/* ntserv/socket.c */
int connectToClient P((char *, int));
void checkSocket P((void));
void initClientData P((void));
int isClientDead P((void));
void updateClient P((void));
void briefUpdateClient P((void));
void updateStatus P((void));
void updateSelf P((void));
void updateShips P((void));
void updatePlanets P((void));
void updateTerrain P((void));
void updateMOTD P((void));
void sendQueuePacket P((short int));
void sendClientPacket P((struct player_spacket *));
int readFromClient P((void));
void sendPickokPacket P((int));
void sendMotdLine P((char *));
void sendMaskPacket P((int));
int checkVersion P((void));
void logEntry P((void));
void logmessage P((char *));
int connUdpConn P((void));
int closeUdpConn P((void));
void printUdpInfo P((void));
#ifdef DOUBLE_UDP
void sendSC P((void));
#endif
void bounce P((char *, int));
void sendShipCap P((void));
void sendMotdPic P((int, int, char *, int, int, int));
void sendMotdNopic P((int, int, int, int, int));
void sendMissileNum P((int));
int site_rsa_exempt P((void));

/* ntserv/sockio.c */
int buffersEmpty P((void));
void resetUDPbuffer P((void));
void resetUDPsequence P((void));
void flushSockBuf P((void));
void build_select_masks P((fd_set *, fd_set *));
int socketPause P((void));
int socketWait P((void));
int gwrite P((int, char *, int));
void sendUDPbuffered P((int, void *, int));
void sendTCPbuffered P((void *, int));
void sendTCPdeferred P((void *, int));
void flushDeferred P((void));
void undeferDeferred P((void));

/* ntserv/timecheck.c */
void load_time_access P((void));
int time_access P((void));

/* ntserv/warning.c */
void warning P((char *));
void updateWarnings P((void));
void imm_warning P((char *));

/* *************************************************************************
   from robot/
   ************************************************************************* */

/* robot/rmove.c */
RETSIGTYPE rmove P((int));

/* robot/robotII.c */
void save_robot P((void));
void config P((void));

/* *************************************************************************
   from snake/
   ************************************************************************* */

/* snake/snake.c */

/* snake/snakemove.c */
RETSIGTYPE snakemove P((int));
RETSIGTYPE exitSnake();

#endif

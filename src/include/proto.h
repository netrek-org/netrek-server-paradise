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
int touch P((char *));
#ifndef HAVE_STDUP
char *strdup P((char *));

/* common/detonate.c */
void detothers P((void));

/* common/enter.c */
int find_start_planet P((int, int));
void peanut_gallery P((void));
void advertise_tourney_queue P((void));
int enter P((int, int, int, int, int));
void auto_peace P((void));

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
void shield_tog P((void));
void bomb_planet P((void));
void beam_up P((void));
void beam_down P((void));
void repair P((void));
void repair_off P((void));
void repeat_message P((void));
void cloak P((void));
void cloak_on P((void));
void cloak_off P((void));
void lock_planet P((int));
void lock_player P((int));
void tractor_player P((int));
void pressor_player P((int));
void declare_war P((int));
void sendwarn P((char *, int, int));
void switch_special_weapon P((void));
void do_refit P((int));
int allowed_ship P((int, int, int, int));
int numShips P((int));
int numPlanets P((int));
void suspendPrep P((void));
void unsuspendPrep P((void));

/* common/orbit.c */
void orbit P((void));
void newdock P((int));

/* common/path.c */
char *build_path P((char *));

/* common/phaser.c */
void phaser P((unsigned char));

/* common/plutil.c */
int idx_to_mask P((int));
int mask_to_idx P((int));
int in_warp P((struct player *));
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

/* common/torp.c */
void ntorp P((unsigned char, int));

/* common/util.c */
int angdist P((unsigned char, unsigned char));
int temporally_spaced P((struct timeval *, int));
int check_fire_warp P((void));
int check_fire_warpprep P((void));
int check_fire_docked P((void));

/* *************************************************************************
   from daemon/
   ************************************************************************* */

/* daemon/conquer.c */
int genocide P((int *, int *));
void checksurrender P((int, int));
void udsurrend P((void));
void conquerMessage P((int, int, int));
void refresh_team_planetcounts P((void));
void endgame_effectss P((int, int, int));
void checkwin P((int));
void displayBest P((FILE *, int));

/* daemon/daemonII.c */
void printdaemonIIUsage P((char *));
RETSIGTYPE setflag();
void starttimer P((void));
void stoptimer P((void));
void move P((void));
RETSIGTYPE freemem();
void check_load P((void));
void ghostmess P((struct player *));
void saveplayer P((struct player *));
void rescue P((int, int, int));
RETSIGTYPE reaper();
unsigned char getcourse P((int, int, int, int));
void teamtimers P((void));
void shipbuild_timers P((void));

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
void log_message P((char *, char *, char *));
void parse_godmessages P((void));

/* daemon/planets.c */
void sortnames P((void));
void initplanets P((void));
void growplanets P((void));
void pvisible P((void));
void blast_resource P((struct player *, struct player *, int, double));
void pbomb P((void));
void pfire P((void));
void revolt P((struct planet *));
void check_revolt P((void));
void gen_planets P((void));
void pl_neworbit P((void));
void moveplanets P((void));
void pop_one_planet P((struct planet *));
void fill_planets P((int *, int *, int));
int find_other_team P((int));
void popplanets P((void));
void plfight P((void));
void save_planets P((void));

/* daemon/player.c */
void beammeupscotty P((struct player *));
void beamdown P((struct player *));
void doshipexplode P((struct player *));
void doorbit P((struct player *));
void repair_docking_ring P((struct player *));
void doresources P((struct player *));
void dobounce P((struct player *));
void doalert P((struct player *));
void changedir P((struct player *));
int being_tractored P((struct player *));
void domove P((struct player *));
void dotractor P((struct player *));
void loserstats P((int));
void killerstats P((int, struct player *));
void checkmaxkills P((int));
void blowup P((struct player *));
void beam P((void));
void udcloak P((void));
void udplayers P((void));

/* daemon/shipvals.c */
void getshipdefaults P((void));

/* daemon/stats.c */
void credit_armiesbombed P((struct player *, int, struct planet *));

/* daemon/sysdefaults.c */
void readstrings P((char *, char *, char **, int *, int));
void readlongflags P((long *, char *, char **));
void shipdefs P((int, FILE *));
void initteamvals P((void));
void readsysdefaults P((void));
int update_sys_defaults P((void));

/* daemon/terrain.c */
void generate_terrain P((void));
void place_nebula P((int, int, int));
void place_asteroids P((int));
void doTerrainEffects P((void));

/* daemon/tourny.c */
void opentlog P((void));
char *id_player P((struct player *));
char *id_planet P((struct player *));
void tlog_refit P((struct player *));
void tlog_plkill P((struct player *, struct player *, struct player *));
void tlog_plquit P((struct player *));
void tlog_plankill P((struct player *, struct player *, struct player *));
void tlog_plandest P((struct planet *, struct player *));
void tlog_plantake P((struct planet *, struct player *));
void tlog_planaban P((struct planet *, struct player *));
void tlog_jsassist P((struct player *));
void tlog_beamup P((struct planet *, struct player *));
void tlog_beamdown P((struct planet *, struct player *));
void tlog_beamup P((struct player *, struct player *));
void tlog_Bbeamdown P((struct player *, struct player *));
void tlog_bomb P((struct planet *, struct player *, int));
void tlog_bres P((struct planet *, struct player *, char *));
void tlog_pop P((struct planet *, int));
void tlog_res P((struct planet *, char *));
void tlog_revolt P((struct planet *));
void scan_for_unexpected_tourny_events P((void));
void closetlog P((void));
void tlog_all P((void));
void tlog_conquerline P((char *));
void udtourny P((void));
void starttourn P((void));
void endtourn P((void));

/* daemon/wander2.c (unused as of now, I believe) */
void pinit P((void));
void pmove P((void));

/* daemon/weapons.c */
int hostile_to P((int, int, struct player *));
void explode_damage P((struct basetorp *, int, int));
void explode P((struct basetorp *));
void pexplode P((struct plasmatorp *));
void udphaser P((void));
int weap_near_object P((struct basetorp *, int, int));
int near_player P((struct basetorp *, int));
int f_load P((struct missile *));
int pnear P((struct plasmatorp *));
int outofbounds P((int, int));
void udtorps P((void));
void udmissiles P((void));
int anticipate_impact P((int, int, int, int, int));
int torp_track_opportunity P((struct basetorp *, int, int));
void udplasmatorps P((void));
unsigned char get_bearing P((int, int, int));
int fighter_track_target P((struct missile *, int));
int f_torp P((struct missile *));

/* *************************************************************************
   from listen/
   ************************************************************************* */

/* listen/listen.c */
/* most (if not all of these) should be static and declared in listen.c, 
   as probably only the listen binary uses them */
void detach P((void));
void getListenSock P((void));
void getConnections P((void));
void set_env P((char *, char *));
char *dateTime P((void));
void terminate();
void reaper();
void multClose P((va_list));
void syserr P((va_list));
void warnerr P((va_list))
void fatlerr P((va_list));
void err P((va_list));
char *lasterr P((void));
void printlistenUsage P((char *));
void print_pid P((void));

/* *************************************************************************
   from ntserv/
   ************************************************************************* */

/* ntserv/cluecheck.c */
void set_clue_word P((char *));
void init_motdbuf P((char *));
void find_suitable_motd_word P((void));
void parse_motd P((void));
void parse_clue_phrases P((void));
void remind_cluecheck P((void));
void demand_clue P((void));
void countdown_clue P((void));
int accept_cluecheck P((char *));
/* should probably be static in cluecheck code */
void free_word P((struct word *));
void free_line P((struct line *));
void free_page P((struct page *));
void free_motdstruct P((void));
void set_clue_word P((char *));

/* ntserv/death.c */
void compute_ratings P((struct player *, struct rating *));
void death P((void));

/* ntserv/fatudp.c */
void reset_fat_list P((void));
void dequeue P((FAT_NODE *));
void enqueue P((FAT_NODE *, int));
void updateFat P((struct player_spacket *));
int fatten P((void));
void fatMerge P((void));

/* ntserv/feature.c */
void handleFeature P((struct feature_cpacket *));
void sendFeature P((char *, int, int, int, int));

/* ntserv/findslot.c */
int findslot P((int, enum HomeAway));
int grabslot P((int, enum HomeAway));
void mapWaitCount P((unsigned int));

/* ntserv/gameconf.c */
void updateGameparams P((void));

/* ntserv/getentry.c */
void detourneyqueue P((void));
void getEntry P((int *, int *));
int leaguemask P((int, int));
int tournamentMask P((int));
int realNumShips P((int));
int deadTeam P((int));

/* ntserv/getname.c */
void getname P((void));
void handleLogin P((void));
void savestats P((void));
int lockout P((void));

/* ntserv/input.c */
void setflag P((void));
void input P((void));
int reconnect P((void));

/* ntserv/main.c */
void stop_interruptor P((void));
void start_interruptor P((void));
void exit_game P((void));
void sendSysDefs P((void));
void sendMotd P((void));
RETSIGTYPE reaper();
void printStats P((void));
void doMotdPics P((void));

/* ntserv/message.c */
enum token_names_e next_token P((char *, struct control_cmd *, char **));
int match_token P((char *, char *, char **));
int get_slotnum P((char *, char **));
void bad_slotnum P((char *));
int get_one_token P((char *, char *, int, char **));
int get_int P((char *, int *, char **));
int get_double P((char *, double *, char **));
int get_teamid P((char *, int *, char **));
int get_shipid P((char *, int *, char **));
void respond_with_help_string P((struct control_cmd *));
void trydefect P((struct player *, enum HomeAway, char *,
                  enum HomeAway, char *, struct player *));
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
void calc_loss P((int, struct ping_cpacket *));
void update_lag_stats P((void));
void update_loss_stats P((void));
int mstime P((void));
int msetime P((void));
int uchar_diff P((int, int));

/* ntserv/plasma.c */
void nplasmatorp P((unsigned char, int));

/* ntserv/redraw.c */
#ifdef AUTHORIZE
void check_authentication P((void));
#endif
void intrupt P((void));
void auto_features P((void));
int newcourse P((int, int));

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
struct player *maybe_watching P((struct player *));
struct planet *maybe_watching_planet P((void));
void updateSelf P((void));
void updateShips P((void));
void updateTorpInfos P((void));
void updateTorps P((void));
int encode_torp_status P((struct torp *, int, char **,
                          struct torp_info_spacket *,
			  struct torp_spacket *, int *));
int encode_torp_position P((struct torp *, int, char **, int *,
                            struct torp_spacket *));
void short_updateTorps P((void));
void updateMissiles P((void));
void updateThingies P((void));
void updatePlasmas P((void));
void updatePhasers P((void));
void updatePlanets P((void));
void updateMessages P((void));
void updateTerrain P((void));
void updateMOTD P((void));
void sendQueuePacket P((short int));
void sendClientPacket P((struct player_spacket *));
void sendClientSizedPacket P((struct player_spacket *, int));
int readFromClient P((void));
int doRead P((int));
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
void forceUpdate P((void));
int isCensured P((char *));
int parseIgnore P((struct mesg_cpacket *));
int parseQuery P((struct mesg_spacket *));
int bouncePingStats P((struct mesg_spacket *));
void bounce P((char *, int));
void sendShipCap P((void));
void sendMotdPic P((int, int, char *, int, int, int));
void sendMotdNopic P((int, int, int, int, int));
void sendMissileNum P((int));
int convert_hostname P((char *, struct in_addr *));
int site_rsa_exempt P((void));

/* ntserv/sockio.c */
int buffersEmpty P((void));
void resetUDPbuffer P((void));
void resetUDPsequence P((void));
int addSequence P((char *));
void flushSockBuf P((void));
void build_select_masks P((fd_set *, fd_set *));
int socketPause P((void));
int socketWait P((void));
int gwrite P((int, char *, int));
void sendUDPbuffered P((int, void *, int));
void sendTCPbuffered P((void *, int));
int haveDeferredPackets P((void));
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
void rmove P((void));
unsigned char getcourse P((int, int));
void go_home P((struct Enemy *));
int phaser_plasmas P((void));
int projectDamage P((int, int *));
int isTractoringMe P((struct Enemy *));
struct Enemy *get_nearest P((void));
struct planet *get_nearest_planet P((void));
int do_repair P((void));
char *robo_message P((struct player *));
char *termie_message P((struct player *));
void exitRobot P((void));
void messAll P((char *));

/* robot/robotII.c */
void do_robot_login P((void));
void save_robot P((void));
int findrslot P((void));
void warning P((char *));
void config P((void));

/* *************************************************************************
   from snake/
   ************************************************************************* */

/* snake/snake.c */
int choose_team P((int));
int findtestslot P((void));
int findrslot P((void));
void warning P((char *));
int config P((void));
void printsnakeUsage P((void));

/* snake/snakemove.c */
struct planet *star_of P((struct planet *));
void snakemove P((void));
void startsnake P((void));
void restore_eye P((void));
void movesnake P((void));
void check_explode P((void));
int crash_killer P((struct player *));
void doeyes P((void));
RETSIGTYPE exitSnake();
void check_tboundary P((int, unsigned char *, int, int, int));
int rrnd P((int));
void snake_torp P((struct torp *, int, struct player *));
struct player *whokilledme P((struct plasmatorp *));
void award P((struct player *));
int bombcheck P((int, int));
void make_war P((struct player *, int));
int sp_area P((int, int));


#endif

/*
 * robot/robot_newbie.c
 *
 * This is a robot designed for newbie servers.  It will spawn a master 
 * robot, RobotServ, which will spawn newbie robots as needed, and kill
 * them off when needed to make room for real players.
 */


#include "robot_functions.h"
#include "robot_newbie.h"


/* this function destroys the robot - may be superceded by exitRobot() */
static void obliterate(int wflag, char kreason) {
   /* 
    * 0 = do nothing to war status, 1= make war with all, 
    * 2= make peace with all
    */
   struct player *j;
   int i, k;

   /* clear torps and plasmas out */
   MZERO(torps, sizeof(struct torp) * MAXPLAYER * (MAXTORP + MAXPLASMA));

   for (j = firstPlayer; j<=lastPlayer; j++) {
      if (j->p_status == PFREE) {
         continue;
      }

      j->p_status = PEXPLODE;
      j->p_whydead = kreason;

      if (j->p_ship.s_type == STARBASE) {
         j->p_explode = 2 * SBEXPVIEWS ;
      }
      else {
         j->p_explode = 10 ;
      }

      j->p_ntorp = 0;
      j->p_nplasmatorp = 0;

      if (wflag == 1) {
         j->p_hostile = (FED | ROM | ORI | KLI);  /* angry */
      }
      else if (wflag == 2) {
         j->p_hostile = 0;  /* otherwise make all peaceful */
      }

      j->p_war = (j->p_swar | j->p_hostile);
   }  /* end for */
}  /* end obliterate() */


/* This function cleans things up */
static void cleanup(int unused) {
   register struct player *j;
   register int i, retry;

   do {
      /* terminate all robots */
      for (i = 0, j = players; i < MAXPLAYER; i++, j++) {
         if ((j->p_status == PALIVE) && rprog(j->p_login, j->p_monitor)) {
            stop_this_bot(j);
         }
      }  /* end for */

      USLEEP(2000000);
      retry=0;

      for (i = 0, j = players; i < MAXPLAYER; i++, j++) {
         if ((j->p_status != PFREE) && rprog(j->p_login, j->p_monitor)) {
            retry++;
         }
      }  /* end for */
   } while (retry);   /* Some robots havn't terminated yet */

   for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
      if ((j->p_status != PALIVE) || (j == me)) {
         continue;
      }

      getship(&(j->p_ship), j->p_ship.s_type);
   }  /* end for */

   obliterate(1,KPROVIDENCE);
   status->gameup &= ~GU_NEWBIE;
   exitRobot();
}


/* Not sure what this does */
static void start_internal(char *type) {
   char *argv[6];
   u_int argc = 0;

   argv[argc++] = "robot";

   if ((strncmp(type, "iggy", 2) == 0) ||
      (strncmp(type, "hunterkiller", 2) == 0)) {
      argv[argc++] = "-Ti";
      argv[argc++] = "-P";
      argv[argc++] = "-f";    /* Allow more than one */
   }
   else if (strncmp (type, "cloaker", 2) == 0) {
      argv[argc++] = "-Ti";
      argv[argc++] = "-C";    /* Never uncloak */
      argv[argc++] = "-F";    /* Needs no fuel */
      argv[argc++] = "-f";
   }
   else if (strncmp (type, "hoser", 2) == 0) {
      argv[argc++] = "-p";
      argv[argc++] = "-f";
   }
   else {
      return;
   }

   argv[argc] = NULL;

   if (fork() == 0) {
      SIGNAL(SIGALRM, SIG_DFL);
      execv(Robot,argv);
      perror(Robot);
      _exit(1);
   }
}  /* end start_internal() */


/* Starts a robot */
static void start_a_robot(char *team) {
   char command[256];

   sprintf(command, "%s %s %s %s -h %s -p %d -n '%s' -X robot! -b -O -i",
      RCMD, robot_host, OROBOT, team, hostname, PORT, namearg() );

   if (fork() == 0) {
      SIGNAL(SIGALRM, SIG_DFL);
      execl("/bin/sh", "sh", "-c", command, 0);
      perror("newbie'execl");
      _exit(1);
   }
}  /* end start_a_robot() */


/* Not sure what this does */
static char * namearg(void) {
   register i, k = 0;
   register struct player *j;
   char *name;
   int namef = 1;

   while (1) {
      name = names[random() % NUMNAMES];
      k++;
      namef = 0;

      for (i = 0, j = players; i < MAXPLAYER; i++, j++) {
         if (j->p_status != PFREE 
            && strncmp(name, j->p_name, strlen(name) - 1) == 0) {
            namef = 1;
            break;
         }
      }  /* end for */

      if (!namef) {
         return name;
      }

      if (k == 50) {
         return "guest";
      }
   }  /* end while */
}  /* end namearg() */


/* Figure out how many players are in the game */
static int num_players(int *next_team) {
   int i;
   struct player *j;
   int team_count[MAXTEAM+1];
   int c = 0;

   team_count[ROM] = 0;
   team_count[FED] = 0;

   for (i = 0, j = players; i < MAXPLAYER; i++, j++) {
      if (j->p_status != PFREE && j->p_status != POBSERV &&
         !(j->p_flags & PFROBOT)) {
         team_count[j->p_team]++;
         c++;
      }
   }  /* end for */

   /* Assign which team gets the next robot. */
   if (team_count[ROM] > team_count[FED]) {
      *next_team = FED;
   }
   else {
      *next_team = ROM;
   }

   return c;
}  /* end num_players() */


/* This saves armies that are on robots that are being ejected */
static void save_armies(struct player *p) {
   int i, k;

   k=10*(remap[p->p_team]-1);

   if (k>=0 && k<=30) {
      for (i=0; i<10; i++) {
         if (planets[i+k].pl_owner==p->p_team) {
            planets[i+k].pl_armies += p->p_armies;
            pmessage(0, MALL, "%s->ALL", "%s's %d armies placed on %s",
               mastername, p->p_name, p->p_armies, planets[k+i].pl_name);
            break;
         }
      }  /* end for */
   }
}  /* end save_armies() */


/* This function stops a robot */
static void stop_this_bot(struct player *p) {
   p->p_ship.s_type = STARBASE;
   p->p_whydead=KQUIT;
   p->p_explode=10;
   p->p_status=PEXPLODE;
   p->p_whodead=0;

   pmessage(0, MALL, "%s->ALL",
      "Robot %s (%2s) was ejected to make room for a human player.",
      mastername, p->p_name, p->p_mapchars);

   if ((p->p_status != POBSERV) && (p->p_armies>0)) {
      save_armies(p);
   }
}  /* end stop_this_bot() */

/* Not sure what this does */
static int rprog(char *login) {
   if (strcmp(login, "robot!") == 0) {
      return 1;
   }

   return 0;
}  /* end rprog() */


/* finds a robot to stop and calls stop_this_bot() */
static void stop_a_robot(void) {
   int i;
   struct player *j;

   /* Simplistic: nuke the first robot we see. */
   for (i = 0, j = players; i < MAXPLAYER; i++, j++) {
      if (j->p_status == PFREE) {
         continue;
      }

      if (j->p_flags & PFROBOT) {
         continue;
      }

      /* If he's at the MOTD we'll get him next time. */
      if (j->p_status == PALIVE && rprog(j->p_login, j->p_monitor)) {
         stop_this_bot(j);
         return;
      }
   }
}   /* end stop_a_robot() */


/* This function sees if the game is populated by robots only */
static int is_robots_only(void) {
   register i;
   register struct player *j;

   for (i = 0, j = players; i < MAXPLAYER; i++, j++) {
      if (j->p_status == PFREE) {
         continue;
      }
      
      if (j->p_flags & PFROBOT) {
         continue;
      }

      if (!rprog(j->p_login, j->p_monitor)) {
         /* Found a human. */
         return 0;
      }
   }  /* end for */

   /* Didn't find any humans. */
   return 1;
}  /* end is_robots_only() */


/* 
 * This function checks the RobotServ and then adjusts the numbers of robots
 * and outputs a message periodically.
 */
void checkmess(int unused) {
   static int no_humans = 0;
   int shmemKey = PKEY;
   int i;

   HANDLE_SIG(SIGALRM,checkmess);
   me->p_ghostbuster = 0;         /* keep ghostbuster away */

   if (me->p_status != PALIVE) {  /* So I'm not alive now... */
      fprintf(stderr, "ERROR: RobotServ died?!\n");
      cleanup(0);   /* RobotServ is dead for some unpredicted reason like xsg */
   }

   /* make sure shared memory is still valid */
   if (shmget(shmemKey, SHMFLAG, 0) < 0) {
      exit(1);
      fprintf(stderr, "ERROR: Invalid shared memory.\n");
   }

   ticks++;

    /* End the current game if no humans for 60 seconds. */
   if ((ticks % ROBOCHECK) == 0) {
      if (no_humans >= 60) {
         cleanup(0);   /* Doesn't return. */
      }

      if (is_robots_only()) {
         no_humans += ROBOCHECK / PERSEC;
      }
      else {
         no_humans = 0;
      }
   }

   /* Stop or start a robot. */
   if ((ticks % ROBOCHECK) == 0) {
      int next_team;
      int np = num_players(&next_team);

      if (queues[QU_PICKUP].count > 0 || np > MIN_NUM_PLAYERS) {
         stop_a_robot();
      }
      else if (np < MIN_NUM_PLAYERS ) {
         if (next_team == FED) {
            start_a_robot("-Tf");
         }
         else {
            start_a_robot("-Tr");
         }
      }
   }

   if ((ticks % SENDINFO) == 0) {
      static int alternate = 0;

      alternate++;

      if (1) {
         messAll(255,roboname,"Welcome to the Newbie Server.");
         messAll(255,roboname,"See http://cow.netrek.org/current/newbie.html");
      }
      else {
         messAll(255,roboname,"Think you have what it takes?  Sign up for the draft league!");
         messAll(255,roboname, "See http://draft.lagparty.org/");
      }
   }
}   /* end checkmess() */


/* Not sure what this does */
static void reaper(int sig) {
   int stat=0;
   static int pid;

   while ((pid = WAIT3(&stat, WNOHANG, 0)) > 0);

   HANDLE_SIG(SIGCHLD,reaper);
}  /* end reaper() */


/* This is where the magic happens - Praise Bob! */
int main(int argc, char *argv[]) {
   enum HomeAway homeaway = NEITHER;
   static char hostname[64];
   int overload = 0;
   int team = 4;
   int pno;
   int class;
   int i;

   strcpy(hostname, TREKSERVER);  
   srandom(time(NULL));
   getpath();

   SIGNAL(SIGCHILD, reaper);

   openmem(1);
   strcpy(robot_host,REMOTEHOST);
   readsysdefaults();

   SIGNAL(SIGALRM, checkmess);

   if (!debug) {
      SIGNAL(SIGINT, cleanup);
   }

   class = ATT;
   target = -1;   /* no targeted player */

   if ((pno = findslot(overload, homeaway)) < 0) {
      fprintf(stderr, "Unable to get a slot");
      exit(0);
   }

   me = &players[pno];
   myship = &me->p_ship;
   mystats = &me->p_stats;
   lastm = mctl->mc_current;

   /* set the robot@nowhere fields */
   robonameset(me);

   /* Enter the game */
   enter(team, 0, pno, class, mastername);

   me->p_pos = -1;           /* So robot stats don't get saved */
   me->p_flags |= PFROBOT;   /* Mark as a robot */
   me->p_x = GWIDTH/2;       /* displace to on overlooking position */
   me->p_y = GWIDTH/2;       /* maybe we should just make it fight? */
   me->p_hostile = 0;
   me->p_swar = 0;
   me->p_war = 0;
   me->p_team = 0;           /* independent */

   oldmctl = mctl->mc_current;

   status->gameup |= GU_NEWBIE;

   /* Robot is signalled by the Daemon */
   fprintf(stderr, "Robot Using Daemon Synchronization Timing\n");

   me->p_process = getpid();
   me->p_timerdelay = HOWOFTEN;

   /* allows robots to be forked by the daemon on some systems */
   {
      sigset_t unblock_everything;
      sigfillset(&unblock_everything);
      sigprocmask(SIG_UNBLOCK, &unblock_everything, NULL);
   }

   me->p_status = PALIVE;   /* Put robot in game */

   while (1) {
      PAUSE(SIGALRM);
   }
}


/* end robot/robot_newbie.c */

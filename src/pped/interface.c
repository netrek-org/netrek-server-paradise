/*
 * interface.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>
#include <struct.h>
#include <curses.h>
#include "common.h"
#include "db.h"
#include "interface.h"
#include "intfdesc.h"
#include "file.h"
#include "data.h"
#include "crypt.h"


static char *shrt_royal_names[] = {
	"",
	"Wes",
	"Cen",
	"Pra",
	"Emp"
};

static char *royal_names[] = {
	"none",
	"Wesley",
	"Centurion",
	"Praetor",
	"Emperor"
};

static char *shrt_rank_names[] = {
	"Recr", "Spec", "Cadt", "Mids", "EnJr", "Ensi", "LtJr",
	"Lieu", "LtCm", "Cmdr", "HiCm", "Capt", "FlCp", "Comd",
	"Adml", "RAdm", "Moff", "GrMo"
};

static char *rank_names[] = {
	"Recruit",     "Specialist", "Cadet",        "Midshipman",
	"Ensn. Jr.",   "Ensign",     "Lt. Jnr. Gr.", "Lieutenant",
	"Lt. Cmdr.",   "Commander",  "High Cmdr.",   "Captain",
	"Fleet Capt.", "Commodore",  "Admiral",      "Rear Adml.",
	"Moff",        "Grand Moff"
};

void getTTYinfo()
{
	struct winsize ws;
	char *name, bp[1024], area[1024], *ap = area;

#ifdef SYSV
	signal(SIGWINCH, getTTYinfo);
#endif

	/* determine # of lines */
	ioctl(0, TIOCGWINSZ, &ws);
	numLines = ws.ws_row;

	/* find terminal sequence to clear the screen */
	if(name = getenv("TERM"))
		if(tgetent(bp, name) > 0)
			clrStr = tgetstr("cl", &ap);
}

void cls()
{
	printf("%s", clrStr); fflush(stdout);
}

void Interface()
{
	struct plnode *p, *pn;
	int lines, top, i, num;
	char buf[18], *c;

	top = 0;
	for(;;) {
		lines = numLines - 4;
		if(lines < 5) err_fatal("not enough tty lines!");

		cls();
		printf("\
 C  #  Rnk  Ryl  Name                  C  #  Rnk  Ryl  Name\n");
		printf("\
 - --- ---- ---- -----------------     - --- ---- ---- -----------------\n");

		for(i = 0; i < lines; i++) {
			if(top + i > numDBentries - 1) {
				printf("\n");
				continue;
			}
			p = GetNode(top + i);
			if(!p) {
				printf("\n");
				continue;
			}
			strncpy(buf, p->player.name, 16);
			strcat(buf, "_");
			printf(" %c%4d %-4s %-4s %-17.17s    ",
				p->status ? '*':' ', top + i,
				shrt_rank_names[p->player.stats.st_rank],
				shrt_royal_names[p->player.stats.st_royal],
				Strip(buf));

			if(top + i + lines > numDBentries - 1) {
				printf("\n");
				continue;
			}
			p = GetNode(top + i + lines);
			if(!p) {
				printf("\n");
				continue;
			}
			strncpy(buf, p->player.name, 16);
			strcat(buf, "_");
			printf(" %c%4d %-4s %-4s %s\n",
				p->status ? '*':' ', top + i + lines,
				shrt_rank_names[p->player.stats.st_rank],
				shrt_royal_names[p->player.stats.st_royal],
				Strip(buf));
		}
		printf("\nIndex: Command (? for help) -->"); fflush(stdout);
		if(fgets(buf, 18, stdin) == NULL)
			continue;

		switch(buf[0]) {
			case '\n':
			case 'n':
				top += (lines * 2);
				if(top >= numDBentries)
					top = 0;
				break;
			case 'p':
				if(!top) {
					if(numDBentries - 2 * lines > 0)
						top = numDBentries - 2 * lines;
					else
						top = 0;
				} else {
					top -= (lines * 2);
					if(top < 0) top = 0;
				}
				break;
			case 'Q':
				GoAway(CheckChanged());
				break;
			case 's':
				DoSave(1);
				break;
			case 'e':
				printf("Enter player name to edit -->");
				fflush(stdout);
				if(fgets(buf, 18, stdin) == NULL)
					break;
				if(c = strrchr(buf, '\n'))
					*c = (char)0;
				num = GetByName(buf);
				if(num < 0) {
					Report("Couldn't find any players by that name.");
					break;
				}
				Edit(num);
				break;
			case '?':
				Report("\n\
[num] : Edit player [num]\n\
  e   : Edit player (by name)\n\
  n   : Next page\n\
[ret] : Next page\n\
  p   : Previous page\n\
  s   : Save to file\n\
  Q   : Quit pped\n");
				break;
			default:
				i = sscanf(buf, "%d", &num);
				if(!i || (num < 0) || (num >= numDBentries))
					break;
				Edit(num);
		}
	}
}

void Edit(int pnum)
{
	struct plnode *p, player;
	char buf[18], txt[80];
	int i, num, lines, top;

	p = GetNode(pnum);
	if(!p) return;
	player = *p;
	player.status = 0;
	
	top = 0;

	for(;;) {
		lines = numLines - 2;
		cls();
		Display(&player, top, top+lines);

		printf("\nEdit: Command (? for help) -->"); fflush(stdout);
		if(fgets(buf, 18, stdin) == NULL)
			continue;

		switch(buf[0]) {
			case '?':  /* help */
				Report("\n\
  n   : Next page\n\
  p   : Previous page\n\
[num] : Change value [num]\n\
  s   : Save and return to index\n\
  w   : Save\n\
[ret] : Return to index, don't save\n\
  x   : Same as [ret]\n\
  D   : Delete this entry and return to index\n\
  r   : Revert (undo changes since last save)\n\
  Q   : Quit pped\n");
				break;

			case 'n':  /* page forward */
				top += lines;
				if(top > NUMDESC + 1) top = 0;
				continue;

			case 'p':  /* page backward */
				top -= lines;
				if(top < 0) top = 0;
				continue;

			case 'D':  /* delete (and exit) */
				sprintf(txt, "delete entry for player %s", p->player.name);
				if(Verify(txt)) {
					DeleteNode(pnum);
					Report("Player deleted.");
					return;
				} else {
					Report("Canceled.");
				}
				break;

			case 'Q':  /* quit */
				GoAway(player.status || CheckChanged());
				break;

			case 's':  /* save and exit */
				if(player.status) {
					*p = player;
					player.status = 0;
					Report("Player saved.");
				}
				return;
				break;

			case 'w':  /* save */
				if(player.status) {
					*p = player;
					player.status = 0;
					Report("Player saved.");
				} else {
					Report("No changes to save.");
				}
				break;

			case 'r':  /* revert */
				if(player.status) {
					sprintf(txt, "undo changes for player %s?", p->player.name);
					if(Verify(txt)) {
						player = *p;
						player.status = 0;
						Report("Changes discarded.");
					} else {
						Report("Canceled.");
					}
				} else {
					Report("No changes to undo.");
				}
				break;

			case 0x1B:  /* (escape) exit, no changes */
			case '\n':
			case 'x':
				if(player.status)
					if(!Verify("exit?  There are unsaved changes."))
						break;
				return;

			default:
				i = sscanf(buf, "%d", &num);
				if(!i || (num < 0) || (num >= NUMDESC))
					break;
				Change(num, &player);
		}
	}
}

int Verify(char *str)
{
	char buffer[18];

	for(;;) {
		printf("Verify %s (y/n) -->", str); fflush(stdout);
		if(fgets(buffer, 18, stdin) == NULL)
			continue;
		switch(buffer[0]) {
			case 'Y':
			case 'y':
				return 1;
			case 'N':
			case 'n':
				return 0;
		}
	}
}

void Report(char *str)
{
	char buffer[18];

	printf("%s\n", str);
	printf("Press return to continue -->"); fflush(stdout);

	fgets(buffer, 18, stdin);
}

void Change(int num, struct plnode *p)
{
	struct inter_desc *it;
	void *ptr;
	char str16[16], *c;
	char pwd[PASSWORD_SIZE];
	char buffer[80];
	int intnum, i, col;
	float floatnum;

	if(num < 0 || num >= NUMDESC) return;
	it = &idesc_tab[num];

	ptr = (void *)((int)(&p->player) + it->offset);

	switch(it->type) {
		case DT_CHAR16:
			printf("Current value for %s: \"%s\"\n", it->name, (char *)ptr);
			printf("Enter new value -->"); fflush(stdout);
			str16[0] = 0;
			fgets(str16, 16, stdin);
			if(c = strrchr(str16, '\n'))
				*c = (char)0;

			sprintf(buffer, "\"%s\" as new value for %s.", str16, it->name);
			if(Verify(buffer)) {
				if(strncmp((char *)ptr, str16, 16)) {
					strncpy((char *)ptr, str16, 16);
					p->status = 1; /* something changed */
				}
			}
			break;

		case DT_PWD:
			printf("Current value for %s: \"%s\"\n", it->name, (char *)ptr);
			printf("Enter new value -->"); fflush(stdout);
			pwd[0] = 0;
			fgets(pwd, PASSWORD_SIZE, stdin);
			if(c = strrchr(pwd, '\n'))
				*c = (char)0;

			sprintf(buffer, "\"%s\" as new value for %s.", pwd, it->name);
			if(Verify(buffer)) {
				if(strncmp((char *)ptr, pwd, PASSWORD_SIZE)) {
					strncpy((char *)ptr, pwd, PASSWORD_SIZE);
					p->status = 1; /* something changed */
				}
			}
			break;

		case DT_INT:
		case DT_TICKS:
			printf("Current value for %s: %d\n", it->name, *((int *)ptr));
			printf("Enter new value -->"); fflush(stdout);
			fgets(buffer, 30, stdin);
			intnum = atoi(buffer);
			sprintf(buffer, "%d as new value for %s.", intnum, it->name);
			if(Verify(buffer)) {
				if(intnum != *((int *)ptr)) {
					*((int *)ptr) = intnum;
					p->status = 1;
				}
			}
			break;

		case DT_FLOAT:
			printf("Current value for %s: %f\n", it->name, *((float *)ptr));
			printf("Enter new value -->"); fflush(stdout);
			fgets(buffer, 30, stdin);
			floatnum = (float)atof(buffer);
			sprintf(buffer, "%f as new value for %s.", floatnum, it->name);
			if(Verify(buffer)) {
				if(floatnum != *((float *)ptr)) {
					*((float *)ptr) = floatnum;
					p->status = 1;
				}
			}
			break;

		case DT_RANK:
			for(i = 0, col = 0; i < NUMRANKS; i++) {
				printf(" %2d: %-7s ", i, shrt_rank_names[i]);
				if(++col == 4) {
					printf("\n");
					col = 0;
				}
			}
			printf("\nCurrent value for %s: %d (%s)\n", it->name,
				  *((int *)ptr), rank_names[*((int *)ptr)]);
			printf("Enter new value -->"); fflush(stdout);
			fgets(buffer, 30, stdin);
			intnum = atoi(buffer);
			sprintf(buffer, "%d (%s) as new value for %s.", intnum,
				  rank_names[intnum], it->name);
			if(Verify(buffer)) {
				if(intnum != *((int *)ptr)) {
					*((int *)ptr) = intnum;
					p->status = 1;
				}
			}
			break;

		case DT_ROYAL:
			for(i = 0; i < NUMROYALRANKS; i++)
				printf(" %2d: %s\n", i, royal_names[i]);

			printf("Current value for %s: %d (%s)\n", it->name,
				  *((int *)ptr), royal_names[*((int *)ptr)]);
			printf("Enter new value -->"); fflush(stdout);
			fgets(buffer, 30, stdin);
			intnum = atoi(buffer);
			sprintf(buffer, "%d (%s) as new value for %s.", intnum,
				  royal_names[intnum], it->name);
			if(Verify(buffer)) {
				if(intnum != *((int *)ptr)) {
					*((int *)ptr) = intnum;
					p->status = 1;
				}
			}
			break;
		default:
			Report("Error in case statement in Change()");
			break;
	}
}

void Display(struct plnode *n, int from, int to)
{
	int i, hour, dt = 0;
	char c;
	void *ptr;
	struct inter_desc *it;
	struct tm *stm;
	time_t tt;

	if(to > NUMDESC) {
		to = NUMDESC;
		dt = 1;
	}

	for(i = from; i < to; i++) {
		it = &idesc_tab[i];
		ptr = (void *)((int)(&n->player) + it->offset);

		switch(it->type) {
			case DT_CHAR16:
				printf("  (%2d) %16s : \"%s\"\n", i, it->name, (char *)ptr);
				break;
			case DT_PWD:
				printf("  (%2d) %16s : \"%s\"\n", i, it->name, (char *)ptr);
				break;
			case DT_INT:
				printf("  (%2d) %16s : %d\n", i, it->name, *((int *)ptr));
				break;
			case DT_FLOAT:
				printf("  (%2d) %16s : %.2f\n", i, it->name, *((float *)ptr));
				break;
			case DT_TICKS:
				printf("  (%2d) %16s : %d (%.2f hours)\n",
					i, it->name, *((int *)ptr), (*((int *)ptr))/36000.0);
				break;
			case DT_RANK:
				printf("  (%2d) %16s : %d (%s)\n", i, it->name, *((int *)ptr),
					rank_names[*((int *)ptr)]);
				break;
			case DT_ROYAL:
				printf("  (%2d) %16s : %d (%s)\n", i, it->name, *((int *)ptr),
					royal_names[*((int *)ptr)]);
				break;
			default:
				printf("Yikes! Unknown it->type in Display()\n");
				break;
		}
	}
	if(dt) {
		tt = n->player.stats.st_lastlogin;
		stm = localtime(&tt);
		hour = stm->tm_hour;
		c = 'A';
		if(!hour) {
			hour = 12;
		} else if(hour > 12) {
			hour -= 12;
			c = 'P';
		}
		printf("        Last login: %2d:%02d %cM  %02d/%02d/%d\n",
		  hour, stm->tm_min, c, stm->tm_mon+1, stm->tm_mday, stm->tm_year);
	}
}

int CheckChanged()
{
	struct plnode *p;

	if(dbDirty) return(1);

	p = firstEnt;
	while(p) {
		if(p->status) return(1);
		p = p->next;
	}
	return(0);
}

void ClearChanged()
{
	struct plnode *p;

	dbDirty = 0;

	p = firstEnt;
	while(p) {
		p->status = 0;
		p = p->next;
	}
}

/* Strip: convert non-printable control chars to ^L notation */
char *Strip(char *str)
{
	static char buff[36], *o;

	o = buff;
	while(*str) {
		*str &= 0x7f;
		if((int)*str < (int)' ') {
			*o++ = '^';
			*o++ = *str + (char)64;
		} else if ((int)*str == 127) {
			*o++ = '^';
			*o++ = '?';
		} else {
			*o++ = *str;
		}
		str++;
	}
	*o = 0;
	return(buff);
}


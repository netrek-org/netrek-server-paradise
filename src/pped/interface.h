/*
 * Prototypes for interface.c
 */

void getTTYinfo();
void cls();
void Interface();
void Edit(int pnum);
int Verify(char *str);
void Report(char *str);
void Change(int num, struct plnode *p);
void Display(struct plnode *n, int from, int to);
int CheckChanged();
void ClearChanged();
char *Strip(char *str);


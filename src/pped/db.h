
struct plnode {
	struct statentry player;
	struct plnode    *next;
	struct plnode    *prev;
	int status;
};

/* prototypes for db.c */
void initDB();
void addDB(struct statentry *player);
struct plnode *GetNode(int n);
void DeleteNode(int n);
int GetByName(char *name);


/*
// Full copyright information is available in the file ../docs/CREDITS
*/

#ifndef net_h
#define net_h

/* socket.c */
void sendClientPacket();
void updateMOTD();
void updateStatus();
void sendMotdNopic(int,int,int,int,int);
int closeUdpConn();
void printUdpInfo();
void logmessage(char *);

/* sockio.c */
int gwrite(int, char *, int);
void sendUDPbuffered(int, void *, int);
void undeferDeferred();
int socketWait(void);
int buffersEmpty(void);
void sendTCPdeferred(void *, int);
void sendTCPbuffered(void *, int);
void build_select_masks(fd_set *, fd_set *);
void flushDeferred();
void resetUDPsequence();
void resetUDPbuffer();

/* ping.c */
void sendClientPing();
void pingResponse();

/* fatudp.c */
int fatten();
void fatMerge();
void updateFat();


#endif

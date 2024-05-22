#ifndef CLIENT_H
#define CLIENT_H

#define NCKNMSIZE 20
#define MSGSIZE 128
#define SHMSIZE 4096
#define PATH "SystemV_server"
		
struct msgbuf{														
	char nick[NCKNMSIZE];
	char text[MSGSIZE];
};

extern int shmid_chat;	
extern char my_nickname[NCKNMSIZE];

void *chat_receiver(void *);
void *chat_sender(void *);

#endif

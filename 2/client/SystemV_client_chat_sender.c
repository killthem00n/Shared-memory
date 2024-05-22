#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "client.h"

void *chat_sender(void *args)
{
	struct msgbuf *send;
	char tmp_buf[MSGSIZE] = { 0 };											//message pre-record buf
	send = shmat(shmid_chat, NULL, 0);										//attach shm to current process									
	if(send == (void *) -1){
		perror("shmat: send");
		exit(1);
	}
	
	printf("Enter your nickname: ");
	fgets(my_nickname, NCKNMSIZE, stdin);										//client determines his nickname
	my_nickname[strcspn(my_nickname, "\n")] = 0;									//remove "\n" symbol from nickname string
	memset(&send->text, 0, MSGSIZE);										//clearing message buffer in shm
	strncpy(send->nick, my_nickname, NCKNMSIZE);									//writing a nickname to shm
	strncpy(send->text, "joined111", 9);										//writing a message about connecting to a chat to shm
	
	printf("\n");
	
	while (1){
		fgets(tmp_buf, MSGSIZE, stdin);										//entering a message
		tmp_buf[strcspn(tmp_buf, "\n")] = 0;									//remove "\n" symbol from message string
		strncpy(send->text, tmp_buf, MSGSIZE);									//writing a message to shm
		strncpy(send->nick, my_nickname, NCKNMSIZE);								//writing a nickname to shm
		if ((strcmp(send->text, "/exit")) == 0){								//if message == "/exit"...		
			memset(&send->text, 0, MSGSIZE);								//...clearing message buffer in shm...							
			strcat(send->text, "left111");									//...writing a message about disconnection to a chat to shm...
			exit(1);											//...and program ends
		}
		sleep(0.1);
	}
}

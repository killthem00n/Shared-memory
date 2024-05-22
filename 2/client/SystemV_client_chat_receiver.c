#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "client.h"

void *chat_receiver(void *args)
{
	struct msgbuf *receive;							
	char check_buf[MSGSIZE];											//buf to eliminate duplicate messages
	receive = shmat(shmid_chat, NULL, 0);										//attach shm to current process						
	if(receive == (void *) -1){
		perror("shmat: receive");
		exit(1);
	}
	
	while (1){
		if (my_nickname[0] == 0){										//don't display messages until the user enters his nickname
			sleep(0.1);
			continue;
		} 
		if (strcmp(check_buf, receive->text) == 0 | strcmp(my_nickname, receive->nick) == 0){			//each message is displayed only once and our messages aren't displayed
			sleep(0.1);
			continue;
		}
		else{
			if ((strcmp(receive->text, "joined111")) == 0){							//if client has connected
				strncpy(check_buf, receive->text, MSGSIZE);						//copy message in check_buf to avoid repetition	
				printf("%s has joined the chat\n", receive->nick);
				continue;
			}
			if ((strcmp(receive->text, "left111")) == 0){							//if client has left
				strncpy(check_buf, receive->text, MSGSIZE);						//copy message in check_buf to avoid repetition	
				printf("%s has left the chat\n", receive->nick);
				continue;
			}
			strncpy(check_buf, receive->text, SHMSIZE);							//copy message in check_buf to avoid repetition					
			printf("%s: %s\n", receive->nick, receive->text);						//display regular message
			sleep(0.1);
		}
	}
}

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "server.h"													

int main(void)
{
	key_t key_chat;
	int shmid_chat;														
	
	if ((key_chat = ftok(PATH, 55)) < 0){								//generate key									
		perror("ftok: chat");
		exit(1);
	}
	
	if ((shmid_chat = shmget(key_chat, SHMSIZE, IPC_CREAT | 0666)) < 0){				//allocate shm segment			
		perror("shmget: chat");
		exit(1);
	}	
	
	while (1){
		sleep(1);
	}							
	
	if ((shmctl(shmid_chat, IPC_RMID, NULL)) < 0){							//remove shm segment												
		perror("shmctl: chat");	
		exit(1);
	}
	
	return 0;
}

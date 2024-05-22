#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "client.h"												

int shmid_chat = 0;	
char my_nickname[NCKNMSIZE] = { 0 };

int main(void)
{
	pthread_t THREADS[2];
	int *status;
	key_t key_chat;
	
	if ((key_chat = ftok(PATH, 55)) < 0){								//generate key									
		perror("ftok: chat");
		exit(1);
	}
	
	if ((shmid_chat = shmget(key_chat, SHMSIZE, 0)) < 0){						//connect to shm segment								
		perror("shmget: chat");
		exit(1);
	}										
	
	pthread_create(&THREADS[0], NULL, chat_receiver, NULL);						//create thread for receive messages							
	pthread_create(&THREADS[1], NULL, chat_sender, NULL);						//create thread for send messages		
	
	for (int i = 0; i < 2; i++){									//terminate both threads						
		pthread_join(THREADS[i], (void **)&status);
	}
	
	return 0;
}

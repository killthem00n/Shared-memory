#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SHMSIZE 4096
#define PATH "SystemV_server"

int main(void)
{
	key_t key;
	int shmid;
	char *addr;
	
	if ((key = ftok(PATH, 5)) < 0){									//generate key								
		perror("ftok");
		exit(1);
	}
	
	if ((shmid = shmget(key, SHMSIZE, IPC_CREAT | 0666)) < 0){					//allocate shm segment						
		perror("shmid");
		exit(1);
	}
	
	addr = shmat(shmid, NULL, 0);									//attach shm to current process
	if(addr == (void *) -1){
		perror("shmat");
		exit(1);
	}
		
	while (1){											//wait for client to connect
		if (strcmp(addr, "Hello!") == 0){							//if client connected, send a response
			memset(addr, 0, SHMSIZE);							
			memcpy(addr, "Hi!", 3);				
			printf("Hi!\n");
			sleep(1);
			break;
		}
		
		sleep(0.1);
	}
	
	if ((shmdt(addr)) < 0){										//detach shm
		perror("shmdt");
		exit(1);
	}
	
	if (shmctl(shmid, IPC_RMID, NULL) < 0){								//remove shm segment						
		perror("shmctl");
		exit(1);
	}
	
	return 0;
}

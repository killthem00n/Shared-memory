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
	
	if ((shmid = shmget(key, SHMSIZE, 0)) < 0){							//connect to shm segment				
		perror("shmid");
		exit(1);
	}		
	
	addr = shmat(shmid, NULL, 0);									//attach shm to current process
	if(addr == (void *) -1){
		perror("shmat");
		exit(1);
	}
	
	memcpy(addr, "Hello!", 6);									//write "Hello!" to shm
	
	while (1){											//wait response from server 
		if (strcmp(addr, "Hi!") == 0){					
			printf("Hello!\n");
			sleep(1);
			break;
		}
	}
	
	if ((shmdt(addr)) < 0){										//detach shm
		perror("shmdt");
		exit(1);
	}
	
	return 0;
}

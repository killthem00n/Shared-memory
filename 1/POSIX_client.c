#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SHMSIZE 4096
#define PATH "/POSIX_server"

int main(void)
{
	int shmid;
	char *addr;
	
	if ((shmid = shm_open(PATH, O_RDWR, 0)) < 0){								//connect to shm object									
		perror("shm_open");
		exit(1);
	}		
	
	addr = mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);				//create new mapping in virtual address space									
	if(addr == (void *) -1){
		perror("mmap");
		exit(1);
	}
	
	memcpy(addr, "Hello!", 6);										//write "Hello!" to shm				
	
	while (1){										
		if (strcmp(addr, "Hi!") == 0){									//wait response from server 
			printf("Hello!\n");
			sleep(1);
			break;
		}
	}
	
	return 0;
}

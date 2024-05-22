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
	
	if ((shmid = shm_open(PATH, O_CREAT | O_RDWR, 0666)) < 0){	 					//create shm object									
		perror("shm_open");
		exit(1);
	}
	
	if ((ftruncate(shmid, SHMSIZE)) < 0){									//truncate object
		perror("ftruncate");
		exit(1);
	}
	
	addr = mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);				//create new mapping in virtual address space		
	if(addr == (void *) -1){
		perror("mmap");
		exit(1);
	}
		
	while (1){												//wait for client to connect					
		if (strcmp(addr, "Hello!") == 0){								//if client connected, send a response						
			memset(addr, 0, SHMSIZE);							
			memcpy(addr, "Hi!", 3);				
			printf("Hi!\n");
			sleep(1);
			break;
		}
		
		sleep(0.1);
	}
	
	if ((shm_unlink(PATH)) < 0){										//unlink shm object							
		perror("shm_unlink");
		exit(1);
	}
	
	if (munmap(addr, SHMSIZE) < 0){										//delete mapping															
		perror("munmap");
		exit(1);
	}
	
	return 0;
}

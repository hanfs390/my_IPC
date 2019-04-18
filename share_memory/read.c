#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#define KEY 5263

struct test {
	int nextid;
	char txt[64];
};


void main(void)
{
	int shmid = 0;
	struct test head;
	struct test *temp;
	/* get the head node */
	if((shmid = shmget(KEY, sizeof(struct test), 0664)) < 0){
		if(errno != EEXIST){
			perror("shmget error");
			return;
		}
	}
	if((temp = shmat(shmid, NULL, 0)) == ((void *) -1)){
		printf("temp = %p\n", temp);
	}
	printf("print txt = %s\n", temp->txt);
	
	while (temp->nextid != 0) {
		if((temp = shmat(temp->nextid, NULL, 0)) == ((void *) -1)){
			printf("temp = %p\n", temp);
		}
		printf("print txt = %s\n", temp->txt);
		
	}

}

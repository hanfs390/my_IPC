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

void init_test_list(struct test *h)
{
	int shmid;
	struct test *temp;
	if((shmid = shmget(KEY, sizeof(struct test), IPC_CREAT|IPC_EXCL|0664)) < 0){
		if(errno != EEXIST){
			perror("shmget error");
			return;
		} else {
			shmid = shmget(KEY, sizeof(struct test), 0664);
		}
	}
	if((temp = shmat(shmid, NULL, 0)) > 0){
		printf("temp = %p\n", temp);
	}
	temp->nextid = 0;
	strcpy(temp->txt,"head");
	h->nextid = shmid;
}

void add_test_node(struct test *h, char *txt)
{
	struct test *head;
	struct test *temp;
	int shmid;
	/* get head node */
	if((head = shmat(h->nextid, NULL, 0)) > 0){
		printf("get head pointer %p\n", head);
	}
	if ((shmid = shmget(0, sizeof(struct test), 0664)) < 0) {
		perror("shmget error");
		return;
	
	}

	printf("new node id %d\n", shmid);

	if ((temp = shmat(shmid, NULL, 0)) > 0) {
		printf("get new node pointer %p\n", temp);
	}

	temp->nextid = head->nextid;
	strcpy(temp->txt, txt);
	head->nextid = shmid;
}

void main(void)
{
	int shmid = 0;
	struct test head;
	init_test_list(&head);
	add_test_node(&head, "hello");
	add_test_node(&head, "world");
	

}

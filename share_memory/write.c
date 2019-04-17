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
void del_test_node(struct test *h, char *txt)
{
	struct test *temp;
	struct test *prev;
	int shmid;
	/* get head node */
	if((temp = shmat(h->nextid, NULL, 0)) > 0){
		printf("get head pointer %p\n", temp);
	}

	while (temp->nextid != 0) {
		prev = temp;
		if ((temp = shmat(temp->nextid, NULL, 0)) > 0) {
			printf("get node pointer %p\n", temp);
		}
		if (!strcmp(temp->txt, txt)) {
			printf("find the node that you want to del\n");
			shmid = prev->nextid;
			prev->nextid = temp->nextid;
			shmctl(shmid, IPC_RMID, 0);
			return;
		}
	}
}

void main(void)
{
	int shmid = 0;
	struct test head;
	init_test_list(&head);
	add_test_node(&head, "hello");
	add_test_node(&head, "world");
	add_test_node(&head, "OK");
	add_test_node(&head, "hanfushun");

//	del_test_node(&head, "OK");	

}

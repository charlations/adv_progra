#include "header.h"

void a_producer(char* program) {
	int semid, shmid, i, j, k;
	Buffer *b;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}
	
	if ( (semid = semget(key, 3, 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	if ( (shmid = shmget(key, sizeof(Buffer), 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}
	b = (Buffer*) shmat(shmid, (void*) 0, 0);
	
	srand( getpid() );
	for (k = 0; k < 10; k++) {
		i = (rand() % 5) + 1;
		printf("Producer %i trying to put %i items\n", getpid(), i);
		sem_wait(semid, FREESPACES, i);
		
		printf("Producer %i trying to get the lock over the buffer.\n", getpid());
		sem_wait(semid, MUTEX, 1);
		printf("Producer %i got the lock.\n", getpid());
		printf("Producer %i is putting the following products: ", getpid());
		for (j = 0; j < i; j++) {
			b->data[b->next] = (rand() % 3) + 1;
			printf("%3i", b->data[b->next]);
			b->next++;
		}
		printf("\n");
		
		printf("buffer = [");
		for (j = 0; j < b->next; j++) {
			printf("%3i", b->data[j]);
		}
		printf("]\n");
		sem_signal(semid, MUTEX, 1);
		printf("Producer %i has released the lock.\n", getpid());
		
		sem_signal(semid, ITEMS, i);
		
		sleep(rand() % 10 + 1);
	}
	
	shmdt(b);
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount, pid, i;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s amount\n", argv[0]);
		return -1;
	}
	
	amount = atoi(argv[1]);
	if (amount < 1) {
		fprintf(stderr, "%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			a_producer(argv[0]);
		} else {
			//do_nothing
		}
	}
	
	return 0;
}

#include "header.h"

void bee(char* program) {
	int porcion, disponible, semid;
	key_t key;

	if ( (key = ftok("/dev/null", 99)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}
	if ( (semid = semget(key, 4, 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}

	srand( getpid() );
	while(1) {
		printf("Bee %i trying to access beeMutex.\n", getpid());
		sem_wait(semid, abejasMutex, 1);
		printf("Bee %i accessed beeMutex.\n", getpid());

		porcion = (rand() % 3) + 1;
		printf("Bee %i trying to place %i portions.\n", getpid(), porcion);
		disponible = semctl(semid, espacios, GETVAL, 0);
		printf("%i espacios disponibles.\n", disponible);
		if (porcion >= disponible) {
			sem_wait(semid, espacios, disponible);
			sem_signal(semid, porciones, disponible);
			printf("Pot full. Waking bear...\n");
			sem_signal(semid, oso, 1);
		} else {
			sem_wait(semid, espacios, porcion);
			sem_signal(semid, porciones, porcion);
			printf("Bee %i left beeMutex.\n", getpid());
			sem_signal(semid, abejasMutex, 1);
		}
		sleep(3);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int n, pid, i;

	if (argc != 2) {
		fprintf(stderr, "usage: %s numAbejas\n", argv[0]);
		return -1;
	}

	n = atoi(argv[1]);
	if (n < 1) {
		fprintf(stderr, "%s: numBees must be a positive integer greater than 0\n", argv[0]);
		return -1;
	}

	for (i = 0; i < n; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			bee(argv[0]);
		} else {
			//doNothing
		}
	}

	return 0;
}
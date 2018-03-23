#include "header.h"

int main(int argc, char* argv[]) {
	int semid, size;
	key_t key;
	int impValor;

	if(argc != 2) {
		fprintf(stderr, "usage: %s sillas\n", argv[0]);
		return -1;
	}
	size = atoi(argv[1]);
	if (size < 1) {
		fprintf(stderr, "%s: Size must be a positive integer\n", argv[0]);
		return -1;
	}

	if ( (key = ftok("/dev/null", 99)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (semid = semget(key, 4, 0666 | IPC_CREAT)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, BARBERO, SETVAL, 0);
	semctl(semid, DISPONIBLES, SETVAL, size);
	semctl(semid, OCUPADAS, SETVAL, 0);
	
	impValor = semctl(semid, MUTEX, GETVAL, 0);
	printf("MUTEX: %i", impValor);
	impValor = semctl(semid, BARBERO, GETVAL, 0);
	printf("BARBERO: %i", impValor);
	impValor = semctl(semid, DISPONIBLES, GETVAL, 0);
	printf("DISPONIBLES: %i", impValor);
	impValor = semctl(semid, OCUPADAS, GETVAL, 0);
	printf("OCUPADAS: %i\n", impValor);
	return 0;
}
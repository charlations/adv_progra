#include "header.h"

int main (int argc, char* argv[]) {
	int p, semid;
	key_t key;
	int value;

	if (argc != 2) {
		fprintf(stderr, "usage: %s portions\n", argv[0]);
		return -1;
	}

	p = atoi(argv[1]);
	if (p < 1) {
		fprintf(stderr, "%s: portions must be a positive integer greater than 0\n", argv[0]);
		return -1;
	}
	printf("p = %i\n", p);

	if ( (key = ftok("/dev/null", 99)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (semid = semget(key, 4, 0666 | IPC_CREAT)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	semctl(semid, oso, SETVAL, 0);
	semctl(semid, abejasMutex, SETVAL, 1);
	semctl(semid, espacios,	 SETVAL, p);
	semctl(semid, porciones,	 SETVAL, 0);

	value = semctl(semid, oso, GETVAL, 0);
	printf("oso: %i\n", value);
	value = semctl(semid, abejasMutex, GETVAL, 0);
	printf("abejasMutex: %i\n", value);
	value = semctl(semid, espacios, GETVAL, 0);
	printf("espacios: %i\n", value);
	value = semctl(semid, porciones, GETVAL, 0);
	printf("porciones: %i\n", value);
	return 0;
}
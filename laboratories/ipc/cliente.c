#include "header.h"

void a_client(char* program) {
	int i, semid, disponibles;
	key_t key;

	if ( (key = ftok("/dev/null", 99)) == (key_t) -1 ) {
		perror(argv[0]);
		exit(-1);
	}
	if ( (semid = semget(key, 4, 0666)) < 0 ) {
		perror(argv[0]);
		exit(-1);
	}

	while(1) {
		printf("Client %i trying to enter the barber shop\n", getpid());
		disponibles = semctl(semid, DISPONIBLES, GETVAL, 0);
		if(disponibles > 0){
			//TODO
		}
	}

	exit(0);
}

int main(int argc, char* argv[]) {
	int tRegreso, pid, i;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s tRegreso\n", argv[0]);
		return -1;
	}
	
	tRegreso = atoi(argv[1]);
	if (tRegreso < 1) {
		fprintf(stderr, "%s: tRegreso must be a positive integer.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < 5; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			a_client(argv[0]);
		} else {
			//do_nothing
		}
	}
	
	return 0;
}

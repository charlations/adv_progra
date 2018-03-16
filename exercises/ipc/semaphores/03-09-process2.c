#include "03-09-header.h"

int main(int argc, char* argv[]) {
	int semid, i, num;
	key_t key;

	/* Genera el token hash */
	if ( (key = ftok("/dev/null", 99)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	/* Ya no se está creando el semáforo, se está recuperando del token, por eso
			ya no se usa el "| IPC_CREAT" */
	if ( (semid = semget(key, 2, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	i = 0;
	srand(getpid());
	while (i < 15) {
		sem_wait(semid, PROCESS_2, 1);

		num = rand() % 3 + 1;
		printf("PROCESS_2 - PID = %i - SLEEP = %i - i = %i\n", getpid(), num, i);

		sem_signal(semid, PROCESS_1, 1);
		sleep(num);
		i++;
	}

	return 0;
}
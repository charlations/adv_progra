#include "header.h"

int main(int argc, char* argv[]) {
  int semid, disponible;
  key_t key;
  int come;
  
  if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}
	if ( (key = ftok("/dev/null", 99)) == (key_t) -1 ) {
		perror(argv[0]);
		exit(-1);
	}
	if ( (semid = semget(key, 4, 0666)) < 0 ) {
		perror(argv[0]);
		exit(-1);
	}

  

  while(1) {
    sem_wait(semid, oso, 1);
    come = 1;
    printf("\t\tThe bear is awake. He is eating\n");
    while (come) {
    	sem_wait(semid, porciones, 1);
    	sem_signal(semid, espacios, 1);
    	disponible = semctl(semid, porciones, GETVAL, 0);
    	if(disponible == 0) {
    		come = 0;
    	}
    }
    sleep(2);
    printf("\t\tThe bear finished eating.\n");
    sem_signal(semid, abejasMutex, 1);
  }
  
  
  return 0;
}
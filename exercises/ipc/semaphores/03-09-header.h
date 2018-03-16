#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PROCESS_1 0
#define PROCESS_2 1

int sem_wait(int semid, int semnum, unsigned int val) {
	struct sembuf op;
	
	op.sem_num = semnum;
	op.sem_op = -val;
	op.sem_flg = 0; 
	return semop(semid, &op, 1);
}

/* semop SÍ es atómico, recibe una operación o un arreglo de operaciones a hacer
		al arreglo de semáforos (el arreglo de semáforos es el semid) */

int sem_signal(int semid, int semnum, unsigned int val) {
	struct sembuf op;
	
	op.sem_num = semnum;
	op.sem_op = val;
	op.sem_flg = 0; 
	return semop(semid, &op, 1);
}

/*
int sem_signal_aareglos(int semid, int* semnum, unsigned int val) {
	struct sembuf op[3];

	op[1].sem_num = semnum[];
	op[1].sem_op = val;
	op[1].sem_flg = 0;
	// -------------------- 
	op[2].sem_num = semnum[];
	op[2].sem_op = -val;
	op[2].sem_flg = nowait;
	// -------------------- 
	op[3].sem_num = semnum[];
	op[3].sem_op = 2 * val;
	op[3].sem_flg = 0;
	// -------------------- 
	return semop(semid, op, 3);
}
*/

#endif

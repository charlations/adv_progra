#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void execute(char* order1, char* order2, char* order3, char* program) {
	int p1p2[2], p2p3[2];
	int pid;
	
	if (pipe(p1p2) < 0) {
		perror(program);
		exit(-1);
	}
	if ( (pid = fork()) < 0 ) {
		perror(program);
		exit(-1);
	} else if (pid == 0) { //PA
	
		if (pipe(p2p3) < 0) {
			perror(program);
			exit(-1);
		}
		if ( (pid = fork()) < 0 ) {
			perror(program);
			exit(-1);
		} else if (pid == 0) { //CHILD
			close(1); //dejar de usar el fd[1] (stdout)
			dup(p2p3[1]); /*devuelve 1, porque el número más pequeño disponible 
											es el recien liberado. Entonces el proceso realmente 
											no se da cuenta que se cambió su stdout */
			close(p2p3[0]);	/* Como ya se estableció la tubería, ya no se necesitan
													los file descriptors que se usaron para mandar a 
													llamar el pipe. No se cierra el pipe, sino el file
													descriptor que hizo la referencia. */
			close(p2p3[1]);
			execlp(order1, order1, (char*) 0);
		} else { 
			close(0);
			dup(p2p3[0]);
			close(p2p3[0]);
			close(p2p3[1]);
			
			close(1);
			dup(p1p2[1]);
			close(p1p2[0]);
			close(p1p2[1]);
			
			execlp(order2, order2, (char*) 0);
		}
		
	} else { // GRANDPA
		close(0);	//cierra la entrada estandar
		dup(p1p2[0]);	/* En este momento hay 2 cosas manejando la entrada del
											proceso: la stdin Y el file descriptor. Por eso se
											cierra luego de esta instrucción. */
		close(p1p2[0]);
		close(p1p2[1]);
		execlp(order3, order3, (char*) 0);
	}
	}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		fprintf(stderr, "usage: %s order1 order2 order3\n", argv[0]);
		return -1;
	}
	
	execute(argv[1], argv[2], argv[3], argv[0]);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h> /* strcpy */

char* inst1;
char* inst2;
char* inst3;

void handler(int signal) {
	int pid;

	if( (pid = fork()) < 0 ) {
		perror("Fork");
		exit(-1);
	} else if (pid == 0) {
		//es hijo
		switch(signal) {
			case 1 : execlp(inst1, inst1, (char*) 0); break;
			case 2 : execlp(inst2, inst2, (char*) 0); break;
			case 3 : execlp(inst3, inst3, (char*) 0); break;
			/*case 1 : printf("Recibió señal 1 - %s\n", inst1); exit(0); break;
			case 2 : printf("Recibió señal 2 - %s\n", inst2); exit(0); break;
			case 3 : printf("Recibió señal 3 - %s\n", inst3); exit(0); break;*/
		}
	} else {
		wait(NULL);
	}
}

int main(int argc, char* argv[]) {
	//char aux1[4],  aux2[4],  aux3[4];
	signal(1, handler);
	signal(2, handler);
	signal(3, handler);

	if(argc > 4) {
		fprintf(stderr, "usage: %s order1 order2 order3\n", argv[0]);
		return -1;
	}

	/*strcpy(aux1, "ls");
	inst1 = aux1;
	strcpy(aux2, "ps");
	inst2 = aux2;
	strcpy(aux3, "pwd");
	inst3 = aux3;*/
	inst1 = "ps";
	inst2 = "ls";
	inst3 = "pwd";

	if(argc = 2) {
		inst1 = argv[1];
	}
	if(argc >= 3) {
		inst2 = argv[2];
	}
	if(argc == 4) {
		inst3 = argv[3];
	}

	while(1) {
		printf("PID %i waiting for a signal.\n", getpid());
		pause();
	}

}
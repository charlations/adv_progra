#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> /* wait(NULL); */

void desciende(int childs, int n, char* program) {
	int i, pid;
	
	if(n <= childs) {
		for(i = 0; i < n; i++) {
			printf("\t");
		}
		printf("PPID = %i PID = %i NIVEL = %i\n", getppid(), getpid(), n);
		n++;
		for(i = 0; i < n; i++) {
			if( (pid = fork()) < 0 ) {
				perror(program);
				exit(-1);
			} else if(pid == 0) {
				//es hijo
				desciende(childs, n, program);
			} else {
				//es padre
				wait(NULL);
			}
		}
	}

	exit(0);
}

int main(int argc, char* argv[]) {
	int pid, childs;

	if(argc != 2) {
		fprintf(stderr, "Usage: %s childs\n", argv[0]);
		return -1;
	}
	childs = atoi(argv[1]);
	if(childs <= 0) {
		fprintf(stderr, "%s: childs must be a number greater than 0\n", argv[0]);
		return -1;
	}

	//printf("childs: %i", childs);
	desciende(childs, 0, argv[0]);

	return 0;
}
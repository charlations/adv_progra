#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> /* sleep(n)? */
#include <string.h> /* NAME_MAX */
#include <dirent.h>

void rota(char *directory, char *log_name, int num, int t, char *program) {
	char filename[NAME_MAX + 1];
	DIR *dir;
	struct dirent *dir_entry;

	if( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}
}

int main(int argc, char* argv[]) {
	//rotacion directorio_log archivo_log numero tiempo
	int num, int t;

	if( argc != 5 ) {
		fprintf(stderr, "usage: %s log_directory log_name num time\n", argv[0]);
		return -1;
	}

	num = atoi(argv[3]);
	if( num <= 0 ) {
		fprintf(stderr, "%s: num must be a whole number greater than 0\n", argv[0]);
		return -1;
	}
	t = atoi(argv[4]);
	if( t <= 0 ) {
		fprintf(stderr, "%s: time must be a whole number greater than 0\n", argv[0]);
		return -1;
	}

	rota(argv[1], argv[2], num, t, argv[0]);

	return 0;
}
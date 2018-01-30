#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h> 		/* name_max */
#include <string.h>

#define ARCHIVO 0
#define DIRECTORIO 1
#define PIPA 2
#define LINK 3


char* tipos[] = {"REG", "DIR", "FIFO", "LNK"};
int cantidad[4];

void initCont() {
	int i;
	for(i = 0; i < 4; i++) {
		cantidad[i] = 0;
	}
}

void printStats() {
	int total = 0;
	int i, por;

	for(i = 0; i < 4; i++) {
		total += cantidad[i];
	}
	printf("TIPO 				PORCENTAJE\n");

	for(i = 0; i < 4; i++) {
		if(cantidad[i]) {
			por = (int)((double)cantidad[i] / (double)total * 100);
			printf("%s 				%i\n", tipos[i], por);
		}
	}
	printf("\n");
}


void list(char *directory, char *program) {
	char filename[NAME_MAX + 1];
	DIR *dir;
	struct dirent *dir_entry;
	struct stat ss;

	if( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}

	initCont();
	//Imprime cada archivo dentro de ese folder
	printf("Directorio %s:\n", directory);
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		
		/* AQUÍ EMPIEZA LA ESTADÍSTICA 
		https://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html */
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
		stat(filename, &ss);
		if (S_ISREG(ss.st_mode)) {
			cantidad[ARCHIVO]++;
		}
		else if (S_ISDIR(ss.st_mode)) {
			cantidad[DIRECTORIO]++;
		}
		else if (S_ISFIFO(ss.st_mode)) {
			cantidad[PIPA]++;
		}
		else if (S_ISLNK(ss.st_mode)) {
			cantidad[LINK]++;
		}
	}
	printStats();

	//ahora entra a cada directorio (ﾉ◕ヮ◕)ﾉ*:・ﾟ✧RECURSIVIDAD✧°・:*
	rewinddir(dir);
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
		stat(filename, &ss);
		if (S_ISDIR(ss.st_mode)) {
			list(filename, program);
		}
	}

	closedir(dir);
}

int main (int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;

	if(argc > 2) {
		fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
		return -1;
	}
	strcpy(dir_name, ".");
	directory = dir_name;
	if (argc == 2) {
		directory = argv[1];
	}

	list(directory, argv[0]);
	return 0;
}
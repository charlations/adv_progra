#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>		/* ss */
#include <dirent.h>			/* name_max y demás */
#include <fcntl.h> 			/* O_RDONLY */
#include <string.h> 		/* strcmp */

/*void minmax(char* filename, char *program) {
	int num, min, max, cont;
	FILE *file;

	file = fopen(filename, "r");
	if(file == NULL) {
		perror(program);
		exit(-1);
	}

	//fclose(file);
	//printf("Abrí archivo %s\n", filename);

	min = 0; max = 0; cont = 0;
	while( fscanf(file, "%i", &num) != EOF ) {
		if(num < min) {
			min = num;
		}
		if(num > max) {
			max = num;
		}
		cont++;
	}

	fclose(file);

	if(!cont) {
		printf("filename = %s - min = %i - max = %i\n", filename, min, max);
	}
}*/

void minmax(char* filename, char *program) {
	int num, min, max, cont;
	int fd, bytes;
	char buffer;

	if( (fd = open(filename, O_RDONLY)) < 0 ) {
		perror(program);
		exit(-1);
	}
	//printf("Abrí archivo %s\n", filename);
	min = 0; max = 0; cont = 0;
	while( (bytes = read(fd, &num, sizeof(int))) != 0 ) {
		if(num < min) {
			min = num;
		}
		if(num > max) {
			max = num;
		}
		cont++;
	}
	//printf("Se leyó el archivo %s con %i datos\n", filename, cont);
	if(cont > 0) {
		printf("filename = %s - min = %i - max = %i\n", filename, min, max);
	}

	close(fd);
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

	printf("%s:\n", directory);

	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
		stat(filename, &ss);
		if (S_ISREG(ss.st_mode)) {
			minmax(filename, program);
			//printf("filename = %s\n", filename);
		}
	}
	printf("\n");

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

int main(int argc, char* argv[]) {
	char *directory;

	if(argc != 2) {
		fprintf(stderr, "Usage: %s directory\n", argv[0]);
		return -1;
	}
	directory = argv[1];

	list(directory, argv[0]);
	return 0;
}
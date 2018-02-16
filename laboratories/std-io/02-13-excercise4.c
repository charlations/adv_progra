#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h> 		/* name_max */
#include <sys/stat.h>
#include <string.h>

float min, max;

float normalize(float num) {
	return (num - min)/(max - min);
}

void write_values(char *filename, char *newfilename, char *program) {
	FILE *file, *newFile;
	int time, intersections, size, i;
	float num;

	file = fopen(filename, "r");
	if (file == NULL) {
		perror("fopen file");
		exit(-1);
	}
	newFile = fopen(newfilename, "w");
	if (newFile == NULL) {
		perror("fopen newFile");
		exit(-1);
	}

	while( fscanf(file, "%i,%i", &time, &intersections) != EOF ) {
		fprintf(newFile, "%i,%i", time, intersections );
		size = intersections * intersections;
		for (i = 0; i < size; i++) {
			fscanf(file, ",%f", &num);
			fprintf(newFile, ",%.2f", normalize(num));
		}
		fprintf(newFile, "\n");
	}

	fclose(file);
	fclose(newFile);
}

void read_values(char *filename, char *program) {
	FILE *file;
	int time, intersections, size, i;
	float num;

	file = fopen(filename, "r");
	if (file == NULL) {
		perror("fopen");
		exit(-1);
	}

	while( fscanf(file, "%i,%i", &time, &intersections) != EOF ) {
		size = intersections * intersections;
		for (i = 0; i < size; i++) {
			fscanf(file, ",%f", &num);
			if( num < min ) {
				min = num;
			}
			if( num > max ) {
				max = num;
			}
		}
	}

	fclose(file);
}

void recorre(char *directory, char *newDirectory, char *program) {
	char filename[NAME_MAX + 1], newfilename[NAME_MAX + 1];
	DIR *dir;
	struct dirent *dir_entry;

	if( (dir = opendir(directory)) == NULL ) {
		perror("opendir");
		exit(-1);
	}
	//printf("Se abrió el directorio %s\n", directory);

	min = 0; max = 0;

	while( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
		read_values(filename, program);
	}
	//printf("Min = %f | Max = %f\n", min, max);

	rewinddir(dir);

	while( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
		sprintf(newfilename, "%s/%s", newDirectory, dir_entry->d_name);	
		write_values(filename, newfilename, program);
	}

}

int main (int argc, char* argv[]) {
	char *directory, *newDirectory;
	struct stat ss;

	if(argc != 3) {
		fprintf(stderr, "Usage: %s directory newDirectory\n", argv[0]);
		return -1;
	}
	directory = argv[1];
	newDirectory = argv[2];
	if (stat(newDirectory, &ss) == -1) {
        mkdir(newDirectory, 0700);
        //printf("Creo directorio nuevo\n");
    }
	recorre(directory, newDirectory, argv[0]);

	return 0;
}
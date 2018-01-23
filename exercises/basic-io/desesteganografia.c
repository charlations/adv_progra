#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

/* Esto solo funciona para archivos de unix. Sirve para archivos de UNIX
	https://es.wikipedia.org/wiki/Windows_bitmap
	Información de los archivos tipo '.bmp'. Te dice en qué bit está qué información

*/

int main(int argc, char* argv[]) {
	int fd;
	unsigned int width, height;
	unsigned short int  colordepth;
	unsigned long length_array;
	char *img, *text;
		
	if (argc != 2) {
		fprintf(stderr, "usage: %s image_file\n", argv[0]);
		return -1;
	}
	
	if ( (fd = open(argv[1], O_RDWR)) < 0 ) {
		perror(argv[1]);
		return -1;
	}
	
	lseek(fd, 18, SEEK_SET);	//Bytes 18 - 21 = Anchura, 22 - 25 = Altura
	read(fd, &width, sizeof(unsigned int));
	read(fd, &height, sizeof(unsigned int));
	
	lseek(fd, 2, SEEK_CUR);		//Bytes 28 y 29 = Tamaño de cada punto
	read(fd, &colordepth, sizeof(unsigned short int));
	
	if (colordepth != 24) {
		fprintf(stderr, "%s: the image must be RGB\n", argv[0]);
		close(fd);
		return -1;
	}
	
	lseek(fd, 54, SEEK_SET);	//ya el primer Byte que no es de la cabezera, ya es la imagen

	length_array = width * height * 3 * sizeof(char); //porque RGB ocupa 3 bytes por pixel
	img = (char*) malloc(length_array);
	read(fd, img, length_array);

	int i;
	//for (i = 1; i < length_array; i += 3) {
	for (i = 1; img[i] != '\0'; i += 3) {
		printf("%c", img[i]);
	}
	
	close(fd);
			
	return 0;
}
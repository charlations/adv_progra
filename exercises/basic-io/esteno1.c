#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

/* Esteganografía simple: ocultar un archivo de texto dentro de un archivo de imágen
	Por cada ciertos bytes, sustituir uno nada más.
	Entrada: Archivo de imagen y archivo del texto que voy a ocultar.

	Esto solo funciona para archivos de unix. Sirve para archivos de UNIX
	https://es.wikipedia.org/wiki/Windows_bitmap
	Información de los archivos tipo '.bmp'. Te dice en qué bit está qué información

*/

int main(int argc, char* argv[]) {
	int fd_img, fd_text;
	unsigned int width, height;
	unsigned short int  colordepth;
	unsigned long length_text, length_img;
	char *img, *text;
		
	if (argc != 3) {
		fprintf(stderr, "usage: %s image_file text_file\n", argv[0]);
		return -1;
	}
	
	if ( (fd_img = open(argv[1], O_RDWR)) < 0 ) {
		perror(argv[1]);
		return -1;
	}
	
	if ( (fd_text = open(argv[2], O_RDONLY)) < 0 ) {
		perror(argv[2]);
		return -1;
	}
	
	lseek(fd_img, 18, SEEK_SET);	//Bytes 18 - 21 = Anchura, 22 - 25 = Altura
	read(fd_img, &width, sizeof(unsigned int));
	read(fd_img, &height, sizeof(unsigned int));
	
	lseek(fd_img, 2, SEEK_CUR);		//Bytes 28 y 29 = Tamaño de cada punto
	read(fd_img, &colordepth, sizeof(unsigned short int));
	
	if (colordepth != 24) {
		fprintf(stderr, "%s: the image must be RGB\n", argv[0]);
		close(fd_img);
		close(fd_text);
		return -1;
	}
	
	length_text = lseek(fd_text, 0, SEEK_END); //regresa el tamaño del archivo del texto
	length_img = width * height;
	if ((length_img - 1) < length_text) {
		fprintf(stderr, "%s: the text must have the same or smaller size of the image\n", argv[0]);
		close(fd_img);
		close(fd_text);
		return -1;
	}
	
	lseek(fd_text, 0, SEEK_SET);
	lseek(fd_img, 54, SEEK_SET);	//ya el primer Byte que no es de la cabezera, ya es la imagen
	
	img = (char*) malloc(length_img * 3 * sizeof(char));
	text = (char*) malloc(length_text * sizeof(char));
	
	read(fd_img, img, length_img * 3 * sizeof(char));
	read(fd_text, text, length_text * sizeof(char));
	
	int i = 1, j = 0;
	for (j = 0; j < length_text; j++) {
		img[i] = text[j];
		i += 3;
	}
	img[i] = '\0';
	
	lseek(fd_img, 54, SEEK_SET);
	write(fd_img, img, length_img * 3 * sizeof(char));
	
	close(fd_img);
	close(fd_text);
			
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> //O_RDONLY

#define SIZE 128

int main(int argc, char* argv[]) {
	int n, bytes;
	int fd_in, fd_out;
	char *buff_desplaz;
	char buffer[SIZE];

	if (argc != 4) {
		fprintf(stderr, "Usage: %s num origen dest\n", argv[0]);
		return -1;
	}

	n = atoi(argv[1]);
	if (n < 0) {
		fprintf(stderr, "%s: el primer parametro debe se un numero entero positivo\n", argv[0]);
		return -1;
	}

	if ( (fd_in = open(argv[2], O_RDONLY)) < 0 ) {
		fprintf(stderr, "%s: el archivo %s no existe\n", argv[0], argv[2]);
		return -1;
	}

	if ( (fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[3]);
		return -1;
	}

	n = n % 128; //por si mandan un número mayor que 128...
	buff_desplaz = (char*) malloc(n * sizeof(char));

	while ( (bytes = read(fd_in, &buffer, 128 * sizeof(char))) != 0 ) {
		if (bytes == 128) {
			buff_desplaz = buffer;
			write(fd_out, &buffer[n], bytes - n);
			write(fd_out, buff_desplaz, n);
		} else {
			write(fd_out, buffer, bytes);
		}
	}


	close(fd_in);
	close(fd_out);
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	int fd, bytes;
	char buffer;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s file\n", argv[0]);
		return -1;
	}
	
	if ( (fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	buffer = 'a';
	lseek(fd, 1048576, SEEK_SET);
	write(fd, &buffer, sizeof(char));
	
	close(fd);
	/* Sin el close, el buffer no hace su 'auto-flush', entonces el buffer - que solo se escribe
		cuando se llena - no se va a escribir si no se llenó. Se trunca lo último de un archivo.
		Con 'close(fd)', se acaba de escribir lo que estaba en el buffer, entonces es OTRA razón 
		por la cual siempre usarlo.
	*/
	return 0;
}

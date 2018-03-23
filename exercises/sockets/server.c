#include "header.h"
#include <string.h>
#include <time.h>

void serves_client(int nsfd) {
	int number_sent, number_guess;
	int guess, answer;
	
	srand(getpid());
	//srand(time(0));
	do {
		guess = 0;
		number_guess = (rand() % 100) + 1;
		printf("PID = %i <-> number to  guess = %i\n", getpid(), number_guess);
		while (!guess) {
			read(nsfd, &number_sent, sizeof(number_sent));
			if (number_sent < number_guess) {
				answer = SMALLER;
			} else if (number_sent > number_guess) {
				answer = BIGGER;
			} else {
				answer = EQUAL;
				guess = 1;
			}
			write(nsfd, &answer, sizeof(answer));
		}
		read(nsfd, &answer, sizeof(answer));
	} while (answer == CONTINUE);
	close(nsfd);
}

void server(char* ip, int port, char* program) {
	/* Estos pasos siempre son los mismos, porque el protocolo de comunicación 
	casi no cambia. (?) Nos mostró que en su tesis, igual usa un proceso muy
	igual, solo cambió porque está escrito en python. */
	int sfd, nsfd, pid;
	struct sockaddr_in server_info, client_info;

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	/* 															ⴿ
									conjunto de familia de funciones que traducen la ip a bits
									inet cambia de string (ip de 4 puntos o dominio) a bits 
									También hay una función que traduce de bits a ip de 4
									puntos, hace la petición de DNS y si tiene dominio, regresa 
	*/
	server_info.sin_port = htons(port);
	/* Little endian o big endian traducido a representación de network 
			Hardware TO Network System
	*/
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		/* Los errores suelen ser si se usan puertos de 0 - 1023, a menos de que
				sea el root quien lo solicita. */
		perror(program);
		exit(-1);
	}
	
	listen(sfd, 1); /* (socket, máximo de fila de espera)
											TCP socket error 87 - too many users 
											HTTP error 503 - server too busy */
	while (1) {
		int len = sizeof(client_info);
		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			/* accept es el único punto donde el servidor puede obtener la
				 información del cliente. (Luego se puede solicitar información
				 extra, pero el protocolo es ese. ) */
			/* New Socket File Descriptor, porque el anterior solo se usa para
				 escuchar */
			perror(program);
			exit(-1);
		}
		
		/* CONCURRENTE*/
		if ( (pid = fork()) < 0 ) {
			perror(program);
		} else if (pid == 0) {
			close(sfd);
			/* El proceso hijo no debe estar escuchando para nuevas conexiones, 
				 entonces lo cierra para él. */
			serves_client(nsfd);
			exit(0);
		} else {
			close(nsfd);
			/* El servidor no va a atender al cliente, entonces él cierra el nsfd */
		}
		/*CONCURRENTE */
		
		//serves_client(nsfd); // ITERATIVO
	}
}

int main(int argc, char* argv[]) {
	char ip[15];	
	int port;
	
	strcpy(ip, DEFAULT_IP);
	port = DEFAULT_PORT;
	if (argc == 3) {
		if (strcmp(argv[1], "-d") == 0) {
			strcpy(ip, argv[2]);
		} else if (strcmp(argv[1], "-p") == 0) {
			port = atoi(argv[2]);
			if (port < 5000) {
        		printf("%s: The port must be greater than 5000.\n", argv[0]);
        		return -1;
        	}
		} else {
			printf("usage: %s [-d dir] [-p port]\n", argv[0]);
			return -1;
		}
	} else if (argc == 5) {
		if (strcmp(argv[1], "-d") == 0) {
			strcpy(ip, argv[2]);
			if (strcmp(argv[3], "-p") == 0) {
				port = atoi(argv[4]);
				if (port < 5000) {
            		printf("%s: The port must be greater than 5000.\n", argv[0]);
            		return -1;
            	}		
			} else {
				printf("usage: %s [-d dir] [-p port]\n", argv[0]);
				return -1;
			}			
		} else if (strcmp(argv[1], "-p") == 0) {
			port = atoi(argv[2]);
			if (port < 5000) {
        		printf("%s: The port must be greater than 5000.\n", argv[0]);
        		return -1;
        	}		
			if (strcmp(argv[3], "-d") == 0) {
				strcpy(ip, argv[4]);
			} else {
				printf("usage: %s [-d dir] [-p port]\n", argv[0]);
				return -1;
			}
		} else {
			printf("usage: %s [-d dir] [-p port]\n", argv[0]);
			return -1;
		}
	} else if (argc != 1) {
		printf("usage: %s [-d dir] [-p port]\n", argv[0]);
		return -1;
	}
	
	server(ip, port, argv[0]);
	
	return 0;
}	
		
		
		
		
		


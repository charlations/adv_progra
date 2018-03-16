#include "03-09-header.h"

int main(int argc, char* argv[]) {
	int semid;
	key_t key;

	/* Genera el token hash */
	if ( (key = ftok("/dev/null", 99)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	/* Está CREANDO, no recuperando un semáforo ya existente dentro de ese token.
			Si se estuviera recuperando sería sin el "| IPC_CREAT" */
	if ( (semid = semget(key, 2, 0666 | IPC_CREAT)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	/* es previo a la sincronización, entonces NO ES UNA OPERACIÓN ATÓMICA 
			Si se quiere hacer algo de sincronización, ya no se usa semctl */
	/* semctl GETVAL, como es de lectura, no hay problema si se usa dentro
			de la sincronización. Ejemplo en consumers.c línea 24 columna 83 */
	semctl(semid, PROCESS_1, SETVAL, 1);
	semctl(semid, PROCESS_2, SETVAL, 0);
	/*  										 GETVAL, &localidadDondeGuardar
													 GETALL, &arregloLocalidadCGuardad
												//Ejemplo de GETALL en buffer.c
													 SETALL, arregloNuevosValores);
													 IPC_STAT saca la información del semáforo
													 IPC_SET, nuevosPrivilegios);
													 IPC_RMID, 0); //borra el semáforo, EL 0 ES PORQUE NO 
													 							REQUIERE UN PARÁMETRO ADICIONAL
		
		man7.org/linux/man-pages/man2/semctl.2.html
	*/

	printf("\n");
	return 0;
}
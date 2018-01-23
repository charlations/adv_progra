/* /excercises/pointers/pointer0.c  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	long *p;
	long x = 10;
	
	/*
	p = (long*) malloc(sizeof(long));
	if (!p) {
		perror(argv[0]);
		exit(0);
	}
	*/
	p = &x;
	printf("x = %li\n", x);
	printf("&x = %p\n", (&x));
	printf("p = %p\n", p);
	
	(*p) = 10;
	printf("p = %p\n", p);
	printf("*p = %li\n", (*p));
	
	(*p)++;
	printf("p = %p\n", p);
	printf("*p = %li\n", (*p));
	
	*(++p) = 11;
	printf("p = %p\n", p);
	printf("*p = %li\n", (*p));
	
	//free(p);
	
	return 0;
}

/**
 * $ gcc 01-12-pointer0.c 
 * $ ./a.out
 * 	x = 10
 * 	&x = 0x7ffd158677a8
 * 	p = 0x7ffd158677a8
 * 	p = 0x7ffd158677a8
 * 	*p = 10
 * 	p = 0x7ffd158677a8
 * 	*p = 11
 * 	p = 0xb
 * 	Violación de segmento (`core' generado)
**/

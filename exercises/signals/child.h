#ifndef CHILD_H
#define CHILD_H
/* Aquí se definen todas las funciones que serán publicas/visibles. Las demás no 
		son invocables. */

int child_process(char* program, char* file_in, char* file_out, int pos, int block);

#endif 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* compile: gcc exercise1.c -lm */
/* execute: ./a.out < input.txt */

typedef struct point {
	float x, y;
} Point;

/* Variables globales. En esta variables se almacenaran
   la cantidad de elementos leidos (size) y los valores
   leídos (points) */
Point *points;
int size;

/* Regresa la distancia entre dos puntos dados (p1, p2) */
float distance(const Point *p1, const Point *p2) {
	return sqrt( ((p2->x - p1->x) * (p2->x - p1->x)) + 
		         ((p2->y - p1->y) * (p2->y - p1->y)) );
}

/* A IMPLEMENTAR
   Carga los datos de entrada entandar (consola). Primero 
   recibe la cantidad de elementos a leer (size), seguido por
   por 'size' pares de flotantes, los puntos a leer */
void load_data() {
	int i;
	float xx, yy;

	scanf("%i", &size);
	points = (Point*) malloc(size * sizeof(Point));

	for(i = 0; i < size; i++) {
		scanf("%f %f", &xx, &yy);
		points[i].x = xx;
		points[i].y = yy;
	}
}


/* A IMPLEMENTAR
   Regresa el apuntador al punto leído (points) más 
   cercano a p */
Point* get_near_from(const Point* p) {
	Point *near = points;
	float aux;
	float min = distance(p, near);
	int i;

	for(i = 1; i < size; i++) {
		aux = distance(p, &points[i]);
		if(min > aux) {
			near = &points[i];
			min = aux;
		}
	}

	//free(paux);

	return near;
}


/* A IMPLEMENTAR
   Regresa un arreglo de puntos (array) conteniendo los 
   últimos num elementos del arreglo points. */
Point* get_last_elements(int num) {
	Point* array = (Point*) malloc(num * sizeof(Point));
	memcpy(&array[0], &points[size - num], num * sizeof(Point));
	
	return array;
}


/* A IMPLEMENTAR
   Recibe un arreglo de puntos cualesquiera (array)y la cantidad 
   de elementos a desplegar (num). Despliega los elementos en el 
   formanto '(x, y)' */
void display_points(Point *array, int num) {
	int i;

	for(i = 0; i < num; i++) {
		printf("(%.2f, %.2f)\n", array[i].x, array[i].y);
	}
}

int main(int argc, char* argv[]) {
	Point p = {0.0, 0.0};
	Point *lasts, *near;

	load_data();
	printf("first 20: ");
	display_points(points, 20);

	near = get_near_from(&p);
	printf("near = (%4.2f, %4.2f)\n", near->x, near->y);
	
	lasts = get_last_elements(10);
	printf("last 10: ");
	display_points(lasts, 10);
	
	free(lasts);
	free(points);
	return 0;
}

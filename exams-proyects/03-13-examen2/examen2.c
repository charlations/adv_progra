/*
	Unas abejas amistosas están alimentando a un oso que se encuentra atrapado 
	recogiendo miel para él. La vida del oso atrapado es solo comer y dormir. 
	Hay N abejas y un oso. El tamaño de la olla es P porciones. Las abejas llevan 
	miel a una olla, entre 1 y 3 porciones por vez hasta que la olla está llena. 
	Cuando la olla está llena, la abeja que trajo la última porción despierta al 
	oso. El oso comienza a comer y las abejas hacen una pausa, esperando que el 
	oso se termine toda la miel. Cuando la olla está vacía, el oso se duermen de
	nuevo y las abejas comienzan a depositar miel nuevamente.
*/

/**
 * 5 puntos ¿De cuál problema de sincronización se trata?
 *	Es parecido al Barber Shop, donde las porciones de miel son las sillas, 
 *	pero una abeja puede llenar más de una silla.
 * 
 * 20 puntos Define la estrategia y mecanismos que emplearás la solución del
 * problema. Fundamenta, de manera clara y concisa, cada una de
 * tus decisiones.
 *	Semáforos:
 *		Abeja mutex
 *		Oso
 * 		Espacio
 * 		Porciones
 * 	La abeja toma el mutex y revisa si sus porciones de miel llenan la olla. 
 * 	Si lo hace, la llena y hace la señal de cuantas porciones sean necesarias
 * 	para llenar la olla (el resto se desperdicia) y llama al Oso. Mientras
 * 	hayan porciones, el oso se las come regresando Espacio a P y Porciones a 0,
 * 	y cuando esté vacío se vuelve a dormir y abre el abejaMutex.
 * 
 * 75 puntos Implementación del problema. Es importante que el comportamiento
 * de su solución sea idéntico al problema del examen y
 * que reciba como parámetros la cantidad de abejas, N, y la cantidad
 * de porciones de la olla, P. Deberás colocar un archivo de
 * texto explicando cómo debe ser probado tu programa. Si no
 * existe el archivo se aplicará una penalización de 20 puntos
 * aun cuando el problema sea correcto.
 *	gcc creaOlla.c -o olla
 *	gcc abejas.c -o abeja
 *	gcc oso.c -o oso
 *	
 *	./olla 8
 *	./oso &
 *	./abeja 3 &
**/
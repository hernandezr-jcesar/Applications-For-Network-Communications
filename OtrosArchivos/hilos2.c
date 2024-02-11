#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct datos{
	int entero;
	char cadena[20];
}dato;

void* holamundo(void* param){
	dato ejem = *(dato*) param;
	printf("Yo soy el hilo: %d\n",ejem.entero);
	printf("%s", ejem.cadena);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	if(argc != 2){
		printf("Hace falta indicar el numero de hilos a crear.");
		exit(-1);
	}
	//paso 1: Creacion de hilos
	int cuenta = atoi(argv[1]);
	pthread_t id[cuenta];
	dato datos[cuenta];

	for(int i=0; i<cuenta; i++){		
		datos[i].entero = i;
		strcpy(datos[i].cadena,"Hi, Hello\n");

		pthread_create(&id[i], NULL, holamundo, &datos[i]);
	}

	//paso 2: espera de los hilos hijos
	for(int j=0; j<cuenta; j++){
		pthread_join(id[j], NULL);
	}
	return 0;
}
//El padre manda una estructura al hijo, y este los imprime.
//retornar parametros del hilo hacia el padre
//estudiar todo lo de los archivos de texto
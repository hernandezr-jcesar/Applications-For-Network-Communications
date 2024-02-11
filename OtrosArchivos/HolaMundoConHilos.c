#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* holamundo(void* param){
	int ent = *(int*) param;
	printf("Yo soy el hilo: %d\n",ent );
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	if(argc != 2){
		printf("Hace falta indicar el numero de hilos a crear.");
		exit(-1);
	}
	//paso 1: Creacion de hilos
	int cuenta = atoi(argv[1]);
	pthread_t id[cuenta];
	int entero[cuenta];
	for(int i=0; i<cuenta; i++){
		entero[i] = i;
		pthread_create(&id[i], NULL, holamundo, &entero[i]);
	}

	//paso 2: espera de los hilos hijos
	for(int j=0; j<cuenta; j++){
		pthread_join(id[j], NULL);
	}
	return 0;
}
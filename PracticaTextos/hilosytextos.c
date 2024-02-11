#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_LARGO_PALABRA 50

char *palabras[MAX_LARGO_PALABRA];

struct Datos{
	int id;
	char nombrearchivo[MAX_LARGO_PALABRA];			
	int *contador;		
};

/*
void* holamundo(void* param){
	struct Datos *ejem = (struct Datos*) param;
	
	printf("Yo soy el hilo: %d\n",ejem->id);
	printf("Archivo: %s\n",ejem->nombrearchivo);

	return NULL;

}
*/


int main(int argc, char **argv)
{
	if(argc != 3){
		printf("Hace falta indicar el numero de hilos a crear, el numero de archivos, asi como el numero de palabras\n");
		exit(-1);
	}

	//paso 1: Creacion de hilos
	int cuenta = atoi(argv[1]);
	int cantpalabras = atoi(argv[2]);	

	//Se crea el array de objetos datos con asignacion de memoria
	struct Datos datos[cuenta];

	for(int i=0; i<cuenta; i++){
		printf("Introduce el nombre del archivo #%d: \n", i+1);
		scanf("%s",datos[i].nombrearchivo);
		datos[i].id = i;
	}
	for(int j=0; j<cantpalabras; j++){
		printf("Introduce la palabra #%d: \n", j+1);
		palabras[j] = malloc(sizeof(MAX_LARGO_PALABRA));
		scanf("%s",palabras[j]);
	}		
	for(int i=0; i<cuenta; i++){						
		printf("Archivo #%d: %s\n",i+1,datos[i].nombrearchivo);
		for(int j=0; j<cantpalabras;j++){									
			printf("Palabra #%d: %s\n",j+1,palabras[j]);
		}		
		printf("\n");
	}	
    /*  
	pthread_t id[cuenta];	
	
	for(int i=0; i<cuenta; i++){				
		pthread_create(&id[i], NULL, holamundo, (void *)datos);
		//El tercer parametro es la funcion que queremos que se vuelva del hilo
		
	}
	for(int j=0; j<cuenta; j++){				
		pthread_join(id[j],NULL);				
	}
	*/
	
	
	return 0;
}